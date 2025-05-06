#ifndef PROCENET_API_H
#define PROCENET_API_H

#include <errno.h>
#include <unistd.h>
#include <sys/types.h>

#define MAX_MESSAGE 128
#define MAX_FRIENDS 32

struct invitation_t {
    pid_t pid;
    char message[MAX_MESSAGE];
};

struct friend_t {
    pid_t pid;
    int alive; 
};
   
   
static inline int procenet_invite(pid_t pid, const char *message) {
    long res;
    __asm__ (
        "pushl %%eax;"
        "pushl %%ebx;"
        "pushl %%ecx;"
        "movl $243, %%eax;"        
        "movl %1, %%ebx;"         // first argument: pid
        "movl %2, %%ecx;"         // second argument: message ptr
        "int $0x80;"              
        "movl %%eax, %0;"         // store return value
        "popl %%ecx;"
        "popl %%ebx;"
        "popl %%eax;"
        : "=m" (res)
        : "m" (pid), "m" (message)
        : "eax", "ebx", "ecx"
    );

    if (res < 0) {
        errno = -res;               // set errno to positive error code
        return -1;                  // failure
    }
    return (int) res;                     // 0 on success
}

static inline int procenet_respond(pid_t pid, int response) {
    long res;
    __asm__ (
        "pushl %%eax;"
        "pushl %%ebx;"
        "pushl %%ecx;"
        "movl $244, %%eax;"        
        "movl %1, %%ebx;"         // first argument: pid
        "movl %2, %%ecx;"         // second argument: message ptr
        "int $0x80;"              
        "movl %%eax, %0;"         // store return value
        "popl %%ecx;"
        "popl %%ebx;"
        "popl %%eax;"
        : "=m" (res)
        : "m" (pid), "m" (response)
        : "eax", "ebx", "ecx"
    );

    if (res < 0) {
        errno = -res;               // set errno to positive error code
        return -1;                  // failure
    }
    return (int) res;                     // 0 on success
}

static inline ssize_t procenet_invitations(struct invitation_t *invitations, ssize_t size) {
    ssize_t res;
    __asm__ (
        "pushl %%eax;"
        "pushl %%ebx;"
        "pushl %%ecx;"
        "movl $245, %%eax;"             
        "movl %1, %%ebx;"              
        "movl %2, %%ecx;"              
        "int $0x80;"                   
        "movl %%eax, %0;"             
        "popl %%ecx;"
        "popl %%ebx;"
        "popl %%eax;"
        : "=m"(res)
        : "m"(invitations), "m"(size)
        : "eax", "ebx", "ecx"
    );

    if (res < 0) {
        errno = -res;   
        return -1;      
    }
    return res;        // success: number of invitations not written (or count)
}

static inline ssize_t procenet_info(pid_t pid, struct friend_t* friends) {
    ssize_t res;
    __asm__ (
        "pushl %%eax;"
        "pushl %%ebx;"
        "pushl %%ecx;"
        "movl $246, %%eax;"             
        "movl %1, %%ebx;"              
        "movl %2, %%ecx;"              
        "int $0x80;"                   
        "movl %%eax, %0;"             
        "popl %%ecx;"
        "popl %%ebx;"
        "popl %%eax;"
        : "=m"(res)
        : "m"(pid), "m"(friends)
        : "eax", "ebx", "ecx"
    );

    if (res < 0) {
        errno = -res;   
        return -1;      
    }
    return res; // success: The number of friends written to friends
}

#endif // PROCENET_API_H
