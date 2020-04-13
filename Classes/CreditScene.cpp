#include "CreditScene.h"

USING_NS_CC;

Scene* CreditScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    auto layer = CreditScene::create();

    scene->addChild(layer);

    return scene;
}

bool CreditScene::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto margin = 30;

    auto background = Sprite::create("sprites/background.jpg");
    background->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    background->setOpacity(70);
    this->addChild(background, 0);

    auto backbutton = Sprite::create("sprites/buttons/BackButton.png");

    auto menu_item = MenuItemSprite::create(backbutton, backbutton, CC_CALLBACK_1(CreditScene::BackButton, this));
    auto menu = Menu::create(menu_item, NULL);
    menu->setPosition(Vec2(margin, visibleSize.height - margin ));
    this->addChild(menu, 1);

    char* credits="Sprites are taken from opengameart.org\n"
                "Sound effects are taken from freesound.org\n"
                "Maps are created by Tiled.\n"
                "The animations are created by TexturePacker.";
    auto label = Label::createWithTTF(credits, "fonts/Nabila.ttf", 40);
    label->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    label->setHorizontalAlignment(TextHAlignment::CENTER);
    label->setLineSpacing(25.0);
    this->addChild(label);

    
}

void CreditScene::BackButton(Ref* pSender) {
    Director::getInstance()->popScene();
}