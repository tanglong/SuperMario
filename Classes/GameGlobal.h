#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include "cocos2d.h"
#include "cocos-ext.h"
#include "SimpleAudioEngine.h"
#include "StrokeLabelTTF.h"
#include <ctime>

USING_NS_CC;
USING_NS_CC_EXT;

using namespace std;
//using namespace gui;
using namespace cocos2d;
using namespace CocosDenshion;

#define USE_FONT "EU-F6.ttf"

struct OrdersList {
	bool jump;
	bool fire;
	bool up;
	bool down;
	bool left;
	bool right;
};

struct xCheckInfo {
	int crashDirection;
	float leftX;
	float rightX;
};

struct yCheckInfo {
	int crashDirection;
	float upY;
	float downY;
};

#endif