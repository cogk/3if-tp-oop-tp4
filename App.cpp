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
    cout << "Top ten logs:" << endl;

    ifstream logfile(options.inputFilename);
    if (logfile.fail()) // si le fichier ne s'ouvre pas bien
    {
        cerr << "Erreur: impossible d'ouvrir le fichier d'entrée." << endl;
        return EXIT_FAILURE;
    }

    readFromFile(logfile);

    logfile.close();

    // Cibles::const_iterator it = cibles.begin();
    // Cibles::const_iterator end = cibles.end();
    // while (it != end)
    // {
    //     cout << it->first << " " << it->second->GetCount() << " hits" << endl;
    //     it++;
    // }

    // Il faudra remplacer cette méthode qui utilise une priority_queue
    // par une autre, par exemple avec une classe "conteneur"
    // appelée KBestQueue :
    //   constructeur(unsigned int k), k le nombre de "meilleurs" éléments à conserver.
    //   void Ajouter(int weight, T *value), weight est l'importance de l'élément value.
    //   std::vector<T*> GetAllBest(), renvoie la liste triée des meilleurs éléments.

  /*  priority_queue<Cible *, std::vector<Cible *>, Cible::ComparePointers> q;
    Cibles::const_iterator it = cibles.begin();
    Cibles::const_iterator end = cibles.end();
    while (it != end)
    {
        q.push(it->second);
        it++;
    }

    const unsigned int N = 10; // nombre de cibles demandé
    for (unsigned int i = 0; i < N; ++i)
    {
        Cible *cible = q.top();
        cout << cible->nomCible << " " << cible->GetCount() << " hits" << endl;

        q.pop();

        if (q.empty())
        {
            break;
        }
    }

    return EXIT_SUCCESS;
} //----- Fin de App::Run*/

// une multimap pour permettre les doublons sur les nombres de hits qui est clé
// on garde la fonction compare initiale, car elle permet de comparer les clés
// on garde l'allocator, car on ne s'en sert pas
   //std::multimap<unsigned int, Cible*> ciblesMap;

   std::multimap<unsigned int, Cible*,std::greater<unsigned int>> ciblesMap;

   Cibles::const_iterator it = cibles.begin();
   Cibles::const_iterator end = cibles.end();
   while (it != end)
   {
     // on insère la pair <nbHitsTotals, Pointeur vers la cible>
       ciblesMap.insert(make_pair(it->second->nbHits,it->second));
       it++;
   }


   std::multimap<unsigned int, Cible*>::iterator itMap = ciblesMap.begin();
   std::multimap<unsigned int, Cible*>::iterator endMap = ciblesMap.end();

// check obligatoire à ce que j'ai compris sinon undefined behavior....
   if (distance(itMap, ciblesMap.end())>NBCIBLES) {
     endMap = next(ciblesMap.begin(),NBCIBLES);
   }

   while(itMap != endMap) {
     cout << itMap->second->nomCible << " " << itMap->first << " hits" << endl;
     itMap++;
   }

   return EXIT_SUCCESS;
}



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

        if (logfile.eof())
        {
            break; // fin du fichier
        }

        // on enlève le début si c'est une adresse locale
        // c'est à dire si l'adresse locale se trouve au début du referer
        if (newHit.referer.find(LOCAL_ADDRESS) == 0)
        {
            // on retire l'adresse locale
            newHit.referer = newHit.referer.substr(LOCAL_ADDRESS.length());
        }

        // cout << "Hit " << newHit.hour << " " << newHit.referer << " -> " << newHit.cible << endl;

        const string key = newHit.cible;
        Cible *value;

        Cibles::iterator oldCibleIt = cibles.find(key);
        if (oldCibleIt != cibles.end())
        {
            // si la cible existe déjà, on l'incrémente
          //  cout << "does exist" << endl;
            value = oldCibleIt->second; // on récupère le pointeur vers la cible déjà existante
        }
        else
        {
            value = new Cible(key);
            cibles.insert(CiblesContainerPair(key, value));
            // s'il n'existe pas, créer une nouvelle cible et l'ajouter a la map
        }

        // on incrémente son nombre de hits total
        value->Increment(newHit.referer);

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
