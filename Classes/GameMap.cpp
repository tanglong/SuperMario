#include "GameMap.h"
#include "CharMario.h"
#include "CharBridge.h"
#include "CharEnemy.h"
#include "CharBlock.h"
#include "CharMisc.h"
#include "CharSnake.h"
#include "CharBowser.h"

GameMap::GameMap(void) {
}

GameMap::~GameMap(void) {
	_shells->release();
	_bullets->release();
	_enemies->release();
	this->unscheduleAllSelectors();
}

GameMap * GameMap::loadnow(GameScene * thescene, int levela, int levelb) {
    GameMap * pRet = new GameMap();
    if (pRet && pRet->initnow(thescene, levela, levelb)) {
		pRet->autorelease();
        return pRet;
    } else {
		CC_SAFE_DELETE(pRet);
        return NULL;
    }
}

bool GameMap::initnow(GameScene * thescene, int levela, int levelb) {
	if (!CCLayer::init()) {
		return false;
	}

	_thescene = thescene;
	gaming = true;
	showonly = false;
	automove = false;
	myx = 1;
	mus = '\0';
	yvel = 0;
	yvelacc = -0.003125f;

	_shells = CCArray::create();
	_shells->retain();
	
	_bullets = CCArray::create();
	_bullets->retain();

	_enemies = CCArray::create();
	_enemies->retain();

	xMap = CCTMXTiledMap::create(CCString::createWithFormat("%d-%d.tmx", levela, levelb)->getCString());
	addChild(xMap, 2000);

	CCTMXLayer * pBonusLayer = xMap->layerNamed("bonus");
	pBonusLayer->setVisible(false);
		
	CCTMXLayer * pPipeinLayer = xMap->layerNamed("pipein");
	pPipeinLayer->setVisible(false);

	CCTMXObjectGroup * sprites = xMap->objectGroupNamed("sprites");
	CCDictionary * born = sprites->objectNamed("born");
	int dx, dy;

	if (_thescene->checkpoint != 0) {
		CCObject * pObj;
		CCArray * pArray = sprites->getObjects();

		CCARRAY_FOREACH(pArray, pObj) {
			CC_BREAK_IF(pObj == NULL);
			CCDictionary * pDic = (CCDictionary *)pObj;
			CCString * cString = (CCString *)(pDic->objectForKey("name"));
			if (cString->m_sString == "cp") {
				int cid = ((CCString *)(pDic->objectForKey("cpid")))->intValue();
				if (cid == _thescene->checkpoint) {
					dx = ((CCString *)(pDic->objectForKey("x")))->intValue();
					dy = ((CCString *)(pDic->objectForKey("y")))->intValue();
					this->setPositionY(((CCString *)(pDic->objectForKey("mapy")))->doubleValue());
				}
			}
		}
	} else {
		this->setPositionY(((CCString *)(born->objectForKey("mapy")))->doubleValue());
		dx = born->valueForKey("x")->intValue();
		dy = born->valueForKey("y")->intValue();
	}
	pMario = CharMario::create("mario", _thescene->mariolevel, _thescene->mariofirerate, this);
	pMario->setPosition(dx + 16, dy + 16);
	backs = (CCString *)(born->objectForKey("back"));
	this->addChild(pMario, 1000000);
	
	CCDictionary * dest = sprites->objectNamed("destination");
	destx = dest->valueForKey("x")->intValue();
	destmy = ((CCString *)(dest->objectForKey("mapy")))->intValue();
	int desty = dest->valueForKey("y")->intValue();
	int castle = ((CCString *)(dest->objectForKey("castle")))->intValue();
	
	fixedcamera = false;
	boss = false;
	if (castle == 0) {
		CCSprite * backcastle = CCSprite::createWithSpriteFrameName("smallcastle.png");
		backcastle->setAnchorPoint(ccp(0.5f, 0));
		backcastle->setPosition(ccp(destx + 560, desty));
		addChild(backcastle, 10);
	} else if (castle == 1) {
		CCSprite * backcastle = CCSprite::createWithSpriteFrameName("bigcastle.png");
		backcastle->setAnchorPoint(ccp(0.5f, 0));
		backcastle->setPosition(ccp(destx + 560, desty));
		addChild(backcastle, 10);
	} else if (castle == 2) {
		boss = true;
	}

	background = GameBack::loadnow(backs);
	background->autorelease();
	addChild(background, 1);
	
	_thescene->Hp1->setVisible(false);
	_thescene->Hp2->setVisible(false);

	onUpdate(0);

	this->schedule(schedule_selector(GameMap::onUpdate));

	return true;
}

OrdersList GameMap::getOrders() {
	return _thescene->orderslist;
}

xCheckInfo GameMap::CheckX(float upY, float downY, float leftX, float rightX) {
	xCheckInfo CheckInfo;
	CheckInfo.crashDirection = -1;
	CheckInfo.leftX = -1;
	CheckInfo.rightX = -1;

	CheckInfo.crashDirection = 0;//初始碰撞判断为，左右未碰撞

	CCSprite * pLeftTile=NULL;
	CCSprite * pRightTile=NULL;

	float tempY;
	float allowableUpY;
	float allowableDownY;

	//设定允许的Y范围
	if (upY > xMap->boundingBox().getMaxY()) {
		allowableUpY = xMap->boundingBox().getMaxY() - 1;//Max点获取的地块已经超过地图范围
	} else {
		allowableUpY = upY;
	}

	if (downY < xMap->boundingBox().getMinY()) {
		allowableDownY = xMap->boundingBox().getMinY();
	} else {
		allowableDownY = downY;
	}

	//左碰撞判断
	if (downY < xMap->boundingBox().getMaxY() && upY >= xMap->boundingBox().getMinY()) {
		tempY = allowableUpY;
		if (leftX - 1 >= xMap->boundingBox().getMaxX()) {
		} else if (leftX - 1 < xMap->boundingBox().getMinX()) {
			CheckInfo.leftX = xMap->boundingBox().getMinX();
			CheckInfo.crashDirection = 1;
		} else {
			while (tempY > allowableDownY) {
				pLeftTile = getTileByCoord(ccp(leftX - 1, tempY));
				if (pLeftTile != NULL) {
					//因为瓦片地图
					CheckInfo.leftX = pLeftTile->boundingBox().getMaxX();
					CheckInfo.crashDirection = 1;
					break;
				}
				tempY = tempY - xMap->getTileSize().height;
			}
			tempY = allowableDownY;
			pLeftTile = getTileByCoord(ccp(leftX - 1, tempY));
			if (pLeftTile != NULL) {
				CheckInfo.leftX = pLeftTile->boundingBox().getMaxX();
				CheckInfo.crashDirection = 1;
			}
		}
	}

	//右碰撞判断
	if (downY < xMap->boundingBox().getMaxY() && upY >= xMap->boundingBox().getMinY()) {
		tempY = allowableUpY;
		if (rightX + 1 < xMap->boundingBox().getMinX()) {
		} else if (rightX + 1 >= xMap->boundingBox().getMaxX()) {
			CheckInfo.rightX = xMap->boundingBox().getMaxX();
			if (CheckInfo.crashDirection == 0) {
				CheckInfo.crashDirection = 2;
			} else if (CheckInfo.crashDirection == 1) {
				CheckInfo.crashDirection = 3;
			}
		} else {
			while (tempY > allowableDownY) {
				pRightTile = getTileByCoord(ccp(rightX + 1, tempY));
				if (pRightTile != NULL) {
					//因为瓦片地图
					CheckInfo.rightX = pRightTile->boundingBox().getMinX();
					if (CheckInfo.crashDirection == 0) {
						CheckInfo.crashDirection = 2;
					} else if (CheckInfo.crashDirection == 1) {
						CheckInfo.crashDirection = 3;
					}
					break;
				}
				tempY = tempY - xMap->getTileSize().height;
			}
			tempY = allowableDownY;
			pRightTile = getTileByCoord(ccp(rightX + 1, tempY));
			if (pRightTile != NULL) {
				CheckInfo.rightX = pRightTile->boundingBox().getMinX();
				if (CheckInfo.crashDirection == 0) {
					CheckInfo.crashDirection = 2;
				} else if (CheckInfo.crashDirection == 1) {
					CheckInfo.crashDirection = 3;
				}
			}
		}
	}
	return CheckInfo;
}

yCheckInfo GameMap::CheckY(float upY, float downY, float leftX, float rightX) {
	yCheckInfo CheckInfo;
	CheckInfo.crashDirection = -1;
	CheckInfo.downY = -1;
	CheckInfo.upY = -1;

	CheckInfo.crashDirection = 0;//初始碰撞判断为，上下均未碰撞

	CCSprite * pUpTile = NULL;
	CCSprite * pDownTile = NULL;

	float tempX;
	float allowableLeftX;
	float allowableRightX;

		//设定允许的X范围
	if (rightX > xMap->boundingBox().getMaxX()) {
		allowableRightX = xMap->boundingBox().getMaxX() - 1;//Max点获取的地块已经超过地图范围
	} else {
		allowableRightX = rightX;
	}

	if (leftX < xMap->boundingBox().getMinX()) {
		allowableLeftX = xMap->boundingBox().getMinX();
	} else {
		allowableLeftX = leftX;
	}

	//上碰撞判断
	if (rightX >= xMap->boundingBox().getMinX() && leftX < xMap->boundingBox().getMaxX()) {
		tempX = allowableLeftX;
		if(upY < xMap->boundingBox().getMaxY() && upY > xMap->boundingBox().getMinY()) {
			while (tempX < allowableRightX) {
				pUpTile = getTileByCoord(ccp(tempX, upY + 1));
				if (pUpTile != NULL) {
					CheckInfo.upY = pUpTile->boundingBox().getMinY();
					CheckInfo.crashDirection = 1;
					break;
				}
				tempX = tempX + xMap->getTileSize().width;
			} 

			tempX = allowableRightX;
			pUpTile = getTileByCoord(ccp(tempX, upY + 1));
			if (pUpTile != NULL) {
				CheckInfo.upY = pUpTile->boundingBox().getMinY();
				CheckInfo.crashDirection = 1;
			}
		}
	}

	//下碰撞判断
	if (rightX >= xMap->boundingBox().getMinX() && leftX < xMap->boundingBox().getMaxX()) {
		tempX = allowableLeftX;
		if (downY < xMap->boundingBox().getMaxY() && downY > xMap->boundingBox().getMinY()) {
			while (tempX < allowableRightX) {
				pDownTile = getTileByCoord(ccp(tempX, downY - 1));
				if (pDownTile != NULL) {
					CheckInfo.downY = pDownTile->boundingBox().getMaxY();
					if (CheckInfo.crashDirection == 1) {
						CheckInfo.crashDirection = 3;
					} else if (CheckInfo.crashDirection == 0) {
						CheckInfo.crashDirection = 2;
					}
					break;
				}
				tempX = tempX + xMap->getTileSize().width;
			}

			tempX = allowableRightX;
			pDownTile = getTileByCoord(ccp(tempX, downY - 1));
			if (pDownTile != NULL) {
				CheckInfo.downY = pDownTile->boundingBox().getMaxY();
				if (CheckInfo.crashDirection == 1) {
					CheckInfo.crashDirection = 3;
				} else if (CheckInfo.crashDirection == 0) {
					CheckInfo.crashDirection = 2;
				}
			}
		}
	}

	return CheckInfo;
}

CCSprite * GameMap::getTileByCoord(CCPoint position) {
	CCPoint tilePosition = ccp(0, 0);

	position.y = xMap->boundingBox().getMaxY() - position.y;
	if (position.y / xMap->getTileSize().height - (float)((int)(position.y / xMap->getTileSize().height)) == 0) {//如果转换后纵向的块数是个整数
		tilePosition.y = (int)(position.y / xMap->getTileSize().height) - 1;//选择纵向-1的那块
	} else {
		tilePosition.y = (int)(position.y / xMap->getTileSize().height);
	}
		
	tilePosition.x = (int)(position.x / xMap->getTileSize().width);

	if (tilePosition.y < 0 || tilePosition.x < 0) {
		return NULL;
	}
		
	CCSprite * pTile = NULL;
	CCTMXLayer * pLandLayer = xMap->layerNamed("land");
	CCTMXLayer * pBonusLayer = xMap->layerNamed("bonus");
	pTile = pLandLayer->tileAt(tilePosition);
	if (pTile == NULL) {
		pTile = pBonusLayer->tileAt(tilePosition);
	}

	return pTile;
}

void GameMap::onUpdate(float dt) {
	setEnemies();
	cameraControl();
	if (myx != this->getPositionY()) {
		myx = this->getPositionY();
		int i = -myx / 800;

		const char * bs = backs->getCString();
		int k = strlen(bs);
		if (bs[k - 1 - i] != mus) {
			char buff[10] = "mx.mp3";
			buff[1] = bs[k - 1 - i];
			mus = bs[k - 1 - i];

			SimpleAudioEngine::sharedEngine()->playBackgroundMusic(buff, true);
		}
	}
}

void GameMap::cameraControl() {
	if (showonly || (automove && gaming)) {
		if (_thescene->levela == 8 && _thescene->levelb == 3) {
			if (this->getPositionX() <= CCDirector::sharedDirector()->getVisibleSize().width - xMap->boundingBox().getMaxX()) {
				if (this->getPositionY() < 0) {
					this->setPositionY(this->getPositionY() + 1);
				} else {
					this->setPositionY(0);
				}
			} else {
				if (this->getPositionX() < -800) {
					yvel += yvelacc;
					if (yvel <= -1 || yvel >= 1) {
						yvelacc = -yvelacc;
					}
					this->setPositionY(this->getPositionY() + yvel);
				}
			}
			this->setPositionX(this->getPositionX() - 1.5);
		} else if (_thescene->levela == 9) {
			this->setPositionX(this->getPositionX() + 1);
		} else {
			this->setPositionX(this->getPositionX() - 1);
		}
		if (this->getPositionX() < CCDirector::sharedDirector()->getVisibleSize().width - xMap->boundingBox().getMaxX()) {
			this->setPositionX(CCDirector::sharedDirector()->getVisibleSize().width - xMap->boundingBox().getMaxX());
		}
		if (this->getPositionX() > 0) {
			this->setPositionX(0);
		}
	}
	if (!fixedcamera && !showonly && !automove) {
		if (pMario->getPositionX() <= xMap->boundingBox().getMaxX() - CCDirector::sharedDirector()->getVisibleSize().width / 2 && pMario->getPositionX() >= CCDirector::sharedDirector()->getVisibleSize().width / 2) {
			this->setPositionX(CCDirector::sharedDirector()->getVisibleSize().width / 2 - pMario->getPositionX());
		} else if (pMario->getPositionX() < CCDirector::sharedDirector()->getVisibleSize().width / 2) {
			this->setPositionX(0);
		} else {
			this->setPositionX(CCDirector::sharedDirector()->getVisibleSize().width - xMap->boundingBox().getMaxX());
		}
	}
	background->setPositionX(-(int)(this->getPositionX() / 800) * 800);
}

void GameMap::setEnemies() {
	CCTMXObjectGroup * pSpritesGroup = xMap->objectGroupNamed("sprites");
	CCObject * pObj;
	CCArray * pArray = pSpritesGroup->getObjects();

	CCARRAY_FOREACH(pArray, pObj) {
		CC_BREAK_IF(pObj == NULL);
		CCDictionary * pDic = (CCDictionary *)pObj;
		float dx = ((CCString *)(pDic->objectForKey("x")))->doubleValue();
		float dy = ((CCString *)(pDic->objectForKey("y")))->doubleValue();
		CCString * pString = (CCString *)(pDic->objectForKey("type"));
		if (pString->m_sString == "enemy") {
			CCString * nString = (CCString *)(pDic->objectForKey("name"));
			if (nString->m_sString == "cloud") {
				if (((CCString *)(pDic->objectForKey("created")))->doubleValue() == 0) {
					CharEnemy * pEnemy;
					pEnemy = CharEnemy::create(nString->getCString(), dx + 16, dy + 16, this);
					pEnemy->setPosition(dx + 16, dy + 16);
					CCString * pStr = CCString::create("1");
					pDic->setObject((CCObject *)(pStr), "created");
					this->addChild(pEnemy, 100000);
				}
			}
		}
		if (pString->m_sString == "snake") {
			if (((CCString *)(pDic->objectForKey("created")))->doubleValue() == 0) {
				CharSnake * pSnake;
				pSnake = CharSnake::create(NULL, dx + 16, dy + 9, this);
				pSnake->setPosition(dx + 16, dy + 9);
				CCString * pStr = CCString::create("1");
				pDic->setObject((CCObject *)(pStr), "created");
				this->addChild(pSnake, 100000);
			}
		}
		if (pString->m_sString == "bowser") {
			if (((CCString *)(pDic->objectForKey("created")))->doubleValue() == 0) {
				CharBowser * pBowser;
				pBowser = CharBowser::create(this);
				pBowser->setPosition(dx + 16, dy + 16);
				CCString * pStr = CCString::create("1");
				pDic->setObject((CCObject *)(pStr), "created");
				this->addChild(pBowser, 100000);
			}
		}
		if (dx >= -140 - this->getPositionX() && dx <= CCDirector::sharedDirector()->getVisibleSize().width - this->getPositionX() + 100 && dy + this->getPositionY() >= 0 && dy + this->getPositionY() <= 480) {
			CCString * cString = (CCString *)(pDic->objectForKey("name"));
			if (cString->m_sString == "cp") {
				int mpy = ((CCString *)(pDic->objectForKey("mapy")))->intValue();
				int cpk = ((CCString *)(pDic->objectForKey("cpid")))->intValue();
				if (pMario->getPositionX() >= dx && pMario->getPositionX() <= dx + 32 && this->getPositionY() == mpy) {
					_thescene->checkpoint = cpk;
				}
			}
			CCString * pString = (CCString *)(pDic->objectForKey("type"));
			if (pString->m_sString == "wall") {
				if (((CCString *)(pDic->objectForKey("created")))->doubleValue() == 0) {
					CharBlock * pBlock;
					pBlock = CharBlock::create(((CCString *)(pDic->objectForKey("id")))->intValue(), ((CCString *)(pDic->objectForKey("name")))->getCString(), pDic, this);
					pBlock->setPosition(dx + 16, dy + 16);
					CCString * pStr = CCString::create("1");
					pDic->setObject((CCObject *)(pStr), "created");
					this->addChild(pBlock, 10000);
				}
			} else if (pString->m_sString == "enemy") {
				if (((CCString *)(pDic->objectForKey("created")))->doubleValue() == 0) {
					CharEnemy * pEnemy;
					CCString * nString = (CCString *)(pDic->objectForKey("name"));
					pEnemy = CharEnemy::create(nString->getCString(), dx + 16, dy + 16, this);
					pEnemy->setPosition(dx + 16, dy + 16);
					CCString * pStr = CCString::create("1");
					pDic->setObject((CCObject *)(pStr), "created");
					this->addChild(pEnemy, 100000);
				}
			} else if (pString->m_sString == "coin") {
				if (((CCString *)(pDic->objectForKey("created")))->doubleValue() == 0) {
					setMisc(1, 0, dx + 16, dy + 16, pDic);
					CCString * pStr = CCString::create("1");
					pDic->setObject((CCObject *)(pStr), "created");
				}
			} else if (pString->m_sString == "bridge") {
				if (((CCString *)(pDic->objectForKey("created")))->doubleValue() == 0) {
					CharBridge * pBridge;
					pBridge = CharBridge::create(((CCString *)(pDic->objectForKey("length")))->intValue(), ((CCString *)(pDic->objectForKey("direct")))->getCString(), this);
					pBridge->setPosition(dx + 16, dy + 16);
					CCString * pStr = CCString::create("1");
					pDic->setObject((CCObject *)(pStr), "created");
					this->addChild(pBridge, 10000);
				}
			}
		}
	}
}

void GameMap::setMisc(int id, int func, int x, int y, CCDictionary * pDic) {
	int depth;
	CharMisc * pMisc;
	pMisc = CharMisc::create(id, func, pDic, this);
	pMisc->setPosition(x, y);
	depth = 1000;
	if (id != 2 && id != 3 && id != 4) {
		depth = 1000000000;
	}
	this->addChild(pMisc, depth);
}