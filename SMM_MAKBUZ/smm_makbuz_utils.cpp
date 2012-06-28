#include "smm_makbuz_utils.h"
#include "adak_sql.h"
#include "smm_console_utils.h"
#include "muh_fis_utils.h"
#include "muh_enum.h"
#include "adak_utils.h"
#include "bnk_fis_utils.h"
#include "e9_enum.h"
#include "smm_enum.h"
#include "banka_enum.h"
#include "dvz_enum.h"
#include "dvz_console_utils.h"
#include "cari_enum.h"
#include "cari_fis_utils.h"

extern ADAK_SQL * DB;

/**************************************************************************************
                   SMM_MAKBUZ_EKLE
***************************************************************************************/

int SMM_MAKBUZ_EKLE(SMM_MAKBUZ_STRUCT *P_MAKBUZ)
{

    if ( P_MAKBUZ->makbuz_no EQ 0 ) {
        P_MAKBUZ->makbuz_no = SMM_SIRADAKI_MAKBUZ_NO_AL(P_MAKBUZ->makbuz_tarihi);
    }

    P_MAKBUZ->tevkifat_tutari = 0;

    if ( P_MAKBUZ->tevkifatli_mi EQ 1 ) {
        P_MAKBUZ->tevkifat_tutari = (P_MAKBUZ->kdv_tutari / 100) * P_MAKBUZ->tevkifat_yuzdesi;
    }


    SQL_QUERY query(DB);

    query.PREPARE_INSERT("smm_makbuzlar" , "makbuz_id" , "makbuz_no,makbuz_tarihi,aciklama,"
                         "cari_hesap_id,"
                         "cari_hesap_adresi,vergi_dairesi,vergi_numarasi,sm_sicil_numarasi,modul_id,"
                         "program_id,base_record_id,hizmet_tutari,muh_kasa_hesap_id,"
                         "tevkifatli_mi,tevkifat_yuzdesi,makbuz_turu,muh_fis_id,"
                         "belge_seri,belge_numarasi,"
                         "tahsil_edildi_odendi,brut_tutar,"
                         "tevkifat_tutari,kdv_tevkifat_muh_hesap_id,"
                         "kdv_orani,vergi_stopaj_orani,kdv_hesap_id,"
                         "vergi_stopaj_tutari, net_tutar,hizmet_id,tah_odm_sekli,"
                         "hesap_no_id,kredi_karti_id,pos_id,"
                         "kdv_tutari");

    query.SET_VALUE(":makbuz_no"                   , P_MAKBUZ->makbuz_no);
    query.SET_VALUE(":makbuz_tarihi"               , P_MAKBUZ->makbuz_tarihi);
    query.SET_VALUE(":aciklama"                    , P_MAKBUZ->aciklama);
    query.SET_VALUE(":cari_hesap_id"               , P_MAKBUZ->cari_hesap_id);
    query.SET_VALUE(":cari_hesap_adresi"           , P_MAKBUZ->cari_hesap_adresi);
    query.SET_VALUE(":vergi_dairesi"               , P_MAKBUZ->vergi_dairesi);
    query.SET_VALUE(":vergi_numarasi"              , P_MAKBUZ->vergi_numarasi);
    query.SET_VALUE(":sm_sicil_numarasi"                , P_MAKBUZ->sm_sicil_numarasi);
    query.SET_VALUE(":modul_id"                    , P_MAKBUZ->modul_id);
    query.SET_VALUE(":program_id"                  , P_MAKBUZ->program_id);
    query.SET_VALUE(":base_record_id"              , P_MAKBUZ->base_record_id);
    query.SET_VALUE(":hizmet_tutari"               , P_MAKBUZ->hizmet_tutari);
    query.SET_VALUE(":muh_kasa_hesap_id"           , P_MAKBUZ->muh_kasa_hesap_id);
    query.SET_VALUE(":tevkifatli_mi"               , P_MAKBUZ->tevkifatli_mi);
    query.SET_VALUE(":tevkifat_yuzdesi"            , P_MAKBUZ->tevkifat_yuzdesi);
    query.SET_VALUE(":makbuz_turu"                 , P_MAKBUZ->makbuz_turu);
    query.SET_VALUE(":muh_fis_id"                  , P_MAKBUZ->muh_fis_id);
    query.SET_VALUE(":belge_seri"                  , P_MAKBUZ->belge_seri);
    query.SET_VALUE(":belge_numarasi"              , P_MAKBUZ->belge_numarasi);
    query.SET_VALUE(":tahsil_edildi_odendi"        , P_MAKBUZ->tahsil_edildi_odendi);
    query.SET_VALUE(":brut_tutar"                  , P_MAKBUZ->brut_tutar);
    query.SET_VALUE(":tevkifat_tutari"             , P_MAKBUZ->tevkifat_tutari);
    query.SET_VALUE(":kdv_tevkifat_muh_hesap_id"   , P_MAKBUZ->kdv_tevkifat_muh_hesap_id);
    query.SET_VALUE(":kdv_orani"                   , P_MAKBUZ->kdv_orani);
    query.SET_VALUE(":vergi_stopaj_orani"          , P_MAKBUZ->vergi_orani);
    query.SET_VALUE(":kdv_hesap_id"                , P_MAKBUZ->kdv_hesap_id);
    query.SET_VALUE(":vergi_stopaj_tutari"         , P_MAKBUZ->vergi_stopaj_tutari);
    query.SET_VALUE(":net_tutar"                   , P_MAKBUZ->net_tutar);
    query.SET_VALUE(":hizmet_id"                   , P_MAKBUZ->hizmet_id);
    query.SET_VALUE(":tah_odm_sekli"               , P_MAKBUZ->tah_odm_sekli);
    query.SET_VALUE(":hesap_no_id"                 , P_MAKBUZ->hesap_no_id);
    query.SET_VALUE(":kredi_karti_id"              , P_MAKBUZ->kredi_karti_id);
    query.SET_VALUE(":pos_id"                      , P_MAKBUZ->pos_id);
    query.SET_VALUE(":kdv_tutari"                  , P_MAKBUZ->kdv_tutari);


    int makbuz_id = query.INSERT();

    if ( P_MAKBUZ->muh_fis_id EQ 0 AND P_MAKBUZ->modul_id EQ SMM_MODULU ) {

        QString aciklama = QString("Fiş SM MAKBUZ modülü tarafından üretilmiştir.Fiş No : %1 \n").arg(P_MAKBUZ->makbuz_no);
        aciklama.append(P_MAKBUZ->aciklama);

        MUH_FIS_STRUCT * MUHASEBE_FISI = new MUH_FIS_STRUCT;

        MUH_CLEAR_FIS_STRUCT ( MUHASEBE_FISI );

        MUHASEBE_FISI->aciklama         = aciklama;
        MUHASEBE_FISI->base_fis_id      = makbuz_id;
        MUHASEBE_FISI->fis_tarihi       = P_MAKBUZ->makbuz_tarihi;
        MUHASEBE_FISI->fis_turu         = ENUM_MAHSUP_FISI;
        MUHASEBE_FISI->modul_id         = P_MAKBUZ->modul_id;
        MUHASEBE_FISI->program_id       = P_MAKBUZ->program_id;

        P_MAKBUZ->muh_fis_id = MUH_FIS_EKLE(MUHASEBE_FISI);

        query.PREPARE_UPDATE("smm_makbuzlar","makbuz_id", "muh_fis_id" ,"makbuz_id = :makbuz_id");
        query.SET_VALUE(":muh_fis_id" , P_MAKBUZ->muh_fis_id);
        query.SET_VALUE(":makbuz_id"  , makbuz_id);
        query.UPDATE();

        delete MUHASEBE_FISI;
    }

    SMM_ENT_FISLERI_OLUSTUR(P_MAKBUZ , makbuz_id);

    return makbuz_id;
}

/**************************************************************************************
                   SM_MAKBUZ_FIS_GUNCELLE
***************************************************************************************/

void SMM_MAKBUZ_GUNCELLE(SMM_MAKBUZ_STRUCT *P_MAKBUZ, int p_makbuz_id)
{
    SQL_QUERY query(DB);

    query.PREPARE_SELECT("smm_makbuzlar" , "modul_id,"
                         "program_id,base_record_id,hesap_no_id",
                         "makbuz_id = :makbuz_id");
    query.SET_VALUE(":makbuz_id" , p_makbuz_id);

    if ( query.SELECT() EQ 0 ) {
        return;
    }
    query.NEXT();

    int    modul_id                 = query.VALUE(0).toInt();
    int    program_id               = query.VALUE(1).toInt();
    int    base_record_id           = query.VALUE(2).toInt();
    int    old_hesap_no_id          = query.VALUE(3).toInt();

    if ( P_MAKBUZ->modul_id NE modul_id OR P_MAKBUZ->program_id NE program_id OR
         P_MAKBUZ->base_record_id NE base_record_id ) {
        return;
    }

    if ( P_MAKBUZ->modul_id EQ SMM_MODULU ) {

        if ( base_record_id EQ 0 ) {
            base_record_id = p_makbuz_id;
        }
    }

    SMM_ENT_FISLERI_SIL(P_MAKBUZ , p_makbuz_id , old_hesap_no_id);

    P_MAKBUZ->tevkifat_tutari = 0.0;

    if ( P_MAKBUZ->tevkifatli_mi EQ 1 ) {
        P_MAKBUZ->tevkifat_tutari = (P_MAKBUZ->kdv_tutari / 100) * P_MAKBUZ->tevkifat_yuzdesi;
    }


    query.PREPARE_UPDATE("smm_makbuzlar" , "makbuz_id" , "makbuz_no,makbuz_tarihi,aciklama,"
                         "cari_hesap_id,cari_hesap_adresi,vergi_dairesi,vergi_numarasi,"
                         "sm_sicil_numarasi,muh_kasa_hesap_id,tevkifatli_mi,"
                         "tevkifat_yuzdesi,belge_seri,belge_numarasi,"
                         "tahsil_edildi_odendi,"
                         "kdv_tevkifat_muh_hesap_id,kdv_orani,vergi_stopaj_orani,"
                         "kdv_hesap_id,hizmet_tutari,brut_tutar,tevkifat_tutari,"
                         "vergi_stopaj_tutari,net_tutar,hizmet_id,tah_odm_sekli,"
                         "hesap_no_id,kredi_karti_id,pos_id,"
                         "kdv_tutari",
                         "makbuz_id = :makbuz_id");

    query.SET_VALUE(":makbuz_no"            , P_MAKBUZ->makbuz_no);
    query.SET_VALUE(":makbuz_tarihi"        , P_MAKBUZ->makbuz_tarihi);
    query.SET_VALUE(":aciklama"             , P_MAKBUZ->aciklama);
    query.SET_VALUE(":cari_hesap_id"        , P_MAKBUZ->cari_hesap_id);
    query.SET_VALUE(":cari_hesap_adresi"    , P_MAKBUZ->cari_hesap_adresi);
    query.SET_VALUE(":vergi_dairesi"        , P_MAKBUZ->vergi_dairesi );
    query.SET_VALUE(":vergi_numarasi"       , P_MAKBUZ->vergi_numarasi);
    query.SET_VALUE(":sm_sicil_numarasi"         , P_MAKBUZ->sm_sicil_numarasi);
    query.SET_VALUE(":muh_kasa_hesap_id"    , P_MAKBUZ->muh_kasa_hesap_id );
    query.SET_VALUE(":tevkifatli_mi"        , P_MAKBUZ->tevkifatli_mi );
    query.SET_VALUE(":tevkifat_yuzdesi"     , P_MAKBUZ->tevkifat_yuzdesi);
    query.SET_VALUE(":belge_seri"           , P_MAKBUZ->belge_seri );
    query.SET_VALUE(":belge_numarasi"       , P_MAKBUZ->belge_numarasi);
    query.SET_VALUE(":tahsil_edildi_odendi" , P_MAKBUZ->tahsil_edildi_odendi);
    query.SET_VALUE(":kdv_tevkifat_muh_hesap_id", P_MAKBUZ->kdv_tevkifat_muh_hesap_id);
    query.SET_VALUE(":kdv_orani"            , P_MAKBUZ->kdv_orani);
    query.SET_VALUE(":vergi_stopaj_orani"   , P_MAKBUZ->vergi_orani);
    query.SET_VALUE(":kdv_hesap_id"         , P_MAKBUZ->kdv_hesap_id);
    query.SET_VALUE(":hizmet_tutari"        , P_MAKBUZ->hizmet_tutari);
    query.SET_VALUE(":brut_tutar"           , P_MAKBUZ->brut_tutar);
    query.SET_VALUE(":tevkifat_tutari"      , P_MAKBUZ->tevkifat_tutari);
    query.SET_VALUE(":vergi_stopaj_tutari"  , P_MAKBUZ->vergi_stopaj_tutari);
    query.SET_VALUE(":net_tutar"            , P_MAKBUZ->net_tutar);
    query.SET_VALUE(":hizmet_id"            , P_MAKBUZ->hizmet_id);
    query.SET_VALUE(":tah_odm_sekli"        , P_MAKBUZ->tah_odm_sekli);
    query.SET_VALUE(":hesap_no_id"          , P_MAKBUZ->hesap_no_id);
    query.SET_VALUE(":kredi_karti_id"       , P_MAKBUZ->kredi_karti_id);
    query.SET_VALUE(":pos_id"               , P_MAKBUZ->pos_id);
    query.SET_VALUE(":kdv_tutari"           , P_MAKBUZ->kdv_tutari);

    query.SET_VALUE(":makbuz_id"            , p_makbuz_id);

    query.UPDATE();



    SMM_ENT_FISLERI_OLUSTUR(P_MAKBUZ , p_makbuz_id);


    MUH_FIS_STRUCT * MUHASEBE_FISI = new MUH_FIS_STRUCT;

    MUH_CLEAR_FIS_STRUCT(MUHASEBE_FISI);

    MUH_FIS_BILGILERINI_OKU(MUHASEBE_FISI , P_MAKBUZ->muh_fis_id);

    QString aciklama = QString("Fiş SM MAKBUZ modülü tarafından üretilmiştir.Makbuz No : %1 \n").arg(P_MAKBUZ->makbuz_no);
    aciklama.append(P_MAKBUZ->aciklama);

    MUHASEBE_FISI->aciklama         = aciklama;
    MUHASEBE_FISI->fis_tarihi       = P_MAKBUZ->makbuz_tarihi;

    MUH_FISI_GUNCELLE ( MUHASEBE_FISI ,P_MAKBUZ->muh_fis_id);

    delete MUHASEBE_FISI;

}

/**************************************************************************************
                   SM_MAKBUZ_FIS_SIL
***************************************************************************************/

void SMM_MAKBUZ_SIL(SMM_MAKBUZ_STRUCT * P_MAKBUZ, int p_makbuz_id)
{
    SQL_QUERY query(DB);

    query.PREPARE_SELECT("smm_makbuzlar" ,"modul_id,program_id,base_record_id,hesap_no_id",
                         "makbuz_id = :makbuz_id");
    query.SET_VALUE(":makbuz_id"  , p_makbuz_id);

    if ( query.SELECT() EQ 0 ) {
        return;
    }
    query.NEXT();

    int modul_id       = query.VALUE(0).toInt();
    int program_id     = query.VALUE(1).toInt();
    int base_record_id = query.VALUE(2).toInt();
    int hesap_no_id    = query.VALUE(3).toInt();

    if ( P_MAKBUZ->modul_id NE modul_id OR P_MAKBUZ->program_id NE program_id OR
         P_MAKBUZ->base_record_id NE base_record_id ) {
        return;
    }

    SMM_ENT_FISLERI_SIL(P_MAKBUZ , p_makbuz_id , hesap_no_id);


    if ( P_MAKBUZ->muh_fis_id > 0 ) {

        MUH_FIS_STRUCT * MUHASEBE_FISI = new MUH_FIS_STRUCT;

        MUH_CLEAR_FIS_STRUCT(MUHASEBE_FISI);

        MUH_FIS_BILGILERINI_OKU(MUHASEBE_FISI , P_MAKBUZ->muh_fis_id);

        MUH_FIS_SIL(MUHASEBE_FISI , P_MAKBUZ->muh_fis_id );
    }

    query.PREPARE_DELETE("smm_makbuzlar" , "makbuz_id = :makbuz_id");
    query.SET_VALUE(":makbuz_id"  , p_makbuz_id);
    query.DELETE();
}

/**************************************************************************************
                   SM_MAKBUZ_FIS_BILGILERINI_OKU
***************************************************************************************/

void SMM_MAKBUZ_BILGILERINI_OKU(SMM_MAKBUZ_STRUCT *P_MAKBUZ, int p_makbuz_id , ADAK_SQL * P_GELEN_DB)
{
    ADAK_SQL * USE_DB;

    if ( P_GELEN_DB EQ NULL ) {
        USE_DB = DB;
    }
    else {
        USE_DB = P_GELEN_DB;
    }

    SQL_QUERY query(USE_DB);

    query.PREPARE_SELECT("smm_makbuzlar" , "makbuz_no,makbuz_tarihi,aciklama,cari_hesap_id,"
                         "cari_hesap_adresi,vergi_dairesi,vergi_numarasi,sm_sicil_numarasi,"
                         "modul_id,program_id,base_record_id,hizmet_tutari,makbuz_turu,"
                         "muh_kasa_hesap_id,tevkifatli_mi,tevkifat_yuzdesi,"
                         "muh_fis_id,belge_seri,belge_numarasi,"
                         "tahsil_edildi_odendi,brut_tutar,kdv_tutari,"
                         "tevkifat_tutari,kdv_tevkifat_muh_hesap_id,"
                         "kdv_orani,vergi_stopaj_orani,kdv_hesap_id,"
                         "vergi_stopaj_tutari,net_tutar,hizmet_id,tah_odm_sekli,"
                         "hesap_no_id,kredi_karti_id,pos_id",
                         "makbuz_id = :makbuz_id");
    query.SET_VALUE(":makbuz_id" , p_makbuz_id);

    if ( query.SELECT() EQ 0 ) {
        return;
    }

    query.NEXT();

    P_MAKBUZ->makbuz_no                  = query.VALUE("makbuz_no").toInt();
    P_MAKBUZ->makbuz_tarihi              = query.VALUE("makbuz_tarihi").toString();
    P_MAKBUZ->aciklama                   = query.VALUE("aciklama").toString();
    P_MAKBUZ->cari_hesap_id              = query.VALUE("cari_hesap_id").toInt();
    P_MAKBUZ->cari_hesap_adresi          = query.VALUE("cari_hesap_adresi").toString();
    P_MAKBUZ->vergi_dairesi              = query.VALUE("vergi_dairesi").toString();
    P_MAKBUZ->vergi_numarasi             = query.VALUE("vergi_numarasi").toString();
    P_MAKBUZ->sm_sicil_numarasi          = query.VALUE("sm_sicil_numarasi").toString();
    P_MAKBUZ->modul_id                   = query.VALUE("modul_id").toInt();
    P_MAKBUZ->program_id                 = query.VALUE("program_id").toInt();
    P_MAKBUZ->base_record_id             = query.VALUE("base_record_id").toInt();
    P_MAKBUZ->hizmet_tutari              = query.VALUE("hizmet_tutari").toDouble();
    P_MAKBUZ->makbuz_turu                = query.VALUE("makbuz_turu").toInt();
    P_MAKBUZ->muh_kasa_hesap_id          = query.VALUE("muh_kasa_hesap_id").toInt();
    P_MAKBUZ->tevkifatli_mi              = query.VALUE("tevkifatli_mi").toInt();
    P_MAKBUZ->tevkifat_yuzdesi           = query.VALUE("tevkifat_yuzdesi").toDouble();
    P_MAKBUZ->muh_fis_id                 = query.VALUE("muh_fis_id").toInt();
    P_MAKBUZ->belge_seri                 = query.VALUE("belge_seri").toString();
    P_MAKBUZ->belge_numarasi             = query.VALUE("belge_numarasi").toString();
    P_MAKBUZ->tahsil_edildi_odendi       = query.VALUE("tahsil_edildi_odendi").toInt();
    P_MAKBUZ->brut_tutar                 = query.VALUE("brut_tutar").toDouble();
    P_MAKBUZ->kdv_tutari                 = query.VALUE("kdv_tutari").toDouble();
    P_MAKBUZ->tevkifat_tutari            = query.VALUE("tevkifat_tutari").toDouble();
    P_MAKBUZ->kdv_tevkifat_muh_hesap_id  = query.VALUE("kdv_tevkifat_muh_hesap_id").toInt();
    P_MAKBUZ->kdv_orani                  = query.VALUE("kdv_orani").toDouble();
    P_MAKBUZ->vergi_orani                = query.VALUE("vergi_stopaj_orani").toDouble();
    P_MAKBUZ->kdv_hesap_id               = query.VALUE("kdv_hesap_id").toInt();
    P_MAKBUZ->vergi_stopaj_tutari        = query.VALUE("vergi_stopaj_tutari").toDouble();
    P_MAKBUZ->net_tutar                  = query.VALUE("net_tutar").toDouble();
    P_MAKBUZ->hizmet_id                  = query.VALUE("hizmet_id").toInt();
    P_MAKBUZ->tah_odm_sekli              = query.VALUE("tah_odm_sekli").toInt();
    P_MAKBUZ->hesap_no_id                = query.VALUE("hesap_no_id").toInt();
    P_MAKBUZ->kredi_karti_id             = query.VALUE("kredi_karti_id").toInt();
    P_MAKBUZ->pos_id                     = query.VALUE("pos_id").toInt();

}


/**************************************************************************************
                   SMM_CLEAR_MAKBUZ_STRUCT
***************************************************************************************/

void SMM_CLEAR_MAKBUZ_STRUCT(SMM_MAKBUZ_STRUCT *P_MAKBUZ)
{
    P_MAKBUZ->aciklama.clear();
    P_MAKBUZ->cari_hesap_adresi.clear();
    P_MAKBUZ->vergi_dairesi.clear();
    P_MAKBUZ->vergi_numarasi.clear();
    P_MAKBUZ->makbuz_tarihi.clear();
    P_MAKBUZ->sm_sicil_numarasi.clear();
    P_MAKBUZ->belge_numarasi.clear();
    P_MAKBUZ->belge_seri.clear();

    P_MAKBUZ->base_record_id            = 0;
    P_MAKBUZ->cari_hesap_id             = 0;
    P_MAKBUZ->makbuz_no                 = 0;
    P_MAKBUZ->makbuz_turu               = -1;
    P_MAKBUZ->modul_id                  = -1;
    P_MAKBUZ->program_id                = -1;
    P_MAKBUZ->muh_fis_id                = 0;
    P_MAKBUZ->muh_kasa_hesap_id         = 0;
    P_MAKBUZ->tevkifatli_mi             = -1;
    P_MAKBUZ->tevkifat_yuzdesi          = 0;
    P_MAKBUZ->hizmet_tutari             = 0;
    P_MAKBUZ->tahsil_edildi_odendi      = 0;
    P_MAKBUZ->brut_tutar                = 0;
    P_MAKBUZ->kdv_tutari                = 0;
    P_MAKBUZ->tevkifat_tutari           = 0;
    P_MAKBUZ->kdv_tevkifat_muh_hesap_id = 0;
    P_MAKBUZ->kdv_hesap_id              = 0;
    P_MAKBUZ->kdv_orani                 = 0;
    P_MAKBUZ->vergi_orani               = 0;
    P_MAKBUZ->vergi_stopaj_tutari       = 0;
    P_MAKBUZ->net_tutar                 = 0;
    P_MAKBUZ->hizmet_id                 = 0;
    P_MAKBUZ->tah_odm_sekli             = 0;
    P_MAKBUZ->hesap_no_id               = 0;
    P_MAKBUZ->pos_id                    = 0;
    P_MAKBUZ->kredi_karti_id            = 0;
    P_MAKBUZ->kdv_tutari                = 0;

}


/**************************************************************************************
                   SM_MAKBUZ_ODENDI_TAHSIL_EDILDI_ENT_FISLERI_OLUSTUR
***************************************************************************************/

void SMM_ODENDI_TAHSIL_EDILDI_ENT_FISLERI_OLUSTUR(SMM_MAKBUZ_STRUCT *P_MAKBUZ , int p_makbuz_id , ADAK_SQL * P_GELEN_DB)
{
    //SM Makbuz detaylarindaki tahsil edildi / odendi checkbox isaretli ise
    //duruma gore cari hesap banka veya kasayla kapatilir.

    //Kasadan sadece nakit tutar cikmali

    //Cari hesabi kapatmak icin bankadan ve kasadan odenen/tahsil edilen tutar kadar
    //Cari hesap borclandirilir/alacaklandirilir.

    //Satir guncelleme esnasinda banka isleminden nakit isleme gecince hesap no id yi kaybediyorum.
    //Oyuzden banka fisi silinemiyor.


    ADAK_SQL * USE_DB;

    if ( P_GELEN_DB EQ NULL ) {
        USE_DB = DB;
    }
    else {
        USE_DB = P_GELEN_DB;
    }

    double kasa_tutari  = 0.0;
    double banka_tutari = 0.0;

    if ( P_MAKBUZ->tah_odm_sekli EQ ENUM_NAKIT ) {
        kasa_tutari += P_MAKBUZ->hizmet_tutari;
    }
    else {
        banka_tutari += P_MAKBUZ->hizmet_tutari;
    }

    BNK_HAREKET_FIS_STRUCT * BNK_FISI              = new BNK_HAREKET_FIS_STRUCT;

    BNK_HAREKET_FIS_SATIRI_STRUCT * BNK_FIS_SATIRI = new BNK_HAREKET_FIS_SATIRI_STRUCT;

    CARI_FIS_STRUCT * CARI_FISI                    = new CARI_FIS_STRUCT;

    CARI_FIS_SATIRI_STRUCT * CARI_FIS_SATIRI       = new CARI_FIS_SATIRI_STRUCT;



    BNK_CLEAR_HAREKET_FIS_STRUCT(BNK_FISI);

    BNK_CLEAR_HAREKET_FIS_SATIRI_STRUCT(BNK_FIS_SATIRI);

    CARI_CLEAR_FIS_STRUCT(CARI_FISI);

    int cari_fis_id = CARI_GET_ENT_FIS_ID( P_MAKBUZ->program_id , P_MAKBUZ->modul_id , p_makbuz_id ,USE_DB);


    CARI_FIS_BILGILERINI_OKU(CARI_FISI , cari_fis_id , USE_DB);


    if ( ( kasa_tutari EQ 0 AND banka_tutari EQ 0 ) OR P_MAKBUZ->tahsil_edildi_odendi EQ 0 ) {

        return;
    }


    if ( banka_tutari > 0 ) {

        BNK_FISI->base_fis_id      = p_makbuz_id;
        BNK_FISI->program_id       = P_MAKBUZ->program_id;
        BNK_FISI->modul_id         = P_MAKBUZ->modul_id;
        BNK_FISI->hesap_no_id      = P_MAKBUZ->hesap_no_id;

        QString fis_aciklama       = QObject::tr("Fiş SM MAKBUZ MODÜLÜ Tarafından Üretilmiştir.Fiş No : %1\n").arg(P_MAKBUZ->makbuz_no);
        fis_aciklama.append(P_MAKBUZ->aciklama);

        BNK_FISI->aciklama         = fis_aciklama;
        BNK_FISI->fis_tarihi       = P_MAKBUZ->makbuz_tarihi;
        BNK_FISI->fis_turu         = ENUM_BANKA_HAREKET_FISI;
        BNK_FISI->muh_fis_id       = P_MAKBUZ->muh_fis_id;
        BNK_FISI->odm_emr_hesabi_etkilenecek_mi = 0;
        BNK_FISI->para_birim_id    = DVZ_GET_TEMEL_PARA_BIRIMI_ID();

        int bnk_fis_id             = BNK_HAREKET_FIS_EKLE(BNK_FISI ,0,true ,USE_DB);


        BNK_FIS_SATIRI->fis_id           = bnk_fis_id;
        BNK_FIS_SATIRI->program_id       = P_MAKBUZ->program_id;
        BNK_FIS_SATIRI->modul_id         = P_MAKBUZ->modul_id;
        BNK_FIS_SATIRI->base_record_id   = BNK_ENT_SATIRI;
        BNK_FIS_SATIRI->aciklama         = P_MAKBUZ->aciklama;
        BNK_FIS_SATIRI->karsi_hesap_eklenecek_mi = 0;


        switch ( P_MAKBUZ->tah_odm_sekli ) {
        case ENUM_KREDI_KARTI :
            {
                if ( P_MAKBUZ->makbuz_turu EQ SMM_ALIS_MAKBUZU ) {
                    BNK_FIS_SATIRI->etkilenecek_hesap_turu = ENUM_BNK_KREDI_KARTI_HESABI;
                    BNK_FIS_SATIRI->etkilenecek_hesap_id   = P_MAKBUZ->kredi_karti_id;
                }
                else {
                    BNK_FIS_SATIRI->etkilenecek_hesap_turu = ENUM_BNK_POS_HESABI;
                    BNK_FIS_SATIRI->etkilenecek_hesap_id   = P_MAKBUZ->pos_id;

                }
            }
            break;
        case ENUM_EFT    :
        case ENUM_HAVALE :
            {
                BNK_FIS_SATIRI->etkilenecek_hesap_turu = ENUM_BNK_BANKA_HESABI;
                BNK_FIS_SATIRI->etkilenecek_hesap_id   = P_MAKBUZ->hesap_no_id;

            }
            break;
        default:
            break;

        }

        if ( P_MAKBUZ->makbuz_turu EQ SMM_ALIS_MAKBUZU ) {
            BNK_FIS_SATIRI->hesaba_giren   = 0.0;
            BNK_FIS_SATIRI->hesaptan_cikan = ROUND(banka_tutari);

        }
        else {
            BNK_FIS_SATIRI->hesaba_giren   = ROUND(banka_tutari);
            BNK_FIS_SATIRI->hesaptan_cikan = 0.0;
        }


        BNK_HAREKET_FIS_SATIRI_EKLE(BNK_FISI , BNK_FIS_SATIRI , 0 , true ,USE_DB);
    }


    //Nakit islem ise cari kasa ile kapatilacak.

    if ( cari_fis_id > 0 ) {
        //Cari hareket fisine cari hesabi kapatacak satir eklenir.

        CARI_CLEAR_FIS_SATIRI_STRUCT(CARI_FIS_SATIRI);

        CARI_FIS_SATIRI->aciklama                      =  P_MAKBUZ->aciklama;
        CARI_FIS_SATIRI->doviz_kodu                    =  DVZ_GET_TEMEL_PARA_BIRIMI_KODU ();
        CARI_FIS_SATIRI->fis_id                        =  cari_fis_id;
        CARI_FIS_SATIRI->cari_hesap_id                 =  P_MAKBUZ->cari_hesap_id;
        CARI_FIS_SATIRI->islem_turu                    = -1;
        CARI_FIS_SATIRI->doviz_id                      =  DVZ_GET_TEMEL_PARA_BIRIMI_ID();
        CARI_FIS_SATIRI->doviz_kuru                    =  1;
        CARI_FIS_SATIRI->parite                        =  1;
        CARI_FIS_SATIRI->modul_id                      =  P_MAKBUZ->modul_id;
        CARI_FIS_SATIRI->program_id                    =  P_MAKBUZ->program_id;
        CARI_FIS_SATIRI->base_record_id                =  CARI_TAH_ODN_SATIRI;

        if ( kasa_tutari > 0 ) {

            CARI_FIS_SATIRI->muh_hesap_id                  =  P_MAKBUZ->muh_kasa_hesap_id;

            if ( P_MAKBUZ->makbuz_turu EQ SMM_ALIS_MAKBUZU ) {
                CARI_FIS_SATIRI->borc_tutari     = ROUND(kasa_tutari );
            }
            else {
                CARI_FIS_SATIRI->alacak_tutari   = ROUND(kasa_tutari );
            }

        }
        if ( banka_tutari > 0  ) {

            CARI_FIS_SATIRI->muh_hesap_id   = 0;

            if ( P_MAKBUZ->makbuz_turu EQ SMM_ALIS_MAKBUZU ) {
                CARI_FIS_SATIRI->borc_tutari    = ROUND(banka_tutari );
            }
            else {
                CARI_FIS_SATIRI->alacak_tutari  = ROUND(banka_tutari );
            }
        }
        if ( CARI_FIS_SATIRI->borc_tutari > 0 OR
                CARI_FIS_SATIRI->alacak_tutari > 0 ) {

            CARI_FIS_SATIRI_EKLE ( CARI_FISI, CARI_FIS_SATIRI ,true , 0 , NULL, USE_DB);
        }

    }
}


/**************************************************************************************
                   SM_MAKBUZ_ENT_FISLERI_OLUSTUR
***************************************************************************************/

void SMM_ENT_FISLERI_OLUSTUR(SMM_MAKBUZ_STRUCT * P_MAKBUZ, int p_makbuz_id,ADAK_SQL * P_GELEN_DB)
{

    ADAK_SQL * USE_DB;

    if ( P_GELEN_DB EQ NULL ) {
        USE_DB = DB;
    }
    else {
        USE_DB = P_GELEN_DB;
    }

    SQL_QUERY query(USE_DB);

    query.PREPARE_SELECT("smm_hizmetler" , "hizmet_hesap_id,vergi_hesap_id" ,"hizmet_id = :hizmet_id");
    query.SET_VALUE(":hizmet_id" , P_MAKBUZ->hizmet_id);

    if ( query.SELECT() EQ 0 ) {
        return;
    }

    query.NEXT();

    int hizmet_muh_hesap_id = query.VALUE(0).toInt();
    int vergi_muh_hesap_id  = query.VALUE(1).toInt();




    MUH_FIS_STRUCT * MUH_FIS               = new MUH_FIS_STRUCT;
    MUH_FIS_SATIRI_STRUCT * MUH_FIS_SATIRI = new MUH_FIS_SATIRI_STRUCT;

    MUH_CLEAR_FIS_STRUCT(MUH_FIS);

    MUH_FIS_BILGILERINI_OKU(MUH_FIS , P_MAKBUZ->muh_fis_id , USE_DB);



    int base_record_id = P_MAKBUZ->base_record_id;

    if ( P_MAKBUZ->modul_id EQ SMM_MODULU ) {

        if ( base_record_id EQ 0 ) {
            base_record_id = p_makbuz_id;
        }
    }

    CARI_FIS_STRUCT * CARI_FISI              = new CARI_FIS_STRUCT;
    CARI_FIS_SATIRI_STRUCT * CARI_FIS_SATIRI = new CARI_FIS_SATIRI_STRUCT;

    CARI_CLEAR_FIS_STRUCT(CARI_FISI);
    CARI_CLEAR_FIS_SATIRI_STRUCT(CARI_FIS_SATIRI);



    CARI_FISI->program_id            = P_MAKBUZ->program_id;
    CARI_FISI->modul_id              = P_MAKBUZ->modul_id;
    CARI_FISI->fis_tarihi            = P_MAKBUZ->makbuz_tarihi;
    CARI_FISI->aciklama              = QObject::tr("Fiş SM MAKBUZ MODÜLÜ Tarafından Oluşturulmuştur."
                                        "Makbuz No : %1\n").arg(P_MAKBUZ->makbuz_no) + P_MAKBUZ->aciklama;
    CARI_FISI->fis_turu              = ENUM_CARI_HAREKET_FISI;
    CARI_FISI->makbuz_para_birimi_id = DVZ_GET_TEMEL_PARA_BIRIMI_ID();
    CARI_FISI->doviz_kuru            = 1;
    CARI_FISI->parite                = 1;
    CARI_FISI->base_fis_id           = base_record_id;
    CARI_FISI->muh_fis_id            = P_MAKBUZ->muh_fis_id;

    int cari_fis_id = CARI_FIS_EKLE(CARI_FISI , USE_DB);


    bool cari_fis_satiri_eklenecek = false;

    CARI_FIS_SATIRI->aciklama                      =  P_MAKBUZ->aciklama;
    CARI_FIS_SATIRI->doviz_kodu                    =  DVZ_GET_TEMEL_PARA_BIRIMI_KODU ();
    CARI_FIS_SATIRI->fis_id                        =  cari_fis_id;
    CARI_FIS_SATIRI->cari_hesap_id                 =  P_MAKBUZ->cari_hesap_id;
    CARI_FIS_SATIRI->islem_turu                    = -1;
    CARI_FIS_SATIRI->doviz_id                      =  DVZ_GET_TEMEL_PARA_BIRIMI_ID();
    CARI_FIS_SATIRI->doviz_kuru                    =  1;
    CARI_FIS_SATIRI->parite                        =  1;
    CARI_FIS_SATIRI->modul_id                      =  P_MAKBUZ->modul_id;
    CARI_FIS_SATIRI->program_id                    =  P_MAKBUZ->program_id;
    CARI_FIS_SATIRI->base_record_id                =  CARI_ENT_SATIRI;

    if ( P_MAKBUZ->makbuz_turu EQ SMM_ALIS_MAKBUZU ) {
        CARI_FIS_SATIRI->alacak_tutari = P_MAKBUZ->hizmet_tutari - P_MAKBUZ->tevkifat_tutari;
        cari_fis_satiri_eklenecek = true;
    }
    else {
        CARI_FIS_SATIRI->borc_tutari = P_MAKBUZ->hizmet_tutari - P_MAKBUZ->tevkifat_tutari;
        CARI_FIS_SATIRI_EKLE ( CARI_FISI, CARI_FIS_SATIRI , true , 0 , NULL, USE_DB);
    }


    MUH_CLEAR_FIS_SATIRI_STRUCT(MUH_FIS_SATIRI);

    MUH_FIS_SATIRI->fis_id         = P_MAKBUZ->muh_fis_id;
    MUH_FIS_SATIRI->program_id     = P_MAKBUZ->program_id;
    MUH_FIS_SATIRI->modul_id       = P_MAKBUZ->modul_id;
    MUH_FIS_SATIRI->base_record_id = base_record_id;
    MUH_FIS_SATIRI->aciklama       = P_MAKBUZ->aciklama;

    if ( P_MAKBUZ->makbuz_turu EQ SMM_SATIS_MAKBUZU ) {

        if ( P_MAKBUZ->vergi_stopaj_tutari > 0 ) {
            if ( vergi_muh_hesap_id > 0 ) {
                MUH_FIS_SATIRI->hesap_id      = vergi_muh_hesap_id;
                MUH_FIS_SATIRI->borc_tutari   = P_MAKBUZ->vergi_stopaj_tutari;
                MUH_FIS_SATIRI->alacak_tutari = 0;
                MUH_FIS_SATIRI->base_record_id= base_record_id;

                MUH_FIS_SATIRI_EKLE(MUH_FIS , MUH_FIS_SATIRI , USE_DB);
            }
        }

        if ( P_MAKBUZ->tevkifat_tutari > 0 ) {

            MUH_FIS_SATIRI->hesap_id      = P_MAKBUZ->kdv_tevkifat_muh_hesap_id;
            MUH_FIS_SATIRI->borc_tutari   = P_MAKBUZ->tevkifat_tutari;
            MUH_FIS_SATIRI->alacak_tutari = 0;
            MUH_FIS_SATIRI->base_record_id = MUH_ENT_SATIRI;

            MUH_FIS_SATIRI_EKLE(MUH_FIS , MUH_FIS_SATIRI , USE_DB);

        }
        if ( hizmet_muh_hesap_id > 0 ) {
            MUH_FIS_SATIRI->hesap_id       =  hizmet_muh_hesap_id;
            MUH_FIS_SATIRI->base_record_id =  base_record_id;
            MUH_FIS_SATIRI->borc_tutari    = 0;
            MUH_FIS_SATIRI->alacak_tutari  = P_MAKBUZ->brut_tutar;

            MUH_FIS_SATIRI_EKLE ( MUH_FIS , MUH_FIS_SATIRI , USE_DB);
        }
        if ( P_MAKBUZ->kdv_tutari > 0 ) {
            MUH_FIS_SATIRI->base_record_id= MUH_ENT_SATIRI;
            MUH_FIS_SATIRI->hesap_id      = P_MAKBUZ->kdv_hesap_id;
            MUH_FIS_SATIRI->borc_tutari   = 0;
            MUH_FIS_SATIRI->alacak_tutari = P_MAKBUZ->kdv_tutari;

            MUH_FIS_SATIRI_EKLE(MUH_FIS , MUH_FIS_SATIRI , USE_DB);
        }
    }


    if ( P_MAKBUZ->makbuz_turu EQ SMM_ALIS_MAKBUZU ) {

        if ( hizmet_muh_hesap_id > 0 ) {
            MUH_FIS_SATIRI->hesap_id       =  hizmet_muh_hesap_id;
            MUH_FIS_SATIRI->base_record_id =  base_record_id;
            MUH_FIS_SATIRI->borc_tutari    =  P_MAKBUZ->brut_tutar;
            MUH_FIS_SATIRI->alacak_tutari  =  0;

            MUH_FIS_SATIRI_EKLE(MUH_FIS , MUH_FIS_SATIRI , USE_DB);
        }


        if ( P_MAKBUZ->kdv_tutari > 0 ) {

            MUH_FIS_SATIRI->base_record_id = MUH_ENT_SATIRI;
            MUH_FIS_SATIRI->hesap_id       = P_MAKBUZ->kdv_hesap_id;
            MUH_FIS_SATIRI->borc_tutari    = P_MAKBUZ->kdv_tutari;
            MUH_FIS_SATIRI->alacak_tutari  = 0;

            MUH_FIS_SATIRI_EKLE(MUH_FIS , MUH_FIS_SATIRI , USE_DB);
        }
        if ( P_MAKBUZ->vergi_stopaj_tutari > 0 ) {
            if ( vergi_muh_hesap_id > 0 ) {
                MUH_FIS_SATIRI->hesap_id      = vergi_muh_hesap_id;
                MUH_FIS_SATIRI->borc_tutari   = 0;
                MUH_FIS_SATIRI->alacak_tutari = P_MAKBUZ->vergi_stopaj_tutari;
                MUH_FIS_SATIRI->base_record_id= base_record_id;

                MUH_FIS_SATIRI_EKLE(MUH_FIS , MUH_FIS_SATIRI , USE_DB);
            }
        }
        if ( P_MAKBUZ->tevkifat_tutari > 0 ) {

            MUH_FIS_SATIRI->hesap_id      = P_MAKBUZ->kdv_tevkifat_muh_hesap_id;
            MUH_FIS_SATIRI->borc_tutari   = 0;
            MUH_FIS_SATIRI->alacak_tutari = P_MAKBUZ->tevkifat_tutari;
            MUH_FIS_SATIRI->base_record_id= MUH_ENT_SATIRI;

            MUH_FIS_SATIRI_EKLE(MUH_FIS , MUH_FIS_SATIRI , USE_DB);

        }
        if ( cari_fis_satiri_eklenecek EQ true ) {
            CARI_FIS_SATIRI_EKLE(CARI_FISI , CARI_FIS_SATIRI , USE_DB);
        }

    }


    if ( P_MAKBUZ->tahsil_edildi_odendi EQ 1 ) {
        SMM_ODENDI_TAHSIL_EDILDI_ENT_FISLERI_OLUSTUR(P_MAKBUZ , p_makbuz_id , USE_DB);
    }

}

/**************************************************************************************
                   SMM_ENT_FISLERI_SIL
***************************************************************************************/

void SMM_ENT_FISLERI_SIL(SMM_MAKBUZ_STRUCT *P_MAKBUZ, int p_makbuz_id,int p_hesap_no_id)
{

    int base_record_id = P_MAKBUZ->base_record_id;

    if ( P_MAKBUZ->modul_id EQ SMM_MODULU ) {
        if ( base_record_id EQ 0 ) {
            base_record_id = p_makbuz_id;
        }
    }


    int cari_fis_id = CARI_GET_ENT_FIS_ID(P_MAKBUZ->program_id , P_MAKBUZ->modul_id,
                                          base_record_id);

    if ( cari_fis_id > 0 ) {

        CARI_FIS_STRUCT * CARI_FISI = new CARI_FIS_STRUCT;

        CARI_CLEAR_FIS_STRUCT(CARI_FISI);

        CARI_FIS_BILGILERINI_OKU(CARI_FISI , cari_fis_id);

        CARI_FISI_SIL(CARI_FISI , cari_fis_id);
    }


    int bnk_fis_id = BNK_GET_ENT_FIS_ID(P_MAKBUZ->program_id , P_MAKBUZ->modul_id , base_record_id,
                                         p_hesap_no_id) ;

    if ( bnk_fis_id > 0 ) {

        BNK_HAREKET_FIS_STRUCT * BNK_FISI = new BNK_HAREKET_FIS_STRUCT;

        BNK_CLEAR_HAREKET_FIS_STRUCT(BNK_FISI);

        BNK_HAREKET_FIS_BILGILERINI_OKU(BNK_FISI , bnk_fis_id);

        BNK_HAREKET_FISI_SIL(BNK_FISI , bnk_fis_id);

        delete BNK_FISI;
    }

    MUH_FIS_STRUCT * MUH_FISI = new MUH_FIS_STRUCT;

    MUH_CLEAR_FIS_STRUCT(MUH_FISI);

    MUH_FIS_BILGILERINI_OKU(MUH_FISI , P_MAKBUZ->muh_fis_id );

    MUH_FIS_TUM_SATIRLARINI_SIL(MUH_FISI , P_MAKBUZ->muh_fis_id);
}




















