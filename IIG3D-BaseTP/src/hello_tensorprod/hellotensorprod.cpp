#include "hellotensorprod.h"
#include "tensorprod.h"

#include <iostream>
#include <cmath>

static const char* vertexshader_source ="#version 410 core\n\
        layout (location = 0) in vec3 position;\n\
        layout (location = 1) in vec3 inormal;\n\
        uniform mat4 model;\n\
        uniform mat4 view;\n\
        uniform mat4 projection;\n\
        out vec3 normal;\n\
        void main()\n\
        {\n\
            // Note that we read the multiplication from right to left\n\
            gl_Position = projection * view * model * vec4(position, 1.0f);\n\
            normal = inormal;\n\
        }\n";

static const char* fragmentshader_source ="#version 410 core\n\
        in vec3 normal;\n\
        out vec4 color;\n\
        void main()\n\
        {\n\
            color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n\
        }\n";

static const char* fragmentshadernormal_source ="#version 410 core\n\
        in vec3 normal;\n\
        out vec4 color;\n\
        void main()\n\
        {\n\
            color = vec4(normal*0.5+0.5, 1.0f);\n\
        }\n";

SimpleTensorProd::SimpleTensorProd(int width, int height) : OpenGLDemo(width, height), _activecamera(0), _camera(nullptr) {
    std::vector<glm::vec3> _ptsControle {
        glm::vec3(-0.3,-0.1,0.2), glm::vec3(-0.1,0,0.25), glm::vec3(0,0.1,0.30),
        glm::vec3(0.1,0.05,0.2), glm::vec3(0.3,-0.05,0.15),
        glm::vec3(-0.25,0.15,0.1), glm::vec3(-0.1,0.15,0.15), glm::vec3(0,0.2,0.2),
        glm::vec3(0.1,0.3,0.15), glm::vec3(0.25,0.2,0.1),
        glm::vec3(-0.2,0.4,0), glm::vec3(-0.1,0.2,0.25), glm::vec3(0,0.2,0.15),
        glm::vec3(0.1,0.2,0), glm::vec3(0.3,0.4,-0.1)
    };

    std::vector<float> _w{
      1,1,1,
      1,1,
      1,1,1,
      1,1,
      1,1,1,
      1,1
    };

    for (unsigned int i = 0; i < _ptsControle.size(); i++)
        _indices2.push_back(i);
    afficherPtsControle = 0;

    int nbdirectrices = 3;
    int k = 3;

    TensorProd tensor = TensorProd(_ptsControle, _w, k, nbdirectrices);
    _vertices = tensor.getVertices();

    for (unsigned int i = 0; i < _vertices.size(); i++)
    {
        _normals.push_back(glm::normalize(_vertices[i]));
        _indices.push_back(i);
    }

    // Initialize the geometry
    // 1. Generate geometry buffers
    glGenBuffers(1, &_vbo) ;
    glGenBuffers(1, &_nbo) ;
    glGenBuffers(1, &_ebo) ;
    glGenVertexArrays(1, &_vao) ;
    // 2. Bind Vertex Array Object
    glBindVertexArray(_vao);
        // 3. Copy our vertices array in a buffer for OpenGL to use
        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        glBufferData(GL_ARRAY_BUFFER, _vertices.size()*sizeof (glm::vec3), _vertices.data(), GL_STATIC_DRAW);
        // 4. Then set our vertex attributes pointers
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
        // 5. Copy our normals array in a buffer for OpenGL to use
        glBindBuffer(GL_ARRAY_BUFFER, _nbo);
        glBufferData(GL_ARRAY_BUFFER, _normals.size()*sizeof (glm::vec3), _normals.data(), GL_STATIC_DRAW);
        // 6. Copy our vertices array in a buffer for OpenGL to use
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(1);
        // 7. Copy our index array in a element buffer for OpenGL to use
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size()*sizeof (GLfloat), _indices.data(), GL_STATIC_DRAW);
    //6. Unbind the VAO
    glBindVertexArray(0);

    // 1. Generate geometry buffers
    glGenBuffers(1, &_vbo2) ;
    glGenBuffers(1, &_ebo2) ;
    glGenVertexArrays(1, &_vao2) ;
    // 2. Bind Vertex Array Object
    glBindVertexArray(_vao2);
        // 3. Copy our vertices array in a buffer for OpenGL to use
        glBindBuffer(GL_ARRAY_BUFFER, _vbo2);
        glBufferData(GL_ARRAY_BUFFER, _ptsControle.size()*sizeof (glm::vec3), _ptsControle.data(), GL_STATIC_DRAW);
        // 4. Then set our vertex attributes pointers
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
        // 7. Copy our index array in a element buffer for OpenGL to use
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo2);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices2.size()*sizeof (GLfloat), _indices2.data(), GL_STATIC_DRAW);
    //6. Unbind the VAO
    glBindVertexArray(0);

    // Initialize shaders
    GLint success;
    GLchar infoLog[512]; // warning fixed size ... request for LOG_LENGTH!!!
    GLuint vertexshader, fragmentshader;

    // 1. Generate the shader
    vertexshader = glCreateShader(GL_VERTEX_SHADER);
    // 2. set the source
    glShaderSource(vertexshader, 1, &vertexshader_source, NULL);
    // 3. Compile
    glCompileShader(vertexshader);
    // 4. test for compile error
    glGetShaderiv(vertexshader, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(vertexshader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    fragmentshader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentshader, 1, &fragmentshader_source, NULL);
    glCompileShader(fragmentshader);
    glGetShaderiv(fragmentshader, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(fragmentshader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // 1. Generate the program
    _programcolor = _program = glCreateProgram();
    // 2. Attach the shaders to the program
    glAttachShader(_program, vertexshader);
    glAttachShader(_program, fragmentshader);
    // 3. Link the program
    glLinkProgram(_program);
    // 4. Test for link errors
    glGetProgramiv(_program, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(_program, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::LINK_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(fragmentshader);

    fragmentshader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentshader, 1, &fragmentshadernormal_source, NULL);
    glCompileShader(fragmentshader);
    glGetShaderiv(fragmentshader, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(fragmentshader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // 1. Generate the program
    _programnormal = glCreateProgram();
    // 2. Attach the shaders to the program
    glAttachShader(_programnormal, vertexshader);
    glAttachShader(_programnormal, fragmentshader);
    // 3. Link the program
    glLinkProgram(_programnormal);
    // 4. Test for link errors
    glGetProgramiv(_programnormal, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(_programnormal, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::LINK_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(fragmentshader);
    glDeleteShader(vertexshader);

    _cameraselector.push_back( []()->Camera*{return new EulerCamera(glm::vec3(0.f, 0.f, 1.f));} );
    _cameraselector.push_back( []()->Camera*{return new TrackballCamera(glm::vec3(0.f, 0.f, 1.f),glm::vec3(0.f, 1.f, 0.f),glm::vec3(0.f, 0.f, 0.f));} );

    _camera.reset(_cameraselector[_activecamera]());

    _camera->setviewport(glm::vec4(0.f, 0.f, _width, _height));
    _view = _camera->viewmatrix();

    _projection = glm::perspective(_camera->zoom(), float(_width) / _height, 0.1f, 100.0f);
}

SimpleTensorProd::~SimpleTensorProd() {
    glDeleteProgram(_programcolor);
    glDeleteProgram(_programnormal);
    glDeleteBuffers(1, &_vbo);
    glDeleteBuffers(1, &_nbo);
    glDeleteBuffers(1, &_ebo);
    glDeleteVertexArrays(1, &_vao) ;
    glDeleteBuffers(1, &_vbo2);
    glDeleteBuffers(1, &_ebo2);
    glDeleteVertexArrays(1, &_vao2) ;
}

void SimpleTensorProd::resize(int width, int height){
    OpenGLDemo::resize(width, height);
    _camera->setviewport(glm::vec4(0.f, 0.f, _width, _height));
    _projection = glm::perspective(_camera->zoom(), float(_width) / _height, 0.1f, 100.0f);
}

void SimpleTensorProd::draw() {
    OpenGLDemo::draw();

    glUseProgram(_program);

    _view = _camera->viewmatrix();

    glUniformMatrix4fv( glGetUniformLocation(_program, "model"), 1, GL_FALSE, glm::value_ptr(_model));
    glUniformMatrix4fv( glGetUniformLocation(_program, "view"), 1, GL_FALSE, glm::value_ptr(_view));
    glUniformMatrix4fv( glGetUniformLocation(_program, "projection"), 1, GL_FALSE, glm::value_ptr(_projection));

    glBindVertexArray(_vao);
        glEnable(GL_PROGRAM_POINT_SIZE);
        glPointSize(10);
    glDrawElements(GL_POINTS, _indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    if (afficherPtsControle)
    {
        glBindVertexArray(_vao2);
        glDrawElements(GL_POINTS, _indices2.size(), GL_UNSIGNED_INT, 0);
    }
    glBindVertexArray(0);
}

void SimpleTensorProd::mouseclick(int button, float xpos, float ypos) {
    _button = button;
    _mousex = xpos;
    _mousey = ypos;
    _camera->processmouseclick(_button, xpos, ypos);
}

void SimpleTensorProd::mousemove(float xpos, float ypos) {
    _camera->processmousemovement(_button, xpos, ypos, true);
}

void SimpleTensorProd::keyboardmove(int key, double time) {
    _camera->processkeyboard(Camera_Movement(key), time);
}

bool SimpleTensorProd::keyboard(unsigned char k) {
    switch(k) {
        case 'a' :
            afficherPtsControle++;
            afficherPtsControle %= 2;
            return true;
        case 'c' :
            _program = _programcolor;
            return true;
        case 'n' :
            _program = _programnormal;
            return true;
        case 'p':
            _activecamera = (_activecamera+1)%2;
            _camera.reset(_cameraselector[_activecamera]());
            _camera->setviewport(glm::vec4(0.f, 0.f, _width, _height));
            return true;
        default:
            return false;
    }
}
