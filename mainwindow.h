#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QMessageBox>
#include <QApplication>
#include <QStandardItemModel>
#include "vehicule.h"
#include"arduino.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    int m_currentPage = 0;
        const int m_pageSize = 3;
        //QSortFilterProxyModel* proxyModel;
        QStandardItemModel *model;

private slots:
    void on_pb_ajouter_clicked();
    void update_label();
    void on_pb_supprimer_clicked();

    void on_pb_modifier_clicked();

    void on_pushButton_rechercher_clicked();

    void on_pushButton_PDF_clicked();
    void on_triID_clicked();
    void on_nom_az_clicked();
    void on_nom_desc_clicked();
    void on_ID_desc_clicked();
    void on_qrcodegen_2_clicked();
    void on_pushButton_2_clicked();
    void addPagination();
    void on_moisuivant_clicked();
    void on_moisprec_clicked();





private:
    Ui::MainWindow *ui;
    Vehicule v;
    Vehicule *ve;
    Arduino A;
    QByteArray data;

};

#endif // MAINWINDOW_H
