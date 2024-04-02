//ADD ACCEL/DECEL
//ADD SPEED POT
//ADD EL

#include <Arduino.h>
#include <U8g2lib.h>
#include <Encoder.h>

/*
#define button_park 40
*/

#define homeAzPosition 55
#define homeElPosition 0

#define leftButtonPin 32
#define rightButtonPin 33
#define leftOutputPin 6
#define rightOutputPin 7
#define encAzPinA 3
#define encAzPinB 21
#define azPPDeg 5.33333

#define azSpeedPin 54
#define azSpeedMin 25
#define azSpeedInc 4
#define azSpeedDec 10
#define azSpeedInterval 50

#define encElPinA 19
#define encElPinB 18
#define upButtonPin 34
#define downButtonPin 35
#define upOutputPin 8
#define downOutputPin 9
#define elPPDeg 4

#define elSpeedPin 53
#define elSpeedMin 25
#define elSpeedInc 10
#define elSpeedDec 10
#define elSpeedInterval 50

unsigned long azSpeedStart = millis();
unsigned long elSpeedStart = millis();

Encoder AzEncoder(encAzPinA, encAzPinB);
Encoder ElEncoder(encElPinA, encElPinB);

U8G2_SSD1309_128X64_NONAME0_F_4W_SW_SPI displayLCD(U8G2_R0, /* clock=*/ 52, /* data=*/ 50, /* cs=*/ 49, /* dc=*/ 53, /* reset=*/ 51);  

float currentAz = -100;
float currentEl = -100;

int azSpeedMax = 255;
int azSpeedLeft = 0;
int azSpeedRight = 0;

int elSpeedMax = 255;
int elSpeedUp = 0;
int elSpeedDown = 0;

void updateDisplay();
void checkAzEl();

void setup() {
  Serial.begin(9600);

  pinMode(leftButtonPin, INPUT);
  pinMode(rightButtonPin, INPUT);
  pinMode(leftButtonPin, INPUT_PULLUP);
  pinMode(rightButtonPin, INPUT_PULLUP);

  pinMode(leftOutputPin, OUTPUT);  
  pinMode(rightOutputPin, OUTPUT);
  analogWrite(leftOutputPin, azSpeedLeft);
  analogWrite(rightOutputPin, azSpeedRight);

  pinMode(upButtonPin, INPUT);
  pinMode(downButtonPin, INPUT);
  pinMode(upButtonPin, INPUT_PULLUP);
  pinMode(downButtonPin, INPUT_PULLUP);

  pinMode(upOutputPin, OUTPUT);
  pinMode(downOutputPin, OUTPUT);
  analogWrite(upOutputPin, elSpeedUp);
  analogWrite(downOutputPin, elSpeedDown);

  displayLCD.begin();

  updateDisplay();
}
void loop() {

  if(millis() - azSpeedStart >= azSpeedInterval){ //Az Button Stuff
    //int tempAzSpeedMax = map(analogRead(azSpeedPin),0,1020,azSpeedMin,255);
    int tempAzSpeedMax = 255;
    
    if(tempAzSpeedMax < azSpeedMax)azSpeedMax -= azSpeedDec;
    else azSpeedMax = tempAzSpeedMax;

    if(!digitalRead(leftButtonPin) && azSpeedRight == 0){
      if(azSpeedLeft == 0)azSpeedLeft = azSpeedMin;
      
      azSpeedLeft += azSpeedInc;
      if(azSpeedLeft > azSpeedMax)azSpeedLeft = azSpeedMax;
    }
    else{
      azSpeedLeft -= azSpeedDec;
      if(azSpeedLeft != 0 && azSpeedLeft < azSpeedMin) azSpeedLeft = 0;
    }
  
    if(!digitalRead(rightButtonPin) && azSpeedLeft == 0){
      if(azSpeedRight == 0)azSpeedRight = azSpeedMin;

      azSpeedRight += azSpeedInc;
      if(azSpeedRight > azSpeedMax)azSpeedRight = azSpeedMax;
    }
  else{
    azSpeedRight -= azSpeedDec;
    if(azSpeedRight != 0 && azSpeedRight < azSpeedMin) azSpeedRight = 0;
  }

  azSpeedStart = millis();

  analogWrite(leftOutputPin, azSpeedLeft);
  analogWrite(rightOutputPin, azSpeedRight);
  }

  if(millis() - elSpeedStart >= elSpeedInterval){ //Az Button Stuff
    //int tempAzSpeedMax = map(analogRead(azSpeedPin),0,1020,azSpeedMin,255);
    int tempElSpeedMax = 255;
    
    if(tempElSpeedMax < elSpeedMax)elSpeedMax -= elSpeedDec;
    else elSpeedMax = tempElSpeedMax;

    if(!digitalRead(upButtonPin) && elSpeedDown == 0){
      if(elSpeedUp == 0)elSpeedUp = elSpeedMin;
      
      elSpeedUp += elSpeedInc;
      if(elSpeedUp > elSpeedMax)elSpeedUp = elSpeedMax;
    }
    else{
      elSpeedUp -= elSpeedDec;
      if(elSpeedUp != 0 && elSpeedUp < elSpeedMin) elSpeedUp = 0;
    }
  
    if(!digitalRead(downButtonPin) && elSpeedUp == 0){
      if(elSpeedDown == 0)elSpeedDown = elSpeedMin;

      elSpeedDown += elSpeedInc;
      if(elSpeedDown > elSpeedMax)elSpeedDown = elSpeedMax;
    }
  else{
    elSpeedDown -= elSpeedDec;
    if(elSpeedDown != 0 && elSpeedDown < elSpeedMin) elSpeedDown = 0;
  }

  elSpeedStart = millis();

  analogWrite(downOutputPin, elSpeedDown);
  analogWrite(upOutputPin, elSpeedUp);
  }

  checkAzEl();
}
void checkAzEl(){
  float tempAz = AzEncoder.read(); tempAz /= azPPDeg;
  float tempEl = ElEncoder.read(); tempEl /= elPPDeg;
  
  tempAz += homeAzPosition;

  if(tempAz < 0)tempAz += 360;
  else if(tempAz > 360) tempAz -= 360;
  /*if(tempEl < 0){
    ElEncoder.write(0);
    tempEl = 0;
  }
  else if(tempEl > 90){
    ElEncoder.write(360);
    tempEl = 90;
  }*/

  if(tempAz != currentAz){
    currentAz = tempAz;
    updateDisplay();
  }

  if(tempEl != currentEl){
    currentEl = tempEl;
    updateDisplay();
  }
}
void updateDisplay(){
  
  displayLCD.setFontMode(1); 
  displayLCD.setDrawColor(1);
  displayLCD.setFont(u8g2_font_osr26_tn);
  displayLCD.clearBuffer();

  if(currentAz >= 0 && currentAz < 10){displayLCD.setCursor(38,25);}
  else if(currentAz >= 10 && currentAz < 100){displayLCD.setCursor(28,25);}
  else{displayLCD.setCursor(17,25);}
  displayLCD.print(currentAz,1);

  if(currentEl <= -10){displayLCD.setCursor(32,63);}
  else if(currentEl < 10 && currentEl >= 0){displayLCD.setCursor(27,63);}
  else{displayLCD.setCursor(17,63);}
  displayLCD.print(currentEl);
  
  displayLCD.sendBuffer();
}