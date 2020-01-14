/*************************************************************************
                           App  -  description
                             -------------------
    début                : 2019-11-19
    copyright            : (C) 2019 FADILI Zineb & FORLER Corentin
    e-mail               : zineb.fadili@insa-lyon.fr
                           corentin.forler@insa-lyon.fr
*************************************************************************/

//---------- Interface de la classe <App> (fichier App.h) ----------------
#if !defined(APP_H)
#define APP_H

//--------------------------------------------------- Interfaces utilisées
#include <iostream>

//------------------------------------------------------ Include personnel

//------------------------------------------------------------- Constantes

//------------------------------------------------------------------ Types

//------------------------------------------------------------------------
// Rôle de la classe <App>
// La classe App sert de chef d'orchestre de tout le programme.
//------------------------------------------------------------------------

class App
{
    //----------------------------------------------------------------- PUBLIC

    struct Options
    {
        std::string inputFilename = "";

        std::string outputDotFilename = "";
        bool shouldOutputDot = false;

        bool shouldExcludeOthers = false;

        unsigned int filterTime = 0;
        bool shouldFilterByTime = false;

        std::string serverReferer = "http://intranet-if.insa-lyon.fr";
    };

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

    void Debug();

    App();

    ~App();

    //----------------------------------------------------- Méthodes protégées
protected:
    static void usage(const char *progName);
    static int atoi(const char *str);

    //----------------------------------------------------- Attributs protégés
    Options options;
};

//-------------------------------- Autres définitions dépendantes de <App>

#endif // APP_H
