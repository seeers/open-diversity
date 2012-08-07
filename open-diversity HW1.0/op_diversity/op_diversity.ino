/* 
Open Diversity - Video based diversity software
Idea by Rangarid
Hardware design by Daniel
Implementation and refinement by Nils, Nachbrenner, Rangarid

Copyright 2011-2012 by Nils, Nachbrenner, Rangarid

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

#include <FlexiTimer2.h> // http://www.pjrc.com/teensy/td_libs_MsTimer2.html

#include "config.h"
#include "uart_custom.h"


volatile long unsigned int numSyncs[2] = {0, 0};
volatile long unsigned int lastPulses[2][HISTORY_SIZE]; 
volatile int lastPulseSlot = 0; // to use lastPulse-Array as ring buffer
volatile int numTimerEvents = 0;
unsigned int inputVoltage;
unsigned int switchFPSResist = 4 ; // How many FPS the video sources must differ to recitify a switch
volatile char buzzerStatus = 0; // 0 ^= no alarm, 1 an -1 ^= switched to source 1, 2 an -2 ^= switched to source 2 , 10,-10,5,-5 low Voltage, 100 be quiet!
volatile unsigned char dipStatus = 1; // Status Byte to control Buzzer function and ??? , here the "%2 bit" denotes the bit of the low voltage warner,  
                                      // "%3 bit" of the channel change indicator %5 %7 are free

volatile int activeSource = 0;

void switchToVideoSource(int source, int fr0,  int fr1) {
  if (source == 0) {
    digitalWrite(VSWITCH_SELECT, HIGH);
    digitalWrite(LED_1, HIGH);
    digitalWrite(LED_2, LOW);
    activeSource=0;
  } else {
    digitalWrite(VSWITCH_SELECT, LOW);
    digitalWrite(LED_1, LOW);
    digitalWrite(LED_2, HIGH);
    activeSource=1;    
  }
#ifdef SERIAL_OUTPUT_ON
  printf("Switching to source %d (%d vs %d)\n", source, fr0, fr1);
#endif
}

void VSVideo1() {
  numSyncs[0]++;
}

void VSVideo2() {
  numSyncs[1]++;
}

// Calculate average frame rate based on last recorded pulses
unsigned int calcAvgFrameRate(int videoIn) {
  unsigned int sum = 0;
  
  for(int i=0;i<HISTORY_SIZE;i++) {
      sum+=lastPulses[videoIn][i] * (1000/FR_UPDATE_INT);;
  }
  return (sum/HISTORY_SIZE);
}


void runTimer() {
  
  // Update framerate
  lastPulseSlot = (lastPulseSlot+1) % HISTORY_SIZE;
  lastPulses[0][lastPulseSlot] = numSyncs[0];
  lastPulses[1][lastPulseSlot] = numSyncs[1];
  numSyncs[0] = numSyncs[1] = 0;
    
  // see if we need to check for video switch. if yes: do it! :)
  numTimerEvents++;
  if (numTimerEvents % SWITCH_EVERY == 0) {
    numTimerEvents = 0;
    
    int fr0 = calcAvgFrameRate(0);
    int fr1 = calcAvgFrameRate(1);
  
    if ((abs(fr1 - 50) + switchFPSResist < abs(fr0 - 50)) && activeSource == 0) {
      switchToVideoSource(1, fr0, fr1);
      if ( buzzerStatus == 0 && dipStatus % 3 == 0) {
        buzzerStatus = 2 ; 
      }
    } else if ( (abs(fr0 - 50) + switchFPSResist < abs(fr1 - 50)) && activeSource == 1) {
      switchToVideoSource(0, fr0, fr1);
       if ( buzzerStatus == 0 && dipStatus % 3 == 0) {
         buzzerStatus = 1 ; 
      }
    }
    
#ifdef SERIAL_OUTPUT_ON
    printf("Last Frames: %d / %d\n",  fr0, fr1);
#endif


  }
  
    if (numTimerEvents % BEEP_EVERY == 0) { 
    if ( buzzerStatus != 0 ) {
      switch (buzzerStatus) {
        case 1: 
         digitalWrite(BUZZER, LOW);
         buzzerStatus = -1;
         break;
  
        case -1: 
         digitalWrite(BUZZER, HIGH);
         buzzerStatus = 100;
         break;
        
        case 2: 
         digitalWrite(BUZZER, LOW);
         buzzerStatus = -2;
         break;
        
        case -2: 
         digitalWrite(BUZZER, HIGH);
         buzzerStatus = 1;
         break;
         
        case 10: 
         digitalWrite(BUZZER, LOW);
         buzzerStatus = 5;
         break;
        
        case 5: 
         digitalWrite(BUZZER, LOW);
         buzzerStatus = -5;
         break;
         
        case -5: 
         digitalWrite(BUZZER, HIGH);
         buzzerStatus = -10;
         break;
         
        case -10: 
         digitalWrite(BUZZER, HIGH);
         buzzerStatus = 10;
         break;
         
        default: 
         digitalWrite(BUZZER, LOW);
         buzzerStatus = 0;
        }
    } 
  } 
}

// Calculate Dip Status in the %x representation
 unsigned char calcDipStatus( int Voltage ) {
    if ( Voltage > 976) {
   return 1 ; 
  }
  if ( Voltage <= 976 && Voltage > 900) {
   return 2 ; 
  }
  if ( Voltage <= 900 && Voltage > 836) {
   return 3 ; 
  }
  if ( Voltage <= 836 && Voltage > 782) {
   return 6 ; 
  }
  if ( Voltage <= 782 && Voltage > 735) {
   return 5 ; 
  }
  if ( Voltage <= 735 && Voltage > 690) {
   return 10 ; 
  }
  if ( Voltage <= 690 && Voltage > 662) {
   return 15 ; 
  }
  if ( Voltage <= 662 && Voltage > 641) {
   return 7 ; 
  }
  if ( Voltage <= 641 && Voltage > 622) {
   return 30 ; 
  }
  if ( Voltage <= 622 && Voltage > 599) {
   return 14 ; 
  }
  if ( Voltage <= 599 && Voltage > 570) {
   return 21 ; 
  }
  if ( Voltage <= 570 && Voltage > 544) {
   return 42 ; 
  }  
  if ( Voltage <= 544 && Voltage > 521) {
   return 35 ; 
  }
  if ( Voltage <= 521 && Voltage > 499) {
   return 70 ; 
  }
  if ( Voltage <= 499 && Voltage > 478) {
   return 105 ; 
  }
  if ( Voltage < 478) {
   return 2 ; 
  }
}

// =====================================================================

void setup() {
  
  initUART(); // for printf to serial
  // Initialize sync-vars
  for(int i=0;i<HISTORY_SIZE;i++) {
    lastPulses[0][i]=0;
    lastPulses[1][i]=0;    
  }
  
  pinMode(VSWITCH_SELECT, OUTPUT);
  pinMode(VSWITCH_DISABLE, OUTPUT);
  digitalWrite(VSWITCH_DISABLE, LOW);
  pinMode(BUZZER, OUTPUT);
  digitalWrite(BUZZER, LOW);
  pinMode(LED_1, OUTPUT);
  digitalWrite(LED_1, LOW);
  pinMode(LED_2, OUTPUT);
  digitalWrite(LED_2, LOW); 
  pinMode(SUP_VOLT, INPUT);
  pinMode(SENS_INPUT, INPUT);
  attachInterrupt(1, VSVideo1, RISING);
  attachInterrupt(0, VSVideo2, RISING);
  
  
  // Timer to update frame rate records and switch video
  FlexiTimer2::set(FR_UPDATE_INT, runTimer);
  FlexiTimer2::start();
  switchToVideoSource(0,0,0);
}

void loop() {
 delay(1000);
 
  inputVoltage=analogRead(SUP_VOLT);
 if ( inputVoltage < ALARM_VOLT && abs(buzzerStatus) < 4 && dipStatus % 2 == 0){ 
   buzzerStatus = 10 ;
   }
 if ( inputVoltage > ALARM_VOLT && abs(buzzerStatus) > 4 || dipStatus % 2 != 0) { 
   buzzerStatus = 100 ;
  }

 switchFPSResist = map(analogRead(SENS_INPUT),0,1023,1,20);
 dipStatus = calcDipStatus(analogRead(DIP_SWITCH));

#ifdef SERIAL_OUTPUT_ON
  printf("input Voltage: %d \n", inputVoltage);
  printf("switchFPSResist: %d \n", switchFPSResist);
  printf("dipStatus: %d \n",dipStatus);
#endif



}
