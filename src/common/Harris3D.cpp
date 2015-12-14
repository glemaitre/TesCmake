/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Harris3D.cpp
 * Author: lpikachu58
 * 
 * Created on 18 novembre 2015, 14:38
 */

#include "Harris3D.h"
#include <algorithm>

Harris3D::Harris3D() 
{
}

Harris3D::Harris3D(const std::string nameFile) 
{
    nameFileDataDI = nameFile;
}

Harris3D::Harris3D(const Harris3D& orig) 
{
}

Harris3D::~Harris3D() 
{

}

void Harris3D::DetectInterestPoints()
{
    double max = 0.0;
    double resp; // /!\ quel valeur initialisation???

    //Clock r;
    //double tiempo = 0.0;

    // allocation dymanique  
//    Vertex* vertices = ;

    // object->getVertices(); // vertices == les points récupérer par la constructior du mesh a priori pas nécessaire pas notre version


    //Face* faces = object->getFaces();

    //Tree tree;

    //Process each vertex
    cout << "Begin responses calculation..." << endl;
    // ici charger les points dans dans l'image de profondeur 

    LoadDataFromDI(nameFileDataDI);

    // pour tous les points de l'image de profondeur (avec effet de bords)

    for(register uint32 i = 3; i< (pts.width()-2); i++)
    {
        for(register uint32 j = 3 ; j < (pts.height()-2) ; j++)
        {
            std::vector<Vertex*> set;

            // testé si le fenêtrage ne contient pas de point z = nodata

            if (TestValeurVoisinage(i,j) == 1) // si pas calculable car point non défini dans le voisinage.
            {
                resp = std::numeric_limits<double>::max(); // on marque la matrice dans les résultats
            }
            else // on a toutes les valeurs pour calculer
            {
                // ici charger les voisins à calculer dans le fenêtrage taille = 5x5 (voir si on teste 3x3)

                GetVoisinage(i , j , &set); 

                //Process "set", first, calculate the centroid
                

                // /!\ Attention je ne va pas dans la boule for:w
                //
                double xc = 0, yc = 0, zc = 0;
                for(register int jj = 0; jj< set.size();jj++){
                    xc += set[jj]->getX();
                    yc += set[jj]->getY();
                    zc += set[jj]->getZ();
                }

                xc /= set.size();
                yc /= set.size();
                zc /= set.size();

                //Translate the vertex, in order the centroid is in [0 0 0]
                for(register int jj = 0; jj< set.size();jj++){
                    set[jj]->setX(set[jj]->getX() - xc);
                    set[jj]->setY(set[jj]->getY() - yc);
                    set[jj]->setZ(set[jj]->getZ() - zc);
                }

                //Aplicar PCA para encontrar una pose de la nube de puntos de manera que la mayor dispersión de los puntos esté en el plano XY
                //La media de las 3 coordenadas ya es (0, 0, 0), así que en realidad no es necesario calcularla, directamente calculamos la 
                //matriz de covarianza
                double A[9];
                memset(A, 0, sizeof(double)*9);

                for(register int jj = 0; jj < set.size(); jj++)
                {
                    double x = set[jj]->getX();
                    double y = set[jj]->getY();
                    double z = set[jj]->getZ();

                    A[0] += x*x;	A[1] += x*y;	A[2] += x*z;
                    A[4] += y*y;	A[5] += y*z;    
                    A[8] += z*z;
                }
                A[3] = A[1];	A[6] = A[2];	A[7] = A[5];
                for(int jj = 0; jj < 9; jj++)
                    A[jj] /= (set.size()-1);

                //Con la matriz de covarianza, calculamos PCA
                
                // ajout code Cyrille

                int32 n = 3;
                core::Matrix<float32> a1, q, evec;
                std::vector<float32> wr;

                a1.create(n, n);
                q.create(n, n);
                evec.create(n, n);

                // on place les valeurs A dans la matrice a1
                for(register int32 ii=0; ii<n; ii++ )
                    for(register int32 jj=0; jj<n; jj++)
                        a1[jj][ii]=A[jj*n+ii];

                nrc::HouseHolder(a1, wr, q);

                //trier les vecteurs propres

                TriVecteurPropre(wr, q, evec, n);
                //Sacamos las componentes del nuevo sistema de coordenadas
                double x_1 = evec[0][0];	double x_2 = evec[1][0]; double x_3 = evec[2][0];
                double y_1 = evec[0][1];	double y_2 = evec[1][1]; double y_3 = evec[2][1];
                double z_1 = evec[0][2];	double z_2 = evec[1][2]; double z_3 = evec[2][2];


                double x2 = set[0]->getX() - xc;
                double y2 = set[0]->getY() - yc;
                double z2 = set[0]->getZ() - zc;

                if((z_1*x2 + z_2*y2 + z_3*z2) < 0){
                    z_1 = -z_1;	
                    z_2 = -z_2;
                    z_3 = -z_3;

                    double aux_x1 = x_1;
                    double aux_x2 = x_2;
                    double aux_x3 = x_3;

                    x_1 = y_1;
                    x_2 = y_2;
                    x_3 = y_3;
                    y_1 = aux_x1;
                    y_2 = aux_x2;
                    y_3 = aux_x3;

                }
                //Realizamos la rotacion, con el nuevo sistema de coordenadas
                for(register int j = 0; j < set.size(); j++){
                    double x = set[j]->getX();
                    double y = set[j]->getY();
                    double z = set[j]->getZ();

                    set[j]->setX(x_1*x + x_2*y + x_3*z);
                    set[j]->setY(y_1*x + y_2*y + y_3*z);
                    set[j]->setZ(z_1*x + z_2*y + z_3*z);
                }

                //Movemos todos los puntos para que el punto de analisis se encuentre en el origen del plano XY. Solo movemos en las coordenadas X e Y
                double x = set[0]->getX();
                double y = set[0]->getY();

                for(register int j = 0; j < set.size(); j++)
                {
                    set[j]->setX(set[j]->getX() - x);
                    set[j]->setY(set[j]->getY() - y);
                }

                //Fit a quadratic surface
                double C[36];
                double D[6];

                memset(C, 0, sizeof(double)*36);
                memset(D, 0, sizeof(double)*6);

                for(register int j = 0; j < set.size(); j++)
                {
                    double x = set[j]->getX();
                    double y = set[j]->getY();
                    double z = set[j]->getZ();

                    double x2 = x*x;
                    double y2 = y*y;
                    double x3 = x2*x;
                    double y3 = y2*y;

                    C[0] += x*x3;	C[1] += x3*y;	C[2] += x2*y2;	C[3] += x3;		C[4] += x2*y;	C[5] += x2;
                    C[7] += x2*y2;	C[8] += x*y3;	C[9] += x2*y;	C[10] += x*y2;	C[11] += x*y;
                    C[14] += y*y3;	C[15] += x*y2;	C[16] += y3;	C[17] += y2;
                    C[21] += x2;	C[22] += x*y;	C[23] += x;
                    C[28] += y2;	C[29] += y;
                    D[0] += z*x2;	D[1] += z*x*y;	D[2] += z*y2;	D[3] += z*x;	D[4] += z*y;	D[5] += z; 
                }

                C[6] = C[1];
                C[12] = C[2];	C[13] = C[8];
                C[18] = C[3];	C[19] = C[9];	C[20] = C[15];
                C[24] = C[4];	C[25] = C[10];	C[26] = C[16];	C[27] = C[22];
                C[30] = C[5];	C[31] = C[11];	C[32] = C[17];	C[33] = C[23];	C[34] = C[29];

                C[35] = (double)set.size();

                float32 m1[6][6] ;
                float32 b1[6], x1[6];

                //recopie C et D dans m1 et b1
                
                for(register int32 ii=0; ii<6; ii++)
                {
                    for(register int32 jj=0; jj<6; j++)
                    {
                        m1[ii][jj]= float32(C[jj*6 +11]);
                    }
                    b1[ii]=float32(D[ii]);
                }

                nrc::LuSol(m1, 6,  b1, x1);

                // extraction des résultats
                //Extract solution of quadratic surface
                double c20_2 = (double)x1[0];
                double c11 = (double)x1[1];
                double c02_2 = (double)x1[2];
                double c10 = (double)x1[3];
                double c01 = (double)x1[4];
                double c0 = (double)x1[5];

                double c20 = (double)c20_2*2;
                double c02 = (double)c02_2*2;

                double fx2 = double(c10*c10 + 2*c20*c20 + 2*c11*c11); //A
                double fy2 = double(c10*c10 + 2*c11*c11 + 2*c02*c02); //B
                double fxfy = double(c10*c01 + 2*c20*c11 + 2*c11*c02); //C

                double k = 0.04;
                resp = fx2*fy2 - fxfy*fxfy - k*(fx2 + fy2)*(fx2 + fy2);
            }



            // mettre les les résutatss dans vertices
            // vertices[i].setResponse(resp);

            matRes[j * pts.width() + i] = resp;

            if(resp > max)
                max = resp;

            for(register int j = 0; j < set.size(); j++)
                delete set[j];
            set.clear();

        }// fin for j
    }// fin for i 


    //cout << cont << " - " << object->getNumberVertices() << endl;
    cout << "Responses calculated..." << endl;

    vector<Vertex> candidatePoints;

    // maxima locaux = valeur maximale dans le voisinage.

    //Search for local maximum à mettre dans une fonction

    for(register uint32 i = 3; i< (pts.width()-2); i++)
    {
        for(register uint32 j = 3 ; j < (pts.height()-2) ; j++)
        {
            matIsMaxiLoc[j * pts.width() +i] = RechercheMaxiLocal(i , j);
        }   // fin for j
    } // fin for i



    cout << "Candidates calculated ..." << endl;


    // sélection finales, on utilise la méthode dites de clustering qui permet d'avoir un échantillonnage spacial meilleur

    int32 distanceRecherche = 15;
    FiltrageFinal(distanceRecherche);

}


void Harris3D::LoadDataFromDI(const std::string name)
{
    // Chargement image vers pts
    LoadFile(name.c_str(),pts); 
    matRes = (double *)malloc(sizeof(double)*pts.width()*pts.height());
    matIsMaxiLoc = (bool *)malloc(sizeof(bool)*pts.width()*pts.height());
    ResFinal = (SResFinal *)malloc(sizeof(SResFinal)*pts.width()*pts.height()); 
}

void Harris3D::GetVoisinage(const uint32 i , const uint32 j , std::vector<Vertex*> * S)
{
    if(S != NULL)
    {
        for(register uint32 n = (i - 2) ; n < (i + 3) ; n++ )
            for(register uint32 m = (j-2); m < (j + 3) ; m++ )
                S->push_back(new Vertex(pts[j][i][0], pts[j][i][1], pts[j][i][2]));   
    }

}

uint32 Harris3D::TestValeurVoisinage(const uint32 i , const uint32 j)
{
    uint32 valrtn = 0;
    for(register uint32 n = (i - 2) ; n < (i + 3) ; n++ )
        for(register uint32 m = (j-2); m < (j + 3) ; m++ )
            if (pts[j][i][2] == 0) // si pas de valeurs disponible
            {
                valrtn = 1;
                n = i + 3;
                m = j + 3;
            }
    return valrtn;

}

bool Harris3D::RechercheMaxiLocal(const uint32 i , const uint32 j)
{

    // on empile ss i,j est bien un maximum
    bool valrtn = false;
    double maxi = 0;
    for(register uint32 n = (i - 2) ; n < (i + 3) ; n++ )
        for(register uint32 m = (j-2); m < (j + 3) ; m++ )
            if (matRes[j * pts.width() + i] > maxi) // si pas de valeurs disponible
            {
                matRes[j * pts.width()+1] = maxi;
                if ((n == i) && (m == j))
                {
                    valrtn = true;
                }
                else
                {
                    valrtn = false;
                    if ((n > i) && (m > j))
                        return valrtn;
                }
            }
    return valrtn;

}

void Harris3D::FiltrageFinal(const int32 distance)
{
    for(register uint32 i = 1; i< (pts.width()); i++)
    {
        for(register uint32 j = 1; j < (pts.height()); j++)
        {
            if (matIsMaxiLoc[j * pts.width() + i ] == true )//si on a un maximum local alors on balaye dans un voisinage
            {
                for(register uint32 n = (i-(distance/2)) ; n < (i+(distance/2)) ; n++ )
                        for(register uint32 m = (j-(distance/2)); m < (j+(distance/2)) ; j++ )
                        {   
                        // on vérifie qu'on soit dans l'image
                            if((n>0)&&(n<pts.width())&&(m>0)&&(m<pts.height()))
                            {
                                if(
                                        (matIsMaxiLoc[m * pts.width() + n] == true) 
                                        && 
                                        ((ResFinal[m * pts.width() + n ].valResp) > (ResFinal[j * pts.width() + i].valResp ))
                                  )
                                {
                                    //on efface la valeur car pas maxima dans le voisinage de grandeur distance    
                                    matIsMaxiLoc[m * pts.width() + n] == false;
                                    // on force la sortie des for 
                                    m = i+(distance/2);
                                    n = j+(distance/2);
                                }
                            }
                     }
            }
        }//fin j
    }//fin j    

    // on balaye une nouvelle fois pour mettre ResFinal à jours 

    for(register uint32 i = 1; i< (pts.width()); i++)
    {
        for(register uint32 j = 1; j < (pts.height()); j++)
        {
            //si maxi local et Resfinal diff 
            if(ResFinal[j*pts.width()+i].isResp != matIsMaxiLoc[j*pts.width()+i])
            {
                ResFinal[j*pts.width()+i].isResp=false;
                ResFinal[j*pts.width()+i].valResp=0.0f;
            }
        }//fin for j
    }//fin for i

}
//balayage de toute l'image de profondeur    



void Harris3D::TriVecteurPropre(const std::vector<float32> wr, const core::Matrix<float32> q, core::Matrix<float32> evec, int32 n)
{
    //creation d'un vecteur pour la LUT
    
    std::vector<float32> cpwr;
    std::vector<int32> tLUT;
    tLUT.resize(n);
    std::copy(wr.begin(), wr.end(), cpwr.begin());
    

    //tri
    std::sort(cpwr.begin(),cpwr.end());

    float32 valTab = 0.0f;
    // on remplie la LUT après tri
    for(int32 i=0; i<n; i++)
    {
        valTab = cpwr[i];
        for (int32 j=0; j<n; j++)
        {
            if(wr[j]==valTab)
            {
                tLUT[i]=j;
            }
        }
    }

    for(int32 i=0; i<n; i++)
    {
        int32 idLUT = tLUT[i];
        for(int32 j=0; j<n; j++)
        {
        evec[j][idLUT] = q[j][i];
        }
    }

}
