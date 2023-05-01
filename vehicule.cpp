
#include "vehicule.h"
#include <QSqlQuery>
#include <QtDebug>
#include <QObject>
#include<QVariant>
#include<QMessageBox>
#include <QApplication>


Vehicule::Vehicule()
{
id=0; kilometrage=0;NumSerie=0;
nom=" ";prenom=" ";marque=" ";modele=" ";
}

Vehicule::Vehicule(int id,int kilometrage,QString nom,QString prenom,QString marque,QString modele,int NumSerie)
{
    this->id=id;
    this->kilometrage=kilometrage;
    this->nom=nom;
    this->prenom=prenom;
    this->marque=marque;
    this->modele=modele;
    this->NumSerie=NumSerie;
}
//Getters
int Vehicule::getid(){return id;}
int Vehicule::getkilometrage(){return kilometrage;}
QString Vehicule::getnom(){return nom;}
QString Vehicule::getprenom(){return prenom;}
QString Vehicule::getmarque(){return marque;}
QString Vehicule::getmodele(){return modele;}
int Vehicule::getNumSerie(){return NumSerie;}

//Setters
void Vehicule::setid(int id){this->id=id;}
void Vehicule::setkilometrage(int kilometrage){this->kilometrage=kilometrage;}
void Vehicule::setnom(QString nom){this->nom=nom;}
void Vehicule::setprenom(QString prenom){this->prenom=prenom;}
void Vehicule::setmarque(QString marque){this->marque=marque;}
void Vehicule::setmodele(QString modele){this->modele=modele;}
void Vehicule::setNumSerie(int NumSerie){this->NumSerie=NumSerie;}


bool Vehicule::ajouter()
{
    QSqlQuery query;
    query.prepare("INSERT INTO vehicule (id, kilometrage, nom, prenom, marque, modele, numSerie) "
                  "VALUES (:id, :km, :nom, :prenom, :marque, :modele, :numSerie)");
    query.bindValue(":id", id);
    query.bindValue(":km", kilometrage);
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":marque", marque);
    query.bindValue(":modele", modele);
    query.bindValue(":numSerie", NumSerie);
    if(query.exec())
        return true;
    else
        return false;
//if(!controleSaisie())               // Vérifie si la saisie est valide
        //return false;

}


QSqlQueryModel* Vehicule::afficher()
{
   QSqlQueryModel* model=new QSqlQueryModel();


         model->setQuery("SELECT* FROM vehicule");
         model->setHeaderData(0, Qt::Horizontal, QObject::tr("Identifiant"));
         model->setHeaderData(1, Qt::Horizontal, QObject::tr("Kilometrage"));
         model->setHeaderData(2, Qt::Horizontal, QObject::tr("Nom"));
         model->setHeaderData(3, Qt::Horizontal, QObject::tr("Prenom"));
         model->setHeaderData(4, Qt::Horizontal, QObject::tr("Marque"));
         model->setHeaderData(5, Qt::Horizontal, QObject::tr("Modele"));
         model->setHeaderData(6, Qt::Horizontal, QObject::tr("NumSerie"));

   return model;
   }
bool Vehicule::supprimer(int id)
{
    QSqlQuery query;
    query.prepare(" Delete from vehicule where id=:id");
    query.bindValue(0, id);

     return query.exec();
}
bool Vehicule::rechercher(int id)
{
    // Requête SQL pour récupérer les informations du véhicule avec l'identifiant id
    QSqlQuery query;
    query.prepare("SELECT * FROM vehicule WHERE id = :id");
    query.bindValue(":id", id);
    if (query.exec() && query.first()) {
        // Si le véhicule est trouvé, on met à jour les attributs de l'objet courant
        this->setid(query.value("id").toInt());
        this->setkilometrage(query.value("kilometrage").toDouble());
        this->setnom(query.value("nom").toString());
        this->setprenom(query.value("prenom").toString());
        this->setmodele(query.value("modele").toString());
        this->setmarque(query.value("marque").toString());
        this->setNumSerie(query.value("NumS").toInt());
        return true;
    }
    return false;
}



bool Vehicule::modifier(int id)
{
    QSqlQuery query;
    query.prepare("UPDATE vehicule SET id=:id, kilometrage=:kilometrage, nom=:nom, prenom=:prenom, marque=:marque, NumSerie=:NumSerie, modele=:modele  WHERE id=:id");
    query.bindValue(":id", id);
    query.bindValue(":kilometrage", kilometrage);
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":marque", marque);
    query.bindValue(":modele", modele);
    query.bindValue(":NumSerie", NumSerie);
    return query.exec();
}
QSqlQueryModel* Vehicule::rechercher(QString valeur)
{
    QSqlQueryModel *model=new QSqlQueryModel();
    QSqlQuery query;
    query.prepare("SELECT * FROM vehicule WHERE id LIKE :valeur or kilometrage LIKE :valeur or nom LIKE :valeur or prenom LIKE :valeur or marque LIKE :valeur or modele LIKE :valeur or NumSerie LIKE :valeur");
    valeur = "%"+valeur+"%";
    query.bindValue(":valeur",valeur);
    query.exec();
    model->setQuery(query);
    return model;
}

QSqlQueryModel * Vehicule::triID()
{
    QSqlQueryModel * model=new QSqlQueryModel();

    model->setQuery("SELECT * from vehicule order by ID ");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("Identifiant"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Kilometrage"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Prenom"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Marque"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Modele"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Numero de Serie"));

    return model;
}
QSqlQueryModel * Vehicule::triNOM()
{
    QSqlQueryModel * model=new QSqlQueryModel();

    model->setQuery("SELECT * from vehicule order by NOM ");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("Identifiant"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Kilometrage"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Prenom"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Marque"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Modele"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Numero de Serie"));

    return model;
}
QSqlQueryModel * Vehicule::triIDdesc()
{
    QSqlQueryModel * model=new QSqlQueryModel();

    model->setQuery("SELECT * from vehicule order by ID desc ");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("Identifiant"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Kilometrage"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Prenom"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Marque"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Modele"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Numero de Serie"));

    return model;
}
QSqlQueryModel * Vehicule::triNOMdesc()
{
    QSqlQueryModel * model=new QSqlQueryModel();

    model->setQuery("SELECT * from vehicule order by NOM desc");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("Identifiant"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Kilometrage"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Prenom"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Marque"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Modele"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Numero de Serie"));

    return model;
}










