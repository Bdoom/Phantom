//
// Created by decil on 4/18/21.
//

#ifndef PHANTOM_SPRITE_H
#define PHANTOM_SPRITE_H

typedef unsigned int GLuint;

class Sprite {

public:
    Sprite();
    ~Sprite();

    void init(int x, int y, int height, int width);
    GLuint vboID;


};


#endif //PHANTOM_SPRITE_H
