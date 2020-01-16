/*************************************************************************
                           App  -  description
                             -------------------
    début                : 2020-01-06
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
#include <map>
#include <queue>
#include <string>
using namespace std;

//------------------------------------------------------ Include personnel
#include "App.h"
#include "Cible.h"
#include "Hit.h"

//------------------------------------------------------------- Constantes

//----------------------------------------------------------------- PUBLIC

//----------------------------------------------------- Méthodes publiques
int App::Run()
{
    cout << "All logs:" << endl;

    ifstream logfile(options.inputFilename);
    if (logfile.fail()) // si le fichier ne s'ouvre pas bien
    {
        cerr << "Erreur: impossible d'ouvrir le fichier d'entrée." << endl;
        return EXIT_FAILURE;
    }

    readFromFile(logfile);

    logfile.close();

    return EXIT_SUCCESS;
} //----- Fin de App::Run

// méthode qui permet d'analyser le fichier
void App::readFromFile(ifstream &logfile)
{
    const string LOCAL_ADDRESS = options.serverReferer;
    Hit newHit;

    // on ajoute les hits à notre priority queue
    while (!logfile.eof())
    {
        logfile >> newHit; // lecture du hit grace à la surcharge d'opérateur >>
                           //
                           // il faut verifier si newHit.cible existe déjà dans le priority queue de cible, je ne sais pas comment faire? parce que qu'il faut comparer
                           // newHit.cible avec Cible.nomCible, il faut comprarer les noms

        // on enlève le début si c'est une adresse locale
        // c'est à dire si l'adresse locale se trouve au début du referer
        if (newHit.referer.find(LOCAL_ADDRESS) == 0)
        {
            // on retire l'adresse locale
            newHit.referer = newHit.referer.substr(LOCAL_ADDRESS.length());
        }

        cout << "Hit " << newHit.hour << " " << newHit.referer << " -> " << newHit.cible << endl;

        // Cible *cible = new Cible();
        // cibles.push(cible);

        // s'il existe, incrémenter son nombre de hits total
        // et checker dans sa map si le referer existe déjà. s'il existe incrémenter le nombre lui correspondant, si le referer n'existe pas le incrémenter

        // s'il n'existe pas, créer une nouvelle cible et l'ajouter a la priority queue
    }
}

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

void App::Debug() const
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
