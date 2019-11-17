#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>

#include "meshes.h"

void Meshes::initializeVerticesAndIndexes(const char* fichier){
  // const char *nomFicMesh = "../src/hello_subdivision/sphere.off";
  char delimEspace = ' ';
  char delimLigne = '\n';
  // FILE *f;
  // char c[50];
  std::ifstream f(fichier);
  if(f){
    std::string c;
    std::getline(f,c);

    std::getline(f,c,delimEspace);
    nbSommets = std::stoi(c);
    // std::cout << "Sommets : " << nbSommets << std::endl;

    std::getline(f,c,delimEspace);
    int nbFaces = std::stoi(c);
    // std::cout << "Face : " << nbFaces << std::endl;

    std::getline(f,c,delimLigne);
    int nbAretes = std::stoi(c);
    std::cout << "Arretes : " << nbAretes << std::endl;

    glm::vec3 g(0,0,0);

    for(int i = 0; i < nbSommets; i++){
      std::string v1, v2, v3;
      std::getline(f,v1,delimEspace);
      std::getline(f,v2,delimEspace);
      std::getline(f,v3,delimLigne);
      _vertices.push_back(glm::vec3(std::stof(v1),std::stof(v2),std::stof(v3)));
      g[0]+=std::stof(v1);
      g[1]+=std::stof(v2);
      g[2]+=std::stof(v1);

      // std::cout << std::stof(v1) << ", " << std::stof(v2) << ", " << std::stof(v3) << std::endl;
    }

    //Centrer le calculerMaillage
    g /= nbSommets;
    for(int i =0 ;i < nbSommets; i++){
      _vertices[i]-=g;
    }

    for(int i = 0; i < nbFaces; i++){
      std::string ind;
      std::getline(f,ind,delimEspace);
      _indices.push_back(std::stoi(ind));
      std::getline(f,ind,delimEspace);
      _indices.push_back(std::stoi(ind));
      std::getline(f,ind,delimLigne);
      _indices.push_back(std::stoi(ind));
    }

    // for(unsigned int i = 0; i< _indices.size(); i+=3){
    //   std::cout << _indices[i] << ", " << _indices[i+1] << ", " << _indices[i+2] << std::endl;
    // }

  }

}
