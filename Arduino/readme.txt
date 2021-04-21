/-------------------------------------------------
//////////////////Mardi 31 Mars///////////////////
--------------------------------------------------

Explication fichier :
---------------------
PilotageMoteurSimultane2.0 est un programme qui va me permettre de déplacer la platine jusqu'a se qu'elle percute les capteurs de fin de course. Cela va permettre de définir le zéro "mécanique" de la platine. Tout cela avec déplacement simultané.

PilotageMoteurSimultane1.0 est un programme qui m'a permis de faire des tests de déplacement simultané sur les deux axes. Les mouvements fonctionnent mais il sont défini en nombre de pas.

/-------------------------------------------------
///////////////////Mardi 9 Mars///////////////////
--------------------------------------------------

Fichier à utiliser : lectureCapteurRLS_v1onMKS

Explication fichier :
---------------------

lectureCapteurRLS_88bits est un essai car la communication SPI se faisait sur 44 bits et ça n'allait pas
donc j'ai essayé de faire le double pour avoir un multiple de 8 bits. Ce fichier n'est pas utile, il ne
doit pas être utilisé.

lectureCapteurRLS_v1 est un fichier Arduino pour Arduino UNO. Qui permet de lire la distance parcourue
sur la règle. Le fichier ne gère pas les codes d'erreur.

lectureCapteurRLS_v1onMKS est un fichier Arduino pour la carte mère MKS GEN L. Une fonction motionControl
permet de se déplacer jusqu'a un point dont on a fixer la position. On génère des impulsions moteurs tant
que la distance réelle, ne dépasse pas la distance voulue.

PilotageMonteur1.0 est un programme de base pour tester la rotation du moteur et le fonctionnement des
drivers.

PilotageMoteur_Primitif est un programme qui m'a permis de faire les premiers tests de déplacement sous
le polytec. Il à des fonctions avec adaptation de la vitesse et des fonctions avec vitesses fixe. Ce
fichier à permis la création du fichier lectureCapteurRLS_v1onMKS.
-------------------------------------------------/
