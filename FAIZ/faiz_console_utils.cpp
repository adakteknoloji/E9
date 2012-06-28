#include "faiz_console_utils.h"
#include <QMessageBox>
#include <QHeaderView>
#include <QDate>
#include "stdio.h"
#include "adak_sql.h"
#include "yonetim.h"
#include "faiz_console_utils.h"
#include "faiz_hesapla_batch_open.h"
#include "faiz_enum.h"



/*****************************************************************
                    FAIZ_GET_HESAPLAMA_TURU_LIST
******************************************************************/

QStringList  FAIZ_GET_HESAPLAMA_TURU_LIST()
{

        QStringList faiz_odeme_sekli;

        faiz_odeme_sekli <<  "Ayda Bir" ;
        faiz_odeme_sekli <<  "Günde Bir" ;

        return faiz_odeme_sekli;
}

/*****************************************************************
                 FAIZ_GET_HESAPLAMA_TURU_ENUM
******************************************************************/

int     FAIZ_GET_HESAPLAMA_TURU_ENUM ( QString faiz_sekli )
{

    if ( faiz_sekli EQ "Günde Bir" )
    {
        return GUNLUK_FAIZ;
    }
    else if ( faiz_sekli EQ "Ayda Bir")
    {
        return AYLIK_FAIZ;
    }
    else
    {
        return ADAK_FAIL;
    }

}

/*******************************************************************
                    FAZI_GET_HESAPLAMA_TURU_STRING
******************************************************************/

QString    FAZI_GET_HESAPLAMA_TURU_STRING ( int faiz_sekli )
{

    if ( faiz_sekli EQ GUNLUK_FAIZ )
    {
        return "Günde Bir";
    }
    else if ( faiz_sekli EQ AYLIK_FAIZ )
    {
        return "Ayda Bir";
    }
    else
    {
        return "null";
    }
}

/*****************************************************************
                     FAIZ_ADD_MONTH
******************************************************************/

QDate   FAIZ_ADD_MONTH( QDate p_date , int month_size )
{
    QDate tarih = p_date;

    for( int i = 0; i < month_size; i++ )
    {
        // eger ayin sonu ise diger ay sonuna otomatik tamamla.
        if( tarih.daysInMonth() EQ  tarih.day() )
        {
            tarih = tarih.addMonths( 1 );
            if( tarih.daysInMonth() > tarih.day() )
            {
                tarih = tarih.addDays( tarih.daysInMonth() - tarih.day() );
            }
        }
        else
        {
            tarih = tarih.addMonths( 1 );
        }

    }

    return tarih;
}
