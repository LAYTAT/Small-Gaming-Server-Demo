//
// Created by laylei on 2020/12/5.
//

#ifndef MY_SERVER_BASECOMPONENT_H
#define MY_SERVER_BASECOMPONENT_H

#define FUNCCLONE(TYPE) \
    TYPE *Clone() override\
    {\
        return new TYPE();\
    }\


class BaseComponent
{
public:
    BaseComponent() {}
    virtual ~BaseComponent() {}
    virtual BaseComponent* Clone() = 0;
};

#endif //MY_SERVER_BASECOMPONENT_H
