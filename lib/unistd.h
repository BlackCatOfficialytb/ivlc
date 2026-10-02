/* Minimal unistd.h stub for IntelliSense on Windows (POSIX subset) */
#ifndef _UNISTD_H
#define _UNISTD_H

/* On Apple platforms, include the real unistd.h */
#if defined(__APPLE__) || defined(__MACH__)
#include <unistd.h>
#else

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef long ssize_t;
typedef int pid_t;
typedef unsigned int uid_t;
typedef unsigned int gid_t;
typedef long off_t;
typedef unsigned int mode_t;

int access(const char *pathname, int mode);
int chdir(const char *path);
char *getcwd(char *buf, size_t size);
int chown(const char *path, uid_t owner, gid_t group);
int close(int fd);
size_t confstr(int name, char *buf, size_t len);
char *crypt(const char *key, const char *salt);
int dup(int fd);
int dup2(int oldfd, int newfd);
void _exit(int status);
int execve(const char *filename, char *const argv[], char *const envp[]);
int execv(const char *path, char *const argv[]);
int execvp(const char *file, char *const argv[]);
pid_t fork(void);
long fpathconf(int fd, int name);
char *getlogin(void);
pid_t getpid(void);
pid_t getppid(void);
uid_t getuid(void);
uid_t geteuid(void);
gid_t getgid(void);
gid_t getegid(void);
int isatty(int fd);
int link(const char *oldpath, const char *newpath);
off_t lseek(int fd, off_t offset, int whence);
int pipe(int pipefd[2]);
ssize_t read(int fd, void *buf, size_t count);
int rmdir(const char *path);
int setgid(gid_t gid);
int setuid(uid_t uid);
unsigned int sleep(unsigned int seconds);
long sysconf(int name);
pid_t tcgetpgrp(int fd);
int tcsetpgrp(int fd, pid_t pgrp);
char *ttyname(int fd);
int unlink(const char *pathname);
ssize_t write(int fd, const void *buf, size_t count);

#define F_OK 0
#define R_OK 4
#define W_OK 2
#define X_OK 1

#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

#define STDIN_FILENO 0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2

#define O_RDONLY 0
#define O_WRONLY 1
#define O_RDWR 2

#ifdef __cplusplus
}
#endif

#endif /* __APPLE__ || __MACH__ */
#endif /* _UNISTD_H */