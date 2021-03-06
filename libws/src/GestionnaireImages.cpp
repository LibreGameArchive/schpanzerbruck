#include <GestionnaireImages.hpp>


namespace ws
{
ImageAnnotee::ImageAnnotee(unsigned int _etalement)
{
    etalement = _etalement;
}


GestionnaireImages::~GestionnaireImages()
{
    for(map<string, map<unsigned int, ImageAnnotee*> >::iterator objsAct=objets.begin() ; objsAct!=objets.end() ; objsAct++)
    {
        for(map<unsigned int, ImageAnnotee*>::iterator it=objsAct->second.begin(); it!=objsAct->second.end(); it++)
            if(it->second != NULL)
                delete it->second; // Delete la valeur
    }
}


bool GestionnaireImages::chargerImagesMap(unsigned int _nbrCases, vector<unsigned int> _numsTuiles, vector<unsigned int> _numsElements, vector<string> _cheminsTuiles, vector<string> _cheminsElements)
{    
    bool b = true;
    for(unsigned int i=0; i<_nbrCases; i++)
    {
        bool tuileOK = chargerImage("tuiles", _numsTuiles[i], _cheminsTuiles[i]);
        bool elemOK = chargerImage("elements", _numsElements[i], _cheminsElements[i]);
        b = b && tuileOK && elemOK;    // ETALEMENT !! A REGLER !
    }
    return b;
}


bool GestionnaireImages::chargerImage(string typeObj, unsigned int num, string chemin, unsigned int etalement)
{
    bool chargementOK = true;
    // num == 0 signifie qu'il n'y a pas d'image de ce type pour cette case
    if (num != 0)
    {   // Si l'image n'a pas déjà été chargée
        if (objets[typeObj].find(num) == objets[typeObj].end())
        {
            objets[typeObj][num] = new ImageAnnotee();
            if ( !((objets[typeObj][num])->LoadFromFile(chemin)) )
                chargementOK = false;
            else
                cout << typeObj << ": no." << num << " [" << chemin << "] chargé." << endl;
        }
    }
    return chargementOK;
}

ImageAnnotee* GestionnaireImages::obtenirImage(string typeObj, unsigned int num)
{
    return objets[typeObj][num];
}

const map<unsigned int, ImageAnnotee*>& GestionnaireImages::operator[](string typeObj)
{
    return objets[typeObj];
}
}
