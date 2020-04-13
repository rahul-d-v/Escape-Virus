#include "LevelScene.h"
#include "MapScene.h"
#include "audio/include/AudioEngine.h"
USING_NS_CC;

Scene* LevelScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    auto layer = LevelScene::create();

    scene->addChild(layer);

    return scene;
}

bool LevelScene::init()
{
    if (!Layer::init())
    {
        return false;
    }

    Size visibleSize = Director::getInstance()->getVisibleSize();

    auto background = Sprite::create("sprites/background.jpg");
    background->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    background->setOpacity(70);
    this->addChild(background, 0);
    int margin = 30;
    
    soundButton = Sprite::create("sprites/buttons/SoundOnButton.png");
    auto menu_item_1 = MenuItemImage::create("sprites/buttons/BackButton.png", "sprites/buttons/BackButton.png", CC_CALLBACK_1(LevelScene::GoBack, this));
    auto menu_item2 = MenuItemSprite::create(soundButton, soundButton, CC_CALLBACK_1(LevelScene::MuteButton, this));
    menu1 = Menu::create(menu_item_1, menu_item2, nullptr);
    menu1->setPosition(Vec2(margin*2, visibleSize.height - margin));
    menu1->alignItemsHorizontallyWithPadding(20.0);
    this->addChild(menu1, 1);

    menu_item_level1 = MenuItemImage::create("sprites/level_1.png", "sprites/level_1.png",  CC_CALLBACK_1(LevelScene::Level1, this));
    menu_item_level2 = MenuItemImage::create("sprites/level_2.png", "sprites/level_2.png",  CC_CALLBACK_1(LevelScene::Level2, this));
    menu_item_level3 = MenuItemImage::create("sprites/level_3.png", "sprites/level_3.png",  CC_CALLBACK_1(LevelScene::Level3, this));

    menu2 = Menu::create(menu_item_level1, menu_item_level2, menu_item_level3, NULL);
    //menu2->alignItemsHorizontallyWithPadding(30);
    menu2->alignItemsHorizontallyWithPadding(50.0);
    menu2->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    this->addChild(menu2, 1);

    

    menu_item_level2->setEnabled(false);    menu_item_level2->setOpacity(127);//127=half opacity
    menu_item_level3->setEnabled(false);    menu_item_level3->setOpacity(127);

    this->scheduleUpdate();

    UserDefault* def = UserDefault::getInstance();
    /*auto temp = def->getIntegerForKey("xxx", 102);
    CCLOG("temp %d", temp);
    def->setIntegerForKey("xxx", 201);*/

    /*level1CompleteTag = def->set*/
    level1CompleteTag = UserDefault::getInstance()->getBoolForKey("level1Complete", false);
    level2CompleteTag = UserDefault::getInstance()->getBoolForKey("level2Complete", false);
    level3CompleteTag = UserDefault::getInstance()->getBoolForKey("level3Complete", false);

    /*UserDefault::getInstance()->setBoolForKey("level1Complete", false);
    UserDefault::getInstance()->setBoolForKey("level2Complete", false);
    UserDefault::getInstance()->setBoolForKey("level2Complete", false);*/

    return true;
}

void LevelScene::update(float delta) {
    Node::update(delta);
    
    level1CompleteTag = UserDefault::getInstance()->getBoolForKey("level1Complete", false);
    level2CompleteTag = UserDefault::getInstance()->getBoolForKey("level2Complete", false);
    level3CompleteTag = UserDefault::getInstance()->getBoolForKey("level3Complete", false);

    if (level1CompleteTag == true) {
        menu_item_level2->setEnabled(true);
        menu_item_level2->setOpacity(255);
    }
    if (level2CompleteTag == true) {
        menu_item_level3->setEnabled(true);
        menu_item_level3->setOpacity(255);
    }
}

void LevelScene::Level1(Ref* pSender)
{
    CCLOG("Level1");
    UserDefault::getInstance()->setIntegerForKey("LevelTag", 1);
    auto newscene = MapScene::createScene();
    Director::getInstance()->pushScene(TransitionFlipX::create(2, newscene));
}

void LevelScene::Level2(Ref* pSender)
{
    CCLOG("Level2");
    UserDefault::getInstance()->setIntegerForKey("LevelTag", 2);
    auto newscene = MapScene::createScene();
    Director::getInstance()->pushScene(TransitionFlipX::create(2, newscene));
}

void LevelScene::Level3(Ref* pSender)
{
    CCLOG("Level3");
    UserDefault::getInstance()->setIntegerForKey("LevelTag", 3);
    auto newscene = MapScene::createScene();
    Director::getInstance()->pushScene(TransitionFlipX::create(2, newscene));
}

void LevelScene::GoBack(Ref* pSender)
{
    CCLOG("Home");
    Director::getInstance()->popScene();
    
}

void LevelScene::MuteButton(Ref* pSender) {
    if (sound) {
        CCLOG("Mute");
        soundButton->setTexture("sprites/buttons/SoundOffButton.png");
        AudioEngine::pauseAll();
        sound = false;
    }
    else {
        CCLOG("Sound on");
        soundButton->setTexture("sprites/buttons/SoundOnButton.png");
        AudioEngine::resume(0);
        sound = true;
    }
}
