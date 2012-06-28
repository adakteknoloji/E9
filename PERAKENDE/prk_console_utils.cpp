#include "adak_sql.h"
 

extern ADAK_SQL * DB;


/*******************************************************************
                 PRK_SIRADAKI_FIS_NO_AL
********************************************************************/

int PRK_SIRADAKI_FIS_NO_AL(QString fis_tarihi)
{
    SQL_QUERY query(DB);

    query.PREPARE_SELECT("prk_fisler" , "fis_no" , "fis_tarihi = :fis_tarihi");

    query.SET_VALUE(":fis_tarihi" , fis_tarihi);

    if ( query.SELECT("fis_no DESC" , 0 , 1) EQ 0 ) {
        return 1;
    }
    query.NEXT();

    return query.VALUE(0).toInt() + 1;
}

/*******************************************************************
                PRK_GET_FIS_KDV_HARIC_TOPLAM_TUTAR
********************************************************************/

QList <double> PRK_GET_FIS_KDV_HARIC_TOPLAM_TUTAR(QString bas_tarihi, QString bts_tarihi, double max_tutar, int fat_alis_satis_turu)
{
    QList < double > kdv_haric_tutarlar;

    SQL_QUERY query( DB );

    QString where_str = " fis_tarihi BETWEEN :bas_tarihi AND :bts_tarihi ";

    if ( max_tutar NE 0.00 ) {
        where_str += QString (" AND kdv_haric_toplam > :kdv_haric_toplam ");
    }
    int gider_fisi_mi = -1;

    if ( fat_alis_satis_turu NE -1 ) {
        if ( fat_alis_satis_turu EQ 1 ) {
            gider_fisi_mi = 0;
        }
        else {
            gider_fisi_mi = 1;
        }
    }
    if ( gider_fisi_mi NE -1 ) {
        where_str += " AND gider_fisi_mi = :gider_fisi_mi ";
    }

    query.PREPARE_SELECT("prk_fisler" , "kdv_haric_toplam" , where_str);

    query.SET_VALUE(":bas_tarihi" , bas_tarihi );
    query.SET_VALUE(":bts_tarihi" , bts_tarihi );

    if ( max_tutar NE 0.0 ) {
        query.SET_VALUE(":kdv_haric_toplam" , max_tutar );
    }
    if ( gider_fisi_mi NE -1 ) {
        query.SET_VALUE(":gider_fisi_mi" , gider_fisi_mi);
    }

    if ( query.SELECT() NE 0 ) {
        while ( query.NEXT() EQ true ) {
            kdv_haric_tutarlar << query.VALUE( 0 ).toDouble();
        }
    }
    return kdv_haric_tutarlar;
}

