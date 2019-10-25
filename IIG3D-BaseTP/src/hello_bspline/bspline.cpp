#include <iostream>
#include "bspline.h"

glm::vec3 BSpline::floraison(float t)
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

    std::vector<glm::vec3> ptI;
    for(int i = 0; i < getOrdre(); i++){
      glm::vec3 p;
      p.x = pt.at(i+dec).x;
      p.y = pt.at(i+dec).y;
      p.z = pt.at(i+dec).z;
      ptI.push_back(p);
    }

//     std::cout << "\nPoints interets : " << std::endl;
//     for(unsigned int i = 0; i<ptI.size(); i++){
//       std::cout << "(" << ptI.at(i).x << "," << ptI.at(i).y << "," << ptI.at(i).z << "," << ")"<< std::endl;
//     }
    //assert(pt.size() =getOrdre());

      //algorithme de Floraison
    int k = getOrdre();
    int l = k;
    for(int j = 0; j<k-1;j++){
      for(int i = 0; i<l-1;i++){
//         std::cout << "ptI(" << i << ") = (" << ptI.at(i).x << "," << ptI.at(i).y << "," << ptI.at(i).z << ") ; pt(" << i+1 << ") = (" << ptI.at(i+1).x << "," << ptI.at(i+1).y << "," << pt.at(i+1).z << ")" << std::endl;
        ptI.at(i).x = (vNodal[dec+l+i]-t) / (vNodal[dec+l+i]-vNodal[dec+1+i])*ptI[i].x + (t-vNodal[dec+1+i]) / (vNodal[dec+l+i]-vNodal[dec+1+i])*ptI[i+1].x;
        ptI.at(i).y = (vNodal[dec+l+i]-t) / (vNodal[dec+l+i]-vNodal[dec+1+i])*ptI[i].y + (t-vNodal[dec+1+i]) / (vNodal[dec+l+i]-vNodal[dec+1+i])*ptI[i+1].y;
        ptI.at(i).z = (vNodal[dec+l+i]-t) / (vNodal[dec+l+i]-vNodal[dec+1+i])*ptI[i].z + (t-vNodal[dec+1+i]) / (vNodal[dec+l+i]-vNodal[dec+1+i])*ptI[i+1].z;
        // std::cout << pt[i].x << "  "  << pt[i].y << "  " << pt[i].z << std::endl;
      }
      l--;
      dec++;
      // std::cout << "dec = " << dec << " l = " << l;
    }

    return ptI[0];
}


void BSpline::display()
{
    for (unsigned int i = 0; i < PtsC.size(); i++)
    {
        std::cout << "(" << PtsC[i].x << "," << PtsC[i].y << "," << PtsC[i].z << ")" << std::endl;
    }

    return;
}
