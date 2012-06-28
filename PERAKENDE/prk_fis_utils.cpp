#include "prk_fis_utils.h"
#include "adak_sql.h"
#include "prk_console_utils.h"
#include "muh_fis_utils.h"
#include "bnk_fis_utils.h"
#include "muh_enum.h"
#include "banka_enum.h"
#include "dvz_enum.h"
#include "dvz_console_utils.h"
#include "e9_enum.h"
 
#include <QList>

extern ADAK_SQL * DB;



/**************************************************************************************
                                    PRK_FIS_EKLE
***************************************************************************************/

int PRK_FIS_EKLE(PRK_FIS_STRUCT * P_FIS)
{
    SQL_QUERY query(DB);


    if ( P_FIS->fis_no EQ 0 ) {
       P_FIS->fis_no = PRK_SIRADAKI_FIS_NO_AL ( P_FIS->fis_tarihi );
    }

    query.PREPARE_INSERT("prk_fisler" , "fis_id" , "fis_no , fis_tarihi , "
                         "aciklama , sube_id , modul_id , program_id , base_fis_id ,"
                         "gider_fisi_mi,muh_kasa_hesap_id");


    query.SET_VALUE(":fis_no"           , P_FIS->fis_no       );
    query.SET_VALUE(":fis_tarihi"       , P_FIS->fis_tarihi   );
    query.SET_VALUE(":aciklama"         , P_FIS->aciklama     );
    query.SET_VALUE(":sube_id"          , P_FIS->sube_id      );
    query.SET_VALUE(":modul_id"         , P_FIS->modul_id     );
    query.SET_VALUE(":program_id"       , P_FIS->program_id   );
    query.SET_VALUE(":base_fis_id"      , P_FIS->base_fis_id);
    query.SET_VALUE(":gider_fisi_mi"    , P_FIS->gider_fisi_mi );
    query.SET_VALUE(":muh_kasa_hesap_id", P_FIS->muh_kasa_hesap_id);

    int fis_id = query.INSERT();

    if ( P_FIS->muh_fis_id EQ 0 AND P_FIS->modul_id EQ PERAKENDE_MODULU ) {

        QString aciklama = QString("Fiş PERAKENDE modülü tarafından üretilmiştir.Fiş No : %1 \n").arg(P_FIS->fis_no);
        aciklama.append(P_FIS->aciklama);

        MUH_FIS_STRUCT * MUHASEBE_FISI = new MUH_FIS_STRUCT;

        MUH_CLEAR_FIS_STRUCT ( MUHASEBE_FISI );

        MUHASEBE_FISI->aciklama         = aciklama;
        MUHASEBE_FISI->base_fis_id      = fis_id;
        MUHASEBE_FISI->fis_tarihi       = P_FIS->fis_tarihi;
        MUHASEBE_FISI->fis_turu         = ENUM_MAHSUP_FISI;
        MUHASEBE_FISI->modul_id         = P_FIS->modul_id;
        MUHASEBE_FISI->program_id       = P_FIS->program_id;

        P_FIS->muh_fis_id = MUH_FIS_EKLE(MUHASEBE_FISI);

        query.PREPARE_UPDATE("prk_fisler","fis_id", "muh_fis_id" ,"fis_id = :fis_id");
        query.SET_VALUE(":muh_fis_id" , P_FIS->muh_fis_id);
        query.SET_VALUE(":fis_id"     , fis_id);
        query.UPDATE();

        delete MUHASEBE_FISI;
    }

    return fis_id;

}
/**************************************************************************************
                                    PRK_FIS_SATIRI_EKLE
***************************************************************************************/

int PRK_FIS_SATIRI_EKLE(PRK_FIS_STRUCT *P_FIS, PRK_FIS_SATIRI_STRUCT *P_FIS_SATIRI ,
                        QList<int>ayni_belge_numarali_satir_id_list,int fis_satiri_id ,
                        bool p_base_fis_satiri_eklenecek , ADAK_SQL * P_GELEN_DB)
{

    ADAK_SQL * USE_DB;

    if ( P_GELEN_DB EQ NULL ) {
        USE_DB = DB;
    }
    else {
        USE_DB = P_GELEN_DB;
    }

    SQL_QUERY query(USE_DB);

    query.PREPARE_SELECT("prk_fisler" , "kdv_haric_toplam , kdv_toplam , kdv_dahil_toplam,"
                         "modul_id , program_id,base_fis_id, "
                         "last_order_number" , "fis_id = :fis_id");

    query.SET_VALUE(":fis_id" , P_FIS_SATIRI->fis_id);

    if ( query.SELECT() EQ 0 ) {
        return 0 ;
    }

    query.NEXT();

    double kdv_haric_toplam = query.VALUE(0).toDouble();
    double kdv_toplam       = query.VALUE(1).toDouble();
    double kdv_dahil_toplam = query.VALUE(2).toDouble();
    int   modul_id          = query.VALUE(3).toInt();
    int   program_id        = query.VALUE(4).toInt();
    int   base_fis_id       = query.VALUE(5).toInt();
    int   last_order_number = query.VALUE(6).toInt();

    if ( P_FIS->modul_id NE modul_id OR P_FIS->program_id NE program_id OR
         P_FIS->base_fis_id NE base_fis_id ) {
        return 0;
    }

    if ( P_FIS_SATIRI->order_number EQ 0 ) {
        P_FIS_SATIRI->order_number = last_order_number + 1024;
    }


    if ( p_base_fis_satiri_eklenecek EQ true ) {

        double kdv_tutari = 0.0;

        if ( P_FIS_SATIRI->kdv_tutari.isEmpty() EQ false ) {
            kdv_tutari = P_FIS_SATIRI->kdv_tutari.at(0);
        }

        int kdv_hesap_id = 0;

        if ( P_FIS_SATIRI->kdv_hesap_id.isEmpty() EQ false ) {
            kdv_hesap_id = P_FIS_SATIRI->kdv_hesap_id.at(0);
        }

        query.PREPARE_INSERT("prk_fis_satirlari" , "fis_satiri_id" , "fis_id , order_number , belge_no ,"
                             "aciklama , kdv_orani , kdv_haric_tutar , kdv_tutari , kdv_dahil_tutar,gider_turu_id,"
                             "modul_id , program_id , base_record_id , kdv_hesap_id ,"
                             "tah_odm_sekli,hesap_no_id,kredi_karti_id,pos_id,kredi_karti_sahibi,"
                             "kredi_karti_numarasi");

        query.SET_VALUE(":fis_id"               , P_FIS_SATIRI->fis_id );
        query.SET_VALUE(":order_number"         , P_FIS_SATIRI->order_number);
        query.SET_VALUE(":belge_no"             , P_FIS_SATIRI->belge_no);
        query.SET_VALUE(":aciklama"             , P_FIS_SATIRI->aciklama);
        query.SET_VALUE(":kdv_orani"            , P_FIS_SATIRI->kdv_orani );
        query.SET_VALUE(":kdv_haric_tutar"      , P_FIS_SATIRI->kdv_haric_tutar);
        query.SET_VALUE(":kdv_tutari"           , kdv_tutari );
        query.SET_VALUE(":kdv_dahil_tutar"      , P_FIS_SATIRI->kdv_dahil_tutar );
        query.SET_VALUE(":gider_turu_id"        , P_FIS_SATIRI->gelir_gider_turu_id );
        query.SET_VALUE(":modul_id"             , P_FIS_SATIRI->modul_id  );
        query.SET_VALUE(":program_id"           , P_FIS_SATIRI->program_id   );
        query.SET_VALUE(":base_record_id"       , 0);
        query.SET_VALUE(":kdv_hesap_id"         , kdv_hesap_id );
        query.SET_VALUE(":tah_odm_sekli"        , P_FIS_SATIRI->tah_odm_sekli);
        query.SET_VALUE(":hesap_no_id"          , P_FIS_SATIRI->hesap_no_id);
        query.SET_VALUE(":kredi_karti_id"       , P_FIS_SATIRI->kredi_karti_id);
        query.SET_VALUE(":pos_id"               , P_FIS_SATIRI->pos_id);
        query.SET_VALUE(":kredi_karti_sahibi"   , P_FIS_SATIRI->kredi_karti_sahibi);
        query.SET_VALUE(":kredi_karti_numarasi" , P_FIS_SATIRI->kredi_karti_numarasi);

        fis_satiri_id = query.INSERT();

        kdv_haric_toplam += P_FIS_SATIRI->kdv_haric_tutar;
        kdv_toplam       += kdv_tutari;
        kdv_dahil_toplam += P_FIS_SATIRI->kdv_dahil_tutar;

        P_FIS->kdv_haric_toplam = kdv_haric_toplam;
        P_FIS->kdv_toplam       = kdv_toplam;
        P_FIS->kdv_dahil_toplam = kdv_dahil_toplam;

        query.PREPARE_UPDATE("prk_fisler","fis_id", "kdv_haric_toplam, kdv_toplam,"
                             "kdv_dahil_toplam ,last_order_number ",
                             "fis_id = :fis_id");

        query.SET_VALUE(":kdv_haric_toplam" , P_FIS->kdv_haric_toplam);
        query.SET_VALUE(":kdv_toplam"       , P_FIS->kdv_toplam);
        query.SET_VALUE(":kdv_dahil_toplam" , P_FIS->kdv_dahil_toplam);
        query.SET_VALUE(":last_order_number", P_FIS_SATIRI->order_number);
        query.SET_VALUE(":fis_id"           , P_FIS_SATIRI->fis_id);

        query.UPDATE();

        QList<int> fis_satiri_id_list = PRK_GET_AYNI_BELGE_NUMARALI_SATIR_LIST_AND_ENTEGRE_ET(fis_satiri_id,USE_DB);

        if ( fis_satiri_id_list.isEmpty() EQ true ) {
            PRK_ENT_FISLERI_OLUSTUR(P_FIS , P_FIS_SATIRI , fis_satiri_id ,ayni_belge_numarali_satir_id_list,USE_DB);
        }
    }
    else {
        PRK_ENT_FISLERI_OLUSTUR(P_FIS , P_FIS_SATIRI , fis_satiri_id ,ayni_belge_numarali_satir_id_list,USE_DB);
    }

    return fis_satiri_id;
}

/**************************************************************************************
                                    PRK_FIS_SATIRINI_SIL
***************************************************************************************/

int PRK_FIS_SATIRINI_SIL(PRK_FIS_STRUCT *P_FIS, int p_fis_satiri_id)
{
    SQL_QUERY query(DB);

    query.PREPARE_SELECT("prk_fisler , prk_fis_satirlari" , "kdv_haric_toplam , kdv_toplam , kdv_dahil_toplam,"
                         "kdv_haric_tutar,kdv_tutari,kdv_dahil_tutar,"
                         "tah_odm_sekli,prk_fis_satirlari.modul_id,"
                         "prk_fis_satirlari.program_id , prk_fisler.base_fis_id,"
                         "prk_fisler.fis_id,hesap_no_id,"
                         "muh_gelir_gider_satiri_id,muh_kdv_satiri_id,"
                         "muh_kasa_satiri_id,gider_fisi_mi", "fis_satiri_id = :fis_satiri_id AND "
                         "prk_fisler.fis_id = prk_fis_satirlari.fis_id");


    query.SET_VALUE(":fis_satiri_id" , p_fis_satiri_id);

    if ( query.SELECT() EQ 0 ) {
        return 0;
    }

    query.NEXT();

    double kdv_haric_toplam = query.VALUE(0).toDouble();
    double kdv_toplam       = query.VALUE(1).toDouble();
    double kdv_dahil_toplam = query.VALUE(2).toDouble();
    double kdv_haric_tutar  = query.VALUE(3).toDouble();
    double kdv_tutari       = query.VALUE(4).toDouble();
    double kdv_dahil_tutar  = query.VALUE(5).toDouble();
    int    tah_odm_sekli    = query.VALUE(6).toInt();
    int    modul_id         = query.VALUE(7).toInt();
    int    program_id       = query.VALUE(8).toInt();
    int    base_fis_id      = query.VALUE(9).toInt();
    int    fis_id           = query.VALUE(10).toInt();
    int    hesap_no_id      = query.VALUE(11).toInt();

    int muh_gelir_gider_satiri_id = query.VALUE(12).toInt();
    int muh_kdv_satiri_id         = query.VALUE(13).toInt();
    int muh_kasa_satiri_id        = query.VALUE(14).toInt();
    int gider_fisi_mi             = query.VALUE(15).toInt();

    if ( P_FIS->program_id NE program_id OR P_FIS->modul_id NE modul_id OR
         P_FIS->base_fis_id NE base_fis_id ) {
        return 0;
    }

    kdv_haric_toplam  -= kdv_haric_tutar;
    kdv_dahil_toplam  -= kdv_dahil_tutar;
    kdv_toplam        -= kdv_tutari;

    P_FIS->kdv_haric_toplam = kdv_haric_toplam;
    P_FIS->kdv_dahil_toplam = kdv_dahil_toplam;
    P_FIS->kdv_toplam       = kdv_toplam;

    //MUHASEBE ENTEGRASYONU

    MUH_FIS_STRUCT * MUHASEBE_FISI = new MUH_FIS_STRUCT;

    MUHASEBE_FISI->program_id       = P_FIS->program_id;
    MUHASEBE_FISI->modul_id         = P_FIS->modul_id;
    MUHASEBE_FISI->base_fis_id      = fis_id;

    MUH_FIS_SATIRI_STRUCT * MUH_FIS_SATIRI = new MUH_FIS_SATIRI_STRUCT;


    if ( muh_gelir_gider_satiri_id > 0 ) {
        MUH_CLEAR_FIS_SATIRI_STRUCT(MUH_FIS_SATIRI);

        MUH_FIS_SATIRINI_OKU(MUH_FIS_SATIRI , muh_gelir_gider_satiri_id);

        if ( gider_fisi_mi EQ 1 ) {
            MUH_FIS_SATIRI->borc_tutari  -= kdv_haric_tutar;
        }
        else {
            MUH_FIS_SATIRI->alacak_tutari -= kdv_haric_tutar;
        }

        if ( MUH_FIS_SATIRI->borc_tutari EQ 0 AND MUH_FIS_SATIRI->alacak_tutari EQ 0 ) {
            MUH_FIS_SATIRINI_SIL(MUHASEBE_FISI , muh_gelir_gider_satiri_id);
        }
        else {
            MUH_FIS_SATIRINI_GUNCELLE(MUHASEBE_FISI , MUH_FIS_SATIRI , muh_gelir_gider_satiri_id);
        }
    }

    if ( muh_kasa_satiri_id > 0 ) {
        MUH_CLEAR_FIS_SATIRI_STRUCT(MUH_FIS_SATIRI);

        MUH_FIS_SATIRINI_OKU(MUH_FIS_SATIRI , muh_kasa_satiri_id);

        if ( gider_fisi_mi EQ 1 ) {
            MUH_FIS_SATIRI->alacak_tutari  -= kdv_dahil_tutar;
        }
        else {
            MUH_FIS_SATIRI->borc_tutari    -= kdv_dahil_tutar;
        }

        if ( MUH_FIS_SATIRI->borc_tutari EQ 0 AND MUH_FIS_SATIRI->alacak_tutari EQ 0 ) {
            MUH_FIS_SATIRINI_SIL(MUHASEBE_FISI , muh_kasa_satiri_id);
        }
        else {
            MUH_FIS_SATIRINI_GUNCELLE(MUHASEBE_FISI , MUH_FIS_SATIRI , muh_kasa_satiri_id);
        }
    }

    if ( muh_kdv_satiri_id > 0 ) {
        MUH_CLEAR_FIS_SATIRI_STRUCT(MUH_FIS_SATIRI);

        MUH_FIS_SATIRINI_OKU(MUH_FIS_SATIRI , muh_kdv_satiri_id);

        if ( gider_fisi_mi EQ 1 ) {
            MUH_FIS_SATIRI->borc_tutari  -= kdv_tutari;
        }
        else {
            MUH_FIS_SATIRI->alacak_tutari -= kdv_tutari;
        }

        if ( MUH_FIS_SATIRI->borc_tutari EQ 0 AND MUH_FIS_SATIRI->alacak_tutari EQ 0 ) {
            MUH_FIS_SATIRINI_SIL(MUHASEBE_FISI , muh_kdv_satiri_id);
        }
        else {
            MUH_FIS_SATIRINI_GUNCELLE(MUHASEBE_FISI , MUH_FIS_SATIRI , muh_kdv_satiri_id);
        }
    }

    //BANKA ENTEGRASYONU
    if ( tah_odm_sekli NE ENUM_NAKIT ) {

        BNK_HAREKET_FIS_STRUCT * BNK_FISI = new BNK_HAREKET_FIS_STRUCT;

        BNK_CLEAR_HAREKET_FIS_STRUCT(BNK_FISI);

        BNK_HAREKET_FIS_BILGILERINI_OKU(BNK_FISI , BNK_GET_ENT_FIS_ID(P_FIS->program_id,
                                                                      P_FIS->modul_id,fis_id,
                                                                      hesap_no_id));



        QStringList bnk_fis_satirlari_list = BNK_GET_ENT_ROW_ID_LIST(BNK_FISI->program_id,BNK_FISI->modul_id ,p_fis_satiri_id);

        for ( int i = 0 ; i < bnk_fis_satirlari_list.size() ; i++ ) {
            int bnk_fis_satiri_id = QVariant(bnk_fis_satirlari_list.at(i)).toInt();

            BNK_HAREKET_FIS_SATIRI_SIL(BNK_FISI , bnk_fis_satiri_id);
        }
        delete BNK_FISI;
    }

    query.PREPARE_DELETE("prk_fis_satirlari", "fis_satiri_id = :fis_satiri_id");
    query.SET_VALUE(":fis_satiri_id"        , p_fis_satiri_id);
    query.DELETE();

    query.PREPARE_UPDATE("prk_fisler","fis_id" ,"kdv_haric_toplam , kdv_toplam ,"
                         "kdv_dahil_toplam ","fis_id = :fis_id");

    query.SET_VALUE(":kdv_haric_toplam" , P_FIS->kdv_haric_toplam);
    query.SET_VALUE(":kdv_toplam"       , P_FIS->kdv_toplam);
    query.SET_VALUE(":kdv_dahil_toplam" , P_FIS->kdv_dahil_toplam);
    query.SET_VALUE(":fis_id"           , fis_id);

    query.UPDATE();

    return fis_id;
}

/**************************************************************************************
                                    PRK_FIS_SIL
***************************************************************************************/

void PRK_FIS_SIL(PRK_FIS_STRUCT *P_FIS, int p_fis_id)
{
    SQL_QUERY query(DB);

    query.PREPARE_SELECT("prk_fis_satirlari" , "fis_satiri_id" , "fis_id = :fis_id");
    query.SET_VALUE(":fis_id" , p_fis_id);

    if ( query.SELECT() NE 0 ) {

        while ( query.NEXT() ) {

            int fis_satiri_id   = query.VALUE(0).toInt();

            PRK_FIS_SATIRINI_SIL(P_FIS , fis_satiri_id);
        }
    }


    if ( P_FIS->muh_fis_id NE 0 ) {

        MUH_FIS_STRUCT * MUHASEBE_FISI = new MUH_FIS_STRUCT;

        MUH_CLEAR_FIS_STRUCT(MUHASEBE_FISI);

        MUH_FIS_BILGILERINI_OKU(MUHASEBE_FISI , P_FIS->muh_fis_id);

        MUH_FIS_SIL(MUHASEBE_FISI , P_FIS->muh_fis_id );

        delete MUHASEBE_FISI;
    }

    query.PREPARE_DELETE("prk_fisler" ,"fis_id = :fis_id");
    query.SET_VALUE(":fis_id" , p_fis_id );

    query.DELETE();
}

/**************************************************************************************
                                    PRK_FIS_SATIRINI_GUNCELLE
***************************************************************************************/

void PRK_FIS_SATIRINI_GUNCELLE(PRK_FIS_STRUCT *P_FIS, PRK_FIS_SATIRI_STRUCT *P_FIS_SATIRI, int p_fis_satiri_id)
{
    SQL_QUERY query(DB);

    query.PREPARE_SELECT("prk_fis_satirlari,prk_fisler" , "kdv_haric_tutar , kdv_tutari , kdv_dahil_tutar,  "
                         "tah_odm_sekli,muh_gelir_gider_satiri_id,muh_kdv_satiri_id,"
                         "muh_kasa_satiri_id,kdv_haric_toplam , kdv_toplam , "
                         "kdv_dahil_toplam,gider_fisi_mi",
                         "fis_satiri_id = :fis_satiri_id");

    query.SET_VALUE(":fis_satiri_id" , p_fis_satiri_id);

    if ( query.SELECT() EQ 0 ) {
        return;
    }

    query.NEXT();

    double old_kdv_haric_tutar       = query.VALUE(0).toDouble();
    double old_kdv_tutari            = query.VALUE(1).toDouble();
    double old_kdv_dahil_tutar       = query.VALUE(2).toDouble();
    int    tah_odm_sekli             = query.VALUE(3).toInt();
    int    muh_gelir_gider_satiri_id = query.VALUE(4).toInt();
    int    muh_kdv_satiri_id         = query.VALUE(5).toInt();
    int    muh_kasa_satiri_id        = query.VALUE(6).toInt();
    double kdv_haric_toplam          = query.VALUE(7).toDouble();
    double kdv_toplam                = query.VALUE(8).toDouble();
    double kdv_dahil_toplam          = query.VALUE(9).toDouble();
    int    gider_fisi_mi             = query.VALUE(10).toInt();

    kdv_haric_toplam  -= old_kdv_haric_tutar;
    kdv_dahil_toplam  -= old_kdv_dahil_tutar;
    kdv_toplam        -= old_kdv_tutari;

    MUH_FIS_STRUCT * MUHASEBE_FISI = new MUH_FIS_STRUCT;

    MUHASEBE_FISI->program_id       = P_FIS->program_id;
    MUHASEBE_FISI->modul_id         = P_FIS->modul_id;
    MUHASEBE_FISI->base_fis_id      = P_FIS_SATIRI->fis_id;

    MUH_FIS_SATIRI_STRUCT * MUH_FIS_SATIRI = new MUH_FIS_SATIRI_STRUCT;


    if ( muh_gelir_gider_satiri_id > 0 ) {
        MUH_CLEAR_FIS_SATIRI_STRUCT(MUH_FIS_SATIRI);

        MUH_FIS_SATIRINI_OKU(MUH_FIS_SATIRI , muh_gelir_gider_satiri_id);

        if ( gider_fisi_mi EQ 1 ) {
            MUH_FIS_SATIRI->borc_tutari  -= old_kdv_haric_tutar;
        }
        else {
            MUH_FIS_SATIRI->alacak_tutari -= old_kdv_haric_tutar;
        }

        if ( MUH_FIS_SATIRI->borc_tutari EQ 0 AND MUH_FIS_SATIRI->alacak_tutari EQ 0 ) {
            MUH_FIS_SATIRINI_SIL(MUHASEBE_FISI , muh_gelir_gider_satiri_id);
        }
        else {
            MUH_FIS_SATIRINI_GUNCELLE(MUHASEBE_FISI , MUH_FIS_SATIRI , muh_gelir_gider_satiri_id);
        }
    }

    if ( muh_kasa_satiri_id > 0 ) {
        MUH_CLEAR_FIS_SATIRI_STRUCT(MUH_FIS_SATIRI);

        MUH_FIS_SATIRINI_OKU(MUH_FIS_SATIRI , muh_kasa_satiri_id);

        if ( gider_fisi_mi EQ 1 ) {
            MUH_FIS_SATIRI->alacak_tutari  -= old_kdv_dahil_tutar;
        }
        else {
            MUH_FIS_SATIRI->borc_tutari    -= old_kdv_dahil_tutar;
        }

        if ( MUH_FIS_SATIRI->borc_tutari EQ 0 AND MUH_FIS_SATIRI->alacak_tutari EQ 0 ) {
            MUH_FIS_SATIRINI_SIL(MUHASEBE_FISI , muh_kasa_satiri_id);
        }
        else {
            MUH_FIS_SATIRINI_GUNCELLE(MUHASEBE_FISI , MUH_FIS_SATIRI , muh_kasa_satiri_id);
        }
    }

    if ( muh_kdv_satiri_id > 0 ) {
        MUH_CLEAR_FIS_SATIRI_STRUCT(MUH_FIS_SATIRI);

        MUH_FIS_SATIRINI_OKU(MUH_FIS_SATIRI , muh_kdv_satiri_id);

        if ( gider_fisi_mi EQ 1 ) {
            MUH_FIS_SATIRI->borc_tutari  -= old_kdv_tutari;
        }
        else {
            MUH_FIS_SATIRI->alacak_tutari -= old_kdv_tutari;
        }

        if ( MUH_FIS_SATIRI->borc_tutari EQ 0 AND MUH_FIS_SATIRI->alacak_tutari EQ 0 ) {
            MUH_FIS_SATIRINI_SIL(MUHASEBE_FISI , muh_kdv_satiri_id);
        }
        else {
            MUH_FIS_SATIRINI_GUNCELLE(MUHASEBE_FISI , MUH_FIS_SATIRI , muh_kdv_satiri_id);
        }
    }

    delete MUHASEBE_FISI;


    if ( tah_odm_sekli NE ENUM_NAKIT ) {

        BNK_HAREKET_FIS_STRUCT * BNK_FISI = new BNK_HAREKET_FIS_STRUCT;

        BNK_CLEAR_HAREKET_FIS_STRUCT(BNK_FISI);

        BNK_HAREKET_FIS_BILGILERINI_OKU(BNK_FISI , BNK_GET_ENT_FIS_ID(P_FIS->program_id,
                                                                      P_FIS->modul_id,P_FIS_SATIRI->fis_id,
                                                                      P_FIS_SATIRI->hesap_no_id));



        QStringList bnk_fis_satirlari_list = BNK_GET_ENT_ROW_ID_LIST(BNK_FISI->program_id,BNK_FISI->modul_id ,p_fis_satiri_id);

        for ( int i = 0 ; i < bnk_fis_satirlari_list.size() ; i++ ) {
            int bnk_fis_satiri_id = QVariant(bnk_fis_satirlari_list.at(i)).toInt();

            BNK_HAREKET_FIS_SATIRI_SIL(BNK_FISI , bnk_fis_satiri_id);
        }
        delete BNK_FISI;
    }

    double kdv_tutari = 0;

    if ( P_FIS_SATIRI->kdv_tutari.isEmpty() EQ false ) {
        kdv_tutari = P_FIS_SATIRI->kdv_tutari.at(0);
    }

    int kdv_hesap_id = 0;

    if ( P_FIS_SATIRI->kdv_hesap_id.isEmpty() EQ false ) {
        kdv_hesap_id = P_FIS_SATIRI->kdv_hesap_id.at(0);
    }

    query.PREPARE_UPDATE("prk_fis_satirlari","fis_satiri_id", "belge_no , aciklama , "
                         "kdv_orani , kdv_haric_tutar , kdv_tutari ,"
                         "kdv_dahil_tutar , gider_turu_id ,"
                         "kdv_hesap_id ,"
                         "tah_odm_sekli ,hesap_no_id ,"
                         "kredi_karti_id ,pos_id ,"
                         "kredi_karti_sahibi,kredi_karti_numarasi ",
                         "fis_satiri_id = :fis_satiri_id");

    query.SET_VALUE(":belge_no"          , P_FIS_SATIRI->belge_no);
    query.SET_VALUE(":aciklama"          , P_FIS_SATIRI->aciklama);
    query.SET_VALUE(":kdv_orani"         , P_FIS_SATIRI->kdv_orani);
    query.SET_VALUE(":kdv_haric_tutar"   , P_FIS_SATIRI->kdv_haric_tutar);
    query.SET_VALUE(":kdv_tutari"        , kdv_tutari);
    query.SET_VALUE(":kdv_dahil_tutar"   , P_FIS_SATIRI->kdv_dahil_tutar);
    query.SET_VALUE(":gider_turu_id"     , P_FIS_SATIRI->gelir_gider_turu_id);
    query.SET_VALUE(":kdv_hesap_id"      , kdv_hesap_id );
    query.SET_VALUE(":tah_odm_sekli"     , P_FIS_SATIRI->tah_odm_sekli );
    query.SET_VALUE(":fis_satiri_id"     , p_fis_satiri_id);
    query.SET_VALUE(":hesap_no_id"       , P_FIS_SATIRI->hesap_no_id);

    if ( P_FIS->gider_fisi_mi EQ 1 ) {
        query.SET_VALUE(":kredi_karti_id"    , P_FIS_SATIRI->kredi_karti_id);
        query.SET_VALUE(":pos_id"            , 0);
    }
    else {
        query.SET_VALUE(":kredi_karti_id"    , 0);
        query.SET_VALUE(":pos_id"            , P_FIS_SATIRI->pos_id);
    }
    query.SET_VALUE(":kredi_karti_sahibi"   , P_FIS_SATIRI->kredi_karti_sahibi);
    query.SET_VALUE(":kredi_karti_numarasi" , P_FIS_SATIRI->kredi_karti_numarasi);


    query.UPDATE();

    kdv_haric_toplam += P_FIS_SATIRI->kdv_haric_tutar;
    kdv_toplam       += kdv_tutari;
    kdv_dahil_toplam += P_FIS_SATIRI->kdv_dahil_tutar;

    P_FIS->kdv_haric_toplam = kdv_haric_toplam;
    P_FIS->kdv_toplam       = kdv_toplam;
    P_FIS->kdv_dahil_toplam = kdv_dahil_toplam;

    query.PREPARE_UPDATE("prk_fisler","fis_id" ,"kdv_haric_toplam , kdv_toplam ,"
                         "kdv_dahil_toplam ","fis_id = :fis_id");

    query.SET_VALUE(":kdv_haric_toplam" , P_FIS->kdv_haric_toplam);
    query.SET_VALUE(":kdv_toplam"       , P_FIS->kdv_toplam);
    query.SET_VALUE(":kdv_dahil_toplam" , P_FIS->kdv_dahil_toplam);
    query.SET_VALUE(":fis_id"           , P_FIS_SATIRI->fis_id);

    query.UPDATE();

    QList<int> fis_satiri_id_list = PRK_GET_AYNI_BELGE_NUMARALI_SATIR_LIST_AND_ENTEGRE_ET(p_fis_satiri_id,DB);

    if ( fis_satiri_id_list.isEmpty() EQ true ) {
        PRK_ENT_FISLERI_OLUSTUR(P_FIS , P_FIS_SATIRI , p_fis_satiri_id ,fis_satiri_id_list,DB);
    }
}

/**************************************************************************************
                                    PRK_FIS_GUNCELLE
***************************************************************************************/

void PRK_FIS_GUNCELLE(PRK_FIS_STRUCT *P_FIS, int p_fis_id)
{
    SQL_QUERY query(DB);

    query.PREPARE_UPDATE("prk_fisler","fis_id" ,"fis_no , "
                         "fis_tarihi  , aciklama  ,"
                         "sube_id , muh_kasa_hesap_id",
                         "fis_id = :fis_id");

    query.SET_VALUE(":fis_no"           , P_FIS->fis_no);
    query.SET_VALUE(":fis_tarihi"       , P_FIS->fis_tarihi);
    query.SET_VALUE(":aciklama"         , P_FIS->aciklama);
    query.SET_VALUE(":sube_id"          , P_FIS->sube_id);
    query.SET_VALUE(":muh_kasa_hesap_id", P_FIS->muh_kasa_hesap_id);
    query.SET_VALUE(":fis_id"           , p_fis_id);

    query.UPDATE();


    MUH_FIS_STRUCT * MUHASEBE_FISI = new MUH_FIS_STRUCT;

    MUH_FIS_BILGILERINI_OKU(MUHASEBE_FISI , P_FIS->muh_fis_id);

    QString aciklama = QString("Fiş PERAKENDE modülü tarafından üretilmiştir.Fiş No : %1 \n").arg(P_FIS->fis_no);
    aciklama.append(P_FIS->aciklama);

    MUHASEBE_FISI->aciklama         = aciklama;
    MUHASEBE_FISI->fis_tarihi       = P_FIS->fis_tarihi;

    MUH_FISI_GUNCELLE ( MUHASEBE_FISI ,P_FIS->muh_fis_id);

    BNK_HAREKET_FIS_STRUCT * BNK_FISI = new BNK_HAREKET_FIS_STRUCT;

    BNK_CLEAR_HAREKET_FIS_STRUCT(BNK_FISI);

    MUH_FIS_SATIRI_STRUCT * MUH_FIS_SATIRI = new MUH_FIS_SATIRI_STRUCT;

    query.PREPARE_SELECT("prk_fis_satirlari" , "tah_odm_sekli,hesap_no_id,muh_kasa_satiri_id" , "fis_id = :fis_id");

    query.SET_VALUE(":fis_id" , p_fis_id);

    if ( query.SELECT() NE 0 ) {

        while ( query.NEXT() ) {

            int tah_odm_sekli      = query.VALUE(0).toInt();
            int hesap_no_id        = query.VALUE(1).toInt();
            int muh_kasa_satiri_id = query.VALUE(2).toInt();

            if ( tah_odm_sekli NE ENUM_NAKIT ) {

                int bnk_fis_id = BNK_GET_ENT_FIS_ID(P_FIS->program_id , P_FIS->modul_id ,
                                                    p_fis_id , hesap_no_id );

                if ( bnk_fis_id > 0 ) {

                    BNK_HAREKET_FIS_BILGILERINI_OKU(BNK_FISI , bnk_fis_id);

                    BNK_FISI->fis_tarihi       = P_FIS->fis_tarihi;
                    BNK_FISI->aciklama         = aciklama;

                    BNK_HAREKET_FISI_GUNCELLE(BNK_FISI , bnk_fis_id);
                }

            }
            else {
                if ( muh_kasa_satiri_id > 0 ) {

                    MUH_CLEAR_FIS_SATIRI_STRUCT(MUH_FIS_SATIRI);
                    MUH_FIS_SATIRINI_OKU(MUH_FIS_SATIRI , muh_kasa_satiri_id );

                    MUH_FIS_SATIRI->hesap_id = P_FIS->muh_kasa_hesap_id;

                    MUH_FIS_SATIRINI_GUNCELLE(MUHASEBE_FISI,MUH_FIS_SATIRI , muh_kasa_satiri_id);
                }
            }
        }
    }




    delete BNK_FISI;
    delete MUHASEBE_FISI;

}


/**************************************************************************************
                                    PRK_FIS_BILGILERINI_OKU
***************************************************************************************/

void PRK_FIS_BILGILERINI_OKU(PRK_FIS_STRUCT *P_FIS, int p_fis_id , ADAK_SQL * P_GELEN_DB)
{
    ADAK_SQL * USE_DB;

    if ( P_GELEN_DB EQ NULL ) {
        USE_DB = DB;
    }
    else {
        USE_DB = P_GELEN_DB;
    }

    SQL_QUERY query(USE_DB);

    query.PREPARE_SELECT("prk_fisler" , "fis_no , fis_tarihi , aciklama , sube_id , kdv_haric_toplam,"
                         "kdv_toplam , kdv_dahil_toplam,modul_id,program_id,base_fis_id ,"
                         "muh_fis_id,gider_fisi_mi,muh_kasa_hesap_id" ,
                         "fis_id = :fis_id");

    query.SET_VALUE(":fis_id" , p_fis_id );

    if ( query.SELECT() EQ 0 ) {
        return;
    }

    query.NEXT();

    P_FIS->fis_no           = query.VALUE("fis_no").toInt();
    P_FIS->fis_tarihi       = query.VALUE("fis_tarihi").toString();
    P_FIS->aciklama         = query.VALUE("aciklama").toString();
    P_FIS->sube_id          = query.VALUE("sube_id").toInt();
    P_FIS->kdv_haric_toplam = query.VALUE("kdv_haric_toplam").toDouble();
    P_FIS->kdv_toplam       = query.VALUE("kdv_toplam").toDouble();
    P_FIS->kdv_dahil_toplam = query.VALUE("kdv_dahil_toplam").toDouble();
    P_FIS->modul_id         = query.VALUE("modul_id").toInt();
    P_FIS->program_id       = query.VALUE("program_id").toInt();
    P_FIS->base_fis_id      = query.VALUE("base_fis_id").toInt();
    P_FIS->muh_fis_id       = query.VALUE("muh_fis_id").toInt();
    P_FIS->gider_fisi_mi    = query.VALUE("gider_fisi_mi").toInt();
    P_FIS->muh_kasa_hesap_id= query.VALUE("muh_kasa_hesap_id").toInt();

}

/**************************************************************************************
                                    PRK_FIS_SATIRI_BILGILERINI_OKU
***************************************************************************************/
void PRK_FIS_SATIRI_BILGILERINI_OKU ( PRK_FIS_SATIRI_STRUCT *P_FIS_SATIRI, int p_fis_satiri_id , ADAK_SQL * P_GELEN_DB)
{

    ADAK_SQL * USE_DB;

    if ( P_GELEN_DB EQ NULL ) {
        USE_DB = DB;
    }
    else {
        USE_DB = P_GELEN_DB;
    }

    SQL_QUERY query(USE_DB);

    query.PREPARE_SELECT("prk_fis_satirlari" , "fis_id , belge_no , aciklama , kdv_orani , kdv_haric_tutar ,"
                         "kdv_dahil_tutar,gider_turu_id , modul_id , program_id,"
                         "base_record_id , kdv_hesap_id , tah_odm_sekli ,"
                         "hesap_no_id,kredi_karti_id , pos_id , kredi_karti_sahibi,"
                         "kredi_karti_numarasi,kdv_tutari,muh_gelir_gider_satiri_id,"
                         "muh_kdv_satiri_id,muh_kasa_satiri_id", "fis_satiri_id = :fis_satiri_id");

    query.SET_VALUE(":fis_satiri_id"  , p_fis_satiri_id );

    if ( query.SELECT() EQ 0 ) {
        return;
    }

    query.NEXT();

    P_FIS_SATIRI->fis_id              = query.VALUE("fis_id").toInt();
    P_FIS_SATIRI->belge_no            = query.VALUE("belge_no").toString();
    P_FIS_SATIRI->aciklama            = query.VALUE("aciklama").toString();
    P_FIS_SATIRI->kdv_orani           = query.VALUE("kdv_orani").toDouble();
    P_FIS_SATIRI->kdv_haric_tutar     = query.VALUE("kdv_haric_tutar").toDouble();
    P_FIS_SATIRI->kdv_dahil_tutar     = query.VALUE("kdv_dahil_tutar").toDouble();
    P_FIS_SATIRI->gelir_gider_turu_id = query.VALUE("gider_turu_id").toInt();
    P_FIS_SATIRI->modul_id            = query.VALUE("modul_id").toInt();
    P_FIS_SATIRI->program_id          = query.VALUE("program_id").toInt();
    P_FIS_SATIRI->base_record_id      = query.VALUE("base_record_id").toInt();
    if ( query.VALUE("kdv_hesap_id").toInt() > 0 ) {
        P_FIS_SATIRI->kdv_hesap_id        << query.VALUE("kdv_hesap_id").toInt();
    }
    P_FIS_SATIRI->tah_odm_sekli       = query.VALUE("tah_odm_sekli").toInt();
    P_FIS_SATIRI->hesap_no_id         = query.VALUE("hesap_no_id").toInt();
    P_FIS_SATIRI->kredi_karti_id      = query.VALUE("kredi_karti_id").toInt();
    P_FIS_SATIRI->pos_id              = query.VALUE("pos_id").toInt();
    P_FIS_SATIRI->kredi_karti_sahibi  = query.VALUE("kredi_karti_sahibi").toString();
    P_FIS_SATIRI->kredi_karti_numarasi= query.VALUE("kredi_karti_numarasi").toString();
    if ( query.VALUE("kdv_tutari").toDouble() > 0 ) {
        P_FIS_SATIRI->kdv_tutari          << query.VALUE("kdv_tutari").toDouble();
    }
    P_FIS_SATIRI->muh_gelir_gider_satiri_id = query.VALUE("muh_gelir_gider_satiri_id").toInt();
    P_FIS_SATIRI->muh_kasa_satiri_id        = query.VALUE("muh_kasa_satiri_id").toInt();
    P_FIS_SATIRI->muh_kdv_satiri_id         = query.VALUE("muh_kdv_satiri_id").toInt();

}


/**************************************************************************************
                                    PRK_CLEAR_FIS_STRUCT
***************************************************************************************/

void PRK_CLEAR_FIS_STRUCT(PRK_FIS_STRUCT *PRK_FISI)
{
    PRK_FISI->modul_id         = -1;
    PRK_FISI->program_id       = -1;
    PRK_FISI->base_fis_id      = 0;
    PRK_FISI->muh_fis_id       = 0;
    PRK_FISI->fis_no           = 0;
    PRK_FISI->kdv_dahil_toplam = 0;
    PRK_FISI->kdv_haric_toplam = 0;
    PRK_FISI->kdv_toplam       = 0;
    PRK_FISI->sube_id          = 0;
    PRK_FISI->gider_fisi_mi    = -1;
    PRK_FISI->muh_kasa_hesap_id= 0;

    PRK_FISI->fis_tarihi.clear();
    PRK_FISI->aciklama.clear();

}
/**************************************************************************************
                                    PRK_CLEAR_FIS_SATIRI_STRUCT
***************************************************************************************/

void PRK_CLEAR_FIS_SATIRI_STRUCT(PRK_FIS_SATIRI_STRUCT *PRK_FIS_SATIRI)
{
    PRK_FIS_SATIRI->modul_id            = -1;
    PRK_FIS_SATIRI->program_id          = -1;
    PRK_FIS_SATIRI->base_record_id      = -1;
    PRK_FIS_SATIRI->fis_id              = 0;
    PRK_FIS_SATIRI->kdv_orani           = 0;
    PRK_FIS_SATIRI->kdv_dahil_tutar     = 0;
    PRK_FIS_SATIRI->kdv_haric_tutar     = 0;
    PRK_FIS_SATIRI->kdv_tutari.clear();
    PRK_FIS_SATIRI->gelir_gider_turu_id = 0;
    PRK_FIS_SATIRI->kdv_hesap_id.clear();
    PRK_FIS_SATIRI->tah_odm_sekli       = -1;
    PRK_FIS_SATIRI->hesap_no_id         = 0;
    PRK_FIS_SATIRI->kredi_karti_id      = 0;
    PRK_FIS_SATIRI->pos_id              = 0;
    PRK_FIS_SATIRI->order_number        = 0;

    PRK_FIS_SATIRI->belge_no.clear();
    PRK_FIS_SATIRI->aciklama.clear();
    PRK_FIS_SATIRI->kredi_karti_numarasi.clear();
    PRK_FIS_SATIRI->kredi_karti_sahibi.clear();

    PRK_FIS_SATIRI->yeni_kdv_satiri_eklenecek = 0;
    PRK_FIS_SATIRI->kdv_oranlari_list.clear();

}

/**************************************************************************************
                                    PRK_ENT_FISLERI_OLUSTUR
***************************************************************************************/

void PRK_ENT_FISLERI_OLUSTUR(PRK_FIS_STRUCT *P_FIS, PRK_FIS_SATIRI_STRUCT *P_FIS_SATIRI,int p_fis_satiri_id ,
                             QList<int>ayni_belge_numarali_satir_id_list,ADAK_SQL * P_GELEN_DB)
{
    ADAK_SQL * USE_DB;

    if ( P_GELEN_DB EQ NULL ) {
        USE_DB = DB;
    }
    else {
        USE_DB = P_GELEN_DB;
    }

    SQL_QUERY query(USE_DB);

    int gider_hesabi_id = 0;

    query.PREPARE_SELECT("prk_gelir_gider_turleri","hesap_id", "tur_id = :tur_id");
    query.SET_VALUE(":tur_id" ,P_FIS_SATIRI->gelir_gider_turu_id);

    if ( query.SELECT() NE 0 ) {
        query.NEXT();
        gider_hesabi_id = query.VALUE(0).toInt();
    }

    if ( gider_hesabi_id EQ 0 ) {
        return;
    }
    BNK_HAREKET_FIS_STRUCT * BNK_FISI = new BNK_HAREKET_FIS_STRUCT;

    BNK_HAREKET_FIS_SATIRI_STRUCT * BNK_FIS_SATIRI = new BNK_HAREKET_FIS_SATIRI_STRUCT;

    BNK_CLEAR_HAREKET_FIS_STRUCT(BNK_FISI);

    BNK_CLEAR_HAREKET_FIS_SATIRI_STRUCT(BNK_FIS_SATIRI);


    MUH_FIS_STRUCT * MUHASEBE_FISI         = new MUH_FIS_STRUCT;

    MUH_FIS_SATIRI_STRUCT * MUH_FIS_SATIRI = new MUH_FIS_SATIRI_STRUCT;


    MUH_CLEAR_FIS_STRUCT ( MUHASEBE_FISI );

    MUH_CLEAR_FIS_SATIRI_STRUCT ( MUH_FIS_SATIRI );

    MUH_FIS_BILGILERINI_OKU(MUHASEBE_FISI , P_FIS->muh_fis_id , USE_DB);

    int muh_kasa_satiri_id        = 0;
    int muh_gelir_gider_satiri_id = 0;
    int muh_kdv_satiri_id         = 0;

    if ( P_FIS_SATIRI->tah_odm_sekli NE ENUM_NAKIT ) {

        int bnk_fis_id = BNK_GET_ENT_FIS_ID ( P_FIS->program_id , P_FIS->modul_id , P_FIS_SATIRI->fis_id , P_FIS_SATIRI->hesap_no_id,USE_DB);

        if ( bnk_fis_id EQ 0 ) {

            BNK_FISI->base_fis_id           = P_FIS_SATIRI->fis_id;
            BNK_FISI->program_id            = P_FIS->program_id;
            BNK_FISI->modul_id              = P_FIS->modul_id;
            BNK_FISI->hesap_no_id           = P_FIS_SATIRI->hesap_no_id;

            QString fis_aciklama         = QObject::tr("Fiş Perakende Modülü Tarafından Üretilmiştir.Fiş No : %1\n").arg(P_FIS->fis_no);
            fis_aciklama.append(P_FIS->aciklama);

            BNK_FISI->aciklama       = fis_aciklama;
            BNK_FISI->fis_tarihi     = P_FIS->fis_tarihi;
            BNK_FISI->fis_turu       = ENUM_BANKA_HAREKET_FISI;
            BNK_FISI->muh_fis_id     = P_FIS->muh_fis_id;
            BNK_FISI->odm_emr_hesabi_etkilenecek_mi = 1;
            BNK_FISI->para_birim_id  = DVZ_GET_TEMEL_PARA_BIRIMI_ID();

            bnk_fis_id = BNK_HAREKET_FIS_EKLE(BNK_FISI ,0,true , USE_DB);
        }
        else {

            BNK_HAREKET_FIS_BILGILERINI_OKU ( BNK_FISI , bnk_fis_id , USE_DB);
            BNK_FISI->hesap_no_id = P_FIS_SATIRI->hesap_no_id;
        }


        BNK_FIS_SATIRI->fis_id           = bnk_fis_id;
        BNK_FIS_SATIRI->program_id       = P_FIS_SATIRI->program_id;
        BNK_FIS_SATIRI->modul_id         = P_FIS_SATIRI->modul_id;
        BNK_FIS_SATIRI->base_record_id   = p_fis_satiri_id;
        BNK_FIS_SATIRI->aciklama         = P_FIS_SATIRI->aciklama;
        BNK_FIS_SATIRI->karsi_hesap_eklenecek_mi = 0;



        switch ( P_FIS_SATIRI->tah_odm_sekli ) {
        case ENUM_KREDI_KARTI :
            {
                if ( P_FIS->gider_fisi_mi EQ 1 ) {
                    BNK_FIS_SATIRI->etkilenecek_hesap_turu = ENUM_BNK_KREDI_KARTI_HESABI;
                    BNK_FIS_SATIRI->etkilenecek_hesap_id   = P_FIS_SATIRI->kredi_karti_id;
                }
                else {
                    BNK_FIS_SATIRI->etkilenecek_hesap_turu = ENUM_BNK_POS_HESABI;
                    BNK_FIS_SATIRI->etkilenecek_hesap_id   = P_FIS_SATIRI->pos_id;

                }
            }
            break;
        case ENUM_EFT    :
        case ENUM_HAVALE :
            {
                BNK_FIS_SATIRI->etkilenecek_hesap_turu = ENUM_BNK_BANKA_HESABI;
                BNK_FIS_SATIRI->etkilenecek_hesap_id   = P_FIS_SATIRI->hesap_no_id;

            }
            break;
        default:
            break;

        }

        if ( P_FIS->gider_fisi_mi EQ 1 ) {
            BNK_FIS_SATIRI->hesaba_giren   = 0.0;
            BNK_FIS_SATIRI->hesaptan_cikan = P_FIS_SATIRI->kdv_dahil_tutar;

        }
        else {
            BNK_FIS_SATIRI->hesaba_giren   = P_FIS_SATIRI->kdv_dahil_tutar;
            BNK_FIS_SATIRI->hesaptan_cikan = 0.0;
        }

        if ( P_FIS->gider_fisi_mi EQ 0 ) {
            BNK_HAREKET_FIS_SATIRI_EKLE(BNK_FISI , BNK_FIS_SATIRI , 0 , true , USE_DB);
        }
    }
    else {
        //NAKIT ISE

        if ( P_FIS_SATIRI->base_record_id EQ 0 ) {
            P_FIS_SATIRI->base_record_id = p_fis_satiri_id;
        }

        MUH_FIS_SATIRI->hesap_id            = P_FIS->muh_kasa_hesap_id;
        MUH_FIS_SATIRI->aciklama            = P_FIS_SATIRI->aciklama;
        MUH_FIS_SATIRI->base_record_id      = P_FIS_SATIRI->base_record_id;
        MUH_FIS_SATIRI->fis_id              = P_FIS->muh_fis_id;
        MUH_FIS_SATIRI->modul_id            = MUHASEBE_FISI->modul_id;
        MUH_FIS_SATIRI->program_id          = MUHASEBE_FISI->program_id;

        if ( P_FIS->gider_fisi_mi EQ 1 ) {
            MUH_FIS_SATIRI->borc_tutari      = 0.0;
            MUH_FIS_SATIRI->alacak_tutari    = P_FIS_SATIRI->kdv_dahil_tutar;
        }
        else {
            MUH_FIS_SATIRI->borc_tutari      = P_FIS_SATIRI->kdv_dahil_tutar;
            MUH_FIS_SATIRI->alacak_tutari    = 0.0;
            muh_kasa_satiri_id = MUH_FIS_SATIRI_EKLE ( MUHASEBE_FISI , MUH_FIS_SATIRI , USE_DB);
        }

    }



    MUH_FIS_SATIRI->aciklama            = P_FIS_SATIRI->aciklama;
    MUH_FIS_SATIRI->base_record_id      = P_FIS_SATIRI->base_record_id;

    if ( P_FIS->gider_fisi_mi EQ 1 ) {
        MUH_FIS_SATIRI->borc_tutari     = P_FIS_SATIRI->kdv_haric_tutar;
        MUH_FIS_SATIRI->alacak_tutari   = 0;
    }
    else {
        MUH_FIS_SATIRI->alacak_tutari   = P_FIS_SATIRI->kdv_haric_tutar;
        MUH_FIS_SATIRI->borc_tutari     = 0;
    }

    MUH_FIS_SATIRI->fis_id              = P_FIS->muh_fis_id;
    MUH_FIS_SATIRI->hesap_id            = gider_hesabi_id;
    MUH_FIS_SATIRI->modul_id            = MUHASEBE_FISI->modul_id;
    MUH_FIS_SATIRI->program_id          = MUHASEBE_FISI->program_id;

    muh_gelir_gider_satiri_id = MUH_FIS_SATIRI_EKLE ( MUHASEBE_FISI , MUH_FIS_SATIRI , USE_DB);


    if ( P_FIS_SATIRI->kdv_tutari.isEmpty() EQ false ) {

        for ( int i = 0 ; i < P_FIS_SATIRI->kdv_hesap_id.size() ; i++ ) {

            if ( P_FIS->gider_fisi_mi EQ 1 ) {
                MUH_FIS_SATIRI->borc_tutari      = P_FIS_SATIRI->kdv_tutari.at(i);
                MUH_FIS_SATIRI->alacak_tutari    = 0;
            }
            else {
                MUH_FIS_SATIRI->alacak_tutari    = P_FIS_SATIRI->kdv_tutari.at(i);
                MUH_FIS_SATIRI->borc_tutari      = 0;
            }

            MUH_FIS_SATIRI->hesap_id             = P_FIS_SATIRI->kdv_hesap_id.at(i);

            double kdv_orani = 0;

            if ( P_FIS_SATIRI->kdv_oranlari_list.isEmpty() EQ false ) {
                kdv_orani = P_FIS_SATIRI->kdv_oranlari_list.at(i);
            }

            muh_kdv_satiri_id = MUH_FIS_SATIRI_EKLE ( MUHASEBE_FISI , MUH_FIS_SATIRI , USE_DB);

            if ( ayni_belge_numarali_satir_id_list.isEmpty() EQ false ) {

                ayni_belge_numarali_satir_id_list.append(p_fis_satiri_id);

                query.PREPARE_SELECT("prk_fis_satirlari" ,
                                     "fis_satiri_id","kdv_orani = :kdv_orani");
                query.SET_VALUE(":kdv_orani" , kdv_orani);
                query.AND_MULTI_EKLE("fis_satiri_id",query.TO_QVARIANT(ayni_belge_numarali_satir_id_list),ADAK_OR);

                if (query.SELECT() NE 0 ) {

                    SQL_QUERY s_query(USE_DB);

                    while ( query.NEXT() ) {
                        int prk_satir_id = query.VALUE(0).toInt();

                        s_query.PREPARE_UPDATE("prk_fis_satirlari" ,"fis_satiri_id",
                                               "muh_kdv_satiri_id","fis_satiri_id = :fis_satiri_id");
                        s_query.SET_VALUE(":muh_kdv_satiri_id" , muh_kdv_satiri_id);
                        s_query.SET_VALUE(":fis_satiri_id"     , prk_satir_id);
                        s_query.UPDATE();
                    }
                }
            }
        }
    }

    if ( P_FIS->gider_fisi_mi EQ 1 ) {
        if ( P_FIS_SATIRI->tah_odm_sekli NE ENUM_NAKIT ) {
            BNK_HAREKET_FIS_SATIRI_EKLE(BNK_FISI , BNK_FIS_SATIRI , 0 , true , USE_DB);
        }
        else {
            MUH_FIS_SATIRI->borc_tutari      = 0.0;
            MUH_FIS_SATIRI->alacak_tutari    = P_FIS_SATIRI->kdv_dahil_tutar;
            MUH_FIS_SATIRI->hesap_id         = P_FIS->muh_kasa_hesap_id;

            muh_kasa_satiri_id               = MUH_FIS_SATIRI_EKLE(MUHASEBE_FISI , MUH_FIS_SATIRI , USE_DB);
        }
    }

    if ( ayni_belge_numarali_satir_id_list.isEmpty() EQ false ) {

        for ( int i = 0 ; i < ayni_belge_numarali_satir_id_list.size() ; i++ ) {


            query.PREPARE_UPDATE("prk_fis_satirlari" , "fis_satiri_id" ,
                                 "muh_kasa_satiri_id,"
                                 "muh_gelir_gider_satiri_id ",
                                 "fis_satiri_id = :fis_satiri_id");
            query.SET_VALUE(":muh_kasa_satiri_id"        , muh_kasa_satiri_id);
            query.SET_VALUE(":muh_gelir_gider_satiri_id" , muh_gelir_gider_satiri_id);
            query.SET_VALUE(":fis_satiri_id"             , ayni_belge_numarali_satir_id_list.at(i));
            query.UPDATE();

        }
    }
    else {
        query.PREPARE_UPDATE("prk_fis_satirlari" ,"fis_satiri_id" ,
                             "muh_kdv_satiri_id,muh_kasa_satiri_id,muh_gelir_gider_satiri_id" ,
                             "fis_satiri_id = :fis_satiri_id");
        query.SET_VALUE(":muh_kdv_satiri_id"         , muh_kdv_satiri_id);
        query.SET_VALUE(":muh_kasa_satiri_id"        , muh_kasa_satiri_id);
        query.SET_VALUE(":muh_gelir_gider_satiri_id" , muh_gelir_gider_satiri_id);
        query.SET_VALUE(":fis_satiri_id"             , p_fis_satiri_id);
        query.UPDATE();
    }

    delete BNK_FISI;
    delete BNK_FIS_SATIRI;

    delete MUHASEBE_FISI;
    delete MUH_FIS_SATIRI;

}

/**************************************************************************************
                                    PRK_GET_FIS_SATIRI_ID_LIST
***************************************************************************************/

QList <int> PRK_GET_FIS_SATIRI_ID_LIST(int p_fis_id , ADAK_SQL * P_GELEN_DB)
{
    ADAK_SQL * USE_DB;

    if ( P_GELEN_DB EQ NULL ) {
        USE_DB = DB;
    }
    else {
        USE_DB = P_GELEN_DB;
    }

    SQL_QUERY query(USE_DB);

    QList<int> fis_satiri_id_list;

    query.PREPARE_SELECT("prk_fis_satirlari" , "fis_satiri_id" , "fis_id = :fis_id");
    query.SET_VALUE(":fis_id" , p_fis_id);

    if ( query.SELECT() EQ 0 ) {
        return fis_satiri_id_list;
    }

    while ( query.NEXT() ) {
        fis_satiri_id_list << query.VALUE(0).toInt();
    }
    return fis_satiri_id_list;
}

/**************************************************************************************
                        PRK_GET_AYNI_BELGE_NUMARALI_SATIR_LIST
***************************************************************************************/

QList<int> PRK_GET_AYNI_BELGE_NUMARALI_SATIR_LIST_AND_ENTEGRE_ET(int p_fis_satiri_id , ADAK_SQL * P_GELEN_DB)
{
    SQL_QUERY query(P_GELEN_DB);

    PRK_FIS_SATIRI_STRUCT * PRK_FIS_SATIRI = new PRK_FIS_SATIRI_STRUCT;

    PRK_CLEAR_FIS_SATIRI_STRUCT(PRK_FIS_SATIRI);


    PRK_FIS_SATIRI_BILGILERINI_OKU(PRK_FIS_SATIRI , p_fis_satiri_id , P_GELEN_DB);


    QList<int> fis_satiri_id_list;

    query.PREPARE_SELECT("prk_fis_satirlari" , "fis_satiri_id,kdv_orani,kdv_haric_tutar,"
                         "kdv_tutari,kdv_dahil_tutar,kdv_hesap_id,muh_gelir_gider_satiri_id,"
                         "muh_kdv_satiri_id,muh_kasa_satiri_id,tah_odm_sekli","fis_id = :fis_id "
                         "AND belge_no = :belge_no AND tah_odm_sekli = :tah_odm_sekli "
                         "AND fis_satiri_id != :fis_satiri_id AND gider_turu_id = :gider_turu_id");

    query.SET_VALUE(":fis_id"        , PRK_FIS_SATIRI->fis_id);
    query.SET_VALUE(":belge_no"      , PRK_FIS_SATIRI->belge_no);
    query.SET_VALUE(":tah_odm_sekli" , PRK_FIS_SATIRI->tah_odm_sekli);
    query.SET_VALUE(":fis_satiri_id" , p_fis_satiri_id);
    query.SET_VALUE(":gider_turu_id" , PRK_FIS_SATIRI->gelir_gider_turu_id);

    if ( query.SELECT() EQ 0 ) {
        return fis_satiri_id_list;
    }

    PRK_FIS_STRUCT * PRK_FISI = new PRK_FIS_STRUCT;

    PRK_CLEAR_FIS_STRUCT(PRK_FISI);


    PRK_FIS_BILGILERINI_OKU(PRK_FISI , PRK_FIS_SATIRI->fis_id ,P_GELEN_DB);

    double kdv_haric_tutar = PRK_FIS_SATIRI->kdv_haric_tutar;
    double kdv_dahil_tutar = PRK_FIS_SATIRI->kdv_dahil_tutar;

    MUH_FIS_STRUCT * MUH_FIS = new MUH_FIS_STRUCT;

    MUH_CLEAR_FIS_STRUCT ( MUH_FIS );

    MUH_FIS->modul_id    = PRK_FISI->modul_id;
    MUH_FIS->program_id  = PRK_FISI->program_id;
    MUH_FIS->base_fis_id = PRK_FIS_SATIRI->fis_id;

    QList<double> kdv_oranlari_list;

    if ( PRK_FIS_SATIRI->kdv_orani > 0) {
        kdv_oranlari_list << PRK_FIS_SATIRI->kdv_orani;
    }

    while ( query.NEXT() ) {
        fis_satiri_id_list << query.VALUE(0).toInt();

        double uygun_satir_kdv_orani         = query.VALUE(1).toDouble();
        double uygun_satir_kdv_haric_tutar   = query.VALUE(2).toDouble();
        double uygun_satir_kdv_tutari        = query.VALUE(3).toDouble();
        double uygun_satir_kdv_dahil_tutar   = query.VALUE(4).toDouble();
        int    uygun_satir_kdv_hesap_id      = query.VALUE(5).toDouble();
        int    muh_gelir_gider_satiri_id     = query.VALUE(6).toInt();
        int    muh_kdv_satiri_id             = query.VALUE(7).toInt();
        int    muh_kasa_satiri_id            = query.VALUE(8).toInt();
        int    tah_odm_sekli                 = query.VALUE(9).toInt();

        kdv_haric_tutar += uygun_satir_kdv_haric_tutar;
        kdv_dahil_tutar += uygun_satir_kdv_dahil_tutar;

        if ( uygun_satir_kdv_orani > 0 ) {

            if ( kdv_oranlari_list.contains(uygun_satir_kdv_orani) EQ true ) {

                int index = kdv_oranlari_list.indexOf(uygun_satir_kdv_orani);

                PRK_FIS_SATIRI->kdv_tutari[index] += uygun_satir_kdv_tutari;
            }
            else {
                kdv_oranlari_list.append(uygun_satir_kdv_orani);
                PRK_FIS_SATIRI->kdv_tutari .append(uygun_satir_kdv_tutari);
                PRK_FIS_SATIRI->kdv_hesap_id.append(uygun_satir_kdv_hesap_id);
            }
        }
        MUH_FIS_SATIRINI_SIL(MUH_FIS , muh_gelir_gider_satiri_id , P_GELEN_DB);

        MUH_FIS_SATIRINI_SIL(MUH_FIS , muh_kdv_satiri_id         , P_GELEN_DB);

        if ( tah_odm_sekli NE ENUM_NAKIT ) {
            QStringList muh_ent_row_id_list = MUH_GET_ENT_ROW_ID_LIST(PRK_FIS_SATIRI->program_id,
                                                                      PRK_FIS_SATIRI->modul_id ,
                                                                      query.VALUE(0).toInt());

            for ( int i = 0 ; i < muh_ent_row_id_list.size() ; i++ ) {
                int muh_fis_satiri_id = QVariant(muh_ent_row_id_list.at(i)).toInt();

                MUH_FIS_SATIRINI_SIL(MUH_FIS,muh_fis_satiri_id , P_GELEN_DB);
            }
        }
        else {
            MUH_FIS_SATIRINI_SIL(MUH_FIS , muh_kasa_satiri_id        , P_GELEN_DB);
        }

    }

    PRK_FIS_SATIRI->kdv_oranlari_list = kdv_oranlari_list;
    PRK_FIS_SATIRI->kdv_dahil_tutar   = kdv_dahil_tutar;
    PRK_FIS_SATIRI->kdv_haric_tutar   = kdv_haric_tutar;


    PRK_FIS_SATIRI_EKLE(PRK_FISI , PRK_FIS_SATIRI ,fis_satiri_id_list, p_fis_satiri_id , false , P_GELEN_DB);

    return fis_satiri_id_list;
}

