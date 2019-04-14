#include "trackapi.h"



TrackAPI::TrackAPI()
{

}


TrackAPI::TrackAPI(QString _tracknum)
{
this->_tracknum_ = _tracknum;
}

TrackAPI::~TrackAPI()
{

}

tracking TrackAPI::getTracking()
{
    return _tracking_;
}

void TrackAPI::getResponse()
{
    QString fUrl = _apiurl_ + _tracknum_;

   // qWarning() << fUrl;
    QUrl qUrl(fUrl);
    QNetworkRequest request(qUrl);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");


    QEventLoop loop;

    QNetworkAccessManager *manager = new QNetworkAccessManager( this );;
    connect(manager, SIGNAL(finished(QNetworkReply*)),this,SLOT(setJsonReply(QNetworkReply*)));
    connect(manager, SIGNAL(finished(QNetworkReply*)),&loop, SLOT(quit()));
    manager->get( request )  ;

    loop.exec();



//    QNetworkReply* reply;
//    connect(networkManager, SIGNAL( finished( QNetworkReply* ) ), &loop, SLOT( quit() ) );
//    reply = networkManager->get(request);
//    loop.exec();
//    reply->deleteLater();
//    json_reply = QJsonDocument::fromJson(reply->readAll());


//    QJsonObject json;

//    QNetworkAccessManager nam;
//    QNetworkReply *reply = nam.post(request, QJsonDocument(json).toJson());

//    while(!reply->isFinished())
//    {
//    qApp->processEvents();
//    }

//    QByteArray response_data = reply->readAll();
//    QJsonDocument response_json = QJsonDocument::fromJson(response_data);

//    reply->deleteLater();


 return;
}

void TrackAPI::setJsonReply(QNetworkReply * reply)
{
    QByteArray response_data = reply->readAll();
    json_reply = QJsonDocument::fromJson(response_data);
    parseJson(json_reply);
}

TrackAPI::TrackAPI(tracking _tracking)
{
this->_tracknum_ = _tracking.numer;
this->_tracking_ = _tracking;
this->_error_ = 0;
this->_tracking_.kurier = "Nie ustalono";
this->_tracking_.aktualnystatus = "Ustal przewoźnika";
}

void TrackAPI::parseJson(QJsonDocument response)
{

}


