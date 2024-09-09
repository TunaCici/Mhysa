
#include <stdio.h>
#include <sel4/sel4.h>
#include <sel4platsupport/bootinfo.h>
#include <utils/util.h>

int main(int argc, char *argv[]) {
    /* parse the location of the seL4_BootInfo data structure from
    the environment variables set up by the default crt0.S */
    seL4_BootInfo *info = platsupport_get_bootinfo();


    printf("    CSlot   \tPaddr           \tSize\tType\n");
    for (seL4_CPtr slot = info->untyped.start; slot != info->untyped.end; slot++) {
        seL4_UntypedDesc *desc = &info->untypedList[slot - info->untyped.start];
        printf("%8p\t%16p\t2^%d\t%s\n", (void *) slot, (void *) desc->paddr, desc->sizeBits, desc->isDevice ? "device untyped" : "untyped");
    }
    seL4_Error error;

    // list of general seL4 objects
    seL4_Word objects[] = {seL4_TCBObject, seL4_EndpointObject, seL4_NotificationObject};
    // list of general seL4 object size_bits
    seL4_Word sizes[] = {seL4_TCBBits, seL4_EndpointBits, seL4_NotificationBits};

    /*
     * BIT(sizes[idx]) gives me an object's size in power-of-two bytes
     * I add them all and find the total required size in bytes
     * Then I convert bytes to bits via LOG_BASE_2()
     */
    seL4_Word untyped_size_bits =
        LOG_BASE_2((BIT(sizes[0]) + BIT(sizes[1]) + BIT(sizes[2])) - 1) + 1;
    seL4_CPtr parent_untyped = 0;
    seL4_CPtr child_untyped = info->empty.start;

    // First, find an untyped big enough to fit all of our objects
    for (int i = 0; i < (info->untyped.end - info->untyped.start); i++) {
        if (info->untypedList[i].sizeBits >= untyped_size_bits && !info->untypedList[i].isDevice) {
            parent_untyped = info->untyped.start + i;
            break;
        }
    }
    // create an untyped big enough to retype all of the above objects from

    error = seL4_Untyped_Retype(parent_untyped, // the untyped capability to retype
                                seL4_UntypedObject, // type
                                untyped_size_bits,  //size
                                seL4_CapInitThreadCNode, // root
                                0, // node_index
                                0, // node_depth
                                child_untyped, // node_offset
                                1 // num_caps
                                );
    ZF_LOGF_IF(error != seL4_NoError, "Failed to retype");

    // use the slot after child_untyped for the new TCB cap:
    seL4_CPtr child_tcb = child_untyped + 1;

    /*
     * Created a TCBObject on the child_untyped
     */
    error = seL4_Untyped_Retype(child_untyped,
                                seL4_TCBObject,
                                seL4_TCBBits,
                                seL4_CapInitThreadCNode,
                                0,
                                0,
                                child_tcb,
                                1);
    ZF_LOGF_IF(error != seL4_NoError, "Failed to create TCB object");

    // try to set the TCB priority
    error = seL4_TCB_SetPriority(child_tcb, seL4_CapInitThreadTCB, 10);
    ZF_LOGF_IF(error != seL4_NoError, "Failed to set priority");

    // use the slot after child_tcb for the new endpoint cap:
    seL4_CPtr child_ep = child_tcb + 1;

    /*
     * Created an EndpointObject on the child_untyped
     */
    error = seL4_Untyped_Retype(child_untyped,
                                seL4_EndpointObject,
                                seL4_EndpointBits,
                                seL4_CapInitThreadCNode,
                                0,
                                0,
                                child_ep,
                                1);
    ZF_LOGF_IF(error != seL4_NoError, "Failed to create Endpoint object");

    // identify the type of child_ep
    uint32_t cap_id = seL4_DebugCapIdentify(child_ep);
    ZF_LOGF_IF(cap_id == 0, "Endpoint cap is null cap");

    // use the slot after child_ep for the new notification cap:
    seL4_CPtr child_ntfn = child_ep + 1;

    /*
     * Created a NotificationObject on the child_untyped
     */
    error = seL4_Untyped_Retype(child_untyped,
                                seL4_NotificationObject,
                                seL4_NotificationBits,
                                seL4_CapInitThreadCNode,
                                0,
                                0,
                                child_ntfn,
                                1);
    ZF_LOGF_IF(error != seL4_NoError, "Failed to create Notification object");

    // try to use child_ntfn
    error = seL4_TCB_BindNotification(child_tcb, child_ntfn);
    ZF_LOGF_IF(error != seL4_NoError, "Failed to bind notification.");

    /*
     * Revoked the child_untyped capability
     * This is interesting, because I thought parent_untyped was the _service
     * But no! service is the root CNode at in the CSpace
     * So parent_untyped is not even a CNode, it's just a capability
     * That's why I had to use seL4_CapInitThreadCNode as the _service
     */
    error = seL4_CNode_Revoke(seL4_CapInitThreadCNode, child_untyped, seL4_WordBits);
    ZF_LOGF_IF(error != seL4_NoError, "Failed to revoke child_untyped.");

    // allocate the whole child_untyped as endpoints
    // Remember the sizes are exponents, so this computes 2^untyped_size_bits / 2^seL4_EndpointBits:
    seL4_Word num_eps = BIT(untyped_size_bits - seL4_EndpointBits);
    error = seL4_Untyped_Retype(child_untyped, seL4_EndpointObject, 0, seL4_CapInitThreadCNode, 0, 0, child_tcb, num_eps);
    ZF_LOGF_IF(error != seL4_NoError, "Failed to create endpoints.");

    printf("Success\n");

    return 0;
}
