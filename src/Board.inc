		switch RomSection
;-------------------------------------------------------------------------------
; Board.inc:
; 
; Verwaltet den Buffer, welcher das Spielfeld repräsentiert.
; Bietet Funktionen um den Buffer zu befüllen und diesen auf dem LCD zu
; Zeichnen.
;-------------------------------------------------------------------------------


;-------------------------------------------------------------------------------
; drawCell:
;
; Zeichnet eine Zelle auf den LCD-Display.
;
; @param cellAddress: Relative Zellenadresse vom Anfang des Buffers
;-------------------------------------------------------------------------------

drawCell
		pshx
		pshy 
		psha
		pshb
		
		; Berechne die absolute Adresse der Zelle im Buffer
		ldy		#board
		xgdy
		addd	cellAddress
		xgdy
		
		; Zeile und Spalte der Zelle berechnen und in D (Spalte) 
		; und X (Zeile) Speichern
		ldd		cellAddress
		ldx		#rowLength
		idiv
		
			
		psha
		pshb
		
		; Zeile setzen	
		xgdx
		tba
		jsr		LCDSetRow
		
		pulb
		pula
		
		; Bildschirm Versatz mit berücksichtigen
		tba
		adda	#boardOffset
		ldb		#0
drawCellLoop
		
		; Spalte setzen
		jsr		LCDSetCol
		
		; Zeichen (Byte) an LCD senden
		psha
		lda		0,y
		jsr 	LCDdata
		pula
		
		iny
		inca
		incb
		cmpb	#8
		bne		drawCellLoop
		
		pulb
		pula
		puly
		pulx
		rts
		
		
;-------------------------------------------------------------------------------
; cellToBuffer:
;
; Schreibt eine leere "Zelle" (8 * 8 Bit) an der Position angegben 
; durch "cellAdress" in den Buffer "Board".
;
; @param cellAddress: Relative Zellenadresse vom Anfang des Buffers
;-------------------------------------------------------------------------------

cellToBuffer
		pshx 
		psha
		pshb
		
		; Berechne Adresse der Zelle im Buffer
		ldx		#board
		xgdx
		addd	cellAddress
		xgdx
		
		
		lda		#%11111111		; linker Rand der Zelle
		staa	0,x
		
		lda		#%10000001
		staa	1,x
		
		lda		#%10000001
		staa	2,x
		
		lda		#%10000001
		staa	3,x
		
		lda		#%10000001
		staa	4,x
		
		lda		#%10000001
		staa	5,x
		
		lda		#%10000001
		staa	6,x
		
		lda		#%11111111 		; rechter Rand der Zelle
		staa	7,x
	
		pulb
		pula
		pulx
		rts
		
		
;-------------------------------------------------------------------------------
; creatGrid:
;
; Füllt den komplettenden Buffer "Board" mit leeren Zellen.
;-------------------------------------------------------------------------------		
		
createGrid
		psha
		pshb
	
		ldd		#0
		std		cellAddress
createGridLoop	
		jsr		cellToBuffer
		addd	#byteSize
		std		cellAddress
		cmpd	#boardSize
		bne		createGridLoop
	
		pula
		pulb
		rts
	
		
;-------------------------------------------------------------------------------
; drawBoard:
;
; Zeichnet den kompletten Buffer auf den Display.
;-------------------------------------------------------------------------------

drawBoard
		psha
		pshb
	
		ldd		#0
		std		cellAddress
drawBoardLoop	
		jsr		drawCell
		addd	#byteSize
		std		cellAddress
		cmpd	#boardSize
		bne		drawBoardLoop
	
		pula
		pulb
		rts
	
;-------------------------------------------------------------------------------
; stone1ToBuffer:
;
; Schreibt einen Spielstein für Spieler1 (gefüllt) in den Buffer,
; der Spielstein wird dabei mit der leeren Zelle oder-Verknüpft.
;
; @param cellAddress: Relative Zellenadresse vom Anfang des Buffers
;-------------------------------------------------------------------------------

stone1ToBuffer
		pshx 
		psha
		pshb
		
		; Berechne Adresse der Zelle im Buffer
		ldx		#board
		xgdx
		addd	cellAddress
		xgdx
		
		lda		#%00011000		; Stein für Spieler 1 (gefüllt)
		oraa	2,x
		staa	2,x
		
		lda		#%00111100
		oraa	3,x
		staa	3,x
		
		lda		#%00111100
		oraa	4,x
		staa	4,x
		
		lda		#%00011000
		oraa	5,x
		staa	5,x
		
		pulb
		pula
		pulx
		rts
		
		
;-------------------------------------------------------------------------------
; stone2ToBuffer:
;
; Schreibt einen Spielstein für Spieler2 (leer) in den Buffer,
; der Spielstein wird dabei mit der leeren Zelle oder-Verknüpft.
;
; @param cellAddress: Relative Zellenadresse vom Anfang des Buffers
;-------------------------------------------------------------------------------

stone2ToBuffer
		pshx 
		psha
		pshb
		
		; Berechne Adresse der Zelle im Buffer
		ldx		#board
		xgdx
		addd	cellAddress
		xgdx
		
		lda		#%00011000		; Stein für Spieler 2 (leer)
		oraa	2,x
		staa	2,x
		
		lda		#%00100100
		oraa	3,x
		staa	3,x
		
		lda		#%00100100
		oraa	4,x
		staa	4,x
		
		
		lda		#%00011000
		oraa	5,x
		staa	5,x
	
		pulb
		pula
		pulx
		rts		
