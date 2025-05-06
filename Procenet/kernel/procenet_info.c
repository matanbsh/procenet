#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/errno.h>
#include <asm/uaccess.h>
#include <linux/procenet.h>


asmlinkage ssize_t sys_procenet_info(pid_t pid, struct friend_t *friends) {
    struct task_struct *target;
    int count, i;

    if (!friends)
        return -EINVAL;

    target = find_task_by_pid(pid);
    if (!target)
        return -ESRCH;

    count = target->friend_count;
    for (i = 0; i < count; i++) {
        if (copy_to_user(&friends[i], &target->friends[i], sizeof(struct friend_t)))
            return -EFAULT;
    }

    return count;  // Number of friends copied
}
