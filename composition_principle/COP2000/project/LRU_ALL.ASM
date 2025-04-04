; 初始化部分，清空必要寄存器和内存
MOV A, #00H
MOV R0, A
MOV R1, A
MOV R2, A
MOV R3, A

MOV 0EFH, A
MOV 0EEH, A
MOV 0EBH, A
MOV 0EAH, A
MOV 0DFH, A
MOV 0DEH, A

MAIN:
    MOV R0, #0F0H       ; 设置原地址指针为0F0H

    MOV A, #04H         ; 设置循环次数
    MOV 0DDH, A         ; 用于跟踪循环计数

    MOV R0, #0F0H
    MOV R3, #0E3H

    CALL PAGES           ; 调用页面置换逻辑

    JMP OVER             ; 完成后结束程序

PAGES:
    MOV R0, #0F4H       ; 初始化原地址R0指向0F4H
    MOV R2, #12         ; 设置循环次数为12次

PAGE_LOOP:
    MOV A, @R0          ; 从原地址读取数据
    CALL CHECK_EXISTENCE ; 检查是否命中

    CALL COPY            ; 调用COPY逻辑，复制页面数据到备份区域
    CALL SHIFT_PAGE      ; 调用页面后移逻辑

    ; 检查是否命中并更新计数
    CALL COMPARE_PAGES

NEXT_ADDRESS:
    MOV A, R0
    ADD A, #01          ; R0指针地址加1
    MOV R0, A
    MOV A, R2
    SUB A, #01          ; 循环计数器减1
    MOV R2, A
    JZ OVER             ; 若完成12次循环则结束

    JMP PAGE_LOOP        ; 否则继续循环

    RET

CHECK_EXISTENCE:
    MOV R3, #0E0H       ; 初始化检查起点为0E0H
    MOV A, #4           ; 设置检查范围为4个地址
    MOV 0EBH, A

CHECK_LOOP:
    MOV A, @R3          ; 读取页面数据
    SUB A, @R0          ; 比较数据是否相等
    JZ EXIST            ; 如果命中，则跳转到EXIST

    MOV A, R3
    ADD A, #01H          ; 移动到下一个地址
    MOV R3, A
    MOV A, 0EBH
    SUB A, #01           ; 检查剩余检查次数
    MOV 0EBH, A
    JZ MISS              ; 若未命中，跳转MISS
    JMP CHECK_LOOP       ; 否则继续检查

EXIST:
    CALL HIT             ; 调用命中更新逻辑
    RET

MISS:
    CALL NHIT            ; 调用未命中更新逻辑
    RET

SHIFT_PAGE:
    MOV R3, #0E3H       ; 初始化后移起点为0E3H
    MOV A, #4           ; 设置后移次数
    MOV 0EAH, A

SHIFT_LOOP:
    MOV A, @R3          ; 从当前地址读取数据
    MOV R1, A           ; 保存数据至R1
    MOV A, R3
    SUB A, #01H         ; 获取前一地址
    MOV @R3, A         ; 将数据写入前一地址
    SUB A, #01          ; 更新地址寄存器
    MOV R3, A
    MOV A, 0EAH
    SUB A, #01          ; 后移次数递减
    MOV 0EAH, A
    JZ END_SHIFT         ; 若完成后移，退出循环
    JMP SHIFT_LOOP       ; 否则继续后移

END_SHIFT:
    RET

COPY:
    MOV R3, #0E0H       ; 源地址起点为0E0H
    MOV R1, #0D5H       ; 备份地址起点为0D5H
    MOV A, #4           ; 设置需要复制的页面数
    MOV 0EBH, A

COPY_LOOP:
    MOV A, @R3          ; 从源地址读取数据
    MOV @R1, A          ; 将数据写入备份地址
    MOV A, R3
    ADD A, #01H         ; 源地址递增
    MOV R3, A
    MOV A, R1
    ADD A, #01H         ; 备份地址递增
    MOV R1, A
    MOV A, 0EBH
    SUB A, #01          ; 剩余需要复制的页面数递减
    MOV 0EBH, A
    JZ END_COPY          ; 若完成复制，退出循环
    JMP COPY_LOOP        ; 否则继续复制

END_COPY:
    RET

COMPARE_PAGES:
    MOV R3, #0E0H       ; 地址起点为0E0H
    MOV R1, #0D0H       ; 对比地址起点为0D0H
    MOV A, #4           ; 设置对比次数
    MOV 0EBH, A

COMPARE_LOOP:
    MOV A, @R3          ; 读取页面数据
    SUB A, @R1          ; 比较数据
    JZ INCREMENT_HIT     ; 若相等，增加命中计数
    JMP INCREMENT_MISS   ; 若不相等，增加未命中计数

INCREMENT_HIT:
    MOV A, 0EFH         ; 读取命中计数
    ADD A, #01H         ; 命中计数加1
    MOV 0EFH, A
    JMP NEXT_COMPARE

INCREMENT_MISS:
    MOV A, 0EEH         ; 读取未命中计数
    ADD A, #01H         ; 未命中计数加1
    MOV 0EEH, A

NEXT_COMPARE:
    MOV A, R3
    ADD A, #01H         ; 页面地址递增
    MOV R3, A
    MOV A, R1
    ADD A, #01H         ; 对比地址递增
    MOV R1, A
    MOV A, 0EBH
    SUB A, #01          ; 对比次数递减
    MOV 0EBH, A
    JZ END_COMPARE       ; 若完成对比，退出循环
    JMP COMPARE_LOOP     ; 否则继续对比

END_COMPARE:
    RET

NHIT:
    MOV A, 0EEH         ; 获取未命中计数
    ADD A, #01H         ; 增加1
    MOV 0EEH, A         ; 保存回内存
    RET

HIT:
    MOV A, 0EFH         ; 获取命中计数
    ADD A, #01H         ; 增加1
    MOV 0EFH, A         ; 保存回内存
    RET

OVER:
    JMP OVER            ; 结束程序循环

