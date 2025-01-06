MOV A, #00H          ; ³õÊ¼»¯
MOV R0, A            ; ³õÊ¼»¯R0£¨ÊýAµÄÔ­Âë´æ´¢£©
MOV R1, A            ; ³õÊ¼»¯R1£¨ÊýBµÄÔ­Âë´æ´¢£©
MOV R2, A            ; ³õÊ¼»¯R2£¨ÊýAµÄ²¹Âë´æ´¢£©
MOV R3, A            ; ³õÊ¼»¯R3£¨ÊýBµÄ²¹Âë´æ´¢£

MOV 0D0H, A          ; ³õÊ¼»¯ÄÚ´æ´æ´¢µØÖ·0D0H
MOV 0D1H, A          ; ³õÊ¼»¯ÄÚ´æ´æ´¢µØÖ·0D1H
MOV 0D2H, A          ; ³õÊ¼»¯ÄÚ´æ´æ´¢µØÖ·0D2H
MOV 0E0H, A          ; ÊýA×î¸ßÎ»´æ´¢
MOV 0E1H, A          ; ÊýA×î¸ßÎ»È¡·´´æ´¢
MOV 0E2H, A          ; ÊýB×î¸ßÎ»´æ´¢
MOV 0E3H, A          ; ÊýB×î¸ßÎ»È¡·´´æ´¢
MOV 0E4H, A          ; ¼Ó·¨½á¹û×î¸ßÎ»´æ´¢
MOV 0E5H, A          ; ¼Ó·¨½á¹û×î¸ßÎ»È¡·´´æ´¢

INPUT:               ; ÊäÈëÔ­ÂëÊý¾Ý
    MOV A, #50H      ; ÊýAµÄÔ­Âë(6E,50,94,94,72,94,00)
    MOV 0D0H, A      ; ´æ´¢µ½ÊýAµØÖ·
    MOV A, #30H      ; ÊýBµÄÔ­Âë(0A,3C,F8,88,1E,0A,00)
    MOV 0D1H, A      ; ´æ´¢µ½ÊýBµØÖ·
    JC INPUT_CHECK

INPUT_CHECK:
    MOV A, 0D0H      ; ¼ÓÔØÊýA
    AND A, #0FFH     ; È·±£ÓÐÐ§ÐÔ¼ì²é(7Î»·¶Î§ÄÚ)
    JC INPUT_ERROR   ; ÊäÈë´íÎóÌø×ª
    MOV R0, A        ; ´æ´¢µ½R0

    MOV A, 0D1H      ; ¼ÓÔØÊýB
    AND A, #0FFH     ; È·±£ÓÐÐ§ÐÔ¼ì²é(7Î»·¶Î§ÄÚ)
    JC INPUT_ERROR   ; ÊäÈë´íÎóÌø×ª
    MOV R1, A        ; ´æ´¢µ½R1

CONVERT_A:
    MOV A, R0        ; ¼ÓÔØÊýAÔ­Âë
    JZ ZERO_A        ; Èç¹ûÊýAÎª0£¬Ìø¹ý×ª»»
    AND A, #80H      ; ¼ì²é·ûºÅÎ»
    JZ POS_A         ; Èç¹ûÎªÕýÊý£¬Ìøµ½ÕýÊý´¦Àí
    MOV A, R0
    CPL A            ; Èç¹ûÎª¸ºÊý£¬È¡·´
    ADD A, #81H      ; ¼Ó1µÃµ½²¹Âë
    JMP SAVE_A

POS_A:
    MOV A, R0        ; Ö±½Ó½«Ô­Âë¸³ÖµÎª²¹Âë
    JMP SAVE_A

ZERO_A:
    MOV A, #00H      ; ÊýAÎª0Ê±£¬²¹ÂëÒ²ÊÇ0

SAVE_A:
    MOV R2, A        ; ½«ÊýAµÄ²¹Âë´æ´¢µ½R2

CONVERT_B:
    MOV A, #00H
    ADD A, R1        ; ¼ÓÔØÊýBÔ­Âë
    JZ ZERO_B        ; Èç¹ûÊýBÎª0£¬Ìø¹ý×ª»»
    AND A, #80H      ; ¼ì²é·ûºÅÎ»
    JZ POS_B         ; Èç¹ûÎªÕýÊý£¬Ìøµ½ÕýÊý´¦Àí
    MOV A, R1
    CPL A            ; Èç¹ûÎª¸ºÊý£¬È¡·´
    ADD A, #81H      ; ¼Ó1µÃµ½²¹Âë
    JMP SAVE_B

POS_B:
    MOV A, R1        ; Ö±½Ó½«Ô­Âë¸³ÖµÎª²¹Âë
    JMP SAVE_B

ZERO_B:
    MOV A, #00H      ; ÊýBÎª0Ê±£¬²¹ÂëÒ²ÊÇ0

SAVE_B:
    MOV R3, A        ; ½«ÊýBµÄ²¹Âë´æ´¢µ½R3

ADD_OPERATION:
    ; ²¹Âë¼Ó·¨
    MOV A, R2        ; ¼ÓÔØÊýA²¹Âë
    ADDC A, R3       ; ¼ÓÊýB²¹Âë²¢¿¼ÂÇ½øÎ»
    MOV 0D2H, A      ; ÔÝ´æ½á¹û

    ; ÌáÈ¡ºÍ¼ÆËãÒç³öÌõ¼þ
    MOV A, R2        ; »ñÈ¡ÊýA²¹ÂëµÄ×î¸ßÎ»
    AND A, #80H      ; ÌáÈ¡×î¸ßÎ»
    MOV 0E0H, A      ; ´æ´¢×î¸ßÎ»
    CPL A            ; È¡·´
    MOV 0E1H, A      ; ´æ´¢×î¸ßÎ»È¡·´

    MOV A, R3        ; »ñÈ¡ÊýB²¹ÂëµÄ×î¸ßÎ»
    AND A, #80H      ; ÌáÈ¡×î¸ßÎ»
    MOV 0E2H, A      ; ´æ´¢×î¸ßÎ»
    CPL A            ; È¡·´
    MOV 0E3H, A      ; ´æ´¢×î¸ßÎ»È¡·´

    MOV A, 0D2H      ; »ñÈ¡¼Ó·¨½á¹ûµÄ×î¸ßÎ»
    AND A, #80H      ; ÌáÈ¡×î¸ßÎ»
    MOV 0E4H, A      ; ´æ´¢×î¸ßÎ»
    CPL A            ; È¡·´
    MOV 0E5H, A      ; ´æ´¢×î¸ßÎ»È¡·´

    ; ¼ÆËãÒç³ö±í´ïÊ½
    MOV A, 0E0H      ; ÊýA×î¸ßÎ»
    AND A, 0E2H      ; ÓëÊýB×î¸ßÎ»
    AND A, 0E5H      ; Óë¼Ó·¨½á¹û×î¸ßÎ»È¡·´
    MOV 0C0H, A      ; ÔÝ´æÖÐ¼ä½á¹û1

    MOV A, 0E1H      ; ÊýA×î¸ßÎ»È¡·´
    AND A, 0E3H      ; ÓëÊýB×î¸ßÎ»È¡·´
    AND A, 0E4H      ; Óë¼Ó·¨½á¹û×î¸ßÎ»
    OR A, 0C0H       ; ºÏ²¢Á½¸ö½á¹û
    JZ STORE_RESULT  ; Èç¹û½á¹ûÎª0£¬²»Òç³ö

    ; ¼ì²éÊÇÉÏÒç»¹ÊÇÏÂÒç
    MOV A, 0E0H      ; ÊýA×î¸ßÎ»
    AND A, 0E2H      ; ÓëÊýB×î¸ßÎ»
    AND A, 0E5H      ; Óë¼Ó·¨½á¹û×î¸ßÎ»È¡·´
    JZ OVERFLOW_UPPER; Èç¹ûÎª0£¬Ìøµ½ÉÏÒç´¦Àí

OVERFLOW_LOWER:
    ; ÏÂÒç´¦Àí
    MOV A, #0FEH     ; ÏÂÒç±êÖ¾
    OUT              ; Êä³ö
    JMP OVER

OVERFLOW_UPPER:
    ;ÉÏÒç´¦Àí
    MOV A, #0FFH     ; ÉÏÒç±êÖ¾
    OUT              ; Êä³ö
    JMP OVER

STORE_RESULT:
    ; Õý³£½á¹û´¦Àí
    MOV A, 0D2H      ; »ñÈ¡½á¹û
    AND A, #80H      ; ¼ì²é·ûºÅÎ»
    JZ POS_RESULT    ; Èç¹ûÎªÕýÊý£¬Ìøµ½ÕýÊý´¦Àí
    MOV A, 0D2H
    SUBC A, #81H      
    CPL A            ; ×ª»»»ØÔ­Âë
    OUT              ; Êä³ö
    JMP OVER
        
POS_RESULT:
    MOV A, 0D2H      ; Ö±½Ó½«Ô­Âë¸³ÖµÎª²¹Âë
    OUT
    JMP OVER
    
OVER:
    JMP OVER         ; ³ÌÐò½áÊø
   
INPUT_ERROR:
    ; ´íÎóÊäÈë´¦Àí£¬Êä³ö´íÎó×´Ì¬ 
    MOV A, #0FFH     ; ´íÎó±êÖ¾
    OUT
    JMP OVER
   
