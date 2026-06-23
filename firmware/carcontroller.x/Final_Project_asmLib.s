.include "xc.inc"

.text                       ;BP (put the following data in ROM(program memory))

; This is a library, thus it can *not* contain a _main function: the C file will
; define main().  However, we will need a .global statement to make available ASM
; functions to C code.
; All functions utilized outside of this file will need to have a leading 
; underscore (_) and be included in a comment delimited list below.
.global _example_public_function, _second_public_function
    
.global _write_0, _write_1, _yyw_wait_100us, _yyw_wait_1ms
     

_yyw_wait_100us:
    
    push W8
    mov #533, w8        
    loop_100us: 
      dec w8, w8          ; 
      bra nz, loop_100us  ; 
      pop     w8              ; 
      return

_yyw_wait_1ms:
    push w8 
    mov #0x14D5, w8
    
    loop_1ms:
      dec w8, w8
      bra nz, loop_1ms
      pop w8
    return
    














