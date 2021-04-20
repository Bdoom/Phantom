//
// Created by decil on 4/14/21.
//

#ifndef PHANTOM_ENGINE_H
#define PHANTOM_ENGINE_H

#include "Logger.h"
#include "Sprite.h"
#include <SDL2/SDL.h>
#include <filesystem>
#include <GL/glew.h>

enum class EGameState {
    Playing,
    Quitting
};

enum class RenderMode
{
    Fill,
    WireFrame
};

class Engine {

public:

    Engine();
    virtual ~Engine();

    void Start();
    void Quit();
    void ToggleWireframe();

    RenderMode renderMode;


    Logger* GetLogger()
    {
        if (logger != nullptr)
        {
            return logger;
        }
        else
        {
            logger = new Logger();
            return logger;
        }
    }


private:
    void Update();
    void CheckInput();
    void Draw();
    bool LoadShader(GLuint program, const std::filesystem::path path, GLenum shaderType);


    Logger* logger;
    SDL_Window* MainWindow;
    EGameState GameState;
    GLuint program;

};


#endif //PHANTOM_ENGINE_H
