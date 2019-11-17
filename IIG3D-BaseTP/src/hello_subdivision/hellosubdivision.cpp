#include "hellosubdivision.h"

#include <iostream>
#include <cmath>

static const char* vertexshader_source ="#version 410 core\n\
        layout (location = 0) in vec3 position;\n\
        layout (location = 1) in vec3 inormal;\n\
        uniform mat4 model;\n\
        uniform mat4 view;\n\
        uniform mat4 projection;\n\
        out vec3 normal;\n\
        out vec3 pos;\n\
        void main()\n\
        {\n\
            // Note that we read the multiplication from right to left\n\
            gl_Position = projection * view * model * vec4(position, 1.0f);\n\
            normal = inormal;\n\
            pos = position;\n\
        }\n";

static const char* fragmentshadernormal_source ="#version 410 core\n\
        in vec3 normal;\n\
        out vec4 color;\n\
        void main()\n\
        {\n\
            //color = vec4(vec3(clamp(dot(normalize(normal), vec3(0,0,1)), 0, 1)), 1.0);\n\
            color = vec4(normalize(normal)*0.5+0.5, 1.0);\n\
        }\n";

static const char* fragmentshaderdist_source ="#version 410 core\n\
        in vec3 normal;\n\
        in vec3 pos;\n\
        out vec4 color;\n\
        void main()\n\
        {\n\
            float distfromsphere = 1.0 - length(pos);\n\
            distfromsphere = 30*distfromsphere;\n\
            color = vec4(distfromsphere, 1.0 - 0.7*distfromsphere, 0.0, 1.0);\n\
            //color = vec4(pos, 1.0);\n\
        }\n";

static const char* fragmentshader_source ="#version 410 core\n\
        in vec3 normal;\n\
        out vec4 color;\n\
        uniform vec3 lightdir;\n\
        uniform vec3 lightcolor;\n\
        void main()\n\
        {\n\
            vec3 ldir = normalize(-lightdir);\n\
            float diff = max(dot(normal, ldir), 0.0f);\n\
            vec3 diffuse = diff * lightcolor;\n\
            vec3 res = vec3(1.0f, 0.5f, 0.2f) * diffuse;\n\
            color = vec4(res, 1.0f);\n\
        }\n";


Subdivision::Subdivision(int width, int height) : OpenGLDemo(width, height), _activecamera(0), _camera(nullptr) {

    Meshes meshSphere;
    meshSphere.initializeVerticesAndIndexes("../src/hello_meshes/sphere.off");
    int nbVertices = meshSphere.getNbSommets();
    std::cout << nbVertices << std::endl;
    _vertices = meshSphere.getVertices();

    _normals = std::vector<glm::vec3>(nbVertices);
    int index = 0;
    for (glm::vec3 vertex : _vertices)
        _normals[index++] = glm::normalize(vertex);

    // std::cout << _vertices.size() << std::endl;
    _indices = meshSphere.getIndices();
    // std::cout << _indices.size() << std::endl;

    for (int i = 0; i < 1; ++i)
        subdivide();

    // light
    light.dir = glm::normalize(glm::vec3(-0.5, -0.5, -0.5));
    light.color = glm::vec3(1.0, 1.0, 1.0);

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
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof (GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(1);
        // 7. Copy our index array in a element buffer for OpenGL to use
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size()*sizeof (GLuint), _indices.data(), GL_STATIC_DRAW);
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
    glShaderSource(fragmentshader, 1, &fragmentshaderdist_source, NULL);
    glCompileShader(fragmentshader);
    glGetShaderiv(fragmentshader, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(fragmentshader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // 1. Generate the program
    _programdist = glCreateProgram();
    // 2. Attach the shaders to the program
    glAttachShader(_programdist, vertexshader);
    glAttachShader(_programdist, fragmentshader);
    // 3. Link the program
    glLinkProgram(_programdist);
    // 4. Test for link errors
    glGetProgramiv(_programdist, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(_programdist, 512, NULL, infoLog);
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

    _cameraselector.push_back( []()->Camera*{return new EulerCamera(glm::vec3(0.f, 0.f, 3.f));} );
    _cameraselector.push_back( []()->Camera*{return new TrackballCamera(glm::vec3(0.f, 0.f, 1.f),glm::vec3(0.f, 1.f, 0.f),glm::vec3(0.f, 0.f, 0.f));} );

    _camera.reset(_cameraselector[_activecamera]());

    _camera->setviewport(glm::vec4(0.f, 0.f, _width, _height));
    _view = _camera->viewmatrix();

    _projection = glm::perspective(_camera->zoom(), float(_width) / _height, 0.1f, 100.0f);
}

Subdivision::~Subdivision() {
    glDeleteProgram(_programcolor);
    glDeleteProgram(_programnormal);
    glDeleteBuffers(1, &_vbo);
    glDeleteBuffers(1, &_nbo);
    glDeleteBuffers(1, &_ebo);
    glDeleteVertexArrays(1, &_vao) ;
}

int Subdivision::addMidVertex(std::map<std::pair<int, int>, int>& visited, int index1, int index2){
    std::pair<int, int> edge(index1, index2);
    if (edge.first > edge.second)
        std::swap(edge.first, edge.second);

    auto inserted = visited.insert({edge, _vertices.size()});
    // insert new vertex
    if (inserted.second) {
        auto v1 = _vertices[index1];
        auto v2 = _vertices[index2];

        // put new vertex in middle
        glm::vec3 new_vertex = glm::vec3((v1[0] + v2[0])*0.5,(v1[1] + v2[1])*0.5,(v1[2] + v2[2])*0.5);
        new_vertex = glm::normalize(new_vertex);
        _vertices.push_back(new_vertex);
        _normals.push_back(new_vertex);
    }
    // return index of new vertice
    return inserted.first->second;
}

void Subdivision::subdivide() {
    std::map<std::pair<int, int>, int> visited;
    std::vector<GLuint> new_indices;

    // for each face
    for (size_t i = 0; i <  _indices.size(); i += 3) {
        int index1 = _indices[i];
        int index2 = _indices[i+1];
        int index3 = _indices[i+2];
        glm::ivec3 mid;

        // get new vertices
        mid[0] = addMidVertex(visited, index1, index2);
        mid[1] = addMidVertex(visited, index2, index3);
        mid[2] = addMidVertex(visited, index3, index1);

        // create new faces
        new_indices.push_back(index1);
        new_indices.push_back(mid[0]);
        new_indices.push_back(mid[2]);

        new_indices.push_back(index2);
        new_indices.push_back(mid[1]);
        new_indices.push_back(mid[0]);

        new_indices.push_back(index3);
        new_indices.push_back(mid[2]);
        new_indices.push_back(mid[1]);

        new_indices.push_back(mid[0]);
        new_indices.push_back(mid[1]);
        new_indices.push_back(mid[2]);
    }

    _indices.clear();
    _indices = new_indices;
}

void Subdivision::draw() {
    OpenGLDemo::draw();

    glUseProgram(_program);

    _view = _camera->viewmatrix();

    glUniformMatrix4fv( glGetUniformLocation(_program, "model"), 1, GL_FALSE, glm::value_ptr(_model));
    glUniformMatrix4fv( glGetUniformLocation(_program, "view"), 1, GL_FALSE, glm::value_ptr(_view));
    glUniformMatrix4fv( glGetUniformLocation(_program, "projection"), 1, GL_FALSE, glm::value_ptr(_projection));

    // Set light
    glUniform3fv(glGetUniformLocation(_program, "lightdir"), 1, glm::value_ptr(light.dir));
    glUniform3fv(glGetUniformLocation(_program, "lightcolor"), 1, glm::value_ptr(light.color));

    glBindVertexArray(_vao);
    glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Subdivision::resize(int width, int height){
    OpenGLDemo::resize(width, height);
    _camera->setviewport(glm::vec4(0.f, 0.f, _width, _height));
    _projection = glm::perspective(_camera->zoom(), float(_width) / _height, 0.1f, 100.0f);
}

void Subdivision::mouseclick(int button, float xpos, float ypos) {
    _button = button;
    _mousex = xpos;
    _mousey = ypos;
    _camera->processmouseclick(_button, xpos, ypos);
}

void Subdivision::mousemove(float xpos, float ypos) {
    _camera->processmousemovement(_button, xpos, ypos, true);
}

void Subdivision::keyboardmove(int key, double time) {
    _camera->processkeyboard(Camera_Movement(key), time);
}

bool Subdivision::keyboard(unsigned char k) {
    switch(k) {
        case 'd' :
            _program = _programdist;
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
