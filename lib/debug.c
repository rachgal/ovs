//==================================================================================================================
//
//==================================================================================================================
#include <config.h>

//==================================================================================================================
//
//==================================================================================================================
#include "netdev-linux.h"
#include "netdev-linux-private.h"
#include "openvswitch/vlog.h"
#include "debug.h"

//==================================================================================================================
//
//==================================================================================================================
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <inttypes.h>
#include <math.h>
#include <linux/filter.h>
#include <linux/gen_stats.h>
#include <linux/if_ether.h>
#include <linux/if_tun.h>
#include <linux/types.h>
#include <linux/ethtool.h>
#include <linux/mii.h>
#include <linux/rtnetlink.h>

/* Required for VLOG* logging */
VLOG_DEFINE_THIS_MODULE(debug);

//==================================================================================================================
//
//==================================================================================================================
char* netlink_to_string(int protocol)
{
  char *prototol_name;

  switch(protocol)
  {
  case NETLINK_ROUTE:
    prototol_name = "NETLINK_ROUTE";
    break;
  
  case NETLINK_UNUSED:
    prototol_name = "NETLINK_UNUSED";
    break;

  case NETLINK_USERSOCK:
    prototol_name = "NETLINK_USERSOCK";
    break;
    
  case NETLINK_FIREWALL:
    prototol_name = "NETLINK_FIREWALL";
    break;

  case NETLINK_SOCK_DIAG:
    prototol_name = "NETLINK_SOCK_DIAG";
    break;

  case NETLINK_NFLOG:
    prototol_name = "NETLINK_NFLOG";
    break;
    
  case NETLINK_XFRM:
    prototol_name = "NETLINK_XFRM";
    break;
    
  case NETLINK_SELINUX:
    prototol_name = "NETLINK_SELINUX";
    break;

  case NETLINK_ISCSI:
    prototol_name = "NETLINK_ISCSI";
    break;
    
  case NETLINK_AUDIT:
    prototol_name = "NETLINK_AUDIT";
    break;

  case NETLINK_FIB_LOOKUP:
    prototol_name = "NETLINK_FIB_LOOKUP";
    break;
  
  case NETLINK_CONNECTOR:
    prototol_name = "NETLINK_CONNECTOR";
    break;

  case NETLINK_NETFILTER:
    prototol_name = "NETLINK_NETFILTER";
    break;
    
  case NETLINK_IP6_FW:
    prototol_name = "NETLINK_IP6_FW";
    break;

  case NETLINK_DNRTMSG:
    prototol_name = "NETLINK_DNRTMSG";
    break;

  case NETLINK_KOBJECT_UEVENT:
    prototol_name = "NETLINK_KOBJECT_UEVENT";
    break;
    
  case NETLINK_GENERIC:
    prototol_name = "NETLINK_GENERIC";
    break;
    
  case NETLINK_SCSITRANSPORT:
    prototol_name = "NETLINK_SCSITRANSPORT";
    break;

  case NETLINK_ECRYPTFS:
    prototol_name = "NETLINK_ECRYPTFS";
    break;
    
  case NETLINK_RDMA:
    prototol_name = "NETLINK_RDMA";
    break;

  case NETLINK_CRYPTO:
    prototol_name = "NETLINK_CRYPTO";
    break;
    
  default:
    prototol_name = "NETLINK_???";
    break;
  }

  return prototol_name;
}

//==================================================================================================================
//
//==================================================================================================================
char* rtm_to_string(int code)
{
  char *rtm_name;

  switch(code)
  {
  case RTM_NEWLINK:
    rtm_name = "RTM_NEWLINK";
    break;

  case RTM_DELLINK:
    rtm_name = "RTM_DELLINK";
    break;

  case RTM_GETLINK:
    rtm_name = "RTM_GETLINK";
    break;

  case RTM_SETLINK:
    rtm_name = "RTM_SETLINK";
    break;

  case RTM_NEWADDR:
    rtm_name = "RTM_NEWADDR";
    break;

  case RTM_DELADDR:
    rtm_name = "RTM_DELADDR";
    break;

  case RTM_GETADDR:
    rtm_name = "RTM_GETADDR";
    break;

  case RTM_NEWROUTE:
    rtm_name = "RTM_NEWROUTE";
    break;

  case RTM_DELROUTE:
    rtm_name = "RTM_DELROUTE";
    break;

  case RTM_GETROUTE:
    rtm_name = "RTM_GETROUTE";
    break;

  case RTM_NEWNEIGH:
    rtm_name = "RTM_NEWNEIGH";
    break;

  case RTM_DELNEIGH:
    rtm_name = "RTM_DELNEIGH";
    break;

  case RTM_GETNEIGH:
    rtm_name = "RTM_GETNEIG";
    break;

  case RTM_NEWRULE:
    rtm_name = "RTM_NEWRULE";
    break;

  case RTM_DELRULE:
    rtm_name = "RTM_DELRULE";
    break;

  case RTM_GETRULE:
    rtm_name = "RTM_GETRULE";
    break;

  case RTM_NEWQDISC:
    rtm_name = "RTM_NEWQDISC";
    break;

  case RTM_DELQDISC:
    rtm_name = "RTM_DELQDISC";
    break;

  case RTM_GETQDISC:
    rtm_name = "RTM_GETQDISC";
    break;

  case RTM_NEWTCLASS:
    rtm_name = "RTM_NEWTCLASS";
    break;

  case RTM_DELTCLASS:
    rtm_name = "RTM_DELTCLASS";
    break;

  case RTM_GETTCLASS:
    rtm_name = "RTM_GETTCLASS";
    break;

  case RTM_NEWTFILTER:
    rtm_name = "RTM_NEWTFILTER";
    break;

  case RTM_DELTFILTER:
    rtm_name = "RTM_DELTFILTER";
    break;

  case RTM_GETTFILTER:
    rtm_name = "RTM_GETTFILTER";
    break;

  case RTM_NEWACTION:
    rtm_name = "RTM_NEWACTION";
    break;

  case RTM_DELACTION:
    rtm_name = "RTM_DELACTION";
    break;

  case RTM_GETACTION:
    rtm_name = "RTM_GETACTION";
    break;

  case RTM_NEWPREFIX:
    rtm_name = "RTM_NEWPREFIX";
    break;

  case RTM_GETMULTICAST:
    rtm_name = "RTM_GETMULTICAST";
    break;

  case RTM_GETANYCAST:
    rtm_name = "RTM_GETANYCAST";
    break;

  case RTM_NEWNEIGHTBL:
    rtm_name = "RTM_NEWNEIGHTBL";
    break;

  case RTM_GETNEIGHTBL:
    rtm_name = "RTM_GETNEIGHTBL";
    break;

  case RTM_SETNEIGHTBL:
    rtm_name = "RTM_SETNEIGHTBL";
    break;

  case RTM_NEWNDUSEROPT:
    rtm_name = "RTM_NEWNDUSEROPT";
    break;

  case RTM_NEWADDRLABEL:
    rtm_name = "RTM_NEWADDRLABEL";
    break;

  case RTM_DELADDRLABEL:
    rtm_name = "RTM_DELADDRLABEL";
    break;

  case RTM_GETADDRLABEL:
    rtm_name = "RTM_GETADDRLABEL";
    break;

  case RTM_GETDCB:
    rtm_name = "RTM_GETDCB";
    break;

  case RTM_SETDCB:
    rtm_name = "RTM_SETDCB";
    break;

  case RTM_NEWNETCONF:
    rtm_name = "RTM_NEWNETCONF";
    break;

//  case RTM_DELNETCONF:
 //   rtm_name = "RTM_DELNETCONF";
  //  break;

  case RTM_GETNETCONF:
    rtm_name = "RTM_GETNETCONF";
    break;

  case RTM_NEWMDB:
    rtm_name = "RTM_NEWMDB";
    break;

  case RTM_DELMDB:
    rtm_name = "RTM_DELMDB";
    break;

  case RTM_GETMDB:
    rtm_name = "RTM_GETMDB";
    break;

  case RTM_NEWNSID:
    rtm_name = "RTM_NEWNSID";
    break;

  case RTM_DELNSID:
    rtm_name = "RTM_DELNSID";
    break;

  case RTM_GETNSID:
    rtm_name = "RTM_GETNSID";
    break;

  case RTM_NEWSTATS:
    rtm_name = "RTM_NEWSTATS";
    break;

  case RTM_GETSTATS:
    rtm_name = "RTM_GETSTATS";
    break;

/*  case RTM_NEWCACHEREPORT:
    rtm_name = "RTM_NEWCACHEREPORT";
    break; */

/*  case RTM_NEWCHAIN:
    rtm_name = "RTM_NEWCHAIN";
    break; */

/*  case RTM_DELCHAIN:
    rtm_name = "RTM_DELCHAIN";
    break; */

/*  case RTM_GETCHAIN:
    rtm_name = "RTM_GETCHAIN";
    break; */

/*  case RTM_NEWNEXTHOP:
    rtm_name = "RTM_NEWNEXTHOP";
    break; */

/*  case RTM_DELNEXTHOP:
    rtm_name = "RTM_DELNEXTHOP";
    break; */

/*  case RTM_GETNEXTHOP:
    rtm_name = "RTM_GETNEXTHOP";
    break; */

  default:
    rtm_name = "RTM_???";
    break;
  }

  return rtm_name;
}

//==================================================================================================================
//
//==================================================================================================================
char* tca_to_string(uint16_t type)
{
  char *tca_name;

  switch(type)
  {
  case TCA_UNSPEC:
    tca_name = "TCA_UNSPEC";
    break;

  case TCA_KIND:
    tca_name = "TCA_KIND";
    break;
  
  case TCA_OPTIONS:
    tca_name = "TCA_OPTIONS";
    break;
  
  case TCA_STATS:
    tca_name = "TCA_STATS";
    break;
  
  case TCA_XSTATS:
    tca_name = "TCA_XSTATS";
    break;
  
  case TCA_RATE:
    tca_name = "TCA_RATE";
    break;
  
  case TCA_FCNT:
    tca_name = "TCA_FCNT";
    break;
  
  case TCA_STATS2:
    tca_name = "TCA_STATS2";
    break;
  
  case TCA_STAB:
    tca_name = "TCA_STAB";
    break;
  
  case TCA_PAD:
    tca_name = "TCA_PAD";
    break;
  
/*  case TCA_DUMP_INVISIBLE:
    tca_name = "TCA_DUMP_INVISIBLE";
    break;*/
  
/*  case TCA_CHAIN:
    tca_name = "TCA_CHAIN";
    break;*/
 
/* case TCA_INGRESS_BLOCK:
    tca_name = "TCA_INGRESS_BLOCK";
    break;*/
  
/*  case TCA_EGRESS_BLOCK:
    tca_name = "TCA_EGRESS_BLOCK";
    break;*/
  
  default:
    tca_name = "TCA_???";
    break;
  }

  return tca_name;
}

//==================================================================================================================
//
//==================================================================================================================
char* ovs_vport_type_to_string(int ovs_vport_type)
{
  char *ovs_vport_type_name;

  switch(ovs_vport_type)
  {
  case OVS_VPORT_TYPE_UNSPEC:
    ovs_vport_type_name = "OVS_VPORT_TYPE_UNSPEC";
    break;

  case OVS_VPORT_TYPE_NETDEV:
    ovs_vport_type_name = "OVS_VPORT_TYPE_NETDEV";
    break;

  case OVS_VPORT_TYPE_INTERNAL:
    ovs_vport_type_name = "OVS_VPORT_TYPE_INTERNAL";
    break;

  case OVS_VPORT_TYPE_GRE:
    ovs_vport_type_name = "OVS_VPORT_TYPE_GRE";
    break;
  
  case OVS_VPORT_TYPE_VXLAN:
    ovs_vport_type_name = "OVS_VPORT_TYPE_VXLAN";
    break;

  case OVS_VPORT_TYPE_GENEVE:
    ovs_vport_type_name = "OVS_VPORT_TYPE_GENEVE";
    break;

  default:
    ovs_vport_type_name = "OVS_VPORT_TYPE_???";
    break;
  }

  return ovs_vport_type_name;
}

//==================================================================================================================
//
//==================================================================================================================
ssize_t dbg_sock_send(int fd, const void *buf, size_t size, int flags, const char* caller)
{
  ssize_t result;

  VLOG_INFO("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
  VLOG_INFO("%s: fd: [%u] caller: [%s]...", __FUNCTION__, fd, caller);
  LogBuffer((char*)caller, buf, size);
  VLOG_INFO("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");

  result = send(fd, buf, size, flags);

  return result;
}

//==================================================================================================================
//
//==================================================================================================================
ssize_t dbg_sock_sendmsg(int fd, const struct msghdr *msg, int flags, const char* caller)
{
  ssize_t result;

  VLOG_INFO("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
  VLOG_INFO("%s: fd: [%u] msg->msg_iovlen: [%lu] caller: [%s]...", __FUNCTION__, fd, msg->msg_iovlen, caller);
  dbg_sock_dump_msghdr(msg);
  VLOG_INFO("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");

  result = sendmsg(fd, msg, flags);

  return result;
}

//==================================================================================================================
//
//==================================================================================================================
ssize_t dbg_sock_recv(int fd, void *buf, size_t size, int flags, const char* caller)
{
  ssize_t result;

  VLOG_INFO("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
  VLOG_INFO("%s: fd: [%u] caller: [%s]...", __FUNCTION__, fd, caller);
  LogBuffer((char*)caller, buf, size);
  VLOG_INFO("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");

  result = recv(fd, buf, size, flags);

  return result;
}

//==================================================================================================================
//
//==================================================================================================================
ssize_t dbg_sock_recvmsg(int fd, struct msghdr *message, int flags, const char* caller)
{
  ssize_t result;

  VLOG_INFO("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
  VLOG_INFO("%s: fd: [%u] caller: [%s]...", __FUNCTION__, fd, caller);
  //LogBuffer((char*)caller, buf, size);
  VLOG_INFO("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");

  result = recvmsg(fd, message, flags);

  return result;
}

//==================================================================================================================
//
//==================================================================================================================
ssize_t dbg_sock_recvfrom(int fd, void *__restrict buf, size_t size, int flags, __SOCKADDR_ARG addr,
                         socklen_t *__restrict addr_len, const char* caller)
{
  ssize_t result;

  VLOG_INFO("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
  VLOG_INFO("%s: fd: [%u] caller: [%s]...", __FUNCTION__, fd, caller);
  LogBuffer((char*)caller, buf, size);
  VLOG_INFO("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");

  result = recvfrom(fd, buf, size, flags, addr, addr_len);
  
  return result;
}

//==================================================================================================================
//
//==================================================================================================================
int dbg_sock_recvmmsg (int fd, struct mmsghdr *vmessages, unsigned int vlen, int flags, struct timespec *tmo, const char* caller)
{
  int result;

  VLOG_INFO("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
  VLOG_INFO("%s: fd: [%u] caller: [%s]...", __FUNCTION__, fd, caller);
  //LogBuffer((char*)caller, buf, size);
  VLOG_INFO("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");

  result = recvmmsg (fd, vmessages, vlen, flags, tmo);
  
  return result;
}

//==================================================================================================================
//
//==================================================================================================================
void dbg_sock_dump_msghdr(const struct msghdr *msg)
{
  int k;
  struct iovec *iovec;

  //VLOG_INFO("%s: fd: [%u] caller: [%s]...", __FUNCTION__, fd, caller);
  LogBuffer((char*)"msg_name", msg->msg_name, msg->msg_namelen);

  for(k = 0; k < msg->msg_iovlen; k++)
  {
    iovec = &msg->msg_iov[k];
    
    dbg_sock_dump_iovec(iovec);
  }
}

//==================================================================================================================
//
//==================================================================================================================
void dbg_sock_dump_iovec(struct iovec *iovec)
{
  LogBuffer((char*)"iovec", iovec->iov_base, iovec->iov_len);
}

#define FORMAT_INFO_INCLUDE_HEX_PREFIX  0x00000002
#define FORMAT_INFO_INCLUDE_DECODING    0x00000004

//==================================================================================================================
//
//==================================================================================================================
void Bin2Hex(char *dest, int sizeof_dest, const uint8_t *src, int len, uint32_t PrintFlags)
{
  int n;
  int k;
  char* pDestination;
  int bytes_converted;
  int remaining_space;
  int iNumberOfPadSpaces;
  const uint8_t* pSource;
  bool bDisplayPrintable;
  bool bDisplayHexIndicator;

  /* Display printable data */
  bDisplayPrintable = (PrintFlags & FORMAT_INFO_INCLUDE_DECODING);

  /* Do not prepend every ascii-hex byte with 0x */
  bDisplayHexIndicator = (PrintFlags & FORMAT_INFO_INCLUDE_HEX_PREFIX);

  /* Save pointer to first byte of destination buffer */
  pDestination = dest;

  /* Keep track of number of bytes converted */
  bytes_converted = 0;

  /* Keep track of space remaining in destination buffer */
  remaining_space = sizeof_dest;

  /* Save pointer to source data, we'll use it to show printable data */
  pSource = src;

  iNumberOfPadSpaces = 8;

  /* Convert binary character to ASCII hex */
  while (len--)
  {
    if (bDisplayHexIndicator)
    {
      /* Make sure there is space for a full ascii-hex character (e.g. 0x30), line termination (CR/LF) and NULL termination. */
      if (remaining_space < 7)
      {
        break;
      }

      /* Append 0x to every byte */
      *(dest++) = '0';
      *(dest++) = 'x';
    }
    else
    {
      /* Make sure there is space for a full ascii-hex character (e.g. 30), line termination (CR/LF) and NULL termination. */
      if (remaining_space < 5)
      {
        break;
      }
    }

    /* Process upper nibble */
    n = (*(src) >> 4) & 0x0f;

    if (n <= 9)
    {
      n += '0';
    }
    else
    {
      n += 'a' - 10;
    }

    *(dest++) = (char)n;

    /* Process lower nibble*/
    n = *(src++) & 0x0f;

    if (n <= 9)
    {
      n += '0';
    }
    else
    {
      n += 'a' - 10;
    }

    *(dest++) = (char)n;

    /* Update number of binary bytes converted to ascii-hex */
    bytes_converted++;

    /* More than one byte remaining to be converted? */
    if (len >= 1)
    {
      /* Print 8 ascii-hex per line */
      if (bytes_converted % 8 == 0)
      {
        iNumberOfPadSpaces = 8;

        /* Display printable characters? */
        if (bDisplayPrintable)
        {
          /* Add spaces between ascii-hex data and their printable values */
          *(dest++) = ' ';
          *(dest++) = ' ';
          *(dest++) = ' ';
          *(dest++) = ' ';

          while (pSource < src)
          {
            /* Printable character? */
            if ((*pSource > 0x1f) && (*pSource < 0x7f))
            {
              *(dest++) = *pSource;
            }
            else
            {
              /* Non printable character */
              *(dest++) = '.';
            }

            /* Point to next data byte to print */
            pSource++;
          }
        }
      }
      else
      {
        /* Add space between bytes */
        *(dest++) = ' ';

        iNumberOfPadSpaces--;
      }
    }

    /* Get number remaining in destination buffer */
    remaining_space = sizeof_dest - (int)(dest - pDestination);
  }

  /* Display printable characters? */
  if (bDisplayPrintable)
  {
    /* Calculate number of spaces (padding) */
    iNumberOfPadSpaces = (iNumberOfPadSpaces - 1) * 3;

    /* Perform the padding */
    for (k = 0; k < iNumberOfPadSpaces; k++)
    {
      *(dest++) = ' ';
    }

    /* Add spaces between ascii-hex data and their printable values */
    *(dest++) = ' ';
    *(dest++) = ' ';
    *(dest++) = ' ';
    *(dest++) = ' ';

    while (pSource < src)
    {
      /* Printable character? */
      if ((*pSource > 0x1f) && (*pSource < 0x7f))
      {
        *(dest++) = *pSource;
      }
      else
      {
        /* Non printable character */
        *(dest++) = '.';
      }

      /* Point to next data byte to print */
      pSource++;
    }
  }
}

//==================================================================================================================
//
//==================================================================================================================
void LogBuffer(char *Message, const uint8_t* pData, int DataSize)
{
  int iOffset;
  int LenRemaining;
  char buffer[512];
  uint32_t PrintFlags;
  const uint8_t* pBuffer;
  int NumberOfByestToPrint;
  const uint8_t *pSourceBuffer;
  const int MaxBytesPerIteration = 8;

  /* Display message and number of bytes in buffer */
  VLOG_INFO("%s: [%d] bytes", Message, DataSize);

  /* Init print flags */
  PrintFlags = 0x00000000;

  /* Insure pData is not NULL */
  if (pData)
  {
    /* Offset into source data to print/log */
    iOffset = 0;
    pSourceBuffer = pData;
    pBuffer = pSourceBuffer;

    /* Number of bytes remaining to print/log */
    LenRemaining = DataSize;

    PrintFlags |= FORMAT_INFO_INCLUDE_DECODING;

    while (LenRemaining > 0)
    {
      /* Convert at most 64 binary bytes at one time */
      NumberOfByestToPrint = (LenRemaining > MaxBytesPerIteration ? MaxBytesPerIteration : LenRemaining);

      /* Used for clarity to print the buffer address */
      pBuffer = &pSourceBuffer[iOffset];

      /* Clear destination buffer */
      memset(buffer, 0x00, sizeof(buffer));

      /* Convert to hex */
      Bin2Hex(buffer, sizeof(buffer), (uint8_t*)&pSourceBuffer[iOffset], NumberOfByestToPrint, PrintFlags);

      VLOG_INFO("0x%p: [%s]", pBuffer, buffer);

      /* Print address and data (right now we print 8 ascii-hex bytes at one time) */
      //DEBUG((DebugMask, "0x%p: [%s]\n", pBuffer, buffer));

      /* Update offset into source buffer */
      iOffset += MaxBytesPerIteration;

      /* Update number of bytes remaining to print */
      LenRemaining -= NumberOfByestToPrint;
    }
  }
}




