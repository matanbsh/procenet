#ifndef _LINUX_PROCENET_H
#define _LINUX_PROCENET_H

#include <linux/types.h>    
#include <linux/list.h>     
#include <linux/errno.h>    
#include <linux/linkage.h>  

#define MAX_MESSAGE 128
#define MAX_FRIENDS 32

struct invitation_t {
    pid_t pid;
    char message[MAX_MESSAGE];
    struct list_head list;
};

struct friend_t {
    pid_t pid;
    int alive;
};

struct user_invitation{
   pid_t pid;
   char message[MAX_MESSAGE];
};

/* Prototypes of the syscall handlers */
asmlinkage long    sys_procenet_invite(pid_t pid, char  *message);
asmlinkage long    sys_procenet_respond(pid_t pid, int response);
asmlinkage ssize_t sys_procenet_invitations(struct invitation_t  *invitations, ssize_t size);
asmlinkage ssize_t sys_procenet_info(pid_t pid, struct friend_t  *friends);

#endif /* _LINUX_PROCENET_H */
