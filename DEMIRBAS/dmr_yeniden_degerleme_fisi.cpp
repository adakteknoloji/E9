#include "ui_dmr_yeniden_degerleme_fisi.h"
#include "dmr_yeniden_degerleme_fisi_class.h"
#include "e9_console_utils.h"
#include "e9_gui_utils.h"
#include "dmr_con_utils.h"
#include <QDate>
#include "e9_log.h"
#include "dmr_fis_utils.h"
#include "dmr_fis_arama_open.h"
#include "dmr_demirbas_karti_arama_open.h"
#include "dmr_con_utils.h"

extern ADAK_SQL *              DB;


#define ROW_ID_COLUMN                           0
#define FIS_ID_COLUMN                           1
#define ORDER_COLUMN                            2
#define DEMIRBAS_ID_COLUMN                      3
#define DMR_KODU_COLUMN                         4
#define DMR_ISMI_COLUMN                         5
#define ACIKLAMA_COLUMN                         6
#define DEMIRBAS_DEGERI_COLUMN                  7
#define BIRIKMIS_AMORTISMAN_TUTARI_COLUMN       8
#define YENI_DEMIRBAS_DEGERI_COLUMN             9
#define YENI_BIRIKMIS_AMORTISMAN_TUTARI_COLUMN  10

/**************************************************************************************
                   F_DMR_YENIDEN_DEGERLEME_FISI
***************************************************************************************/

void OPEN_DMR_YENIDEN_DEGERLEME_FISI ( int record_id, QWidget * parent )
{
    DMR_YENIDEN_DEGERLEME_FISI * F = new DMR_YENIDEN_DEGERLEME_FISI ( record_id,parent );
    F->SHOW( FULL_SCREEN );
}

/**************************************************************************************
                   DMR_YENIDEN_DEGERLEME_FISI::DMR_YENIDEN_DEGERLEME_FISI
***************************************************************************************/

DMR_YENIDEN_DEGERLEME_FISI::DMR_YENIDEN_DEGERLEME_FISI(int record_id, QWidget * parent) :FIS_KERNEL(parent),
m_ui ( new Ui::DMR_YENIDEN_DEGERLEME_FISI )
{

    m_start_record_id = record_id;

    m_ui->setupUi    ( this );
    START_FIS_KERNEL ( this, DB );
 }

/**************************************************************************************
                   DMR_YENIDEN_DEGERLEME_FISI::SETUP_FORM();
***************************************************************************************/

void DMR_YENIDEN_DEGERLEME_FISI::SETUP_FORM()
{
    SET_HELP_PAGE  ( "sabit-kiymetler-demirbaslar_yeniden-degerleme-fisi" );
    SET_PAGE_TITLE ( tr("DMR - YENİDEN DEĞERLENDİRME FİŞİ") );

    REGISTER_BUTTONS_WIDGET           ( m_ui->navigation_button_widget );

    M_FIS        = new DMR_FIS_STRUCT;
    M_FIS_SATIRI = new DMR_FIS_SATIRI_STRUCT;

    QRegExp rx ( "(\\*?)(\\d+)" );
    QRegExpValidator * validator = new QRegExpValidator ( rx, this );
    m_ui->lineedit_fis_no->setValidator ( validator );

    m_ui->lineedit_fis_no->setMaxLength(10);

     

    m_ui->lineedit_fis_no->setText("*");

    DISABLE_CLEAR_ON_WIDGET(m_ui->lineedit_fis_no);

    REGISTER_TABLE_WIDGET             ( m_ui->tablewidget_fis_satirlari, 11 );

    SET_FIS_ORDER_COLUMN_NUMBER       ( ORDER_COLUMN );
    SET_FORM_ID_COLUMN_POSITION       ( FIS_ID_COLUMN );
    SET_ROW_ID_COLUMN_POSITION        ( ROW_ID_COLUMN );

    SET_NUMBER_OF_HIDDEN_COLUMN       ( 4 );

    SET_FIS_ORDER_DATABASE ( "dmr_fis_satirlari", "order_number", "fis_satiri_id" );


    SET_TABLE_ROW_WIDGETS ( DMR_KODU_COLUMN                        , WIDGET_SEARCH_EDIT );
    SET_TABLE_ROW_WIDGETS ( DMR_ISMI_COLUMN                        , WIDGET_LINE_EDIT   );
    SET_TABLE_ROW_WIDGETS ( DEMIRBAS_DEGERI_COLUMN                 , WIDGET_COMMA_EDIT  );
    SET_TABLE_ROW_WIDGETS ( BIRIKMIS_AMORTISMAN_TUTARI_COLUMN      , WIDGET_COMMA_EDIT  );
    SET_TABLE_ROW_WIDGETS ( YENI_DEMIRBAS_DEGERI_COLUMN            , WIDGET_COMMA_EDIT  );
    SET_TABLE_ROW_WIDGETS ( YENI_BIRIKMIS_AMORTISMAN_TUTARI_COLUMN , WIDGET_COMMA_EDIT  );
    SET_TABLE_ROW_WIDGETS ( ACIKLAMA_COLUMN                        , WIDGET_LINE_EDIT   );
    SET_TABLE_ROW_WIDGETS ( DEMIRBAS_ID_COLUMN                     , WIDGET_LINE_EDIT   );


    SET_FIRST_FOCUS_WIDGET ( m_ui->lineedit_fis_no );

    m_ui->tablewidget_fis_satirlari->setHorizontalHeaderLabels ( QStringList()<<"row_id"<<"fis_id"<<"order_column"<< "demirbas id" <<tr("Sabit Kıymet Kodu")
                                                                              << tr ( "Sabit Kıymet Adı") << tr("Açıklama")<< tr("Sabit Kıymet\nDeğeri")
                                                                              << tr ( "Birikmiş Amortisman" )<< tr("Yeni Sabit\nKıymet Değeri")
                                                                              << tr ( "Yeni Birikmiş \nAmortisman" )
                                                                              << tr ( "" ) << tr("") );

    m_ui->tablewidget_fis_satirlari->setColumnWidth ( DMR_KODU_COLUMN                        , 131 );
    m_ui->tablewidget_fis_satirlari->setColumnWidth ( DMR_ISMI_COLUMN                        , 189 );
    m_ui->tablewidget_fis_satirlari->setColumnWidth ( ACIKLAMA_COLUMN                        , 280 );
    m_ui->tablewidget_fis_satirlari->setColumnWidth ( DEMIRBAS_DEGERI_COLUMN                 , 124 );
    m_ui->tablewidget_fis_satirlari->setColumnWidth ( BIRIKMIS_AMORTISMAN_TUTARI_COLUMN      , 142 );
    m_ui->tablewidget_fis_satirlari->setColumnWidth ( YENI_DEMIRBAS_DEGERI_COLUMN            , 148 );
    m_ui->tablewidget_fis_satirlari->setColumnWidth ( YENI_BIRIKMIS_AMORTISMAN_TUTARI_COLUMN , 160 );

    SET_SETTING_NAME                                ( "DMR_YENIDEN_DEGERLEME_FISI" );

    m_ui->tablewidget_fis_satirlari->setSelectionMode ( QAbstractItemView::NoSelection );
    m_ui->tablewidget_fis_satirlari->setFocus (  Qt::OtherFocusReason  );

    m_ui->limitedTextEdit_aciklama->SET_MAX_STRING_LENGTH(512);

    FOCUS_FIRST_WIDGET();

    AUTOSTART_FIS_KERNEL ( m_start_record_id );
}

/**************************************************************************************
                   DMR_YENIDEN_DEGERLEME_FISI::CLEAR_FORM_MEMBERS();
***************************************************************************************/

void DMR_YENIDEN_DEGERLEME_FISI::CLEAR_FORM_MEMBERS()
{
    DMR_CLEAR_FIS_STRUCT       (M_FIS);
    DMR_CLEAR_FIS_SATIRI_STRUCT(M_FIS_SATIRI);

    M_FIS->program_id = E9_PROGRAMI;
    M_FIS->modul_id   = DEMIRBAS_MODULU;

    m_log_kaydi_eklenecek_mi = false;
    m_kayit_eklendi          = false;
    m_kayit_silindi          = false;

}

/**************************************************************************************
                   DMR_YENIDEN_DEGERLEME_FISI::NEW_FIS_RECORD
***************************************************************************************/

void DMR_YENIDEN_DEGERLEME_FISI::NEW_FIS_RECORD()
{
    m_ui->lineedit_fis_no->setText("*");
}

/**************************************************************************************
                   DMR_YENIDEN_DEGERLEME_FISI::GET_FIS_RECORD
***************************************************************************************/

int DMR_YENIDEN_DEGERLEME_FISI::GET_FIS_RECORD ( int record_id )
{
    DMR_FIS_BILGILERINI_OKU(M_FIS , record_id);


    m_ui->lineedit_fis_no->setText ( QVariant(M_FIS->fis_no).toString() );
    m_ui->adakDate_fis_tarihi->SET_DATE(M_FIS->fis_tarihi);
    m_ui->limitedTextEdit_aciklama->setPlainText   ( M_FIS->aciklama);
    m_ui->commaedit_top_yeni_demirbas_degeri->SET_DOUBLE ( M_FIS->toplam_tutar );
    m_ui->commaEdit_degerleme_orani->SET_DOUBLE ( M_FIS->degerleme_orani);
    m_ui->commaEdit_top_yeni_bir_amor_degeri->SET_DOUBLE ( M_FIS->top_birikmis_amor_tutari );

    SQL_QUERY select_query(DB);

    select_query.PREPARE_SELECT ( "dmr_fis_satirlari","fis_satiri_id,demirbas_id,aciklama,birikmis_amortisman_tutari ",
                                  "fis_id = :fis_id" );
    select_query.SET_VALUE      ( ":fis_id" , record_id );

    if ( select_query.SELECT() EQ 0 ) {
        return ADAK_OK;
    }

    QTableWidgetItem * new_item;
    int current_row;

    while (select_query.NEXT()) {

        current_row = ADD_NEW_LINE();

        new_item = new QTableWidgetItem  ( select_query.VALUE(0).toString() );
        m_ui->tablewidget_fis_satirlari->setItem ( current_row, ROW_ID_COLUMN, new_item );

        new_item = new QTableWidgetItem ( QVariant ( record_id ).toString() );
        m_ui->tablewidget_fis_satirlari->setItem ( current_row,FIS_ID_COLUMN,new_item );

        int demirbas_id = select_query.VALUE(1).toInt();


        QLineEdit   * lineedit_aciklama                                 = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget ( current_row, ACIKLAMA_COLUMN                        );
        QCommaEdit  * commaedit_degerleme_oncesi_demirbas_degeri        = ( QCommaEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( current_row, DEMIRBAS_DEGERI_COLUMN                 );
        QCommaEdit  * commaedit_degerleme_sonrasi_demirbas_degeri       = ( QCommaEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( current_row, YENI_DEMIRBAS_DEGERI_COLUMN            );
        QSearchEdit * searchedit_demirbas_kodu                          = ( QSearchEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget ( current_row, DMR_KODU_COLUMN                        );
        QLineEdit   * lineedit_demirbas_adi                             = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget ( current_row, DMR_ISMI_COLUMN                        );
        QCommaEdit  * commaedit_degerleme_oncesi_birikmis_amortisman    = ( QCommaEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( current_row, BIRIKMIS_AMORTISMAN_TUTARI_COLUMN      );
        QCommaEdit  * commaedit_degerleme_sonrasi_birikmis_amortisman   = ( QCommaEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( current_row, YENI_BIRIKMIS_AMORTISMAN_TUTARI_COLUMN );
        QLineEdit   * lineedit_demirbas_id                              = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget ( current_row, DEMIRBAS_ID_COLUMN );

        lineedit_demirbas_id->setText(QVariant(demirbas_id).toString());
        lineedit_aciklama->setText ( select_query.VALUE(2).toString() );

        commaedit_degerleme_oncesi_birikmis_amortisman->SET_DOUBLE ( select_query.VALUE(3).toDouble() );

        SQL_QUERY query ( DB );

        query.PREPARE_SELECT ( "dmr_demirbaslar","demirbas_kodu,demirbas_adi,demirbasin_degeri,demirbasin_yeni_degeri, "
                               "yeni_brkms_amortisman_tutari ",
                               "demirbas_id = :demirbas_id" );
        query.SET_VALUE      ( ":demirbas_id" , demirbas_id );

        if ( query.SELECT() NE 0 ) {
             query.NEXT();
            searchedit_demirbas_kodu->SET_TEXT ( query.VALUE(0).toString() );
            lineedit_demirbas_adi->setText(query.VALUE(1).toString() );
            commaedit_degerleme_oncesi_demirbas_degeri->SET_DOUBLE ( query.VALUE(2).toDouble() );
            commaedit_degerleme_sonrasi_demirbas_degeri->SET_DOUBLE ( query.VALUE(3).toDouble() );
            commaedit_degerleme_sonrasi_birikmis_amortisman->SET_DOUBLE ( query.VALUE(4).toDouble() );
        }
    }
    return ADAK_OK;
}

/**************************************************************************************
                   DMR_YENIDEN_DEGERLEME_FISI::SET_LINE_DEFAULTS
***************************************************************************************/

void DMR_YENIDEN_DEGERLEME_FISI::SET_LINE_DEFAULTS ( int row_number )
{
    QLineEdit  * lineedit_demirbas_ismi               = ( QLineEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, DMR_ISMI_COLUMN                        );
    QCommaEdit * commaedit_demirbas_degeri            = ( QCommaEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, DEMIRBAS_DEGERI_COLUMN                 );
    QCommaEdit * commaedit_birikmis_amortisman_degeri = ( QCommaEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, BIRIKMIS_AMORTISMAN_TUTARI_COLUMN      );
    QCommaEdit * yeni_birikmis_amortisman_degeri      = ( QCommaEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, YENI_BIRIKMIS_AMORTISMAN_TUTARI_COLUMN );
    QLineEdit  * lineedit_aciklama                    = ( QLineEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, ACIKLAMA_COLUMN);

    lineedit_aciklama->setMaxLength(128);
    commaedit_demirbas_degeri->setReadOnly ( true );
    lineedit_demirbas_ismi->setReadOnly ( true );
    commaedit_birikmis_amortisman_degeri->setReadOnly ( true );
    yeni_birikmis_amortisman_degeri->setReadOnly ( true );

    lineedit_demirbas_ismi->setMaxLength(128);

}

/**************************************************************************************
                   DMR_YENIDEN_DEGERLEME_FISI::CHECK_FIS_FORM_VAR
***************************************************************************************/

int DMR_YENIDEN_DEGERLEME_FISI::CHECK_FIS_FORM_VAR ( QObject * object )
{
     if ( object EQ m_ui->commaEdit_degerleme_orani ) {

        if ( m_ui->commaEdit_degerleme_orani->GET_DOUBLE() > 100 ) {
            MSG_WARNING( tr ( "Değerleme Oranı 100'den büyük olamaz!.." ), m_ui->commaEdit_degerleme_orani );

            return ADAK_FAIL;
        }
        SQL_QUERY select_query ( DB );
        select_query.PREPARE_SELECT ( "dmr_fis_satirlari,dmr_fisler","fis_satiri_id ",
                                      "dmr_fis_satirlari.fis_id = dmr_fisler.fis_id AND fis_no = :fis_no");
        select_query.SET_VALUE      ( ":fis_no" , m_ui->lineedit_fis_no->text().toInt() );

        if ( select_query.SELECT() NE 0 ) {
            MSG_WARNING( tr ( "Değerleme oranını değiştiremezsiniz!..Fişi silip,tekrardan oluşturabilirsiniz!.."), m_ui->commaEdit_degerleme_orani );

            return ADAK_FAIL;
        }
    }

    return ADAK_OK;    
}

/**************************************************************************************
                   DMR_YENIDEN_DEGERLEME_FISI::CHECK_FIS_FORM_EMPTY
***************************************************************************************/

int DMR_YENIDEN_DEGERLEME_FISI::CHECK_FIS_FORM_EMPTY()
{
    if ( m_ui->lineedit_fis_no->text().isEmpty() EQ true ) {
        MSG_WARNING( tr ( "Yeni Fiş Kaydı Yapabilmek için Fiş No değeri olarak (*) girmelisiniz!.."), m_ui->lineedit_fis_no );

        return ADAK_FAIL;
    }
    if ( m_ui->commaEdit_degerleme_orani->GET_DOUBLE() EQ 0.0 ) {
        MSG_WARNING( tr ( "Değerleme Oranını Boş Bırakamazsınız!.. " ), m_ui->commaEdit_degerleme_orani );

        return ADAK_FAIL;
    }

    return ADAK_OK;
}

/**************************************************************************************
                   DMR_YENIDEN_DEGERLEME_FISI::CHECK_FIS_RECORD_ADD
***************************************************************************************/

int DMR_YENIDEN_DEGERLEME_FISI::CHECK_FIS_RECORD_ADD()
{
    if ( QString ( m_ui->lineedit_fis_no->text().at(0) ) NE "*" ) {
        if ( DMR_FIS_NO_KULLANILDI_MI ( m_ui->lineedit_fis_no->text().toInt(), m_ui->adakDate_fis_tarihi->DATE()) EQ true ) {
            MSG_WARNING( tr ( "Aynı Fiş Numarası daha önce kullanılmış.Lütfen fiş numarasını değiştiriniz!.."), m_ui->lineedit_fis_no );

            return ADAK_FAIL;
        }
    }
    return ADAK_OK;
}

/**************************************************************************************
                   DMR_YENIDEN_DEGERLEME_FISI::ADD_FIS_RECORD
***************************************************************************************/

int DMR_YENIDEN_DEGERLEME_FISI::ADD_FIS_RECORD()
{
    int fis_no = 0;

    if ( QString ( m_ui->lineedit_fis_no->text().at(0) ) EQ "*" ) {
        fis_no = DMR_SIRADAKI_FIS_NO_AL ( m_ui->adakDate_fis_tarihi->DATE());
    }
    else {
        fis_no = m_ui->lineedit_fis_no->text().toInt();
    }

    M_FIS->aciklama        = m_ui->limitedTextEdit_aciklama->toPlainText();
    M_FIS->fis_no          = fis_no;
    M_FIS->degerleme_orani = m_ui->commaEdit_degerleme_orani->GET_DOUBLE();
    M_FIS->fis_tarihi      = m_ui->adakDate_fis_tarihi->DATE();
    M_FIS->fis_turu        = YENIDEN_DEGERLEME_FISI;

    int fis_id = DMR_FIS_EKLE(M_FIS );

    m_ui->lineedit_fis_no->setText ( QVariant(M_FIS->fis_no).toString());

    m_log_kaydi_eklenecek_mi = true;
    m_kayit_eklendi          = true;

    m_log_detaylari = QObject::tr ( "Yeniden Değerleme Fişi , Yeniden Değerleme Oranı : % " ) +
                      m_ui->commaEdit_degerleme_orani->GET_TEXT();

    m_log_detaylari.append ( "\nFiş Tarihi : " + m_ui->adakDate_fis_tarihi->QDATE().toString("dd MMMM YYYY")
                           + ",Fiş No : " + QVariant ( fis_no ).toString());
    return fis_id;
}

/**************************************************************************************
                   DMR_YENIDEN_DEGERLEME_FISI::CHECK_FIS_RECORD_UPDATE
***************************************************************************************/

int DMR_YENIDEN_DEGERLEME_FISI::CHECK_FIS_RECORD_UPDATE ( int record_id)
{
    if ( QString ( m_ui->lineedit_fis_no->text().at(0)) EQ "*" ) {
        MSG_WARNING( tr ( "Fiş numarasını güncellemek için fiş numarası kısmına sadece sayı girebilirsiniz."), m_ui->lineedit_fis_no );

        return ADAK_FAIL;
    }

    int fis_no = m_ui->lineedit_fis_no->text().toInt();

    SQL_QUERY select_query ( DB );

    select_query.PREPARE_SELECT ( "dmr_fisler","fis_id ",
                                  "fis_no = :fis_no "
                                  "AND fis_tarihi = :fis_tarihi" );

    select_query.SET_VALUE     ( ":fis_no"     ,fis_no );
    select_query.SET_VALUE     ( ":fis_tarihi" ,m_ui->adakDate_fis_tarihi->DATE() );

    if ( select_query.SELECT() NE 0 ) {
        select_query.NEXT();
        if ( select_query.VALUE(0).toInt() NE record_id ) {
            MSG_WARNING( tr ( "Aynı numarada kayıtlı başka bir fiş bulunmaktadır.Güncelleme yapılamadı!.."), m_ui->lineedit_fis_no );

            return ADAK_FAIL;
        }
    }

    return ADAK_OK;
}

/**************************************************************************************
                   DMR_YENIDEN_DEGERLEME_FISI::UPDATE_FIS_RECORD
***************************************************************************************/

void DMR_YENIDEN_DEGERLEME_FISI::UPDATE_FIS_RECORD ( int record_id )
{
    DMR_FIS_BILGILERINI_OKU(M_FIS , record_id);

    M_FIS->aciklama        = m_ui->limitedTextEdit_aciklama->toPlainText();
    M_FIS->fis_tarihi      = m_ui->adakDate_fis_tarihi->DATE();
    M_FIS->degerleme_orani = m_ui->commaEdit_degerleme_orani->GET_DOUBLE();
    M_FIS->fis_no          = m_ui->lineedit_fis_no->text().toInt();

    DMR_FIS_GUNCELLE(M_FIS , record_id);

    m_log_kaydi_eklenecek_mi = true;

    m_log_detaylari = QObject::tr ( "Yeniden Değerleme Fişi , Yeniden Değerleme Oranı : % " ) +
                      m_ui->commaEdit_degerleme_orani->GET_TEXT();

    m_log_detaylari.append ( "\nFiş Tarihi : " + m_ui->adakDate_fis_tarihi->QDATE().toString("dd MMMM YYYY")
                            + ",Fiş No : " + m_ui->lineedit_fis_no->text());
}

/**************************************************************************************
                   DMR_YENIDEN_DEGERLEME_FISI::CHECK_FIS_RECORD_DELETE
***************************************************************************************/

int DMR_YENIDEN_DEGERLEME_FISI::CHECK_FIS_RECORD_DELETE ( int record_id )
{
    Q_UNUSED ( record_id );

    return ADAK_OK;
}

/**************************************************************************************
                   DMR_YENIDEN_DEGERLEME_FISI::DELETE_FIS_RECORD
***************************************************************************************/

void DMR_YENIDEN_DEGERLEME_FISI::DELETE_FIS_RECORD ( int record_id )
{
    DMR_FIS_SIL(M_FIS , record_id);

    m_log_kaydi_eklenecek_mi = true;
    m_kayit_silindi          = true;

    m_log_detaylari = QObject::tr ( "Yeniden Değerleme Fişi , Yeniden Değerleme Oranı : % " ) +
                      VIRGUL_EKLE ( QVariant ( ROUND  ( M_FIS->degerleme_orani ) ).toString(),2 );

    m_log_detaylari.append("\nFiş Tarihi : " + QDate::fromString(M_FIS->fis_tarihi,"yyyy.MM.dd").toString("dd MMMM yyyy dddd")
                           + ",Fiş No : " + QVariant ( M_FIS->fis_no ).toString());

}

/**************************************************************************************
      DMR_YENIDEN_DEGERLEME_FISI::SELECT_FIS_RECORD
***************************************************************************************/

int DMR_YENIDEN_DEGERLEME_FISI::SELECT_FIS_RECORD()
{
   return OPEN_DMR_FIS_ARAMA ( YENIDEN_DEGERLEME_FISI,this );
}

/**************************************************************************************
     DMR_YENIDEN_DEGERLEME_FISI::FIND_FIS_RECORD
***************************************************************************************/

int DMR_YENIDEN_DEGERLEME_FISI::FIND_FIS_RECORD()
{
    SQL_QUERY select_query ( DB );

    select_query.PREPARE_SELECT ( "dmr_fisler","fis_id ",
                                  "fis_no = :fis_no "
                                  "AND fis_turu     = :fis_turu "
                                  "AND fis_tarihi   = :fis_tarihi" );

    select_query.SET_VALUE      ( ":fis_no"       , m_ui->lineedit_fis_no->text().toInt() );
    select_query.SET_VALUE      ( ":fis_turu"     , YENIDEN_DEGERLEME_FISI );
    select_query.SET_VALUE      ( ":fis_tarihi"   , m_ui->adakDate_fis_tarihi->DATE() );

    if ( select_query.SELECT() EQ 0 ) {
        MSG_WARNING(  tr ( "Aradığınız Yeniden Değerlendirme Fişi Bulunamadı!.." ), m_ui-> lineedit_fis_no );

        return ADAK_FAIL;
    }
    select_query.NEXT();

    return select_query.VALUE(0).toInt();
}

/**************************************************************************************
                   DMR_YENIDEN_DEGERLEME_FISI::FIND_FIRST_FIS_RECORD
***************************************************************************************/

int DMR_YENIDEN_DEGERLEME_FISI::FIND_FIRST_FIS_RECORD()
{
    SQL_QUERY select_query ( DB );

    select_query.PREPARE_SELECT ( "dmr_fisler","fis_id  ",
                                  "fis_turu = :fis_turu ");


    select_query.SET_VALUE ( ":fis_turu" , YENIDEN_DEGERLEME_FISI );

    if ( select_query.SELECT("fis_tarihi,fis_no",0,1)  EQ 0 ) {
        return 0;
    }
    select_query.NEXT();

    return select_query.VALUE(0).toInt();
}

/**************************************************************************************
                   DMR_YENIDEN_DEGERLEME_FISI::FIND_LAST_FIS_RECORD
***************************************************************************************/

int DMR_YENIDEN_DEGERLEME_FISI::FIND_LAST_FIS_RECORD()
{
    SQL_QUERY select_query ( DB );

    select_query.PREPARE_SELECT  ( "dmr_fisler","fis_id  ",
                                   "fis_turu = :fis_turu ");


    select_query.SET_VALUE ( ":fis_turu" , YENIDEN_DEGERLEME_FISI );

    if ( select_query.SELECT("fis_tarihi DESC, fis_no DESC",0,1) EQ 0 ) {
        return 0;
    }
    select_query.NEXT();

    return select_query.VALUE(0).toInt();
}

/**************************************************************************************
                   DMR_YENIDEN_DEGERLEME_FISI::FIND_PREV_FIS_RECORD
***************************************************************************************/

int DMR_YENIDEN_DEGERLEME_FISI::FIND_PREV_FIS_RECORD()
{
    SQL_QUERY select_query ( DB );

    select_query.PREPARE_SELECT ( "dmr_fisler","fis_id",
                                  "fis_no             < :fis_no     "
                                  "AND fis_turu       = :fis_turu   "
                                  "AND fis_tarihi     = :fis_tarihi ");


    select_query.SET_VALUE ( ":fis_no"       , m_ui->lineedit_fis_no->text().toInt() );
    select_query.SET_VALUE ( ":fis_turu"     , YENIDEN_DEGERLEME_FISI);
    select_query.SET_VALUE ( ":fis_tarihi"   , m_ui->adakDate_fis_tarihi->DATE() );

    if ( select_query.SELECT("fis_no DESC",0,1) NE 0 ) {
        select_query.NEXT();
        return select_query.VALUE(0).toInt();
    }

    select_query.PREPARE_SELECT ( "dmr_fisler","fis_id  ",
                                  "fis_turu   = :fis_turu "
                                  "AND   fis_tarihi < :fis_tarihi");


    select_query.SET_VALUE  ( ":fis_turu"     , YENIDEN_DEGERLEME_FISI );
    select_query.SET_VALUE  ( ":fis_tarihi"   , m_ui->adakDate_fis_tarihi->DATE() );

    if ( select_query.SELECT("fis_tarihi DESC, fis_no DESC",0,1) EQ 0 ) {
        return 0;
    }
    select_query.NEXT();
    return select_query.VALUE(0).toInt();
}

/**************************************************************************************
                   DMR_YENIDEN_DEGERLEME_FISI::FIND_NEXT_FIS_RECORD
***************************************************************************************/

int DMR_YENIDEN_DEGERLEME_FISI::FIND_NEXT_FIS_RECORD()
{
    SQL_QUERY select_query ( DB );

    select_query.PREPARE_SELECT ( "dmr_fisler","fis_id ",
                                  "fis_no             > :fis_no   "
                                  "AND fis_turu       = :fis_turu  "
                                  "AND fis_tarihi     = :fis_tarihi");


    select_query.SET_VALUE  ( ":fis_no"       , m_ui->lineedit_fis_no->text().toInt() );
    select_query.SET_VALUE  ( ":fis_turu"     , YENIDEN_DEGERLEME_FISI );
    select_query.SET_VALUE  ( ":fis_tarihi"   , m_ui->adakDate_fis_tarihi->DATE() );

    if ( select_query.SELECT("fis_no ASC",0,1) NE 0 ) {
        select_query.NEXT();
        return select_query.VALUE(0).toInt();
    }

    select_query.PREPARE_SELECT ( "dmr_fisler"," fis_id ",
                                  "fis_turu       = :fis_turu  "
                                  "AND fis_tarihi > :fis_tarihi");


    select_query.SET_VALUE ( ":fis_turu"     , YENIDEN_DEGERLEME_FISI );
    select_query.SET_VALUE ( ":fis_tarihi"   , m_ui->adakDate_fis_tarihi->DATE() );

    if ( select_query.SELECT("fis_tarihi ASC, fis_no ASC",0,1) EQ 0 ) {
        return 0;
    }
    select_query.NEXT();
    return select_query.VALUE(0).toInt();
}

/**************************************************************************************
                   DMR_YENIDEN_DEGERLEME_FISI::CHECK_LINE_VAR
***************************************************************************************/

int DMR_YENIDEN_DEGERLEME_FISI::CHECK_LINE_VAR ( int row_number, QObject * object )
{

    QSearchEdit * searchedit_demirbas_kodu                  = ( QSearchEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, DMR_KODU_COLUMN                        );
    QCommaEdit  * commaedit_demirbasin_degeri               = ( QCommaEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, DEMIRBAS_DEGERI_COLUMN                 );
    QCommaEdit  * commaedit_birikmis_amortisman             = ( QCommaEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, BIRIKMIS_AMORTISMAN_TUTARI_COLUMN      );
    QCommaEdit  * commaedit_yeni_birikmis_amortisman_degeri = ( QCommaEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, YENI_BIRIKMIS_AMORTISMAN_TUTARI_COLUMN );
    QCommaEdit  * commaedit_yeni_demirbas_degeri            = ( QCommaEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, YENI_DEMIRBAS_DEGERI_COLUMN            );
    QLineEdit   * lineedit_demirbas_id                      = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, DEMIRBAS_ID_COLUMN);
    QLineEdit * lineedit_demirbas_ismi                      = ( QLineEdit * )  m_ui->tablewidget_fis_satirlari->cellWidget ( row_number,DMR_ISMI_COLUMN);

    if ( object EQ searchedit_demirbas_kodu ) {

        if ( searchedit_demirbas_kodu->GET_TEXT().isEmpty() EQ true ) {
            lineedit_demirbas_id->clear();
            lineedit_demirbas_ismi->clear();
            return ADAK_OK;
        }

        SQL_QUERY select_query ( DB );

        select_query.PREPARE_SELECT ( "dmr_demirbaslar","yeniden_degerleme_yapildi,yeniden_degerleme_yapilacak_mi , "
                                      "demirbas_adi,demirbasin_degeri,alis_tarihi,demirbas_id ",
                                      "demirbas_kodu = :demirbas_kodu" );
        select_query.SET_VALUE ( ":demirbas_kodu" , searchedit_demirbas_kodu->GET_TEXT() );

        if ( select_query.SELECT() EQ 0 ) {
            MSG_WARNING( tr ( "Aradığınız demirbaş kodu bulunamadı!.." ), searchedit_demirbas_kodu );

            return ADAK_FAIL;
        }


        select_query.NEXT();

        int yeniden_degerleme_yapilacak_mi = select_query.VALUE(1).toInt();
        if ( yeniden_degerleme_yapilacak_mi EQ 0 ) {
            MSG_WARNING( tr ( "Seçilen demirbaş için yeniden değerleme işlemi yapılmayacaktır."), NULL );
            return ADAK_FAIL;
        }

        int yeniden_degerleme_yapildi = select_query.VALUE(0).toInt();
        if ( yeniden_degerleme_yapildi EQ 1 ) {
            MSG_WARNING( tr ( "Girdiğiniz demirbaş için yeniden değerleme yapılmıştır.Tekrar yapamazsınız!.."), NULL );
            return ADAK_FAIL;
        }

        QDate demirbas_alis_tarihi = QDate::fromString ( select_query.VALUE(4).toString(),"yyyy.MM.dd" );
        int demirbas_alim_yili     = demirbas_alis_tarihi.year();
        int current_year           = m_ui->adakDate_fis_tarihi->QDATE().year();


        if ( demirbas_alim_yili EQ current_year ) {
            MSG_WARNING( tr ("Seçilen demirbaş bu yıl içerisinde alınmıştır.Yeniden değerlemeye tabi tutulamaz!.."), NULL );
            return ADAK_FAIL;
        }
        lineedit_demirbas_id->setText(select_query.VALUE("demirbas_id").toString());


        lineedit_demirbas_ismi->setText(select_query.VALUE(2).toString());
        commaedit_demirbasin_degeri->SET_DOUBLE ( select_query.VALUE(3).toDouble() );

        commaedit_birikmis_amortisman->SET_DOUBLE ( DMR_GET_BIRIKMIS_AMORTISMAN_TUTARI ( select_query.VALUE(5).toInt(),m_ui->adakDate_fis_tarihi->MALI_YIL_ARRAY_INDIS() ));

        double degerleme_orani                 = m_ui->commaEdit_degerleme_orani->GET_DOUBLE();
        double demirbasin_yeni_degeri          = select_query.VALUE(3).toDouble() * (1 + (degerleme_orani / 100));
        double yeni_birikmis_amortisman_degeri = commaedit_birikmis_amortisman->GET_DOUBLE() * (1 + (degerleme_orani / 100 ));

        commaedit_yeni_birikmis_amortisman_degeri->SET_DOUBLE ( yeni_birikmis_amortisman_degeri );
        commaedit_yeni_demirbas_degeri->SET_DOUBLE ( demirbasin_yeni_degeri );
    }

    return ADAK_OK;
}

/**************************************************************************************
                   DMR_YENIDEN_DEGERLEME_FISI::CHECK_LINE_EMPTY
***************************************************************************************/

int DMR_YENIDEN_DEGERLEME_FISI::CHECK_LINE_EMPTY ( int row_number )
{
    QSearchEdit * searchedit_demirbas_kodu =  ( QSearchEdit* ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number,DMR_KODU_COLUMN);

    if ( searchedit_demirbas_kodu->GET_TEXT().isEmpty() EQ true ) {
        MSG_WARNING( tr ( "Demirbaş kodunu boş bırakamazsınız!.." ), searchedit_demirbas_kodu );

        return ADAK_FAIL;
    }
    return ADAK_OK;
}


/**************************************************************************************
                   DMR_YENIDEN_DEGERLEME_FISI::CHECK_ADD_LINE
***************************************************************************************/

int DMR_YENIDEN_DEGERLEME_FISI::CHECK_ADD_LINE ( int record_id, int row_number )
{
    Q_UNUSED ( record_id );
    Q_UNUSED ( row_number );
    return ADAK_OK;
}

/**************************************************************************************
                   DMR_YENIDEN_DEGERLEME_FISI::ADD_LINE
***************************************************************************************/

void DMR_YENIDEN_DEGERLEME_FISI::ADD_LINE ( int record_id, int row_number )
{
    QLineEdit   * lineedit_aciklama                         = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, ACIKLAMA_COLUMN                        );
    QCommaEdit  * commaedit_degerleme_sonrasi_tutar         = ( QCommaEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, YENI_DEMIRBAS_DEGERI_COLUMN            );
    QCommaEdit  * commaedit_yeni_brkms_amortisman_tutari    = ( QCommaEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, YENI_BIRIKMIS_AMORTISMAN_TUTARI_COLUMN );
    QCommaEdit  * commaedit_birikmis_amortisman_tutari      = ( QCommaEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, BIRIKMIS_AMORTISMAN_TUTARI_COLUMN      );
    QLineEdit   * lineedit_demirbas_id                      = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, DEMIRBAS_ID_COLUMN);

    int demirbas_id = lineedit_demirbas_id->text().toInt();

    DMR_CLEAR_FIS_SATIRI_STRUCT(M_FIS_SATIRI);

    M_FIS_SATIRI->program_id                = E9_PROGRAMI;
    M_FIS_SATIRI->modul_id                  = DEMIRBAS_MODULU;
    M_FIS_SATIRI->demirbas_id               = demirbas_id;
    M_FIS_SATIRI->aciklama                  = lineedit_aciklama->text();
    M_FIS_SATIRI->satir_tutari              = commaedit_degerleme_sonrasi_tutar->GET_DOUBLE();
    M_FIS_SATIRI->birikmis_amor_tutari      = commaedit_birikmis_amortisman_tutari->GET_DOUBLE();
    M_FIS_SATIRI->yeni_birikmis_amor_tutari = commaedit_yeni_brkms_amortisman_tutari->GET_DOUBLE();
    M_FIS_SATIRI->fis_id                    = record_id;

    int row_id  = DMR_FIS_SATIRI_EKLE(M_FIS , M_FIS_SATIRI );

    QTableWidgetItem * item = new QTableWidgetItem ( QVariant(row_id).toString() );
    m_ui->tablewidget_fis_satirlari->setItem ( row_number,ROW_ID_COLUMN,item );

    item = new QTableWidgetItem ( QVariant ( record_id ).toString() );
    m_ui->tablewidget_fis_satirlari->setItem ( row_number,FIS_ID_COLUMN,item );


    m_ui->commaedit_top_yeni_demirbas_degeri->SET_DOUBLE ( M_FIS->toplam_tutar );
    m_ui->commaEdit_top_yeni_bir_amor_degeri->SET_DOUBLE ( M_FIS->top_birikmis_amor_tutari );

    m_log_kaydi_eklenecek_mi = true;

    m_log_detaylari = QObject::tr ( "Yeniden Değerleme Fişi , Yeniden Değerleme Oranı : % ") +
                      VIRGUL_EKLE ( QVariant( ROUND (M_FIS->degerleme_orani )).toString(),2 );

    m_log_detaylari.append ( "\nFiş Tarihi : " + QDate::fromString(M_FIS->fis_tarihi , "yyyy.MM.dd").toString("dd MMMM yyyy dddd")
                           + ",Fiş No : " + QVariant (M_FIS->fis_no).toString() );
}

/**************************************************************************************
                   DMR_YENIDEN_DEGERLEME_FISI::CHECK_UPDATE_LINE
***************************************************************************************/

int DMR_YENIDEN_DEGERLEME_FISI::CHECK_UPDATE_LINE ( int record_id, int row_number )
{
    Q_UNUSED ( record_id );
    Q_UNUSED ( row_number );

    return ADAK_OK;
}

/**************************************************************************************
                   DMR_YENIDEN_DEGERLEME_FISI::UPDATE_LINE
***************************************************************************************/

void DMR_YENIDEN_DEGERLEME_FISI::UPDATE_LINE ( int record_id, int row_number )
{
    Q_UNUSED(record_id);

    QLineEdit   * lineedit_aciklama                         = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, ACIKLAMA_COLUMN                         );
    QCommaEdit  * commaedit_degerleme_sonrasi_tutar         = ( QCommaEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, YENI_DEMIRBAS_DEGERI_COLUMN             );
    QCommaEdit  * commaedit_birikmis_amortisman_tutari      = ( QCommaEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, BIRIKMIS_AMORTISMAN_TUTARI_COLUMN       );
    QLineEdit   * lineedit_demirbas_id                      = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, DEMIRBAS_ID_COLUMN);
    QCommaEdit  * commaedit_yeni_bir_amortisman_tutari      = ( QCommaEdit   *) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, YENI_BIRIKMIS_AMORTISMAN_TUTARI_COLUMN );

    int fis_satiri_id = m_ui->tablewidget_fis_satirlari->item(row_number , ROW_ID_COLUMN)->text().toInt();

    DMR_FIS_SATIRINI_OKU(M_FIS_SATIRI , fis_satiri_id);

    int demirbas_id = lineedit_demirbas_id->text().toInt();

    M_FIS_SATIRI->demirbas_id               = demirbas_id;
    M_FIS_SATIRI->aciklama                  = lineedit_aciklama->text();
    M_FIS_SATIRI->satir_tutari              = commaedit_degerleme_sonrasi_tutar->GET_DOUBLE();
    M_FIS_SATIRI->birikmis_amor_tutari      = commaedit_birikmis_amortisman_tutari->GET_DOUBLE();
    M_FIS_SATIRI->yeni_birikmis_amor_tutari = commaedit_yeni_bir_amortisman_tutari->GET_DOUBLE();

    DMR_FIS_SATIRI_GUNCELLE(M_FIS , M_FIS_SATIRI , fis_satiri_id);


    m_ui->commaedit_top_yeni_demirbas_degeri->SET_DOUBLE ( M_FIS->toplam_tutar );
    m_ui->commaEdit_top_yeni_bir_amor_degeri->SET_DOUBLE ( M_FIS->top_birikmis_amor_tutari );

    m_log_kaydi_eklenecek_mi = true;

    m_log_detaylari = QObject::tr ( "Yeniden Değerleme Fişi , Yeniden Değerleme Oranı : % " ) +
                      VIRGUL_EKLE ( QVariant ( ROUND ( M_FIS->degerleme_orani ) ).toString(),2 );

    m_log_detaylari.append ( "\nFiş Tarihi : " + QDate::fromString(M_FIS->fis_tarihi , "yyyy.MM.dd").toString("dd MMMM yyyy")
                           + ",Fiş No : " + QVariant ( M_FIS->fis_no ).toString() );
}

/**************************************************************************************
                   DMR_YENIDEN_DEGERLEME_FISI::CHECK_DELETE_LINE
***************************************************************************************/

int DMR_YENIDEN_DEGERLEME_FISI::CHECK_DELETE_LINE ( int record_id, int row_number )
{
    Q_UNUSED ( record_id );
    Q_UNUSED ( row_number );
    return ADAK_OK;
}

/**************************************************************************************
                   DMR_YENIDEN_DEGERLEME_FISI::DELETE_LINE
***************************************************************************************/

void DMR_YENIDEN_DEGERLEME_FISI::DELETE_LINE ( int record_id, int row_number )
{
    Q_UNUSED(record_id);


    int fis_satiri_id = m_ui->tablewidget_fis_satirlari->item(row_number , ROW_ID_COLUMN)->text().toInt();

    DMR_FIS_SATIRI_SIL(M_FIS , fis_satiri_id);

    m_ui->commaedit_top_yeni_demirbas_degeri->SET_DOUBLE ( M_FIS->toplam_tutar );
    m_ui->commaEdit_top_yeni_bir_amor_degeri->SET_DOUBLE ( M_FIS->top_birikmis_amor_tutari);


    m_log_kaydi_eklenecek_mi = true;

    m_log_detaylari = QObject::tr ( "Yeniden Değerleme Fişi , Yeniden Değerleme Oranı : % " ) +
                      VIRGUL_EKLE ( QVariant( ROUND ( M_FIS->degerleme_orani ) ).toString(),2 );

    m_log_detaylari.append ( "\nFiş Tarihi : " + QDate::fromString(M_FIS->fis_tarihi , "yyyy.MM.dd").toString("dd MMM yyy")
                            + ",Fiş No : " + QVariant (M_FIS->fis_no).toString() );

}

/**************************************************************************************
     DMR_YENIDEN_DEGERLEME_FISI::LOCK_FIS_RECORD
****************************************************/

int DMR_YENIDEN_DEGERLEME_FISI::LOCK_FIS_RECORD ( int record_id )
{
    return DB->LOCK_ROW ( "fis_id","dmr_fisler", QString ( "fis_id = %1" ).arg ( record_id ) );
}

/**************************************************************************************
                   DMR_YENIDEN_DEGERLEME_FISI::UNLOCK_FIS_RECORD
***************************************************************************************/

void DMR_YENIDEN_DEGERLEME_FISI::UNLOCK_FIS_RECORD(int record_id)
{
    DB->UNLOCK_ROW( "fis_id","dmr_fisler", QString ( "fis_id = %1" ).arg ( record_id ));
}

/**************************************************************************************
                   DMR_YENIDEN_DEGERLEME_FISI::CHECK_EXIT
***************************************************************************************/

int DMR_YENIDEN_DEGERLEME_FISI::CHECK_EXIT()
{
    return ADAK_EXIT;
}

/**************************************************************************************
                   DMR_YENIDEN_DEGERLEME_FISI::LINE_SEARCH_EDIT_CLICKED
***************************************************************************************/

void DMR_YENIDEN_DEGERLEME_FISI::LINE_SEARCH_EDIT_CLICKED ( int row_number,QWidget * , QLineEdit * lineedit )
{
    Q_UNUSED ( row_number );
    int demirbas_id = OPEN_DMR_DEMIRBAS_KARTI_ARAMA ( lineedit->text(), this );

    if ( demirbas_id NE -1 ) {
        SQL_QUERY select_query ( DB );

        select_query.PREPARE_SELECT ( "dmr_demirbaslar","demirbas_kodu","demirbas_id = :demirbas_id");
        select_query.SET_VALUE ( ":demirbas_id" , demirbas_id );

        if ( select_query.SELECT() NE 0 ) {
            select_query.NEXT();
            lineedit->setText ( select_query.VALUE(0).toString() );
        }
    }
}

/**************************************************************************************
                   DMR_YENIDEN_DEGERLEME_FISI::LEAVE_RECORD
***************************************************************************************/

int DMR_YENIDEN_DEGERLEME_FISI::LEAVE_RECORD()
{
    if ( m_log_kaydi_eklenecek_mi EQ true ) {
        DB->START_TRANSACTION();
        if ( m_kayit_eklendi EQ true ) {
            E9_LOG_KAYDI_EKLE ( DEMIRBAS_MODULU , LOG_DMR_FISLER , LOG_ISLEM_ADD , m_log_detaylari );
        }
        if ( m_kayit_silindi EQ true ) {
            E9_LOG_KAYDI_EKLE ( DEMIRBAS_MODULU , LOG_DMR_FISLER , LOG_ISLEM_DELETE , m_log_detaylari );
        }
        if ( m_kayit_eklendi EQ false AND m_kayit_silindi EQ false ) {
            E9_LOG_KAYDI_EKLE ( DEMIRBAS_MODULU , LOG_DMR_FISLER , LOG_ISLEM_UPDATE , m_log_detaylari );
        }
        DB->COMMIT_TRANSACTION();
    }
    return ADAK_OK;
}









