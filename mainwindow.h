#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql>
#include <QMessageBox>
#include <QListWidget>

#include "dialog_add.h"
#include "tracking.h"
#include "pocztapolskaapi.h"
#include "inpostapi.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_add_parcels_triggered();

    void on_remove_triggered();

    void on_copy_triggered();

    void on_lwNumery_itemSelectionChanged();

    void on_import_parcels_triggered();

    void on_edit_triggered();

    void on_lwNumery_itemClicked(QListWidgetItem *item);

    void on_twStatusy_cellClicked(int row, int column);

    void on_refresh_triggered();

    void on_export_parcels_triggered();

    void on_about_app_triggered();

    void on_exit_triggered();

private:
    Ui::MainWindow *ui;
    QSqlDatabase baza_danych;

    QMessageBox mes;
    QString PP = "Poczta Polska";
    QString IP = "InPost";
    QString NF = "Nie ustalono";
public:
    QList<tracking> lista_numerow;
     QList<tracking> lista_numerow_tmp;
    void Refresh();
    bool loadFromFile();
    void validateNumbers(QStringList tempNumeryLista);
    void setLabels(tracking tmp);
    void updateStatus();
    void updateSql(int i,int id);
    void clearSqlTrack(int id);

    bool isOpen;
    QMessageBox _msg;
    QString checkCourier(QString num);

    int getLastID();
    void getData();
    void set_list_widget();
};



#endif // MAINWINDOW_H
