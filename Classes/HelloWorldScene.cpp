#include "HelloWorldScene.h"
#include "InstructionScene.h"
#include "LevelScene.h"
#include "CreditScene.h"
#include "audio/include/AudioEngine.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    auto layer = HelloWorld::create();

    scene->addChild(layer);

    return scene;
}

bool HelloWorld::init()
{
    if (!Layer::init())
    {
        return false;
    }

    auto backgroundMusic = AudioEngine::play2d("sounds/background_music.mp3", true);

    Size visibleSize = Director::getInstance()->getVisibleSize();

    auto background = Sprite::create("sprites/background.jpg");
    background->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    background->setOpacity(70);
    this->addChild(background, 0);

    float margin = 30.0;

    auto title = Sprite::create("sprites/title.png");
    title->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - 100));
    this->addChild(title);

    soundButton = Sprite::create("sprites/buttons/SoundOnButton.png");
    auto menu_item0 = MenuItemSprite::create(soundButton, soundButton, CC_CALLBACK_1(HelloWorld::MuteButton, this));
    auto* menu1 = Menu::create( menu_item0, nullptr);
    menu1->setPosition(Vec2(margin, visibleSize.height - margin));
    this->addChild(menu1, 1);

    auto menu_item_1 = MenuItemFont::create("Play", CC_CALLBACK_1(HelloWorld::Play, this));
    auto menu_item_2 = MenuItemFont::create(" Instructions", CC_CALLBACK_1(HelloWorld::Instructions, this));
    auto menu_item_3 = MenuItemFont::create(" Credits", CC_CALLBACK_1(HelloWorld::Credits, this));
    auto menu_item_4 = MenuItemFont::create(" Exit", CC_CALLBACK_1(HelloWorld::Exit, this));
    //auto menu_item_4 = MenuItemImage::create("CloseNormal.png", "CloseSelected.png", CC_CALLBACK_1(HelloWorld::ImageButton, this));

    menu_item_1->setFontNameObj("fonts/Nabila.ttf");    menu_item_1->setFontSizeObj(48);
    menu_item_2->setFontNameObj("fonts/Nabila.ttf");    menu_item_2->setFontSizeObj(48);
    menu_item_3->setFontNameObj("fonts/Nabila.ttf");    menu_item_3->setFontSizeObj(48);
    menu_item_4->setFontNameObj("fonts/Nabila.ttf");    menu_item_4->setFontSizeObj(48);

    auto* menu = Menu::create(menu_item_1, menu_item_2, menu_item_3, menu_item_4, NULL);
    menu->alignItemsVerticallyWithPadding(30.0);
    menu->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 - 50));
    this->addChild(menu);

    auto instructiosTag = UserDefault::getInstance()->getBoolForKey("instructionTag", true);
    //UserDefault::getInstance()->setBoolForKey("instructionTag", true);
    return true;
}

void HelloWorld::Play(Ref* pSender)
{
    Scene *newscene;
    if (UserDefault::getInstance()->getBoolForKey("instructionTag") == true) {
        newscene = InstructionScene::createScene();
        UserDefault::getInstance()->setBoolForKey("instructionTag", false);
    }
    else {
        newscene = LevelScene::createScene();
    }
    Director::getInstance()->pushScene(TransitionFlipX::create(2, newscene));
}

void HelloWorld::Instructions(Ref* pSender)
{
    UserDefault::getInstance()->setBoolForKey("instructionMenu", true);
    auto newscene = InstructionScene::createScene();
    Director::getInstance()->pushScene(TransitionFlipX::create(2, newscene));
}

void HelloWorld::Credits(Ref* pSender)
{
    auto newscene = CreditScene::createScene();
    Director::getInstance()->pushScene(TransitionFlipX::create(2, newscene));
}

void HelloWorld::Exit(Ref* pSSender)
{
    Director::getInstance()->end();
}

void HelloWorld::MuteButton(Ref* pSender)
{
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
