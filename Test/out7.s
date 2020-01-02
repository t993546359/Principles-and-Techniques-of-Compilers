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

func: 
addi $fp, $sp, 0
addi $sp, $sp, -4
li $t0, 4
sw $t0, -4($fp)
addi $sp, $sp, -4
li $t0, 2
sw $t0, -8($fp)
addi $sp, $sp, -4
lw $t1, -4($fp)
lw $t2, -8($fp)
add $t0, $t1, $t2
sw $t0, -12($fp)
sw $t1, -4($fp)
sw $t2, -8($fp)
lw $t1, -12($fp)
move $t0, $t1
sw $t0, -8($fp)
sw $t1, -12($fp)
lw $t0, -8($fp)
move $v0, $t0
jr $ra 
sw $t0, -8($fp)
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
addi $sp, $sp, -4
addi $sp, $sp, -8 
sw $ra, 0($sp) 
sw $fp, 4($sp) 
jal func 
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
addi $sp, $sp, -4
lw $t1, -16($fp)
lw $t2, -8($fp)
add $t0, $t1, $t2
sw $t0, -20($fp)
sw $t1, -16($fp)
sw $t2, -8($fp)
lw $t1, -20($fp)
move $t0, $t1
sw $t0, -16($fp)
sw $t1, -20($fp)
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
