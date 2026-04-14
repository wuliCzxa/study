    MOV  A, #1
LOOP:
mov r0, #10h
;mov @r0, a
;nop
    out
    JMP  LOOP

    ORG  0E0H
    rrc  a
;    ADD  A, #01
;    in
;    cpl  a
    RETI
    END

    mov   a, #12h
    out
loop:
    jmp   loop
    end

