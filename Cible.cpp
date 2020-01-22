/*************************************************************************
                           Cible  -  description
                             -------------------
    début                : 2020-01-06
    copyright            : (C) 2019 FADILI Zineb & FORLER Corentin
    e-mail               : zineb.fadili@insa-lyon.fr
                           corentin.forler@insa-lyon.fr
*************************************************************************/

//---------- Réalisation de la classe <Cible> (fichier Cible.cpp) ------------

//---------------------------------------------------------------- INCLUDE

//-------------------------------------------------------- Include système
using namespace std;
#include <iostream>

//------------------------------------------------------ Include personnel
#include "Cible.h"

//------------------------------------------------------------- Constantes

//----------------------------------------------------------------- PUBLIC

//----------------------------------------------------- Méthodes publiques
void Cible::Increment(string referer)
// Algorithme :
//
{
  //on augmente le nombre de Hit total de la cible
    nbHitsTotal++;

    // on vérifie si le referer est déjà repertorié
    CibleReferersMap::iterator it = referers.find(referer);
    if (it != referers.end()) // si on n'est pas arrivé au bout c'est à dire le referer existe, on incrémente la liaison
    //referer -> cible
    {
        it->second++;
    }
    else // sinon on créer le referer
    {
        referers.insert(pair<string, unsigned int>(referer, 1));
    }

} //----- Fin de Increment

unsigned int Cible::GetCount()
{
    return nbHitsTotal;
}

//------------------------------------------------- Surcharge d'opérateurs
// Définition d'un opérateur de comparaison pour le tri
// pour les ordonner selon le nombre de hits
bool operator<(const Cible &cible1, const Cible &cible2)
{
    return cible1.nbHitsTotal < cible2.nbHitsTotal;
}

//-------------------------------------------- Constructeurs - destructeur
Cible::Cible(string cible) : nomCible(cible), nbHitsTotal(0), referers()
// Algorithme :
//
{
#ifdef MAP
    cout << "Appel au constructeur de <Cible>" << endl;
#endif
} //----- Fin de Cible

Cible::~Cible()
// Algorithme :
//
{
#ifdef MAP
    cout << "Appel au destructeur de <Cible>" << endl;
#endif
} //----- Fin de ~Cible

//------------------------------------------------------------------ PRIVE

//----------------------------------------------------- Méthodes protégées
