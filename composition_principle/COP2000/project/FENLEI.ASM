MOV R0, #04H     ; 将循环起始位置设置为1

MOV R1, #07H     ; 将7存储在R1寄存器中，用于判断是否能被7整除



MOV A,#00H

MOV 0F0H, A     ; 正数个数初始化为0

MOV 0F1H, A     ; 负数个数初始化为0

MOV 0F2H, A     ; 零的个数初始化为0

MOV 0F3H, A     ; 奇数个数初始化为0

MOV 0F4H, A     ; 偶数个数初始化为0

MOV 0F5H, A     ; 能被7整除的数的个数初始化为0



LOOP:
    MOV R0, #04H     ; 将循环起始位置设置为1

    MOV A,R0	    ; 将R0中的值加载到累加器A中，用于判断循环是否结束

    SUB A,#05H	    ; 设置循环结束位置为101（十进制）101不执行

    JZ LOOP_HOLD    ; 如果循环结束，停止循环



    MOV A, @R0      ; 将内存中的值加载到累加器A中

    AND A,#0FFH     ;设置累加器A

    JZ IS_ZERO      ; 如果值为零，跳转到IS_ZERO标签



    ADD A,#80H      ;累加器最高位加一

    JC IS_NEGATIVE    ; 如果是负数，跳转到IS_NEGATIVE标签



    MOV A,0F0H      ;读取结果相应位置

    ADD A,#01H      ;累加器加一

    MOV 0F0H, A     ; 增加正数个数计数器



    MOV A,@R0



    JMP ODD_EVEN_CHECK



IS_ZERO:

    MOV A,0F2H

    ADD A,#01H

    MOV 0F2H, A     ; 增加零的个数计数器



    MOV A,0F5H      ;读取结果相应位置

    ADD A,#01H      ;累加器加一

    MOV 0F5H, A        ; 增加能被7整除的数的计数器



    MOV A,@R0

    JMP ODD_EVEN_CHECK   ; 继续判断奇偶性



IS_NEGATIVE:

    MOV A,0F1H      ;读取结果相应位置

    ADD A,#01H      ;累加器加一

    MOV 0F1H, A     ; 增加负数个数计数器

    MOV A,@R0



ODD_EVEN_CHECK:

    AND A, #01H    ; 判断奇偶性

    JZ IS_EVEN     ; 如果是偶数，跳转到IS_EVEN标签



    MOV A,0F3H      ;读取结果相应位置

    ADD A,#01H      ;累加器加一

    MOV 0F3H, A    ; 增加奇数个数计数器



    MOV A,@R0

    JMP DIVISIBLE_7_CHECK



IS_EVEN:

    MOV A,0F4H      ;读取结果相应位置

    ADD A,#01H      ;累加器加一

    MOV 0F4H, A    ; 增加偶数个数计数器



DIVISIBLE_7_CHECK:

    MOV A,@R0

    AND A,#7FH

DIV_LOOP:

    SUB A, R1           ; A减去7

    JC NOT_DIVISIBLE_7   ; 如果借位了，则说明不能被7整除

    JZ DIVISIBLE_7       ; 如果为零，则说明能被7整除

    JMP DIV_LOOP

DIVISIBLE_7:

    MOV A,0F5H      ;读取结果相应位置

    ADD A,#01H      ;累加器加一

    MOV 0F5H, A        ; 增加能被7整除的数的计数器



    MOV A,@R0

    JMP LOOP_NEXT       ; 继续下一个循环



NOT_DIVISIBLE_7:

    NOP                  ; 不可整除，不进行任何操作



LOOP_NEXT:

    MOV A,R0

    ADD A,#01H              ; 移动到下一个内存位置

    MOV R0,A

    JMP LOOP;



LOOP_HOLD:

    JMP LOOP_HOLD     ; 计数完毕，在此处原地循环



