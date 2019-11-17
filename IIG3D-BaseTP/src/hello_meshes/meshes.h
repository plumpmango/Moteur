#ifndef MESHES_H
#define MESHES_H

#include "opengldemo.h"
#include "../hello_camera/camera.h"

#include <memory>
#include <functional>
#include <vector>
#include <iostream>
#include <glm/glm.hpp>

class Meshes {
public:
    /*_pc  : Points de contrôle
     * _ordre : Ordre de la Meshes */
     Meshes(){};
    ~Meshes(){};
    //Initialize the mesh geometry in fic
    void initializeVerticesAndIndexes(const char* fichier);
    inline std::vector<glm::vec3> getVertices(){return _vertices;};
    inline std::vector<GLuint> getIndices(){return _indices;};
    inline int getNbSommets(){return nbSommets;};
private:
    /*sommets */
    std::vector<glm::vec3>_vertices ;

    /* faces */
    std::vector<GLuint> _indices ;
    /*nbsommets*/
    int nbSommets;

};
#endif
