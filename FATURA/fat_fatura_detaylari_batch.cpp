#include "fat_fatura_detaylari_batch_class.h"
#include "ui_fat_fatura_detaylari_batch.h"
#include "sube_struct.h"
#include "muh_console_utils.h"
#include "fat_enum.h"
#include "fat_struct.h"
#include "fat_console_utils.h"
#include "e9_enum.h"
#include "dvz_enum.h"
#include "dvz_console_utils.h"
#include "e9_console_utils.h"
#include "muh_hesap_arama_open.h"
#include "muh_enum.h"
#include "muh_struct.h"
#include "sube_console_utils.h"
#include "sube_struct.h"
#include "e9_gui_utils.h"

extern ADAK_SQL *           DB;

/**************************************************************************************
                   OPEN_FAT_FATURA_DETAYLARI_BATCH
***************************************************************************************/

bool OPEN_FAT_FATURA_DETAYLARI_BATCH ( FATURA_DETAYLARI_STRUCT * P_FATURA_DETAYLARI, int p_fis_id, int p_fatura_alis_satis_iade_turu, QWidget * p_parent, int p_iade_faturasi_mi )
{
    bool return_value = false;
    FAT_FATURA_DETAYLARI_BATCH * F = new FAT_FATURA_DETAYLARI_BATCH ( P_FATURA_DETAYLARI, p_fis_id, p_fatura_alis_satis_iade_turu, p_parent, p_iade_faturasi_mi  );
    F->m_return_value = &return_value;
    F->EXEC( NOT_FULL_SCREEN );
    return return_value;
}

/**************************************************************************************
                   FAT_FATURA_DETAYLARI_BATCH::FAT_FATURA_DETAYLARI_BATCH
***************************************************************************************/

FAT_FATURA_DETAYLARI_BATCH::FAT_FATURA_DETAYLARI_BATCH ( FATURA_DETAYLARI_STRUCT * P_FATURA_DETAYLARI, int p_fis_id, int p_fatura_alis_satis_iade_turu,  QWidget * parent , int p_iade_faturasi_mi ) :  BATCH_KERNEL ( parent ), m_ui ( new Ui::FAT_FATURA_DETAYLARI_BATCH )
{
    m_fis_id                        = p_fis_id;
    m_fatura_alis_satis_iade_turu   = p_fatura_alis_satis_iade_turu;
    m_iade_faturasi_mi              = p_iade_faturasi_mi;

    M_FATURA_DETAYLARI              = P_FATURA_DETAYLARI;

    m_ui->setupUi( this );
    START_BATCH_KERNEL(this, DB );
}

/**************************************************************************************
                   FAT_FATURA_DETAYLARI_BATCH::SETUP_FORM
***************************************************************************************/

void FAT_FATURA_DETAYLARI_BATCH::SETUP_FORM()
{

    SET_PAGE_TITLE( tr ( "FAT - FATURA ENTEGRASYON HESAPLARI" ) );
    SET_SETTING_NAME( "FATURA_ENT_HESAPLARI_BATCH" );
    SET_HELP_PAGE( "fatura-entegrasyon-hesaplari" );

    REGISTER_BUTTON_WIDGET          ( m_ui->widget_batch_buttons );

    m_ui->commaEdit_cari_fis_no->VIRGULSUZ_CALIS  ( true  );
    m_ui->commaEdit_cari_fis_no->SET_PRECISION    ( 0     );
    m_ui->commaEdit_cari_fis_no->SET_RETURN_EMPTY ( true  );

    m_ui->commaEdit_muh_fis_no->VIRGULSUZ_CALIS   ( true  );
    m_ui->commaEdit_muh_fis_no->SET_PRECISION     ( 0     );
    m_ui->commaEdit_muh_fis_no->SET_RETURN_EMPTY  ( true  );

    SUBE_ENTEGRASYON_HESAPLARI_STRUCT * SB_ENT_HESAPLARI = ( SUBE_ENTEGRASYON_HESAPLARI_STRUCT * ) SUBE_GET_ENTEGRASYON_HESAPLARI_STRUCT();

    m_ui->commaEdit_tevkifat_yuzdesi->VIRGULSUZ_CALIS ( true );

    if ( m_fatura_alis_satis_iade_turu EQ ENUM_ALIS_IRSALIYESI_FATURASI ) {
        m_ui->label_yurtici_satislar->setHidden(true);
        m_ui->searchEdit_sube_yurtici_satis_hsp_id->setHidden(true);
        m_ui->lineEdit_sube_yurtici_satis_hsp_id->setHidden(true);
    }

    if ( M_FATURA_DETAYLARI->yurtici_satislar_hesap_id EQ 0 ) {
        m_sube_yurtici_satis_hsp_id = SB_ENT_HESAPLARI->yurtici_sts_hsp_id;
    }
    else {
        m_sube_yurtici_satis_hsp_id = M_FATURA_DETAYLARI->yurtici_satislar_hesap_id;
    }

    if ( M_FATURA_DETAYLARI->hizmet_urt_maliyeti_hesap_id EQ 0 ) {
        m_sube_hizmet_urt_maliyet_hsp_id = SB_ENT_HESAPLARI->hizmet_uretim_maliyeti_hsp_id;
    }
    else {
        m_sube_hizmet_urt_maliyet_hsp_id = M_FATURA_DETAYLARI->hizmet_urt_maliyeti_hesap_id;
    }

    if ( M_FATURA_DETAYLARI->satistan_iade_hesap_id EQ 0 ) {
        m_satistan_iade_hesap_id = SB_ENT_HESAPLARI->satistan_iade_hesap_id;
    }
    else {
        m_satistan_iade_hesap_id = M_FATURA_DETAYLARI->satistan_iade_hesap_id;
    }

    m_ui->checkBox_tevkifatli_fatura_mi->setChecked ( QVariant ( M_FATURA_DETAYLARI->tevkifatli_fatura_mi ).toBool() );
    m_ui->frame_tevkifat->setEnabled ( m_ui->checkBox_tevkifatli_fatura_mi->isChecked() );

    if ( m_ui->checkBox_tevkifatli_fatura_mi->isChecked() EQ true ) {
        m_ui->commaEdit_tevkifat_yuzdesi->SET_DOUBLE ( M_FATURA_DETAYLARI->tevkifat_yuzdesi );
    }

    m_ui->checkBox_kdv_muaf_mi->setChecked(QVariant(M_FATURA_DETAYLARI->kdv_muaf_mi).toBool());
    m_ui->checkBox_otv_muaf_mi->setChecked(QVariant(M_FATURA_DETAYLARI->otv_muaf_mi).toBool());

    QString tam_hesap_kodu,hesap_ismi;

    if ( m_sube_yurtici_satis_hsp_id > 0 ) {

        MUH_GET_HESAP_KODU_HESAP_ISMI(m_sube_yurtici_satis_hsp_id , tam_hesap_kodu,hesap_ismi);
        m_ui->searchEdit_sube_yurtici_satis_hsp_id->SET_TEXT ( tam_hesap_kodu );
        m_ui->lineEdit_sube_yurtici_satis_hsp_id->setText    ( hesap_ismi );
    }

    if ( m_sube_hizmet_urt_maliyet_hsp_id > 0 ) {

        MUH_GET_HESAP_KODU_HESAP_ISMI(m_sube_hizmet_urt_maliyet_hsp_id , tam_hesap_kodu,hesap_ismi);
        m_ui->searchEdit_sube_hizmet_urt_maliyet_hsp_id->SET_TEXT ( tam_hesap_kodu );
        m_ui->lineEdit_sube_hizmet_urt_maliyet_hsp_id->setText    ( hesap_ismi );
    }

    if ( m_satistan_iade_hesap_id > 0 ) {

        MUH_GET_HESAP_KODU_HESAP_ISMI(m_satistan_iade_hesap_id , tam_hesap_kodu,hesap_ismi);
        m_ui->searchEdit_satistan_iade_hesap_id->SET_TEXT ( tam_hesap_kodu );
        m_ui->lineEdit_satistan_iade_hesap_id->setText    ( hesap_ismi );
    }

    if ( M_FATURA_DETAYLARI->kdv_muaf_mi EQ 1 ) {
        m_ui->checkBox_kdv_muaf_mi->setChecked(true);
    }
    if ( M_FATURA_DETAYLARI->otv_muaf_mi EQ 1 ){
        m_ui->checkBox_otv_muaf_mi->setChecked(true);
    }

    m_ui->commaEdit_muh_fis_no->SET_INTEGER  ( M_FATURA_DETAYLARI->muh_fis_no  );
    m_ui->commaEdit_cari_fis_no->SET_INTEGER ( M_FATURA_DETAYLARI->cari_fis_no );

    SET_NAME_OF_RUN_BATCH_BUTTON    ( tr ( "Detayları Kaydet" )  );
    SET_FIRST_FOCUS_WIDGET          ( m_ui->searchEdit_sube_yurtici_satis_hsp_id );
    SET_ENTER_KEY_FOR_RUN_BATCH     ( true );
}


/**************************************************************************************
                   FAT_FATURA_DETAYLARI_BATCH::MUHASEBE_HESAP_KONTROL
***************************************************************************************/

int FAT_FATURA_DETAYLARI_BATCH::MUHASEBE_HESAP_KONTROL( QSearchEdit *p_search_edit, QLineEdit *p_line_edit, int * p_hesap_degiskeni )
{
    if ( p_search_edit->GET_TEXT().isEmpty() EQ true ) {
        p_line_edit->clear();
        p_hesap_degiskeni = 0;
        return ADAK_OK;
    }
    MUH_HESAP_STRUCT * MUHASEBE_HESABI = new MUH_HESAP_STRUCT;
    MUH_CLEAR_HESAP_STRUCT(MUHASEBE_HESABI);
    if ( MUH_HESAPLAR_TABLOSUNU_OKU(MUHASEBE_HESABI , p_search_edit->GET_TEXT()) EQ 0 ) {
        MSG_WARNING(  tr ( "Muhasebe hesabı bulunamadı." ), p_search_edit );
        return ADAK_FAIL;
    }
    if ( MUHASEBE_HESABI->tali_hesap EQ 0 ) {
        MSG_WARNING(  tr ( "Seçilen hesap tali hesap değil. Entegrasyon yapılamaz."), NULL);
        return ADAK_FAIL;
    }   \
    p_line_edit->setText ( MUHASEBE_HESABI->hesap_ismi );
    *p_hesap_degiskeni = MUHASEBE_HESABI->hesap_id;
    return ADAK_OK;
}

/**************************************************************************************
                   FAT_FATURA_DETAYLARI_BATCH::CHECK_VAR
***************************************************************************************/

int FAT_FATURA_DETAYLARI_BATCH::CHECK_VAR ( QObject * p_object )
{
    SQL_QUERY query ( DB );

    if ( p_object EQ m_ui->searchEdit_sube_yurtici_satis_hsp_id )  {
        MUHASEBE_HESAP_KONTROL( m_ui->searchEdit_sube_yurtici_satis_hsp_id, m_ui->lineEdit_sube_yurtici_satis_hsp_id, &m_sube_yurtici_satis_hsp_id );
    }
    else if ( p_object EQ m_ui->searchEdit_sube_hizmet_urt_maliyet_hsp_id )  {
        MUHASEBE_HESAP_KONTROL( m_ui->searchEdit_sube_hizmet_urt_maliyet_hsp_id, m_ui->lineEdit_sube_hizmet_urt_maliyet_hsp_id, &m_sube_hizmet_urt_maliyet_hsp_id );
    }
    else if ( p_object EQ m_ui->searchEdit_satistan_iade_hesap_id)  {
        MUHASEBE_HESAP_KONTROL( m_ui->searchEdit_satistan_iade_hesap_id, m_ui->lineEdit_satistan_iade_hesap_id, &m_satistan_iade_hesap_id );
    }
    else if ( p_object EQ m_ui->checkBox_tevkifatli_fatura_mi ) {
        if ( m_ui->checkBox_tevkifatli_fatura_mi->isChecked() EQ true ) {
            m_ui->frame_tevkifat->setEnabled ( true );
            m_ui->commaEdit_tevkifat_yuzdesi->SET_DOUBLE ( 50 );
        }
        else {
            m_ui->frame_tevkifat->setEnabled ( false );
            m_ui->commaEdit_tevkifat_yuzdesi->clear();
        }
    }
    else if ( p_object EQ m_ui->commaEdit_tevkifat_yuzdesi ) {
        if ( m_ui->commaEdit_tevkifat_yuzdesi->GET_DOUBLE() > 100 ) {
            MSG_WARNING(  tr("Tevkifat yüzdesi 100'den büyük olamaz"), m_ui->commaEdit_tevkifat_yuzdesi );

            return ADAK_FAIL_UNDO;
        }
    }
    return ADAK_OK;
}

/**************************************************************************************
                   FAT_FATURA_DETAYLARI_BATCH::CHECK_RUN
***************************************************************************************/

int FAT_FATURA_DETAYLARI_BATCH::CHECK_RUN ()
{
    return ADAK_OK;
}

/**************************************************************************************
                   FAT_FATURA_DETAYLARI_BATCH::RUN_BATCH
***************************************************************************************/

void FAT_FATURA_DETAYLARI_BATCH::RUN_BATCH ()
{
    int  new_tevkifatli_fatura_mi = QVariant ( m_ui->checkBox_tevkifatli_fatura_mi->isChecked() ).toInt();

    M_FATURA_DETAYLARI->yurtici_satislar_hesap_id     = m_sube_yurtici_satis_hsp_id;
    M_FATURA_DETAYLARI->hizmet_urt_maliyeti_hesap_id  = m_sube_hizmet_urt_maliyet_hsp_id;
    M_FATURA_DETAYLARI->tevkifatli_fatura_mi          = new_tevkifatli_fatura_mi;
    M_FATURA_DETAYLARI->tevkifat_yuzdesi              = m_ui->commaEdit_tevkifat_yuzdesi->GET_DOUBLE();
    M_FATURA_DETAYLARI->kdv_muaf_mi                   = QVariant ( m_ui->checkBox_kdv_muaf_mi->isChecked()).toInt();
    M_FATURA_DETAYLARI->otv_muaf_mi                   = QVariant ( m_ui->checkBox_otv_muaf_mi->isChecked()).toInt();
    M_FATURA_DETAYLARI->satistan_iade_hesap_id        = m_satistan_iade_hesap_id;

    bool tevkifatli_fatura_secenegi_degisti = false;
    bool hic_degisiklik_yapilmadi           = true;
    bool tevkifat_yuzdesi_degisti           = false;


    SQL_QUERY query ( DB );

    query.PREPARE_SELECT("fat_faturalar" , "isk_sonrasi_kdv_tutari, fatura_tutari, "
                         "tevkifatli_fatura_mi, tevkifat_yuzdesi, tevkifatli_kdv_tutari, "
                         "yurtici_satislar_hesap_id, hizmet_urt_maliyeti_hesap_id, "
                         "kdv_muaf_mi,otv_muaf_mi ,fatura_tarihi, "
                         "iskonto_yuzdesi, satistan_iade_hesap_id " , "fis_id = :fis_id");

    query.SET_VALUE ( ":fis_id", m_fis_id );

    if ( query.SELECT() EQ 0 ) {
        MSG_WARNING(  tr ( "Fatura bilgileri okunamadı"), NULL );
        return;
    }

    query.NEXT();

    double  kdv_tutari                              = query.VALUE ( "isk_sonrasi_kdv_tutari" ).toDouble();
    double  fatura_tutari                           = query.VALUE ( "fatura_tutari" ).toDouble();

    int     old_tevkifatli_fatura_mi                = query.VALUE ( "tevkifatli_fatura_mi" ).toInt();
    double  old_tevkifat_yuzdesi                    = query.VALUE ( "tevkifat_yuzdesi" ).toDouble();
    double  old_tevkifatli_kdv_tutari               = query.VALUE ( "tevkifatli_kdv_tutari" ).toDouble();

    int     old_yurtici_satislar_hesap_id           = query.VALUE ( "yurtici_satislar_hesap_id" ).toInt();
    int     old_hizmet_urt_maliyeti_hesap_id        = query.VALUE ( "hizmet_urt_maliyeti_hesap_id" ).toInt();
    int     old_satistan_iade_hesap_id              = query.VALUE ( "satistan_iade_hesap_id" ).toInt();
    int     old_kdv_muaf_mi                         = query.VALUE ( "kdv_muaf_mi" ).toInt();
    int     old_otv_muaf_mi                         = query.VALUE ( "otv_muaf_mi").toInt();

    QDate   fatura_tarihi                           = QDate::fromString(query.VALUE("fatura_tarihi").toString() , "yyyy.MM.dd");

    double  iskonto_yuzdesi                         = query.VALUE("iskonto_yuzdesi").toDouble();

    if ( old_tevkifatli_fatura_mi NE new_tevkifatli_fatura_mi ) {
        tevkifatli_fatura_secenegi_degisti  = true;
        hic_degisiklik_yapilmadi            = false;
    }

    if ( old_yurtici_satislar_hesap_id NE m_sube_yurtici_satis_hsp_id ) {
        hic_degisiklik_yapilmadi = false;
    }

    if ( old_hizmet_urt_maliyeti_hesap_id NE m_sube_hizmet_urt_maliyet_hsp_id ) {
        hic_degisiklik_yapilmadi = false;
    }

    if ( old_satistan_iade_hesap_id NE m_satistan_iade_hesap_id ) {
        hic_degisiklik_yapilmadi = false;
    }

    if ( old_tevkifat_yuzdesi NE m_ui->commaEdit_tevkifat_yuzdesi->GET_DOUBLE() ) {
        hic_degisiklik_yapilmadi = false;
        tevkifat_yuzdesi_degisti = true;
    }

    bool kdv_otv_muaf_mi_secenegi_degisti = false;

    if ( old_kdv_muaf_mi NE QVariant(m_ui->checkBox_kdv_muaf_mi->isChecked()).toInt() OR
         old_otv_muaf_mi NE QVariant(m_ui->checkBox_otv_muaf_mi->isChecked()).toInt() )  {
        hic_degisiklik_yapilmadi = false;
        kdv_otv_muaf_mi_secenegi_degisti = true;
    }

    if ( hic_degisiklik_yapilmadi EQ true ) {
        *m_return_value = false;
        close ();
    }

    DB->START_TRANSACTION();


    if ( kdv_otv_muaf_mi_secenegi_degisti EQ true ) {
        //vergiden muaf ise kdv ve otv oranlari tum satirlarda sifirlanir.
        SQL_QUERY update_query(DB);

        double brut_tutar                 = 0.0;
        double isk_oncesi_otv_dahil_tutar = 0.0;
        double isk_oncesi_kdv_dahil_tutar = 0.0;


        query.PREPARE_SELECT("fat_fatura_satirlari", "satirdaki_urun_miktari,kdv_dahil_mi,doviz_id,fis_satiri_id,"
                             "isk_sonrasi_urun_birim_fiyati,kdv_orani,otv_orani,"
                             "satir_kdv_tutari,satir_otv_tutari", "fis_id = :fis_id");

        query.SET_VALUE(":fis_id" , m_fis_id );

        if ( query.SELECT() NE 0 ) {

            while ( query.NEXT() ) {

                double satirdaki_urun_miktari        = query.VALUE(0).toDouble();
                int    kdv_dahil_mi                  = query.VALUE(1).toInt();
                int    doviz_id                      = query.VALUE(2).toInt();
                int    fis_satiri_id                 = query.VALUE(3).toInt();
                double isk_sonrasi_urun_birim_fiyati = query.VALUE(4).toDouble();
                double kdv_orani                     = query.VALUE("kdv_orani").toDouble();
                double otv_orani                     = query.VALUE("otv_orani").toDouble();
                double satir_kdv_tutari              = query.VALUE("satir_kdv_tutari").toDouble();
                double satir_otv_tutari              = query.VALUE("satir_otv_tutari").toDouble();

                double satir_tutari                  = satirdaki_urun_miktari * isk_sonrasi_urun_birim_fiyati;

                if ( m_ui->checkBox_kdv_muaf_mi->isChecked() EQ true ) {
                    kdv_orani        = 0;
                    satir_kdv_tutari = 0;
                    kdv_tutari       = 0;
                }
                if ( m_ui->checkBox_otv_muaf_mi->isChecked() EQ true ) {
                    otv_orani        = 0;
                    satir_otv_tutari = 0;
                }

                double otv_dahil_tutar   = 0.0;
                double vergi_haric_tutar = 0.0;

                if ( kdv_dahil_mi EQ 1 ) {
                    otv_dahil_tutar   = ROUND ( satir_tutari / ( 1 + ( kdv_orani / 100 ) )    );
                    vergi_haric_tutar = ROUND ( otv_dahil_tutar / ( 1 + ( otv_orani / 100 ) ) );
                }
                else {
                    vergi_haric_tutar = satir_tutari;
                    otv_dahil_tutar = ROUND ( vergi_haric_tutar * ( 1 + ( otv_orani / 100 ) ) );
                }



                double temel_para_birimi_vergi_haric_tutar = DVZ_PARA_BIRIMLERINI_DONUSTUR ( doviz_id,
                                                                                         vergi_haric_tutar,
                                                                                         DVZ_GET_TEMEL_PARA_BIRIMI_ID(),
                                                                                         fatura_tarihi);

                double temel_para_birimi_otv_dahil_tutar = ROUND ( temel_para_birimi_vergi_haric_tutar * ( 1 + ( otv_orani / 100 ) ) );

                double temel_para_birimi_kdv_tutari      = ROUND ( temel_para_birimi_otv_dahil_tutar * ( kdv_orani / 100 ) );

                double temel_para_birimi_kdv_dahil_tutar = ROUND ( temel_para_birimi_otv_dahil_tutar + temel_para_birimi_kdv_tutari );

                update_query.PREPARE_UPDATE("fat_fatura_satirlari","fis_satiri_id", "kdv_orani,"
                                     "otv_orani,satir_kdv_tutari , satir_kdv_dahil_tutar ,"
                                     "satir_vergi_haric_tutar , satir_otv_tutari  , satir_otv_dahil_tutar,"
                                     "satir_tutari ",
                                     "fis_satiri_id = :fis_satiri_id");

                update_query.SET_VALUE(":kdv_orani"               , kdv_orani);
                update_query.SET_VALUE(":otv_orani"               , otv_orani);
                update_query.SET_VALUE(":satir_kdv_tutari"        , satir_kdv_tutari);
                update_query.SET_VALUE(":satir_kdv_dahil_tutar"   , ROUND ( temel_para_birimi_kdv_dahil_tutar));
                update_query.SET_VALUE(":satir_vergi_haric_tutar" , ROUND ( temel_para_birimi_vergi_haric_tutar ));
                update_query.SET_VALUE(":satir_otv_tutari"        , satir_otv_tutari);
                update_query.SET_VALUE(":satir_otv_dahil_tutar"   , ROUND ( temel_para_birimi_otv_dahil_tutar ));
                update_query.SET_VALUE(":satir_tutari"            , ROUND ( satir_tutari));
                update_query.SET_VALUE(":fis_satiri_id"           , fis_satiri_id);

                update_query.UPDATE();

                brut_tutar                 += temel_para_birimi_vergi_haric_tutar;
                isk_oncesi_otv_dahil_tutar += temel_para_birimi_otv_dahil_tutar;
                isk_oncesi_kdv_dahil_tutar += temel_para_birimi_kdv_dahil_tutar;
            }

            //Satirlara gore fatura toplamlari ayarlanacak

            double  isk_sonrasi_otv_dahil_tutar     = ROUND ( isk_oncesi_otv_dahil_tutar    * ( 1 - ( iskonto_yuzdesi / 100 ) ) );
            double  ara_toplam                      = ROUND ( brut_tutar * ( 1 - ( iskonto_yuzdesi / 100 ) ) );

            fatura_tutari = ROUND ( isk_oncesi_kdv_dahil_tutar    * ( 1 - ( iskonto_yuzdesi / 100 ) ) );


            double KDV_TUTARLAR_ARRAY[KDV_OTV_ORANLARI_SAYISI];
            double OTV_TUTARLAR_ARRAY[KDV_OTV_ORANLARI_SAYISI];

            for ( int i = 0 ; i < KDV_OTV_ORANLARI_SAYISI ; i++ ) {
                KDV_TUTARLAR_ARRAY[i] = 0;
            }

            for ( int i = 0 ; i < KDV_OTV_ORANLARI_SAYISI ; i++ ) {
                OTV_TUTARLAR_ARRAY[i] = 0;
            }

            query.PREPARE_SELECT("fat_faturalar" ,"isk_oncesi_kdv_tutari,isk_oncesi_otv_tutari,"
                                 "isk_sonrasi_otv_tutari,isk_sonrasi_kdv_tutari" ,"fis_id = :fis_id");
            query.SET_VALUE(":fis_id" , m_fis_id);

            double isk_oncesi_kdv_tutari  = 0.0;
            double isk_oncesi_otv_tutari  = 0.0;
            double isk_sonrasi_otv_tutari = 0.0;
            double isk_sonrasi_kdv_tutari = 0.0;

            if ( query.SELECT() NE 0 ) {
                query.NEXT();

                isk_oncesi_kdv_tutari = query.VALUE(0).toDouble();
                isk_oncesi_otv_tutari = query.VALUE(1).toDouble();
                isk_sonrasi_otv_tutari= query.VALUE(2).toDouble();
                isk_sonrasi_kdv_tutari= query.VALUE(3).toDouble();
            }

            if ( m_ui->checkBox_kdv_muaf_mi->isChecked() EQ true ) {
                isk_oncesi_kdv_tutari = 0;
                isk_sonrasi_kdv_tutari= 0;
            }
            if ( m_ui->checkBox_otv_muaf_mi->isChecked() EQ true ) {
                isk_oncesi_otv_tutari = 0;
                isk_sonrasi_otv_tutari= 0;
            }

            query.PREPARE_UPDATE("fat_faturalar","fis_id", "isk_oncesi_kdv_tutari,"
                                 "isk_oncesi_otv_tutari ,brut_tutar ,"
                                 "isk_oncesi_kdv_dahil_tutar ,"
                                 "isk_oncesi_otv_dahil_tutar , "
                                 "isk_sonrasi_otv_tutari, isk_sonrasi_kdv_tutari,"
                                 "isk_sonrasi_otv_dahil_tutar , "
                                 "fatura_tutari , ara_toplam ,"
                                 "kdv_oran_bilgileri  , "
                                 "otv_oran_bilgileri  , "
                                 "kdv_oran_gore_tutarlar_array ,"
                                 "otv_oran_gore_tutarlar_array ",
                                 "fis_id = :fis_id");

            query.SET_VALUE(":isk_oncesi_kdv_tutari"        , isk_oncesi_kdv_tutari);
            query.SET_VALUE(":isk_oncesi_otv_tutari"        , isk_oncesi_otv_tutari);
            query.SET_VALUE(":brut_tutar"                   , ROUND ( brut_tutar ));
            query.SET_VALUE(":isk_oncesi_kdv_dahil_tutar"   , ROUND ( isk_oncesi_kdv_dahil_tutar));
            query.SET_VALUE(":isk_oncesi_otv_dahil_tutar"   , ROUND ( isk_oncesi_otv_dahil_tutar ));
            query.SET_VALUE(":isk_sonrasi_otv_tutari"       , isk_sonrasi_otv_tutari);
            query.SET_VALUE(":isk_sonrasi_kdv_tutari"       , isk_sonrasi_kdv_tutari);
            query.SET_VALUE(":isk_sonrasi_otv_dahil_tutar"  , ROUND ( isk_sonrasi_otv_dahil_tutar));
            query.SET_VALUE(":fatura_tutari"                , ROUND ( fatura_tutari ));
            query.SET_VALUE(":ara_toplam"                   , ROUND ( ara_toplam));
            query.SET_VALUE(":kdv_oran_bilgileri"           , "");
            query.SET_VALUE(":otv_oran_bilgileri"           , "");
            query.SET_VALUE(":kdv_oran_gore_tutarlar_array" , PACK_DOUBLE_ARRAY(KDV_TUTARLAR_ARRAY , KDV_OTV_ORANLARI_SAYISI));
            query.SET_VALUE(":otv_oran_gore_tutarlar_array" , PACK_DOUBLE_ARRAY(OTV_TUTARLAR_ARRAY , KDV_OTV_ORANLARI_SAYISI));
            query.SET_VALUE(":fis_id"                       , m_fis_id);

            query.UPDATE();

        }

    }
    double  tevkifatli_kdv_tutari                   = 0.00;

    if ( tevkifatli_fatura_secenegi_degisti EQ true ) {
        if ( old_tevkifatli_fatura_mi EQ 1 ) {
            kdv_tutari              = ROUND ( kdv_tutari       + old_tevkifatli_kdv_tutari );
            fatura_tutari           = ROUND ( fatura_tutari    + old_tevkifatli_kdv_tutari );
            tevkifatli_kdv_tutari   = 0.00;
        }
        else {
            tevkifatli_kdv_tutari = ROUND ( ( kdv_tutari * m_ui->commaEdit_tevkifat_yuzdesi->GET_DOUBLE() ) / 100 );
            kdv_tutari            = ROUND ( kdv_tutari     - tevkifatli_kdv_tutari );
            fatura_tutari         = ROUND ( fatura_tutari  - tevkifatli_kdv_tutari );
        }
    }
    else if ( tevkifat_yuzdesi_degisti EQ true ) {

        kdv_tutari            = ROUND ( kdv_tutari       + old_tevkifatli_kdv_tutari );
        fatura_tutari         = ROUND ( fatura_tutari    - kdv_tutari + old_tevkifatli_kdv_tutari );
        tevkifatli_kdv_tutari = ROUND ( ( kdv_tutari     * m_ui->commaEdit_tevkifat_yuzdesi->GET_DOUBLE() ) / 100 );
        kdv_tutari            = ROUND ( kdv_tutari       - tevkifatli_kdv_tutari );
        fatura_tutari         = ROUND ( fatura_tutari    + kdv_tutari );
    }


    query.PREPARE_UPDATE ( "fat_faturalar ","fis_id",
                           "yurtici_satislar_hesap_id , "
                           "hizmet_urt_maliyeti_hesap_id  , "
                           "tevkifatli_fatura_mi          , "
                           "tevkifat_yuzdesi              , "
                           "tevkifatli_kdv_tutari         , "
                           "isk_sonrasi_kdv_tutari        , "
                           "fatura_tutari                 , "
                           "satistan_iade_hesap_id        , "
                           "kdv_muaf_mi                   , "
                           "otv_muaf_mi                     ",
                           "fis_id = :fis_id" );

    query.SET_VALUE ( ":yurtici_satislar_hesap_id",         m_sube_yurtici_satis_hsp_id                           );
    query.SET_VALUE ( ":hizmet_urt_maliyeti_hesap_id",      m_sube_hizmet_urt_maliyet_hsp_id                   );
    query.SET_VALUE ( ":tevkifatli_fatura_mi",              new_tevkifatli_fatura_mi                         );
    query.SET_VALUE ( ":tevkifat_yuzdesi",                  m_ui->commaEdit_tevkifat_yuzdesi->GET_DOUBLE()   );
    query.SET_VALUE ( ":tevkifatli_kdv_tutari",             tevkifatli_kdv_tutari                            );
    query.SET_VALUE ( ":isk_sonrasi_kdv_tutari",            kdv_tutari                                       );
    query.SET_VALUE ( ":fatura_tutari",                     fatura_tutari                                    );
    query.SET_VALUE ( ":satistan_iade_hesap_id",            m_satistan_iade_hesap_id                         );
    query.SET_VALUE ( ":kdv_muaf_mi",                       QVariant(m_ui->checkBox_kdv_muaf_mi->isChecked()).toInt());
    query.SET_VALUE ( ":otv_muaf_mi",                       QVariant(m_ui->checkBox_otv_muaf_mi->isChecked()).toInt());
    query.SET_VALUE ( ":fis_id",                            m_fis_id                                         );

    query.UPDATE();

    DB->COMMIT_TRANSACTION();

    *m_return_value = true;
    close();
}

/**************************************************************************************
                   FAT_FATURA_DETAYLARI_BATCH::SEARCH_EDIT_CLICKED
***************************************************************************************/

void FAT_FATURA_DETAYLARI_BATCH::SEARCH_EDIT_CLICKED ( QWidget * p_widget, QLineEdit * p_lineEdit )
{
    int hesap_id = OPEN_MUH_HESAP_ARAMA ( p_lineEdit->text(), this, 1 );

    if ( hesap_id > 0 ) {
        p_lineEdit->setText ( MUH_GET_HESAP_KODU ( hesap_id ) );
    }

    if ( p_widget EQ m_ui->searchEdit_sube_yurtici_satis_hsp_id ) {
        m_ui->lineEdit_sube_yurtici_satis_hsp_id->clear();
    }
    else if ( p_widget EQ m_ui->searchEdit_sube_hizmet_urt_maliyet_hsp_id ) {
        m_ui->lineEdit_sube_yurtici_satis_hsp_id->clear();
    }

}
