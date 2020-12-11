#include <iostream>
#include "SocketServer.h"
#include "EventSystem.h"
#include <signal.h>

// SIGPIPE ignore handler
void sigpipe_ignore_handler(int ignore){
    perror("sigpipe ignored ! 忽略了一次SIGPIPE");
}

// 顺序建立单例类
void CreatSingletons(){
    CREATE_SINGLETON(SocketServer)
    CREATE_SINGLETON(EventSystem)
}

// 逆序清除单例类
void DestroySingletons(){
    DESTORY_SINGLETON(EventSystem)
    DESTORY_SINGLETON(SocketServer)
}

int main() {

    //todo: 添加心跳包
    //防止因为向被对端关掉的socket写入而被断掉进程
    //sigaction(SIGPIPE, &(struct sigaction){sigpipe_ignore_handler}, nullptr);
    signal(SIGPIPE, sigpipe_ignore_handler);

    std::cout << "=== Gate server initialized ===" << std::endl;
    CreatSingletons();

    // main logic loop
    // TODO: add ticking mechanism
    SocketServer::Instance()->Dojob();

    DestroySingletons();
    return 0;
}

