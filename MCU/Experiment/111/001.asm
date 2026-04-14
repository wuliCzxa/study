; ----------- 头文件与寄存器定义 -----------
ORG 0000H            ; 程序起始地址
LJMP MAIN            ; 跳转到主程序
ORG 0003H            ; 外部中断0向量地址
LJMP INT0_ISR        ; 中断服务程序

; ----------- 常量定义 -----------
SEG_PORT EQU P0      ; 段选数据口（接74HC573）
BIT_PORT EQU P2      ; 位选控制口（接74HC138，P2.0-P2.2）

; 共阳数码管段码表（0-9）
SEG_TABLE:
    DB 0C0H, 0F9H, 0A4H, 0B0H, 99H, 92H, 82H, 0F8H, 80H, 90H

; ----------- 变量定义 -----------
DSEG AT 30H
COUNT:     DS 2      ; 16位计数器（0-999999）
DIGITS:    DS 6      ; 存储6位BCD码
DIG_INDEX: DS 1      ; 当前显示位索引（0-5）

; ----------- 主程序 -----------
CSEG
MAIN:
    MOV SP, #5FH      ; 初始化栈指针
    LCALL INIT        ; 初始化硬件
    LCALL DISPLAY_INIT ; 初始化显示

LOOP:
    LCALL UPDATE_DISPLAY ; 更新数码管显示
    SJMP LOOP          ; 主循环

; ----------- 初始化函数 -----------
INIT:
    ; 配置外部中断0（下降沿触发）
    SETB IT0          ; 边沿触发模式
    SETB EX0          ; 允许INT0中断
    SETB EA           ; 开总中断
    ; 初始化计数器
    MOV COUNT, #0
    MOV COUNT+1, #0
    RET

DISPLAY_INIT:
    MOV DIG_INDEX, #0 ; 从第0位开始显示
    RET

; ----------- 中断服务程序（按键计数） -----------
INT0_ISR:
    PUSH ACC          ; 保护现场
    PUSH PSW
    ; 计数器加1（16位）
    MOV A, COUNT
    ADD A, #1
    MOV COUNT, A
    MOV A, COUNT+1
    ADDC A, #0
    MOV COUNT+1, A
    ; 转换为6位BCD码
    LCALL COUNT_TO_BCD
    POP PSW           ; 恢复现场
    POP ACC
    RETI

; ----------- 计数器转BCD码 -----------
COUNT_TO_BCD:
    MOV R0, #DIGITS   ; R0指向BCD数组
    MOV R1, #6        ; 6位数字
    MOV R2, #0        ; 清零临时变量
    ; 此处省略具体转换代码（可用除法或查表法）
    RET

; ----------- 动态显示更新 -----------
UPDATE_DISPLAY:
    ; 1. 关闭当前位选（消隐）
    MOV A, #07H       ; 74HC138输入111（不选任何位）
    MOV BIT_PORT, A
    ; 2. 输出段码
    MOV A, DIG_INDEX  ; 获取当前位索引
    ADD A, #SEG_TABLE ; 计算段码地址
    MOVC A, @A+DPTR   ; 读取段码
    MOV SEG_PORT, A   ; 输出到74HC573
    ; 3. 打开当前位选
    MOV A, DIG_INDEX  ; 位索引（0-5）
    MOV BIT_PORT, A    ; 74HC138选择对应位
    ; 4. 更新位索引
    INC DIG_INDEX
    MOV A, DIG_INDEX
    CJNE A, #6, SKIP_RESET
    MOV DIG_INDEX, #0  ; 超过5则归零
SKIP_RESET:
    ; 5. 延时1ms
    LCALL DELAY_1MS
    RET

; ----------- 延时1ms -----------
DELAY_1MS:
    MOV R6, #10
DELAY_LOOP:
    MOV R7, #5
    DJNZ R7, $
    DJNZ R6, DELAY_LOOP
    RET

; ----------- 段码表存储 -----------
ORG 300H
SEG_DATA:
    DB 0C0H, 0F9H, 0A4H, 0B0H, 99H, 92H, 82H, 0F8H, 80H, 90H

END