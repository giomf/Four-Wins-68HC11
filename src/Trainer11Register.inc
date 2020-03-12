;***	Registerdefinitionen für den trainer11

PIO_A       equ  $8000
PIO_B       equ  $8001
PIO_C       equ  $8002
PIO_CONFIG  equ  $8003

  .ifndef MAP_PAGE
MAP_PAGE	equ $00		; wahlweise Offsets oder Adressen
  .endif

;*** Register ***

PORTA		equ $00+MAP_PAGE	; Port A Data Register

PIOC		equ $02+MAP_PAGE	; Parallel I/O Control Register
PORTC		equ $03+MAP_PAGE	; Port C Data Register
PORTB		equ $04+MAP_PAGE	; Port B Data Register
PORTCL		equ $05+MAP_PAGE	; Alternate Latched Port C

DDRC		equ $07+MAP_PAGE	; Port C Data Direction Register
PORTD		equ $08+MAP_PAGE	; Port D Data Register
DDRD		equ $09+MAP_PAGE	; Port D Data Direction Register
PORTE		equ $0A+MAP_PAGE	; Port E Data Register
CFORC		equ $0B+MAP_PAGE	; Timer Compare Force Register
OC1M		equ $0C+MAP_PAGE	; Action Mask Register
OC1D		equ $0D+MAP_PAGE	; Action Data Register
TCNT		equ $0E+MAP_PAGE	; Timer Counter Register
TIC1		equ $10+MAP_PAGE	; Timer Input Capture
TIC2		equ $12+MAP_PAGE	; Timer Input Capture
TIC3		equ $14+MAP_PAGE	; Timer Input Capture
TOC1		equ $16+MAP_PAGE	; Timer Output Compare
TOC2		equ $18+MAP_PAGE	; Timer Output Compare
TOC3		equ $1A+MAP_PAGE	; Timer Output Compare
TOC4		equ $1C+MAP_PAGE	; Timer Output Compare
TOC5		equ $1E+MAP_PAGE	; Timer Output Compare
TCTL1		equ $20+MAP_PAGE	; Timer Control Register 1
TCTL2		equ $21+MAP_PAGE	; Timer Control Register 2
TMSK1		equ $22+MAP_PAGE	; Timer Interrupt Mask Register 1
TFLG1		equ $23+MAP_PAGE	; Timer Interrupt Flag Register 1
TMSK2		equ $24+MAP_PAGE	; Timer Interrupt Mask Register 2
TFLG2		equ $25+MAP_PAGE	; Timer Interrupt Flag Register 2
PACTL		equ $26+MAP_PAGE	; Pulse Accumulator Control Register
PACNT		equ $27+MAP_PAGE	; Pulse Accumulator Count Register High
SPCR		equ $28+MAP_PAGE	; SPI Control Register
SPSR		equ $29+MAP_PAGE	; SPI Status Register
SPDR		equ $2A+MAP_PAGE	; SPI Data Register
BAUD		equ $2B+MAP_PAGE	; SCI Baud Rate Register
SCCR1		equ $2C+MAP_PAGE	; SCI Control Register 1
SCCR2		equ $2D+MAP_PAGE	; SCI Control Register 2
SCSR		equ $2E+MAP_PAGE	; SCI Status Register
SCDR		equ $2F+MAP_PAGE	; SCI Data Register
ADCTL		equ $30+MAP_PAGE	; A/D Control Register
ADR1		equ $31+MAP_PAGE	; A/D Converter Result Register 1
ADR2		equ $32+MAP_PAGE	; A/D Converter Result Register 2
ADR3		equ $33+MAP_PAGE	; A/D Converter Result Register 3
ADR4		equ $34+MAP_PAGE	; A/D Converter Result Register 4

OPTION		equ $39+MAP_PAGE	; System Configuration Options
COPRST		equ $3A+MAP_PAGE	; Arm/Reset COP Timer register
PPROG		equ $3B+MAP_PAGE	; EEPROM Programming Control Register
HPRIO		equ $3C+MAP_PAGE	; Highest Priority Interrupt Register
INIT		equ $3D+MAP_PAGE	; RAM and I/O Mapping
TEST1		equ $3E+MAP_PAGE	; Factory TEST Control Register
CONFIG		equ $3F+MAP_PAGE	; COP, ROM  and EEPROM Enables

;*** Vektoren ***

VSCI		equ $FFD6
VSPI		equ $FFD8
VPAI		equ $FFDA
VPAO		equ $FFDC
VTOF		equ $FFDE
VTIC4		equ $FFE0
VTOC4		equ $FFE2
VTOC3		equ $FFE4
VTOC2		equ $FFE6
VTOC1		equ $FFE8
VTIC3		equ $FFEA
VTIC2		equ $FFEC
VTIC1		equ $FFEE
VRTI		equ $FFF0
VIRQ		equ $FFF2
VXIRQ		equ $FFF4
VSWI		equ $FFF6
VIOT		equ $FFF8
VCOP		equ $FFFA
VCLK		equ $FFFC
VRESET		equ $FFFE



