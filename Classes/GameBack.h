#ifndef __GAMEBACK_H__
#define __GAMEBACK_H__

#include "GameGlobal.h"

class GameBack: public CCLayer {
public:
	GameBack(void);
	~GameBack(void);
	static GameBack * loadnow(CCString * backs);
	bool initnow(CCString * backs);

	CREATE_FUNC(GameBack);
};
#endif