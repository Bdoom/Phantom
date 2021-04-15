//
// Created by decil on 4/14/21.
//

#include "../Public/Engine.h"
#include "SDL2/SDL.h"

void Engine::Start() {
    SDL_Init(SDL_INIT_EVERYTHING);

    logger = new Logger(LOG_TYPE_LEVEL_ALL);

    GetLogger()->Log("test", LOG_TYPE_LEVEL_ERROR, LogCategory::Engine);
    GetLogger()->Log("test", LOG_TYPE_LEVEL_INFO, LogCategory::Engine);
    GetLogger()->RemoveLogLevel(LOG_TYPE_LEVEL_INFO);
    GetLogger()->Log("test", LOG_TYPE_LEVEL_INFO, LogCategory::Engine);
    GetLogger()->AddLogLevel(LOG_TYPE_LEVEL_INFO);
    GetLogger()->Log("test", LOG_TYPE_LEVEL_INFO, LogCategory::Engine);

}

void Engine::Update() {

}

void Engine::CheckInput() {

}

void Engine::Quit() {

    delete logger;
    SDL_Quit();
}

Engine::Engine() {

}

Engine::~Engine() {
    Quit();
}
