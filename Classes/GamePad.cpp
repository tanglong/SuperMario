#include "GamePad.h"

GamePad::GamePad(void) {
}

GamePad::~GamePad(void) {
}

GamePad * GamePad::loadnow() {
    GamePad * pRet = new GamePad();
    if (pRet && pRet->initnow()) {
		pRet->autorelease();
        return pRet;
    } else {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

bool GamePad::initnow() {
	if (!CCLayer::init()) {
		return false;
	}
	
	setTouchEnabled(true);

	CCTexture2D * texture = CCTextureCache::sharedTextureCache()->textureForKey("sprites.png");
	CCSpriteBatchNode * node = CCSpriteBatchNode::createWithTexture(texture);
	addChild(node);

//	CCSprite * arrpad = CCSprite::createWithSpriteFrameName("arrpad.png");
//	arrpad->setPosition(ccp(140, 140));
//	arrpad->setScale(2);
//	node->addChild(arrpad);
    
    
    leftS=CCSprite::create("jiantou.png");
    leftS->setPosition(ccp(50,80));
    leftS->setScale(2);
    this->addChild(leftS);
    leftS->setFlipX(true);
    leftS->setOpacity(80);
    
    midS=CCSprite::create("jiantou.png");
    midS->setPosition(ccp(140,80));
    midS->setScale(2);
    midS->setRotation(90);
    this->addChild(midS);
    midS->setOpacity(100);
    
    
    rightS=CCSprite::create("jiantou.png");
    rightS->setPosition(ccp(230,80));
    rightS->setScale(2);
    this->addChild(rightS);
    rightS->setOpacity(100);
	
	CCSprite * funpad = CCSprite::createWithSpriteFrameName("funpad.png");
	funpad->setPosition(ccp(660, 80));
	funpad->setScale(2);
    funpad->setColor(ccc3(255, 255, 0));
	node->addChild(funpad);
	
	CCSprite * qutpad = CCSprite::createWithSpriteFrameName("qutpad.png");
	qutpad->setPosition(ccp(760, 440));
	qutpad->setScale(2);
	node->addChild(qutpad);
	
	orderslist.up = false;
	orderslist.down = false;
	orderslist.left = false;
	orderslist.right = false;
	orderslist.jump = false;
	orderslist.fire = false;

	return true;
}

void GamePad::ccTouchesBegan(CCSet * pTouches, CCEvent * pEvent) {
    CCSetIterator iter = pTouches->begin();
    for (; iter != pTouches->end(); iter++) {
        CCTouch * pTouch = (CCTouch *)(* iter);
        CCPoint location = pTouch->getLocation();
		KPadPress(location.x, location.y, true);
    }
}

void GamePad::ccTouchesMoved(CCSet * pTouches, CCEvent * pEvent) {
    CCSetIterator iter = pTouches->begin();
    for (; iter != pTouches->end(); iter++) {
        CCTouch * pTouch = (CCTouch *)(* iter);
        CCPoint location2 = pTouch->getPreviousLocation();
		KPadPress(location2.x, location2.y, false);
        CCPoint location = pTouch->getLocation();
		KPadPress(location.x, location.y, true);
    }
}

void GamePad::ccTouchesEnded(CCSet * pTouches, CCEvent * pEvent) {
    CCSetIterator iter = pTouches->begin();
    for (; iter != pTouches->end(); iter++) {
        CCTouch * pTouch = (CCTouch *)(* iter);
        CCPoint location = pTouch->getLocation();
		KPadPress(location.x, location.y, false);
    }
}

void GamePad::KPadPress(int tx, int ty, bool value) {
    /*
	if (tx >= 100 && tx <= 180 && ty >= 160 && ty <= 260) { //up
		orderslist.up = value;
	}
	if (tx >= 100 && tx <= 180 && ty <= 120) { //down
		orderslist.down = value;
	} 
	if ((tx >= 180 && tx <= 290 && ty <= 260) || (tx >= 140 && ty >= 120 && ty <= 160)) { //right
		orderslist.right = value;
	}
	if ((tx <= 100 && ty <= 260) || (tx <= 140 && ty >= 120 && ty <= 160)) { //left
		orderslist.left = value;
	}
     */
    
    if (leftS->boundingBox().containsPoint(ccp(tx, ty))) {
        orderslist.left = value;
    }
    if (midS->boundingBox().containsPoint(ccp(tx, ty))) {
        orderslist.up = value;
    }
    if (rightS->boundingBox().containsPoint(ccp(tx, ty))) {
        orderslist.right = value;
    }
    
    
	if (tx >= 520 && tx <= 684 && ty <= 140) {
		orderslist.fire = value;
	}
	if (tx >= 684 && ty <= 140) {
		orderslist.jump = value;
	}
	if (tx >= 720 && ty >= 400 && value) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
		CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
#else
		CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		exit(0);
#endif
#endif
	}
}