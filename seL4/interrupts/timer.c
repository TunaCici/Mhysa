

#include <stdio.h>
#include <assert.h>
#include <sel4/sel4.h>
#include <timer_driver/driver.h>

// CSlots pre-initialised in this CSpace
extern seL4_CPtr endpoint;
// capability to a reply object
extern seL4_CPtr ntfn;
// capability to the device untyped for the timer

extern seL4_CPtr device_untyped;
// empty cslot for the frame
extern seL4_CPtr timer_frame;
// cnode of this process
extern seL4_CPtr cnode;
// vspace of this process
extern seL4_CPtr vspace;
// frame to map the timer to
extern seL4_CPtr frame;
extern const char timer_vaddr[4096];
// irq control capability
extern seL4_CPtr irq_control;
// empty slot for the irq
extern seL4_CPtr irq_handler;

/* constants */
#define EP_BADGE 61 // arbitrary (but unique) number for a badge
#define MSG_DATA 0x6161 // arbitrary data to send

#define DEFAULT_TIMER_ID 0
#define TTC0_TIMER1_IRQ 42

int main(void) {
    /* wait for a message */
    seL4_Word sender_badge;
    seL4_MessageInfo_t tag = seL4_Recv(endpoint, &sender_badge);

    /* make sure the message is what we expected */
    assert(sender_badge == EP_BADGE);
    assert(seL4_MessageInfo_get_length(tag) == 1);

    /* get the message stored in the first message register */
    seL4_Word msg = seL4_GetMR(0);
    printf("timer: got a message from %u to sleep %zu seconds\n", sender_badge, msg);

    /* retype the device untyped into a frame */
    seL4_Error error = seL4_Untyped_Retype(device_untyped, seL4_ARM_SmallPageObject, 0,
                                          cnode, 0, 0, timer_frame, 1);
    ZF_LOGF_IF(error, "Failed to retype device untyped");

    /* unmap the existing frame mapped at vaddr so we can map the timer here */
    error = seL4_ARM_Page_Unmap(frame);
    ZF_LOGF_IF(error, "Failed to unmap frame");

    /* map the device frame into the address space */
    error = seL4_ARM_Page_Map(timer_frame, vspace, (seL4_Word) timer_vaddr, seL4_AllRights, 0);
    ZF_LOGF_IF(error, "Failed to map device frame");

    timer_drv_t timer_drv = {0};

    

    /*
     * irq_control: is basicaly a collection of all irq_handlers given to root
     * TTC0_TIMER1_IRQ: is the interrupt number we want to get
     * irq_handler: is the cslot of the received interrupt handler
     * 
     * I like this way of managing inital interrupts. Just collect them together
     * in irq_control and then give it to the root task to manage the rest
     */
    error = seL4_IRQControl_Get(irq_control, TTC0_TIMER1_IRQ, cnode, irq_handler, seL4_WordBits);
    ZF_LOGF_IF(error, "Failed to get TTC0_TIMER1_IRQ from irq_control");
    

    /*
     * Now we simply register the irq_handler to a notfication object
     */
    error = seL4_IRQHandler_SetNotification(irq_handler, ntfn);
    ZF_LOGF_IF(error, "Failed to register irq_handler to ntfn");

    /* set up the timer driver */
    int timer_err = timer_init(&timer_drv, DEFAULT_TIMER_ID, (void *) timer_vaddr);
    ZF_LOGF_IF(timer_err, "Failed to init timer");

    timer_err = timer_start(&timer_drv);
    ZF_LOGF_IF(timer_err, "Failed to start timer");

    /* ack the irq in case of any pending interrupts int the driver */
    error = seL4_IRQHandler_Ack(irq_handler);
    ZF_LOGF_IF(error, "Failed to ack irq");

    timer_err = timer_set_timeout(&timer_drv, NS_IN_MS, true);
    ZF_LOGF_IF(timer_err, "Failed to set timeout");

    int count = 0;
    while (1) {
        /* Handle the timer interrupt */
        seL4_Word badge;
        seL4_Wait(ntfn, &badge);
        timer_handle_irq(&timer_drv);
        if (count == 0) {
            printf("Tick\n");
        }
        
        /*
         * acknowledging an interrupts (and implicitly unmasking it) is pretty
         * straightforward as well. I am loving this
         */
        seL4_IRQHandler_Ack(irq_handler);

        count++;
        if (count == 1000 * msg) {
            break;
        }
    }

    // stop the timer
    timer_stop(&timer_drv);

   /* modify the message */
    seL4_SetMR(0, 0);

    /* send the modified message back */
    seL4_ReplyRecv(endpoint, tag, &sender_badge);

    return 0;
}