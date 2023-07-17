.global __bigUInt_add, __bigUInt_sub, __bigUInt_inc, __bigUInt_dec
.type __bigUInt_add, %function
.type __bigUInt_sub, %function
.type __bigUInt_inc, %function
.type __bigUInt_dec, %function

__bigUInt_add:
  mov x5, x3

  mov x10, xzr
_loop_ncf_add:
  cmp x10, x0
  b.eq _end_add
  ldr x11, [x1, x10, LSL #3]
  ldr x12, [x2, x10, LSL #3]
  adds x11, x11, x12
  str x11, [x3, x10, LSL #3]
  add x10, x10, #1
  b.cs _loop_cf_add
  b _loop_ncf_add

_loop_cf_add:
  cmp x10, x0
  b.eq _end_add
  ldr x11, [x1, x10, LSL #3]
  ldr x12, [x2, x10, LSL #3]
  adds x11, x11, x12
  add x11, x11, #1
  str x11, [x3, x10, LSL #3]
  add x10, x10, #1
  b.cs _loop_cf_add
  b _loop_ncf_add

_end_add:
  mov x3, x5
  mov x0, xzr
  ret

.size __bigUInt_add, .-__bigUInt_add

__bigUInt_sub: // not done yet
  mov x5, x3

  mov x10, xzr
_loop_ncf_sub:
  cmp x10, x0
  b.eq _end_sub
  ldr x11, [x1, x10, LSL #3]
  ldr x12, [x2, x10, LSL #3]
  subs x11, x11, x12
  str x11, [x3, x10, LSL #3]
  add x10, x10, #1
  b.cs _loop_cf_sub
  b _loop_ncf_sub

_loop_cf_sub:
  cmp x10, x0
  b.eq _end_sub
  ldr x11, [x1, x10, LSL #3]
  sub x11, x11, #1
  ldr x12, [x2, x10, LSL #3]
  subs x11, x11, x12
  str x11, [x3, x10, LSL #3]
  add x10, x10, #1
  b.cs _loop_cf_sub
  b _loop_ncf_sub

_end_sub:
  mov x3, x5
  mov x0, xzr
  ret

.size __bigUInt_sub, .-__bigUInt_sub

__bigUInt_inc:
  mov x10, xzr
_loop_inc:
  cmp x10, x0
  b.eq _end_inc
  ldr x11, [x1, x10, LSL #3]
  adds x11, x11, #1
  str x11, [x1, x10, LSL #3]
  add x10, x10, #1
  b.cs _loop_inc
  b _end_inc

_end_inc:
  mov x0, xzr
  ret

.size __bigUInt_inc, .-__bigUInt_inc

__bigUInt_dec:
  mov x10, xzr
_loop_dec:
  cmp x10, x0
  b.eq _end_dec
  ldr x11, [x1, x10, LSL #3]
  subs x11, x11, #1
  str x11, [x1, x10, LSL #3]
  add x10, x10, #1
  b _loop_dec
  b.cs _end_dec

_end_dec:
  mov x0, xzr
  ret

.size __bigUInt_dec, .-__bigUInt_dec
