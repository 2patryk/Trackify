#ifndef INPOSTAPI_H
#define INPOSTAPI_H
#include "trackapi.h"
#include <QMap>

class InPostAPI: public TrackAPI
{
    Q_OBJECT
public slots:
    void setJsonStatusReply(QNetworkReply* reply);
    void setJsonServicesReply(QNetworkReply* reply);

protected:
    QMap<QString,std::pair<QString,QString>> statuses;
    QMap<QString,QString> services;
public:
    InPostAPI();
    InPostAPI(QString _trackum);
    InPostAPI(tracking _tracking);
    virtual ~InPostAPI();

protected:
    virtual void parseJson(QJsonDocument response);
    void parseJsonStatuses(QJsonDocument response);
    void parseJsonServices(QJsonDocument response);

    void getStatuses();

};

#endif // INPOSTAPI_H
