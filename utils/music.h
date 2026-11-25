// MTE MK1 (la Churrera) v5.0
// Copyleft 2010-2014, 2020 by the Mojon Twins

#asm
                ;org 60000
; *****************************************************************************
; * Special FX Music Player Engine
; *
; * Based on code written by Jonathan Smith for the Special FX game, Firefly.
; * Modified by Chris Cowley
; *
; * Produced by Beepola v1.08.01
; ******************************************************************************

.START          ;di
                EXX
                PUSH  HL                    ; Preserve HL  for return to BASIC
                CALL  PLAY_MUSIC
                IM    1
                POP   HL
                EXX
                ;ei
                RET

.PLAY_MUSIC
                CALL  MAKE_VECTOR_TABLE
                CALL  INIT_ISR
                IM    2
                LD    A,0xFE00/ 256
                LD    I,A

                LD    HL,MUSICDATA         ;  <- Pointer to Music Data. Change
                                           ;     this to play a different song
                LD    E,(HL)
                INC   HL
                LD    D,(HL)
                INC   HL
                LD    (NEXT_PATT_PTR),HL
                LD    (NEXT_PERC_PATT_PTR),DE
                LD    (SAVED_SP),SP
                LD    HL,PLAYER_ISR
                LD    (0xFFF5),HL           ; Set up the JP instruction to point
                                           ; to our player ISR
                LD    A,0x0
                LD    (OUT_VAL1),A         ; Beeper out values for chan1
                LD    (OUT_VAL2),A         ; Beeper out values for chan1
                LD    (OUT_VAL3),A         ; Beeper out values for chan2
                LD    (OUT_VAL4),A         ; Beeper out values for chan2
                CALL  SET_NEXT_PATT
                CALL  SET_NEXT_PERC_PATT
                LD    C,0x01
                EXX
                LD    BC,0x0101
                ;ei
                HALT
.NEXT_NOTE      CALL  C,LF02C
                LD    A,B
                OR    A
                JR    NZ,LEF99
.LEF61          DEFB  0x21               ; LD HL,nn
.CURR_PATT_CH1
                DEFW  0x0000             ; Current Pattern for chan1
.LEF64          LD    A,(HL)
                OR    A
                JP    M,LEB9B           ; bit7 set - command (sustain, pattern end, etc)
                CALL  LF0F6
                LD    (LEFA2),A
                SRL   A
                SRL   A
                SRL   A
                LD    D,A
                LD    (LEFAD),A
                XOR   A
                LD    (LF02D),A
                INC   A
                LD    (LEFA4),A
                LD    A,(OUT_VAL1)      ; Adjust the EAR and MIC bits of this
                OR    0x18               ; port output value for the tone
                LD    (OUT_VAL1),A      ; generator
                INC   HL
.POST_MUTE1     LD    A,(HL)
                LD    (LEF96),A
                INC   HL
                LD    (CURR_PATT_CH1),HL
                DEFB  0x06               ; LD    B,n
.LEF96          DEFB  0x54
                JR    LEF9E
.LEF99          LD    A,0x03
.LEF9B          DEC   A
                JR    NZ,LEF9B
.LEF9E          DEC   D
                JR    NZ,LEFB5

                DEFB  0x16               ; LD D,n
.LEFA2          DEFB  0x3B
                DEFB  0x3E               ; LD A,n
.LEFA4          DEFB  0x07
.LEFA5          DEC   A
                JR    NZ,LEFA5

                DEFB  0x3E               ; LD A,n
.OUT_VAL1       DEFB  0x0
                OUT   (0xFE),A
                DEFB  0x3E               ; LD A,n
;LEFAD:          DEFB  0x01               ; frequency counter for CH1
.LEFAE          DEC   A
                JR    NZ,LEFAE

                DEFB  0x3E               ; LD A,n
.OUT_VAL2       DEFB  0x00
                OUT   (0xFE),A
.LEFB5          LD    A,C
                OR    A
                JR    NZ,LEFEF

                DEFB  0x21               ; LD HL,nn
.CURR_PATT_CH2  DEFW  0x0000             ; Current pattern for CH2
.LEFBC          LD    A,(HL)
                OR    A
                JP    M,LEBA6           ; bit7 set - command (sustain, pattern end, etc)
                CALL  LF0F6
                LD    (LEFF9),A
                SRL   A
                SRL   A
                LD    (LF004),A
                LD    E,A
                XOR   A
                LD    (LF045),A
                INC   A
                LD    (LEFFB),A
                LD    A,(OUT_VAL3)
                OR    0x18
                LD    (OUT_VAL3),A
                INC   HL
.POST_MUTE2     LD    A,(HL)
                LD    (LEFEC),A
                INC   HL
                LD    (CURR_PATT_CH2),HL
                DEFB  0x0E               ; LD C,n
.LEFEC          DEFB  0x0E
                JR    LEFF4

.LEFEF          LD    A,0x03
.LEFF1          DEC   A
                JR    NZ,LEFF1
.LEFF4          DEC   E
                JP    NZ,NEXT_NOTE

                DEFB  0x1E               ; LD E,n
.LEFF9          DEFB  0x3B
                DEFB  0x3E               ; LD A,n
.LEFFB          DEFB  0x02
.LEFFC          DEC   A
                JR    NZ,LEFFC

                DEFB  0x3E               ; LD A,n
.OUT_VAL3       DEFB  0x18
                OUT   (0xFE),A
                DEFB  0x3E               ; LD A,n
;LF004:          DEFB  0x0D               ; ch2 note frequency
.LF005          DEC   A
                JR    NZ,LF005

                DEFB  0x3E
.OUT_VAL4       DEFB  0x0
                OUT   (0xFE),A
                JP    NEXT_NOTE

.LF00F
                POP   HL
                LD    A,(HL)
                LD    (LF033),A
                INC   HL
                JP    LEF64

.LF018          POP   HL
                LD    A,(HL)
                LD    (LF04B),A
                INC   HL
                JR    LEFBC

.MUTE_CH1       POP   HL
                LD    A,(OUT_VAL1)
                AND   0x07
                LD    (OUT_VAL1),A
                JP    POST_MUTE1

.MUTE_CH2       POP   HL
                LD    A,(OUT_VAL3)
                AND   0x07
                LD    (OUT_VAL3),A
                JP    POST_MUTE2
.NULL_NOTE1     POP   HL
                JP    POST_MUTE1
.NULL_NOTE2     POP   HL
                JP    POST_MUTE2

.LF021
                POP   HL
                CALL  SET_NEXT_PATT
                JP    LEF61

.LF02C          DEFB  0x3E
.LF02D          DEFB  0x00
                INC   A
                LD    (LF02D),A
                DEFB  0xFE               ; CP
.LF033          DEFB  0x02
                JR    C,LF044
                XOR   A
                LD    (LF02D),A
                LD    HL,LEFAD
                DEC   (HL)
                JR    Z,LF043
                LD    HL,LEFA4
.LF043          INC   (HL)

.LF044          DEFB  0x3E               ; LD A,n
.LF045          DEFB  0x00
                INC   A
                LD    (LF045),A
                DEFB  0xFE               ; CP
.LF04B          DEFB  0x04
                RET   C
                XOR   A
                LD    (LF045),A
                LD    HL,LF004
                DEC   (HL)
                JR    Z,LF05A
                LD    HL,LEFFB
.LF05A          INC   (HL)
                RET

.LF0F6          PUSH  HL
                PUSH  DE
                LD    HL,FREQ_TABLE
                LD    E,A
                LD    D,0x0
                ADD   HL,DE
                LD    A,(HL)
                POP   DE
                POP   HL
                RET

; ** Reads and sets up the next melody pattern to play
.SET_NEXT_PATT
               DEFB  0x21   ; LD HL,nn
.NEXT_PATT_PTR DEFW  0x0000              ; holds a pointer to the
                                        ; next pattern in the patter list
.GET_PATT_ADDR LD    E,(HL)
               INC   HL
               LD    D,(HL)
               INC   HL
               LD    A,E
               OR    D
               JR    NZ,STORE_NEXT_P
               LD    E,(HL)
               INC   HL
               LD    D,(HL)
               EX    DE,HL
               JR    GET_PATT_ADDR
.STORE_NEXT_P  LD    (NEXT_PATT_PTR),HL
               EX    DE,HL
               LD    E,(HL)
               INC   HL
               LD    D,(HL)
               INC   HL
               LD    (CURR_PATT_CH1),HL
               LD    (CURR_PATT_CH2),DE
               RET

; ** Reads and sets up the next percussion pattern to play
.SET_NEXT_PERC_PATT
               DEFB  0x21   ; LD HL,nn
.NEXT_PERC_PATT_PTR
               DEFW  0x0000
.GET_PERC_ADDR LD    E,(HL)
               INC   HL
               LD    D,(HL)
               INC   HL
               LD    (NEXT_PERC_PATT_PTR),HL
               LD    (PERC_PATT),DE
               LD    A,E
               OR    D
               RET   NZ
               LD    E,(HL)
               INC   HL
               LD    D,(HL)
               EX    DE,HL
               JR    GET_PERC_ADDR
               CALL  SET_NEXT_PERC_PATT
               JR    PLAY_PERC

.LEB9B
               INC   HL
               PUSH  HL
               AND   0x7F
               CALL  JUMP_PERC_ADDR
               DEFW  LF021
               DEFW  LF00F
               DEFW  MUTE_CH1
               DEFW  NULL_NOTE1

.LEBA6         INC   HL
               PUSH  HL
               AND   0x7F
               CALL  JUMP_PERC_ADDR
               DEFW  LF021
               DEFW  LF018
               DEFW  MUTE_CH2
               DEFW  NULL_NOTE2

.PLAY_PERC
               DEFB  0x21                   ; LD HL,nn
.PERC_PATT     DEFW  0x0000                 ; Address of the percussion data
               LD    A,(HL)
               INC   HL
               LD    C,(HL)
               INC   HL
               LD    (PERC_PATT),HL        ; Point PERC_PATT at next datum
               AND   0x7F
               CALL  JUMP_PERC_ADDR
               DEFW  DRUM00                ; F094
               DEFW  DRUM01                ; F09A
               DEFW  DRUM02                ; F0AC
               DEFW  DRUM03                ; F0BB
               DEFW  DRUM04                ; F0D4
               DEFW  DRUM05                ; F07C

.JUMP_PERC_ADDR
               POP   HL                    ; F0ED
               ADD   A,A
               ADD   A,L
               LD    L,A
               LD    A,(HL)
               INC   HL
               LD    H,(HL)
               LD    L,A
               JP    (HL)

.DRUM00        CALL  SET_NEXT_PERC_PATT
               JR    PLAY_PERC

.DRUM01        LD    E,0x0A
               LD    A,0x0
               LD    HL,0x0100
.DRUM01NOISE   XOR   0x18
               OUT   (0xFE),A
               LD    B,(HL)
.DRUM01LOOP    DJNZ  DRUM01LOOP
               INC   HL
               DEC   E
               JR    NZ,DRUM01NOISE
               RET

.DRUM02        LD    HL,0x005A
.DRUM02LOOP    LD    A,(HL)
               OR    A
               RET   Z
               AND   0x18
               OR    0x0
               OUT   (0xFE),A
               INC   HL
               JR    DRUM02LOOP

.DRUM03        LD    HL,0x0F18
               LD    D,0x0A
.DRUM03LOOP3   LD    B,(HL)
.DRUM03LOOP    DJNZ  DRUM03LOOP
               LD    A,0x18
               OR    0x0
               OUT   (0xFE),A
               INC   HL
               LD    B,(HL)
.DRUM03LOOP2   DJNZ  DRUM03LOOP2
               LD    A,0x0
               OUT   (0xFE),A
               INC   HL
               DEC   D
               JR    NZ,DRUM03LOOP3
               RET

.DRUM04        LD    E,0x3F
               LD    D,0x05
.DRUM04LOOP3   LD    B,E
.DRUM04LOOP    DJNZ  DRUM04LOOP
               LD    A,0x18
               OR    0x0
               OUT   (0xFE),A
               LD    A,E
               RRCA
               LD    E,A
               LD    B,A
.DRUM04LOOP2   DJNZ  DRUM04LOOP2
               LD    A,0x0
               OUT   (0xFE),A
               DEC   D
               JR    NZ,DRUM04LOOP3
.DRUM05        RET

; ** Creates a vector table of 257 0xFF bytes at the location specified
; ** by 0xFE00
.MAKE_VECTOR_TABLE
                LD    HL,0xFE00
                LD    DE,0xFE00+ 1
                LD    BC,0x0100
                LD    (HL),0xFF
                LDIR
                RET

; *** The IM 2 service routine active throughout the life of the player
; *** updates counters, plays any active percussion sounds, and checks for
; *** keypresses or Kempston joystick fire button to terminate
.PLAYER_ISR
                PUSH  AF
                PUSH  DE
                PUSH  HL
                DEC   C
                DEC   B
                EXX
                DEC   C
                CALL  Z,PLAY_PERC          ; EBB1
                ; Read keyboard
                XOR   A
                IN    A,(0xFE)
                CPL
                AND   0x1F
                JR    NZ,ISR_KEY_PRESSED
                XOR   A
                IN    A,(0x1F)
                BIT   5,A
                JR    NZ,END_PLAYER_ISR
                BIT   4,A
                JR    NZ,ISR_FIRE_PRESSED
.END_PLAYER_ISR EXX
                POP   HL
                POP   DE
                POP   AF
                SCF
                ;ei
                RETI
.ISR_FIRE_PRESSED
.ISR_KEY_PRESSED
                DEFB  0x31                  ; LD SP,nn
.SAVED_SP       DEFW  0x0000                ;
                ;ei
                RETI

; ** Sets up everything for our IM2 service routine. Specifically, copies a JR
; ** instruction to 0xFFFF and a JP 0xF0FF to 0xFFF4
.INIT_ISR
                LD    HL,0xFFFF
                LD    (HL),0x18               ; Copies in our JR for JR FFF4
                LD     HL,0xFFF4
                LD    (HL),0xC3               ; JP (jump address filled-in
                                             ; during player initialization)
                RET


; *** DATA ***
;0xFE00   EQU 0xFE00
;0x0         EQU 0x0
.FREQ_TABLE          DEFB 0xFD,0xEE,0xE1,0xD4,0xC8,0xBD,0xB2,0xA8,0x9F,0x96,0x8E,0x86,0x7E
                     DEFB 0x77,0x70,0x6A,0x64,0x5E,0x59,0x54,0x4F,0x4B,0x47,0x43,0x3F
                     DEFB 0x3B,0x38,0x35,0x32,0x2F,0x2C,0x2A,0x27,0x25,0x23,0x21,0x1F
                     DEFB 0x1D,0x1C,0x1B,0x19,0x17,0x16,0x15,0x13,0x12,0x11,0x10,0x0F
                     DEFB 0x0E,0x0D,0x0C,0x01,0x00


.MUSICDATA        DEFW      PERCSTART
.SONGSTART        DEFW      PAT0
                  DEFW      0x0000
                  DEFW      SONGSTART

.PAT0     DEFW  PAT0C2
     DEFB 11,7
     DEFB 13,7
     DEFB 17,7
     DEFB 25,7
     DEFB 32,7
     DEFB 1,77
     DEFB 0x80
.PAT0C2
     DEFB 0x81,0
     DEFB 131,112
     DEFB 0x80
.PERCSTART   DEFW      DRM0
             DEFW      0x0000
             DEFW      PERCSTART

.DRM0
     DEFB 133,112
     DEFB 0x80
#endasm

