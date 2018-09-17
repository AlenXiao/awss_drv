#include <sys/ioctl.h>

#if defined(__cplusplus)
extern "C"
{
#endif
//#define PROC_NET_WIRELESS	"/proc/net/wireless"
//#define PROC_NET_DEV		"/proc/net/dev"
#define PROC_NET_DEV		"/dev/awss"
#define PROC_NET_PATH       (PROC_NET_DEV)
//#define PROC_NET_PATH       (PROC_NET_WIRELESS)
//
enum {
    AWSS_IOC_SCAN_IDX = 0,
    AWSS_IOC_OPEN_MGMT_FILTER_IDX,
    AWSS_IOC_CLOSE_MGMT_FILTER_IDX,
    AWSS_IOC_OPEN_SNIFFER_IDX,
    AWSS_IOC_CLOSE_SNIFFER_IDX,
    AWSS_IOC_CONN_AP_IDX,
    AWSS_IOC_SWITCH_CHN_IDX,
    AWSS_IOC_CMD_MAX_IDX,
};

#define AWSS_IOC_MAGIC              'c'
#define AWSS_IOC_CMD(idx)           _IO(AWSS_IOC_MAGIC, idx)

#if defined(__cplusplus)
}
#endif
