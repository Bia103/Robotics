const int redPin = 11;
const int greenPin = 10;
const int bluePin = 9;

const int potPinRed = A0;
const int potPinGreen = A1;
const int potPinBlue = A2;

int ledValue = 0;
int potValueRed, potValueGreen, potValueBlue;
void setup() {
 pinMode(potPinRed, INPUT);
 pinMode(potPinGreen, INPUT);
 pinMode(potPinBlue, INPUT);
 
 pinMode(redPin, OUTPUT);
 pinMode(greenPin, OUTPUT);
 pinMode(bluePin, OUTPUT);
 Serial.begin(9600);
}

void loop() {
potValueRed = analogRead(potPinRed);
ledValue = map(potPinRed, 0, 1023, 0, 255);
analogWrite(redPin, potValueRed);
potValueGreen = analogRead(potPinGreen);
ledValue = map(potPinGreen, 0, 1023, 0, 255);
analogWrite(greenPin, potValueGreen);
potValueBlue = analogRead(potPinBlue);
ledValue = map(potPinBlue, 0, 1023, 0, 255);
analogWrite(bluePin, potValueBlue);
}
