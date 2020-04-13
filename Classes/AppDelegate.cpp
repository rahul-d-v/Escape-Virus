#include "AppDelegate.h"
#include "HelloWorldScene.h"

USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate()
{
}

bool AppDelegate::applicationDidFinishLaunching() {
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if (!glview) {
        glview = GLViewImpl::create("Escape Virus");
        glview->setFrameSize(800, 600);
        director->setOpenGLView(glview);
    }

    // the two missing lines
    glview->setDesignResolutionSize(800, 600, ResolutionPolicy::FIXED_WIDTH);
    director->setContentScaleFactor(1.0f);

    auto scene = HelloWorld::createScene();
    director->runWithScene(scene);

    return true;
}

void AppDelegate::applicationDidEnterBackground() {
}

void AppDelegate::applicationWillEnterForeground() {
}