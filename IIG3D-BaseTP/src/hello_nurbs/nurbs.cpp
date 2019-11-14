#include <iostream>
#include "nurbs.h"


std::vector<int> Nurbs::setVectNodalNU(){
  int ordre = getOrdre();
  int nbPC = getNbPC();

  std::vector<int> vectNodal;

  for(int i=0; i < ordre; i++)
    vectNodal.push_back(0);
  for(int i=ordre; i<nbPC; i++)
    vectNodal.push_back(i-ordre+1);
  for(int i = nbPC; i<ordre+nbPC; i++)
    vectNodal.push_back(nbPC-1);

  assert(int(vectNodal.size()) == ordre+nbPC);
  return vectNodal;


}

glm::vec3 Nurbs::floraison(float t)
{
    //Determination des points d'influence
    int dec = 0;
    int ind = getOrdre();
    std::vector<int> vNodal = getVectNodal();

    while(vNodal.at(ind)<t){
      ++dec;
      ++ind;
    }

    std::vector<glm::vec3> pt = getPc();
    std::vector<float> w = getW();

    std::vector<glm::vec3> ptI;
    std::vector<float> wI;

    for(int i = 0; i < getOrdre(); i++){
      glm::vec3 p;
      p.x = pt[i+dec].x;
      p.y = pt[i+dec].y;
      p.z = pt[i+dec].z;
      wI.push_back(w[i+dec]);
      ptI.push_back(p);
    }



      //algorithme de Floraison


    int k = getOrdre();
    int l = k;

    for(int j = 0; j<k-1;j++){
      for(int i = 0; i<l-1;i++){
//         std::cout << "ptI(" << i << ") = (" << ptI.at(i).x << "," << ptI.at(i).y << "," << ptI.at(i).z << ") ; pt(" << i+1 << ") = (" << ptI.at(i+1).x << "," << ptI.at(i+1).y << "," << pt.at(i+1).z << ")" << std::endl;
        float ni =(vNodal[dec+l+i]-t) / (vNodal[dec+l+i]-vNodal[dec+1+i]);
        float nip1 = (t-vNodal[dec+1+i]) / (vNodal[dec+l+i]-vNodal[dec+1+i]);


        ptI[i].x = (ni*ptI[i].x*wI[i] + nip1*ptI[i+1].x*wI[i+1]) ;
        ptI[i].y = (ni*ptI[i].y*wI[i] + nip1*ptI[i+1].y*wI[i+1]) ;
        ptI[i].z = (ni*ptI[i].z*wI[i] + nip1*ptI[i+1].z*wI[i+1]) ;

        wI[i] = ni*wI[i] + nip1*wI[i+1];
        // std::cout << pt[i].x << "  "  << pt[i].y << "  " << pt[i].z << std::endl;
      }
      l--;
      dec++;
      // std::cout << "dec = " << dec << " l = " << l;
    }

    return ptI[0]/wI[0];
}


void Nurbs::display()
{
    for (unsigned int i = 0; i < PtsC.size(); i++)
    {
        std::cout << "(" << PtsC[i].x << "," << PtsC[i].y << "," << PtsC[i].z << ")" << std::endl;
    }

    return;
}
