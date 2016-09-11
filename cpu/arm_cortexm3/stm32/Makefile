#
# (C) Copyright 2000-2003
# Wolfgang Denk, DENX Software Engineering, wd@denx.de.
#
# (C) Copyright 2011
# Port to STM32
# Yuri Tikhonov, Emcraft Systems, yur@emcraft.com.
#
# See file CREDITS for list of people who contributed to this
# project.
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License as
# published by the Free Software Foundation; either version 2 of
# the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston,
# MA 02111-1307 USA
#

include $(TOPDIR)/config.mk

LIB	= $(obj)lib$(SOC).a

COBJS	:= cpu.o wdt.o soc.o 

ifeq ($(CONFIG_SYS_STM32F103ZET6),y)
# Add stm32f103 configs
COBJS	+= clock_stm32f10x.o system_stm32f10x.o envm_stm32f10x.o stm32f10x_flash.o stm32f10x_fsmc.o stm32f10x_rcc.o
else
# Default is for stm32f2, f4 devices
COBJS	+= clock.o envm.o fsmc.o
endif

ifeq ($(CONFIG_CMD_BUFCOPY),y)
COBJS	+= cmd_bufcopy.o
endif
SOBJS	:=

SRCS	:= $(COBJS:.o=.c)
OBJS	:= $(addprefix $(obj),$(COBJS))
SOBJS	:= $(addprefix $(obj),$(SOBJS))

$(LIB):	$(obj).depend $(OBJS) $(SOBJS)
	$(AR) $(ARFLAGS) $@ $(OBJS)

clean:
	rm -f $(SOBJS) $(OBJS)

distclean:
	rm -f $(LIB) core *.bak $(obj).depend

#########################################################################

# defines $(obj).depend target
include $(SRCTREE)/rules.mk

sinclude $(obj).depend

#########################################################################
