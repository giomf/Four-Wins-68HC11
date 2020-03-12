		switch RomSection
;-------------------------------------------------------------------------------
; LCDutils.inc:
;	
; Bietet Funktionen um das Display zu initialisieren und anzusteuern.		
;-------------------------------------------------------------------------------
	
		
;-------------------------------------------------------------------------------
; initSPI:	
;		
; Initialisiert die SPI-Schnittstelle.
;-------------------------------------------------------------------------------		
		
initSPI 	
			psha
			ldaa #%00000011
			staa PORTD
			ldaa #%00111010
			staa DDRD ; TXD,MOSI,SCK,-SS = out
			ldaa #%01010010 ; 125kHz,Mode=0
			staa SPCR ; enable,no int
			pula
			rts
			
			
;-------------------------------------------------------------------------------	
; SPItransfer:
;
; Hilfsmethode für LCDsend	
;-------------------------------------------------------------------------------
			
SPItransfer 
			staa SPDR ; Byte senden
			
SPIwait2 	
			tst SPSR ; Warten mal anders
			bpl SPIwait2 ; auf MSB = SPIF = 1
			ldaa SPDR ; Antwortbyte holen
			rts					
			
			
;-------------------------------------------------------------------------------
; initLCD:
;		
; Initialisierung des Displays.
;-------------------------------------------------------------------------------

initLCD 	psha
			ldaa #$A2
			bsr LCDcommand
			ldaa #$A0
			bsr LCDcommand
			ldaa #$C8
			bsr LCDcommand
			ldaa #$24
			bsr LCDcommand
			ldaa #$81
			bsr LCDcommand
			ldaa #$2F
			bsr LCDcommand
			ldaa #$2F
			bsr LCDcommand
			ldaa #$AF
			bsr LCDcommand
			bclr PORTA,%01000000 	; PA6 = DIMM
			pula 					; Hintergrundbeleuchtung an
			rts
			
			
;-------------------------------------------------------------------------------
; LCDcommand:
;
; Sendet ein Kommandobyte an das Display.
;
; @param Akku a: Das zu sendende Kommandobyte
;-------------------------------------------------------------------------------

LCDcommand 
			bclr PORTD,%00100000 ; PD5, LCD_A0=0
			bra LCDsend
			
			
;-------------------------------------------------------------------------------
; LCDdata:
;		
; Sendet ein Datenbyte an das Display.
;
; @param Akku a: Das zu sendende Datenbyte
;-------------------------------------------------------------------------------

LCDdata 	
			bset PORTD,%00100000 ; PD5, LCD_A0=1
			bra LCDsend
			
			
;-------------------------------------------------------------------------------	
; LCDSend:
;
; Hilfsmethode für LCDcommand und LCDdata.		
; Sendet ein Daten- oder Kommando an das Display.
;
; @param Akku a: Das zu sendende Datenbyte
;-------------------------------------------------------------------------------

LCDSend		
			psha
			ldaa	PIO_C
			anda	#%00111111	; SPI_SEL = 0 = LCD
			staa	PIO_C
			pula
			
			psha
			jsr		SPItransfer
			
			ldaa	PIO_C		; Deselect LCD
			oraa	#%11000000	; SPI_SEL = 3 = EEPROM
			staa	PIO_C
			
			pula
			rts

;-------------------------------------------------------------------------------			
; LCDSetRow:
;
; Setzt die Zeile des LCD-Cursors.
;
; @param Akku a: Die Cursorzeile
;-------------------------------------------------------------------------------
LCDSetRow
			psha
			oraa	pageCmdMsk
			jsr		LCDcommand
			pula
			rts


;-------------------------------------------------------------------------------
; LCDsetColMSB:
;			
; Hilfsmethode für LCDSetCol.
; Setzt die ersten vier Bits der Spalte des LCD-Cursors.
;
; @param Akku a: die ersten vier Bits der Spalte des LCD-Cursors
;-------------------------------------------------------------------------------
			
LCDsetColMSB
			psha
			oraa	colCmdMskM
			jsr		LCDcommand
			pula
			rts			


;-------------------------------------------------------------------------------
; LCDsetColLSB:
;			
; Hilfsmethode für LCDSetCol.
; Setzt die letzten vier Bits der Spalte des LCD-Cursors.
;
; @param Akku a: die letzten vier Bits der Spalte des LCD-Cursors
;-------------------------------------------------------------------------------

LCDsetColLSB
			psha
			oraa	colCmdMskL
			jsr		LCDcommand
			pula
			rts
			
			
;-------------------------------------------------------------------------------
; LCDSetCol:
;			
; Setzt die Spalte des LCD-Cursors.
;
; @param Akku a: die Spalte des LCD-Cursors
;-------------------------------------------------------------------------------
				
LCDSetCol
			psha
			psha
			anda	#%00001111
			jsr		LCDSetColLSB
			pula
			lsra				;Shifte um 4 nach links
			lsra
			lsra
			lsra
			jsr		LCDSetColMSB
			pula
			rts

		
;-------------------------------------------------------------------------------			
; LCDClrRow:
;
; Hilfsmethode für LCDClr.
;-------------------------------------------------------------------------------

LCDClrRow	
			psha
			pshb
			ldab	#LCDCols
			ldaa	#0
			jsr		LCDSetCol
			
LCDClrRowFor
			jsr		LCDdata
			decb
			bne		LCDClrRowFor
				
			pulb
			pula
			rts
		
		
;-------------------------------------------------------------------------------
; LCDClr:
; 
; Löscht alle Pixel des LCD's.
;-------------------------------------------------------------------------------		
		
LCDClr
			psha
			ldaa	#0
LCDClrFor
			jsr		LCDSetRow
			jsr		LCDClrRow
			inca
			cmpa	#LCDRows
			bne		LCDClrFor
			
			ldaa	#0			; Setze Cursor wieder auf 0:0
			jsr		LCDSetCol
			jsr		LCDSetRow
			pula
			rts
			
			
;-------------------------------------------------------------------------------
; showText:
; 
; Schreibt die Texte für "Player" und "Turn:" auf den LCD.
;-------------------------------------------------------------------------------

showText
	
		psha
		pshb
		pshx
	
	
		ldd		#0			
		ldx		#turnText		; Text "Turn:" oben links auf LCD
		jsr		drawText
	
	
		ldaa	#1
		ldab	#0
		ldx		#playerText		; Text "Player" in die 2. Zeile, links auf LCD
		jsr		drawText
	
		pulx
		pulb
		pula
	
		rts
		

;-------------------------------------------------------------------------------
; drawText:
;
; Zeichnet den Text an die übergebene Zeile und Spalte des Displays
;
; @param Akku a: Zeile des LCD's
; @param Akku b: Spalte des LCD's
; @param Register x: Adresse des Textes im Speicher
;-------------------------------------------------------------------------------

drawText

		psha
		pshb
		pshx
		
		jsr		LCDSetRow
		tba						; nach setzen der Zeile, Spalte in A schreiben
		ldab	0,x
		inx						; erstes Byte in X ist die Länge des Texts
		
drawTextLoop
		
		jsr		LCDSetCol
		psha
		ldaa	0,x
		jsr		LCDdata
		pula
		inca
		inx
		decb
		bne		drawTextLoop
		
		pulx
		pulb
		pula
		rts
		
		
;-------------------------------------------------------------------------------
; drawPlayer:
;
; Schreibt die Spielernummer des Spielers, welcher gerade am Zug ist.
;
; @param player: Die Nummer des Spielers, welcher gerade am Zug ist
;-------------------------------------------------------------------------------		

drawPlayer

		psha
		pshb
		pshx
		pshy
		
		
		ldaa	#1
		ldy		#playerText
		ldab	0,y
		incb
		
		psha
		ldaa	player
		cmpa	#2
		pula
		
		beq		drawPlayer2
		
drawPlayer1
		ldx		#oneText
		jsr		drawText
		bra		drawPlayerEnd
drawPlayer2		
		ldx		#twoText
		jsr		drawText
drawPlayerEnd
		
		puly
		pulx
		pulb	
		pula
		
		rts
		
		
;-------------------------------------------------------------------------------
; drawWinner:
;
; Schreibt eine Nachricht an den Spieler, welcher das Spiel gewonnen hat, 
; auf den LCD.
; Löscht alle anderen Texte vom LCD und sperrt alle Tasten außer der 
; Reset-Taste.
;
; @param player: Die Nummer des Spielers, welcher gewonnen hat
;-------------------------------------------------------------------------------

drawWinner
		psha
		pshb
		pshx
		
		
		ldaa 	#6				; Zeichne "Spieler"
		ldab	#boardOffset
		ldx		#playerText
		jsr		drawText
		
		
		addb	0,x				; Zeichne die Spielernummer
		psha
		ldaa	player
		cmpa	#2
		pula
		beq		drawWinner2		; bestimmen welcher Spieler gewonnen hat
		
drawWinner1						; Fall Spieler1 gewonnen
		ldx		#oneText
		jsr		drawText
		bra		drawNumberEnd
drawWinner2						; Fall Spieler2 gewonnen
		ldx		#twoText
		jsr		drawText

drawNumberEnd		
		
		addb	0,x
		ldx		#wonText
		jsr		drawText
		
		pulx
		pulb
		pula
		rts
