#ifndef _LWIPOPTS_H
#define _LWIPOPTS_H

#include "lwipopts_examples_common.h"

#define NO_SYS 0
#define LWIP_SOCKET 1
#if !NO_SYS
#define TCPIP_THREAD_STACKSIZE 1024
#define DEFAULT_THREAD_STACKSIZE 1024
#define DEFAULT_RAW_RECVMBOX_SIZE 8
#define TCPIP_MBOX_SIZE 8
#define LWIP_TIMEVAL_PRIVATE 0

// not necessary, can be done either way
#define LWIP_TCPIP_CORE_LOCKING_INPUT 1
#endif

#define LWIP_HTTPD 1
#define LWIP_HTTPD_SSI 1
// don't include the tag comment - less work for the CPU, but may be harder to debug
#define LWIP_HTTPD_SSI_INCLUDE_TAG 0
#define LWIP_HTTPD_SUPPORT_POST 1
// use generated fsdata
#define HTTPD_FSDATA_FILE "my_fsdata.c"

#endif