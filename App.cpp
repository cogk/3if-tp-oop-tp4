/*************************************************************************
                           App  -  description
                             -------------------
    début                : 2019-11-19
    copyright            : (C) 2019 FADILI Zineb & FORLER Corentin
    e-mail               : zineb.fadili@insa-lyon.fr
                           corentin.forler@insa-lyon.fr
*************************************************************************/

//---------- Réalisation de la classe <App> (fichier App.cpp) ------------

//---------------------------------------------------------------- INCLUDE

//-------------------------------------------------------- Include système
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
using namespace std;

//------------------------------------------------------ Include personnel
#include "App.h"

//------------------------------------------------------------- Constantes

//----------------------------------------------------------------- PUBLIC

//----------------------------------------------------- Méthodes publiques
int App::Run()
{
    cout << "Bienvenue dans l'application blabla !" << endl;
    return 0;
} //----- Fin de App::Run

void App::usage(const char *progName)
{
    cerr << "Usage: " << progName
         << " [-e] [-t 0-23] [-g out.dot] [-R baseReferer] in.log"
         << endl;
}

int App::atoi(const char *str)
{
    int num = 0;

    if (str[0] == '\0')
    {
        return -1; // chaîne vide en paramètre
    }

    for (unsigned int i = 0; str[i] != 0; i++)
    {
        char c = str[i];
        if (c >= '0' && c <= '9')
        {
            num = num * 10 + (c - '0');
        }
        else
        {
            return -1;
        }
    }

    return num;
}

void App::Debug()
{
    cout << "inputFilename:       " << options.inputFilename << endl;

    cout << "shouldFilterByTime:  " << (options.shouldFilterByTime ? "true" : "false") << endl;
    cout << "filterTime:          " << options.filterTime << endl;

    cout << "shouldOutputDot:     " << (options.shouldOutputDot ? "true" : "false") << endl;
    cout << "outputDotFilename:   " << options.outputDotFilename << endl;

    cout << "shouldExcludeOthers: " << (options.shouldExcludeOthers ? "true" : "false") << endl;

    cout << "serverReferer:       " << options.serverReferer << endl;
}

int App::ReadOptions(int argc, char const *argv[])
{
    const char *progName = argv[0];

    if (argc == 1)
    {
        App::usage(progName);
        return EXIT_FAILURE;
    }

    int i = 1;
    while (i < argc)
    {
        if (!strcmp(argv[i], "-g"))
        {
            if (i == argc - 1)
            {
                App::usage(progName);
                return EXIT_FAILURE;
            }

            // On ignore les options répétées, seule la dernière valeur est prise en compte.
            // On lit ensuite le nom du fichier de sortie
            i++;
            this->options.shouldOutputDot = true;

            this->options.outputDotFilename = argv[i];
        }
        else if (!strcmp(argv[i], "-e"))
        {
            this->options.shouldExcludeOthers = true;
        }
        else if (!strcmp(argv[i], "-t"))
        {
            if (i == argc - 1)
            {
                App::usage(progName);
                return EXIT_FAILURE;
            }

            // On ignore les options répétées, seule la dernière valeur est prise en compte.
            i++;
            int hour = App::atoi(argv[i]);
            if (hour > 23 || hour < 0)
            {
                cerr << "Erreur: paramètre ‘heure’ de l'option -t"
                     << " doit être un nombre entre 0 et 23 (bornes incluses)." << endl;
                App::usage(progName);
                return EXIT_FAILURE;
            }

            this->options.filterTime = (unsigned int)hour;
            this->options.shouldFilterByTime = true;
        }
        else if (!strcmp(argv[i], "-R"))
        {
            // On ignore les options répétées, seule la dernière valeur est prise en compte.
            this->options.serverReferer = argv[i];
        }
        else
        {
            // Si le paramètre commence par un tiret -
            // Alors c'est une option invalide
            if (!strncmp(argv[i], "-", 1))
            {
                cerr << "Erreur: option non reconnue ‘" << argv[i] << "’." << endl;
                App::usage(progName);
                return EXIT_FAILURE;
            }
            else if (this->options.inputFilename == "")
            {
                // On n'a pas encore lu de nom de fichier d'entrée.
                this->options.inputFilename = argv[i];
            }
            else
            {
                cerr << "Erreur: trop d'arguments." << endl;
                App::usage(progName);
                return EXIT_FAILURE;
            }
        }

        i++;
    }

    return EXIT_SUCCESS;
}

//------------------------------------------------- Surcharges d'opérateurs

//-------------------------------------------- Constructeurs - destructeurs
App::App()
{
#ifdef MAP
    cout << "Appel au constructeur de <App>" << endl;
#endif
} //----- Fin de App

App::~App()
{
#ifdef MAP
    cout << "Appel au destructeur de <App>" << endl;
#endif
} //----- Fin de ~App
