/*************************************************************************
                           Hit  -  description
                             -------------------
    début                : 2020-01-06
    copyright            : (C) 2019 FADILI Zineb & FORLER Corentin
    e-mail               : zineb.fadili@insa-lyon.fr
                           corentin.forler@insa-lyon.fr
*************************************************************************/

//---------- Interface de la classe <Hit> (fichier Hit.h) ----------------
#if !defined(HIT_H)
#define HIT_H

//--------------------------------------------------- Interfaces utilisées
#include <fstream>
#include <iostream>
#include <string>

//------------------------------------------------------------- Constantes

//------------------------------------------------------------------ Types

//------------------------------------------------------------------------
// Rôle de la classe <Hit>
// Cette classe sert de structure de donnée qui contient les informations
// nécessaires à la représentation d'une ligne de fichier log.
// Elle contient donc le nom du referer et de la cible, ainsi que l'heure
// de l'accès à la ressource.
//------------------------------------------------------------------------

class Hit
{
    //----------------------------------------------------------------- PUBLIC
public:
    //----------------------------------------------------- Méthodes publiques
    friend class App;

    //------------------------------------------------- Surcharge d'opérateurs
    Hit &operator=(const Hit &unHit) = delete;
    // Pas d'opérateur d'assignation.

    friend ifstream &operator>>(ifstream &fichierLog, Hit &unHit);

    //-------------------------------------------- Constructeurs - destructeur
    Hit(const Hit &unHit) = delete;
    // Pas de constructeur de copie, inutile

    Hit();
    // Constructeur vide, les propriétés sont initialisées à :
    // - cible : chaîne vide
    // - referer : chaîne vide
    // - hour : 0

    virtual ~Hit();

    //------------------------------------------------------------------ PRIVE

protected:
    //----------------------------------------------------- Méthodes protégées

    //----------------------------------------------------- Attributs protégés

    std::string cible;
    std::string referer;
    unsigned int hour;
};

//-------------------------------- Autres définitions dépendantes de <Hit>

#endif // HIT_H
