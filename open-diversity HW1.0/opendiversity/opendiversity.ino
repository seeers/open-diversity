/* 
Open Diversity - Video based diversity software
Idea by Rangarid
Hardware design by Daniel
Implementation by Rangarid

Copyright 2011-2012 by Rangarid

This file is part of Open Diversity

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA. */

#include "defines.h"
#include "config.h"

void setup()
{
  #ifdef BUZZERENABLED
    digitalWrite(BUZZER, HIGH);
  #endif

#ifdef DEBUG
  Serial.begin(9600);
#endif

  //vsync0 and vsync1
  pinMode(VSYNC0,INPUT);
  pinMode(VSYNC1,INPUT);

  //ADG794 switch inputs
  //IN LOW, EN LOW: vid0
  //IN HIGH, EN LOW: vid1
  pinMode(IN, OUTPUT);
  pinMode(EN, OUTPUT);

  //LEDs
  pinMode(LED0, OUTPUT);
  pinMode(LED1, OUTPUT);

  //LED 0/1 HIGH for on
  digitalWrite(LED0, LOW);
  digitalWrite(LED1, LOW);

  //interrupts to read vsync
  attachInterrupt(0, sync0, RISING);
  attachInterrupt(1, sync1, RISING); 
}

void loop()
{
  #ifdef BUZZERENABLED
    delay(500);
    digitalWrite(BUZZER, LOW);
  #endif
  
  long timeVideo = millis();
  
  #ifdef BUZZERENABLED
    #ifdef BATMONITOR
      long timeBuzzer = timeVideo;
      boolean alarm = false;
    #endif
  #endif
  
  int interval = 0;
  
  while(true)
  {
    long currentTime = millis();
    
    #ifdef BUZZERENABLED
      #ifdef BATMONITOR
        //Low Voltage Alarm
        if (currentTime - timeBuzzer > 500)
        {
          if (checkVoltage())
          {
            digitalWrite(BUZZER, alarm);
            timeBuzzer = currentTime;
            alarm = !alarm;
          }
          else
          {
            timeBuzzer = currentTime;
          }
        }
      #endif
    #endif
    
    //define sensitivity with poti
    interval = analogRead(POTI);
    if (interval < 342)
    {
      //slow
      interval = 1000;
    }
    else if (interval >= 342 && interval < 684)
    {
      //medium
      interval = 500;
    }
    else if (interval >=684 && interval < 1024)
    {
      //fast
      interval = 200;
    }
    
    if (currentTime - timeVideo >= interval)
    {
      #ifdef DEBUG
        Serial.print("Vid 0: " ); Serial.print(syncs[0]); Serial.print(" Vid 1: "); Serial.println(syncs[1]);
      #endif
      selectVideo();
      timeVideo = currentTime;
    }
    delay(50);
  }
}

void selectVideo()
{
  //if vsync0 has more frames then vsync1
  if (syncs[0] > syncs[1])
  {
    digitalWrite(EN, LOW);
    digitalWrite(IN, LOW);
    digitalWrite(LED0, HIGH);
    digitalWrite(LED1, LOW);
  }
  //if vsync1 has more frames then vsync0
  else if (syncs[1] > syncs[0])
  {
    digitalWrite(EN, LOW);
    digitalWrite(IN, HIGH);
    digitalWrite(LED0, LOW);
    digitalWrite(LED1, HIGH);
  }
  //if both video signals have same frames
  else if (syncs[0] == syncs[1])
  {
    //Compare RSSI if enabled
    //if no RSSI is enabled video source won't be switched
    #ifdef RSSI
      if (rssi[0] > rssi[1])
      {
        digitalWrite(EN, LOW);
        digitalWrite(IN, LOW);
        digitalWrite(LED0, HIGH);
        digitalWrite(LED1, LOW);
      }
      //if vsync1 has more frames then vsync0
      else if (rssi[1] > rssi[0])
      {
        digitalWrite(EN, LOW);
        digitalWrite(IN, HIGH);
        digitalWrite(LED0, LOW);
        digitalWrite(LED1, HIGH);
      }
    #endif
  }
  clearSyncs();
}

void clearSyncs()
{
  syncs[0] = 0;
  syncs[1] = 0;
}

//Interrupt Service Routine for vsync0
void sync0()
{
  syncs[0]++;
}

//Interrupt Service Routine for vsync1
void sync1()
{
  syncs[1]++;
}

boolean checkVoltage()
{
  float voltage = (float)analogRead(VBAT)/1024.0*19.4123;
  
  if (abs(voltage) <= abs(LOWVOLTAGE))
  {
    return true;
  }
  return false;
}
