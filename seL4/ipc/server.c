

#include <assert.h>
#include <sel4/sel4.h>
#include <stdio.h>
#include <utils/util.h>

// cslot containing IPC endpoint capability
extern seL4_CPtr endpoint;
// cslot containing a capability to the cnode of the server
extern seL4_CPtr cnode;
// empty cslot
extern seL4_CPtr free_slot;

int main(int c, char *argv[]) {

	seL4_Word sender;
    seL4_MessageInfo_t info = seL4_Recv(endpoint, &sender);
    while (1) {
	    seL4_Error error;
        if (sender == 0) {

             /* No badge! give this sender a badged copy of the endpoint */
             seL4_Word badge = seL4_GetMR(0);
             seL4_Error error = seL4_CNode_Mint(cnode, free_slot, seL4_WordBits,
                                                cnode, endpoint, seL4_WordBits,
                                                seL4_AllRights, badge);
             printf("Badged %lu\n", badge);

             /*
              * The concept of transfering capabilites is intersting
              * I need to study this a lot more and get myself familier with it
              *
              * Here's what going on below,
              * 1. (Assumption) client set up it's receive paths (cnode, slot)
              * 2. Modify the message info (inside IPC buffer)
              * 2.1. label is '0' because we don't use them (refer to manual)
              * 2.2. capsUnwrapped is '0' because IDK????
              * 2.3. extraCaps is '1' because we are only transfering one cap.
              * 2.4. length is '0' because we don't send a message; only cap
              * 3. Use a helper to set the idx '0' of 'caps' arr inside IPC buff
              * 3.1. By doing this we are Granting the badged endpoint to client
              * 3.2. How is this different than copying I wonder...
              * 4. Later 'seL4_Reply' is called that completes the transfer
              */
             info = seL4_MessageInfo_new(0, 0, 1, 0);
             seL4_SetCap(0, free_slot);

             /* reply to the sender and wait for the next message */
             seL4_Reply(info);

             /* now delete the transferred cap */
             error = seL4_CNode_Delete(cnode, free_slot, seL4_WordBits);
             assert(error == seL4_NoError);

              /* wait for the next message */
             info = seL4_Recv(endpoint, &sender);
        } else {

             /*
              * Print all the messages
              */
             for (int i = 0; i < seL4_MessageInfo_get_length(info); i++) {
               printf("%c", (char) seL4_GetMR(i));
             }

             printf("\n");

             /*
              * Reply to the client and wait to Recv the next message
              */
             info = seL4_MessageInfo_new(0, 0, 0, 0);
             info = seL4_ReplyRecv(endpoint, info, &sender);
        }
    }

    return 0;
}