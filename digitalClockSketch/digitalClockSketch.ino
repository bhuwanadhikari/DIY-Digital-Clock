    
#include <Wire.h>
#include "Sodaq_DS3231.h"

#define UP  8
#define DOWN  7
#define DIVIDER 13


  int led[7] = {6, 5, 4, 3, 2, 1, 0}; // serially a, b, c, d, e, f, g
  
  
  int selector[4] = {9, 10, 11, 12}; // four digit selectors: first, second, third, fourth
  int digit[10][7] = {  {1, 1, 1, 1, 1, 1, 0},
                        {0, 1, 1, 0, 0, 0, 0},
                        {1, 1, 0, 1, 1, 0, 1},
                        {1, 1, 1, 1, 0, 0, 1},
                        {0, 1, 1, 0, 0, 1, 1},
                        {1, 0, 1, 1, 0, 1, 1},
                        {1, 0, 1, 1, 1, 1, 1},
                        {1, 1, 1, 0, 0, 0, 0},
                        {1, 1, 1, 1, 1, 1, 1},
                        {1, 1, 1, 1, 0, 1, 1}
                       };

 int lightPin = A3;
 int tempPin = A2;
 
 unsigned long int delayInMs;
 
 int temp;
 long int light;
 int reading;
 long int limit=500;
 
 int h, m, s;

 

void setup() {

  
    Wire.begin();
    rtc.begin();
    //rtc.setDateTime(dt);
  
  for(int i = 0; i<=13; i++){
    pinMode(i, OUTPUT);
   }
  pinMode(tempPin, INPUT); // to LM 35
  pinMode(lightPin, INPUT); // to LDR
   int taketempque = 0;
}



//LOOP LOOP  ----------------------------------------------
void loop() {
   DateTime now = rtc.now();
   h = now.hour();
   m = now.minute();
   s = now.second();
   operate();
}


//HELPER FUNCTIONS-----------------------------------------
void echo(int arr[]){
  for(int i = 0; i<7; i++){
    if(arr[i] == 1){
      high(led[i]);
    } else{
      low(led[i]);
    }
  }
}



//------------------------------------------
void select(int x){
  for(int i = 0; i<4; i++){
    if(i == x-1){
      high(selector[i]);
    }else{
      low(selector[i]);
    }
  }
}
//-------------------------------------------
  void high( int x){
    digitalWrite(x, HIGH);
  }
//-------------------------------------------
  void low(int x){
    digitalWrite(x, LOW);
  }
//-------------------------------------------
  void reset(){
    for(int i = 0; i<=6; i++){
      low(i);
    }
     for(int i = 9; i<=13; i++){
      low(i);
    }
  }

//------------------------------------------
void sense(){
  light = analogRead(lightPin);
  if (light<limit){
    delayInMs  = 10;
    
  }else{
    delayInMs = 3000;
  }
}
//------------------------------------------
void autoBlinkDivider( ){
  if(millis()%1000 <= 500){
    if(light < limit){
      high(DIVIDER);
      high(UP);
      high(DOWN);
      delayMicroseconds(delayInMs);
      low(DIVIDER);
      low(UP);
      low(DOWN);
    } else{
      high(DIVIDER);
      high(UP);
      high(DOWN);
    }
    
  }else{
    low(DIVIDER);
    low(UP);
    low(DOWN);
  }
}

//-----------------------------------------
void autoSetDivider(){
    high(DIVIDER);
    high(UP);
    low(DOWN);
    delayMicroseconds(delayInMs);
    low(DIVIDER);
    low(UP);
    low(DOWN);
}

//------------------------------------------

void operate(){
  sense();
  
  if(s%60 == 7){
      reading = analogRead(tempPin);
      temp = (5.00*reading*1000.00/(1023*10));
      temp = temp*1.35;
  }
      
  if (s%15 > 7 && s%15 < 11){
    displayTemperature();
  }else{
    displayTime();
  }
  
}
//------------------------------------------
void displayTemperature(){
  
  autoSetDivider();
  
  select(1);
  echo(digit[temp/10]);
  delayMicroseconds(delayInMs/1.5);
  reset();
  
  select(2);
  echo(digit[temp%10]); 
  delayMicroseconds(delayInMs/1.5);
  reset();

  displayC();
  delayMicroseconds(delayInMs/1.5);
  reset();
  
  
}
//------------------------------------------
//------------------------------------------
  //Display C after temperature
  
  void displayC(){
    select(3);
    high(led[0]);
    low(led[1]);
    low(led[2]);
    high(led[3]);
    high(led[4]);
    high(led[5]);
    low(led[6]);
    delayMicroseconds(delayInMs);
    reset();
  }
  
//-------------------------------------------
 // DISPLAY TIME FUNCTION DISPLAY TIME FUNCTION ---------------
 void displayTime(){
  
  autoBlinkDivider();
  
  select(4);
  echo(digit[m%10]);
  delayMicroseconds(delayInMs);
  reset();
  
  select(3);
  echo(digit[m/10]);
  delayMicroseconds(delayInMs);
  reset();
  
  select(2);
  echo(digit[(h%12)%10]);
  delayMicroseconds(delayInMs);
  reset();
  
  select(1);
  echo(digit[(h%12)/10]);
  delayMicroseconds(delayInMs);
  reset();

 }
//-----------------------------------------



