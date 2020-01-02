.data
.globl main
.text
main:
addi $fp, $sp, 4
addi $sp, $sp, -4
li $t0, 2
sw $t0, -4($fp)
addi $sp, $sp, -4
lw $t1, -4($fp)
addi $t0, $t1, -1
sw $t0, -8($fp)
sw $t1, -4($fp)
addi $sp, $sp, -4
lw $t1, -8($fp)
move $t0, $t1
sw $t0, -12($fp)
sw $t1, -8($fp)
lw $t0, -4($fp)
lw $t1, -12($fp)
bgt $t0, $t1, label1
sw $t0, -4($fp)
sw $t1, -12($fp)
j label2
label1:
addi $sp, $sp, -4
li $t0, 1
sw $t0, -16($fp)
j label3
label2:
addi $sp, $sp, -4
li $t0, -1
sw $t0, -20($fp)
lw $t1, -20($fp)
move $t0, $t1
sw $t0, -16($fp)
sw $t1, -20($fp)
label3:
move $v0, $0
jr $ra
