// PINS
#define VSWITCH_DISABLE 10
#define VSWITCH_SELECT 9

#define LED_1 7
#define LED_2 6

#define HISTORY_SIZE 10 // How many old framerates to keep to calculate avg
#define FR_UPDATE_RATE 100 // update framerate records every XX milliseconds
#define SWITCH_RATE 500 // look if I need to switch video input every XXX milliseconds. Needs to be > FR_UPDATE_RATE 100

#define SWITCH_FPS_RESIST 4 // How many FPS the video sources must differ to recitify a switch


// ===================

#define SWITCH_EVERY SWITCH_RATE / FR_UPDATE_RATE



