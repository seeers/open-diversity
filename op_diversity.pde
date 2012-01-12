#include <FlexiTimer2.h> // http://www.pjrc.com/teensy/td_libs_MsTimer2.html

#include "config.h"
#include "uart_custom.h"


volatile long unsigned int numSyncs[2] = {0, 0};
volatile long unsigned int lastFrameRates[2][HISTORY_SIZE]; 
volatile int lastFrameRateSlot = 0; // to use lastFrameRates-Array as ring buffer
volatile int numTimerEvents = 0;

int activeSource = 0;

void switchToVideoSource(int source) {
  printf("Switching to source %d\n", source);
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
}

void VSVideo1() {
  numSyncs[0]++;
}

void VSVideo2() {
  numSyncs[1]++;
}


// Calculate average frame rate based on last recorded frame rates
unsigned int calcAvgFrameRate(int videoIn) {
  unsigned int sum = 0;
  cli(); // guarantees that lastFrameRates does 
  
  for(int i=0;i<HISTORY_SIZE;i++) {
      sum+=lastFrameRates[videoIn][i] * (1000/FR_UPDATE_RATE);;
  }
  sei();
  return (sum/HISTORY_SIZE);
}


void runTimer() {
  
  // Update framerate
  lastFrameRateSlot = (lastFrameRateSlot+1) % HISTORY_SIZE;
  lastFrameRates[0][lastFrameRateSlot] = numSyncs[0];
  lastFrameRates[1][lastFrameRateSlot] = numSyncs[1];
  numSyncs[0] = numSyncs[1] = 0;
  
  
  // see if we need to check for video switch. if yes: do it! :)
  numTimerEvents++;
  if (numTimerEvents >= SWITCH_EVERY) {
    numTimerEvents = 0;
    
    unsigned int fr0 = calcAvgFrameRate(0);
    unsigned int fr1 = calcAvgFrameRate(1);
  
    if ((abs(fr0 - 25) > abs(fr1 - 25) + SWITCH_FPS_RESIST) && activeSource == 0) {
      switchToVideoSource(1);
    } else if ((abs(fr0 - 25) + SWITCH_FPS_RESIST < abs(fr1 - 25)) && activeSource == 1) {
      switchToVideoSource(0);
    }
  
    printf("Last Frames: %d / %d\n",  fr0, fr1);
  }
}


// =====================================================================

void setup() {
  initUART(); // for printf to serial
  
  // Initialize sync-vars
  for(int i=0;i<HISTORY_SIZE;i++) {
    lastFrameRates[0][i]=0;
    lastFrameRates[1][i]=0;    
  }
  
  pinMode(VSWITCH_SELECT, OUTPUT);
  pinMode(VSWITCH_DISABLE, OUTPUT);
  digitalWrite(VSWITCH_DISABLE, LOW);
  pinMode(LED_1, OUTPUT);
  digitalWrite(LED_1, LOW);
  pinMode(LED_2, OUTPUT);
  digitalWrite(LED_2, LOW);  
  attachInterrupt(0, VSVideo1, RISING);
  attachInterrupt(1, VSVideo2, RISING);
  
  // Timer to update frame rate records and switch video
  FlexiTimer2::set(FR_UPDATE_RATE, runTimer);
  FlexiTimer2::start();
  
  // Test Pins
  pinMode(11,OUTPUT);
  digitalWrite(11,LOW);
  pinMode(13,OUTPUT);
  digitalWrite(13,LOW);
}




void loop() {

  // Generate test frames
  for(int i=0;i<25;i++) {
    digitalWrite(13,HIGH);  
    digitalWrite(13,LOW);  
  }
  for(int i=0;i<50;i++) {
    digitalWrite(11,HIGH); 
    digitalWrite(11,LOW);  
  }
  delay(1000);
}
