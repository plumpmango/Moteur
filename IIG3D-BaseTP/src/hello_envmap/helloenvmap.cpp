#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "helloenvmap.h"
#include <iostream>
#include <cmath>

static const char* vertexshader_source ="#version 410 core\n\
        layout (location = 0) in vec3 position;\n\
        uniform mat4 view;\n\
        uniform mat4 projection;\n\
        out vec3 TexCoords;\n\
        void main()\n\
        {\n\
            // Note that we read the multiplication from right to left\n\
            gl_Position = projection * view * vec4(position, 1.0f);\n\
            TexCoords = position;\n\
        }\n";

static const char* fragmentshader_source ="#version 410 core\n\
        out vec4 FragColor;\n\
        in vec3 TexCoords;\n\
        uniform samplerCube skybox;\n\
        void main()\n\
        {\n\
            FragColor = texture(skybox,TexCoords);\n\
        }\n";



SimpleEnvMap::SimpleEnvMap(int width, int height) : OpenGLDemo(width, height), _activecamera(0), _camera(nullptr) {

  std::vector<std::string> faces = {
    "../src/hello_envmap/skybox/right.jpg",
    "../src/hello_envmap/skybox/left.jpg",
    "../src/hello_envmap/skybox/top.jpg",
    "../src/hello_envmap/skybox/bottom.jpg",
    "../src/hello_envmap/skybox/front.jpg",
    "../src/hello_envmap/skybox/back.jpg"
  };
  cubemapTexture = loadCubemap(faces);

  _vertices = {
-1.0f,  1.0f, -1.0f,
-1.0f, -1.0f, -1.0f,
 1.0f, -1.0f, -1.0f,
 1.0f, -1.0f, -1.0f,
 1.0f,  1.0f, -1.0f,
-1.0f,  1.0f, -1.0f,

-1.0f, -1.0f,  1.0f,
-1.0f, -1.0f, -1.0f,
-1.0f,  1.0f, -1.0f,
-1.0f,  1.0f, -1.0f,
-1.0f,  1.0f,  1.0f,
-1.0f, -1.0f,  1.0f,

 1.0f, -1.0f, -1.0f,
 1.0f, -1.0f,  1.0f,
 1.0f,  1.0f,  1.0f,
 1.0f,  1.0f,  1.0f,
 1.0f,  1.0f, -1.0f,
 1.0f, -1.0f, -1.0f,

-1.0f, -1.0f,  1.0f,
-1.0f,  1.0f,  1.0f,
 1.0f,  1.0f,  1.0f,
 1.0f,  1.0f,  1.0f,
 1.0f, -1.0f,  1.0f,
-1.0f, -1.0f,  1.0f,

-1.0f,  1.0f, -1.0f,
 1.0f,  1.0f, -1.0f,
 1.0f,  1.0f,  1.0f,
 1.0f,  1.0f,  1.0f,
-1.0f,  1.0f,  1.0f,
-1.0f,  1.0f, -1.0f,

-1.0f, -1.0f, -1.0f,
-1.0f, -1.0f,  1.0f,
 1.0f, -1.0f, -1.0f,
 1.0f, -1.0f, -1.0f,
-1.0f, -1.0f,  1.0f,
 1.0f, -1.0f,  1.0f
  };

      _indices.push_back(0);
      for (unsigned int i = 1; i < _vertices.size() - 1; i++)
      {
          _indices.push_back(i);
          _indices.push_back(i);
      }
      _indices.push_back(_vertices.size() - 1);

      //Initialize textures
      textureId = loadCubemap(faces);

      // Initialize the geometry
      // 1. Generate geometry buffers
      glGenBuffers(1, &_vbo) ;
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
                  // 6. Copy our vertices array in a buffer for OpenGL to use
          glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
          glEnableVertexAttribArray(1);
          // 7. Copy our index array in a element buffer for OpenGL to use
          glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
          glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size()*sizeof (GLfloat), _indices.data(), GL_STATIC_DRAW);
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
      glShaderSource(fragmentshader, 1, &fragmentshader_source, NULL);
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

  SimpleEnvMap::~SimpleEnvMap() {
      glDeleteProgram(_programcolor);
      glDeleteProgram(_programnormal);
      glDeleteBuffers(1, &_vbo);
      glDeleteBuffers(1, &_ebo);
      glDeleteVertexArrays(1, &_vao) ;

  }

  unsigned int SimpleEnvMap::loadCubemap(std::vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                         0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
            );
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}

  void SimpleEnvMap::resize(int width, int height){
      OpenGLDemo::resize(width, height);
      _camera->setviewport(glm::vec4(0.f, 0.f, _width, _height));
      _projection = glm::perspective(_camera->zoom(), float(_width) / _height, 0.1f, 100.0f);
  }

  void SimpleEnvMap::draw() {
      OpenGLDemo::draw();

      glUseProgram(_program);

      _view = _camera->viewmatrix();

      if(!destroy){
        glDepthMask(GL_FALSE);
        glBindVertexArray(_vao);
        glm::mat4 view = glm::mat4(glm::mat3(_view));
        glUniformMatrix4fv( glGetUniformLocation(_program, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv( glGetUniformLocation(_program, "projection"), 1, GL_FALSE, glm::value_ptr(_projection));
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glDepthMask(GL_TRUE);
        glBindVertexArray(0);

      }


  }

  void SimpleEnvMap::mouseclick(int button, float xpos, float ypos) {
      _button = button;
      switch (_button) {
        case 0: //left
          _mousex = xpos;
          _mousey = ypos;
          _camera->processmouseclick(_button, xpos, ypos);
          break;
        case 1: //right
          _mousex = xpos;
          _mousey = ypos;
          _ptsControle.push_back(glm::vec3(_mousex,_mousey,0));
          //_camera->processmouseclick(_button, xpos, ypos);
          break;
        default:
          break;
      }
  }

  void SimpleEnvMap::mousemove(float xpos, float ypos) {
      _camera->processmousemovement(_button, xpos, ypos, true);
  }

  void SimpleEnvMap::keyboardmove(int key, double time) {
      _camera->processkeyboard(Camera_Movement(key), time);
  }

  bool SimpleEnvMap::keyboard(unsigned char k) {
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
          case 'r' :
            glGenBuffers(1, &_vbo) ;
            glGenBuffers(1, &_nbo) ;
            glGenBuffers(1, &_ebo) ;
            glGenVertexArrays(1, &_vao) ;


            return true;
          case 's':
            glDeleteBuffers(1, &_vbo);
            glDeleteBuffers(1, &_nbo);
            glDeleteBuffers(1, &_ebo);
            glDeleteVertexArrays(1, &_vao) ;
            destroy = true;
            return true;
          default:
              return false;
      }
  }
