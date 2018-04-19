#ifndef __GAMEPAD_H__
#define __GAMEPAD_H__

#include "GameGlobal.h"

class GamePad: public CCLayer {
public:
	GamePad(void);
	~GamePad(void);
	static GamePad * loadnow();
	bool initnow();
	void KPadPress(int tx, int ty, bool value);
	void ccTouchesBegan(CCSet * pTouches, CCEvent * pEvent);
	void ccTouchesMoved(CCSet * pTouches, CCEvent * pEvent);
	void ccTouchesEnded(CCSet * pTouches, CCEvent * pEvent);

	OrdersList orderslist;
    CCSprite* leftS,*rightS,*midS;

	CREATE_FUNC(GamePad);
};
#endif