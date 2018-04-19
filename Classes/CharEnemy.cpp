#include "CharEnemy.h"
#include "CharMario.h"
#include "CharBullet.h"

CharEnemy * CharEnemy::create(const char * pFileName, float dx, float dy, GameMap * gameMap) {
	do {
		CharEnemy * pEnemy = new CharEnemy;
		CC_BREAK_IF(pEnemy == NULL);

		pEnemy->init(pFileName, dx, dy, gameMap);
		pEnemy->autorelease();
		return pEnemy;
	} while (false);
	return NULL;
}

bool CharEnemy::init(const char * pFileName, float dx, float dy, GameMap * gameMap) {
	do {
		if (strcmp(pFileName, "fireround") == 0 || strcmp(pFileName, "fireballs2") == 0 || strcmp(pFileName, "burst") == 0 || strcmp(pFileName, "firegun") == 0) {
			CC_BREAK_IF(CCArmature::init("fireballs") == false);
		} else {
			CC_BREAK_IF(CCArmature::init(pFileName) == false);
		}

		_themap = gameMap;
		xvelfixed = 1;
		xvel = -xvelfixed;
		yvel = 0;
		nowplaying = 1;
		aniplays = 0;
		addy = 0;
		frames = 0;
		_id = 0;
		_dx = dx;
		_dy = dy;
		_mapposy = _themap->getPositionY();
		if (strcmp(pFileName, "badturt") == 0) {
			_id = 1;
			addy = 10;
		}
		if (strcmp(pFileName, "fireballs") == 0) {
			_id = 2;
			for (int i = 0; i < 8; i++) {
				CharBullet * pBullet;
				pBullet = CharBullet::create(1, true, true, _themap, dx, dy, -15 * i);
				pBullet->setPosition(dx, dy);
				_themap->getBullets()->addObject(pBullet);
				_themap->addChild(pBullet, 100000);
			}
		}
		if (strcmp(pFileName, "fireballs2") == 0) {
			_id = 2;
			for (int i = 0; i < 16; i++) {
				CharBullet * pBullet;
				pBullet = CharBullet::create(1, true, true, _themap, dx, dy, -15 * i);
				pBullet->setPosition(dx, dy);
				_themap->getBullets()->addObject(pBullet);
				_themap->addChild(pBullet, 100000);
			}
		}
		if (strcmp(pFileName, "riseball") == 0) {
			_id = 3;
			xvel = 0;
			yvel = 10;
		}
		if (strcmp(pFileName, "badflower") == 0) {
			_id = 4;
			xvel = 0;
			yvel = 0;
		}
		if (strcmp(pFileName, "fireround") == 0) {
			_id = 5;
		}
		if (strcmp(pFileName, "cannon") == 0) {
			_id = 6;
		}
		if (strcmp(pFileName, "hedgehog") == 0) {
			_id = 7;
		}
		if (strcmp(pFileName, "burst") == 0) {
			_id = 8;
		}
		if (strcmp(pFileName, "fireflower") == 0) {
			_id = 9;
			xvel = 0;
			yvel = 0;
		}
		if (strcmp(pFileName, "cloud") == 0) {
			_id = 10;
			yvel = 0;
			addy = 15;
		}
		if (strcmp(pFileName, "hedball") == 0) {
			_id = 11;
			xvel = 0;
			yvel = 10;
		}
		if (strcmp(pFileName, "hamturt") == 0) {
			_id = 12;
			addy = 10;
		}
		if (strcmp(pFileName, "gunturt") == 0) {
			_id = 13;
			addy = 10;
		}
		if (strcmp(pFileName, "flyfish") == 0) {
			_id = 14;
			srand((unsigned)time(0));
			xvel = rand() % 5 + 1;
			int mx = _themap->pMario->getPositionX();
			if (dx > mx) {
				xvel = -xvel;
			}
			yvel = rand() % 10 + 5;
		}
		if (strcmp(pFileName, "wheels") == 0 || strcmp(pFileName, "flight") == 0) {
			_id = 15;
			xvel = 0;
			yvel = 0;
		}
		if (strcmp(pFileName, "firegun") == 0) {
			_id = 16;
			xvel = 0;
			yvel = 0;
		}
		landed = false;
		treaden = false;
		shelled = false;
		shellpushed = false;
		died = false;
		growing = false;
		atk = false;

		if (_id != 2 || _id != 5 || _id != 6 || _id != 8) {
			_themap->getEnemies()->addObject(this);
		}

		this->schedule(schedule_selector(CharEnemy::onUpdate));

		return true;
	} while (false);
	return false;
}

void CharEnemy::CloudMove() {
	int mx = _themap->pMario->getPositionX();
	if (this->getPositionX() > mx + 4000) {
		this->setPositionX(mx + 4000);
	}
	if (this->getPositionX() < mx - 4000) {
		this->setPositionX(mx - 4000);
	}
	if (this->getPositionX() > mx) {
		xvel -= 0.2f;
		if (xvel < -9.5f) {
			xvel = -9.5f;
		}
	} else {
		xvel += 0.2f;
		if (xvel > 9.5f) {
			xvel = 9.5f;
		}
	}
	if (this->getPositionY() + _themap->getPositionY() > 0 && this->getPositionY() + _themap->getPositionY() < 480 && this->getPositionX() + _themap->getPositionX() > 0 && this->getPositionX() + _themap->getPositionX() < 800 && !atk) {
		frames++;
		if (frames == 100) {
			atk = true;
			frames = 0;
		}
	}
	if (atk) {
		frames++;
		if (frames >= 30) {
			frames = 0;
			atk = false;
			float dx = this->getPositionX();
			float dy = this->getPositionY();
			CharEnemy * pEnemy;
			pEnemy = CharEnemy::create("hedball", dx, dy, _themap);
			pEnemy->setPosition(dx, dy);
			_themap->addChild(pEnemy, 100000);
			srand((unsigned)time(0));
			int h = rand() % 2;
			if (h == 1) {
				SimpleAudioEngine::sharedEngine()->playEffect("taunt1.mp3");
			} else {
				SimpleAudioEngine::sharedEngine()->playEffect("taunt2.mp3");
			}
		}
	}
	this->setPositionX(this->getPositionX() + xvel);
}

void CharEnemy::FireGun() {
	int mx = _themap->pMario->getPositionX();
	if (this->getPositionY() + _themap->getPositionY() > 0 && this->getPositionY() + _themap->getPositionY() < 480 && this->getPositionX() + _themap->getPositionX() > 0 && this->getPositionX() + _themap->getPositionX() < 800) {
		frames++;
		if (frames >= 90) {
			SimpleAudioEngine::sharedEngine()->playEffect("gun.mp3");
			CharBullet * pBullet;
			pBullet = CharBullet::create(0, false, true, _themap, this->getPositionX() - 20, this->getPositionY() + 5, 0);
			pBullet->setPosition(this->getPositionX() - 20, this->getPositionY() + 5);
			_themap->getBullets()->addObject(pBullet);
			_themap->addChild(pBullet, 100000);
			frames = 0;
		}
	}
}

void CharEnemy::FireRounds() {
	int mx = _themap->pMario->getPositionX();
	if (this->getPositionY() + _themap->getPositionY() > 0 && this->getPositionY() + _themap->getPositionY() < 480 && this->getPositionX() + _themap->getPositionX() > 0 && this->getPositionX() + _themap->getPositionX() < 800) {
		frames++;
		if (frames >= 120) {
			CharBullet * pBullet;
			pBullet = CharBullet::create(3, true, true, _themap, this->getPositionX(), this->getPositionY(), 0);
			pBullet->setPosition(this->getPositionX(), this->getPositionY());
			_themap->getBullets()->addObject(pBullet);
			_themap->addChild(pBullet, 100000);
			frames = 0;
		}
	}
}

void CharEnemy::FireRocket() {
	int mx = _themap->pMario->getPositionX();
	if ((mx < this->getPositionX() - 48 || mx > this->getPositionX() + 48) && this->getPositionY() + _themap->getPositionY() > 0 && this->getPositionY() + _themap->getPositionY() < 480 && this->getPositionX() + _themap->getPositionX() > 0 && this->getPositionX() + _themap->getPositionX() < 800) {
		frames++;
		if (frames >= 120) {
			SimpleAudioEngine::sharedEngine()->playEffect("firechain.mp3");
			CharBullet * pBullet;
			pBullet = CharBullet::create(4, mx >= this->getPositionX(), true, _themap, this->getPositionX(), this->getPositionY(), 0);
			pBullet->setPosition(this->getPositionX(), this->getPositionY());
			_themap->getBullets()->addObject(pBullet);
			_themap->addChild(pBullet, 100000);
			frames = 0;
		}
	}
}

void CharEnemy::FireEnemies() {
	int mx = _themap->pMario->getPositionX();
	if ((mx < this->getPositionX() - 48 || mx > this->getPositionX() + 48) && this->getPositionY() + _themap->getPositionY() > 0 && this->getPositionY() + _themap->getPositionY() < 480 && this->getPositionX() + _themap->getPositionX() > 0 && this->getPositionX() + _themap->getPositionX() < 800) {
		frames++;
		if (frames >= 90) {
			srand((unsigned)time(0));
			CharEnemy * pEnemy;
			int d = rand() % 2;
			if (d == 0) {
				pEnemy = CharEnemy::create("badmush", this->getPositionX(), this->getPositionY() + 32, _themap);
			} else {
				pEnemy = CharEnemy::create("hedgehog", this->getPositionX(), this->getPositionY() + 32, _themap);
			}
			pEnemy->yvel = 13 + (rand() % 10) / 2;
			int h = rand() % 2;
			if (h == 1) {
				pEnemy->xvel = pEnemy->xvelfixed;
			}
			pEnemy->setPosition(this->getPositionX(), this->getPositionY() + 32);
			_themap->addChild(pEnemy, 100000);
			frames = 0;
		}
	}
}

void CharEnemy::TestGrow() {
	int mx = _themap->pMario->getPositionX();
	if (!growing) {
		if (mx < this->getPositionX() - 64 || mx > this->getPositionX() + 64) {
			FlowerGrow();
			frames = 0;
		}
	} else if (_id == 9) {
		frames++;
		if (frames == 140) {
			srand((unsigned)time(0));
			for (int i = 0; i < 5; i++) {
				CharBullet * pBullet;
				pBullet = CharBullet::create(0, mx >= this->getPositionX(), true, _themap, this->getPositionX(), this->getPositionY(), 1);
				pBullet->setPosition(this->getPositionX(), this->getPositionY());
				pBullet->xvel = 9 - (rand() % 30) / 2;
				pBullet->yvel = 10 + (rand() % 10) / 2;
				_themap->getBullets()->addObject(pBullet);
				_themap->addChild(pBullet, 100000);
			}
		}
	}
}

void CharEnemy::FlowerDown() {
	growing = false;
}

void CharEnemy::FlowerGrow() {
	growing = true;
	CCAction * action = CCSequence::create(CCMoveBy::create(2, ccp(0, 54)),
		CCDelayTime::create(2.f),
		CCMoveBy::create(2, ccp(0, -54)),
		CCDelayTime::create(3.f),
		CCCallFunc::create(this, callfunc_selector(CharEnemy::FlowerDown)),
		NULL);
	this->runAction(action);
}

void CharEnemy::Movement() {
	JudgePoint.setRect(this->getPositionX() - 14, this->getPositionY() - 14 - addy, 28, 28);
	if (shelled && !shellpushed) {
		return;
	}
	xCheckInfo xcheckinfo;
	if (_id == 12 || _id == 13) {
		frames++;
		if (frames % 120 == 0) {
			xvel = -xvel;
		}
		if (frames == 150) {
			atk = true;
		}
		if (frames == 180 && _id == 12) {
			atk = false;
			if (!(this->getPositionY() + _themap->getPositionY() > 0 && this->getPositionY() + _themap->getPositionY() < 480 && this->getPositionX() + _themap->getPositionX() > 0 && this->getPositionX() + _themap->getPositionX() < 800)) {
				return;
			}
			SimpleAudioEngine::sharedEngine()->playEffect("throw.mp3");
			CharBullet * pBullet;
			pBullet = CharBullet::create(5, true, true, _themap, this->getPositionX(), this->getPositionY(), 0);
			pBullet->setPosition(this->getPositionX(), this->getPositionY());
			_themap->getBullets()->addObject(pBullet);
			_themap->addChild(pBullet, 100000);
		}
		if ((frames == 180 || frames == 185 || frames == 190) && _id == 13) {
			if (frames == 190) {
				atk = false;
			}
			if (!(this->getPositionY() + _themap->getPositionY() > 0 && this->getPositionY() + _themap->getPositionY() < 480 && this->getPositionX() + _themap->getPositionX() > 0 && this->getPositionX() + _themap->getPositionX() < 800)) {
				return;
			}
			SimpleAudioEngine::sharedEngine()->playEffect("gun.mp3");
			CharBullet * pBullet;
			CharMario * pMario = _themap->pMario;
			int mx = pMario->getPositionX();
			pBullet = CharBullet::create(0, mx >= this->getPositionX(), true, _themap, this->getPositionX(), this->getPositionY(), 0);
			pBullet->setPosition(this->getPositionX(), this->getPositionY());
			_themap->getBullets()->addObject(pBullet);
			_themap->addChild(pBullet, 100000);
		}
		if (frames == 240) {
			frames = 0;
		}
	}

	if (_id == 3 || _id == 14) {
		xcheckinfo.crashDirection = 0;
	} else {
		xcheckinfo = _themap->CheckX(JudgePoint.getMaxY(), JudgePoint.getMinY(), JudgePoint.getMinX() + xvel, JudgePoint.getMaxX() + xvel);
	}
	
	if (xcheckinfo.crashDirection == 0) {//如果水平无碰撞
		this->setPositionX(this->getPositionX() + xvel);
	} else if (xcheckinfo.crashDirection == 1) {//如果左侧碰撞
		this->setPositionX(xcheckinfo.leftX + JudgePoint.size.width / 2 + 2.f);//+2.f为防止纵向碰撞判断到右侧重叠的瓦片
		xvel = xvelfixed;
	} else if (xcheckinfo.crashDirection == 2) {
		this->setPositionX(xcheckinfo.rightX - JudgePoint.size.width / 2 - 2.f);//-2.f为防止纵向碰撞判断到左侧重叠的瓦片
		xvel = -xvelfixed;
	}
	
	JudgePoint.setRect(this->getPositionX() - 14, this->getPositionY() - 14 - addy, 28, 28);
	yCheckInfo ycheckinfo;
	if (_id == 3 || _id == 14) {
		ycheckinfo.crashDirection = 0;
	} else {
		ycheckinfo = _themap->CheckY(JudgePoint.getMaxY() + yvel, JudgePoint.getMinY() + yvel, JudgePoint.getMinX(), JudgePoint.getMaxX());
	}

	if (ycheckinfo.crashDirection == 0) {
		this->setPositionY(this->getPositionY() + yvel);
		if (_id == 3 || _id == 14) {
			yvel -= 0.2f;
			if (yvel < -15) {
				yvel = 15;
			}
		} else {
			yvel--;
			if (yvel <= -15) {
				yvel = -15;
			}
		}
		landed = false;
	} else if (ycheckinfo.crashDirection == 1) {
		this->setPositionY(ycheckinfo.upY - JudgePoint.size.height / 2 - addy - 4.f);//-2.f为防止再侦测到上判断
		yvel = 0;
		landed = false;
	} else if (ycheckinfo.crashDirection == 2) {
		this->setPositionY(ycheckinfo.downY + JudgePoint.size.height / 2 + addy);//没有修正是为了确保探测到地面
		yvel = 0;
		landed = true;
	}
	if (ycheckinfo.crashDirection == 3) {
		yvel = 0;
		landed = true;
	}
	
	if (_id == 11 && landed) {
		float dx = this->getPositionX();
		float dy = this->getPositionY();
		CharEnemy * pEnemy;
		pEnemy = CharEnemy::create("hedgehog", dx, dy, _themap);
		pEnemy->setPosition(dx, dy);
		_themap->addChild(pEnemy, 100000);
		GetRemoved();
	}

	if (this->getPositionY() + _mapposy < -50 && _id != 3) {
		if (_id == 14) {
			this->setPosition(ccp(_dx, _dy - 32));
			srand((unsigned)time(0) + _dx);
			xvel = rand() % 5 + 1;
			int mx = _themap->pMario->getPositionX();
			if (_dx > mx) {
				xvel = -xvel;
			}
			yvel = rand() % 10 + 5;
			return;
		}
		GetRemoved();
	}
}

void CharEnemy::CheckMario() {
	CharMario * pMario = _themap->pMario;
	CCRect MarioJP = pMario->JudgePoint;
	if (_id == 4 || _id == 9) {
		JudgePoint.setRect(this->getPositionX() - 16, this->getPositionY() - 22, 32, 40);
	}
	if (_id == 10) {
		JudgePoint.setRect(this->getPositionX() - 16, this->getPositionY() - 48, 32, 32);
	}

	if (pMario->died) {
		return;
	}
	
	CCObject * pObj = NULL;
	CCArray * pArray = _themap->getShells();

	CCARRAY_FOREACH(pArray, pObj) {
		if (pObj != this) {
			CharEnemy * pEnemy = (CharEnemy *)pObj;
			CCRect ShellJP = pEnemy->JudgePoint;
			if (!(ShellJP.getMaxX() < JudgePoint.getMinX() + 5 || JudgePoint.getMaxX() - 5 < ShellJP.getMinX() || ShellJP.getMaxY() < JudgePoint.getMinY() || JudgePoint.getMaxY() < ShellJP.getMinY())) { //龟壳与我碰撞了
				if (pEnemy->shelled && pEnemy->shellpushed) {
					GetDied();
				}
			}
		}
	}
	
	pObj = NULL;
	pArray = _themap->getBullets();

	CCARRAY_FOREACH(pArray, pObj) {
		CharBullet * pBullet = (CharBullet *)pObj;
		CCRect BulletJP = pBullet->JudgePoint;
		if (pBullet->hurtenemies) {
			if (!(BulletJP.getMaxX() < JudgePoint.getMinX() + 5 || JudgePoint.getMaxX() - 5 < BulletJP.getMinX() || BulletJP.getMaxY() < JudgePoint.getMinY() || JudgePoint.getMaxY() < BulletJP.getMinY())) { //子弹与我碰撞了
				GetDied();
				pBullet->GetDied();
			}
		}
	}

	if (!(MarioJP.getMaxX() < JudgePoint.getMinX() + 5 || JudgePoint.getMaxX() - 5 < MarioJP.getMinX() || MarioJP.getMaxY() < JudgePoint.getMinY() || JudgePoint.getMaxY() < MarioJP.getMinY())) { //玛丽与我碰撞了
		if (_id == 3 || _id == 4 || _id == 7 || _id == 9 || _id == 11) {
			pMario->GetHurt();
			return;
		}
		if (MarioJP.getMinX() >= JudgePoint.getMinX() - 20 && MarioJP.getMaxX() <= JudgePoint.getMaxX() + 20 && MarioJP.getMinY() >= JudgePoint.getMaxY() - 20) { //踩到头上
			pMario->yvel = 10;
			pMario->landed = true;
			if (_id == 0 || _id == 10) {
				GetTread();
			} else if (_id == 12 || _id == 13 || _id == 14) {
				GetDied();
			} else if (_id == 1) {
				pMario->setPositionY(pMario->getPositionY() + 30);
				if (!shelled) {
					addy = 0;
					this->setPositionY(this->getPositionY() - 10);
					GetShell(MarioJP.getMinX() >= JudgePoint.getMinX());
					shellpushed = false;
				} else {
					GetShell(MarioJP.getMinX() >= JudgePoint.getMinX());
				}
			}
		} else {
			if (_id == 10) {
				return;
			}
			if (_id == 1) {
				if (shellpushed || !shelled) {
					pMario->GetHurt();
				} else {
					if (MarioJP.getMinX() >= JudgePoint.getMinX()) {
						this->setPositionX(this->getPositionX() - 9);
					} else {
						this->setPositionX(this->getPositionX() + 9);
					}
					GetShell(MarioJP.getMinX() >= JudgePoint.getMinX());
				}
			} else {
				pMario->GetHurt();
			}
		}
	}
}

void CharEnemy::GetShell(bool left) {
	xvelfixed = 9;
	xvel = 0;
	if (!shelled) {
		_themap->getShells()->addObject(this);
	}
	shelled = true;
	shellpushed = !shellpushed;
	if (shellpushed) {
		SimpleAudioEngine::sharedEngine()->playEffect("shell.mp3");
		xvel = xvelfixed;
		if (left) {
			xvel = -xvel;
		}
	} else {
		SimpleAudioEngine::sharedEngine()->playEffect("tread.mp3");
	}
}

void CharEnemy::GetDied() {
	if (_id == 10) {
		float dx = this->getPositionX() + 4000;
		float dy = this->getPositionY();
		CharEnemy * pEnemy;
		pEnemy = CharEnemy::create("cloud", dx, dy, _themap);
		pEnemy->setPosition(dx, dy);
		_themap->addChild(pEnemy, 100000);
	}
	SimpleAudioEngine::sharedEngine()->playEffect("shell.mp3");
	died = true;
	if (_id == 4 || _id == 9) {
		GetRemoved();
		return;
	}
	CCAction * action = CCSequence::create(CCJumpTo::create(1, ccp(this->getPositionX() + xvel * 20, -_mapposy - 50), 100, 1),
		CCCallFunc::create(this, callfunc_selector(CharEnemy::GetRemoved)),
		NULL);
	this->runAction(action);
}

void CharEnemy::GetTread() {
	if (_id == 10) {
		float dx = this->getPositionX() + 4000;
		float dy = this->getPositionY();
		CharEnemy * pEnemy;
		pEnemy = CharEnemy::create("cloud", dx, dy, _themap);
		pEnemy->setPosition(dx, dy);
		_themap->addChild(pEnemy, 100000);
	}
	SimpleAudioEngine::sharedEngine()->playEffect("tread.mp3");
	treaden = true;
	CCAction * action = CCSequence::create(CCFadeOut::create(1.0f),
		CCCallFunc::create(this, callfunc_selector(CharEnemy::GetRemoved)),
		NULL);
	this->runAction(action);
}

void CharEnemy::GetRemoved() {
	_themap->removeChild(this);
	if (shelled) {
		_themap->getShells()->removeObject(this);
	}
	if (_id != 2 || _id != 5 || _id != 6 || _id != 8) {
		_themap->getEnemies()->removeObject(this);
	}
}

void CharEnemy::PlayAnimation() {
	aniplays = 0;
	if (treaden) {
		aniplays = 1;
	}
	if (shelled || atk) {
		aniplays = 2;
	}
	if (shellpushed) {
		aniplays = 3;
	}
	if (_id == 12 || _id == 13) {
		int mx = _themap->pMario->getPositionX();
		if (this->getPositionX() < mx) {
			this->setRotationY(0);
		} else {
			this->setRotationY(180);
		}
	} else {
		if (xvel > 0) {
			this->setRotationY(0);
		} else if (xvel < 0) {
			this->setRotationY(180);
		}
	}
	if (_id == 3) {
		if (yvel > 0) {
			this->setRotationX(0);
		} else if (yvel < 0) {
			this->setRotationX(180);
		}
	}
	if (_id == 4 || _id == 9) {
		this->setZOrder(1500);
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

void CharEnemy::CheckReset() {
	if (this->getPositionX() + _themap->getPositionX() < -100 || this->getPositionX() + _themap->getPositionX() > 900 || this->getPositionY() + _themap->getPositionY() < -50 || this->getPositionY() + _themap->getPositionY() > 530) {
		this->setVisible(false);
	} else {
		this->setVisible(true);
	}
}

void CharEnemy::onUpdate(float dt) {
	if (!treaden && !died && _themap->gaming && _id != 2 && _id != 15) {
		if (_id == 4 || _id == 9) {
			TestGrow();
		} else if (_id == 5) {
			FireRounds();
		} else if (_id == 6) {
			FireRocket();
		} else if (_id == 8) {
			FireEnemies();
		} else if (_id == 10) {
			CloudMove();
		} else if (_id == 16) {
			FireGun();
		} else {
			Movement();
		}
		if (_id != 5) {
			CheckMario();
		}
	}
	PlayAnimation();
	CheckReset();
}