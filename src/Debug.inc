			switch RomSection		
;-------------------------------------------------------------------------------
; Debug.inc:
; Bietet Funktionen zum Debugging. Gibt Akkumulatoren, Register und CCR aus.
; Basisfunktionen von J.Voelker.
; Ergänzt um debugBuffer und debugCell.
;-------------------------------------------------------------------------------		
		

; -----------------------------------------------------------------------------
;   Vier Bits - ein Nibble - aus B als Hexadezimalzahl ausgeben
; -----------------------------------------------------------------------------

nibbletohex		dc.b	"0123456789ABCDEF"

putNibble     pshy
              psha
              pshb

              andb #$0F           ; Nibble maskieren,
              ldy #nibbletohex    ; aus der Zeichentabelle
              aby                 ; das passende Zeichen waehlen
              ldaa 0,y            ; und in A laden.

              jsr putChar

              pulb
              pula
              puly
              rts

; -----------------------------------------------------------------------------
;   Inhalt von B als Hexadezimalzahl ausgeben
; -----------------------------------------------------------------------------

putByte       pshb

              rorb               ; Oberes Nibble von B zur Ausgabe vorbereiten
              rorb
              rorb
              rorb
              jsr putNibble

              pulb

              jsr putNibble      ; Unteres Nibble von B ausgeben
              rts

; -----------------------------------------------------------------------------
;   Inhalt von D als Hexadezimalzahl ausgeben
; -----------------------------------------------------------------------------

putHex        psha
              ldaa #'$'
              jsr putChar
              pula

              pshb
              tab                ; Zuerst A ausgeben, den oberen Teil von D
              jsr putByte
              pulb

              jsr putByte        ; Anschliessend B ausgeben, den unteren Teil von D

              jsr space
              rts
              
; -----------------------------------------------------------------------------
;   Inhalt von D als Hexadezimalzahl ausgeben
; -----------------------------------------------------------------------------

putHexBuffer
				pshb
				tab                ; Zuerst A ausgeben, den oberen Teil von D
				jsr putByte
				pulb
				
				jsr putByte        ; Anschliessend B ausgeben, den unteren Teil von D

				jsr space
				rts
             

; -----------------------------------------------------------------------------
;   Carry-Flag als Binaerzahl ausgeben
; -----------------------------------------------------------------------------

putCarry      psha

              bcs putC1          ; War das Carry-Flag gesetzt ?

              ldaa #'0'
              bra putCAusgabe

putC1         ldaa #'1'

putCAusgabe   jsr putChar

              pula
              rts

; -----------------------------------------------------------------------------
;   Wert in A als Binaerzahl ausgeben
; -----------------------------------------------------------------------------

putBinary     psha
              ldaa #'%'
              jsr putChar
              pula

              psha

              rola               ; Das oberste Bit ins Carry-Flag rotieren
              bsr putCarry       ; und als Binaerzahl ausgeben.
              rola
              bsr putCarry
              rola
              bsr putCarry
              rola
              bsr putCarry
              rola
              bsr putCarry
              rola
              bsr putCarry
              rola
              bsr putCarry
              rola
              bsr putCarry

              ldaa #32           ; Leerzeichen anfuegen
              jsr putChar

              pula
              rts

; -----------------------------------------------------------------------------
;   Alle Register ausgeben, dabei nichts veraendern, auch die Flags bleiben erhalten
; -----------------------------------------------------------------------------

debug       ; Alter PC             (Stackpointer + 7)
              pshy               ; (Stackpointer + 5)
              pshx               ; (Stackpointer + 3)
              pshb               ; (Stackpointer + 2)
              psha               ; (Stackpointer + 1)
              tpa                ;                    Flags in A holen
              psha               ; (Stackpointer + 0)   und ebenfalls sichern.

              ; -----------------------------------------------

              tsx                ; Stackpointer in X holen

              ldaa #'P'          ; PC=
              jsr putChar
              ldaa #'C'
              jsr putChar
              ldaa #'='
              jsr putChar

              ldd 7,x            ; Den alten PC vom Stack in D laden
              jsr putHex

              ; -----------------------------------------------

              ldaa #'D'          ; D=
              jsr putChar
              ldaa #'='
              jsr putChar

              ldd 1,x            ; Gesicherten Inhalt von D vom Stack laden
              jsr putHex

              ; -----------------------------------------------

              ldaa #'X'          ; X=
              jsr putChar
              ldaa #'='
              jsr putChar

              ldd 3,x            ; Gesicherten Inhalt von X vom Stack laden
              jsr putHex

              ; -----------------------------------------------

              ldaa #'Y'          ; Y=
              jsr putChar
              ldaa #'='
              jsr putChar

              ldd 5,x            ; Gesicherten Inhalt von Y vom Stack laden
              jsr putHex

              ; -----------------------------------------------

              ldaa #'S'          ; SP=
              jsr putChar
              ldaa #'P'
              jsr putChar
              ldaa #'='
              jsr putChar

              xgdx               ; Den Stackpointer selbst in D laden
              jsr putHex

              ; -----------------------------------------------

              ldaa #'C'          ; CCR=
              jsr putChar
              ldaa #'C'
              jsr putChar
              ldaa #'R'
              jsr putChar
              ldaa #'='
              jsr putChar

              tsx                ; Nochmal den Stackpointer in X holen
              ldaa 0,x           ; Die auf dem Stack gesicherten Flags in A laden
              jsr putBinary

              ; -----------------------------------------------

              jsr crlf

              pula               ; Alten Inhalt
              tap                ;   der Flags zurueckholen
              pula
              pulb
              pulx
              puly               ; Alle Register wiederhergestellt.
              rts                ; Nach dem Ruecksprung ist auch der Stackpointer wieder wie vorher.
              
              
;-------------------------------------------------------------------------------
; debugBuffer:
;
; Gibt den gesamten Buffer in Hexadezimal auf dem Terminal aus.
;
; @param board: Das Board (Buffer)
;-------------------------------------------------------------------------------
debugBuffer
		pshx
		psha
		pshb
		
		ldx		#board
		ldd		#336
debugBufferLoop
		psha
		pshb
		ldd		0,x
		jsr		putHexBuffer	
		pulb
		pula
		inx
		inx
		subd	#2
		bne		debugBufferLoop	
		
		
		pulb
		pula
		pulx
		rts
		
		
;-------------------------------------------------------------------------------
; debugCell:
;
; Gibt eine Zelle des Buffers in Hexadezimal auf dem Terminal aus.
;
; @param debugCellAdress: Relative Zellenadresse vom Anfang des Buffers
; @param board: Das Board (Buffer)
;-------------------------------------------------------------------------------
debugCell
		pshx
		psha
		pshb
		
		ldx		#board
		xgdx
		addd	debugCellAdress
		xgdx
		
		ldd		#8
debugCellLoop
		psha
		pshb
		ldd		0,x
		jsr		putHexBuffer	
		pulb
		pula
		inx
		inx
		subd	#2
		bne		debugCellLoop
		
		jsr 	crlf
		pulb
		pula
		pulx
		rts
