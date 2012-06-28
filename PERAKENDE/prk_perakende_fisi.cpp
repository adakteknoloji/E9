#include <QMessageBox>
#include <QList>
#include "adak_std_utils.h"
#include "adak_sql.h"
#include "prk_perakende_fisi_class.h"
#include "prk_perakende_fisi_open.h"
#include "e9_struct.h"
#include "e9_enum.h"
#include "prk_fisi_yazdir_raporu_open.h"
#include "prk_console_utils.h"
#include "sube_console_utils.h"
#include "print.h"
#include "muh_fis_utils.h"
#include "muh_enum.h"
#include "muh_struct.h"
#include "dvz_enum.h"
#include "dvz_console_utils.h"
#include "bnk_fis_utils.h"
#include "prk_fis_utils.h"
#include "e9_console_utils.h"
#include "e9_gui_utils.h"
#include "prk_fis_arama_open.h"
#include "e9_uretilen_fis_secimi_open.h"
#include "e9_eft_batch_open.h"
#include "e9_kk_tahsilat_batch_open.h"
#include "e9_kk_odeme_batch_open.h"
#include "e9_ent_detaylari_batch_open.h"
#include "sube_arama_open.h"
#include "muh_console_utils.h"
#include "sube_enum.h"

extern ADAK_SQL *           DB;


#define ROW_ID_COLUMN                  0
#define FIS_ID_COLUMN                  1
#define ORDER_NUMBER_COLUMN            2
#define HESAP_NO_ID_COLUMN             3
#define POS_KK_ID_COLUMN               4
#define KK_SAHIBI_COLUMN               5
#define KK_NUMARASI_COLUMN             6

#define TAHSILAT_ODEME_SEKLI_COLUMN    7
#define GELIR_GIDER_TURU_COLUMN        8
#define BELGE_NO_COLUMN                9
#define ACIKLAMA_COLUMN                10
#define KDV_ORANI_COLUMN               11
#define KDV_HARIC_TUTAR_COLUMN         12
#define KDV_TUTARI_COLUMN              13
#define KDV_DAHIL_TUTAR_COLUMN         14
#define DETAYLAR_COLUMN                15

/**************************************************************************************
                   OPEN_PRK_PERAKENDE_FISI
***************************************************************************************/

void OPEN_PRK_PERAKENDE_FISI (  int p_sube_id ,int p_gider_fisi_mi , QWidget * p_parent )
{
    PRK_PERAKENDE_FISI * F = new PRK_PERAKENDE_FISI ( p_sube_id ,p_gider_fisi_mi , p_parent );
    F->EXEC( FULL_SCREEN );
}

/**************************************************************************************
                   PRK_PERAKENDE_FISI::PRK_PERAKENDE_FISI
***************************************************************************************/


PRK_PERAKENDE_FISI::PRK_PERAKENDE_FISI ( int p_sube_id , int p_gider_fisi_mi ,QWidget * p_parent) :FIS_KERNEL(p_parent) , m_ui ( new Ui::PRK_PERAKENDE_FISI )

{
    m_sube_id        = p_sube_id;
    m_gider_fisi_mi  = p_gider_fisi_mi;

    m_ui->setupUi   ( this );

    START_FIS_KERNEL(this, DB );

 }

/**************************************************************************************
                   PRK_PERAKENDE_FISI::SETUP_FORM();
***************************************************************************************/

void PRK_PERAKENDE_FISI::SETUP_FORM()
{

    SQL_QUERY query( DB );

    if ( m_gider_fisi_mi EQ 1 ) {
        SET_PAGE_TITLE( tr ( "PRK - ALIŞ / GİDER FİŞİ" ) );
        SET_SETTING_NAME  ("PERAKENDE_ALIS_FISI");
        SET_HELP_PAGE     ("kasa-perakende_alısgider-fisi");
    }
    else {
        SET_PAGE_TITLE( tr ( "PRK - SATIŞ / GELİR FİŞİ" ) );
        SET_SETTING_NAME  ("PERAKENDE_SATIS_FISI");
        SET_HELP_PAGE     ("kasa-perakenda_satisgelir-fisi");
    }

    REGISTER_BUTTONS_WIDGET ( m_ui->kernel_buttons_widget );

    REGISTER_SAVER_BUTTON( m_ui->pushButton_urettigi_fisler );
    if ( E9_KULLANICI_FISIN_URETTIGI_ENT_FISLERINI_GOREBILIR_MI() EQ HAYIR ) {
        m_ui->pushButton_urettigi_fisler->setVisible( false );
    }

    M_FIS             = new PRK_FIS_STRUCT;
    M_FIS_SATIRI      = new PRK_FIS_SATIRI_STRUCT;
    M_ISLEM_DETAYLARI = new E9_ISLEM_DETAYLARI;
    M_ENT_DETAYLARI   = new E9_ENT_DETAYLARI_STRUCT;

    m_ui->lineEdit_fis_no->setText("*");

    DISABLE_CLEAR_ON_WIDGET( m_ui->lineEdit_fis_no );
    DISABLE_CLEAR_ON_WIDGET( m_ui->comboBox_sirket );

    SET_LINE_BUTTON_ICON(QStringList() << "");

    REGISTER_TABLE_WIDGET ( m_ui->tablewidget_fis_satirlari, 16 );
    REGISTER_SAVER_BUTTON ( m_ui->pushButton_yazdir );

    SET_FIS_ORDER_COLUMN_NUMBER ( ORDER_NUMBER_COLUMN );
    SET_FORM_ID_COLUMN_POSITION ( FIS_ID_COLUMN );
    SET_ROW_ID_COLUMN_POSITION  ( ROW_ID_COLUMN );
    SET_NUMBER_OF_HIDDEN_COLUMN ( 7 );

    SET_FIS_ORDER_DATABASE ("prk_fis_satirlari", "order_number", "fis_satiri_id" );

    SET_TABLE_ROW_WIDGETS ( BELGE_NO_COLUMN                 , WIDGET_LINE_EDIT  );
    SET_TABLE_ROW_WIDGETS ( ACIKLAMA_COLUMN                 , WIDGET_LINE_EDIT  );
    SET_TABLE_ROW_WIDGETS ( KDV_ORANI_COLUMN                , WIDGET_COMBO_BOX  );
    SET_TABLE_ROW_WIDGETS ( KDV_HARIC_TUTAR_COLUMN          , WIDGET_COMMA_EDIT );
    SET_TABLE_ROW_WIDGETS ( KDV_TUTARI_COLUMN               , WIDGET_COMMA_EDIT );
    SET_TABLE_ROW_WIDGETS ( KDV_DAHIL_TUTAR_COLUMN          , WIDGET_COMMA_EDIT );
    SET_TABLE_ROW_WIDGETS ( GELIR_GIDER_TURU_COLUMN         , WIDGET_COMBO_BOX  );
    SET_TABLE_ROW_WIDGETS ( TAHSILAT_ODEME_SEKLI_COLUMN     , WIDGET_COMBO_BOX  );
    SET_TABLE_ROW_WIDGETS ( DETAYLAR_COLUMN                 , WIDGET_PUSH_BUTTON );
    SET_TABLE_ROW_WIDGETS ( HESAP_NO_ID_COLUMN              , WIDGET_LINE_EDIT  );
    SET_TABLE_ROW_WIDGETS ( POS_KK_ID_COLUMN                , WIDGET_LINE_EDIT  );
    SET_TABLE_ROW_WIDGETS ( KK_SAHIBI_COLUMN                , WIDGET_LINE_EDIT  );
    SET_TABLE_ROW_WIDGETS ( KK_NUMARASI_COLUMN              , WIDGET_LINE_EDIT  );

    SET_FIRST_FOCUS_WIDGET ( m_ui->lineEdit_fis_no     );

    REGISTER_CHANGER_BUTTON( m_ui->pushButton_detaylar );

    m_sube_islem_yetkisi = SUBE_GET_SUBE_ISLEM_YETKISI();

    m_ui->commaEdit_kasa_mevcudu->setDisabled(true);

    m_ontanimli_kdv_orani = 0;

    m_ui->searchEdit_sube_kodu->setDisabled(true);

    m_ui->lineEdit_sube_ismi->setDisabled(true);

    M_KDV_ORANLARI_ARRAY              = new double [KDV_OTV_ORANLARI_SAYISI];
    M_INDIRILECEK_KDV_HESABI_ID_ARRAY = new int [KDV_OTV_ORANLARI_SAYISI];
    M_HESAPLANAN_KDV_HESABI_ID_ARRAY  = new int [KDV_OTV_ORANLARI_SAYISI];

    query.PREPARE_SELECT("e9_sabit_degerler" , "ontanimli_kdv_orani,kdv_oranlari_array,ind_kdv_hesap_id_array, hes_kdv_hesap_id_array ",
                         "sabit_deger_id = :sabit_deger_id");
    query.SET_VALUE(":sabit_deger_id" , 1);

    if ( query.SELECT() NE 0 ) {
        query.NEXT();
        m_ontanimli_kdv_orani = query.VALUE(0).toDouble();
        UNPACK_DOUBLE_ARRAY( query.VALUE(1).toString(), M_KDV_ORANLARI_ARRAY,               KDV_OTV_ORANLARI_SAYISI );
        UNPACK_INT_ARRAY   ( query.VALUE(2).toString(), M_INDIRILECEK_KDV_HESABI_ID_ARRAY,  KDV_OTV_ORANLARI_SAYISI );
        UNPACK_INT_ARRAY   ( query.VALUE(3).toString(), M_HESAPLANAN_KDV_HESABI_ID_ARRAY,   KDV_OTV_ORANLARI_SAYISI );
    }

    if ( m_gider_fisi_mi EQ 1 ) {
        m_ui->tablewidget_fis_satirlari->setHorizontalHeaderLabels (QStringList() << "row_id" << "fis_id" << "order_number"
                                                                <<"Hesap No id"<< "POS/KK id"<<"KK Sahibi"<<"KK Numarasi"<<"Ödeme Şekli"
                                                                << tr("Gider Türü") << tr ("Belge No")<< tr ("Açıklama") << tr("Kdv Oranı")
                                                                << tr("Kdv Hariç Tutar") << tr("Kdv Tutarı")
                                                                << tr("Kdv Dahil Tutar") << ""<< tr("") << tr(""));
    }
    else {
        m_ui->tablewidget_fis_satirlari->setHorizontalHeaderLabels (QStringList() << "row_id" << "fis_id" << "order_number"
                                                                << "Hesap No id"<< "POS/KK id"<<"KK Sahibi"<<"KK Numarasi"<<"Tahsilat Şekli"
                                                                << tr("Gelir Türü") << tr ("Belge No")<< tr ("Açıklama") << tr("Kdv Oranı")
                                                                << tr("Kdv Hariç Tutar") << tr("Kdv Tutarı")
                                                                << tr("Kdv Dahil Tutar") << ""<< tr("") << tr(""));
    }

    m_ui->tablewidget_fis_satirlari->setColumnWidth (BELGE_NO_COLUMN            , 111);
    m_ui->tablewidget_fis_satirlari->setColumnWidth (ACIKLAMA_COLUMN            , 200);
    m_ui->tablewidget_fis_satirlari->setColumnWidth (KDV_ORANI_COLUMN           , 74);
    m_ui->tablewidget_fis_satirlari->setColumnWidth (KDV_HARIC_TUTAR_COLUMN     , 122);
    m_ui->tablewidget_fis_satirlari->setColumnWidth (KDV_TUTARI_COLUMN          , 115);
    m_ui->tablewidget_fis_satirlari->setColumnWidth (KDV_DAHIL_TUTAR_COLUMN     , 118);
    m_ui->tablewidget_fis_satirlari->setColumnWidth (GELIR_GIDER_TURU_COLUMN    , 227);
    m_ui->tablewidget_fis_satirlari->setColumnWidth (TAHSILAT_ODEME_SEKLI_COLUMN, 100);
    m_ui->tablewidget_fis_satirlari->setColumnWidth (DETAYLAR_COLUMN            , 100);

    m_ui->lineEdit_fis_no->setMaxLength(10);
    m_ui->lineEdit_sube_ismi->setMaxLength(30);

    DISABLE_CLEAR_ON_WIDGET(m_ui->comboBox_fis_filtresi);
    DISABLE_CLEAR_ON_WIDGET(m_ui->commaEdit_kasa_mevcudu);

    if ( E9_KULLANICI_ENT_BILGILERINI_DEGISTIREBILIR_MI() EQ HAYIR ) {
        m_ui->pushButton_detaylar->setVisible( false );
    }
    if ( E9_KULLANICI_SIRKET_DEGISTIRE_BILIR_MI() EQ HAYIR ) {
        m_ui->comboBox_fis_filtresi->setVisible( false );
        m_ui->label_fis_filtresi->setVisible( false );

        m_ui->comboBox_sirket->setVisible( false );
        m_ui->label_sirket->setVisible( false );
    }

    m_ui->limitedTextEdit_aciklama->SET_MAX_STRING_LENGTH(512);

    m_ui->commaEdit_kasa_mevcudu->EKSI_DEGER_GIRILSIN(true);

    FOCUS_FIRST_WIDGET ();
}

/**************************************************************************************
                   PRK_PERAKENDE_FISI::CLEAR_FORM_MEMBERS();
***************************************************************************************/

void PRK_PERAKENDE_FISI::CLEAR_FORM_MEMBERS()
{

    PRK_CLEAR_FIS_STRUCT(M_FIS);

    E9_CLEAR_ENT_DETAYLARI_STRUCT(M_ENT_DETAYLARI);

    M_ISLEM_DETAYLARI->bnk_hesap_no_id   =   0;
    M_ISLEM_DETAYLARI->kredi_karti_id    =   0;
    M_ISLEM_DETAYLARI->banka_ismi.clear();
    M_ISLEM_DETAYLARI->sube_kodu.clear();
    M_ISLEM_DETAYLARI->sube_adi.clear();
    M_ISLEM_DETAYLARI->hesap_no.clear();
    M_ISLEM_DETAYLARI->pos_id = 0;
    M_ISLEM_DETAYLARI->kredi_karti_numarasi.clear();
    M_ISLEM_DETAYLARI->kredi_karti_sahibi.clear();
}

/**************************************************************************************
                   PRK_PERAKENDE_FISI::NEW_FIS_RECORD
***************************************************************************************/

void PRK_PERAKENDE_FISI::NEW_FIS_RECORD()
{
    m_ui->lineEdit_fis_no->setText("*");

    if ( m_sube_islem_yetkisi EQ TUM_SUBELERDE ) {
        //Eger yalnizca bir subesi varsa sube ismi ve kodu hidelanacak;

        QList < int > sube_id_list = SUBE_GET_SUBE_ID_LIST();

        if ( sube_id_list.size() <= 1 ) {
            m_ui->lineEdit_sube_ismi->setHidden(true);
            m_ui->searchEdit_sube_kodu->setHidden(true);
            m_ui->label_subesi->setHidden(true);
        }
        else {
            QString sube_kodu, sube_adi;
            SUBE_GET_SUBE_KODU_ADI( m_sube_id, &sube_kodu, &sube_adi );

            m_ui->searchEdit_sube_kodu->SET_TEXT( sube_kodu );
            m_ui->lineEdit_sube_ismi->setText   ( sube_adi  );
        }
    }
    else {
        m_ui->lineEdit_sube_ismi->setHidden(true);
        m_ui->searchEdit_sube_kodu->setHidden(true);
        m_ui->label_subesi->setHidden(true);
    }

    M_ENT_DETAYLARI->muh_kasa_hesap_id = SUBE_GET_UNITE_MUH_KASA_HESABI_ID();
    M_FIS->muh_kasa_hesap_id = M_ENT_DETAYLARI->muh_kasa_hesap_id;

    SET_KASA_MEVCUDU();
}

/**************************************************************************************
                   PRK_PERAKENDE_FISI::GET_FIS_RECORD
***************************************************************************************/

int PRK_PERAKENDE_FISI::GET_FIS_RECORD ( int record_id )
{
    SQL_QUERY query(DB);

    query.PREPARE_SELECT("prk_fisler" ,
                         "fis_no , fis_tarihi , aciklama , sube_id , kdv_haric_toplam , kdv_toplam ,"
                         "kdv_dahil_toplam, modul_id , program_id, "
                         "muh_fis_id , base_fis_id,muh_kasa_hesap_id" ,
                         "fis_id = :fis_id AND gider_fisi_mi = :gider_fisi_mi");

    query.SET_VALUE(":fis_id"        , record_id);
    query.SET_VALUE(":gider_fisi_mi" , m_gider_fisi_mi);

    if ( query.SELECT() EQ 0 ) {
        return ADAK_FAIL;
    }

    query.NEXT();

    m_ui->lineEdit_fis_no->setText(query.VALUE(0).toString());
    m_ui->adakDate_fis_tarihi->SET_DATE(query.VALUE(1).toString());
    m_ui->limitedTextEdit_aciklama->setText(query.VALUE(2).toString());
    int sube_id = query.VALUE(3).toInt();

    m_ui->commaEdit_kdv_haric_toplam->SET_DOUBLE(query.VALUE(4).toDouble());
    m_ui->commaEdit_kdv_toplam->SET_DOUBLE(query.VALUE(5).toDouble());
    m_ui->commaEdit_kdv_dahil_toplam->SET_DOUBLE(query.VALUE(6).toDouble());

    M_FIS->fis_no           = query.VALUE(0).toInt();
    M_FIS->fis_tarihi       = m_ui->adakDate_fis_tarihi->DATE();
    M_FIS->aciklama         = query.VALUE(2).toString();
    M_FIS->base_fis_id      = query.VALUE("base_fis_id").toInt();
    M_FIS->sube_id          = sube_id;
    M_FIS->modul_id         = query.VALUE(7).toInt();
    M_FIS->program_id       = query.VALUE(8).toInt();
    M_FIS->muh_fis_id       = query.VALUE(9).toInt();
    M_FIS->gider_fisi_mi    = m_gider_fisi_mi;
    M_FIS->muh_kasa_hesap_id= query.VALUE("muh_kasa_hesap_id").toInt();

    M_ENT_DETAYLARI->muh_kasa_hesap_id = M_FIS->muh_kasa_hesap_id;

    QString sube_kodu, sube_adi;

    SUBE_GET_SUBE_KODU_ADI( sube_id, &sube_kodu, &sube_adi );

    m_ui->searchEdit_sube_kodu->SET_TEXT    ( sube_kodu );
    m_ui->lineEdit_sube_ismi->setText       ( sube_adi  );

    query.PREPARE_SELECT("prk_fis_satirlari" , "fis_satiri_id , order_number , belge_no , "
                         "aciklama , kdv_orani , kdv_haric_tutar , "
                         "kdv_tutari , kdv_dahil_tutar , gider_turu_id,tah_odm_sekli,"
                         "hesap_no_id , kredi_karti_id,pos_id,kredi_karti_sahibi,"
                         "kredi_karti_numarasi" ,"fis_id = :fis_id");


    query.SET_VALUE(":fis_id" , record_id);

    if ( query.SELECT("order_number ASC") EQ 0 ) {
        return ADAK_OK;
    }

    int current_row = -1;

    QTableWidgetItem * item;
    SQL_QUERY s_query(DB);

    while ( query.NEXT() ) {

        current_row = ADD_NEW_LINE();

        item = new QTableWidgetItem(query.VALUE(0).toString());
        m_ui->tablewidget_fis_satirlari->setItem(current_row , ROW_ID_COLUMN , item);

        item = new QTableWidgetItem(query.VALUE(1).toString());
        m_ui->tablewidget_fis_satirlari->setItem(current_row , ORDER_NUMBER_COLUMN , item);

        item = new QTableWidgetItem(QVariant(record_id).toString());
        m_ui->tablewidget_fis_satirlari->setItem(current_row , FIS_ID_COLUMN , item);

        QLineEdit * lineedit_belge_no          = ( QLineEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(current_row,BELGE_NO_COLUMN);
        lineedit_belge_no->setText(query.VALUE(2).toString());

        QLineEdit * lineedit_aciklama          = ( QLineEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(current_row,ACIKLAMA_COLUMN);
        lineedit_aciklama->setText(query.VALUE(3).toString());

        QComboBox * combobox_kdv_orani         = ( QComboBox * ) m_ui->tablewidget_fis_satirlari->cellWidget(current_row , KDV_ORANI_COLUMN);
        combobox_kdv_orani->setCurrentIndex(combobox_kdv_orani->findText(query.VALUE(4).toString()));

        QCommaEdit * commaedit_kdv_haric_tutar = ( QCommaEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(current_row , KDV_HARIC_TUTAR_COLUMN);
        commaedit_kdv_haric_tutar->SET_DOUBLE(query.VALUE(5).toDouble());

        QCommaEdit * commaedit_kdv_tutari      = ( QCommaEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(current_row , KDV_TUTARI_COLUMN);
        commaedit_kdv_tutari->SET_DOUBLE(query.VALUE(6).toDouble());

        QCommaEdit * commaedit_kdv_dahil_tutar = ( QCommaEdit *) m_ui->tablewidget_fis_satirlari->cellWidget(current_row , KDV_DAHIL_TUTAR_COLUMN);
        commaedit_kdv_dahil_tutar->SET_DOUBLE(query.VALUE(7).toDouble());

        QComboBox * combobox_gider_turu    = ( QComboBox * ) m_ui->tablewidget_fis_satirlari->cellWidget(current_row,GELIR_GIDER_TURU_COLUMN);

        int gider_turu_id = query.VALUE(8).toInt();

        int tah_odm_sekli = query.VALUE(9).toInt();

        QComboBox * combobox_tah_odm_sekli = ( QComboBox * ) m_ui->tablewidget_fis_satirlari->cellWidget(current_row , TAHSILAT_ODEME_SEKLI_COLUMN);
        combobox_tah_odm_sekli->setCurrentIndex(combobox_tah_odm_sekli->findText(E9_GET_TAH_ODM_SEKLI_STRING(tah_odm_sekli)));

        QLineEdit * lineedit_hesap_no_id = ( QLineEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(current_row , HESAP_NO_ID_COLUMN);
        lineedit_hesap_no_id->setText(query.VALUE("hesap_no_id").toString());


        QLineEdit * lineedit_pos_kk_id = ( QLineEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(current_row , POS_KK_ID_COLUMN);

        if ( M_FIS->gider_fisi_mi EQ 1 ) {
            lineedit_pos_kk_id->setText(query.VALUE("kredi_karti_id").toString());
        }
        else {
            lineedit_pos_kk_id->setText(query.VALUE("pos_id").toString());
        }

        QLineEdit * lineedit_kk_sahibi = ( QLineEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(current_row , KK_SAHIBI_COLUMN);
        lineedit_kk_sahibi->setText(query.VALUE("kredi_karti_sahibi").toString());

        QLineEdit * lineedit_kk_numarasi = ( QLineEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(current_row , KK_NUMARASI_COLUMN);
        lineedit_kk_numarasi->setText(query.VALUE("kredi_karti_numarasi").toString());


        QPushButton * button_detaylar = ( QPushButton * ) m_ui->tablewidget_fis_satirlari->cellWidget(current_row , DETAYLAR_COLUMN);

        button_detaylar->setEnabled(true);

        switch ( tah_odm_sekli) {
            case ENUM_NAKIT:
                button_detaylar->setText("");
                button_detaylar->setDisabled(true);
                break;
            default:
                button_detaylar->setText("Detaylar");
                break;
        }

        s_query.PREPARE_SELECT("prk_gelir_gider_turleri","gelir_gider_adi" ,"tur_id = :tur_id");
        s_query.SET_VALUE(":tur_id" , gider_turu_id);

        if ( s_query.SELECT() NE 0 ) {
            s_query.NEXT();
            combobox_gider_turu->setCurrentIndex(combobox_gider_turu->findText(s_query.VALUE(0).toString()));
        }
    }
    return ADAK_OK;
}

/**************************************************************************************
                   PRK_PERAKENDE_FISI::SET_LINE_DEFAULTS
***************************************************************************************/

void PRK_PERAKENDE_FISI::SET_LINE_DEFAULTS ( int row_number )
{
    QComboBox * combobox_kdv_orani   = ( QComboBox * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number , KDV_ORANI_COLUMN);
    QLineEdit * lineedit_aciklama    = ( QLineEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number , ACIKLAMA_COLUMN);
    QComboBox * combobox_gider_turu  = ( QComboBox * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number , GELIR_GIDER_TURU_COLUMN);
    QLineEdit * lineedit_belge_no    = ( QLineEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number , BELGE_NO_COLUMN);
    QComboBox * combobox_tah_odm_sekli = ( QComboBox * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number, TAHSILAT_ODEME_SEKLI_COLUMN );
    QPushButton * button_detaylar    = ( QPushButton * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number , DETAYLAR_COLUMN);

    button_detaylar->setDisabled(true);
    button_detaylar->setText("");


    SQL_QUERY query(DB);

    query.PREPARE_SELECT("prk_gelir_gider_turleri" , "gelir_gider_adi" , "sube_id = :sube_id AND gider_turu_mu = :gider_turu_mu");


    query.SET_VALUE(":sube_id" , m_sube_id);
    if ( m_gider_fisi_mi EQ 1 ) {
        query.SET_VALUE(":gider_turu_mu" , 1);
    }
    else {
        query.SET_VALUE(":gider_turu_mu" , 0);
    }

    if ( query.SELECT("gelir_gider_adi ASC") NE 0 ) {
        while ( query.NEXT() ) {
            combobox_gider_turu->addItem(query.VALUE(0).toString());
        }
        combobox_gider_turu->setCurrentIndex(-1);
    }

    E9_FILL_KDV_ORANLARI_COMBOBOX(combobox_kdv_orani);

    combobox_kdv_orani->setCurrentIndex(combobox_kdv_orani->findText(QVariant(m_ontanimli_kdv_orani).toString()));

    lineedit_aciklama->setMaxLength(128);

    lineedit_belge_no->setMaxLength(20);

    combobox_tah_odm_sekli->addItems(QStringList() << QObject::tr("Nakit")<< QObject::tr("Kredi Kartı")
                                   << QObject::tr("Havale")<< QObject::tr("Eft"));

    combobox_tah_odm_sekli->setCurrentIndex(-1);

    REGISTER_LINE_CHANGER_BUTTON(row_number , DETAYLAR_COLUMN);

}
/**************************************************************************************
                   PRK_PERAKENDE_FISI::CHECK_FIS_FORM_VAR
***************************************************************************************/

int PRK_PERAKENDE_FISI::CHECK_FIS_FORM_VAR ( QObject * object )
{

    if ( object EQ m_ui->searchEdit_sube_kodu ) {

        if ( m_ui->searchEdit_sube_kodu->GET_TEXT().isEmpty() EQ true ) {
            m_ui->lineEdit_sube_ismi->clear();
            m_sube_id = -1;
            return ADAK_OK;
        }

        QString sube_adi;
        int     sube_id;

        int result = SUBE_GET_SUBE_ID_ADI( m_ui->searchEdit_sube_kodu->GET_TEXT() , &sube_id, &sube_adi );

        if ( result EQ 0 ) {
            QMessageBox::warning(this,"UYARI",tr("Aradığınız sube kodu bulunamadı."));
            SET_FOCUS( m_ui->searchEdit_sube_kodu );
            return ADAK_FAIL_UNDO;
        }
        m_ui->lineEdit_sube_ismi->setText( sube_adi );
        m_sube_id = sube_id;
    }
    return ADAK_OK;
}

/**************************************************************************************
                   PRK_PERAKENDE_FISI::CHECK_FIS_FORM_EMPTY
***************************************************************************************/

int PRK_PERAKENDE_FISI::CHECK_FIS_FORM_EMPTY()
{
    if ( m_ui->lineEdit_fis_no->text().isEmpty() EQ true ) {
        QMessageBox::warning(this,"UYARI",tr("Yeni Fiş Kaydı Yapabilmek için Fiş No değeri olarak (*) girmelisiniz!.."));
        SET_FOCUS( m_ui->lineEdit_fis_no );
        return ADAK_FAIL;
    }

    return ADAK_OK;
}

/**************************************************************************************
                   PRK_PERAKENDE_FISI::CHECK_FIS_RECORD_ADD
***************************************************************************************/

int PRK_PERAKENDE_FISI::CHECK_FIS_RECORD_ADD ()
{

    if ( QString ( m_ui->lineEdit_fis_no->text().at(0)) NE "*") {

        SQL_QUERY query(DB);

        query.PREPARE_SELECT("prk_fisler","fis_id" ,"fis_tarihi = :fis_tarihi AND fis_no = :fis_no");
        query.SET_VALUE(":fis_tarihi" , m_ui->adakDate_fis_tarihi->DATE());
        query.SET_VALUE(":fis_no"     , m_ui->lineEdit_fis_no->text().toInt());

        if ( query.SELECT() NE 0 ) {
            QMessageBox::warning(this , "UYARI" , tr("Aynı fiş numarası ile kayıtlı başka bir fiş bulunmaktadır.Fiş kaydedilemedi."));
            SET_FOCUS( m_ui->lineEdit_fis_no );
            return ADAK_FAIL;
        }
    }



    return ADAK_OK;
}

/**************************************************************************************
                   PRK_PERAKENDE_FISI::ADD_FIS_RECORD
***************************************************************************************/

int PRK_PERAKENDE_FISI::ADD_FIS_RECORD ()
{
    int fis_no = 0;

    if ( QString (m_ui->lineEdit_fis_no->text().at(0)) EQ "*" ) {
        fis_no = PRK_SIRADAKI_FIS_NO_AL(m_ui->adakDate_fis_tarihi->DATE());
    }
    else {
        fis_no = m_ui->lineEdit_fis_no->text().toInt();
    }

    M_FIS->program_id       = E9_PROGRAMI;
    M_FIS->modul_id         = PERAKENDE_MODULU;
    M_FIS->fis_no           = fis_no;
    M_FIS->aciklama         = m_ui->limitedTextEdit_aciklama->toPlainText();
    M_FIS->fis_tarihi       = m_ui->adakDate_fis_tarihi->DATE();
    M_FIS->gider_fisi_mi    = m_gider_fisi_mi;
    M_FIS->sube_id          = m_sube_id;

    int fis_id = PRK_FIS_EKLE ( M_FIS );

    m_ui->lineEdit_fis_no->setText(QVariant(fis_no).toString());

    return fis_id;

}

/**************************************************************************************
                   PRK_PERAKENDE_FISI::CHECK_FIS_RECORD_UPDATE
***************************************************************************************/

int PRK_PERAKENDE_FISI::CHECK_FIS_RECORD_UPDATE ( int record_id )
{

    if ( QString ( m_ui->lineEdit_fis_no->text().at(0)) NE "*") {

        SQL_QUERY query(DB);

        query.PREPARE_SELECT("prk_fisler","fis_id"  ,"fis_tarihi = :fis_tarihi AND fis_no = :fis_no");
        query.SET_VALUE(":fis_tarihi" , m_ui->adakDate_fis_tarihi->DATE());
        query.SET_VALUE(":fis_no"     , m_ui->lineEdit_fis_no->text().toInt());

        if ( query.SELECT() NE 0 ) {
            query.NEXT();

            if ( query.VALUE(0).toInt() NE record_id ) {

                QMessageBox::warning(this , "UYARI" , tr("Aynı fiş numarası ile kayıtlı başka bir fiş bulunmaktadır.Fiş güncellenemedi."));
                SET_FOCUS( m_ui->lineEdit_fis_no );
                return ADAK_FAIL;
            }
        }
    }

    return ADAK_OK;
}

/**************************************************************************************
                   PRK_PERAKENDE_FISI::UPDATE_FIS_RECORD
***************************************************************************************/

void PRK_PERAKENDE_FISI::UPDATE_FIS_RECORD ( int record_id )
{

    PRK_FIS_BILGILERINI_OKU  ( M_FIS , record_id );

    M_FIS->fis_no           = m_ui->lineEdit_fis_no->text().toInt();
    M_FIS->fis_tarihi       = m_ui->adakDate_fis_tarihi->DATE();
    M_FIS->aciklama         = m_ui->limitedTextEdit_aciklama->toPlainText();
    M_FIS->sube_id          = m_sube_id;
    M_FIS->muh_kasa_hesap_id= M_ENT_DETAYLARI->muh_kasa_hesap_id;

    PRK_FIS_GUNCELLE ( M_FIS , record_id);
}

/**************************************************************************************
                   PRK_PERAKENDE_FISI::CHECK_FIS_RECORD_DELETE
***************************************************************************************/

int PRK_PERAKENDE_FISI::CHECK_FIS_RECORD_DELETE(int record_id)
{

    Q_UNUSED(record_id);

    return ADAK_OK;
}

/**************************************************************************************
                   PRK_PERAKENDE_FISI::DELETE_FIS_RECORD
***************************************************************************************/

void PRK_PERAKENDE_FISI::DELETE_FIS_RECORD ( int record_id )
{
    PRK_FIS_SIL ( M_FIS , record_id );
}

/**************************************************************************************
                   PRK_PERAKENDE_FISI::SELECT_FIS_RECORD
***************************************************************************************/

int PRK_PERAKENDE_FISI::SELECT_FIS_RECORD()
{
    return OPEN_PRK_FIS_ARAMA(m_gider_fisi_mi , this);
}

/**************************************************************************************
                   PRK_PERAKENDE_FISI::FIND_FIS_RECORD
***************************************************************************************/

int PRK_PERAKENDE_FISI::FIND_FIS_RECORD()
{
    SQL_QUERY query(DB);

    query.PREPARE_SELECT("prk_fisler","fis_id" ,"fis_no = :fis_no AND fis_tarihi = :fis_tarihi "
                         "AND gider_fisi_mi = :gider_fisi_mi");
    query.SET_VALUE(":fis_no"        , m_ui->lineEdit_fis_no->text().toInt());
    query.SET_VALUE(":fis_tarihi"    , m_ui->adakDate_fis_tarihi->DATE());
    query.SET_VALUE(":gider_fisi_mi" , m_gider_fisi_mi);

    if ( query.SELECT() EQ 0 ) {
        QMessageBox::warning ( this , "UYARI" , tr("Aradığınız perakende fişi bulunamadı."));
        SET_FOCUS( m_ui->lineEdit_fis_no );
        return 0;
    }
    query.NEXT();

    return query.VALUE(0).toInt();
}

/**************************************************************************************
                   PRK_PERAKENDE_FISI::FIND_FIRST_FIS_RECORD
***************************************************************************************/

int PRK_PERAKENDE_FISI::FIND_FIRST_FIS_RECORD()
{
    SQL_QUERY query(DB);

    query.PREPARE_SELECT("prk_fisler" , "fis_id" ,
                         "sube_id = :sube_id AND gider_fisi_mi = :gider_fisi_mi ");

    query.SET_VALUE(":sube_id"       , m_sube_id);
    query.SET_VALUE(":gider_fisi_mi" , m_gider_fisi_mi);

    if ( query.SELECT("fis_tarihi ASC , fis_no ASC" , 0 , 1) EQ 0 ) {
        return 0;
    }

    query.NEXT();

    return query.VALUE(0).toInt();
}

/**************************************************************************************
                   PRK_PERAKENDE_FISI::FIND_LAST_FIS_RECORD
***************************************************************************************/

int PRK_PERAKENDE_FISI::FIND_LAST_FIS_RECORD()
{
    SQL_QUERY query(DB);

    query.PREPARE_SELECT("prk_fisler" , "fis_id" ,
                         " sube_id = :sube_id AND gider_fisi_mi = :gider_fisi_mi ");

    query.SET_VALUE(":sube_id"       , m_sube_id);
    query.SET_VALUE(":gider_fisi_mi" , m_gider_fisi_mi);

    if ( query.SELECT("fis_tarihi DESC , fis_no DESC" , 0 , 1) EQ 0 ) {
        return 0;
    }

    query.NEXT();

    return query.VALUE(0).toInt();
}

/**************************************************************************************
             PRK_PERAKENDE_FISI::FIND_PREV_FIS_RECORD
***************************************************************************************/

int PRK_PERAKENDE_FISI::FIND_PREV_FIS_RECORD()
{
    SQL_QUERY query(DB);

    query.PREPARE_SELECT("prk_fisler" , "fis_id" , " fis_tarihi = :fis_tarihi AND "
                         "fis_no < :fis_no AND sube_id = :sube_id "
                         "AND gider_fisi_mi = :gider_fisi_mi ");


    query.SET_VALUE(":fis_tarihi"    , m_ui->adakDate_fis_tarihi->DATE());
    query.SET_VALUE(":fis_no"        , m_ui->lineEdit_fis_no->text().toInt());
    query.SET_VALUE(":sube_id"       , m_sube_id );
    query.SET_VALUE(":gider_fisi_mi" , m_gider_fisi_mi);

    if ( query.SELECT("fis_no DESC" , 0 , 1) NE 0 ) {
        query.NEXT();

        return query.VALUE(0).toInt();
    }

    query.PREPARE_SELECT("prk_fisler" , "fis_id" , " fis_tarihi < :fis_tarihi AND sube_id = :sube_id "
                         "AND gider_fisi_mi = :gider_fisi_mi ");

    query.SET_VALUE(":fis_tarihi"    , m_ui->adakDate_fis_tarihi->DATE());
    query.SET_VALUE(":sube_id"       , m_sube_id);
    query.SET_VALUE(":gider_fisi_mi" , m_gider_fisi_mi);

    if ( query.SELECT("fis_tarihi DESC,fis_no DESC" , 0,1) EQ 0 ) {
        return 0;
    }
    query.NEXT();

    return query.VALUE(0).toInt();

}

/**************************************************************************************
                   PRK_PERAKENDE_FISI::FIND_NEXT_FIS_RECORD
***************************************************************************************/

int PRK_PERAKENDE_FISI::FIND_NEXT_FIS_RECORD()
{
    SQL_QUERY query(DB);

    query.PREPARE_SELECT("prk_fisler" , "fis_id" , " fis_tarihi = :fis_tarihi AND fis_no > :fis_no "
                         "AND gider_fisi_mi = :gider_fisi_mi AND sube_id = :sube_id ");

    query.SET_VALUE(":fis_tarihi"     , m_ui->adakDate_fis_tarihi->DATE());
    query.SET_VALUE(":fis_no"         , m_ui->lineEdit_fis_no->text().toInt());
    query.SET_VALUE(":gider_fisi_mi"  , m_gider_fisi_mi);
    query.SET_VALUE(":sube_id"        , m_sube_id );

    if ( query.SELECT("fis_no ASC" , 0 , 1) NE 0 ) {
        query.NEXT();

        return query.VALUE(0).toInt();
    }


    query.PREPARE_SELECT("prk_fisler" , "fis_id" ,
                         " fis_tarihi > :fis_tarihi AND gider_fisi_mi = :gider_fisi_mi ");

    query.SET_VALUE(":fis_tarihi"    , m_ui->adakDate_fis_tarihi->DATE());
    query.SET_VALUE(":gider_fisi_mi" , m_gider_fisi_mi);

    if ( query.SELECT("fis_tarihi ASC , fis_no ASC" , 0 , 1) EQ 0 ) {
        return 0;
    }
    query.NEXT();

    return query.VALUE(0).toInt();

}



/**************************************************************************************
                   PRK_PERAKENDE_FISI::CHECK_LINE_VAR
***************************************************************************************/

int PRK_PERAKENDE_FISI::CHECK_LINE_VAR ( int row_number, QObject * object )
{

    QCommaEdit * commaedit_kdv_haric_tutar = ( QCommaEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number , KDV_HARIC_TUTAR_COLUMN);
    QCommaEdit * commaedit_kdv_tutari      = ( QCommaEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number , KDV_TUTARI_COLUMN);
    QCommaEdit * commaedit_kdv_dahil_tutar = ( QCommaEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number , KDV_DAHIL_TUTAR_COLUMN);
    QComboBox  * combobox_kdv_orani        = ( QComboBox  * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number , KDV_ORANI_COLUMN);
    QComboBox  * combobox_tah_odm_sekli    = ( QComboBox  * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number , TAHSILAT_ODEME_SEKLI_COLUMN );
    QComboBox  * combobox_gelir_gider_turu = ( QComboBox  * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number , GELIR_GIDER_TURU_COLUMN);
    QLineEdit  * lineedit_belge_no         = ( QLineEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number , BELGE_NO_COLUMN );
    QLineEdit  * lineedit_aciklama         = ( QLineEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number , ACIKLAMA_COLUMN );



    if ( ( object EQ commaedit_kdv_haric_tutar) OR  ( object EQ commaedit_kdv_dahil_tutar )
        OR ( object EQ commaedit_kdv_tutari ) OR ( object EQ combobox_kdv_orani )) {

        double kdv_haric_tutar = 0.0;
        double kdv_tutari      = 0.0;
        double kdv_dahil_tutar = 0.0;
        double kdv_orani       = QVariant(combobox_kdv_orani->currentText()).toDouble();

        if ( object EQ commaedit_kdv_haric_tutar ) {
            kdv_haric_tutar = commaedit_kdv_haric_tutar->GET_DOUBLE();
            kdv_dahil_tutar = ROUND(kdv_haric_tutar * ( 1 + (kdv_orani / 100)));
            kdv_tutari      = kdv_dahil_tutar - kdv_haric_tutar;

        }
        else if ( object EQ commaedit_kdv_tutari ) {
            kdv_tutari      = commaedit_kdv_tutari->GET_DOUBLE();
            kdv_haric_tutar = kdv_tutari * (100 / kdv_orani);
            kdv_dahil_tutar = kdv_haric_tutar + kdv_tutari;
        }
        else {
            kdv_dahil_tutar = commaedit_kdv_dahil_tutar->GET_DOUBLE();
            kdv_tutari      = ( kdv_dahil_tutar * kdv_orani) / (kdv_orani + 100);
            kdv_haric_tutar = kdv_dahil_tutar - kdv_tutari;

        }

        commaedit_kdv_dahil_tutar->SET_DOUBLE(kdv_dahil_tutar);
        commaedit_kdv_haric_tutar->SET_DOUBLE(kdv_haric_tutar);
        commaedit_kdv_tutari->SET_DOUBLE(kdv_tutari);

        ////

        if ( E9_GET_TAH_ODM_SEKLI_ENUM(combobox_tah_odm_sekli->currentText()) EQ ENUM_NAKIT  ) {
            double max_nakit_tutari         = 0.00;
            double kdv_dahil_toplam_tutar   = 0.00;
            double islem_tutar      = ROUND( commaedit_kdv_dahil_tutar->GET_DOUBLE() );
            SQL_QUERY query( DB );

            query.PREPARE_SELECT("prk_fisler","kdv_dahil_toplam" ,"fis_id = :fis_id");
            query.SET_VALUE(":fis_id" , GET_RECORD_ID() );

            if ( query.SELECT() NE 0 ) {
                query.NEXT();
                kdv_dahil_toplam_tutar = query.VALUE(0).toDouble();
            }
            query.PREPARE_SELECT("e9_sabit_degerler" , "max_nakit_giris_cikis_tutari" ,
                                 "sabit_deger_id = :sabit_deger_id");
            query.SET_VALUE(":sabit_deger_id" , 1);

            if ( query.SELECT() NE 0 ) {
                query.NEXT();
                max_nakit_tutari    = query.VALUE( 0 ).toDouble();
            }

            if ( kdv_dahil_toplam_tutar + islem_tutar > max_nakit_tutari ) {
                int msg_secim   = MSG_YES_NO( tr( "Vergi Usul Kanununca belirlenen tahsilat ve ödeme sınırlarını aştınız. \n "
                                                  "İşleme Devam Etmek İstermisiniz ?" ), commaedit_kdv_dahil_tutar );

                if ( msg_secim EQ ADAK_NO ) {
                    return ADAK_FAIL_UNDO;
                }
             }
        }


    }
    else if ( object EQ combobox_tah_odm_sekli ) {
        QPushButton * button_detaylar = ( QPushButton * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number,DETAYLAR_COLUMN);

        int tah_odm_sekli = E9_GET_TAH_ODM_SEKLI_ENUM(combobox_tah_odm_sekli->currentText());

        button_detaylar->setEnabled(true);

        switch (tah_odm_sekli ) {
            case ENUM_NAKIT :
                button_detaylar->setDisabled(true);
                button_detaylar->setText("");
                break;

            default:
                button_detaylar->setText("Detaylar");
                break;
        }
        combobox_gelir_gider_turu->setCurrentIndex(-1);
        lineedit_aciklama->clear();
        lineedit_belge_no->clear();
        combobox_kdv_orani->setCurrentIndex(combobox_kdv_orani->findText(QVariant(m_ontanimli_kdv_orani).toString()));
        commaedit_kdv_dahil_tutar->clear();
        commaedit_kdv_haric_tutar->clear();
        commaedit_kdv_tutari->clear();

        M_ISLEM_DETAYLARI->bnk_hesap_no_id   =   0;
        M_ISLEM_DETAYLARI->kredi_karti_id    =   0;
        M_ISLEM_DETAYLARI->banka_ismi.clear();
        M_ISLEM_DETAYLARI->sube_kodu.clear();
        M_ISLEM_DETAYLARI->sube_adi.clear();
        M_ISLEM_DETAYLARI->hesap_no.clear();
        M_ISLEM_DETAYLARI->pos_id = 0;
        M_ISLEM_DETAYLARI->kredi_karti_numarasi.clear();
        M_ISLEM_DETAYLARI->kredi_karti_sahibi.clear();


        QLineEdit * lineedit_hesap_no_id = ( QLineEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number , HESAP_NO_ID_COLUMN);
        lineedit_hesap_no_id->clear();

        QLineEdit * lineedit_pos_kk_id  = ( QLineEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number , POS_KK_ID_COLUMN);
        lineedit_pos_kk_id->clear();

        QLineEdit * lineedit_kk_sahibi = ( QLineEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number , KK_SAHIBI_COLUMN);
        lineedit_kk_sahibi->clear();

        QLineEdit * lineedit_kk_numarasi = ( QLineEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number , KK_NUMARASI_COLUMN);
        lineedit_kk_numarasi->clear();


    }

    return ADAK_OK;
}

/**************************************************************************************
                   PRK_PERAKENDE_FISI::CHECK_LINE_EMPTY
***************************************************************************************/

int PRK_PERAKENDE_FISI::CHECK_LINE_EMPTY ( int row_number )
{
    QCommaEdit * commaedit_kdv_haric_tutar = ( QCommaEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number , KDV_HARIC_TUTAR_COLUMN);
    QCommaEdit * commaedit_kdv_tutari      = ( QCommaEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number , KDV_TUTARI_COLUMN);
    QCommaEdit * commaedit_kdv_dahil_tutar = ( QCommaEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number , KDV_DAHIL_TUTAR_COLUMN);
    QComboBox  * combobox_gider_turu       = ( QComboBox * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number  , GELIR_GIDER_TURU_COLUMN);
    QLineEdit  * lineedit_belge_no         = ( QLineEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number , BELGE_NO_COLUMN);
    QComboBox  * combobox_tah_odm_sekli      = ( QComboBox * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number , TAHSILAT_ODEME_SEKLI_COLUMN );

    if ( combobox_tah_odm_sekli->currentIndex() EQ -1 ) {
        if ( m_gider_fisi_mi EQ 1 ) {
            MSG_WARNING(QObject::tr("Ödeme şeklini seçmelisiniz."),combobox_tah_odm_sekli);
        }
        else {
            MSG_WARNING(QObject::tr("Tahsilat şeklini seçmelisiniz."),combobox_tah_odm_sekli);
        }
        return ADAK_FAIL;
    }

    if ( combobox_gider_turu->currentIndex() EQ -1 ) {
        if ( m_gider_fisi_mi EQ 1 ) {
            QMessageBox::warning(this,"UYARI",tr("Gider Türünü seçmelisiniz."));
        }
        else {
            QMessageBox::warning(this,"UYARI",tr("Gelir Türünü seçmelisiniz."));
        }
        SET_FOCUS( combobox_gider_turu );
        return ADAK_FAIL;
    }

    if ( lineedit_belge_no->text().isEmpty() EQ true ) {
        MSG_INFO(QObject::tr("Belge numarasını girmelisiniz.") , lineedit_belge_no);
        return ADAK_FAIL;
    }

    if ( commaedit_kdv_haric_tutar->GET_DOUBLE() EQ 0 AND commaedit_kdv_tutari->GET_DOUBLE() EQ 0 AND
         commaedit_kdv_dahil_tutar->GET_DOUBLE() EQ 0 ) {
        QMessageBox::warning(this , "UYARI" , tr("Tutar girmelisiniz."));
        SET_FOCUS( commaedit_kdv_haric_tutar );
        return ADAK_FAIL;
    }


    return ADAK_OK;
}

/**************************************************************************************
                   PRK_PERAKENDE_FISI::CHECK_ADD_LINE
***************************************************************************************/

int PRK_PERAKENDE_FISI::CHECK_ADD_LINE ( int record_id, int row_number )
{

    Q_UNUSED ( record_id );

    QComboBox * combobox_tah_odm_sekli = ( QComboBox * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number , TAHSILAT_ODEME_SEKLI_COLUMN);

    int tah_odm_sekli = E9_GET_TAH_ODM_SEKLI_ENUM(combobox_tah_odm_sekli->currentText());

    int fis_satiri_id = -1;

    QTableWidgetItem * item = m_ui->tablewidget_fis_satirlari->item(row_number ,ROW_ID_COLUMN);

    if ( item NE NULL ) {
        fis_satiri_id = item->text().toInt();
    }

    switch ( tah_odm_sekli ) {
        case ENUM_HAVALE:
        {
            QLineEdit * lineedit_hesap_no_id = ( QLineEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number , HESAP_NO_ID_COLUMN);

            if ( lineedit_hesap_no_id->text().toInt() EQ 0 ) {
                if ( OPEN_E9_EFT_BATCH(-1 , ENUM_HAVALE , fis_satiri_id ,DVZ_GET_TEMEL_PARA_BIRIMI_ID(),
                                       M_ISLEM_DETAYLARI , PERAKENDE_MODULU , this) EQ false ){
                    return ADAK_FAIL;
                }

                lineedit_hesap_no_id->setText(QVariant(M_ISLEM_DETAYLARI->bnk_hesap_no_id).toString());
            }
        }
        break;

        case ENUM_EFT   :
        {
            QLineEdit * lineedit_hesap_no_id = ( QLineEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number , HESAP_NO_ID_COLUMN);

            if ( lineedit_hesap_no_id->text().toInt() EQ 0 ) {
                if ( OPEN_E9_EFT_BATCH(-1 , ENUM_EFT,fis_satiri_id ,DVZ_GET_TEMEL_PARA_BIRIMI_ID(),
                                       M_ISLEM_DETAYLARI , PERAKENDE_MODULU , this) EQ false ){

                    return ADAK_FAIL;
                }

                lineedit_hesap_no_id->setText(QVariant(M_ISLEM_DETAYLARI->bnk_hesap_no_id).toString());
            }
        }

            break;

        case ENUM_KREDI_KARTI:
        {

            QLineEdit * lineedit_pos_kk_id = ( QLineEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number , POS_KK_ID_COLUMN);

            if ( m_gider_fisi_mi EQ 1 ) {
                if ( lineedit_pos_kk_id->text().toInt() EQ 0 ) {
                    if ( OPEN_E9_KK_ODEME_BATCH(fis_satiri_id , M_ISLEM_DETAYLARI ,
                                                PERAKENDE_MODULU,this) EQ false ) {
                        return ADAK_FAIL;
                    }

                    lineedit_pos_kk_id->setText(QVariant( M_ISLEM_DETAYLARI->kredi_karti_id).toString());
                }
            }
            else {
                if ( lineedit_pos_kk_id->text().toInt() EQ 0 ) {
                    if ( OPEN_E9_KK_TAHSILAT_BATCH(fis_satiri_id , M_ISLEM_DETAYLARI , PERAKENDE_MODULU,
                                                   this) EQ false ) {
                        return ADAK_FAIL;
                    }
                    lineedit_pos_kk_id->setText(QVariant(M_ISLEM_DETAYLARI->pos_id).toString());

                    QLineEdit * lineedit_kk_sahibi = ( QLineEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number , KK_SAHIBI_COLUMN);
                    lineedit_kk_sahibi->setText(M_ISLEM_DETAYLARI->kredi_karti_sahibi);

                    QLineEdit * lineedit_kk_numarasi = ( QLineEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number , KK_NUMARASI_COLUMN);
                    lineedit_kk_numarasi->setText(M_ISLEM_DETAYLARI->kredi_karti_numarasi);
                }
            }
            QLineEdit * lineedit_hesap_no_id = ( QLineEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number , HESAP_NO_ID_COLUMN);
            lineedit_hesap_no_id->setText(QVariant(M_ISLEM_DETAYLARI->bnk_hesap_no_id).toString());

        }
        break;

        default:
            break;

    }

    return ADAK_OK;
}

/**************************************************************************************
                   PRK_PERAKENDE_FISI::ADD_LINE
***************************************************************************************/

void PRK_PERAKENDE_FISI::ADD_LINE ( int record_id, int row_number )
{

    QCommaEdit * commaedit_kdv_haric_tutar = ( QCommaEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number , KDV_HARIC_TUTAR_COLUMN);
    QCommaEdit * commaedit_kdv_tutari      = ( QCommaEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number , KDV_TUTARI_COLUMN);
    QCommaEdit * commaedit_kdv_dahil_tutar = ( QCommaEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number , KDV_DAHIL_TUTAR_COLUMN);
    QComboBox  * combobox_kdv_orani        = ( QComboBox  * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number , KDV_ORANI_COLUMN);
    QLineEdit  * lineedit_belge_no         = ( QLineEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number , BELGE_NO_COLUMN);
    QLineEdit  * lineedit_aciklama         = ( QLineEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number , ACIKLAMA_COLUMN);
    QComboBox  * combobox_gider_turu       = ( QComboBox  * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number , GELIR_GIDER_TURU_COLUMN);
    QComboBox  * combobox_tah_odm_sekli    = ( QComboBox  * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number , TAHSILAT_ODEME_SEKLI_COLUMN);
    QLineEdit  * lineedit_hesap_no_id      = ( QLineEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number , HESAP_NO_ID_COLUMN);
    QLineEdit  * lineedit_pos_kk_id        = ( QLineEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number , POS_KK_ID_COLUMN);
    QLineEdit  * lineedit_kk_sahibi        = ( QLineEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number , KK_SAHIBI_COLUMN );
    QLineEdit  * lineedit_kk_numarasi      = ( QLineEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number , KK_NUMARASI_COLUMN);

    SQL_QUERY query(DB);

    query.PREPARE_SELECT("prk_gelir_gider_turleri","tur_id" ,"gelir_gider_adi = :gelir_gider_adi");
    query.SET_VALUE(":gelir_gider_adi" , combobox_gider_turu->currentText());

    int gider_turu_id = -1;

    if ( query.SELECT() NE 0 ) {
        query.NEXT();
        gider_turu_id = query.VALUE(0).toInt();
    }

    double kdv_orani = QVariant(combobox_kdv_orani->currentText()).toDouble();

    int kdv_hesap_id = 0;

    if ( kdv_orani NE 0 ) {

        for ( int i = 0 ; i < KDV_OTV_ORANLARI_SAYISI ; i++ ) {

            if ( m_gider_fisi_mi EQ 1 ) {
                if ( M_KDV_ORANLARI_ARRAY[i] EQ kdv_orani ) {
                   kdv_hesap_id = M_INDIRILECEK_KDV_HESABI_ID_ARRAY[i];
                   break;
                }
            }
            else {
                if ( M_KDV_ORANLARI_ARRAY[i] EQ kdv_orani ) {
                   kdv_hesap_id = M_HESAPLANAN_KDV_HESABI_ID_ARRAY[i];
                   break;
                }
            }
        }
    }

    int tah_odm_sekli    = E9_GET_TAH_ODM_SEKLI_ENUM(combobox_tah_odm_sekli->currentText());


    PRK_CLEAR_FIS_SATIRI_STRUCT( M_FIS_SATIRI );

    QList <int> kdv_hesap_id_list;

    if (kdv_hesap_id > 0 ) {
        kdv_hesap_id_list << kdv_hesap_id;
    }



    QList <double> kdv_tutari_list;

    if ( commaedit_kdv_tutari->GET_DOUBLE() > 0 ) {

        kdv_tutari_list << commaedit_kdv_tutari->GET_DOUBLE();
    }

    M_FIS_SATIRI->modul_id            = M_FIS->modul_id;
    M_FIS_SATIRI->program_id          = M_FIS->program_id;
    M_FIS_SATIRI->aciklama            = lineedit_aciklama->text();
    M_FIS_SATIRI->belge_no            = lineedit_belge_no->text();
    M_FIS_SATIRI->fis_id              = record_id;
    M_FIS_SATIRI->gelir_gider_turu_id = gider_turu_id;
    M_FIS_SATIRI->kdv_hesap_id        = kdv_hesap_id_list;
    M_FIS_SATIRI->kdv_orani           = kdv_orani;
    M_FIS_SATIRI->hesap_no_id         = lineedit_hesap_no_id->text().toInt();
    M_FIS_SATIRI->kredi_karti_numarasi= lineedit_kk_numarasi->text();
    M_FIS_SATIRI->kredi_karti_sahibi  = lineedit_kk_sahibi->text();
    M_FIS_SATIRI->kdv_dahil_tutar     = commaedit_kdv_dahil_tutar->GET_DOUBLE();
    M_FIS_SATIRI->kdv_haric_tutar     = commaedit_kdv_haric_tutar->GET_DOUBLE();
    M_FIS_SATIRI->kdv_tutari          = kdv_tutari_list;
    M_FIS_SATIRI->tah_odm_sekli       = tah_odm_sekli;

    if ( m_gider_fisi_mi EQ 1 ) {
        M_FIS_SATIRI->kredi_karti_id = lineedit_pos_kk_id->text().toInt();
    }
    else {
        M_FIS_SATIRI->pos_id = lineedit_pos_kk_id->text().toInt();
    }

    QList<int> list;
    int row_id = PRK_FIS_SATIRI_EKLE(M_FIS , M_FIS_SATIRI ,list);

    QTableWidgetItem * item;

    item = new QTableWidgetItem(QVariant(row_id).toString());
    m_ui->tablewidget_fis_satirlari->setItem(row_number , ROW_ID_COLUMN , item );

    item = new QTableWidgetItem(QVariant(record_id).toString());
    m_ui->tablewidget_fis_satirlari->setItem(row_number , FIS_ID_COLUMN , item );

    m_ui->commaEdit_kdv_dahil_toplam->SET_DOUBLE(M_FIS->kdv_dahil_toplam);
    m_ui->commaEdit_kdv_haric_toplam->SET_DOUBLE(M_FIS->kdv_haric_toplam);
    m_ui->commaEdit_kdv_toplam->SET_DOUBLE(M_FIS->kdv_toplam);

    if ( tah_odm_sekli EQ ENUM_NAKIT ) {
        SET_KASA_MEVCUDU();
    }
}

/**************************************************************************************
                   PRK_PERAKENDE_FISI::CHECK_UPDATE_LINE
***************************************************************************************/

int PRK_PERAKENDE_FISI::CHECK_UPDATE_LINE ( int record_id, int row_number )
{

    Q_UNUSED ( record_id );

    QComboBox * combobox_tah_odm_sekli = ( QComboBox * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number , TAHSILAT_ODEME_SEKLI_COLUMN);

    int tah_odm_sekli = E9_GET_TAH_ODM_SEKLI_ENUM(combobox_tah_odm_sekli->currentText());

    int fis_satiri_id = -1;

    QTableWidgetItem * item = m_ui->tablewidget_fis_satirlari->item(row_number ,ROW_ID_COLUMN);

    if ( item NE NULL ) {
        fis_satiri_id = item->text().toInt();
    }

    switch ( tah_odm_sekli ) {
        case ENUM_HAVALE:
        {
            QLineEdit * lineedit_hesap_no_id = ( QLineEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number , HESAP_NO_ID_COLUMN);

            if ( lineedit_hesap_no_id->text().toInt() EQ 0 ) {
                if ( OPEN_E9_EFT_BATCH(-1 , ENUM_HAVALE , fis_satiri_id ,DVZ_GET_TEMEL_PARA_BIRIMI_ID(),
                                       M_ISLEM_DETAYLARI , PERAKENDE_MODULU , this) EQ false ){
                    return ADAK_FAIL;
                }

                lineedit_hesap_no_id->setText(QVariant(M_ISLEM_DETAYLARI->bnk_hesap_no_id).toString());
            }
            break;
        }
        case ENUM_EFT   :
        {
            QLineEdit * lineedit_hesap_no_id = ( QLineEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number , HESAP_NO_ID_COLUMN);

            if ( lineedit_hesap_no_id->text().toInt() EQ 0 ) {
                if ( OPEN_E9_EFT_BATCH(-1 , ENUM_EFT,fis_satiri_id ,DVZ_GET_TEMEL_PARA_BIRIMI_ID(),
                                       M_ISLEM_DETAYLARI , PERAKENDE_MODULU , this) EQ false ){

                    return ADAK_FAIL;
                }

                lineedit_hesap_no_id->setText(QVariant(M_ISLEM_DETAYLARI->bnk_hesap_no_id).toString());
            }

            break;
        }
        case ENUM_KREDI_KARTI:
        {

            QLineEdit * lineedit_pos_kk_id = ( QLineEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number , POS_KK_ID_COLUMN);

            if ( m_gider_fisi_mi EQ 1 ) {
                if ( lineedit_pos_kk_id->text().toInt() EQ 0 ) {
                    if ( OPEN_E9_KK_ODEME_BATCH(fis_satiri_id , M_ISLEM_DETAYLARI ,
                                                PERAKENDE_MODULU,this) EQ false ) {
                        return ADAK_FAIL;
                    }

                    lineedit_pos_kk_id->setText(QVariant( M_ISLEM_DETAYLARI->kredi_karti_id).toString());
                }
            }
            else {
                if ( lineedit_pos_kk_id->text().toInt() EQ 0 ) {
                    if ( OPEN_E9_KK_TAHSILAT_BATCH(fis_satiri_id , M_ISLEM_DETAYLARI , PERAKENDE_MODULU,
                                                   this) EQ false ) {
                        return ADAK_FAIL;
                    }
                    lineedit_pos_kk_id->setText(QVariant(M_ISLEM_DETAYLARI->pos_id).toString());

                    QLineEdit * lineedit_kk_sahibi = ( QLineEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number , KK_SAHIBI_COLUMN);
                    lineedit_kk_sahibi->setText(M_ISLEM_DETAYLARI->kredi_karti_sahibi);

                    QLineEdit * lineedit_kk_numarasi = ( QLineEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number , KK_NUMARASI_COLUMN);
                    lineedit_kk_numarasi->setText(M_ISLEM_DETAYLARI->kredi_karti_numarasi);
                }
            }
            QLineEdit * lineedit_hesap_no_id = ( QLineEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number , HESAP_NO_ID_COLUMN);
            lineedit_hesap_no_id->setText(QVariant(M_ISLEM_DETAYLARI->bnk_hesap_no_id).toString());
            break;
        }
        default:
            break;

    }
    return ADAK_OK;
}

/**************************************************************************************
                   PRK_PERAKENDE_FISI::UPDATE_LINE
***************************************************************************************/

void PRK_PERAKENDE_FISI::UPDATE_LINE ( int record_id, int row_number )
{
    Q_UNUSED(record_id);

    QCommaEdit * commaedit_kdv_haric_tutar = ( QCommaEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number , KDV_HARIC_TUTAR_COLUMN);
    QCommaEdit * commaedit_kdv_tutari      = ( QCommaEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number , KDV_TUTARI_COLUMN);
    QCommaEdit * commaedit_kdv_dahil_tutar = ( QCommaEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number , KDV_DAHIL_TUTAR_COLUMN);
    QComboBox  * combobox_kdv_orani        = ( QComboBox  * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number , KDV_ORANI_COLUMN);
    QLineEdit  * lineedit_belge_no         = ( QLineEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number , BELGE_NO_COLUMN);
    QLineEdit  * lineedit_aciklama         = ( QLineEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number , ACIKLAMA_COLUMN);
    QComboBox  * combobox_gider_turu       = ( QComboBox  * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number , GELIR_GIDER_TURU_COLUMN);
    QComboBox  * combobox_tah_odm_sekli    = ( QComboBox  * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number , TAHSILAT_ODEME_SEKLI_COLUMN);
    QLineEdit  * lineedit_hesap_no_id      = ( QLineEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number , HESAP_NO_ID_COLUMN);
    QLineEdit  * lineedit_pos_kk_id        = ( QLineEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number , POS_KK_ID_COLUMN);
    QLineEdit  * lineedit_kk_sahibi        = ( QLineEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number , KK_SAHIBI_COLUMN);
    QLineEdit  * lineedit_kk_numarasi      = ( QLineEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number , KK_NUMARASI_COLUMN);

    SQL_QUERY query(DB);

    query.PREPARE_SELECT("prk_gelir_gider_turleri ","tur_id",
                         "gelir_gider_adi = :gelir_gider_adi");
    query.SET_VALUE(":gelir_gider_adi" , combobox_gider_turu->currentText());

    int gider_turu_id = -1;

    if ( query.SELECT() NE 0 ) {
        query.NEXT();
        gider_turu_id = query.VALUE(0).toInt();
    }

    double kdv_orani = QVariant(combobox_kdv_orani->currentText()).toDouble();

    int kdv_hesap_id = 0;

    if ( kdv_orani NE 0 ) {

        for ( int i = 0 ; i < KDV_OTV_ORANLARI_SAYISI ; i++ ) {

            if ( M_KDV_ORANLARI_ARRAY[i] EQ kdv_orani ) {
               kdv_hesap_id = M_INDIRILECEK_KDV_HESABI_ID_ARRAY[i];
               break;
            }
        }
    }
    int fis_satiri_id = m_ui->tablewidget_fis_satirlari->item(row_number , ROW_ID_COLUMN)->text().toInt();

    int tah_odm_sekli = E9_GET_TAH_ODM_SEKLI_ENUM(combobox_tah_odm_sekli->currentText());

    PRK_FIS_SATIRI_BILGILERINI_OKU(M_FIS_SATIRI , fis_satiri_id);

    QList<int> kdv_hesap_id_list;

    if ( kdv_hesap_id > 0 ) {
       kdv_hesap_id_list << kdv_hesap_id;
    }


    QList<double> kdv_tutari_list;

    if ( commaedit_kdv_tutari->GET_DOUBLE() > 0 ) {
        kdv_tutari_list << commaedit_kdv_tutari->GET_DOUBLE();
    }


    M_FIS_SATIRI->aciklama             = lineedit_aciklama->text();
    M_FIS_SATIRI->belge_no             = lineedit_belge_no->text();
    M_FIS_SATIRI->gelir_gider_turu_id  = gider_turu_id;
    M_FIS_SATIRI->hesap_no_id          = lineedit_hesap_no_id->text().toInt();
    M_FIS_SATIRI->kdv_dahil_tutar      = commaedit_kdv_dahil_tutar->GET_DOUBLE();
    M_FIS_SATIRI->kdv_haric_tutar      = commaedit_kdv_haric_tutar->GET_DOUBLE();
    M_FIS_SATIRI->kdv_hesap_id         = kdv_hesap_id_list;
    M_FIS_SATIRI->kdv_orani            = kdv_orani;
    M_FIS_SATIRI->kdv_tutari           = kdv_tutari_list;
    M_FIS_SATIRI->kredi_karti_numarasi = lineedit_kk_numarasi->text();
    M_FIS_SATIRI->kredi_karti_sahibi   = lineedit_kk_sahibi->text();

    if ( m_gider_fisi_mi EQ 1 ) {
        M_FIS_SATIRI->kredi_karti_id = lineedit_pos_kk_id->text().toInt();
    }
    else {
        M_FIS_SATIRI->pos_id         = lineedit_pos_kk_id->text().toInt();
    }



    PRK_FIS_SATIRINI_GUNCELLE( M_FIS , M_FIS_SATIRI , fis_satiri_id);

    m_ui->commaEdit_kdv_dahil_toplam->SET_DOUBLE(M_FIS->kdv_dahil_toplam);
    m_ui->commaEdit_kdv_haric_toplam->SET_DOUBLE(M_FIS->kdv_haric_toplam);
    m_ui->commaEdit_kdv_toplam->SET_DOUBLE(M_FIS->kdv_toplam);


    if ( tah_odm_sekli EQ ENUM_NAKIT ) {
        SET_KASA_MEVCUDU();
    }
}
/**************************************************************************************
                   PRK_PERAKENDE_FISI::CHECK_DELETE_LINE()
***************************************************************************************/

int PRK_PERAKENDE_FISI::CHECK_DELETE_LINE(int fis_id, int row_number)
{
    Q_UNUSED(fis_id);
    Q_UNUSED(row_number);

    return ADAK_OK;
}

/**************************************************************************************
                   PRK_PERAKENDE_FISI::DELETE_LINE
***************************************************************************************/
void PRK_PERAKENDE_FISI::DELETE_LINE(int fis_id, int row_number)
{
    Q_UNUSED ( fis_id );

    int fis_satiri_id = m_ui->tablewidget_fis_satirlari->item(row_number , ROW_ID_COLUMN)->text().toInt();

    PRK_FIS_SATIRINI_SIL(M_FIS , fis_satiri_id );

    m_ui->commaEdit_kdv_dahil_toplam->SET_DOUBLE(M_FIS->kdv_dahil_toplam);
    m_ui->commaEdit_kdv_haric_toplam->SET_DOUBLE(M_FIS->kdv_haric_toplam);
    m_ui->commaEdit_kdv_toplam->SET_DOUBLE(M_FIS->kdv_toplam);

    SET_KASA_MEVCUDU();
}


/**************************************************************************************
                   PRK_PERAKENDE_FISI::LOCK_FIS_RECORD
***************************************************************************************/

int PRK_PERAKENDE_FISI::LOCK_FIS_RECORD ( int record_id )
{
    return DB->LOCK_ROW ( "fis_id", "prk_fisler", QString ( "fis_id = %1" ).arg ( record_id ));
}

/**************************************************************************************
                   PRK_PERAKENDE_FISI::UNLOCK_FIS_RECORD
***************************************************************************************/

void PRK_PERAKENDE_FISI::UNLOCK_FIS_RECORD ( int record_id )
{
    DB->UNLOCK_ROW ( "fis_id", "prk_fisler", QString ( "fis_id = %1" ).arg ( record_id ));
}

/**************************************************************************************
                   PRK_PERAKENDE_FISI::SEARCH_EDIT_CLICKED()
***************************************************************************************/

void PRK_PERAKENDE_FISI::SEARCH_EDIT_CLICKED(QWidget *widget, QLineEdit *line_edit)
{
    Q_UNUSED ( widget );
    Q_UNUSED ( line_edit );


    m_sube_id = OPEN_SUBE_ARAMA(m_ui->searchEdit_sube_kodu->GET_TEXT(),this);


    if ( m_sube_id NE -1 ) {
        QString sube_kodu;

        SUBE_GET_SUBE_KODU_ADI( m_sube_id, &sube_kodu );
        m_ui->searchEdit_sube_kodu->SET_TEXT    ( sube_kodu );
    }
}

/**************************************************************************************
                   PRK_PERAKENDE_FISI::SAVER_BUTTON_CLICKED()
***************************************************************************************/

void PRK_PERAKENDE_FISI::SAVER_BUTTON_CLICKED(QAbstractButton *button, int record_id)
{
    Q_UNUSED ( button);
    if ( button EQ m_ui->pushButton_yazdir ) {
        OPEN_REPORT_SHOWER(GET_PRK_FISI_YAZDIR_RAPORU(record_id),nativeParentWidget());
    }
    else if ( button EQ m_ui->pushButton_urettigi_fisler ) {
        E9_URETILEN_FIS_SEC( E9_PROGRAMI, PERAKENDE_MODULU, record_id, this );
    }
}

/**************************************************************************************
                    PRK_PERAKENDE_FISI::CHANGER_BUTTON_CLICKED()
***************************************************************************************/

int PRK_PERAKENDE_FISI::CHANGER_BUTTON_CLICKED(QAbstractButton *button)
{
    if ( button EQ m_ui->pushButton_detaylar ) {

        int record_id = GET_RECORD_ID();

        if ( record_id > 0 ) {
            int muh_fis_id = MUH_GET_ENT_FIS_ID(E9_PROGRAMI , PERAKENDE_MODULU,
                                                record_id);
            M_ENT_DETAYLARI->muh_fis_no        = E9_GET_FIS_NO(MUHASEBE_MODULU , muh_fis_id);

        }
        OPEN_E9_ENT_DETAYLARI_BATCH ( M_ENT_DETAYLARI ,true, this);

        if ( M_ENT_DETAYLARI->muh_kasa_hesap_id NE M_FIS->muh_kasa_hesap_id ) {
            return ADAK_RECORD_CHANGED;
        }
    }
    return ADAK_RECORD_UNCHANGED;
}

/**************************************************************************************
                    PRK_PERAKENDE_FISI::LINE_CHANGER_BUTTON_CLICKED
***************************************************************************************/

int PRK_PERAKENDE_FISI::LINE_CHANGER_BUTTON_CLICKED(QAbstractButton *button, int row_number)
{
    Q_UNUSED(button);

    QComboBox * combobox_tah_odm_sekli = ( QComboBox * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number , TAHSILAT_ODEME_SEKLI_COLUMN);

    int tah_odm_sekli = E9_GET_TAH_ODM_SEKLI_ENUM(combobox_tah_odm_sekli->currentText());

    int fis_satiri_id = -1;

    QTableWidgetItem * item = m_ui->tablewidget_fis_satirlari->item(row_number ,ROW_ID_COLUMN);

    if ( item NE NULL ) {
        fis_satiri_id = item->text().toInt();
    }

    switch ( tah_odm_sekli ) {
        case ENUM_HAVALE:
            if ( OPEN_E9_EFT_BATCH(-1 , ENUM_HAVALE , fis_satiri_id ,DVZ_GET_TEMEL_PARA_BIRIMI_ID(),
                                   M_ISLEM_DETAYLARI , PERAKENDE_MODULU , this) EQ true ){
                QLineEdit * lineedit_hesap_no_id = ( QLineEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number , HESAP_NO_ID_COLUMN);
                lineedit_hesap_no_id->setText(QVariant(M_ISLEM_DETAYLARI->bnk_hesap_no_id).toString());

                return ADAK_RECORD_CHANGED;
            }
            break;

        case ENUM_EFT   :
            if ( OPEN_E9_EFT_BATCH(-1 , ENUM_EFT,fis_satiri_id ,DVZ_GET_TEMEL_PARA_BIRIMI_ID(),
                                   M_ISLEM_DETAYLARI , PERAKENDE_MODULU , this) EQ true ){
                QLineEdit * lineedit_hesap_no_id = ( QLineEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number , HESAP_NO_ID_COLUMN);
                lineedit_hesap_no_id->setText(QVariant(M_ISLEM_DETAYLARI->bnk_hesap_no_id).toString());

                return ADAK_RECORD_CHANGED;
            }
            break;

        case ENUM_KREDI_KARTI:
        {

            if ( m_gider_fisi_mi EQ 1 ) {
                if ( OPEN_E9_KK_ODEME_BATCH(fis_satiri_id , M_ISLEM_DETAYLARI ,
                                            PERAKENDE_MODULU,this) EQ true ) {
                    QLineEdit * lineedit_kredi_karti_id = ( QLineEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number , POS_KK_ID_COLUMN);
                    lineedit_kredi_karti_id->setText(QVariant( M_ISLEM_DETAYLARI->kredi_karti_id).toString());

                    return ADAK_RECORD_CHANGED;
                }
            }
            else {
                if ( OPEN_E9_KK_TAHSILAT_BATCH(fis_satiri_id , M_ISLEM_DETAYLARI,
                                               PERAKENDE_MODULU,this) EQ true ) {
                    QLineEdit * lineedit_pos_id = ( QLineEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number , POS_KK_ID_COLUMN);
                    lineedit_pos_id->setText(QVariant(M_ISLEM_DETAYLARI->pos_id).toString());

                    QLineEdit * lineedit_kk_sahibi = ( QLineEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number , KK_SAHIBI_COLUMN );
                    lineedit_kk_sahibi->setText(M_ISLEM_DETAYLARI->kredi_karti_sahibi);

                    QLineEdit * lineedit_kk_numarasi = ( QLineEdit *) m_ui->tablewidget_fis_satirlari->cellWidget(row_number , KK_NUMARASI_COLUMN);
                    lineedit_kk_numarasi->setText(M_ISLEM_DETAYLARI->kredi_karti_numarasi);

                }
            }
            QLineEdit * lineedit_hesap_no_id = ( QLineEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number , HESAP_NO_ID_COLUMN);
            lineedit_hesap_no_id->setText( QVariant(M_ISLEM_DETAYLARI->bnk_hesap_no_id).toString());
            break;
        }

        default:
            break;

    }

    return ADAK_RECORD_UNCHANGED;
}


/**************************************************************************************
                    PRK_PERAKENDE_FISI::SET_KASA_MEVCUDU
***************************************************************************************/

void PRK_PERAKENDE_FISI::SET_KASA_MEVCUDU()
{
    MUH_HESAP_STRUCT * MUHASEBE_HESABI = new MUH_HESAP_STRUCT;

    MUH_CLEAR_HESAP_STRUCT(MUHASEBE_HESABI);

    MUH_HESAPLAR_TABLOSUNU_OKU(MUHASEBE_HESABI , M_FIS->muh_kasa_hesap_id);



    double borc_array[E9_ARRAY_SIZE];
    double alacak_array[E9_ARRAY_SIZE];

    UNPACK_DOUBLE_ARRAY(MUHASEBE_HESABI->toplam_borc_array   , borc_array , E9_ARRAY_SIZE);
    UNPACK_DOUBLE_ARRAY(MUHASEBE_HESABI->toplam_alacak_array , alacak_array , E9_ARRAY_SIZE);

    double toplam_borc   = borc_array[E9_SON_BAKIYE];
    double toplam_alacak = alacak_array[E9_SON_BAKIYE];


    if ( toplam_borc > toplam_alacak ) {
        m_ui->commaEdit_kasa_mevcudu->SET_DOUBLE(toplam_borc - toplam_alacak);
        m_ui->commaEdit_kasa_mevcudu->setStyleSheet("color:black");
    }
    else {
        m_ui->commaEdit_kasa_mevcudu->SET_DOUBLE(toplam_borc - toplam_alacak);
        m_ui->commaEdit_kasa_mevcudu->setStyleSheet("color:red");
    }
}





