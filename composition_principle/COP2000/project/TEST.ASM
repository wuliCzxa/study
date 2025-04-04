MOV A, #00H          ; 初始化
MOV R0, A            ; 初始化R0（数A的原码存储）
MOV R1, A            ; 初始化R1（数B的原码存储）
MOV R2, A            ; 初始化R2（数A的补码存储）
MOV R3, A            ; 初始化R3（数B的补码存储�

MOV 0D0H, A          ; 初始化内存存储地址0D0H
MOV 0D1H, A          ; 初始化内存存储地址0D1H
MOV 0D2H, A          ; 初始化内存存储地址0D2H
MOV 0E0H, A          ; 数A最高位存储
MOV 0E1H, A          ; 数A最高位取反存储
MOV 0E2H, A          ; 数B最高位存储
MOV 0E3H, A          ; 数B最高位取反存储
MOV 0E4H, A          ; 加法结果最高位存储
MOV 0E5H, A          ; 加法结果最高位取反存储

INPUT:               ; 输入原码数据
    MOV A, #50H      ; 数A的原码(6E,50,94,94,72,94,00)
    MOV 0D0H, A      ; 存储到数A地址
    MOV A, #30H      ; 数B的原码(0A,3C,F8,88,1E,0A,00)
    MOV 0D1H, A      ; 存储到数B地址
    JC INPUT_CHECK

INPUT_CHECK:
    MOV A, 0D0H      ; 加载数A
    AND A, #0FFH     ; 确保有效性检查(7位范围内)
    JC INPUT_ERROR   ; 输入错误跳转
    MOV R0, A        ; 存储到R0

    MOV A, 0D1H      ; 加载数B
    AND A, #0FFH     ; 确保有效性检查(7位范围内)
    JC INPUT_ERROR   ; 输入错误跳转
    MOV R1, A        ; 存储到R1

CONVERT_A:
    MOV A, R0        ; 加载数A原码
    JZ ZERO_A        ; 如果数A为0，跳过转换
    AND A, #80H      ; 检查符号位
    JZ POS_A         ; 如果为正数，跳到正数处理
    MOV A, R0
    CPL A            ; 如果为负数，取反
    ADD A, #81H      ; 加1得到补码
    JMP SAVE_A

POS_A:
    MOV A, R0        ; 直接将原码赋值为补码
    JMP SAVE_A

ZERO_A:
    MOV A, #00H      ; 数A为0时，补码也是0

SAVE_A:
    MOV R2, A        ; 将数A的补码存储到R2

CONVERT_B:
    MOV A, #00H
    ADD A, R1        ; 加载数B原码
    JZ ZERO_B        ; 如果数B为0，跳过转换
    AND A, #80H      ; 检查符号位
    JZ POS_B         ; 如果为正数，跳到正数处理
    MOV A, R1
    CPL A            ; 如果为负数，取反
    ADD A, #81H      ; 加1得到补码
    JMP SAVE_B

POS_B:
    MOV A, R1        ; 直接将原码赋值为补码
    JMP SAVE_B

ZERO_B:
    MOV A, #00H      ; 数B为0时，补码也是0

SAVE_B:
    MOV R3, A        ; 将数B的补码存储到R3

ADD_OPERATION:
    ; 补码加法
    MOV A, R2        ; 加载数A补码
    ADDC A, R3       ; 加数B补码并考虑进位
    MOV 0D2H, A      ; 暂存结果

    ; 提取和计算溢出条件
    MOV A, R2        ; 获取数A补码的最高位
    AND A, #80H      ; 提取最高位
    MOV 0E0H, A      ; 存储最高位
    CPL A            ; 取反
    MOV 0E1H, A      ; 存储最高位取反

    MOV A, R3        ; 获取数B补码的最高位
    AND A, #80H      ; 提取最高位
    MOV 0E2H, A      ; 存储最高位
    CPL A            ; 取反
    MOV 0E3H, A      ; 存储最高位取反

    MOV A, 0D2H      ; 获取加法结果的最高位
    AND A, #80H      ; 提取最高位
    MOV 0E4H, A      ; 存储最高位
    CPL A            ; 取反
    MOV 0E5H, A      ; 存储最高位取反

    ; 计算溢出表达式
    MOV A, 0E0H      ; 数A最高位
    AND A, 0E2H      ; 与数B最高位
    AND A, 0E5H      ; 与加法结果最高位取反
    MOV 0C0H, A      ; 暂存中间结果1

    MOV A, 0E1H      ; 数A最高位取反
    AND A, 0E3H      ; 与数B最高位取反
    AND A, 0E4H      ; 与加法结果最高位
    OR A, 0C0H       ; 合并两个结果
    JZ STORE_RESULT  ; 如果结果为0，不溢出

    ; 检查是上溢还是下溢
    MOV A, 0E0H      ; 数A最高位
    AND A, 0E2H      ; 与数B最高位
    AND A, 0E5H      ; 与加法结果最高位取反
    JZ OVERFLOW_UPPER; 如果为0，跳到上溢处理

OVERFLOW_LOWER:
    ; 下溢处理
    MOV A, #0FEH     ; 下溢标志
    OUT              ; 输出
    JMP OVER

OVERFLOW_UPPER:
    ;上溢处理
    MOV A, #0FFH     ; 上溢标志
    OUT              ; 输出
    JMP OVER

STORE_RESULT:
    ; 正常结果处理
    MOV A, 0D2H      ; 获取结果
    AND A, #80H      ; 检查符号位
    JZ POS_RESULT    ; 如果为正数，跳到正数处理
    MOV A, 0D2H
    SUBC A, #81H      
    CPL A            ; 转换回原码
    OUT              ; 输出
    JMP OVER
        
POS_RESULT:
    MOV A, 0D2H      ; 直接将原码赋值为补码
    OUT
    JMP OVER
    
OVER:
    JMP OVER         ; 程序结束
   
INPUT_ERROR:
    ; 错误输入处理，输出错误状态 
    MOV A, #0FFH     ; 错误标志
    OUT
    JMP OVER
   
