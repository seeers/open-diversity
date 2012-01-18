// PINS
#define VSWITCH_DISABLE 10
#define VSWITCH_SELECT 9

#define LED_1 7
#define LED_2 6

#define HISTORY_SIZE 5 // How many old framerates to keep to calculate avg
#define FR_UPDATE_RATE 100 // update framerate records every XX milliseconds
#define SWITCH_RATE 200 // look if I need to switch video input every XXX milliseconds. Needs to be > FR_UPDATE_RATE

#define SWITCH_FPS_RESIST 2 // How many FPS the video sources must differ to recitify a switch


// === Optional RSSI

#define RSSI_ON // comment out to disable RSSI output
#define RSSI1 A1 
#define RSSI2 A0 // A0 ist the Pin that is next to yellow led

// ===================

#define SWITCH_EVERY SWITCH_RATE / FR_UPDATE_RATE



