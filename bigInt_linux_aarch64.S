.global __bigUInt_add, __bigUInt_inc
.type __bigUInt_add, %function
//.type __bigUInt_sub, %function
.type __bigUInt_inc, %function

__bigUInt_add:
  mov x5, x3

  mov x10, xzr
_loop_ncf:
  cmp x10, x0
  b.eq _end
  ldr x11, [x1, x10, LSL #3]
  ldr x12, [x2, x10, LSL #3]
  adds x11, x11, x12
  str x11, [x3, x10, LSL #3]
  add x10, x10, #1
  b.cs _loop_cf
  b _loop_ncf

_loop_cf:
  cmp x10, x0
  b.eq _end
  ldr x11, [x1, x10, LSL #3]
  ldr x12, [x2, x10, LSL #3]
  adds x11, x11, x12
  add x11, x11, #1
  str x11, [x3, x10, LSL #3]
  add x10, x10, #1
  b.cs _loop_cf
  b _loop_ncf

_end:
  mov x3, x5
  mov x0, xzr
  ret

.size __bigUInt_add, .-__bigUInt_add

__bigUInt_sub: ; not done yet
  mov x5, x3

  mov x10, xzr
_loop_ncf:
  cmp x10, x0
  b.eq _end
  ldr x11, [x1, x10, LSL #3]
  ldr x12, [x2, x10, LSL #3]
  subs x11, x11, x12
  str x11, [x3, x10, LSL #3]
  add x10, x10, #1
  b.cs _loop_cf
  b _loop_ncf

_loop_cf:
  cmp x10, x0
  b.eq _end
  ldr x11, [x1, x10, LSL #3]
  ldr x12, [x2, x10, LSL #3]
  adds x11, x11, x12
  add x11, x11, #1
  str x11, [x3, x10, LSL #3]
  add x10, x10, #1
  b.cs _loop_cf
  b _loop_ncf

_end:
  mov x3, x5
  mov x0, xzr
  ret

.size __bigUInt_add, .-__bigUInt_add

__bigUInt_inc:
  mov x10, xzr
_loop:
  cmp x10, x0
  b.eq _end
  ldr x11, [x1, x10, LSL #3]
  adds x11, x11, #1
  str x11, [x1, x10, LSL #3]
  add x10, x10, #1
  b.cs _loop
  b _end

_end:
  mov x0, mzr
  ret

.size __bigUInt_inc, .-__bigUInt_inc
