#include "HelloWorldScene.h"
#include "kazmath/kazmath.h"
#include "kazmath/GL/matrix.h"

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
	CC_SAFE_RELEASE(m_ccCamera);
}

bool HelloWorld::init()
{
	if (!CCLayer::init()) return false;

	CCSize vs = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	CCDirector::sharedDirector()->setProjection(kCCDirectorProjection2D);

	CCSprite* sprite = CCSprite::create("HelloWorld.png");
	if (sprite) {
		sprite->setPosition(ccp(vs.width / 2 + origin.x, vs.height / 2 + origin.y));
		addChild(sprite, 0);
	}
	else {
		CCLOG("Error: HelloWorld.png not load");
		CCLayerColor* bg = CCLayerColor::create(ccc4(100, 100, 200, 255));
		addChild(bg, -1);
	}

	m_zoomFactor = 1.f;
	m_ccCamera = new CCCamera();
	m_ccCamera->init();
	m_ccCamera->setEyeXYZ(0, 0, 500);
	m_ccCamera->setCenterXYZ(0, 0, 0);
	m_ccCamera->retain();

	// Estado inicial del touch
	m_isDragging = false;
	m_lastTouchPosition = CCPointZero;

	updateCameraProjection();

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
	kmMat4OrthographicProjection(&orthoMatrix,
		0, width,    // left, right 
		0, height,   // bottom, top
		-500, 500);  // near, far 
	kmGLMultMatrix(&orthoMatrix);

	updateCameraView();
}

void HelloWorld::updateCameraView()
{
	kmGLMatrixMode(KM_GL_MODELVIEW);
	kmGLLoadIdentity();

	if (m_ccCamera) {
		float eyeX, eyeY, eyeZ;
		m_ccCamera->getEyeXYZ(&eyeX, &eyeY, &eyeZ);

		kmVec3 eye = { eyeX, eyeY, eyeZ };
		kmVec3 center = { eyeX, eyeY, 0 }; 
		kmVec3 up = { 0, 1, 0 };           

		kmMat4 lookAt;
		kmMat4LookAt(&lookAt, &eye, &center, &up);
		kmGLMultMatrix(&lookAt);
	}
}

void HelloWorld::registerWithTouchDispatcher()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
}

bool HelloWorld::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
	m_lastTouchPosition = touch->getLocation();
	m_isDragging = true;
	return true;
}

void HelloWorld::ccTouchMoved(CCTouch* touch, CCEvent* event)
{
	if (!m_isDragging || !m_ccCamera) return;

	CCPoint currentTouch = touch->getLocation();
	CCPoint delta = ccpSub(currentTouch, m_lastTouchPosition);

	delta = ccpMult(delta, -1.f);

	float eyeX, eyeY, eyeZ;
	m_ccCamera->getEyeXYZ(&eyeX, &eyeY, &eyeZ);

	float newX = eyeX + delta.x * m_zoomFactor;
	float newY = eyeY + delta.y * m_zoomFactor;

	m_ccCamera->setEyeXYZ(newX, newY, eyeZ);
	m_ccCamera->setCenterXYZ(newX, newY, 0);

	m_lastTouchPosition = currentTouch;

	updateCameraView();
}

void HelloWorld::ccTouchEnded(CCTouch* touch, CCEvent* event)
{
	m_isDragging = false;

	CCPoint screenPos = touch->getLocation();
	CCPoint worldPos = this->convertToNodeSpace(screenPos);

	CCSprite* spr = CCSprite::create("Plus.png");
	if (spr) {
		spr->setPosition(worldPos);
		spr->setContentSize(CCSizeMake(20, 20));
		spr->runAction(CCSequence::create(
			CCFadeOut::create(2.f),
			CCRemoveSelf::create(),
			NULL));
		addChild(spr, 1);
	}
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
		updateCameraView();
	}
}

void HelloWorld::setCameraZoom(float zoomFactor)
{
	if (!m_ccCamera) return;

	zoomFactor = MAX(0.1f, MIN(zoomFactor, 5.f));
	m_zoomFactor = zoomFactor;

	updateCameraProjection();
}
