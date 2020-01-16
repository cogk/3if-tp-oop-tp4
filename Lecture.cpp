/*************************************************************************
                           Lecture  -  description
                             -------------------
    début                : 2019-11-19
    copyright            : (C) 2019 FADILI Zineb & FORLER Corentin
    e-mail               : zineb.fadili@insa-lyon.fr
                           corentin.forler@insa-lyon.fr
*************************************************************************/

//---------- Réalisation de la classe <Lecture> (fichier Lecture.cpp) ------------

//---------------------------------------------------------------- INCLUDE

//-------------------------------------------------------- Include système
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include <queue>
using namespace std;

//------------------------------------------------------ Include personnel
//#include "Lecture.h" je lui ai pas fait de.h

enum state {FAIL, SUCCESS};
const string LOCALADRESS = "http://intranet-if.insa-lyon.fr/"; // l'adresse locale qu'on veut enlever à chaque fois

//------------------------------------------------------------- Constantes

/////////// LE HIT ET SON OPERATEUR ///////
// la struct Hit
struct Hit{
  string cible;
  string referer;
  string date;


};

// la surcharge d'operateur
ifstream & operator >> (ifstream & fichierLog,  Hit & unHit)
{
  string garbage; // on y stocke à chaque fois ce qu'on veut pas.

  getline(fichierLog, garbage, ":");
  getline(fichierLog, unHit.date, ':');

  getline(fichierLog, garbage, ' ');
  getline(fichierLog, garbage, ' ');
  getline(fichierLog, unHit.cible, ' ');

  getline(fichierLog, garbage, ' ');
  getline(fichierLog, garbage, '"');
  getline(fichierLog, unHit.referer,'"');

  // on enlève le début si c'est une adresse local
  if (unHit.referer.find(LOCALADRESS)==0) //c'est à dire l'adresse locale se trouve au début du referer
  {
    unHit.referer = unHit.referer.substr(LOCALADRESS.length()); // on enleve l'adresse local
  }
  istream.getline(garbage); // on prend le reste de la ligne pour le
  return fichierLog; // on retourne le stream quand on a termine de l'utiliser
}


/////////////////// CREATION DE LA PRIORITY QUEUE DE CIBLES////////////
// la struct cible
 typedef struct Cible {
  string nomCible; // le nom de la cible
  unsigned int nbHits; // le nombre de hits total vers la cible
  map<string, unsigned int> referers; // une map qui prend le nom du referer et le nombre de hit vers la cible en question
}Cible;


// nécessaire pour définir notre propre operator pour la priority queue pour les classer selon le nombre de hits
struct compareNbHits {
  bool operator() (const Cible& cible1, const Cible& cible2)
  {
      return cible1.nbHits < cible2.nbHits;
  }
};

// notre queue qui s'auto classe, mais pourquoi vector?
priority_queue<Cible, vector<Cible>, compareNbHits > cibles;
//----------------------------------------------------------------- PUBLIC

//----------------------------------------------------- Méthodes publiques

/////////////LECTURE DEPUIS LE FICHIER LOG////////////
// méthode qui permet d'analyser le fichier, retourne un boolean pour
// indiquer si la lecture s'est bien passée ou non
bool LectureComplete(string pathFichierLog)
{
  ifstream fichierLog(pathFichierLog);
  if(fichierLog.fail()) // si le fichier ne s'ouvre pas bien
  {
    return FAIL;
  }
  struct Hit newHit;

// on ajoute les hits à notre priority queue
  while(!fichierLog.eof())
  {
    fichierLog >> newHit; // lecture du hit grace à la surcharge d'opateur >>
    //
    // il faut verifier si newHit.cible existe déjà dans le priority queue de cible, je ne sais pas comment faire? parce que qu'il faut comparer
    // newHit.cible avec Cible.nomCible, il faut comprarer les noms

    // s'il existe, incrémenter son nombre de hits total
    //et checker dans sa map si le referer existe déjà. s'il existe incrémenter le nombre lui correspondant, si le referer n'existe pas le incrémenter

    // s'il n'existe pas, créer une nouvelle cible et l'ajouter a la priority queue

  }

  return SUCCESS;

}



//------------------------------------------------- Surcharges d'opérateurs
// surchage de l'opérateur d'entrée : <<


//-------------------------------------------- Constructeurs - destructeurs
