#ifndef _CHARENEMY_H_
#define _CHARENEMY_H_

#include "GameGlobal.h"
#include "GameScene.h"
#include "GameMap.h"

class CharEnemy: public CCArmature {
public:
	bool init(const char * pFileName, float dx, float dy, GameMap * gameMap);
	static CharEnemy * create(const char * pFileName, float dx, float dy, GameMap * gameMap);
	void CloudMove();
	void Movement();
	void PlayAnimation();
	void FireGun();
	void GetDied();
	void GetTread();
	void CheckReset();
	void FireEnemies();
	void FireRounds();
	void FireRocket();
	void CheckMario();
	void GetShell(bool left);
	void GetRemoved();
	void TestGrow();
	void FlowerDown();
	void FlowerGrow();
	void onUpdate(float dt);

	float xvel, xvelfixed, yvel, _dx, _dy;
	int nowplaying, aniplays, _id, addy, _mapposy, frames;
	bool landed, treaden, died, shelled, shellpushed, growing, atk;
	GameMap * _themap;
	CCRect JudgePoint;
};

#endif