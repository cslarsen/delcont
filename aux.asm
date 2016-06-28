bits 64

%ifidn __OUTPUT_FORMAT__,macho64
  align 16
%endif

global _getrbp
global _getrsp

section .text

_getrbp:
  mov rax, rbp
  ret

_getrsp:
  mov rax, rsp
  ret
