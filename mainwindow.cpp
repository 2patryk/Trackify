#include "mainwindow.h"

#include "dialog_edit.h"
#include "ui_mainwindow.h"

#include <QtNetwork>
#include <QClipboard>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    lista_numerow_tmp = lista_numerow;
    baza_danych= QSqlDatabase::addDatabase("QSQLITE","baza");
    baza_danych.setDatabaseName("C:\\Users\\Patryk Ordon\\Documents\\Trackify\\db.sqlite3");

    isOpen = baza_danych.open();

    setFixedSize(size());
    getData();
    set_list_widget();
    setWindowIcon(QIcon(":/res/tracking.svg"));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::getData()
{
    if(isOpen)
    {


        QSqlQuery zapytanie(baza_danych);
        zapytanie.exec("CREATE TABLE IF NOT EXISTS TrackNum (id INTEGER UNIQUE PRIMARY KEY, tracking TEXT, nazwa TEXT, czy_dostarczono INTEGER, data_nadania DATE, data_akt DATE, rodzaj_przesylki TEXT, kurier TEXT, aktualny_status TEXT, miejscen TEXT, miejsced TEXT, dodatkowe TEXT)");
        zapytanie.clear();

        zapytanie.exec("CREATE TABLE IF NOT EXISTS TrackStat (id INTEGER UNIQUE PRIMARY KEY AUTOINCREMENT, datas DATE, miejsce TEXT, zdarzenie TEXT, opis TEXT, idTN int)");
        zapytanie.clear();


        ui->lwNumery->clear();
        lista_numerow.clear();
        tracking tmp;

        zapytanie.exec("SELECT nazwa, tracking, czy_dostarczono, kurier, id,data_nadania,rodzaj_przesylki,aktualny_status,miejscen,miejsced,dodatkowe,data_akt FROM TrackNum");

        while(zapytanie.next())
        {
            tmp.nazwa = zapytanie.value(0).toString();
            tmp.numer = zapytanie.value(1).toString();
            tmp.czydostarczono = zapytanie.value(2).toBool();
            tmp.kurier = zapytanie.value(3).toString();
            tmp.id = zapytanie.value(4).toInt();
            tmp.data_nadania = zapytanie.value(5).toDate();
            tmp.rodzaj_przesylki = zapytanie.value(6).toString();
            tmp.aktualnystatus = zapytanie.value(7).toString();
            tmp.miaston = zapytanie.value(8).toString();
            tmp.miastod = zapytanie.value(9).toString();
            tmp.extra = zapytanie.value(10).toString();
            tmp.data_akt = zapytanie.value(11).toDateTime();
            lista_numerow.append(tmp);
        }
        zapytanie.clear();
    int l = 0;
        foreach(tracking num,lista_numerow)
        {
            zapytanie.exec("SELECT datas, miejsce, zdarzenie, opis FROM TrackStat WHERE idTN = "+QString::number(num.id));
            while(zapytanie.next())
            {
                status temp;
                temp.data = zapytanie.value(0).toDateTime();
                temp.miejsce = zapytanie.value(1).toString();
                temp.zdarzenie = zapytanie.value(2).toString();
                temp.opis  = zapytanie.value(3).toString();
                lista_numerow[l].lista_zdarzen.append(temp);
            }
            zapytanie.clear();
l++;
        }
    }
    else
    {
        _msg.setText("Błąd odczytu bazy danych: " + baza_danych.lastError().text());
        _msg.exec();
    }

    qDebug() << "Liczba numerow: " << lista_numerow.count();


}


void MainWindow::Refresh()
{
int cr = ui->lwNumery->currentRow();

getData();
set_list_widget();

if(cr >= 0)
{
ui->lwNumery->setCurrentRow(cr);
}
else
{
ui->lwNumery->setCurrentRow(ui->lwNumery->count()-1);
}
}

void MainWindow::on_add_parcels_triggered()
{
          Dialog_add * dialog_add = new Dialog_add(lista_numerow,baza_danych,this);
          dialog_add->setWindowTitle("Dodaj paczke/paczki");
          dialog_add->exec();

          QStringList trackNum = dialog_add->getnumeryWyjscie();
          validateNumbers(trackNum);

          Refresh();


        int cr = ui->lwNumery->count();
        ui->lwNumery->setCurrentRow(cr-1);
}

void MainWindow::set_list_widget()
{
    if(lista_numerow.count() > 0)
    {


        ui->lwNumery->clear();

        for(int i=0; i<lista_numerow.count(); i++)
        {
              QListWidgetItem *newItem = new QListWidgetItem;
              newItem->setIcon(QIcon(":/res/default.png"));
            if(lista_numerow[i].kurier == PP)  newItem->setIcon(QIcon(":/res/envelo.png"));
            if(lista_numerow[i].kurier == IP)  newItem->setIcon(QIcon(":/res/inpost.png"));


            QFont f;
            f.setPointSize(10); // It cannot be 0
            newItem->setFont(f);
            QString tracknum;

            if(lista_numerow[i].nazwa == "not")
            {          
               tracknum = lista_numerow[i].numer;
            }
            else
            {
                 tracknum = lista_numerow[i].nazwa;
            }

            QColor color(124,124,124,255);
            QBrush brush(color);
            if(lista_numerow[i].czydostarczono == 1) {tracknum = "[D] " + tracknum ; newItem->setForeground(brush);}

            newItem->setText(tracknum);


            ui->lwNumery->addItem(newItem);
        }



    }else
    {
        tracking t;
        setLabels(t);
        ui->twStatusy->setRowCount(0);
    }


}

void MainWindow::on_remove_triggered()
{
    int cr = ui->lwNumery->currentRow();
    tracking tmp = lista_numerow[ui->lwNumery->currentIndex().row()];

    QSqlQuery zapytanie(baza_danych);
    zapytanie.exec("DELETE FROM TrackNum WHERE id = " + QString::number(tmp.id));
    zapytanie.clear();






    clearSqlTrack(tmp.id);
    Refresh();


    int l = ui->lwNumery->count();
    if(cr==l)
    {
     ui->lwNumery->setCurrentRow(cr-1);
    }
    else
    {
        ui->lwNumery->setCurrentRow(cr);
    }

    if(l <= 0)
    {
        ui->edit->setEnabled(false);
        ui->remove->setEnabled(false);
        ui->copy->setEnabled(false);
        ui->refresh->setEnabled(false);
    }

}


void MainWindow::on_copy_triggered()
{
    tracking tmp = lista_numerow[ui->lwNumery->currentIndex().row()];

    QClipboard *p_Clipboard = QApplication::clipboard();
    p_Clipboard->setText(tmp.numer);
}

void MainWindow::on_lwNumery_itemSelectionChanged()
{

    if(ui->lwNumery->count() > 0)
    {
       ui->edit->setEnabled(true);
       ui->remove->setEnabled(true);
       ui->copy->setEnabled(true);
       ui->refresh->setEnabled(true);
    }
    else
    {
        ui->edit->setEnabled(false);
        ui->remove->setEnabled(false);
        ui->copy->setEnabled(false);
        ui->refresh->setEnabled(false);
    }

     QSqlQuery zapytanie(baza_danych);
    zapytanie.exec("SELECT data_akt FROM TrackNum WHERE id = "+QString::number(lista_numerow[ui->lwNumery->currentRow()].id));
    QDateTime dt;
    bool datapusta = 0;
    while(zapytanie.next())
    {
        if(zapytanie.value(0).toString() != nullptr)
         dt = QDateTime::fromString(zapytanie.value(0).toString(),"yyyy-MM-dd hh:mm:ss");
        else
            datapusta = 1;

    }
QDateTime now = QDateTime::currentDateTime();
//  Odświeża autmoatyczne jeżeli ostatnio odświeżano poł godziny temu
if(!datapusta){
if(dt.secsTo(now)>1800 && lista_numerow[ui->lwNumery->currentRow()].czydostarczono==0)
{
   updateStatus();
}
else
{
    setLabels(lista_numerow[ui->lwNumery->currentRow()]);
}
}
else
{
    updateStatus();
}

}

void MainWindow::setLabels(tracking tmp)
{
   // ui->lwStatusy->setWrapping(false);
    ui->twStatusy->clear();
    ui->twStatusy->setColumnCount(3);
    ui->twStatusy->setColumnWidth(0,145);
    ui->twStatusy->setColumnWidth(1,295);
    ui->twStatusy->setColumnWidth(2,295);
    ui->twStatusy->setRowCount(0);

    ui->lUp->setText((tmp.data_akt.toString("yyyy-MM-dd HH:mm") == "") ? " - " : tmp.data_akt.toString("yyyy-MM-dd HH:mm"));

    ui->twStatusy->setHorizontalHeaderLabels({"Data","Status","Dodatkowe"});


//    ui->lwStatusy->clear();
    ui->lStatus->setText((tmp.aktualnystatus == "") ? " - " : tmp.aktualnystatus );
    ui->lNumer->setText(tmp.numer);
    tmp.nazwa!="not" ? ui->lNazwa->setText(tmp.nazwa) : ui->lNazwa->setText(" - ");

    ui->lKurier->setText(tmp.kurier);
    if(ui->lwNumery->count() < 0) ui->lKurier->setText(" - ");

    (tmp.extra == "") ? ui->lExtra->setText(" - ") : ui->lExtra->setText(tmp.extra);

    ui->lRodzaj->setText(tmp.rodzaj_przesylki);
    tmp.data_nadania == QDate::fromString("1999-09-09","yyyy-MM-dd") ? ui->lDataN->setText(" - ") :  ui->lDataN->setText(tmp.data_nadania.toString("yyyy-MM-dd"));
    ui->lMiejsceD->setText(tmp.miastod);
    ui->lMiejsceN->setText(tmp.miaston);


    int ile = tmp.lista_zdarzen.size();
    int r = 255,g=255,b=255;
    foreach(status stat,tmp.lista_zdarzen)
    {
        r-=(55/ile);
        g-=(55/ile);
        status temp;
        QTableWidgetItem * data = new QTableWidgetItem();
        QTableWidgetItem * status = new QTableWidgetItem();
        QTableWidgetItem * opis = new QTableWidgetItem();
        data->setText(stat.data.toString(("yyyy-MM-dd hh:mm")));
        data->setFlags(data->flags() ^ Qt::ItemIsEditable);
        ui->twStatusy->insertRow(ui->twStatusy->rowCount());
        ui->twStatusy->setItem(ui->twStatusy->rowCount()-1,0,data);
        QFont font;
        status->setBackground(QBrush(QColor(r, g, b)));
        status->setFont(font);
        status->setText(stat.zdarzenie);
        opis->setFlags(opis->flags() ^ Qt::ItemIsEditable);
        status->setFlags(status->flags() ^ Qt::ItemIsEditable);
        ui->twStatusy->setItem(ui->twStatusy->rowCount()-1,1,status);
        opis->setText((tmp.kurier == "Poczta Polska") ? stat.miejsce : stat.opis);
        ui->twStatusy->setItem(ui->twStatusy->rowCount()-1,2,opis);



//        QString tempS = stat.data.toString("yyyy-MM-dd hh:mm") + "  " + stat.zdarzenie + "  " + stat.opis + "  " + stat.miejsce;
//    ui->lwStatusy->addItem(tempS);
    }
    if(ui->twStatusy->rowCount() > 9) ui->twStatusy->setColumnWidth(2,286);

   // ui->twStatusy->setTextElideMode(Qt::ElideMiddle);
    //ui->twStatusy->resizeRowsToContents();
}

void MainWindow::updateStatus()
{

    tracking tmp = lista_numerow[ui->lwNumery->currentRow()];


    tmp.lista_zdarzen.clear();
    qWarning() << tmp.numer;
    TrackAPI *api;

    if(tmp.kurier == "Poczta Polska"){
    api = new PocztaPolskaAPI(tmp);

}else
    {
        if(tmp.kurier == "InPost")
        {
            api = new InPostAPI(tmp);
        }
        else
        {

             api = new TrackAPI(tmp);
             ui->statusBar->showMessage("Nie rozpoznano przewoźnika, edytuj przesyłkę. Program obsługuje przewoźników: Poczta Polska, InPost",3000);
        }
    }

    if(api->_error_) ui->statusBar->showMessage(api->_errordetails_,3000);


    lista_numerow[ui->lwNumery->currentRow()] = api->getTracking();

    if(api->getTracking().lista_zdarzen.count()>0) clearSqlTrack(tmp.id);

    tmp = lista_numerow[ui->lwNumery->currentRow()];

    //  Ustalenie czasu aktualizacji
    QString data_godzina =  QDate::currentDate().toString("yyyy-MM-dd")+ " " + QTime::currentTime().toString("HH:mm:ss");
     lista_numerow[(ui->lwNumery->currentRow())].data_akt = QDateTime::fromString(data_godzina,"yyyy-MM-dd HH:mm:ss");
     tmp.data_akt = lista_numerow[(ui->lwNumery->currentRow())].data_akt;
    updateSql(ui->lwNumery->currentRow(),lista_numerow[ui->lwNumery->currentRow()].id);
    setLabels(tmp);

    Refresh();
    return;
}

void MainWindow::updateSql(int i,int id)
{
    //nazwa, tracking, czy_dostarczono, kurier, id,data_nadania,rodzaj_przesylki,kurier,aktualny_status,miejscen,miejsced,dodatkowe,data_akt
     QSqlQuery zapytanie(baza_danych);
    zapytanie.exec("UPDATE TrackNum SET czy_dostarczono = '"+QString::number(lista_numerow[i].czydostarczono)+"', data_nadania = '"+lista_numerow[i].data_nadania.toString("yyyy-MM-dd")+"', rodzaj_przesylki = '"+lista_numerow[i].rodzaj_przesylki+"', aktualny_status = '"+lista_numerow[i].aktualnystatus+"', miejscen = '"+lista_numerow[i].miaston+"', miejsced = '"+lista_numerow[i].miastod+"', dodatkowe = '"+lista_numerow[i].extra+"', data_akt = '"+lista_numerow[i].data_akt.toString("yyyy-MM-dd HH:mm:ss")+"' WHERE id = "+QString::number(id));
            zapytanie.clear();
            qDebug() << "UPDATE TrackNum SET data_nadania = '"+lista_numerow[i].data_nadania.toString("yyyy-MM-dd")+"', rodzaj_przesylki = '"+lista_numerow[i].rodzaj_przesylki+"', aktualny_status = '"+lista_numerow[i].aktualnystatus+"', miejscen = '"+lista_numerow[i].miaston+"', miejsced = '"+lista_numerow[i].miastod+"', dodatkowe = '"+lista_numerow[i].extra+"', data_akt = '"+lista_numerow[i].data_akt.toString("yyyy-MM-dd HH:mm:ss")+"' WHERE id = "+QString::number(id);


     foreach(status stat,lista_numerow[i].lista_zdarzen)
     {

         zapytanie.exec("INSERT INTO TrackStat (datas, miejsce, zdarzenie, opis, idTN) VALUES('" +
                         stat.data.toString("yyyy-MM-dd HH:mm:ss")+"', '" + stat.miejsce + "', '"
                         + stat.zdarzenie + "', '" + stat.opis + "', '" + QString::number(id) +"')");
     zapytanie.clear();
     }

}

void MainWindow::clearSqlTrack(int id)
{
    QSqlQuery zapytanie(baza_danych);
    zapytanie.exec("DELETE FROM TrackStat WHERE idTN = " + QString::number(id));
    zapytanie.clear();
}

void MainWindow::on_import_parcels_triggered()
{
loadFromFile();

}


bool MainWindow::loadFromFile()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Wczytaj paczki z pliku"), "",
        tr("Dokument tekstowy (*.txt);;All Files (*)"));

    if (fileName.isEmpty())
            return 0;
        else {

            QFile file(fileName);

            if (!file.open(QIODevice::ReadOnly)) {
                QMessageBox::information(this, tr("Unable to open file"),
                    file.errorString());
                return 0;
            }

            QTextStream in(&file);
            QString test;
            test = in.readAll();
            test.replace("\r", "" );
            QStringList lista = test.split('\n');
            validateNumbers(lista);
            Refresh();
            int cr = ui->lwNumery->count();
            ui->lwNumery->setCurrentRow(cr-1);


}

    return 1;
}



void MainWindow::validateNumbers(QStringList tempNumeryLista)
{
    //  Obsługa błędów
    int POWTORZONY_NUMER_ERR = 2;

    int kod_bledu = 0;
    bool czy_powtorka = false;

    QString powtorkiNumerow;


    tempNumeryLista.removeDuplicates();
    int lastID = getLastID();


    for(int j=0; j<tempNumeryLista.count(); j++)
    {
        czy_powtorka = false;

        if(tempNumeryLista[j].count() > 0)
        {
        for(int i=0; i<lista_numerow.count(); i++)
        {
                if(tempNumeryLista[j] == lista_numerow[i].numer)
                {
                    kod_bledu = POWTORZONY_NUMER_ERR;
                    czy_powtorka = true;
                    powtorkiNumerow+= tempNumeryLista[j] + ", ";
                }
        }



        if(!czy_powtorka)
        {
            tracking tmp;
            tmp.numer = tempNumeryLista[j];

            if(baza_danych.isOpen())
            {
                QSqlQuery zapytanie(baza_danych);
                QString data_godzina =  QDate::currentDate().toString("yyyy-MM-dd")+ " " + QTime::currentTime().toString("HH:mm:ss");
                QString nazwa = "not";
                QString kurier = checkCourier(tmp.numer);
                bool czydostarczono = 0;

                lastID++;
                zapytanie.exec("INSERT INTO TrackNum (id, tracking, nazwa, kurier, czy_dostarczono) VALUES(" +
                                QString::number(lastID)+", '" + tmp.numer + "', '"
                                + nazwa + "', '" + kurier + "', '" + QString::number(czydostarczono) +"')");
                zapytanie.clear();

                 qDebug() << "INSERT INTO TrackNum (id, tracking, nazwa, kurier, czy_dostarczono, data_akt) VALUES(" +
                             QString::number(lastID)+", '" + tmp.numer + "', '"
                             + nazwa + "', '" + kurier + "', '" + QString::number(czydostarczono) + "', '" + data_godzina + "')";

            }
        }
        }
    }

    if(kod_bledu==2)
    {
        powtorkiNumerow.remove(powtorkiNumerow.count()-2, 2);
        _msg.setText("Nie dodano numerów: " + powtorkiNumerow + " z powodu powtórzenia");
        _msg.exec();
    }
}

int MainWindow::getLastID()
{
    if(lista_numerow.count() > 0)
    {
        return lista_numerow[lista_numerow.count()-1].id;
    }
    else
    {
        return 0;
    }
}
QString MainWindow::checkCourier(QString num)
{

    if(num.mid(0,5) == "testp") return PP;
    if(num.count() == 20) return PP;
    if(num.count() == 24) return IP;
    if(num.count() == 13)
    {
    if((num[0] > 64 && num[0] < 123)  && (num[1] > 64 && num[1] < 123) && (num[11] > 64 && num[11] < 123) && (num[12] > 64 && num[12] < 123) )
        return PP;
    }
    return NF;
}




void MainWindow::on_edit_triggered()
{
    //int cr = ui->lwNumery->currentRow();
    tracking tmp = lista_numerow[ui->lwNumery->currentIndex().row()];

    QSqlQuery zapytanie(baza_danych);
    zapytanie.clear();

    zapytanie.exec("SELECT nazwa, tracking, czy_dostarczono, kurier, id FROM TrackNum WHERE id = " + QString::number(lista_numerow[ui->lwNumery->currentIndex().row()].id));

    while(zapytanie.next())
    {
        tmp.nazwa = zapytanie.value(0).toString();
        tmp.numer = zapytanie.value(1).toString();
        tmp.czydostarczono = zapytanie.value(2).toBool();
        tmp.kurier = zapytanie.value(3).toString();
        tmp.id = zapytanie.value(4).toInt();
    }
    zapytanie.clear();


//    zapytanie.exec("SELECT DISTINCT kurier FROM TrackNum");

    QStringList firmykurierskie = {PP,IP,NF};
//    while(zapytanie.next())
//    {
//        firmykurierskie.append(zapytanie.value(0).toString());
//    }



    Dialog_edit * dialog_edit = new Dialog_edit(tmp,baza_danych,firmykurierskie,this);
    dialog_edit->setWindowTitle("Edytuj paczkę");
    dialog_edit->exec();

    Refresh();

}

void MainWindow::on_lwNumery_itemClicked(QListWidgetItem *item)
{

}

void MainWindow::on_twStatusy_cellClicked(int row, int column)
{

ui->statusBar->showMessage(lista_numerow[ui->lwNumery->currentRow()].lista_zdarzen[row].opis,3000);


}

void MainWindow::on_refresh_triggered()
{
updateStatus();

ui->statusBar->showMessage("Zaaktualizowano " + lista_numerow[ui->lwNumery->currentRow()].numer,3000);
}

void MainWindow::on_export_parcels_triggered()
{

//QString filename="C:\\Users\\Patryk Ordon\\Documents\\Trackify\\data.txt";
//QFile file( filename );

//if ( file.open(QIODevice::ReadWrite) )
//{
//    QTextStream stream( &file );
//    foreach(tracking t,lista_numerow)
//    {
//         stream << t.numer << endl;
//    }
//   file.close();
//}


    QString filter="Dokument tekstowy (*.txt);;All Files (*)";
    QString fileName = QFileDialog::getSaveFileName(this, "Save as","C://",filter);
    QFile file(fileName);

    if (!file.open(QFile::WriteOnly | QFile::Text)) {
         QMessageBox::warning(this, "Warning", "Cannot save file: " + file.errorString());
         return;
     }

   QString nazwa_pliku = fileName;

    QTextStream stream( &file );
      foreach(tracking t,lista_numerow)
       {
            stream << t.numer << endl;
       }
      file.flush();
      file.close();

ui->statusBar->showMessage("Zapisano numery do pliku "+fileName,3000);
}

void MainWindow::on_about_app_triggered()
{
    QMessageBox::about(this,
                           tr("O programie..."),
                   QString::fromUtf8(
                           "<p><b>Trackify</b></p>"
                           "<p lign=\"left\">Program służący do śledzenia paczek.<br> Obsługiwane serwisy: Poczta Polska, InPost<br><i>by Patryk Ordon<br>2019</i>")
                   );
}

void MainWindow::on_exit_triggered()
{
    close();
}
