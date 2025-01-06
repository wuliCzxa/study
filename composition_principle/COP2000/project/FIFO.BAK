MOV A, #00H          ; 初始化累加器A为0
MOV R0, A            ; 初始化寄存器R0为0
MOV R1, A            ; 初始化寄存器R1为0
MOV R2, A            ; 初始化寄存器R2为0
MOV R3, A            ; 初始化寄存器R3为0

MOV 0EFH, A          ; 初始化存储位置0EFH为0
MOV 0EEH, A          ; 初始化存储位置0EEH为0
MOV 0EBH, A          ; 初始化存储位置0EBH为0
MOV 0EAH, A          ; 初始化存储位置0EAH为0
MOV 0DFH, A          ; 初始化存储位置0DFH为0
MOV 0DEH, A          ; 初始化存储位置0DEH为0

MAIN:
     MOV R0, #0F0H   ; 设置R0为数据起始地址0F0H

     MOV A, #06H     ; 初始化页面缓冲区大小为4
     MOV 0DDH, A     ; 将页面缓冲区大小存入地址0DDH
     MOV R0, #0F0H   ; 重设R0为数据起始地址
     MOV R3, #0E5H   ; 设置R3为页面缓冲区尾地址
     MOV A, @R0      ; 从地址R0读取数据到A
     MOV @R3, A      ; 将数据从A存入R3指向的位置
     CALL NHIT       ; 调用未命中页面的处理函数
     CALL NULL       ; 调用空页面的处理函数
     CALL TIME       ; 调用时间更新函数
     CALL PAGES      ; 调用页面管理函数

PAGES:
    MOV R0, #0F6H       ; 初始化R0为0F4H地址
    MOV R2, #10         ; 设置循环次数为16，用于处理0F0H到0FFH的地址范围

PAGE_LOOP:
    MOV A, @R0          ; 读取当前R0指向地址的数据
    CALL CHECK_EXISTENCE ; 检查该数据是否已存在页面缓冲区
    JZ NEXT_ADDRESS      ; 如果存在，则跳转到处理下一个地址

    CALL SHIFT_PAGE      ; 如果不存在，调用页面后移逻辑
    MOV 0E0H, A         ; 将当前数据存入页面缓冲区头部0E0H
    MOV A, #00H         ; 清空0E4H用于腾出空间
    MOV 0E6H, A

NEXT_ADDRESS:
    MOV A, R0
    ADD A, #01          ; 增加R0地址，指向下一个数据
    MOV R0, A
    MOV A, R2
    SUB A, #01          ; 减少循环计数器值
    MOV R2, A
    JZ OVER             ; 如果循环结束，跳转到OVER
    JMP PAGE_LOOP        ; 未完成循环时，继续处理下一个地址
    RET

CHECK_EXISTENCE:
    MOV R3, #0E0H       ; 设置R3为页面缓冲区起始地址
    MOV A, #06H         ; 设置检查范围为4（缓冲区大小）
    MOV 0EBH, A

CHECK_LOOP:
    MOV A, @R3          ; 从缓冲区读取数据
    SUB A, @R0          ; 比较当前缓冲区数据与R0指向的数据
    JZ EXIST            ; 如果相等，说明数据已存在
    MOV A, R3
    ADD A, #01H         ; 移动R3到下一个缓冲区地址
    MOV R3, A
    MOV A, 0EBH
    SUB A, #01          ; 检查剩余检查次数
    MOV 0EBH, A
    JZ LOOP             ; 如果检查完成，跳转到LOOP
    JMP CHECK_LOOP       ; 否则继续检查

    ; CLR C标志位逻辑，通过MOV, ADD, SUB实现
    MOV A, #00H         ; 清零A寄存器

    MOV A, 0EEH
    ADD A, #01H
    MOV 0EEH, A
    RET

LOOP:
    CALL SHIFT_PAGE      ; 调用页面后移逻辑
LOOP2:
    MOV A, 0D0H
    MOV 0E0H, A         ; 将当前数据存入缓冲区头部
    MOV A, #00H         ; 清空最后一个缓冲区地址的数据
    MOV 0E6H, A
    CALL NHIT
    CALL NEXT_ADDRESS

EXIST:
    ; SETB C标志位逻辑，通过MOV, ADD实现
    MOV A, #0FFH
    ADD A, #01H          ; 通过加法设置进位标志
    MOV A, 0EFH
    ADD A, #01H
    MOV 0EFH, A
    JMP NEXT_ADDRESS
    RET

SHIFT_PAGE:
    MOV R3, #0E5H       ; 设置后移起点为缓冲区尾部地址0E3H
    MOV A, #06H         ; 设置后移次数为缓冲区大小4
    MOV 0EAH, A
    MOV A, @R0
    MOV 0D0H, A
    CALL SHIFT_LOOP

SHIFT_LOOP:
    MOV A, @R3
    MOV R1, A
    MOV A, R3
    ADD A, #01H
    MOV R3, A
    MOV A, R1
    MOV @R3, A
    MOV A, R3
    SUB A, #02H
    MOV R3, A
    MOV A, 0EAH
    SUB A, #01H
    MOV 0EAH, A
    JZ LOOP2
    JMP SHIFT_LOOP       ; 完成后移逻辑
    RET

TIME:
     CALL PAGES          ; 时间管理逻辑调用页面管理

NHIT:
     MOV A, 0EEH
     ADD A, #01H         ; 更新未命中计数
     MOV 0EEH, A
     RET

NULL:
     MOV A, 0DDH
     SUB A, #01H         ; 更新空页面计数
     MOV 0DDH, A
     OUT
     JZ TIME             ; 如果空页面耗尽，调用时间更新
     MOV A, R0
     ADD A, #01H         ; 地址递增
     MOV R0, A
     MOV A, R3
     SUB A, #01H         ; 地址递减
     MOV R3, A
     MOV A, @R0
     MOV @R3, A
     CALL NHIT           ; 处理页面未命中逻辑
     JMP NULL

HIT:
     MOV A, 0EFH
     ADD A, #01H         ; 更新命中计数
     MOV 0EFH, A
     CALL PAGES          ; 页面更新逻辑

OVER:
     JMP OVER            ; 程序结束

