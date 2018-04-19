#include "CharBoss.h"
#include "CharMario.h"
#include "CharBullet.h"

CharBoss * CharBoss::create(const char * pFileName, GameMap * gameMap) {
	do {
		CharBoss * pBoss = new CharBoss;
		CC_BREAK_IF(pBoss == NULL);

		pBoss->init(pFileName, gameMap);
		pBoss->autorelease();
		return pBoss;
	} while (false);
	return NULL;
}

bool CharBoss::init(const char * pFileName, GameMap * gameMap) {
	do {
		CC_BREAK_IF(CCArmature::init(pFileName) == false);

		_themap = gameMap;
		xvelmax = 4;
		xvel = 0;
		yvel = 0;
		nowplaying = 1;
		aniplays = 0;
		_id = 0;
		anid = 0;
		hp1 = 100;
		hp2 = 100;
		_mapposy = _themap->getPositionY();
		landed = false;
		died = false;
		fire = false;
		immuneTime = 0;
		_themap->_thescene->Hp1->setTotalProgress(hp1);
		_themap->_thescene->Hp1->setCurrentProgress(hp1);
		_themap->_thescene->Hp2->setTotalProgress(hp2);
		_themap->_thescene->Hp2->setCurrentProgress(hp2);

		if (strcmp(pFileName, "koppa") == 0) {
			_id = 0;
			this->setPosition(ccp(13400, 480));
			_themap->_thescene->Hp1->setVisible(true);
			_themap->_thescene->Hp1->setPosition(ccp(600, 440));
			_themap->_thescene->Hp2->setVisible(true);
			_themap->_thescene->Hp2->setPosition(ccp(380, 180));
		}
		if (strcmp(pFileName, "brain") == 0) {
			_id = 1;
			hp2 = 1;
			this->setPosition(ccp(8192, 320));
			_themap->_thescene->Hp1->setVisible(true);
			_themap->_thescene->Hp1->setPosition(ccp(600, 400));
			_themap->_thescene->Hp2->setVisible(true);
			_themap->_thescene->Hp2->setPosition(ccp(400, 140));
		}
		crack1 = CCSprite::createWithSpriteFrameName("kpbridge1.png");
		crack1->setAnchorPoint(ccp(0, 0));
		crack1->setPosition(ccp(12672, 192));
		_themap->addChild(crack1, 3000);
		crack1->setVisible(false);

		crack2 = CCSprite::createWithSpriteFrameName("kpbridge2.png");
		crack2->setAnchorPoint(ccp(0, 0));
		crack2->setPosition(ccp(12672, 192));
		_themap->addChild(crack2, 3000);
		crack2->setVisible(false);

		this->schedule(schedule_selector(CharBoss::onUpdate));

		return true;
	} while (false);
	return false;
}

void CharBoss::Movement() {
	JudgePoint.setRect(this->getPositionX() - 32, this->getPositionY() - 36, 64, 72);
	xCheckInfo xcheckinfo;
	xcheckinfo = _themap->CheckX(JudgePoint.getMaxY(), JudgePoint.getMinY(), JudgePoint.getMinX() + xvel, JudgePoint.getMaxX() + xvel);
	
	if (xcheckinfo.crashDirection == 0) {//如果水平无碰撞
		this->setPositionX(this->getPositionX() + xvel);
	} else if (xcheckinfo.crashDirection == 1) {//如果左侧碰撞
		this->setPositionX(xcheckinfo.leftX + JudgePoint.size.width / 2 + 2.f);//+2.f为防止纵向碰撞判断到右侧重叠的瓦片
		xvel = 0;
	} else if (xcheckinfo.crashDirection == 2) {
		this->setPositionX(xcheckinfo.rightX - JudgePoint.size.width / 2 - 2.f);//-2.f为防止纵向碰撞判断到左侧重叠的瓦片
		xvel = 0;
	}
	
	JudgePoint.setRect(this->getPositionX() - 32, this->getPositionY() - 36, 64, 72);
	yCheckInfo ycheckinfo;
	ycheckinfo = _themap->CheckY(JudgePoint.getMaxY() + yvel, JudgePoint.getMinY() + yvel, JudgePoint.getMinX(), JudgePoint.getMaxX());

	if (ycheckinfo.crashDirection == 0) {
		this->setPositionY(this->getPositionY() + yvel);
		yvel--;
		if (yvel <= -15) {
			yvel = -15;
		}
		landed = false;
	} else if (ycheckinfo.crashDirection == 1) {
		this->setPositionY(ycheckinfo.upY - JudgePoint.size.height / 2 - 4.f);//-2.f为防止再侦测到上判断
		yvel = 0;
		landed = false;
	} else if (ycheckinfo.crashDirection == 2) {
		if (_id == 0 && yvel < 0 && this->getPositionX() + _themap->getPositionX() > 64 && this->getPositionX() + _themap->getPositionX() < 704) {
			SimpleAudioEngine::sharedEngine()->playEffect("blocksmash.mp3");
			hp2 -= 20;
			_themap->_thescene->Hp2->setCurrentProgress(hp2);
			if (hp2 <= 60) {
				crack1->setVisible(true);
			}
			if (hp2 <= 20) {
				crack2->setVisible(true);
			}
			if (hp2 <= 0) {
				hp2 = 0;
				GetDie();
			}
			CCScene * f = CCDirector::sharedDirector()->getRunningScene();
			CCMoveBy * left1 = CCMoveBy::create(0.05f, ccp(5, 0));
			CCMoveBy * right1 = CCMoveBy::create(0.05f, ccp(-5, 0));
			CCMoveBy * top1 = CCMoveBy::create(0.05f, ccp(0, 5));
			CCMoveBy * rom1 = CCMoveBy::create(0.05f, ccp(0, -5));
			CCAction * action = CCSequence::create(left1, right1, top1, rom1, left1->reverse(), right1->reverse(), top1->reverse(), rom1->reverse(), NULL);
			f->runAction(action);
		}

		this->setPositionY(ycheckinfo.downY + JudgePoint.size.height / 2);//没有修正是为了确保探测到地面
		yvel = 0;
		landed = true;
	}
	if (ycheckinfo.crashDirection == 3) {
		yvel = 0;
		landed = true;
	}

	if (this->getPositionY() + _mapposy < -50) {
		GetRemoved();
	}
}

void CharBoss::CheckMario() {
	CharMario * pMario = _themap->pMario;
	CCRect MarioJP = pMario->JudgePoint;

	if (pMario->died) {
		return;
	}
	
	if (_id == 1) {	
		JudgePoint.setRect(this->getPositionX() - 80, this->getPositionY() - 64, 20, 128);
	}
	
	CCObject * pObj = NULL;
	CCArray * pArray = _themap->getBullets();

	CCARRAY_FOREACH(pArray, pObj) {
		CharBullet * pBullet = (CharBullet *)pObj;
		CCRect BulletJP = pBullet->JudgePoint;
		if (pBullet->hurtenemies) {
			if (!(BulletJP.getMaxX() < JudgePoint.getMinX() + 5 || JudgePoint.getMaxX() - 5 < BulletJP.getMinX() || BulletJP.getMaxY() < JudgePoint.getMinY() || JudgePoint.getMaxY() < BulletJP.getMinY())) { //子弹与我碰撞了
				GetHurt();
				pBullet->GetDied();
			}
		}
	}
	if (_id == 0) {
		if (!(MarioJP.getMaxX() < JudgePoint.getMinX() + 5 || JudgePoint.getMaxX() - 5 < MarioJP.getMinX() || MarioJP.getMaxY() < JudgePoint.getMinY() || JudgePoint.getMaxY() < MarioJP.getMinY())) { //玛丽与我碰撞了
			pMario->GetHurt();
		}
	}
}

void CharBoss::PlayBrain() {
	CharMario * pMario = _themap->pMario;
	if (pMario->level < 3) {
		if (hp2 < 100) {
			anid++;
		}
		if (anid >= 5) {
			anid = 0;
			hp2++;
		}
		if (hp2 >= 100) {
			if (_themap->_thescene->levela >= 6) {
				_themap->setMisc(4, 0, 7920, 176, NULL);
			} else {
				_themap->setMisc(3, 0, 7920, 176, NULL);
			}
			hp2 = 1;
		}
	} else {
		anid = 0;
		hp2 = 1;
	}
	_themap->_thescene->Hp2->setCurrentProgress(hp2);
}

void CharBoss::PlayKoppa() {
	CharMario * pMario = _themap->pMario;
	if (pMario->getPositionX() + 12 <= this->getPositionX()) {
		xvel -= 0.1f;
		if (xvel <= -xvelmax) {
			xvel = -xvelmax;
		}
	} else {
		xvel += 0.1f;
		if (xvel >= xvelmax) {
			xvel = xvelmax;
		}
	}
	
	anid++;
	if (anid % 50 == 5) {
		fire = true;
	} else if (anid % 150 == 25) {
		SimpleAudioEngine::sharedEngine()->playEffect("firechain.mp3");
		CharBullet * pBullet;
		pBullet = CharBullet::create(0, this->getRotationY() == 180, true, _themap, this->getPositionX(), this->getPositionY() + 10, 0);
		pBullet->setPosition(this->getPositionX(), this->getPositionY() + 10);
		_themap->getBullets()->addObject(pBullet);
		_themap->addChild(pBullet, 100000);
	} else if (anid % 150 == 75) {
		SimpleAudioEngine::sharedEngine()->playEffect("firechain.mp3");
		CharBullet * pBullet;
		pBullet = CharBullet::create(0, this->getRotationY() == 180, true, _themap, this->getPositionX(), this->getPositionY() + 10, 0);
		pBullet->setPosition(this->getPositionX(), this->getPositionY() + 10);
		_themap->getBullets()->addObject(pBullet);
		_themap->addChild(pBullet, 100000);
	} else if (anid % 150 == 80) {
		SimpleAudioEngine::sharedEngine()->playEffect("firechain.mp3");
		CharBullet * pBullet;
		pBullet = CharBullet::create(0, this->getRotationY() == 180, true, _themap, this->getPositionX(), this->getPositionY() + 10, 0);
		pBullet->setPosition(this->getPositionX(), this->getPositionY() + 10);
		_themap->getBullets()->addObject(pBullet);
		_themap->addChild(pBullet, 100000);
	} else if (anid % 150 == 125) {
		SimpleAudioEngine::sharedEngine()->playEffect("firechain.mp3");
		CharBullet * pBullet;
		pBullet = CharBullet::create(2, this->getRotationY() == 180, true, _themap, this->getPositionX(), this->getPositionY() + 10, 0);
		pBullet->setPosition(this->getPositionX(), this->getPositionY() + 10);
		_themap->getBullets()->addObject(pBullet);
		_themap->addChild(pBullet, 100000);
	} else if (anid % 150 == 145 && _themap->_thescene->levela >= 7) {
		SimpleAudioEngine::sharedEngine()->playEffect("throw.mp3");
		CharBullet * pBullet;
		pBullet = CharBullet::create(5, true, true, _themap, this->getPositionX(), this->getPositionY() + 10, 0);
		pBullet->setPosition(this->getPositionX(), this->getPositionY() + 10);
		_themap->getBullets()->addObject(pBullet);
		_themap->addChild(pBullet, 100000);
	} else if (anid % 50 == 35) {
		fire = false;
	}
	if (anid == 300) {
		yvel = 20;
	}
	if (anid >= 300) {
		anid = 0;
	}
}

void CharBoss::GetPass() {
	_themap->_thescene->Hp1->setVisible(false);
	_themap->_thescene->Hp2->setVisible(false);
	CharMario * pMario = _themap->pMario;
	pMario->GetPass2();
}

void CharBoss::GetDie() {
	SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
	SimpleAudioEngine::sharedEngine()->playEffect("passed.mp3");
	crack1->setVisible(false);
	crack2->setVisible(false);
	died = true;
	if (hp2 <= 0) {
		for (int i = 0; i < 22; i++) {
			CCPoint loc;
			loc = ccp(396 + i, 13);
			CCTMXLayer * pLandLayer = _themap->xMap->layerNamed("land");
			pLandLayer->removeTileAt(loc);
		}
		CCAction * action = CCSequence::create(CCJumpTo::create(0.5f, ccp(this->getPositionX(), -_mapposy - 100), 0, 1),
			CCDelayTime::create(5.f),
			CCCallFunc::create(this, callfunc_selector(CharBoss::GetPass)),
			NULL);
		this->runAction(action);
	} else if (hp1 <= 0) {
		CCAction * action = CCSequence::create(CCJumpTo::create(0.5f, ccp(this->getPositionX(), -_mapposy - 50), 200, 1),
			CCDelayTime::create(5.f),
			CCCallFunc::create(this, callfunc_selector(CharBoss::GetPass)),
			NULL);
		this->runAction(action);
	}
	if (_id == 1) {
		_themap->gaming = false;
	}
}

void CharBoss::GetHurt() {
	if (immuneTime > 0) {
		return;
	}
	SimpleAudioEngine::sharedEngine()->playEffect("shell.mp3");
	hp1 -= 20;
	_themap->_thescene->Hp1->setCurrentProgress(hp1);
	if (hp1 <= 0) {
		hp1 = 0;
		GetDie();
	} else {
		immuneTime = 180;
	}
}

void CharBoss::GetRemoved() {
	_themap->removeChild(this);
}

void CharBoss::PlayAnimation() {
	immuneTime--;
	if (immuneTime <= 0) {
		immuneTime = 0;
	}
	if (immuneTime % 4 <= 1) {
		this->setVisible(true);
	} else {
		this->setVisible(false);
	}
	if (_id == 0) {
		aniplays = 0;
		if (xvel != 0) {
			aniplays = 1;
		}
		if (fire) {
			aniplays = 2;
		}
		if (xvel > 0) {
			this->setRotationY(180);
		} else if (xvel < 0) {
			this->setRotationY(0);
		}
	} else if (_id == 1) {
		if (hp1 >= 90) {
			aniplays = 0;
		} else if (hp1 >= 80) {
			aniplays = 1;
		} else if (hp1 >= 70) {
			aniplays = 2;
		} else if (hp1 >= 60) {
			aniplays = 3;
		} else if (hp1 >= 50) {
			aniplays = 4;
		} else {
			aniplays = 5;
		}
	}
	if (hp1 <= 0) {
		this->setRotationX(0);
		this->setRotationY(0);
		this->setRotation(this->getRotation() + 20);
	}
	if (nowplaying != aniplays) {
		this->getAnimation()->playByIndex(aniplays);
		nowplaying = aniplays;
	}
}

void CharBoss::onUpdate(float dt) {
	if (!died && _themap->gaming) {
		CheckMario();
		if (_id == 0) {
			Movement();
			PlayKoppa();
		} else if (_id == 1) {
			PlayBrain();
		}
	}
	PlayAnimation();
}