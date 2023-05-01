#include "mainwindow.h"
#include "vehicule.h"
#include "ui_mainwindow.h"
#include<QMessageBox>
#include <QApplication>
#include<QIntValidator>
#include <QFileDialog>
#include <QPdfWriter>
#include <QtDebug>
#include <QPainter>
#include <QDesktopServices>
#include <QTextDocument>
#include <QDate>
#include "qrcode.h"
#include <QtSql>
#include <QtCharts>
#include <QtGui>
#include<QtCharts>
#include<QChartView>
#include<QPieSeries>
#include<QPieSlice>
#include <QChart>

using namespace QrCodegen;
QString days="";
using namespace std;







MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
   // ui->leid->setValidator( new QIntValidator(0, 9999999, this));
    ui->kilometrage->setValidator( new QIntValidator(0, 9999999, this));
    ui->NumS->setValidator( new QIntValidator(0, 9999999, this));
    ui->leid->setValidator( new QIntValidator(0, 9999999, this));
    ui->kilometrage->setValidator( new QIntValidator(0, 9999999, this));
    ui->NumS->setValidator( new QIntValidator(0, 9999999, this));

    ui->tableView_2->setModel(v.afficher());

m_currentPage = 0;
int ret=A.connect_arduino(); // lancer la connexion à arduino

switch(ret){
case(0):qDebug()<< "arduino is available and connected to : "<< A.getarduino_port_name();
    break;
case(1):qDebug() << "arduino is available but not connected to :" <<A.getarduino_port_name();
   break;
case(-1):qDebug() << "arduino is not available";
}
 QObject::connect(A.getserial(),SIGNAL(readyRead()),this,SLOT(update_label())); // permet de lancer
 //le slot update_label suite à la reception du signal readyRead (reception des données).

}
void MainWindow::update_label()
{
 data="";


data=A.read_from_arduino();


qDebug()<<data;
if(data!="")
{
    if(A.chercherid(data)!=-1)
    {QString nom=A.chercher(data);
        qDebug() << nom ;
        QByteArray x=nom.toUtf8();
        qDebug() << x ;
        A.write_to_arduino(x);
        qDebug()<<"valdie";

         }
    else
    { A.write_to_arduino("0"); qDebug()<<"invaldie"; }


}
 }

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_pb_ajouter_clicked()
{
    //Récupération des informations saisies dans les 5 champs
       int id=ui->leid->text().toInt();
       int kilometrage=ui->kilometrage->text().toInt();
       QString nom=ui->lenom->text();
       QString prenom=ui->leprenom->text();
       QString marque=ui->lamarque->text();
       QString modele=ui->lemodele->text();
       int NumSerie=ui->NumS->text().toInt();

       //instancier l'objet de la classe vehicule
       Vehicule V(id,kilometrage,prenom,nom,marque,modele,NumSerie);

    //insérer l'objet piste instancié dans la table piste
    //et récupérer la valeur de retour de query.exec()
    bool test=V.ajouter();

    //si la requete executée => QMessageBox::information
    if(test)
    {
        QMessageBox::information(nullptr, QObject::tr("OK"),
                     QObject::tr("Ajout effectué\n"
                                 "Click Cancel to exit"), QMessageBox::Cancel);

        //Actualiser le tableau apres l'ajout
        ui->tableView_2->setModel(v.afficher());
    }
    //si la requete non exécuté => QMessageBox::critical
    else
    {
        QMessageBox::critical(nullptr, QObject::tr("NOT OK"),
                     QObject::tr("Ajout non effectué\n"
                                 "Click Cancel to exit"), QMessageBox::Cancel);
    }


}

void MainWindow::on_pb_supprimer_clicked()
{

        Vehicule v1;
        v1.setid(ui->id_supp->text().toInt());

        bool existe = v1.rechercher(v1.getid()); // Rechercher le véhicule

        if (existe) {
            // Demander une confirmation avant la suppression
            QMessageBox::StandardButton confirmation = QMessageBox::question(this, "Confirmation", "Voulez-vous vraiment supprimer ce véhicule ?", QMessageBox::Yes | QMessageBox::No);
            if (confirmation == QMessageBox::Yes) {
                bool test = v1.supprimer(v1.getid()); // Supprimer le véhicule
                if (test) {
                    QMessageBox::information(this, "Succès", "Suppression avec succès.");
                    ui->tableView_2->setModel(v.afficher());
                } else {
                    QMessageBox::warning(this, "Echec", "Echec de suppression.");
                }
            }
        } else {
            QMessageBox::warning(this, "Erreur", "Le véhicule n'existe pas."); // Alerter l'utilisateur que le véhicule n'a pas été trouvé
        }


}
void MainWindow::on_pb_modifier_clicked()
{

        int id=ui->leid->text().toInt();
        int kilometrage=ui->kilometrage->text().toInt();
        QString nom=ui->lenom->text();
        QString prenom=ui->leprenom->text();
        QString marque=ui->lamarque->text();
        QString modele=ui->lemodele->text();
        int NumSerie=ui->NumS->text().toInt();


    Vehicule V(id,kilometrage,nom,prenom,marque,modele,NumSerie);
    bool test=V.modifier(id);
    if(test){
    ui->tableView_2->setModel(V.afficher());}
    // Vérifier que l'identifiant saisi est valide
            if (id <= 0)
            {
                QMessageBox::warning(this, "Erreur", "Veuillez entrer une valeur numérique valide pour l'identifiant.");
                return;
            }
            // Vérifier que les champs obligatoires sont remplis
                if (kilometrage <=0|| nom.isEmpty() ||prenom.isEmpty()|| marque.isEmpty() || modele.isEmpty()|| NumSerie <=0 )
                {
                    QMessageBox::warning(this, "Erreur", "Veuillez remplir tous les champs obligatoires.");
                    return;
                }

            // Vérifier que l'identifiant existe dans la table
            QSqlQuery query;
            query.prepare("SELECT * FROM reservation WHERE id = :id");
            query.bindValue(":id", id);

            if (!query.exec())
            {
                QMessageBox::critical(this, "Erreur", "Une erreur s'est produite lors de la vérification de l'identifiant.");
                return;
            }
            if (!query.next())
            {
                QMessageBox::warning(this, "Attention", "L'identifiant à modifier n'existe pas dans la table.");
                return;
            }

            // Demander à l'utilisateur de confirmer la modification
            QMessageBox::StandardButton confirmUpdate = QMessageBox::question(this, "Confirmation",
                "Êtes-vous sûr de vouloir modifier cet enregistrement ?",
                QMessageBox::Yes|QMessageBox::No);

            if (confirmUpdate == QMessageBox::No) {
                return; // L'utilisateur a annulé la modification, sortir de la fonction
            }

            // Exécuter la requête SQL pour mettre à jour l'enregistrement

            query.prepare("UPDATE reservation SET kilometrage = :kilometrage, nom = :nom, prenom = :prenom ,marque = :marque, modele = :modele WHERE id = :id");
            query.bindValue(":id", id);
            query.bindValue(":kilometrage", kilometrage);
            query.bindValue(":nom", nom);
            query.bindValue(":prenom", prenom);
            query.bindValue(":marque", marque);
            query.bindValue(":modele", modele);
            query.bindValue(":NumSerie", NumSerie);


            if (query.exec())
            {
                QMessageBox::information(nullptr, QObject::tr("OK"),
                             QObject::tr("Modification effectuée\n"
                                         "Click Cancel to exit"), QMessageBox::Cancel);

                //Actualiser le tableau après la modification
                ui->tableView_2->setModel(V.afficher());
            }
            else
            {
                QMessageBox::critical(nullptr, QObject::tr("NOT OK"),
                             QObject::tr("Modification non effectuée\n"
                                         "Click Cancel to exit"), QMessageBox::Cancel);
            }
    }
void MainWindow::on_pushButton_rechercher_clicked()
{
    // Récupérer la valeur de recherche et supprimer les espaces inutiles
    QString valeur = ui->lerechercher->text().trimmed();

    // Vérifier si la valeur de recherche est vide
    if (valeur.isEmpty())
    {
        // Afficher un message d'erreur
        QMessageBox::warning(this, "Recherche", "Veuillez saisir une valeur de recherche.");
        // Quitter la fonction sans exécuter la recherche
        return;
    }

    // Appeler la fonction de recherche dans la classe Piste et récupérer le modèle de résultats
    QSqlQueryModel *model = v.rechercher(valeur);

    // Vérifier si aucun résultat n'a été trouvé
    if (model->rowCount() == 0)
    {
        // Afficher un message d'information
        QMessageBox::information(this, "Recherche", "AQucun résultat trouvé.");
    }
    else
    {
        // Créer un message avec le nombre de résultats trouvés
        QString message = QString("%1 résultat(s) trouvé(s).").arg(model->rowCount());
        // Afficher un message d'information avec le nombre de résultats trouvés
        QMessageBox::information(this, "Recherche", message);
        // Définir le modèle de résultats dans la table
        ui->tableView_2->setModel(model);
        // Trier les résultats par ordre croissant de l'id
        ui->tableView_2->sortByColumn(0, Qt::AscendingOrder);
    }
}


void MainWindow::on_pushButton_PDF_clicked()
{

    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                         "/home",
                                                         QFileDialog::ShowDirsOnly
                                                         | QFileDialog::DontResolveSymlinks);
    qDebug()<<dir;
    QPdfWriter pdf(dir+"/PdfList.pdf");
    QPainter painter(&pdf);
    int i = 4000;

    painter.drawPixmap(QRect(100,100,2000,2000),QPixmap("C:/Users/Admin/Desktop/msi.png"));
    painter.setPen(Qt::red);
    painter.setFont(QFont("Time New Roman", 25));
    painter.drawText(3000,1450,"Liste Des Vehicule");
    painter.setPen(Qt::black);
    painter.setFont(QFont("Time New Roman", 15));
    painter.drawRect(100,100,9400,2500);
    painter.drawRect(100,3000,9400,500);
    painter.setFont(QFont("Time New Roman", 10));
    painter.drawText(300,3300,"ID");
    painter.drawText(1500,3300,"Kilométrage");
    painter.drawText(3000,3300,"Nom");
    painter.drawText(4800,3300,"Prenom");
    painter.drawText(6600,3300,"Marque");
    painter.drawText(8400,3300,"Modele");
    painter.drawText(10000,3300,"NumSerie");

    painter.drawRect(100,3000,9400,10700);

    QTextDocument previewDoc;
    QString pdflist = QDate::currentDate().toString("'data_'MM_dd_yyyy'.txt'");

    QTextCursor cursor(&previewDoc);

    QSqlQuery query;
    query.prepare("SELECT * FROM vehicule");
    query.exec();
    while (query.next())
    {
        painter.drawText(300,i,query.value(0).toString());
        painter.drawText(1500,i,query.value(1).toString());
        painter.drawText(3000,i,query.value(2).toString());
        painter.drawText(4800,i,query.value(3).toString());
        painter.drawText(6600,i,query.value(4).toString());
        painter.drawText(8400,i,query.value(5).toString());
        painter.drawText(10000,i,query.value(6).toString());

        i = i +500;
    }
    int reponse = QMessageBox::question(this, "Générer PDF", " PDF Enregistré ! Voulez Vous Affichez Le PDF ?",
                                            QMessageBox::Yes|QMessageBox::No);
    if (reponse == QMessageBox::Yes)
    {
        QDesktopServices::openUrl(QUrl::fromLocalFile(dir+"/PdfList.pdf"));
        painter.end();
    }
    else
    {
         painter.end();
    }
}


void MainWindow::on_triID_clicked()
{
    ui->tableView_2->setModel(v.triID());

}

void MainWindow::on_nom_az_clicked()
{
     ui->tableView_2->setModel(v.triNOM());
}

void MainWindow::on_nom_desc_clicked()
{
    ui->tableView_2->setModel(v.triNOMdesc());
}

void MainWindow::on_ID_desc_clicked()
{
    ui->tableView_2->setModel(v.triIDdesc());
}

void MainWindow::on_qrcodegen_2_clicked()
{



    int rowIndex = ui->tableView_2->currentIndex().row();
    QVariant idValue = ui->tableView_2->model()->data(ui->tableView_2->model()->index(rowIndex, 0));
    int id = idValue.toInt();

    QSqlQuery qry;
    qry.prepare("SELECT * FROM vehicule WHERE id=:id");
    qry.bindValue(":id", id);
    qry.exec();

    QString qrText = "";
    while (qry.next()) {
        int NumSerie = qry.value(1).toInt();
        QString nom = qry.value(2).toString();
        QString prenom = qry.value(3).toString();
        QString marque = qry.value(4).toString();
        QString modele = qry.value(5).toString();
        int kilometrage = qry.value(6).toInt();

        // Create QR code text
        qrText = "NumSerie:" + QString::number(NumSerie) +
                 ",Nom:" + nom +
                 ",Prenom:" + prenom +
                 ",Marque:" + marque +
                 ",Modele:" + modele +
                 ",Kilometrage:" + QString::number(kilometrage);
    }

    QrCode qr = QrCode::encodeText(qrText.toUtf8().constData(), QrCode::Ecc::HIGH);

    // Read the black & white pixels
    QImage image(qr.getSize(), qr.getSize(), QImage::Format_RGB888);
    for (int y = 0; y < qr.getSize(); y++) {
        for (int x = 0; x < qr.getSize(); x++) {
            int color = qr.getModule(x, y);  // 0 for white, 1 for black

            // You need to modify this part
            if (color == 0)
                image.setPixel(x, y, qRgb(254, 254, 254));
            else
                image.setPixel(x, y, qRgb(0, 0, 0));
        }
    }
    image = image.scaled(200, 200);
    ui->qrcode->setPixmap(QPixmap::fromImage(image));


}
void MainWindow::on_pushButton_2_clicked()
{
    QSqlQueryModel * model= new QSqlQueryModel();
                                    model->setQuery("SELECT type FROM vehicule ");
                                    float e=model->rowCount();
                                    model->setQuery("SELECT kilometrage FROM vehicule ");
                                    float ee=model->rowCount();
                                    float total=e+ee;
                                    QString a=QString("NumSerie"+QString::number((e*100)/total,'f',2)+"%" );
                                    QString b=QString("kilometrage"+QString::number((ee*100)/total,'f',2)+"%" );
                                    QPieSeries *series = new QPieSeries();
                                    series->append(a,e);
                                    series->append(b,ee);
                            if (e!=0)
                            {QPieSlice *slice = series->slices().at(0);
                             slice->setLabelVisible();
                             slice->setPen(QPen());}
                            if ( ee!=0)
                            {
                                     // Add label, explode and define brush for 2nd slice
                                     QPieSlice *slice1 = series->slices().at(1);
                                     //slice1->setExploded();
                                     slice1->setLabelVisible();
                            }
                                    // Create the chart widget
                                    QChart *chart = new QChart();
                                    // Add data to chart with title and hide legend
                                    chart->addSeries(series);
                                    chart->setTitle("Pourcentage des NumSerie et kilometrage : nombre total : "+ QString::number(total));
                                    chart->legend()->hide();
                                    // Used to display the chart
                                    QChartView *chartView = new QChartView(chart);
                                    chartView->setRenderHint(QPainter::Antialiasing);
                                    chartView->resize(1000,500);
                                    chartView->show();
}
void MainWindow::addPagination()
{


int currentPage = ui->tableView_2->currentIndex().row() / m_pageSize;
int rowCount = ui->tableView_2->model()->rowCount();

int startRow = currentPage * m_pageSize;
int endRow = qMin(startRow + m_pageSize, rowCount) - 1;

// Show only the rows for the current page
// QAbstractItemModel *model = ui->tabmedi_3->model();
for (int row = 0; row < rowCount; row++) {
if (row >= startRow && row <= endRow) {
ui->tableView_2->setRowHidden(row, false);
} else {
ui->tableView_2->setRowHidden(row, true);
}
}


/*
// Disable "Previous Month" and "Next Month" buttons if necessary
ui->moisprec->setEnabled(currentPage > 0);
ui->moisuivant->setEnabled(currentPage < totalPages - 1);
*/



}

void MainWindow::on_moisuivant_clicked()
{ QModelIndex currentIndex = ui->tableView_2->currentIndex();
int nextIndexRow = currentIndex.row() + m_pageSize;
int rowCount = ui->tableView_2->model()->rowCount();
if (nextIndexRow >= rowCount) {
nextIndexRow = rowCount - 1;
}

m_currentPage = nextIndexRow / m_pageSize;
addPagination();

QModelIndex nextIndex = ui->tableView_2->model()->index(nextIndexRow, currentIndex.column());
ui->tableView_2->setCurrentIndex(nextIndex);
}



void MainWindow::on_moisprec_clicked()
{
m_currentPage--;
if (m_currentPage < 0) {
m_currentPage = 0;
}
addPagination();

// Aller à la page précédente
int prevPageStartRow = m_currentPage * m_pageSize;
int rowCount = ui->tableView_2->model()->rowCount();
if (prevPageStartRow < rowCount) {
QModelIndex prevIndex = ui->tableView_2->model()->index(prevPageStartRow, 0);
ui->tableView_2->setCurrentIndex(prevIndex);
}
}

