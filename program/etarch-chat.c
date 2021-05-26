// etarch-chat.c

#include <linux/if_packet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <net/if.h>
#include <netinet/ether.h>
#include <netinet/in.h>

#define MAXCHAR        1024
#define ETHER_TYPE    0x0880

/*C program to convert hexadecimal Byte to integer.*/
//function : getNum
//this function will return number corresponding
//0,1,2..,9,A,B,C,D,E,F

int getNum(char ch) {
    int num;
    if (ch >= '0' && ch <= '9') {
        num = ch - 0x30;
    } else {
        switch (ch) {
            case 'A':
            case 'a':
                num = 10;
                break;
            case 'B':
            case 'b':
                num = 11;
                break;
            case 'C':
            case 'c':
                num = 12;
                break;
            case 'D':
            case 'd':
                num = 13;
                break;
            case 'E':
            case 'e':
                num = 14;
                break;
            case 'F':
            case 'f':
                num = 15;
                break;
            default:
                num = 0;
        }
    }
    return num;
}


//function : hex2int
//this function will return integer value against
//hexValue - which is in string format

unsigned int hex2int(char hex[]) {
    unsigned int x;
    x = (getNum(hex[0])) * 16 + (getNum(hex[1]));
    return x;
}


int main(int argc, char *argv[]) {
    ssize_t numbytes;
    uint8_t buf[MAXCHAR];
    (void) argc;
    char iface[MAXCHAR], et[MAXCHAR], crc[MAXCHAR];

    // Open configuration file and get variables
    FILE *fp;
    char filename[] = "/etc/etarch/etarch.conf";

    fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Could not open file %s", filename);
        return 1;
    }

    // Construct and execute command workspace start
    char wsname[MAXCHAR];
    strcat(wsname, "etarch-ws start ");
    strcat(wsname, argv[1]);
    system(wsname);

    while ((fscanf(fp, "%s %s", iface, et)) == 2) {

        // INIT construct command conversion
        char ws[MAXCHAR];
        strcpy(ws, argv[1]);
        char exec1[] = "/usr/local/bin/etarch-cv ";
        strcat(exec1, ws);

        // INIT convert et
        FILE *fp1;

        if ((fp1 = popen(exec1, "r")) == NULL) {
            printf("Error opening pipe!\n");
            return -1;
        }

        while (fgets(crc, MAXCHAR, fp1) != NULL) {
            // Do whatever you want here...
        }

        if (pclose(fp1)) {
            printf("Command not found or exited with error status\n");
            return -1;
        }
        //END convert et

        // CONTINUE construct command conversion
        char m0[20], m1[3], m2[3], m3[3], m4[3], m5[3];
        sprintf(m0, "%.2s", &crc[0]);
        sprintf(m1, "%.2s", &crc[2]);
        sprintf(m2, "%.2s", &crc[4]);
        sprintf(m3, "%.2s", &crc[6]);
        sprintf(m4, "%.2s", &crc[8]);
        sprintf(m5, "%.2s", &crc[10]);

        unsigned int m0a; //can be stored in unsigned char
        unsigned int m1a; //can be stored in unsigned char
        unsigned int m2a; //can be stored in unsigned char
        unsigned int m3a; //can be stored in unsigned char
        unsigned int m4a; //can be stored in unsigned char
        unsigned int m5a; //can be stored in unsigned char

        m0a = hex2int(m0);
        m1a = hex2int(m1);
        m2a = hex2int(m2);
        m3a = hex2int(m3);
        m4a = hex2int(m4);
        m5a = hex2int(m5);

        int sockfd;
        int sockfd2;
        int sockopt;
        struct ifreq if_idx;
        struct ifreq if_mac;
        int tx_len;
        char sendbuf[MAXCHAR];

        //RAW Socket send component
        /* Header structures */
        struct ether_header *eh = (struct ether_header *) sendbuf;
        //	struct iphdr *iph = (struct iphdr *) (sendbuf + sizeof(struct ether_header));
        //	struct udphdr *udph = (struct udphdr *) (buf + sizeof(struct iphdr) + sizeof(struct ether_header));
        struct sockaddr_ll socket_address;


        /* Open RAW socket to send on */
        if ((sockfd = socket(AF_PACKET, SOCK_RAW, IPPROTO_RAW)) == -1) {
            perror("socket");
        }

        /* Open PF_PACKET socket, listening for EtherType ETHER_TYPE */
        if ((sockfd2 = socket(PF_PACKET, SOCK_RAW, htons(ETHER_TYPE))) == -1) {
            perror("listener: socket");
            return -1;
        }

        /* Allow the socket to be reused - incase connection is closed prematurely */
        if (setsockopt(sockfd2, SOL_SOCKET, SO_REUSEADDR, &sockopt, sizeof sockopt) == -1) {
            perror("setsockopt");
            exit(EXIT_FAILURE);
        }
        /* Bind to device */
        if (setsockopt(sockfd2, SOL_SOCKET, SO_BINDTODEVICE, argv[1], IFNAMSIZ - 1) == -1) {
            perror("SO_BINDTODEVICE");
            exit(EXIT_FAILURE);
        }

        /* Get the index of the interface to send on */
        memset(&if_idx, 0, sizeof(struct ifreq));
        strncpy(if_idx.ifr_name, argv[1], IFNAMSIZ - 1);
        if (ioctl(sockfd, SIOCGIFINDEX, &if_idx) < 0)
            perror("SIOCGIFINDEX");
        /* Get the MAC address of the interface to send on */
        memset(&if_mac, 0, sizeof(struct ifreq));
        strncpy(if_mac.ifr_name, argv[1], IFNAMSIZ - 1);
        if (ioctl(sockfd, SIOCGIFHWADDR, &if_mac) < 0)
            perror("SIOCGIFHWADDR");

        while (1) {

            /* Construct the Ethernet header */
            memset(sendbuf, 0, MAXCHAR);
            tx_len = 0;
            /* Ethernet header */
            eh->ether_shost[0] = ((uint8_t *) &if_mac.ifr_hwaddr.sa_data)[0];
            eh->ether_shost[1] = ((uint8_t *) &if_mac.ifr_hwaddr.sa_data)[1];
            eh->ether_shost[2] = ((uint8_t *) &if_mac.ifr_hwaddr.sa_data)[2];
            eh->ether_shost[3] = ((uint8_t *) &if_mac.ifr_hwaddr.sa_data)[3];
            eh->ether_shost[4] = ((uint8_t *) &if_mac.ifr_hwaddr.sa_data)[4];
            eh->ether_shost[5] = ((uint8_t *) &if_mac.ifr_hwaddr.sa_data)[5];
            eh->ether_dhost[0] = m0a;
            eh->ether_dhost[1] = m1a;
            eh->ether_dhost[2] = m2a;
            eh->ether_dhost[3] = m3a;
            eh->ether_dhost[4] = m4a;
            eh->ether_dhost[5] = m5a;
            /* Ethertype field */
            eh->ether_type = htons(ETHER_TYPE);
            tx_len += sizeof(struct ether_header);

            /* Index of the network device */
            socket_address.sll_ifindex = if_idx.ifr_ifindex;
            /* Address length*/
            socket_address.sll_halen = ETH_ALEN;
            /* Destination MAC */
            socket_address.sll_addr[0] = m0a;
            socket_address.sll_addr[1] = m1a;
            socket_address.sll_addr[2] = m2a;
            socket_address.sll_addr[3] = m3a;
            socket_address.sll_addr[4] = m4a;
            socket_address.sll_addr[5] = m5a;

            char message[MAXCHAR];
            printf("Send a message: ");
            printf("\n");
            fgets(message, 100, stdin);

            unsigned int i;

            for (i = 0; i < strlen(message); i++) {
                sendbuf[tx_len++] = message[i];
            }

            /* Send packet */
            if (sendto(sockfd, sendbuf, tx_len, 0, (struct sockaddr *) &socket_address, sizeof(struct sockaddr_ll)) < 0)
                printf("Send failed\n");

            /* receive message*/
            numbytes = recvfrom(sockfd2, buf, MAXCHAR, 0, NULL, NULL);

            /* Print packet */
            printf("\nReceived Message: ");
            printf("\n");
            for (i = 14; i < numbytes; i++) { printf("%c", buf[i]); }
            printf("\n");

        }
    }
}