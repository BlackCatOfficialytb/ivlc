/* Minimal spawn.h stub for IntelliSense on Windows */
#ifndef _SPAWN_H
#define _SPAWN_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct posix_spawn_file_actions_t posix_spawn_file_actions_t;
typedef struct posix_spawnattr_t posix_spawnattr_t;

extern char **environ;

int posix_spawn(pid_t *pid, const char *path,
                const posix_spawn_file_actions_t *file_actions,
                const posix_spawnattr_t *attrp,
                char *const argv[], char *const envp[]);

int posix_spawn_file_actions_init(posix_spawn_file_actions_t *file_actions);
int posix_spawn_file_actions_destroy(posix_spawn_file_actions_t *file_actions);
int posix_spawn_file_actions_addopen(posix_spawn_file_actions_t *file_actions,
                                     int fd, const char *path, int oflag, mode_t mode);
int posix_spawn_file_actions_addclose(posix_spawn_file_actions_t *file_actions, int fd);
int posix_spawn_file_actions_adddup2(posix_spawn_file_actions_t *file_actions, int fd, int newfd);

#ifdef __cplusplus
}
#endif

#endif /* _SPAWN_H */