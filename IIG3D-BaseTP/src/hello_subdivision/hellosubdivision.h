#ifndef HELLOSUBDIVISION_H
#define HELLOSUBDIVISION_H

#include "opengldemo.h"
#include "../hello_camera/camera.h"

#include <memory>
#include <functional>
#include <map>

struct DirLight{
    glm::vec3 dir;
    glm::vec3 color;
};

class Subdivision : public OpenGLDemo {
public:
    explicit Subdivision(int width, int height);
    ~Subdivision() override;

    int addMidVertex(std::map<std::pair<int, int>, int>& visited, int index1, int index2);
    void subdivide();
    void draw() override;
    void resize(int width, int height) override;

    void mouseclick(int button, float xpos, float ypos) override;
    void mousemove(float xpos, float ypos) override;
    void keyboardmove(int key, double time) override;

    bool keyboard(unsigned char k) override;
private:
    // A simple geometry
    std::vector<glm::vec3> _vertices;
    std::vector<glm::vec3> _normals;
    std::vector<GLuint> _indices;

    // OpenGL object for geometry
    // Vertex Array Buffer
    GLuint _vao;
    // Vertex Buffer Object
    GLuint _vbo;
    // Normal buffer
    GLuint _nbo;
    // Face buffer
    GLuint _ebo;

    // Shader program for rendering
    GLuint _program;

    // Different availableprograms
    GLuint _programcolor;
    GLuint _programnormal;
    GLuint _programdist;

    // for mouse management
    int _button; // 0 --> left. 1 --> right. 2 --> middle. 3 --> other
    float _mousex{0};
    float _mousey{0};

    // Camera
    using CameraSelector=std::function<Camera*()>;
    std::vector<CameraSelector> _cameraselector;
    unsigned int _activecamera;

    std::unique_ptr<Camera> _camera;

    // matrices
    glm::mat4 _model;
    glm::mat4 _view;
    glm::mat4 _projection;

    // light
    struct DirLight light;
};

#endif // HELLOSUBDIVISION_H
