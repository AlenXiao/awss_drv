#include <linux/if.h>
#include <linux/sockios.h>
/* Standard headers */
#include <sys/types.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <math.h>
#include <errno.h>
#include <fcntl.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>      /* gethostbyname, getnetbyname */
#include <net/ethernet.h>   /* struct ether_addr */
#include <sys/time.h>       /* struct timeval */
#include <unistd.h>
  
/* This is our header selection. Try to hide the mess and the misery :-(
 * Don't look, you would go blind ;-)
 * Note : compatibility with *old* distributions has been removed,
 * you will need Glibc 2.2 and older to compile (which means 
 * Mandrake 8.0, Debian 2.3, RH 7.1 or older).
 */
  
/* Set of headers proposed by Dr. Michael Rietz <rietz@mail.amps.de>, 27.3.2 */
#include <net/if_arp.h>     /* For ARPHRD_ETHER */
#include <sys/socket.h>     /* For AF_INET & struct sockaddr */
#include <netinet/in.h>         /* For struct sockaddr_in */
#include <netinet/if_ether.h>

#include "awss_drv.h"

#if defined(__cplusplus)
extern "C"
{
#endif

static inline char *iw_get_ifname(
        char *name,	/* Where to store the name */
	    int nsize,	/* Size of name buffer */
	    char *buf)	/* Current position in buffer */
{
    char *	end;
    /* Skip leading spaces */
    while (isspace(*buf))
        buf++;

    printf("dev:%s\r\n", buf);
#ifndef IW_RESTRIC_ENUM
    /* Get name up to the last ':'. Aliases may contain ':' in them,
     * but the last one should be the separator
     */
    end = strrchr(buf, ':');
#else
    /* Get name up to ": "
     * Note : we compare to ": " to make sure to process aliased interfaces
     * properly. Doesn't work on /proc/net/dev, because it doesn't guarantee
     * a ' ' after the ':'
     */
    end = strstr(buf, ": ");
#endif

    /* Not found ??? To big ??? */
    if ((end == NULL) || (((end - buf) + 1) > nsize))
        return(NULL);

    /* Copy */
    memcpy(name, buf, (end - buf));
    name[end - buf] = '\0';

    /* Return value currently unused, just make sure it's non-NULL */
    return (end);
}

static int print_iface_version_info(
        int	skfd,
        char *ifname,
        char *args[],  /* Command line args */
        int	count)     /* Args count */
{
#if 0
  struct iwreq		wrq;
  char			buffer[sizeof(iwrange) * 2];	/* Large enough */
  struct iw_range *	range;

  /* Avoid "Unused parameter" warning */
  args = args; count = count;

  /* If no wireless name : no wireless extensions.
   * This enable us to treat the SIOCGIWRANGE failure below properly. */
  if(iw_get_ext(skfd, ifname, SIOCGIWNAME, &wrq) < 0)
    return(-1);

  /* Cleanup */
  memset(buffer, 0, sizeof(buffer));

  wrq.u.data.pointer = (caddr_t) buffer;
  wrq.u.data.length = sizeof(buffer);
  wrq.u.data.flags = 0;
  if(iw_get_ext(skfd, ifname, SIOCGIWRANGE, &wrq) < 0)
    {
      /* Interface support WE (see above), but not IWRANGE */
      fprintf(stderr, "%-8.16s  Driver has no Wireless Extension version information.\n\n", ifname);
      return(0);
    }

  /* Copy stuff at the right place, ignore extra */
  range = (struct iw_range *) buffer;

  /* For new versions, we can check the version directly, for old versions
   * we use magic. 300 bytes is a also magic number, don't touch... */
  if(wrq.u.data.length >= 300)
    {
      /* Version is always at the same offset, so it's ok */
      printf("%-8.16s  Recommend Wireless Extension v%d or later,\n",
	     ifname, range->we_version_source);
      printf("          Currently compiled with Wireless Extension v%d.\n\n",
	     range->we_version_compiled);
    }
  else
    {
      fprintf(stderr, "%-8.16s  Wireless Extension version too old.\n\n",
		      ifname);
    }


#endif
  return(0);
}

int main(void)
{
    int fd = -1;
    char buff[1024] = {0};
    fd = open(PROC_NET_PATH, O_RDWR);
    printf("open fd:%d\r\n", fd);
    if (fd != -1) {
#if 0
        fgets(buff, sizeof(buff), fh);
        fgets(buff, sizeof(buff), fh);
        while(fgets(buff, sizeof(buff), fh)) {
	        char name[IFNAMSIZ + 1];
            char *s;

            /* Skip empty or almost empty lines. It seems that in some
	         * cases fgets return a line with only a newline.
             */
	        if ((buff[0] == '\0') || (buff[1] == '\0'))
	            continue;

            /* Extract interface name */
	        s = iw_get_ifname(name, sizeof(name), buff);
            if (s == NULL)
                printf("Cann't parse %s\n", PROC_NET_PATH);
            else
                printf("Devine:%s\n", name);
        }
#endif
        //int ret = ioctl(fd , SIOCGIFNAME, &fd);
        int ret = ioctl(fd , AWSS_IOC_CMD(AWSS_IOC_SCAN_IDX), &fd);
        printf("ret:%d\r\n", ret);
    }
    close(fd);
    return 0;
}

#if defined(__cplusplus)
}
#endif
