#u-boot for STM32F103 series
---
##Goals:
Run u-boot on STM32F103ZET6, with S71GL032NA0 (2MB PSRAM and 4MB nor flash) and NAND512-A(64MB nand flash).

##Status:
Added ST STD-lib, and modified a bit to fit with the original library.<br>
Wrapped some functions to fit the common parts.<br>
Add `stm32f103zet6_config` and add my files to Makefile's.<br>
Environment variables are saved to embedded-flash, 128k offset.
2MB SRAM drived by FSMC started from 0x68000000.<br>
64MB NAND flash drived by FSMC, with uboot nand commands.<br>

##Shortcommings
Currently NO NOR flash support...<br>
Using software ECC for NAND, not hardware ECC from FSMC...<br>
Under developing...

##Previous README...
This is the source tree of U-Boot, a popular boot loader for embedded devices. U-Boot was developed by DENX Software Engineering (www.denx.de).

This particular version of U-Boot is maintained by Emcraft Systems and supports several Cortex-M3 and Cortex-M4 based MCUs.

A complete list of Cortex-M MCUs and boards supported by this version of U-Boot is available at http://www.emcraft.com.

If you want to contribute code to this source tree, please email your patches to a2f-linux-support@emcraft.com.


