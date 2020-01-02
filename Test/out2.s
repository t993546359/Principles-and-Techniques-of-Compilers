.data
.globl main
.text
main:
addi $fp, $sp, 4
addi $sp, $sp, -4
li $t0, 1
sw $t0, -4($fp)
addi $sp, $sp, -4
li $t0, 6
sw $t0, -8($fp)
addi $sp, $sp, -4
lw $t1, -8($fp)
move $t0, $t1
sw $t0, -12($fp)
sw $t1, -8($fp)
addi $sp, $sp, -4
lw $t1, -12($fp)
addi $t0, $t1, 2
sw $t0, -16($fp)
sw $t1, -12($fp)
addi $sp, $sp, -4
lw $t1, -16($fp)
move $t0, $t1
sw $t0, -20($fp)
sw $t1, -16($fp)
addi $sp, $sp, -4
lw $t1, -12($fp)
addi $t0, $t1, -4
sw $t0, -24($fp)
sw $t1, -12($fp)
addi $sp, $sp, -4
lw $t1, -24($fp)
move $t0, $t1
sw $t0, -28($fp)
sw $t1, -24($fp)
addi $sp, $sp, -4
lw $t1, -28($fp)
lw $t2, -12($fp)
sub $t0, $t1, $t2
sw $t0, -32($fp)
sw $t1, -28($fp)
sw $t2, -12($fp)
addi $sp, $sp, -4
lw $t1, -32($fp)
move $t0, $t1
sw $t0, -36($fp)
sw $t1, -32($fp)
addi $sp, $sp, -4
lw $t1, -20($fp)
lw $t2, -12($fp)
mul $t0, $t1, $t2
sw $t0, -40($fp)
sw $t1, -20($fp)
sw $t2, -12($fp)
addi $sp, $sp, -4
lw $t1, -40($fp)
move $t0, $t1
sw $t0, -44($fp)
sw $t1, -40($fp)
addi $sp, $sp, -4
lw $t1, -12($fp)
div $t1, $t1
mflo $t0
sw $t0, -48($fp)
sw $t1, -12($fp)
addi $sp, $sp, -4
lw $t1, -48($fp)
move $t0, $t1
sw $t0, -52($fp)
sw $t1, -48($fp)
move $v0, $0
jr $ra
