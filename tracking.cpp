#include "tracking.h"


tracking::tracking()
{
   nazwa = " - ";
    id = 0;
    czydostarczono = 0;
    numer = " - ";
    data_nadania = QDate::fromString("19990909", "yyyyMMdd");
    rodzaj_przesylki =" - ";
    data_dodania = QDate::fromString("19990909", "yyyyMMdd");
    kurier ="Nie ustalono";
    krajn =" - ";
    krajd=" - ";
    miastod=" - ";
    miaston=" - ";
    lista_zdarzen.empty();


}

tracking & tracking::operator=(const tracking & trc)
{
    //qDebug("operator = tracking ");
    this->czydostarczono = trc.czydostarczono;
    this->data_dodania = trc.data_dodania;
    this->data_nadania = trc.data_nadania;
    this->id = trc.id;
    this->krajd = trc.krajd;
    this->krajn = trc.krajn;
    this->kurier = trc.kurier;
    this->lista_zdarzen = trc.lista_zdarzen;
    this->miastod = trc.miastod;
    this->miaston = trc.miaston;
    this->nazwa = trc.nazwa;
    this->numer = trc.numer;
    this->rodzaj_przesylki = trc.rodzaj_przesylki;
    this->aktualnystatus = trc.aktualnystatus;
    this->extra = trc.extra;
    this->data_akt = trc.data_akt;

    return *this;
}

tracking::tracking(const tracking &tracking)
{
    //qDebug("copy tracking konsturktor");
    this->czydostarczono = tracking.czydostarczono;
    this->data_dodania = tracking.data_dodania;
    this->data_nadania = tracking.data_nadania;
    this->id = tracking.id;
    this->krajd = tracking.krajd;
    this->krajn = tracking.krajn;
    this->kurier = tracking.kurier;
    this->lista_zdarzen = tracking.lista_zdarzen;
    this->miastod = tracking.miastod;
    this->miaston = tracking.miaston;
    this->nazwa = tracking.nazwa;
    this->numer = tracking.numer;
    this->rodzaj_przesylki = tracking.rodzaj_przesylki;
    this->aktualnystatus = tracking.aktualnystatus;
    this->extra = tracking.extra;
    this->data_akt = tracking.data_akt;

}
