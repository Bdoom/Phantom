//
// Created by decil on 4/18/21.
//

#include "Sprite.h"
#include <GL/glew.h>
#include <GL/gl.h>

Sprite::Sprite() {
    vboID = 0;
}

Sprite::~Sprite() {

}

void Sprite::init(int x, int y, int height, int width) {

    if (vboID == 0)
    {
        glGenBuffers(1, &vboID);
    }

    float vertexData[12];


    glBindBuffer(GL_ARRAY_BUFFER, vboID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

}
