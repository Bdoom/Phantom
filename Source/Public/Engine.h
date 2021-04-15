//
// Created by decil on 4/14/21.
//

#ifndef PHANTOM_ENGINE_H
#define PHANTOM_ENGINE_H

#include "Logger.h"

class Engine {

public:

    Engine();
    virtual ~Engine();

    void Start();
    void Quit();

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

    Logger* logger;

};


#endif //PHANTOM_ENGINE_H
