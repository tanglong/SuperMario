#include "CharBridge.h"
#include "CharMario.h"

CharBridge * CharBridge::create(int length, const char * dir, GameMap * gameMap) {
	do {
		CharBridge * pBridge = new CharBridge;
		CC_BREAK_IF(pBridge == NULL);

		pBridge->init(length, dir, gameMap);
		pBridge->autorelease();
		return pBridge;
	} while (false);
	return NULL;
}

bool CharBridge::init(int length, const char * dir, GameMap * gameMap) {
	do {
		_mySprite = CCSprite::createWithSpriteFrameName("bridge.png");
		_mySprite->setAnchorPoint(ccp(0, 0));
		CC_BREAK_IF(_mySprite == NULL);
        this->addChild(_mySprite);

		_themap = gameMap;
		_dir = dir;
		_length = length * 32;
		xvelmax = 3;
		yvelmax = 1;
		xvelacc = 0.1f;
		yvelacc = 0.1f;
		yvel = 0;
		xvel = 0;
		nlength = 0;
		_mapposy = _themap->getPositionY();
		stmove = false;

		if (strcmp(_dir, "u") == 0) {
			yvel = yvelmax;
		}
		if (strcmp(_dir, "d") == 0) {
			yvel = -yvelmax;
		}
		if (strcmp(_dir, "rl") == 0) {
			xvel = xvelmax;
		}
		if (strcmp(_dir, "lr") == 0) {
			xvel = -xvelmax;
		}
		if (strcmp(_dir, "du") == 0) {
			yvelmax = 3;
			yvel = -yvelmax;
		}
		if (strcmp(_dir, "ud") == 0) {
			yvelmax = 3;
			yvel = yvelmax;
		}

		this->schedule(schedule_selector(CharBridge::onUpdate));
		
		return true;
	} while (false);
	return false;
}

void CharBridge::Movement() {
	if (strcmp(_dir, "d") == 0) {
		if (this->getPositionY() + _mapposy < 0) {
			this->setPositionY(480 - _mapposy);
		}
	}
	if (strcmp(_dir, "u") == 0) {
		if (this->getPositionY() + _mapposy > 480) {
			this->setPositionY(-_mapposy);
		}
	}
	if (strcmp(_dir, "rl") == 0) {
		nlength += xvel;
		if (nlength >= _length) {
			if (xvel > -xvelmax) {
				xvel -= xvelacc;
			} else {
				xvel = -xvelmax;
			}
		}
		if (nlength <= 0) {
			if (xvel < xvelmax) {
				xvel += xvelacc;
			} else {
				xvel = xvelmax;
			}
		}
	}
	if (strcmp(_dir, "lr") == 0) {
		nlength -= xvel;
		if (nlength >= _length) {
			if (xvel < xvelmax) {
				xvel += xvelacc;
			} else {
				xvel = xvelmax;
			}
		}
		if (nlength <= 0) {
			if (xvel > -xvelmax) {
				xvel -= xvelacc;
			} else {
				xvel = -xvelmax;
			}
		}
	}
	if (strcmp(_dir, "ud") == 0) {
		nlength += yvel;
		if (nlength >= _length) {
			if (yvel > -yvelmax) {
				yvel -= yvelacc;
			} else {
				yvel = -yvelmax;
			}
		}
		if (nlength <= 0) {
			if (yvel < yvelmax) {
				yvel += yvelacc;
			} else {
				yvel = yvelmax;
			}
		}
	}
	if (strcmp(_dir, "du") == 0) {
		nlength -= yvel;
		if (nlength >= _length) {
			if (yvel < yvelmax) {
				yvel += yvelacc;
			} else {
				yvel = yvelmax;
			}
		}
		if (nlength <= 0) {
			if (yvel > -yvelmax) {
				yvel -= yvelacc;
			} else {
				yvel = -yvelmax;
			}
		}
	}
	if (strcmp(_dir, "dd") == 0) {
		if (this->getPositionY() + _mapposy < -50) {
			yvel = 0;
		} else if (stmove) {
			yvel -= 0.3f;
		}
	}
	if (strcmp(_dir, "rr") == 0) {
		nlength += xvel;
		if (nlength >= _length) {
			xvel = 0;
		} else if (stmove) {
			xvelmax = 2;
			xvel = xvelmax;
		}
	}
	this->setPositionX(this->getPositionX() + xvel);
	this->setPositionY(this->getPositionY() + yvel);
	JudgePoint.setRect(this->getPositionX(), this->getPositionY() + 16, 128, 16);
}

void CharBridge::CheckMario() {
	CharMario * pMario = _themap->pMario;
	CCRect MarioJP = pMario->JudgePoint;

	if (pMario->died) {
		return;
	}
	if (MarioJP.getMinX() >= JudgePoint.getMinX() - 20 && MarioJP.getMaxX() <= JudgePoint.getMaxX() + 20 && MarioJP.getMinY() >= JudgePoint.getMinY() - 8 && MarioJP.getMinY() <= JudgePoint.getMaxY() && pMario->yvel <= 0) { //²Èµ½Í·ÉÏ
		if (pMario->yvel < 0) {
			pMario->yvel = 0;
		}
		pMario->landed = true;
		pMario->setPositionY(this->getPositionY() + 46);
		pMario->setPositionX(pMario->getPositionX() + xvel);
		if (strcmp(_dir, "dd") == 0 || strcmp(_dir, "rr") == 0) {
			stmove = true;
		}
	}
}

void CharBridge::CheckReset() {
	if (this->getPositionX() + _themap->getPositionX() < -140 || this->getPositionX() + _themap->getPositionX() > 900 || this->getPositionY() + _themap->getPositionY() < -50 || this->getPositionY() + _themap->getPositionY() > 530) {
		this->setVisible(false);
	} else {
		this->setVisible(true);
	}
}

void CharBridge::onUpdate(float dt) {
	if (_themap->gaming) {
		Movement();
		CheckMario();
	}
	CheckReset();
}