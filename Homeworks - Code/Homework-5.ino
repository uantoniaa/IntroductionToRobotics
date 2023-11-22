# I have a problem in the code (probably related to memory): I can't scrolling through all the menus,
# I mean at some point, after a few scrolls through the menu, it returns to the main menu. Another problem is that 
# I haven't implemented the requirement for the LED to light up when it gets dark and to turn off when there is light.
# Also, there may be a little extra code :)
#include <EEPROM.h>

// Pin Definitions
const int pinRed = 7;
const int pinGreen = 6;
const int pinBlue = 5;
const int echoPin = 10;
const int trigPin = 9;
const int ldrPin = A0;

// Sensor Settings
int sampleInterval = 5; // Default to 5 seconds
int ultrasonicThreshold = 100;
int ldrThreshold = 500; 

// LED Mode
bool ledAutomatic = false;

int ldrSampleInterval = 2;
int ultrasonicSampleInterval = 2;

// EEPROM Addresses
const int addrSampleInterval = 0;
const int addrUltrasonicThreshold = sizeof(int);
const int addrLdrThreshold = sizeof(int) * 2;
// EEPROM Addresses for Manual RGB Values and LED Mode
const int addrLedAutomatic = sizeof(int) * 3;
const int addrManualRedValue = addrLedAutomatic + sizeof(bool);
const int addrManualGreenValue = addrManualRedValue + sizeof(int);
const int addrManualBlueValue = addrManualGreenValue + sizeof(int);

// Manual RGB LED Values
int manualRedValue = 255;
int manualGreenValue = 255;
int manualBlueValue = 255;


// Sensor Data Arrays and Index
const int maxDataPoints = 10; // Increase to 10 data points
uint8_t ultrasonicData[maxDataPoints];
uint8_t ldrData[maxDataPoints];

int dataIndex = 0;

void setup() {
  // Initialize Serial Communication
  Serial.begin(9600);

  // Initialize Pin Modes
  pinMode(pinRed, OUTPUT);
  pinMode(pinGreen, OUTPUT);
  pinMode(pinBlue, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ldrPin, INPUT);

  // Load Settings from EEPROM
  EEPROM.update(addrSampleInterval, sampleInterval);
  EEPROM.update(addrUltrasonicThreshold, ultrasonicThreshold);
  EEPROM.update(addrLdrThreshold, ldrThreshold);
  EEPROM.update(addrLedAutomatic, ledAutomatic);
  EEPROM.update(addrManualRedValue, manualRedValue);
  EEPROM.update(addrManualGreenValue, manualGreenValue);
  EEPROM.update(addrManualBlueValue, manualBlueValue);



  // Validate loaded settings and apply defaults if necessary
  sampleInterval = constrain(sampleInterval, 1, 10); // Ensure it's between 1 and 10 seconds

  // Set initial LED state
  if (ledAutomatic) {
    setRGBColor(0, 0, 0); 
  } else {
    setRGBColor(manualRedValue, manualGreenValue, manualBlueValue); // Set to manual values
  }

  // Initialize Sensor Data Arrays
  memset(ultrasonicData, 0, sizeof(ultrasonicData));
  memset(ldrData, 0, sizeof(ldrData));
  handleSerialMenu();
}
void loop() {
  unsigned long previousMillis = 0;
    // Handle serial commands
    if (Serial.available() > 0) {
        handleSerialMenu();
    }

    // Get the current time
    unsigned long currentMillis = millis();

    // Check if it's time to read the sensors
    if (currentMillis - previousMillis >= sampleInterval * 1000) {
        // Update the last read time
        previousMillis = currentMillis;

        // Read sensors
        int ultrasonicReading = readUltrasonicDistance();
        int ldrReading = readLDRValue();

        // Validate and log sensor readings
        if (isValidUltrasonicReading(ultrasonicReading) && isValidLDRReading(ldrReading)) {
            logSensorData(ultrasonicReading, ldrReading);
        }

        // Update LED based on automatic or manual mode
        if (ledAutomatic) {
            updateLEDForSensorConditions(ultrasonicReading, ldrReading);
        }
    }
    // Handle Ultrasonic Sensor Reading based on its Interval
    static unsigned long previousUltrasonicMillis = 0;
    if (currentMillis - previousUltrasonicMillis >= ultrasonicSampleInterval * 1000) {
        previousUltrasonicMillis = currentMillis;
        int ultrasonicReading = readUltrasonicDistance();

        if (isValidUltrasonicReading(ultrasonicReading)) {
            // Process ultrasonic reading
            logSensorData(ultrasonicReading, -1); // -1 indicates no LDR reading
        }
    }

    // Handle LDR Reading based on its Interval
    static unsigned long previousLdrMillis = 0;
    if (currentMillis - previousLdrMillis >= ldrSampleInterval * 1000) {
        previousLdrMillis = currentMillis;
        int ldrReading = readLDRValue();

        if (isValidLDRReading(ldrReading)) {
            // Process LDR reading
            logSensorData(-1, ldrReading); // -1 indicates no ultrasonic reading
        }
    }

    // Update LED based on automatic or manual mode
    if (ledAutomatic) {
        updateLEDForSensorConditions(readUltrasonicDistance(), readLDRValue());
    }
      int ultrasonicReading = readUltrasonicDistance();
    int ldrReading = readLDRValue();

    // Validate and log sensor readings
    if (isValidUltrasonicReading(ultrasonicReading) && isValidLDRReading(ldrReading)) {
        logSensorData(ultrasonicReading, ldrReading);
    }

    // Update LED based on automatic or manual mode
    updateLEDForSensorConditions(ultrasonicReading, ldrReading);
}


// Function to set the RGB LED color
void setRGBColor(int red, int green, int blue) {
  analogWrite(pinRed, red);
  analogWrite(pinGreen, green);
  analogWrite(pinBlue, blue);
}

void handleSerialMenu() {
  Serial.println("Main Menu:");
  Serial.println("1. Sensor Settings");
  Serial.println("2. Reset Logger Data");
  Serial.println("3. System Status");
  Serial.println("4. RGB LED Control");

  while (!Serial.available()) {
    // Wait for user input
  }

  String choice = Serial.readStringUntil('\n');
  choice.trim(); // Remove any whitespace

  if (choice == "1") {
    handleSensorSettingsMenu();
  } else if (choice == "2") {
    handleResetLoggerData();
  } else if (choice == "3") {
    handleSystemStatus();
  } else if (choice == "4") {
    handleLEDControl();
  } else {
    handleSerialMenu(); // Redisplay this menu
  }
}


void handleSensorSettingsMenu() {
  Serial.println("1. Sensor Settings:");
  Serial.println("    1.1 Sensors Sampling Interval");
  Serial.println("    1.2 Ultrasonic Alert Threshold");
  Serial.println("    1.3 LDR Alert Threshold");
  Serial.println("    1.4 Back");

  while (!Serial.available()) {
    // Wait for user input
  }

  String choice = Serial.readStringUntil('\n');
  choice.trim(); // Remove any whitespace
  Serial.print("Received choice: '"); 
  Serial.print(choice);
  Serial.println("'");
  if (choice == "1") {
    setUltrasonicSamplingInterval();
  } else if (choice == "2") {
    setUltrasonicThreshold();
  } else if (choice == "3") {
    setLdrThreshold();
  } else if (choice == "4") {
    handleSerialMenu(); // Return to the main menu
  } else {
    handleSensorSettingsMenu(); // Redisplay this menu
  }
}

void setUltrasonicSamplingInterval() {
    Serial.println("Enter the new sampling interval between 1 and 10(in seconds):");
    while (!Serial.available()) {
        // Wait for user input
    }
    int interval = Serial.parseInt();
    if (interval > 0) {
        ultrasonicSampleInterval = interval;
        EEPROM.update(addrUltrasonicSampleInterval, ultrasonicSampleInterval);
        Serial.print("Ultrasonic sampling interval set to ");
        Serial.print(ultrasonicSampleInterval);
        Serial.println(" seconds.");
    } else {
        Serial.println("Invalid interval. Please enter a positive value.");
        setUltrasonicSamplingInterval(); // Prompt again
    }
}

void setUltrasonicThreshold() {
  Serial.println("Enter the new ultrasonic sensor threshold:");
  while (!Serial.available()) {
    // Wait for user input
  }
  int threshold = Serial.parseInt();
  if (threshold > 0) {
    ultrasonicThreshold = threshold;
    EEPROM.update(addrUltrasonicThreshold, ultrasonicThreshold);
    Serial.print("Ultrasonic threshold set to ");
    Serial.print(ultrasonicThreshold);
    Serial.println(".");
  } else {
    Serial.println("Invalid threshold. Please enter a positive value.");
    setUltrasonicThreshold(); // Prompt again
  }
}

void setLdrSamplingInterval() {
    Serial.println("Enter the new LDR sampling interval (in seconds):");
    while (!Serial.available()) {
        // Wait for user input
    }
    int interval = Serial.parseInt();
    if (interval > 0) {
        ldrSampleInterval = interval;
        EEPROM.update(addrLdrSampleInterval, ldrSampleInterval);
        Serial.print("LDR sampling interval set to ");
        Serial.print(ldrSampleInterval);
        Serial.println(" seconds.");
    } else {
        Serial.println("Invalid interval. Please enter a positive value.");
        setLdrSamplingInterval(); // Prompt again
    }
}




void handleResetLoggerData() {
  Serial.println("2. Are you sure you want to reset the logger data?");
  Serial.println("    2.1 Yes");
  Serial.println("    2.2 No");
  while (!Serial.available()) {
    // Wait for user input
  }


  String choice = Serial.readStringUntil('\n');
  choice.trim(); // Remove any whitespace
  Serial.print("Received choice: '"); 
  Serial.print(choice);
  Serial.println("'");
  if (choice == "1") {
    resetData();
    Serial.println("Logger data has been reset.");
    handleSerialMenu();
  } else if (choice == "2") {
    Serial.println("Reset cancelled.");
    handleSerialMenu();
  } else {
    Serial.println("Invalid response. Please answer 1 or 2.");
    handleResetLoggerData(); // Ask again
  }


}

void resetData() {
  // Reset the data arrays
  memset(ultrasonicData, 0, sizeof(ultrasonicData));
  memset(ldrData, 0, sizeof(ldrData));
  dataIndex = 0; // Reset the index for new data
}

void handleSystemStatus() {
  Serial.println("3.System Status Menu:");
  Serial.println("    3.1. Current Sensor Readings");
  Serial.println("    3.2. Current Sensor Settings");
  Serial.println("    3.3. Display Logged Data");
  Serial.println("    3.4. Back");

  while (!Serial.available()) {
    // Wait for user input
  }

  String choice = Serial.readStringUntil('\n');
  choice.trim(); // Remove any whitespace
  Serial.print("Received choice: '"); 
  Serial.print(choice);
  Serial.println("'");
  if (choice == "1") {
    displayCurrentSensorReadings();
  } else if (choice == "2") {
    displaySensorSettings();
  } else if (choice == "3") {
    displayLoggedData();
  } else if (choice == "4") {
    handleSerialMenu(); // return to the main menu
  } else {
    handleSystemStatus(); // Re-display this menu
  }
}

void displayCurrentSensorReadings() {
  Serial.println("Press 'x' to stop displaying sensor readings.");

  while (true) {
    // Check for user input to exit
    if (Serial.available() > 0) {
      char ch = Serial.read();
      if (ch == 'x' || ch == 'X') {
        Serial.println("Exiting sensor readings display.");
        handleSerialMenu();
        break; // Exit the loop
      }
    }

    // Reading from Ultrasonic Sensor
    long ultrasonicDistance = measureUltrasonicDistance();
    Serial.print("Ultrasonic Sensor Distance: ");
    Serial.print(ultrasonicDistance);
    Serial.println(" cm");

    // Reading from LDR
    int ldrValue = analogRead(ldrPin);
    Serial.print("LDR Sensor Value: ");
    Serial.println(ldrValue);

    // Wait for the next sampling interval
    delay(sampleInterval * 1000);
  }
}


long measureUltrasonicDistance() {
  // Ensure the trigger pin is set to LOW
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  // Trigger the sensor by setting the trigPin high for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(2);
  digitalWrite(trigPin, LOW);

  // Read the echoPin, returns the sound wave travel time in microseconds
  long duration = pulseIn(echoPin, HIGH);

  // Calculating the distance
  long distanceCm = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
  return distanceCm;
}


void displaySensorSettings() {
  Serial.println("Current Sensor Settings:");

  // Display Ultrasonic Sensor Settings
  Serial.print("Ultrasonic Sensor Sampling Interval: ");
  Serial.print(ultrasonicSampleInterval);
  Serial.println(" seconds");
  Serial.print("Ultrasonic Sensor Threshold: ");
  Serial.println(ultrasonicThreshold);

  // Display LDR Sensor Settings
  Serial.print("LDR Sensor Sampling Interval: ");
  Serial.print(ldrSampleInterval);
  Serial.println(" seconds");
  Serial.print("LDR Sensor Threshold: ");
  Serial.println(ldrThreshold);
}


void displayLoggedData() {
  Serial.println("Logged Data:");
  Serial.println("Ultrasonic Sensor Readings:");
  for (int i = 0; i < maxDataPoints; i++) {
    int index = (dataIndex - i - 1 + maxDataPoints) % maxDataPoints; 
    Serial.print("Reading ");
    Serial.print(index + 1);
    Serial.print(": ");
    Serial.println(ultrasonicData[index]);
  }
  Serial.println("LDR Sensor Readings:");
  for (int i = 0; i < maxDataPoints; i++) {
    int index = (dataIndex - i - 1 + maxDataPoints) % maxDataPoints; 
    Serial.print("Reading ");
    Serial.print(index + 1);
    Serial.print(": ");
    Serial.println(ldrData[index]);
  }
}

void handleLEDControl() {
  Serial.println("4.RGB LED Control Menu:");
  Serial.println("    4.1. Manual Color Control");
  Serial.println("    4.2. Toggle Automatic Mode (Currently " + String(ledAutomatic ? "ON" : "OFF") + ")");
  Serial.println("    4.3. Back");

  while (!Serial.available()) {
    // Wait for user input
  }

  String choice = Serial.readStringUntil('\n');
  choice.trim(); // Remove any whitespace
  Serial.print("Received choice: '"); 
  Serial.print(choice);
  Serial.println("'");
  if (choice == "1") {
    manualColorControl();
  } else if (choice == "2") {
    toggleAutomaticMode();
  } else if (choice == "3") {
    handleSerialMenu();
  } else {
    Serial.println("Invalid choice. Please try again.");
    handleLEDControl(); // Re-display this menu
  }
}

void manualColorControl() {
  Serial.println("Enter RGB values (0-255) in the format R,G,B:");
  while (!Serial.available()) {
    // Wait for user input
  }
  String rgbInput = Serial.readStringUntil('\n');
  rgbInput.trim(); // Remove any whitespace

  int r, g, b;
  if (sscanf(rgbInput.c_str(), "%d,%d,%d", &r, &g, &b) == 3) {
    // Validate RGB values
    r = constrain(r, 0, 255);
    g = constrain(g, 0, 255);
    b = constrain(b, 0, 255);

    // Set manual RGB values and update LED
    manualRedValue = r;
    manualGreenValue = g;
    manualBlueValue = b;
    EEPROM.update(addrManualRedValue, manualRedValue);
    EEPROM.update(addrManualGreenValue, manualGreenValue);
    EEPROM.update(addrManualBlueValue, manualBlueValue);

    if (!ledAutomatic) {
      setRGBColor(manualRedValue, manualGreenValue, manualBlueValue);
    }

    Serial.println("RGB values set to: " + String(r) + ", " + String(g) + ", " + String(b));
  } else {
    Serial.println("Invalid input. Please use the format R,G,B.");
    manualColorControl(); // Prompt again
  }
}

void toggleAutomaticMode() {
    ledAutomatic = !ledAutomatic;
    EEPROM.update(addrLedAutomatic, ledAutomatic);
    Serial.println("Automatic mode " + String(ledAutomatic ? "enabled" : "disabled"));

    // Read current sensor values
    int ultrasonicReading = readUltrasonicDistance();
    int ldrReading = readLDRValue();

    // Update LED based on current sensor conditions
    updateLEDForSensorConditions(ultrasonicReading, ldrReading);
}


void updateLEDForSensorConditions(int ultrasonicReading, int ldrReading) {
    bool isAlert = false;

    if (ultrasonicReading < ultrasonicThreshold) {
        isAlert = true;
        Serial.println("Ultrasonic sensor alert!");
    }

    if (ldrReading > ldrThreshold) {
        isAlert = true;
        Serial.println("LDR sensor alert!");
    }

    if (ledAutomatic) {
        if (isAlert) {
            setRGBColor(255, 0, 0); // Red color for alert
        } else {
            setRGBColor(0, 255, 0); // Green color if no alert
        }
    }
}



long readUltrasonicDistance() {
    // Ensure the trigger pin is set to LOW
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);

    // Trigger the sensor by setting the trigPin high for 10 microseconds
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    // Read the echoPin; measure the sound wave travel time in microseconds
    long duration = pulseIn(echoPin, HIGH);

    // Calculate the distance
    long distanceCm = duration * 0.034 / 2; // Speed of sound wave divided by 2 (to and fro)
    return distanceCm;
}

int readLDRValue() {
    int ldrValue = analogRead(ldrPin); // Read the value from the LDR
    return ldrValue;
}

template <typename T> T eepromRead(int address) {
    T value;
    EEPROM.update(address, value);
    return value;
}

void loadSettings() {
    sampleInterval = eepromRead<unsigned int>(addrSampleInterval);
    ultrasonicThreshold = eepromRead<unsigned int>(addrUltrasonicThreshold);
    ldrThreshold = eepromRead<unsigned int>(addrLdrThreshold);
    ledAutomatic = eepromRead<bool>(addrLedAutomatic);
    manualRedValue = eepromRead<int>(addrManualRedValue);
    manualGreenValue = eepromRead<int>(addrManualGreenValue);
    manualBlueValue = eepromRead<int>(addrManualBlueValue);
}

void logSensorData(int ultrasonicReading, int ldrReading) {
    if (ultrasonicReading != -1) {
        ultrasonicData[dataIndex] = ultrasonicReading;
    }
    if (ldrReading != -1) {
        ldrData[dataIndex] = ldrReading;
    }

    dataIndex = (dataIndex + 1) % maxDataPoints; // Ensure wrapping around
}


bool isValidUltrasonicReading(int reading) {
    return reading > 0 && reading < 500; 
}

String readSerialInput() {
    String input = "";
    while (!Serial.available()) {
        // Wait for input
    }
    input = Serial.readStringUntil('\n');
    input.trim();
    return input;
}


void checkEEPROMSettings() {
    // Ensure settings are within reasonable limits
    sampleInterval = constrain(sampleInterval, 1, 10);
    ultrasonicThreshold = max(1, ultrasonicThreshold); // Example constraints
    ldrThreshold = max(1, ldrThreshold);
}

bool isValidLDRReading(int reading) {
    return reading >= 300 && reading <= 900; 
}

void setLdrThreshold() {
    Serial.println("Enter the new LDR sensor threshold:");
    while (!Serial.available()) {
        // Wait for user input
    }
    int threshold = Serial.parseInt();
    if (threshold > 0) {
        ldrThreshold = threshold;
        EEPROM.update(addrLdrThreshold, ldrThreshold);
        Serial.print("LDR threshold set to ");
        Serial.print(ldrThreshold);
        Serial.println(".");
    } else {
        Serial.println("Invalid threshold. Please enter a positive value.");
        setLdrThreshold(); // Prompt again
    }
}
