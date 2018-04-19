#include "CharSnake.h"
#include "CharMario.h"

CharSnake * CharSnake::create(CharSnake * ahead, float dx, float dy, GameMap * gameMap) {
	do {
		CharSnake * pSnake = new CharSnake;
		CC_BREAK_IF(pSnake == NULL);

		pSnake->init(ahead, dx, dy, gameMap);
		pSnake->autorelease();
		return pSnake;
	} while (false);
	return NULL;
}

bool CharSnake::init(CharSnake * ahead, float dx, float dy, GameMap * gameMap) {
	do {
		CC_BREAK_IF(CCArmature::init("snake") == false);

		_themap = gameMap;
		velfixed = 2;
		xvel = 0;
		yvel = 0;
		prex = 0;
		prey = 0;
		stopframe = 0;
		_dx = dx;
		_dy = dy;
		_ahead = ahead;
		this->getAnimation()->playByIndex(0);

		startmoving = false;

		this->schedule(schedule_selector(CharSnake::onUpdate));

		return true;
	} while (false);
	return false;
}

void CharSnake::Movement() {
	if (stopframe > 0) {
		stopframe--;
	} else {
		stopframe = 0;
		this->setPositionX(this->getPositionX() + xvel);
		this->setPositionY(this->getPositionY() + yvel);
	}
	JudgePoint.setRect(this->getPositionX() - 16, this->getPositionY() - 4, 32, 32);
	prex = (this->getPositionX() - 16) / 32;
	prey = (this->getPositionY() - 8) / 32;
	if (_ahead == NULL) {
		if (!startmoving) {
			return;
		}
		if (prex < 0 || prex > _themap->xMap->boundingBox().getMaxX() / 32 || prey < 0 || prey > _themap->xMap->boundingBox().getMaxY() / 32 - 1) {
			return;
		}
		CCTMXLayer * pWayPoints = _themap->xMap->layerNamed("waypoints");
		int wp = pWayPoints->tileGIDAt(ccp(prex, (int)(_themap->xMap->boundingBox().getMaxY() / 32) - prey - 1));
		if (wp == 194) {
			xvel = 0;
			yvel = velfixed;
		} else if (wp == 195) {
			xvel = 0;
			yvel = -velfixed;
		} else if (wp == 196) {
			xvel = -velfixed;
			yvel = 0;
		} else if (wp == 197) {
			xvel = velfixed;
			yvel = 0;
		}
		if (wp > 0) {
			stopframe = 30;
			pWayPoints->removeTileAt(ccp(prex, (int)(_themap->xMap->boundingBox().getMaxY() / 32) - prey - 1));
		}
	} else {
		if (prex < _ahead->prex) {
			xvel = velfixed;
		} else if (prex > _ahead->prex) {
			xvel = -velfixed;
		} else {
			xvel = 0;
		}
		if (prey < _ahead->prey) {
			yvel = velfixed;
		} else if (prey > _ahead->prey) {
			yvel = -velfixed;
		} else {
			yvel = 0;
		}
	}
	if (this->getPositionY() + _themap->getPositionY() < -50) {
		xvel = 0;
		yvel = 0;
	}
}

void CharSnake::CheckMario() {
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
		pMario->setPositionY(this->getPositionY() + 52 + yvel);
		if (!startmoving) {
			startmoving = true;
			xvel = velfixed;
			CharSnake * aheadSnake;
			aheadSnake = this;
			for (int i = 0; i < 6; i++) {
				float dx = this->getPositionX();
				float dy = this->getPositionY();
				CharSnake * pSnake;
				pSnake = CharSnake::create(aheadSnake, dx, dy, _themap);
				pSnake->setPosition(dx, dy);
				pSnake->startmoving = true;
				_themap->addChild(pSnake, 100000);
				aheadSnake = pSnake;
			}
		}
	}
}

void CharSnake::GetRemoved() {
	_themap->removeChild(this);
}

void CharSnake::onUpdate(float dt) {
	if (_themap->gaming) {
		Movement();
		CheckMario();
	}
}