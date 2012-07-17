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

//####battery Monitor#####
//comment if you dont need low voltage alarm
#define BATMONITOR

//Choose low voltage value
//3s > 9V
//2s > 6V
//Minimum voltage needed by diversity controller: 6V
#define LOWVOLTAGE 9.5

//####Debug mode#####
//comment if not used
#define DEBUG

//####RSSI#####
//comment if not used
//#define RSSI

//####Sensitivity#####
//The higher the Sensitivity the more often it will switch.
//SLOW - 1Hz - Pretty slow, but works best. One switch per second.
//MEDIUM - 2Hz - Two switches per second, should work good as well.
//FAST - 5hz - Five switches per second, excellent switching results, but more switching even if signal qualitity might be the same.
#define SENSITIVITY FAST

//####Buzzer#####
//only use if buzzer is soldered, else comment
//#define BUZZERENABLED
//makes beep if video source is switched, comment if not used
//#define BEEPONSWITCH 
