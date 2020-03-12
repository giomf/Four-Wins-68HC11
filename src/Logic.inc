		switch	RomSection
;-------------------------------------------------------------------------------
; Logic.inc:
; 
; Bietet alle Funktionen zur Spiellogik (Berechnen des Gewinners, 
; Zugwechsel, etc.).
;-------------------------------------------------------------------------------


;-------------------------------------------------------------------------------
; getCellAddress:
;
; Rechnet eine Spielfeldkoordinate (X und y) in eine Adresse im Buffer um.
;
; @param Akku a: X-Wert der Spielfeldkoordinate
; @param Akku b: Y-Wert der Spielfeldkoordinate
; @return cellAddress: Die Adresse der Zelle relativ zur Bufferadresse 
;-------------------------------------------------------------------------------

getCellAddress				; Address = (b * 56) + (a * byteSize)
		psha
		pshb
		pshb
		
		ldab	#8
		mul
		std		cellAddress
		
		pulb
		
		ldaa	#rowLength
		mul
		addd	cellAddress
		std		cellAddress
		
		pulb
		pula
		rts
		
			
;-------------------------------------------------------------------------------
; setStone:
;
; Prüft, ob in die Spalte an der aktuellen Cursorposition ein Stein 
; gelegt werden kann. Wenn die Position gültig ist, wird ein Stein 
; gesetzt.
;
; @param cursorColumn: Die Spalte, in die ein Stein gesetzt werden soll
;-------------------------------------------------------------------------------			  

setStone					
		pshb
		psha							
		pshx
		pshy
		
		
		ldaa	#0
		ldab	cursorColumn
		subb	#boardOffset
		
		ldx		#byteSize
		idiv
		xgdx						;ergebnis der Division in d
		tba
		ldab	#5
		
		ldy		#6
freeCellLoop
		jsr 	getCellAddress		; Ergebnis in Variable cellAddress
		
		psha
		jsr		getCellOccupancy	
		cmpa	#0					; Wenn Leer dann wird der 
									; Stein gesetzt
		pula
		bne		checkNextCell
		
		psha
		ldaa		player			; Überprüfe welcher Spieler dran ist
		cmpa		#1
		pula
		bne			Player2Stone

Player1Stone	
		jsr		stone1ToBuffer	
		bra		setStoneEnd
		
Player2Stone	
		jsr		stone2ToBuffer	
		bra		setStoneEnd

checkNextCell	
		
		decb
		dey
		bne		freeCellLoop
		
setStoneEnd
		
		; speichern der Spielfeldkoordinaten zum prüfen ob gewonnen wurde
		staa	coordx			
		stab	coordy
		
		jsr		drawBoard
	
		puly
		pulx
		pula
		pulb
		rts
		
	
;-------------------------------------------------------------------------------
; getCellOccupancy:
;
; Gibt die Belegung einer Zeller wieder.
;
; @param cellAddress: Die Zellenadresse
; @return Akku a: 0 = leer, 1 = Spieler1, 2 = Spieler2
;-------------------------------------------------------------------------------		
		
getCellOccupancy								; Rückgabewert in a 
		pshb
		pshx
		
		ldx		#board
		xgdx
		addd	cellAddress
		xgdx
		; 3. Byte der Zelle: prüfen ob Zelle nur Nullen enthällt (keinen Stein)
		lda		3,x			
		; erste und letzte 1 repräsentieren den Zellenrand	
		cmpa	#%10000001	
		beq		isEmpty
		cmpa	#%10111101
		beq		isPlayer1
		cmpa	#%10100101
		beq		isPlayer2
			
isEmpty			
		ldaa	#0
		bra		getCellOccupancyEnd
		
isPlayer1			
		ldaa	#1
		bra		getCellOccupancyEnd
		
isPlayer2			
		ldaa	#2
		bra		getCellOccupancyEnd
	
getCellOccupancyEnd	
		
		pulx
		pulb
		rts
		

		
;-------------------------------------------------------------------------------
; nextPlayer:
;
; Wechselt den aktuellen Spieler. Wenn Spieler1 am Zug ist, wird auf 
; Spieler2 gewechselt und umgekehrt.
;
; @param player: Der Spieler, der aktuell am Zug ist
; @return player: 1 = Spieler1, 2 = Spieler2
;-------------------------------------------------------------------------------		
		
nextPlayer
			psha
			
			ldaa	player			
			cmpa	#1
			beq		setPlayer2
			
setPlayer1	
			ldaa	#1
			staa	player
			bra		nextPlayerEnd

setPlayer2			
			ldaa	#2
			staa	player
			
nextPlayerEnd			
			
			pula
			rts
			
			
;-------------------------------------------------------------------------------
; checkWin:
;
; Überprüft, ob der aktuelle Spieler eine Viererreihe erreicht hat
;
; @param coordx: Die X-Koordinate des gesetzten Steins
; @param coordy: Die Y-Koordinate des gesetzten Steins
; @return Akku a: 0 = False, 1 = True
;-------------------------------------------------------------------------------				

checkWin
		
		pshb
		
		
		; Vertikal
		ldaa	#0					; Richtung unten
		ldab	#1
		jsr		getLineLength
		cmpa	#4
		bhs		checkWinTrue
		
		
		; Horizontal
		ldaa	#1					; Richtung Rechts
		ldab	#0
		jsr		getLineLength
		tab
		pshb
		ldaa	#-1					; Richtung Links
		ldab	#0
		jsr		getLineLength
		pulb
		deca
		aba
		cmpa	#4
		bhs		checkWinTrue	
		
		
		; Diagonal Oben Rechts nach unten Links
		ldaa	#1					; Richtung Oben Rechts
		ldab	#-1
		jsr		getLineLength
		tab
		pshb
		ldaa	#-1					; Richtung unten Links
		ldab	#1
		jsr		getLineLength
		pulb
		deca
		aba
		cmpa	#4
		bhs		checkWinTrue
		
		
		; Diagonal unten Rechts nach links Oben
		ldaa	#-1					; Richtung links Oben
		ldab	#-1
		jsr		getLineLength
		tab
		pshb
		ldaa	#1					; Richtung unten Rechts
		ldab	#1
		jsr		getLineLength
		pulb
		deca
		aba
		cmpa	#4
		bhs		checkWinTrue
	
		bra		checkWinFalse
		
checkWinTrue	
		ldaa	#1
		staa	playerWonFlag
		bra		checkWinEnd
		
checkWinFalse	
		ldaa	#0
				
checkWinEnd		
		
		pulb
		rts
		
		
;-------------------------------------------------------------------------------
; getLineLength:
;
; Zählt die Anzahl der verbundenen Steine des aktuellen Spielers in einer 
; Reihe. 
;
; @param Akku a: X-Richtung in die gelaufen werden soll
; @param Akku b: Y-Richtung in die gelaufen werden soll
; @param player: Der Spieler, der aktuell am Zug ist
; @return Akku a: Anzahl an Steinen die in einer Reihe gefunden wurden
;-------------------------------------------------------------------------------

getLineLength
		
		pshb
		pshy
		pshx
			
		staa	xoffset
		stab	yoffset
		ldaa	coordx
		ldab	coordy
		
		ldx		#0						; Zähler für die Länge einer Linie
		
getLineLengthLoop

		adda	xoffset			
		addb	yoffset
		
		psha
		jsr		isOutOfBorder			; Überprüfe ob nächster Stein sich noch 
		cmpa	#1						; innerhalb des Spielfeldes befindet
		pula
		beq		getLineLengthLoopEnd
		
		jsr		getCellAddress
		psha
		jsr		getCellOccupancy		; Überprüft, ob die nächste Zelle
		cmpa	player					; den Stein des Spielers enthält.
		pula
		bne		getLineLengthLoopEnd
		
		inx								; Laufe nur 3 Steine lang
		cpx		#3						
		bne		getLineLengthLoop
		
getLineLengthLoopEnd
	
		xgdx
		tba	
		inca							; Inkrementiere A, sodass der neu 
		bra		getLineLengthEnd		; gelegte Stein mitgezählt wird
		
getLineLengthEnd

		pulx
		puly
		pulb
		rts
		
		
;-------------------------------------------------------------------------------
; isOutOfBorder:
;
; Gibt an ob die übergebenen Koordinaten sich innerhalb oder ausserhalb des
; Spielfeldes befinden.
;
; @param Akku a: X-Kooridnate des Spielfeldes
; @param Akku b: Y-Kooridnate des Spielfeldes
; @return Akku a: 0 = innerhalb des Spielfeldes, 1 = ausserhalb des Spielfeldes
;-------------------------------------------------------------------------------
		
isOutOfBorder
		
		pshb
		
		cmpa	#-1
		beq		isOutOfBorderTrue
		cmpa	#7
		beq		isOutOfBorderTrue
		
		cmpb	#-1
		beq		isOutOfBorderTrue
		cmpb	#6
		beq		isOutOfBorderTrue
		
		bra		isOutOfBorderFalse

isOutOfBorderTrue
		ldaa	#1
		bra		isOutOfBorderEnd
		
isOutOfBorderFalse
		ldaa	#0
		bra		isOutOfBorderEnd
		
isOutOfBorderEnd
	
		pulb
		rts
		
;-------------------------------------------------------------------------------
; playerwon:
;
; Gibt an ob die übergebenen Koordinaten sich innerhalb oder ausserhalb des
; Spielfeldes befinden.
;
; @param Akku a: X-Kooridnate des Spielfeldes
; @param Akku b: Y-Kooridnate des Spielfeldes
; @return Akku a: 0 = innerhalb des Spielfeldes, 1 = ausserhalb des Spielfeldes
;-------------------------------------------------------------------------------		
playerwon
	
	; Flag setzen das die Eingabe blockiert und nur Reset erlaubt
	jsr		LCDClr
	jsr		drawBoard
	jsr	drawWinner
	
	rts
