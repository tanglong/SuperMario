#ifndef _CHARMARIO_H_
#define _CHARMARIO_H_

#include "GameGlobal.h"
#include "GameScene.h"
#include "GameMap.h"

class CharMario: public CCArmature {
public:
	bool init(char * pFileName, int lv, int fr, GameMap * gameMap);
	static CharMario * create(char * pFileName, int lv, int fr, GameMap * gameMap);
	void PipeIn(int mapx, int mapy, float outx, float outy, CCString * outd);
	void PipeOut2();
	void PipeOut();
	void JudgeMovement();
	void Movement();
	void PlayAnimation();
	void GetPass();
	void GetPass2();
	void BossFight();
	void BossFight2();
	void GetDied();
	void GetRestarted();
	void GetHurt();
	void GrowUp(int id);
	void onUpdate(float dt);

	float xvel, yvel, xvelmax, xaccpress, xaccrelease;
	int level, nowplaying, aniplays, immuneTime, firerate, firecount, jumpcount;
	int _mapy, _mapx;
	float _outx, _outy;
	CCString * _outd;
	bool landed, died, downed, bossfighting;
	GameMap * _themap;
	CCRect JudgePoint;
};

#endif