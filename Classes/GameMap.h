#ifndef __GAMEMAP_H__
#define __GAMEMAP_H__

#include "GameGlobal.h"
#include "GameScene.h"
#include "GameBack.h"

class CharMario;

class GameMap: public CCLayer {
public:
	GameMap(void);
	~GameMap(void);
	static GameMap * loadnow(GameScene * thescene, int levela, int levelb);
	bool initnow(GameScene * thescene, int levela, int levelb);
	void onUpdate(float dt);
	xCheckInfo CheckX(float upY, float downY, float leftX, float rightX);
	yCheckInfo CheckY(float upY, float downY, float leftX, float rightX);
	CCSprite * getTileByCoord(CCPoint position);
	void cameraControl();
	void setEnemies();
	void setMisc(int id, int func, int x, int y, CCDictionary * pDic);

	bool gaming, boss, fixedcamera, showonly, automove;
	int destx, destmy, myx;
	float yvel, yvelacc;
	char mus;
	char * bossname;
	CCString * backs;
	CharMario * pMario;
	OrdersList getOrders();
	GameScene * _thescene;
	CCTMXTiledMap * xMap;
	GameBack * background;

	CREATE_FUNC(GameMap);
	CC_SYNTHESIZE_RETAIN(CCArray *, _shells, Shells);
	CC_SYNTHESIZE_RETAIN(CCArray *, _bullets, Bullets);
	CC_SYNTHESIZE_RETAIN(CCArray *, _enemies, Enemies);
};
#endif