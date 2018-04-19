#ifndef __GAMESWAP_H__
#define __GAMESWAP_H__

#include "GameGlobal.h"

class GameSwap: public CCLayer {
public:
	GameSwap(void);
	~GameSwap(void);
	static GameSwap * loadnow(int levela, int levelb, int deaths);
	bool initnow(int levela, int levelb, int deaths);

	CREATE_FUNC(GameSwap);
};
#endif