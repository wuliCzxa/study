###############################################################################
# Makefile for project 
###############################################################################

## General options
PROJECT = 
MCU = AT89C52
COMPILER = "Keil for 8051"
TARGET = Release
SHELL = C:\Windows\system32\cmd.exe

## Tools general options
CCFLAGS=ROM(SMALL)

## Processing Tools
CC=C51.exe
LD=BL51.exe
ASM=A51.exe
#  C:/Users/Administrator/Desktop/Proteus/Project_C/EXP1_IO开关量输入实验/P_PROTEUS/AT89C52/Release/Release.hex: TOOL=LD
#  Post: "..\..\..\..\..\..\..\..\..\Keil\C51\BIN\OH51.exe" ".\Release.OMF"
#     C:/Users/Administrator/Desktop/Proteus/Project_C/EXP1_IO开关量输入实验/P_PROTEUS/AT89C52/Release/main.OBJ: TOOL=CC
#        C:/Users/Administrator/Desktop/Proteus/Project_C/EXP1_IO开关量输入实验/P_PROTEUS/AT89C52/main.c

# Build tree:
all:	Release

Release:	Release.hex
	cp "Release.hex" "C:/Users/Administrator/Desktop/Proteus/Project_C/EXP1_IO开关量输入实验/P_PROTEUS/AT89C52.hex"

Release.hex:	main.OBJ 
	"..\..\..\..\..\..\..\..\..\Program Files\Labcenter Electronics\Proteus 8 Professional\Tools\MAKE\RunTool.exe" --good-exits=0,1 --executable=$(LD) "main.OBJ" TO ".\Release.OMF" $(LDFLAGS) 
	"..\..\..\..\..\..\..\..\..\Keil\C51\BIN\OH51.exe" ".\Release.OMF"

main.OBJ:	../main.c 
	"..\..\..\..\..\..\..\..\..\Program Files\Labcenter Electronics\Proteus 8 Professional\Tools\MAKE\RunTool.exe" --good-exits=0,1 --executable=$(CC) "..\main.c" $(CCFLAGS)  OBJECT("main.OBJ")

# tidy - delete all temporary files which are not involved in the target generation
tidy:
	rm -rf Release.M51
	rm -rf ./main.lst
	rm -rf ./main.I

# cleanup - delete all generated files
clean:	tidy
	rm -rf Release.hex
	rm -rf main.OBJ
