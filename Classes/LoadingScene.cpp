#include "LoadingScene.h"

bool LoadingScene::init() {
	totalNum = 45;
	haveLoadedNum = 0;
	this->loading();
	return true;
}

CCScene * LoadingScene::scene() {
    CCScene * scene = CCScene::create();
    LoadingScene * layer = LoadingScene::create();
    scene->addChild(layer);
    return scene;
}

void LoadingScene::loading() {
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    ttf = CCLabelTTF::create("0%", USE_FONT, 20);
      
    CCLabelTTF * havettf = CCLabelTTF::create("LOADING", USE_FONT, 20);
    this->addChild(ttf, 1);
    this->addChild(havettf, 1);
    ttf->setPosition(ccp(size.width / 2 - 40, size.height / 2));
    havettf->setPosition(ccp(size.width / 2 + 40, size.height / 2));
	
	loadpro = new ProgressView;
    loadpro->setPosition(ccp(size.width / 2, size.height / 2 - 30));
    loadpro->setBackgroundTexture("hpback.png", false);
    loadpro->setForegroundTexture("hpfore2.png", false);
    loadpro->setTotalProgress(100);
    loadpro->setCurrentProgress(0);
    loadpro->setScale(0.3f);
    this->addChild(loadpro);
	
    CCTextureCache::sharedTextureCache()->addImageAsync("back1.png", this, callfuncO_selector(LoadingScene::loadedCallBack));
    CCTextureCache::sharedTextureCache()->addImageAsync("back2.png", this, callfuncO_selector(LoadingScene::loadedCallBack));
    CCTextureCache::sharedTextureCache()->addImageAsync("back3.png", this, callfuncO_selector(LoadingScene::loadedCallBack));
    CCTextureCache::sharedTextureCache()->addImageAsync("back4.png", this, callfuncO_selector(LoadingScene::loadedCallBack));
    CCTextureCache::sharedTextureCache()->addImageAsync("back5.png", this, callfuncO_selector(LoadingScene::loadedCallBack));
    CCTextureCache::sharedTextureCache()->addImageAsync("back6.png", this, callfuncO_selector(LoadingScene::loadedCallBack));
    CCTextureCache::sharedTextureCache()->addImageAsync("back7.png", this, callfuncO_selector(LoadingScene::loadedCallBack));
    CCTextureCache::sharedTextureCache()->addImageAsync("back8.png", this, callfuncO_selector(LoadingScene::loadedCallBack));
    CCTextureCache::sharedTextureCache()->addImageAsync("back9.png", this, callfuncO_selector(LoadingScene::loadedCallBack));
    CCTextureCache::sharedTextureCache()->addImageAsync("back0.png", this, callfuncO_selector(LoadingScene::loadedCallBack));
    CCTextureCache::sharedTextureCache()->addImageAsync("backa.png", this, callfuncO_selector(LoadingScene::loadedCallBack));
    CCTextureCache::sharedTextureCache()->addImageAsync("backb.png", this, callfuncO_selector(LoadingScene::loadedCallBack));
    CCTextureCache::sharedTextureCache()->addImageAsync("backc.png", this, callfuncO_selector(LoadingScene::loadedCallBack));
    CCTextureCache::sharedTextureCache()->addImageAsync("backd.png", this, callfuncO_selector(LoadingScene::loadedCallBack));
    CCTextureCache::sharedTextureCache()->addImageAsync("backe.png", this, callfuncO_selector(LoadingScene::loadedCallBack));
    CCTextureCache::sharedTextureCache()->addImageAsync("backf.png", this, callfuncO_selector(LoadingScene::loadedCallBack));
    CCTextureCache::sharedTextureCache()->addImageAsync("backg.png", this, callfuncO_selector(LoadingScene::loadedCallBack));
    CCTextureCache::sharedTextureCache()->addImageAsync("backh.png", this, callfuncO_selector(LoadingScene::loadedCallBack));
    CCTextureCache::sharedTextureCache()->addImageAsync("backi.png", this, callfuncO_selector(LoadingScene::loadedCallBack));
    CCTextureCache::sharedTextureCache()->addImageAsync("sprites.png", this, callfuncO_selector(LoadingScene::loadedCallBack));
    CCTextureCache::sharedTextureCache()->addImageAsync("title.png", this, callfuncO_selector(LoadingScene::loadedCallBack));
    CCTextureCache::sharedTextureCache()->addImageAsync("credit.png", this, callfuncO_selector(LoadingScene::loadedCallBack));
	CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfoAsync("mario.ExportJson", this, schedule_selector(LoadingScene::loadedCallBack));
	CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfoAsync("blocks.ExportJson", this, schedule_selector(LoadingScene::loadedCallBack));
	CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfoAsync("misc.ExportJson", this, schedule_selector(LoadingScene::loadedCallBack));
	CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfoAsync("bullet.ExportJson", this, schedule_selector(LoadingScene::loadedCallBack));
	CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfoAsync("badmush.ExportJson", this, schedule_selector(LoadingScene::loadedCallBack));
	CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfoAsync("badturt.ExportJson", this, schedule_selector(LoadingScene::loadedCallBack));
	CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfoAsync("fireballs.ExportJson", this, schedule_selector(LoadingScene::loadedCallBack));
	CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfoAsync("riseball.ExportJson", this, schedule_selector(LoadingScene::loadedCallBack));
	CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfoAsync("koppa.ExportJson", this, schedule_selector(LoadingScene::loadedCallBack));
	CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfoAsync("badflower.ExportJson", this, schedule_selector(LoadingScene::loadedCallBack));
	CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfoAsync("brain.ExportJson", this, schedule_selector(LoadingScene::loadedCallBack));
	CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfoAsync("cannon.ExportJson", this, schedule_selector(LoadingScene::loadedCallBack));
	CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfoAsync("hedgehog.ExportJson", this, schedule_selector(LoadingScene::loadedCallBack));
	CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfoAsync("fireflower.ExportJson", this, schedule_selector(LoadingScene::loadedCallBack));
	CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfoAsync("cloud.ExportJson", this, schedule_selector(LoadingScene::loadedCallBack));
	CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfoAsync("hedball.ExportJson", this, schedule_selector(LoadingScene::loadedCallBack));
	CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfoAsync("hamturt.ExportJson", this, schedule_selector(LoadingScene::loadedCallBack));
	CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfoAsync("gunturt.ExportJson", this, schedule_selector(LoadingScene::loadedCallBack));
	CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfoAsync("flyfish.ExportJson", this, schedule_selector(LoadingScene::loadedCallBack));
	CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfoAsync("wheels.ExportJson", this, schedule_selector(LoadingScene::loadedCallBack));
	CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfoAsync("flight.ExportJson", this, schedule_selector(LoadingScene::loadedCallBack));
	CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfoAsync("snake.ExportJson", this, schedule_selector(LoadingScene::loadedCallBack));
	CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfoAsync("bowser.ExportJson", this, schedule_selector(LoadingScene::loadedCallBack));
	
	SimpleAudioEngine::sharedEngine()->preloadEffect("blocksmash.mp3");
	SimpleAudioEngine::sharedEngine()->preloadEffect("playerdied.mp3");
	SimpleAudioEngine::sharedEngine()->preloadEffect("pipe.mp3");
	SimpleAudioEngine::sharedEngine()->preloadEffect("grow.mp3");
	SimpleAudioEngine::sharedEngine()->preloadEffect("passed.mp3");
	SimpleAudioEngine::sharedEngine()->preloadEffect("coin.mp3");
	SimpleAudioEngine::sharedEngine()->preloadEffect("tread.mp3");
	SimpleAudioEngine::sharedEngine()->preloadEffect("shell.mp3");
	SimpleAudioEngine::sharedEngine()->preloadEffect("bump.mp3");
	SimpleAudioEngine::sharedEngine()->preloadEffect("firechain.mp3");
	SimpleAudioEngine::sharedEngine()->preloadEffect("spawn.mp3");
	SimpleAudioEngine::sharedEngine()->preloadEffect("taunt1.mp3");
	SimpleAudioEngine::sharedEngine()->preloadEffect("taunt2.mp3");
	SimpleAudioEngine::sharedEngine()->preloadEffect("gun.mp3");
	SimpleAudioEngine::sharedEngine()->preloadEffect("throw.mp3");
	SimpleAudioEngine::sharedEngine()->preloadEffect("smash.mp3");
	SimpleAudioEngine::sharedEngine()->preloadEffect("gamebeat.mp3");
	
	SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("b1.mp3");
	SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("b2.mp3");

	SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("m1.mp3");
	SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("m2.mp3");
	SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("m3.mp3");
	SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("m4.mp3");
	SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("m5.mp3");
	SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("m6.mp3");
	SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("m7.mp3");
	SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("m8.mp3");
	SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("m9.mp3");
	SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("m0.mp3");
	SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("ma.mp3");
	SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("mb.mp3");
	SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("mc.mp3");
	SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("md.mp3");
	SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("me.mp3");
	SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("mf.mp3");
	SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("mg.mp3");
	SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("mh.mp3");
	SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("mi.mp3");
}

void LoadingScene::loadedCallBack(CCObject * obj) {
    haveLoadedNum++;
	
    this->runAction(CCDelayTime::create(15));
    char tmp[10];
	int percentage = (int)((float)haveLoadedNum / totalNum * 100);
	if (percentage >= 100) {
		percentage = 100;
	}
    sprintf(tmp, "%d%%", percentage);
    ttf->setString(tmp);
    loadpro->setCurrentProgress(percentage);
    if (haveLoadedNum == totalNum) {
		CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("sprites.plist", CCTextureCache::sharedTextureCache()->textureForKey("sprites.png"));
        this->removeChild(ttf, true);
        CCScene * newscne = GameScene::LoadLayers();
        CCDirector::sharedDirector()->replaceScene(newscne);
    }
}  