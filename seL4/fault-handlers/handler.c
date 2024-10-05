
#include <assert.h>
#include <stdio.h>
#include <sel4/sel4.h>
#include <utils/util.h>
#include <autoconf.h>

#define FAULTER_BADGE_VALUE     (0xBEEF)
#define PROGNAME                "Handler: "
/* We signal on this notification to let the fauler know when we're ready to
 * receive its fault message.
 */
extern seL4_CPtr sequencing_ep_cap;

extern seL4_CPtr faulter_fault_ep_cap;

extern seL4_CPtr handler_cspace_root;
extern seL4_CPtr badged_faulter_fault_ep_cap;

extern seL4_CPtr faulter_tcb_cap;
extern seL4_CPtr faulter_vspace_root;
extern seL4_CPtr faulter_cspace_root;

int main(void)
{
    int error;
    seL4_Word tmp_badge;
    seL4_CPtr foreign_badged_faulter_empty_slot_cap;
    seL4_CPtr foreign_faulter_capfault_cap;
    seL4_MessageInfo_t seq_msginfo;

    printf(PROGNAME "Handler thread running!\n"
           PROGNAME "About to wait for empty slot from faulter.\n");


    seq_msginfo = seL4_Recv(sequencing_ep_cap, &tmp_badge);
    foreign_badged_faulter_empty_slot_cap = seL4_GetMR(0);
    printf(PROGNAME "Received init sequence msg: slot in faulter's cspace is "
           "%lu.\n",
           foreign_badged_faulter_empty_slot_cap);



    /* Mint the fault ep with a badge */

    error = seL4_CNode_Mint(
        handler_cspace_root,
        badged_faulter_fault_ep_cap,
        seL4_WordBits,
        handler_cspace_root,
        faulter_fault_ep_cap, /* we are badging to this cap */
        seL4_WordBits,
        seL4_AllRights, FAULTER_BADGE_VALUE);

    ZF_LOGF_IF(error != 0, PROGNAME "Failed to mint ep cap with badge!");
    printf(PROGNAME "Successfully minted fault handling ep into local cspace.\n");


    /* This step is only necessary on the master kernel. On the MCS kernel it
     * can be skipped because we do not need to copy the badged fault EP into
     * the faulting thread's cspace on the MCS kernel.
     */

    error = seL4_CNode_Copy(
        faulter_cspace_root,
        foreign_badged_faulter_empty_slot_cap, /* target slot on faulter cspace */
        seL4_WordBits,
        handler_cspace_root,
        badged_faulter_fault_ep_cap,
        seL4_WordBits,
        seL4_AllRights);

    ZF_LOGF_IF(error != 0, PROGNAME "Failed to copy badged ep cap into faulter's cspace!");
    printf(PROGNAME "Successfully copied badged fault handling ep into "
           "faulter's cspace.\n"
           PROGNAME "(Only necessary on Master kernel.)\n");

    /*
     * This is an interesting detail about the design
     *
     * seL4_TCB_SetSpace is used to modify CSpace, VSpace and fault ep of a TCB
     * meaning, the faulter needs write/modify access to target TCB
     *
     * it's not crazy, but still suprised me to learn this
     * so yeah, faulter has WRITE/MODIFY access to target TCB
     */
    error = seL4_TCB_SetSpace(
        faulter_tcb_cap,
        foreign_badged_faulter_empty_slot_cap,
        faulter_cspace_root,
        0,
        faulter_vspace_root,
        0);

    ZF_LOGF_IF(error != 0, PROGNAME "Failed seL4_TCB_SetSpace on faulter TCB");
    printf(PROGNAME "Successfull seL4_TCB_SetSpace on faulter TCB\n");

    /*
     * reply back to the faulter
     */
    seL4_Reply(seL4_MessageInfo_new(0, 0, 0, 0));

    /*
     * now we wait on the endpoint for any faults
     */
    
    seL4_Word badge = 0;
    seL4_MessageInfo_t msginfo = {0};

    while (1) {
        msginfo = seL4_Recv(foreign_badged_faulter_empty_slot_cap, &badge);
        printf(PROGNAME "Received fault from %lu\n", badge);

        /*
         * figure out what was the fault about (refere to manual here)
         * the tutorial tells us that only CapFault will be generated
         * so, i just get the IPC msgs of a CapFault, instead of a switch-case
         */
        foreign_faulter_capfault_cap = seL4_GetMR(seL4_CapFault_Addr);

        /*
         * ready to handle it
         * for now all we do is put a valid capability to the faulting cap slot
         */
        error = seL4_CNode_Copy(
            faulter_cspace_root,
            foreign_faulter_capfault_cap,
            seL4_WordBits,
            handler_cspace_root,
            sequencing_ep_cap, /* randomly choosen */
            seL4_WordBits,
            seL4_AllRights);
        ZF_LOGF_IF(error != 0, PROGNAME "Failed handle CapFault\n");

        seL4_Reply(seL4_MessageInfo_new(0, 0, 0, 0));
    }

    return 0;
}