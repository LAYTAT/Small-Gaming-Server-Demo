#include <iostream>
#include "SocketServer.h"
#include "EventSystem.h"


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
    std::cout << "=== Gate server initialized ===" << std::endl;
    CreatSingletons();

    // main logic loop
    // TODO: add ticking mechanism
    SocketServer::Instance()->Dojob();

    DestroySingletons();
    return 0;
}

