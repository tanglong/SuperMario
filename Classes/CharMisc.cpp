#include "CharMisc.h"
#include "CharMario.h"

CharMisc * CharMisc::create(int id, int func, CCDictionary * pDic, GameMap * gameMap) {
	do {
		CharMisc * pMisc = new CharMisc;
		CC_BREAK_IF(pMisc == NULL);

		pMisc->init(id, func, pDic, gameMap);
		pMisc->autorelease();
		return pMisc;
	} while (false);
	return NULL;
}

bool CharMisc::init(int id, int func, CCDictionary * pDic, GameMap * gameMap) {
	do {
		CC_BREAK_IF(CCArmature::init("misc") == false);
		
		_themap = gameMap;
		_id = id;
		_func = func;
		_pdic = pDic;
		xvelfixed = 0;
		if (_id == 2) {
			xvelfixed = 3;
		}
		xvel = xvelfixed;
		yvel = 0;
		landed = false;
		enabled = false;
		gravity = false;
		if (_id == 1) {
			enabled = true;
			if (_func == 1) {
				gravity = true;
			}
		}
		if (_id == 2 || _id == 3 || _id == 4) {
			gravity = true;
			CCAction * action = CCSequence::create(CCMoveBy::create(1, ccp(0, 32)),
				CCCallFunc::create(this, callfunc_selector(CharMisc::SetEnabled)),
				NULL);
			this->runAction(action);
		}
		if (_id == 0 && _func == 0) {
			CCAction * action = CCSequence::create(CCJumpBy::create(0.5f, ccp(0, 0), 100, 1),
				CCDelayTime::create(0.1f),
				CCCallFunc::create(this, callfunc_selector(CharMisc::GetRemoved)),
				NULL);
			this->runAction(action);
		}
		this->getAnimation()->playByIndex(_id);

		this->schedule(schedule_selector(CharMisc::onUpdate));
		
		return true;
	} while (false);
	return false;
}

void CharMisc::SetEnabled() {
	this->setZOrder(10000);
	enabled = true;
}

void CharMisc::Movement() {
	JudgePoint.setRect(this->getPositionX() - 16, this->getPositionY() - 16, 32, 32);
	if (!gravity) {
		return;
	}
	xCheckInfo xcheckinfo = _themap->CheckX(JudgePoint.getMaxY(), JudgePoint.getMinY(), JudgePoint.getMinX() + xvel, JudgePoint.getMaxX() + xvel);
	
	if (yvel > 0 && _id == 1 && _func == 1) {
		xcheckinfo.crashDirection = 0;
	}
	if (xcheckinfo.crashDirection == 0) {//如果水平无碰撞{
		this->setPositionX(this->getPositionX() + xvel);
	} else if (xcheckinfo.crashDirection == 1) {//如果左侧碰撞
		this->setPositionX(xcheckinfo.leftX + JudgePoint.size.width / 2 + 2.f);//+2.f为防止纵向碰撞判断到右侧重叠的瓦片
		xvel = xvelfixed;
	} else if (xcheckinfo.crashDirection == 2) {
		this->setPositionX(xcheckinfo.rightX - JudgePoint.size.width / 2 - 2.f);//-2.f为防止纵向碰撞判断到左侧重叠的瓦片
		xvel = -xvelfixed;
	}

	JudgePoint.setRect(this->getPositionX() - 14, this->getPositionY() - 16, 28, 32);
	yCheckInfo ycheckinfo = _themap->CheckY(JudgePoint.getMaxY() + yvel, JudgePoint.getMinY() + yvel, JudgePoint.getMinX(), JudgePoint.getMaxX());
	
	if (yvel > 0 && _id == 1 && _func == 1) {
		ycheckinfo.crashDirection = 0;
	}
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
		this->setPositionY(ycheckinfo.downY + JudgePoint.size.height / 2);//没有修正是为了确保探测到地面
		yvel = 0;
		landed = true;
	}
	if (ycheckinfo.crashDirection == 3) {
		yvel = 0;
		landed = true;
	}

	if (this->getPositionY() + _themap->getPositionY() < -50) {
		GetRemoved();
	}
}

void CharMisc::CheckMario() {
	CharMario * pMario = _themap->pMario;
	CCRect MarioJP = pMario->JudgePoint;

	if (pMario->died) {
		return;
	}
	if (!(MarioJP.getMaxX() < JudgePoint.getMinX() + 5 || JudgePoint.getMaxX() - 5 < MarioJP.getMinX() || MarioJP.getMaxY() < JudgePoint.getMinY() || JudgePoint.getMaxY() < MarioJP.getMinY())) { //玛丽与我碰撞了
		if (_id != 1) {
			pMario->GrowUp(_id);
		} else {
			_themap->_thescene->getCoin();
		}
		GetRemoved();
	}
}

void CharMisc::CheckReset() {
	if (this->getPositionX() + _themap->getPositionX() < -100 || this->getPositionX() + _themap->getPositionX() > 900 || this->getPositionY() + _themap->getPositionY() < -50 || this->getPositionY() + _themap->getPositionY() > 530) {
		this->setVisible(false);
	} else {
		this->setVisible(true);
	}
}

void CharMisc::GetRemoved() {
	_themap->removeChild(this);
}

void CharMisc::onUpdate(float dt) {
	if (_themap->gaming || (_id == 1 && _func == 1)) {
		if (enabled) {
			Movement();
			CheckMario();
		}
	}
	CheckReset();
}