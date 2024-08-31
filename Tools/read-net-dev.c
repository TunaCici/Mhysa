#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>

#include <fcntl.h>

#include <net/if.h>

#define MAX_NET_DEV 16u
#define NET_DEV_BUFF_SIZE 128u // bytes

/*
 * Format of '/proc/net/dev'
 *
 * See: https://www.kernel.org/doc/Documentation/filesystems/proc.txt
 */
typedef struct net_dev_t {
        char            name[IFNAMSIZ];

        uint64_t        rx_bytes;
        uint64_t        rx_packets;
        uint64_t        rx_errs;
        uint64_t        rx_drop;
        uint64_t        rx_fifo;
        uint64_t        rx_frame;
        uint64_t        rx_compressed;
        uint64_t        rx_multicast;

        uint64_t        tx_bytes;
        uint64_t        tx_packets;
        uint64_t        tx_errs;
        uint64_t        tx_drop;
        uint64_t        tx_fifo;
        uint64_t        tx_colls;
        uint64_t        tx_carrier;
        uint64_t        tx_compressed;
} net_dev_t;

/*
 *
 * Returns: Amount of devices read.
 */
int read_net_dev(net_dev_t *devs, unsigned int devs_count)
{
        int read_devs = 0;

        static int fd = 0;
        static mode_t mode = O_RDONLY;
        static char *filename = "/proc/net/dev";
        static char buff[MAX_NET_DEV * NET_DEV_BUFF_SIZE] = {0};

        if (!devs) {
                return read_devs;
        }

        if (!fd) {
                fd = open(filename, mode);

                if (!fd) {
                        fprintf(stderr, "%s: unable to open '%s'",
                                __FILE__, filename);
                        exit(EXIT_FAILURE);
                }
        }
        
        ssize_t offset = lseek(fd, 0, SEEK_SET);
        ssize_t bytes_read = read(fd, buff, MAX_NET_DEV * NET_DEV_BUFF_SIZE);

        if (offset == -1 || bytes_read == -1) {
                fprintf(stderr, "%s: unable to read '%s'",
                        __FILE__, filename);
                exit(EXIT_FAILURE);
        }

        unsigned int cursor = 0;

        /* Skip the first 2 lines */
        while (cursor < bytes_read && buff[cursor++] != '\n');
        while (cursor < bytes_read && buff[cursor++] != '\n');

        /* Parsing */
        for (int i = 0; i < MAX_NET_DEV; i++) {
                /* Skip over the leading spaces */
                while (cursor < bytes_read && buff[++cursor] == ' ');

                sscanf(buff + cursor,
                        "%[^:]: %lu%lu%lu%lu%lu%lu%lu%lu\
                                %lu%lu%lu%lu%lu%lu%lu%lu",
                        devs[i].name,
                        
                        &devs[i].rx_bytes,
                        &devs[i].rx_packets,
                        &devs[i].rx_errs,
                        &devs[i].rx_drop,
                        &devs[i].rx_fifo,
                        &devs[i].rx_frame,
                        &devs[i].rx_compressed,
                        &devs[i].rx_multicast,

                        &devs[i].tx_bytes,
                        &devs[i].tx_packets,
                        &devs[i].tx_errs,
                        &devs[i].tx_drop,
                        &devs[i].tx_fifo,
                        &devs[i].tx_colls,
                        &devs[i].tx_carrier,
                        &devs[i].tx_compressed);
                
                read_devs++;
                
                /* On to the next one */
                while (cursor < bytes_read && buff[cursor++] != '\n');

                if (bytes_read <= cursor) {
                        break;
                }
        }

        return read_devs;
}

int main(int argc, char **argv)
{
        net_dev_t devs[MAX_NET_DEV] = {0};

        while (1) {
                int count = read_net_dev(devs, MAX_NET_DEV);

                /* Clear the terminal */
                printf("\033[H\033[J");

                for (int i = 0; i < count; i++) {
                        printf("%s : RX %lu (bytes), TX (%lu) bytes\n",
                                devs[i].name,
                                devs[i].rx_bytes,
                                devs[i].tx_bytes);
                }

                sleep(1);
        }

}
