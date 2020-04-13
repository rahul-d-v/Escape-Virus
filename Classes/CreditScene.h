#pragma once

#include "cocos2d.h"

class CreditScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(CreditScene);
    void BackButton(Ref* pSender);

};