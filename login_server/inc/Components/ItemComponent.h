#ifndef _ITEMCOMPONENT_H_
#define _ITEMCOMPONENT_H_

#include "Config.h"
#include "BaseComponent.h"
#include "BagContainer.h"
#include "EquipContainer.h"
#include "CurrencyContainer.h"

class ItemContainerComponent : public BaseComponent
{
public:
    ItemContainerComponent(){}
    ~ItemContainerComponent(){}
    FUNCCLONE(ItemContainerComponent)
    
    BagContainer m_bagContainer;  // 背包容器
    EquipContainer m_equipContainer;  // 装备容器
    CurrencyContainer m_curContainer;  // 货币容器（写全名可太长了QAQ）
    
};

#endif
