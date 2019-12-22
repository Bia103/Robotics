
#include "LedControl.h" //  need the library
LedControl lc = LedControl(12, 11, 10, 1); //DIN, CLK, LOAD, No. DRIVER
 
// pin 12 is connected to the MAX7219 pin 1
// pin 11 is connected to the CLK pin 13
// pin 10 is connected to LOAD pin 12
// 1 as we are only using 1 MAX7219
#include <EEPROM.h>
 
#define SAMPLE_TIME 2000  //The time between each EEPROM write function call in ms
int address = 0;  
#include <LiquidCrystal.h>
const int RS = 9;
const int enable = 8;
const int d4 = 5;
const int d5 = 4;
const int d6 = 3;
const int d7 = 2;

LiquidCrystal lcd(RS, enable, d4, d5, d6, d7);

float conv_coeff = 0.0;

int matrix[8][8] = {
{0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 5, 5, 0, 0, 0},
{0, 0, 0, 5, 5, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0}
};

int matrix2[8][8] = {
{0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 1, 0, 0, 1, 0, 0},
{0, 0, 1, 0, 0, 1, 0, 0},
{0, 0, 1, 0, 0, 1, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 1, 1, 1, 1, 0, 0},
{0, 1, 0, 0, 0, 0, 1, 0},
{0, 0, 0, 0, 0, 0, 0, 0}
};

int matrix3[8][8] = {
{0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 1, 0, 0, 1, 0, 0},
{0, 0, 1, 0, 0, 1, 0, 0},
{0, 0, 1, 0, 0, 1, 0, 0},
{0, 1, 0, 0, 0, 0, 1, 0},
{0, 0, 1, 1, 1, 1, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0}
};
const int joyX = A1;
const int joyY = A0;
const int joy = 7;

unsigned long lastMillis = 0;
unsigned long millisWaitInterval = 1000;

int ecran = 0, ok2;
int startingLevelValue = 0;

int prevX = 0, prevY = 0, posX = 0, posY = 0, buttonState=0;
int valX, valY;
boolean movedX = false, movedY = false;
int ok = 0, side = 0;
int lastTime = 0, i = 0, j = 0;

int highScore = 0;
int score = 0;
int lvl = 0;
int lives = 3;

unsigned long debounceDelay = 30000;
unsigned long lastDebounceTime = 0;
unsigned long lastMoveTimeOrb = 0;
unsigned long lastGenerationTime = 0;
unsigned long lastGenerated = 0;
unsigned long lastGenerationTimeBonus = 0;
unsigned long lastGeneratedBonus = 0;
unsigned long lastMoveBonus = 0;
unsigned long blinkTime = 0;

int n = 0, m = 0;
int v[50], a[50];

void setup()
{
  pinMode(joyX , INPUT);
  pinMode(joyY, INPUT);
  // the zero refers to the MAX7219 number, it is zero for 1 chip
  lc.shutdown(0, false); // turn off power saving, enables display
  lc.setIntensity(0, 2); // sets brightness (0~15 possible values)
  lc.clearDisplay(0);// clear screen
  Serial.begin(9600);
  lcd.begin(16, 2);
// Print a message to the LCD.
  lcd.print("     Welcome  ");
  Serial.begin(9600);
  pinMode(joy, INPUT_PULLUP);
  randomSeed(analogRead(0));
  conv_coeff = 5.0/1024.0;
}
void generateBonus(){
  
  int j = random(0,4);
  if(j == 0){
    int y = random(3,5);
    matrix[7][y] = 4;
  }else if(j == 2){
    int y = random(3,5);
    matrix[y][7] = 4;
  }else if(j == 1){
    int y = random(3,5);
    matrix[0][y] = 4;
  }else if(j == 3){
    int y = random(3,5);
    matrix[y][0] = 4;
  }
}
void generateOrbs(){
  
  int j = random(0,4);
  if(j == 0){
    int y = random(3,5);
    matrix[7][y] = 2;
  }else if(j == 2){
    int y = random(3,5);
    matrix[y][7] = 2;
  }else if(j == 1){
    int y = random(3,5);
    matrix[0][y] = 2;
  }else if(j == 3){
    int y = random(3,5);
    matrix[y][0] = 2;
  }
}
/*void moveBonusNow(int n){
  
if((millis() - lastMoveBonus) > (5000/(lvl+1))){
  //lastMoveTimeOrb = millis();
  
  for(int i = 0; i < n ;i++)
  {
    int row = v[i*2];
    int col = v[i*2+1];
    matrix[row][col] = 0;
    Serial.println(row);
    Serial.println(col);
    if(col >= 5){
     if(matrix[row][col - 1] == 0)
        {matrix[row][col - 1] = 4;
        }
       if(matrix[row][col - 1] == 5)
        score++;
    }
    else if(row <= 3)
    {
   if(matrix[row + 1][col] == 0)
    matrix[row + 1][col] = 4;
    if(matrix[row + 1][col] == 5)
        score++;
  }else if(row >= 5){
      if(matrix[row - 1][col] == 0)
       { matrix[row - 1][col] = 4;
       
       }
        if(matrix[row - 1][col] == 5)
        score++;
  }else if(col <= 3){
     if(matrix[row][col + 1] == 0)
        matrix[row][col + 1] = 4;
      if(matrix[row][col + 1] == 5)
        score++;
  }
}
lastMoveBonus = millis();}

}
*/
void cleanMatrix(){
  for (int row = 0; row < 8; row++)
{
  for (int col = 0; col < 8; col++)
  {
    if((matrix[row][col] == 2 || matrix[row][col] == 4)){
      matrix[row][col] = 0;
    }
  }
}
//mo
}
void moveOrb(int n){
  
if((millis() - lastMoveTimeOrb) > (5000/(lvl+1))){
  //lastMoveTimeOrb = millis();
  
  for(int i = 0; i < n ;i++)
  {
    int row = v[i*2];
    int col = v[i*2+1];
    matrix[row][col] = 0;
    Serial.println(row);
    Serial.println(col);
    if(col >= 5){
     if(matrix[row][col - 1] == 0)
        {matrix[row][col - 1] = 2;
        }
       if(matrix[row][col - 1] == 5)
        lives --;
    }
    else if(row < 3)
    {
   if(matrix[row + 1][col] == 0)
    matrix[row + 1][col] = 2;
    if(matrix[row + 1][col] == 5)
        lives --;
  }else if(row >= 5){
      if(matrix[row - 1][col] == 0)
       { matrix[row - 1][col] = 2;
       
       }
        if(matrix[row - 1][col] == 5)
        lives --;
  }else if(col < 3){
     if(matrix[row][col + 1] == 0)
        matrix[row][col + 1] = 2;
      if(matrix[row][col + 1] == 5)
        lives --;
  }
}
 for(int i = 0; i < m ;i++)
  {
    int row = a[i*2];
    int col = a[i*2+1];
    matrix[row][col] = 0;
    Serial.println(row);
    Serial.println(col);
    if(col >= 5){
     if(matrix[row][col - 1] == 0)
        {matrix[row][col - 1] = 4;
        }
       if(matrix[row][col - 1] == 5)
        score++;
    }
    else if(row < 3)
    {
   if(matrix[row + 1][col] == 0)
    matrix[row + 1][col] = 4;
    if(matrix[row + 1][col] == 5)
        score++;
  }else if(row >= 5){
      if(matrix[row - 1][col] == 0)
       { matrix[row - 1][col] = 4;
       
       }
        if(matrix[row - 1][col] == 5)
        score++;
  }else if(col < 3){
     if(matrix[row][col + 1] == 0)
        matrix[row][col + 1] = 4;
      if(matrix[row][col + 1] == 5)
        score++;
  }
}

lastMoveTimeOrb = millis();}

}
void sad(){
for(int i = 1;i <=500;i++){
  for (int row = 0; row < 8; row++)
{
  for (int col = 0; col < 8; col++)
  {
    
     lc.setLed(0, row, col, matrix2[row][col]);
    }
  }

//mo
}
}
void happy(){
for(int i = 1;i <=500;i++){
  for (int row = 0; row < 8; row++)
{
  for (int col = 0; col < 8; col++)
  {
    
     lc.setLed(0, row, col, matrix3[row][col]);
    }
  }

//mo
}
}
void moveMagicOrbs()
{
 n = 0;
 m = 0;
for (int row = 0; row < 8; row++)
{
  for (int col = 0; col < 8; col++)
  {
    if((matrix[row][col] == 2)){
       v[n*2] = row;
       v[n*2+1] = col;
       n++;
    }
      if((matrix[row][col] == 4)){
       a[m*2] = row;
       a[m*2+1] = col;
       m++;
    }
  }
}
moveOrb(n);

}
void moveBonus()
{
 n = 0;
for (int row = 0; row < 8; row++)
{
  for (int col = 0; col < 8; col++)
  {
    if((matrix[row][col] == 4)){
       v[n*2] = row;
       v[n*2+1] = col;
       n++;
    }
  }
}
//moveBonusNow(n);

}
int ok1 = 0;
void startGame()
{
  
  if(millis() - lastGenerationTime >= lastGenerated)
    {
      generateOrbs();
    lastGenerationTime = millis();
    lastGenerated = random(1000,3000);
    ok1 = random(0,4);
    if(ok1 == 2)
      generateBonus();
    }
   /* if(millis() - lastGenerationTimeBonus >= lastGeneratedBonus)
    {
      generateBonus();
      lastGenerationTimeBonus = millis();
      lastGeneratedBonus = random(3000,7000);
    }*/
   
moveMagicOrbs();
//moveBonus();
int prevX = 0, prevY = 0, posX = 0, posY = 0;
for (int row = 0; row < 8; row++)
{
  for (int col = 0; col < 8; col++)
  {
    if(matrix[row][col] == 4){
      if((millis() - blinkTime) >= 500){
        lc.setLed(0, row, col, matrix[row][col]);
        blinkTime = millis();
      }
       else lc.setLed(0, row, col, 0);
    }else{
      lc.setLed(0, row, col, matrix[row][col]);
    }
    
  }
}
  i++; 
 valX = analogRead(joyX);
 valY = analogRead(joyY);


  if (valY < 400)
  {
    if (movedY == false)
    {
      prevY = posY;
      posY = 0;
      side = 0;
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
        posY = 1;
        side = 1;
        movedY = true;
      }
    }
    else movedY = false;
  }

  if (valX < 400)
  {
    if (movedX == false)
    {
      prevX = posX;
      posX = 0;
      side = 2;
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
        posX = 1;
        side = 3;
        movedX = true;
      }
    }
    else movedX = false;
  }

  if(side == 0){
    if(matrix[2][3] != 2 && matrix[2][3] != 4)
      matrix[2][3] = 0;
    if(matrix[2][4] != 2 && matrix[2][4] != 4)
      matrix[2][4] = 0;
    if(matrix[3][2] != 2 && matrix[3][2] != 4)
      matrix[3][2] = 0;
    if(matrix[4][2] != 2 && matrix[4][2] != 4)
      matrix[4][2] = 0;
    matrix[5][3] = 1;//
    matrix[5][4] = 1;//
    if(matrix[3][5] != 2 && matrix[3][5] != 4)
      matrix[3][5] = 0;
    if(matrix[4][5] != 2 && matrix[4][5] != 4)
      matrix[4][5] = 0;

  }else if(side == 1){
    matrix[2][3] = 1;//
    matrix[2][4] = 1;//
   if(matrix[3][2] != 2 && matrix[3][2] != 4)
      matrix[3][2] = 0;
    if(matrix[4][2] != 2 && matrix[4][2] != 4)
      matrix[4][2] = 0;
    if(matrix[5][3] != 2 && matrix[5][3] != 4)
      matrix[5][3] = 0;
    if(matrix[5][4] != 2 && matrix[5][4] != 4)
      matrix[5][4] = 0;
    if(matrix[3][5] != 2 && matrix[3][5] != 4)
      matrix[3][5] = 0;
    if(matrix[4][5] != 2 && matrix[4][5] != 4)
      matrix[4][5] = 0;

  }else if(side == 2){
     if(matrix[2][3] != 2 && matrix[2][3] != 4)
      matrix[2][3] = 0;
    if(matrix[2][4] != 2 && matrix[2][4] != 4)
      matrix[2][4] = 0;
  if(matrix[3][2] != 2 && matrix[3][2] != 4)
      matrix[3][2] = 0;
    if(matrix[4][2] != 2 && matrix[4][2] != 4)
      matrix[4][2] = 0;
    if(matrix[5][3] != 2 && matrix[5][3] != 4)
      matrix[5][3] = 0;
    if(matrix[5][4] != 2 && matrix[5][4] != 4)
      matrix[5][4] = 0;
    matrix[3][5] = 1;//
    matrix[4][5] = 1;//
  }else if(side == 3){
  if(matrix[2][3] != 2 && matrix[2][3] != 4)
      matrix[2][3] = 0;
    if(matrix[2][4] != 2 && matrix[2][4] != 4)
      matrix[2][4] = 0;
    matrix[3][2] = 1;//
    matrix[4][2] = 1;//
  if(matrix[5][3] != 2 && matrix[5][3] != 4)
      matrix[5][3] = 0;
    if(matrix[5][4] != 2 && matrix[5][4] != 4)
      matrix[5][4] = 0;
    if(matrix[3][5] != 2 && matrix[3][5] != 4)
      matrix[3][5] = 0;
    if(matrix[4][5] != 2 && matrix[4][5] != 4)
      matrix[4][5] = 0;
  }
}
int ok3 = 0, ok7 = 0;
void loop()
{
  if(ok7 == 0){
  if(ok3 == 0){
    ok3 = 1;
    lcd.print("   Welcome   ");
    delay(5000);
  }
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
   if (posY == 4) posY = 0;
   if (posY == -1) posY = 2;
   //Serial.println(buttonState);
  }
  if( ecran == 0 )
  {
    lives = 3;
    lvl = 0;
     for (int row = 0; row < 8; row++)
        {
          for (int col = 0; col < 8; col++)
            {
              lc.setLed(0, row, col, 0);
            }
        }
    if(posY == 0)
    {
      lcd.setCursor(0, 0);
      lcd.print(">Start    Setup ");
      lcd.setCursor(0, 1);
      lcd.print(" Highscore  Info          ");
      if(buttonState == 0 && millis() - lastMillis > millisWaitInterval)
        {
          startGame();
          lastMoveTimeOrb = millis();
          ecran = 1;
          buttonState = 1;
          lastMillis = millis();
          ok7 = 1;
        }

    }
    else if(posY == 1)
    {
      lcd.setCursor(0, 0);
      lcd.print(" Start   >Setup ");
      lcd.setCursor(0, 1);
      lcd.print(" Highscore  Info          ");
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
      lcd.print(">Highscore  Info           ");
      if(buttonState == 0 && millis() - lastMillis > millisWaitInterval)
        {
          ecran = 3;
          buttonState = 1;
          lastMillis = millis();
        }

    }else if(posY == 3)
    {
      lcd.setCursor(0, 0);
      lcd.print(" Start    Setup ");
      lcd.setCursor(0, 1);
      lcd.print(" Highscore >Info     ");
      if(buttonState == 0 && millis() - lastMillis > millisWaitInterval)
        {
          ecran = 4;
          buttonState = 1;
          lastMillis = millis();
        }

    }
  }
  else if(ecran == 1){
    
       startGame();
       lcd.setCursor(0, 0);
       lcd.print("Level: ");
       lcd.print(lvl+1);
       lcd.print("             ");
       lcd.setCursor(0, 1);
       lcd.print("Score: ");
       lcd.print(score);
       lcd.print(" Lives:");
       lcd.print(lives);
       lcd.print("        ");
        if((millis() - lastDebounceTime) > debounceDelay){
          lvl ++;
          lastDebounceTime = millis();
       }
       if(lives <= 0)
       { ok7 = 0;
        int value = EEPROM.read(0); 
        if(value > highScore)
        {
          highScore = score;
          EEPROM.update(0,highScore); 
          ecran = 0;
        
        score = 0;
        cleanMatrix();
        lcd.setCursor(0, 0);
        lcd.print("Congratulations on making");
        lcd.setCursor(0, 1);
         lcd.print(" a highscore ");
         lcd.print(lvl +1);
         happy();
         delay(2000);
        lvl = 0;

        //ecran = 5;
        }else{
        ecran = 0;
                sad();
        score = 0;
        cleanMatrix();
        lcd.setCursor(0, 0);
        lcd.print("Congratulations on making");
        lcd.setCursor(0, 1);
         lcd.print(" past level ");
         lcd.print(lvl +1);
         lcd.print("    ");
         delay(2000);
        lvl = 0;
        //ecran = 5;
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
       lcd.print("Unknown: Highscore:");
       //lcd.print(highScore);
      // lcd.setCursor(0, 1);
    
    int value = EEPROM.read(0);                //read EEPROM data at address i
    lcd.setCursor(0,1);
    lcd.print(value);
   lcd.print("                ");
       
  if(buttonState == 0 && (millis() - lastMillis > millisWaitInterval))
        {
         
          ecran = 0;
          buttonState = 1;
          lastMillis = millis();
        }

    

}else if(ecran == 4){
  lcd.setCursor(0, 0);
       lcd.print("  Furculesteanu         ");
      
       lcd.setCursor(0, 1);
  
       lcd.print("    Bianca ");
       
       lcd.print("        ");
       delay(3000);
     lcd.setCursor(0, 0);
       lcd.print("     Magic        ");
      
       lcd.setCursor(0, 1);
  
       lcd.print("      Orbs ");
       
       lcd.print("        ");
       delay(3000);
   lcd.setCursor(0, 0);
       lcd.print("      Github         ");
      
       lcd.setCursor(0, 1);
  
       lcd.print("https://github.com ");
       
       lcd.print("        ");
       delay(3000);
       

          ecran = 0;
       

    

}


}
/*
 * 
 * matrix[2][3] = 0;
    matrix[2][4] = 0;
    matrix[3][2] = 0;
    matrix[4][2] = 0;
    matrix[5][3] = 0;
    matrix[5][4] = 0;
    matrix[3][5] = 1;
    matrix[4][5] = 1;
 */
