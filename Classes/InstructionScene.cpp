#include "InstructionScene.h"
#include "LevelScene.h"
USING_NS_CC;

Scene* InstructionScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    auto layer = InstructionScene::create();

    scene->addChild(layer);

    return scene;
}

bool InstructionScene::init()
{
    if (!Layer::init())
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    
    auto background = Sprite::create("sprites/background.jpg");
    background->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    background->setOpacity(70);
    this->addChild(background, 0);

    auto margin = 30;

    auto menu_item1 = MenuItemImage::create("sprites/buttons/BackButton.png", "sprites/buttons/BackButton.png", CC_CALLBACK_1(InstructionScene::BackButton, this));
    auto menu1 = Menu::create(menu_item1, NULL);
    menu1->setPosition(Vec2(margin, visibleSize.height - margin));
    this->addChild(menu1, 1);

    char* info1 = "There was a mad scientist in the town who spread a deadly virus.\n"
                    "You have very little time to find the way to get out of the town.";
                 

    auto label1 = Label::createWithTTF(info1, "fonts/Nabila.ttf", 40);
    label1->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - 100));
    label1->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    //label1->setAnchorPoint(Vec2(0.5, 1));
    this->addChild(label1,1);

    auto label2 = Label::createWithTTF("Movement", "fonts/Nabila.ttf", 40);
    label2->setPosition(Vec2(visibleSize.width / 2, 
                                label1->getPosition().y -label1->getBoundingBox().size.height/2 - 50));
    //label2->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - 100));
    label2->enableBold();
    //label2->setColor(Color3B::BLUE);
    //label2->setAnchorPoint(Vec2(0.5, 1));
    this->addChild(label2);

    auto label3 = Label::createWithTTF("Arrow Keys [Up, Down, Lef, Right]", "fonts/Nabila.ttf", 40);
    label3->setPosition(Vec2(visibleSize.width / 2,
                                label2->getPosition().y - label2->getBoundingBox().size.height / 2 - 25));
    label3->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    //label1->setAnchorPoint(Vec2(0.5, 1));
    this->addChild(label3, 1);

    auto healthBar = Sprite::create("sprites/healthBar.png");
    healthBar->setPosition(Vec2(visibleSize.width * 1.5 / 8 ,
        label3->getPosition().y - label3->getBoundingBox().size.height / 2 - 75));
    this->addChild(healthBar);

    auto muteButton = Sprite::create("sprites/buttons/SoundOnButton.png");
    muteButton->setPosition(Vec2(visibleSize.width * 3.5 / 8,
        label3->getPosition().y - label3->getBoundingBox().size.height / 2 - 75));
    this->addChild(muteButton);

    auto pauseButton = Sprite::create("sprites/buttons/pauseButton.png");
    pauseButton->setPosition(Vec2(visibleSize.width * 5.25 / 8,
        label3->getPosition().y - label3->getBoundingBox().size.height / 2 - 75));
    this->addChild(pauseButton);

    auto restartButton = Sprite::create("sprites/buttons/restartButton.png");
    restartButton->setPosition(Vec2(visibleSize.width * 7 / 8,
        label3->getPosition().y - label3->getBoundingBox().size.height / 2 - 75));
    this->addChild(restartButton);

    auto label4 = Label::createWithTTF("Health Bar", "fonts/Nabila.ttf", 40);
    label4->setPosition(Vec2(healthBar->getPosition().x,
        healthBar->getPosition().y - healthBar->getBoundingBox().size.height/2 - 30));
    this->addChild(label4);

    auto label5 = Label::createWithTTF("Mute Button", "fonts/Nabila.ttf", 40);
    label5->setPosition(Vec2(muteButton->getPosition().x,
        muteButton->getPosition().y - muteButton->getBoundingBox().size.height / 2 - 30));
    this->addChild(label5);

    auto label6 = Label::createWithTTF("Pause Button", "fonts/Nabila.ttf", 40);
    label6->setPosition(Vec2(pauseButton->getPosition().x,
        pauseButton->getPosition().y - pauseButton->getBoundingBox().size.height / 2 - 30));
    this->addChild(label6);

    auto label7 = Label::createWithTTF("Restart Button", "fonts/Nabila.ttf", 40);
    label7->setPosition(Vec2(restartButton->getPosition().x,
        restartButton->getPosition().y - restartButton->getBoundingBox().size.height / 2 - 30));
    this->addChild(label7);

    char* info2 = "Over time the density of fog of virus will increase \nand your health will decrease.";

    auto label8 = Label::createWithTTF(info2, "fonts/Nabila.ttf", 40);
    label8->setPosition(Vec2(visibleSize.width / 2,
        label7->getPosition().y - label7->getBoundingBox().size.height - 70));
    label8->setHorizontalAlignment(TextHAlignment::CENTER);
    this->addChild(label8);

    auto menu_item2 = MenuItemImage::create("sprites/buttons/BackButton.png", "sprites/buttons/BackButton.png", CC_CALLBACK_1(InstructionScene::NextButton, this));
    menu_item2->runAction(RotateTo::create(0, 0, 180));
    auto menu2 = Menu::create(menu_item2, NULL);
    menu2->setPosition(Vec2(visibleSize.width/2,
                                label8->getPosition().y - label8->getBoundingBox().size.height / 2 - 40));
    this->addChild(menu2, 1);

    if (UserDefault::getInstance()->getBoolForKey("instructionMenu") == true) {
        menu2->removeFromParent();
        UserDefault::getInstance()->setBoolForKey("instructionMenu", false);
    }

    return true;

}

void InstructionScene::BackButton(Ref* pSender) {
    Director::getInstance()->popScene();
}

void InstructionScene::NextButton(Ref* pSender)
{
    auto newscene = LevelScene::createScene();
    Director::getInstance()->pushScene(TransitionFlipX::create(2, newscene));
}
