#include "HelloWorldScene.h"
#include "kazmath\kazmath.h"
#include "kazmath\GL\matrix.h"

USING_NS_CC;

CCScene* HelloWorld::scene()
{
	CCScene *scene = CCScene::create();
	HelloWorld *layer = HelloWorld::create();
	scene->addChild(layer);
	return scene;
}

HelloWorld::~HelloWorld()
{
	CC_SAFE_DELETE(m_ccCamera);
}

bool HelloWorld::init()
{
	if (!CCLayer::init()) return false;

	CCSize vs = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	CCSprite* sprite = CCSprite::create("HelloWorld.png");
	sprite->setPosition(ccp(vs.width / 2 + origin.x, vs.height / 2 + origin.y));
	addChild(sprite, 0);

	m_zoomFactor = 1.f;
	m_ccCamera = new CCCamera();
	m_ccCamera->init();
	m_ccCamera->setEyeXYZ(0, 0, 500);
	m_ccCamera->setCenterXYZ(0, 0, 0);
	m_ccCamera->retain();

	updateCameraProjection();

	CCDirector::sharedDirector()->setProjection(kCCDirectorProjectionCustom);
	// CCDirector::sharedDirector()->setCamera(m_ccCamera);

	this->setTouchEnabled(true);

	CCMenuItemImage* zoomIn = CCMenuItemImage::create(
		"ZoomInIcon.png",
		"ZoomInIcon.png",
		this,
		menu_selector(HelloWorld::zoomInCallback));

	CCMenuItemImage* zoomOut = CCMenuItemImage::create(
		"ZoomOutIcon.png",
		"ZoomOutIcon.png",
		this,
		menu_selector(HelloWorld::zoomOutCallback));

	CCMenuItemImage* resetPosition = CCMenuItemImage::create(
		"ResetIcon.png",
		"ResetIcon.png",
		this,
		menu_selector(HelloWorld::resetPositionCallback));

	if (zoomIn && zoomOut && resetPosition) {
		CCMenu* zoomMenu = CCMenu::create(zoomIn, zoomOut, resetPosition, NULL);
		zoomMenu->setPosition(ccp(vs.width / 2 + origin.x, 50 + origin.y));
		zoomMenu->alignItemsVerticallyWithPadding(20.f);
		zoomMenu->setScale(0.5f);
		addChild(zoomMenu, 1);
	}

	return true;
}

void HelloWorld::updateCameraProjection()
{
	CCSize vs = CCDirector::sharedDirector()->getVisibleSize();
	float width = vs.width / m_zoomFactor;
	float height = vs.height / m_zoomFactor;

	kmGLMatrixMode(KM_GL_PROJECTION);
	kmGLLoadIdentity();

	kmMat4 orthoMatrix;
	kmMat4OrthographicProjection(&orthoMatrix, -width / 2, width / 2, -height / 2, height / 2, 1, 1000);
	kmGLMultMatrix(&orthoMatrix);

	kmGLMatrixMode(KM_GL_MODELVIEW);
	kmGLLoadIdentity();
}

void HelloWorld::registerWithTouchDispatcher()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
}

bool HelloWorld::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
	ccTouchBeganPos = touch->getLocation();
	return true;
}

void HelloWorld::ccTouchMoved(CCTouch* touch, CCEvent* event)
{
	if (m_ccCamera) {
		CCPoint delta = touch->getDelta();

		float eyeX, eyeY, eyeZ;
		m_ccCamera->getEyeXYZ(&eyeX, &eyeY, &eyeZ);
		CCPoint currentPos = ccp(eyeX, eyeY);

		CCPoint newPos = ccp(currentPos.x - delta.x, currentPos.y - delta.y);

		m_ccCamera->setEyeXYZ(newPos.x, newPos.y, 500);
		m_ccCamera->setCenterXYZ(newPos.x, newPos.y, 0);
	}
}

void HelloWorld::ccTouchEnded(CCTouch* touch, CCEvent* event)
{
	CCPoint screenPos = touch->getLocation();
	CCPoint worldPos = this->convertToNodeSpace(screenPos);

	CCSprite* spr = CCSprite::create("Plus.png");
		spr->setPosition(worldPos);
		spr->setContentSize(CCSizeMake(20, 20));

		spr->runAction(CCSequence::create(
			CCFadeOut::create(2.f),
			CCRemoveSelf::create(),
			NULL));

		addChild(spr, 1);
}

void HelloWorld::zoomInCallback(CCObject* pSender)
{
	setCameraZoom(m_zoomFactor + 0.1f);
}

void HelloWorld::zoomOutCallback(CCObject* pSender)
{
	setCameraZoom(m_zoomFactor - 0.1f);
}

void HelloWorld::resetPositionCallback(CCObject* pSender)
{
	if (m_ccCamera) {
		m_ccCamera->setEyeXYZ(0, 0, 500);
		m_ccCamera->setCenterXYZ(0, 0, 0);
		setCameraZoom(1.f);
	}
}

void HelloWorld::setCameraZoom(float zoomFactor)
{
	if (!m_ccCamera) return;

	zoomFactor = MAX(0.1f, MIN(zoomFactor, 5.f));
	m_zoomFactor = zoomFactor;

	updateCameraProjection();
}