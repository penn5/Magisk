/* util.h - Header for all utility functions
 */

#ifndef _UTILS_H_
#define _UTILS_H_

#include <stdio.h>
#include <dirent.h>
#include <pthread.h>
#include <poll.h>
#include <sys/socket.h>
#include <sys/stat.h>

#ifdef __cplusplus

#include "array.h"
#include "CharArray.h"
#include "cpputils.h"

int file_to_array(const char *filename, Array<CharArray> &arr);
char *strdup2(const char *s, size_t *size = nullptr);


extern "C" {
#endif

#define UID_SHELL  (get_shell_uid())
#define UID_ROOT   0
#define UID_SYSTEM (get_system_uid())
#define UID_RADIO  (get_radio_uid())

// xwrap.c

FILE *xfopen(const char *pathname, const char *mode);
FILE *xfdopen(int fd, const char *mode);
#define GET_MACRO(_1, _2, _3, NAME, ...) NAME
#define xopen(...) GET_MACRO(__VA_ARGS__, xopen3, xopen2)(__VA_ARGS__)
int xopen2(const char *pathname, int flags);
int xopen3(const char *pathname, int flags, mode_t mode);
int xopenat(int dirfd, const char *pathname, int flags);
ssize_t xwrite(int fd, const void *buf, size_t count);
ssize_t xread(int fd, void *buf, size_t count);
ssize_t xxread(int fd, void *buf, size_t count);
int xpipe2(int pipefd[2], int flags);
int xsetns(int fd, int nstype);
int xunshare(int flags);
DIR *xopendir(const char *name);
DIR *xfdopendir(int fd);
struct dirent *xreaddir(DIR *dirp);
pid_t xsetsid();
int xsocket(int domain, int type, int protocol);
int xbind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
int xconnect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
int xlisten(int sockfd, int backlog);
int xaccept4(int sockfd, struct sockaddr *addr, socklen_t *addrlen, int flags);
void *xmalloc(size_t size);
void *xcalloc(size_t nmemb, size_t size);
void *xrealloc(void *ptr, size_t size);
ssize_t xsendmsg(int sockfd, const struct msghdr *msg, int flags);
ssize_t xrecvmsg(int sockfd, struct msghdr *msg, int flags);
int xpthread_create(pthread_t *thread, const pthread_attr_t *attr,
	void *(*start_routine) (void *), void *arg);
int xsocketpair(int domain, int type, int protocol, int sv[2]);
int xstat(const char *pathname, struct stat *buf);
int xlstat(const char *pathname, struct stat *buf);
int xdup2(int oldfd, int newfd);
int xdup3(int oldfd, int newfd, int flags);
ssize_t xreadlink(const char *pathname, char *buf, size_t bufsiz);
ssize_t xreadlinkat(int dirfd, const char *pathname, char *buf, size_t bufsiz);
int xsymlink(const char *target, const char *linkpath);
int xsymlinkat(const char *target, int newdirfd, const char *linkpath);
int xlinkat(int olddirfd, const char *oldpath, int newdirfd, const char *newpath, int flags);
int xmount(const char *source, const char *target,
	const char *filesystemtype, unsigned long mountflags,
	const void *data);
int xumount(const char *target);
int xumount2(const char *target, int flags);
int xrename(const char *oldpath, const char *newpath);
int xmkdir(const char *pathname, mode_t mode);
int xmkdirs(const char *pathname, mode_t mode);
int xmkdirat(int dirfd, const char *pathname, mode_t mode);
void *xmmap(void *addr, size_t length, int prot, int flags,
	int fd, off_t offset);
ssize_t xsendfile(int out_fd, int in_fd, off_t *offset, size_t count);
pid_t xfork();
int xpoll(struct pollfd *fds, nfds_t nfds, int timeout);

// misc.c

unsigned get_shell_uid();
unsigned get_system_uid();
unsigned get_radio_uid();
ssize_t fdgets(char *buf, size_t size, int fd);
int is_num(const char *s);
int exec_array(int err, int *fd, void (*cb)(void), const char *argv[]);
int exec_command(int err, int *fd, void (*cb)(void), const char *argv0, ...);
int exec_command_sync(const char *argv0, ...);
int switch_mnt_ns(int pid);
int fork_dont_care();
void gen_rand_str(char *buf, int len);
int strend(const char *s1, const char *s2);

#define getline __getline
#define getdelim __getdelim
#define fsetxattr __fsetxattr

ssize_t __getline(char **lineptr, size_t *n, FILE *stream);
ssize_t __getdelim(char **lineptr, size_t *n, int delim, FILE *stream);
int __fsetxattr(int fd, const char *name, const void *value, size_t size, int flags);

// file.c

#define align(p, a)     (((p) + (a) - 1) / (a) * (a))
#define align_off(p, a) (align(p, a) - (p))

extern const char **excl_list;

struct file_attr {
	struct stat st;
	char con[128];
};

int fd_getpath(int fd, char *path, size_t size);
int fd_getpathat(int dirfd, const char *name, char *path, size_t size);
int mkdirs(const char *pathname, mode_t mode);
void in_order_walk(int dirfd, void (*callback)(int, struct dirent*));
void rm_rf(const char *path);
void frm_rf(int dirfd);
void mv_f(const char *source, const char *destination);
void mv_dir(int src, int dest);
void cp_afc(const char *source, const char *destination);
void link_dir(int src, int dest);
void clone_dir(int src, int dest);
int getattr(const char *path, struct file_attr *a);
int getattrat(int dirfd, const char *pathname, struct file_attr *a);
int fgetattr(int fd, struct file_attr *a);
int setattr(const char *path, struct file_attr *a);
int setattrat(int dirfd, const char *pathname, struct file_attr *a);
int fsetattr(int fd, struct file_attr *a);
void fclone_attr(const int sourcefd, const int targetfd);
void clone_attr(const char *source, const char *target);
void mmap_ro(const char *filename, void **buf, size_t *size);
void mmap_rw(const char *filename, void **buf, size_t *size);
void fd_full_read(int fd, void **buf, size_t *size);
void full_read(const char *filename, void **buf, size_t *size);
void full_read_at(int dirfd, const char *filename, void **buf, size_t *size);
void stream_full_read(int fd, void **buf, size_t *size);
void write_zero(int fd, size_t size);

#ifdef __cplusplus
}
#endif

#endif
