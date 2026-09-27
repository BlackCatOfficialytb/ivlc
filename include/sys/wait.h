/* Minimal sys/wait.h stub for IntelliSense on Windows */
#ifndef _SYS_WAIT_H
#define _SYS_WAIT_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef int pid_t;

pid_t wait(int *status);
pid_t waitpid(pid_t pid, int *status, int options);

#define WNOHANG    1
#define WUNTRACED  2

#define WIFEXITED(status)   (((status) & 0x7f) == 0)
#define WEXITSTATUS(status) (((status) >> 8) & 0xff)
#define WIFSIGNALED(status) (((status) & 0x7f) != 0x7f && ((status) & 0x7f) != 0)
#define WTERMSIG(status)    ((status) & 0x7f)
#define WIFSTOPPED(status)  (((status) & 0xff) == 0x7f)
#define WSTOPSIG(status)    (((status) >> 8) & 0xff)

#ifdef __cplusplus
}
#endif

#endif /* _SYS_WAIT_H */