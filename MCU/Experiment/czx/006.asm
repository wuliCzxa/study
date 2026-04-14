		org 	0000h
		LJMP 	MAIN
		org		50h
MAIN:	MOV		30H,#0AH
		MOV		31H,#0AH
		MOV		32H,#0AH
		MOV		33H,#0AH
		MOV		34H,#0AH
lop:	jb		p1.0,lop2
lop3:	mov		dptr,#0A000H
		MOVX	@DPTR,A
		MOV		A,#50
		DJNZ	ACC,$
		MOVX	A,@DPTR
		MOV		B,#10
		DIV		AB
		MOV		37H,B
		MOV		B,#10
		DIV		AB
		MOV		36H,B
		MOV		35H,A

tt:		call	disp
		jmp		lop
lop2:	
		mov		a,#200
		mov		dptr,#9000h
		movx	@dptr,a
		jmp		lop3
disp:	MOV		B,#80H
		MOV		R0,#30h
		mov		r7,#8
dip1:	mov		dptr,#8001h
		clr		a
		movx	@dptr,a
		MOV 	A, @R0
		mov		dptr,#tab1
		movc	a,@a+dptr
		CJNE	R7,#3,dip2
		orl		a,#80h
dip2:	mov		dptr,#8002h
		movx	@dptr,a
		mov		a,b
		mov		dptr,#8001h
		MOVX	@dptr,a
		rr		a
		mov		b,a
		mov		a,#22
		djnz	acc,$
		inc		r0
		djnz	r7,dip1
		ret
tab1:	db		3fh, 06h, 5bh, 4fh, 66h, 6dh, 7dh, 07h,7fh, 6fh,0
	
		END