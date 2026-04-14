MOV A, #00H         ; 将累加器 A 初始化为 0
MOV R0, A           ; 初始化寄存器 R0 为 0
MOV R1, A           ; 初始化寄存器 R1 为 0
MOV R2, A           ; 初始化高位结果寄存器 R2 为 0
MOV R3, A           ; 初始化低位结果寄存器 R3 为 0

MOV 0E1H, A         ; 初始化存储乘数绝对值的 0E1H 单元为 0
MOV 0D0H, A         ; 初始化存储被乘数符号位的 0D0H 单元为 0
MOV 0D1H, A         ; 初始化存储乘数符号位的 0D1H 单元为 0

MAIN:
     ; 输入测试数据
     MOV R0, #92H  ; 设置被乘数 X 的初始值为 0xD6（十进制 214）
     MOV R1, #00H   ; 设置乘数 Y 的初始值为 0x3B（十进制 59）

     ; 提取符号位并保存
     MOV A, R0      ; 将被乘数 R0 的值加载到累加器 A 中
     AND A, #80H    ; 提取被乘数的符号位
     MOV 0D0H, A    ; 将被乘数符号位保存到存储器 0D0H 单元
     MOV A, R1      ; 将乘数 R1 的值加载到累加器 A 中
     AND A, #80H    ; 提取乘数的符号位
     MOV 0D1H, A    ; 将乘数符号位保存到存储器 0D1H 单元

     ; 获取被乘数和乘数的绝对值
     MOV A, R0
     AND A, #7FH    ; 清除符号位，只保留被乘数的绝对值
     MOV 0E0H, A    ; 将被乘数绝对值保存到存储器 0E0H 单元
     MOV A, R1
     AND A, #7FH    ; 清除符号位，只保留乘数的绝对值
     MOV 0E1H, A    ; 将乘数绝对值保存到存储器 0E1H 单元

     ; 初始化循环
     MOV R2, #00H   ; 清空高位寄存器 R2
     MOV A, 0E1H    ; 将存储的乘数绝对值加载到累加器 A
     MOV R3, A      ; 将累加器 A 中的值加载到低位寄存器 R3
     MOV A, #00H
     ADD A, #08H    ; 设置循环次数为 8（8 位二进制位）
     MOV 0D2H, A    ; 将循环次数保存到 0D2H 单元
     OUT            ; 输出当前循环状态
     CALL MUL       ; 调用乘法运算子程序
     CALL TIME      ; 调用计时和循环控制子程序

SAVE:
     ; 处理符号位以确定结果符号
     MOV A, R2
     MOV 0D5H, A
     MOV R2, A
     MOV A, 0D0H    ; 加载被乘数符号位
     SUB A, 0D1H    ; 计算被乘数与乘数符号位之差
     JZ OVER        ; 若相减结果为 0，则结果为正数，无需修改
     MOV A, R2      ; 否则加载高位寄存器 R2 的值
     ADD A, #80H    ; 将结果值修改为负数
     MOV R2, A      ; 将修改后的值保存到 R2
     JMP OVER       ; 跳转到程序结束部分

TIME:
     ; 减少循环次数并检查
     MOV A, 0D2H    ; 加载当前的循环计数
     SUB A, #01H    ; 循环计数减 1
     MOV 0D2H, A    ; 保存减少后的计数
     OUT            ; 输出当前状态
     AND A, #0FFH   ; 检查计数是否为 0
     JZ SAVE        ; 如果循环结束，跳转到保存步骤

MUL:
     ; 开始逐位实现乘法
     MOV A, R3      ; 加载低位结果寄存器 R3 的值
     AND A, #01H    ; 检查最低位是否为 1
     JZ LOP         ; 若最低位为 0，则跳过加法
     MOV A, R2      ; 加载高位结果寄存器 R2 的值
     ADD A, 0E0H    ; 加上被乘数绝对值
     MOV R2, A      ; 保存加法结果到高位寄存器 R2

LOP:
     ; 循环位移处理
     MOV A, R2
     AND A, #01H    ; 检查最高位是否进位
     JZ BIT         ; 若无进位，直接处理低位寄存器 R3
     MOV A, R3
     RR A           ; 右移 R3
     ADD A, #10000000B ; 模拟高位填充
     MOV R3, A
     MOV A, R2
     RR A           ; 高位右移
     MOV R2, A      ; 保存右移结果
     JMP TIME       ; 继续循环

BIT:
     MOV A, R3      ; 右移 R3 的普通位移
     RR A
     MOV R3, A      ; 保存低位寄存器结果
     MOV A, R2
     RR A           ; 右移高位寄存器 R2
     MOV R2, A      ; 保存结果
     JMP TIME       ; 循环继续

OVER:

     ; 程序结束
     JMP OVER       ; 无限循环保持结果显示


