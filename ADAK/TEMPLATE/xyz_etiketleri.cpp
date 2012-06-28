#include "adak_utils.h"
#include "xyz_etiketleri_open.h"
#include "xyz_etiketleri_class.h"

extern ADAK_SQL * XYZ_DB;

/**************************************************************************************
                   GET_XYZ_ETIKETLERI
***************************************************************************************/

QString GET_XYZ_ETIKETLERI ( int grup_id )
{
    XYZ_ETIKETLERI  *  F = new XYZ_ETIKETLERI ();

    return F->ETIKET_TABLOLARINI_OLUSTUR ( grup_id );
}

/**************************************************************************************
                   XYZ_ETIKETLERI::XYZ_ETIKETLERI
***************************************************************************************/

XYZ_ETIKETLERI::XYZ_ETIKETLERI ()
{   // Etiketin boyutlari mm olarak gonderilecek
    SET_ETIKET_SIZE ( 60, 15 );

    // Etiketin sol ve altindan birakilacak bosluklar
    SET_ETIKET_MARGIN ( 5, 10 );

    // Etiketin sayfanin ust ve sol tarafindan birakilacak bosluk.Bunun marginden farki burda sadece 1 kere ust ve sagdan bosluk birakilirken
    // margin de her etiketin arasinda bosluk birakilir.
    SET_PAGE_SPACE ( 5, 5);

    // 1 satira konacak etiket sayisi
    SET_SATIRDAKI_ETIKET_SAYISI ( 2 );
}

// Burada etiketlerimizin her birini table olarak olusturup her bir table ida qstringlist te ekleriz ve sonrasinda ise
// bu string list i ETIKETLERI_OLUSTUR fonksiyonuna parametre olarak gondeririz donen degeri direk print preview de gosterebiliriz.
// Cunku ETIKETLERI_OLUSTUR bize yazdirilmaya hazir olan sayfalari dondururur.Yukardaki degerlere gore sayfa yapisi olusturulur.

/**************************************************************************************
                   XYZ_ETIKETLERI::ETIKETLERI_OLUSTUR
***************************************************************************************/

QString XYZ_ETIKETLERI::ETIKET_TABLOLARINI_OLUSTUR ( int grup_id )
{
    SQL_QUERY query (DB);
    SQL_QUERY sql_query (DB);

    QStringList etiketler;

    query.PREPARE_SELECT ("adr_firma_bilgileri","firma_id");

    // duzeltilecek
    if (query.SELECT() NE 0 ){
        while(query.NEXT()) {
            int adres_gruplari[GRUP_SAYISI];


            SQL_QUERY query( DB );
            query.PREPARE_SELECT( "adr_firma_bilgileri","adres_gruplari","adres_gruplari = :adres_gruplari " );

            query.SET_VALUE( ":adres_gruplari ", query.VALUE(0).toInt()  );

            if ( query.SELECT() NE 0 ) {
                query.NEXT();

                UNPACK_DOUBLE_ARRAY( query.VALUE( 0 ).toString(), adres_gruplari,GRUP_SAYISI) );
            }

            for (int i=0; i < GRUP_SAYISI ; i++) {
                if(adres_gruplari[i] EQ grup_id){
                    sql_query.PREPARE_SELECT("adr_firma_bilgileri","firma_adi,mahalle,cadde,sokak,bina_no,daire_no,kat,ilce,semt,ulke,sehir,tel_ulke_kodu,tel_alan_kodu,telefon,email","firma_id=:firma_id");
                    sql_query.SET_VALUE(":firma_id",query.VALUE(0).toInt());
                    if (sql_query.SELECT() EQ 0) {
                        continue;
                    }
                    sql_query.NEXT();
                    SET_TABLE_TAG_STRING("<table border=0 >");
                    CREATE_TABLE(QStringList() << sql_query.VALUE(0).toString() + " " + " " ,0,0);
                    ADD_ROW_TO_TABLE(QStringList() << sql_query.VALUE(1).toString()+" Mah." + sql_query.VALUE(2).toString()+" Cad." + sql_query.VALUE(3).toString()+" Sok." +"No:"+sql_query.VALUE(4).toString());
                    ADD_ROW_TO_TABLE(QStringList() << sql_query.VALUE(7).toString()+" - " + sql_query.VALUE(8).toString() + "");
                    etiketler << GET_TABLE_HTML_STRING();
                }
            }
        }
    }

    query.PREPARE_SELECT ("adr_sahis_bilgileri","sahis_id");

    // duzeltilecek
    if (query.SELECT() NE 0 ){
        while(query.NEXT()) {
            int adres_gruplari[GRUP_SAYISI];

            SQL_QUERY query( DB );
            query.PREPARE_SELECT( "adr_firma_bilgileri","adres_gruplari","adres_gruplari = :adres_gruplari " );

            query.SET_VALUE( ":adres_gruplari ", query.VALUE(0).toInt()  );

            if ( query.SELECT() NE 0 ) {
                query.NEXT();

                UNPACK_DOUBLE_ARRAY( query.VALUE( 0 ).toString(), adres_gruplari,GRUP_SAYISI) );
            }

            for (int i=0; i < GRUP_SAYISI ; i++) {
                if(adres_gruplari[i] EQ grup_id){
                    sql_query.PREPARE_SELECT("adr_sahis_bilgileri","sahis_adi,mahalle,cadde,sokak,bina_no,daire_no,kat,ilce,semt,ulke,sehir,ev_tel_ulke_kodu,ev_tel_alan_kodu,ev_tel,email","sahis_id=:sahis_id");
                    sql_query.SET_VALUE(":firma_id",query.VALUE(0).toInt());
                    if (sql_query.SELECT() EQ 0) {
                        continue;
                    }
                    sql_query.NEXT();
                    SET_TABLE_TAG_STRING("<table border=0 >");
                    CREATE_TABLE(QStringList() << sql_query.VALUE(0).toString() + " " + " " ,0,0);
                    ADD_ROW_TO_TABLE(QStringList() << sql_query.VALUE(1).toString()+" Mah." + sql_query.VALUE(2).toString()+" Cad." + sql_query.VALUE(3).toString()+" Sok." +"No:"+sql_query.VALUE(4).toString());
                    ADD_ROW_TO_TABLE(QStringList() << sql_query.VALUE(7).toString()+" - " + sql_query.VALUE(8).toString() + "");
                    etiketler << GET_TABLE_HTML_STRING();
                }
            }
        }
    }

    return ETIKETLERI_OLUSTUR(etiketler);
}
