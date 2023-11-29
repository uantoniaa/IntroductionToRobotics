#include "LedControl.h" 

unsigned long lastBlinkTime = 0; // Time since the last player LED toggle
const long blinkInterval = 200;  // Interval for player LED blinking
const long BOMB_BLINK_RATE = 50;  // Blink rate of the bomb
bool isPlayerLedOn = false;       // State of the player's LED
bool bombActivated = false;       // State of bomb activation

// Pin Definitions for LED matrix and controls
const int dinPin = 12;   // Data-in pin for LED matrix
const int clockPin = 11; // Clock pin for LED matrix
const int loadPin = 10;  // Load (CS) pin for LED matrix
const int pinX = A0;     // Joystick X-axis
const int pinY = A1;     // Joystick Y-axis
const int pinSW = 2;     // Joystick button (Switch)
const int BOMB = 3;      // Identifier for bomb in the game map

unsigned long lastUpdate = 0;     // Time since the last game update
const long updateInterval = 80;   // Game update interval

// Initialize the LED matrix control
LedControl lc = LedControl(dinPin, clockPin, loadPin, 1);

// Game grid dimensions
const int rows = 8;
const int cols = 8;

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
const unsigned long debounceDelay = 20;  // Delay for debouncing the button


void setup() { 
  Serial.begin(9600); // Initialize serial communication at 9600 bits per second

  // Initialize the LED matrix
  lc.shutdown(0, false); // Turn off power saving mode
  lc.setIntensity(0, matrixBrightness); // Set brightness of the LED matrix
  lc.clearDisplay(0); // Clear the LED matrix display

  // Seed the random number generator for wall generation
  randomSeed(analogRead(A3)); // Use an unconnected analog pin for a random seed

  // Initialize the game map
  generateWalls(); // Generate and place walls on the game map
  placePlayer();   // Place the player on the game map
}

// Define a struct for the bomb
struct Bomb {
  int x, y;           // X and Y coordinates of the bomb
  bool isActive;      // Status of the bomb (active/inactive)
  char direction;     // Direction of the bomb: 'U' (up), 'D' (down), 'L' (left), 'R' (right)
} bomb; // Create an instance of the Bomb struct

void loop() {
  // Store the current time
  unsigned long currentMillis = millis();

  // Check if the time elapsed since the last update is greater than or equal to the update interval
  if (currentMillis - lastUpdate >= updateInterval) {
    movePlayer();        // Call function to handle player movement based on joystick input
    updateDisplay();     // Update the LED matrix display with the current game state
    handleButtonPress(); // Check and handle joystick button presses

    // Update the time of the last game update to the current time
    lastUpdate = currentMillis; 
  }
}


// Function to handle the press of the joystick button
void handleButtonPress() {
  // Check if enough time has passed since the last button press (debouncing)
  if ((millis() - lastDebounceTime) > debounceDelay) {
    buttonReading = digitalRead(pinSW); // Read the current state of the button

    // If the button is pressed (button state is LOW when pressed)
    if (buttonReading == LOW) {
      // Initialize the bomb at the player's position
      bomb.x = playerX;
      bomb.y = playerY;
      bomb.isActive = true; // Set the bomb as active
      gameMap[bomb.x][bomb.y] = BOMB; // Update the game map to include the bomb
    }
  }
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
  // Read the analog values from the joystick
  int xVal = analogRead(pinX); // X-axis
  int yVal = analogRead(pinY); // Y-axis

  // Determine the new position based on joystick input
  int newX = playerX, newY = playerY;

  // Adjust player position based on joystick input
  if (xVal < 400) newX--; // Move left
  else if (xVal > 600) newX++; // Move right

  if (yVal < 400) newY--; // Move up
  else if (yVal > 600) newY++; // Move down

  // Check if the new position is within the game boundaries
  if (newX >= 0 && newX < cols && newY >= 0 && newY < rows) {
    if (gameMap[newX][newY] == 1) {
      // If the new position is a wall, "destroy" the wall
      gameMap[newX][newY] = 0;
    } 
    if (gameMap[newX][newY] == 0) {
      // Move the player to the new position
      gameMap[playerX][playerY] = 0; // Clear the old position
      playerX = newX; // Update player's X-coordinate
      playerY = newY; // Update player's Y-coordinate
      gameMap[playerX][playerY] = 2; // Mark the new position as occupied by the player
    }
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
        // Blink the player LED at regular inte

      }
    }
  }
}

void movePlayer() {
  // Read the analog values from the joystick
  int xVal = analogRead(pinX); // X-axis
  int yVal = analogRead(pinY); // Y-axis

  // Determine the new position based on joystick input
  int newX = playerX, newY = playerY;

  // Adjust player position based on joystick input
  if (xVal < 400) newX--; // Move left
  else if (xVal > 600) newX++; // Move right

  if (yVal < 400) newY--; // Move up
  else if (yVal > 600) newY++; // Move down

  // Check if the new position is within the game boundaries
  if (newX >= 0 && newX < cols && newY >= 0 && newY < rows) {
    if (gameMap[newX][newY] == 1) {
      // If the new position is a wall, "destroy" the wall
      gameMap[newX][newY] = 0;
    } 
    if (gameMap[newX][newY] == 0) {
      // Move the player to the new position
      gameMap[playerX][playerY] = 0; // Clear the old position
      playerX = newX; // Update player's X-coordinate
      playerY = newY; // Update player's Y-coordinate
      gameMap[playerX][playerY] = 2; // Mark the new position as occupied by the player
    }
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
