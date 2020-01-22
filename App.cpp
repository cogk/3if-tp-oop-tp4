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
#include <string>
using namespace std;

//------------------------------------------------------ Include personnel
#include "App.h"
#include "Cible.h"
#include "Hit.h"

//------------------------------------------------------------- Constantes
const string PROG_NAME = "analog"; //nom de l'executable

//----------------------------------------------------------------- PUBLIC

//----------------------------------------------------- Méthodes publiques
int App::Run()
{

    ifstream logfile(options.inputFilename); // ouverture du fichier
    if (logfile.fail()) // si le fichier ne s'ouvre pas correctement
    {
        cerr << "Erreur: impossible d'ouvrir le fichier d'entrée." << endl;
        return EXIT_FAILURE;
    }

    readFromFile(logfile); // appel à la fonction de lecture du fichier

    logfile.close();

    if (options.shouldFilterByTime)
    {
        cout << "Warning: only hits between " << options.filterTime << "h and " << (options.filterTime + 1) << "h have been taken into account" << endl;
    }


    if (options.shouldOutputDot) //génération du fichier .dot si demandé
    {
        if (WriteDotGraph() == EXIT_FAILURE)
        {
            return EXIT_FAILURE;
        }
    }

    ShowStatistics(); //Appel à la méthode qui Affiche les 10 documents les plus consultés

    // on détruit les cibles car c'est la fin du programme, on ne les utilise plus
    Cibles::iterator it = cibles.begin();
    const Cibles::iterator end = cibles.end();
    while (it != end)
    {
      delete it->second;
      it++;
    }

    return EXIT_SUCCESS;
} //----- Fin de App::Run

int App::WriteDotGraph() const
{
    ofstream dotfile(options.outputDotFilename); //fichier de sortie
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

        //on vérifie si un noeud a déjà été constitué par la cible en cours
        const NodeMap::const_iterator cibleToNodeName = mapCibleToNodeName.find(cibleName);
        if (cibleToNodeName == mapCibleToNodeName.end())
        {
            // clé non-trouvée, donc la cible est donnée comme nouveau noeud au graph
            nodeName1 = "node" + std::to_string(nodeCount);
            nodeCount++;
            mapCibleToNodeName.insert(pair<string, string>(cibleName, nodeName1));
            dotfile << nodeName1 << " [label=\"" << cibleName << "\"];" << endl;
        }
        else
        {
          //sinon on travaille avec le nom de noeud déjà existant pour cette cible
            nodeName1 = cibleToNodeName->second;
        }

        // on parcours les referer de la cible etudie
        CibleReferersMap::const_iterator it2 = refs.begin();
        const CibleReferersMap::const_iterator end2 = refs.end();
        while (it2 != end2)
        {
            const string refName = it2->first;
            const unsigned int count = it2->second;

            string nodeName2;

            // on vérifie si un noeud existe déjà ayant comme nom celui du referer en ours
            const NodeMap::const_iterator cibleToNodeName = mapCibleToNodeName.find(refName);
            if (cibleToNodeName == mapCibleToNodeName.end())
            {
                // clé non-trouvée, donc le referer est donné comme nouveau noeud au graph

                nodeName2 = "node" + std::to_string(nodeCount);
                nodeCount++;
                mapCibleToNodeName.insert(pair<string, string>(refName, nodeName2));
                dotfile << nodeName2 << " [label=\"" << refName << "\"];" << endl;
            }
            else
            {
              //sinon on travaille avec le nom de noeud déjà donné pour ce referer
                nodeName2 = cibleToNodeName->second;
            }

            // on insère dans le fichier la nouvelle liaison
            dotfile << nodeName2 << " -> " << nodeName1 << " [label=\"" << count << "\"];" << endl;
            it2++;
        }

        it++;
    }

    dotfile << "}" << endl;

    cout << "Dot-file " << options.outputDotFilename << " generated" << endl;
    return EXIT_SUCCESS;
}

void App::ShowStatistics() const
{
    // une multimap pour permettre les doublons sur les nombres de hits total qui est clé
    // on utilise la fonction compare std::greater pour classer par ordre décroissant,
    // on garde l'allocator, car on ne s'en sert pas
    // std::multimap<unsigned int, Cible*> ciblesMap;

    std::multimap<unsigned int, Cible *, std::greater<unsigned int>> ciblesMap;

    Cibles::const_iterator it = cibles.begin();
    Cibles::const_iterator end = cibles.end();
    while (it != end)
    {
        // on insère la pair <nbHitsTotal, Pointeur vers la cible> dans la multimap
        ciblesMap.insert(make_pair(it->second->nbHitsTotal, it->second));
        it++;
    }

    std::multimap<unsigned int, Cible *>::iterator itMap = ciblesMap.begin();
    std::multimap<unsigned int, Cible *>::iterator endMap = ciblesMap.end();

    // check obligatoire pour vérifier que l'on a au moins 10 cibles, car si on utilise un next
    // 10 éléments après et qu'il y en a moins que dix, cela résulte en undefined behavior
    if (distance(itMap, ciblesMap.end()) > options.topStatsCount)
    {
        endMap = next(ciblesMap.begin(), options.topStatsCount);
    }

    while (itMap != endMap) // affichage d'au plus 10 documents les plus consultés
    {
        cout << itMap->second->nomCible << " "
             << "(" << itMap->first << " hits)" << endl;
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

        if ((newHit.referer == "") && (newHit.cible == "")) // on vérifie la validité du hit
        {
            break; // le hit non valide signifie la fin du fichier
        }

        if (options.shouldExcludeOthers)
        {
            if (EndsWith(newHit.referer) || EndsWith(newHit.cible)) //on vérifie si la cible ou le referer
            //ne sont pas du type souhaité, c'est à dire ont une extensions indésirable
            {
                continue; // on n'ajoute pas le hit si la cible ou le referer a pour extension celles refusées
            }
        }

        if (options.shouldFilterByTime)
        {
            if (newHit.hour != options.filterTime)
            {
                continue; // on n'ajoute pas le hit s'il a été effectué à une autre heure que celle voulue
            }
        }

        // si l'adresse locale insa se trouve au début de referer on l'enleve
        if (newHit.referer.find(LOCAL_ADDRESS) == 0)
        {
            // on retire l'adresse locale
            newHit.referer = newHit.referer.substr(LOCAL_ADDRESS.length());
        }

        const string key = newHit.cible;
        Cible *value;

        //On vérifie si la cible existe déjà
        Cibles::iterator oldCibleIt = cibles.find(key);
        if (oldCibleIt != cibles.end())
        {
            // si la cible existe déjà
            value = oldCibleIt->second; // on récupère le pointeur vers la cible déjà existante
        }
        else
        {
            //sinon on créer une nouvelle cible et on l'ajoute à la map
            value = new Cible(key);
            cibles.insert(CiblesContainerPair(key, value));
        }

        // appel à la méthode Increment qui on incrémente le nombre de hits total de la cibles
        //et soit ajoute le referer s'il ne fait pas déjà partie des referer de cette cible
        // ou incremente le nombre de liaison entre cette cible et ce referer
        value->Increment(newHit.referer);
    }
}

bool App::EndsWith(string toStudy)
{
  //on vérifie si la chaine de caractère paramètre effectif a pour terminaison l'une des extensions interdites
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

int App::Atoi(const char *str)
{
  // la méthode convertie une chaine de caractère en entier qu'elle retourne
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
  //on affiche sur la sortie standard le récapitulatif des options choisie
  // méthode utile pour vérifier la bonne interprétation des options en ligne de commande
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
      //traitment de l'option "-g"
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
            options.shouldOutputDot = true;

            options.outputDotFilename = argv[i];
        }
        //traitement de l'option "-e"
        else if (!strcmp(argv[i], "-e"))
        {
            options.shouldExcludeOthers = true;
        }
        //traitement de l'option "-t"
        else if (!strcmp(argv[i], "-t"))
        {
            if (i == argc - 1)
            {
                App::usage();
                return EXIT_FAILURE;
            }

            // On ignore les options répétées, seule la dernière valeur est prise en compte.
            i++;
            int hour = App::Atoi(argv[i]);
            if (hour > 23 || hour < 0)
            {
                cerr << "Erreur: paramètre ‘heure’ de l'option -t"
                     << " doit être un nombre entre 0 et 23 (bornes incluses)." << endl;
                App::usage();
                return EXIT_FAILURE;
            }

            options.filterTime = (unsigned int)hour;
            options.shouldFilterByTime = true;
        }

        //possibilité d'évolution ci-dessous en commentaire : ajout de l'option pour que
        //l'utilisateur fournisse l'adresse locale
      /*  else if (!strcmp(argv[i], "-R"))
        {
            // On ignore les options répétées, seule la dernière valeur est prise en compte.
            options.serverReferer = argv[i];
        }*/
        else
        {
            // Si le paramètre commence par un tiret - et qu'il n'a satisfait aucune des conditions précédentes
            // Alors c'est une option invalide
            if (!strncmp(argv[i], "-", 1))
            {
                cerr << "Erreur: option non reconnue ‘" << argv[i] << "’." << endl;
                App::usage();
                return EXIT_FAILURE;
            }
            else if (options.inputFilename == "")
            {
                // On n'a pas encore lu de nom de fichier d'entrée.
                options.inputFilename = argv[i];
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

    if (options.inputFilename == "") // si aucun fichier d'entrée n'est donné
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
