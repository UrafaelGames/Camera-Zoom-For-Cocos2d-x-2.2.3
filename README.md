# Camera-Zoom-For-Cocos2d-x-2.2.3

Hello everyone, I hope you are well,

I have made this code mostly to experiment with version 2.2.3 of cocos2d-x (yes, I know it is a version too old and obsolete, but I have it as an experimentation use) the code is about how to zoom with the CCCamera class of cocos2d, in this version is a little more complex to do this because I do not have an orthogonal projection as there is in more current versions of the framework, so try to make a more customized one.

> **Warning**: I recommend using cocos2d-x 2.2.3 on Windows 10 and below for better compatibility, on Windows 11 the engine has too many compilation bugs (which I try to fix).

 **NOTE**

```
// CCDirector::sharedDirector()->setCamera(m_ccCamera);
```
Its a custom function that I made in my cocos2d-x modification, don't uncomment as it will give you errors.


