#pragma once

#include "cocos2d.h"

class HelloWorld : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(HelloWorld);

    void Play(Ref* pSender);
    void Instructions(Ref* pSender);
    void Credits(Ref* pSender);
    void Exit(Ref* pSender);
    void MuteButton(Ref* pSender);
    cocos2d::Sprite* soundButton;
    bool sound = true;

    //private:
    //    TMXTiledMap* _tileMap;
    //    TMXLayer* _background;

};