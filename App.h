/*************************************************************************
                           App  -  description
                             -------------------
    début                : 2020-01-06
    copyright            : (C) 2019 FADILI Zineb & FORLER Corentin
    e-mail               : zineb.fadili@insa-lyon.fr
                           corentin.forler@insa-lyon.fr
*************************************************************************/

//---------- Interface de la classe <App> (fichier App.h) ----------------
#if !defined(APP_H)
#define APP_H

//--------------------------------------------------- Interfaces utilisées
#include <fstream>
#include <iostream>
#include <map>
#include <string>
using namespace std;

//------------------------------------------------------ Include personnel
#include "Cible.h"

//------------------------------------------------------------- Constantes

#define NBCIBLES 10

//------------------------------------------------------------------ Types
typedef std::map<string, Cible *> Cibles;
typedef std::pair<string, Cible *> CiblesContainerPair;


//------------------------------------------------------------------------
// Rôle de la classe <App>
// La classe App sert de chef d'orchestre de tout le programme.
//------------------------------------------------------------------------

class App
{
    //----------------------------------------------------------------- PUBLIC


public:
    //----------------------------------------------------- Méthodes publiques
    // Mode d'emploi :
    // Cette méthode est le point d'entrée dans le programme
    // Elle renvoie le code de retour du programme, qui est :
    //   * 0 s'il n'y a pas eu d'erreur
    //   * 1 s'il y a eu une erreur fatale
    // Contrat :
    // La valeur de retour est soit 0 soit 1.
    int Run();

    // renvoie status FAILURE ou SUCCESS
    int ReadOptions(int argc, char const *argv[]);

    void Debug() const;

    struct Options
    {
        string inputFilename = "";

        string outputDotFilename = "";
        bool shouldOutputDot = false;

        bool shouldExcludeOthers = false;

        unsigned int filterTime = 0;
        bool shouldFilterByTime = false;

        string serverReferer = "http://intranet-if.insa-lyon.fr";
    };

    App();

    ~App();


    //----------------------------------------------------- Méthodes protégées
protected:
    static void usage(const char *progName);
    static int atoi(const char *str);

    void readFromFile(ifstream &logfile);

    //----------------------------------------------------- Attributs protégés
    Options options;
    Cibles cibles;
};

//-------------------------------- Autres définitions dépendantes de <App>

#endif // APP_H
