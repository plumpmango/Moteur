#include "tensorprod.h"

std::vector<std::vector<glm::vec3>> TensorProd::construireDirectrices(int n){
  std::vector<glm::vec3> P = getPc();
  int m = P.size() / n;
  /* Construction des directrices */
  std::vector<std::vector<glm::vec3>> directrices;
  for (int v = 0; v < n; v++)
  {
      std::vector<glm::vec3> tmp;
      std::vector<float> pds;
      for (int u = 0; u < m; u++){
          tmp.push_back(P[u + v*m]);
          pds.push_back(w[u+v*m]);
      }
      Nurbs dir(tmp, pds, getOrdre());
      directrices.push_back(dir.getVertices());
  }

  return directrices;
}

std::vector<glm::vec3> TensorProd::construireGeneratrice(int n){
  int m = getVec_directricesSize();

  std::vector<glm::vec3> PtsControle;

  for (int u = 0; u < m; u++)
  {
      std::vector<glm::vec3> tmp;
      std::vector<float> pds;
      for (int v = 0; v < n; v++){
        tmp.push_back(vec_directrices[v][u]);
        pds.push_back(1);
      }

      Nurbs gen(tmp,pds, getOrdre());
      tmp = gen.getVertices();
      PtsControle.insert(PtsControle.end(), tmp.begin(), tmp.end());
  }

  return PtsControle;
}
