#include "CubeInsWindow.h"
#include "FpsCounter.h"

#include <fstream>
#include <sstream>
#include <vector>
#include <limits.h>

CubeInsWindow::CubeInsWindow(int xBlocks, int yBlocks, int zBlocks) {
    _xBlocks = xBlocks;
    _yBlocks = yBlocks;
    _zBlocks = zBlocks;
}

int CubeInsWindow::run() {

    // GLFW init
    if (glfwInit() == GL_FALSE) {
        cerr << "failed to glfwInit" << endl;
        return 1;
    }
    atexit(glfwTerminate);
    
    // OpenGL 4.1
    //glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // create window
    GLFWwindow *const window(glfwCreateWindow(_WIN_W, _WIN_H, "CubeInsWindow", NULL, NULL));
    if (NULL == window) {
        cerr << "failed to create window" << endl;
        return 1;
    }

    // make context
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    
    // GLEW init
    glewExperimental = GL_TRUE; 
    if (GLEW_OK != glewInit()) {
        cerr << "failed to glewInit" << endl;
        return -1;
    }

    cout << "Vendor :" << glGetString(GL_VENDOR) << endl;
    cout << "GPU : " << glGetString(GL_RENDERER) << endl;
    cout << "OpenGL version : " << glGetString(GL_VERSION) << endl;

    _programID = loadShaders("shaderIns.vert", "shaderIns.frag");
    glUseProgram(_programID);

    // load texutre
    _textureID = this->loadTexture("cube.raw", GL_TEXTURE0, 128, 128);

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);

    // enable primitive restart
    glEnable(GL_PRIMITIVE_RESTART);
    glEnable(GL_PRIMITIVE_RESTART_FIXED_INDEX);
    glPrimitiveRestartIndex(UINT_MAX);

    // make data
    int xs = (-_xBlocks + 1) / 2;
    int xm = xs + _xBlocks - 1;
    int ys = (-_yBlocks + 1) / 2;
    int ym = ys + _yBlocks - 1;
    int zs = (-_zBlocks + 1) / 2;
    int zm = zs + _zBlocks - 1;

    cout << &_vertAry << endl;

    float distance = 1.5f;
    int vertAryNum = 0;
    int texAryNum = 0;
    int posAryNum = 0;
    int idxAryNum = 0;
    this->makeData(xs, ys, zs, xm, ym, zm,
                    distance, distance, distance, 
                    &_vertAry, &_texAry, &_posAry, &_idxAry,
                    &vertAryNum, &texAryNum, &posAryNum, &idxAryNum);
    cout << &_vertAry << endl;

    glGenVertexArrays(1, &_vaoID);
    glBindVertexArray(_vaoID);

    // vertex array
    glGenBuffers(1, &_vboID_V);
    glBindBuffer(GL_ARRAY_BUFFER, _vboID_V);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertAryNum, _vertAry, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3, (void*)0);

    // texture coord array
    glGenBuffers(1, &_vboID_T);
    glBindBuffer(GL_ARRAY_BUFFER, _vboID_T);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * texAryNum, _texAry, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 2, (void*)0);

    // pos array
    glGenBuffers(1, &_vboID_P);
    glBindBuffer(GL_ARRAY_BUFFER, _vboID_P);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * posAryNum, _posAry, GL_STATIC_DRAW);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3, (void*)0);

    // indices array
    glGenBuffers(1, &_iboID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _iboID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLfloat) * idxAryNum, _idxAry, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glVertexAttribDivisor(0, 0);
    glVertexAttribDivisor(1, 0);
    glVertexAttribDivisor(2, 1);

    _projectionMatrix = mat4::Perspective(45.0f, _WIN_W * 1.0f / _WIN_H, 0.5f, 10000.0f);
    vec3 cameraPos = vec3(0.0f, 0.0f, 3 * (_xBlocks + _yBlocks + _zBlocks) / 3.0f);
    _viewMatrix = mat4::Translate(-cameraPos.x, -cameraPos.y, -cameraPos.z);
    _modelViewMatrix = mat4::Translate(0.0f, 0.0f, 0.0f);

    _mvpMatrixLocation = glGetUniformLocation(_programID, "mvpMatrix");

    // clear
    glClearColor(0.0f, 0.5f, 0.0f, 1.0f);

    double lastTime = glfwGetTime();
    FpsCounter fpsCounter;

    while (glfwWindowShouldClose(window) == GL_FALSE) {

        double thisTime = glfwGetTime();
        double time = thisTime - lastTime;
        lastTime = thisTime;
        
        if (fpsCounter.update(time)) {
            glfwSetWindowTitle(window, fpsCounter.getFpsInfo().c_str());
        }

        _modelViewMatrix = mat4::Rotation(0.005f, 0.0f, 1.0f, 0.0f) * mat4::Rotation(0.01f, 1.0f, 0.0f, 0.0f) * _modelViewMatrix;

        mat4 mvpMatrix = _modelViewMatrix * _viewMatrix * _projectionMatrix;
        glUniformMatrix4fv(_mvpMatrixLocation, 1, GL_FALSE, mvpMatrix.Pointer());

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glDrawElementsInstanced(GL_TRIANGLE_STRIP, idxAryNum - 1, GL_UNSIGNED_INT, 0, _xBlocks * _yBlocks * _zBlocks);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return 0;
}

GLuint CubeInsWindow::loadShaders(const char * vertexPath,const char * fragmentPath) {

    GLuint vsID = glCreateShader(GL_VERTEX_SHADER);
    std::string vsCode;
    std::ifstream vsStream(vertexPath, std::ios::in);
    if (vsStream.is_open()) {
        std::stringstream sstr;
        sstr << vsStream.rdbuf();
        vsCode = sstr.str();
        vsStream.close();
    }

    GLuint fsID = glCreateShader(GL_FRAGMENT_SHADER);
    std::string fsCode;
    std::ifstream fsStream(fragmentPath, std::ios::in);
    if (fsStream.is_open()) {
        std::stringstream sstr;
        sstr << fsStream.rdbuf();
        fsCode = sstr.str();
        fsStream.close();
    }

    GLint result = GL_FALSE;
    int infoLogLen;

    cout << "Compiling shader : " + string(vertexPath);
    char const * pVS = vsCode.c_str();
    glShaderSource(vsID, 1, &pVS , NULL);
    glCompileShader(vsID);

    glGetShaderiv(vsID, GL_COMPILE_STATUS, &result);
    glGetShaderiv(vsID, GL_INFO_LOG_LENGTH, &infoLogLen);
    getShaderInfoLog(vsID, infoLogLen);

    cout << "Compiling shader : " + string(fragmentPath);
    char const * pFS = fsCode.c_str();
    glShaderSource(fsID, 1, &pFS , NULL);
    glCompileShader(fsID);

    glGetShaderiv(fsID, GL_COMPILE_STATUS, &result);
    glGetShaderiv(fsID, GL_INFO_LOG_LENGTH, &infoLogLen);
    getShaderInfoLog(fsID, infoLogLen);

    cout << "Linking program";
    GLuint programID = glCreateProgram();
    glAttachShader(programID, vsID);
    glAttachShader(programID, fsID);
    glLinkProgram(programID);

    glGetProgramiv(programID, GL_LINK_STATUS, &result);
    glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLen);
    getProgramInfoLog(programID, infoLogLen);

    glDeleteShader(vsID);
    glDeleteShader(fsID);

    return programID;
}

void CubeInsWindow::getShaderInfoLog(GLuint id, int length) {
    GLchar *infoLog = (GLchar *)malloc(length);
    if (NULL != infoLog) {
        glGetShaderInfoLog(id, length, NULL, infoLog);
        cerr << infoLog << endl;
        free (infoLog);
    } else {
        cerr << "colud not allocate shader infoLog buffer." << endl;
    }
}

void CubeInsWindow::getProgramInfoLog(GLuint id, int length) {
    GLchar *infoLog = (GLchar *)malloc(length);
    if (NULL != infoLog) {
        glGetProgramInfoLog(id, length, NULL, infoLog);
        cerr << infoLog << endl;
        free (infoLog);
    } else {
        cerr << "colud not allocate program infoLog buffer." << endl;
    }
}

int CubeInsWindow::loadTexture(string fileName, GLenum texUnit, int width, int height) {

    GLuint textureID;
    glGenTextures(1, &textureID);

    glActiveTexture(texUnit);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    ifstream fst(fileName, ios::binary);
    const size_t fileSize = static_cast<size_t>(fst.seekg(0, fst.end).tellg());
    fst.seekg(0, fst.beg);
    vector<char> textureBuffer(fileSize);
    fst.read(&textureBuffer[0], fileSize);

    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &textureBuffer[0]);

    return textureID;
}

void CubeInsWindow::makeData(int xs, int ys, int zs, int xm, int ym, int zm, float xd, float yd, float zd, 
                            float** vboV, float** vboT, float** vboP, unsigned int** indices,
                            int* numVboV, int* numVboT, int* numVboP, int *numIndices) {
    int num = (xm - xs + 1) * (ym - ys + 1) * (zm - zs + 1);
            
    const int VERT_PER_CUBE = 42;
    const int TEX_PER_CUBE = 28;
    const int POS_PER_CUBE = 3;
    const int IDX_PER_CUBE = 15;

    long memorySize = (VERT_PER_CUBE + TEX_PER_CUBE + POS_PER_CUBE * num) * sizeof(float)
                            + IDX_PER_CUBE * sizeof(unsigned int);
    cout << "memory usage = " + to_string(memorySize) + " bytes" << endl;

    *numVboV = VERT_PER_CUBE * 1;
    if (NULL == (*vboV = (GLfloat*)malloc(sizeof(GLfloat) * (*numVboV)))) {
        cerr << "failed to allocate memory vboV" << endl;
        exit(1);
    }
    *numVboT = TEX_PER_CUBE * 1;
    if (NULL == (*vboT = (GLfloat*)malloc(sizeof(GLfloat) * (*numVboT)))) {
        cerr << "failed to allocate memory vboT" << endl;
        exit(1);
    }
    *numVboP = POS_PER_CUBE * num;
    if (NULL == (*vboP = (GLfloat*)malloc(sizeof(GLfloat) * (*numVboP)))) {
        cerr << "failed to allocate memory vboP" << endl;
        exit(1);
    }
    *numIndices = IDX_PER_CUBE * 1;
    if (NULL == (*indices = (unsigned int*)malloc(sizeof(unsigned int) * (*numIndices)))) {
        cerr << "failed to allocate memory indices" << endl;
        exit(1);
    }

    float xr = 0.5f;
    float yr = 0.5f;
    float zr = 0.5f;

    float texBaseX = 0.0f;
    float texBaseY = 0.0f;
    float texUnitX = 16.0f;
    float texUnitY = 16.0f;
    float texUnitZ = 16.0f;

    const float TEX_SIZE = 128.0f;

    texBaseX /= TEX_SIZE;
    texBaseY /= TEX_SIZE;
    texUnitX /= TEX_SIZE;
    texUnitY /= TEX_SIZE;
    texUnitZ /= TEX_SIZE;

    int vi = 0;
    int ti = 0;
    int pi = 0;
    int ii = 0;

    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
    
    // 0
    (*vboV)[vi++] = x - xr;
    (*vboV)[vi++] = y - yr;
    (*vboV)[vi++] = z + zr;
    (*vboT)[ti++] = texBaseX + texUnitX * 2 + texUnitY * 2;
    (*vboT)[ti++] = texBaseY + texUnitY;
    // 1
    (*vboV)[vi++] = x + xr;
    (*vboV)[vi++] = y - yr;
    (*vboV)[vi++] = z + zr;
    (*vboT)[ti++] = texBaseX + texUnitX * 3 + texUnitY * 2;
    (*vboT)[ti++] = texBaseY + texUnitY;
    // 2
    (*vboV)[vi++] = x - xr;
    (*vboV)[vi++] = y + yr;
    (*vboV)[vi++] = z + zr;
    (*vboT)[ti++] = texBaseX + texUnitX * 2 + texUnitY * 2;
    (*vboT)[ti++] = texBaseY;
    // 3
    (*vboV)[vi++] = x + xr;
    (*vboV)[vi++] = y + yr;
    (*vboV)[vi++] = z + zr;
    (*vboT)[ti++] = texBaseX + texUnitX * 3 + texUnitY * 2;
    (*vboT)[ti++] = texBaseY;
    // 4
    (*vboV)[vi++] = x - xr;
    (*vboV)[vi++] = y - yr;
    (*vboV)[vi++] = z - zr;
    (*vboT)[ti++] = texBaseX + texUnitX * 2 + texUnitY * 2;
    (*vboT)[ti++] = texBaseY + texUnitY + texUnitZ;
    // 5
    (*vboV)[vi++] = x - xr;
    (*vboV)[vi++] = y + yr;
    (*vboV)[vi++] = z + zr;
    (*vboT)[ti++] = texBaseX + texUnitX * 2 + texUnitY;
    (*vboT)[ti++] = texBaseY + texUnitY;
    // 6
    (*vboV)[vi++] = x - xr;
    (*vboV)[vi++] = y + yr;
    (*vboV)[vi++] = z - zr;
    (*vboT)[ti++] = texBaseX + texUnitX * 2 + texUnitY;
    (*vboT)[ti++] = texBaseY + texUnitY + texUnitZ;
    // 7
    (*vboV)[vi++] = x + xr;
    (*vboV)[vi++] = y + yr;
    (*vboV)[vi++] = z + zr;
    (*vboT)[ti++] = texBaseX + texUnitX + texUnitY;
    (*vboT)[ti++] = texBaseY + texUnitY;
    // 8
    (*vboV)[vi++] = x + xr;
    (*vboV)[vi++] = y + yr;
    (*vboV)[vi++] = z - zr;
    (*vboT)[ti++] = texBaseX + texUnitX + texUnitY;
    (*vboT)[ti++] = texBaseY + texUnitY + texUnitZ;
    // 9
    (*vboV)[vi++] = x + xr;
    (*vboV)[vi++] = y - yr;
    (*vboV)[vi++] = z + zr;
    (*vboT)[ti++] = texBaseX + texUnitX;
    (*vboT)[ti++] = texBaseY + texUnitY;
    // 10
    (*vboV)[vi++] = x + xr;
    (*vboV)[vi++] = y - yr;
    (*vboV)[vi++] = z - zr;
    (*vboT)[ti++] = texBaseX + texUnitX;
    (*vboT)[ti++] = texBaseY + texUnitY + texUnitZ;
    // 11
    (*vboV)[vi++] = x - xr;
    (*vboV)[vi++] = y - yr;
    (*vboV)[vi++] = z - zr;
    (*vboT)[ti++] = texBaseX;
    (*vboT)[ti++] = texBaseY + texUnitY + texUnitZ;
    // 12
    (*vboV)[vi++] = x + xr;
    (*vboV)[vi++] = y + yr;
    (*vboV)[vi++] = z - zr;
    (*vboT)[ti++] = texBaseX + texUnitX;
    (*vboT)[ti++] = texBaseY + texUnitY * 2 + texUnitZ;
    // 13
    (*vboV)[vi++] = x - xr;
    (*vboV)[vi++] = y + yr;
    (*vboV)[vi++] = z - zr;
    (*vboT)[ti++] = texBaseX;
    (*vboT)[ti++] = texBaseY + texUnitY * 2 + texUnitZ;

    int count = 0;
    (*indices)[ii++] = (unsigned int)(count * (IDX_PER_CUBE - 1) + 3);
    (*indices)[ii++] = (unsigned int)(count * (IDX_PER_CUBE - 1) + 2);
    (*indices)[ii++] = (unsigned int)(count * (IDX_PER_CUBE - 1) + 1);
    (*indices)[ii++] = (unsigned int)(count * (IDX_PER_CUBE - 1) + 0);
    (*indices)[ii++] = (unsigned int)(count * (IDX_PER_CUBE - 1) + 4);
    (*indices)[ii++] = (unsigned int)(count * (IDX_PER_CUBE - 1) + 5);
    (*indices)[ii++] = (unsigned int)(count * (IDX_PER_CUBE - 1) + 6);
    (*indices)[ii++] = (unsigned int)(count * (IDX_PER_CUBE - 1) + 7);
    (*indices)[ii++] = (unsigned int)(count * (IDX_PER_CUBE - 1) + 8);
    (*indices)[ii++] = (unsigned int)(count * (IDX_PER_CUBE - 1) + 9);
    (*indices)[ii++] = (unsigned int)(count * (IDX_PER_CUBE - 1) + 10);
    (*indices)[ii++] = (unsigned int)(count * (IDX_PER_CUBE - 1) + 11);
    (*indices)[ii++] = (unsigned int)(count * (IDX_PER_CUBE - 1) + 12);
    (*indices)[ii++] = (unsigned int)(count * (IDX_PER_CUBE - 1) + 13);
    (*indices)[ii++] = UINT_MAX;   // primitive restart

    for (float x = xs * xd; x <= xm * xd; x+= xd) {
        for (float y = ys * yd; y <= ym * yd; y+= yd) {
            for (float z = zs * zd; z <= zm * zd; z += zd) {
                (*vboP)[pi++] = x;
                (*vboP)[pi++] = y;
                (*vboP)[pi++] = z;
            }
        }
    }
}
CubeInsWindow::~CubeInsWindow() {
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glDisable(GL_PRIMITIVE_RESTART);
    glDisable(GL_PRIMITIVE_RESTART_FIXED_INDEX);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    glDeleteVertexArrays(1, &_vaoID);
    glDeleteBuffers(1, &_vboID_V);
    glDeleteBuffers(1, &_vboID_T);
    glDeleteBuffers(1, &_vboID_P);
    glDeleteBuffers(1, &_iboID);

    if (NULL != _vertAry) {
        free(_vertAry);
    }
    if (NULL != _texAry) {
        free(_texAry);
    }
    if (NULL != _posAry) {
        free(_posAry);
    }
    if (NULL != _idxAry) {
        free(_idxAry);
    }
    glDeleteTextures(1, &_textureID);
    glDeleteProgram(_programID);
    cout << "~CubeInsWindow" << endl;
}