	.xlist
;
FALSE   equ     0
TRUE    equ     NOT FALSE
??????  equ     0
;
;
;


const   MACRO   name,N
name     equ    (N)
	 public name
	endm
;
;
;
warn    MACRO   text
	 if1
	  .printx % text
	 endif
	endm
;
;
;
error   MACRO   text
	 if1
	  .error text
	 endif
	endm
;
;
;
pr_dec  MACRO   msg1,N,msg2
	 warn    <msg1 N msg2>
	endm
;
;
pr_hex  MACRO   msg1,N,msg2
	 warn    <msg1 N!h msg2>
	endm
;
;
code    MACRO   instr,arg

_code_ defl 0

	ifidn <instr>,<RET>
_code_ defl 0C9h
    endif

	ifidn <instr>,<ret>
_code_ defl 0C9h
    endif

    ifidn <instr>,<CPI>
_code_ defl 0FEh
    endif

    ifidn <instr>,<cpi>
_code_ defl 0FEh
    endif

    ifidn <instr>,<sta>
_code_ defl 032h
    endif

    ifidn <instr>,<call>
_code_ defl 0CDh
    endif

    ifidn <instr>,<CALL>
_code_ defl 0CDh
    endif

    ifidn <instr>,<jz>
_code_ defl 0CAh
    endif

    ifidn <instr>,<ori>
_code_ defl 0F6h
    endif

    ifidn <instr>,<CNZ>
_code_ defl 0C4h
    endif

    ifidn <instr>,<JMP>
_code_ defl 0C3h
    endif

    ifidn <instr>,<push>
    ifidn <arg>,<psw>
_code_ defl 0F5h
    endif
    endif

    ifidn <instr>,<MVI>
    ifidn <arg>,<A>
_code_ defl 3Eh
    endif
    ifidn <arg>,<M>
_code_ defl 36h
    endif
    ifidn <arg>,<C>
_code_ defl 0Eh
    endif
    endif

    ifidn <instr>,<mvi>
    ifidn <arg>,<a>
_code_ defl 3Eh
    endif
    ifidn <arg>,<b>
_code_ defl 06h
    endif
    ifidn <arg>,<m>
_code_ defl 36h
    endif
    ifidn <arg>,<c>
_code_ defl 0Eh
    endif
    endif

    ifidn <instr>,<LXI>
    ifidn <arg>,<H>
_code_ defl 21h
    endif
    ifidn <arg>,<SP>
_code_ defl 31h
    endif
    ifidn <arg>,<HL>
_code_ defl 21h
    endif
    endif

    ifidn <instr>,<lxi>
    ifidn <arg>,<h>
_code_ defl 21h
    endif
    ifidn <arg>,<hl>
_code_ defl 21h
    endif
    endif

    if _code_ eq 0
    if1
    .printx Undefined CODE arguments!!!!!!!!!!!!!!!!!!!!!!!!!!!
	.printx instr,arg
    endif
    endif

db _code_

endm
;
;
;-----------------------------------------------------------------------------
;
OFFSET  equ     4100h
PHASED  defl    FALSE
;
rammod  MACRO
PHASED   defl    TRUE
	 .phase  $-OFFSET
	endm
;
;
start:
;
finish  MACRO   name
	 if      PHASED
	  .dephase
	 endif
	 if1
	  pr_dec  <Size of module "&name" is>,%($-start),<bytes>
	 endif
	endm
;
;

;--- The "proc" MACRO is used to define a label that is invoked
;    at RAM in page 0 (when PHASED) or at ROM bank 4 (when not PHASED).

proc    MACRO   name                    ;;Macro for declaring a public
	if	PHASED
name&:                                  ;; label to be called.
	 .dephase
?&name&::
	 .phase  $-OFFSET
	else
name&:		;;For local call
?&name&::	;;For external call
	endif
	endm
;
;
;
pcall   MACRO   cc,name                 ;;Macro for calling an external
					;; routine.
	 ifb    <name>
	  call   ?&cc&##-OFFSET         ;;Unconditional CALL.
	 else
	  call   cc,?&name&##-OFFSET    ;;Conditional CALL.
	 endif
	endm
;
;
;

; STROUT implementation with the ESC-Y bug corrected.
; It is declared as as MACRO because it is defined
; twince in the kernel (bdos.mac, char.mac)
; and once more in MSXDOS2.SYS.
;
; OUTDIR is the routine used to print a character.
; RETDIR is the routine jumped to at end. If 0, ret is generated instead.
; LOADCE: if specified, E and C registers are loaded prior to calling CONOUT.

do_strout	MACRO	OUTDIR,RETDIR,LOADCE
	local	stro_init
	local	STROU2
	local	ST10

stro_init:
	LD	A,(DE)
	INC	DE
	cp	ESC
	jr	nz,STROU2

	call	ST10		;Print 'ESC'
	ld	a,(DE)
	inc	de
	cp	'Y'
	jr	nz,STROU2

	call	ST10		;Print 'Y'
	ld	a,(DE)
	inc	de
	call	ST10		;Print X coordinate
	ld	a,(DE)
	inc	de
	call	ST10		;Print Y coordinate

	jr	stro_init

STROU2:
	CP	'$'	
	ifidn <RETDIR>,<0>
	RET	Z
	else	
	JP	Z,&RETDIR&
	endif
	call	ST10
	JR	stro_init

ST10:	PUSH	DE

	ifnb	<LOADCE>
	LD	E,A
	LD	C,_CONOUT##
	endif

	CALL	&OUTDIR&
	POP	DE

	RET

	endm


djpnz MACRO address
    dec b
	jp nz,address
  endm

;------------------------------------------------------------------------------
;
	.list
;

UNDOC_OP equ	0	; user or not undocummented Z-80 instructions

	IF UNDOC_OP
;;;;;;;;;;;;;;;;;;;;;;;;;;;

LD_IXL_N MACRO N
	ld	ixl,N
	ENDM

LD_IXH_N MACRO N
	ld	ixh,N
	ENDM
	
LD_IYL_N MACRO N
	ld	iyl,N
	ENDM

LD_IYH_N MACRO N
	ld	iyh,N
	ENDM

LD_N_IXH MACRO N
	ld	N,ixh
	ENDM

LD_N_IXL MACRO N
	ld	N,ixl
	ENDM

LD_N_IYH MACRO N
	ld	N,iyh
	ENDM

LD_N_IYL MACRO N
	ld	N,iyl
	ENDM

CP_IXH MACRO
	cp	ixh
	ENDM

CP_IXL MACRO
	cp	ixl
	ENDM

CP_IYH MACRO
	cp	iyh
	ENDM

CP_IYL MACRO
	cp	iyl
	ENDM


OR_IYL MACRO
	or	iyl
	ENDM

DEC_IYL MACRO
	dec	iyl
	ENDM

	ELSE

;;;;;;;;;;;;;;;;;;;;;;;;;;;;

LD_IXH_N MACRO N
	ex 	(sp),ix
	ex 	(sp),hl
	ld	h,N
	ex 	(sp),hl
	ex 	(sp),ix
	ENDM

LD_IXL_N MACRO N
	ex 	(sp),ix
	ex 	(sp),hl
	ld	l,N
	ex 	(sp),hl
	ex 	(sp),ix
	ENDM

LD_IYH_N MACRO N
	ex 	(sp),iy
	ex 	(sp),hl
	ld	h,N
	ex 	(sp),hl
	ex 	(sp),iy
	ENDM

LD_IYL_N MACRO N
	ex 	(sp),iy
	ex 	(sp),hl
	ld	l,N
	ex 	(sp),hl
	ex 	(sp),iy
	ENDM

LD_N_IXH MACRO N
	ex 	(sp),ix
	ex 	(sp),hl
	ld  N,h
	ex 	(sp),hl
	ex 	(sp),ix
	ENDM

LD_N_IXL MACRO N
	ex 	(sp),ix
	ex 	(sp),hl
	ld  N,l
	ex 	(sp),hl
	ex 	(sp),ix
	ENDM

LD_N_IYH MACRO N
	ex 	(sp),iy
	ex 	(sp),hl
	ld  N,h
	ex 	(sp),hl
	ex 	(sp),iy
	ENDM

LD_N_IYL MACRO N
	ex 	(sp),iy
	ex 	(sp),hl
	ld  N,l
	ex 	(sp),hl
	ex 	(sp),iy
	ENDM

CP_IXH MACRO
	ex 	(sp),ix
	ex 	(sp),hl
	cp	h`
	ex 	(sp),hl
	ex 	(sp),ix
	ENDM

CP_IXL MACRO
	ex 	(sp),ix
	ex 	(sp),hl
	cp	l
	ex 	(sp),hl
	ex 	(sp),ix
	ENDM

CP_IYH MACRO
	ex 	(sp),iy
	ex 	(sp),hl
	cp	h
	ex 	(sp),hl
	ex 	(sp),iy
	ENDM

CP_IYL MACRO
	ex 	(sp),iy
	ex 	(sp),hl
	cp	l
	ex 	(sp),hl
	ex 	(sp),iy
	ENDM

OR_IYL MACRO
	ex 	(sp),iy
	ex 	(sp),hl
	or	l
	ex 	(sp),hl
	ex 	(sp),iy
	ENDM

DEC_IYL MACRO
	ex 	(sp),iy
	ex 	(sp),hl
	dec	l
	ex 	(sp),hl
	ex 	(sp),iy
	ENDM

	ENDIF
