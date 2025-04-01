mcro m2

jmp &LOOP
add SHARE, SHARE
EXIT: stop

mcroend

.extern EXTVAL
START: add r2, NUMS
LOOP: prn #99
lea EXTVAL, r4
inc r4
mov r2, VALUE
sub r5, r6

mcro m1 
bne EXIT
cmp VALUE, #-12
bne &EXIT
mcroend

m1
dec EXTVAL
.entry START
m2
TXT: .string "hello world"
NUMS: .data 3, 7
.data -55
VALUE: .data 42
.extern SHARE



