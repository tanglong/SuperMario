#include "GameScene.h"
#include "GameMap.h"
#include "CharMario.h"

CCScene * GameScene::LoadLayers() {
    GameScene * scene = NULL;
	do {
		scene = GameScene::create();
		CC_BREAK_IF(!scene);
	} while(0);
    return scene;
}

bool GameScene::init() {
	deaths = 0;
	coins = 0;
	score = 0;
	levela = 0;
	levelb = 0;
	checkpoint = 0;
	mariofirerate = 30;
	mariolevel = 1;
	titlepanel = true;
	
	Hp1 = new ProgressView;
    Hp1->setPosition(ccp(400, 100));
    Hp1->setBackgroundTexture("hpback.png", true);
    Hp1->setForegroundTexture("hpfore1.png", true);
    Hp1->setTotalProgress(100);
    Hp1->setCurrentProgress(100);
    Hp1->setScale(0.3f);
	Hp1->setVisible(false);
    this->addChild(Hp1, 200);

	Hp2 = new ProgressView;
    Hp2->setPosition(ccp(400, 150));
    Hp2->setBackgroundTexture("hpback.png", true);
    Hp2->setForegroundTexture("hpfore2.png", true);
    Hp2->setTotalProgress(100);
    Hp2->setCurrentProgress(100);
    Hp2->setScale(0.3f);
	Hp2->setVisible(false);
    this->addChild(Hp2, 200);
	
//#if(CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
	gamePad = GamePad::loadnow();
	this->addChild(gamePad, 300);
//#endif

	lilcoin = CCSprite::createWithSpriteFrameName("littlecoin.png");
	lilcoin->setPosition(ccp(380, 460));
	lilcoin->setVisible(false);
	addChild(lilcoin, 5000);

    ttfcoin = StrokeLabelTTF::create("x 0", USE_FONT, 30);
    ttfcoin->setAnchorPoint(ccp(0, 0.5f));
    ttfcoin->setPosition(ccp(400, 460));
	ttfcoin->setVisible(false);
    addChild(ttfcoin, 5000);
	
	if (titlepanel) {
		title = CCSprite::createWithTexture(CCTextureCache::sharedTextureCache()->textureForKey("title.png"));
		title->setPosition(ccp(400, 240));
		addChild(title, 1000);

		mask = CCSprite::createWithSpriteFrameName("black.png");
		mask->setScaleX(30.f);
		mask->setScaleY(20.f);
		mask->setPosition(ccp(400, 240));
		mask->setVisible(false);
		addChild(mask, 10000);
		
		srand((unsigned)time(0));
		int lva = rand() % 3 + 1;
		int lvb = rand() % 3 + 1;
		this->removeChild(gameMap, true);
		gameMap = GameMap::loadnow(this, lva, lvb);
		gameMap->showonly = true;
		gameMap->pMario->setVisible(false);
		gameMap->pMario->died = true;
		this->addChild(gameMap, 100);
	} else {
		LoadLevel();
	}

	this->scheduleUpdate();
	
	return true;
}

void GameScene::LoadLevel() {
	this->removeChild(gameMap, true);
	
	lilcoin->setVisible(true);
	ttfcoin->setVisible(true);
	Hp1->setVisible(false);
	Hp2->setVisible(false);
	mask->setVisible(false);
	gameSwap = GameSwap::loadnow(levela, levelb, deaths);
	this->addChild(gameSwap, 100);

	scheduleOnce(schedule_selector(GameScene::LoadLevel2), 2);
}

void GameScene::LoadLevel2(float dt) {
	this->removeChild(gameSwap, true);
	gameMap = GameMap::loadnow(this, levela, levelb);
	if (levela == 8 && (levelb == 1 || levelb == 3)) {
		gameMap->automove = true;
	}
	if (levela == 8 && levelb == 5) {
		Hp1->setVisible(true);
		Hp1->setPosition(ccp(650, 440));
		Hp1->setTotalProgress(100);
	    Hp1->setCurrentProgress(100);
		Hp2->setVisible(true);
		Hp2->setPosition(ccp(150, 440));
		Hp2->setTotalProgress(100);
	    Hp2->setCurrentProgress(1);
	}
	this->addChild(gameMap, 100);
}

void GameScene::Restarts() {
	deaths++;
	mariolevel = 1;
	mariofirerate = 30;
	LoadLevel();
}

void GameScene::NextLevel() {
	checkpoint = 0;
	levelb += 1;
	if (levelb >= 5) {
		levela += 1;
		levelb = 1;
	}

	string path = CCFileUtils::sharedFileUtils()->getWritablePath() + "save";
	FILE * file = fopen(path.c_str(), "wb");
	int fff = deaths * 100 + levela * 10 + levelb;
	if (file) {
		fwrite(&fff, sizeof(int), 1, file);
		fclose(file);
	}

	LoadLevel();
}

void GameScene::GotoLevel(int lva, int lvb) {
	checkpoint = 0;
	levela = lva;
	levelb = lvb;

	string path = CCFileUtils::sharedFileUtils()->getWritablePath() + "save";
	FILE * file = fopen(path.c_str(), "wb");
	int fff = deaths * 100 + levela * 10 + levelb;
	if (file) {
		fwrite(&fff, sizeof(int), 1, file);
		fclose(file);
	}

	LoadLevel();
}

void GameScene::GetPass3() {
	CCScene * newscne = GameScene::LoadLayers();
	CCDirector::sharedDirector()->replaceScene(CCTransitionCrossFade::create(0.5f, newscne));
}

void GameScene::GetPass2() {
	this->removeChild(gameMap, true);
	levela = 9;
	levelb = 1;
	gameMap = GameMap::loadnow(this, 9, 1);
	gameMap->showonly = true;
	gameMap->pMario->setVisible(false);
	gameMap->pMario->died = true;
	this->addChild(gameMap, 100);
	mask->runAction(CCMoveBy::create(3.f, ccp(0, -1000)));
	CCSprite * credit = CCSprite::createWithTexture(CCTextureCache::sharedTextureCache()->textureForKey("credit.png"));
	credit->setAnchorPoint(ccp(0.5f, 0));
	credit->setPosition(ccp(400, -2000));
	addChild(credit, 9000);
	CCAction * action = CCSequence::create(CCMoveBy::create(45.f, ccp(0, 2200)),
		CCDelayTime::create(10.f),
		CCCallFunc::create(this, callfunc_selector(GameScene::GetPass3)),
		NULL);
	credit->runAction(action);
}

void GameScene::GetPass() {
	lilcoin->setVisible(false);
	ttfcoin->setVisible(false);
	mask->setVisible(true);
    mask->setOpacity(0);
	SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
	SimpleAudioEngine::sharedEngine()->playEffect("gamebeat.mp3");
	CCAction * action = CCSequence::create(CCDelayTime::create(7.f),
		CCFadeIn::create(3.f),
		CCCallFunc::create(this, callfunc_selector(GameScene::GetPass2)),
		NULL);
	mask->runAction(action);
}

void GameScene::getCoin() {
	SimpleAudioEngine::sharedEngine()->playEffect("coin.mp3");
	coins++;
	if (coins == 100) {
		SimpleAudioEngine::sharedEngine()->playEffect("grow.mp3");
		gameMap->pMario->immuneTime = 300;
		coins = 0;
	}
	ttfcoin->setString(CCString::createWithFormat("x %d", coins)->getCString());
}

void GameScene::StartGame(bool readfile) {
	SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
	titlepanel = false;
	this->removeChild(title, true);
	deaths = 0;
	coins = 0;
	score = 0;
	levela = 1;
	levelb = 1;
	checkpoint = 0;
	mariofirerate = 30;
	mariolevel = 1;

	if (readfile) {
		string path = CCFileUtils::sharedFileUtils()->getWritablePath() + "save";
		FILE * file = fopen(path.c_str(), "rb");
		int fff = 0;
		int nlevel = 0;
		if (file) {
			fread(&fff, sizeof(int), 1, file);
			fclose(file);
			nlevel = fff % 100;
			deaths = fff / 100;
			levelb = nlevel % 10;
			levela = nlevel / 10;
		}
	}
	LoadLevel();
}

void GameScene::update(float dt) {
#if(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	keyboardMapping();
#else
	orderslist = gamePad->orderslist;
#endif

	if (titlepanel) {
		if (orderslist.jump) {
			StartGame(false);
		}
		if (orderslist.fire) {
			StartGame(true);
		}
	}
}

#if(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
void GameScene::keyboardMapping() {
	if (KEY_DOWN(KEY_KEY_SPACE)) {
		orderslist.fire = true;
	}
	if (KEY_UP(KEY_KEY_SPACE)) {
		orderslist.fire = false;
	}
	if (KEY_DOWN(KEY_KEY_UP)) {
		orderslist.jump = true;
	}
	if (KEY_UP(KEY_KEY_UP)) {
		orderslist.jump = false;
	}
	if (KEY_DOWN(KEY_KEY_DOWN)) {
		orderslist.down = true;
	}
	if (KEY_UP(KEY_KEY_DOWN)) {
		orderslist.down = false;
	}
	if (KEY_DOWN(KEY_KEY_LEFT)) {
		orderslist.left = true;
	}
	if (KEY_UP(KEY_KEY_LEFT)) {
		orderslist.left = false;
	}
	if (KEY_DOWN(KEY_KEY_RIGHT)) {
		orderslist.right = true;
	}
	if (KEY_UP(KEY_KEY_RIGHT)) {
		orderslist.right = false;
	}
}
#endif