#ifndef TENSORPROD_H
#define TENSORPROD_H

#include <vector>
#include <iostream>
#include <glm/glm.hpp>

#include "../hello_bspline/bspline.h"
#include "../hello_nurbs/nurbs.h"

class TensorProd {
public:
    TensorProd(const std::vector<glm::vec3>& pc, std::vector<float> w, int ordre, int n) : pc(pc), w(w), ordre(ordre) {

        vec_directrices = construireDirectrices(n);
        vec_generatrice = construireGeneratrice(n);

    }

    ~TensorProd(){};
    inline std::vector<glm::vec3> getPc(){return pc;};
    inline int getOrdre(){return ordre;};
    inline int getVec_directricesSize(){return vec_directrices[0].size();};
    inline std::vector<glm::vec3> getVertices() {return vec_generatrice;};
    std::vector<std::vector<glm::vec3>> construireDirectrices(int n);
    std::vector<glm::vec3> construireGeneratrice(int n);


private:
    /* Points de contrôle */
    std::vector<glm::vec3> pc;

    /*Poids points*/
    std::vector<float> w;
    /* Ordre des BSpline */
    int ordre;


    std::vector<std::vector<glm::vec3>> vec_directrices;

    /* Points de la surface */
    std::vector<glm::vec3> vec_generatrice;
};
#endif
