#ifndef VEHICULE_H
#define VEHICULE_H
#include <QString>
#include<QSqlQueryModel>
#include<QMessageBox>
#include <QApplication>
#include <QSqlQuery>
#include <QtCharts/QPieSlice>
#include <QtCharts/QChartView>
#include <QtWidgets/QMainWindow>



class Vehicule
{
public:
  Vehicule();
  Vehicule(int,int,QString,QString,QString,QString,int);

  //Getters
  int getid();
  int getkilometrage();
  QString getnom();
  QString getprenom();
  QString getmarque();
  QString getmodele();
  int getNumSerie ();

  //Setters
  void setid(int);
  void setkilometrage(int);
  void setnom(QString);
  void setprenom(QString);
  void setmarque(QString);
  void setmodele(QString);
  void setNumSerie(int);

  //Les fonctions
  bool ajouter();
  QSqlQueryModel *afficher();
  bool supprimer(int);
  // Déclaration de la méthode rechercher
  bool modifier(int);
  bool rechercher(int);
  QSqlQueryModel *chercher(int);
  QSqlQueryModel * rechercher(QString valeur);
  QSqlQueryModel * triID();
  QSqlQueryModel * triNOM();
  QSqlQueryModel * triIDdesc();
  QSqlQueryModel * triNOMdesc();





private:
    int id,kilometrage,NumSerie;
    QString nom ,prenom,marque,modele;
};



#endif // VEHICULE_H
