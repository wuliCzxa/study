	ORG 0       
	LJMP START      
	ORG 000BH       
	LJMP IT0P       
	ORG 0023H       
	LJMP INPUT      
	ORG 50H       
	
	START: 
;	    ORL 8EH,	#20H  
		CLR	A
		MOV	33H,	A   
	    MOV TMOD,	#21H 
	    MOV SCON,	#50H
	    MOV TH0,	#3CH  
	    MOV TL0,	#0B0H
	    MOV TH1,	#0F4H
	    MOV TL1,	#00H 
	    SETB	PS     
	    MOV	IE,	#92H  
	    ;SETB	TR0      
	    SETB	TR1    
		
	WAIT_INPUT:
		MOV	A,	33H  
		JZ	WAIT_INPUT
		SETB	TR0  
		
		MOV	R4,	#20
	
	LOOP: 
	    MOV R0,#30H  
	    MOV R1,#20H
	
	RI0:
	
	BCD: 
	    MOV A,	@R0    
	    SWAP A       
	    LCALL	DISP    
	    MOV A,	@R0    
	    LCALL	DISP    
	    INC	R0       
	    CJNE	R0,	#33H,	BCD 
	    LJMP	LOOP     
	
	DISP:
	    ANL A,	#0FH    
	    MOV DPTR,	#TAB
	    MOVC	A,	@A+DPTR
	    MOV DPTR,	#8002H
	    MOVX	@DPTR,	A 
	    MOV A,	R1     
	    MOV DPTR,	#8001H
	    MOVX	@DPTR,A
	    RR A      
	    MOV R1,	A     
	    LCALL	DELAY  
	    CLR A       
	    MOV DPTR,	#8001H
	    MOVX	@DPTR,	A 
	    RET          
	
	TAB: 
		DB	3FH,	06H,	5BH,	4FH,	66H,	6DH,	7DH,	07H,	7FH,	6FH
	
	DELAY: 
	    MOV R7,	#1    
	L0: 
	    MOV R6,	#2   
	L1: 
	    MOV R5,	#250  
	    DJNZ	R5,	$    
	    DJNZ	R6,	L1   
	    DJNZ	R7,	L0   
	    RET          
	
	IT0P: 
	    MOV TH0,	#3CH  
	    MOV TL0,	#0B0H 
	    DJNZ	R4,	RETURN
	    MOV R4,	#20   
	    MOV R0,	#32H  
	    MOV B,	#02H  
	    CLR A        
	    SETB	CY      
	    ADDC	A,	@R0    
	    DA A         
	MINS: 
	    CJNE	A,	#60H,	SEND 
	    CLR	A      
	    MOV @R0,	A   
	    DEC R0       
	    SETB	CY      
	    ADDC	A,	@R0   
	    DA	A         
	    DJNZ	B,	MINS  
	    CJNE	A,	#24H,	SEND
	    CLR A        
	SEND: 
	    MOV @R0,	A     
	RETURN: 
	    RETI         
	
	INPUT: 
	    CLR RI       
	    CLR EA 
		      
	    JNB	RI,	$
		CLR	RI
		MOV 30H,	SBUF     
	
	    JNB	RI,	$
		CLR	RI        
	    MOV 31H,	SBUF  
	
	    JNB	RI,	$
		CLR	RI       
	    MOV 32H,	SBUF  
	
		MOV	33H,	#01H
	    SETB EA      
	    RETI        
	
	END             
