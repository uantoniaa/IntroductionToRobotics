// Define connections to the shift register
const int latchPin = 11; // Connects to STCP (latch pin) on the shift register
const int clockPin = 10; // Connects to SHCP (clock pin) on the shift register
const int dataPin = 12; // Connects to DS (data pin) on the shift register
unsigned long lastResetTime = 0;
const unsigned long debounceDelay = 50;
// Define connections to the digit control pins for a 4-digit display
const int segD1 = 4;
const int segD2 = 5;
const int segD3 = 6;
const int segD4 = 7;
// Define pin connections for buttons
const int startPauseButton = 2;  // Start/Pause button
const int resetButton = 3;       // Reset button
const int lapButton = 9;         // Lap button

// Variables for the stopwatch
volatile bool running = false;
volatile unsigned long stopwatchTime = 0;
unsigned long lastUpdateTime = 0;
const unsigned long updateInterval = 100; // Update every 100ms (0.1s)

// Lap time storage
unsigned long lapTimes[4] = {0};
int currentLap = 0;
int displayedLap = 0;
bool viewingLaps = false;

// Define the number of unique encodings (0-9, A-F for hexadecimal)
const int encodingsNumber = 16;
// Store the digits in an array for easy access
int displayDigits[] = {segD1, segD2, segD3, segD4};
const int displayCount = sizeof(displayDigits) / sizeof(displayDigits[0]); // Number of digits in the display

byte byteEncodings[encodingsNumber] = {
//A B C D E F G DP 
  B11111100, // 0 
  B01100000, // 1
  B11011010, // 2
  B11110010, // 3
  B01100110, // 4
  B10110110, // 5
  B10111110, // 6
  B11100000, // 7
  B11111110, // 8
  B11110110, // 9
  B11101110, // A
  B00111110, // b
  B10011100, // C
  B01111010, // d
  B10011110, // E
  B10001110  // F
};
// Variables for controlling the display update timing

unsigned long lastIncrement = 0;
unsigned long delayCount = 50; // Delay between updates (milliseconds)
unsigned long number = 0; // The number being displayed
unsigned long lastLapPress = 0;
const unsigned long lapDebounceDelay = 50;
void setup() {
  // Initialize shift register pins
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  // Setup buttons with pull-up resistors and attach interrupts
  setupButton(startPauseButton, startPausePressed);
  setupButton(resetButton, resetPressed);
  pinMode(lapButton, INPUT_PULLUP); // Lap button without interrupt

  // Initialize display digit control pins
  for (int i = 0; i < displayCount; i++) {
    pinMode(displayDigits[i], OUTPUT);
    digitalWrite(displayDigits[i], LOW);
  }

  Serial.begin(9600);
}

void loop() {
  updateStopwatch();
  manageDisplay();
  checkLapButton();
}
// Function to setup a button with an interrupt
void setupButton(int buttonPin, void (*ISR)()) {
  pinMode(buttonPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(buttonPin), ISR, FALLING);
}

// Update the stopwatch time
void updateStopwatch() {
  if (running && millis() - lastUpdateTime >= updateInterval) {
    stopwatchTime += millis() - lastUpdateTime;
    lastUpdateTime = millis();
  }
}

// Manage the display of stopwatch or lap times
void manageDisplay() {
  if (viewingLaps) {
    writeNumber(lapTimes[displayedLap] / 100); // Display lap time
  } else {
    writeNumber(stopwatchTime / 100); // Display stopwatch time
  }
}

// Check if the lap button has been pressed
void checkLapButton() {
  if (digitalRead(lapButton) == LOW && millis() - lastLapPress > lapDebounceDelay) {
    lastLapPress = millis();
    lapPressed();
  }
}



void writeReg(int digit) {
    // Prepare to shift data by setting the latch pin low
    digitalWrite(latchPin, LOW);
    // Shift out the byte representing the current digit to the shift register
    shiftOut(dataPin, clockPin, MSBFIRST, digit);
    // Latch the data onto the output pins by setting the latch pin high
    digitalWrite(latchPin, HIGH);
}


void activateDisplay(int displayNumber) {
  // Turn off all digit control pins to avoid ghosting
  for (int i = 0; i < displayCount; i++) {
    digitalWrite(displayDigits[i], HIGH);
  }
  // Turn on the current digit control pin
  digitalWrite(displayDigits[displayNumber], LOW);
}

void writeNumber(int number) {
  int currentNumber = number;
  int displayDigit = 3; // Start with the least significant digit

  for (int i = 0; i < displayCount; i++) {
    int lastDigit = currentNumber % 10;
    // Check if we need to add the decimal point for the first digit
    byte digitEncoding = byteEncodings[lastDigit];
    if (i == 1) { // Add decimal point to the second digit from right
      digitEncoding |= B00000001; // Turn on the decimal point (DP)
    }

    activateDisplay(displayDigit);
    writeReg(digitEncoding);
    delay(5); // A small delay for multiplexing visibility

    displayDigit--;
    if (i < displayCount - 1 || currentNumber > 0) {
      currentNumber /= 10;
    }
    writeReg(B00000000); // Clear the register to avoid ghosting
  }
}


// Button interrupt service routines
void startPausePressed() {
  running = !running;
  if (running) {
    lastUpdateTime = millis();
  }
}

void lapPressed() {
  if (running) {
    // Save the current lap time
    lapTimes[currentLap] = stopwatchTime;
    currentLap = (currentLap + 1) % 4; // Move to the next lap, wrap around after 4 laps
  } else {
    // If not running, cycle through saved laps
    viewingLaps = true; // Enable lap viewing mode
    displayedLap++;
    if(displayedLap >= 4) displayedLap = 0; // Reset to first lap after the fourth
  }
}





void resetPressed() {
  unsigned long currentTime = millis();
  if (currentTime - lastResetTime > debounceDelay) {
    if (!running) {
      // Reset the stopwatch time and display
      stopwatchTime = 0;
      lastUpdateTime = millis(); // Reset last update time to prevent time jump
      currentLap = 0; // Reset current lap index
      viewingLaps = false; // Exit lap viewing mode

      // Reset the lap times
      for (int i = 0; i < 4; i++) {
        lapTimes[i] = 0;
      }

      // Update the display to show "000.0"
      writeNumber(0);

      // Debugging messages
      Serial.println("Reset button pressed");
      Serial.print("stopwatchTime after reset: ");
      Serial.println(stopwatchTime);
    }
    lastResetTime = currentTime;
  }
}
