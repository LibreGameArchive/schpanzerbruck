#ifndef MOTEUR_COMBAT_HEADER
#define MOTEUR_COMBAT_HEADER

#include "MoteurJeu.hpp"
#include "MapGraphique.hpp"
#include "InterfaceCombat.hpp"
#include "GestionnaireImages.hpp"
#include "Utils.hpp"

#include <list>
#include <map>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <GL/gl.h>


using namespace std;


namespace ws
{

struct Touches
{
    sf::Key::Code zoomAvant;
    sf::Key::Code zoomArriere;
};

class MoteurJeu;

class MoteurCombat      // Synchronise la Map et l'Interface
{
private:
    sf::Window* app;
    GestionnaireImages* gestImages;
    unsigned int L, H, curseurX, curseurY;
    Touches touches;
    MapGraphique* mapGraph;
    InterfaceCombat* gui;
    Camera* camera;
    
    unsigned int vitesseDefil, bordureDefil;
    bool elemsON, deplEffectue, actionEffectuee, persoCourantPossede;
    
    void scrolling();
    void zoom(float delta);
    
    friend class MoteurJeu;
    
    MoteurCombat(sf::Window* _app, GestionnaireImages* _gestImages, const DonneesMap& _DM, const Touches& _touches);
    
    void traiterSelectInterface(int* selec, bool clic, float delta, unsigned int& whatHappens);

public:
    ~MoteurCombat();
    
    void centrerCurseur();
    
    unsigned int evenementsEtAffichage();
    
    float getFPS();
    
    int* selectMapActuelle();
    void maitrisesChoisies(int* mtrChoisies, int* gradesChoisis);
    
    void setInfosDsBarre(string tuile="", string element="", string perso="");
    void setChrono(float temps=0);
    void setPersoCourant(bool possede, int numPerso, string nom, float VIE, float FTG);
    void modifVieFtgPersoCourant(float VIE, float FTG);
    void setMaitrisesAffichees(vector<string> listeMtr, vector<int> listeGrades);
    void setListePersos(list<PersoGraphique> listePersos);
    void chargerImagesPersos(string cheminFantome, string cheminHalo, map<int, string> cheminsArmes);
    void deplacerPersoCourant(list<int> chemin, bool _deplEffectue);
    void setCasesPossibles(vector<int> casesPossibles);
    void afficherMessage(string message, Couleur clr, float temps);
    void mortPerso(int numPerso, bool retirer);
    void mortElement(int numCase);
    void lancerAnimationCombat(int numLanceur, bool cibleEstElement, int numCible, bool aDistance, float modifLanceurVie, float modifLanceurFatigue, float modifCibleVie, float modifCibleFatigue);
};

}

#endif
