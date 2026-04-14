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

     MOV A, #04H     ; 初始化页面缓冲区大小为4
     MOV 0DDH, A     ; 将页面缓冲区大小存入地址0DDH
     MOV R0, #0F0H   ; 重设R0为数据起始地址
     MOV R3, #0E3H   ; 设置R3为页面缓冲区尾地址
     MOV A, @R0      ; 从地址R0读取数据到A
     MOV @R3, A      ; 将数据从A存入R3指向的位置
     CALL NHIT       ; 调用未命中页面的处理函数
     CALL NULL       ; 调用空页面的处理函数

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

TIME:
     JMP TIME
