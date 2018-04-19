#include "CharBullet.h"
#include "CharMario.h"

CharBullet * CharBullet::create(int id, bool right, bool hurt, GameMap * gameMap, float dx, float dy, int len) {
	do {
		CharBullet * pBullet = new CharBullet;
		CC_BREAK_IF(pBullet == NULL);

		pBullet->init(id, right, hurt, gameMap, dx, dy, len);
		pBullet->autorelease();
		return pBullet;
	} while (false);
	return NULL;
}

bool CharBullet::init(int id, bool right, bool hurt, GameMap * gameMap, float dx, float dy, int len) {
	do {
		CC_BREAK_IF(CCArmature::init("bullet") == false);
		
		_themap = gameMap;
		xvelfixed = 9;
		xvel = xvelfixed;
		if (!right) {
			xvel = -xvelfixed;
		}
		yvel = 0;
		nowplaying = 1;
		aniplays = 0;
		frames = 0;
		_id = id;
		_len = len;
		_dx = dx;
		_dy = dy;
		died = false;
		hurtmario = hurt;
		hurtenemies = !hurt;
		if (_id <= 1) {
			this->setScale(1.5f);
		}
		if (_id == 1) {
			xvel = 0;
		}
		if (_id == 3) {
			xvel = 0;
			yvel = 0;
		}
		if (_id == 4) {
			if (hurtmario) {
				xvelfixed = 4;
				xvel = 4;
				if (!right) {
					xvel = -4;
				}
			}
		}
		if (_id == 5) {
			CharMario * pMario = _themap->pMario;
			int mx = pMario->getPositionX();
			int my = pMario->getPositionY();
			srand((unsigned)time(0));
			xvel = (rand() % 10 + 10) / 2;
			if (mx < dx) {
				xvel = -xvel;
			}
			yvel = (my - dy) * fabs(xvel * 1.f / (mx - dx)) + fabs((mx - dx) * 1.f / (2 * xvel));
			if (yvel >= 25) {
				yvel = 25;
			}
		}
		if (_id == 6) {
			xvel = 0;
			yvel = 0;
		}

		this->schedule(schedule_selector(CharBullet::onUpdate));

		return true;
	} while (false);
	return false;
}

void CharBullet::Movement() {
	if (_id == 4) {
		JudgePoint.setRect(this->getPositionX() - 15, this->getPositionY() - 14, 30, 28);
		this->setPositionX(this->getPositionX() + xvel);
		return;
	}

	if (_id == 5) {
		JudgePoint.setRect(this->getPositionX() - 16, this->getPositionY() - 16, 16, 16);
		yvel--;
		if (yvel <= -15) {
			yvel = -15;
		}
		this->setRotation(this->getRotation() + 2);
		this->setPositionX(this->getPositionX() + xvel);
		this->setPositionY(this->getPositionY() + yvel);
		return;
	}

	JudgePoint.setRect(this->getPositionX() - 8, this->getPositionY() - 8, 16, 16);
	if (_id == 1) {
		this->setRotation(this->getRotation() + 2);
		float rota = this->getRotation();
		this->setPositionX(_dx + _len * cos(rota * 3.14f / 180));
		this->setPositionY(_dy + _len * sin(rota * 3.14f / 180));
		return;
	}
	if (_id == 3) {
		frames++;
		if (frames < 60) {
			return;
		}
		if (frames == 60) {
			SimpleAudioEngine::sharedEngine()->playEffect("firechain.mp3");
			CharMario * pMario = _themap->pMario;
			int mx = pMario->getPositionX();
			int my = pMario->getPositionY();
			float tanx = (_dy - my) / (_dx - mx);
			float cosx = sqrt(1 / (1 + tanx * tanx));
			float sinx = sqrt(tanx * tanx / (1 + tanx * tanx));
			if (_dx > mx) {
				cosx = -cosx;
			}
			if (_dy > my) {
				sinx = -sinx;
			}
			xvel = 3 * cosx;
			yvel = 3 * sinx;
		}
		this->setPositionX(this->getPositionX() + xvel);
		this->setPositionY(this->getPositionY() + yvel);
		return;
	}
	if (_id == 6) {
		this->setPositionX(this->getPositionX() + xvel);
		this->setPositionY(this->getPositionY() + yvel);
		return;
	}
	xCheckInfo xcheckinfo = _themap->CheckX(JudgePoint.getMaxY(), JudgePoint.getMinY(), JudgePoint.getMinX() + xvel, JudgePoint.getMaxX() + xvel);
	
	if (xcheckinfo.crashDirection == 0) {//如果水平无碰撞
		this->setPositionX(this->getPositionX() + xvel);
	} else {
		GetDied();
	}
	
	if (_id == 2) {
		return;
	}
	JudgePoint.setRect(this->getPositionX() - 8, this->getPositionY() - 8, 16, 16);
	yCheckInfo ycheckinfo = _themap->CheckY(JudgePoint.getMaxY() + yvel, JudgePoint.getMinY() + yvel, JudgePoint.getMinX(), JudgePoint.getMaxX());

	if (ycheckinfo.crashDirection == 0) {
		this->setPositionY(this->getPositionY() + yvel);
		yvel--;
		if (yvel <= -15) {
			yvel = -15;
		}
	} else if (ycheckinfo.crashDirection == 2) {
		this->setPositionY(ycheckinfo.downY + JudgePoint.size.height / 2);//没有修正是为了确保探测到地面
		yvel = 6;
		if (_id == 0 && _len == 1) {
			GetDied();
		}
	} else {
		GetDied();
	}
}

void CharBullet::CheckMario() {
	CharMario * pMario = _themap->pMario;
	CCRect MarioJP = pMario->JudgePoint;
	JudgePoint.setRect(this->getPositionX() - 8, this->getPositionY() - 8, 16, 16);

	if (pMario->died) {
		return;
	}
	
	if (_id == 4 || _id == 3) {
		CCObject * pObj = NULL;
		CCArray * pArray = _themap->getBullets();

		CCARRAY_FOREACH(pArray, pObj) {
			CharBullet * pBullet = (CharBullet *)pObj;
			CCRect BulletJP = pBullet->JudgePoint;
			if (pBullet->hurtenemies && pBullet->_id == 4) {
				if (!(BulletJP.getMaxX() < JudgePoint.getMinX() + 5 || JudgePoint.getMaxX() - 5 < BulletJP.getMinX() || BulletJP.getMaxY() < JudgePoint.getMinY() || JudgePoint.getMaxY() < BulletJP.getMinY())) { //子弹与我碰撞了
					SimpleAudioEngine::sharedEngine()->playEffect("shell.mp3");
					died = true;
					CCAction * action = CCSequence::create(CCDelayTime::create(0.2f),
						CCCallFunc::create(this, callfunc_selector(CharBullet::GetRemoved)),
						NULL);
					this->runAction(action);
				}
			}
		}
	}

	if (!(MarioJP.getMaxX() < JudgePoint.getMinX() + 5 || JudgePoint.getMaxX() - 5 < MarioJP.getMinX() || MarioJP.getMaxY() < JudgePoint.getMinY() || JudgePoint.getMaxY() < MarioJP.getMinY())) { //玛丽与我碰撞了
		if (_id == 4) {
			if (MarioJP.getMinX() >= JudgePoint.getMinX() - 20 && MarioJP.getMaxX() <= JudgePoint.getMaxX() + 20 && MarioJP.getMinY() >= JudgePoint.getMaxY() - 20) { //踩到头上
				SimpleAudioEngine::sharedEngine()->playEffect("tread.mp3");
				pMario->yvel = 10;
				pMario->landed = true;
				died = true;
				CCAction * action = CCSequence::create(CCDelayTime::create(0.2f),
					CCCallFunc::create(this, callfunc_selector(CharBullet::GetRemoved)),
					NULL);
				this->runAction(action);
				return;
			}
		}
		if (_id != 1 && _id != 4) {
			GetDied();
		}
		pMario->GetHurt();
	}
}

void CharBullet::GetDied() {
	if (_id == 4) {
		return;
	}
	died = true;
	CCAction * action = CCSequence::create(CCDelayTime::create(0.2f),
		CCCallFunc::create(this, callfunc_selector(CharBullet::GetRemoved)),
		NULL);
	this->runAction(action);
}

void CharBullet::GetRemoved() {
	_themap->removeChild(this);
	_themap->getBullets()->removeObject(this);
}

void CharBullet::PlayAnimation() {
	aniplays = 0;
	if (_id == 2 || _id == 6) {
		aniplays = 2;
	}
	if (_id == 3) {
		aniplays = 3;
	}
	if (_id == 4) {
		aniplays = 4;
	}
	if (_id == 5) {
		aniplays = 5;
	}
	if (died) {
		aniplays = 1;
	}
	if (_id == 2 || _id == 4 || _id == 6) {
		if (xvel > 0) {
			this->setRotationY(180);
		} else if (xvel < 0) {
			this->setRotationY(0);
		}
	}
	if (nowplaying != aniplays) {
		this->getAnimation()->playByIndex(aniplays);
		nowplaying = aniplays;
	}
}

void CharBullet::CheckReset() {
	if (this->getPositionX() + _themap->getPositionX() < -100 || this->getPositionX() + _themap->getPositionX() > 900 || this->getPositionY() + _themap->getPositionY() < -150 || this->getPositionY() + _themap->getPositionY() > 630) {
		if (_id != 1) {
			GetRemoved();
		} else {
			this->setVisible(false);
		}
	} else {
		this->setVisible(true);
	}
}

void CharBullet::onUpdate(float dt) {
	if (!died && _themap->gaming) {
		if (hurtmario) {
			CheckMario();
		}
		Movement();
	}
	PlayAnimation();
	CheckReset();
}