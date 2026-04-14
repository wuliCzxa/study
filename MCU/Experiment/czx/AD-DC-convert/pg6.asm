ORG 0
AJMP MAIN
ORG 50H
MAIN:
MOV 30H,#0B0H
OPEN:

    JNB P3.0,CLOSE
    MOV DPTR,#8000H
    MOV A,#0
    MOVX @DPTR,A
    JNB P3.1,$
    MOVX A,@DPTR
    
    MOV P1,A
    LJMP OPEN

CLOSE:
	
    MOV DPTR,#8000H
    MOV A,#0
    MOVX @DPTR,A

    MOV DPTR,#9000H
    MOV A,30H
    MOVX @DPTR,A

    MOV DPTR,#8001H
    JNB P3.1,$
    MOVX A,@DPTR

    MOV P1,A
    LJMP MAIN
    
    END
