		switch	RomSection
;-------------------------------------------------------------------------------
; Cursor.inc:
; 
; Verwaltet die Curosrfunktionen.
; Bietet Funktionen um den Cursor zu verschieben und diesen auf dem LCD zu
; Zeichnen.
;-------------------------------------------------------------------------------


;-------------------------------------------------------------------------------
; resetCursor:
;
; Setzt den Cursor auf die Mitte des Spielfelds und aktualisiert 
; das Display.
;
; @return cursorColumn: Die mittlere Spalte des Spielfeldes
;-------------------------------------------------------------------------------
		
resetCursor
		psha
		
		ldaa	#boardMiddleColumn
		adda	#boardOffset
		staa	cursorColumn
		jsr		drawCursor
		
		pula
		rts
		
		
;---------------------------------------------------------------------------------------
; moveCursorLeft:
;
; Bewegt den Cursor um eine Spalte nach links, bei der äußerst linken 
; Spalte wird der Cursor nach ganz rechts gesetzt.
; 
; @param cursorColumn: Die aktuelle Cursorposition
; @return cursorColumn: Die neue Cursorposition um eine Zelle nach links geschoben
;---------------------------------------------------------------------------------------

moveCursorLeft
		psha
		jsr		deleteCursor
		
		ldaa	cursorColumn
		; vergleicht die Cursorposition mit dem Wert des äußerst linken 
		; Spielfeldrandes "boardOffset"
		cmpa	#boardOffset		
		beq		moveCursorToEnd
		
		suba	#byteSize
		staa	cursorColumn
		jmp		endMoveCursorLeft
		
moveCursorToEnd
		; setzt den Cursor auf die äußerst rechte Position 
		; (-8 weil die Position immer den Anfang einer Zelle angibt)
		ldaa	#boardOffset + rowLength - byteSize
		staa	cursorColumn
		
endMoveCursorLeft
		
		jsr		drawCursor
		
		pula
		rts
		
		
;---------------------------------------------------------------------------------------
; moveCursorRight:
;
; Bewegt den Cursor um eine Spalte nach rechts, bei der äußerst rechten 
; Spalte wird der Cursor nach ganz links gesetzt.
; 
; @param cursorColumn: Die aktuelle Cursorposition
; @return cursorColumn: Die neue Cursorposition um eine Zelle nach rechts geschoben
;---------------------------------------------------------------------------------------

moveCursorRight
		psha
		jsr		deleteCursor
		
		ldaa	cursorColumn
		; vergleicht den Cursor mit der äußerst rechten Position 
		; (-8 weil die Position immer den Anfang einer Zelle angibt)
		cmpa	#boardOffset + rowLength - byteSize
		beq		moveCursorToStart
		
		adda	#byteSize
		staa	cursorColumn
		jmp		endMoveCursorRight
		
moveCursorToStart
		ldaa	#boardOffset
		staa	cursorColumn

endMoveCursorRight

		jsr		drawCursor

		pula
		rts


;-------------------------------------------------------------------------------
; drawCursor:
;
; Gibt den Cursor an der  aktuelle Cursorposition auf dem Display aus.
;
; @param cursorColumn: Die aktuelle Cursorposition
;-------------------------------------------------------------------------------

drawCursor
		psha
		
		ldaa	#cursorRow
		jsr		LCDSetRow
		
		ldaa	cursorColumn
		adda	#1 
		jsr		LCDSetCol
		lda		#%01100000
		jsr 	LCDdata
		
		ldaa	cursorColumn
		adda	#2
		jsr		LCDSetCol
		lda		#%01110000
		jsr 	LCDdata
		
		ldaa	cursorColumn
		adda	#3
		jsr		LCDSetCol
		lda		#%01111100
		jsr 	LCDdata
		
		ldaa	cursorColumn
		adda	#4
		jsr		LCDSetCol
		lda		#%01111100
		jsr 	LCDdata
		
		ldaa	cursorColumn
		adda	#5
		jsr		LCDSetCol
		lda		#%01110000
		jsr 	LCDdata
		
		ldaa	cursorColumn
		adda	#6
		jsr		LCDSetCol
		lda		#%01100000
		jsr 	LCDdata
		
		pula
		rts
		
		
;-------------------------------------------------------------------------------
; deleteCursor:
;
; Löscht die aktuelle Cursorposition auf dem Display
;
; @param cursorColumn: Die aktuelle Cursorposition
;-------------------------------------------------------------------------------

deleteCursor
		psha
		
		ldaa	#cursorRow
		jsr		LCDSetRow
		
		ldaa	cursorColumn
		adda	#1
		jsr		LCDSetCol
		lda		#%00000000
		jsr 	LCDdata
		
		ldaa	cursorColumn
		adda	#2
		jsr		LCDSetCol
		lda		#%00000000
		jsr 	LCDdata
		
		ldaa	cursorColumn
		adda	#3
		jsr		LCDSetCol
		lda		#%00000000
		jsr 	LCDdata
		
		ldaa	cursorColumn
		adda	#4
		jsr		LCDSetCol
		lda		#%00000000
		jsr 	LCDdata
		
		ldaa	cursorColumn
		adda	#5
		jsr		LCDSetCol
		lda		#%00000000
		jsr 	LCDdata
		
		ldaa	cursorColumn
		adda	#6
		jsr		LCDSetCol
		lda		#%00000000
		jsr 	LCDdata
		
		pula
		rts
