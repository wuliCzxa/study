MOV A, #00H          ; 初始化累加器A为0
MOV R0, A            ; 初始化寄存器R0为0
MOV R1, A            ; 初始化寄存器R1为0
MOV R2, A            ; 初始化寄存器R2为0
MOV R3, A            ; 初始化寄存器R3为0

MOV 0EFH, A          ; 初始化存储位置0EFH为0（命中计数器）
MOV 0EEH, A          ; 初始化存储位置0EEH为0（未命中计数器）
MOV 0EBH, A          ; 初始化存储位置0EBH为0（检查范围计数器）
MOV 0EAH, A          ; 初始化存储位置0EAH为0（后移计数器）
MOV 0DFH, A          ; 初始化存储位置0DFH为0（备用计数器）
MOV 0DEH, A          ; 初始化存储位置0DEH为0（备用计数器）

MAIN:
     MOV R0, #0F0H   ; 设置R0为数据起始地址0F0H

     MOV A, #04H     ; 初始化页面缓冲区大小为4
     MOV 0DDH, A     ; 将页面缓冲区大小存入地址0DDH
     MOV R0, #0F0H   ; 重设R0为数据起始地址
     MOV R3, #0E3H   ; 设置R3为页面缓冲区尾地址
     MOV A, @R0      ; 从地址R0读取数据到A
     MOV @R3, A      ; 将数据从A存入R3指向的位置
     CALL NHIT       ; 调用未命中页面的处理函数
     CALL NULL       ; 调用空页面的处理函数
     CALL TIME       ; 调用时间更新函数
     CALL PAGES      ; 调用页面管理函数

PAGES:
    MOV R0, #0F4H       ; 初始化R0为0F4H地址
    MOV R2, #12         ; 设置循环次数为12，用于处理0F4H到0FFH的地址范围

PAGE_LOOP:
    MOV A, @R0          ; 读取当前R0指向地址的数据
    CALL CHECK_EXISTENCE ; 检查该数据是否已存在页面缓冲区
    JZ NEXT_ADDRESS      ; 如果存在，则跳转到处理下一个地址

    CALL SHIFT_PAGE      ; 如果不存在，调用页面后移逻辑
    MOV 0E0H, A         ; 将当前数据存入页面缓冲区头部0E0H
    MOV A, #00H         ; 清空0E4H用于腾出空间
    MOV 0E4H, A

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
    MOV A, #04H         ; 设置检查范围为4（缓冲区大小）
    MOV 0EBH, A         ; 初始化检查计数器为4

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
        ; 清零操作逻辑，用于保证标志位未设置时的正确性
    MOV A, #00H         ; 清零累加器A

    MOV A, 0EEH         ; 读取未命中计数器的当前值
    ADD A, #01H         ; 将未命中计数器值加1
    MOV 0EEH, A         ; 更新未命中计数器值
    RET                 ; 返回主程序

LOOP:
    CALL SHIFT_PAGE      ; 调用页面后移逻辑
LOOP2:
    MOV A, 0D0H         ; 从地址0D0H读取数据
    MOV 0E0H, A         ; 将读取的数据存入缓冲区头部0E0H
    MOV A, #00H         ; 清空最后一个缓冲区地址的数据
    MOV 0E4H, A         ; 清零缓冲区尾部数据
    CALL NHIT           ; 调用未命中页面的处理逻辑
    CALL NEXT_ADDRESS   ; 跳转处理下一个地址

EXIST:
    ; 设置命中标志位逻辑，确保存在的数据正确计数
    MOV A, #0FFH        ; 将A设置为全1，用于标志设置
    ADD A, #01H         ; 通过加法设置进位标志
    MOV A, 0EFH         ; 读取命中计数器的当前值
    ADD A, #01H         ; 将命中计数器值加1
    MOV 0EFH, A         ; 更新命中计数器值
    JMP NEXT_ADDRESS    ; 跳转到处理下一个地址逻辑
    RET                 ; 返回主程序

SHIFT_PAGE:
    MOV R3, #0E3H       ; 设置后移起点为缓冲区尾部地址0E3H
    MOV A, #04H         ; 设置后移次数为缓冲区大小4
    MOV 0EAH, A         ; 将后移次数存入计数器0EAH
    MOV A, @R0          ; 从数据起始地址读取当前数据
    MOV 0D0H, A         ; 临时保存当前数据到0D0H
    CALL SHIFT_LOOP     ; 调用页面后移循环逻辑

SHIFT_LOOP:
    MOV A, @R3          ; 从当前后移位置读取数据
    MOV R1, A           ; 将读取的数据暂存到寄存器R1
    MOV A, R3           ; 取当前后移地址
    ADD A, #01H         ; 地址加1，准备移入下一个位置
    MOV R3, A           ; 更新后移地址
    MOV A, R1           ; 将暂存的数据取出
    MOV @R3, A          ; 写入到后移位置
    MOV A, R3           ; 读取当前后移位置
    SUB A, #02H         ; 地址回退2个单元
    MOV R3, A           ; 更新后移地址
    MOV A, 0EAH         ; 获取后移计数器当前值
    SUB A, #01H         ; 计数器值减1
    MOV 0EAH, A         ; 更新计数器值
    JZ LOOP2            ; 如果计数器值为0，跳转到下一步逻辑
    JMP SHIFT_LOOP      ; 如果计数未结束，继续后移
    RET                 ; 返回主程序

TIME:
    CALL PAGES           ; 时间管理逻辑，调用页面更新程序
    RET                 ; 返回主程序

NHIT:
    MOV A, 0EEH         ; 读取未命中计数器的当前值
    ADD A, #01H         ; 计数器值加1
    MOV 0EEH, A         ; 更新未命中计数器值
    RET                 ; 返回主程序

NULL:
    MOV A, 0DDH         ; 读取空页面计数器当前值
    SUB A, #01H         ; 计数器值减1
    MOV 0DDH, A         ; 更新空页面计数器值
    OUT                 ; 输出当前空页面计数器值
    JZ TIME             ; 如果空页面耗尽，跳转到时间更新逻辑
    MOV A, R0           ; 读取当前R0地址值
    ADD A, #01H         ; 地址加1，指向下一个数据
    MOV R0, A           ; 更新R0地址值
    MOV A, R3           ; 读取R3地址值
    SUB A, #01H         ; 地址减1，指向缓冲区尾部数据
    MOV R3, A           ; 更新R3地址值
    MOV A, @R0          ; 读取当前R0地址数据
    MOV @R3, A          ; 写入到缓冲区尾部位置
    CALL NHIT           ; 调用未命中逻辑
    JMP NULL            ; 跳转继续处理下一个空页面数据

HIT:
    MOV A, 0EFH         ; 读取命中计数器当前值
    ADD A, #01H         ; 计数器值加1
    MOV 0EFH, A         ; 更新命中计数器值
    CALL PAGES          ; 调用页面更新逻辑
    RET                 ; 返回主程序

OVER:
    JMP OVER            ; 程序结束，保持循环防止意外退出
