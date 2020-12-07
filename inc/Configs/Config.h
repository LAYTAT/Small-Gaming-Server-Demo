//
// Created by laylei on 2020/12/2.
//

#ifndef MY_SERVER_CONFIG_H
#define MY_SERVER_CONFIG_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <string>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <cstring>
#include <vector>
#include <set>
#include <map>
#include <unordered_map>
#include <unordered_set>

#if defined(WIN32) || defined(_WIN32)
#include <winsock2.h>
#include <sys/types.h>
#include <sys/timeb.h>
#else
#include <signal.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/file.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/sockios.h>
#include <netinet/tcp.h>
#include <net/if.h>
#include <dirent.h>
#include <dlfcn.h>
#include <sys/select.h>
#include <assert.h>
#include <iostream>
#include <string>
#include <arpa/inet.h>
#include <sys/epoll.h>
#endif


#define DB_SERVER_PORT 8001
#define DB_SERVER_IP_ADDR "10.0.150.52"

#define LOGIN_SERVER_PORT 8002
#define LOGIN_SERVER_IP_ADDR "10.0.150.52"

#define GATE_SERVER_PORT 8003
#define GATE_SERVER_IP_ADDR "10.0.150.52"

#define DEFAULT_SERVER_PORT 8000  // occupied by lay
#define DEFAULT_SERVER_ADDR "10.0.150.52" // "192.168.11.25"
#define MAX_LINK_COUNT 1024
#define BUFFER_SIZE (1024*1024)    //0x200000
#define HEADBUFFER_SIZE (1024*1024)   //0x100000
#define MAX_BAG_SIZE 48  // bag size
#define MAX_ITEM_BAGOVERLY 99
#define MAX_ITEM_CURRENCYOVERLY 999999999

#define DATABASE_NAME "game"
#define DATABASE_IP  "10.0.150.52" // "192.168.11.25" //"127.0.0.1"
#define DATABASE_USERNAME "root"
#define DATABASE_PWD "123456"

typedef int32_t INT32;
typedef uint32_t UINT32;
typedef int64_t INT64;
typedef uint64_t UINT64;


enum COMPONENTS_ENUM
{
    COMPONENTS_POSITION = 1,
    COMPONENTS_ORIENT = 2,
    COMPONENTS_SPEED = 3,
    COMPONENTS_BAG = 4,
    COMPONENTS_PLAYERINFO = 5,
};

enum ENTITY_ENUM
{
    ENTITY_PLAYER = 1,
};
#endif //MY_SERVER_CONFIG_H
