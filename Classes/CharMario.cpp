

#include "CharMario.h"
#include "CharBullet.h"
#include "CharBoss.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <jni.h>
#include "platform/android/jni/JniHelper.h"
#include <android/log.h>
#endif
CharMario * CharMario::create(char * pFileName, int lv, int fr, GameMap * gameMap) {
	do {
		CharMario * pMario = new CharMario;
		CC_BREAK_IF(pMario == NULL);

		pMario->init(pFileName, lv, fr, gameMap);
		pMario->autorelease();
		return pMario;
	} while (false);
	return NULL;
}

bool CharMario::init(char * pFileName, int lv, int fr, GameMap * gameMap) {
	do {
		CC_BREAK_IF(CCArmature::init(pFileName) == false);

		_themap = gameMap;
		xvel = 0;
		yvel = 0;
		xvelmax = 8;
		xaccpress = 0.5f;
		xaccrelease = 0.5f;
		level = lv;
		nowplaying = 0;
		aniplays = 1;
		immuneTime = 0;
		firerate = fr;
		firecount = 0;
		jumpcount = 0;
		landed = false;
		died = false;
		downed = false;
		bossfighting = false;

		this->setScale(2.0f);

		this->schedule(schedule_selector(CharMario::onUpdate));

		return true;
	} while (false);
	return false;
}

void CharMario::BossFight() {
	SimpleAudioEngine::sharedEngine()->playBackgroundMusic(CCString::createWithFormat("b%d.mp3", (_themap->_thescene->levela - 1) % 2 + 1)->getCString(), true);
	bossfighting = true;
	_themap->fixedcamera = true;
	_themap->gaming = false;
	CCAction * action = CCSequence::create(CCMoveTo::create(2, ccp(-_themap->destx, _themap->getPositionY())),
		CCCallFunc::create(this, callfunc_selector(CharMario::BossFight2)),
		NULL);
	_themap->runAction(action);
}

void CharMario::BossFight2() {
	_themap->gaming = true;
	CCTMXObjectGroup * pSpritesGroup = _themap->xMap->objectGroupNamed("sprites");
	CCDictionary * dest = pSpritesGroup->objectNamed("destination");
	CharBoss * pBoss;
	pBoss = CharBoss::create(((CCString *)(dest->objectForKey("bname")))->getCString(), _themap);
	_themap->addChild(pBoss, 1000000);
}

void CharMario::PipeOut2() {
	_themap->gaming = true;
	this->setZOrder(1000000);
}

void CharMario::PipeOut() {
	if (_outd->m_sString == "up") {
		SimpleAudioEngine::sharedEngine()->playEffect("pipe.mp3");
		this->setPositionX(_outx * 32);
		this->setPositionY(_outy * 32 - 32);
		CCAction * action = CCSequence::create(CCMoveBy::create(1, ccp(0, 64)),
			CCCallFunc::create(this, callfunc_selector(CharMario::PipeOut2)),
			NULL);
		this->runAction(action);
		_themap->setPositionX(_mapx);
		_themap->setPositionY(_mapy);
	}
	if (_outd->m_sString == "other") {
		_themap->_thescene->mariolevel = level;
		_themap->_thescene->mariofirerate = firerate;
		_themap->_thescene->GotoLevel(_outx, _outy);
	}
}

void CharMario::PipeIn(int mapx, int mapy, float outx, float outy, CCString * outd) {
	xvel = 0;
	yvel = 0;
	_themap->gaming = false;
	
	_mapx = mapx;
	_mapy = mapy;
	_outx = outx;
	_outy = outy;
	_outd = outd;
	
	this->setZOrder(1999);
	CCAction * action = CCSequence::create(CCMoveBy::create(1, ccp(0, -64)),
		CCCallFunc::create(this, callfunc_selector(CharMario::PipeOut)),
		NULL);
	this->runAction(action);
	SimpleAudioEngine::sharedEngine()->playEffect("pipe.mp3");
}

void CharMario::JudgeMovement() {
	OrdersList orderslist = _themap->getOrders();

	if (orderslist.jump) {
		if (landed) {
			jumpcount = 0;
		}
		if (landed || (jumpcount <= 12 && yvel > 0)) {
			yvel = 10;
			jumpcount++;
		}
	} else {
		if (yvel > 0) {
			jumpcount = 99;
		}
	}
	downed = orderslist.down;
	if (downed) {
		orderslist.left = false;
		orderslist.right = false;
		orderslist.fire = false;
		
		CCPoint loc;
		loc = ccp((int)this->getPositionX() / 32, _themap->xMap->getMapSize().height - (int)this->getPositionY() / 32);
		if (loc.x >= 0 && loc.y >= 0) {
			CCTMXLayer * pPipeinLayer = _themap->xMap->layerNamed("pipein");
			CCSprite * pTile = NULL;
			pTile = pPipeinLayer->tileAt(loc);
			if (pTile != NULL) {
				CCTMXObjectGroup * pSpritesGroup = _themap->xMap->objectGroupNamed("sprites");
				CCObject * pObj;
				CCArray * pArray = pSpritesGroup->getObjects();

				CCARRAY_FOREACH(pArray, pObj) {
					CC_BREAK_IF(pObj == NULL);
					
					CCDictionary * pDic = (CCDictionary *)pObj;
					int px = ((CCString *)(pDic->objectForKey("x")))->intValue() / 32;
					int py = ((CCString *)(pDic->objectForKey("y")))->intValue() / 32;
					CCString * pString = (CCString *)(pDic->objectForKey("type"));
					if (this->getPositionX() > px * 32 + 10 && this->getPositionX() < px * 32 + 54 && py == _themap->xMap->getMapSize().height - loc.y - 1 && pString->m_sString == "pipein") {
						CCString * dString = (CCString *)(pDic->objectForKey("ind"));
						if (dString->m_sString == "down") {
							this->setPositionX(px * 32 + 32);
							PipeIn(((CCString *)(pDic->objectForKey("mapx")))->intValue(), ((CCString *)(pDic->objectForKey("mapy")))->intValue(), ((CCString *)(pDic->objectForKey("outx")))->doubleValue(), ((CCString *)(pDic->objectForKey("outy")))->doubleValue(), (CCString *)(pDic->objectForKey("outd")));
						}
					}
				}
			}
		}
	}
	if (orderslist.left) {
		xvel -= xaccpress;
		if (xvel > 0) {
			xvel -= xaccpress;
		}
		if (xvel <= -xvelmax) {
			xvel = -xvelmax;
		}
	}
	if (orderslist.right) {
		xvel += xaccpress;
		if (xvel < 0) {
			xvel += xaccpress;
		}
		if (xvel >= xvelmax) {
			xvel = xvelmax;
		}
	}
	if (orderslist.fire && firecount == 0) {
		if (level == 3) {
			CharBullet * pBullet;
			pBullet = CharBullet::create(0, this->getRotationY() == 0, false, _themap, this->getPositionX(), this->getPositionY() + 10, 0);
			pBullet->setPosition(this->getPositionX(), this->getPositionY() + 10);
			_themap->getBullets()->addObject(pBullet);
			_themap->addChild(pBullet, 100000);
			firecount = firerate;
		} else if (level == 4) {
			CharBullet * pBullet;
			int cx = -10;
			if (this->getRotationY() == 0) {
				cx = 10;
			}
			SimpleAudioEngine::sharedEngine()->playEffect("firechain.mp3");
			pBullet = CharBullet::create(4, this->getRotationY() == 0, false, _themap, this->getPositionX() + cx, this->getPositionY() - 12, 0);
			pBullet->setPosition(this->getPositionX() + cx, this->getPositionY() - 12);
			_themap->getBullets()->addObject(pBullet);
			_themap->addChild(pBullet, 100000);
			firecount = firerate;
		}
	}
	if (!orderslist.left && !orderslist.right) {
		if (xvel > xaccrelease) {
			xvel -= xaccrelease;
		} else if (xvel < -xaccrelease) {
			xvel += xaccrelease;
		} else {
			xvel = 0;
		}
	}
}

void CharMario::GrowUp(int id) {
	SimpleAudioEngine::sharedEngine()->playEffect("grow.mp3");
	if (level == 1) {
		level = 2;
	} else {
		if (id == 3) {
			level = 3;
		} else if (id == 4) {
			level = 4;
		}
	}
}

void CharMario::GetHurt() {
	//return;
	//伤害判定
	if (immuneTime > 0) {
		return;
	}
	if (level >= 2) {
		SimpleAudioEngine::sharedEngine()->playEffect("pipe.mp3");
		level = 1;
		immuneTime = 180;
	} else if (level == 1) {
		GetDied();
	}
}

void CharMario::GetDied() {
    
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo minfo;
    bool isHave = JniHelper::getStaticMethodInfo(minfo,"com/duck/katong/DuckHuntSuper","showMyAds", "()V");
    if (isHave) {
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID);
        //CCLog("00000000000000000000000000");
        
    }else{
        //CCLog("00000000000000000000000000000000000000000000000000");
    }
#endif
    
	SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
	SimpleAudioEngine::sharedEngine()->playEffect("playerdied.mp3");
	_themap->gaming = false;
	died = true;
	CCAction * action = CCSequence::create(CCDelayTime::create(0.5f),
		CCJumpTo::create(1, ccp(this->getPositionX(), - _themap->getPositionY() - 50), 200, 1),
		CCDelayTime::create(2.f),
		CCCallFunc::create(this, callfunc_selector(CharMario::GetRestarted)),
		NULL);
	this->runAction(action);
}

void CharMario::GetPass() {
	xvel = 1;
	_themap->gaming = false;
	if (_themap->automove) {
		_themap->runAction(CCMoveTo::create(2, ccp(CCDirector::sharedDirector()->getVisibleSize().width - _themap->xMap->boundingBox().getMaxX(), _themap->getPositionY())));
	}
	CCAction * action = CCSequence::create(CCMoveTo::create(3, ccp(_themap->destx + 560, this->getPositionY())),
		CCFadeOut::create(0.5f),
		CCDelayTime::create(3.f),
		CCCallFunc::create(this, callfunc_selector(CharMario::GetPass2)),
		NULL);
	this->runAction(action);
	SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
	SimpleAudioEngine::sharedEngine()->playEffect("passed.mp3");
}

void CharMario::GetPass2() {
	_themap->_thescene->mariolevel = level;
	_themap->_thescene->mariofirerate = firerate;
	_themap->_thescene->NextLevel();
}

void CharMario::GetRestarted() {
	_themap->_thescene->Restarts();
}

void CharMario::Movement() {
	if (immuneTime > 0) {
		immuneTime--;
		if (immuneTime <= 0) {
			immuneTime = 0;
		}
		if (immuneTime % 4 == 0 || immuneTime % 4 == 1) {
			this->setVisible(true);
		} else {
			this->setVisible(false);
		}
	} else {
		this->setVisible(true);
	}
	if (level == 1 || downed) {
		JudgePoint.setRect(this->getPositionX() - 12, this->getPositionY() - 29, 24, 28);
	} else if (level >= 2) {
		JudgePoint.setRect(this->getPositionX() - 12, this->getPositionY() - 29, 24, 58);
	}
	xCheckInfo xcheckinfo = _themap->CheckX(JudgePoint.getMaxY(), JudgePoint.getMinY(), JudgePoint.getMinX() + xvel, JudgePoint.getMaxX() + xvel);
	
	if (_themap->automove && this->getPositionX() < 16 - _themap->getPositionX()) {
		this->setPositionX(16 - _themap->getPositionX());
		if (xvel < 0) {
			xvel = 0;
		}
		if (xcheckinfo.crashDirection >= 2) {
			GetDied();
		}
	}
	if (_themap->automove && this->getPositionX() > 784 - _themap->getPositionX()) {
		this->setPositionX(784 - _themap->getPositionX());
		if (xvel > 0) {
			xvel = 0;
		}
	}

	if (xcheckinfo.crashDirection == 0) {//如果水平无碰撞
		this->setPositionX(this->getPositionX() + xvel);
		if (_themap->fixedcamera && this->getPositionX() <= 16 - _themap->getPositionX()) {
			this->setPositionX(16 - _themap->getPositionX());
			xvel = 0;
		}
	} else if (xcheckinfo.crashDirection == 1) {//如果左侧碰撞
		this->setPositionX(xcheckinfo.leftX + JudgePoint.size.width / 2 + 2.f);//+2.f为防止纵向碰撞判断到右侧重叠的瓦片
		if (xvel < 0) {
			xvel = 0;
		}
	} else if (xcheckinfo.crashDirection == 2) {
		this->setPositionX(xcheckinfo.rightX - JudgePoint.size.width / 2 - 2.f);//-2.f为防止纵向碰撞判断到左侧重叠的瓦片
		if (xvel > 0) {
			xvel = 0;
		}
	}
	
	if (level == 1 || downed) {
		JudgePoint.setRect(this->getPositionX() - 12, this->getPositionY() - 29, 24, 28);
	} else if (level >= 2) {
		JudgePoint.setRect(this->getPositionX() - 12, this->getPositionY() - 29, 24, 58);
	}
	yCheckInfo ycheckinfo = _themap->CheckY(JudgePoint.getMaxY() + yvel, JudgePoint.getMinY() + yvel, JudgePoint.getMinX(), JudgePoint.getMaxX());

	if (ycheckinfo.crashDirection == 0) {
		this->setPositionY(this->getPositionY() + yvel);
		yvel--;
		if (yvel <= -15) {
			yvel = -15;
		}
		landed = false;
	} else if (ycheckinfo.crashDirection == 1) {
		if (level == 1 || downed) {
			this->setPositionY(ycheckinfo.upY - 2.f);//-2.f为防止再侦测到上判断
		} else {
			this->setPositionY(ycheckinfo.upY - 29 - 2.f);//-2.f为防止再侦测到上判断
		}
		if (yvel > 0) {
			yvel = 0;
		}
		landed = false;
	} else if (ycheckinfo.crashDirection == 2) {
		this->setPositionY(ycheckinfo.downY + 29);//没有修正是为了确保探测到地面
		if (yvel < 0) {
			yvel = 0;
		}
		landed = true;
	}
	if (ycheckinfo.crashDirection == 3) {
		yvel = 0;
		landed = true;
	}

	if (this->getPositionY() + _themap->getPositionY() < -50) {
		//yvel = 20;
		GetDied();
	}
	if (this->getPositionX() > _themap->destx && _themap->getPositionY() == _themap->destmy && !bossfighting && _themap->boss) {
		BossFight();
	}
	if (this->getPositionX() > _themap->destx && _themap->getPositionY() == _themap->destmy && landed && !bossfighting && !_themap->boss) {
		GetPass();
	}

	if (_themap->_thescene->levela == 4 && _themap->_thescene->levelb == 4) {
		if (this->getPositionX() >= 2464 && this->getPositionX() <= 2496 && this->getPositionY() >= 64 && this->getPositionY() <= 160) {
			this->setPositionY(this->getPositionY() + 800);
			_themap->setPositionY(-800);
		}
		if (this->getPositionX() >= 1120 && this->getPositionX() <= 1152 && this->getPositionY() >= 864 && this->getPositionY() <= 1184) {
			this->setPositionX(this->getPositionX() + 1856);
		}
		if (this->getPositionX() >= 4320 && this->getPositionX() <= 4352 && this->getPositionY() >= 864 && this->getPositionY() <= 960) {
			this->setPositionX(this->getPositionX() - 1856);
		}
		if (this->getPositionX() >= 4320 && this->getPositionX() <= 4352 && this->getPositionY() >= 1088 && this->getPositionY() <= 1184) {
			this->setPositionX(this->getPositionX() - 1856);
			this->setPositionY(this->getPositionY() - 800);
			_themap->setPositionY(0);
		}
	}
	
	if (_themap->_thescene->levela == 5 && _themap->_thescene->levelb == 3) {
		if (this->getPositionX() >= 3008 && this->getPositionX() <= 3040 && this->getPositionY() >= 1440 && this->getPositionY() <= 2080) {
			this->setPositionY(this->getPositionY() - 800);
			_themap->setPositionY(-800);
		}
		if (this->getPositionX() >= 608 && this->getPositionX() <= 640 && this->getPositionY() >= 640 && this->getPositionY() <= 1280) {
			this->setPositionX(this->getPositionX() + 2400);
		}
		if (this->getPositionX() >= 5408 && this->getPositionX() <= 5440 && this->getPositionY() >= 640 && this->getPositionY() <= 1280) {
			this->setPositionX(this->getPositionX() - 2400);
		}
	}
}

void CharMario::PlayAnimation() {
	if (level == 1) {
		if (xvel != 0) {
			aniplays = 0;
		} else {
			aniplays = 1;
		}
		if (!landed) {
			aniplays = 2;
		}
	}
	if (level == 2) {
		if (xvel != 0) {
			aniplays = 4;
		} else {
			aniplays = 5;
		}
		if (!landed) {
			aniplays = 6;
		}
		if (downed) {
			aniplays = 7;
		}
	}
	if (level == 3) {
		if (xvel != 0) {
			aniplays = 8;
		} else {
			aniplays = 9;
		}
		if (!landed) {
			aniplays = 10;
		}
		if (downed) {
			aniplays = 11;
		}
		if (firecount >= firerate - 2) {
			aniplays = 12;
		}
	}
	if (level == 4) {
		if (xvel != 0) {
			aniplays = 13;
		} else {
			aniplays = 14;
		}
		if (!landed) {
			aniplays = 15;
		}
		if (downed) {
			aniplays = 16;
		}
	}
	if (died) {
		aniplays = 3;
	}
	if (xvel > 0) {
		this->setRotationY(0);
	} else if (xvel < 0) {
		this->setRotationY(180);
	}
	if (nowplaying != aniplays) {
		this->getAnimation()->playByIndex(aniplays);
		nowplaying = aniplays;
	}
}

void CharMario::onUpdate(float dt) {
	PlayAnimation();
	if (!died && _themap->gaming) {
		JudgeMovement();
		Movement();
	}
	if (firecount > 0) {
		firecount--;
	} else {
		firecount = 0;
	}
}