###############################################################################
# Makefile for project 
###############################################################################

## General options
PROJECT = 
MCU = AT89C52
COMPILER = "Keil for 8051"
TARGET = Debug
SHELL = C:\Windows\system32\cmd.exe

## Tools general options
CCFLAGS=ROM(SMALL) BROWSE DEBUG CODE OBJECTEXTEND PREPRINT

## Processing Tools
CC=C51.exe
LD=BL51.exe
ASM=A51.exe
#  F:/微控制器仿真实验实训系统(FB-EDU-MCU-F)/微控制器仿真实验实训系统(FB-EDU-MCU-F)-V8-V300/MCU-51-AT89S52-V8.1/PROJECT/Project_C/EXP1_IO开关量输入实验/P_PROTEUS/AT89C52/Debug/Debug.OMF: TOOL=LD
#     F:/微控制器仿真实验实训系统(FB-EDU-MCU-F)/微控制器仿真实验实训系统(FB-EDU-MCU-F)-V8-V300/MCU-51-AT89S52-V8.1/PROJECT/Project_C/EXP1_IO开关量输入实验/P_PROTEUS/AT89C52/Debug/main.OBJ: TOOL=CC
#        F:/微控制器仿真实验实训系统(FB-EDU-MCU-F)/微控制器仿真实验实训系统(FB-EDU-MCU-F)-V8-V300/MCU-51-AT89S52-V8.1/PROJECT/Project_C/EXP1_IO开关量输入实验/P_PROTEUS/AT89C52/main.c

# Build tree:
all:	Debug

Debug:	Debug.OMF

Debug.OMF:	main.OBJ 
	"C:\Program Files (x86)\Labcenter Electronics\Proteus 8 Professional\Tools\MAKE\RunTool.exe" --good-exits=0,1 --executable=$(LD) "main.OBJ" TO "Debug.OMF" $(LDFLAGS) 

main.OBJ:	../main.c 
	"C:\Program Files (x86)\Labcenter Electronics\Proteus 8 Professional\Tools\MAKE\RunTool.exe" --good-exits=0,1 --executable=$(CC) "..\main.c" $(CCFLAGS)  OBJECT("main.OBJ")

# tidy - delete all temporary files which are not involved in the target generation
tidy:
	rm -rf Debug.M51
	rm -rf ./main.lst
	rm -rf ./main.I

# cleanup - delete all generated files
clean:	tidy
	rm -rf Debug.OMF
	rm -rf main.OBJ
