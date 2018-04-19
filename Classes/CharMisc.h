#ifndef _CHARMISC_H_
#define _CHARMISC_H_

#include "GameGlobal.h"
#include "GameScene.h"
#include "GameMap.h"

class CharMisc: public CCArmature {
public:
	bool init(int id, int func, CCDictionary * pDic, GameMap * gameMap);
	static CharMisc * create(int id, int func, CCDictionary * pDic, GameMap * gameMap);
	void Movement();
	void CheckMario();
	void SetEnabled();
	void GetRemoved();
	void CheckReset();
	void onUpdate(float dt);

	float xvel, xvelfixed, yvel;
	bool landed, enabled, gravity;
	int _id, _func;
	CCDictionary * _pdic;
	GameMap * _themap;
	CCRect JudgePoint;
};

#endif