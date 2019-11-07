const int redPin = 11;
const int greenPin = 10;
const int bluePin = 9;

const int potPinRed = A0;
const int potPinGreen = A1;
const int potPinBlue = A2;

int red = 0;
int green = 0;
int blue = 0;
int potValueRed, potValueGreen, potValueBlue;
void setup() {
 pinMode(potPinRed, INPUT);
 pinMode(potPinGreen, INPUT);
 pinMode(potPinBlue, INPUT);
 
 pinMode(redPin, OUTPUT);
 pinMode(greenPin, OUTPUT);
 pinMode(bluePin, OUTPUT);

}

void loop() {
red = analogRead(potPinRed);
red = map(red, 0, 1023, 0, 255);
//analogWrite(redPin, potValueRed);
green = analogRead(potPinGreen);
green = map(green, 0, 1023, 0, 255);
//analogWrite(greenPin, potValueGreen);
blue = analogRead(potPinBlue);
blue = map(blue, 0, 1023, 0, 255);
//analogWrite(bluePin, potValueBlue);
setColor(red, green, blue);
}

void setColor(int r,int g,int b)
{
  analogWrite(redPin, r);
  analogWrite(greenPin, g);
  analogWrite(bluePin, b);
}
