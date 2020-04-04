    # timetemplate.asm
    # Written 2015 by F Lundevall
    # Copyright abandonded - this file is in the public domain.

.global delay
.global time2string


.macro    PUSH reg
      addi    $sp,$sp,-4
      sw    \reg,0($sp)
.endm

  .macro    POP reg
      lw    \reg,0($sp)
      addi    $sp,$sp,4
.endm

      .data
      .align 2
  mytime:    .word 0x5957
  timstr:    .ascii "text more text lots of text\0"
      .text
hexasc:
    move      $v0, $a0        # Adds a0 in v0
    andi    $v0, 0x0000000F        # Masking v0 therefor we just want the first 4 bits.
    add    $t0, $zero, 0x00000009    # Adds 9 to t0
    ble    $v0, $t0, below        # Checks if the inputed noumber is smaler or equal to a0.
    addi     $v0, $v0, 0x00000037    # If its not. We add 0x37 to the noumber
    jr $ra
below:
    addi     $v0, $v0, 0x00000030    # Else we add 0x30.
    jr $ra

delay:                    # Delay block

    PUSH    $ra
    
    addi    $t0, $zero, 1000        # t0 : INPUT: Delay in ms
    addi    $t1, $zero, 32000        # t1 : Delay constant
    addi    $t2, $zero, 0            # t2 : i
    addi    $t3, $zero, 0            # t3 : slt control
     

while:
    beq    $t0, $zero, exit
    nop
    addi    $t0, $t0, -1

    for:
    slt    $t3, $t2, $t1
    beq    $t3, $0, done
    nop
    addi    $t2, $t2, 1
    j    for
    nop
    done:
    j    while

exit:

    POP    $ra
    nop
    jr    $ra

time2string:

    PUSH    $s0            # For using hexasc
    PUSH    $v0            # For storage of the clock
    PUSH    $ra            # For using hexasc

    
    andi    $t0, $a1, 0x0000000f        # The fist  number stored to t0
    andi    $t1, $a1, 0x000000f0        # Second to t1
    srl    $t1, $t1, 4                  # Shift to LSN of t1
    andi    $t3, $a1, 0x00000f00        # third to t2
    srl    $t3, $t3, 8                  # Shift to LSN of t3
    andi    $t4, $a1, 0x0000f000        # forth to t3
    srl    $t4, $t4, 12                 # Shift to LSN of t4
    addi    $t5, $0, 0x0
    
    move    $s0, $a0

    move     $a0, $t4
    jal     hexasc
    nop
    sb     $v0, 00($s0)
    
    move    $a0, $t3
    jal    hexasc
    nop
    sb    $v0, 01($s0)
    
    addi    $t2, $zero, 0x3a
    sb    $t2, 02($s0)
    
    move    $a0, $t1
    jal    hexasc
    nop
    sb    $v0, 03($s0)
    
    move    $a0, $t0
    jal    hexasc
    nop
    sb    $v0, 04($s0)
    
    sb    $t5, 05($s0)
    
    move    $a0, $s0            # The code could be more simple with a loop. If time will do.

    POP    $ra
    POP    $v0
    POP    $s0                # Must POP in reverse PUSH order. Stack law.
    
    jr    $ra
