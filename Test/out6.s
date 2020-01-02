.data
 _prompt: .asciiz "Enter an integer : "
_ret: .asciiz "\n"
.globl main
.text
 read: 
    li $v0, 4 
    la $a0, _prompt 
    syscall 
    li $v0, 5 
    syscall 
    jr $ra 

 write: 
    li $v0, 1 
    syscall 
    li $v0, 4 
    la $a0, _ret 
    syscall 
    move $v0, $0 
    jr $ra 

main: 
addi $fp, $sp, 0
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
li $t1, 1
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
li $t1, 10
sw $t1, 0($t0)
sw $t0, -56($fp)
addi $sp, $sp, -4
li $t1, 2
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
li $t1, 1
li $t2, 4
mul $t0, $t1, $t2
sw $t0, -68($fp)
addi $sp, $sp, -4
addi $t1, $fp, -4
lw $t2, -68($fp)
sub $t0, $t1, $t2
sw $t0, -72($fp)
sw $t2, -68($fp)
addi $sp, $sp, -4
lw $t1, -72($fp)
lw $t0, 0($t1)
sw $t0, -76($fp)
sw $t1, -72($fp)
addi $sp, $sp, -4
li $t1, 0
li $t2, 4
mul $t0, $t1, $t2
sw $t0, -80($fp)
addi $sp, $sp, -4
addi $t1, $fp, -4
lw $t2, -80($fp)
sub $t0, $t1, $t2
sw $t0, -84($fp)
sw $t2, -80($fp)
addi $sp, $sp, -4
lw $t1, -84($fp)
lw $t0, 0($t1)
sw $t0, -88($fp)
sw $t1, -84($fp)
addi $sp, $sp, -4
lw $t1, -76($fp)
lw $t2, -88($fp)
sub $t0, $t1, $t2
sw $t0, -92($fp)
sw $t1, -76($fp)
sw $t2, -88($fp)
lw $t0, -64($fp)
lw $t1, -92($fp)
sw $t1, 0($t0)
sw $t0, -64($fp)
sw $t1, -92($fp)
addi $sp, $sp, -4
li $t1, 2
li $t2, 4
mul $t0, $t1, $t2
sw $t0, -96($fp)
addi $sp, $sp, -4
addi $t1, $fp, -4
lw $t2, -96($fp)
sub $t0, $t1, $t2
sw $t0, -100($fp)
sw $t2, -96($fp)
addi $sp, $sp, -4
lw $t1, -100($fp)
lw $t0, 0($t1)
sw $t0, -104($fp)
sw $t1, -100($fp)
lw $t0, -104($fp)
move $a0, $t0 
addi $sp, $sp, -4 
sw $ra, 0($sp) 
jal write  
lw $ra, 0($sp) 
addi $sp, $sp, 4  
sw $t0, -104($fp)
li $t0, 0
move $v0, $t0
jr $ra 
