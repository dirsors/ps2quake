/*
# _____     ___ ____     ___ ____
#  ____|   |    ____|   |        | |____|
# |     ___|   |____ ___|    ____| |    \    PS2DEV Open Source Project.
#-----------------------------------------------------------------------
# Copyright 2001-2004, ps2dev - http://www.ps2dev.org
# Licenced under Academic Free License version 2.0
# Review ps2sdk README & LICENSE files for further details.
#
# $Id: pad.c 1152 2005-06-12 17:49:50Z oopo $
# Pad demo app
# Quick and dirty, little or no error checks etc.. 
# Distributed as is
*/


#include "pad.h"
static char padBuf_t[2][256] __attribute__((aligned(64)));
struct padButtonStatus buttons_t[2];
u32 paddata, paddata_t[2];
u32 old_pad = 0, old_pad_t[2] = {0, 0};
u32 Button, Button_t[2];
u32 joy_value;
static int test_joy;


int Read_Pad(void)
{
	static int n[2]={0,0}, nn[2]={0,0};
	int port, state, ret[2];

	for(port=0; port<2; port++){
		if((state=padGetState(port, 0))==PAD_STATE_STABLE
			||(state == PAD_STATE_FINDCTP1)){
			//Deal with cases where pad state is valid for padRead
			ret[port] = padRead(port, 0, &buttons_t[port]);
			if (ret[port] != 0){
				paddata_t[port] = 0xffff ^ buttons_t[port].btns;
				if(++test_joy==2) test_joy=0;
				if(test_joy){
					joy_value=0;
					if(buttons_t[port].rjoy_h >= 0xbf){
						paddata_t[port]=PAD_R3_H1;
						joy_value=buttons_t[port].rjoy_h-0xbf;
					}else if(buttons_t[port].rjoy_h <= 0x40){
						paddata_t[port]=PAD_R3_H0;
						joy_value=-(buttons_t[port].rjoy_h-0x40);
					}else if(buttons_t[port].rjoy_v <= 0x40){
						paddata_t[port]=PAD_R3_V0;
						joy_value=-(buttons_t[port].rjoy_v-0x40);
					}else if(buttons_t[port].rjoy_v >= 0xbf){
						paddata_t[port]=PAD_R3_V1;
						joy_value=buttons_t[port].rjoy_v-0xbf;
					}else if(buttons_t[port].ljoy_h >= 0xbf){
						paddata_t[port]=PAD_L3_H1;
						joy_value=buttons_t[port].ljoy_h-0xbf;
					}else if(buttons_t[port].ljoy_h <= 0x40){
						paddata_t[port]=PAD_L3_H0;
						joy_value=-(buttons_t[port].ljoy_h-0x40);
					}else if(buttons_t[port].ljoy_v <= 0x40){
						paddata_t[port]=PAD_L3_V0;
						joy_value=-(buttons_t[port].ljoy_v-0x40);
					}else if(buttons_t[port].ljoy_v >= 0xbf){
						paddata_t[port]=PAD_L3_V1;
						joy_value=buttons_t[port].ljoy_v-0xbf;
					}
				}
				Button_t[port] = paddata_t[port] & ~old_pad_t[port];
				if(old_pad_t[port]==paddata_t[port]){
					n[port]++;
					/*if(gsKit_detect_signal()==GS_MODE_NTSC){
						if(n[port]>=25){
							Button_t[port]=paddata_t[port];
							if(nn[port]++ < 20)	n[port]=20;
							else			n[port]=23;
						}
					}*/
                    //else{
						if(n[port]>=21){
							Button_t[port]=paddata_t[port];
							if(nn[port]++ < 20)	n[port]=17;
							else			n[port]=19;
						}
				//	}
				}else{
					n[port]=0;
					nn[port]=0;
					old_pad_t[port] = paddata_t[port];
				}
			}
		}else{
			//Deal with cases where pad state is not valid for padRead
			Button_t[port]=0;
		}  //ends 'if' testing for state valid for padRead
	}  //ends for
	Button = Button_t[0]|Button_t[1];
	//paddata = paddata_t[0]|paddata_t[1];
    return (ret[0]|ret[1]);
}

void WaitPadReady(int port, int slot)
{
	int state, lastState;
	char stateString[16];

	state = padGetState(port, slot);
	lastState = -1;
	while((state != PAD_STATE_DISCONN)
		&& (state != PAD_STATE_STABLE)
		&& (state != PAD_STATE_FINDCTP1)){
		if (state != lastState)
			padStateInt2String(state, stateString);
		lastState = state;
		state=padGetState(port, slot);
	}
}

void Wait_Pad_Ready(void)
{
	int state_1, state_2;

	state_1 = padGetState(0, 0);
	state_2 = padGetState(1, 0);
	while((state_1 != PAD_STATE_DISCONN) && (state_2 != PAD_STATE_DISCONN)
		&& (state_1 != PAD_STATE_STABLE) && (state_2 != PAD_STATE_STABLE)
		&& (state_1 != PAD_STATE_FINDCTP1) && (state_2 != PAD_STATE_FINDCTP1)){
		state_1 = padGetState(0, 0);
		state_2 = padGetState(1, 0);
	}
}

int Setup_Pad(void)
{
	int ret, i, port, state, modes;

	padReset();
	padInit(0);

	for(port=0; port<2; port++){
		if((ret = padPortOpen(port, 0, &padBuf_t[port][0])) == 0)
			return 0;
		WaitPadReady(port, 0);
		state = padGetState(port, 0);
		if(state != PAD_STATE_DISCONN){
			modes = padInfoMode(port, 0, PAD_MODETABLE, -1);
			if (modes != 0){
				i = 0;
				do{
					if (padInfoMode(port, 0, PAD_MODETABLE, i) == PAD_TYPE_DUALSHOCK){
						padSetMainMode(port, 0, PAD_MMODE_DUALSHOCK, PAD_MMODE_LOCK);
						break;
					}
					i++;
				} while (i < modes);
			}
		}
	}
	return 1;
}

