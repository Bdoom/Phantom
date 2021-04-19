//
// Created by decil on 4/14/21.
//

#include "Engine.h"
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <SDL2/SDL_video.h>

void Engine::Start() {
    SDL_Init(SDL_INIT_EVERYTHING);

    logger = new Logger(LOG_TYPE_LEVEL_ALL);

    MainWindow = SDL_CreateWindow("Decil Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
    if (MainWindow == nullptr)
    {
        GetLogger()->Log("Failed to create window.", LOG_TYPE_LEVEL_ERROR, LogCategory::Initialization);
        Quit();
        return;
    }

    SDL_GLContext glContext = SDL_GL_CreateContext(MainWindow);
    if (glContext == nullptr)
    {
        GetLogger()->Log("Failed to create GL context", LOG_TYPE_LEVEL_ERROR,LogCategory::Initialization);
        Quit();
    }

    if(glewInit() != GLEW_OK)
    {
        GetLogger()->Log("Could not initialize glew.", LOG_TYPE_LEVEL_ERROR, LogCategory::Initialization);
        Quit();
    }

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    glClearColor(0,0,1, 1);

    GameState = EGameState::Playing;
    Update();

}

void Engine::Update() {

    while(GameState == EGameState::Playing)
    {
        CheckInput();
        Draw();
    }

    Quit();
}

void Engine::CheckInput() {
    SDL_Event event;

    while(SDL_PollEvent(&event))
    {
        switch(event.type)
        {
            case SDL_QUIT:
                GameState = EGameState::Quitting;
                break;

            case SDL_MOUSEMOTION:

                // extremely slow, need to find a better solution to this.
                char buffer[50];
                sprintf(buffer, "X: %d Y: %d", event.motion.x, event.motion.y);
                GetLogger()->Log(buffer, LOG_TYPE_LEVEL_INFO);
                break;


        }
    }
}

void Engine::Quit() {
    delete logger;
    SDL_Quit();
}

Engine::Engine() {
    GameState = EGameState::Playing;
}

Engine::~Engine() {
}

void Engine::Draw() {
    glClearDepth(1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



    SDL_GL_SwapWindow(MainWindow);
}
