#include "adak_sql.h"
#include "isl_struct.h"
#include "isl_enum.h"
#include "adak_utils.h"
 

extern ADAK_SQL * DB;
static QString     g_son_fis_tarihi;

/**************************************************************************************
                   ISL_SIRADAKI_FIS_NO_AL
***************************************************************************************/

int ISL_SIRADAKI_FIS_NO_AL ( QString p_fis_tarihi , ADAK_SQL * P_GELEN_DB)
{
    ADAK_SQL * USE_DB;

    if ( P_GELEN_DB EQ NULL ) {
        USE_DB = DB;
    }
    else {
        USE_DB = P_GELEN_DB;
    }

    SQL_QUERY sql_query ( USE_DB );

    sql_query.PREPARE_SELECT ( "isl_fisi","fis_no ",
                                     "fis_tarihi = :fis_tarihi");

    sql_query.SET_VALUE            ( ":fis_tarihi", p_fis_tarihi );

    if ( sql_query.SELECT("fis_no DESC",0,1) EQ 0 ) {
        return 1;
    }

    sql_query.NEXT();
    return sql_query.VALUE (0).toInt() + 1;
}

/**************************************************************************************
                 ISL_GET_FIS_TURU_STRING
***************************************************************************************/

QString  ISL_GET_FIS_TURU_STRING ( int fis_turu )
{
    switch ( fis_turu ) {
        case ENUM_GELIR_FISI :
            return "Gelir Fişi";
        case ENUM_GIDER_FISI :
            return "Gider Fişi";
        default :
            return NULL;
    }
}

/**************************************************************************************
                   ISL_GET_FIS_TURU_ENUM
***************************************************************************************/

int ISL_GET_FIS_TURU_ENUM(QString fis_turu)
{
    if ( fis_turu EQ QObject::tr("Gelir Fişi")) {
        return ENUM_GELIR_FISI;
    }
    else if ( fis_turu EQ QObject::tr("Gider Fişi")) {
        return ENUM_GIDER_FISI;
    }

    return -1;
}












