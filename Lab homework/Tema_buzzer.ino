const int pushButton = 2;
const int activeBuzzerPin = 11;
const int speakerPin = A0;

int speakerValue = 0;
int buttonState = 0;
int ok = 0;
int buzzerTone = 2500;
int buzzerValue = 0;

unsigned long previousMillis = 0;

void setup() {
Serial.begin(9600);
pinMode(pushButton, INPUT_PULLUP);
pinMode(speakerPin, INPUT);
pinMode(activeBuzzerPin, OUTPUT);

}

void loop() {
  unsigned long currentMillis = millis();
  speakerValue = analogRead(speakerPin);
    if (speakerValue != 0)
      Serial.println(speakerValue);
      
     if (speakerValue > 30) {
      Serial.println("Knock");
      ok = 1;
    }
    if ((currentMillis - previousMillis >= 5000) && ok == 1) {
          previousMillis = currentMillis;
          tone (activeBuzzerPin, buzzerTone);
  }
  buttonState = digitalRead(pushButton);
  if(buttonState){
    noTone(activeBuzzerPin); 
    buttonState = 0;
  }

}
