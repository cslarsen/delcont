bits 64

%ifidn __OUTPUT_FORMAT__,macho64
  align 16
%endif

global _getrbp
global _getrsp
global _reify
global _reinstate

extern _malloc

section .text

_getrbp:
  mov rax, rbp
  ret

_getrsp:
  mov rax, rsp
  ret

_getrip:
  mov rax, [rsp]
  ret

; rdi: char* start
; rsi: struct continuation*
; rdx: int initial_value
_reify:
  ; calculate size
  sub rdi, rsp
  mov rcx, rdi

  ; allocate memory
  push rdx
  push rcx
  push rsi
  push rdi
  call _malloc
  pop rdi
  pop rsi
  pop rcx
  pop rdx

  ; store in continuation
  mov [rsi], rcx ; p->size
  mov [rsi + 8], rax ; p->stack

  ; copy stack frames
  mov rbx, rsi
  mov rdi, rax ; to p->stack
  mov rsi, rsp ; from rsp
  cld
  rep movsb

  ; save the most important registers
  ; the following does not work on osx (bug?)
  ;mov qword [rsi + 2*8], 0 ; .end

  mov [rbx + 3*8], rsp
  mov [rbx + 4*8], rbp
  ; ... todo: more registers
  call _getrip
  add rax, 8 ; make rax point to .end
  mov [rbx + 2*8], rax

.end:
  ; return initial/new value (rdx coincides with reinstate)
  mov rax, rdx
  ret

; rdi: char* start
; rsi: struct continuation*
; rdx: int initial_value
_reinstate:
  ; reinstate old stack frames
  mov rbx, rsi
  mov rcx, [rsi]
  sub rdi, rcx
  mov rsi, [rsi + 8]
  cld
  rep movsb

  ; restore registers
  mov rsp, [rbx + 3*8]
  mov rbp, [rbx + 4*8]

  ; set return value and jump
  mov rax, rdx
  mov rsi, [rbx + 2*8] ; rip
  jmp rsi
;  push rsi ; instead of jump, we can do this
;  jmp [rsi + 2*8] 
  ret
