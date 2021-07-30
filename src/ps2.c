
#include "ps2.h"
#include "pad.h"
extern u32 usbd_irx;
extern u32 size_usbd_irx;

extern u32 usbhdfsd_irx;
extern u32 size_usbhdfsd_irx;

extern u32 iomanx_irx;
extern u32 size_iomanx_irx;

 
void IOP_reset() // resets IOP and apply sbv patches.	
{	
	SifInitRpc(0);
	
	while(!SifIopReset("rom0:UDNL rom0:EELOADCNF",0));
	while(!SifIopSync());
	fioExit();
	SifExitIopHeap();
	SifLoadFileExit();
	SifExitRpc();
	SifExitCmd();
	SifInitRpc(0);
  	FlushCache(0);
  	FlushCache(2);
  	//twice, some in-hdloader hack
  	while(!SifIopReset("rom0:UDNL rom0:EELOADCNF",0));
  	while(!SifIopSync());
  	fioExit();
  	SifExitIopHeap();
  	SifLoadFileExit();
  	SifExitRpc();
  	SifExitCmd();

  	SifInitRpc(0);
  	FlushCache(0);
  	FlushCache(2);

  	SifLoadFileInit();

}	

void loadmodules()
{
   int ret, id;

   ret = SifLoadModule("rom0:SIO2MAN", 0, NULL);
   if(ret < 0){
          printf("Error loading rom0:XSIO2MAN\n");
              }
   /*ret = SifLoadModule("rom0:MCMAN", 0, NULL);
   if(ret < 0){
          printf("Error loading rom0:MCMAN\n");
              }
   ret = SifLoadModule("rom0:MCSERV", 0, NULL);
   if(ret < 0){
          printf("Error loading rom0:MCSERV\n");
          }*/
  #ifdef _IOPRESET
   if ((id = SifExecModuleBuffer(&iomanx_irx, size_iomanx_irx, 0, NULL, &ret)) < 0) {
		printf("Error loading iomanx\n");
	}
  #endif            
   ret = SifLoadModule("rom0:PADMAN", 0, NULL);
   if(ret < 0){
          printf("Error loading rom0:XPADMAN\n");
              }
  
  /* ret = SifLoadModule("rom0:LIBSD", 0, NULL);
   if(ret < 0){
          printf("Error loading rom0:LIBSD\n");
              }*/
      
    if ((id = SifExecModuleBuffer(&usbd_irx, size_usbd_irx, 0, NULL, &ret)) < 0) {
		printf("Error loading usbd\n");
	}
    if ((id = SifExecModuleBuffer(&usbhdfsd_irx, size_usbhdfsd_irx, 0, NULL, &ret)) < 0) {
		printf("Error loading usbhdfsd\n");
	}
	 
    /*if ((id = SifExecModuleBuffer(&ps2kbd_irx, size_ps2kbd_irx, 0, NULL, &ret)) < 0) {
		scr_printf("Error loading ps2kbd\n");
	}
	 if ((id = SifExecModuleBuffer(&ps2mouse_irx, size_ps2mouse_irx, 0, NULL, &ret)) < 0) {
		scr_printf("Error loading ps2mouse\n");
	}
	if ((id = SifExecModuleBuffer(&ps2snd_irx, size_ps2snd_irx, 0, NULL, &ret)) < 0) {
		scr_printf("Error loading ps2snd\n");
	}*/
	ret = SifLoadModule("mass:irx/ps2kbd.irx", 0, NULL);
	if (ret < 0) {
		printf("Failed to load module: PS2KBD");
		//SleepThread();
	}	

	ret = SifLoadModule("mass:irx/ps2mouse.irx", 0, NULL);
	if (ret < 0) {
		printf("Failed to load module: PS2MOUSE");
	//	SleepThread();
	}
}
