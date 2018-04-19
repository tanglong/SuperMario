#include "GameSwap.h"

GameSwap::GameSwap(void) {
}

GameSwap::~GameSwap(void) {
}

GameSwap * GameSwap::loadnow(int levela, int levelb, int deaths) {
    GameSwap * pRet = new GameSwap();
    if (pRet && pRet->initnow(levela, levelb, deaths)) {
        return pRet;
    } else {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

bool GameSwap::initnow(int levela, int levelb, int deaths) {
	if (!CCLayer::init()) {
		return false;
	}
	
    CCSize size = CCDirector::sharedDirector()->getWinSize();

    CCLabelTTF * ttf = CCLabelTTF::create("WORLD", USE_FONT, 30);
	ttf->setVisible(false);
    this->addChild(ttf);

    CCLabelTTF * ttf1 = CCLabelTTF::create("WORLD", USE_FONT, 30);
    ttf1->setPosition(ccp(size.width / 2, size.height / 2 + 60));
    this->addChild(ttf1);

    CCLabelTTF * ttf2 = CCLabelTTF::create(CCString::createWithFormat("%d - %d", levela, levelb)->getCString(), USE_FONT, 30);
    ttf2->setPosition(ccp(size.width / 2, size.height / 2 + 30));
    this->addChild(ttf2);

    CCLabelTTF * ttf3 = CCLabelTTF::create("DEATHS", USE_FONT, 30);
    ttf3->setPosition(ccp(size.width / 2, size.height / 2 - 30));
    this->addChild(ttf3);

    CCLabelTTF * ttf4 = CCLabelTTF::create(CCString::createWithFormat("%d", deaths)->getCString(), USE_FONT, 30);
    ttf4->setPosition(ccp(size.width / 2, size.height / 2 - 60));
    this->addChild(ttf4);

	return true;
}