.code
  __bigUInt_add proc
    mov r11, r9
    clc
    
    xor rbx, rbx
    _loop_nof:
      cmp rbx, rcx
      je _end
      mov r10, [rdx+rbx*8]
      add r10, [r8+rbx*8]
      mov [r9+rbx*8], r10
      inc rbx
      jc _loop_of
      jmp _loop_nof

    _loop_of:
      cmp rbx, rcx
      je _end
      mov r10, [rdx+rbx*8]
      adc r10, [r8+rbx*8]
      mov [r9+rbx*8], r10
      inc rbx
      jc _loop_of
      jmp _loop_nof

    _end:
      mov r9, r11
      xor rax, rax
      ret

  __bigUInt_add endp
end