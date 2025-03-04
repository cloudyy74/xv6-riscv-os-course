.global main

.section .data
format_str:       .string "|%s|\n"
format_res:       .string "%d\n"
error_input_msg:    .string "addasm: input error\n"
error_input_len:   .word 20
error_overflow_msg: .string "addasm: buffer overflow\n"
error_overflow_len: .word 24
error_char_msg:     .string "addasm: invalid character\n"
error_char_len:     .word 26
error_space_msg:    .string "addasm: wrong input format\n"
error_space_len:    .word 27

.section .bss
buffer: .space 512

.section .text
main:
    addi sp, sp, -48
    sd ra, 0(sp)
    sd s0, 8(sp)
    sd s1, 16(sp)
    sd s2, 24(sp)
    sd s3, 32(sp)
    la a0, buffer
    li a1, 512
    call gets
    beqz a0, error_input
    la t0, buffer
    addi t0, t0, 511
    lb t1, (t0)
    bnez t1, error_overflow
    mv s2, a0
    mv t0, s2

replace_n0:
    lb t1, (t0)
    beqz t1, end_replace
    li t2, '\n'
    bne t1, t2, next_char
    sb zero, (t0)
    j end_replace

next_char:
    addi t0, t0, 1
    j replace_n0

end_replace:
    mv t0, s2

valid_check:
    lb t1, (t0)
    beqz t1, is_valid
    li t2, '0'
    blt t1, t2, check_space
    li t2, '9'
    ble t1, t2, valid_char
    
check_space:
    li t2, ' '
    beq t1, t2, valid_char
    j error_invalid_char
    
valid_char:
    addi t0, t0, 1
    j valid_check

is_valid:
    la a0, format_str
    mv a1, s2
    call printf
    mv t0, s2

search:
    lb t1, (t0)
    beqz t1, error_no_space
    li t2, ' '
    beq t1, t2, found_space
    addi t0, t0, 1
    j search

found_space:
    sb zero, (t0)
    mv a0, s2
    call atoi
    mv s0, a0
    addi a0, t0, 1
    call atoi
    mv s1, a0
    add a1, s0, s1
    la a0, format_res
    call printf
    li a0, 0
    call exit

error_overflow:
    li a0, 2
    la a1, error_overflow_msg
    lw a2, error_overflow_len
    call write
    li a0, 1
    call exit

error_invalid_char:
    li a0, 2
    la a1, error_char_msg
    lw a2, error_char_len
    call write
    li a0, 1
    call exit

error_no_space:
    li a0, 2
    la a1, error_space_msg
    lw a2, error_space_len
    call write
    li a0, 1
    call exit

error_input:
    li a0, 2
    la a1, error_input_msg
    lw a2, error_input_len
    call write
    li a0, 1
    call exit
