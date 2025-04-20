#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

class HelloWorld : public cocos2d::CCLayer
{
public:
	static cocos2d::CCScene* scene();
	virtual ~HelloWorld();
	virtual bool init();
	CREATE_FUNC(HelloWorld);

	virtual void registerWithTouchDispatcher();
	void updateCameraProjection();
	virtual bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
	virtual void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
	virtual void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);

	void zoomInCallback(cocos2d::CCObject* pSender);
	void zoomOutCallback(cocos2d::CCObject* pSender);
	void resetPositionCallback(cocos2d::CCObject* pSender);

	void setCameraZoom(float zoomFactor);

private:
	cocos2d::CCCamera* m_ccCamera;
	float m_zoomFactor;
	cocos2d::CCPoint ccTouchBeganPos;
	// CCPoint m_lastTouchPosition;
   // bool m_isDragging;
};

#endif // __HELLOWORLD_SCENE_H__
