#ifndef POCZTAPOLSKAAPI_H
#define POCZTAPOLSKAAPI_H

#include "trackapi.h"
#include <QDate>


class PocztaPolskaAPI: public TrackAPI
{
public:
    PocztaPolskaAPI();
    PocztaPolskaAPI(QString _trackum);
    PocztaPolskaAPI(tracking _tracking);
    virtual ~PocztaPolskaAPI();

protected:
    virtual void parseJson(QJsonDocument response);
};

#endif // POCZTAPOLSKAAPI_H
