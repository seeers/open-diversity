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

//Low Voltage Alarm Value
#define LOWVOLTAGE 9.0

//Video values filter size
#define HISTORY_SIZE 5 // How many old framerates to keep to calculate avg

//Update Rate
#define FR_UPDATE_RATE 100 // update framerate records every XX milliseconds
#define SWITCH_RATE 200 // look if I need to switch video input every XXX milliseconds. Needs to be > FR_UPDATE_RATE

// === Optional RSSI
//uncomment to enable RSSI, comment to disable
//#define RSSI_ON

// === Debug
//uncomment to enable, comment to disable
#define DEBUG

// === Buzzer Config
//uncomment to enable, comment to disable
#define BEEPONSTART






