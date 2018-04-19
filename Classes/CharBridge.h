#ifndef _CHARBRIDGE_H_
#define _CHARBRIDGE_H_

#include "GameGlobal.h"
#include "GameScene.h"
#include "GameMap.h"

class CharBridge: public CCNode {
public:
	bool init(int length, const char * dir, GameMap * gameMap);
	static CharBridge * create(int length, const char * dir, GameMap * gameMap);
	void CheckMario();
	void Movement();
	void CheckReset();
	void onUpdate(float dt);

	int _length, xvelmax, yvelmax, _mapposy;
	float xvel, yvel, xvelacc, yvelacc, nlength;
	bool stmove;
	const char * _dir;
	GameMap * _themap;
	CCSprite * _mySprite;
	CCRect JudgePoint;
};

#endif