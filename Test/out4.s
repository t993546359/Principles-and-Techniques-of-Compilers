.data
.globl main
.text
main:
addi $fp, $sp, 4
addi $sp, $sp, -40
addi $sp, $sp, -4
li $t1, 0
li $t2, 4
mul $t0, $t1, $t2
sw $t0, -44($fp)
addi $sp, $sp, -4
addi $t1, $fp, -4
lw $t2, -44($fp)
sub $t0, $t1, $t2
sw $t0, -48($fp)
sw $t2, -44($fp)
lw $t0, -48($fp)
li $t1, 4
sw $t1, 0($t0)
sw $t0, -48($fp)
addi $sp, $sp, -4
li $t1, 1
li $t2, 4
mul $t0, $t1, $t2
sw $t0, -52($fp)
addi $sp, $sp, -4
addi $t1, $fp, -4
lw $t2, -52($fp)
sub $t0, $t1, $t2
sw $t0, -56($fp)
sw $t2, -52($fp)
lw $t0, -56($fp)
li $t1, 2
sw $t1, 0($t0)
sw $t0, -56($fp)
addi $sp, $sp, -4
li $t1, 0
li $t2, 4
mul $t0, $t1, $t2
sw $t0, -60($fp)
addi $sp, $sp, -4
addi $t1, $fp, -4
lw $t2, -60($fp)
sub $t0, $t1, $t2
sw $t0, -64($fp)
sw $t2, -60($fp)
addi $sp, $sp, -4
lw $t1, -64($fp)
lw $t0, 0($t1)
sw $t0, -68($fp)
sw $t1, -64($fp)
addi $sp, $sp, -4
li $t1, 1
li $t2, 4
mul $t0, $t1, $t2
sw $t0, -72($fp)
addi $sp, $sp, -4
addi $t1, $fp, -4
lw $t2, -72($fp)
sub $t0, $t1, $t2
sw $t0, -76($fp)
sw $t2, -72($fp)
addi $sp, $sp, -4
lw $t1, -76($fp)
lw $t0, 0($t1)
sw $t0, -80($fp)
sw $t1, -76($fp)
addi $sp, $sp, -4
lw $t1, -68($fp)
lw $t2, -80($fp)
add $t0, $t1, $t2
sw $t0, -84($fp)
sw $t1, -68($fp)
sw $t2, -80($fp)
addi $sp, $sp, -4
lw $t1, -84($fp)
move $t0, $t1
sw $t0, -88($fp)
sw $t1, -84($fp)
move $v0, $0
jr $ra
