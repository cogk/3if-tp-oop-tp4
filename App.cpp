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
#include <fstream>
#include <iostream>
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

//------------------------------------------------- Surcharges d'opérateurs

//-------------------------------------------- Constructeurs - destructeurs
App::App(int argc, char *argv[])
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
