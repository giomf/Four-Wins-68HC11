;-------------------------------------------------------------------------------
; Viergewinnt.asm:
;
; @Author: Michael Persiehl (tinf102296), Guillaume Fournier-Mayer (tinf101922)
; Implementiert das Spiel "4-Gewinnt" für 2 Spieler auf dem Motorola 68HC11.
;-------------------------------------------------------------------------------


;-------------------------------------------------------------------------------
;   Register- und Makrodefinitionen
;-------------------------------------------------------------------------------

 include trainer11Register.inc


;-------------------------------------------------------------------------------
;   Definition von Variablen im RAM
;-------------------------------------------------------------------------------

RamSection		Section
				org $0040
				
ramBegin		equ		*				; Anfang des Rams
board 			ds.b	boardSize		; Der Buffer für das Spielfeld
cellAddress		ds.w	1				; Die Adresse für eine Zeile des 
										; Spielfeldes (als Funktionsparameter)

cursorColumn	ds.b	1				; Horizontale Position des Cursors 
										; in Pixeln

debugCellAdress	ds.w	1				; Die Adresse für eine Zeile des 
										; Spielfeldes (Funktionsparameter 
										; fürs Debugging)

buttonFlag		ds.b	1				; Flag zur Flankenerkennung 
										; -> 1 = gedrückt, 0 = nicht gedrückt

timer			ds.b	1				; Timer zum entprellen der Tasten
player			ds.b	1				; Der Spieler der aktuell am Zug ist
										; (1 = Spieler 1, 2 = Spieler 2)

coordx			ds.b	1				; X-Koodrindate auf dem Spielfed
coordy			ds.b	1				; Y-Koodrindate auf dem Spielfed
xoffset			ds.b	1				; X-Offset für Linienerkennung
yoffset			ds.b	1				; Y-Offset für Linienerkennung
playerWonFlag	ds.b	1				; Flag um Tasten zu sperren wenn ein 
										;Spieler gewonnen hat

RomSection		Section
				org $C000
			
				include Utils.inc
				include LCDutils.inc
				include Board.inc
				include Cursor.inc
				include Input.inc
				include	Logic.inc
			
				switch		RamSection
ramEnd			equ		*				; Ende des Rams


;-------------------------------------------------------------------------------
;   Beginn des Programmcodes im schreibgeschuetzten Teil des Speichers
;-------------------------------------------------------------------------------

					switch		RomSection
pageCmdMsk			dc.b		%10110000		; Steuerkommando fürs Display	
colCmdMskM			dc.b		%00010000		; Steuerkommando fürs Display
colCmdMskL			dc.b		%00000000		; Steuerkommando fürs Display
adrMask				dc.b		%00000111		; Maske

LCDCols 			equ			128				; Anzahl an horizontalen Pixeln des Displays
LCDRows 			equ			8				; Anzahl an vertikalen Bytes des Displays
boardSize			equ			336				; Byteanzahl des Buffers
byteSize			equ			8				; Größe eines Bytes in Bit
rowLength			equ			56				; Länge einer Zeile des Spielfeldes in Pixeln
cursorRow			equ			6				; Vertikale Position des Cursors in Byte
boardOffset			equ			36				; Der horizontale Versatz des Spielfeldes
boardMiddleColumn	equ			24				; Die mittlere Spalte des Spielfeldes


;-------------------------------------------------------------------------------
; Konstanten für Textausgabe auf dem LCD
; der erste Wert jeder Konstante repräsentiert deren Länge in Byte
;-------------------------------------------------------------------------------

turnText
		dc.b 		21,$02,$7E,$02,$00	; T
		dc.b 		$3C,$40,$40,$7C,$00	; u
		dc.b 		$7C,$08,$04,$08,$00	; r
		dc.b 		$7C,$08,$04,$78,$00	; n
		dc.b 		$28,$00				; :
        
playerText
		dc.b 		30,$7E,$12,$12,$0C,$00 	; P
		dc.b $02,$7E,$00				; l
		dc.b $20,$54,$54,$78,$00		; a
		dc.b $1C,$A0,$A0,$7C,$00		; y
		dc.b $38,$54,$54,$48,$00		; e
		dc.b $7C,$08,$04,$08,$00		; r
		dc.b $00,$00					; 
				
oneText	
		dc.b 4,$04,$7E,$00,$00			; 1

twoText	
		dc.b 5,$64,$52,$52,$4C,$00		; 2

wonText	
		dc.b 17,$00,$00,$3C,$40,$20
		,$40,$3C,$00	; w
		dc.b $38,$44,$44,$38,$00				; o
		dc.b $7C,$08,$04,$78,$00				; n


;-------------------------------------------------------------------------------
;   Hauptprogramm
;-------------------------------------------------------------------------------

initGame

		psha
		
		ldaa		#$0C				; Löscht das Terminal
		jsr 		putChar
		jsr			clrRam				; Überschreibt den Ram mit Nullen
		jsr			initSPI				; Initialisierung der SPI-Schnittstelle
		jsr			initLCD				; Initialisierung des Displays
		jsr			LCDClr				; Löscht das Display
		jsr			createGrid			; Schreibt das Spielfeld in den Buffer
		jsr			drawBoard			; Schreibt den Bufferinhalt auf den Display
		jsr			resetCursor			; Setz den Cursor in die Mitte des Spielfeldes
		ldaa		#1
		staa		player				; wechselt zu Spieler 1
		jsr			showText
		jsr			drawPlayer
		pula

		rts

Start	
		lds			#$3FFF				; Einstiegspunkt des Spieles
		jsr			initGame			
				
mainLoop								; Hauptschleife
		jsr			readInput
		bra			mainLoop
		
End		bra			*


;-------------------------------------------------------------------------------
;   Vektortabelle
;-------------------------------------------------------------------------------
VSection			Section
		org			VRESET		; Reset-Vektor
		dc.w		Start		; Programm-Startadresse

