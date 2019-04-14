#include "pocztapolskaapi.h"

#include <QDate>




PocztaPolskaAPI::PocztaPolskaAPI()
{

}

PocztaPolskaAPI::PocztaPolskaAPI(QString _tracknum)
{
    this->_tracknum_ = _tracknum;
    this->_error_ = 0;
    this->_tracking_.kurier = "Poczta Polska";
    this->_apiurl_ = "http://guider.vxm.pl/testpatryk/test.php?number=";
    this->getResponse();

}

PocztaPolskaAPI::PocztaPolskaAPI(tracking _tracking)
{
    this->_tracking_ = _tracking;
    this->_tracknum_ = _tracking.numer;
    this->_error_ = 0;
    this->_tracking_.kurier = "Poczta Polska";
    this->_apiurl_ = "http://guider.vxm.pl/testpatryk/test.php?number=";
    this->getResponse();

}

PocztaPolskaAPI::~PocztaPolskaAPI()
{

}

void PocztaPolskaAPI::parseJson(QJsonDocument response)
{

    QJsonObject obj = response.object();
    QJsonValue danePrzesylki = obj.value(QString("danePrzesylki"));
    QJsonObject danePrzesylkiObj = danePrzesylki.toObject();
    QString numr = obj["numer"].toString();
    int status = obj["status"].toInt();



    this->_errordetails_ = "test";
    if(status != 0 )
    {
    this->_error_ = 1;
    switch(status)
    {
    case -1:
        _errordetails_ = "Brak przesyłki o podanym numerze " + numr + " [Poczta Polska]";
        _tracking_.aktualnystatus = "Brak przesyłki o podanym numerze";
        break;
    case 1:
        _errordetails_ = "Są inne przesyłki o podanym numerze " + numr + " [Poczta Polska]";
        _tracking_.aktualnystatus = "Są inne przesyłki o podanym numerze";
        break;
    case -2:
        _errordetails_ = "Niepoprawny numer przesyłki " + numr + " [Poczta Polska]";
        _tracking_.aktualnystatus = "Niepoprawny numer przesyłki";
        break;
    case -99:
        _errordetails_ = "Nieokreślony błąd " + numr + " [Poczta Polska]";
         _tracking_.aktualnystatus = "Nieokreślony błąd";
        break;
    default:
        break;
    }
    class status tmp;
    this->_tracking_.numer = _tracknum_;
//    tmp.zdarzenie = "Brak danych";
//    this->_tracking_.lista_zdarzen.append(tmp);
    //this->_tracking_.aktualnystatus = "Brak danych";
    if(status!=1) return;
    }

    //  NUMER POPRAWNY

    this->_tracking_.data_nadania = QDate::fromString((danePrzesylkiObj["dataNadania"].toString() == "") ? "1999-09-09" : danePrzesylkiObj["dataNadania"].toString(),"yyyy-MM-dd");
    _tracking_.numer = (_error_ != 1) ? danePrzesylkiObj["numer"].toString() : _tracknum_;
    _tracking_.krajn = danePrzesylkiObj["krajNadania"].toString();
    _tracking_.krajd = danePrzesylkiObj["krajPrzezn"].toString();
    _tracking_.rodzaj_przesylki = danePrzesylkiObj["rodzPrzes"].toString();
    _tracking_.czydostarczono = danePrzesylkiObj["zakonczonoObsluge"].toBool();
    _isdelivered_ = _tracking_.czydostarczono;

    _tracking_.extra = "Kraj nadania: " + _tracking_.krajn + ",   Kraj docelowy: " + _tracking_.krajd;
    //  MIASTO NADANIA
    QJsonValue daneMiasta = danePrzesylkiObj.value(QString("urzadNadania"));
    QJsonObject daneMiastaObj = daneMiasta.toObject();
    QJsonValue daneMiastaSzcz = daneMiastaObj.value(QString("daneSzczegolowe"));
    QJsonObject daneMiastaSzczObj = daneMiastaSzcz.toObject();
    _tracking_.miaston = (daneMiastaSzczObj["miejscowosc"].toString() == "") ? "Brak danych" : daneMiastaSzczObj["miejscowosc"].toString();

    //  MIASTO PRZEZNACZENIA
    daneMiasta = danePrzesylkiObj.value(QString("urzadPrzezn"));
    daneMiastaObj = daneMiasta.toObject();
    daneMiastaSzcz = daneMiastaObj.value(QString("daneSzczegolowe"));
    daneMiastaSzczObj = daneMiastaSzcz.toObject();
    _tracking_.miastod = (daneMiastaSzczObj["miejscowosc"].toString() == "") ? "Brak danych" : daneMiastaSzczObj["miejscowosc"].toString();

    //  ZDARZENIA

    QJsonValue Zdarzenia = danePrzesylkiObj.value(QString("zdarzenia"));
    QJsonObject Zdarzenie = Zdarzenia.toObject();
    QJsonValue listaZd = Zdarzenie.value(QString("zdarzenie"));
    QJsonArray listaZdarzen = listaZd.toArray();
    //  Jeżeli tylko jeden status
    QJsonObject listaZdarzen2 = listaZd.toObject();

    int id = 0;

    if(listaZd.isArray()){
    foreach(QJsonValue val,listaZdarzen)
    {
        QJsonObject tempzd = val.toObject();
        class status temp;
        temp.data = QDateTime::fromString(tempzd["czas"].toString(),"yyyy-MM-dd hh:mm");
        temp.zdarzenie = tempzd["nazwa"].toString();
        temp.id = id++;

        //  NAZWA MIEJSCA
        QJsonValue miejsce = tempzd.value(QString("jednostka"));
        QJsonObject miejsceObj = miejsce.toObject();
        temp.miejsce = (miejsceObj["nazwa"].toString() == "") ? " - " : miejsceObj["nazwa"].toString();
        _tracking_.lista_zdarzen.append(temp);
         if(tempzd["konczace"].toBool()) break; //  OSTATNI STATUS
    }
    }else
    {
        QJsonObject tempzd = listaZdarzen2;
        class status temp;
        temp.data = QDateTime::fromString(tempzd["czas"].toString(),"yyyy-MM-dd hh:mm");
        temp.zdarzenie = tempzd["nazwa"].toString();
        temp.id = id++;

        //  NAZWA MIEJSCA
        QJsonValue miejsce = tempzd.value(QString("jednostka"));
        QJsonObject miejsceObj = miejsce.toObject();
        temp.miejsce = (miejsceObj["nazwa"].toString() == "") ? " - " : miejsceObj["nazwa"].toString();
        _tracking_.lista_zdarzen.append(temp);
    }

    _num_of_events_ = _tracking_.lista_zdarzen.size();

    if(_num_of_events_>0)
    _tracking_.aktualnystatus = _tracking_.lista_zdarzen[_num_of_events_-1].zdarzenie;
    if(status == 1)
     _tracking_.aktualnystatus = "Są inne przesyłki o podanym numerze";

return;

}
