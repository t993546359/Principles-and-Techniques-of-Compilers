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

fact: 
addi $fp, $sp, 0
addi $sp, $sp, -4
move $t0, $a0 
sw $t0, -4($fp)
lw $t0, -4($fp)
li $t1, 1
beq  $t0, $t1, label1
sw $t0, -4($fp)
j label2
label1:
lw $t0, -4($fp)
move $v0, $t0
jr $ra 
sw $t0, -4($fp)
j label3
label2:
addi $sp, $sp, -4
lw $t1, -4($fp)
addi $t0, $t1, -1
sw $t0, -8($fp)
sw $t1, -4($fp)
lw $t0, -8($fp)
move $a0, $t0 
sw $t0, -8($fp)
addi $sp, $sp, -4
addi $sp, $sp, -8 
sw $ra, 0($sp) 
sw $fp, 4($sp) 
jal fact 
move $sp, $fp 
lw $ra, 0($sp) 
lw $fp, 4($sp) 
addi $sp, $sp, 8 
move $t0, $v0 
sw $t0, -12($fp)
addi $sp, $sp, -4
lw $t1, -4($fp)
lw $t2, -12($fp)
mul $t0, $t1, $t2
sw $t0, -16($fp)
sw $t1, -4($fp)
sw $t2, -12($fp)
lw $t0, -16($fp)
move $v0, $t0
jr $ra 
sw $t0, -16($fp)
label3:
main: 
addi $fp, $sp, 0
addi $sp, $sp, -4
addi $sp, $sp, -4 
sw $ra, 0($sp) 
jal read 
lw $ra, 0($sp) 
addi $sp, $sp, 4 
move $t0, $v0 
sw $t0, -4($fp)
addi $sp, $sp, -4
lw $t1, -4($fp)
move $t0, $t1
sw $t0, -8($fp)
sw $t1, -4($fp)
lw $t0, -8($fp)
li $t1, 1
bgt $t0, $t1, label4
sw $t0, -8($fp)
j label5
label4:
lw $t0, -8($fp)
move $a0, $t0 
sw $t0, -8($fp)
addi $sp, $sp, -4
addi $sp, $sp, -8 
sw $ra, 0($sp) 
sw $fp, 4($sp) 
jal fact 
move $sp, $fp 
lw $ra, 0($sp) 
lw $fp, 4($sp) 
addi $sp, $sp, 8 
move $t0, $v0 
sw $t0, -12($fp)
addi $sp, $sp, -4
lw $t1, -12($fp)
move $t0, $t1
sw $t0, -16($fp)
sw $t1, -12($fp)
j label6
label5:
li $t0, 1
sw $t0, -16($fp)
label6:
lw $t0, -16($fp)
move $a0, $t0 
addi $sp, $sp, -4 
sw $ra, 0($sp) 
jal write  
lw $ra, 0($sp) 
addi $sp, $sp, 4  
sw $t0, -16($fp)
li $t0, 0
move $v0, $t0
jr $ra 
