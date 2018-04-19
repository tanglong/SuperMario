#include "CharBlock.h"
#include "CharMario.h"
#include "CharEnemy.h"

CharBlock * CharBlock::create(int id, const char * func, CCDictionary * pDic, GameMap * gameMap) {
	do {
		CharBlock * pBlock = new CharBlock;
		CC_BREAK_IF(pBlock == NULL);

		pBlock->init(id, func, pDic, gameMap);
		pBlock->autorelease();
		return pBlock;
	} while (false);
	return NULL;
}

bool CharBlock::init(int id, const char * func, CCDictionary * pDic, GameMap * gameMap) {
	do {
		CC_BREAK_IF(CCArmature::init("blocks") == false);

		_themap = gameMap;
		_func = func;
		_id = id;
		_pdic = pDic;
		died = false;
		this->getAnimation()->playByIndex(id);

		this->schedule(schedule_selector(CharBlock::onUpdate));

		return true;
	} while (false);
	return false;
}

void CharBlock::Headed() {
	CharMario * pMario = _themap->pMario;
	int lv = pMario->level;
	
	CCObject * pObj = NULL;
	CCArray * pArray = _themap->getEnemies();

	CCARRAY_FOREACH(pArray, pObj) {
		if (pObj != this) {
			CharEnemy * pEnemy = (CharEnemy *)pObj;
			CCRect EnemyJP = pEnemy->JudgePoint;
			if (EnemyJP.getMinY() >= JudgePoint.getMinY() && EnemyJP.getMinY() <= JudgePoint.getMaxY() + 5 && EnemyJP.getMinX() <= JudgePoint.getMaxX() - 5 && EnemyJP.getMaxX() >= JudgePoint.getMinX() + 5) {
				pEnemy->GetDied();
			}
		}
	}
	if (_id == 3) {
		CCPoint loc;
		loc = ccp((int)this->getPositionX() / 32, _themap->xMap->getMapSize().height - (int)this->getPositionY() / 32 - 1);
		CCTMXLayer * pBonusLayer = _themap->xMap->layerNamed("bonus");
		// pBonusLayer->setTileGID(9, loc);
//        pBonusLayer->setTileGID(1, loc);
        
	}
	if (strcmp(_func, "none") == 0) {
		if (lv > 1) {
			SimpleAudioEngine::sharedEngine()->playEffect("blocksmash.mp3");
			GetReset();
			died = true;
			int xv = pMario->xvel;
			CCAction * action = CCSequence::create(CCJumpTo::create(1, ccp(this->getPositionX() + xv * 5,  -_themap->getPositionY() - 50), 200, 1),
				CCCallFunc::create(this, callfunc_selector(CharBlock::GetRemoved)),
				NULL);
			this->runAction(CCRotateBy::create(1, 120 * xv));
			this->runAction(action);
		} else {
			SimpleAudioEngine::sharedEngine()->playEffect("bump.mp3");
			this->runAction(CCJumpBy::create(0.3f, ccp(0, 0), 10, 1));
		}
	} else if (strcmp(_func, "coin") == 0) {
		died = true;
		_themap->_thescene->getCoin();
		this->getAnimation()->playByIndex(2);
		_themap->setMisc(0, 0, this->getPositionX(), this->getPositionY() + 32, NULL);
		CCAction * action = CCSequence::create(CCJumpBy::create(0.3f, ccp(0, 0), 10, 1),
			CCCallFunc::create(this, callfunc_selector(CharBlock::GetRemoved)),
			NULL);
		this->runAction(action);
	} else if (strcmp(_func, "flower") == 0) {
		SimpleAudioEngine::sharedEngine()->playEffect("spawn.mp3");
		died = true;
		int sid;
		if (pMario->level == 1) {
			sid = 2;
		} else {
			sid = 3;
		}
		this->getAnimation()->playByIndex(2);
		CCAction * action = CCSequence::create(CCJumpBy::create(0.3f, ccp(0, 0), 10, 1),
			CCCallFuncND::create(this, callfuncND_selector(CharBlock::Grows), (void *)sid),
			NULL);
		this->runAction(action);
	} else if (strcmp(_func, "rocket") == 0) {
		SimpleAudioEngine::sharedEngine()->playEffect("spawn.mp3");
		died = true;
		int sid;
		if (pMario->level == 1) {
			sid = 2;
		} else {
			sid = 4;
		}
		this->getAnimation()->playByIndex(2);
		CCAction * action = CCSequence::create(CCJumpBy::create(0.3f, ccp(0, 0), 10, 1),
			CCCallFuncND::create(this, callfuncND_selector(CharBlock::Grows), (void *)sid),
			NULL);
		this->runAction(action);
	}
}

void CharBlock::Grows(CCNode * pTarget, void * data) {
	int id = (int)data;
	_themap->setMisc(id, 0, this->getPositionX(), this->getPositionY(), NULL);
	CCAction * action = CCSequence::create(CCDelayTime::create(1),
		CCCallFunc::create(this, callfunc_selector(CharBlock::GetRemoved)),
		NULL);
	this->runAction(action);
}

void CharBlock::CheckMario() {
	CharMario * pMario = _themap->pMario;
	CCRect MarioJP = pMario->JudgePoint;
	JudgePoint.setRect(this->getPositionX() - 16, this->getPositionY() - 16, 32, 32);

	if (pMario->died) {
		return;
	}
	if (MarioJP.getMinX() >= JudgePoint.getMinX() - 20 && MarioJP.getMaxX() <= JudgePoint.getMaxX() + 20 && MarioJP.getMaxY() >= JudgePoint.getMinY() - 4 && MarioJP.getMaxY() <= JudgePoint.getMaxY() - 1) { //²Èµ½Í·ÉÏ
		if (_id == 3) {
			if (pMario->yvel <= -2) {
				return;
			} else {
				pMario->setPositionY(pMario->getPositionY() - pMario->yvel - 10);
				pMario->yvel = 0;
			}
		}
		Headed();
	}
}

void CharBlock::GetReset() {
	CCPoint loc;
	loc = ccp((int)this->getPositionX() / 32, _themap->xMap->getMapSize().height - (int)this->getPositionY() / 32 - 1);
	CCTMXLayer * pBonusLayer = _themap->xMap->layerNamed("bonus");
	pBonusLayer->removeTileAt(loc);
}

void CharBlock::CheckReset() {
	if (this->getPositionX() + _themap->getPositionX() < -100 || this->getPositionX() + _themap->getPositionX() > 900 || this->getPositionY() + _themap->getPositionY() < -50 || this->getPositionY() + _themap->getPositionY() > 530) {
		this->setVisible(false);
	} else {
		this->setVisible(true);
	}
}

void CharBlock::GetRemoved() {
	if (strcmp(_func, "none") != 0) {
		CCPoint loc;
		loc = ccp((int)this->getPositionX() / 32, _themap->xMap->getMapSize().height - (int)this->getPositionY() / 32 - 1);
		CCTMXLayer * pBonusLayer = _themap->xMap->layerNamed("bonus");
		pBonusLayer->removeTileAt(loc);
		CCTMXLayer * pLandLayer = _themap->xMap->layerNamed("land");
		pLandLayer->setTileGID(9, loc);
	}
	_themap->removeChild(this);
}

void CharBlock::onUpdate(float dt) {
	if (_themap->gaming && !died) {
		CheckMario();
	}
	CheckReset();
}