#include <InterfaceCombat.hpp>


namespace ws
{

InterfaceCombat::InterfaceCombat(GestionnaireImages* _gestImages, unsigned int _appL, unsigned int _appH)
{
    appL = _appL;
    appH = _appH;
    gestImages = _gestImages;
    for(unsigned int i=0; i<4; i++)
        picked[i] = -1;
    menuEchapON = false;
    barreInfosON = false;
    fenetreMaitrisesON = false;
    menuTriangleON = false;
    clic = false;
    for(unsigned int i=0; i<3; i++)
    {
        mtrChoisies[i] = -1;
        gradesChoisis[i] = -1;
    }
    factAssomb = 1;
    setInfosDsBarre();
    valChrono = 0;
    infosPersoActuel.nom = "";
    infosPersoActuel.VIE = 0;
    infosPersoActuel.FTG = 0;
    numPremMtrAffichee = 0;
    
    menuTriangle.affiche = false;
    menuTriangle.btnDeplX = -1;
    menuTriangle.btnDeplY = -1;
    menuTriangle.btnActionX = -1;
    menuTriangle.btnActionY = -1;
    menuTriangle.btnPasserX = -1;
    menuTriangle.btnPasserY = -1;
    
    message = "";
    tempsMessage = 5;
    clrMessage.R = 255; clrMessage.V = 255; clrMessage.B = 255;
}


void InterfaceCombat::assombrir(bool assomb)
{
    if(assomb)
        factAssomb = 5;
    else
        factAssomb = 1;
}


void InterfaceCombat::GL_LigneTexte(string texte, float largeurTxt, float hauteurTxt, unsigned int numPoliceBmp)
{
    // Le texte sera affiché dans le plan (0xy), en (0,0,0)
    
    glEnable(GL_TEXTURE_2D);
    gestImages->obtenirImage("polices_bmp", numPoliceBmp)->Bind();
    
    float unit = 1.0/16;
    float largeurLettres = largeurTxt / texte.length();
    float hauteurLettres = hauteurTxt;
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    for(unsigned int i=0; i<texte.length(); i++)
    {
        int numAscii = texte[i];
        float coordTexX, coordTexY;
        coordTexX = (numAscii % 16) * unit;
        coordTexY = (numAscii / 16) * unit;
        
        glBegin(GL_QUADS);
            glTexCoord2f(coordTexX, coordTexY);             glVertex3f(0, hauteurLettres, 0);
            glTexCoord2f(coordTexX, coordTexY+unit);        glVertex3f(0, 0, 0);
            glTexCoord2f(coordTexX+unit, coordTexY+unit);   glVertex3f(largeurLettres, 0, 0);
            glTexCoord2f(coordTexX+unit, coordTexY);        glVertex3f(largeurLettres, hauteurLettres, 0);
        glEnd();
        
        glTranslatef(largeurLettres, 0, 0);
    }
    glPopMatrix();
    
    glDisable(GL_TEXTURE_2D);
}

void InterfaceCombat::GL_LigneTexteLargeurMax(string texte, float largeurTxtMax, float hauteurTxt, bool centrer, unsigned int numPoliceBmp)
{
    if(texte.length() == 0)
        return;
    
    float largeurTxt = (hauteurTxt/2)*texte.length();
    if(largeurTxt > largeurTxtMax)
        largeurTxt = largeurTxtMax;
    
    if(centrer)
    {   glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glTranslatef(largeurTxtMax/2 - largeurTxt/2, 0, 0);
    }
    
    GL_LigneTexte(texte, largeurTxt, hauteurTxt, numPoliceBmp);
    
    if(centrer)
        glPopMatrix();
}

void InterfaceCombat::GL_LigneTexteHauteurMax(string texte, float largeurTxt, float hauteurTxtMax, bool centrer, unsigned int numPoliceBmp)
{
    if(texte.length() == 0)
        return;
    
    float hauteurTxt = (largeurTxt/texte.length())*2;
    if(hauteurTxt > hauteurTxtMax)
        hauteurTxt = hauteurTxtMax;
    
    if(centrer)
    {   glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glTranslatef(0, hauteurTxtMax/2 - hauteurTxt/2, 0);
    }
    
    GL_LigneTexte(texte, largeurTxt, hauteurTxt, numPoliceBmp);
    
    if(centrer)
        glPopMatrix();
}

void InterfaceCombat::GL_Cadre(float L, float H, float rayon)
{
    float min = (L < H ? L : H)/2;
    if(rayon <= 0)  // On dessine dans ce cas un simple rectangle
    {
        rayon = 0;
        glBegin(GL_QUADS);
            glVertex2f(0, 0);
            glVertex2f(L, 0);
            glVertex2f(L, H);
            glVertex2f(0, H);
        glEnd();
        return;
    }
    
    if(rayon > min)
        rayon = min;
    
    float conv = M_PI/180;
    
    glBegin(GL_POLYGON);
        for(unsigned int i=0; i<=90; i+=10)
            glVertex2f(rayon*(1-cos(i*conv)), rayon*(1-sin(i*conv)));
        for(unsigned int i=0; i<=90; i+=10)
            glVertex2f(L - rayon*(1-sin(i*conv)), rayon*(1-cos(i*conv)));
        for(unsigned int i=0; i<=90; i+=10)
            glVertex2f(L - rayon*(1-cos(i*conv)), H-rayon*(1-sin(i*conv)));
        for(unsigned int i=0; i<=90; i+=10)
            glVertex2f(rayon*(1-sin(i*conv)), H-rayon*(1-cos(i*conv)));
    glEnd();
}

void InterfaceCombat::GL_Bouton(string texte, int R_txt, int V_txt, int B_txt, float L, float H, float rayon)
{    
    float L_txt = (4.0/5)*L;
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
        GL_Cadre(L, H, rayon);
        glColor3ub(R_txt, V_txt, B_txt);
        glTranslatef((1.0/10)*L, 0, 0);
        GL_LigneTexteLargeurMax(texte, L_txt, H);
    glPopMatrix();
}


void InterfaceCombat::GL_MenuEchapPourSelection()
{
    float menuL = appL/3;
    float menuH = (3.0/4)*menuL;
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glTranslatef(appL/2 - menuL/2, appH/2 - menuH/2, 0);    // Origine au point en bas à gauche du MenuEchap
    
    float obActL = (2.0/3)*menuL;
    float obActH = (1.5/10)*menuH;
    
    glPushMatrix();
        glPushName(SLC_QUITTER);      // BOUTON "QUITTER"
            glTranslatef(menuL/2 - obActL/2, (1.5/10)*menuH, 0);
            GL_Cadre(obActL, obActH, 10);
        glPopName();
        
        glPushName(SLC_CONTINUER);     // BOUTON "CONTINUER"
            glTranslatef(0, (3.0/10)*menuH, 0);
            GL_Cadre(obActL, obActH, 10);
        glPopName();
    glPopMatrix();
    
    glPopMatrix();
}


void InterfaceCombat::GL_MenuEchap()
{
    float menuL = appL/3.0;
    float menuH = (3.0/4)*menuL;
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    
    glTranslatef(appL/2 - menuL/2, appH/2 - menuH/2, 0);    // Origine au point en bas à gauche du MenuEchap
    glColor4ub(20, 20, 20, 200);
    GL_Cadre(menuL, menuH, appH/16.0);
    
    float obActL = (2.0/3)*menuL;
    float obActH = (1.5/10)*menuH;
    
    glPushMatrix();
        // BOUTON "QUITTER"
        glTranslatef(menuL/2 - obActL/2, (1.5/10)*menuH, 0);
        glColor3ub(255, 25, 25);
        if (picked[1] == SLC_QUITTER)
        {   if (clic)
                glColor3ub(255, 204, 0);
            GL_Bouton("QUITTER", 255, 150, 0, obActL, obActH, 10);
        }
        else
            GL_Bouton("QUITTER", 255, 255, 255, obActL, obActH, 10);
        
        // BOUTON "CONTINUER"
        glTranslatef(0, (3.0/10)*menuH, 0);
        glColor3ub(255, 25, 25);
        if (picked[1] == SLC_CONTINUER)
        {   if (clic)
                glColor3ub(255, 204, 0);
            GL_Bouton("CONTINUER", 255, 150, 0, obActL, obActH, 10);
        }
        else
            GL_Bouton("CONTINUER", 255, 255, 255, obActL, obActH, 10);
        
        // TITRE
        glTranslatef(0, (3.0/10)*menuH, 0);
        glColor3ub(255, 255, 255);
        GL_LigneTexte("QUE FAIRE ?", obActL, obActH);
    glPopMatrix();
    
    glPopMatrix();
}


void InterfaceCombat::GL_BarreInfos()
{
    float barreL = appL/2.0;
    float barreH = barreL/9;
    float rayonSommets = 10;
    float decalA45Deg = rayonSommets*(sqrt(2.0)/2);
    float zoneEcritureL = barreL - decalA45Deg*2;
    float zoneEcritureH = barreH - decalA45Deg*2;
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    
    glTranslatef(5, 5, 0);
    glColor4ub(225/factAssomb, 162/factAssomb, 0, 220);
    GL_Cadre(barreL, barreH, rayonSommets);
    
    glTranslatef(decalA45Deg, decalA45Deg, 0);
    glColor3ub(0, 0, 0);
    
    float hautTxt = zoneEcritureH*(2.0/5);
    float largTxtMax = zoneEcritureL*(4.0/10);
    
    glPushMatrix();
        // Infos TUILE
        GL_LigneTexteLargeurMax(infosActDsBarre[0] == "" ? "---" : infosActDsBarre[0], largTxtMax, hautTxt, false);
        
        glTranslatef(largTxtMax + zoneEcritureL/10, 0, 0);       // Infos ELEMENT
        GL_LigneTexteLargeurMax(infosActDsBarre[1] == "" ? "---" : infosActDsBarre[1], largTxtMax, hautTxt, false);
    glPopMatrix();
    
    glTranslatef(0, hautTxt + zoneEcritureH/5, 0);    // Infos PERSO
    GL_LigneTexteLargeurMax(infosActDsBarre[2] == "" ? "---" : infosActDsBarre[2], zoneEcritureL, hautTxt, false);
    
    glPopMatrix();
}


void InterfaceCombat::GL_Chrono()
{
    float tailleChrono = appL/12;
    float rayonSommets = 10;
    float decalA45Deg = rayonSommets*(sqrt(2.0)/2);
    float tailleZoneEcriture = tailleChrono - decalA45Deg*2;
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    
    glTranslatef(appL*(11.0/12) - 5, 5, 0);
    glColor4ub(0, 0, 0, 220);
    GL_Cadre(tailleChrono, tailleChrono, rayonSommets);
    
    char txtChrono[10];
    sprintf(txtChrono, "%.0f", valChrono);
    
    glTranslatef(decalA45Deg, decalA45Deg, 0);
    if(valChrono <= 5)
        glColor3ub(255, 0, 0);
    else
        glColor3ub(255, 255, 255);
    GL_LigneTexteLargeurMax(txtChrono, tailleZoneEcriture, tailleZoneEcriture);
    
    glPopMatrix();
}


void InterfaceCombat::GL_CadrePersoActuel()
{
    float cadreL = appL*(2.5/8);
    float cadreH = cadreL*(2.5/10);
    float rayonSommets = 10;
    float decalA45Deg = rayonSommets*(sqrt(2.0)/2);
    float zoneEcritureL = cadreL - decalA45Deg*2;
    float zoneEcritureH = cadreH - decalA45Deg*2;
    float elemsH = zoneEcritureH*(9.5/20);
    float jaugesL = zoneEcritureL*(9.5/20);
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    
    glTranslatef(5, appH - cadreH - 5, 0);
    glColor4ub(205/factAssomb, 0, 20/factAssomb, 220);
    GL_Cadre(cadreL, cadreH, rayonSommets);
    
    glTranslatef(decalA45Deg, decalA45Deg, 0);
    
    glPushMatrix();     // DESSIN DES JAUGES
        // VIE
        glColor3ub(0, 0, 0);
        GL_Cadre(jaugesL, elemsH, rayonSommets);
        glColor3ub(255/factAssomb, 0, 0);
        GL_Cadre(jaugesL*(infosPersoActuel.VIE/100), elemsH, rayonSommets);
        glColor3ub(255/factAssomb, 255/factAssomb, 255/factAssomb);
        char VIE_c_str[10];
        sprintf(VIE_c_str, "%.1f%%", infosPersoActuel.VIE);
        GL_LigneTexteLargeurMax(VIE_c_str, jaugesL, elemsH);
        
        // FATIGUE
        glTranslatef(jaugesL + zoneEcritureL/20, 0, 0);
        glColor3ub(0, 0, 0);
        GL_Cadre(jaugesL, elemsH, rayonSommets);
        glColor3ub(100/factAssomb, 0, 255/factAssomb);
        GL_Cadre(jaugesL*(infosPersoActuel.FTG/100), elemsH, rayonSommets);
        glColor3ub(255/factAssomb, 255/factAssomb, 255/factAssomb);
        char FTG_c_str[10];
        sprintf(FTG_c_str, "%.1f%%", infosPersoActuel.FTG);
        GL_LigneTexteLargeurMax(FTG_c_str, jaugesL, elemsH);
    glPopMatrix();
    
    glTranslatef(0, zoneEcritureH - elemsH, 0);
    glColor3ub(255/factAssomb, 255/factAssomb, 255/factAssomb);
    GL_LigneTexteLargeurMax(infosPersoActuel.nom, zoneEcritureL, elemsH);
    
    glPopMatrix();
}


void InterfaceCombat::GL_FenetreMaitrisesPourSelection()
{
    float cadreH = appH*(2.0/3);
    float cadreL = cadreH*(5.0/6);
    float rayonSommets = 10;
    float decalA45Deg = rayonSommets*(sqrt(2.0)/2);
    float zoneEcritureL = cadreL - decalA45Deg*2;
    float zoneEcritureH = cadreH - decalA45Deg*2;
    float txtH = zoneEcritureH/20;
    float btnsH = txtH*1.5;
    float gradeL = txtH/2;
    float zoneAffMaitrisesH = zoneEcritureH - txtH*4;
    float zoneAffMaitrisesL = zoneEcritureL - decalA45Deg*2;
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    
    glTranslatef(appL/2 - cadreL/2, appH/2 - cadreH/2, 0);
    GL_Cadre(cadreL, cadreH, rayonSommets);
    
    glTranslatef(decalA45Deg, decalA45Deg, 0);
    
    glPushMatrix();
        glPushName(SLC_FERMER_MAITRISES);
            GL_Cadre(zoneEcritureL/3, btnsH, rayonSommets);
        glPopName();
        
        glPushName(SLC_VALIDER_MAITRISES);
            glTranslatef(zoneEcritureL*(2.0/3), 0, 0);
            GL_Cadre(zoneEcritureL/3, btnsH, rayonSommets);
        glPopName();
        
        glPushName(SLC_LISTE_MAITRISES);
            glTranslatef(-zoneEcritureL*(2.0/3) + decalA45Deg, txtH*2 + zoneAffMaitrisesH - decalA45Deg, 0);
            
            for(unsigned int i=numPremMtrAffichee; i<mtrAffichees.size() && i<(numPremMtrAffichee + 15); i++)
            {
                glPushName(i);
                    glTranslatef(0, -txtH, 0);
                    glPushName(SLC_MAITRISE);
                        GL_Cadre(zoneAffMaitrisesL, txtH);
                    glPopName();
                    
                    glPushMatrix();
                        glTranslatef(zoneAffMaitrisesL + gradeL, 0, 0);
                        for(int j=GRADE_E; j<=gradesMtrAffichees[i]; j++)     // j itère sur les grades
                        {
                            glTranslatef(-gradeL*2, 0, 0);
                            glPushName(j);
                                GL_Cadre(gradeL*2, txtH);
                            glPopName();
                        }
                    glPopMatrix();
                glPopName();
            }
        glPopName();
    glPopMatrix();
    
    glPopMatrix();
}


int InterfaceCombat::numMtrDsChoix(int numMtr)
{
    for(unsigned int i=0; i<3; i++)
        if(mtrChoisies[i] == numMtr)
            return i;
    
    return -1;
}

unsigned int InterfaceCombat::nbrMaitrisesChoisies()
{
    unsigned int nbr = 0;
    for(unsigned int i=0; i<3; i++)
        if(mtrChoisies[i] != -1)
            nbr++;
    
    return nbr;
}


void InterfaceCombat::GL_FenetreMaitrises()
{
    float cadreH = appH*(2.0/3);
    float cadreL = cadreH*(5.0/6);
    float rayonSommets = 10;
    float decalA45Deg = rayonSommets*(sqrt(2.0)/2);
    float zoneEcritureL = cadreL - decalA45Deg*2;
    float zoneEcritureH = cadreH - decalA45Deg*2;
    float txtH = zoneEcritureH/20;
    float btnsH = txtH*1.5;
    float gradeL = txtH/2;
    float zoneAffMaitrisesH = zoneEcritureH - txtH*4;
    float zoneAffMaitrisesL = zoneEcritureL - decalA45Deg*2;
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    
    glTranslatef(appL/2 - cadreL/2, appH/2 - cadreH/2, 0);
    glColor4ub(60/factAssomb, 30/factAssomb, 0, 220);
    GL_Cadre(cadreL, cadreH, rayonSommets);
    
    glTranslatef(decalA45Deg, decalA45Deg, 0);
    
    glPushMatrix();
        glColor3ub(255/factAssomb, 0, 0);
        if (picked[1] == SLC_FERMER_MAITRISES)
        {
            if (clic)
                glColor3ub(255/factAssomb, 204/factAssomb, 0);
            GL_Bouton("FERMER", 255/factAssomb, 150/factAssomb, 0, zoneEcritureL/3, btnsH, rayonSommets);
        }
        else
            GL_Bouton("FERMER", 255/factAssomb, 255/factAssomb, 255/factAssomb, zoneEcritureL/3, btnsH, rayonSommets);
        
        // Nbr maitrises choisies/3:
        glTranslatef(zoneEcritureL/3, 0, 0);
        char ratio[5];
        unsigned int nbr = nbrMaitrisesChoisies();
        sprintf(ratio, "%d/3", nbr);
        if(nbr == 3)
            glColor3ub(255/factAssomb, 170/factAssomb, 0);
        else
            glColor3ub(255/factAssomb, 255/factAssomb, 255/factAssomb);
        GL_LigneTexteLargeurMax(ratio, zoneEcritureL/3, btnsH);
        
        glTranslatef(zoneEcritureL/3, 0, 0);
        glColor3ub(0, 205/factAssomb, 50/factAssomb);
        if (picked[1] == SLC_VALIDER_MAITRISES)
        {   if (clic)
                glColor3ub(255/factAssomb, 204/factAssomb, 0);
            GL_Bouton("VALIDER", 255/factAssomb, 150/factAssomb, 0, zoneEcritureL/3, btnsH, rayonSommets);
        }
        else
            GL_Bouton("VALIDER", 255/factAssomb, 255/factAssomb, 255/factAssomb, zoneEcritureL/3, btnsH, rayonSommets);
        
        glTranslatef(-zoneEcritureL*(2.0/3), txtH*2, 0);
        glColor3ub(0, 0, 0);
        GL_Cadre(zoneEcritureL, zoneAffMaitrisesH, rayonSommets);
        glTranslatef(decalA45Deg, zoneAffMaitrisesH - decalA45Deg, 0);
        
        char gradeAct[5];
        
        for(unsigned int i=numPremMtrAffichee; i<mtrAffichees.size() && i<(numPremMtrAffichee + 15); i++)
        {
            glTranslatef(0, -txtH, 0);
            bool curseurSurMtr = false;
            int mtrChoisieEn = numMtrDsChoix(i);
            if(picked[2] == static_cast<int>(i))
                curseurSurMtr = true;
            
            glColor3ub(255/factAssomb, 255/factAssomb, 255/factAssomb);
            if(curseurSurMtr)
            {   glColor3ub(0, 255/factAssomb, 0);
                if(mtrChoisieEn != -1)
                    glColor3ub(255/factAssomb, 0, 0);
            }
            else
                if(mtrChoisieEn != -1)
                    glColor3ub(255/factAssomb, 170/factAssomb, 0);
            
            GL_LigneTexteLargeurMax(mtrAffichees[i], zoneAffMaitrisesL - gradeL*9, txtH, false);
            
            glPushMatrix();
                glTranslatef(zoneAffMaitrisesL + gradeL, 0, 0);
                for(int j=GRADE_E; j<=gradesMtrAffichees[i]; j++)     // j itère sur les grades
                {
                    glColor3ub(255/factAssomb, 255/factAssomb, 255/factAssomb);
                    if(curseurSurMtr)   // Si la maitrise actuelle est sélectionnée
                    {
                        if(mtrChoisieEn != -1)   // Si la maitrise actuelle est choisie
                        {
                            if(gradesChoisis[mtrChoisieEn] == j)    // Si le grade actuel est choisi
                            {
                                glColor3ub(255/factAssomb, 170/factAssomb, 0);
                                if(picked[3] == j)  // Si le grade actuel est en plus sélectionné
                                    glColor3ub(255/factAssomb, 0, 0);
                            }
                            else if(picked[3] == j)  // Si le grade actuel n'est que sélectionné
                                glColor3ub(0, 255/factAssomb, 0);
                        }
                        else
                            if(picked[3] == j)  // Si le grade actuel est sélectionné
                                glColor3ub(0, 255/factAssomb, 0);
                    }
                    else
                        if(mtrChoisieEn != -1 && gradesChoisis[mtrChoisieEn] == j)  // Si le grade actuel est choisi
                            glColor3ub(255/factAssomb, 170/factAssomb, 0);
                    
                    glTranslatef(-gradeL*2, 0, 0);
                    sprintf(gradeAct, "%c", 'E'-j+GRADE_E);
                    GL_LigneTexte(gradeAct, gradeL, txtH);
                }
            glPopMatrix();
        }
    glPopMatrix();
    
    // TITRE:
    glTranslatef(0, zoneEcritureH-btnsH, 0);
    glColor3ub(255/factAssomb, 255/factAssomb, 255/factAssomb);
    GL_LigneTexteLargeurMax("Choix des Maitrises", zoneEcritureL, btnsH);
    
    glPopMatrix();
}


void InterfaceCombat::GL_MenuTrianglePourSelection()
{
    double rayonBtn = appL/35;
    double diamBtn = rayonBtn*2;
    
    glMatrixMode(GL_MODELVIEW);
    
    // Bouton DEPLACEMENT
    glPushMatrix();
        glPushName(SLC_DEPLACEMENT);
            glTranslated(menuTriangle.btnDeplX - rayonBtn, menuTriangle.btnDeplY - rayonBtn, 0);
            GL_Cadre(diamBtn, diamBtn);
        glPopName();
    glPopMatrix();
    
    // Bouton ACTION
    glPushMatrix();
        glPushName(SLC_ACTION);
            glTranslated(menuTriangle.btnActionX - rayonBtn, menuTriangle.btnActionY - rayonBtn, 0);
            GL_Cadre(diamBtn, diamBtn);
        glPopName();
    glPopMatrix();
    
    // Bouton PASSER
    glPushMatrix();
        glPushName(SLC_PASSER);
            glTranslated(menuTriangle.btnPasserX - rayonBtn, menuTriangle.btnPasserY - rayonBtn, 0);
            GL_Cadre(diamBtn, diamBtn);
        glPopName();
    glPopMatrix();
}


void InterfaceCombat::GL_MenuTriangle()
{
    double rayonBtn = appL/35;
    double diamBtn = rayonBtn*2;
    
    glMatrixMode(GL_MODELVIEW);
    
    // Bouton DEPLACEMENT
    glPushMatrix();
        glTranslated(menuTriangle.btnDeplX - rayonBtn, menuTriangle.btnDeplY - rayonBtn, 0);
        glColor3ub(0, 128/factAssomb, 255/factAssomb);
        if(picked[1] == SLC_DEPLACEMENT)
            GL_Bouton("MVT", 255/factAssomb, 255/factAssomb, 0, diamBtn, diamBtn, rayonBtn);
        else
            GL_Bouton("MVT", 255/factAssomb, 255/factAssomb, 255/factAssomb, diamBtn, diamBtn, rayonBtn);
    glPopMatrix();
    
    // Bouton ACTION
    glPushMatrix();
        glTranslated(menuTriangle.btnActionX - rayonBtn, menuTriangle.btnActionY - rayonBtn, 0);
        glColor3ub(255/factAssomb, 0, 0);
        if(picked[1] == SLC_ACTION)
            GL_Bouton("ACT", 255/factAssomb, 255/factAssomb, 0, diamBtn, diamBtn, rayonBtn);
        else
            GL_Bouton("ACT", 255/factAssomb, 255/factAssomb, 255/factAssomb, diamBtn, diamBtn, rayonBtn);
    glPopMatrix();
    
    // Bouton PASSER
    glPushMatrix();
        glTranslated(menuTriangle.btnPasserX - rayonBtn, menuTriangle.btnPasserY - rayonBtn, 0);
        glColor3ub(0, 0, 0);
        if(picked[1] == SLC_PASSER)
            GL_Bouton("FIN", 255/factAssomb, 255/factAssomb, 0, diamBtn, diamBtn, rayonBtn);
        else
            GL_Bouton("FIN", 255/factAssomb, 255/factAssomb, 255/factAssomb, diamBtn, diamBtn, rayonBtn);
    glPopMatrix();
}


void InterfaceCombat::GL_DessinPourSelection(unsigned int curseurX, unsigned int curseurY, bool _clic)
{
    clic = _clic;
    
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPickMatrix(curseurX, appH - curseurY, 1, 1, viewport);
    gluOrtho2D(0, appL, 0, appH);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    glDisable(GL_ALPHA_TEST);
    
    if(menuEchapON)
    {
        glPushName(SLC_MENU_ECHAP);
        GL_MenuEchapPourSelection();
        glPopName();
    }
    else
    {
        if(menuTriangleON)
        {
            glPushName(SLC_MENU_TRIANGLE);
            GL_MenuTrianglePourSelection();
            glPopName();
        }
        
        if(fenetreMaitrisesON)
        {
            glPushName(SLC_FENETRE_MAITRISES);
            GL_FenetreMaitrisesPourSelection();
            glPopName();
        }
    }
}


void InterfaceCombat::GL_Dessin()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, appL, 0, appH);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);
    
    glEnable(GL_BLEND);
    glEnable(GL_ALPHA_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glAlphaFunc(GL_GREATER, 0);
    
    // On est ici en 2D, donc les menus doivent être affichés dans un ordre précis
    if(menuTriangleON)
        GL_MenuTriangle();
    
    if(barreInfosON)
        GL_BarreInfos();
    
    GL_Chrono();
    
    if(infosPersoActuel.nom != "")
        GL_CadrePersoActuel();
    
    if(message != "")
    {
        float largMsg = appL*(3.0/4);
        float hautMsg = largMsg/32;
        float decal = appL/18 + 20;     // Pour que le message s'affiche juste au dessus de la BarreInfos
        
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
            glTranslatef(appL/2 - largMsg/2, decal, 0);
            glColor3ub(clrMessage.R, clrMessage.V, clrMessage.B);
            GL_LigneTexteLargeurMax(message, largMsg, hautMsg);
        glPopMatrix();
        
        if(clockMsg.GetElapsedTime() >= tempsMessage)
            message = "";
    }
    
    if(fenetreMaitrisesON)
        GL_FenetreMaitrises();
    
    if(menuEchapON)
        GL_MenuEchap();
}


void InterfaceCombat::passerSelection(int* selection)
{
    picked[0] = selection[0];
    picked[1] = selection[1];
    picked[2] = selection[2];   // Sert uniquement pour les maitrises : indique le numéro de la maitrise choisie
    picked[3] = selection[3];   // Idem: sert à indiquer si c'est la maitrise ou un grade qui est choisi
    clic = false;
}


void InterfaceCombat::pasDeSelection()
{
    picked[0] = -1; picked[1] = -1; picked[2] = -1; picked[3] = -1;
    clic = false;
}

void InterfaceCombat::setInfosDsBarre(string tuile, string element, string perso)
{
    if(tuile == "" && element == "" && perso == "")
        barreInfosON = false;
    else
        barreInfosON = true;
    
    infosActDsBarre[0] = tuile;
    infosActDsBarre[1] = element;
    infosActDsBarre[2] = perso;
}

void InterfaceCombat::afficherMessage(string _message, Couleur _clr, float _temps)
{
    message = _message;
    clrMessage.R = _clr.R;
    clrMessage.V = _clr.V;
    clrMessage.B = _clr.B;
    tempsMessage = _temps;
    
    clockMsg.Reset();
}

}
