/*
Author: Donavan Sirois
Date: Febuary 1, 2026
File name: Engine.h
Description: Header file for the game engine handling all events.
Modifications:
*/

#ifndef ENGINE_H
#define ENGINE_H

#include"Controller.h"

class Engine {
private:
	Controller controller;
    double timer;
public:
    Engine();
	~Engine();
	bool isRunning();
	virtual void processInput();
	void update();
    void render();
	void shutdown();
};

#endif
