.text

# Decodes a quadtree to the original matrix
#
# Arguments:
#     quadtree (qNode*)
#     matrix (void*)
#     matrix_width (int)
#
# Recall that quadtree representation uses the following format:
#     struct qNode {
#         int leaf;
#         int size;
#         int x;
#         int y;
#         int gray_value;
#         qNode *child_NW, *child_NE, *child_SE, *child_SW;
#     }

quad2matrix:
		addi $sp, $sp, -8
		sw $ra, 8($sp)	# save return address

		lw $t0, 0($a0)	# $t0 = a0->leaf
		bne $t0, $0, label1 # if $t0 != 0
		j label2 # else
label1:
		lw $t1, 4($a0)	# $t1 = a0->size
		add $t2, $0, $0	# $t2 = 0
for0:	
		add $t3, $0, $0	# $t3 = 0
		slt $t6, $t2, $t1	# $t6 = 1 if  $t2 < $t1
		bne $t6, $0, for1	# $t6 jump to for1
		j fin 			# jump to fin
for1:	
		slt $t6, $t3, $t1	# $t6 = 1 if $t3 < $t1
		bne $t6, $0, set	# $t6 jump to set
		j for2 			# jump to fin
set:
		lb $t0, 16($a0)	# $t0 = a0->gray_value
		lw $t4, 12($a0)	# $t4 = a0->y
		add $t4, $t4, $t3	# $t4 = $t4 + $t3
		mul $t4, $t4, $a2	# $t4 = $t4 * $a2
		lw $t5, 8($a0)	# $t5 = a0->x
		add $t5, $t5, $t2	# $t5 = $t5 + $t2
		add $t4, $t4, $t5 # $t4 = $t4 + $t5
		# sll $t4, $t4, 2	# $t4 = $t4 * 4
		add $t4, $a1, $t4	# $t4 = a1[$t4]
		sb $t0, 0($t4)	# $t4 = $t0
		addi $t3, $t3, 1 	# $t3 = $t3 + 1
		j for1			# jump to for1

for2:
		addi $t2, $t2, 1	# $t2 = $t2 + 1
		j for0 			# jump to for0

label2: 
		sw $a0, 4($sp)
		lw $a0, 20($a0)
		jal quad2matrix
		lw $a0, 4($sp)
		sw $a0, 4($sp)
		lw $a0, 24($a0)
		jal quad2matrix
		lw $a0, 4($sp)
		sw $a0, 4($sp)
		lw $a0, 28($a0)
		jal quad2matrix
		lw $a0, 4($sp)
		sw $a0, 4($sp)
		lw $a0, 32($a0)
		jal quad2matrix
		lw $a0, 4($sp)
fin:
		lw $ra, 8($sp)
		addi $sp, $sp, 8
        jr $ra
