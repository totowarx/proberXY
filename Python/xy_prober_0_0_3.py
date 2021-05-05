import requests
import json
import csv
import datetime

class prober_xy_reticule_typecomposant:
    "entrées : x et y du prober"
    "sortie : nom reticule, nom composant"
    #L104C_08
    def __init__(self) :
        print("Nom de ton wafer :")
        self.nomWafer=input()
        response = requests.get("http://vmicro.fr/database/BDD_1.0/API/api.php?action=NomWafer_infoxy&Nom_Wafer=" + self.nomWafer)
        NomWafer_infoxy_json = json.loads(response.text)
        self.id_composant=NomWafer_infoxy_json['results']['id_composant']
        self.type_composant_ref=NomWafer_infoxy_json['results']['type_composant_ref']
        self.Nom_Type_Reticule=NomWafer_infoxy_json['results']['Nom_Type_Reticule']
        self.pos_x_ret=NomWafer_infoxy_json['results']['pos_x_ret']
        self.pos_y_ret=NomWafer_infoxy_json['results']['pos_y_ret']
        self.pos_x_c=NomWafer_infoxy_json['results']['pos_x_c']
        self.pos_y_c=NomWafer_infoxy_json['results']['pos_y_c']
        self.pos_x_wafer=NomWafer_infoxy_json['results']['pos_x_wafer']
        self.pos_y_wafer=NomWafer_infoxy_json['results']['pos_y_wafer']
        self.taille_composant_x=NomWafer_infoxy_json['results']['taille_composant_x']
        self.taille_composant_y=NomWafer_infoxy_json['results']['taille_composant_y']
        self.coord_x_c=NomWafer_infoxy_json['results']['coord_x_c']
        self.coord_y_c=NomWafer_infoxy_json['results']['coord_y_c']
        self.x=0
        self.y=0
        self.indice=0

        #print(self.pos_x_wafer[30])
        
    def fichier_map(self) :
        'génére le fichier map d''un wafer  donné'
        indice_de_tableau=0
        fichier = open("C:\\Users\\33783\\Documents\\Vmicro\\Vmicro_python_prober\\data.csv", "w")

        for i in self.pos_x_wafer :

            fichier.write(str(indice_de_tableau) + "," + str(self.type_composant_ref[indice_de_tableau]) + "," + str(self.Nom_Type_Reticule[indice_de_tableau]) + "," + str(self.pos_x_ret[indice_de_tableau]) + "," + str(self.pos_y_ret[indice_de_tableau]) + "," + str(self.pos_x_c[indice_de_tableau]) + "," + str(self.pos_y_c[indice_de_tableau]) + "," + str(self.pos_x_wafer[indice_de_tableau]) + "," + str(self.pos_y_wafer[indice_de_tableau]) + "," + str(self.taille_composant_x[indice_de_tableau]) + "," + str(self.taille_composant_y[indice_de_tableau]) + "," + str(self.coord_x_c[indice_de_tableau]) + "," + str(self.coord_y_c[indice_de_tableau]) +"," + str(self.id_composant[indice_de_tableau]) + "\n")
            indice_de_tableau=indice_de_tableau+1

        fichier.close()

    def info_composant(self, x, y) :
        'entré : x et y de la platine'
        'sortie : toute les infos nécéssaires (xy reticule , nom compo , nom reticule) '

        with open('C:\\Users\\33783\\Documents\\Vmicro\\Vmicro_python_prober\\data.csv', newline='', encoding='utf-8') as f:
            reader = csv.reader(f)
            for row in reader:
                
                taille_composant_x=int(row[9])
                taille_composant_y=int(row[10])
                
                if x <= (int(row[7])+(int(taille_composant_x)/2)) and x >= (int(row[7])-(int(taille_composant_x)/2)) :
                    if y <= (int(row[8])+(int(taille_composant_y)/2)) and y >= (int(row[8])-(int(taille_composant_y)/2)) :
                        #print(str(row[7]) + " "+str(taille_composant_x)+" " + str(taille_composant_y) + "\n")
                        print("Le composant est :" + str(row[1]) + "\n")
                        print("Le type de reticule du composant est :" + str(row[2]) + "\n")
                        print("Le x matrice reticule  : " + str(row[3]) + "\n")
                        print("Le y matrice reticule  : " + str(row[4]) + "\n")
                        print("Le x composant dans son reticule  : " + str(row[11]) + "\n")
                        print("Le y composant dans son reticule  : " + str(row[12]) + "\n")
                        print("Le x du composant : " + str(row[7]) + "\n")
                        print("Le y du composant  : " + str(row[8]) + "\n")
                        self.id_composant_choisi= row[13]
                        break

        f.close()
    
    def info_xy(self, x_composant,y_composant, x_reticule, y_reticule) :
        'entré : x et y de la matrice du reticule, le type composant'
        'sortie : x et y de l''emplacement du composant'
        
        with open('C:\\Users\\33783\\Documents\\Vmicro\\Vmicro_python_prober\\data.csv', newline='', encoding='utf-8') as f:
            reader = csv.reader(f)
            for row in reader:
         #       print("\n" + str(row[4]))
                if (str(row[3])==str(x_reticule) and int(row[4])== int(y_reticule)) and (int(row[11])==int(x_composant) and int(row[12])==int(y_composant))  : 

                    print("\n" + " x wafer : " + str(row[7]) + " y wafer : " + str(row[8]))
        #print("\n" + str(type_composant))
        f.close()
    

    def envoie_image_BDD(self,Commentaire) : # #,id_composant,nom_photo
        url = 'http://vmicro.fr/database/BDD_1.0/API/api.php'
        files = {'file_photo': open('C:\\Users\\33783\\Documents\\Vmicro\\Vmicro_python_prober\\telechargement.jpg', 'rb')}
        data = [('action', 'addCommentaire'),('id_auteur', 5), ('id_composant', self.id_composant_choisi), ('id_photo', 0), ('id_fichier', -1), ('contenu_Commentaire', Commentaire)]
        r = requests.post(url, files=files, data=data)
        print(r)
    

#L104C_08
a=prober_xy_reticule_typecomposant()
a.fichier_map()
a.info_composant(10000,1000)
#a.info_xy(1,1,5,5)
a.envoie_image_BDD("test")
