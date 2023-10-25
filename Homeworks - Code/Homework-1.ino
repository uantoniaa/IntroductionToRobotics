// Define the analog input pins for each color.
const int INPUT_PIN_RED = A0;    // Input pin for the Red color
const int INPUT_PIN_GREEN = A1;  // Input pin for the Green color
const int INPUT_PIN_BLUE = A2;   // Input pin for the Blue color

// Define the pins connected to each LED for red, green, and blue.
const int LED_PIN_RED = 9;       // Output pin for the Red LED
const int LED_PIN_GREEN = 10;    // Output pin for the Green LED
const int LED_PIN_BLUE = 11;     // Output pin for the Blue LED

// Define the range of possible values from the analog input (0-1023).
const int MIN_INPUT_VALUE = 0;
const int MAX_INPUT_VALUE = 1023;
// Define the range for analogWrite function to control LED brightness (0-255).
const int MIN_LED_VALUE = 0;
const int MAX_LED_VALUE = 255;

// Variables to hold the values read from the analog inputs for each color.
int inputValueRed = 0;
int inputValueGreen = 0;
int inputValueBlue = 0;

// Variables to hold the mapped values to be sent to the LEDs for each color.
int ledOutputRed = 0;
int ledOutputGreen = 0;
int ledOutputBlue = 0;

// The setup function runs once when the Arduino starts.
void setup() {
  // Configure the analog pins as INPUTs.
  pinMode(INPUT_PIN_RED, INPUT);
  pinMode(INPUT_PIN_GREEN, INPUT);
  pinMode(INPUT_PIN_BLUE, INPUT);
  
  // Configure the LED pins as OUTPUTs.
  pinMode(LED_PIN_RED, OUTPUT);
  pinMode(LED_PIN_GREEN, OUTPUT);
  pinMode(LED_PIN_BLUE, OUTPUT);
}

// The loop function runs continuously after the setup function.
void loop() {
  // Read the analog values for each color.
  inputValueRed = analogRead(INPUT_PIN_RED);
  inputValueGreen = analogRead(INPUT_PIN_GREEN);
  inputValueBlue = analogRead(INPUT_PIN_BLUE);

  // Map the analog values (0-1023) to values suitable for LED brightness (0-255).
  ledOutputRed = mapValue(inputValueRed);
  ledOutputGreen = mapValue(inputValueGreen);
  ledOutputBlue = mapValue(inputValueBlue);

  // Control the brightness of each LED based on the mapped values.
  analogWrite(LED_PIN_RED, ledOutputRed);
  analogWrite(LED_PIN_GREEN, ledOutputGreen);
  analogWrite(LED_PIN_BLUE, ledOutputBlue);
}

// Helper function to map the values from the analog range to the LED brightness range.
int mapValue(int inputValue) {
  return map(inputValue, MIN_INPUT_VALUE, MAX_INPUT_VALUE, MIN_LED_VALUE, MAX_LED_VALUE);
}
