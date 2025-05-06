#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/list.h>
#include <linux/string.h>
#include <linux/errno.h>
#include <asm/uaccess.h>
#include <linux/procenet.h>

asmlinkage ssize_t sys_procenet_invitations(struct invitation_t *invitations, ssize_t size) {
    struct list_head *pos;
    struct invitation_t *inv;
    struct user_invitation tmp;

    ssize_t total;
    ssize_t copied;
    int i;

    /* initialize counters */
    total  = 0;
    copied = 0;

    /* Invalid size argument */
    if (size < 0){
        return -EINVAL;
    }

    // Count total pending invitations
    list_for_each(pos, &current->invitations) {
        total++;
    }

    // If only counting or buffer is NULL, return total count
    if (size == 0 || invitations == NULL)
        return total;

    // Copy up to 'size' invitations to user buffer
    list_for_each(pos, &current->invitations) {
        if (copied >= size)
            break;
        inv = list_entry(pos, struct invitation_t, list);
        tmp.pid = inv->pid;
        strncpy(tmp.message, inv->message, MAX_MESSAGE - 1);
        tmp.message[MAX_MESSAGE - 1] = '\0';
	
	
        if (copy_to_user(&((struct user_invitation *) invitations)[copied], &tmp, sizeof(tmp))){
            return -EFAULT;
	}
        copied++;
    }

    // Pad remaining entries with empty values
    for (i = copied; i < size; i++) {
        tmp.pid = 0;
        tmp.message[0] = '\0';
        if (copy_to_user(&((struct user_invitation *) invitations)[i], &tmp, sizeof(tmp))){
            return -EFAULT;
	}
    }

    // Return number of invitations not written
    if (total > size)
        return total - size;
    return 0;
}
