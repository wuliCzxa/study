    ; 初始化相关寄存器及内存
    MOV A, #00H         ; 清零累加器A
    MOV R0, A           ; 初始化R0为0
    MOV R1, A           ; 初始化R1为0
    MOV R2, A           ; 初始化R2为0
    MOV R3, A           ; 初始化R3为0

MAIN:
    MOV R0, #0F4H       ; 设置数据起始地址为0F4H
    MOV R2, #12         ; 设置循环次数为12

LOOP_START:
    ; 每次循环操作逻辑
    MOV A, @R0          ; 读取当前地址R0指向的数据
    CALL COPY_BLOCK     ; 调用COPY程序复制缓冲区0E0H~0E3H到0D5H~0D8H
    CALL SHIFT_BLOCK    ; 调用后移程序，对0E0H~0E3H实施无条件后移
    MOV 0E0H, A         ; 将读取的数据存入缓冲区头部0E0H
    CALL COMPARE_BLOCK  ; 对比0E0H~0E3H与0D0H~0D3H的数据，并更新命中或未命中计数
NEXT_ADDRESS:
    ; 更新地址和循环计数
    MOV A, R0
    ADD A, #01H         ; 更新R0指向下一个数据地址
    MOV R0, A
    MOV A, R2
    SUB A, #01H         ; 减少循环计数
    MOV R2, A
    JZ LOOP_END         ; 若循环计数为0，跳转结束循环
    JMP LOOP_START      ; 否则继续下一个循环

LOOP_END:
    JMP LOOP_END         ; 程序结束，进入死循环防止意外退出

; COPY程序：复制0E0H~0E3H到0D5H~0D8H
COPY_BLOCK:
    MOV R3, #0E0H       ; 设置R3指向0E0H
    MOV R1, #0D5H       ; 设置R1指向0D5H
    MOV A, #04H         ; 设置循环计数为4
    MOV 0EAH, A         ; 暂存计数器值

COPY_LOOP:
    MOV A, @R3          ; 从R3指向的地址读取数据
    MOV @R1, A          ; 将数据写入R1指向的地址
    MOV A, R3
    ADD A, #01H         ; 移动R3到下一个地址
    MOV R3, A
    MOV A, R1
    ADD A, #01H         ; 移动R1到下一个地址
    MOV R1, A
    MOV A, 0EAH
    SUB A, #01H         ; 减少计数器值
    MOV 0EAH, A
    JZ COPY_END         ; 如果计数完成，跳转结束
    JMP COPY_LOOP       ; 否则继续复制

COPY_END:
    RET                 ; 返回主程序

; SHIFT程序：对0E0H~0E3H实施后移
SHIFT_BLOCK:
    MOV R3, #0E3H       ; 设置R3指向缓冲区尾部0E3H
    MOV A, #04H         ; 设置后移次数为4
    MOV 0EAH, A         ; 暂存计数器值

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

LOOP2:
    MOV A, 0D0H         ; 从地址0D0H读取数据
    MOV 0E0H, A         ; 将读取的数据存入缓冲区头部0E0H
    MOV A, #00H         ; 清空最后一个缓冲区地址的数据
    MOV 0E4H, A         ; 清零缓冲区尾部数据
    CALL MISS_FOUND           ; 调用未命中页面的处理逻辑
    CALL NEXT_ADDRESS   ; 跳转处理下一个地址

; COMPARE程序：比较0E0H~0E3H与0D0H~0D3H
COMPARE_BLOCK:
    MOV R3, #0E0H       ; 设置R3指向0E0H
    MOV R1, #0D0H       ; 设置R1指向0D0H
    MOV A, #04H         ; 设置比较次数为4
    MOV 0EAH, A         ; 暂存计数器值

COMPARE_LOOP:
    MOV A, @R3          ; 从R3指向的缓冲区读取数据
    SUB A, @R1          ; 比较R3和R1指向的数据
    JZ HIT_FOUND        ; 如果相等，跳转到命中逻辑
    MOV A, R3        ; 移动R3到下一个缓冲区地址
    ADD A, #01H
    MOV R3, A
    MOV A, R1     ; 移动R1到下一个缓冲区地址
    ADD A, #01H
    MOV R1, A
    MOV A, 0EAH            ; 减少比较计数
    SUB A, #01H
    MOV 0EAH, A
    JZ MISS_FOUND
    JMP COMPARE_LOOP    ; 如果计数未完成，继续比较

MISS_FOUND:
    CALL UPDATE_MISS    ; 调用未命中更新逻辑
    RET                 ; 返回主程序

HIT_FOUND:
    CALL UPDATE_HIT     ; 调用命中更新逻辑
    RET                 ; 返回主程序

; 更新命中计数
UPDATE_HIT:
    MOV A, 0EFH         ; 读取命中计数器值
    ADD A, #01H         ; 计数加1
    MOV 0EFH, A         ; 更新计数器
    RET                 ; 返回主程序

; 更新未命中计数
UPDATE_MISS:
    MOV A, 0EEH         ; 读取未命中计数器值
    ADD A, #01H         ; 计数加1
    MOV 0EEH, A         ; 更新计数器
    RET                 ; 返回主程序


OVER:
     JMP OVER
