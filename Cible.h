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

//------------------------------------------------------------- Constantes

//------------------------------------------------------------------ Types

//------------------------------------------------------------------------
// Rôle de la classe <Cible>
//
//
//------------------------------------------------------------------------

class Cible
{
    //----------------------------------------------------------------- PUBLIC

    std::string nomCible; // le nom de la cible

    unsigned int nbHits; // le nombre de hits total vers la cible

    std::map<std::string, unsigned int> referers; // une map qui prend le nom du referer et le nombre de hit vers la cible en question

public:
    //----------------------------------------------------- Méthodes publiques
    // type Méthode ( liste des paramètres );
    // Mode d'emploi :
    //
    // Contrat :
    //

    //------------------------------------------------- Surcharge d'opérateurs
    Cible &operator=(const Cible &unCible) = delete;
    // On ne souhaite pas d'opérateur de copie
    // car la copie en profondeur est très coûteuse.

    friend bool operator<(const Cible &cible1, const Cible &cible2);
    // Mode d'emploi :
    //
    // Contrat :
    //

    //-------------------------------------------- Constructeurs - destructeur
    Cible(const Cible &unCible) = delete;
    // On ne souhaite pas de constructeur de copie
    // car la copie en profondeur est très coûteuse.

    Cible();
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
};

//-------------------------------- Autres définitions dépendantes de <Cible>

#endif // CIBLE_H
