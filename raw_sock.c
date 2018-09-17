#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>       // close()
#include <string.h>       // strcpy, memset(), and memcpy()
#include <netdb.h>         // struct addrinfo
#include <sys/types.h>     // needed for socket(), uint8_t, uint16_t, uint32_t
#include <sys/socket.h>     // needed for socket()
#include <netinet/in.h>     // IPPROTO_ICMP, INET_ADDRSTRLEN
#include <netinet/ip.h>     // struct ip and IP_MAXPACKET (which is 65535)
#include <netinet/ip_icmp.h> // struct icmp, ICMP_ECHO
#include <arpa/inet.h>     // inet_pton() and inet_ntop()
#include <sys/ioctl.h>     // macro ioctl is defined
#include <bits/ioctls.h>     // defines values for argument "request" of ioctl.
#include <net/if.h>       // struct ifreq
#include <linux/if_ether.h>   // ETH_P_IP = 0x0800, ETH_P_IPV6 = 0x86DD
#include <linux/if_packet.h> // struct sockaddr_ll (see man 7 packet)
#include <net/ethernet.h>
#include <errno.h>         // errno, perror()
#define ETH_P_DEAN 0x8874 //自定义的以太网协议type

int main (int argc, char **argv)
{
    int i, datalen,frame_length, sd, bytes;
    char *interface="eth1";;
    uint8_t data[IP_MAXPACKET];
    uint8_t src_mac[6];
    uint8_t dst_mac[6];;
    uint8_t ether_frame[IP_MAXPACKET];
    struct sockaddr_ll device;
    // Submit request for a socket descriptor to look up interface.
    if ((sd = socket (AF_PACKET, SOCK_RAW, htons (ETH_P_ALL))) < 0) {//第一次创建socket是为了获取本地网卡信息
        perror ("socket() failed to get socket descriptor for using ioctl() ");
        exit (EXIT_FAILURE);
    }
    // Use ioctl() to look up interface name and get its MAC address.
    struct ifreq ifr;
    memset (&ifr, 0, sizeof (ifr));
    snprintf (ifr.ifr_name, sizeof (ifr.ifr_name), "%s", interface);

    if (ioctl (sd, SIOCGIFHWADDR, &ifr) < 0) {
        perror ("ioctl() failed to get source MAC address ");
        return (EXIT_FAILURE);
    }
    close (sd);
    memcpy (src_mac, ifr.ifr_hwaddr.sa_data, 6); // Copy source MAC address.
    memset (&device, 0, sizeof (device));
    if ((device.sll_ifindex = if_nametoindex(interface)) == 0) {
        perror ("if_nametoindex() failed to obtain interface index ");
        exit (EXIT_FAILURE);
    }

    // Set destination MAC address: you need to fill these out
    //设置目的网卡地址  

    dst_mac[0] = 0x10; dst_mac[1] = 0x78; dst_mac[2] = 0xd2; 

    dst_mac[3] = 0xc6; dst_mac[4] = 0x2f; dst_mac[5] = 0x89;


    device.sll_family = AF_PACKET;
    memcpy (device.sll_addr, src_mac, 6);
    device.sll_halen = htons (6);
    // 发送的data，但是抓包时看到最小数据长度为46，以太网协议规定以太网帧数据域最小为46字节，不足的自动补零处理
    datalen = 12;
    data[0] = 'h';data[1] = 'e';data[2] = 'l';data[3] = 'l';data[4] = 'o';data[5] = ' ';
    data[6] = 'w';data[7] = 'o';data[8] = 'r';data[9] = 'l';data[10] = 'd';data[11] = '!';

    memcpy (ether_frame, dst_mac, 6);
    memcpy (ether_frame + 6, src_mac, 6);
    ether_frame[12] = ETH_P_DEAN / 256;
    ether_frame[13] = ETH_P_DEAN % 256;
    memcpy (ether_frame + 14 , data, datalen); 

    frame_length = 6 + 6 + 2 + datalen;

    if ((sd = socket (PF_PACKET, SOCK_RAW, htons (ETH_P_ALL))) < 0) {//创建正真发送的socket
        perror ("socket() failed ");
        exit (EXIT_FAILURE);
    }
    if ((bytes = sendto (sd, ether_frame, frame_length, 0, (struct sockaddr *) &device, sizeof (device))) <= 0) {
        perror ("sendto() failed");
        exit (EXIT_FAILURE);
     }  
    close (sd);
    return (EXIT_SUCCESS);
}
