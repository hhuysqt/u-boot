/*
 * (C) Copyright 2010,2011
 * Vladimir Khusainov, Emcraft Systems, vlad@emcraft.com
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 *
 * This file is NOT the real link script, but is used to create it!
 * We can cat this file to gcc and generate the REAL link script...
 * arm-none-eabi-gcc -E \
 *    -DCONFIG_MEM_NVM_BASE="0x00000000" -DCONFIG_MEM_NVM_LEN="(1024 * 1024 * 2)" \
 *    -DCONFIG_MEM_RAM_BASE="0x20000000" -DCONFIG_MEM_RAM_LEN="(20 * 1024)" \
 *    (other defines and parameters) \
 *    <cpu/arm_cortexm3/u-boot.lds >u-boot.lds
 */

OUTPUT_FORMAT("elf32-littlearm", "elf32-bigarm",
	      "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(_start)

#define NVM_BASE	(CONFIG_MEM_NVM_BASE + CONFIG_MEM_NVM_UBOOT_OFF)
#define NVM_LEN		(CONFIG_MEM_NVM_LEN - CONFIG_MEM_NVM_UBOOT_OFF)
#define RAM_BUF_BASE	(CONFIG_MEM_RAM_BASE + CONFIG_MEM_RAM_LEN)
#define MALLOC_BASE	(RAM_BUF_BASE + CONFIG_MEM_RAM_BUF_LEN)
#define STACK_BASE	(MALLOC_BASE + CONFIG_MEM_MALLOC_LEN)

MEMORY
{
	NVM (r):	ORIGIN = NVM_BASE, \
			LENGTH = NVM_LEN
	RAM (rw):	ORIGIN = CONFIG_MEM_RAM_BASE, \
			LENGTH = CONFIG_MEM_RAM_LEN
	RAM_BUF (r):	ORIGIN = RAM_BUF_BASE, \
			LENGTH = CONFIG_MEM_RAM_BUF_LEN
	MALLOC (r):	ORIGIN = MALLOC_BASE, \
			LENGTH = CONFIG_MEM_MALLOC_LEN
	STACK (r):	ORIGIN = STACK_BASE, \
			LENGTH = CONFIG_MEM_STACK_LEN
#if defined(CONFIG_MEM_RAMCODE_BASE) && defined(CONFIG_MEM_RAMCODE_LEN)
	RAMCODE (rw):	ORIGIN = CONFIG_MEM_RAMCODE_BASE, \
			LENGTH = CONFIG_MEM_RAMCODE_LEN
#endif
}

SECTIONS
{
	.vectors NVM_BASE :
	{
		*(.vectors)
	} >NVM

	/*
	 * This section is only necessary for Freescale Kinetis. For other MCUs,
	 * this section will be empty and the linker will drop it.
	 */
	.kinetis_flash_conf 0x00000400 :
	{
		KEEP(*(.kinetis_flash_conf))
	} >NVM

	/*
	 * Store contents of these two sections in the beginning of the U-Boot
	 * image to ensure they are loaded by the Boot ROM of the LPC4350 MCU.
	 */
	.lpc18xx_image_top :
	{
		*(.lpc18xx_image_top_text)
		*(.lpc18xx_image_top_data)
	} >NVM

	.text :
	{
		*(.text)
		*(.text*)
		*(.rodata*)

	} >NVM

	.data :
	{
		_data_start = .;
		_data_lma_start = LOADADDR(.data);
		*(.data)
		. = ALIGN(4);
#if ! (defined(CONFIG_MEM_RAMCODE_BASE) && defined(CONFIG_MEM_RAMCODE_LEN))
		*(.ramcode)
#endif
		_data_end = .;
	} >RAM AT>NVM

#if defined(CONFIG_MEM_RAMCODE_BASE) && defined(CONFIG_MEM_RAMCODE_LEN)
	.ramcode :
	{
		_ramcode_start = .;
		_ramcode_lma_start = LOADADDR(.ramcode);
		*(.ramcode)
		_ramcode_end = .;
	} >RAMCODE AT>NVM
#endif

	.u_boot_cmd :
	{
		. = ALIGN(4);
		__u_boot_cmd_start = .;
		*(.u_boot_cmd)
		__u_boot_cmd_end = .;
	} >NVM

	.bss :
	{
		_bss_start = .;
		*(.bss)
		*(COMMON)
		_bss_end = .;
	} >RAM

	.stack :
	{
		*(.stack)
	} >STACK

	/DISCARD/ :
	{
		*(*)
	}
}

/*
 * Export memory addresses
 */
_mem_nvm_base		= CONFIG_MEM_NVM_BASE;
_mem_nvm_size		= CONFIG_MEM_NVM_LEN;
_mem_ram_base		= CONFIG_MEM_RAM_BASE;
_mem_ram_size		= CONFIG_MEM_RAM_LEN;
_mem_ram_buf_base	= RAM_BUF_BASE;
_mem_ram_buf_size	= CONFIG_MEM_RAM_BUF_LEN;
_mem_malloc_base	= MALLOC_BASE;
_mem_malloc_size	= CONFIG_MEM_MALLOC_LEN;
_mem_stack_base		= STACK_BASE;
_mem_stack_size		= CONFIG_MEM_STACK_LEN;
_mem_stack_end		= STACK_BASE + CONFIG_MEM_STACK_LEN;
