		switch RomSection
;-------------------------------------------------------------------------------
; Utils.inc:
;
; Hilfsmethoden für das Terminal und den RAM.
;-------------------------------------------------------------------------------		
		
		
;-------------------------------------------------------------------------------
; clrRam:
;
; Setzt alle Bits im RAM auf 0.
;-------------------------------------------------------------------------------

clrRam	
		psha
		pshx
		
		ldaa	#0
		ldx		#ramBegin
		
clrRamLoop		
		staa	0,x
		inx
		cpx		#ramEnd
		bne		clrRamLoop
		
		pulx
		pula
		rts


;-------------------------------------------------------------------------------
; putChar:
;
; Zeichen in A auf dem Terminal ausgeben.
;
; @param Akku a: Das auszugebende Zeichen
;-------------------------------------------------------------------------------

putChar       brclr scsr, #$80, * ; Warte, bis der Ausgabepuffer leer ist
              staa scdr           ; Zeichen in A abschicken
              rts


;-------------------------------------------------------------------------------
; crlf:
;
; Zeilenumbruch auf dem Terminal ausgeben.
;-------------------------------------------------------------------------------

crlf          psha
              ldaa #13           ; CR
              jsr putChar
              ldaa #10           ; LF
              jsr putChar
              pula
              rts


;-------------------------------------------------------------------------------
; space:
;
; Leerzeichen auf dem Terminal ausgeben.
;-------------------------------------------------------------------------------

space         psha
              ldaa #32           ; Leerzeichen
              jsr putChar
              pula
              rts
