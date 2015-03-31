#include "SceneFailedConnected.hpp"
#include "const.hpp"

USING_NS_CC;

Scene* SceneFailedConnected::createScene()
{
    auto scene = Scene::create();
    auto layer = SceneFailedConnected::create();

    scene->addChild(layer);
    return scene;
}

bool SceneFailedConnected::init()
{
    char buff[100];
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    sprintf(buff,"Failed to Connected %s", IP);
    auto label = Label::createWithTTF(buff, "fonts/Marker Felt.ttf", 24);
    LOG("origin x %f y %f",origin.x,origin.y);
    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height/2));
    // add the label as a child to this layer
    this->addChild(label, 1);

    return true;
}
