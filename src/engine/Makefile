# The following Makefile will build Jam on Unix systems
# You can also modify it to compile the program on other
# systems, or also use one of the specific Makefiles
# located in the "builds" directory
#

CC = cc
TARGET = -o jam0
CFLAGS =

# Borland C++ on Windows
#CC = bcc32
#TARGET = -ejam0
#CFLAGS = /DNT -w- -q

# Special flavors - uncomment appropriate lines

# NCR seems to have a broken readdir() -- use gnu
#CC = gcc

# AIX needs -lbsd, and has no identifying cpp symbol
# Use _AIX41 if you're not on 3.2 anymore.
#LINKLIBS = -lbsd
#CFLAGS = -D_AIX

# NT (with Microsoft compiler)
# Use FATFS if building on a DOS FAT file system
#Lib = $(MSVCNT)/lib
#Include = $(MSVCNT)/include
#CC = cl /nologo
#CFLAGS = -I $(Include) -DNT 
#TARGET = /Fejam0
#LINKLIBS = $(Lib)/oldnames.lib $(Lib)/kernel32.lib $(Lib)/libc.lib

# BeOS - Metroworks CodeWarrior
#CC = mwcc
#Include = /NewDisk/develop/headers/posix
#CFLAGS = -I $(Include)

# BeOS - gcc
#CC = gcc
#LINKLIBS = -lnet

# Interix - gcc
#CC = gcc

# Cygwin - gcc & cygwin
#CC = gcc
#CFLAGS = -D__cygwin__

# MingW - gcc on Win32
#
#CC = gcc
#CFLAGS = -DMINGW

# MPEIX
#CC = gcc
#CFLAGS = -I/usr/include -D_POSIX_SOURCE

# QNX rtp (neutrino)
#CC = gcc


#
#SOURCES = \
#	command.c compile.c execnt.c execunix.c execvms.c expand.c \
#	filent.c fileos2.c fileunix.c filevms.c glob.c hash.c \
#	hdrmacro.c headers.c jam.c jambase.c jamgram.c lists.c make.c make1.c \
#	newstr.c option.c parse.c pathunix.c pathvms.c regexp.c \
#	rules.c scan.c search.c subst.c timestamp.c variable.c

# for Unix systems
#
# we need to ensure that "jambase.c" has write permissions, since it is
# going to be re-generated from "Jambase".
#
all: jam0
	chmod a+w jambase.c
	./jam0 -sJAMBASE= -sBOOST_ROOT= -sBOOST_BUILD_PATH=

# for other systems
#all: jam0
#	jam0

include common.mk

#jam0:
#	$(CC) $(TARGET) $(CFLAGS) $(SOURCES) $(LINKLIBS)
