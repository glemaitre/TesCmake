/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Harris3D.h
 * Author: lpikachu58
 *
 * Created on 18 novembre 2015, 14:38
 */

#ifndef HARRIS3D_H
#define HARRIS3D_H

#include <iostream>
#include "loadfile.h"
#include "typedef.h"
#include "core.h"
#include "nrc.h"
#include "cycle.h"
#include <vector>
#include "Vertex.h"
#include <limits>
struct SResFinal
{
    double valResp ;
    bool isResp ;
};

class Harris3D {
public:
    Harris3D(const std::string nameFile); 
    Harris3D();
    void DetectInterestPoints();
    Harris3D(const Harris3D& orig);
    virtual ~Harris3D();
    bool * matIsMaxiLoc;
    SResFinal * ResFinal;
    double * matRes;
    PointCloud pts;
private:
    void LoadDataFromDI(const std::string name);
    void GetVoisinage(uint32 i , uint32 j , std::vector<Vertex*> * S);
    uint32 TestValeurVoisinage(const uint32 i , const uint32 j);
    bool RechercheMaxiLocal(const uint32 i , const uint32 j);
    std::string nameFileDataDI;
    void FiltrageFinal(const int32 distance);
    void TriVecteurPropre(const std::vector<float32> wr, const core::Matrix<float32> q, core::Matrix<float32> evec, int32 n);
};

#endif /* HARRIS3D_H */

