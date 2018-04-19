#ifndef _CHARBOWSER_H_
#define _CHARBOWSER_H_

#include "GameGlobal.h"
#include "GameScene.h"
#include "GameMap.h"

class CharBowser: public CCArmature {
public:
	bool init(GameMap * gameMap);
	static CharBowser * create(GameMap * gameMap);
	void Movement();
	void CheckMario();
	void GetRemoved();
	void UpdateItem();
	void TestAndTurn();
	void Playing();
	void BurstCoin();
	void GetPass();
	void GetHurt();
	void GetDie();
	void MakeFire(int addAngle);
	void PlayAnimation();
	void onUpdate(float dt);

	float xvel, yvel, yvelacc;
	int frames, nowplaying, aniplays, hp1, hp2, anid, playid, immuneTime;
	bool died, left, turning;
	GameMap * _themap;
	CCTMXLayer * pLandLayer;
	CCRect JudgePoint;
};

#endif