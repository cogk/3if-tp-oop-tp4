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
    nbHits++;

    CibleReferersMap::iterator it = referers.find(referer);
    if (it != referers.end())
    {
        it->second++;
    }
    else
    {
        referers.insert(pair<string, unsigned int>(referer, 1));
    }

} //----- Fin de Increment

unsigned int Cible::GetCount()
{
    return nbHits;
}

//------------------------------------------------- Surcharge d'opérateurs
// Définition d'un opérateur de comparaison pour le tri
// pour les ordonner selon le nombre de hits
bool operator<(const Cible &cible1, const Cible &cible2)
{
    return cible1.nbHits < cible2.nbHits;
}

//-------------------------------------------- Constructeurs - destructeur
Cible::Cible(string cible) : nomCible(cible), nbHits(0), referers()
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
