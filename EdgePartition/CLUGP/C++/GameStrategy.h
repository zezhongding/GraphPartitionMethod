#ifndef GAMESTRATEGY_H
#define GAMESTRATEGY_H

#include "common.h"
#include "globalConfig.h"

class GameStrategy {
public:
	virtual ~GameStrategy() {}
	virtual void initGame(GlobalConfig& config) = 0;
	virtual void startGame(GlobalConfig& config) = 0;
};

#endif