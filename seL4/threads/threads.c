#include <stdio.h>
#include <sel4/sel4.h>
#include <utils/util.h>
#include <sel4utils/util.h>
#include <sel4utils/helpers.h>


// the root CNode of the current thread
extern seL4_CPtr root_cnode;
// VSpace of the current thread
extern seL4_CPtr root_vspace;
// TCB of the current thread
extern seL4_CPtr root_tcb;
// Untyped object large enough to create a new TCB object

extern seL4_CPtr tcb_untyped;
extern seL4_CPtr buf2_frame_cap;
extern const char buf2_frame[4096];

// Empty slot for the new TCB object
extern seL4_CPtr tcb_cap_slot;
// Symbol for the IPC buffer mapping in the VSpace, and capability to the mapping
extern seL4_CPtr tcb_ipc_frame;
extern const char thread_ipc_buff_sym[4096];
// Symbol for the top of a 16 * 4KiB stack mapping, and capability to the mapping
extern const char tcb_stack_base[65536];
static const uintptr_t tcb_stack_top = (const uintptr_t)&tcb_stack_base + sizeof(tcb_stack_base);

/*
 * Simple function and variable to be passed onto the 'new_thread'
 */
int global_number = 42;

void printbruh(int n) {
    printf("n: %d\n", n);
}

int new_thread(void *arg1, void *arg2, void *arg3) {
    printf("Hello2: arg1 %p, arg2 %p, arg3 %p\n", arg1, arg2, arg3);
    void (*func)(int) = arg1;
    func(*(int *)arg2);
    while(1);
}


int main(int c, char* arbv[]) {

    printf("Hello, World!\n");

    seL4_DebugDumpScheduler();

    /*
     * The 'tcb_untyped' untyped object is retyped into 'seL4_TCBObject'
     * Invocation is done on the CSpace pointed by 'root_cnode'
     * Capability to the TCB object is at the slot index 'tcb_cap_slot'
     *
     * Why do we have 'tcb_untyped' AND 'tcb_cap_slot'?
     * 'tcb_untyped' is the empty/untyped memory for the TCB object (allocation)
     * 'tcb_cap_slot' is the capability to the newly created TCB object (~ptr)
     */
    seL4_Error result = seL4_Untyped_Retype(tcb_untyped,
                                            seL4_TCBObject,
                                            seL4_TCBBits,
                                            root_cnode,
                                            0,
                                            0,
                                            tcb_cap_slot,
                                            1);
    ZF_LOGF_IF(result, "Failed to retype thread: %d", result);
    seL4_DebugDumpScheduler();

    /*
     * CSpace is set to be the same as the current thread's (via root_cnode)
     * VSpace is also set to be the same (via root_vspace)
     *
     * Creation of IPC buffer is abstracted from me at this point,
     * but this is how it basically goes
     *
     * 1. Use 'seL4_Untyped_Retype' to create a frame object
     * 2. Map the frame to a VSpace (arch dependent) 
     * 3. Then pass it's virtual address to 'seL4_TCB_Configure' (like below)
     */
    result = seL4_TCB_Configure(tcb_cap_slot,
                                seL4_CapNull,
                                root_cnode,
                                seL4_CapNull,
                                root_vspace,
                                0,
                                (seL4_Word) thread_ipc_buff_sym,
                                tcb_ipc_frame);
    ZF_LOGF_IF(result, "Failed to configure thread: %d", result);

    /*
     * This is interesting
     *
     * To set a priority we must have authority (naturally)
     * It is said [by the tut.] that it can be done via 'root_tcb'
     * How does this authority check happens?
     * Obviously via capabilities, but below invoc. isn't explicit about it...
     */
    result = seL4_TCB_SetPriority(tcb_cap_slot, root_tcb, seL4_MaxPrio - 1);
    ZF_LOGF_IF(result, "Failed to set the priority for the new TCB object.\n");
    seL4_DebugDumpScheduler();

    seL4_UserContext regs = {0};
    int error = seL4_TCB_ReadRegisters(tcb_cap_slot, 0, 0, sizeof(regs)/sizeof(seL4_Word), &regs);
    ZF_LOGF_IFERR(error, "Failed to read the new thread's register set.\n");

    /*
     * Should point to a function; in our case 'new_thred'
     */
    sel4utils_set_instruction_pointer(&regs, (seL4_Word) new_thread);

    /*
     * Should point to an area of memory; in our case 'tcb_stack_top'
     */
    sel4utils_set_stack_pointer(&regs, tcb_stack_top);

    /*
     * Additionally, pass args by a helper (or by directly modify the registers)
     */
    sel4utils_arch_init_local_context((void*)new_thread,
                                (void *)printbruh, (void *)&global_number, (void *)3,
                                (void *)tcb_stack_top, &regs);

    /*
     * Set all user context registers (including IP and SP)
     */
    error = seL4_TCB_WriteRegisters(tcb_cap_slot, 0, 0, sizeof(regs)/sizeof(seL4_Word), &regs);
    ZF_LOGF_IFERR(error, "Failed to write the new thread's register set.\n"
                  "\tDid you write the correct number of registers? See arg4.\n");
    seL4_DebugDumpScheduler();

    /*
     * Simply resume it via 'tcb_cap_slot'
     */
    error = seL4_TCB_Resume(tcb_cap_slot);
    ZF_LOGF_IFERR(error, "Failed to start new thread.\n");
    while(1);
    return 0;
}