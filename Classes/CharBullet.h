#ifndef _CHARBULLET_H_
#define _CHARBULLET_H_

#include "GameGlobal.h"
#include "GameScene.h"
#include "GameMap.h"

class CharBullet: public CCArmature {
public:
	bool init(int id, bool right, bool hurt, GameMap * gameMap, float dx, float dy, int len);
	static CharBullet * create(int id, bool right, bool hurt, GameMap * gameMap, float dx, float dy, int len);
	void Movement();
	void PlayAnimation();
	void GetDied();
	void CheckMario();
	void CheckReset();
	void GetRemoved();
	void onUpdate(float dt);

	float xvel, xvelfixed, yvel, _dx, _dy;
	int nowplaying, aniplays, _id, _len, frames;
	bool died, hurtenemies, hurtmario;
	GameMap * _themap;
	CCRect JudgePoint;
};

#endif