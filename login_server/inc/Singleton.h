//
// Created by laylei on 2020/12/2.
//

#ifndef _SINGLETON_H_
#define _SINGLETON_H_

#define DECLARE_SINGLETON(classtype)        \
private:                                    \
    static classtype* m_singleton;          \
public:                                     \
    static void CreateInstance()            \
    {                                       \
        if (NULL == m_singleton)            \
        {                                   \
            m_singleton = new classtype();  \
        }                                   \
    }                                       \
    static classtype* Instance()            \
    {                                       \
        return m_singleton;                 \
    }                                       \
    static void DestoryInstance(){          \
        if(m_singleton != NULL){            \
             delete m_singleton;            \
        }                                   \
    }                                       \

#define INSTANCE_SINGLETON(type)	type* type::m_singleton = NULL;

#define CREATE_SINGLETON(classtype) classtype::CreateInstance();if(classtype::Instance()->Init() == false){ std::cout <<"create singleton failed!" << std::endl; };
#define DESTORY_SINGLETON(classtype) classtype::Instance()->Uinit();classtype::DestoryInstance();

#endif
