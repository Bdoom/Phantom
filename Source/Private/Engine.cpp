//
// Created by decil on 4/14/21.
//

#include "Engine.h"
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <SDL2/SDL_video.h>
#include <filesystem>
#include <cassert>

void Engine::Start() {
    SDL_Init(SDL_INIT_EVERYTHING);

    logger = new Logger(LOG_TYPE_LEVEL_ALL);

    MainWindow = SDL_CreateWindow("Decil Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
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

    if(LoadShaders())
    {
        GetLogger()->Log("Shaders loaded and compiled successfully.", LOG_TYPE_LEVEL_INFO);
    }
    else
    {
        GetLogger()->Log("Shaders failed to load and or compile.", LOG_TYPE_LEVEL_ERROR);
    }

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

bool Engine::LoadShaders() {

    // load up vertex shader
    char infoLog[512];
    GLint success;

    std::string temp;
    std::string src = "";

    std::ifstream file;
    file.open(std::filesystem::current_path().parent_path() / "Shaders/vertex_shader.glsl");

    if (file.is_open())
    {
        while(std::getline(file, temp))
        {
            src += temp + "\n";
        }
    }
    else
    {
        GetLogger()->Log("Could not find the vertex shader file.", LOG_TYPE_LEVEL_ERROR);
        return false;
    }

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const GLchar* vertSrc = src.c_str();
    glShaderSource(vertexShader, 1, &vertSrc, NULL);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        GetLogger()->Log(infoLog, LOG_TYPE_LEVEL_ERROR);
        return false;
    }
    file.close();
    // end vertex shader loading

    // load up fragment shader
    memset(infoLog, 0, 512);
    temp = "";
    src = "";

    file.open(std::filesystem::current_path().parent_path() / "Shaders/fragment_shader.glsl");
    if (file.is_open())
    {
        while(std::getline(file, temp))
        {
            src += temp + "\n";
        }
    }
    else
    {
        GetLogger()->Log("Failed to open fragment shader.", LOG_TYPE_LEVEL_ERROR);
        return false;
    }

    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    const GLchar* fragSrc = src.c_str();
    glShaderSource(fragShader, 1, &fragSrc, NULL);
    glCompileShader(fragShader);
    glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragShader, 512, NULL, infoLog);
        GetLogger()->Log(infoLog, LOG_TYPE_LEVEL_ERROR);
        return false;
    }

    // end loading fragment shader

    return false;
}
