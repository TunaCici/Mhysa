#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <unistd.h>
#include <sched.h>
#include <fcntl.h>
#include <limits.h>
#include <sys/types.h>
#include <dirent.h>
#include <linux/sched.h>

#define PROG_NAME "read-proc-stat"
#define PROC_STAT_BUFF_SIZE 512u // /proc/.../stat buffer size in bytes
#define PROC_SCHED_BUFF_SIZE 32u // scheduling policy string size in bytes

static const char sched_rr[] = "SCHED_RR";
static const char sched_fifo[] = "SCHED_FIFO";
static const char sched_deadline[] = "SCHED_DEADLINE";

static const char sched_other[] = "SCHED_OTHER";
static const char sched_batch[] = "SCHED_BATCH";
static const char sched_idle[] = "SCHED_IDLE";
static const char sched_unkown[] = "SCHED_UNKOWN";

/*
 * Format of '/proc/<PID>/task/<TID>'
 *
 * See: https://www.kernel.org/doc/Documentation/filesystems/proc.txt
 * Also: $ man proc
 */

typedef struct proc_stat_t {
        int             pid;
        char            comm[NAME_MAX];
        char            state;

        int             ppid;
        int             pgrp;
        int             session;
        int             tty_nr;
        int             tgpid;

        unsigned int    flags;
        unsigned long   minflt;
        unsigned long   cminflt;
        unsigned long   majflt;
        unsigned long   cmajflt;

        unsigned long   utime;
        unsigned long   stime;
        unsigned int    cutime;
        unsigned int    cstime;

        long            priority;
        long            nice;
        long            num_threads;
        long            iter_value;

        unsigned long long      starttime;

        unsigned long   vsize;
        long            rss;
        unsigned long   rsslim;
        unsigned long   startcode;
        unsigned long   endcode;
        unsigned long   startstack;
        unsigned long   kstkesp;
        unsigned long   kstkeip;

        unsigned long   signal;
        unsigned long   blocked;
        unsigned long   sigignore;
        unsigned long   sigcatch;
        unsigned long   wchan;
        unsigned long   nswap;
        unsigned long   cnswap;

        int             exit_signal;
        int             processor;
        unsigned int    rt_priority;
        unsigned int    policy;

        unsigned long long      delayacct_blkio_ticks;
        
        unsigned long   guest_time;
        long            cguest_time;
        unsigned long   start_data;
        unsigned long   end_data;
        unsigned long   start_brk;
        unsigned long   arg_start;
        unsigned long   arg_end;
        unsigned long   env_start;
        unsigned long   env_end;

        int exit_code;
} proc_stat_t;

/*
 * ?
 */
static void read_proc_stat(proc_stat_t *proc, int pid, int tid)
{
        int fd = 0;

        static mode_t mode = O_RDONLY;
        static char buff[PROC_STAT_BUFF_SIZE] = {0};
        static char pathname[NAME_MAX] = {'\0'};

        if (!proc) {
                return;
        }

        sprintf(pathname, "/proc/%d/task/%d/stat", pid, tid);

        fd = open(pathname, mode);

        if (!fd) {
                fprintf(stderr, "%s: unable to open '%s'\n",
                        PROG_NAME, pathname);
                exit(EXIT_FAILURE);
        }

        ssize_t offset = lseek(fd, 0, SEEK_SET);
        ssize_t bytes_read = read(fd, buff, PROC_STAT_BUFF_SIZE);

        if (offset == -1 || bytes_read == -1) {
                fprintf(stderr, "%s: unable to read '%s'",
                        PROG_NAME, pathname);
                exit(EXIT_FAILURE);
        }

        sscanf(buff, "%d %s %c "
                     "%d %d %d %d %d "
                     "%u %lu %lu %lu %lu "
                     "%lu %lu %u %u "
                     "%ld %ld %ld %ld "
                     "%llu "
                     "%lu %ld %lu %lu %lu %lu %lu %lu "
                     "%lu %lu %lu %lu %lu %lu %lu "
                     "%d %d %u %u "
                     "%llu "                            
                     "%lu %ld %lu %lu %lu %lu %lu %lu %lu "
                     "%d",
               &proc->pid,
               proc->comm,
               &proc->state,

               &proc->ppid,
               &proc->pgrp,
               &proc->session,
               &proc->tty_nr,
               &proc->tgpid,

               &proc->flags,
               &proc->minflt,
               &proc->cminflt,
               &proc->majflt,
               &proc->cmajflt,

               &proc->utime,
               &proc->stime,
               &proc->cutime,
               &proc->cstime,

               &proc->priority,
               &proc->nice,
               &proc->num_threads,
               &proc->iter_value,

               &proc->starttime,

               &proc->vsize,
               &proc->rss,
               &proc->rsslim,
               &proc->startcode,
               &proc->endcode,
               &proc->startstack,
               &proc->kstkesp,
               &proc->kstkeip,

               &proc->signal,
               &proc->blocked,
               &proc->sigignore,
               &proc->sigcatch,
               &proc->wchan,
               &proc->nswap,
               &proc->cnswap,

               &proc->exit_signal,
               &proc->processor,
               &proc->rt_priority,
               &proc->policy,

               &proc->delayacct_blkio_ticks,

               &proc->guest_time,
               &proc->cguest_time,
               &proc->start_data,
               &proc->end_data,
               &proc->start_brk,
               &proc->arg_start,
               &proc->arg_end,
               &proc->env_start,
               &proc->env_end,

               &proc->exit_code);
}

static void print_threads(proc_stat_t *proc, DIR *proc_dir, int pid)
{
        if (!proc || !proc_dir) {
                return;
        }
        
        static const char *policy = NULL;
        static struct dirent *entry = NULL;

        printf("TID\tNAME\t\tPROCESSOR\tPOLICY\t\tPRIO\tRT_PRIO\n");

        while (entry = readdir(proc_dir)) {
                /* Skip '.' and '..' directory thingies */
                if (strchr(entry->d_name, '.')) {
                        continue;
                }

                int tid = atoi(entry->d_name);
                
                read_proc_stat(proc, pid, tid);

                switch (proc->policy) {
                case SCHED_RR:
                        policy = sched_rr;
                break;
                case SCHED_FIFO:
                        policy = sched_fifo;
                break;
                case SCHED_DEADLINE:
                        policy = sched_deadline;
                break;
                case SCHED_OTHER:
                        policy = sched_other;
                break;
                case SCHED_BATCH:
                        policy = sched_batch;
                break;
                case SCHED_IDLE:
                        policy = sched_idle;
                break;
                default:
                        policy = sched_unkown;
                }

                printf("%d\t%s\t\t%d\t%s\t%ld\t%u\n",
                        proc->pid, proc->comm, proc->processor, policy,
                        proc->priority, proc->rt_priority);
        }
}

int main(int argc, char **argv)
{
        proc_stat_t proc = {0};
        
        int pid = 0;

        char proc_path[NAME_MAX] = {'\0'};
        DIR *proc_dir = NULL;
        struct dirent *proc_dir_entry = NULL;

        if (argc != 2) {
                fprintf(stderr, "usage: ./%s <PID>\n", PROG_NAME);
                exit(EXIT_FAILURE);
        }

        pid = atoi(argv[1]);
        sprintf(proc_path, "/proc/%d/task/", pid);
        proc_dir = opendir(proc_path);

        if (!proc_dir) {
                fprintf(stderr, "%s: couldn't open '%s'\n",
                        PROG_NAME, proc_path);
                exit(EXIT_FAILURE);
        }


        while (1) {
                printf("\033[H\033[J");

                /* Others */
                print_threads(&proc, proc_dir, pid);
                rewinddir(proc_dir);

                usleep(1e6);
        }

        return EXIT_SUCCESS;
}
