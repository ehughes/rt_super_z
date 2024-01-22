# SuperMonkey - Software 

This repo has software for the SuperMonkey.     

### Build Tools :

- NXP MCUXpresso 10.4 IDE (or newer)

- i.MXRT685 SDK 2.13 Should be baked into the projects.   

### Debug tools

The SuperMonkey has a standard ARM SWD connector.

I like to use J-Link + Ozone for debug.  MCUXpresso can also program/debug.   

Low cost debuggers:

- MCU Link ($10) 
- J-Link EDU  (@$20 for non-commercial use)

## Projects

### SRC/IO_Toggle

Test project that toggles a handful of IO.   Toggle P39 and the LCD Backlight at 250mSec

### SRC/SuperMonkeyGFX

A project to test the 1.54" Display.   It currently will put a monkey on the screen (slowly).  There is an issue where the GFX functions will work after programming w/ the debug tools but will not start from a cold boot.   The processor is booting correctly as my IO toggle function still works but there is something wrong with LCD initialization.   To be debugged...