###############################################################################
# Makefile for project 
###############################################################################

## General options
PROJECT = 
MCU = AT89C51
COMPILER = "Keil for 8051"
TARGET = Release
SHELL = C:\Windows\system32\cmd.exe

## Tools general options
CCFLAGS=ROM(SMALL)

## Processing Tools
CC=C51.exe
LD=BL51.exe
ASM=A51.exe
#  G:/实验箱刻盘资料存档/F型箱/微控制器EDA仿真实验系统（F型）V8/MCU-51-AT89S52-V8/PROJECT/Project_C/EXP4_计数器实验/P_PROTEUS/AT89C51/Release/Release.hex: TOOL=LD
#  Post: "C:\Keil\C51\OH51.exe" ".\Release.OMF"
#     G:/实验箱刻盘资料存档/F型箱/微控制器EDA仿真实验系统（F型）V8/MCU-51-AT89S52-V8/PROJECT/Project_C/EXP4_计数器实验/P_PROTEUS/AT89C51/Release/main.OBJ: TOOL=CC
#        G:/实验箱刻盘资料存档/F型箱/微控制器EDA仿真实验系统（F型）V8/MCU-51-AT89S52-V8/PROJECT/Project_C/EXP4_计数器实验/P_PROTEUS/AT89C51/main.c

# Build tree:
all:	Release

Release:	Release.hex
	cp "Release.hex" "G:/实验箱刻盘资料存档/F型箱/微控制器EDA仿真实验系统（F型）V8/MCU-51-AT89S52-V8/PROJECT/Project_C/EXP4_计数器实验/P_PROTEUS/AT89C51.hex"

Release.hex:	main.OBJ 
	"C:\Program Files\Labcenter Electronics\Proteus 8 Professional\Tools\MAKE\RunTool.exe" --good-exits=0,1 --executable=$(LD) "main.OBJ" TO ".\Release.OMF" $(LDFLAGS) 
	"C:\Keil\C51\OH51.exe" ".\Release.OMF"

main.OBJ:	../main.c 
	"C:\Program Files\Labcenter Electronics\Proteus 8 Professional\Tools\MAKE\RunTool.exe" --good-exits=0,1 --executable=$(CC) "..\main.c" $(CCFLAGS)  OBJECT("main.OBJ")

# tidy - delete all temporary files which are not involved in the target generation
tidy:
	rm -rf Release.M51
	rm -rf ./main.lst
	rm -rf ./main.I

# cleanup - delete all generated files
clean:	tidy
	rm -rf Release.hex
	rm -rf main.OBJ
