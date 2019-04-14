#include "inpostapi.h"

InPostAPI::InPostAPI()
{

}

InPostAPI::InPostAPI(QString _tracknum)
{
    this->_tracknum_ = _tracknum;
    this->_error_ = 0;
    this->_tracking_.kurier = "InPost";
    this->_apiurl_ = "https://api-shipx-pl.easypack24.net//v1/tracking/";
    this->getResponse();

}

InPostAPI::InPostAPI(tracking _tracking)
{
    this->_tracking_ = _tracking;
    this->_tracknum_ = _tracking.numer;
    this->_error_ = 0;
    this->_tracking_.kurier = "InPost";
    this->_apiurl_ = "https://api-shipx-pl.easypack24.net//v1/tracking/";
    this->getStatuses();
    this->getResponse();


}

InPostAPI::~InPostAPI()
{

}

void InPostAPI::parseJson(QJsonDocument response)
{
    QJsonObject danePrzesylkiObj = response.object();
    QString status = QString::number((danePrzesylkiObj["status"].toDouble()));


    //qDebug() << status;
    if(status == "404" || status == "400")
    {
    _error_ = 1;
    _errordetails_ = "Paczka o numerze " + _tracknum_ + " nie istnieje [InPost]";

    class status tmp;
    tmp.zdarzenie = "Brak danych";
   _tracking_.miaston = " - ";
   _tracking_.miastod = " - ";
   _tracking_.aktualnystatus = "Przesyłka o podanym numerze nie istnieje";
    return;
    }

    //  NUMER POPRAWNY
    _tracking_.data_nadania = QDate::fromString((danePrzesylkiObj["created_at"].toString().mid(0,10) == "") ? "1999-09-09" : danePrzesylkiObj["created_at"].toString().mid(0,10),"yyyy-MM-dd");
    _tracking_.numer = danePrzesylkiObj["tracking_number"].toString();
    //_tracking_.krajn = danePrzesylkiObj["krajNadania"].toString();
    //_tracking_.krajd = danePrzesylkiObj["krajPrzezn"].toString();
    _tracking_.rodzaj_przesylki = services.value(danePrzesylkiObj["service"].toString());
    _tracking_.czydostarczono = (danePrzesylkiObj["status"].toString() == "delivered") ? 1 : 0;
    _isdelivered_ = _tracking_.czydostarczono;


    QJsonObject jsons;

     QJsonValue atr = danePrzesylkiObj.value(QString("custom_attributes"));
     QJsonObject atrObj = atr.toObject();
QString rozmiar;
     if(atrObj["size"].toString() != "")
     rozmiar = "Gabaryt " + atrObj["size"].toString() + ", ";
     else
      rozmiar = "";
     QString paczkomat = atrObj["target_machine_id"].toString();

     QJsonValue atrs = atrObj.value(QString("target_machine_detail"));
     QJsonObject atrsObj = atrs.toObject();
     QString opisp = atrsObj["location_description"].toString();

//     QJsonValue atrss = atrsObj.value(QString("address"));
//     QJsonObject atrssObj = atrss.toObject();
//     QString adres = atrssObj["line1"].toString() + ", " + atrssObj["line2"].toString();

     if(paczkomat != "")
     _tracking_.extra = rozmiar + "Paczkomat " + paczkomat + ", " + opisp;
     else
         _tracking_.extra = " - ";


    //  MIASTO NADANIA
    _tracking_.miaston = (_error_ == 1 ) ? "Brak danych" : (_tracknum_.mid(1,2) + "-" + _tracknum_.mid(3,3));
    //  MIASTO PRZEZNACZENIA
   _tracking_.miastod = (_error_ == 1 ) ? "Brak danych" : (_tracknum_.mid(10,2) + "-" + _tracknum_.mid(12,3));

//   QString fUrlN = "http://kodypocztoweapi.pl/json/" + _tracknum_.mid(1,2) + "-" + _tracknum_.mid(3,3);
//   QString fUrlD = "http://kodypocztoweapi.pl/json/" + _tracknum_.mid(10,2) + "-" + _tracknum_.mid(12,3);
//   qWarning() << fUrlN;
//   QUrl qUrlN(fUrlN);
//   QUrl qUrlD(fUrlD);
//   QNetworkRequest request(qUrlN);
//   QNetworkRequest request2(qUrlD);
//   request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
//   request2.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
//    QByteArray postData;
//   QNetworkAccessManager nam,nam2;
//   QNetworkReply *reply = nam.post(request, QJsonDocument(jsons).toJson());

//   while(!reply->isFinished())
//   {
//   qApp->processEvents();
//   }
//   QByteArray response_data = reply->readAll();
//   QJsonDocument json = QJsonDocument::fromJson(response_data);

//   //qDebug() << json;
//   reply->deleteLater();


    //  ZDARZENIA

    QJsonValue Zdarzenia = danePrzesylkiObj.value(QString("tracking_details"));
    QJsonArray listaZdarzen = Zdarzenia.toArray();
    //qDebug()<< listaZdarzen;

    int id = 0;

    foreach(QJsonValue val,listaZdarzen)
    {

        QJsonObject tempzd = val.toObject();
        class status temp;
        temp.data = QDateTime::fromString((tempzd["datetime"].toString().mid(0,16) == "") ? "1999-09-09 00:00" : tempzd["datetime"].toString().mid(0,10)+" "+tempzd["datetime"].toString().mid(11,5),"yyyy-MM-dd hh:mm");
        temp.zdarzenie = statuses.value(tempzd["status"].toString()).first;
        temp.opis = statuses.value(tempzd["status"].toString()).second;
        temp.id = id++;

//        //  NAZWA MIEJSCA
//        QJsonValue miejsce = tempzd.value(QString("jednostka"));
//        QJsonObject miejsceObj = miejsce.toObject();
//        temp.miejsce = (miejsceObj["nazwa"].toString() == "") ? " - " : miejsceObj["nazwa"].toString();
           _tracking_.lista_zdarzen.append(temp);
    }

    _num_of_events_ = _tracking_.lista_zdarzen.size();
 _tracking_.aktualnystatus = statuses.value(danePrzesylkiObj["status"].toString()).first;
 std::reverse(_tracking_.lista_zdarzen.begin(), _tracking_.lista_zdarzen.end());    //  ODWRÓCENIE KOLEJNOŚCI STATUSÓW
return;
}

void InPostAPI::parseJsonStatuses(QJsonDocument response)
{

    QJsonObject obj = response.object();
    QJsonValue lista = obj.value(QString("items"));
    QJsonArray tablica = lista.toArray();
//    qDebug() << tablica.size();


     foreach(QJsonValue val,tablica)
     {
          QJsonObject tempzd = val.toObject();
          statuses.insert(tempzd["name"].toString(),std::make_pair(tempzd["title"].toString(),tempzd["description"].toString()));
     }

     return;
}

void InPostAPI::parseJsonServices(QJsonDocument response)
{
    QJsonArray tablica = response.array();


     foreach(QJsonValue val,tablica)
     {
          QJsonObject tempzd = val.toObject();
          services.insert(tempzd["id"].toString(),tempzd["name"].toString());
     }

     //qDebug() << tablica.size();
     return;
}



void InPostAPI::getStatuses()
{
    QString fUrl = "https://api-shipx-pl.easypack24.net/v1/statuses";
    QString fUrl2 = "https://api-shipx-pl.easypack24.net/v1/services";
    //QString fUrl3 = "http://kodypocztoweapi.pl/json/";
    //qWarning() << fUrl;
    QUrl qUrl(fUrl);
    QNetworkRequest request(qUrl);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");


    QEventLoop loop;
    QNetworkAccessManager *manager = new QNetworkAccessManager( this );
    connect(manager, SIGNAL(finished(QNetworkReply*)),this,SLOT(setJsonStatusReply(QNetworkReply*)));
    connect(manager, SIGNAL(finished(QNetworkReply*)),&loop, SLOT(quit()));
    manager->get( request )  ;

    loop.exec();

    QEventLoop loop2;
    request.setUrl(fUrl2);
    manager = new QNetworkAccessManager( this );
    connect(manager, SIGNAL(finished(QNetworkReply*)),this,SLOT(setJsonServicesReply(QNetworkReply*)));
    connect(manager, SIGNAL(finished(QNetworkReply*)),&loop2, SLOT(quit()));
    manager->get( request )  ;
    loop2.exec();
    return;
}

void InPostAPI::setJsonStatusReply(QNetworkReply * reply)
{
    QByteArray response_data = reply->readAll();
    parseJsonStatuses(QJsonDocument::fromJson(response_data));
}

void InPostAPI::setJsonServicesReply(QNetworkReply *reply)
{
    QByteArray response_data = reply->readAll();
    parseJsonServices(QJsonDocument::fromJson(response_data));
}


