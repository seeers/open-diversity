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
unsigned int SWITCH_FPS_RESIST = 4 ; // How many FPS the video sources must differ to recitify a switch
volatile char buzzerStatus = 0; // 0 ^= no alarm, 1 an -1 ^= switched to source 1, 2 an -2 ^= switched to source 2 , 10,-10,5,-5 low Voltage, 100 be quiet!

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
  
    if ((abs(fr1 - 50) + SWITCH_FPS_RESIST < abs(fr0 - 50)) && activeSource == 0) {
      switchToVideoSource(1, fr0, fr1);
      if ( buzzerStatus == 0) {
#ifdef BUZZER_CHANNEL_IND
        buzzerStatus = 2 ; 
#endif
      }
    } else if ( (abs(fr0 - 50) + SWITCH_FPS_RESIST < abs(fr1 - 50)) && activeSource == 1) {
      switchToVideoSource(0, fr0, fr1);
       if ( buzzerStatus == 0) {
#ifdef BUZZER_CHANNEL_IND
         buzzerStatus = 1 ; 
#endif
      }
    }
    
#ifdef RSSI_ON
    int rssi0 = analogRead(RSSI1);
    int rssi1 = analogRead(RSSI2);
#ifdef SERIAL_OUTPUT_ON
    printf("Last Frames: %d (%d) / %d (%d)\n",  fr0, rssi0, fr1, rssi1);
#endif
#else
#ifdef SERIAL_OUTPUT_ON
    printf("Last Frames: %d / %d\n",  fr0, fr1);
#endif
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
  digitalWrite(LED_1, HIGH);
  digitalWrite(BUZZER, LOW);
}

void loop() {
 delay(1000);
  printf("%d",buzzerStatus);
 #ifdef LOWVOLTAG_WARN
  inputVoltage=analogRead(SUP_VOLT);
 if ( inputVoltage < ALARM_VOLT && abs(buzzerStatus) < 4 )
  { buzzerStatus = 10 ;
   }
 if ( inputVoltage > ALARM_VOLT && abs(buzzerStatus) > 4)
  { buzzerStatus = 100 ;
  }
 #endif
 
  SWITCH_FPS_RESIST=map(analogRead(SENS_INPUT),0,1023,0,50);
#ifdef SERIAL_OUTPUT_ON
  printf("input Voltage: %d \n", inputVoltage);
  printf("SWITCH_FPS_RESIST: %d \n", SWITCH_FPS_RESIST);
#endif


}
