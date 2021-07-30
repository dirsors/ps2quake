#ifndef _PS2_H_
#define _PS2_H_
#include <stdio.h> 
#include <tamtypes.h>
#include <sifrpc.h>
#include <kernel.h>
#include <loadfile.h>
#include <fileio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <debug.h>
#include <iopcontrol.h>
#include <sbv_patches.h>
#include <iopheap.h>

void wyjscie();
void IOP_reset();
int exit1();
void LoadElf(const char *elf, char* path);
void loadmodules();
void CleanUp();
void Reset_IOP();
#endif
