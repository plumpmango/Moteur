#ifndef BSPLINE_H
#define BSPLINE_H

#include <vector>
#include <iostream>
#include <glm/glm.hpp>

class BSpline {
public:
    /*_pc  : Points de contrôle
     * _ordre : Ordre de la BSpline */
    BSpline(const std::vector<glm::vec3>& pc, int ordre) : _pc(pc), _ordre(ordre) {
        pas = 0.1;
        for (unsigned int i = 0; i < _ordre + _pc.size(); i++)
            _vectNodal.push_back(i);

        for (float t = _vectNodal[_ordre-1]; t < _vectNodal[_pc.size()]; t += pas)
        {
            PtsC.push_back(floraison(t));
        }
    };
    ~BSpline(){};

    /* Retourne les vertices de la courbe */
    inline std::vector<glm::vec3> getVertices() {
        return PtsC;
    };
    
    inline int getOrdre(){return _ordre;};
    inline std::vector<int> getVectNodal(){return _vectNodal;};
    inline std::vector<glm::vec3> getPc(){return _pc;};
    /* Affiche les points de la courbe */
    void display();

private:
    /* Points de contrôle */
    std::vector<glm::vec3>_pc ;
    /* Ordre de la BSpline */
    int _ordre;
    /* Vecteur nodal */
    std::vector<int> _vectNodal;

    /* Le pas de temps entre 2 points de la courbe */
    float pas;

    /* Points de la courbe */
    std::vector<glm::vec3> PtsC;


    /* Calcule la position d'un point de la courbe à l'instant t */
    glm::vec3 floraison(float  t);
};
#endif
