org	0
jmp	main
org	50h
main:
	mov	dptr,	#8001h
	mov	a,	#55h
	movx	@dptr,	a
	mov	dptr,	#8002h
	mov	a,	#0aah
	movx	@dptr,	a
	jmp	main