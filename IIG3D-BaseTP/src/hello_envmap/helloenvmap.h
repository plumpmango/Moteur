#ifndef SIMPLEENVMAP_H
#define SIMPLEENVMAP_H

#include "opengldemo.h"

#include "../hello_camera/camera.h"


#include <memory>
#include <functional>

class SimpleEnvMap : public OpenGLDemo {
public:
    explicit SimpleEnvMap(int width, int height);
    ~SimpleEnvMap() override;

    unsigned int loadCubemap(std::vector<std::string> faces);
    void resize(int width, int height) override;
    void draw() override;
    void calculerMaillage();
    void mouseclick(int button, float xpos, float ypos) override;
    void mousemove(float xpos, float ypos) override;
    void keyboardmove(int key, double time) override;
    bool keyboard(unsigned char k) override;
private:

    //texture
    unsigned int textureId;
    unsigned int cubemapTexture;

    // A simple geometry
    std::vector<float> _vertices;
    std::vector<GLfloat> _normals;
    std::vector<GLuint> _indices;

    std::vector<glm::vec3> _ptsControle;
    std::vector<GLuint> _indices2;
    int afficherPtsControle;

    //geometry Object
    std::vector<glm::vec3> _verticesObj;
    std::vector<glm::vec3> _normalsObj;
    std::vector<GLuint> _indicesObj;
    // OpenGL object for geometry
    // Vertex Array Buffer
    GLuint _vao;
    GLuint _vaoObj;
    // Vertex Buffer Object
    GLuint _vbo;
    GLuint _vboObj;

    // Normal buffer
    GLuint _nbo;
    GLuint _nboObj;

    // Face buffer
    GLuint _ebo;
    GLuint _ebo2;
    GLuint _eboObj;

    // Shader program for rendering
    GLuint _program;
    GLuint _programObj;


    // Different availableprograms
    GLuint _programcolor;
    GLuint _programnormal;
    GLuint _programdist;
    GLuint _programcolorObj;
    GLuint _programnormalObj;
    // for mouse management
    int _button; // 0 --> left. 1 --> right. 2 --> middle. 3 --> other
    float _mousex{0};
    float _mousey{0};

    //Verifier
    bool destroy = false;

    // Camera
    using CameraSelector=std::function<Camera*()>;
    std::vector<CameraSelector> _cameraselector;
    unsigned int _activecamera;

    std::unique_ptr<Camera> _camera;

    // matrices
    glm::mat4 _model;
    glm::mat4 _view;
    glm::mat4 _projection;
};

#endif // SIMPLEENVMAP_H
