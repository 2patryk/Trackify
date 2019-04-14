#ifndef TRACKAPI_H
#define TRACKAPI_H

#include "tracking.h"

#include <QDate>
#include <QJsonDocument>
#include <QUrl>
#include <QtNetwork>


class TrackAPI : public QObject
{
    Q_OBJECT

public slots:
    void setJsonReply(QNetworkReply *reply);

    public:
     tracking _tracking_;
     QString _tracknum_;
     QString _apiurl_;
     int _num_of_events_;
     bool _isdelivered_;
     bool _error_;
     QString _errordetails_;

     QNetworkAccessManager *manager;
     QNetworkRequest request;
     QJsonDocument json_reply;

public:
    TrackAPI();
    TrackAPI(QString _tracknum);
    TrackAPI(tracking _tracking);
    virtual ~TrackAPI();

    tracking getTracking();


protected:
    void getResponse();
    virtual void parseJson(QJsonDocument response);


};

#endif // TRACKAPI_H
