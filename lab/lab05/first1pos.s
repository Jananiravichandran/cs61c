.data
shouldben1:	.asciiz "Should be -1, and firstposshift and firstposmask returned: "
shouldbe0:	.asciiz "Should be 0 , and firstposshift and firstposmask returned: "
shouldbe16:	.asciiz "Should be 16, and firstposshift and firstposmask returned: "
shouldbe31:	.asciiz "Should be 31, and firstposshift and firstposmask returned: "

.text
main:
	la	$a0, shouldbe31
	jal	print_str
	lui	$a0, 0x8000	# should be 31
	jal	first1posshift
	move	$a0, $v0
	jal	print_int
	jal	print_space

	lui	$a0, 0x8000
	jal	first1posmask
	move	$a0, $v0
	jal	print_int
	jal	print_newline

	la	$a0, shouldbe16
	jal	print_str
	lui	$a0, 0x0001	# should be 16
	jal	first1posshift
	move	$a0, $v0
	jal	print_int
	jal	print_space

	lui	$a0, 0x0001
	jal	first1posmask
	move	$a0, $v0
	jal	print_int
	jal	print_newline

	la	$a0, shouldbe0
	jal	print_str
	li	$a0, 1		# should be 0
	jal	first1posshift
	move	$a0, $v0
	jal	print_int
	jal	print_space

	li	$a0, 1
	jal	first1posmask
	move	$a0, $v0
	jal	print_int
	jal	print_newline

	la	$a0, shouldben1
	jal	print_str
	move	$a0, $0		# should be -1
	jal	first1posshift
	move	$a0, $v0
	jal	print_int
	jal	print_space

	move	$a0, $0
	jal	first1posmask
	move	$a0, $v0
	jal	print_int
	jal	print_newline

	li	$v0, 10
	syscall

first1posshift:
	bne $a0, $0, notzero
	add $v0, $0, -1
	jr $ra

notzero:
	addi $sp, $sp, -4
	sw $s0, 0($sp)
	addi $s0, $0, 31

loop:
	slt $t0, $a0, $0
	bne $t0, $0, done
	addi $s0, $s0, -1
	sll $a0, $a0, 1
	j loop

done:
	add $v0, $s0, $0
	lw $s0, 0($sp)
	addi $sp, $sp, 4
	jr $ra



first1posmask:
	bne $a0, $0, notzero2
	add $v0, $0, -1
	jr $ra

notzero2:
	addi $sp, $sp, -8
	sw $s0, 0($sp)
	sw $s0, 4($sp)
	addi $s0, $0, 31
	addi $s1, $0, 0x80000000

loop2:
	and $t0, $a0, $s1
	bne $t0, $0, done2
	addi $s0, $s0, -1
	srl $s1, $s1, 1
	j loop2

done2:
	add $v0, $s0, $0
	lw $s1, 4($sp)
	lw $s0, 0($sp)
	addi $sp, $sp, 8
	jr $ra

print_int:
	move	$a0, $v0
	li	$v0, 1
	syscall
	jr	$ra

print_str:
	li	$v0, 4
	syscall
	jr	$ra

print_space:
	li	$a0, ' '
	li	$v0, 11
	syscall
	jr	$ra

print_newline:
	li	$a0, '\n'
	li	$v0, 11
	syscall
	jr	$ra
