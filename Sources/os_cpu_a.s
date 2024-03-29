#*********************************************************************************************************
#*                                                uC/OS-II
#*                                          The Real-Time Kernel
#*                                     Freescale MPC55xx Specific code
#*
#*                                 (c) Copyright 2007; Micrium; Weston, FL
#*                                           All Rights Reserved
#*
#* LICENSING TERMS:
#* ---------------
#*   uC/OS-II is provided in source form for FREE evaluation, for educational use or for peaceful research.
#* If you plan on using  uC/OS-II  in a commercial product you need to contact Micri�m to properly  license
#* its use in your product.  We provide ALL the source code for your convenience and to help you experience
#* uC/OS-II.   The fact that the  source  is provided does  NOT  mean that you can use it without  paying a
#* licensing fee.
#*********************************************************************************************************

#*********************************************************************************************************
#*
#*                                             OS PORT FILE
#*
#*                                          Freescale  MPC55xx
#*                                            GNU C Compiler
#*
#* Filename      : os_cpu_a.s
#* Programmer(s) : FGK
#*********************************************************************************************************


#*********************************************************************************************************
#*                                             ASM HEADER
#*********************************************************************************************************

    .include "os_cpu_a.inc"

#    .section .xcptn_vle, "ax"
    .section .text_vle

    .extern BSP_TmrTickISR
    .extern OSTimeTick
    .extern OSTaskSwHook
    .extern OSIntEnter
    .extern OSIntExit

    .extern OSTCBCur                                # pointer to current tasks TCB
    .extern OSTCBHighRdy                            # pointer to highest priority ready task
    .extern OSPrioHighRdy
    .extern OSPrioCur
    .extern OSRunning
    .extern OSIntNesting


#*********************************************************************************************************
#*                                              DEFINES
#*********************************************************************************************************

    .equ INTC_IACKR, 0xfff48010                     # Interrupt Acknowledge Register address
    .equ INTC_EOIR,  0xfff48018                     # End of Interrupt Register address


#*********************************************************************************************************
#*                                          PUBLIC DECLARATIONS
#*********************************************************************************************************

    .global OSCtxSw
    .global OSStartHighRdy
    .global OSIntCtxSw

    .global OSTickISR
    .global OSExtIntISR


#$PAGE
#*********************************************************************************************************
#*                                      ISR HANDLER FOR SW VECTOR MODE
#*
#* Description : This function is executed when IVOR4 External input interrupt (SPR 404) exception occurs.
#*********************************************************************************************************
    .align  4
OSExtIntISR:
    prologue                                        # Save context

    e_lis   r11,OSIntNesting@ha                     # load OSIntNesting
    e_lbz   r3, OSIntNesting@l(r11)
    e_addi  r0, r3, 1
    e_stb   r0, OSIntNesting@l(r11)                 # OSIntNesting++

    e_cmpl16i r0, 1                                 # if (OSIntNesting != 1) ...
    se_bne  OSExtIntISR_NotSaveSP                   # do not save the stackpointer

    e_lis   r11,OSTCBCur@ha                         # Get pointer to current TCB
    e_lwz   r11,OSTCBCur@l(r11)
    e_stw   r1, 0(r11)                              # Save stack pointer in current TCB

OSExtIntISR_NotSaveSP:
    e_lis   r3, INTC_IACKR@h                        # store address of INTC.IACKR in r3
    e_or2i  r3, INTC_IACKR@l
    e_lwz   r3, 0(r3)                               # load contents of INTC.IACKR in r3
    e_lwz   r12,0(r3)                               # load the base adress of Vector Table

    mtctr   r12                                     # load Count register
    se_bctrl                                        # branch to ISR_Handler

    se_li   r0, 0                                   # load r0 with 0x0000
    e_lis   r3, INTC_EOIR@h                         # get the address of the INTC.EOIR Register ...
    e_or2i  r3, INTC_EOIR@l
    e_stw   r0, 0(r3)                               # and clear the INTC.EOIR register

    e_bl    OSIntExit                               # call to decrement OSIntNesting

    epilogue                                        # restore the context

    se_rfi                                          # return from interrupt


#$PAGE
#*********************************************************************************************************
#*                                            SYSTEM TICK ISR
#*
#* Description : This function is the ISR to notify uC/OS-II that a system tick has occurred.
#*********************************************************************************************************
    .align  4
OSTickISR:
    prologue

    e_cmpl16i r0, 1                                 # if (OSIntNesting != 1) ...
    se_bne  OSTickISR_NotSaveSP                     # do not save the stackpointer

    e_lis   r11,OSTCBCur@ha                         # Get pointer to current TCB
    e_lwz   r11,OSTCBCur@l(r11)
    e_stw   r1, 0(r11)                              # Save stack pointer in current TCB

OSTickISR_NotSaveSP:
	e_li      r0, 0                                   # load r0 with 0x0000
#    e_lis     r3, INTC_EOIR@ha                        # get the address of the INTC.EOIR Register ...
#    e_stw     r0, INTC_EOIR@l(r3)                     # and clear the INTC.EOIR register

    e_bl    OSTimeTick                          # call TmrTick handler
#    e_bl    OSIntExit                               # call to decrement OSIntNesting

    epilogue                                        # Restore context

#    se_rfi                                          # run task
    se_blr

#$PAGE
#*********************************************************************************************************
#*                                 START HIGHEST PRIORITY TASK READY-TO-RUN
#*
#* Description : This function is called by OSStart() to start the highest priority task that was created
#*               by the application before calling OSStart().
#*
#* Arguments   : none
#*
#* Note(s)     : 1) OSStartHighRdy() MUST:
#*                   a) Call OSTaskSwHook() then,
#*                   b) Set OSRunning to TRUE,
#*                   c) Switch to the highest priority task by loading the stack pointer of the highest
#*                      priority task into the SP register and execute an rfi instruction.
#*********************************************************************************************************
    .align  4
OSStartHighRdy:
    e_bl    OSTaskSwHook                            # Call OSTaskSwHook

    e_li    r0, 1                                   # Set OSRunning to 1
    e_lis   r11,OSRunning@ha
    e_stb   r0, OSRunning@l(r11)

    e_lis   r11,OSTCBHighRdy@ha                     # Get pointer to ready task TCB
    e_lwz   r11,OSTCBHighRdy@l(r11)

    e_lis   r12,OSTCBCur@ha                         # Save as current task TCB ptr.
    e_stw   r11,OSTCBCur@l(r12)

    e_lwz   r1, 0(r11)                              # Get new stack pointer

    epilogue                                        # Restore context

    se_rfi                                          # run task


#$PAGE
#*********************************************************************************************************
#*                                         TASK LEVEL CONTEXT SWITCH
#*
#* Description : This function is called when a task makes a higher priority task ready-to-run.
#*
#* Arguments   : none
#*
#* Note(s)     : 1) Upon entry,
#*                  OSTCBCur     points to the OS_TCB of the task to suspend
#*                  OSTCBHighRdy points to the OS_TCB of the task to resume
#*********************************************************************************************************
    .align  4
OSCtxSw:
    prologue                                        # Save context

    e_lis   r11,OSTCBCur@ha                         # Get pointer to current TCB
    e_lwz   r11,OSTCBCur@l(r11)
    e_stw   r1, 0(r11)                              # Save stack pointer in current TCB

    e_bl    OSTaskSwHook                            # Call OSTaskSwHook

    e_lis   r11,OSTCBHighRdy@ha                     # Get pointer to ready task TCB
    e_lwz   r11,OSTCBHighRdy@l(r11)

    e_lis   r12,OSTCBCur@ha                         # Save as current task TCB ptr.
    e_stw   r11,OSTCBCur@l(r12)

    e_lis   r12,OSPrioHighRdy@ha                    # Get High Ready Priority
    e_lbz   r10,OSPrioHighRdy@l(r12)

    e_lis   r12,OSPrioCur@ha                        # Save as Current Priority
    e_stb   r10,OSPrioCur@l(r12)

    e_lwz   r1, 0(r11)                              # Get new stack pointer from TCB

    epilogue                                        # Restore context

    se_rfi                                          # run task


#$PAGE
#*********************************************************************************************************
#*                                      INTERRUPT LEVEL CONTEXT SWITCH
#*
#* Description : This function is called by OSIntExit() to perform a context switch to a task that has
#*               been made ready-to-run by an ISR.
#*********************************************************************************************************
    .align  4
OSIntCtxSw:
    mbar    1

    e_bl    OSTaskSwHook                            # Call OSTaskSwHook

    e_lis   r11,OSTCBHighRdy@ha                     # Get pointer to ready task TCB
    e_lwz   r11,OSTCBHighRdy@l(r11)

    e_lis   r12,OSTCBCur@ha                         # Save as current task TCB ptr.
    e_stw   r11,OSTCBCur@l(r12)

    e_lis   r12,OSPrioHighRdy@ha                    # Get High Ready Priority
    e_lbz   r10,OSPrioHighRdy@l(r12)

    e_lis   r12,OSPrioCur@ha                        # Save as Current Priority
    e_stb   r10,OSPrioCur@l(r12)

    e_lwz   r1, 0(r11)                              # Get new stack pointer

    epilogue                                        # Restore context

    mbar    1

    se_rfi                                          # run task


#$PAGE
#*********************************************************************************************************
#*                                     RETURN FROM INTERRUPT TRAP
#*********************************************************************************************************
    .align  4
RFI_trap:
    se_rfi


#*********************************************************************************************************
#*                                     CPU ASSEMBLY PORT FILE END
#*********************************************************************************************************
    .end
