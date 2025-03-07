Overview
========
The mpi_loader_dsp_hello_world example demonstrates how to add secondary bootloader in CM33 application
to load the multicore packed image, created by elftosb tool. Once the DSP image is loaded, CM33 can
kick off it and both cores then execute in parallel.


Toolchain supported
===================
- MCUXpresso  11.8.0
- Keil MDK  5.38.1
- IAR embedded Workbench  9.40.1
- GCC ARM Embedded  12.2

Hardware requirements
=====================
- Micro USB cable
- MIMXRT685-AUD-EVK board
- Personal Computer

Board settings
==============


Prepare the Demo
================
1. Connect a micro USB cable between the PC host and the CMSIS DAP USB port (J5) on the board
2. DSP example boards\mimxrt685audevk\dsp_examples\hello_world_usart\hifi4 should be built and we can then
   get dsp_text_release.bin and dsp_data_release.bin.
3. Build this example, and we can get sdk20-app.bin.
4. Copy all those binaries to elf2sb workspace "input_images" folder identified in mpi_loader_<type>.json.
5. Copy mpi_loader_<type>.json to elf2sb workspace "image_config" folder.
6. Execute elf2sb.exe with mpi_loader_<type>.json as parameter. E.g., (only 1 command needed depending on your build)
   - elftosb.exe -V -d -f rt6xx -J workspace/image_config/mpi_loader_ram.json (for debug/release built sdk20-app.bin)
   - elftosb.exe -V -d -f rt6xx -J workspace/image_config/mpi_loader_flash.json (for flash_debug/flash_release built sdk20-app.bin)
7. In elf2sb workspace "output_images" folder, you can get the multicore packed image mpi_loader_dsp_hello_world.bin.
8. Write flash config block to flash offset 0x400 (address 0x08000400) and write mpi_loader_dsp_hello_world.bin to
   flash offset 0x1000 (address 0x08001000). E.g. with blhost tool, set board boot mode DIP to Serial ISP
   [high, high, low](SW5 1-ON, 2-OFF, 3-OFF), reset the board and use command window to communicate with it (need to identify the COM port).
   - blhost.exe -p COM29 -- fill-memory 0x1c000 4 0xc1000001
   - blhost.exe -p COM29 -- fill-memory 0x1c004 4 0x20000006
   - blhost.exe -p COM29 -- configure-memory 9 0x1c000
   - blhost.exe -p COM29 -t 100000 -- flash-erase-region 0x08000000 0x100000
   - blhost.exe -p COM29 -- fill-memory 0x1d000 4 0xf000000f
   - blhost.exe -p COM29 -- configure-memory 0x9 0x1d000
   - blhost.exe -p COM29 -- write-memory 0x08001000 workspace\output_images\mpi_loader_dsp_hello_world.bin
9. Set board boot mode to flash portb boot [low, high, low](SW5 1-ON, 2-OFF, 3-ON).
10. Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
11. Reset the board and you will see the result.


Running the demo CM33
=====================
When the demo runs successfully, the terminal will display the following:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Hello World running on core 'Cortex-M33'

Hello World running on DSP core 'nxp_rt600_RI2020_5_newlib'
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

