#include "cek_musteri_cek_senet_arama_class.h"
#include "ui_cek_musteri_cek_senet_arama.h"
#include "adak_utils.h"
#include "e9_console_utils.h"
#include "e9_gui_utils.h"
#include "e9_enum.h"
#include "cari_console_utils.h"
#include "cek_enum.h"
#include "e9_log.h"
#include "cari_kart_arama_open.h"
#include "cek_enum.h"
#include "cek_console_utils.h"


extern ADAK_SQL * DB;


/**************************************************************************************
                   OPEN_CEK_MUSTERI_CEK_SENET_ARAMA
***************************************************************************************/

void OPEN_CEK_MUSTERI_CEK_SENET_ARAMA ( int cek_senet_turu,QWidget * parent )
{
    CEK_MUSTERI_CEK_SENET_ARAMA * F = new CEK_MUSTERI_CEK_SENET_ARAMA ( cek_senet_turu,parent );
    F->EXEC( NOT_FULL_SCREEN );
}

/**************************************************************************************
                   CEK_MUSTERI_CEK_SENET_ARAMA::CEK_MUSTERI_CEK_SENET_ARAMA
***************************************************************************************/

CEK_MUSTERI_CEK_SENET_ARAMA::CEK_MUSTERI_CEK_SENET_ARAMA ( int cek_senet_turu,QWidget *parent ) : ARAMA_KERNEL (parent), m_ui(new Ui::CEK_MUSTERI_CEK_SENET_ARAMA)
{
    m_cek_senet_turu = cek_senet_turu;

    m_ui->setupUi     ( this );
    START_ARAMA_KERNEL( this, DB );
}

/**************************************************************************************
                   CEK_MUSTERI_CEK_SENET_ARAMA::SETUP_FORM
***************************************************************************************/

void CEK_MUSTERI_CEK_SENET_ARAMA::SETUP_FORM()
{

    REGISTER_TABLE_WIDGET          ( m_ui->tableWidget );
    REGISTER_ARAMA_BUTTONS_WIDGET  ( m_ui->widget_batch_buttons );

    SET_FIRST_FOCUS_WIDGET         ( m_ui->combobox_cekin_durumu );

    m_ui->adakDate_vade_bas_tarihi->MALI_YIL_KISITI_YOK( true );
    m_ui->adakDate_vade_bts_tarih->MALI_YIL_KISITI_YOK( true );

    m_ui->adakDate_vade_bas_tarihi->SET_DATE( MALI_YIL_FIRST_DATE()  );
    m_ui->adakDate_vade_bts_tarih->SET_DATE( MALI_ARRAY_LAST_DATE() );

    if ( m_cek_senet_turu EQ ENUM_CEK ) {
        m_ui->label_cek_senet_durumu->setText("Çekin Durumu");
        SET_HEADERS(QStringList()<<tr ( "Çek Id" )<<tr ( "Çek No" )<<tr ( "Vade Tarihi")<<tr ( "Düzenleme Tarihi")<<tr("Cari Hesap")<<
                    tr ( "Banka İsmi" )<<tr("Şube İsmi")<<tr("Tutar")<<tr ( "Durumu" ) );
        SET_FIND_BUTTON_NAME ( tr ( "Çekleri Bul" ) );
        SET_PAGE_TITLE ( tr ( "ÇEK-SNT - MÜŞTERİ ÇEKLERİ ARAMA"  ) );
        SET_HELP_PAGE  ( "cek-senet-islemleri_musteri-cekleri-arama" );
    }
    else {
        m_ui->label_cek_senet_durumu->setText("Senedin Durumu");
        SET_HEADERS ( QStringList()<<tr( "Senet Id" )<<tr ( "Senet No" )<<tr ( "Vade Tarihi" )<<tr ( "Düzenleme Tarihi" )<<tr ( "Cari Hesap" )
                                   <<tr( "Tutar" )<< ( "Senedin Durumu" ) );
        SET_FIND_BUTTON_NAME ( tr ( "Senetleri Bul" ) );
        SET_PAGE_TITLE ( tr( "ÇEK-SNT - MÜŞTERİ SENETLERİ ARAMA" ) );
        SET_HELP_PAGE  ( "cek-senet-islemleri_musteri-senetleri-arama" );
    }

    m_ui->date_frame->setEnabled ( false );

    m_ui->cari_hesap_frame->setEnabled ( false );
    m_ui->cekin_durumu_frame->setEnabled ( false );

    m_cari_hesap_id = 0;

    FILL_COMBOBOX_CEKIN_DURUMU();

    SET_ENTER_KEY_FOR_RUN_BATCH ( true );

    if ( m_cek_senet_turu EQ ENUM_CEK ) {
        m_ui->tableWidget->setColumnWidth   ( 1,100 );
        m_ui->tableWidget->setColumnWidth   ( 2,170 );
        m_ui->tableWidget->setColumnWidth   ( 3,170 );
        m_ui->tableWidget->setColumnWidth   ( 4,100 );
        m_ui->tableWidget->setColumnWidth   ( 5,90 );
        m_ui->tableWidget->setColumnWidth   ( 6,100 );
        m_ui->tableWidget->setColumnWidth   ( 7,80 );
        m_ui->tableWidget->setColumnWidth   ( 8,100 );

        SET_SETTING_NAME                    ( "CEK_MUS_CEK_SENET_ARAMA_CEK" );
    }
    else {
        m_ui->tableWidget->setColumnWidth   ( 1,100 );
        m_ui->tableWidget->setColumnWidth   ( 2,190 );
        m_ui->tableWidget->setColumnWidth   ( 3,190 );
        m_ui->tableWidget->setColumnWidth   ( 4,140 );
        m_ui->tableWidget->setColumnWidth   ( 5,110 );
        m_ui->tableWidget->setColumnWidth   ( 6,180 );

        SET_SETTING_NAME                    ( "CEK_MUS_CEK_SENET_ARAMA_SENET" );
    }

    m_ui->lineedit_cari_hesap_ismi->setMaxLength(60);

    m_ui->tableWidget->hideColumn(0);

    m_ui->checkbox_cekin_durumu->setChecked( true );
    m_ui->cekin_durumu_frame->setEnabled( true );
}

/**************************************************************************************
                   CEK_MUSTERI_CEK_SENET_ARAMA::CHECK_VAR
***************************************************************************************/

int CEK_MUSTERI_CEK_SENET_ARAMA::CHECK_VAR ( QObject * object )
{

    if ( object EQ m_ui->searchEdit_cari_hesap_kodu ) {

        QString cari_hesap_ismi;
        int     cari_hesap_id;

        int result = CARI_GET_HESAP_KODU_ISMI_PARA_BIRIM_ID( m_ui->searchEdit_cari_hesap_kodu->GET_TEXT(), &cari_hesap_id, &cari_hesap_ismi );
        if ( result EQ 0 ) {
            MSG_WARNING( tr ( "Aradığınız cari hesap kodu bulunamadı!.." ), m_ui->searchEdit_cari_hesap_kodu );
            return ADAK_FAIL;
        }

        m_ui->lineedit_cari_hesap_ismi->setText ( cari_hesap_ismi );
        m_cari_hesap_id = cari_hesap_id;

    }
    else if ( object EQ m_ui->checkbox_vade_tarihi ) {

        if ( m_ui->checkbox_vade_tarihi->isChecked() EQ true ) {
            m_ui->date_frame->setEnabled ( true );
            m_ui->adakDate_vade_bas_tarihi->SET_DATE( MALI_YIL_FIRST_DATE() );
            m_ui->adakDate_vade_bts_tarih->SET_DATE( MALI_ARRAY_LAST_DATE() );
            SET_FOCUS( m_ui->adakDate_vade_bas_tarihi );
        }
        else {
            m_ui->date_frame->setEnabled ( false );
        }
    }
    else if ( object EQ m_ui->checkbox_cari_hesap ) {

        if ( m_ui->checkbox_cari_hesap->isChecked() EQ true ) {
            m_ui->cari_hesap_frame->setEnabled ( true );
            SET_FOCUS( m_ui->searchEdit_cari_hesap_kodu );
        }
        else {
            m_ui->cari_hesap_frame->setEnabled ( false );
        }
    }
    else if ( object EQ m_ui->checkbox_cekin_durumu ) {

        if ( m_ui->checkbox_cekin_durumu->isChecked() EQ true ) {
             m_ui->cekin_durumu_frame->setEnabled ( true );
             m_ui->combobox_cekin_durumu->setCurrentIndex(-1);
             SET_FOCUS( m_ui->combobox_cekin_durumu );
        }
        else {
            m_ui->cekin_durumu_frame->setEnabled ( false );
            m_ui->combobox_cekin_durumu->setCurrentIndex(-1);
        }
    }

    return ADAK_OK;
}

/**************************************************************************************
                   CEK_MUSTERI_CEK_SENET_ARAMA::CHECK_RUN
***************************************************************************************/

int CEK_MUSTERI_CEK_SENET_ARAMA::CHECK_RUN()
{
    if ( m_ui->checkbox_cekin_durumu->isChecked() EQ true ) {
        if ( m_ui->combobox_cekin_durumu->currentIndex() EQ -1 ) {
            MSG_WARNING( tr ( "Aranacak çekin durumunu seçmelisiniz1.." ), m_ui->combobox_cekin_durumu );

            return ADAK_FAIL;
        }
    }
    if ( m_ui->checkbox_cari_hesap->isChecked() EQ true ) {
        if ( m_ui->searchEdit_cari_hesap_kodu->GET_TEXT().isEmpty() EQ true ) {
             MSG_WARNING( tr ( "Cari hesap kodunu girmelisiniz!.." ), m_ui->searchEdit_cari_hesap_kodu );

             return ADAK_FAIL;
        }
    }
    return ADAK_OK;
}

/**************************************************************************************
                   CEK_MUSTERI_CEK_SENET_ARAMA::SEARCH
***************************************************************************************/

void CEK_MUSTERI_CEK_SENET_ARAMA::SEARCH()
{
    QString log_detaylari;

    if ( m_cek_senet_turu EQ ENUM_CEK ) {
        log_detaylari = QObject::tr ( "Müşteri Çekleri Arama " );
    }
    else {
        log_detaylari = QObject::tr ( "Müşteri Senetleri Arama ");
    }
    if(m_ui->checkbox_cekin_durumu->isChecked() EQ true ) {
        log_detaylari.append ( " , " + m_ui->combobox_cekin_durumu->currentText());
    }
    if(m_ui->checkbox_cari_hesap->isChecked() EQ true ) {
        log_detaylari.append ( " , " + m_ui->searchEdit_cari_hesap_kodu->GET_TEXT() + " " + m_ui->lineedit_cari_hesap_ismi->text());
    }
    if(m_ui->checkbox_vade_tarihi->isChecked() EQ true ) {
        log_detaylari.append ( "\nVade Tarihi Aralığı : " + m_ui->adakDate_vade_bas_tarihi->QDATE().toString("dd MMMM YYYY")
                              + " - " + m_ui->adakDate_vade_bts_tarih->QDATE().toString("dd MMMM yyyy"));
    }


    DB->START_TRANSACTION();
    E9_LOG_KAYDI_EKLE (CEKSENET_MODULU , LOG_CEK_RAPORLAR , LOG_ISLEM_RAPOR_ALINDI , log_detaylari);
    DB->COMMIT_TRANSACTION();

    QString table_names = "";

    QString vade_bas_tarihi = m_ui->adakDate_vade_bas_tarihi->DATE();
    QString vade_bts_tarihi = m_ui->adakDate_vade_bts_tarih->DATE();

    if ( m_cek_senet_turu EQ ENUM_CEK ) {
        table_names = QString ( "cek_senet_id,cek_senet_numarasi,vade_tarihi,keside_tarihi,cek_senet_tutari,"
                                 "cek_senet_son_durumu,banka_ismi,sube_ismi");
    }
    else {
        table_names = QString ( "cek_senet_id,cek_senet_numarasi,vade_tarihi,keside_tarihi,cek_senet_tutari,"
                                 "cek_senet_son_durumu " );
    }

    SQL_QUERY f_query      ( DB );
    SQL_QUERY s_query      ( DB );

    f_query.PREPARE_SELECT (  "cek_cekler_senetler", table_names, "cek_senet_turu=:cek_senet_turu" );

    if  ( m_ui->checkbox_vade_tarihi->isChecked() EQ true ) {
        f_query.AND_EKLE ("vade_tarihi BETWEEN :baslangic_tarihi AND :bitis_tarihi" );
        f_query.SET_VALUE(":baslangic_tarihi",vade_bas_tarihi );
        f_query.SET_VALUE(":bitis_tarihi"    ,vade_bts_tarihi );
    }


    if ( m_ui->checkbox_cekin_durumu->isChecked() EQ true ) {
        f_query.AND_EKLE ("cek_senet_son_durumu = :cek_senet_son_durumu");
        f_query.SET_VALUE(":cek_senet_son_durumu",CEK_GET_CEK_SENET_POZISYONU_ENUM ( m_ui->combobox_cekin_durumu->currentText()));
    }

    f_query.SET_VALUE(":cek_senet_turu", m_cek_senet_turu );

    f_query.AND_EKLE ("musteri_ceki_mi=:musteri_ceki_mi");
    f_query.SET_VALUE(":musteri_ceki_mi", 1 );

    if ( f_query.SELECT() EQ 0 ) {
        return;
    }

    int current_row = 0;

    s_query.PREPARE_SELECT("cek_bordrolar , cek_bordro_satirlari","cari_hesap_id",
                         "bordro_islem_turu = :bordro_islem_turu AND cek_senet_id = :cek_senet_id "
                         "AND cek_bordrolar.bordro_id = cek_bordro_satirlari.bordro_id AND "
                         "cek_bordrolar.modul_id != :modul_id");

    while ( f_query.NEXT() EQ true ) {
        int cek_senet_id = f_query.VALUE(0).toInt();

        s_query.SET_VALUE(":bordro_islem_turu" , ENUM_CEK_MUSTERIDEN_ALINDI);
        s_query.SET_VALUE(":cek_senet_id"      , cek_senet_id );
        s_query.SET_VALUE(":modul_id"          , CEKSENET_MODULU);

        int cari_hesap_id = 0;

        if ( s_query.SELECT() NE 0 )  {
            s_query.NEXT();
            cari_hesap_id = s_query.VALUE(0).toInt();
        }
        else {
            SQL_QUERY t_query      ( DB );

            t_query.PREPARE_SELECT("cek_bordrolar,cek_bordro_satirlari ",
                                 "alinan_cari_hesap_id ",
                                 "bordro_islem_turu = :bordro_islem_turu AND cek_senet_id = :cek_senet_id "
                                 "AND cek_bordrolar.bordro_id = cek_bordro_satirlari.bordro_id AND "
                                 "bordro_turu = :bordro_turu ");
            t_query.SET_VALUE(":bordro_islem_turu" , ENUM_CEK_MUSTERIDEN_ALINDI);
            t_query.SET_VALUE(":cek_senet_id"      , cek_senet_id);
            t_query.SET_VALUE(":bordro_turu"       , ENUM_CEK_SENET_ACILIS_BORDROSU);


            if ( t_query.SELECT() EQ 0 ) {
                continue;
            }
            t_query.NEXT();
            cari_hesap_id = t_query.VALUE(0).toInt();
        }
        if ( m_ui->checkbox_cari_hesap->isChecked() EQ true  ) {
            int aranacak_cari_hesap_id = m_cari_hesap_id;
            if ( cari_hesap_id NE aranacak_cari_hesap_id ) {
                continue;
            }
        }

        QString cari_hesap_kodu, cari_hesap_ismi;

        int result = CARI_GET_HESAP_KODU_ISMI_PARA_BIRIM_ID( cari_hesap_id, &cari_hesap_kodu, &cari_hesap_ismi );
        if ( result EQ 0 )  {
            continue;
        }

        QString cari_hesap = cari_hesap_kodu+ "  " + cari_hesap_ismi;
        current_row = ADD_NEW_ROW();

        if ( m_cek_senet_turu EQ ENUM_CEK ) {
            m_ui->tableWidget->item ( current_row,0 )->setText(f_query.VALUE(0).toString());
            m_ui->tableWidget->item ( current_row,1 )->setText(f_query.VALUE(1).toString());
            m_ui->tableWidget->item ( current_row,2 )->setText(f_query.VALUE(2).toDate().toString("dd MMMM yyyy dddd"));
            m_ui->tableWidget->item ( current_row,3 )->setText(f_query.VALUE(3).toDate().toString ( "dd MMMM yyyy dddd"));
            m_ui->tableWidget->item ( current_row,4 )->setText(cari_hesap );
            m_ui->tableWidget->item ( current_row,5 )->setText(f_query.VALUE(6).toString() );
            m_ui->tableWidget->item ( current_row,6 )->setText(f_query.VALUE(7).toString() );
            m_ui->tableWidget->item ( current_row,7 )->setText(VIRGUL_EKLE(f_query.VALUE(4).toString() ) );

        }
        else {
            m_ui->tableWidget->item ( current_row,0 )->setText(f_query.VALUE(0).toString());
            m_ui->tableWidget->item ( current_row,1 )->setText(f_query.VALUE(1).toString());
            m_ui->tableWidget->item ( current_row,2 )->setText(f_query.VALUE(2).toDate().toString("dd MMMM yyyy dddd"));
            m_ui->tableWidget->item ( current_row,3 )->setText(f_query.VALUE(3).toDate().toString("dd MMMM yyyy dddd"));
            m_ui->tableWidget->item ( current_row,4 )->setText(cari_hesap );
            m_ui->tableWidget->item ( current_row,5 )->setText(VIRGUL_EKLE ( f_query.VALUE(4).toString() ) );
        }

        QString cekin_durumu = CEK_GET_CEK_SENET_POZISYONU_STRING ( f_query.VALUE(5).toInt() );

        if ( m_cek_senet_turu EQ ENUM_CEK ) {
            m_ui->tableWidget->item ( current_row,8 )->setText ( cekin_durumu );
        }
        else {
            m_ui->tableWidget->item ( current_row,6 )->setText ( cekin_durumu );
        }
    }

}

/**************************************************************************************
                   CEK_MUSTERI_CEK_SENET_ARAMA::SINGLE_LINE_SELECTED
***************************************************************************************/

int CEK_MUSTERI_CEK_SENET_ARAMA::SINGLE_LINE_SELECTED ( int selected_row_number )
{
    Q_UNUSED ( selected_row_number );
    return ADAK_CONTINUE;
}

/**************************************************************************************
                   CEK_MUSTERI_CEK_SENET_ARAMA::FILL_COMBOBOX_CEKIN_DURUMU
***************************************************************************************/

void CEK_MUSTERI_CEK_SENET_ARAMA::FILL_COMBOBOX_CEKIN_DURUMU()
{
    QStringList cek_status;

    cek_status << tr ( "Portföyde" );
    cek_status << tr ( "Bankaya Teminata Verildi" );
    cek_status << tr ( "Bankaya Tahsilata Verildi" );
    cek_status << tr ( "Tahsil Edildi-Nakit" );
    cek_status << tr ( "Tahsil Edildi-Hesaptan" );
    cek_status << tr ( "Satıcıya Verildi" );
    cek_status << tr ( "Müşteriye İade Edildi" );
    cek_status << tr ( "Karşılıksız / Protestolu" );
    cek_status << tr ( "Şüpheli Alacaklar" );
    cek_status << tr ( "Tahsil Edilemeyen" );

    m_ui->combobox_cekin_durumu->addItems ( cek_status );
    m_ui->combobox_cekin_durumu->setCurrentIndex( 0 );
}

/**************************************************************************************
                   CEK_MUSTERI_CEK_SENET_ARAMA::SEARCH_EDIT_CLICKED
***************************************************************************************/

void CEK_MUSTERI_CEK_SENET_ARAMA::SEARCH_EDIT_CLICKED ( QWidget * widget, QLineEdit * lineEdit )
{
    Q_UNUSED ( widget );

    int cari_hesap_id = OPEN_CARI_KART_ARAMA ( "", SAHIS_ARTI_FIRMA,this, 1 , E9_PROGRAMI, CARI_MODULU  );
    if ( cari_hesap_id NE -1  ) {
        lineEdit->setText ( CARI_FIND_HESAP_KODU ( cari_hesap_id ) );
    }
}
