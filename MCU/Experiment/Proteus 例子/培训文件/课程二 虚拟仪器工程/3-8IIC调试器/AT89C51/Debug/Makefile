###############################################################################
# Makefile for project 
###############################################################################

## General options
PROJECT = 
MCU = AT89C51
COMPILER = "Keil for 8051"
TARGET = Debug
SHELL = C:\Windows\system32\cmd.exe

## Tools general options
CCFLAGS=ROM(SMALL) BROWSE DEBUG CODE OBJECTEXTEND PREPRINT

## Processing Tools
CC=C51.exe
LD=BL51.exe
ASM=A51.exe
#  G:/实验箱刻盘资料存档/E型箱/单片机实验系统E型(FB-EDU-P51E)/单片机实验系统E型(FB-EDU-P51E)-V8/51单片机工程/C代码工程/18(I2C实验)/AT89C51/Debug/Debug.OMF: TOOL=LD
#     G:/实验箱刻盘资料存档/E型箱/单片机实验系统E型(FB-EDU-P51E)/单片机实验系统E型(FB-EDU-P51E)-V8/51单片机工程/C代码工程/18(I2C实验)/AT89C51/Debug/I2C.OBJ: TOOL=CC
#        G:/实验箱刻盘资料存档/E型箱/单片机实验系统E型(FB-EDU-P51E)/单片机实验系统E型(FB-EDU-P51E)-V8/51单片机工程/C代码工程/18(I2C实验)/AT89C51/I2C.C

# Build tree:
all:	Debug

Debug:	Debug.OMF

Debug.OMF:	I2C.OBJ 
	"C:\Program Files\Labcenter Electronics\Proteus 8 Professional\Tools\MAKE\RunTool.exe" --good-exits=0,1 --executable=$(LD) "I2C.OBJ" TO "Debug.OMF" $(LDFLAGS) 

I2C.OBJ:	../I2C.C 
	"C:\Program Files\Labcenter Electronics\Proteus 8 Professional\Tools\MAKE\RunTool.exe" --good-exits=0,1 --executable=$(CC) "..\I2C.C" $(CCFLAGS)  OBJECT("I2C.OBJ")

# tidy - delete all temporary files which are not involved in the target generation
tidy:
	rm -rf Debug.M51
	rm -rf ./I2C.lst
	rm -rf ./I2C.I

# cleanup - delete all generated files
clean:	tidy
	rm -rf Debug.OMF
	rm -rf I2C.OBJ
