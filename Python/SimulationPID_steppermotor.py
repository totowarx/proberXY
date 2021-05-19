valeurDeConsigne = 50000
valeurCapteur = 0

erreur = valeurDeConsigne-valeurCapteur
somme_erreur = 0;
erreur_precedente = 0;

Kp = 1;
Ki = 1;
Kd = 1;

commande = 0;
tCommande = 0;
tMin = 100;

step = 10000

currentT = 0
pastT = 0

distance = 0

i=0

while erreur>10: 
    erreur = valeurDeConsigne-valeurCapteur
    somme_erreur += erreur
    variation_erreur = erreur-erreur_precedente

    erreur_precedente = erreur
    
    commande = (Kp * erreur) + (Ki * somme_erreur) + (Kd*variation_erreur)
    print(commande)
    tCommande = tMin + (commande/100000)
    print("tCommande ="+str(tCommande))

    pastT = currentT
    currentT +=step
    nbImpulsion = (step/(2*tCommande))
    print("nbImpulsion  ="+str(nbImpulsion))
    distance += 0.315*nbImpulsion
    valeurCapteur = distance
    print("valeur capteur ="+str(valeurCapteur))


#Toute les ms ont regarde la position et on recalcule l'erreur

    
