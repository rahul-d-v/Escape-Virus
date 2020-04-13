#include "MapScene.h"
#include "HelloWorldScene.h"
#include <string>
//#include <vector>
#include "audio/include/AudioEngine.h"

#define MAX(a,b) (((a) > (b)) ? (a) : (b))
#define MIN(a,b) (((a) > (b)) ? (b) : (a))
USING_NS_CC;

MapSceneHud* MapScene::_hud = NULL;
SplashLayer* MapScene::_splash = NULL;

Scene* MapScene::createScene()
{
    auto scene = Scene::create();
    auto layer = MapScene::create();

    scene->addChild(layer,1);

    auto hud = MapSceneHud::create();
    _hud = hud;
    scene->addChild(hud,2);

    auto splash = SplashLayer::create();
    _splash = splash;
    scene->addChild(splash,3);

    return scene;
}

bool SplashLayer::init() {
    if (!Layer::init()) {
        return false;
    }

    /*auto nextButton = Sprite::create("sprites/buttons/BackButton.png");
    nextButton->runAction(RotateTo::create(0, 0, 180));
    auto menu_item = MenuItemSprite::create(nextButton, nextButton, CC_CALLBACK_1(SplashLayer::nextLevel, this));
    auto menu = Menu::create(menu_item, NULL);
    menu->setPosition(Vec2(visibleSize.width / 2, 100));
    this->addChild(menu, 2);*/
    /*MapLevelTag = UserDefault::getInstance()->getIntegerForKey("LevelTag");
    CCLOG("Splash MapLevelTag %d", MapLevelTag);*/
}

bool MapSceneHud::init()
{
    if (!Layer::init())
    {
        return false;
    }

    visibleSize = Director::getInstance()->getVisibleSize();

    int margin = 10;
    backButton = Sprite::create("sprites/buttons/backButton.png");
    soundButton = Sprite::create("sprites/buttons/SoundOnButton.png");
    playButton = Sprite::create("sprites/buttons/pauseButton.png");
    RestartButton = Sprite::create("sprites/buttons/RestartButton.png");

    auto menu_item1 = MenuItemSprite::create(backButton, backButton, CC_CALLBACK_1(MapSceneHud::BackButton, this));
    auto menu_item2 = MenuItemSprite::create(soundButton, soundButton, CC_CALLBACK_1(MapSceneHud::MuteButton, this));
    auto menu_item3 = MenuItemSprite::create(playButton, playButton, CC_CALLBACK_1(MapSceneHud::pauseButton, this));
    auto menu_item4 = MenuItemSprite::create(RestartButton, RestartButton, CC_CALLBACK_1(MapSceneHud::restartButton, this));
    //auto menu_item1 = MenuItemImage::create("BackButton.png","BackButton.png", CC_CALLBACK_1(MapSceneHud::BackButton, this));
    //auto menu_item2 = MenuItemImage::create("SoundOnButton.png", "SoundOnButton.png", CC_CALLBACK_1(MapSceneHud::MuteButton, this));
    menu = Menu::create(menu_item1, menu_item2, menu_item3, menu_item4, NULL);
    menu->setPosition(Vec2(margin*12, visibleSize.height - margin*3));
    menu->alignItemsHorizontallyWithPadding(20.0);
    this->addChild(menu, 1);


    startingPercentage = 100.0;
    currentPercentage = startingPercentage;

    MapLevelTagHud = UserDefault::getInstance()->getIntegerForKey("LevelTag");
    if (MapLevelTagHud == 1) {
        time = 40.0;//40
    }
    else if (MapLevelTagHud == 2) {
        time = 90.0;//90
    }
    else if (MapLevelTagHud == 3) {
        time = 150;
    }
    percentChangeRate = startingPercentage / time;

    fog = Sprite::create("sprites/fog.jpg");
    //fog->setOpacity(70.253);
    fog->setOpacity(currentFogOpacity);
    fog->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    this->addChild(fog, 0);

    auto healthBox = Sprite::create("sprites/healthbox.png");
    healthBox->setAnchorPoint(Vec2(1, 1));
    healthBox->setPosition(Vec2(visibleSize.width -margin , visibleSize.height -margin ));
    this->addChild(healthBox, 1);

    healthBar = ProgressTimer::create(Sprite::create("sprites/healthbar.png"));
    if (healthBar != NULL)
    {
        healthBar->setType(ProgressTimer::Type::BAR);
        //healthBar->setType(kCCProgressTimerTypeBar);
        healthBar->setMidpoint(Point(0, 0));
        healthBar->setBarChangeRate(Point(1, 0));
        healthBar->setPercentage(startingPercentage);
        healthBar->setAnchorPoint(Vec2(1, 1));
        healthBar->setPosition(healthBox->getPosition());
        this->addChild(healthBar,2);
    }

    label = Label::createWithTTF("Health :", "fonts/Nabila.ttf", 30);
    label->setAnchorPoint(Vec2(1, 1));
    label->setPosition(Vec2(healthBox->getPosition().x - healthBox->getBoundingBox().size.width ,
        healthBox->getPosition().y));
    this->addChild(label, 1);

    this->scheduleOnce(CC_SCHEDULE_SELECTOR(MapSceneHud::UpdateHud), 2.0);

}

bool MapScene::init()
{
    if (!Layer::init())
    {
        return false;
    }

    //_splash->removeFromParent();
    
    /*auto color = LayerColor::create(Color4B(64, 64, 64, 255));
    this->addChild(color, -1);*/


    visibleSize = Director::getInstance()->getVisibleSize();

    MapLevelTag = UserDefault::getInstance()->getIntegerForKey("LevelTag");
    UserDefault::getInstance()->setIntegerForKey("HighScore1", 999999.999999);
    UserDefault::getInstance()->setIntegerForKey("HighScore2", 999999.999999);

    map = new TMXTiledMap();

    if (MapLevelTag == 1)
        map->initWithTMXFile("maps/map1.tmx");
    else if (MapLevelTag == 2)
        map->initWithTMXFile("maps/map2.tmx");
    else if (MapLevelTag == 3)
        map->initWithTMXFile("maps/map3.tmx");

    background = map->getLayer("Path");
    this->addChild(map, -1);

    blockage = map->getLayer("Collider");
    blockage->setVisible(false);
    

    Size CC_UNUSED s = map->getContentSize(); // 1600 1792

    Size ms = map->getMapSize(); // 0 1078525952
    Size ts = map->getTileSize(); // 0 1077936128

    objects = map->getObjectGroup("Objects");
    CCASSERT(NULL != objects, "'Object-Player' object group not found");

    auto playerSpwanPoint = objects->getObject("SpawnPoint");
    CCASSERT(!playerSpwanPoint.empty(), "SpwanPoint object not found");
    float x = playerSpwanPoint["x"].asFloat();
    float y = playerSpwanPoint["y"].asFloat();
    
    //SpriteFrameCache* frameCache = SpriteFrameCache::addSpriteFramesWithFile("boy.plist");

    SpriteFrameCache* frameCache = SpriteFrameCache::getInstance();
    frameCache->addSpriteFramesWithFile("sprites/character/boyRun.plist");
    SpriteBatchNode* spritesheet = SpriteBatchNode::create("sprites/character/boyRun.png");
    this->addChild(spritesheet);

    Vector<SpriteFrame*> boyFrames;
    for (int i = 0; i <= 8; i++) {
        if (i == 1 || i == 7)
            continue;
        std::string num = StringUtils::format("Run%d.png", i);
        SpriteFrame* frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(num);
        boyFrames.pushBack(frame);
        //boyFrames.pushBack(frameCache->getSpriteFrameByName("Run (" + num + ").png"));
    }

    runAnimation = Animation::createWithSpriteFrames(boyFrames, 0.1f);
    //runAnimation->retain();
    player = Sprite::createWithSpriteFrameName("Ideal0.png");
    player->setPosition(x + ts.width / 2, y + ts.height / 2);
    //player->setName("boy");
    //player->setScale(0.5);
    

    action = RepeatForever::create(Animate::create(runAnimation));
    //action2 = Animate::create(runAnimation);
    //RefPtr<RepeatForever> action = RepeatForever::create(Animate::create(runAnimation));
    //player->runAction(action);
    action->retain();
    spritesheet->addChild(player);
    
    //player->runAction(Animate::create(runAnimation));
    
    //player = Sprite::create("static.png");
    //player->setPosition(x + ts.width / 2, y + ts.height / 2);
    //this->addChild(player);
    //auto *action = RepeatForever::create(Animate::create(runAnimation));
    //player->runAction(action);
    //spritesheet->addChild(player);
    //player->setContentSize(Size(40.0f, 40.0f));
    
    Size p = player->getBoundingBox().size;
    CCLOG("player bounding box widht:%f height%f", p.width, p.height);
    
    
    /*UserDefault* def = UserDefault::getInstance();
    auto highscore = def->getIntegerForKey("HighScore", 0);

    def->setIntegerForKey("HighScore", highscore);*/


    auto keyboardListener = EventListenerKeyboard::create();

    runningSound.insert(1);

    keyboardListener->onKeyPressed = [&](EventKeyboard::KeyCode keyCode, Event* event) {

        if (!runningSound.empty()) {
            auto running = AudioEngine::play2d("sounds/running.mp3", true, 0.5f);
            //runningSoundId = running;
            UserDefault::getInstance()->setIntegerForKey("runningSounId", running);
            CCLOG("running sound id %d", UserDefault::getInstance()->getIntegerForKey("runningSounId") );
            runningSound.erase(1);
        }
        
        onKeyPressed();

        switch (keyCode)
        {
        case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
            keys.insert(keyCode);
            if (!died) {
                player->stopAllActions();
                player->runAction(action->clone()); 
            }
            break;

        case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
            keys.insert(keyCode);
            if (!died) {
                player->stopAllActions();
                player->runAction(action->clone());
            }
            break;

        case EventKeyboard::KeyCode::KEY_UP_ARROW:
            keys.insert(keyCode);
            if (!died) {
                player->stopAllActions();
                player->runAction(action->clone());
            }
            break;

        case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
            keys.insert(keyCode);
            if (!died) {
                player->stopAllActions();
                player->runAction(action->clone());
            }
            break;

        case EventKeyboard::KeyCode::KEY_ENTER:
            keys.insert(keyCode);
            break;

        case EventKeyboard::KeyCode::KEY_ESCAPE:
            keys.insert(keyCode);
            break;
        }
    };
    keyboardListener->onKeyReleased = [&](EventKeyboard::KeyCode keyCode, Event* event) {
        
        switch (keyCode)
        {
        case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
            keys.erase(keyCode);
            break;

        case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
            keys.erase(keyCode);
            break;

        case EventKeyboard::KeyCode::KEY_UP_ARROW:
            keys.erase(keyCode);
            break;

        case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
            keys.erase(keyCode);
            break;

        case EventKeyboard::KeyCode::KEY_ENTER:
            keys.erase(keyCode);
            break;

        case EventKeyboard::KeyCode::KEY_ESCAPE:
            keys.erase(keyCode);
            break;
        }
       
        onKeyReleased();
    };

    this->_eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, player);
    this->scheduleUpdate();
    return true;
}

void MapScene::update(float delta) {
    Node::update(delta);
    Point position = player->getPosition();

    /*if (keys.empty() && _hud->sound) {
        CCLOG("resuming");
        AudioEngine::resume(1);
    }
    
    if (keys.empty() || !_hud->sound) {
        CCLOG("pausing");
        AudioEngine::pause(1);
    }*/

    //if (_hud->runningSoundId == 0)
    //    _hud->runningSoundId = runningSoundId;

    if (_splash->MapLevelTag == 0) {
        _splash->MapLevelTag = MapLevelTag;
        CCLOG("spalsh mapleveltag %d", _splash->MapLevelTag);
    }
    if (_splash->splashLayer) {
        _splash->splashLayer = false;
        _splash->setVisible(false);
        return;
    }
    if (_splash->splashTag) {
        _splash->setVisible(true);
        player->pause();
        return;
    }

    if (_hud->isPaused) {
        isPaused = true;
        player->pause();
    }
    else {
        if (isPaused) {
            isPaused = false;
            player->resume();
        }
        if (keys.size())
        {
            if (keys.find(EventKeyboard::KeyCode::KEY_LEFT_ARROW) != keys.end())
            {
                position.x -= 2;
                this->setPlayerPosition(position);
                player->runAction(RotateTo::create(0, 0, 180));
            }

            else if (keys.find(EventKeyboard::KeyCode::KEY_RIGHT_ARROW) != keys.end()) {
                position.x += 2;
                this->setPlayerPosition(position);
                player->runAction(RotateTo::create(0, 0, 0));
            }

            else if (keys.find(EventKeyboard::KeyCode::KEY_UP_ARROW) != keys.end()) {
                position.y += 2;
                this->setPlayerPosition(position);
            }

            else if (keys.find(EventKeyboard::KeyCode::KEY_DOWN_ARROW) != keys.end()) {
                position.y -= 2;
                this->setPlayerPosition(position);
            }
        }
    }

    if(_hud->currentPercentage < 0.0) {
        CCLOG("Died");
        keys.clear();
        _splash->splashTag = true; //for showing splash layer
        _splash->timeUp = true;
        _hud->splashTag = true; //for stoping effects of hud layer
        _splash->generateSplash();
    }

   /* _hud->currentPercentage -= _hud->percentChangeRate;
    CCLOG("currentPErcentage %f", _hud->currentPercentage);*/

    this->setViewPointCenter(position);
}

Point MapScene::tileCoordForPosition(Point position) {
    int x = position.x / map->getTileSize().width;
    int y = ((map->getMapSize().height * map->getTileSize().height) - position.y) / map->getTileSize().height;
    return Point(x, y);
}

Point MapScene::LeftTileCoordForPosition(Point position) {
    int x = (position.x - player->getBoundingBox().size.width / 2) / map->getTileSize().width;
    int y = ((map->getMapSize().height * map->getTileSize().height) - (position.y)) / map->getTileSize().height;
    return Point(x, y);
}

Point MapScene::RightTileCoordForPosition(cocos2d::Point position) {
    int x = (position.x + player->getBoundingBox().size.width / 2) / map->getTileSize().width;
    int y = ((map->getMapSize().height * map->getTileSize().height) - (position.y)) / map->getTileSize().height;
    return Point(x, y);
}

Point MapScene::TopTileCoordForPosition(cocos2d::Point position) {
    int x = (position.x) / map->getTileSize().width;
    int y = ((map->getMapSize().height * map->getTileSize().height) - (position.y + player->getBoundingBox().size.height / 2)) / map->getTileSize().height;
    return Point(x, y);
}

Point MapScene::BottomTileCoordForPosition(cocos2d::Point position) {
    int x = (position.x) / map->getTileSize().width;
    int y = ((map->getMapSize().height * map->getTileSize().height) - (position.y - player->getBoundingBox().size.height / 2)) / map->getTileSize().height;
    return Point(x, y);
}

// Because cocos2d-x requres createScene to be static, we need to make other non-pointer members static
std::unordered_set<cocos2d::EventKeyboard::KeyCode> MapScene::keys;
std::unordered_set<int> MapScene::runningSound;

void MapScene::setViewPointCenter(Point position) {
    auto winSize = Director::getInstance()->getWinSize();
    //CCLOG("winSize: width:%f height:%f",winSize.width,winSize.height);

    float x = MAX(position.x, winSize.width / 2);
    float y = MAX(position.y, winSize.height / 2);
    //CCLOG("setViewBefore: x:%f y:%f", x, y);
    x = MIN(x, (map->getMapSize().width * map->getTileSize().width) - winSize.width / 2);
    y = MIN(y, (map->getMapSize().height * map->getTileSize().height) - winSize.height / 2);
    //CCLOG("setViewAfter: x:%f y:%f", x, y);
    auto actualPosition = Point(x, y);

    auto centerOfView = Point(winSize.width / 2, winSize.height / 2);
    auto viewPoint = centerOfView - actualPosition;
    //CCLOG("viewPoint: X:%f Y:%f",viewPoint.x,viewPoint.y);

    this->setPosition(viewPoint);
}


void MapScene::setPlayerPosition(Point position)
{    
    //Point tileCoord = this->tileCoordForPosition(position);
    Point tileCoordLeft = this->LeftTileCoordForPosition(position);
    Point tileCoordRight = this->RightTileCoordForPosition(position);
    Point tileCoordTop = this->TopTileCoordForPosition(position);
    Point tileCoordBottom = this->BottomTileCoordForPosition(position);

    //int tileGid = blockage->getTileGIDAt(tileCoord);
    int tileGidLeftW = blockage->getTileGIDAt(tileCoordLeft);
    int tileGidRightW = blockage->getTileGIDAt(tileCoordRight);
    int tileGidTopW = blockage->getTileGIDAt(tileCoordTop);
    int tileGidBottomW = blockage->getTileGIDAt(tileCoordBottom);
    
    /*if (tileGid) {
        auto properties = map->getPropertiesForGID(tileGid).asValueMap();
        if (!properties.empty()) {
            auto collision1 = properties["Collidable"].asBool();
            if (collision1) {
                return;
            }
            auto collision2 = properties["FinishPointCollidable"].asBool();
            if (collision2) {
                this->updateHighScore();
                return;
            }
        }
    }*/

    if (tileGidLeftW) {
        auto properties = map->getPropertiesForGID(tileGidLeftW).asValueMap();
        if (!properties.empty()) {
            auto collision1 = properties["Collidable"].asBool();
            if (collision1) {
                return;
            }
            auto collision2 = properties["FinishPointCollidable"].asBool();
            if (collision2) {
                keys.clear();
                _splash->splashTag = true;
                _splash->levelComplete = true;
                _hud->splashTag = true;
                _splash->generateSplash();
                //this->updateHighScore();
                return;
            }
        }
    }

    if (tileGidRightW) {
        auto properties = map->getPropertiesForGID(tileGidRightW).asValueMap();
        if (!properties.empty()) {
            auto collision1 = properties["Collidable"].asBool();
            if (collision1) {
                return;
            }
            auto collision2 = properties["FinishPointCollidable"].asBool();
            if (collision2) {
                keys.clear();
                _splash->splashTag = true;
                _splash->levelComplete = true;
                _hud->splashTag = true;
                _splash->generateSplash();
                //this->updateHighScore();
                return;
            }
        }
    }

    if (tileGidTopW) {
        auto properties = map->getPropertiesForGID(tileGidTopW).asValueMap();
        if (!properties.empty()) {
            auto collision1 = properties["Collidable"].asBool();
            if (collision1) {
                return;
            }
            auto collision2 = properties["FinishPointCollidable"].asBool();
            if (collision2) {
                keys.clear();
                _splash->splashTag = true;
                _splash->levelComplete = true;
                _hud->splashTag = true;
                _splash->generateSplash();
                //this->updateHighScore();
                return;
            }
        }
    }

    if (tileGidBottomW) {
        auto properties = map->getPropertiesForGID(tileGidBottomW).asValueMap();
        if (!properties.empty()) {
            auto collision1 = properties["Collidable"].asBool();
            if (collision1) {
                return;
            }
            auto collision2 = properties["FinishPointCollidable"].asBool();
            if (collision2) {
                keys.clear();
                _splash->splashTag = true;
                _splash->levelComplete = true;
                _hud->splashTag = true;
                _splash->generateSplash();
                //this->updateHighScore();
                return;
            }
        }
    }
    player->setPosition(position);
}

void MapScene::onKeyPressed()
{
    if (keys.empty() && _hud->sound) 
        AudioEngine::resume(UserDefault::getInstance()->getIntegerForKey("runningSounId"));
}

void MapScene::onKeyReleased()
{
    if (keys.empty())
        player->stopAllActions();
    if (keys.empty() && _hud->sound)
        AudioEngine::pause(UserDefault::getInstance()->getIntegerForKey("runningSounId"));
}

void SplashLayer::generateSplash()
{
    CCLOG("in splash sound id %d", UserDefault::getInstance()->getIntegerForKey("runningSounId"));
    AudioEngine::pause(UserDefault::getInstance()->getIntegerForKey("runningSounId"));
    
    visibleSize = Director::getInstance()->getVisibleSize();
    //label1->removeFromParent();
    //if(levelComplete || timeUp) {
    if (levelComplete) {

        AudioEngine::play2d("sounds/WinSound.mp3", false, 0.5f);
        auto win_icon = Sprite::create("sprites/level_complete.png");
        win_icon->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - 150));
        this->addChild(win_icon);

        SpriteFrameCache* frameCache = SpriteFrameCache::getInstance();
        frameCache->addSpriteFramesWithFile("sprites/character/boyWin.plist");
        SpriteBatchNode* spritesheet = SpriteBatchNode::create("sprites/character/boyWin.png");
        this->addChild(spritesheet);

        Vector<SpriteFrame*> boyFrames;
        for (int i = 1; i <= 7; i++) {
            if (i == 2 || i == 3)
                continue;
            std::string num = StringUtils::format("Win__00%d.png", i);
            SpriteFrame* frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(num);
            boyFrames.pushBack(frame);
        }

        auto runAnimation = Animation::createWithSpriteFrames(boyFrames, 0.2f);
        auto sprite = Sprite::createWithSpriteFrameName("Win__001.png");
        sprite->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
        //sprite->setAnchorPoint(Vec2(0, 0.5));
        sprite->setScale(0.5);

        auto action = Animate::create(runAnimation);
        action->retain();
        sprite->runAction(action);
        sprite->setTexture(spritesheet->getTexture());
        spritesheet->addChild(sprite);

        if (MapLevelTag == 1)
            UserDefault::getInstance()->setBoolForKey("level1Complete", true);
        else if (MapLevelTag == 2)
            UserDefault::getInstance()->setBoolForKey("level2Complete", true);
        else if (MapLevelTag == 3)
            UserDefault::getInstance()->setBoolForKey("level3Complete", true);

        
    }

    if (timeUp) {
        AudioEngine::play2d("sounds/DieSound.mp3", false, 0.5f);
        auto dead_icon = Sprite::create("sprites/dead_icon.png");
        dead_icon->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - 100));
        this->addChild(dead_icon);

        SpriteFrameCache* frameCache = SpriteFrameCache::getInstance();
        frameCache->addSpriteFramesWithFile("sprites/character/boyDie.plist");
        SpriteBatchNode* spritesheet = SpriteBatchNode::create("sprites/character/boyDie.png");
        this->addChild(spritesheet);

        Vector<SpriteFrame*> boyFrames;
        for (int i = 1; i <= 5; i++) {
            
            std::string num = StringUtils::format("Dead__00%d.png", i);
            SpriteFrame* frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(num);
            boyFrames.pushBack(frame);
        }

        auto runAnimation = Animation::createWithSpriteFrames(boyFrames, 0.2f);
        auto sprite = Sprite::createWithSpriteFrameName("Dead__001.png");
        sprite->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
        //sprite->setAnchorPoint(Vec2(0, 0.5));
        sprite->setScale(0.5);


        auto action = Animate::create(runAnimation);
        action->retain();
        sprite->runAction(action);
        sprite->setTexture(spritesheet->getTexture());
        spritesheet->addChild(sprite);
    }
}

//void SplashLayer::nextLevel(Ref* pSender)
//{
//    CCLOG("mapleveltag %d", MapLevelTag);
//}

void MapSceneHud::UpdateHud(float dt)
{
    this->schedule(CC_SCHEDULE_SELECTOR(MapSceneHud::UpdateHealth), 0.01);
    if (time < 80)
        this->schedule(CC_SCHEDULE_SELECTOR(MapSceneHud::UpdateFog), 100 / time);
    else
        this->schedule(CC_SCHEDULE_SELECTOR(MapSceneHud::UpdateFog), 100 / 80);
    
    //this->schedule(CC_SCHEDULE_SELECTOR(MapSceneHud::UpdateFog), 1);
}

void MapSceneHud::UpdateHealth(float dt) {
    if (!splashTag && !isPaused) {
        //if (currentPercentage > 0.0) {
            currentPercentage -= dt * percentChangeRate;
            //CCLOG("curretPercentage %f", currentPercentage);
            healthBar->setPercentage(currentPercentage);
       //}
    }
}

void MapSceneHud::UpdateFog(float dt) {
    float requiredPercentage = 70.0;
    float timeFog;
    if (time < 70) timeFog = time;
    else timeFog = 70;
    if (!splashTag && !isPaused) {
        currentFogOpacity += dt * requiredPercentage / timeFog;
        fog->setOpacity(currentFogOpacity);
    }
}

void MapSceneHud::pauseButton(Ref* psender)
{
    if (isPaused) {
        CCLOG("game resumed");
        isPaused = false;
        if (label1) {
            CCLOG("removing label1");
            label1->removeFromParent();
            playButton->setTexture("sprites/buttons/pauseButton.png");
        }
    }
    else {
        CCLOG("game paused");
        isPaused = true;
        CCLOG("adding label1");
        label1 = Label::createWithTTF("Game Paused", "fonts/Nabila.ttf", 54);
        label1->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
        label1->enableBold();
        this->addChild(label1, 1);
        playButton->setTexture("sprites/buttons/resumeButton.png");
    }
}

void MapSceneHud::restartButton(Ref* pSender) {
    auto newscene = MapScene::createScene();
    Director::getInstance()->replaceScene(newscene);
}

void MapSceneHud::BackButton(Ref* pSender) {
    CCLOG("Home");
    CCLOG("in backbutton sound id %d", UserDefault::getInstance()->getIntegerForKey("runningSounId"));
    AudioEngine::stop(UserDefault::getInstance()->getIntegerForKey("runningSounId"));
    CCLOG("total sounds %d", AudioEngine::getPlayingAudioCount());
    Director::getInstance()->popScene();
    
}

void MapSceneHud::MuteButton(Ref* pSender) {
    if (sound) {
        CCLOG("Mute");
        //soundButton = Sprite::create("SoundOffButton.png");
        soundButton->setTexture("sprites/buttons/SoundOffButton.png");
        AudioEngine::pauseAll();
        sound = false;
    }
    else {
        CCLOG("Sound on");
        //soundButton = Sprite::create("SoundOnButton.png");
        soundButton->setTexture("sprites/buttons/SoundOnButton.png");
        AudioEngine::resume(0);
        sound = true;
    }
}
