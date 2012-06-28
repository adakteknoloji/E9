#include "adak_sql.h"
#include "kernel_utils.h"
#include "e9_sabit_degerler_formu_class.h"
#include "ui_e9_sabit_degerler_formu.h"
#include "muh_console_utils.h"
#include "adak_std_utils.h"
#include "dvz_enum.h"
#include "dvz_console_utils.h"
#include "dmr_con_utils.h"
#include "fat_console_utils.h"
#include "e9_enum.h"
#include "e9_yetki.h"
#include "e9_log.h"
#include "e9_gui_utils.h"
#include "e9_console_utils.h"
#include "yonetim.h"
#include "muh_hesap_arama_open.h"
#include "dvz_gui_utils.h"
#include "dmr_con_utils.h"


// KDV OTV TANIMLARI 2 NOLU TAB INDEXTEDIR CHECK VAR KONTROLU ICIN EKLENMISTIR.
// TAB SIRASI DEGISTIRILDIGINDE GEREKEN DEGER SETLENMELIDIR

#define KDV_OTV_TAB_INDEX               2
#define ENT_HESAPLARI_TAB_INDEX         3

extern ADAK_SQL * DB;

const char * AYRACLAR[][2] = {{" ","Boşluk"},
                                        {".","Nokta" },
                                        {":","İki Nokta Üstüste"},
                                        {"/","Taksim"},
                                        {"*","Yıldız"},
                                        {"-","Tire"},
                                        {"+","Artı"},
                                        {"," ,"Virgül"},
                                        {"_","Alt Çizgi"},
                                        {";","Noktalı Virgül"},
                                        {"#","Diyez"}
                                       };

//      AYRAC SAYISI DEFINITION e9.h DE YER ALMAKTADIR. BURADAKI DEGISIKLIK DEFIMITION A YANSITILMALIDIR



/*************************************************************************************************************
                                            OPEN_E9_SABIT_DEGERLER_FORMU
*************************************************************************************************************/

void OPEN_E9_SABIT_DEGERLER_FORMU  ( QWidget * parent)
{
    E9_SABIT_DEGERLER_FORMU * F = new E9_SABIT_DEGERLER_FORMU ( parent);
    F->SHOW( FULL_SCREEN );
     
}

/*************************************************************************************************************
                                    E9_SABIT_DEGERLER_FORMU:: E9_SABIT_DEGERLER_FORMU
*************************************************************************************************************/

 E9_SABIT_DEGERLER_FORMU:: E9_SABIT_DEGERLER_FORMU ( QWidget * p_parent ) : FORM_KERNEL ( p_parent ), m_ui ( new Ui:: E9_SABIT_DEGERLER_FORMU )
{
    m_ui->setupUi     ( this );
    START_FORM_KERNEL ( this, DB );
}

 /*************************************************************************************************************
                                     E9_SABIT_DEGERLER_FORMU::SETUP_FORM
 *************************************************************************************************************/

void  E9_SABIT_DEGERLER_FORMU::SETUP_FORM()
{
     SET_HELP_PAGE       ( "yonetim-islemleri_parametreler");
     SET_PAGE_TITLE      ( tr ("PARAMETRELER" )    );

     m_ui->adakDate_mali_yil_baslangic_tarihi->MALI_YIL_KISITI_YOK( true );

     SET_SINGLE_RECORD_ID     ( 1 );

     REGISTER_BUTTONS_WIDGET    ( m_ui->navigation_buttons_widget );

     SET_SETTING_NAME    ( "E9_SABIT_DEGERLER_FORMU" );
      
     m_ui->comboBox_stok_birim_hassasiyet->addItems( QStringList() << "2" << "3" << "4" << "5" << "6");
     m_ui->comboBox_stok_miktar_hassasiyet->addItems( QStringList()<< "2" << "3" << "4" << "5" << "6");

     // ilk acilan tab index alindi
     m_onceki_tab_index = m_ui->tabWidget_sabit_degerler->currentIndex();

     m_ui->tabWidget_sabit_degerler->setCurrentIndex(0);

     m_ui->comboBox_amortisman_yontemi->addItems(QStringList() << "Normal Bakiye" << "Azalan Bakiye");
     m_ui->comboBox_amortisman_yontemi->setCurrentIndex(-1);

     m_ui->comboBox_acik_fatura_mi->addItems(QStringList() << tr("Açık Fatura") << tr("Kapalı Fatura"));
     m_ui->comboBox_acik_fatura_mi->setCurrentIndex(-1);

     SET_WIDGET_DEFAULT_LENGTHS();

     m_ui->commaEdit_irs_kac_gunde_faturalanacak->SET_PRECISION   ( 0    );
     m_ui->commaEdit_irs_kac_gunde_faturalanacak->VIRGULSUZ_CALIS ( true );

     m_ui->commaEdit_ciro_sonrasi_gun_sayisi->VIRGULSUZ_CALIS(true);
     m_ui->commaEdit_ciro_sonrasi_gun_sayisi->SET_PRECISION(0);

     DISABLE_CLEAR_ON_WIDGET( m_ui->tableWidget_kdv_oranlari );
     DISABLE_CLEAR_ON_WIDGET( m_ui->tableWidget_otv_oranlari );

     GET_ULKELER                ( m_ui->comboBox_ulke );

     FIND_AND_SET_COMBOBOX_TEXT ( m_ui->comboBox_ulke, tr("Türkiye") );

     GET_ILLER                  ( m_ui->comboBox_sehir, m_ui->comboBox_ulke->currentText());

     GET_ULKE_TELEFON_KODLARI   ( m_ui->comboBox_telefon_ulke_kodu );
     GET_ULKE_TELEFON_KODLARI   ( m_ui->comboBox_fax_ulke_kodu     );

     QString ulke_kodu = GET_ULKE_TELEFON_KODU ( m_ui->comboBox_ulke->currentText() );

     FIND_AND_SET_COMBOBOX_TEXT ( m_ui->comboBox_telefon_ulke_kodu, ulke_kodu );
     FIND_AND_SET_COMBOBOX_TEXT ( m_ui->comboBox_fax_ulke_kodu, ulke_kodu );

     GET_ALAN_KODLARI           ( m_ui->comboBox_telefon_alan_kodu );
     GET_ALAN_KODLARI           ( m_ui->comboBox_fax_alan_kodu );


     // KDV OTV ORANLARI TABLEWIDGET AYARLAMALARI.....................................

     m_ui->tableWidget_kdv_oranlari->setRowCount( KDV_OTV_ORANLARI_SAYISI );
     m_ui->tableWidget_otv_oranlari->setRowCount(KDV_OTV_ORANLARI_SAYISI);

     M_KDV_ORANLARI_COMMAEDIT_ARRAY = new QCommaEdit * [KDV_OTV_ORANLARI_SAYISI];
     M_KDV_ORANLARI_ARRAY           = new double       [KDV_OTV_ORANLARI_SAYISI];

     M_OTV_ORANLARI_COMMAEDIT_ARRAY = new QCommaEdit * [KDV_OTV_ORANLARI_SAYISI];
     M_OTV_ORANLARI_ARRAY           = new double       [KDV_OTV_ORANLARI_SAYISI];


     M_IND_KDV_HESAPLARI_ID_ARRAY      = new     int [ KDV_OTV_ORANLARI_SAYISI ];
     M_HES_KDV_HESAPLARI_ID_ARRAY      = new     int [ KDV_OTV_ORANLARI_SAYISI ];
     M_IND_KDV_IADE_HESAPLARI_ID_ARRAY = new     int [ KDV_OTV_ORANLARI_SAYISI ];
     M_HES_KDV_IADE_HESAPLARI_ID_ARRAY = new     int [ KDV_OTV_ORANLARI_SAYISI ];


     M_OTV_HESAPLARI_ALIS_ID_ARRAY     = new     int [ KDV_OTV_ORANLARI_SAYISI ];
     M_OTV_HESAPLARI_SATIS_ID_ARRAY    = new     int [ KDV_OTV_ORANLARI_SAYISI ];

     M_TEVKIFAT_KDV_HESAPLARI_ID_ARRAY = new     int [ KDV_OTV_ORANLARI_SAYISI ];

     for ( int i = 0; i < KDV_OTV_ORANLARI_SAYISI; i++ ) {

         M_KDV_ORANLARI_COMMAEDIT_ARRAY[i] = new QCommaEdit ( this );
         M_KDV_ORANLARI_COMMAEDIT_ARRAY[i]->setObjectName( "tableWidget_kdv_oranlari" );
         M_KDV_ORANLARI_COMMAEDIT_ARRAY[i]->SET_RETURN_EMPTY(false);
         M_KDV_ORANLARI_COMMAEDIT_ARRAY[i]->SET_PRECISION(2);
         m_ui->tableWidget_kdv_oranlari->setCellWidget ( i, 0, M_KDV_ORANLARI_COMMAEDIT_ARRAY[i]);



         M_OTV_ORANLARI_COMMAEDIT_ARRAY[i] = new QCommaEdit ( this );
         M_OTV_ORANLARI_COMMAEDIT_ARRAY[i]->setObjectName( "tableWidget_otv_oranlari");;
         M_OTV_ORANLARI_COMMAEDIT_ARRAY[i]->SET_RETURN_EMPTY(false);
         M_OTV_ORANLARI_COMMAEDIT_ARRAY[i]->SET_PRECISION(2);
         m_ui->tableWidget_otv_oranlari->setCellWidget ( i, 0, M_OTV_ORANLARI_COMMAEDIT_ARRAY[i]);
     }

     WIDGET_DIZILERINI_DUZENLE                   ();
     KDV_OTV_HESAP_SAYISINA_GORE_EKRANI_DUZENLE  ();

     // KDV OTV ORANLARI TABLEWIDGET AYARLAMALARI.......................................

     SET_FIRST_FOCUS_WIDGET   ( m_ui->lineEdit_firma_ismi       );

     DVZ_FILL_COMBOBOX        ( m_ui->combobox_para_birimi      );

     m_ui->combobox_para_birimi->setCurrentIndex ( m_ui->combobox_para_birimi->findText ( DVZ_GET_TEMEL_PARA_BIRIMI_KODU ( ) ) );
     m_ui->lineedit_para_birimi->setText         ( DVZ_GET_DOVIZ_ADI ( DVZ_GET_TEMEL_PARA_BIRIMI_ID() ) );

     for ( int i = 0; i < AYRAC_SAYISI ; i++ ) {
         m_ui->combobox_e9_ayraci->addItem(AYRACLAR[i][0]);
     }

     REGISTER_CHANGER_BUTTON(m_ui->pushButton_logo);

     E9_FILL_KDV_ORANLARI_COMBOBOX(m_ui->comboBox_ontanimli_kdv_orani);

     DISABLE_CLEAR_ON_WIDGET ( m_ui->combobox_para_birimi );
     DISABLE_CLEAR_ON_WIDGET ( m_ui->lineedit_para_birimi );
     DISABLE_CLEAR_ON_WIDGET ( m_ui->comboBox_amortisman_yontemi);

     m_ui->comboBox_acik_fatura_mi->setEditable(false);
}

 /*************************************************************************************************************
                                     E9_SABIT_DEGERLER_FORMU::CLEAR_FORM_MEMBERS
 *************************************************************************************************************/

void  E9_SABIT_DEGERLER_FORMU::CLEAR_FORM_MEMBERS ()
{
}

 /*************************************************************************************************************
                                     E9_SABIT_DEGERLER_FORMU::NEW_RECORD
 *************************************************************************************************************/

void  E9_SABIT_DEGERLER_FORMU::NEW_RECORD()
{
}

/*************************************************************************************************************
                                    E9_SABIT_DEGERLER_FORMU::GET_RECORD
*************************************************************************************************************/

int E9_SABIT_DEGERLER_FORMU::GET_RECORD ( int p_record_id )
{
    SQL_QUERY sql_query ( DB );

    sql_query.PREPARE_SELECT("e9_sabit_degerler",
                             "cek_ciro_sonrasi_gun_sayisi, "
                             "dmr_amortisman_ayirma_yontemi,"
                             "dmr_amor_yntemi_degisebilir_mi, "
                             "dmr_max_amortisman_orani, "
                             "fat_acik_fatura_mi_kapali_mi, "
                             "irs_faturalandirilacak_gun, "
                             "firma_ismi, "
                             "firma_yetkilisi, firma_adres_satiri_1, firma_adres_satiri_2, "
                             "firma_adres_satiri_3, firma_ulke, "
                             "firma_sehir, firma_ilce, firma_semt, firma_posta_kodu, "
                             "firma_tel_ulke_kodu, firma_fax_ulke_kodu, "
                             "firma_tel_alan_kodu, firma_fax_alan_kodu, "
                             "firma_telefon, firma_fax, firma_email, firma_web_adresi, "
                             "firma_vergi_dairesi, firma_vergi_numarasi, "
                             "firma_aciklama, mali_yil_ilk_tarih, "
                             "e9_ayraci,firma_logosu, ontanimli_kdv_orani, "
                             "kdv_orani_sayisi, otv_orani_sayisi, kdv_oranlari_array,"
                             "otv_oranlari_array , ind_kdv_hesap_id_array ,"
                             "ind_kdv_iade_hesap_id_array , hes_kdv_hesap_id_array ,"
                             "hes_kdv_iade_hesap_id_array , otv_alis_hesap_id_array,"
                             "otv_satis_hesap_id_array , tevkifat_kdv_hesap_id_array,"
                             "max_nakit_giris_cikis_tutari,   "
                             "muhasebe_alarmlari_kapalimi, sm_sicil_numarasi, stok_birim_fiyat_hassasiyeti ,"
                             "stok_birim_miktar_hassasiyeti ",
                             "sabit_deger_id = :sabit_deger_id      ");

    sql_query.SET_VALUE( ":sabit_deger_id", p_record_id );

    if ( sql_query.SELECT() EQ 0 ) {
        return ADAK_FAIL;
    }

    sql_query.NEXT();

    m_ui->lineEdit_sm_sicil_numarasi->setText( sql_query.VALUE( "sm_sicil_numarasi" ).toString() );

    FIND_AND_SET_COMBOBOX_TEXT( m_ui->comboBox_stok_birim_hassasiyet, sql_query.VALUE( "stok_birim_fiyat_hassasiyeti" ).toString() );
    FIND_AND_SET_COMBOBOX_TEXT( m_ui->comboBox_stok_miktar_hassasiyet, sql_query.VALUE( "stok_birim_miktar_hassasiyeti" ).toString() );

    m_ui->commaEdit_ciro_sonrasi_gun_sayisi->SET_INTEGER ( sql_query.VALUE("cek_ciro_sonrasi_gun_sayisi").toInt()     );
    m_ui->comboBox_amortisman_yontemi->setCurrentIndex   (
            m_ui->comboBox_amortisman_yontemi->findText  ( DMR_GET_AMORTISMAN_SEKLI_STRING ( sql_query.VALUE("dmr_amortisman_ayirma_yontemi").toInt())));

    m_ui->comboBox_amor_yontemi_degisebilir_mi->setCurrentIndex ( sql_query.VALUE ( "dmr_amor_yntemi_degisebilir_mi" ).toInt());

    m_ui->commaEdit_max_amortisman_orani->SET_DOUBLE            ( sql_query.VALUE("dmr_max_amortisman_orani").toDouble());

    if ( sql_query.VALUE("dmr_amor_yntemi_degisebilir_mi").toInt() EQ 1 ) {
        m_ui->comboBox_amortisman_yontemi->setDisabled     ( true );
        m_ui->comboBox_amortisman_yontemi->setCurrentIndex ( -1   );
        m_ui->label_amortisman_yontemi->setDisabled        ( true );
    }

    m_ui->comboBox_acik_fatura_mi->setCurrentIndex ( m_ui->comboBox_acik_fatura_mi->findText ( FAT_GET_ACIK_KAPALI_FATURA_STRING ( sql_query.VALUE("fat_acik_fatura_mi_kapali_mi").toInt())));
    m_ui->commaEdit_irs_kac_gunde_faturalanacak->SET_INTEGER ( sql_query.VALUE("irs_faturalandirilacak_gun").toInt());

    m_ui->lineEdit_firma_ismi->setText     ( sql_query.VALUE("firma_ismi").toString());
    m_ui->lineedit_yetkili->setText        ( sql_query.VALUE("firma_yetkilisi").toString());
    m_ui->lineEdit_adres_satiri_1->setText ( sql_query.VALUE("firma_adres_satiri_1").toString());
    m_ui->lineEdit_adres_satiri_2->setText ( sql_query.VALUE("firma_adres_satiri_2").toString());
    m_ui->lineEdit_adres_satiri_3->setText ( sql_query.VALUE("firma_adres_satiri_3").toString());

    FIND_AND_SET_COMBOBOX_TEXT ( m_ui->comboBox_ulke,  sql_query.VALUE("firma_ulke").toString());

    m_ui->comboBox_sehir->clear();

    GET_ILLER ( m_ui->comboBox_sehir, m_ui->comboBox_ulke->currentText() );

    FIND_AND_SET_COMBOBOX_TEXT ( m_ui->comboBox_sehir, sql_query.VALUE("firma_sehir").toString());

    GET_ILCELER ( m_ui->comboBox_ilce, m_ui->comboBox_sehir->currentText() );

    m_ui->lineEdit_semt->setText ( sql_query.VALUE("firma_semt").toString());
    m_ui->comboBox_ilce->setCurrentIndex(m_ui->comboBox_ilce->findText(sql_query.VALUE("firma_ilce").toString()));

    m_ui->lineEdit_posta_kodu->setText( sql_query.VALUE("firma_posta_kodu").toString() );


    FIND_AND_SET_COMBOBOX_TEXT ( m_ui->comboBox_telefon_ulke_kodu, sql_query.VALUE("firma_tel_ulke_kodu").toString() );
    FIND_AND_SET_COMBOBOX_TEXT ( m_ui->comboBox_fax_ulke_kodu,     sql_query.VALUE("firma_fax_ulke_kodu").toString() );

    m_ui->comboBox_telefon_alan_kodu->clear();
    m_ui->comboBox_fax_alan_kodu->clear();

    GET_ALAN_KODLARI           ( m_ui->comboBox_telefon_alan_kodu );
    GET_ALAN_KODLARI           ( m_ui->comboBox_fax_alan_kodu );

    FIND_AND_SET_COMBOBOX_TEXT ( m_ui->comboBox_telefon_alan_kodu, sql_query.VALUE("firma_tel_alan_kodu").toString());
    FIND_AND_SET_COMBOBOX_TEXT ( m_ui->comboBox_fax_alan_kodu,     sql_query.VALUE("firma_fax_alan_kodu").toString());

    m_ui->lineEdit_telefon->setText                   ( sql_query.VALUE("firma_telefon").toString() );
    m_ui->lineEdit_fax->setText                       ( sql_query.VALUE("firma_fax").toString()     );

    m_ui->lineedit_mail->setText                      ( sql_query.VALUE("firma_email").toString()      );
    m_ui->lineedit_web_adresi->setText                ( sql_query.VALUE("firma_web_adresi").toString() );

    GET_VERGI_DAIRELERI                               ( m_ui->comboBox_vergi_dairesi, m_ui->comboBox_sehir->currentText() );

    FIND_AND_SET_COMBOBOX_TEXT                        ( m_ui->comboBox_vergi_dairesi, sql_query.VALUE("firma_vergi_dairesi").toString() );

    m_ui->lineEdit_vergi_numarasi->setText ( sql_query.VALUE("firma_vergi_numarasi").toString() );
    m_ui->limitedTextEdit_aciklama->setText( sql_query.VALUE("firma_aciklama").toString()       );

    m_ui->adakDate_mali_yil_baslangic_tarihi->SET_DATE( sql_query.VALUE("mali_yil_ilk_tarih").toString());
    int e9_ayrac_index = m_ui->combobox_e9_ayraci->findText ( sql_query.VALUE("e9_ayraci").toString() );
    m_ui->combobox_e9_ayraci->setCurrentIndex ( e9_ayrac_index );
    m_ui->lineedit_e9_ayraci->setText         ( AYRACLAR[e9_ayrac_index][1]);

    m_ui->pushButton_logo->setIcon(QIcon());
    m_ui->pushButton_logo->setText(" ");
    m_ui->pushButton_logo->setIcon(QIcon(ADAK_BYTE_ARRAY_2_PIXMAP(sql_query.VALUE("firma_logosu").toByteArray())));

    int logo_width = m_ui->pushButton_logo->width();
    int logo_height = m_ui->pushButton_logo->height();

    m_ui->pushButton_logo->setIconSize(QSize(logo_width,logo_height));

    m_ui->comboBox_ontanimli_kdv_orani->setCurrentIndex(m_ui->comboBox_ontanimli_kdv_orani->findText(sql_query.VALUE("ontanimli_kdv_orani").toString()));

    m_ui->commaEdit_max_nakit_giris_cikis_tutari->SET_DOUBLE( sql_query.VALUE( "max_nakit_giris_cikis_tutari" ).toDouble() );

    m_ui->combo_box_muh_alarmlari->setCurrentIndex(sql_query.VALUE("muhasebe_alarmlari_kapalimi").toInt());

    ADRES_BILGILERINI_OLUSTUR();


    // KDV OTV ORANLARININ OKUNMASI ................................................


    m_kdv_orani_sayisi = sql_query.VALUE("kdv_orani_sayisi").toInt();
    m_otv_orani_sayisi = sql_query.VALUE("otv_orani_sayisi").toInt();

    if ( m_kdv_orani_sayisi > 0 ) {

        UNPACK_DOUBLE_ARRAY ( sql_query.VALUE ( "kdv_oranlari_array" ).toString(), M_KDV_ORANLARI_ARRAY, KDV_OTV_ORANLARI_SAYISI );

        for ( int i = m_kdv_orani_sayisi; i < KDV_OTV_ORANLARI_SAYISI; i++ ) {
            M_KDV_ORANLARI_COMMAEDIT_ARRAY[i]->clear();
        }

        for ( int i = 0; i < m_kdv_orani_sayisi; i++ ) {
            M_KDV_ORANLARI_COMMAEDIT_ARRAY[i]->SET_DOUBLE ( M_KDV_ORANLARI_ARRAY[i] );
        }
    }

    if ( m_otv_orani_sayisi > 0 ) {

        UNPACK_DOUBLE_ARRAY ( sql_query.VALUE ( "otv_oranlari_array" ).toString(), M_OTV_ORANLARI_ARRAY, KDV_OTV_ORANLARI_SAYISI );

        for ( int i = m_otv_orani_sayisi; i < KDV_OTV_ORANLARI_SAYISI; i++ ) {
            M_OTV_ORANLARI_COMMAEDIT_ARRAY[i]->clear();
        }

        for ( int i = 0; i < m_otv_orani_sayisi; i++ ) {
            M_OTV_ORANLARI_COMMAEDIT_ARRAY[i]->SET_DOUBLE ( M_OTV_ORANLARI_ARRAY[i] );
        }
    }
    QString     ind_kdv_hesap_id_array          =   sql_query.VALUE ( "ind_kdv_hesap_id_array"      ).toString();
    QString     ind_kdv_iade_hesap_id_array     =   sql_query.VALUE ( "ind_kdv_iade_hesap_id_array" ).toString();
    QString     hes_kdv_hesap_id_array          =   sql_query.VALUE ( "hes_kdv_hesap_id_array"      ).toString();
    QString     hes_kdv_iade_hesap_id_array     =   sql_query.VALUE ( "hes_kdv_iade_hesap_id_array" ).toString();
    QString     otv_alis_hesap_id_array         =   sql_query.VALUE ( "otv_alis_hesap_id_array"     ).toString();
    QString     otv_satis_hesap_id_array        =   sql_query.VALUE ( "otv_satis_hesap_id_array"    ).toString();
    QString     tevkifat_kdv_hesap_id_array     =   sql_query.VALUE ( "tevkifat_kdv_hesap_id_array" ).toString();

    UNPACK_INT_ARRAY    ( ind_kdv_hesap_id_array,       M_IND_KDV_HESAPLARI_ID_ARRAY,       KDV_OTV_ORANLARI_SAYISI );
    UNPACK_INT_ARRAY    ( ind_kdv_iade_hesap_id_array,  M_IND_KDV_IADE_HESAPLARI_ID_ARRAY,  KDV_OTV_ORANLARI_SAYISI );
    UNPACK_INT_ARRAY    ( hes_kdv_hesap_id_array,       M_HES_KDV_HESAPLARI_ID_ARRAY,       KDV_OTV_ORANLARI_SAYISI );
    UNPACK_INT_ARRAY    ( hes_kdv_iade_hesap_id_array,  M_HES_KDV_IADE_HESAPLARI_ID_ARRAY,  KDV_OTV_ORANLARI_SAYISI );

    UNPACK_INT_ARRAY    ( otv_alis_hesap_id_array,      M_OTV_HESAPLARI_ALIS_ID_ARRAY,      KDV_OTV_ORANLARI_SAYISI );
    UNPACK_INT_ARRAY    ( otv_satis_hesap_id_array,     M_OTV_HESAPLARI_SATIS_ID_ARRAY,     KDV_OTV_ORANLARI_SAYISI );

    UNPACK_INT_ARRAY    ( tevkifat_kdv_hesap_id_array,  M_TEVKIFAT_KDV_HESAPLARI_ID_ARRAY,  KDV_OTV_ORANLARI_SAYISI );

    QString muh_hesap_kodu;
    QString muh_hesap_ismi;

    for ( int i = 0; i < m_kdv_orani_sayisi; i++ ) {

        MUH_GET_HESAP_KODU_HESAP_ISMI                            ( M_IND_KDV_HESAPLARI_ID_ARRAY[i], muh_hesap_kodu, muh_hesap_ismi );
        M_IND_KDV_HESAPLARI_SEARCHEDIT_ARRAY[i]->SET_TEXT        ( muh_hesap_kodu );
        M_IND_KDV_HESAPLARI_LINEEDIT_ARRAY[i]->setText           ( muh_hesap_ismi );

        MUH_GET_HESAP_KODU_HESAP_ISMI                            ( M_HES_KDV_HESAPLARI_ID_ARRAY[i], muh_hesap_kodu, muh_hesap_ismi );
        M_HES_KDV_HESAPLARI_SEARCHEDIT_ARRAY[i]->SET_TEXT        ( muh_hesap_kodu );
        M_HES_KDV_HESAPLARI_LINEEDIT_ARRAY[i]->setText           ( muh_hesap_ismi );

        MUH_GET_HESAP_KODU_HESAP_ISMI                            ( M_IND_KDV_IADE_HESAPLARI_ID_ARRAY[i], muh_hesap_kodu, muh_hesap_ismi );
        M_IND_KDV_HESAPLARI_IADE_SEARCHEDIT_ARRAY[i]->SET_TEXT   ( muh_hesap_kodu );
        M_IND_KDV_HESAPLARI_IADE_LINEEDIT_ARRAY[i]->setText      ( muh_hesap_ismi );

        MUH_GET_HESAP_KODU_HESAP_ISMI                            ( M_HES_KDV_IADE_HESAPLARI_ID_ARRAY[i], muh_hesap_kodu, muh_hesap_ismi );
        M_HES_KDV_HESAPLARI_IADE_SEARCHEDIT_ARRAY[i]->SET_TEXT   ( muh_hesap_kodu );
        M_HES_KDV_HESAPLARI_IADE_LINEEDIT_ARRAY[i]->setText      ( muh_hesap_ismi );

        MUH_GET_HESAP_KODU_HESAP_ISMI                            ( M_TEVKIFAT_KDV_HESAPLARI_ID_ARRAY[i], muh_hesap_kodu, muh_hesap_ismi );
        M_TEVKIFAT_KDV_HESAPLARI_SEARCHEDIT_ARRAY[i]->SET_TEXT   ( muh_hesap_kodu );
        M_TEVKIFAT_KDV_HESAPLARI_LINEEDIT_ARRAY[i]->setText      ( muh_hesap_ismi );

    }

    for ( int i = 0; i < m_otv_orani_sayisi; i++ ) {

        MUH_GET_HESAP_KODU_HESAP_ISMI                   ( M_OTV_HESAPLARI_ALIS_ID_ARRAY[i], muh_hesap_kodu, muh_hesap_ismi );
        M_OTV_HESAPLARI_ALIS_SEARCHEDIT_ARRAY[i]->SET_TEXT   ( muh_hesap_kodu );
        M_OTV_HESAPLARI_ALIS_LINEEDIT_ARRAY[i]->setText      ( muh_hesap_ismi );

        MUH_GET_HESAP_KODU_HESAP_ISMI                   ( M_OTV_HESAPLARI_SATIS_ID_ARRAY[i], muh_hesap_kodu, muh_hesap_ismi );
        M_OTV_HESAPLARI_SATIS_SEARCHEDIT_ARRAY[i]->SET_TEXT   ( muh_hesap_kodu );
        M_OTV_HESAPLARI_SATIS_LINEEDIT_ARRAY[i]->setText      ( muh_hesap_ismi );

    }

    return ADAK_OK;
}

/*************************************************************************************************************
                                    E9_SABIT_DEGERLER_FORMU::CHECK_VAR
*************************************************************************************************************/

int E9_SABIT_DEGERLER_FORMU::CHECK_VAR ( QObject * p_object )
{

    if ( p_object EQ m_ui->adakDate_mali_yil_baslangic_tarihi ) {

        SQL_QUERY query(DB);

        query.PREPARE_SELECT("muh_fisler","fis_id");

        if ( query.SELECT() > 0 ) {
            MSG_WARNING( tr("Muhasebeye fiş girişi yapılmiş durumda, mali yılı değiştiremezsiniz!.."), NULL);
            return ADAK_FAIL_UNDO;
        }
    }
    else if ( p_object EQ m_ui->combobox_para_birimi ) {
        m_ui->lineedit_para_birimi->setText ( DVZ_GET_DOVIZ_ADI ( DVZ_GET_DOVIZ_ID ( m_ui->combobox_para_birimi->currentText() ) ));
    }

    else if ( p_object EQ m_ui->combobox_e9_ayraci ) {
        m_ui->lineedit_e9_ayraci->setText ( AYRACLAR [ m_ui->combobox_e9_ayraci->currentIndex() ][1] );
    }
    else if ( p_object EQ m_ui->comboBox_ulke ) {

        if ( m_ui->comboBox_ulke->currentText().size() > 30 ) {
            MSG_WARNING(  tr("Ülke adı 30 karakterden daha uzun olanmaz."), NULL);
            return ADAK_FAIL_UNDO;
        }

        QString ulke_kodu = GET_ULKE_TELEFON_KODU ( m_ui->comboBox_ulke->currentText() );

        FIND_AND_SET_COMBOBOX_TEXT ( m_ui->comboBox_telefon_ulke_kodu, ulke_kodu );
        FIND_AND_SET_COMBOBOX_TEXT ( m_ui->comboBox_fax_ulke_kodu, ulke_kodu );

        m_ui->comboBox_telefon_alan_kodu->clear();
        m_ui->comboBox_fax_alan_kodu->clear();

        GET_ALAN_KODLARI           ( m_ui->comboBox_telefon_alan_kodu );
        GET_ALAN_KODLARI           ( m_ui->comboBox_fax_alan_kodu );

        m_ui->comboBox_sehir->clear();

        GET_ILLER                  ( m_ui->comboBox_sehir, m_ui->comboBox_ulke->currentText());

        m_ui->comboBox_vergi_dairesi->clear();
        m_ui->comboBox_ilce->clear();
    }
    else if ( p_object EQ m_ui->comboBox_sehir ) {

        QString il_alan_kodu = GET_IL_ALAN_KODU ( m_ui->comboBox_sehir->currentText() , "" );

        FIND_AND_SET_COMBOBOX_TEXT ( m_ui->comboBox_telefon_alan_kodu, il_alan_kodu );
        FIND_AND_SET_COMBOBOX_TEXT ( m_ui->comboBox_fax_alan_kodu, il_alan_kodu );

        m_ui->comboBox_ilce->clear();


        GET_ILCELER ( m_ui->comboBox_ilce, m_ui->comboBox_sehir->currentText() );

        m_ui->comboBox_vergi_dairesi->clear();

        GET_VERGI_DAIRELERI ( m_ui->comboBox_vergi_dairesi, m_ui->comboBox_sehir->currentText() );

    }
    else if ( p_object EQ m_ui->comboBox_amor_yontemi_degisebilir_mi ) {
        if ( m_ui->comboBox_amor_yontemi_degisebilir_mi->currentIndex() EQ 0 ) {
            m_ui->label_amortisman_yontemi->setEnabled    ( true );
            m_ui->comboBox_amortisman_yontemi->setEnabled ( true );

        }
        else {
            m_ui->comboBox_amortisman_yontemi->setDisabled     ( true );
            m_ui->label_amortisman_yontemi->setDisabled        ( true );
            m_ui->comboBox_amortisman_yontemi->setCurrentIndex ( -1 );
        }
    }  
    else if ( m_ui->tabWidget_sabit_degerler->currentIndex() EQ KDV_OTV_TAB_INDEX ) {  // DOSYA BASINDA INDEX DEFINE SETLENMISTIR

        int comma_edit_turu  = 0;

        for ( int i = 0; i < KDV_OTV_ORANLARI_SAYISI; i++ ) {
            if ( p_object EQ M_KDV_ORANLARI_COMMAEDIT_ARRAY[i] ) {
                comma_edit_turu = 1; // KDV
                if ( M_KDV_ORANLARI_COMMAEDIT_ARRAY[i]->GET_DOUBLE() > 99 ) {
                    MSG_WARNING(  tr("%n nolu satırda hatalı giriş. \n"
                                                               "KDV oranı 100 den küçük olmalı.","", i + 1 ), NULL);
                    return ADAK_FAIL;
                }

                if ( i < KDV_OTV_ORANLARI_SAYISI - 1 ) {
                    if ( M_KDV_ORANLARI_COMMAEDIT_ARRAY[i]->GET_DOUBLE() EQ 0.0 ) {
                        if ( M_KDV_ORANLARI_COMMAEDIT_ARRAY[i+1]->GET_DOUBLE() NE 0.0 ) {
                            MSG_WARNING(  tr("%n nolu satırda bir KDV oranı mevcut o satırı silmeden "
                                                                       "bu satırda silme işlemi yapamazsınız", "", i+2 ), NULL);
                            return ADAK_FAIL;
                        }
                    }
                }
            }
            if ( p_object EQ M_OTV_ORANLARI_COMMAEDIT_ARRAY[i] ) {
                comma_edit_turu = 2; // OTV
                if ( M_OTV_ORANLARI_COMMAEDIT_ARRAY[i]->GET_DOUBLE() > 99 ) {
                    MSG_WARNING(  tr("%n nolu satırda hatalı giriş. \n"
                                                               "OTV oranı 100 den küçük olmalı.","", i + 1 ), NULL);
                    return ADAK_FAIL;
                }

                if ( i < KDV_OTV_ORANLARI_SAYISI - 1 ) {
                    if ( M_OTV_ORANLARI_COMMAEDIT_ARRAY[i]->GET_DOUBLE() EQ 0.0 ) {
                        if ( M_OTV_ORANLARI_COMMAEDIT_ARRAY[i+1]->GET_DOUBLE() NE 0.0 ) {
                            MSG_WARNING(  tr("%n nolu satırda bir OTV oranı mevcut o satırı silmeden "
                                                                       "bu satırda silme işlemi yapamazsınız", "", i+2 ), NULL);
                            return ADAK_FAIL;
                        }
                    }
                }
            }
        }

        if ( comma_edit_turu EQ 1 ) {
            m_kdv_orani_sayisi = 0;
            for ( int i = 0; i < KDV_OTV_ORANLARI_SAYISI; i++  ) {
                M_KDV_ORANLARI_ARRAY[i] = M_KDV_ORANLARI_COMMAEDIT_ARRAY[i]->GET_DOUBLE();
                m_kdv_orani_sayisi++;
                if( i < KDV_OTV_ORANLARI_SAYISI -1 ) {
                    if ( M_KDV_ORANLARI_COMMAEDIT_ARRAY[i + 1]->GET_DOUBLE() EQ 0.00 ) {
                        break;
                    }
                }
            }
        }
        if( comma_edit_turu EQ 2 ) {
            m_otv_orani_sayisi = 0;
            for( int i = 0; i < KDV_OTV_ORANLARI_SAYISI; i++ ) {
                m_otv_orani_sayisi++;
                M_OTV_ORANLARI_ARRAY[i] = M_OTV_ORANLARI_COMMAEDIT_ARRAY[i]->GET_DOUBLE();
                if( i < KDV_OTV_ORANLARI_SAYISI -1 ) {
                    if ( M_OTV_ORANLARI_COMMAEDIT_ARRAY[i + 1]->GET_DOUBLE() EQ 0.00 ) {
                        break;
                    }
                }
            }
        }

        for ( int i = 0; i < m_kdv_orani_sayisi; i++ ) {

            M_IND_KDV_HESAPLARI_LABEL_ARRAY             [i]->setVisible( true );
            M_IND_KDV_HESAPLARI_SEARCHEDIT_ARRAY        [i]->setVisible( true );
            M_IND_KDV_HESAPLARI_LINEEDIT_ARRAY          [i]->setVisible( true );

            M_IND_KDV_HESAPLARI_IADE_LABEL_ARRAY        [i]->setVisible( true );
            M_IND_KDV_HESAPLARI_IADE_SEARCHEDIT_ARRAY   [i]->setVisible( true );
            M_IND_KDV_HESAPLARI_IADE_LINEEDIT_ARRAY     [i]->setVisible( true );

            M_HES_KDV_HESAPLARI_LABEL_ARRAY             [i]->setVisible( true );
            M_HES_KDV_HESAPLARI_SEARCHEDIT_ARRAY        [i]->setVisible( true );
            M_HES_KDV_HESAPLARI_LINEEDIT_ARRAY          [i]->setVisible( true );

            M_HES_KDV_HESAPLARI_IADE_LABEL_ARRAY        [i]->setVisible( true );
            M_HES_KDV_HESAPLARI_IADE_SEARCHEDIT_ARRAY   [i]->setVisible( true );
            M_HES_KDV_HESAPLARI_IADE_LINEEDIT_ARRAY     [i]->setVisible( true );

            M_TEVKIFAT_KDV_HESAPLARI_LABEL_ARRAY        [i]->setVisible( true );
            M_TEVKIFAT_KDV_HESAPLARI_SEARCHEDIT_ARRAY   [i]->setVisible( true );
            M_TEVKIFAT_KDV_HESAPLARI_LINEEDIT_ARRAY     [i]->setVisible( true );

            // indirilecek kdv
            M_IND_KDV_HESAPLARI_LABEL_ARRAY[i]->setText      ( "KDV %" + QVariant   ( M_KDV_ORANLARI_ARRAY [i] ).toString() );
            // hesaplanan kdv
            M_HES_KDV_HESAPLARI_LABEL_ARRAY[i]->setText      ( "KDV %" + QVariant   ( M_KDV_ORANLARI_ARRAY [i] ).toString() );
            // indrilicek kdv hesaplari iade
            M_IND_KDV_HESAPLARI_IADE_LABEL_ARRAY[i]->setText ( "KDV %" + QVariant   ( M_KDV_ORANLARI_ARRAY [i] ).toString() );
            // hesaplanan kdv hesaplari iade
            M_HES_KDV_HESAPLARI_IADE_LABEL_ARRAY[i]->setText ( "KDV %" + QVariant   ( M_KDV_ORANLARI_ARRAY [i] ).toString() );
            // tevkifat kdv hesaplari
            M_TEVKIFAT_KDV_HESAPLARI_LABEL_ARRAY[i]->setText ( "KDV %" + QVariant   ( M_KDV_ORANLARI_ARRAY [i] ).toString() );

            if(  M_KDV_ORANLARI_ARRAY[i] EQ 0.00 ) {
                M_IND_KDV_HESAPLARI_LABEL_ARRAY             [i]->setDisabled( true );
                M_IND_KDV_HESAPLARI_SEARCHEDIT_ARRAY        [i]->setDisabled( true );
                M_IND_KDV_HESAPLARI_LINEEDIT_ARRAY          [i]->setDisabled( true );

                M_IND_KDV_HESAPLARI_IADE_LABEL_ARRAY        [i]->setDisabled( true );
                M_IND_KDV_HESAPLARI_IADE_SEARCHEDIT_ARRAY   [i]->setDisabled( true );
                M_IND_KDV_HESAPLARI_IADE_LINEEDIT_ARRAY     [i]->setDisabled( true );

                M_HES_KDV_HESAPLARI_LABEL_ARRAY             [i]->setDisabled( true );
                M_HES_KDV_HESAPLARI_SEARCHEDIT_ARRAY        [i]->setDisabled( true );
                M_HES_KDV_HESAPLARI_LINEEDIT_ARRAY          [i]->setDisabled( true );

                M_HES_KDV_HESAPLARI_IADE_LABEL_ARRAY        [i]->setDisabled( true );
                M_HES_KDV_HESAPLARI_IADE_SEARCHEDIT_ARRAY   [i]->setDisabled( true );
                M_HES_KDV_HESAPLARI_IADE_LINEEDIT_ARRAY     [i]->setDisabled( true );

                M_TEVKIFAT_KDV_HESAPLARI_LABEL_ARRAY        [i]->setDisabled( true );
                M_TEVKIFAT_KDV_HESAPLARI_SEARCHEDIT_ARRAY   [i]->setDisabled( true );
                M_TEVKIFAT_KDV_HESAPLARI_LINEEDIT_ARRAY     [i]->setDisabled( true );
            }
        }

        for ( int i = 0; i < m_otv_orani_sayisi; i++ ) {

            M_OTV_HESAPLARI_ALIS_LABEL_ARRAY        [i]->setVisible    ( true );
            M_OTV_HESAPLARI_ALIS_SEARCHEDIT_ARRAY   [i]->setVisible    ( true );
            M_OTV_HESAPLARI_ALIS_LINEEDIT_ARRAY     [i]->setVisible    ( true );

            M_OTV_HESAPLARI_SATIS_LABEL_ARRAY       [i]->setVisible    ( true );
            M_OTV_HESAPLARI_SATIS_SEARCHEDIT_ARRAY  [i]->setVisible    ( true );
            M_OTV_HESAPLARI_SATIS_LINEEDIT_ARRAY    [i]->setVisible    ( true );

            M_OTV_HESAPLARI_ALIS_LABEL_ARRAY[i]->setText  ( "ÖTV %" + QVariant  ( M_OTV_ORANLARI_ARRAY [i] ).toString() );
            M_OTV_HESAPLARI_SATIS_LABEL_ARRAY[i]->setText ( "ÖTV %" + QVariant  ( M_OTV_ORANLARI_ARRAY [i] ).toString() );

            if( M_OTV_ORANLARI_ARRAY[i] EQ 0.00 ) {

                M_OTV_HESAPLARI_ALIS_LABEL_ARRAY        [i]->setDisabled    ( true );
                M_OTV_HESAPLARI_ALIS_SEARCHEDIT_ARRAY   [i]->setDisabled    ( true );
                M_OTV_HESAPLARI_ALIS_LINEEDIT_ARRAY     [i]->setDisabled    ( true );

                M_OTV_HESAPLARI_SATIS_LABEL_ARRAY       [i]->setDisabled    ( true );
                M_OTV_HESAPLARI_SATIS_SEARCHEDIT_ARRAY  [i]->setDisabled    ( true );
                M_OTV_HESAPLARI_SATIS_LINEEDIT_ARRAY    [i]->setDisabled    ( true );
            }
        }

    }

    SQL_QUERY query ( DB );

    query.PREPARE_SELECT ( "muh_hesaplar"," hesap_id, hesap_ismi,tali_hesap", "tam_hesap_kodu = :tam_hesap_kodu" );

    for ( int i = 0; i < KDV_OTV_ORANLARI_SAYISI; i ++ ) {

        if ( p_object EQ M_IND_KDV_HESAPLARI_SEARCHEDIT_ARRAY[i] ) {

            if ( M_IND_KDV_HESAPLARI_SEARCHEDIT_ARRAY [i]->GET_TEXT().isEmpty() EQ true ) {
                M_IND_KDV_HESAPLARI_ID_ARRAY        [i] = 0;
                M_IND_KDV_HESAPLARI_LINEEDIT_ARRAY  [i]->clear();
                return ADAK_OK;
            }

            query.SET_VALUE ( ":tam_hesap_kodu", M_IND_KDV_HESAPLARI_SEARCHEDIT_ARRAY[i]->GET_TEXT() );

            if ( query.SELECT() EQ 0 ) {
                MSG_WARNING(  tr("Hesap bulunamadı"), M_IND_KDV_HESAPLARI_SEARCHEDIT_ARRAY[i] );

                return ADAK_FAIL_UNDO;
            }

            query.NEXT ();

            int         tali_hesap      =   query.VALUE ( "tali_hesap" ).toInt();
            int         hesap_id        =   query.VALUE ( "hesap_id"   ).toInt();
            QString     hesap_ismi      =   query.VALUE ( "hesap_ismi" ).toString();

            if ( tali_hesap EQ 0 ) {
                MSG_WARNING(  tr ( "Seçilen hesap tali değil, tali bir hesap seçmelisiniz." ), M_IND_KDV_HESAPLARI_IADE_SEARCHEDIT_ARRAY[i] );

                return ADAK_FAIL_UNDO;
            }

            M_IND_KDV_HESAPLARI_ID_ARRAY        [i]     = hesap_id;
            M_IND_KDV_HESAPLARI_LINEEDIT_ARRAY  [i]->setText ( hesap_ismi );

            return ADAK_OK;

        }
        else if ( p_object EQ M_HES_KDV_HESAPLARI_SEARCHEDIT_ARRAY [i] ) {

            if ( M_HES_KDV_HESAPLARI_SEARCHEDIT_ARRAY [i]->GET_TEXT().isEmpty() EQ true ) {
                M_HES_KDV_HESAPLARI_ID_ARRAY        [i] = 0;
                M_HES_KDV_HESAPLARI_LINEEDIT_ARRAY  [i]->clear();
                return ADAK_OK;
            }

            query.SET_VALUE ( ":tam_hesap_kodu", M_HES_KDV_HESAPLARI_SEARCHEDIT_ARRAY[i]->GET_TEXT() );

            if ( query.SELECT() EQ 0 ) {
                MSG_WARNING(  tr("Hesap bulunamadı"), M_IND_KDV_HESAPLARI_IADE_SEARCHEDIT_ARRAY[i] );

                return ADAK_FAIL_UNDO;
            }

            query.NEXT ();

            int         tali_hesap      =   query.VALUE ( "tali_hesap" ).toInt();
            int         hesap_id        =   query.VALUE ( "hesap_id"   ).toInt();
            QString     hesap_ismi      =   query.VALUE ( "hesap_ismi" ).toString();

            if ( tali_hesap EQ 0 ) {
                MSG_WARNING(  tr ( "Seçilen hesap tali değil, tali bir hesap seçmelisiniz." ), M_IND_KDV_HESAPLARI_IADE_SEARCHEDIT_ARRAY[i] );

                return ADAK_FAIL_UNDO;
            }

            M_HES_KDV_HESAPLARI_ID_ARRAY        [i]     = hesap_id;
            M_HES_KDV_HESAPLARI_LINEEDIT_ARRAY  [i]->setText ( hesap_ismi );

            return ADAK_OK;

        }
        else if ( p_object EQ M_IND_KDV_HESAPLARI_IADE_SEARCHEDIT_ARRAY[i] ) {

            if ( M_IND_KDV_HESAPLARI_IADE_SEARCHEDIT_ARRAY [i]->GET_TEXT().isEmpty() EQ true ) {
                M_IND_KDV_IADE_HESAPLARI_ID_ARRAY        [i] = 0;
                M_IND_KDV_HESAPLARI_IADE_LINEEDIT_ARRAY  [i]->clear();
                return ADAK_OK;
            }

            query.SET_VALUE ( ":tam_hesap_kodu", M_IND_KDV_HESAPLARI_IADE_SEARCHEDIT_ARRAY[i]->GET_TEXT() );

            if ( query.SELECT() EQ 0 ) {
                MSG_WARNING(  tr("Hesap bulunamadı"), M_IND_KDV_HESAPLARI_IADE_SEARCHEDIT_ARRAY[i] );

                return ADAK_FAIL_UNDO;
            }

            query.NEXT ();

            int         tali_hesap      =   query.VALUE ( "tali_hesap" ).toInt();
            int         hesap_id        =   query.VALUE ( "hesap_id"   ).toInt();
            QString     hesap_ismi      =   query.VALUE ( "hesap_ismi" ).toString();

            if ( tali_hesap EQ 0 ) {
                MSG_WARNING(  tr ( "Seçilen hesap tali değil, tali bir hesap seçmelisiniz." ), M_IND_KDV_HESAPLARI_IADE_SEARCHEDIT_ARRAY[i] );

                return ADAK_FAIL_UNDO;
            }

            M_IND_KDV_IADE_HESAPLARI_ID_ARRAY        [i]     = hesap_id;
            M_IND_KDV_HESAPLARI_IADE_LINEEDIT_ARRAY  [i]->setText ( hesap_ismi );

            return ADAK_OK;

        }
        else if ( p_object EQ M_HES_KDV_HESAPLARI_IADE_SEARCHEDIT_ARRAY [i] ) {

            if ( M_HES_KDV_HESAPLARI_IADE_SEARCHEDIT_ARRAY [i]->GET_TEXT().isEmpty() EQ true ) {
                M_HES_KDV_IADE_HESAPLARI_ID_ARRAY        [i] = 0;
                M_HES_KDV_HESAPLARI_IADE_LINEEDIT_ARRAY  [i]->clear();
                return ADAK_OK;
            }

            query.SET_VALUE ( ":tam_hesap_kodu", M_HES_KDV_HESAPLARI_IADE_SEARCHEDIT_ARRAY[i]->GET_TEXT() );

            if ( query.SELECT() EQ 0 ) {
                MSG_WARNING(  tr("Hesap bulunamadı"), M_IND_KDV_HESAPLARI_IADE_SEARCHEDIT_ARRAY[i] );

                return ADAK_FAIL_UNDO;
            }

            query.NEXT ();

            int         tali_hesap      =   query.VALUE ( "tali_hesap" ).toInt();
            int         hesap_id        =   query.VALUE ( "hesap_id"   ).toInt();
            QString     hesap_ismi      =   query.VALUE ( "hesap_ismi" ).toString();

            if ( tali_hesap EQ 0 ) {
                MSG_WARNING(  tr ( "Seçilen hesap tali değil, tali bir hesap seçmelisiniz." ), M_IND_KDV_HESAPLARI_IADE_SEARCHEDIT_ARRAY[i] );

                return ADAK_FAIL_UNDO;
            }

            M_HES_KDV_IADE_HESAPLARI_ID_ARRAY        [i]     = hesap_id;
            M_HES_KDV_HESAPLARI_IADE_LINEEDIT_ARRAY  [i]->setText ( hesap_ismi );

            return ADAK_OK;

        }
        else if ( p_object EQ M_OTV_HESAPLARI_ALIS_SEARCHEDIT_ARRAY [i] ) {

            if ( M_OTV_HESAPLARI_ALIS_SEARCHEDIT_ARRAY [i]->GET_TEXT().isEmpty() EQ true ) {
                M_OTV_HESAPLARI_ALIS_ID_ARRAY            [i] = 0;
                M_OTV_HESAPLARI_ALIS_LINEEDIT_ARRAY      [i]->clear();
                return ADAK_OK;
            }

            query.SET_VALUE ( ":tam_hesap_kodu", M_OTV_HESAPLARI_ALIS_SEARCHEDIT_ARRAY[i]->GET_TEXT() );

            if ( query.SELECT() EQ 0 ) {
                MSG_WARNING(  tr("Hesap bulunamadı"), M_OTV_HESAPLARI_ALIS_SEARCHEDIT_ARRAY[i] );

                return ADAK_FAIL_UNDO;
            }

            query.NEXT ();

            int         tali_hesap      =   query.VALUE ( "tali_hesap" ).toInt();
            int         hesap_id        =   query.VALUE ( "hesap_id"   ).toInt();
            QString     hesap_ismi      =   query.VALUE ( "hesap_ismi" ).toString();

            if ( tali_hesap EQ 0 ) {
                MSG_WARNING(  tr ( "Seçilen hesap tali değil, tali bir hesap seçmelisiniz." ), M_OTV_HESAPLARI_ALIS_SEARCHEDIT_ARRAY[i] );

                return ADAK_FAIL_UNDO;
            }

            M_OTV_HESAPLARI_ALIS_ID_ARRAY        [i]     = hesap_id;
            M_OTV_HESAPLARI_ALIS_LINEEDIT_ARRAY  [i]->setText ( hesap_ismi );

            return ADAK_OK;

        }
        else if ( p_object EQ M_OTV_HESAPLARI_SATIS_SEARCHEDIT_ARRAY [i] ) {

            if ( M_OTV_HESAPLARI_SATIS_SEARCHEDIT_ARRAY [i]->GET_TEXT().isEmpty() EQ true ) {
                M_OTV_HESAPLARI_SATIS_ID_ARRAY            [i] = 0;
                M_OTV_HESAPLARI_SATIS_LINEEDIT_ARRAY      [i]->clear();
                return ADAK_OK;
            }

            query.SET_VALUE ( ":tam_hesap_kodu", M_OTV_HESAPLARI_SATIS_SEARCHEDIT_ARRAY[i]->GET_TEXT() );

            if ( query.SELECT() EQ 0 ) {
                MSG_WARNING(  tr("Hesap bulunamadı"), M_OTV_HESAPLARI_SATIS_SEARCHEDIT_ARRAY[i] );

                return ADAK_FAIL_UNDO;
            }

            query.NEXT ();

            int         tali_hesap      =   query.VALUE ( "tali_hesap" ).toInt();
            int         hesap_id        =   query.VALUE ( "hesap_id"   ).toInt();
            QString     hesap_ismi      =   query.VALUE ( "hesap_ismi" ).toString();

            if ( tali_hesap EQ 0 ) {
                MSG_WARNING(  tr ( "Seçilen hesap tali değil, tali bir hesap seçmelisiniz." ), M_OTV_HESAPLARI_SATIS_SEARCHEDIT_ARRAY[i] );

                return ADAK_FAIL_UNDO;
            }

            M_OTV_HESAPLARI_SATIS_ID_ARRAY        [i]     = hesap_id;
            M_OTV_HESAPLARI_SATIS_LINEEDIT_ARRAY  [i]->setText ( hesap_ismi );

            return ADAK_OK;

        }
        else if ( p_object EQ M_TEVKIFAT_KDV_HESAPLARI_SEARCHEDIT_ARRAY [i] ) {

            if ( M_TEVKIFAT_KDV_HESAPLARI_SEARCHEDIT_ARRAY [i]->GET_TEXT().isEmpty() EQ true ) {
                M_TEVKIFAT_KDV_HESAPLARI_ID_ARRAY            [i] = 0;
                M_TEVKIFAT_KDV_HESAPLARI_LINEEDIT_ARRAY      [i]->clear();
                return ADAK_OK;
            }

            query.SET_VALUE ( ":tam_hesap_kodu", M_TEVKIFAT_KDV_HESAPLARI_SEARCHEDIT_ARRAY[i]->GET_TEXT() );

            if ( query.SELECT() EQ 0 ) {
                MSG_WARNING(  tr("Hesap bulunamadı"), M_TEVKIFAT_KDV_HESAPLARI_SEARCHEDIT_ARRAY[i] );

                return ADAK_FAIL_UNDO;
            }

            query.NEXT ();

            int         tali_hesap      =   query.VALUE ( "tali_hesap" ).toInt();
            int         hesap_id        =   query.VALUE ( "hesap_id"   ).toInt();
            QString     hesap_ismi      =   query.VALUE ( "hesap_ismi" ).toString();

            if ( tali_hesap EQ 0 ) {
                MSG_WARNING(  tr ( "Seçilen hesap tali değil, tali bir hesap seçmelisiniz." ), M_TEVKIFAT_KDV_HESAPLARI_SEARCHEDIT_ARRAY[i] );

                return ADAK_FAIL_UNDO;
            }

            M_TEVKIFAT_KDV_HESAPLARI_ID_ARRAY        [i]     = hesap_id;
            M_TEVKIFAT_KDV_HESAPLARI_LINEEDIT_ARRAY  [i]->setText ( hesap_ismi );

            return ADAK_OK;
        }
    }

    // en son hangi tab'da idi
    m_onceki_tab_index = m_ui->tabWidget_sabit_degerler->currentIndex();

    ADRES_BILGILERINI_OLUSTUR();

    return ADAK_OK;
}

/*************************************************************************************************************
                                    E9_SABIT_DEGERLER_FORMU::CHECK_EMPTY
*************************************************************************************************************/

int E9_SABIT_DEGERLER_FORMU::CHECK_EMPTY ()
{
    if ( m_ui->combobox_para_birimi->currentText().isEmpty() EQ true ) {
        MSG_WARNING(  tr("Temel para birimini tanımlamalısınız!.."), NULL);
        return ADAK_FAIL;
    }

    if ( m_ui->commaEdit_ciro_sonrasi_gun_sayisi->GET_INTEGER() EQ 0 ) {
        MSG_WARNING( tr("Ciro Edilen çeklerin, ödenmiş kabul edilecekleri gün sayısını boş bırakamazsınız."), NULL);
        return ADAK_FAIL;
    }

    if ( m_ui->comboBox_amor_yontemi_degisebilir_mi->currentIndex() EQ 0 ) {
        if ( m_ui->comboBox_amortisman_yontemi->currentIndex() EQ -1 ) {
            MSG_WARNING( tr("Demirbaşların genelinde uygulanacak amortisman ayırma yöntemini seçmelisiniz!.."), NULL);
            return ADAK_FAIL;
        }
    }

    if ( m_ui->commaEdit_irs_kac_gunde_faturalanacak->GET_INTEGER() EQ 0 ) {
        MSG_WARNING( tr("Faturalandırılacak gün sayısını girmelisiniz."), NULL );
        return ADAK_FAIL;
    }

    m_kdv_orani_sayisi = 0;

    for ( int i = 0; i < KDV_OTV_ORANLARI_SAYISI; i++ ) {
        if ( M_KDV_ORANLARI_COMMAEDIT_ARRAY[i]->GET_TEXT().isEmpty() EQ true ) {
            break;
        }
        M_KDV_ORANLARI_ARRAY[i] = M_KDV_ORANLARI_COMMAEDIT_ARRAY[i]->GET_DOUBLE();
        m_kdv_orani_sayisi++;
    }

    m_otv_orani_sayisi = 0;
    for ( int i = 0; i < KDV_OTV_ORANLARI_SAYISI; i++ ) {
        if ( M_OTV_ORANLARI_COMMAEDIT_ARRAY[i]->GET_TEXT().isEmpty() EQ true ) {
            break;
        }
        M_OTV_ORANLARI_ARRAY[i] = M_OTV_ORANLARI_COMMAEDIT_ARRAY[i]->GET_DOUBLE();
        m_otv_orani_sayisi++;
    }

    for ( int i = 0 ; i < m_kdv_orani_sayisi ; i++ ) {

        if ( M_IND_KDV_HESAPLARI_SEARCHEDIT_ARRAY[i]->GET_TEXT().isEmpty() EQ true ) {
            if ( M_IND_KDV_HESAPLARI_SEARCHEDIT_ARRAY[i]->isEnabled() EQ true ) {
                MSG_INFO(QString("%1 İndirilecek KDV Hesabını boş bırakamazsınız.").arg(M_IND_KDV_HESAPLARI_LABEL_ARRAY[i]->text().remove("KDV")),
                         M_IND_KDV_HESAPLARI_SEARCHEDIT_ARRAY[i]);
                return ADAK_FAIL;
            }
        }
        if ( M_HES_KDV_HESAPLARI_SEARCHEDIT_ARRAY[i]->GET_TEXT().isEmpty() EQ true ) {
            if ( M_HES_KDV_HESAPLARI_SEARCHEDIT_ARRAY[i]->isEnabled() EQ true ) {
                MSG_INFO(QString("%1 Hesaplanan KDV Hesabını boş bırakamazsınız.").arg(M_HES_KDV_HESAPLARI_LABEL_ARRAY[i]->text().remove("KDV")),
                         M_HES_KDV_HESAPLARI_SEARCHEDIT_ARRAY[i]);
                return ADAK_FAIL;
            }
        }
        if ( M_IND_KDV_HESAPLARI_IADE_SEARCHEDIT_ARRAY[i]->GET_TEXT().isEmpty() EQ true ) {
            if ( M_IND_KDV_HESAPLARI_IADE_SEARCHEDIT_ARRAY[i]->isEnabled() EQ true ) {
                MSG_INFO(QString("%1 İndirilecek KDV ( İade ) Hesabını boş bırakamazsınız.").arg(M_IND_KDV_HESAPLARI_IADE_LABEL_ARRAY[i]->text().remove("KDV")),
                         M_IND_KDV_HESAPLARI_IADE_SEARCHEDIT_ARRAY[i]);
                return ADAK_FAIL;
            }
        }
        if ( M_HES_KDV_HESAPLARI_IADE_SEARCHEDIT_ARRAY[i]->GET_TEXT().isEmpty() EQ true ) {
            if ( M_HES_KDV_HESAPLARI_IADE_SEARCHEDIT_ARRAY[i]->isEnabled() EQ true ) {
                MSG_INFO(QString("%1 Hesaplanan KDV ( İade ) Hesabını boş bırakamazsınız.").arg(M_HES_KDV_HESAPLARI_IADE_LABEL_ARRAY[i]->text().remove("KDV")),
                         M_HES_KDV_HESAPLARI_IADE_SEARCHEDIT_ARRAY[i]);
                return ADAK_FAIL;
            }
        }

        if ( M_TEVKIFAT_KDV_HESAPLARI_SEARCHEDIT_ARRAY[i]->GET_TEXT().isEmpty() EQ true ) {
            if ( M_TEVKIFAT_KDV_HESAPLARI_SEARCHEDIT_ARRAY[i]->isEnabled() EQ true ) {
                MSG_INFO(QString("%1 Tevkifat KDV Hesabını boş bırakamazsınız.").arg(M_TEVKIFAT_KDV_HESAPLARI_LABEL_ARRAY[i]->text().remove("KDV")),
                         M_TEVKIFAT_KDV_HESAPLARI_SEARCHEDIT_ARRAY[i]);
                return ADAK_FAIL;
            }
        }
    }
    for ( int i = 0 ; i < m_otv_orani_sayisi ; i++ ) {
        if ( M_OTV_HESAPLARI_ALIS_SEARCHEDIT_ARRAY[i]->GET_TEXT().isEmpty() EQ true ) {
            if ( M_OTV_HESAPLARI_ALIS_SEARCHEDIT_ARRAY[i]->isEnabled() EQ true ) {
                MSG_INFO(QString("%1 Ötv Alış Hesabını boş bırakamazsınız.").arg(M_OTV_HESAPLARI_ALIS_LABEL_ARRAY[i]->text().remove("ÖTV")),
                         M_OTV_HESAPLARI_ALIS_SEARCHEDIT_ARRAY[i]);
                return ADAK_FAIL;
            }
        }
        if ( M_OTV_HESAPLARI_SATIS_SEARCHEDIT_ARRAY[i]->GET_TEXT().isEmpty() EQ true ) {
            if ( M_OTV_HESAPLARI_SATIS_SEARCHEDIT_ARRAY[i]->isEnabled() EQ true ) {
                MSG_INFO(QString("%1 Ötv Satış Hesabını boş bırakamazsınız.").arg(M_OTV_HESAPLARI_SATIS_LABEL_ARRAY[i]->text().remove("ÖTV")),
                         M_OTV_HESAPLARI_SATIS_SEARCHEDIT_ARRAY[i]);
                return ADAK_FAIL;
            }
        }
    }

    return ADAK_OK;
}

/*************************************************************************************************************
                                    E9_SABIT_DEGERLER_FORMU::CHECK_ADD
*************************************************************************************************************/

int E9_SABIT_DEGERLER_FORMU::CHECK_ADD ()
{
    return ADAK_OK;
}

/*************************************************************************************************************
                                    E9_SABIT_DEGERLER_FORMU::ADD_RECORD
*************************************************************************************************************/

int E9_SABIT_DEGERLER_FORMU::ADD_RECORD ()
{
    return 0;
}

/*************************************************************************************************************
                                    E9_SABIT_DEGERLER_FORMU::CHECK_UPDATE
*************************************************************************************************************/

int E9_SABIT_DEGERLER_FORMU::CHECK_UPDATE ( int p_record_id )
{
    m_e9_ayraci_guncellenecek = 0;
    m_mali_yil_degisti        = 0;

    SQL_QUERY sql_query ( DB );

    sql_query.PREPARE_SELECT("e9_sabit_degerler",
                             "e9_ayraci ",
                             "sabit_deger_id = :sabit_deger_id");
    sql_query.SET_VALUE(":sabit_deger_id", p_record_id );

    if ( sql_query.SELECT() EQ 0 ) {
        MSG_WARNING(  tr("Muhasebe ayracı bulunamadı"), NULL);
        return ADAK_FAIL;
    }

    sql_query.NEXT();

    QString old_e9_ayraci = sql_query.VALUE(0).toString();

    if ( old_e9_ayraci NE m_ui->combobox_e9_ayraci->currentText() ) {

        int clicked_button = MSG_YES_NO( tr("Bu işlem tüm alt hesaplarınızın tam hesap kodlarında "
                                                                            "ayraç değişikliğe neden olacaktır."
                                                                            "\n Emin misiniz? "), NULL );
        if ( clicked_button EQ ADAK_NO ) {
            return ADAK_FAIL;
        }

        m_e9_ayraci_guncellenecek = 1;
        m_old_e9_ayraci           = old_e9_ayraci;
    }

    sql_query.PREPARE_SELECT("e9_sabit_degerler", "mali_yil_ilk_tarih");
    if ( sql_query.SELECT() EQ 0 ) {
        MSG_WARNING(  tr("Mali yıl veritabanından alınamadı"), NULL );
        return ADAK_FAIL;
    }
    sql_query.NEXT();
    if ( sql_query.VALUE(0).toString() NE m_ui->adakDate_mali_yil_baslangic_tarihi->DATE() ) {
        sql_query.PREPARE_SELECT( "muh_fisler", "fis_id");
        if ( sql_query.SELECT() > 0 ) {
            MSG_WARNING(  tr("Mali yıla ait muhasebe fişi girilmiş. Mali yılı değiştiremezsiniz."), NULL );
            return ADAK_FAIL;
        }
    }


    return ADAK_OK;
}

/*************************************************************************************************************
                                    E9_SABIT_DEGERLER_FORMU::UPDATE_RECORD
*************************************************************************************************************/

void E9_SABIT_DEGERLER_FORMU::UPDATE_RECORD ( int p_record_id )
{

    SQL_QUERY sql_query ( DB );

    QString telefon                 = m_ui->lineEdit_telefon->text().replace("-", "");
    QString fax                     = m_ui->lineEdit_fax->text().replace("-", "");

    QDate mali_yil_ilk_tarih        = m_ui->adakDate_mali_yil_baslangic_tarihi->QDATE();

    int mali_yil_son_tarih_yil      = mali_yil_ilk_tarih.year() + 1;
    int mali_yil_son_tarih_ay       = mali_yil_ilk_tarih.month();
    int mali_yil_son_tarih_gun      = mali_yil_ilk_tarih.day();

    QDate mali_yil_son_tarih        = QDate ( mali_yil_son_tarih_yil, mali_yil_son_tarih_ay, mali_yil_son_tarih_gun  );
    mali_yil_son_tarih              = mali_yil_son_tarih.addDays ( -1 );

    QDate e9_array_son_tarih        = mali_yil_ilk_tarih.addDays ( E9_ARRAY_SIZE - 2 );

    if ( m_e9_ayraci_guncellenecek NE 0 ) {
        E9_DELIMETER_UPDATE ( m_old_e9_ayraci, m_ui->combobox_e9_ayraci->currentText(), this );
    }
    QString firma_web_adresi = m_ui->lineedit_web_adresi->text();
    QString ilce             = m_ui->comboBox_ilce->currentText();


    QString     ind_kdv_hesap_id_array          =   PACK_INT_ARRAY ( M_IND_KDV_HESAPLARI_ID_ARRAY,      KDV_OTV_ORANLARI_SAYISI );
    QString     ind_kdv_iade_hesap_id_array     =   PACK_INT_ARRAY ( M_IND_KDV_IADE_HESAPLARI_ID_ARRAY, KDV_OTV_ORANLARI_SAYISI );
    QString     hes_kdv_hesap_id_array          =   PACK_INT_ARRAY ( M_HES_KDV_HESAPLARI_ID_ARRAY,      KDV_OTV_ORANLARI_SAYISI );
    QString     hes_kdv_iade_hesap_id_array     =   PACK_INT_ARRAY ( M_HES_KDV_IADE_HESAPLARI_ID_ARRAY, KDV_OTV_ORANLARI_SAYISI );
    QString     otv_alis_hesap_id_array         =   PACK_INT_ARRAY ( M_OTV_HESAPLARI_ALIS_ID_ARRAY,     KDV_OTV_ORANLARI_SAYISI );
    QString     otv_satis_hesap_id_array        =   PACK_INT_ARRAY ( M_OTV_HESAPLARI_SATIS_ID_ARRAY,    KDV_OTV_ORANLARI_SAYISI );
    QString     tevkifat_kdv_hesap_id_array     =   PACK_INT_ARRAY ( M_TEVKIFAT_KDV_HESAPLARI_ID_ARRAY, KDV_OTV_ORANLARI_SAYISI );


    sql_query.PREPARE_UPDATE("e9_sabit_degerler",
                             "sabit_deger_id",
                             "cek_ciro_sonrasi_gun_sayisi         , "
                             "dmr_amortisman_ayirma_yontemi       , "
                             "dmr_amor_yntemi_degisebilir_mi      , "
                             "dmr_max_amortisman_orani            , "
                             "fat_acik_fatura_mi_kapali_mi        , "
                             "irs_faturalandirilacak_gun          , "
                             "firma_ismi                          , "
                             "firma_yetkilisi                     , "
                             "firma_adres_satiri_1                , "
                             "firma_adres_satiri_2                , "
                             "firma_adres_satiri_3                , "
                             "firma_ulke                          , "
                             "firma_sehir                         , "
                             "firma_ilce                          , "
                             "firma_semt                          , "
                             "firma_posta_kodu                    , "
                             "firma_tel_ulke_kodu                 , "
                             "firma_fax_ulke_kodu                 , "
                             "firma_tel_alan_kodu                 , "
                             "firma_fax_alan_kodu                 , "
                             "firma_telefon                       , "
                             "firma_fax                           , "
                             "firma_email                         , "
                             "firma_web_adresi                    , "
                             "firma_vergi_dairesi                 , "
                             "firma_vergi_numarasi                , "
                             "firma_aciklama                      , "
                             "mali_yil_ilk_tarih                  , "
                             "mali_yil_son_tarih                  , "
                             "e9_array_son_tarih                  , "
                             "e9_ayraci                           , "
                             "ontanimli_kdv_orani                 , "
                             "kdv_orani_sayisi                    , "
                             "otv_orani_sayisi                    , "
                             "kdv_oranlari_array                  , "
                             "otv_oranlari_array                  , "
                             "ind_kdv_hesap_id_array              , "
                             "ind_kdv_iade_hesap_id_array         , "
                             "hes_kdv_hesap_id_array              , "
                             "hes_kdv_iade_hesap_id_array         , "
                             "otv_alis_hesap_id_array             , "
                             "otv_satis_hesap_id_array            , "
                             "tevkifat_kdv_hesap_id_array         , "
                             "muhasebe_alarmlari_kapalimi         , "
                             "sm_sicil_numarasi                   , "
                             "stok_birim_fiyat_hassasiyeti        , "
                             "stok_birim_miktar_hassasiyeti         ",
                             "sabit_deger_id = :sabit_deger_id   " );

    sql_query.SET_VALUE ( ":cek_ciro_sonrasi_gun_sayisi",               m_ui->commaEdit_ciro_sonrasi_gun_sayisi->GET_INTEGER()                                              );
    sql_query.SET_VALUE ( ":dmr_amortisman_ayirma_yontemi",             DMR_GET_AMORTISMAN_SEKLI_ENUM(m_ui->comboBox_amortisman_yontemi->currentText())                         );
    sql_query.SET_VALUE ( ":dmr_amor_yntemi_degisebilir_mi",           m_ui->comboBox_amor_yontemi_degisebilir_mi->currentIndex()                                           );
    sql_query.SET_VALUE ( ":dmr_max_amortisman_orani",                  m_ui->commaEdit_max_amortisman_orani->GET_DOUBLE()                                                  );
    sql_query.SET_VALUE ( ":fat_acik_fatura_mi_kapali_mi",              FAT_GET_ACIK_KAPALI_FATURA_ENUM ( m_ui->comboBox_acik_fatura_mi->currentText())                         );
    sql_query.SET_VALUE ( ":irs_faturalandirilacak_gun",                m_ui->commaEdit_irs_kac_gunde_faturalanacak->GET_INTEGER()                                          );
    sql_query.SET_VALUE ( ":firma_ismi",                                m_ui->lineEdit_firma_ismi->text()                                                                   );
    sql_query.SET_VALUE ( ":firma_yetkilisi",                           m_ui->lineedit_yetkili->text()                                                                      );
    sql_query.SET_VALUE ( ":firma_adres_satiri_1",                      m_ui->lineEdit_adres_satiri_1->text()                                                               );
    sql_query.SET_VALUE ( ":firma_adres_satiri_2",                      m_ui->lineEdit_adres_satiri_2->text()                                                               );
    sql_query.SET_VALUE ( ":firma_adres_satiri_3",                      m_ui->lineEdit_adres_satiri_3->text()                                                               );
    sql_query.SET_VALUE ( ":firma_ulke",                                m_ui->comboBox_ulke->currentText()                                                                  );
    sql_query.SET_VALUE ( ":firma_sehir",                               m_ui->comboBox_sehir->currentText()                                                                 );
    sql_query.SET_VALUE ( ":firma_ilce",                                ilce                                                                                                );
    sql_query.SET_VALUE ( ":firma_semt",                                m_ui->lineEdit_semt->text()                                                                         );
    sql_query.SET_VALUE ( ":firma_posta_kodu",                          m_ui->lineEdit_posta_kodu->text()                                                              );
    sql_query.SET_VALUE ( ":firma_tel_ulke_kodu",                       m_ui->comboBox_telefon_ulke_kodu->currentText()                                                     );
    sql_query.SET_VALUE ( ":firma_fax_ulke_kodu",                       m_ui->comboBox_fax_ulke_kodu->currentText()                                                         );
    sql_query.SET_VALUE ( ":firma_tel_alan_kodu",                       m_ui->comboBox_telefon_alan_kodu->currentText()                                                     );
    sql_query.SET_VALUE ( ":firma_fax_alan_kodu",                       m_ui->comboBox_fax_alan_kodu->currentText()                                                         );
    sql_query.SET_VALUE ( ":firma_telefon",                             telefon                                                                                             );
    sql_query.SET_VALUE ( ":firma_fax",                                 fax                                                                                                 );
    sql_query.SET_VALUE ( ":firma_email",                               m_ui->lineedit_mail->text()                                                                         );
    sql_query.SET_VALUE ( ":firma_web_adresi",                          firma_web_adresi                                                                                    );
    sql_query.SET_VALUE ( ":firma_vergi_dairesi",                       m_ui->comboBox_vergi_dairesi->currentText()                                                         );
    sql_query.SET_VALUE ( ":firma_vergi_numarasi",                      m_ui->lineEdit_vergi_numarasi->text()                                                         );
    sql_query.SET_VALUE ( ":firma_aciklama",                            m_ui->limitedTextEdit_aciklama->toPlainText()                                                       );
    sql_query.SET_VALUE ( ":mali_yil_ilk_tarih",                        mali_yil_ilk_tarih.toString("yyyy.MM.dd")                                                           );
    sql_query.SET_VALUE ( ":mali_yil_son_tarih",                        mali_yil_son_tarih.toString("yyyy.MM.dd")                                                           );
    sql_query.SET_VALUE ( ":e9_array_son_tarih",                        e9_array_son_tarih.toString("yyyy.MM.dd")                                                           );
    sql_query.SET_VALUE ( ":e9_ayraci",                                 m_ui->combobox_e9_ayraci->currentText()                                                             );
    sql_query.SET_VALUE ( ":ontanimli_kdv_orani",                       QVariant(m_ui->comboBox_ontanimli_kdv_orani->currentText()).toDouble()                              );
    sql_query.SET_VALUE ( ":kdv_orani_sayisi",                          m_kdv_orani_sayisi                                                                                  );
    sql_query.SET_VALUE ( ":otv_orani_sayisi",                          m_otv_orani_sayisi                                                                                  );
    sql_query.SET_VALUE ( ":kdv_oranlari_array",                        PACK_DOUBLE_ARRAY ( M_KDV_ORANLARI_ARRAY, KDV_OTV_ORANLARI_SAYISI )                                 );
    sql_query.SET_VALUE ( ":otv_oranlari_array",                        PACK_DOUBLE_ARRAY ( M_OTV_ORANLARI_ARRAY, KDV_OTV_ORANLARI_SAYISI )                                 );
    sql_query.SET_VALUE ( ":ind_kdv_hesap_id_array",                    ind_kdv_hesap_id_array                                                                              );
    sql_query.SET_VALUE ( ":ind_kdv_iade_hesap_id_array",               ind_kdv_iade_hesap_id_array                                                                         );
    sql_query.SET_VALUE ( ":hes_kdv_hesap_id_array",                    hes_kdv_hesap_id_array                                                                              );
    sql_query.SET_VALUE ( ":hes_kdv_iade_hesap_id_array",               hes_kdv_iade_hesap_id_array                                                                         );
    sql_query.SET_VALUE ( ":otv_alis_hesap_id_array",                   otv_alis_hesap_id_array                                                                             );
    sql_query.SET_VALUE ( ":otv_satis_hesap_id_array",                  otv_satis_hesap_id_array                                                                            );
    sql_query.SET_VALUE ( ":tevkifat_kdv_hesap_id_array",               tevkifat_kdv_hesap_id_array                                                                         );
    sql_query.SET_VALUE ( ":muhasebe_alarmlari_kapalimi"                , m_ui->combo_box_muh_alarmlari->currentIndex());
    sql_query.SET_VALUE ( ":sm_sicil_numarasi"                          , m_ui->lineEdit_sm_sicil_numarasi->text() );
    sql_query.SET_VALUE ( ":stok_birim_fiyat_hassasiyeti"               , m_ui->comboBox_stok_birim_hassasiyet->currentText().toInt() );
    sql_query.SET_VALUE ( ":stok_birim_miktar_hassasiyeti"              , m_ui->comboBox_stok_miktar_hassasiyet->currentText().toInt() );

    sql_query.SET_VALUE ( ":sabit_deger_id",                            p_record_id                                                                  );

    sql_query.UPDATE();

    E9_RESTORE_MUHASEBE_ALAMLARI_VALUE();

    E9_SET_ACIK_KAPALI_FATURA_SABIT_DEGER   (  FAT_GET_ACIK_KAPALI_FATURA_ENUM ( m_ui->comboBox_acik_fatura_mi->currentText() ) );

    if ( m_logo_path.isEmpty() EQ false ) {
        sql_query.PREPARE_UPDATE("e9_sabit_degerler",
                                 "sabit_deger_id",
                                 "firma_logosu",
                                 "sabit_deger_id = :sabit_deger_id");
        sql_query.SET_VALUE(":firma_logosu"   , ADAK_PIXMAP_2_BYTE_ARRAY(m_logo_path));
        sql_query.SET_VALUE(":sabit_deger_id" , p_record_id);
        sql_query.UPDATE();
    }

    SET_MALI_YIL_FIRST_DATE( mali_yil_ilk_tarih );

    DVZ_SET_TEMEL_PARA_BIRIMI_ID ( DVZ_GET_DOVIZ_ID ( m_ui->combobox_para_birimi->currentText()) );

    E9_SET_KDV_ORANLARI_OKUNDUMU_FLAG ( false );
    E9_SET_OTV_ORANLARI_OKUNDUMU_FLAG ( false );

    E9_SET_FIRMA_ISMI( m_ui->lineEdit_firma_ismi->text() );


    E9_LOG_KAYDI_EKLE (E9_YONETIM , LOG_E9_SABIT_DEGERLER , LOG_ISLEM_UPDATE , "Sabit Değerler");

}

/*************************************************************************************************************
                                    E9_SABIT_DEGERLER_FORMU::CHECK_DELETE
*************************************************************************************************************/

int E9_SABIT_DEGERLER_FORMU::CHECK_DELETE ( int p_record_id )
{
    Q_UNUSED ( p_record_id );
    return ADAK_OK;
}

/*************************************************************************************************************
                                    E9_SABIT_DEGERLER_FORMU::DELETE_RECORD
*************************************************************************************************************/

void E9_SABIT_DEGERLER_FORMU::DELETE_RECORD ( int p_record_id )
{
    Q_UNUSED ( p_record_id );
}

/*************************************************************************************************************
                                    E9_SABIT_DEGERLER_FORMU::FIND_FIRST_RECORD
*************************************************************************************************************/

int E9_SABIT_DEGERLER_FORMU::FIND_FIRST_RECORD ()
{
    return 0;
}

/*************************************************************************************************************
                                    E9_SABIT_DEGERLER_FORMU::FIND_LAST_RECORD
*************************************************************************************************************/

int E9_SABIT_DEGERLER_FORMU::FIND_LAST_RECORD ()
{
    return 0;
}

/*************************************************************************************************************
                                    E9_SABIT_DEGERLER_FORMU::FIND_NEXT_RECORD
*************************************************************************************************************/

int E9_SABIT_DEGERLER_FORMU::FIND_NEXT_RECORD ()
{
    return 0;
}

/*************************************************************************************************************
                                    E9_SABIT_DEGERLER_FORMU::FIND_PREV_RECORD
*************************************************************************************************************/

int E9_SABIT_DEGERLER_FORMU::FIND_PREV_RECORD ()
{
    return 0;
}

/*************************************************************************************************************
                                    E9_SABIT_DEGERLER_FORMU::SELECT_RECORD
*************************************************************************************************************/

int E9_SABIT_DEGERLER_FORMU::SELECT_RECORD()
{
    return 0;
}

/*************************************************************************************************************
                                    E9_SABIT_DEGERLER_FORMU::LOCK_RECORD
*************************************************************************************************************/

int E9_SABIT_DEGERLER_FORMU::LOCK_RECORD ( int p_record_id )
{
    return DB->LOCK_ROW ( "sabit_deger_id","e9_sabit_degerler", QString ( "sabit_deger_id = %1").arg ( p_record_id ) );
}

/*************************************************************************************************************
                                    E9_SABIT_DEGERLER_FORMU::FIND_FIRST_RECORD
*************************************************************************************************************/

void E9_SABIT_DEGERLER_FORMU::UNLOCK_RECORD ( int p_record_id )
{
    DB->UNLOCK_ROW ( "sabit_deger_id","e9_sabit_degerler", QString ( "sabit_deger_id = %1").arg ( p_record_id ) );
}

/*************************************************************************************************************
                                    E9_SABIT_DEGERLER_FORMU::FIND_RECORD
*************************************************************************************************************/

int E9_SABIT_DEGERLER_FORMU::FIND_RECORD ()
{
    return 0;
}

/*************************************************************************************************************
                            E9_SABIT_DEGERLER_FORMU::SET_WIDGET_DEFAULT_LENGTHS
*************************************************************************************************************/

void E9_SABIT_DEGERLER_FORMU::SET_WIDGET_DEFAULT_LENGTHS ()
{
    m_ui->lineEdit_firma_ismi->setMaxLength               ( 100 );
    m_ui->lineedit_yetkili->setMaxLength                  ( 50  );
    m_ui->lineEdit_adres_satiri_1->setMaxLength           ( 35  );
    m_ui->lineEdit_adres_satiri_2->setMaxLength           ( 35  );
    m_ui->lineEdit_adres_satiri_3->setMaxLength           ( 35  );
    m_ui->lineEdit_semt->setMaxLength                     ( 30  );
    m_ui->lineEdit_posta_kodu->setMaxLength               ( 10  );
    m_ui->lineEdit_vergi_numarasi->setMaxLength           ( 15  );
    m_ui->lineedit_web_adresi->setMaxLength               ( 50  );
    m_ui->lineedit_mail->setMaxLength                     ( 50  );
    m_ui->limitedTextEdit_aciklama->SET_MAX_STRING_LENGTH ( 512 );

    SET_COMBOBOX_LENGTH(m_ui->comboBox_acik_fatura_mi, 10 );
    SET_COMBOBOX_LENGTH(m_ui->comboBox_amortisman_yontemi,10);
    SET_COMBOBOX_LENGTH(m_ui->comboBox_amor_yontemi_degisebilir_mi,10);
    SET_COMBOBOX_LENGTH(m_ui->comboBox_fax_alan_kodu,10);
    SET_COMBOBOX_LENGTH(m_ui->comboBox_fax_ulke_kodu,10);
    SET_COMBOBOX_LENGTH(m_ui->comboBox_ilce,30);
    SET_COMBOBOX_LENGTH(m_ui->combobox_e9_ayraci,1);
    SET_COMBOBOX_LENGTH(m_ui->comboBox_ontanimli_kdv_orani,10);
    SET_COMBOBOX_LENGTH(m_ui->combobox_para_birimi,5);
    SET_COMBOBOX_LENGTH(m_ui->comboBox_sehir,30);
    SET_COMBOBOX_LENGTH(m_ui->comboBox_telefon_alan_kodu,10);
    SET_COMBOBOX_LENGTH(m_ui->comboBox_telefon_ulke_kodu,10);
    SET_COMBOBOX_LENGTH(m_ui->comboBox_ulke             ,30);
    SET_COMBOBOX_LENGTH(m_ui->comboBox_vergi_dairesi    ,50);

    m_ui->lineEdit_sm_sicil_numarasi->setMaxLength( 20 );
    m_ui->commaEdit_max_nakit_giris_cikis_tutari->setMaxLength( 15 );
}

/**************************************************************************/
/*               E9_SABIT_DEGERLER_FORMU::ADRES_BILGILERINI_OLUSTUR             */
/**************************************************************************/

void E9_SABIT_DEGERLER_FORMU::ADRES_BILGILERINI_OLUSTUR()
{
    QString adres_line_1 = "";
    QString adres_line_2 = "";
    QString adres_line_3 = "";
    QString adres_line_4 = "";

    if ( m_ui->lineEdit_adres_satiri_1->text().isEmpty() EQ false ) {
        adres_line_1 = m_ui->lineEdit_adres_satiri_1->text();
    }
    if ( m_ui->lineEdit_adres_satiri_2->text().isEmpty() EQ false ) {
        adres_line_2 = m_ui->lineEdit_adres_satiri_2->text();
    }
    if ( m_ui->lineEdit_adres_satiri_3->text().isEmpty() EQ false ) {
        adres_line_3 = m_ui->lineEdit_adres_satiri_3->text();
    }
    if ( m_ui->lineEdit_posta_kodu->text().isEmpty() EQ false ) {
        adres_line_3+= m_ui->lineEdit_posta_kodu->text() + " " ;
    }
    if ( m_ui->lineEdit_semt->text().isEmpty() EQ false ) {
        adres_line_3+= m_ui->lineEdit_semt->text() + " " ;
    }
    if ( m_ui->comboBox_ilce->currentText().isEmpty() EQ false ) {
        adres_line_3+= m_ui->comboBox_ilce->currentText() + " " ;
    }
    if ( m_ui->comboBox_sehir->currentText().isEmpty() EQ false ) {
        adres_line_4+= m_ui->comboBox_sehir->currentText() + " ";
    }
    if ( m_ui->comboBox_ulke->currentText().isEmpty() EQ false ) {
        adres_line_4+= m_ui->comboBox_ulke->currentText() + " ";
    }

    m_ui->textedit_adresi->blockSignals(true);
    m_ui->textedit_adresi->setText(adres_line_1 + "\n" + adres_line_2 + "\n" + adres_line_3 + "\n" + adres_line_4);
    m_ui->textedit_adresi->blockSignals(false);
}

/**************************************************************************/
/*               E9_SABIT_DEGERLER_FORMU::CHANGER_BUTTON_CLICKED();       */
/**************************************************************************/

int E9_SABIT_DEGERLER_FORMU::CHANGER_BUTTON_CLICKED(QAbstractButton *button)
{
    Q_UNUSED(button);

    return LOGO_SEC();

}
/**************************************************************************************
                   E9_SABIT_DEGERLER_FORMU::SEARCH_EDIT_CLICKED
***************************************************************************************/

void E9_SABIT_DEGERLER_FORMU::SEARCH_EDIT_CLICKED ( QWidget * p_widget, QLineEdit * p_lineEdit )
{
    Q_UNUSED ( p_widget );

    SQL_QUERY query ( DB );

    int hesap_id = OPEN_MUH_HESAP_ARAMA ( p_lineEdit->text(), this, 1 );

    if ( hesap_id < 1 ) {
        return;
    }

    query.PREPARE_SELECT ( "muh_hesaplar",
                           "tam_hesap_kodu, tali_hesap",
                           "hesap_id = :hesap_id" );
    query.SET_VALUE      ( ":hesap_id", hesap_id );

    if ( query.SELECT() EQ 0 ) {
        MSG_WARNING(  tr ( "Hesap bulunamadı" ), NULL );

        return;
    }

    query.NEXT();

    if ( query.VALUE (1).toInt() EQ 0 ) {
        MSG_WARNING(  tr ( "Seçtiğiniz hesap tali değil" ), NULL );
        return;
    }

    p_lineEdit->setText( query.VALUE (0).toString() );
}

/**************************************************************************/
/*               E9_SABIT_DEGERLER_FORMU::LOGO_SEC()                      */
/**************************************************************************/

int E9_SABIT_DEGERLER_FORMU::LOGO_SEC()
{
    ADAK_ADD_PIXMAP_TO_BUTTON(m_ui->pushButton_logo,&m_logo_path,this);

    m_ui->pushButton_logo->setText("");

    if ( m_logo_path.isEmpty() EQ true ) {
        return ADAK_RECORD_UNCHANGED;
    }
    return ADAK_RECORD_CHANGED;
}
/**************************************************************************************
                   E9_SABIT_DEGERLER_FORMU::WIDGET_DIZILERINI_DUZENLE
***************************************************************************************/

void E9_SABIT_DEGERLER_FORMU::WIDGET_DIZILERINI_DUZENLE ()
{
    M_IND_KDV_HESAPLARI_LABEL_ARRAY                 =   new  QLabel         *  [ KDV_OTV_ORANLARI_SAYISI ];
    M_HES_KDV_HESAPLARI_LABEL_ARRAY                 =   new  QLabel         *  [ KDV_OTV_ORANLARI_SAYISI ];
    M_IND_KDV_HESAPLARI_IADE_LABEL_ARRAY            =   new  QLabel         *  [ KDV_OTV_ORANLARI_SAYISI ];
    M_HES_KDV_HESAPLARI_IADE_LABEL_ARRAY            =   new  QLabel         *  [ KDV_OTV_ORANLARI_SAYISI ];

    M_OTV_HESAPLARI_ALIS_LABEL_ARRAY                =   new  QLabel         *  [ KDV_OTV_ORANLARI_SAYISI ];
    M_OTV_HESAPLARI_SATIS_LABEL_ARRAY               =   new  QLabel         *  [ KDV_OTV_ORANLARI_SAYISI ];

    M_TEVKIFAT_KDV_HESAPLARI_LABEL_ARRAY            =   new  QLabel         *  [ KDV_OTV_ORANLARI_SAYISI ];

    M_IND_KDV_HESAPLARI_SEARCHEDIT_ARRAY            =   new  QSearchEdit    *  [ KDV_OTV_ORANLARI_SAYISI ];
    M_HES_KDV_HESAPLARI_SEARCHEDIT_ARRAY            =   new  QSearchEdit    *  [ KDV_OTV_ORANLARI_SAYISI ];
    M_IND_KDV_HESAPLARI_IADE_SEARCHEDIT_ARRAY       =   new  QSearchEdit    *  [ KDV_OTV_ORANLARI_SAYISI ];
    M_HES_KDV_HESAPLARI_IADE_SEARCHEDIT_ARRAY       =   new  QSearchEdit    *  [ KDV_OTV_ORANLARI_SAYISI ];

    M_OTV_HESAPLARI_ALIS_SEARCHEDIT_ARRAY           =   new  QSearchEdit    *  [ KDV_OTV_ORANLARI_SAYISI ];
    M_OTV_HESAPLARI_SATIS_SEARCHEDIT_ARRAY          =   new  QSearchEdit    *  [ KDV_OTV_ORANLARI_SAYISI ];

    M_TEVKIFAT_KDV_HESAPLARI_SEARCHEDIT_ARRAY       =   new  QSearchEdit    *  [ KDV_OTV_ORANLARI_SAYISI ];

    M_IND_KDV_HESAPLARI_LINEEDIT_ARRAY              =   new  QLineEdit      *  [ KDV_OTV_ORANLARI_SAYISI ];
    M_HES_KDV_HESAPLARI_LINEEDIT_ARRAY              =   new  QLineEdit      *  [ KDV_OTV_ORANLARI_SAYISI ];
    M_IND_KDV_HESAPLARI_IADE_LINEEDIT_ARRAY         =   new  QLineEdit      *  [ KDV_OTV_ORANLARI_SAYISI ];
    M_HES_KDV_HESAPLARI_IADE_LINEEDIT_ARRAY         =   new  QLineEdit      *  [ KDV_OTV_ORANLARI_SAYISI ];

    M_OTV_HESAPLARI_ALIS_LINEEDIT_ARRAY             =   new  QLineEdit      *  [ KDV_OTV_ORANLARI_SAYISI ];
    M_OTV_HESAPLARI_SATIS_LINEEDIT_ARRAY            =   new  QLineEdit      *  [ KDV_OTV_ORANLARI_SAYISI ];

    M_TEVKIFAT_KDV_HESAPLARI_LINEEDIT_ARRAY         =   new  QLineEdit      *  [ KDV_OTV_ORANLARI_SAYISI ];

    M_ENTEGRASYON_HESAPLARI_SEARCHEDIT_ARRAY        =   new  QSearchEdit    *  [ONTANIMLI_ENTEGRASYON_HESAPLARI_SAYISI];
    M_ENTEGRASYON_HESAPLARI_LINEEDIT_ARRAY          =   new  QLineEdit      *  [ONTANIMLI_ENTEGRASYON_HESAPLARI_SAYISI];

    // KDV LABELLAR

    M_IND_KDV_HESAPLARI_LABEL_ARRAY             [ 0 ]   =   m_ui->label_ind_kdv_orani_0;
    M_IND_KDV_HESAPLARI_LABEL_ARRAY             [ 1 ]   =   m_ui->label_ind_kdv_orani_1;
    M_IND_KDV_HESAPLARI_LABEL_ARRAY             [ 2 ]   =   m_ui->label_ind_kdv_orani_2;
    M_IND_KDV_HESAPLARI_LABEL_ARRAY             [ 3 ]   =   m_ui->label_ind_kdv_orani_3;
    M_IND_KDV_HESAPLARI_LABEL_ARRAY             [ 4 ]   =   m_ui->label_ind_kdv_orani_4;
    M_IND_KDV_HESAPLARI_LABEL_ARRAY             [ 5 ]   =   m_ui->label_ind_kdv_orani_5;
    M_IND_KDV_HESAPLARI_LABEL_ARRAY             [ 6 ]   =   m_ui->label_ind_kdv_orani_6;
    M_IND_KDV_HESAPLARI_LABEL_ARRAY             [ 7 ]   =   m_ui->label_ind_kdv_orani_7;
    M_IND_KDV_HESAPLARI_LABEL_ARRAY             [ 8 ]   =   m_ui->label_ind_kdv_orani_8;
    M_IND_KDV_HESAPLARI_LABEL_ARRAY             [ 9 ]   =   m_ui->label_ind_kdv_orani_9;

    M_HES_KDV_HESAPLARI_LABEL_ARRAY             [ 0 ]   =   m_ui->label_hes_kdv_orani_0;
    M_HES_KDV_HESAPLARI_LABEL_ARRAY             [ 1 ]   =   m_ui->label_hes_kdv_orani_1;
    M_HES_KDV_HESAPLARI_LABEL_ARRAY             [ 2 ]   =   m_ui->label_hes_kdv_orani_2;
    M_HES_KDV_HESAPLARI_LABEL_ARRAY             [ 3 ]   =   m_ui->label_hes_kdv_orani_3;
    M_HES_KDV_HESAPLARI_LABEL_ARRAY             [ 4 ]   =   m_ui->label_hes_kdv_orani_4;
    M_HES_KDV_HESAPLARI_LABEL_ARRAY             [ 5 ]   =   m_ui->label_hes_kdv_orani_5;
    M_HES_KDV_HESAPLARI_LABEL_ARRAY             [ 6 ]   =   m_ui->label_hes_kdv_orani_6;
    M_HES_KDV_HESAPLARI_LABEL_ARRAY             [ 7 ]   =   m_ui->label_hes_kdv_orani_7;
    M_HES_KDV_HESAPLARI_LABEL_ARRAY             [ 8 ]   =   m_ui->label_hes_kdv_orani_8;
    M_HES_KDV_HESAPLARI_LABEL_ARRAY             [ 9 ]   =   m_ui->label_hes_kdv_orani_9;

    M_IND_KDV_HESAPLARI_IADE_LABEL_ARRAY        [ 0 ]   =   m_ui->label_ind_kdv_iade_orani_0;
    M_IND_KDV_HESAPLARI_IADE_LABEL_ARRAY        [ 1 ]   =   m_ui->label_ind_kdv_iade_orani_1;
    M_IND_KDV_HESAPLARI_IADE_LABEL_ARRAY        [ 2 ]   =   m_ui->label_ind_kdv_iade_orani_2;
    M_IND_KDV_HESAPLARI_IADE_LABEL_ARRAY        [ 3 ]   =   m_ui->label_ind_kdv_iade_orani_3;
    M_IND_KDV_HESAPLARI_IADE_LABEL_ARRAY        [ 4 ]   =   m_ui->label_ind_kdv_iade_orani_4;
    M_IND_KDV_HESAPLARI_IADE_LABEL_ARRAY        [ 5 ]   =   m_ui->label_ind_kdv_iade_orani_5;
    M_IND_KDV_HESAPLARI_IADE_LABEL_ARRAY        [ 6 ]   =   m_ui->label_ind_kdv_iade_orani_6;
    M_IND_KDV_HESAPLARI_IADE_LABEL_ARRAY        [ 7 ]   =   m_ui->label_ind_kdv_iade_orani_7;
    M_IND_KDV_HESAPLARI_IADE_LABEL_ARRAY        [ 8 ]   =   m_ui->label_ind_kdv_iade_orani_8;
    M_IND_KDV_HESAPLARI_IADE_LABEL_ARRAY        [ 9 ]   =   m_ui->label_ind_kdv_iade_orani_9;

    M_HES_KDV_HESAPLARI_IADE_LABEL_ARRAY        [ 0 ]   =   m_ui->label_hes_kdv_iade_orani_0;
    M_HES_KDV_HESAPLARI_IADE_LABEL_ARRAY        [ 1 ]   =   m_ui->label_hes_kdv_iade_orani_1;
    M_HES_KDV_HESAPLARI_IADE_LABEL_ARRAY        [ 2 ]   =   m_ui->label_hes_kdv_iade_orani_2;
    M_HES_KDV_HESAPLARI_IADE_LABEL_ARRAY        [ 3 ]   =   m_ui->label_hes_kdv_iade_orani_3;
    M_HES_KDV_HESAPLARI_IADE_LABEL_ARRAY        [ 4 ]   =   m_ui->label_hes_kdv_iade_orani_4;
    M_HES_KDV_HESAPLARI_IADE_LABEL_ARRAY        [ 5 ]   =   m_ui->label_hes_kdv_iade_orani_5;
    M_HES_KDV_HESAPLARI_IADE_LABEL_ARRAY        [ 6 ]   =   m_ui->label_hes_kdv_iade_orani_6;
    M_HES_KDV_HESAPLARI_IADE_LABEL_ARRAY        [ 7 ]   =   m_ui->label_hes_kdv_iade_orani_7;
    M_HES_KDV_HESAPLARI_IADE_LABEL_ARRAY        [ 8 ]   =   m_ui->label_hes_kdv_iade_orani_8;
    M_HES_KDV_HESAPLARI_IADE_LABEL_ARRAY        [ 9 ]   =   m_ui->label_hes_kdv_iade_orani_9;


    // OTV LABELLAR

    M_OTV_HESAPLARI_ALIS_LABEL_ARRAY            [ 0 ]   =   m_ui->label_alis_otv_orani_0;
    M_OTV_HESAPLARI_ALIS_LABEL_ARRAY            [ 1 ]   =   m_ui->label_alis_otv_orani_1;
    M_OTV_HESAPLARI_ALIS_LABEL_ARRAY            [ 2 ]   =   m_ui->label_alis_otv_orani_2;
    M_OTV_HESAPLARI_ALIS_LABEL_ARRAY            [ 3 ]   =   m_ui->label_alis_otv_orani_3;
    M_OTV_HESAPLARI_ALIS_LABEL_ARRAY            [ 4 ]   =   m_ui->label_alis_otv_orani_4;
    M_OTV_HESAPLARI_ALIS_LABEL_ARRAY            [ 5 ]   =   m_ui->label_alis_otv_orani_5;
    M_OTV_HESAPLARI_ALIS_LABEL_ARRAY            [ 6 ]   =   m_ui->label_alis_otv_orani_6;
    M_OTV_HESAPLARI_ALIS_LABEL_ARRAY            [ 7 ]   =   m_ui->label_alis_otv_orani_7;
    M_OTV_HESAPLARI_ALIS_LABEL_ARRAY            [ 8 ]   =   m_ui->label_alis_otv_orani_8;
    M_OTV_HESAPLARI_ALIS_LABEL_ARRAY            [ 9 ]   =   m_ui->label_alis_otv_orani_9;

    M_OTV_HESAPLARI_SATIS_LABEL_ARRAY           [ 0 ]   =   m_ui->label_satis_otv_orani_0;
    M_OTV_HESAPLARI_SATIS_LABEL_ARRAY           [ 1 ]   =   m_ui->label_satis_otv_orani_1;
    M_OTV_HESAPLARI_SATIS_LABEL_ARRAY           [ 2 ]   =   m_ui->label_satis_otv_orani_2;
    M_OTV_HESAPLARI_SATIS_LABEL_ARRAY           [ 3 ]   =   m_ui->label_satis_otv_orani_3;
    M_OTV_HESAPLARI_SATIS_LABEL_ARRAY           [ 4 ]   =   m_ui->label_satis_otv_orani_4;
    M_OTV_HESAPLARI_SATIS_LABEL_ARRAY           [ 5 ]   =   m_ui->label_satis_otv_orani_5;
    M_OTV_HESAPLARI_SATIS_LABEL_ARRAY           [ 6 ]   =   m_ui->label_satis_otv_orani_6;
    M_OTV_HESAPLARI_SATIS_LABEL_ARRAY           [ 7 ]   =   m_ui->label_satis_otv_orani_7;
    M_OTV_HESAPLARI_SATIS_LABEL_ARRAY           [ 8 ]   =   m_ui->label_satis_otv_orani_8;
    M_OTV_HESAPLARI_SATIS_LABEL_ARRAY           [ 9 ]   =   m_ui->label_satis_otv_orani_9;

    // TEVKIFAT_KDV_LABELLAR

    M_TEVKIFAT_KDV_HESAPLARI_LABEL_ARRAY        [ 0 ]   =   m_ui->label_tevkifat_kdv_ent_hesabi_0;
    M_TEVKIFAT_KDV_HESAPLARI_LABEL_ARRAY        [ 1 ]   =   m_ui->label_tevkifat_kdv_ent_hesabi_1;
    M_TEVKIFAT_KDV_HESAPLARI_LABEL_ARRAY        [ 2 ]   =   m_ui->label_tevkifat_kdv_ent_hesabi_2;
    M_TEVKIFAT_KDV_HESAPLARI_LABEL_ARRAY        [ 3 ]   =   m_ui->label_tevkifat_kdv_ent_hesabi_3;
    M_TEVKIFAT_KDV_HESAPLARI_LABEL_ARRAY        [ 4 ]   =   m_ui->label_tevkifat_kdv_ent_hesabi_4;
    M_TEVKIFAT_KDV_HESAPLARI_LABEL_ARRAY        [ 5 ]   =   m_ui->label_tevkifat_kdv_ent_hesabi_5;
    M_TEVKIFAT_KDV_HESAPLARI_LABEL_ARRAY        [ 6 ]   =   m_ui->label_tevkifat_kdv_ent_hesabi_6;
    M_TEVKIFAT_KDV_HESAPLARI_LABEL_ARRAY        [ 7 ]   =   m_ui->label_tevkifat_kdv_ent_hesabi_7;
    M_TEVKIFAT_KDV_HESAPLARI_LABEL_ARRAY        [ 8 ]   =   m_ui->label_tevkifat_kdv_ent_hesabi_8;
    M_TEVKIFAT_KDV_HESAPLARI_LABEL_ARRAY        [ 9 ]   =   m_ui->label_tevkifat_kdv_ent_hesabi_9;

    // KDV SEARCHEDITLER

    M_IND_KDV_HESAPLARI_SEARCHEDIT_ARRAY        [ 0 ]   =   m_ui->searchEdit_ind_kdv_0;
    M_IND_KDV_HESAPLARI_SEARCHEDIT_ARRAY        [ 1 ]   =   m_ui->searchEdit_ind_kdv_1;
    M_IND_KDV_HESAPLARI_SEARCHEDIT_ARRAY        [ 2 ]   =   m_ui->searchEdit_ind_kdv_2;
    M_IND_KDV_HESAPLARI_SEARCHEDIT_ARRAY        [ 3 ]   =   m_ui->searchEdit_ind_kdv_3;
    M_IND_KDV_HESAPLARI_SEARCHEDIT_ARRAY        [ 4 ]   =   m_ui->searchEdit_ind_kdv_4;
    M_IND_KDV_HESAPLARI_SEARCHEDIT_ARRAY        [ 5 ]   =   m_ui->searchEdit_ind_kdv_5;
    M_IND_KDV_HESAPLARI_SEARCHEDIT_ARRAY        [ 6 ]   =   m_ui->searchEdit_ind_kdv_6;
    M_IND_KDV_HESAPLARI_SEARCHEDIT_ARRAY        [ 7 ]   =   m_ui->searchEdit_ind_kdv_7;
    M_IND_KDV_HESAPLARI_SEARCHEDIT_ARRAY        [ 8 ]   =   m_ui->searchEdit_ind_kdv_8;
    M_IND_KDV_HESAPLARI_SEARCHEDIT_ARRAY        [ 9 ]   =   m_ui->searchEdit_ind_kdv_9;

    M_HES_KDV_HESAPLARI_SEARCHEDIT_ARRAY        [ 0 ]   =   m_ui->searchEdit_hes_kdv_0;
    M_HES_KDV_HESAPLARI_SEARCHEDIT_ARRAY        [ 1 ]   =   m_ui->searchEdit_hes_kdv_1;
    M_HES_KDV_HESAPLARI_SEARCHEDIT_ARRAY        [ 2 ]   =   m_ui->searchEdit_hes_kdv_2;
    M_HES_KDV_HESAPLARI_SEARCHEDIT_ARRAY        [ 3 ]   =   m_ui->searchEdit_hes_kdv_3;
    M_HES_KDV_HESAPLARI_SEARCHEDIT_ARRAY        [ 4 ]   =   m_ui->searchEdit_hes_kdv_4;
    M_HES_KDV_HESAPLARI_SEARCHEDIT_ARRAY        [ 5 ]   =   m_ui->searchEdit_hes_kdv_5;
    M_HES_KDV_HESAPLARI_SEARCHEDIT_ARRAY        [ 6 ]   =   m_ui->searchEdit_hes_kdv_6;
    M_HES_KDV_HESAPLARI_SEARCHEDIT_ARRAY        [ 7 ]   =   m_ui->searchEdit_hes_kdv_7;
    M_HES_KDV_HESAPLARI_SEARCHEDIT_ARRAY        [ 8 ]   =   m_ui->searchEdit_hes_kdv_8;
    M_HES_KDV_HESAPLARI_SEARCHEDIT_ARRAY        [ 9 ]   =   m_ui->searchEdit_hes_kdv_9;

    M_IND_KDV_HESAPLARI_IADE_SEARCHEDIT_ARRAY   [ 0 ]   =   m_ui->searchEdit_ind_kdv_iade_0;
    M_IND_KDV_HESAPLARI_IADE_SEARCHEDIT_ARRAY   [ 1 ]   =   m_ui->searchEdit_ind_kdv_iade_1;
    M_IND_KDV_HESAPLARI_IADE_SEARCHEDIT_ARRAY   [ 2 ]   =   m_ui->searchEdit_ind_kdv_iade_2;
    M_IND_KDV_HESAPLARI_IADE_SEARCHEDIT_ARRAY   [ 3 ]   =   m_ui->searchEdit_ind_kdv_iade_3;
    M_IND_KDV_HESAPLARI_IADE_SEARCHEDIT_ARRAY   [ 4 ]   =   m_ui->searchEdit_ind_kdv_iade_4;
    M_IND_KDV_HESAPLARI_IADE_SEARCHEDIT_ARRAY   [ 5 ]   =   m_ui->searchEdit_ind_kdv_iade_5;
    M_IND_KDV_HESAPLARI_IADE_SEARCHEDIT_ARRAY   [ 6 ]   =   m_ui->searchEdit_ind_kdv_iade_6;
    M_IND_KDV_HESAPLARI_IADE_SEARCHEDIT_ARRAY   [ 7 ]   =   m_ui->searchEdit_ind_kdv_iade_7;
    M_IND_KDV_HESAPLARI_IADE_SEARCHEDIT_ARRAY   [ 8 ]   =   m_ui->searchEdit_ind_kdv_iade_8;
    M_IND_KDV_HESAPLARI_IADE_SEARCHEDIT_ARRAY   [ 9 ]   =   m_ui->searchEdit_ind_kdv_iade_9;

    M_HES_KDV_HESAPLARI_IADE_SEARCHEDIT_ARRAY   [ 0 ]   =   m_ui->searchEdit_hes_kdv_iade_0;
    M_HES_KDV_HESAPLARI_IADE_SEARCHEDIT_ARRAY   [ 1 ]   =   m_ui->searchEdit_hes_kdv_iade_1;
    M_HES_KDV_HESAPLARI_IADE_SEARCHEDIT_ARRAY   [ 2 ]   =   m_ui->searchEdit_hes_kdv_iade_2;
    M_HES_KDV_HESAPLARI_IADE_SEARCHEDIT_ARRAY   [ 3 ]   =   m_ui->searchEdit_hes_kdv_iade_3;
    M_HES_KDV_HESAPLARI_IADE_SEARCHEDIT_ARRAY   [ 4 ]   =   m_ui->searchEdit_hes_kdv_iade_4;
    M_HES_KDV_HESAPLARI_IADE_SEARCHEDIT_ARRAY   [ 5 ]   =   m_ui->searchEdit_hes_kdv_iade_5;
    M_HES_KDV_HESAPLARI_IADE_SEARCHEDIT_ARRAY   [ 6 ]   =   m_ui->searchEdit_hes_kdv_iade_6;
    M_HES_KDV_HESAPLARI_IADE_SEARCHEDIT_ARRAY   [ 7 ]   =   m_ui->searchEdit_hes_kdv_iade_7;
    M_HES_KDV_HESAPLARI_IADE_SEARCHEDIT_ARRAY   [ 8 ]   =   m_ui->searchEdit_hes_kdv_iade_8;
    M_HES_KDV_HESAPLARI_IADE_SEARCHEDIT_ARRAY   [ 9 ]   =   m_ui->searchEdit_hes_kdv_iade_9;

    // OTV SEARCHEDITLER

    M_OTV_HESAPLARI_ALIS_SEARCHEDIT_ARRAY       [ 0 ]   =   m_ui->searchEdit_alis_otv_orani_0;
    M_OTV_HESAPLARI_ALIS_SEARCHEDIT_ARRAY       [ 1 ]   =   m_ui->searchEdit_alis_otv_orani_1;
    M_OTV_HESAPLARI_ALIS_SEARCHEDIT_ARRAY       [ 2 ]   =   m_ui->searchEdit_alis_otv_orani_2;
    M_OTV_HESAPLARI_ALIS_SEARCHEDIT_ARRAY       [ 3 ]   =   m_ui->searchEdit_alis_otv_orani_3;
    M_OTV_HESAPLARI_ALIS_SEARCHEDIT_ARRAY       [ 4 ]   =   m_ui->searchEdit_alis_otv_orani_4;
    M_OTV_HESAPLARI_ALIS_SEARCHEDIT_ARRAY       [ 5 ]   =   m_ui->searchEdit_alis_otv_orani_5;
    M_OTV_HESAPLARI_ALIS_SEARCHEDIT_ARRAY       [ 6 ]   =   m_ui->searchEdit_alis_otv_orani_6;
    M_OTV_HESAPLARI_ALIS_SEARCHEDIT_ARRAY       [ 7 ]   =   m_ui->searchEdit_alis_otv_orani_7;
    M_OTV_HESAPLARI_ALIS_SEARCHEDIT_ARRAY       [ 8 ]   =   m_ui->searchEdit_alis_otv_orani_8;
    M_OTV_HESAPLARI_ALIS_SEARCHEDIT_ARRAY       [ 9 ]   =   m_ui->searchEdit_alis_otv_orani_9;

    M_OTV_HESAPLARI_SATIS_SEARCHEDIT_ARRAY      [ 0 ]   =   m_ui->searchEdit_satis_otv_orani_0;
    M_OTV_HESAPLARI_SATIS_SEARCHEDIT_ARRAY      [ 1 ]   =   m_ui->searchEdit_satis_otv_orani_1;
    M_OTV_HESAPLARI_SATIS_SEARCHEDIT_ARRAY      [ 2 ]   =   m_ui->searchEdit_satis_otv_orani_2;
    M_OTV_HESAPLARI_SATIS_SEARCHEDIT_ARRAY      [ 3 ]   =   m_ui->searchEdit_satis_otv_orani_3;
    M_OTV_HESAPLARI_SATIS_SEARCHEDIT_ARRAY      [ 4 ]   =   m_ui->searchEdit_satis_otv_orani_4;
    M_OTV_HESAPLARI_SATIS_SEARCHEDIT_ARRAY      [ 5 ]   =   m_ui->searchEdit_satis_otv_orani_5;
    M_OTV_HESAPLARI_SATIS_SEARCHEDIT_ARRAY      [ 6 ]   =   m_ui->searchEdit_satis_otv_orani_6;
    M_OTV_HESAPLARI_SATIS_SEARCHEDIT_ARRAY      [ 7 ]   =   m_ui->searchEdit_satis_otv_orani_7;
    M_OTV_HESAPLARI_SATIS_SEARCHEDIT_ARRAY      [ 8 ]   =   m_ui->searchEdit_satis_otv_orani_8;
    M_OTV_HESAPLARI_SATIS_SEARCHEDIT_ARRAY      [ 9 ]   =   m_ui->searchEdit_satis_otv_orani_9;

    // TEVKIFAT KDV SEARCHEDITLER

    M_TEVKIFAT_KDV_HESAPLARI_SEARCHEDIT_ARRAY   [ 0 ]   =   m_ui->searchEdit_tevkifat_kdv_ent_hesabi_0;
    M_TEVKIFAT_KDV_HESAPLARI_SEARCHEDIT_ARRAY   [ 1 ]   =   m_ui->searchEdit_tevkifat_kdv_ent_hesabi_1;
    M_TEVKIFAT_KDV_HESAPLARI_SEARCHEDIT_ARRAY   [ 2 ]   =   m_ui->searchEdit_tevkifat_kdv_ent_hesabi_2;
    M_TEVKIFAT_KDV_HESAPLARI_SEARCHEDIT_ARRAY   [ 3 ]   =   m_ui->searchEdit_tevkifat_kdv_ent_hesabi_3;
    M_TEVKIFAT_KDV_HESAPLARI_SEARCHEDIT_ARRAY   [ 4 ]   =   m_ui->searchEdit_tevkifat_kdv_ent_hesabi_4;
    M_TEVKIFAT_KDV_HESAPLARI_SEARCHEDIT_ARRAY   [ 5 ]   =   m_ui->searchEdit_tevkifat_kdv_ent_hesabi_5;
    M_TEVKIFAT_KDV_HESAPLARI_SEARCHEDIT_ARRAY   [ 6 ]   =   m_ui->searchEdit_tevkifat_kdv_ent_hesabi_6;
    M_TEVKIFAT_KDV_HESAPLARI_SEARCHEDIT_ARRAY   [ 7 ]   =   m_ui->searchEdit_tevkifat_kdv_ent_hesabi_7;
    M_TEVKIFAT_KDV_HESAPLARI_SEARCHEDIT_ARRAY   [ 8 ]   =   m_ui->searchEdit_tevkifat_kdv_ent_hesabi_8;
    M_TEVKIFAT_KDV_HESAPLARI_SEARCHEDIT_ARRAY   [ 9 ]   =   m_ui->searchEdit_tevkifat_kdv_ent_hesabi_9;

    // KDV LINEEDITLER

    M_IND_KDV_HESAPLARI_LINEEDIT_ARRAY          [ 0 ]   =   m_ui->lineEdit_ind_kdv_0;
    M_IND_KDV_HESAPLARI_LINEEDIT_ARRAY          [ 1 ]   =   m_ui->lineEdit_ind_kdv_1;
    M_IND_KDV_HESAPLARI_LINEEDIT_ARRAY          [ 2 ]   =   m_ui->lineEdit_ind_kdv_2;
    M_IND_KDV_HESAPLARI_LINEEDIT_ARRAY          [ 3 ]   =   m_ui->lineEdit_ind_kdv_3;
    M_IND_KDV_HESAPLARI_LINEEDIT_ARRAY          [ 4 ]   =   m_ui->lineEdit_ind_kdv_4;
    M_IND_KDV_HESAPLARI_LINEEDIT_ARRAY          [ 5 ]   =   m_ui->lineEdit_ind_kdv_5;
    M_IND_KDV_HESAPLARI_LINEEDIT_ARRAY          [ 6 ]   =   m_ui->lineEdit_ind_kdv_6;
    M_IND_KDV_HESAPLARI_LINEEDIT_ARRAY          [ 7 ]   =   m_ui->lineEdit_ind_kdv_7;
    M_IND_KDV_HESAPLARI_LINEEDIT_ARRAY          [ 8 ]   =   m_ui->lineEdit_ind_kdv_8;
    M_IND_KDV_HESAPLARI_LINEEDIT_ARRAY          [ 9 ]   =   m_ui->lineEdit_ind_kdv_9;

    M_HES_KDV_HESAPLARI_LINEEDIT_ARRAY          [ 0 ]   =   m_ui->lineEdit_hes_kdv_0;
    M_HES_KDV_HESAPLARI_LINEEDIT_ARRAY          [ 1 ]   =   m_ui->lineEdit_hes_kdv_1;
    M_HES_KDV_HESAPLARI_LINEEDIT_ARRAY          [ 2 ]   =   m_ui->lineEdit_hes_kdv_2;
    M_HES_KDV_HESAPLARI_LINEEDIT_ARRAY          [ 3 ]   =   m_ui->lineEdit_hes_kdv_3;
    M_HES_KDV_HESAPLARI_LINEEDIT_ARRAY          [ 4 ]   =   m_ui->lineEdit_hes_kdv_4;
    M_HES_KDV_HESAPLARI_LINEEDIT_ARRAY          [ 5 ]   =   m_ui->lineEdit_hes_kdv_5;
    M_HES_KDV_HESAPLARI_LINEEDIT_ARRAY          [ 6 ]   =   m_ui->lineEdit_hes_kdv_6;
    M_HES_KDV_HESAPLARI_LINEEDIT_ARRAY          [ 7 ]   =   m_ui->lineEdit_hes_kdv_7;
    M_HES_KDV_HESAPLARI_LINEEDIT_ARRAY          [ 8 ]   =   m_ui->lineEdit_hes_kdv_8;
    M_HES_KDV_HESAPLARI_LINEEDIT_ARRAY          [ 9 ]   =   m_ui->lineEdit_hes_kdv_9;

    M_IND_KDV_HESAPLARI_IADE_LINEEDIT_ARRAY     [ 0 ]   =   m_ui->lineEdit_ind_kdv_iade_0;
    M_IND_KDV_HESAPLARI_IADE_LINEEDIT_ARRAY     [ 1 ]   =   m_ui->lineEdit_ind_kdv_iade_1;
    M_IND_KDV_HESAPLARI_IADE_LINEEDIT_ARRAY     [ 2 ]   =   m_ui->lineEdit_ind_kdv_iade_2;
    M_IND_KDV_HESAPLARI_IADE_LINEEDIT_ARRAY     [ 3 ]   =   m_ui->lineEdit_ind_kdv_iade_3;
    M_IND_KDV_HESAPLARI_IADE_LINEEDIT_ARRAY     [ 4 ]   =   m_ui->lineEdit_ind_kdv_iade_4;
    M_IND_KDV_HESAPLARI_IADE_LINEEDIT_ARRAY     [ 5 ]   =   m_ui->lineEdit_ind_kdv_iade_5;
    M_IND_KDV_HESAPLARI_IADE_LINEEDIT_ARRAY     [ 6 ]   =   m_ui->lineEdit_ind_kdv_iade_6;
    M_IND_KDV_HESAPLARI_IADE_LINEEDIT_ARRAY     [ 7 ]   =   m_ui->lineEdit_ind_kdv_iade_7;
    M_IND_KDV_HESAPLARI_IADE_LINEEDIT_ARRAY     [ 8 ]   =   m_ui->lineEdit_ind_kdv_iade_8;
    M_IND_KDV_HESAPLARI_IADE_LINEEDIT_ARRAY     [ 9 ]   =   m_ui->lineEdit_ind_kdv_iade_9;

    M_HES_KDV_HESAPLARI_IADE_LINEEDIT_ARRAY     [ 0 ]   =   m_ui->lineEdit_hes_kdv_iade_0;
    M_HES_KDV_HESAPLARI_IADE_LINEEDIT_ARRAY     [ 1 ]   =   m_ui->lineEdit_hes_kdv_iade_1;
    M_HES_KDV_HESAPLARI_IADE_LINEEDIT_ARRAY     [ 2 ]   =   m_ui->lineEdit_hes_kdv_iade_2;
    M_HES_KDV_HESAPLARI_IADE_LINEEDIT_ARRAY     [ 3 ]   =   m_ui->lineEdit_hes_kdv_iade_3;
    M_HES_KDV_HESAPLARI_IADE_LINEEDIT_ARRAY     [ 4 ]   =   m_ui->lineEdit_hes_kdv_iade_4;
    M_HES_KDV_HESAPLARI_IADE_LINEEDIT_ARRAY     [ 5 ]   =   m_ui->lineEdit_hes_kdv_iade_5;
    M_HES_KDV_HESAPLARI_IADE_LINEEDIT_ARRAY     [ 6 ]   =   m_ui->lineEdit_hes_kdv_iade_6;
    M_HES_KDV_HESAPLARI_IADE_LINEEDIT_ARRAY     [ 7 ]   =   m_ui->lineEdit_hes_kdv_iade_7;
    M_HES_KDV_HESAPLARI_IADE_LINEEDIT_ARRAY     [ 8 ]   =   m_ui->lineEdit_hes_kdv_iade_8;
    M_HES_KDV_HESAPLARI_IADE_LINEEDIT_ARRAY     [ 9 ]   =   m_ui->lineEdit_hes_kdv_iade_9;

    // OTV LINEEDITLER

    M_OTV_HESAPLARI_ALIS_LINEEDIT_ARRAY         [ 0 ]   =   m_ui->lineEdit_alis_otv_orani_0;
    M_OTV_HESAPLARI_ALIS_LINEEDIT_ARRAY         [ 1 ]   =   m_ui->lineEdit_alis_otv_orani_1;
    M_OTV_HESAPLARI_ALIS_LINEEDIT_ARRAY         [ 2 ]   =   m_ui->lineEdit_alis_otv_orani_2;
    M_OTV_HESAPLARI_ALIS_LINEEDIT_ARRAY         [ 3 ]   =   m_ui->lineEdit_alis_otv_orani_3;
    M_OTV_HESAPLARI_ALIS_LINEEDIT_ARRAY         [ 4 ]   =   m_ui->lineEdit_alis_otv_orani_4;
    M_OTV_HESAPLARI_ALIS_LINEEDIT_ARRAY         [ 5 ]   =   m_ui->lineEdit_alis_otv_orani_5;
    M_OTV_HESAPLARI_ALIS_LINEEDIT_ARRAY         [ 6 ]   =   m_ui->lineEdit_alis_otv_orani_6;
    M_OTV_HESAPLARI_ALIS_LINEEDIT_ARRAY         [ 7 ]   =   m_ui->lineEdit_alis_otv_orani_7;
    M_OTV_HESAPLARI_ALIS_LINEEDIT_ARRAY         [ 8 ]   =   m_ui->lineEdit_alis_otv_orani_8;
    M_OTV_HESAPLARI_ALIS_LINEEDIT_ARRAY         [ 9 ]   =   m_ui->lineEdit_alis_otv_orani_9;

    M_OTV_HESAPLARI_SATIS_LINEEDIT_ARRAY        [ 0 ]   =   m_ui->lineEdit_satis_otv_orani_0;
    M_OTV_HESAPLARI_SATIS_LINEEDIT_ARRAY        [ 1 ]   =   m_ui->lineEdit_satis_otv_orani_1;
    M_OTV_HESAPLARI_SATIS_LINEEDIT_ARRAY        [ 2 ]   =   m_ui->lineEdit_satis_otv_orani_2;
    M_OTV_HESAPLARI_SATIS_LINEEDIT_ARRAY        [ 3 ]   =   m_ui->lineEdit_satis_otv_orani_3;
    M_OTV_HESAPLARI_SATIS_LINEEDIT_ARRAY        [ 4 ]   =   m_ui->lineEdit_satis_otv_orani_4;
    M_OTV_HESAPLARI_SATIS_LINEEDIT_ARRAY        [ 5 ]   =   m_ui->lineEdit_satis_otv_orani_5;
    M_OTV_HESAPLARI_SATIS_LINEEDIT_ARRAY        [ 6 ]   =   m_ui->lineEdit_satis_otv_orani_6;
    M_OTV_HESAPLARI_SATIS_LINEEDIT_ARRAY        [ 7 ]   =   m_ui->lineEdit_satis_otv_orani_7;
    M_OTV_HESAPLARI_SATIS_LINEEDIT_ARRAY        [ 8 ]   =   m_ui->lineEdit_satis_otv_orani_8;
    M_OTV_HESAPLARI_SATIS_LINEEDIT_ARRAY        [ 9 ]   =   m_ui->lineEdit_satis_otv_orani_9;

    // TEVKIFAT KDV LINEEDITLER

    M_TEVKIFAT_KDV_HESAPLARI_LINEEDIT_ARRAY     [ 0 ]   =   m_ui->lineEdit_tevkifat_kdv_ent_hesabi_0;
    M_TEVKIFAT_KDV_HESAPLARI_LINEEDIT_ARRAY     [ 1 ]   =   m_ui->lineEdit_tevkifat_kdv_ent_hesabi_1;
    M_TEVKIFAT_KDV_HESAPLARI_LINEEDIT_ARRAY     [ 2 ]   =   m_ui->lineEdit_tevkifat_kdv_ent_hesabi_2;
    M_TEVKIFAT_KDV_HESAPLARI_LINEEDIT_ARRAY     [ 3 ]   =   m_ui->lineEdit_tevkifat_kdv_ent_hesabi_3;
    M_TEVKIFAT_KDV_HESAPLARI_LINEEDIT_ARRAY     [ 4 ]   =   m_ui->lineEdit_tevkifat_kdv_ent_hesabi_4;
    M_TEVKIFAT_KDV_HESAPLARI_LINEEDIT_ARRAY     [ 5 ]   =   m_ui->lineEdit_tevkifat_kdv_ent_hesabi_5;
    M_TEVKIFAT_KDV_HESAPLARI_LINEEDIT_ARRAY     [ 6 ]   =   m_ui->lineEdit_tevkifat_kdv_ent_hesabi_6;
    M_TEVKIFAT_KDV_HESAPLARI_LINEEDIT_ARRAY     [ 7 ]   =   m_ui->lineEdit_tevkifat_kdv_ent_hesabi_7;
    M_TEVKIFAT_KDV_HESAPLARI_LINEEDIT_ARRAY     [ 8 ]   =   m_ui->lineEdit_tevkifat_kdv_ent_hesabi_8;
    M_TEVKIFAT_KDV_HESAPLARI_LINEEDIT_ARRAY     [ 9 ]   =   m_ui->lineEdit_tevkifat_kdv_ent_hesabi_9;


}

/**************************************************************************************
        E9_SABIT_DEGERLER_FORMU::KDV_OTV_HESAP_SAYISINA_GORE_EKRANI_DUZENLE
***************************************************************************************/

void E9_SABIT_DEGERLER_FORMU::KDV_OTV_HESAP_SAYISINA_GORE_EKRANI_DUZENLE ()
{
    SQL_QUERY query ( DB );

    query.PREPARE_SELECT ( "e9_sabit_degerler",
                           "kdv_orani_sayisi, kdv_oranlari_array, otv_orani_sayisi, otv_oranlari_array ");

    if ( query.SELECT() EQ 0 ) {
        MSG_WARNING(  tr("Sabit değerler tablosu okunamadı."), NULL );
        return;
    }

    query.NEXT();

            m_kdv_orani_sayisi = query.VALUE ( "kdv_orani_sayisi"   ).toInt();
            m_otv_orani_sayisi = query.VALUE ( "otv_orani_sayisi"   ).toInt();
    QString kdv_oranlari       = query.VALUE ( "kdv_oranlari_array" ).toString();
    QString otv_oranlari       = query.VALUE ( "otv_oranlari_array" ).toString();

    double  KDV_ORANLARI_ARRAY [ KDV_OTV_ORANLARI_SAYISI ];
    double  OTV_ORANLARI_ARRAY [ KDV_OTV_ORANLARI_SAYISI ];

    UNPACK_DOUBLE_ARRAY ( kdv_oranlari, KDV_ORANLARI_ARRAY, KDV_OTV_ORANLARI_SAYISI );
    UNPACK_DOUBLE_ARRAY ( otv_oranlari, OTV_ORANLARI_ARRAY, KDV_OTV_ORANLARI_SAYISI );

    for ( int i = 0; i < m_kdv_orani_sayisi; i++ ) {
        if ( KDV_ORANLARI_ARRAY[i] EQ 0 ) {
            M_IND_KDV_HESAPLARI_SEARCHEDIT_ARRAY        [i]->setDisabled        ( true );
            M_IND_KDV_HESAPLARI_LINEEDIT_ARRAY          [i]->setDisabled        ( true );

            M_IND_KDV_HESAPLARI_IADE_SEARCHEDIT_ARRAY   [i]->setDisabled        ( true );
            M_IND_KDV_HESAPLARI_IADE_LINEEDIT_ARRAY     [i]->setDisabled        ( true );

            M_HES_KDV_HESAPLARI_SEARCHEDIT_ARRAY        [i]->setDisabled        ( true );
            M_HES_KDV_HESAPLARI_LINEEDIT_ARRAY          [i]->setDisabled        ( true );

            M_HES_KDV_HESAPLARI_IADE_SEARCHEDIT_ARRAY   [i]->setDisabled        ( true );
            M_HES_KDV_HESAPLARI_IADE_LINEEDIT_ARRAY     [i]->setDisabled        ( true );

            M_TEVKIFAT_KDV_HESAPLARI_SEARCHEDIT_ARRAY   [i]->setDisabled        ( true );
            M_TEVKIFAT_KDV_HESAPLARI_LINEEDIT_ARRAY     [i]->setDisabled        ( true );
        }
        // indirilecek kdv
        M_IND_KDV_HESAPLARI_LABEL_ARRAY[i]->setText      ( "KDV %" + QVariant   ( KDV_ORANLARI_ARRAY [i] ).toString() );
        // hesaplanan kdv
        M_HES_KDV_HESAPLARI_LABEL_ARRAY[i]->setText      ( "KDV %" + QVariant   ( KDV_ORANLARI_ARRAY [i] ).toString() );
        // indrilicek kdv hesaplari iade
        M_IND_KDV_HESAPLARI_IADE_LABEL_ARRAY[i]->setText ( "KDV %" + QVariant   ( KDV_ORANLARI_ARRAY [i] ).toString() );
        // hesaplanan kdv hesaplari iade
        M_HES_KDV_HESAPLARI_IADE_LABEL_ARRAY[i]->setText ( "KDV %" + QVariant   ( KDV_ORANLARI_ARRAY [i] ).toString() );
        // tevkifat kdv hesaplari
        M_TEVKIFAT_KDV_HESAPLARI_LABEL_ARRAY[i]->setText ( "KDV %" + QVariant   ( KDV_ORANLARI_ARRAY [i] ).toString() );
    }

    for ( int i = 0; i < m_otv_orani_sayisi; i++ ) {
        if ( OTV_ORANLARI_ARRAY[i] EQ 0 ) {
            M_OTV_HESAPLARI_ALIS_SEARCHEDIT_ARRAY   [i]->setDisabled    ( true );
            M_OTV_HESAPLARI_ALIS_LINEEDIT_ARRAY     [i]->setDisabled    ( true );
            M_OTV_HESAPLARI_SATIS_SEARCHEDIT_ARRAY  [i]->setDisabled    ( true );
            M_OTV_HESAPLARI_SATIS_LINEEDIT_ARRAY    [i]->setDisabled    ( true );
        }
        M_OTV_HESAPLARI_ALIS_LABEL_ARRAY[i]->setText  ( "ÖTV %" + QVariant  ( OTV_ORANLARI_ARRAY [i] ).toString() );
        M_OTV_HESAPLARI_SATIS_LABEL_ARRAY[i]->setText ( "ÖTV %" + QVariant  ( OTV_ORANLARI_ARRAY [i] ).toString() );
    }

    for ( int i = m_kdv_orani_sayisi; i < KDV_OTV_ORANLARI_SAYISI; i++ ) {

        M_IND_KDV_HESAPLARI_LABEL_ARRAY             [i]->setVisible ( false );
        M_IND_KDV_HESAPLARI_SEARCHEDIT_ARRAY        [i]->setVisible ( false );
        M_IND_KDV_HESAPLARI_LINEEDIT_ARRAY          [i]->setVisible ( false );

        M_HES_KDV_HESAPLARI_LABEL_ARRAY             [i]->setVisible ( false );
        M_HES_KDV_HESAPLARI_SEARCHEDIT_ARRAY        [i]->setVisible ( false );
        M_HES_KDV_HESAPLARI_LINEEDIT_ARRAY          [i]->setVisible ( false );

        M_IND_KDV_HESAPLARI_IADE_LABEL_ARRAY        [i]->setVisible ( false );
        M_IND_KDV_HESAPLARI_IADE_SEARCHEDIT_ARRAY   [i]->setVisible ( false );
        M_IND_KDV_HESAPLARI_IADE_LINEEDIT_ARRAY     [i]->setVisible ( false );

        M_HES_KDV_HESAPLARI_IADE_LABEL_ARRAY        [i]->setVisible ( false );
        M_HES_KDV_HESAPLARI_IADE_SEARCHEDIT_ARRAY   [i]->setVisible ( false );
        M_HES_KDV_HESAPLARI_IADE_LINEEDIT_ARRAY     [i]->setVisible ( false );

        M_TEVKIFAT_KDV_HESAPLARI_LABEL_ARRAY        [i]->setVisible ( false );
        M_TEVKIFAT_KDV_HESAPLARI_SEARCHEDIT_ARRAY   [i]->setVisible ( false );
        M_TEVKIFAT_KDV_HESAPLARI_LINEEDIT_ARRAY     [i]->setVisible ( false );
    }

    for ( int i = m_otv_orani_sayisi; i < KDV_OTV_ORANLARI_SAYISI; i++ ) {

        M_OTV_HESAPLARI_ALIS_LABEL_ARRAY        [i]->setVisible ( false );
        M_OTV_HESAPLARI_ALIS_SEARCHEDIT_ARRAY   [i]->setVisible ( false );
        M_OTV_HESAPLARI_ALIS_LINEEDIT_ARRAY     [i]->setVisible ( false );

        M_OTV_HESAPLARI_SATIS_LABEL_ARRAY       [i]->setVisible ( false );
        M_OTV_HESAPLARI_SATIS_SEARCHEDIT_ARRAY  [i]->setVisible ( false );
        M_OTV_HESAPLARI_SATIS_LINEEDIT_ARRAY    [i]->setVisible ( false );

    }
}

/**************************************************************************************
        E9_SABIT_DEGERLER_FORMU::KDV_OTV_HESAPLARI_GUNCELLE
***************************************************************************************/

void E9_SABIT_DEGERLER_FORMU::KDV_OTV_HESAPLARI_GUNCELLE()
{
    SQL_QUERY sql_query( DB );

    QString     ind_kdv_hesap_id_array          =   PACK_INT_ARRAY ( M_IND_KDV_HESAPLARI_ID_ARRAY,      KDV_OTV_ORANLARI_SAYISI );
    QString     ind_kdv_iade_hesap_id_array     =   PACK_INT_ARRAY ( M_IND_KDV_IADE_HESAPLARI_ID_ARRAY, KDV_OTV_ORANLARI_SAYISI );
    QString     hes_kdv_hesap_id_array          =   PACK_INT_ARRAY ( M_HES_KDV_HESAPLARI_ID_ARRAY,      KDV_OTV_ORANLARI_SAYISI );
    QString     hes_kdv_iade_hesap_id_array     =   PACK_INT_ARRAY ( M_HES_KDV_IADE_HESAPLARI_ID_ARRAY, KDV_OTV_ORANLARI_SAYISI );
    QString     otv_alis_hesap_id_array         =   PACK_INT_ARRAY ( M_OTV_HESAPLARI_ALIS_ID_ARRAY,     KDV_OTV_ORANLARI_SAYISI );
    QString     otv_satis_hesap_id_array        =   PACK_INT_ARRAY ( M_OTV_HESAPLARI_SATIS_ID_ARRAY,    KDV_OTV_ORANLARI_SAYISI );
    QString     tevkifat_kdv_hesap_id_array     =   PACK_INT_ARRAY ( M_TEVKIFAT_KDV_HESAPLARI_ID_ARRAY, KDV_OTV_ORANLARI_SAYISI );


    sql_query.PREPARE_UPDATE("e9_sabit_degerler",
                             "sabit_deger_id",

                             "kdv_orani_sayisi                    , "
                             "otv_orani_sayisi                    , "
                             "kdv_oranlari_array                  , "
                             "otv_oranlari_array                  , "
                             "ind_kdv_hesap_id_array              , "
                             "ind_kdv_iade_hesap_id_array         , "
                             "hes_kdv_hesap_id_array              , "
                             "hes_kdv_iade_hesap_id_array         , "
                             "otv_alis_hesap_id_array             , "
                             "otv_satis_hesap_id_array            , "
                             "tevkifat_kdv_hesap_id_array           ",

                             "sabit_deger_id = :sabit_deger_id   " );

    sql_query.SET_VALUE ( ":kdv_orani_sayisi",                          m_kdv_orani_sayisi                                                                                  );
    sql_query.SET_VALUE ( ":otv_orani_sayisi",                          m_otv_orani_sayisi                                                                                  );
    sql_query.SET_VALUE ( ":kdv_oranlari_array",                        PACK_DOUBLE_ARRAY ( M_KDV_ORANLARI_ARRAY, KDV_OTV_ORANLARI_SAYISI )                                 );
    sql_query.SET_VALUE ( ":otv_oranlari_array",                        PACK_DOUBLE_ARRAY ( M_OTV_ORANLARI_ARRAY, KDV_OTV_ORANLARI_SAYISI )                                 );
    sql_query.SET_VALUE ( ":ind_kdv_hesap_id_array",                    ind_kdv_hesap_id_array                                                                              );
    sql_query.SET_VALUE ( ":ind_kdv_iade_hesap_id_array",               ind_kdv_iade_hesap_id_array                                                                         );
    sql_query.SET_VALUE ( ":hes_kdv_hesap_id_array",                    hes_kdv_hesap_id_array                                                                              );
    sql_query.SET_VALUE ( ":hes_kdv_iade_hesap_id_array",               hes_kdv_iade_hesap_id_array                                                                         );
    sql_query.SET_VALUE ( ":otv_alis_hesap_id_array",                   otv_alis_hesap_id_array                                                                             );
    sql_query.SET_VALUE ( ":otv_satis_hesap_id_array",                  otv_satis_hesap_id_array                                                                            );
    sql_query.SET_VALUE ( ":tevkifat_kdv_hesap_id_array",               tevkifat_kdv_hesap_id_array                                                                         );


    sql_query.SET_VALUE ( ":sabit_deger_id", GET_RECORD_ID() );

    sql_query.UPDATE();
}










