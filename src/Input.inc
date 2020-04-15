		switch	RomSection
;-------------------------------------------------------------------------------
; Input.inc:
;
; Verarbeitet die Benutzereingaben und reagiert dementsprechend.
;-------------------------------------------------------------------------------


;-------------------------------------------------------------------------------
; readInput:
;
; Liest das Datenbyte des Tastenfeldes (PIO_B) aus und reagiert entsprechend
; auf die Taste.
;
; @param PIO_B: Das Signal Byte der 8 Tasten in 1 aus n Kodierung
;-------------------------------------------------------------------------------

readInput
		psha
		pshb
	
		ldaa	PIO_B					; Lese Button byte ein
		ldab	buttonFlag				; Lese ButtonfLag ein
		
		cmpb	#1						; 1 Wenn Taste gedrückt
		beq		testButtonRelease
		
button4									; Wenn Taste 4 gedrückt, reset 
										; des Spiels
		cmpa	#%11101111 
		bne		button0
		jsr		setButtonFlag
		jsr		initGame
		bra		readInputEnd
		
button0									; Wenn Taste 0 gedrückt, setze Stein 
		tst		playerWonFlag			; alle Tasten ausser reset sperren wenn 
										; das Spiel gewonnen ist
		bne		readInputEnd
		
		cmpa	#%11111110
		bne		button1
		jsr		setButtonFlag
		jsr		setStone
		
		ldab	coordy					; testen ob ein Stein gesetzt werden 
										; konnte, wenn nicht KEIN 
										; Spielerwechsel / checkWin
		cmpb	#$FF
		beq		readInputEnd
		
		jsr		checkWin
		cmpa	#1
		beq		someoneWon
		
		jsr		nextPlayer
		jsr		drawPlayer
		
		bra		readInputEnd		
		
button1									; Wenn Taste 1 gedrückt, setze Flag 
										; und bewege Cursor nach Links
		cmpa	#%11111101
		bne		button3
		jsr		setButtonFlag
		jsr		moveCursorLeft
		bra		readInputEnd
button3									; Wenn Taste 3 gedrückt, setze Flag 
										; und bewege Cursor nach Rechts
		cmpa	#%11110111 
		bne		readInputEnd
		jsr		setButtonFlag
		jsr		moveCursorRight
		bra		readInputEnd
		
			
testButtonRelease						
		jsr		incTimer				; entprellen der Tasten mit einem Timer
		ldab	timer
		cmpb	#250
		bne		readInputEnd
		jsr		resetTimer
		
		cmpa	#%11111111				; Überprüfe ob Taste losgelassen 
										; wurde -> Reset Buttonflag
		bne		readInputEnd
		jsr		resetButtonFlag
		
		
		
		bra		readInputEnd
someoneWon	
		jsr		playerwon
			
readInputEnd		

		pulb
		pula
		rts	
		
		
;-------------------------------------------------------------------------------
; setButtonFlag:
;
; Setzt ein Flag das eine Taste gedrückt -und bisher noch nicht 
; losgelassen wurde.
;
; @return buttonFlag: auf 1 gesetzt
;-------------------------------------------------------------------------------		
		
setButtonFlag
		psha
		
		ldaa	#1
		staa	buttonFlag
		
		pula
		rts
		
		
;-------------------------------------------------------------------------------
; resetButtonFlag:
;
; Setzt das Buttonflag zurück.
;
; @return buttonFlag: auf 0 gesetzt
;-------------------------------------------------------------------------------		

resetButtonFlag
		psha
		
		ldaa	#0
		staa	buttonFlag
		
		pula
		rts
		
		
;-------------------------------------------------------------------------------
; incTimer:
;
; Inkrementiert einen Timer zum entprellen der Tasten.
;
; @param timer: der Timer der inkrementiert werden soll
; @return timer: der inkrementierte Timer
;-------------------------------------------------------------------------------

incTimer
		psha
		
		ldaa	timer
		adda	#1
		staa	timer
		
		pula
		rts
		
		
;-------------------------------------------------------------------------------
; resetTimer:
;
; Setzt den Timer zum entprellen der Tasten zurück.
;
; @param timer: der Timer der zurückgesetzt werden soll
; @return timer: der zurückgesetzte Timer
;-------------------------------------------------------------------------------		
		
resetTimer
		psha
		
		ldaa	#0
		staa	timer
		
		pula
		rts
