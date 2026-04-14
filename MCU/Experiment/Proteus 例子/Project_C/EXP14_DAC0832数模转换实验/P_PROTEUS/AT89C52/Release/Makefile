###############################################################################
# Makefile for project 
###############################################################################

## General options
PROJECT = 
MCU = AT89C52
COMPILER = "Keil for 8051"
TARGET = Release
SHELL = C:\WINDOWS\system32\cmd.exe

## Tools general options
CCFLAGS=ROM(SMALL)

## Processing Tools
CC=C51.exe
LD=BL51.exe
ASM=A51.exe
#  C:/Documents and Settings/wangdl/桌面/EXP14_DAC0832数模转换实验/P_PROTEUS/AT89C52/Release/Release.hex: TOOL=LD
#  Post: "..\..\..\..\..\..\..\Keil\C51\OH51.exe" ".\Release.OMF"
#     C:/Documents and Settings/wangdl/桌面/EXP14_DAC0832数模转换实验/P_PROTEUS/AT89C52/Release/main.OBJ: TOOL=CC
#        C:/Documents and Settings/wangdl/桌面/EXP14_DAC0832数模转换实验/P_PROTEUS/AT89C52/main.c

# Build tree:
all:	Release

Release:	Release.hex
	cp "Release.hex" "C:/Documents and Settings/wangdl/桌面/EXP14_DAC0832数模转换实验/P_PROTEUS/AT89C52.hex"

Release.hex:	main.OBJ 
	"..\..\..\..\..\..\..\Program Files\Labcenter Electronics\Proteus 8 Professional\Tools\MAKE\RunTool.exe" --good-exits=0,1 --executable=$(LD) "main.OBJ" TO ".\Release.OMF" $(LDFLAGS) 
	"..\..\..\..\..\..\..\Keil\C51\OH51.exe" ".\Release.OMF"

main.OBJ:	../main.c 
	"..\..\..\..\..\..\..\Program Files\Labcenter Electronics\Proteus 8 Professional\Tools\MAKE\RunTool.exe" --good-exits=0,1 --executable=$(CC) "..\main.c" $(CCFLAGS)  OBJECT("main.OBJ")

# tidy - delete all temporary files which are not involved in the target generation
tidy:
	rm -rf Release.M51
	rm -rf ./main.lst
	rm -rf ./main.I

# cleanup - delete all generated files
clean:	tidy
	rm -rf Release.hex
	rm -rf main.OBJ
