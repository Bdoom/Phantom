//
// Created by decil on 4/14/21.
//

#include "Engine.h"
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <SDL2/SDL_video.h>
#include <filesystem>
#include <glm/glm.hpp>

struct Vertex
{
    glm::vec3 position;
    glm::vec3 color;
    glm::vec2 texcoord;
};

Vertex verticies[] =
        {
            glm::vec3(0, 0.5f, 0), glm::vec3(1,0,0), glm::vec2(0, 1),
            glm::vec3(-0.5f, -0.5f, 0), glm::vec3(0, 1, 0), glm::vec2(0, 0),
            glm::vec3(0.5, -0.5, 0), glm::vec3(0, 0, 1), glm::vec2(1, 0)
        };

unsigned numVerts = sizeof(verticies) / sizeof(Vertex);
GLuint indicies[] =
        {
            0, 1, 2
        };

unsigned numOfIndicies = sizeof(indicies) / sizeof(GLuint);

void Engine::Start() {

    SDL_Init(SDL_INIT_EVERYTHING);

    logger = new Logger(LOG_TYPE_LEVEL_ALL);

    MainWindow = SDL_CreateWindow("Decil Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if (MainWindow == nullptr)
    {
        GetLogger()->Log("Failed to create window.", LOG_TYPE_LEVEL_ERROR, LogCategory::Initialization);
        return;
    }

    SDL_GLContext glContext = SDL_GL_CreateContext(MainWindow);
    if (glContext == nullptr)
    {
        GetLogger()->Log("Failed to create GL context", LOG_TYPE_LEVEL_ERROR,LogCategory::Initialization);
        return;
    }

    if(glewInit() != GLEW_OK)
    {
        GetLogger()->Log("Could not initialize glew.", LOG_TYPE_LEVEL_ERROR, LogCategory::Initialization);
        return;
    }

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    // OpenGL Major/Minor Versions set to use opengl 4.5
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);

    // enable depth
    glEnable(GL_DEPTH_TEST);

    // enable opengl culling for the back side
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    // counter clockwise - means that we want to cull the back of the verts.
    // http://docs.gl/gl4/glFrontFace
    glFrontFace(GL_CCW);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_DEBUG_OUTPUT);


    glClearColor(1,1,1, 1);

    std::filesystem::path vertexShaderPath = std::filesystem::absolute(std::filesystem::current_path() / "Shaders/vertex_shader.glsl");
    std::filesystem::path fragShaderPath = std::filesystem::absolute(std::filesystem::current_path() / "Shaders/fragment_shader.glsl");

    // create program
    program = glCreateProgram();

    bool vertexShaderLoaded = LoadShader(program, vertexShaderPath, GL_VERTEX_SHADER);
    bool fragmentShaderLoaded = LoadShader(program, fragShaderPath, GL_FRAGMENT_SHADER);

    if (vertexShaderLoaded)
    {
        GetLogger()->Log("Vertex shader loaded and compiled successfully.", LOG_TYPE_LEVEL_INFO, LogCategory::Shader_Initialization);
    }

    if (fragmentShaderLoaded)
    {
        GetLogger()->Log("Fragment shader loaded and compiled successfully.", LOG_TYPE_LEVEL_INFO, LogCategory::Shader_Initialization);
    }

    if (!vertexShaderLoaded || !fragmentShaderLoaded)
    {
        GetLogger()->Log("Shaders failed to load and or compile.", LOG_TYPE_LEVEL_ERROR, LogCategory::Shader_Initialization);
        return;
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

            case SDL_KEYDOWN:
                //printf("key: %s\n", SDL_GetKeyName(event.key.keysym.sym)); // not sure what 'keysym' is or 'keysym.sym'
                //printf("%d", event.key.keysym.sym);
                if (event.key.keysym.sym == SDLK_F1)
                {
                    ToggleWireframe();
                }
                break;

            default:
                std::string evtStr = "Event type: " + std::to_string(event.type);
                GetLogger()->Log(evtStr, LOG_TYPE_LEVEL_INFO);
                break;

        }
    }
}

void Engine::Quit() {
    delete logger;
    glDeleteProgram(program);
    SDL_Quit();
}

Engine::Engine() {
    GameState = EGameState::Playing;
    renderMode = RenderMode::Fill;
}

Engine::~Engine() {
}

void Engine::Draw() {
    glClearDepth(1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // VAO - vertex array object
    // vao stores the attributes set in glVertexAttribPointer() and glEnableVertexAttribArray() and glDisableVertexAttribArray() functions
    // the VAO could be reused for multiple different configurations, using one VAO.
    // this is basically a configuration object which defines the spacing and order of how the data inside of a vbo is stored.
    GLuint VAO;
    glCreateVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // VBO generation - vertex buffer object
    // contains the actual vertices and vertex data.
    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW);

    // ebo - element buffer object
    // tells opengl the order in which to draw the vertices, by the indices set in the EBO object.
    // when we bind the EBO, the VAO that is currently in use will also store our EBO as the current EBO object.
    // so if we unbind the VAO, it will keep that EBO, and we can bind a new VAO
    GLuint EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);

    // vertex attribute pointers
    GLuint attribLocation = glGetAttribLocation(program, "vertex_position");
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texcoord));
    glEnableVertexAttribArray(2);

    glUseProgram(program);

    // draw to screen
    // glDrawElements requires a bound EBO - element array buffer object.
    glDrawElements(GL_TRIANGLES, numOfIndicies + 1, GL_UNSIGNED_INT, 0);

    // glDrawArrays takes the opengl primitive type we want to draw, and then the starting index in the vertex array
    // and then the last parameter is the number of number of vertices we would like to draw.
    //glDrawArrays(GL_TRIANGLES, 0, 3);

    // unbind vert array
    glBindVertexArray(0);



    SDL_GL_SwapWindow(MainWindow);
}

bool Engine::LoadShader(GLuint program, const std::filesystem::path path, GLenum shaderType) {

    // load up shader source
    char infoLog[512];
    GLint success;

    std::string temp;
    std::string src = "";

    std::ifstream file;
    file.open(path.c_str());

    if (file.is_open())
    {
        while(std::getline(file, temp))
        {
            src += temp + "\n";
        }
    }
    else
    {
        GetLogger()->Log("Could not find the shader file.", LOG_TYPE_LEVEL_ERROR, LogCategory::Shader_Initialization);
        return false;
    }

    GLuint shader = glCreateShader(shaderType);
    const GLchar* vertSrc = src.c_str();
    glShaderSource(shader, 1, &vertSrc, NULL);
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (success != GL_TRUE)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        GetLogger()->Log(infoLog, LOG_TYPE_LEVEL_ERROR, LogCategory::Shader_Initialization);
        return false;
    }
    file.close();
    // end shader loading

    glAttachShader(program, shader);
    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (success != GL_TRUE)
    {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        GetLogger()->Log(infoLog, LOG_TYPE_LEVEL_ERROR, LogCategory::Shader_Initialization);



        return false;
    }

    glDeleteShader(shader);
    
    return true;
}

void Engine::ToggleWireframe() {
    if(renderMode == RenderMode::Fill) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        renderMode = RenderMode::WireFrame;
    }
    else if (renderMode == RenderMode::WireFrame)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        renderMode = RenderMode::Fill;
    }
}
