.code
  __bigUInt_add proc
    mov r11, r9
    clc
    
    xor rbx, rbx
    _loop_ncf_add:
      cmp rbx, rcx
      je _end_add
      mov r10, [rdx+rbx*8]
      add r10, [r8+rbx*8]
      mov [r9+rbx*8], r10
      inc rbx
      jc _loop_cf_add
      jmp _loop_ncf_add

    _loop_cf_add:
      cmp rbx, rcx
      je _end_add
      mov r10, [rdx+rbx*8]
      adc r10, [r8+rbx*8]
      mov [r9+rbx*8], r10
      inc rbx
      jc _loop_cf_add
      jmp _loop_ncf_add

    _end_add:
      mov r9, r11
      xor rax, rax
      ret

  __bigUInt_add endp

  __bigUInt_sub proc

  __bigUInt_sub endp

  __bigUInt_inc proc
    xor rbx, rbx
    _loop_inc:
      cmp rbx, rcx
      je _end_inc
      mov r10, [rdx+rbx*8]
      inc r10
      mov [rdx+rbx*8], r10
      inc rbx
      jc _loop_inc
      jmp _end_inc

    _end_inc:
      xor rax, rax
      ret

  __bigUInt_inc endp

  __bigUInt_dec proc
    xor rcx, rcx
    _loop_dec:
      cmp rbx, rcx
      je _end_dec
      mov r10, [rdx+rcx*8]
      dec r10
      mov [rdx+rcx*8], r10
      inc rcx
      jmp _loop_dec
      jc _end_dec

    _end_dec:
      xor rax, rax
      ret

  __bigUInt_dec endp
end

