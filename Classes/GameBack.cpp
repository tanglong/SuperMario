#include "GameBack.h"

GameBack::GameBack(void) {
}

GameBack::~GameBack(void) {
}

GameBack * GameBack::loadnow(CCString * backs) {
    GameBack * pRet = new GameBack();
    if (pRet && pRet->initnow(backs)) {
        return pRet;
    } else {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

bool GameBack::initnow(CCString * backs) {
	if (!CCLayer::init()) {
		return false;
	}

	const char * bs = backs->getCString();
	int k = strlen(bs);
	
	char buff[10] = "backx.png";
	for (int i = 1; i <= k; i++) {
		buff[4] = bs[k - i];

		CCSprite * pic = CCSprite::createWithTexture(CCTextureCache::sharedTextureCache()->textureForKey(buff));
		pic->setAnchorPoint(ccp(0, 0));
		pic->setPosition(ccp(0, 800 * (i - 1)));
		addChild(pic);

		CCSprite * pic2 = CCSprite::createWithTexture(CCTextureCache::sharedTextureCache()->textureForKey(buff));
		pic2->setAnchorPoint(ccp(0, 0));
		pic2->setPosition(ccp(800, 800 * (i - 1)));
		addChild(pic2);
	}

	return true;
}