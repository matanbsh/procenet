#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/list.h>
#include <linux/errno.h>
#include <asm/uaccess.h>
#include <linux/procenet.h>

asmlinkage long sys_procenet_respond(pid_t pid, int response)
{
    struct task_struct *self = current;
    struct invitation_t *inv;
    struct list_head   *pos, *n;
    struct task_struct *sender;
    struct friend_t   new_friend;
    int               i;

    /* Validate response */
    if (response != 0 && response != 1)
        return -EINVAL;

    /* Search for the invitation */
    list_for_each_safe(pos, n, &self->invitations) {
        inv = list_entry(pos, struct invitation_t, list);
        if (inv->pid == pid) {
            /* If accepted, add to friends */
            if (response == 1) {
                if (self->friend_count >= MAX_FRIENDS)
                    return -ENOSPC;

                /* Add sender to self */
                new_friend.pid   = pid;
                new_friend.alive = 1;
                self->friends[self->friend_count++] = new_friend;
           
                /* Add self to sender, if still space */
                sender = find_task_by_pid(pid);
                if (sender && sender != self && sender->friend_count < MAX_FRIENDS) {
                    new_friend.pid   = self->pid;
                    new_friend.alive = 1;
                    sender->friends[sender->friend_count++] = new_friend;
                }
            }

            /* Remove the invitation node */
            list_del(pos);
            kfree(inv);
            return 0;
        }
    }

    /* No such invitation found */
    return -ESRCH;
}
