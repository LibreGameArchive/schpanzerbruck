#ifndef INTERFACE_COMBAT_HEADER
#define INTERFACE_COMBAT_HEADER

#include "GestionnaireImages.hpp"

#include <SFML/Graphics.hpp>
#include <GL/gl.h>
#include <GL/glu.h>

#include <vector>
#include <string>
#include <math.h>

using namespace std;


namespace ws
{

struct InfosSuccintesSurPerso
{
    string nom;
    float VIE, FTG;
};

class InterfaceCombat
{
private:
    bool menuEchapON, barreInfosON, fenetreMaitrisesON, clic;
    
    unsigned int appL, appH;
    GestionnaireImages* gestImages;
    
    int picked[3];
    
    void GL_LigneTexte(string texte, float largeurTxt, float hauteurTxt, unsigned int numPoliceBmp=1);
    void GL_LigneTexteLargeurMax(string texte, float largeurTxtMax, float hauteurTxt, bool centrer=true, unsigned int numPoliceBmp=1);
    void GL_LigneTexteHauteurMax(string texte, float largeurTxt, float hauteurTxtMax, bool centrer=true, unsigned int numPoliceBmp=1);
    
    void GL_Cadre(float L, float H, float rayon=0);
    
    void GL_Bouton(string texte, int R_txt, int V_txt, int B_txt, float L, float H, float offset=0);
    
    void GL_MenuEchapPourSelection();
    void GL_MenuEchap();
    
    void GL_BarreInfos();
    
    void GL_Chrono();
    
    void GL_CadrePersoActuel();
    
    void GL_FenetreMaitrisesPourSelection();
    void GL_FenetreMaitrises();
    
    unsigned int factAssomb;
    string infosActDsBarre[3];
    
    unsigned int numPremMtrAffichee;
    
public:
    enum {
        SLC_MENU_ECHAP, SLC_CONTINUER, SLC_QUITTER,
        SLC_MENU_TRIANGLE, SLC_DEPLACEMENT, SLC_ACTION, SLC_FIN_DU_TOUR,
        SLC_FENETRE_MAITRISES, SLC_FERMER_MAITRISES, SLC_VALIDER_MAITRISES, SLC_LISTE_MAITRISES
    };
    
    int mtrChoisies[3];
    vector<string> mtrAffichees;
    string txtChrono;
    InfosSuccintesSurPerso infosPersoActuel;
    
    InterfaceCombat(GestionnaireImages* _gestImages, unsigned int _appL, unsigned int _appH);
    
    void switchMenuEchap();
    void switchFenetreMaitrises();
    
    void GL_DessinPourSelection(unsigned int curseurX, unsigned int curseurY, bool _clic=false);
    void GL_Dessin();
    
    void passerSelection(int* selection);
    void pasDeSelection();
    void setInfosDsBarre(string tuile="", string element="", string perso="");
};

}

#endif
