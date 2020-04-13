#pragma once

#include "cocos2d.h"

class LevelScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(LevelScene);

    void Level1(Ref* pSender);
    void Level2(Ref* pSender);
    void Level3(Ref* pSender);
    void GoBack(Ref* pSender);
    void MuteButton(Ref* pSender);
    virtual void update(float delta) override;

    int LevelTag;
    bool sound = true, level1CompleteTag, level2CompleteTag, level3CompleteTag, levelAccessibility = false;
    cocos2d::Sprite *soundButton, *level_1, *level_2, *level_3;
    cocos2d::MenuItemImage *menu_item_level1, *menu_item_level2, *menu_item_level3;
    cocos2d::Menu *menu1, *menu2;
    
};