#include "CharBowser.h"
#include "CharMario.h"
#include "CharMisc.h"
#include "CharBullet.h"

CharBowser * CharBowser::create(GameMap * gameMap) {
	do {
		CharBowser * pBowser = new CharBowser;
		CC_BREAK_IF(pBowser == NULL);

		pBowser->init(gameMap);
		pBowser->autorelease();
		return pBowser;
	} while (false);
	return NULL;
}

bool CharBowser::init(GameMap * gameMap) {
	do {
		CC_BREAK_IF(CCArmature::init("bowser") == false);

		_themap = gameMap;
		xvel = 0;
		yvel = -1;
		yvelacc = 0;
		anid = 0;
		hp1 = 100;
		hp2 = 1;
		frames = 0;
		nowplaying = 1;
		aniplays = 0;
		playid = 0;
		immuneTime = 0;
		died = false;
		left = true;
		turning = false;
		pLandLayer = _themap->xMap->layerNamed("land");

		this->getAnimation()->playByIndex(0);

		this->schedule(schedule_selector(CharBowser::onUpdate));

		return true;
	} while (false);
	return false;
}

void CharBowser::Movement() {
	if (yvel < 0 && this->getPositionY() < _themap->xMap->boundingBox().getMaxY()) {
		CCPoint bowp;
		bowp.x = (int)(this->getPositionX() / 32);
		bowp.y = (int)((_themap->xMap->boundingBox().getMaxY() - this->getPositionY() + 4 + yvel) / 32) + 1;
		int lp = pLandLayer->tileGIDAt(bowp);
		if (lp > 0) {
			yvel = 0;
			yvelacc = 0;
			this->setPositionY(pLandLayer->tileAt(bowp)->boundingBox().getMaxY() + 36);
		}

		bowp.x = (int)(this->getPositionX() / 32) - 1;
		lp = pLandLayer->tileGIDAt(bowp);
		if (lp > 0) {
			yvel = 0;
			yvelacc = 0;
			this->setPositionY(pLandLayer->tileAt(bowp)->boundingBox().getMaxY() + 36);
		}
	}

	this->setPositionX(this->getPositionX() + xvel);
	this->setPositionY(this->getPositionY() + yvel);
	yvel += yvelacc;
	JudgePoint.setRect(this->getPositionX() - 32, this->getPositionY() - 40, 64, 80);

	if (this->getPositionY() + _themap->getPositionY() < -50) {
		GetRemoved();
	}
}

void CharBowser::CheckMario() {
	CharMario * pMario = _themap->pMario;
	CCRect MarioJP = pMario->JudgePoint;
	
	if (pMario->died) {
		return;
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
	if (!(MarioJP.getMaxX() < JudgePoint.getMinX() + 5 || JudgePoint.getMaxX() - 5 < MarioJP.getMinX() || MarioJP.getMaxY() < JudgePoint.getMinY() || JudgePoint.getMaxY() < MarioJP.getMinY())) { //玛丽与我碰撞了
		pMario->GetHurt();
	}
}

void CharBowser::GetHurt() {
	if (immuneTime > 0) {
		return;
	}
	SimpleAudioEngine::sharedEngine()->playEffect("shell.mp3");
	hp1 -= 7;
	_themap->_thescene->Hp1->setCurrentProgress(hp1);
	if (hp1 <= 0) {
		hp1 = 0;
		GetDie();
	} else {
		immuneTime = 180;
	}
}

void CharBowser::GetPass() {
	_themap->_thescene->Hp1->setVisible(false);
	_themap->_thescene->Hp2->setVisible(false);
	_themap->_thescene->GetPass();
}

void CharBowser::GetDie() {
	died = true;
	frames = 0;
	CCAction * action = CCSequence::create(CCJumpTo::create(0.5f, ccp(this->getPositionX(), -100), 0, 1),
		CCDelayTime::create(2.f),
		CCCallFunc::create(this, callfunc_selector(CharBowser::GetPass)),
		NULL);
	this->runAction(action);
	_themap->gaming = false;
}

void CharBowser::GetRemoved() {
	_themap->removeChild(this);
}

void CharBowser::BurstCoin() {
	frames++;
	if (frames < 3) {
		return;
	}
	CharMisc * pMisc;
	pMisc = CharMisc::create(1, 1, NULL, _themap);
	pMisc->setPosition(this->getPositionX(), 0);
	pMisc->xvel = 5 - rand() % 10;
	pMisc->xvelfixed = fabs(pMisc->xvel);
	pMisc->yvel = rand() % 20 + 12;
	_themap->addChild(pMisc, 1000000);
	frames = 0;
}

void CharBowser::PlayAnimation() {
	immuneTime--;
	if (immuneTime <= 0) {
		immuneTime = 0;
	}
	if (immuneTime % 4 <= 1) {
		this->setVisible(true);
	} else {
		this->setVisible(false);
	}
	if (left) {
		this->setRotationY(0);
	} else {
		this->setRotationY(180);
	}
	if (died) {
		this->setRotationX(0);
		this->setRotationY(0);
		this->setRotation(this->getRotation() + 20);
	}
	if (nowplaying != aniplays) {
		this->getAnimation()->playByIndex(aniplays);
		nowplaying = aniplays;
	}
}

void CharBowser::Playing() {
	if (playid == 1) {
		if (frames == 60) {
			aniplays = 1;
		}
		if (frames == 70) {
			aniplays = 0;
			MakeFire(0);
		}
		if (frames == 100) {
			playid = 0;
		}
	} else if (playid == 2) {
		if (frames == 60) {
			aniplays = 1;
		}
		if (frames == 70) {
			aniplays = 0;
			MakeFire(0);
		}
		if (frames == 110) {
			aniplays = 1;
		}
		if (frames == 120) {
			aniplays = 0;
			MakeFire(0);
			MakeFire(20);
			MakeFire(-20);
		}
		if (frames == 150) {
			playid = 0;
		}
	} else if (playid == 3) {
		CharMario * pMario = _themap->pMario;
		int mx = pMario->getPositionX();

		if (frames == 20) {
			int dy = 448 - this->getPositionY();
			int aimx = (int)(mx / 32) * 32;
			if (aimx == 0) {
				aimx = 32;
			}
			int dx = aimx - this->getPositionX();
			yvel = dy * 1.f / 30;
			xvel = dx * 1.f / 60;
			yvelacc = -dy * 1.f / 1800;
			aniplays = 2;
		}
		if (frames == 80) {
			this->setPositionX((int)((this->getPositionX() + 2) / 32) * 32);
			xvel = 0;
			yvel = -1;
			yvelacc = -0.5f;
			aniplays = 3;
		}
		if (yvel == 0 && frames > 80 && frames < 1000) {
			yvelacc = 0;
			aniplays = 4;
			frames = 1000;
			left = this->getPositionX() > mx;

			CCPoint bowp;
			bowp.x = (int)(this->getPositionX() / 32);
			bowp.y = (int)((_themap->xMap->boundingBox().getMaxY() - this->getPositionY() + 4 + yvel) / 32) + 1;
			int lp = pLandLayer->tileGIDAt(bowp);
			if (lp == 16) {
				SimpleAudioEngine::sharedEngine()->playEffect("smash.mp3");
				yvel = -1;
				pLandLayer->removeTileAt(bowp);
			}

			bowp.x = (int)(this->getPositionX() / 32) - 1;
			lp = pLandLayer->tileGIDAt(bowp);
			if (lp == 16) {
				SimpleAudioEngine::sharedEngine()->playEffect("smash.mp3");
				yvel = -1;
				pLandLayer->removeTileAt(bowp);
			}

			if (bowp.y > 13) {
				aniplays = 3;
				GetDie();
			}
		}
		if (frames == 1030) {
			aniplays = 0;
			playid = 0;
		}
	} else if (playid == 0 && !turning) {
		srand((unsigned)time(0));
		frames = 0;
		playid = rand() % 4 + 1;
		if (playid > 3) {
			playid = 3;
		}
	} 
	if (playid > 0) {
		frames++;
	}
}


void CharBowser::MakeFire(int addAngle) {
	SimpleAudioEngine::sharedEngine()->playEffect("firechain.mp3");
	CharBullet * pBullet;
	pBullet = CharBullet::create(6, !left, true, _themap, this->getPositionX(), this->getPositionY() + 10, 0);
	pBullet->setPosition(this->getPositionX(), this->getPositionY() + 10);
	CharMario * pMario = _themap->pMario;
	int dx = this->getPositionX();
	int dy = this->getPositionY() + 10;
	int mx = pMario->getPositionX();
	int my = pMario->getPositionY();
	float angle = atan((dy - my) * 1.f / (dx - mx));
	angle += addAngle * 3.141f / 180;
	float cosx = cos(angle);
	float sinx = sin(angle);
	if (mx < dx) {
		cosx = -cosx;
		sinx = -sinx;
	}
	pBullet->xvel = 4 * cosx;
	pBullet->yvel = 4 * sinx;
	_themap->getBullets()->addObject(pBullet);
	_themap->addChild(pBullet, 100000);
}

void CharBowser::TestAndTurn() {
	if (playid == 3) {
		return;
	}

	CharMario * pMario = _themap->pMario;

	if (turning) {
		frames++;
		aniplays = 5;
		if (frames == 25) {
			aniplays = 0;
			left = !left;
			turning = false;
		}
	} else {
		int mx = pMario->getPositionX();
		if ((left && mx > this->getPositionX()) || (!left && mx < this->getPositionX())) {
			turning = true;
			frames = 0;
			playid = 0;
		}
	}
}

void CharBowser::UpdateItem() {
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
			if (pMario->level == 1) {
				_themap->setMisc(2, 0, 48, 272, NULL);
			} else {
				_themap->setMisc(4, 0, 48, 272, NULL);
			}
			hp2 = 1;
		}
	} else {
		anid = 0;
		hp2 = 1;
	}
	_themap->_thescene->Hp2->setCurrentProgress(hp2);
}

void CharBowser::onUpdate(float dt) {
	if (_themap->gaming && !died) {
		Movement();
		CheckMario();
		UpdateItem();
		TestAndTurn();
		Playing();
	}
	if (died) {
		BurstCoin();
	}
	PlayAnimation();
}