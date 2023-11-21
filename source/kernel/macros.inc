	.xlist
;
FALSE   equ     0
TRUE    equ     NOT FALSE
??????  equ     0
;
;
;


const   macro   name,value
name     equ    (value)
	 public name
	endm
;
;
;
warn    macro   text
	 if1
	  .printx % text
	 endif
	endm
;
;
;
error   macro   text
	 if1
	  .error text
	 endif
	endm
;
;
;
pr_dec  macro   msg1,value,msg2
	 warn    <msg1 value msg2>
	endm
;
;
pr_hex  macro   msg1,value,msg2
	 warn    <msg1 value!h msg2>
	endm
;
;
code    macro   instr,arg

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
rammod  macro
PHASED   defl    TRUE
	 .phase  $-OFFSET
	endm
;
;
start:
;
finish  macro   name
	 if      PHASED
	  .dephase
	 endif
	 if1
	  pr_dec  <Size of module "&name" is>,%($-start),<bytes>
	 endif
	endm
;
;

;--- The "proc" macro is used to define a label that is invoked
;    at RAM in page 0 (when PHASED) or at ROM bank 4 (when not PHASED).

proc    macro   name                    ;;Macro for declaring a public
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
pcall   macro   cc,name                 ;;Macro for calling an external
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
; It is declared as as macro because it is defined
; twince in the kernel (bdos.mac, char.mac)
; and once more in MSXDOS2.SYS.
;
; OUTDIR is the routine used to print a character.
; RETDIR is the routine jumped to at end. If 0, ret is generated instead.
; LOADCE: if specified, E and C registers are loaded prior to calling CONOUT.

do_strout	macro	OUTDIR,RETDIR,LOADCE
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


djpnz macro address
    dec b
	jp nz,address
  endm

;------------------------------------------------------------------------------
;
	.list
;

	ifndef DISABLE_UNDOCUMENTED_OPCODES 

ld_ixh_a macro
	ld	ixh,a
	endm

ld_iyh_a macro
	ld	iyh,a
	endm
ld_iyh_c macro
	ld	iyh,c
	endm
ld_iyh_e macro
	ld	iyh,e
	endm
ld_iyh_d macro
	ld	iyh,d
	endm

ld_iy_de macro
	ld	iyl,e
	ld	iyl,d
	endm

ld_iyl_e macro
	ld	iyl,e
	endm
ld_iyl_a macro
	ld	iyl,a
	endm
ld_iyl_b macro
	ld	iyl,b
	endm
ld_iyl_1 macro
	ld	iyl,1
	endm
ld_iyl_c macro
	ld	iyl,c
	endm

ld_a_ixh macro
	ld	a,ixh
	endm

ld_a_ixl macro
	ld	a,ixl
	endm

ld_a_iyh macro
	ld	a,iyh
	endm

ld_a_iyl macro
	ld	a,iyl
	endm

cp_ixl macro
	cp	ixl
	endm

cp_iyh macro
	cp	iyh
	endm

cp_iyl macro
	cp	iyl
	endm

or_iyl macro
	or	iyl
	endm

dec_iyl macro
	dec	iyl
	endm

	else 

ld_ixh_a macro
	ex 	(sp),ix
	ex 	(sp),hl
	ld	h,a
	ex 	(sp),hl
	ex 	(sp),ix
	endm

ld_iyh_a macro
	ex 	(sp),iy
	ex 	(sp),hl
	ld	h,a
	ex 	(sp),hl
	ex 	(sp),iy
	endm
ld_iyh_c macro
	ex 	(sp),iy
	ex 	(sp),hl
	ld	h,c
	ex 	(sp),hl
	ex 	(sp),iy
	endm
ld_iyh_e macro
	ex 	(sp),iy
	ex 	(sp),hl
	ld	h,e
	ex 	(sp),hl
	ex 	(sp),iy
	endm
ld_iyh_d macro
	ex 	(sp),iy
	ex 	(sp),hl
	ld	h,d
	ex 	(sp),hl
	ex 	(sp),iy
	endm

ld_iy_de macro n
	ex 	(sp),iy
	ex 	(sp),hl
	ld	l,e
	ld	h,d
	ex 	(sp),hl
	ex 	(sp),iy
	endm

ld_iyl_e macro n
	ex 	(sp),iy
	ex 	(sp),hl
	ld	l,e
	ex 	(sp),hl
	ex 	(sp),iy
	endm
ld_iyl_a macro n
	ex 	(sp),iy
	ex 	(sp),hl
	ld	l,a
	ex 	(sp),hl
	ex 	(sp),iy
	endm
ld_iyl_b macro n
	ex 	(sp),iy
	ex 	(sp),hl
	ld	l,b
	ex 	(sp),hl
	ex 	(sp),iy
	endm
ld_iyl_1 macro n
	ex 	(sp),iy
	ex 	(sp),hl
	ld	l,1
	ex 	(sp),hl
	ex 	(sp),iy
	endm
ld_iyl_c macro n
	ex 	(sp),iy
	ex 	(sp),hl
	ld	l,c
	ex 	(sp),hl
	ex 	(sp),iy
	endm

ld_a_ixh macro n
	ex 	(sp),ix
	ex 	(sp),hl
	ld  a,h
	ex 	(sp),hl
	ex 	(sp),ix
	endm

ld_a_ixl macro
	ex 	(sp),ix
	ex 	(sp),hl
	ld  a,l
	ex 	(sp),hl
	ex 	(sp),ix
	endm

ld_a_iyh macro n
	ex 	(sp),iy
	ex 	(sp),hl
	ld  a,h
	ex 	(sp),hl
	ex 	(sp),iy
	endm

ld_a_iyl macro n
	ex 	(sp),iy
	ex 	(sp),hl
	ld  a,l
	ex 	(sp),hl
	ex 	(sp),iy
	endm

cp_ixl macro
	ex 	(sp),ix
	ex 	(sp),hl
	cp	l
	ex 	(sp),hl
	ex 	(sp),ix
	endm

cp_iyh macro
	ex 	(sp),iy
	ex 	(sp),hl
	cp	h
	ex 	(sp),hl
	ex 	(sp),iy
	endm

cp_iyl macro
	ex 	(sp),iy
	ex 	(sp),hl
	cp	l
	ex 	(sp),hl
	ex 	(sp),iy
	endm

or_iyl macro
	ex 	(sp),iy
	ex 	(sp),hl
	or	l
	ex 	(sp),hl
	ex 	(sp),iy
	endm

dec_iyl macro
	ex 	(sp),iy
	ex 	(sp),hl
	dec	l
	ex 	(sp),hl
	ex 	(sp),iy
	endm

	endif
