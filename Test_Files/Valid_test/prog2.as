.extern EXTVAL
START: add r2, NUMS
LOOP: prn #99
lea EXTVAL, r4
inc r4
mov r2, VALUE
sub r5, r6
bne EXIT
cmp VALUE, #-12
bne &EXIT
dec EXTVAL
.entry START
jmp &LOOP
add SHARE, SHARE
EXIT: stop
TXT: .string "hello world"
NUMS: .data 3, 7
.data -55
VALUE: .data 42
.extern SHARE
