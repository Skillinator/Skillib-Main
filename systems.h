#ifndef _skilLibSystems
#define _skilLibSystems

#include <stdio.h>
#include <vector>
#include <GLFW/glfw3.h>
#include <string>
#include "skilLib.h"

const int SYSTEM_DEBUG = 0;
const int SYSTEM_WINDOW = 1;
const int SYSTEM_TIMEKEEPER = 2;
const int SYSTEM_MOVEMENT = 3;
const int SYSTEM_COLLISION = 4;
const int SYSTEM_RENDER = 5;
const int SYSTEM_INPUT = 6;
const int SYSTEM_GARBAGECOLLECT = 7;
const int SYSTEM_CLICKDRAG = 8;
class WindowSystem : public System{
public:
	WindowSystem();
	WindowSystem(int gwidth, int gheight, std::string gtitle);
	void initializeWindow();
	void closeWindow();
	void update(float delta);
protected:
	int width;
	int height;
	char* title;
};

class TimekeeperSystem : public System{
public:
	TimekeeperSystem();
	void update(float delta);
	double getCurrentTime();
	int getDelta();
	int getFPS();
protected:
	int ticks;
	int fps;
	float delta;
	double lastTime;
	double lastSecond;
	double currentTime;
};

class MovementSystem : public System{
public:
	MovementSystem();
	void update(float delta); // The method where the float delta actually matters
};

class CollisionSystem : public System{
public:
	CollisionSystem();
	void update(float delta);
};

class RenderSystem : public System{
public:
	RenderSystem();
	void update(float delta);
protected:
};

class InputSystem : public System{
public:
	InputSystem();
	void update();
};

class ClickDragSystem : public System{
public:
	ClickDragSystem();
	void update(float delta);
};

#endif
