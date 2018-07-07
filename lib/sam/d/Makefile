##
## This file is part of the libopencm3 project.
##
## Copyright (C) 2016 Karl Palsson <karlp@tweak.net.au>
## Copyright (C) 2018 Jason Kotzin <jasonkotzin@gmail.com>
##
## This library is free software: you can redistribute it and/or modify
## it under the terms of the GNU Lesser General Public License as published by
## the Free Software Foundation, either version 3 of the License, or
## (at your option) any later version.
##
## This library is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU Lesser General Public License for more details.
##
## You should have received a copy of the GNU Lesser General Public License
## along with this library.  If not, see <http://www.gnu.org/licenses/>.
##

LIBNAME		= libopencm3_samd
SRCLIBDIR	?= ../..

PREFIX		?= arm-none-eabi

CC		= $(PREFIX)-gcc
AR		= $(PREFIX)-ar
TGT_CFLAGS	= -Os -Wall -Wextra -I../../../include -fno-common \
		  -mcpu=cortex-m0plus -mthumb $(FP_FLAGS) -Wstrict-prototypes \
		  -ffunction-sections -fdata-sections -MD -DSAMD
# Use CPU IOBUS for direct GPIO access which is single cycle access
TGT_CFLAGS	+= -DSAMD_HIGHSPEED_IO
TGT_CFLAGS      += $(DEBUG_FLAGS)
#TGT_CFLAGS	+= $(STANDARD_FLAGS)
# ARFLAGS	= rcsv
ARFLAGS		= rcs
OBJS		= gclk.o nvmctrl.o gpio.o usb_samdx.o
OBJS		+= usb.o usb_control.o usb_standard.o

VPATH += ../../usb:../:../../cm3:../common

include ../../Makefile.include

