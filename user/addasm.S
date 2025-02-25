.global main

.section .data
format_str:       .string "|%s|\n"
format_res:            .string "%d\n"
error_input_msg:    .string "addasm: input error\n"
error_overflow_msg: .string "addasm: buffer overflow\n"
error_char_msg:     .string "addasm: invalid character\n"
error_space_msg:    .string "addasm: no space found\n"

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
    j exit

error_overflow:
    la a0, error_overflow_msg
    call printf
    j exit

error_invalid_char:
    la a0, error_char_msg
    call printf
    j exit

error_no_space:
    la a0, error_space_msg
    call printf
    j exit

error_input:
    la a0, error_input_msg
    call printf

exit:
    ld s3, 32(sp)
    ld s2, 24(sp)
    ld s1, 16(sp)
    ld s0, 8(sp)
    ld ra, 0(sp)
    addi sp, sp, 48
    li a0, 0
    ret

