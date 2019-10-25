#ifndef SIMPLESHADOWMAP_H
#define SIMPLESHADOWMAP_H

#include "opengldemo.h"

#include "../hello_camera/camera.h"

#include <memory>
#include <functional>

class SimpleShadowMap : public OpenGLDemo {
public:
    explicit SimpleShadowMap(int width, int height);
    ~SimpleShadowMap() override;

    void resize(int width, int height) override;
    void draw() override;

    void mouseclick(int button, float xpos, float ypos) override;
    void mousemove(float xpos, float ypos) override;
    void keyboardmove(int key, double time) override;
    bool keyboard(unsigned char k) override;
private:
    // A simple geometry
    std::vector<glm::vec3> _vertices;
    std::vector<GLfloat> _normals;
    std::vector<GLuint> _indices;

    std::vector<glm::vec3> _ptsControle;
    std::vector<GLuint> _indices2;
    int afficherPtsControle;

    // OpenGL object for geometry
    // Vertex Array Buffer
    GLuint _vao;
    GLuint _vao2;
    // Vertex Buffer Object
    GLuint _vbo;
    GLuint _vbo2;
    // Normal buffer
    GLuint _nbo;
    // Face buffer
    GLuint _ebo;
    GLuint _ebo2;

    // Shader program for rendering
    GLuint _program;

    // Different availableprograms
    GLuint _programcolor;
    GLuint _programnormal;

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

#endif // SIMPLESHADOWMAP_H
