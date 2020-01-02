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
li $t0, 1
sw $t0, -44($fp)
addi $sp, $sp, -4
li $t1, 0
li $t2, 4
mul $t0, $t1, $t2
sw $t0, -48($fp)
addi $sp, $sp, -4
addi $t1, $fp, -4
lw $t2, -48($fp)
sub $t0, $t1, $t2
sw $t0, -52($fp)
sw $t2, -48($fp)
lw $t0, -52($fp)
li $t1, 0
sw $t1, 0($t0)
sw $t0, -52($fp)
label1:
lw $t0, -44($fp)
li $t1, 10
blt $t0, $t1, label2
sw $t0, -44($fp)
j label3
label2:
addi $sp, $sp, -4
lw $t1, -44($fp)
li $t2, 4
mul $t0, $t1, $t2
sw $t0, -56($fp)
sw $t1, -44($fp)
addi $sp, $sp, -4
addi $t1, $fp, -4
lw $t2, -56($fp)
sub $t0, $t1, $t2
sw $t0, -60($fp)
sw $t2, -56($fp)
addi $sp, $sp, -4
lw $t1, -44($fp)
addi $t0, $t1, -1
sw $t0, -64($fp)
sw $t1, -44($fp)
addi $sp, $sp, -4
lw $t1, -64($fp)
li $t2, 4
mul $t0, $t1, $t2
sw $t0, -68($fp)
sw $t1, -64($fp)
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
lw $t1, -76($fp)
addi $t0, $t1, 1
sw $t0, -80($fp)
sw $t1, -76($fp)
lw $t0, -60($fp)
lw $t1, -80($fp)
sw $t1, 0($t0)
sw $t0, -60($fp)
sw $t1, -80($fp)
addi $sp, $sp, -4
lw $t1, -44($fp)
addi $t0, $t1, 1
sw $t0, -84($fp)
sw $t1, -44($fp)
lw $t1, -84($fp)
move $t0, $t1
sw $t0, -44($fp)
sw $t1, -84($fp)
j label1
label3:
addi $sp, $sp, -4
li $t1, 9
li $t2, 4
mul $t0, $t1, $t2
sw $t0, -88($fp)
addi $sp, $sp, -4
addi $t1, $fp, -4
lw $t2, -88($fp)
sub $t0, $t1, $t2
sw $t0, -92($fp)
sw $t2, -88($fp)
addi $sp, $sp, -4
lw $t1, -92($fp)
lw $t0, 0($t1)
sw $t0, -96($fp)
sw $t1, -92($fp)
lw $t0, -96($fp)
move $a0, $t0 
addi $sp, $sp, -4 
sw $ra, 0($sp) 
jal write  
lw $ra, 0($sp) 
addi $sp, $sp, 4  
sw $t0, -96($fp)
li $t0, 0
move $v0, $t0
jr $ra 
