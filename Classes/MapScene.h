#pragma once

#include "cocos2d.h"
#include <unordered_set>
#include "LevelScene.h"

class SplashLayer : public cocos2d::Layer {
public:
    virtual bool init();
    CREATE_FUNC(SplashLayer);
    cocos2d::Label* label;
    cocos2d::Label* label1;
    cocos2d::Sprite* splashPlayer;
    void generateSplash();
    //void nextLevel(Ref* pSender);
    bool splashLayer = true, splashTag = false, levelComplete = false, timeUp = false, isPaused = false;
    int MapLevelTag = 0;

private:
    cocos2d::Size visibleSize;
    cocos2d::Animation* dieAnimation;
    cocos2d::Animate* action;
};

class MapSceneHud : public cocos2d::Layer
{
public:
    //void numCollectedChanged(int numCollected);
    virtual bool init();
    CREATE_FUNC(MapSceneHud);

    cocos2d::Label* label, *label1;
    void BackButton(Ref* pSender);
    void MuteButton(Ref* pSender);
    void UpdateHud(float dt);
    void UpdateHealth(float dt);
    void UpdateFog(float dt);
    void pauseButton(Ref* pSender);
    void restartButton(Ref* pSender);
    float time, startingPercentage, currentPercentage, percentChangeRate;
    bool splashTag = false, isPaused = false, sound = true;

private:
    cocos2d::Size visibleSize;
    cocos2d::Menu* menu;
    cocos2d::ProgressTimer* healthBar;
    cocos2d::Sprite* fog, *backButton, *soundButton, *playButton, *RestartButton;
    int MapLevelTagHud, currentFogOpacity = 0;
};

class MapScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    virtual void update(float delta) override;
    void setViewPointCenter(cocos2d::Point position);
    void setPlayerPosition(cocos2d::Point position);
    void updateHighScore();
    void onKeyPressed();
    void onKeyReleased();

    cocos2d::Point tileCoordForPosition(cocos2d::Point position);
    cocos2d::Point LeftTileCoordForPosition(cocos2d::Point position);
    cocos2d::Point RightTileCoordForPosition(cocos2d::Point position);
    cocos2d::Point TopTileCoordForPosition(cocos2d::Point position);
    cocos2d::Point BottomTileCoordForPosition(cocos2d::Point position);

    int runningSoundId;
    float HighScore1 = 9999999.999999;
    float HighScore2 = 9999999.999999;

    CREATE_FUNC(MapScene);

private:
    cocos2d::Sprite* player, *boyn;
    cocos2d::TMXTiledMap* map;
    cocos2d::TMXLayer *background , *blockage, *fog;
    cocos2d::TMXObjectGroup* objects;
    cocos2d::Size visibleSize;
    cocos2d::Animation* runAnimation, *dieAnimation;
    cocos2d::RepeatForever* action;
    cocos2d::Animate* action2;

    bool finishTag = false, animate = false, splashLayer = true, died = false, levelComplete = false, isPaused = false;

    static MapSceneHud* _hud;
    static SplashLayer* _splash;
    static std::unordered_set<cocos2d::EventKeyboard::KeyCode> keys;
    static std::unordered_set<int> runningSound;

    LevelScene* _level;
    int MapLevelTag;
};