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
    mov r11, r9
    clc
    
    xor rbx, rbx
    _loop_ncf_sub:
      cmp rbx, rcx
      je _end_sub
      mov r10, [rdx+rbx*8]
      sub r10, [r8+rbx*8]
      mov [r9+rbx*8], r10
      inc rbx
      jc _loop_cf_sub
      jmp _loop_ncf_sub

    _loop_cf_sub:
      cmp rbx, rcx
      je _end_sub
      mov r10, [rdx+rbx*8]
      dec r10
      sub r10, [r8+rbx*8]
      mov [r9+rbx*8], r10
      inc rbx
      jc _loop_cf_sub
      jmp _loop_ncf_sub

    _end_sub:
      mov r9, r11
      xor rax, rax
      ret


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
    xor rbx, rbx

    _loop_dec:
      cmp rbx, rcx
      je _end_dec
      mov r10, [rdx+rbx*8]
      dec r10
      mov [rdx+rbx*8], r10
      inc rbx
      xor rax, rax
      not rax
      cmp r10, rax
      je _loop_dec
      jmp _end_dec

    _end_dec:
      xor rax, rax
      ret

  __bigUInt_dec endp

  __bigUInt_bit_shl proc
    xor rbx, rbx

    _loop_shl_ncf:
      cmp rbx, rcx
      je _end_shl
      mov r10, [rdx+rbx*8]
      shl r10, 1
      mov [r8+rbx*8], r10
      inc rbx
      jc _loop_shl_cf
      jmp _loop_shl_ncf

    _loop_shl_cf:
      cmp rbx, rcx
      je _end_shl
      mov r10, [rdx+rbx*8]
      shl r10, 1
      or r10, 1
      mov [r8+rbx*8], r10
      inc rbx
      jc _loop_shl_cf
      jmp _loop_shl_ncf

    _end_shl:
      xor rax, rax
      ret
    
  __bigUInt_bit_shl endp

  __bigUInt_bit_shr proc
    xor rbx, rbx

    _loop_shr_ncf:
      cmp rbx, rcx
      je _end_shr
      mov r10, [rdx+rbx*8]
      shr r10, 1
      mov [r8+rbx*8], r10
      inc rbx
      jc _loop_shr_cf
      jmp _loop_shr_ncf

    _loop_shr_cf:
      cmp rbx, rcx
      cmp rbx, rcx
      je _end_shr
      mov rax, [rdx+rbx*8]
      shr r10, 1
      mov rax, 1
      shl rax, 63
      or r10, rax
      mov [r8+rbx*8], r10
      inc rbx
      jc _loop_shr_cf
      jmp _loop_shr_ncf

    _end_shr:
      xor rax, rax
      ret
    
  __bigUInt_bit_shr endp
end

