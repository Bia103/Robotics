#include <LiquidCrystal.h>
const int RS = 12;
const int enable = 11;
const int d4 = 5;
const int d5 = 4;
const int d6 = 3;
const int d7 = 2;

int ecran = 0;
int startingLevelValue = 0;

const int joyX = A1;
const int joyY = A0;
const int joy = 10;

int ok = 0, ok2 = 0;
int prevX = 3, prevY = 3, posX = 0, posY = 0;
int valX, valY;
boolean movedX = false, movedY = false;

const int V0_PIN = 9;
int i,j,buttonState=0;
int lastMillis = 0;
int millisWaitInterval = 1000;

int highScore = 0;

char incomingByte = 0; // for incoming serial data
String inputString = ""; // a String to hold incoming data
bool stringComplete = false; 

LiquidCrystal lcd(RS, enable, d4, d5, d6, d7);

void setup() {
// set up the LCD's number of columns and rows:
lcd.begin(16, 2);
// Print a message to the LCD.
pinMode(V0_PIN, OUTPUT); // PWN in loc de POTENTIOMETRU
analogWrite(V0_PIN, 90); // PWN in loc de POTENTIOMETRU
Serial.begin(9600);
pinMode(joy, INPUT_PULLUP);

}

void loop() {
   buttonState = digitalRead(joy);
   valX = analogRead(joyX);
   valY = analogRead(joyY);


  if (valY < 400)
  {
    if (movedY == false)
    {
      prevY = posY;
      posY--;
      movedY = true;
    }
  }
  else
  {
    if (valY > 600)
    {
      if (movedY == false)
      {
        prevY = posY;
        posY++;
        movedY = true;
      }
    }
    else movedY = false;
  }
  posX = startingLevelValue;
  if(ok2 == 1)
  {
       if (valX < 400)
  {
    if (movedX == false)
    {
      prevX = posX;
      posX++;
      movedX = true;
    }
  }
  else
  {
    if (valX > 600)
    {
      if (movedX == false)
      {
        prevX = posX;
        if(posX > 0)
        posX--;
        movedX = true;
      }
    }
    else movedX = false;
  }

  }
   if (posY == 3) posY = 0;
   if (posY == -1) posY = 2;
   Serial.println(buttonState);
   
  if( ecran == 0 )
  {
    if(posY == 0)
    {
      lcd.setCursor(0, 0);
      lcd.print(">Start    Setup ");
      lcd.setCursor(0, 1);
      lcd.print(" Highscore      ");
      if(buttonState == 0 && millis() - lastMillis > millisWaitInterval)
        {
          ecran = 1;
          buttonState = 1;
          lastMillis = millis();
        }

    }
    else if(posY == 1)
    {
      lcd.setCursor(0, 0);
      lcd.print(" Start   >Setup ");
      lcd.setCursor(0, 1);
      lcd.print(" Highscore      ");
       if(buttonState == 0 && millis() - lastMillis > millisWaitInterval)
        {
          ecran = 2;
          buttonState = 1;
          lastMillis = millis();
        }
     

    }
    else if(posY == 2)
    {
      lcd.setCursor(0, 0);
      lcd.print(" Start    Setup ");
      lcd.setCursor(0, 1);
      lcd.print(">Highscore      ");
      if(buttonState == 0 && millis() - lastMillis > millisWaitInterval)
        {
          ecran = 3;
          buttonState = 1;
          lastMillis = millis();
        }

    }
  }
  else if(ecran == 1){
    
    if(ok == 0)
    { lcd.setCursor(0, 0);
      lcd.print("Lives: 3 Lvl: ");
      lcd.print( startingLevelValue);
      lcd.setCursor(0, 1);
      lcd.print("Score: ");
      lcd.print( startingLevelValue*3);
      startingLevelValue++;
      delay(5000);
      lcd.setCursor(0, 0);
      lcd.print("Lives: 3 Lvl: ");
      lcd.print( startingLevelValue);
      lcd.setCursor(0, 1);
      lcd.print("Score: ");
      lcd.print( startingLevelValue*3);
      lcd.print("Score    ");
      startingLevelValue++;
      delay(5000);
      lcd.print("Lives: 3 Lvl: ");
       lcd.print( startingLevelValue);
      lcd.setCursor(0, 1);
      lcd.print("Score: ");
      lcd.print( startingLevelValue*3);
      delay(1000);
      ok = 1;
      if(startingLevelValue > highScore)
        highScore = startingLevelValue;
        lcd.print("        ");
  }else{
      lcd.setCursor(0, 0);
      lcd.print("Congratulations, youfinished the game.");
      
      lcd.setCursor(0, 1);
      lcd.print("Press button to exit");
       if(buttonState == 0 && (millis() - lastMillis > millisWaitInterval))
        {
          ok = 0;
          ecran = 0;
          buttonState = 1;
          lastMillis = millis();
        }
     }
     
  }
 else if(ecran == 2)
 {
      lcd.setCursor(0, 0);
      lcd.print("Level: ");
      lcd.print(startingLevelValue);
      startingLevelValue = posX;
      lcd.print("        ");
      lcd.setCursor(0, 1);
      lcd.print("Player: Unknown");
      ok2 = 1;
     // incomingByte = Serial.read();
     // incomingByte = (char)Serial.read();
     // inputString += incomingByte;

     // lcd.print(incomingByte);
      if(buttonState == 0 && (millis() - lastMillis > millisWaitInterval))
        {
          ok2 = 0;
          ecran = 0;
          buttonState = 1;
          lastMillis = millis();
        }
     
 }else if(ecran == 3){
  lcd.setCursor(0, 0);
       lcd.print("Unknown: ");
        lcd.print(startingLevelValue);
        lcd.print("           ");
       lcd.setCursor(0, 1);
       lcd.print("                ");
       
  if(buttonState == 0 && (millis() - lastMillis > millisWaitInterval))
        {
         
          ecran = 0;
          buttonState = 1;
          lastMillis = millis();
        }
 }
// set the cursor to column 0, line 1
// (note: line 1 is the second row, since counting begins with 0):
// lcd.setCursor(0, 1);
// print the number of seconds since reset:
// lcd.print(millis() / 1000);
}
