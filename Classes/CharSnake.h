#ifndef _CHARSNAKE_H_
#define _CHARSNAKE_H_

#include "GameGlobal.h"
#include "GameScene.h"
#include "GameMap.h"

class CharSnake: public CCArmature {
public:
	bool init(CharSnake * ahead, float dx, float dy, GameMap * gameMap);
	static CharSnake * create(CharSnake * ahead, float dx, float dy, GameMap * gameMap);
	void Movement();
	void CheckMario();
	void GetRemoved();
	void onUpdate(float dt);

	float xvel, yvel, velfixed, _dx, _dy;
	int prex, prey, stopframe;
	bool startmoving;
	GameMap * _themap;
	CharSnake * _ahead;
	CCRect JudgePoint;
};

#endif