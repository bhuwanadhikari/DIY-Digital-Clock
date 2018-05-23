
#include <Wire.h>
#include "Sodaq_DS3231.h"

int i ; 
char weekDay[][4] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };

//year, month, date, hour, min, sec and week-day(starts from 0 and goes to 6)
//writing any non-existent time-data may interfere with normal operation of the RTC.
//Take care of week-day also.
DateTime dt(2018, 5, 23, 00, 26, 0,3); // year, month, day, hour in 24, minute, sec, week day from ) to 7


void setup () 
{
    Serial.begin(9600);
    Wire.begin();
    rtc.begin();
  rtc.setDateTime(dt); //Adjust date-time as defined 'dt' above 
}

void loop () 
{
    DateTime now = rtc.now(); //get the current date-time
    Serial.print(now.year());
    Serial.print('/');
    Serial.print(now.month());
    Serial.print('/');
    Serial.print(now.date());
    Serial.print(' ');
    Serial.print(now.hour());
    Serial.print(':');
    Serial.print(now.minute());
    Serial.print(':');
    Serial.print(now.second());
    Serial.println();
    Serial.print(weekDay[now.dayOfWeek()]);
    Serial.println();
   
    delay(1000);
   

   
    




    
}
