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
#include <array>
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
const string PROG_NAME = "analog";

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

    ShowStatistics();

    if (options.shouldOutputDot)
    {
        if (writeDotGraph() == EXIT_FAILURE)
        {
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
} //----- Fin de App::Run

int App::writeDotGraph() const
{
    ofstream dotfile(options.outputDotFilename);
    if (dotfile.fail()) // si le fichier ne s'ouvre pas bien
    {
        cerr << "Erreur: impossible d'ouvrir le fichier de sortie." << endl;
        return EXIT_FAILURE;
    }

    dotfile << "digraph {" << endl;

    unsigned int nodeCount = 0;
    typedef map<string, string> NodeMap;
    NodeMap mapCibleToNodeName;

    Cibles::const_iterator it = cibles.begin();
    const Cibles::const_iterator end = cibles.end();
    while (it != end)
    {
        const string cibleName = it->first;
        const CibleReferersMap refs = it->second->referers;

        string nodeName1;

        const NodeMap::const_iterator cibleToNodeName = mapCibleToNodeName.find(cibleName);
        if (cibleToNodeName == mapCibleToNodeName.end())
        {
            // clé non-trouvée
            nodeName1 = "node" + std::to_string(nodeCount);
            nodeCount++;
            mapCibleToNodeName.insert(pair<string, string>(cibleName, nodeName1));
            dotfile << nodeName1 << " [label=\"" << cibleName << "\"];" << endl;
        }
        else
        {
            nodeName1 = cibleToNodeName->second;
        }

        CibleReferersMap::const_iterator it2 = refs.begin();
        const CibleReferersMap::const_iterator end2 = refs.end();
        while (it2 != end2)
        {
            const string refName = it2->first;
            const unsigned int count = it2->second;

            string nodeName2;

            const NodeMap::const_iterator cibleToNodeName = mapCibleToNodeName.find(refName);
            if (cibleToNodeName == mapCibleToNodeName.end())
            {
                // clé non-trouvée

                nodeName2 = "node" + std::to_string(nodeCount);
                nodeCount++;
                mapCibleToNodeName.insert(pair<string, string>(refName, nodeName2));
                dotfile << nodeName2 << " [label=\"" << refName << "\"];" << endl;
            }
            else
            {
                nodeName2 = cibleToNodeName->second;
            }

            dotfile << nodeName2 << " -> " << nodeName1 << " [label=\"" << count << "\"];" << endl;
            it2++;
        }

        it++;
    }

    // Cibles::const_iterator it3 = cibles.begin();
    // const Cibles::const_iterator end3 = cibles.end();
    // while (it3 != end3)
    // {
    //     const string nodeName = it3->first;
    //     const CibleReferersMap hitrefs = it3->second->referers;

    //     CibleReferersMap::const_iterator hit = hitrefs.begin();
    //     const CibleReferersMap::const_iterator hitrefsEnd = hitrefs.end();
    //     while (hit != hitrefsEnd)
    //     {
    //         dotfile << "link " << (hit->first) << " -> " << nodeName << " × " << hit->second << endl;
    //         hit++;
    //     }
    //     it2++;
    // }

    dotfile << "}" << endl;

    return EXIT_SUCCESS;
}

void App::ShowStatistics() const
{
    // une multimap pour permettre les doublons sur les nombres de hits qui est clé
    // on garde la fonction compare initiale, car elle permet de comparer les clés
    // on garde l'allocator, car on ne s'en sert pas
    // std::multimap<unsigned int, Cible*> ciblesMap;

    std::multimap<unsigned int, Cible *, std::greater<unsigned int>> ciblesMap;

    Cibles::const_iterator it = cibles.begin();
    Cibles::const_iterator end = cibles.end();
    while (it != end)
    {
        // on insère la pair <nbHitsTotals, Pointeur vers la cible>
        ciblesMap.insert(make_pair(it->second->nbHits, it->second));
        it++;
    }

    std::multimap<unsigned int, Cible *>::iterator itMap = ciblesMap.begin();
    std::multimap<unsigned int, Cible *>::iterator endMap = ciblesMap.end();

    // check obligatoire à ce que j'ai compris sinon undefined behavior....
    if (distance(itMap, ciblesMap.end()) > options.topStatsCount)
    {
        endMap = next(ciblesMap.begin(), options.topStatsCount);
    }

    while (itMap != endMap)
    {
        cout << itMap->second->nomCible << " " << itMap->first << " hits" << endl;
        itMap++;
    }
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

        if ((newHit.referer == "") && (newHit.cible == "")) // on vérifie la validité du hit
        {
            break; // fin du fichier
        }

        if (options.shouldExcludeOthers)
        {
            if (endsWith(newHit.referer) || endsWith(newHit.cible))
            {
                continue; // on n'ajout pas le hit si la cible ou le referer a pour extension celles refusées
            }
        }

        if (options.shouldFilterByTime)
        {
            if (newHit.hour != options.filterTime)
            {
                continue; // on n'ajoute pas le hit s'il a été effectué à une autre heureu que celle voulue
            }
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

bool App::endsWith(string toStudy)
{
    for (unsigned int i = 0; i < extensions.size(); i++)
    {
        if (toStudy.find(extensions[i]) == (toStudy.length() - extensions[i].length()))
            return true;
    }
    return false;
}

void App::usage()
{
    cerr << "Usage: " << PROG_NAME << " [-e] [-t 0-23] [-g out.dot] in.log" << endl;
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
    if (argc == 1)
    {
        App::usage();
        return EXIT_FAILURE;
    }

    int i = 1;
    while (i < argc)
    {
        if (!strcmp(argv[i], "-g"))
        {
            if (i == argc - 1)
            {
                App::usage();
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
                App::usage();
                return EXIT_FAILURE;
            }

            // On ignore les options répétées, seule la dernière valeur est prise en compte.
            i++;
            int hour = App::atoi(argv[i]);
            if (hour > 23 || hour < 0)
            {
                cerr << "Erreur: paramètre ‘heure’ de l'option -t"
                     << " doit être un nombre entre 0 et 23 (bornes incluses)." << endl;
                App::usage();
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
                App::usage();
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
                App::usage();
                return EXIT_FAILURE;
            }
        }

        i++;
    }

    if (this->options.inputFilename == "") // si aucun fichier d'entrée n'est donné
    {
        cerr << "Erreur: argument manquant du nom de fichier d'entrée." << endl;
        App::usage();
        return EXIT_FAILURE;
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
