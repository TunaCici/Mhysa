#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include <unistd.h>
#include <pthread.h>
#include <sched.h>
#include <linux/sched.h>
#include <linux/prctl.h>
#include <sys/prctl.h>
#include <sys/time.h>

#define PROG_NAME "ping-pong"
#define PING_PROCESSOR 3
#define PONG_PROCESSOR 2
#define PING_SCHED_PRIO 80
#define PONG_SCHED_PRIO 80

/* Binary-Compare-And-Swap */
#define BCAS(ptr, expected, desired) \
                __atomic_compare_exchange_n(ptr, expected, desired, 0, \
                        __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST)

/* These are variables, but should be used as 'const's due to BCAS */
static int OP_NONE = 0;
static int OP_PING = 1;
static int OP_PONG = 2;

/* Atomics */
static int table = 0;
static unsigned long long ping_missed = 0;
static unsigned long long pong_missed = 0;

static unsigned long long ping_hit = 0;
static unsigned long long pong_hit = 0;

static long get_usec()
{
        static struct timeval currentTime = {0};

	gettimeofday(&currentTime, NULL);

	return currentTime.tv_sec * (long) 1e6 + currentTime.tv_usec;       
}

static void *ping_guy(void *arg)
{
        unsigned int cycle = *((unsigned int*) arg);
        cpu_set_t set = {0};

        CPU_ZERO(&set);
        CPU_SET(PING_PROCESSOR, &set);

        int res = prctl(PR_SET_NAME, "ping_thread");
        res = sched_setaffinity(gettid(), sizeof(set), &set);

        /* Start ping ponging */
        if (table == OP_NONE) {
                BCAS(&table, &OP_NONE, OP_PING);
        }

        long begin_time = get_usec();

        while (1) {
                begin_time = get_usec();

                if (table == OP_PONG) {
                        BCAS(&table, &OP_PONG, OP_PING);
                        ping_hit++;
                }

                /* DEBUG: does the scheduler respect me? */
                if (sched_getcpu() != PING_PROCESSOR) {
                        fprintf(stderr, "ping_guy: not scheduled on %d\n",
                                PING_PROCESSOR);
                        exit(EXIT_FAILURE);
                }

                long elapsed = (get_usec() - begin_time);
                if (cycle < elapsed) {
                        ping_missed++;
                        continue;
                }

                usleep(cycle - elapsed);
        }
}

static void *pong_guy(void *arg)
{
        unsigned int cycle = *((unsigned int*) arg);
        cpu_set_t set = {0};

        CPU_ZERO(&set);
        CPU_SET(PONG_PROCESSOR, &set);

        int res = prctl(PR_SET_NAME, "pong_thread");
        res = sched_setaffinity(gettid(), sizeof(set), &set);

        long begin_time = get_usec();

        while (1) {
                long begin_time = get_usec();

                if (table == OP_PING) {
                        BCAS(&table, &OP_PING, OP_PONG);
                        pong_hit++;
                }

                /* DEBUG: does the scheduler respect me? */
                if (sched_getcpu() != PONG_PROCESSOR) {
                        fprintf(stderr, "pong_guy: not scheduled on %d\n",
                                PONG_PROCESSOR);
                        exit(EXIT_FAILURE);
                }

                long elapsed = (get_usec() - begin_time);
                if (cycle < elapsed) {
                        pong_missed++;
                        continue;
                }

                usleep(cycle - elapsed);
        }
}

int main(int argc, char **argv)
{
        if (argc != 3) {
                fprintf(stderr,
                        "%s: usage: "
                        "./%s <PING CYCLE (us)> <PONG CYCLE (us)>\n",
                        PROG_NAME, PROG_NAME);
                exit(EXIT_FAILURE);
        }

        unsigned int ping_cycle = strtoul(argv[1], NULL, 10);
        unsigned int pong_cycle = strtoul(argv[2], NULL, 10);

        pthread_t ping_thr = 0;
        pthread_t pong_thr = 0;

        pthread_attr_t ping_thr_attr = {0};
        pthread_attr_t pong_thr_attr = {0};

        struct sched_param ping_thr_param = {0};
        struct sched_param pong_thr_param = {0};

        int ping_policy = 0;
        int pong_policy = 0;

        /* Thread initializiation */
        int res = pthread_attr_init(&ping_thr_attr);
        res = pthread_attr_init(&pong_thr_attr);

        res = pthread_attr_setschedpolicy(&ping_thr_attr, SCHED_RR);
        res = pthread_attr_setschedpolicy(&pong_thr_attr, SCHED_RR);

        ping_thr_param.sched_priority = PING_SCHED_PRIO;
        pong_thr_param.sched_priority = PONG_SCHED_PRIO;

        res = pthread_attr_setschedparam(&ping_thr_attr, &ping_thr_param);
        res = pthread_attr_setschedparam(&pong_thr_attr, &pong_thr_param);

        res = pthread_attr_setinheritsched(&ping_thr_attr, PTHREAD_EXPLICIT_SCHED);
        res = pthread_attr_setinheritsched(&pong_thr_attr, PTHREAD_EXPLICIT_SCHED);

        /* Create the threads */
        if (pthread_create(&ping_thr, &ping_thr_attr, ping_guy, &ping_cycle)) {
                fprintf(stderr, "%s: failed to create ping_guy\n", PROG_NAME);
                exit(EXIT_FAILURE);
        }
        
        if (pthread_create(&pong_thr, &pong_thr_attr, pong_guy, &pong_cycle)) {
                fprintf(stderr, "%s: failed to create pong_guy\n", PROG_NAME);
                exit(EXIT_FAILURE);
        }

        while (1) {
                printf("main: ping miss/hit %llu/%llu pong miss/hit %llu/%llu\n",
                        ping_missed, ping_hit, pong_missed, pong_hit);

                usleep(1e6);
        }

        return EXIT_SUCCESS;
}
