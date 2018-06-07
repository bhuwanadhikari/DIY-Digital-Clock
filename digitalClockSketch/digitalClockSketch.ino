    
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
 bool tempChecker = false, adjustChecker = false;
 long int limit=500;
 int hourFixer;
 
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
}



//LOOP LOOP  ----------------------------------------------
void loop() {
   DateTime now = rtc.now();
   h = now.hour();
   m = now.minute();
   s = now.second();
   
  if((h%12==11) && m == 14 && s ==35){
    adjustChecker = true;
  }
  
  if(m == 15 && s ==35 && adjustChecker==true){
   adjustTime();
   adjustChecker = false;
  }

  
   if(s==8 || s==38){
    tempChecker = true;
   }

  
   if(tempChecker == false){
      sense();
   }
   if(tempChecker==true){
      temp = analogRead(tempPin);
      temp = (5.0*temp*1000.0)/(1023*10);
      tempChecker = false;
      }
      
  if(s ==9 || s==10 || s ==11 || s ==39 || s==40 || s ==41){ 
    displayTemperature();
  } else {
    displayTime();
  }
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
  if(millis()%1000 <= 5
  00){
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
    
  } else{
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

  if(h==12 || h==00){
    
    select(2);
    echo(digit[2]);
    delayMicroseconds(delayInMs);
    reset();
  
    select(1);
    echo(digit[1]);
    delayMicroseconds(delayInMs);
    reset();
  
  } else{
    select(2);
    echo(digit[(h%12)%10]);
    delayMicroseconds(delayInMs);
    reset();
  
    select(1);
    echo(digit[(h%12)/10]);
    delayMicroseconds(delayInMs);
    reset();

  }
  }
//-----------------------------------------
void adjustTime(){
  DateTime dt(2018, 06, 02, h, m, s-13, 5); // year, month, day, hour in 24, minute, sec, week day from ) to 7
  rtc.setDateTime(dt); //Adjust date-time as defined 'dt' above
  }



