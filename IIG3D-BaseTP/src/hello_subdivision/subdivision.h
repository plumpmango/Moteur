#ifndef SUBDIVISION_H
#define SUBDIVISION_H

#include <vector>
#include <iostream>
#include <glm/glm.hpp>

class LoopSubdivision {
public:
    /*_pc  : Points de contrôle
     * _ordre : Ordre de la LoopSubdivision */
    LoopSubdivision(const std::vector<glm::vec3>& vertices, std::vector<GLuint> indices) : _vertices(vertices) _indices(indices) {};
    ~LoopSubdivision(){};
    //Initialize the mesh geometry in fic
    void initializeVerticesAndIndexes(){};
    inline std::vector<glm::vec3> getVertices(return _vertices;){};
    inline std::vector<GLuint> getIndices(){return _indices;}{};

private:
    /* Points de contrôle */
    std::vector<glm::vec3>_vertices ;

    /* Points de la courbe */
    std::vector<GLuint> _indices ;


};
#endif
