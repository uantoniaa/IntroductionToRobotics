#include "LedControl.h" 
#include <LiquidCrystal.h>
#include <EEPROM.h>
unsigned long lastBlinkTime = 0; // Time since the last player LED toggle
const long blinkInterval = 200;  // Interval for player LED blinking
const long BOMB_BLINK_RATE = 50;  // Blink rate of the bomb
bool isPlayerLedOn = false;       // State of the player's LED
bool bombActivated = false;       // State of bomb activation
int menuSelection = 0; 

int settingSelected = 0; // 0 for LCD brightness, 1 for Matrix brightness
enum MenuState { MENU_START, MENU_SETTINGS, MENU_ABOUT, MENU_GAME, MENU_END };
MenuState currentMenu;

int lcdBrightness = 100; 
const int ADDR_LCD_BRIGHTNESS = 1; // Alegeți o adresă diferită de ADDR_MATRIX_BRIGHTNESS

// Pin Definitions for LED matrix and controls
const int dinPin = 12;   // Data-in pin for LED matrix
const int clockPin = 11; // Clock pin for LED matrix
const int loadPin = 10;  // Load (CS) pin for LED matrix
const int pinX = A0;     // Joystick X-axis
const int pinY = A1;     // Joystick Y-axis
const int pinSW = 2;     // Joystick button (Switch)
const int BOMB = 3;      // Identifier for bomb in the game map

const byte rs = 9;
const byte en = 8;
const byte d4 = 7;
const byte d5 = 6;
const byte d6 = 5;
const byte d7 = 4; 

unsigned long lastUpdate = 0;     // Time since the last game update
const long updateInterval = 80;   // Game update interval
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Initialize the LED matrix control
LedControl lc = LedControl(dinPin, clockPin, loadPin, 1);

// Game grid dimensions
const int rows = 8;
const int cols = 8;

const int ADDR_MATRIX_BRIGHTNESS = 0;

// Player's initial position
int playerX = 5, playerY = 1;

// Game map: 0 for empty, 1 for wall, 2 for player
int gameMap[rows][cols]; 

// LED matrix brightness level
byte matrixBrightness = 2;

// Variable to track if the matrix needs updating
bool matrixChanged = true;

// Debounce management for joystick button
volatile bool debounceFlags[3] = {false, false, false}; 
bool buttonState = HIGH;              // Initial state of the button (unpressed)
bool buttonReading = HIGH;            // Current reading from the button pin
unsigned long lastDebounceTime = 0;   // Time of the last button state change
volatile bool possiblePress = false;  // Indicates a potential button press
volatile bool possibleLongPress = false; // Indicates a potential long button press
const unsigned long debounceDelay = 50;  // Delay for debouncing the button

// Define a struct for the bomb
struct Bomb {
  int x, y;           // X and Y coordinates of the bomb
  bool isActive;      // Status of the bomb (active/inactive)
  char direction;     // Direction of the bomb: 'U' (up), 'D' (down), 'L' (left), 'R' (right)
} bomb; // Create an instance of the Bomb struct

void setup() { 
  // Initialize serial communication
  Serial.begin(9600);

  // Initialize the LED matrix
  lc.shutdown(0, false);
  lc.clearDisplay(0);
int readY = analogRead(pinY);
Serial.print("Joystick Y: ");
Serial.println(readY);

  // Load settings from EEPROM
  matrixBrightness = EEPROM.read(ADDR_MATRIX_BRIGHTNESS);
  lc.setIntensity(0, matrixBrightness);

  // Initialize the LCD
  lcd.begin(16, 2);

  // Display Intro Message
  lcd.print("Welcome to Game!");
  delay(3000); // Display message for 3 seconds
  lcd.clear();

  // Initialize the game map, player, and bomb settings
  randomSeed(analogRead(A3));


  // Set initial menu state
  currentMenu = MENU_START;
}

void loop() {
  // Store the current time
  unsigned long currentMillis = millis();

  // Check if the time elapsed since the last update is greater than or equal to the update interval
  if (currentMillis - lastUpdate >= updateInterval) {
      switch (currentMenu) {
    case MENU_START:
      showMainMenu();
      break;
    case MENU_SETTINGS:
      showSettingsMenu();
      break;
    case MENU_ABOUT:
      showAboutScreen();
      break;
    case MENU_GAME:
      runGame();
      break;
    case MENU_END:
      showEndScreen();
      break;
  }

    // Update the time of the last game update to the current time
    lastUpdate = currentMillis; 
  }

}


void handleButtonPress() {
  // Read the current state of the button
  buttonReading = digitalRead(pinSW);

  // Check if button state has changed
  if (buttonReading != buttonState) {
    // Reset the last debounce time
    lastDebounceTime = millis();
  }

  // If the button state has been stable for longer than the debounce delay
  if ((millis() - lastDebounceTime) > debounceDelay) {
    // If the button is currently pressed down
    if (buttonReading == LOW) {
      // Check if it's been pressed for more than 2 seconds
      if ((millis() - lastDebounceTime) > 2000 && !bomb.isActive) {
        // Activate bomb mode
        bomb.x = playerX;
        bomb.y = playerY;
        bomb.isActive = true;

        // Store the last direction for bomb throwing logic
        bomb.direction = determineBombDirection();
      }
    } else {
      // Reset the last debounce time when button is released
      lastDebounceTime = millis();
    }
  }

  // Update the last button state
  buttonState = buttonReading;
}

// Function to determine bomb direction based on joystick position
char determineBombDirection() {
  int xVal = analogRead(pinX);
  int yVal = analogRead(pinY);

  if (xVal < 400) return 'L'; 
  if (xVal > 600) return 'R';
  if (yVal < 400) return 'U'; 
  if (yVal > 600) return 'D';

  return 'N'; // Return 'N' for None if the joystick is in neutral position
}

// Function to generate walls on the game map
void generateWalls() {
  // Loop through each cell in the game map
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      // Ensure the player's starting position is not blocked by a wall
      if (i == playerX && j == playerY) {
        gameMap[i][j] = 0; // Set the player's position to be empty
      } else {
        // Randomly place walls on other positions
        gameMap[i][j] = (random(100) < 60) ? 1 : 0; // 60% chance of placing a wall
      }
    }
  }
}

// Function to place the player on the game map
void placePlayer() {
  gameMap[playerX][playerY] = 2; // Set the cell in the game map to indicate the player's presence
}

void movePlayer() {
  int xVal = analogRead(pinX);
  int yVal = analogRead(pinY);

  int newX = playerX, newY = playerY;

  // Record the last movement direction for bomb throwing logic
  char lastDirection = 'N'; // 'N' for None, 'U' for Up, 'D' for Down, 'L' for Left, 'R' for Right

  if (xVal < 400) { newX--; lastDirection = 'L'; } 
  else if (xVal > 600) { newX++; lastDirection = 'R'; }

  if (yVal < 400) { newY--; lastDirection = 'U'; } 
  else if (yVal > 600) { newY++; lastDirection = 'D'; }

  if (newX >= 0 && newX < cols && newY >= 0 && newY < rows && gameMap[newX][newY] != 1) {
    // Move the player to the new position if it's not a wall
    gameMap[playerX][playerY] = 0;
    playerX = newX;
    playerY = newY;
    gameMap[playerX][playerY] = 2;

    // Store the last direction for bomb throwing logic
    bomb.direction = lastDirection;
  }
}


void updateDisplay() {
  // Get the current time
  unsigned long currentMillis = millis();

  // Iterate over each cell of the game map
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      if (gameMap[i][j] == 1) {
        lc.setLed(0, i, j, true); // Display a wall on the LED matrix
      } else if (gameMap[i][j] == 2) {
        // Blink the player LED at regular intervals
        if (currentMillis - lastBlinkTime >= blinkInterval) {
          isPlayerLedOn = !isPlayerLedOn; // Toggle player LED state
          lastBlinkTime = currentMillis; // Update the last blink time
        }
        lc.setLed(0, i, j, isPlayerLedOn); // Set the LED state for the player
      } else {
        lc.setLed(0, i, j, false); // Turn off the LED for empty spaces
      }

      // Bomb blinking logic
      if (bomb.isActive && (currentMillis / BOMB_BLINK_RATE) % 2) {
        // Make the bomb LED blink faster when active
        lc.setLed(0, bomb.x, bomb.y, true);
      } else if (bomb.isActive) {
        lc.setLed(0, bomb.x, bomb.y, false);
      }
    }
  }
}

void moveBomb() {
  // Logic to move the bomb based on its direction
  switch (bomb.direction) {
    case 'U': bomb.y--; break;
    case 'D': bomb.y++; break;
    case 'L': bomb.x--; break;
    case 'R': bomb.x++; break;
  }

  // Check boundaries and deactivate the bomb if it hits a wall
  if (bomb.x < 0 || bomb.x >= cols || bomb.y < 0 || bomb.y >= rows || gameMap[bomb.x][bomb.y] == 1) {
    bomb.isActive = false;
    if (gameMap[bomb.x][bomb.y] == 1) {
      gameMap[bomb.x][bomb.y] = 0; // Destroy the wall if the bomb hits it
    }
  }
}

void showEndScreen() {
  lcd.clear();
  lcd.print("Game Over!");
  delay(2000); // Display end message for 2 seconds
  lcd.clear();

  // Wait for a button press to return to main menu
  while (digitalRead(pinSW) == HIGH) {}
  currentMenu = MENU_START;
}

void showMainMenu() {
  // Read the joystick Y position to navigate the menu
  int readY = analogRead(pinY);

  // If the joystick is moved up
  if (readY < 400) {
    menuSelection--;
    if (menuSelection < 0) {
      menuSelection = 2; // Wrap around to the last menu option
    }
    delay(200); // Debounce delay
  }
  // If the joystick is moved down
  else if (readY > 600) {
    menuSelection++;
    if (menuSelection > 2) {
      menuSelection = 0; // Wrap around to the first menu option
    }
    delay(200); // Debounce delay
  }

  // Display the menu on the LCD
  lcd.clear();
  switch(menuSelection) {
    case 0:
      lcd.print("Menu");
      lcd.setCursor(0, 1); // Move cursor to the second line
      lcd.print("> ");
      lcd.print("Start Game");
      break;
    case 1:
      lcd.print("> ");
      lcd.print("Settings");
      lcd.setCursor(0, 1); // Move cursor to the second line
      lcd.print("< Back");
      break;
    case 2:
      lcd.print("> ");
      lcd.print("About");
      lcd.setCursor(0, 1); // Move cursor to the second line
      lcd.print("< Back");
      break;
  }

  // Debounce the joystick button
  boolean reading = digitalRead(pinSW);
  if (reading != buttonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer
    // than the debounce delay, so take it as the actual current state:
    buttonState = reading;
    // only toggle the LED if the new button state is HIGH
    if (buttonState == LOW) {
      switch(menuSelection) {
        case 0:
          currentMenu = MENU_GAME;
          initializeGame(); // Initialize the game
          break;
        case 1:
          currentMenu = MENU_SETTINGS;
          break;
        case 2:
          currentMenu = MENU_ABOUT;
          break;
      }
    }
  }
}


void showSettingsMenu() {
  int readY = analogRead(pinY);

  // Navigare în sus și în jos prin setări
  if (readY < 400) {
    settingSelected = (settingSelected + 1) % 2;
    delay(200); // Pentru debouncing
  } else if (readY > 600) {
    settingSelected = (settingSelected - 1 + 2) % 2;
    delay(200); // Pentru debouncing
  }

  // Afișarea setărilor curente pe LCD
  lcd.clear();
  lcd.print(settingSelected == 0 ? "LCD Brightness" : "Matrix Bright");

  lcd.print(settingSelected == 0 ? lcdBrightness : matrixBrightness);

  // Ajustarea setărilor cu butonul joystick-ului
  int readX = analogRead(pinX);
  if (readX < 400 || readX > 600) {
    adjustSetting(settingSelected, readX > 600);
    delay(200); // Pentru debouncing
  }

  // Întoarcere la meniul principal
  if (digitalRead(pinSW) == LOW) {
    delay(200); // Debouncing pentru buton
    currentMenu = MENU_START;
  }
}



void adjustSetting(int setting, bool increase) {
  if (setting == 0) { // LCD Brightness
    lcdBrightness = increase ? min(255, lcdBrightness + 1) : max(0, lcdBrightness - 1);
    // Code to actually set LCD brightness
    EEPROM.update(ADDR_LCD_BRIGHTNESS, lcdBrightness);
  } else if (setting == 1) { // Matrix Brightness
    matrixBrightness = increase ? min(15, matrixBrightness + 1) : max(0, matrixBrightness - 1);
    lc.setIntensity(0, matrixBrightness);
    EEPROM.update(ADDR_MATRIX_BRIGHTNESS, matrixBrightness);
  }
}

void showAboutScreen() {
  const char* aboutText = "Game by Antonia Ungureanu - github.com/uantoniaa/IntroductionToRobotics";
  int textLength = strlen(aboutText);
  int startPosition = 0;
  int displayWidth = 16; // Presupunem că este un ecran LCD 16x2

  unsigned long lastScroll = 0;
  const unsigned long scrollInterval = 300; // Milisecunde între derulări

  while (buttonPressed()) {
    if (millis() - lastScroll >= scrollInterval) {
      lcd.clear();
      for (int i = 0; i < displayWidth; i++) {
        // Verifică dacă suntem la sfârșitul textului
        if (startPosition + i < textLength) {
          lcd.setCursor(i, 0); // Setează cursorul pe prima linie
          lcd.write(aboutText[startPosition + i]);
        }
      }

      startPosition++;
      if (startPosition >= textLength) {
        startPosition = 0; // Resetează la începutul textului
        delay(1500); // Pauză la sfârșitul textului
      }

      lastScroll = millis();
    }
  }
  lcd.clear();
}

bool buttonPressed() {
  return digitalRead(pinSW) == LOW;
}



void runGame() {
  // Setează starea inițială a jocului
  initializeGame();

  while (currentMenu == MENU_GAME) {
    unsigned long currentMillis = millis();

    // Verifică dacă a trecut intervalul pentru actualizarea jocului
    if (currentMillis - lastUpdate >= updateInterval) {
      movePlayer();         
      handleButtonPress(); 
      updateBombState();    // Actualizează starea bombei, dacă este activă
      updateDisplay();      // Actualizează afișajul LED matrix
      
      // Verifică condițiile de sfârșit al jocului
      if (checkGameOver()) {
        currentMenu = MENU_END;
        break;
      }

      lastUpdate = currentMillis;
    }

    delay(10);
  }
}

void initializeGame() {
  generateWalls();
  placePlayer();
  bomb.isActive = false;
  bomb.direction = 'N';
}

bool checkGameOver() {
  return false; 
}

void updateBombState() {
  if (bomb.isActive) {
    moveBomb();
    
  }
}
