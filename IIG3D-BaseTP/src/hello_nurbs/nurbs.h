#ifndef NURBS_H
#define NURBS_H

#include <vector>
#include <iostream>
#include <glm/glm.hpp>

class Nurbs {
public:
    /*_pc  : Points de contrôle
     * _ordre : Ordre de la Nurbs */
    Nurbs(const std::vector<glm::vec3>& pc, const std::vector<float> w, int ordre) : _pc(pc), _w(w), _ordre(ordre) {
        pas = 0.1;

        // for (unsigned int i = 0; i < _ordre + _pc.size(); i++)
        //     _vectNodal.push_back(i);

        _vectNodal = setVectNodalNU();

        for (float t = _vectNodal[_ordre-1]; t < _vectNodal[_pc.size()]; t += pas)
        {
            PtsC.push_back(floraison(t));
        }
    };
    ~Nurbs(){};
    // std::vector<float> nurbsBasisFunctionsRec(float t,int ordre,int i);
    /* Retourne les vertices de la courbe */
    inline std::vector<glm::vec3> getVertices() {return PtsC;};
    inline int getOrdre(){return _ordre;};
    inline std::vector<int> getVectNodal(){return _vectNodal;};
    inline std::vector<glm::vec3> getPc(){return _pc;};
    inline std::vector<float> getW(){return _w;};
    inline unsigned int getNbPC(){return _pc.size();};
    std::vector<int> setVectNodalNU();
    /* Affiche les points de la courbe */
    void display();


private:
    /* Points de contrôle */
    std::vector<glm::vec3>_pc ;

    /*Vecteur de poids associé à chaque point*/
    std::vector<float> _w;

    /* Ordre de la Nurbs */
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
