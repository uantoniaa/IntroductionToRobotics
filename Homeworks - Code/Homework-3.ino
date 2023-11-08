// Define joystick and button pins
const int pinSW = 2; // Button pin
const int pinX = A0; // Joystick X-axis analog pin
const int pinY = A1; // Joystick Y-axis analog pin

// Initialize joystick values
int xValue = 0; // To store the X-axis reading
int yValue = 0; // To store the Y-axis reading

// Define pins for the 7-segment display segments
const int segmentPins[] = {12, 10, 9, 8, 7, 6, 5, 4};
// Calculate the number of segments
const int segSize = sizeof(segmentPins) / sizeof(segmentPins[0]);
// Initialize the segment states (on/off)
int state[segSize] = {0};
int blinkState[segSize] = {0}; // For blinking state tracking
int position = 7; // Default position on the 7-segment display

// Define thresholds for joystick readings
const int readingThresholds[5] = {350, 650, 450, 550, 500};

// Define timing for blinking and button debouncing
const int blinkInterval = 150; // Blink interval in milliseconds
unsigned long lastBlinkToggleTime = 0; // Time since the last blink toggle

// Define debouncing parameters
const unsigned long debounceDelay = 20; // Debounce delay in milliseconds
const unsigned long pushDuration = 2000; // Duration to consider as a push (in milliseconds)
const unsigned long longPushDuration = 4000; // Duration to consider as a long push (in milliseconds)
volatile bool debounceFlags[3] = {false, false, false}; // Flags to manage debounce states
bool buttonState = HIGH; // Current state of the button (unpressed)
bool buttonReading = HIGH; // Reading from the button pin
unsigned long lastDebounceTime = 0; // Time of the last debounce check
volatile bool possiblePress = false; // Flag for a possible button press
volatile bool possibleLongPress = false; // Flag for a possible long button press
volatile bool shouldStartCounter = false; // Flag to start debounce timing
int verticalMoving = 0; // Flag for vertical movement
int horizontalMoving = 0; // Flag for horizontal movement
int center = 1; // Flag to determine if the joystick is centered
unsigned long currentBlinkToggleTime = 0; // Current time for blink toggling
int blinkPosition = 7; // Current position of the blinking LED

// Set up the Arduino pins and serial communication
void setup() {
  pinMode(pinSW, INPUT_PULLUP); // Set button pin with an internal pull-up resistor
  for (int i = 0; i < segSize; i++) {
    pinMode(segmentPins[i], OUTPUT); // Set segment pins as outputs
  }
  attachInterrupt(digitalPinToInterrupt(pinSW), handleInterrupt, FALLING); // Attach an interrupt to the button press (FALLING edge)
  Serial.begin(9600); // Start serial communication at 9600 baud rate
}

// Main loop to process input and update the display
void loop() {
  processJoystickInput();
  handleButtonPress();
  handleMovement();
  updateDisplay();
  processBlinking(position);
}

// Interrupt service routine to manage button press debouncing
void handleInterrupt() {
  debounceFlags[0] = true;
  debounceFlags[1] = true;
  debounceFlags[2] = false; 
  shouldStartCounter = true;
}

// Function to update the display with the current state
void updateDisplay() {
  writeState(); // Write the current state to the display
}

// Function to handle button press logic with debouncing
void handleButtonPress() {
  // Check the first debounce state
  if (debounceFlags[0]) {
    // If enough time has passed since the last debounce check
    if ((millis() - lastDebounceTime) > debounceDelay) {
      buttonReading = digitalRead(pinSW); // Read the button state
      debounceFlags[0] = false; // Reset the debounce flag
      // If the button is pressed
      if (buttonReading == LOW) {
        state[position] = !state[position]; // Toggle the LED state
        writeState(); // Update the display
      }
    }
  }

  // Check for a standard button press (more than pushDuration but less than longPushDuration)
  if (debounceFlags[1] && !debounceFlags[2]) {
    // If the pushDuration has passed
    if ((millis() - lastDebounceTime) > pushDuration) {
      buttonReading = digitalRead(pinSW); // Read the button state
      debounceFlags[1] = false; // Reset the flag to prevent block from running again
      // If the button is still pressed
      if (buttonReading == LOW) {
        upLed(); // Update the LED
        debounceFlags[2] = true; // Now wait for a longer press
        lastDebounceTime = millis(); // Reset the debounce timer for the long press detection
      }
    }
  }
  // Check for a long button press
  if (debounceFlags[2]) {
    // If the longPushDuration has passed
    if ((millis() - lastDebounceTime) > longPushDuration) {
      buttonReading = digitalRead(pinSW); // Read the button state
      debounceFlags[2] = false; // Reset the flag to prevent block from running again
      // If the button is still pressed
      if (buttonReading == LOW) {
        resetAll(); // Reset the display to initial state
      }
    }
  }
}


// Function to update the LED states on the 7-segment display
void upLed()
{
  for(int i = 0; i < segSize; i++) // Loop through each segment
  {
    digitalWrite(segmentPins[i], state[i]); // Set the segment LED to the state defined in the state array
  }
}
void processJoystickInput() {
  // Define local variables for joystick thresholds
  int readingMinThreshold = readingThresholds[0];
  int readingMaxThreshold = readingThresholds[1];
  int resetMinThreshold = readingThresholds[2];
  int resetMaxThreshold = readingThresholds[3];
  int centerReferenceValue = readingThresholds[4];

 // Check if a press or a long press is possible
  if(possiblePress || possibleLongPress) {
    // If we should start counting for a debounce
    if (shouldStartCounter) {
      lastDebounceTime = millis(); // Record the time when we started
      shouldStartCounter = false;
    }

  // Check if enough time has passed for a debounce and a normal press
    if ((millis() - lastDebounceTime) > debounceDelay && possiblePress) {
      buttonReading = digitalRead(pinSW); // Read the state of the button

      // If the button state has changed
      if (buttonReading != buttonState) {
        // If the button is pressed
        if (buttonReading == LOW) {
          state[position] = !state[position]; // Toggle the LED state at the current position
        }
      }
      possiblePress = false; // Reset the possible press flag
    }
    
      // Check if a long press has occurred
  if (millis() - lastDebounceTime > pushDuration) {
      buttonReading = digitalRead(pinSW); // Read the state of the button

      // If the button state has changed
      if (buttonReading != buttonState) {
        // If the button is pressed
        if (buttonReading == LOW) {
          Serial.println(6666); // Send a value to the serial monitor for debugging
          for (int i = 0; i < segSize; i++) {
            state[i] = 0; // Reset the state of all LEDs
            blinkState[i] = state[i]; // Sync the blink state with the new state
          }
          position = 7; // Reset the position to the default
        }
      }
      possibleLongPress = false; // Reset the possible long press flag
    }
  }

  // Read the values from the joystick axes
  xValue = analogRead(pinX);
  yValue = analogRead(pinY);

  // Process the X and Y values to determine the direction of joystick movement
  // and if it has returned to the center

  if (xValue > readingMaxThreshold && abs(xValue-centerReferenceValue) > abs(yValue-centerReferenceValue) && center == 1) {
    verticalMoving = 1;
    center = 0;
  }
  if (xValue < readingMinThreshold && abs(xValue-centerReferenceValue) > abs(yValue-centerReferenceValue) && center == 1) {
    verticalMoving = -1;
    center = 0;
  }
  if (yValue > readingMaxThreshold && abs(yValue-centerReferenceValue) > abs(xValue-centerReferenceValue) && center == 1) {
    horizontalMoving = 1;
    center = 0;
  }
  if (yValue < readingMinThreshold && abs(yValue-centerReferenceValue) > abs(xValue-centerReferenceValue) && center == 1) {
    horizontalMoving = -1;
    center = 0;
  }


  if (xValue >= readingMinThreshold && xValue <= readingMaxThreshold && yValue >= readingMinThreshold && yValue <= readingMaxThreshold) {
    center = 1;
  }
  return;
}

// Function to handle movement based on joystick input
void handleMovement() {
  // Logic for determining the new position based on movement direction
  if (position == 0) {
    if (horizontalMoving == 1) {
      position = 1;
    } else if (horizontalMoving == -1) {
      position = 5;
    } else if (verticalMoving == -1) {
      position = 6;
    }
  } else if (position == 1) {
    if (horizontalMoving == -1) {
      position = 5;
    } else if (verticalMoving == 1) {
      position = 0;
    } else if (verticalMoving == -1) {
      position = 6;
    }
  } else if (position == 2) {
    if (horizontalMoving == 1) {
      position = 7;
    } else if (horizontalMoving == -1) {
      position = 4;
    } else if (verticalMoving == 1) {
      position = 6;
    } else if (verticalMoving == -1) {
      position = 3;
    }
  } else if (position == 3) {
    if (horizontalMoving == 1) {
      position = 2;
    } else if (horizontalMoving == -1) {
      position = 4;
    } else if (verticalMoving == 1) {
      position = 6;
    }
  } else if (position == 4) {
    if (horizontalMoving == 1) {
      position = 2;
    } else if (verticalMoving == 1) {
      position = 6;
    } else if (verticalMoving == -1) {
      position = 3;
    }
  } else if (position == 5) {
    if (horizontalMoving == 1) {
      position = 1;
    } else if (verticalMoving == 1) {
      position = 0;
    } else if (verticalMoving == -1) {
      position = 6;
    }
  } else if (position == 6) {
    if (verticalMoving == 1) {
      position = 0;
    } else if (verticalMoving == -1) {
      position = 3;
    }
  } else if (position == 7) {
    if (horizontalMoving == -1) {
      position = 2;
    }
  }

  // If a movement has been detected, update the blink position and reset the movement indicators
  if (position != blinkPosition) {
    lastBlinkToggleTime = millis();
    blinkState[blinkPosition] = state[blinkPosition];
    blinkPosition = position; // Update blinkPosition to the new position
    verticalMoving = 0;
    horizontalMoving = 0;
  }
}


// Function to process the blinking of the LED at the current position
void processBlinking(int newBlinkPosition) {
  currentBlinkToggleTime = millis(); // Get the current time
  // If the interval for blinking has passed
  if (currentBlinkToggleTime - lastBlinkToggleTime > blinkInterval) {
    lastBlinkToggleTime = currentBlinkToggleTime; // Update the last blink toggle time
    blinkState[newBlinkPosition] = !blinkState[newBlinkPosition]; // Toggle the blink state
  }
  blinkPosition = newBlinkPosition; // Update the blink position
  for (int i = 0; i < segSize; i++) {
    digitalWrite(segmentPins[i], blinkState[i]); // Update the display with the new blink state
  }
  return;
}

// Function to write the current state to the 7-segment display
void writeState() {
  for (int i = 0; i < segSize; i++) {
    digitalWrite(segmentPins[i], state[i]); // Write each segment's state to the corresponding pin
  }
}


// Function to reset all the states and the display
void resetAll() {
  for (int i = 0; i < segSize; i++) {
    state[i] = 0; // Reset the state of each segment to 'off'
    blinkState[i] = 0; // Reset the blink state of each segment
  }
  position = 7; // Reset the current position to the default
  processBlinking(position); // Process the blinking for the reset position
  for (int i = 0; i < segSize; i++) {
    digitalWrite(segmentPins[i], state[i]); // Update the display with the new states
  }
}
