#ifndef CUBEINSWINDOW_H
#define CUBEINSWINDOW_H

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Matrix.hpp"

using namespace std;

class CubeInsWindow {
public:
    CubeInsWindow(int xBlocks, int yBlocks, int zBlocks);
    ~CubeInsWindow();
    int run();
    GLuint loadShaders(const char * vertex_file_path,const char * fragment_file_path);
private:
    void getShaderInfoLog(GLuint id, int length);
    void getProgramInfoLog(GLuint id, int length);
    int loadTexture(string fileName, GLenum texUnit, int width, int height);
    void makeData(int xs, int ys, int zs, int xm, int ym, int zm, float xd, float yd, float zd, 
                            float** vboV, float** vboT, float** vboP, unsigned int** indices,
                            int* numVboV, int* numVboT, int* numVboP, int *numIndices);
    
    static const int _WIN_W = 1200;
    static const int _WIN_H = 720;

    int _xBlocks;
    int _yBlocks;
    int _zBlocks;

    GLuint _vaoID;
    GLuint _vboID_T;
    GLuint _vboID_V;
    GLuint _vboID_P;
    GLuint _iboID;

    GLfloat* _vertAry;
    GLfloat* _texAry;
    GLfloat* _posAry;
    unsigned int* _idxAry;

    GLuint _programID;
    GLuint _textureID;

    mat4 _projectionMatrix;
    mat4 _viewMatrix;
    mat4 _modelViewMatrix;
    GLuint _mvpMatrixLocation;
};

#endif
