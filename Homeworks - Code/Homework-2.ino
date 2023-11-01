// Define the LED pin numbers.
const int LEDs[] = {9, 10, 12};

// Define the button pin numbers.
const int buttons[] = {2, 4, 5};

// Define the operation LED pin number.
const int operationLED = 7;

// Define the buzzer pin number.
const int buzzerPin = 6;

// Current position (floor) of the elevator.
int currentFloor = 0;

// Target floor that the elevator should reach.
int targetFloor = -1;

// The next floor the elevator will pass through or stop at.
int nextFloor = -1;

// Boolean to determine if the elevator is moving.
bool isMoving = false;

// Store when the LED fade started.
unsigned long fadeStartTime;

// Store the last time a button was pressed.
unsigned long lastButtonPressTime;

// Store the last time the operation LED blinked.
unsigned long lastBlinkTime;

// Time (in ms) to wait for button debouncing.
const long debounceTime = 50;

// Time interval (in ms) for the operation LED to blink.
const long blinkInterval = 300;

// Duration (in ms) of the LED fade effect.
const int fadeDuration = 1200;

void setup() {
  // Initialize the LED and button pins.
  for (int i = 0; i < 3; i++) {
    pinMode(LEDs[i], OUTPUT);  // Set LED pins as output.
    // Turn on the LED for the current floor and turn off the others.
    analogWrite(LEDs[i], (i == currentFloor) ? 255 : 0);
    pinMode(buttons[i], INPUT_PULLUP); // Set button pins as input with pull-up resistor.
  }
  pinMode(operationLED, OUTPUT); // Set operation LED pin as output.
  digitalWrite(operationLED, HIGH); // Turn on the operation LED.
  pinMode(buzzerPin, OUTPUT); // Set buzzer pin as output.
}

void loop() {
  // Check if any floor button is pressed.
// Check if any floor button is pressed.
for (int i = 0; i < 3; i++) {
  // If a button is pressed and it's been longer than debounce time since the last button press.
  if (digitalRead(buttons[i]) == LOW && millis() - lastButtonPressTime > debounceTime) {
    lastButtonPressTime = millis(); // Update the last button press time.

    // If the pressed floor is the same as the current floor or the target floor, ignore the press.
    if (i == currentFloor || i == targetFloor) {
      continue; // Skip to the next iteration of the loop without executing the code below.
    }

    // If the elevator isn't moving, set the target floor.
    if (!isMoving) {
      targetFloor = i; // Set the target floor.

      // Determine the direction of movement.
      if (currentFloor < targetFloor) {
        nextFloor = currentFloor + 1;
      } else if (currentFloor > targetFloor) {
        nextFloor = currentFloor - 1;
      }

      isMoving = true; // Elevator starts moving.
      tone(buzzerPin, 400); // Play a tone indicating the elevator started moving.

      fadeStartTime = millis(); // Start the LED fade effect.
    }
  }
}

  // If the elevator is moving, manage the LED fading effect.
  if (isMoving) {
    unsigned long fadeElapsedTime = millis() - fadeStartTime; // Calculate elapsed time since fade started.

    // Calculate LED brightness levels for current and next floors.
    int currentBrightness = 255 - map(fadeElapsedTime, 0, fadeDuration, 0, 255);
    int nextBrightness = map(fadeElapsedTime, 0, fadeDuration, 0, 255);

    // Update the brightness of LEDs for the current and next floors.
    analogWrite(LEDs[currentFloor], currentBrightness);
    analogWrite(LEDs[nextFloor], nextBrightness);

    // If the fade duration has passed, process floor changes.
    if (fadeElapsedTime >= fadeDuration) {
      noTone(buzzerPin); // Stop the moving tone.
      currentFloor = nextFloor; // Update the current floor.
      if (nextFloor == targetFloor) {
        isMoving = false; // Stop the elevator if the target floor is reached.
        digitalWrite(operationLED, HIGH); // Turn on the operation LED.
        tone(buzzerPin, 1000, 200); // Play a tone indicating the elevator has stopped.
      } else {
        // Determine the next floor if not at the target yet.
        if (currentFloor < targetFloor) {
          nextFloor = currentFloor + 1;
        } else {
          nextFloor = currentFloor - 1;
        }
        fadeStartTime = millis(); // Restart the LED fade effect for the new floor.
      }
    }
  }

  // Blink the operation LED when the elevator is moving.
  if (isMoving && millis() - lastBlinkTime >= blinkInterval) {
    digitalWrite(operationLED, !digitalRead(operationLED)); // Toggle the operation LED.
    lastBlinkTime = millis(); // Update the last blink time.
  }
}
