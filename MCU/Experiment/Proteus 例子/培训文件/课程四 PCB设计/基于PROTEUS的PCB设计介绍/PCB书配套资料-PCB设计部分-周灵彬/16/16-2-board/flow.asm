ORG   0H
      	LJMP  STAR
      	ORG   30H
STAR:	MOV   A,#7FH		;(A)←7FH
LOOP: 	RL    A			;循环左移
      	MOV   P2,A		;输出到P1口显示
      	LCALL DELAY		;调延时子程序
      	LJMP  LOOP		;返回运行
DELAY:	MOV   R7,#250	;延时500ms
D1:   	MOV   R6,#250
D2:   NOP
      NOP
      NOP
      NOP
      NOP
      NOP
      DJNZ  R6,D2
      DJNZ  R7,D1
      RET
      END 
