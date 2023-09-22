.code
  ; bigUInt_add 함수
  __bigUInt_add proc
    mov r11, r9
    clc
    
    ; nums[0]부터 nums[끝] 까지 반복 (carry가 발생하면 _loop_cf_add로 점프, carry가 발생 안하면 _loop_ncf_add로 점프)
    xor rbx, rbx
    .loop_ncf_add:
      cmp rbx, rcx
      je .end_add
      mov r10, [rdx+rbx*8]
      add r10, [r8+rbx*8]
      mov [r9+rbx*8], r10
      inc rbx
      jc .loop_cf_add
      jmp .loop_ncf_add

    .loop_cf_add:
      cmp rbx, rcx
      je .end_add
      mov r10, [rdx+rbx*8]
      adc r10, [r8+rbx*8]
      mov [r9+rbx*8], r10
      inc rbx
      jc .loop_cf_add
      jmp .loop_ncf_add

    ; 계산 후에 dest가 가리키던 포인터 위치 복구
    .end_add:
      mov r9, r11
      xor rax, rax
      ret

  __bigUInt_add endp

  ; bigUInt_add 함수
  __bigUInt_sub proc
    mov r11, r9
    clc
    
    ; nums[0]부터 nums[끝] 까지 반복 (carry가 발생하면 _loop_cf_add로 점프, carry가 발생 안하면 _loop_ncf_add로 점프)
    xor rbx, rbx
    .loop_ncf_sub:
      cmp rbx, rcx
      je .end_sub
      mov r10, [rdx+rbx*8]
      sub r10, [r8+rbx*8]
      mov [r9+rbx*8], r10
      inc rbx
      jc .loop_cf_sub
      jmp .loop_ncf_sub

    .loop_cf_sub:
      cmp rbx, rcx
      je .end_sub
      mov r10, [rdx+rbx*8]
      dec r10
      sub r10, [r8+rbx*8]
      mov [r9+rbx*8], r10
      inc rbx
      jc .loop_cf_sub
      jmp .loop_ncf_sub

    ; 계산 후에 dest가 가리키던 포인터 위치 복구
    .end_sub:
      mov r9, r11
      xor rax, rax
      ret

  __bigUInt_sub endp

  ; bigUInt_inc 함수
  __bigUInt_inc proc
    ; nums[0]부터 nums[끝] 까지 inc 명령어 반복
    xor rbx, rbx
    .loop_inc:
      cmp rbx, rcx
      je .end_inc
      mov r10, [rdx+rbx*8]
      inc r10
      mov [rdx+rbx*8], r10
      inc rbx
      jc .loop_inc
      jmp .end_inc
    
    ; 리턴값 0으로 바꾸고 리턴
    .end_inc:
      xor rax, rax
      ret

  __bigUInt_inc endp

  ; bigUInt_dec 함수
  __bigUInt_dec proc
    xor rbx, rbx

    ; nums[0]부터 nums[끝] 까지 inc 명령어 반복
    .loop_dec:
      cmp rbx, rcx
      je .end_dec
      mov r10, [rdx+rbx*8]
      dec r10
      mov [rdx+rbx*8], r10
      inc rbx
      xor rax, rax
      not rax
      cmp r10, rax
      je .loop_dec
      jmp .end_dec

    ; 리턴값 0으로 바꾸고 리턴
    .end_dec:
      xor rax, rax
      ret

  __bigUInt_dec endp

  __bigUInt_bit_shl proc
    xor rbx, rbx

    .loop_shl_ncf:
      cmp rbx, rcx
      je .end_shl
      mov r10, [rdx+rbx*8]
      shl r10, 1
      mov [rdx+rbx*8], r10
      inc rbx
      jc .loop_shl_cf
      jmp .loop_shl_ncf

    .loop_shl_cf:
      cmp rbx, rcx
      je .end_shl
      mov r10, [rdx+rbx*8]
      shl r10, 1
      inc r10
      mov [rdx+rbx*8], r10
      inc rbx
      jc .loop_shl_cf
      jmp .loop_shl_ncf

    .end_shl:
      xor rax, rax
      ret
    
  __bigUInt_bit_shl endp

  __bigUInt_bit_shr proc
    xor rbx, rbx

    .loop_shr_ncf:
      cmp rbx, rcx
      je .end_shr
      mov r10, [rdx+rbx*8]
      shr r10, 1
      mov [rdx+rbx*8], r10
      inc rbx
      jc .loop_shr_cf
      jmp .loop_shr_ncf

    .loop_shr_cf:
      cmp rbx, rcx
      je .end_shr
      mov rax, [rdx+rbx*8]
      shr r10, 1
      pushfq
      mov rax, 1
      shl rax, 63
      or r10, rax
      popfq
      mov [rdx+rbx*8], r10
      inc rbx
      jc .loop_shr_cf
      jmp .loop_shr_ncf

    .end_shr:
      xor rax, rax
      ret
    
  __bigUInt_bit_shr endp
end

