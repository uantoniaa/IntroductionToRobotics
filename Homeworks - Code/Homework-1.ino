const int INPUT_PIN_RED = A0;   // Input for Red
const int INPUT_PIN_GREEN = A1; // Input for Green
const int INPUT_PIN_BLUE = A2;  // Input for Blue

const int LED_PIN_RED = 9;    // LED Red
const int LED_PIN_GREEN = 10; // LED Green
const int LED_PIN_BLUE = 11;  // LED Blue

const int MIN_INPUT_VALUE = 0;
const int MAX_INPUT_VALUE = 1023;
const int MIN_LED_VALUE = 0;
const int MAX_LED_VALUE = 255;

int inputValueRed = 0;
int inputValueGreen = 0;
int inputValueBlue = 0;

int ledOutputRed = 0;
int ledOutputGreen = 0;
int ledOutputBlue = 0;

void setup() {
  pinMode(INPUT_PIN_RED, INPUT);
  pinMode(LED_PIN_RED, OUTPUT);
  pinMode(INPUT_PIN_GREEN, INPUT);
  pinMode(LED_PIN_GREEN, OUTPUT);
  pinMode(INPUT_PIN_BLUE, INPUT);
  pinMode(LED_PIN_BLUE, OUTPUT);
}

void loop() {
  inputValueRed = analogRead(INPUT_PIN_RED);
  ledOutputRed = mapValue(inputValueRed);

  inputValueGreen = analogRead(INPUT_PIN_GREEN);
  ledOutputGreen = mapValue(inputValueGreen);

  inputValueBlue = analogRead(INPUT_PIN_BLUE);
  ledOutputBlue = mapValue(inputValueBlue);

  analogWrite(LED_PIN_RED, ledOutputRed);
  analogWrite(LED_PIN_GREEN, ledOutputGreen);
  analogWrite(LED_PIN_BLUE, ledOutputBlue);
}

int mapValue(int inputValue) {
  return map(inputValue, MIN_INPUT_VALUE, MAX_INPUT_VALUE, MIN_LED_VALUE, MAX_LED_VALUE);
}
