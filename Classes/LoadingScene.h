#ifndef __LOADINGSCENE_H__
#define __LOADINGSCENE_H__

#include "GameGlobal.h"
#include "GameScene.h"
#include "ProgressView.h"

class LoadingScene : public CCScene {
public:
    virtual bool init();
    static CCScene * scene();
	void loading();
	void loadedCallBack(CCObject * obj);

	ProgressView * loadpro;
	int totalNum, haveLoadedNum;
	CCLabelTTF * ttf;
    CREATE_FUNC(LoadingScene);
};

#endif