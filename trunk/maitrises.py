# encoding=utf-8

from random import randrange


"""Classe des maitrises (les fonctions des maitrises re�oivent en arg le grade qu'elle doivent utiliser)"""


class Maitrise:
    """D�crit une maitrise"""

    def __init__(self, nom, nomComplet, antagonismes, FCP, FCM, AGI, portee):
        self.nom = nom				        # Nom courant utilis� pour la comparaison
        self.nomComplet = nomComplet		# Nom complet, utilis� pour les cr�tins qui vont jouer � ce jeu stupide
        self.antagonismes = antagonismes	# Liste des maitrises qui ne peuvent pas etre utilis�s ensemble
        self.FCP = FCP
        self.FCM = FCM
        self.AGI = AGI
        self.portee = portee
    
    def __calculCoeff(self, grade):
        
        if grade == "E":
            ret = randrange(11)     # G�n�re un nombre compris entre 0 et 11-1=10
        elif grade == "D":
            ret = randrange(11) + 10
        elif grade == "C":
            ret = randrange(21) + 20
        elif grade == "B":
            ret = randrange(21) + 40
        elif grade == "A":
            ret = randrange(41) + 60
        
        return ret / 100.0		# Adapt� � l'algorithme de combat : chaque stat est sur une base 100, donc modificateur compris entre 0 et 1
    
    def calculStatEnFctGrade(self, stat, grade):
        return int(self.__dict__[stat] * self.__calculCoeff(grade))
    
    def calcVal(self, grade):
        if grade == "A":
            return 100
        if grade == "B":
            return 75
        if grade == "C":
            return 50
        if grade == "D":
            return 25
        if grade == "E":
            return 10



feu = Maitrise("feu","Ma�trise du Feu",["eau"],22,15,23,12)

eau = Maitrise("eau","Ma�trise de l'Eau",["feu"],15,22,23,12)

epees = Maitrise("epees","Maniement des �p�es",["arcs"],30,0,1,14)

armures = Maitrise("armures","Techniques de d�fenses � l'armure",[],0,0,1,0)

arcs = Maitrise("arcs","Maniement des arcs",["epees","eau"],0,25,33,2)
