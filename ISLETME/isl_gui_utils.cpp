#include <QComboBox>
#include <QString>
#include "isl_enum.h"
#include "adak_defines.h"
#include "isl_gui_utils.h"


/**************************************************************************************
                   FILL_ISL_GIDERLERI
***************************************************************************************/

void FILL_ISL_GIDERLERI( QComboBox *p_combobox )
{
    QStringList list;

    list << QObject::tr("GENEL GİDERLER");
    list << QObject::tr("MAL ALIMI");
    list << QObject::tr("DÖNEM BAŞI STOĞU");
    list << QObject::tr("İŞYERİ KİRASİ");
    list << QObject::tr("İŞYERİ SİGORTA GİDERİ");
    list << QObject::tr("İŞYERİ PERSONEL GİDERİ");
    list << QObject::tr("İŞYERİ FAIZ GİDERİ");
    list << QObject::tr("YIL SONU AMORTİSMANLAR");

    p_combobox->clear();
    p_combobox->addItems(list);

}

/**************************************************************************************
                   FILL_ISL_GELIRLERI
***************************************************************************************/

void FILL_ISL_GELIRLERI( QComboBox *p_combobox )
{
    QStringList list;
    list << QObject::tr("MAL SATIŞI");
    list << QObject::tr("HİZMET SATIŞI");
    list << QObject::tr("AMORTİSMAN SATIŞI");
    list << QObject::tr("FAİZ GELİRİ");
    list << QObject::tr("YIL SONU STOGU");

    p_combobox->clear();
    p_combobox->addItems(list);
}

/**************************************************************************************
                   ISL_GET_GIDER_ENUM
***************************************************************************************/

int ISL_GET_GIDER_ENUM( QString p_str )
{

    if ( p_str EQ "GENEL GİDERLER" ) {
        return ENUM_GENEL_GIDERLER;
    }
    else if ( p_str EQ  "MAL ALIMI" ) {
        return ENUM_MAL_ALIMI;
    }
    else if ( p_str EQ  "DÖNEM BAŞI STOĞU" ) {
        return ENUM_DONEM_BASI_STOGU;
    }
    else if ( p_str EQ  "İŞYERİ KİRASİ" ) {
        return ENUM_ISYERI_KIRASI;
    }
    else if ( p_str EQ  "İŞYERİ SİGORTA GİDERİ" ) {
        return ENUM_ISYERI_SIGORTA_GIDERI;
    }
    else if ( p_str EQ  "İŞYERİ PERSONEL GİDERİ" ) {
        return ENUM_ISYERI_PERSONEL_GIDERI;
    }
    else if ( p_str EQ  "İŞYERİ FAIZ GİDERİ" ) {
        return ENUM_ISYERI_FAIZ_GIDERI;
    }
    else if ( p_str EQ  "YIL SONU AMORTİSMANLAR" ) {
        return ENUM_YILSONU_AMORTISMANLAR;
    }

    return -1;
}

/**************************************************************************************
                   ISL_GET_GELIR_ENUM
***************************************************************************************/

int ISL_GET_GELIR_ENUM( QString p_str )
{
    if ( p_str EQ  "MAL SATIŞI" ) {
        return ENUM_MAL_SATISI;
    }
    else if ( p_str EQ  "HİZMET SATIŞI" ) {
        return ENUM_HIZMET_SATISI;
    }
    else if ( p_str EQ  "AMORTİSMAN SATIŞI" ) {
        return ENUM_AMORTISMAN_SATISI;
    }
    else if ( p_str EQ  "FAİZ GELİRİ" ) {
        return ENUM_FAIZ_GELIRI;
    }
    else if ( p_str EQ  "YIL SONU STOGU" ) {
        return ENUM_YILSONU_STOGU;
    }

    return -1;
}

/**************************************************************************************
                   ISL_GET_GIDER_STRING
***************************************************************************************/

QString ISL_GET_GIDER_STRING( int p_enum )
{
    switch(p_enum) {
        case ENUM_GENEL_GIDERLER                  :
            return "GENEL GİDERLER";
        case ENUM_MAL_ALIMI              :
            return "MAL ALIMI";
        case ENUM_DONEM_BASI_STOGU    :
            return "DÖNEM BAŞI STOĞU";
        case ENUM_ISYERI_KIRASI          :
            return "İŞYERİ KİRASİ";
        case ENUM_ISYERI_SIGORTA_GIDERI  :
            return "İŞYERİ SİGORTA GİDERİ";
        case ENUM_ISYERI_PERSONEL_GIDERI :
            return "İŞYERİ PERSONEL GİDERİ";
        case ENUM_ISYERI_FAIZ_GIDERI     :
            return "İŞYERİ FAIZ GİDERİ";
        case ENUM_YILSONU_AMORTISMANLAR  :
            return "YIL SONU AMORTİSMANLAR";
        default:
            break;
    }

    return "NULL";
}

/**************************************************************************************
                   ISL_GET_GELIR_STRING
***************************************************************************************/

QString ISL_GET_GELIR_STRING( int p_enum )
{

    switch(p_enum) {
        case ENUM_MAL_SATISI        :
            return "MAL SATIŞI";
        case ENUM_HIZMET_SATISI     :
            return "HİZMET SATIŞI";
        case ENUM_AMORTISMAN_SATISI :
            return "AMORTİSMAN SATIŞI";
        case ENUM_FAIZ_GELIRI       :
            return "FAİZ GELİRİ";
        case ENUM_YILSONU_STOGU     :
            return "YIL SONU STOGU";
        default:
            break;
    }

    return "NULL";
}
