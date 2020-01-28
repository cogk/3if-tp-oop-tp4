/*************************************************************************
                           Cible  -  description
                             -------------------
    début                : 2020-01-06
    copyright            : (C) 2019 BERTHOMET Guillaume & FORLER Corentin
    e-mail               : guillaume.berthomet@insa-lyon.fr
                           corentin.forler@insa-lyon.fr
*************************************************************************/

//---------- Interface de la classe <Cible> (fichier Cible.h) ----------------
#if !defined(CIBLE_H)
#define CIBLE_H

//--------------------------------------------------- Interfaces utilisées
#include <map>
#include <string>
using namespace std;

//------------------------------------------------------------- Constantes

//------------------------------------------------------------------ Types

typedef map<string, unsigned int> CibleReferersMap;

//------------------------------------------------------------------------
// Rôle de la classe <Cible>
//
//
//------------------------------------------------------------------------

class Cible
{
    //----------------------------------------------------------------- PUBLIC
public:
    //----------------------------------------------------- Méthodes publiques
    void Increment(string referer);
    // Mode d'emploi :
    //
    // Contrat :
    //

    unsigned int GetCount();
    // Mode d'emploi :
    //
    // Contrat :
    //

    //------------------------------------------------- Surcharge d'opérateurs
    Cible &operator=(const Cible &unCible) = delete;
    // On ne souhaite pas d'opérateur de copie
    // car la copie en profondeur est très coûteuse.

    //-------------------------------------------- Constructeurs - destructeur
    Cible(const Cible &unCible) = delete;
    // On ne souhaite pas de constructeur de copie
    // car la copie en profondeur est très coûteuse.

    Cible(string nomCible);
    // Mode d'emploi :
    //
    // Contrat :
    //

    virtual ~Cible();
    // Mode d'emploi :
    //
    // Contrat :
    //

    //------------------------------------------------------------------ PRIVE

protected:
    //----------------------------------------------------- Méthodes protégées

    //----------------------------------------------------- Attributs protégés

    string nomCible; // le nom de la cible

    unsigned int nbHitsTotal; // le nombre de hits total vers la cible

    CibleReferersMap referers; // une map qui prend le nom du referer et le nombre de hit vers la cible en question
};

//-------------------------------- Autres définitions dépendantes de <Cible>

#endif // CIBLE_H
