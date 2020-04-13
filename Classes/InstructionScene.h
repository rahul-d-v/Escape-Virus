#pragma once

#include "cocos2d.h"

class InstructionScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(InstructionScene);
    void BackButton(Ref* pSender);
    void NextButton(Ref* pSender);

};