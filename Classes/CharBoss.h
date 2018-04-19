#ifndef _CHARBOSS_H_
#define _CHARBOSS_H_

#include "GameGlobal.h"
#include "GameScene.h"
#include "GameMap.h"

class CharBoss: public CCArmature {
public:
	bool init(const char * pFileName, GameMap * gameMap);
	static CharBoss * create(const char * pFileName, GameMap * gameMap);
	void Movement();
	void PlayAnimation();
	void PlayKoppa();
	void PlayBrain();
	void GetPass();
	void GetHurt();
	void GetDie();
	void CheckMario();
	void GetRemoved();
	void onUpdate(float dt);

	float xvel, xvelmax, yvel;
	int nowplaying, aniplays, _id, _mapposy, anid, hp1, hp2, immuneTime;
	bool landed, died, fire;
	CCSprite * crack1, * crack2;
	GameMap * _themap;
	CCRect JudgePoint;
};

#endif