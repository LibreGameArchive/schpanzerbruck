from random import *


"""Classe des maitrises (les fonctions des maitrises re�oivent en arg le grade qu'elle doivent utiliser)"""


class Maitrise:
    """D�crit une maitrise"""

    def __init__(self, nom, nomComplet,antagonismes,attaqueMagique,attaquePhysique,portee,precision):
        self.nom = nom						# Nom courant utilis� pour la comparaison
        self.nomComplet = nomComplet		# Nom complet, utilis� pour les cr�tins qui vont jouer � ce jeu stupide
        self.antagonismes = antagonismes	# Liste des maitrises qui ne peuvent pas etre utilis�s ensemble
        self.attaqueMagique = attaqueMagique
        self.attaquePhysique = attaquePhysique
        self.portee = portee
        self.precision = precision
    
    def __calculDegats(grade):
        
        if grade == "E":
            ret = randrange(10)
        elif grade == "D":
            ret = randrange(20)
        elif grade == "C":
            ret = randrange(40)
        elif grade == "B":
            ret = randrange(60)
        elif grade == "A":
            ret = randrange(100)
        
        return ret
    
    def __calculDegatsMagiques(grade):
        return (self.attaqueMagique * self.__calculDegats(grade))
    
    def __calculDegatsPhysiques(grade):
        return (self.attaquePhysique * self.__calculDegats(grade))
    
    def __calculPortee(grade):
        return (self.portee)
    
    def __calculPrecision(grade):
        return (self.precision * self.__calculDegats(grade))
    
    def retourneStat():
        return ((self.__calculDegatsMagique(),self.__calculDegatsPhysiques(),self.__calculPortee(),self.__calculPrecision()))



feu = Maitrise("feu","Ma�trise du Feu",["eau"],22,15,23,12)

eau = Maitrise("eau","Maitrise de l'Eau",["feu"],15,22,23,12)

epees = Maitrise("epees","Maniement des �p�es",["arcs"],0,30,1,14)

armures = Maitrise("armures","Techniques de d�fenses � l'armure",[],0,0,1,0)

arcs = Maitrise("arcs","Maniement des arcs",["epees","eau"],0,25,33)