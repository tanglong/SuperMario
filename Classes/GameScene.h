#ifndef __GAMESCENE_H__
#define __GAMESCENE_H__

#include "GameGlobal.h"
#include "ProgressView.h"
#include "GamePad.h"
#include "GameSwap.h"

class GameMap;

class GameScene : public CCScene {
public:
    virtual bool init();
    static CCScene * LoadLayers();
	void LoadLevel();
	void LoadLevel2(float dt);
	void Restarts();
	void NextLevel();
	void GotoLevel(int lva, int lvb);
	void GetPass();
	void GetPass2();
	void GetPass3();
	void getCoin();
	void StartGame(bool readfile);
	void update(float dt);

#if(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	void keyboardMapping();
#endif

	OrdersList orderslist;
	GameMap * gameMap;
	GameSwap * gameSwap;
	GamePad * gamePad;
	CCSprite * title, * mask, * lilcoin;
	StrokeLabelTTF * ttfcoin;
	int deaths, score, coins, levela, levelb, mariofirerate, mariolevel, checkpoint;
	ProgressView * Hp1, * Hp2;
	bool titlepanel;

    CREATE_FUNC(GameScene);
};

#if(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include <windows.h>
#define KEY_DOWN(vk_code) (GetAsyncKeyState(vk_code) & 0x8000 ? 1 : 0)
#define KEY_UP(vk_code) (GetAsyncKeyState(vk_code) & 0x8000 ? 0 : 1)

enum EKEY_CODE {
	KEY_KEY_UP = 0x26,  // Up key
	KEY_KEY_DOWN = 0x28,  // Down key
	KEY_KEY_LEFT = 0x25,  // Left key
	KEY_KEY_RIGHT = 0x27,  // Right key
	KEY_KEY_SPACE = 32,  // Space key

	KEY_KEY_CODES_COUNT  = 0xFF // this is not a key, but the amount of keycodes there are.
};

#endif

#endif