###############################################################################
# Makefile for project 
###############################################################################

## General options
PROJECT = 
MCU = 80C51
COMPILER = "Keil for 8051"
TARGET = Release
SHELL = C:\Windows\system32\cmd.exe

## Tools general options
CCFLAGS=ROM(SMALL)

## Processing Tools
CC=C51.exe
LD=BL51.exe
ASM=A51.exe
#  F:/微控制器仿真实验实训系统(FB-EDU-MCU-F)/微控制器仿真实验实训系统(FB-EDU-MCU-F)-V8-V300/MCU-51-AT89S52-V8.1/PROJECT/Project_C/EXP16_PC机串口通讯实验/P_PROTEUS/80C51/Release/Release.hex: TOOL=LD
#  Post: "C:\Keil\C51\BIN\OH51.exe" ".\Release.OMF"
#     F:/微控制器仿真实验实训系统(FB-EDU-MCU-F)/微控制器仿真实验实训系统(FB-EDU-MCU-F)-V8-V300/MCU-51-AT89S52-V8.1/PROJECT/Project_C/EXP16_PC机串口通讯实验/P_PROTEUS/80C51/Release/main.OBJ: TOOL=CC
#        F:/微控制器仿真实验实训系统(FB-EDU-MCU-F)/微控制器仿真实验实训系统(FB-EDU-MCU-F)-V8-V300/MCU-51-AT89S52-V8.1/PROJECT/Project_C/EXP16_PC机串口通讯实验/P_PROTEUS/80C51/main.c

# Build tree:
all:	Release

Release:	Release.hex
	cp "Release.hex" "F:/微控制器仿真实验实训系统(FB-EDU-MCU-F)/微控制器仿真实验实训系统(FB-EDU-MCU-F)-V8-V300/MCU-51-AT89S52-V8.1/PROJECT/Project_C/EXP16_PC机串口通讯实验/P_PROTEUS/80C51.hex"

Release.hex:	main.OBJ 
	"C:\Program Files (x86)\Labcenter Electronics\Proteus 8 Professional\Tools\MAKE\RunTool.exe" --good-exits=0,1 --executable=$(LD) "main.OBJ" TO ".\Release.OMF" $(LDFLAGS) 
	"C:\Keil\C51\BIN\OH51.exe" ".\Release.OMF"

main.OBJ:	../main.c 
	"C:\Program Files (x86)\Labcenter Electronics\Proteus 8 Professional\Tools\MAKE\RunTool.exe" --good-exits=0,1 --executable=$(CC) "..\main.c" $(CCFLAGS)  OBJECT("main.OBJ")

# tidy - delete all temporary files which are not involved in the target generation
tidy:
	rm -rf Release.M51
	rm -rf ./main.lst
	rm -rf ./main.I

# cleanup - delete all generated files
clean:	tidy
	rm -rf Release.hex
	rm -rf main.OBJ
