###############################################################################
# Makefile for project 
###############################################################################

## General options
PROJECT = 
MCU = STM32F103R6
COMPILER = "Keil for ARM"
TARGET = Debug
SHELL = C:\Windows\system32\cmd.exe

## Tools general options
ASMFLAGS=--cpu=cortex-m3 --thumb -g
CCFLAGS=--cpu=cortex-m3 --thumb -I.. -IC:/Keil/ARM/CMSIS/Include -g -DUSE_STDPERIPH_DRIVER -I../CMSIS -I../CMSIS/startup -DSTM32F10X_LD -I../FWlib/inc -I../USER
LDFLAGS=--scatter ../stm32f103x6.sct

## Processing Tools
CC=armcc.exe
LD=armlink.exe
ASM=armasm.exe
BIN2HEX=fromelf.exe
#  E:/汪伟捷学习资料/stm32与Proteus仿真工程/流水灯工程/STM32F103R6/Debug/Debug.elf: TOOL=LD
#     E:/汪伟捷学习资料/stm32与Proteus仿真工程/流水灯工程/STM32F103R6/Debug/startup_stm32f103.o: TOOL=ASM
#        E:/汪伟捷学习资料/stm32与Proteus仿真工程/流水灯工程/STM32F103R6/startup_stm32f103.s
#     E:/汪伟捷学习资料/stm32与Proteus仿真工程/流水灯工程/STM32F103R6/Debug/main.o: TOOL=CC
#        E:/汪伟捷学习资料/stm32与Proteus仿真工程/流水灯工程/STM32F103R6/USER/main.c
#     E:/汪伟捷学习资料/stm32与Proteus仿真工程/流水灯工程/STM32F103R6/Debug/stm32f10x_gpio.o: TOOL=CC
#        E:/汪伟捷学习资料/stm32与Proteus仿真工程/流水灯工程/STM32F103R6/FWlib/src/stm32f10x_gpio.c
#     E:/汪伟捷学习资料/stm32与Proteus仿真工程/流水灯工程/STM32F103R6/Debug/core_cm3.o: TOOL=CC
#        E:/汪伟捷学习资料/stm32与Proteus仿真工程/流水灯工程/STM32F103R6/CMSIS/core_cm3.c
#     E:/汪伟捷学习资料/stm32与Proteus仿真工程/流水灯工程/STM32F103R6/Debug/system_stm32f10x.o: TOOL=CC
#        E:/汪伟捷学习资料/stm32与Proteus仿真工程/流水灯工程/STM32F103R6/CMSIS/system_stm32f10x.c
#     E:/汪伟捷学习资料/stm32与Proteus仿真工程/流水灯工程/STM32F103R6/Debug/led.o: TOOL=CC
#        E:/汪伟捷学习资料/stm32与Proteus仿真工程/流水灯工程/STM32F103R6/USER/led.c
#     E:/汪伟捷学习资料/stm32与Proteus仿真工程/流水灯工程/STM32F103R6/Debug/stm32f10x_rcc.o: TOOL=CC
#        E:/汪伟捷学习资料/stm32与Proteus仿真工程/流水灯工程/STM32F103R6/FWlib/src/stm32f10x_rcc.c

# Build tree:
all:	Debug

Debug:	Debug.elf

Debug.elf:	startup_stm32f103.o main.o stm32f10x_gpio.o core_cm3.o system_stm32f10x.o led.o stm32f10x_rcc.o 
	$(LD)  $(LDFLAGS)  "startup_stm32f103.o" "main.o" "stm32f10x_gpio.o" "core_cm3.o" "system_stm32f10x.o" "led.o" "stm32f10x_rcc.o" --output "Debug.elf"

startup_stm32f103.o:	../startup_stm32f103.s 
	$(ASM) $(ASMFLAGS)  "../startup_stm32f103.s" -o "startup_stm32f103.o"

main.o:	../USER/main.c 
	$(CC) $(CCFLAGS)  "../USER/main.c" -c -o "main.o"

stm32f10x_gpio.o:	../FWlib/src/stm32f10x_gpio.c 
	$(CC) $(CCFLAGS)  "../FWlib/src/stm32f10x_gpio.c" -c -o "stm32f10x_gpio.o"

core_cm3.o:	../CMSIS/core_cm3.c 
	$(CC) $(CCFLAGS)  "../CMSIS/core_cm3.c" -c -o "core_cm3.o"

system_stm32f10x.o:	../CMSIS/system_stm32f10x.c 
	$(CC) $(CCFLAGS)  "../CMSIS/system_stm32f10x.c" -c -o "system_stm32f10x.o"

led.o:	../USER/led.c 
	$(CC) $(CCFLAGS)  "../USER/led.c" -c -o "led.o"

stm32f10x_rcc.o:	../FWlib/src/stm32f10x_rcc.c 
	$(CC) $(CCFLAGS)  "../FWlib/src/stm32f10x_rcc.c" -c -o "stm32f10x_rcc.o"

# tidy - delete all temporary files which are not involved in the target generation
tidy:

# cleanup - delete all generated files
clean:	tidy
	rm -rf Debug.elf
	rm -rf startup_stm32f103.o
	rm -rf main.o
	rm -rf stm32f10x_gpio.o
	rm -rf core_cm3.o
	rm -rf system_stm32f10x.o
	rm -rf led.o
	rm -rf stm32f10x_rcc.o
