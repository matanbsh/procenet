#include <linux/kernel.h>      
#include <linux/sched.h>       
#include <linux/slab.h>        
#include <linux/string.h>            
#include <linux/errno.h>       
#include <asm/uaccess.h>       
#include <linux/procenet.h> 


asmlinkage long sys_procenet_invite(pid_t pid, char *message){
   struct task_struct *sender = current;
    struct task_struct *receiver;
    struct invitation_t *inv;
    struct list_head *pos;
    char local_msg[MAX_MESSAGE];
    int i;

    if(pid<1)
       return -EINVAL;

    if(pid == sender->pid){ //self-invitation
        receiver = sender;
    } else {
        receiver = find_task_by_pid(pid); //lookup by PID
        if (!receiver)
            return -ESRCH;
    }
    
    /* Block only if sender already has an alive friend==pid */
    for (i = 0; i < sender->friend_count; i++) {
     if (sender->friends[i].pid == pid && sender->friends[i].alive) {
         return -EEXIST;
     }
 }
 

    // Check if an invitation from this sender is already pending
    list_for_each(pos, &receiver->invitations){
        inv = list_entry(pos, struct invitation_t, list);
        if (inv->pid == sender->pid)
            return 0;
    }

    // Prepare the invitation message
    if (!message){
        strncpy(local_msg, "Hi! Please be my friend", MAX_MESSAGE);
    } else {
        if(copy_from_user(local_msg, message, MAX_MESSAGE))
            return -EFAULT;
        local_msg[MAX_MESSAGE - 1] = '\0';
    }
    
    // Allocate memory for a new invitation structure
    inv = kmalloc(sizeof(*inv), GFP_KERNEL);
    
    if (!inv){
       return -ENOMEM;
    }
    // Initialize and insert the new invitation
    inv->pid = sender->pid;
    strncpy(inv->message, local_msg, MAX_MESSAGE);
    INIT_LIST_HEAD(&inv->list);
    list_add_tail(&inv->list, &receiver->invitations);

    return 0; 
}
