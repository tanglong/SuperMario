#ifndef _CHARBLOCK_H_
#define _CHARBLOCK_H_

#include "GameGlobal.h"
#include "GameScene.h"
#include "GameMap.h"

class CharBlock: public CCArmature {
public:
	bool init(int id, const char * func, CCDictionary * pDic, GameMap * gameMap);
	static CharBlock * create(int id, const char * func, CCDictionary * pDic, GameMap * gameMap);
	void CheckMario();
	void Headed();
	void Grows(CCNode * pTarget, void * data);
	void CheckReset();
	void GetReset();
	void GetRemoved();
	void onUpdate(float dt);

	bool died;
	int _id;
	CCDictionary * _pdic;
	const char * _func;
	GameMap * _themap;
	CCRect JudgePoint;
};

#endif