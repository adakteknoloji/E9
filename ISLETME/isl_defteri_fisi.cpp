#include "adak_sql.h"
#include "print.h"
#include "adak_utils.h"
#include "isl_defteri_fisi_class.h"
#include "isl_defteri_fisi_open.h"
#include "ui_isl_defteri_fisi.h"
#include "yonetim.h"
#include "isl_struct.h"
#include "e9_console_utils.h"
#include "e9_gui_utils.h"
#include "adak_std_utils.h"
#include "e9_uretilen_fis_secimi_open.h"
#include "e9_log.h"
#include "e9_yetki.h"
#include "isl_fis_utils.h"
#include "isl_defteri_raporu_batch_open.h"
#include "isl_kdv_raporu_batch_open.h"
#include "isl_fis_arama_open.h"
#include "isl_fis_kopyalama_batch_open.h"
#include "isl_fis_yazdir_open.h"
#include "isl_console_utils.h"
#include "isl_enum.h"
#include "e9_enum.h"
#include "e9_struct.h"
#include "dvz_enum.h"
#include "dvz_console_utils.h"
#include "e9_eft_batch_open.h"
#include "e9_kk_odeme_batch_open.h"
#include "e9_kk_tahsilat_batch_open.h"
#include "isl_gui_utils.h"

extern ADAK_SQL * DB;


#define ROW_ID_COLUMN             0
#define FIS_ID_COLUMN             1
#define ORDER_COLUMN              2

#define BELGE_NO_COLUMN           3
#define ACIKLAMA_COLUMN           4
#define GIDER_TURU_COLUMN         5
#define KDV_ORANI_COLUMN          6
#define KDV_HARIC_TUTAR_COLUMN    7
#define KDV_TUTARI_COLUMN         8
#define KDV_DAHIL_TUTAR_COLUMN    9

/**************************************************************************************
                   OPEN_ISL_ISLETME_FISI
***************************************************************************************/

void OPEN_ISL_ISLETME_FISI ( int fis_id , int fis_turu , QWidget * parent )
{

    ISL_DEFTERI_FISI * F = new ISL_DEFTERI_FISI ( fis_id,fis_turu, parent );
    F->setWindowTitle ( VERITABANI_ISMI() + " ( "+ QObject::tr ("İşletme Defteri" )+" )");
    F->SHOW( FULL_SCREEN );
}

/**************************************************************************************
                   ISL_DEFTERI_FISI::ISL_DEFTERI_FISI
***************************************************************************************/

ISL_DEFTERI_FISI::ISL_DEFTERI_FISI (  int fis_id, int fis_turu ,QWidget * parent  ) : FIS_KERNEL ( parent ), m_ui ( new Ui::ISL_DEFTERI_FISI )
{
    m_fis_id  = fis_id;
    m_fis_turu = fis_turu;

    m_ui->setupUi    ( this );

    START_FIS_KERNEL ( this, DB );
 }

/**************************************************************************************
                   ISL_DEFTERI_FISI::SETUP_FORM
***************************************************************************************/

void ISL_DEFTERI_FISI::SETUP_FORM()
{

    if( m_fis_turu EQ ENUM_GELIR_FISI ) {
        SET_HELP_PAGE ( tr("isletme-defteri_gelir-fisi") );
        SET_PAGE_TITLE ( tr ( "İŞL - GELİR FİŞİ" ) );
    }
    else {
        SET_HELP_PAGE ( tr("isletme-defteri_gider-fisi") );
        SET_PAGE_TITLE ( tr ( "İŞL - GİDER FİŞİ" ) );
    }

    REGISTER_BUTTONS_WIDGET  ( m_ui->navigation_button_widget );

    M_ISL_FISI            = new ISL_FIS_STRUCT;
    M_ISL_FIS_SATIRI      = new ISL_FIS_SATIRI_STRUCT;

    ISL_CLEAR_FIS_STRUCT       (M_ISL_FISI);
    ISL_CLEAR_FIS_SATIRI_STRUCT(M_ISL_FIS_SATIRI);

    M_ISL_FISI->program_id = E9_PROGRAMI;
    M_ISL_FISI->modul_id   = ISLETME_MODULU;

    QRegExp rx ( "(\\*?)(\\d+)" );

    QRegExpValidator * validator = new QRegExpValidator ( rx, this );

    m_ui->line_edit_fis_no->setValidator ( validator );

    REGISTER_TABLE_WIDGET             ( m_ui->tablewidget_isletme_defteri , 10 );
    SET_FORM_ID_COLUMN_POSITION       ( FIS_ID_COLUMN );
    SET_FIS_ORDER_COLUMN_NUMBER       ( ORDER_COLUMN  );
    SET_ROW_ID_COLUMN_POSITION        ( ROW_ID_COLUMN );
    SET_NUMBER_OF_HIDDEN_COLUMN       ( 3 );

    SET_LINE_BUTTON_ICON(QStringList() << "");

    m_log_kaydi_eklensin_mi       = false;
    m_kayit_eklendi               = false;
    m_kayit_silindi               = false;

    m_ui->line_edit_fis_no->setMaxLength(10);
    M_KDV_ORANLARI_ARRAY              = new double [KDV_OTV_ORANLARI_SAYISI];
    M_INDIRILECEK_KDV_HESABI_ID_ARRAY = new int [KDV_OTV_ORANLARI_SAYISI];

    SQL_QUERY query(DB);

    query.PREPARE_SELECT("e9_sabit_degerler" , "ontanimli_kdv_orani,kdv_oranlari_array,ind_kdv_hesap_id_array ",
                         "sabit_deger_id = :sabit_deger_id");
    query.SET_VALUE(":sabit_deger_id" , 1);

    if ( query.SELECT() NE 0 ) {
        query.NEXT();
        m_ontanimli_kdv_orani = query.VALUE(0).toDouble();
        UNPACK_DOUBLE_ARRAY(query.VALUE(1).toString() , M_KDV_ORANLARI_ARRAY , KDV_OTV_ORANLARI_SAYISI);
        UNPACK_INT_ARRAY(query.VALUE(2).toString() ,M_INDIRILECEK_KDV_HESABI_ID_ARRAY , KDV_OTV_ORANLARI_SAYISI );
    }

    SET_FIS_ORDER_DATABASE ( "isl_fis_satirlari", "order_number", "fis_satiri_id" );

    SET_TABLE_ROW_WIDGETS ( GIDER_TURU_COLUMN,      WIDGET_COMBO_BOX  );
    SET_TABLE_ROW_WIDGETS ( KDV_ORANI_COLUMN,       WIDGET_COMBO_BOX  );
    SET_TABLE_ROW_WIDGETS ( KDV_HARIC_TUTAR_COLUMN, WIDGET_COMMA_EDIT );
    SET_TABLE_ROW_WIDGETS ( KDV_TUTARI_COLUMN,      WIDGET_COMMA_EDIT );
    SET_TABLE_ROW_WIDGETS ( KDV_DAHIL_TUTAR_COLUMN, WIDGET_COMMA_EDIT );
    SET_TABLE_ROW_WIDGETS ( ACIKLAMA_COLUMN,        WIDGET_LINE_EDIT  );
    SET_TABLE_ROW_WIDGETS ( BELGE_NO_COLUMN,        WIDGET_LINE_EDIT  );

    SET_FIRST_FOCUS_WIDGET ( m_ui->line_edit_fis_no );

    REGISTER_SAVER_BUTTON  ( m_ui->Button_yazdir      );
    REGISTER_SAVER_BUTTON  ( m_ui->button_fis_kopyala );

    m_ui->limitedTextEdit->SET_LINE_LIMIT        ( 5 );
    m_ui->limitedTextEdit->SET_MAX_STRING_LENGTH ( 512 );

    m_ui->line_edit_fis_no->setText("*");

    DISABLE_CLEAR_ON_WIDGET ( m_ui->line_edit_fis_no );

    if ( m_fis_turu EQ ENUM_GELIR_FISI ) {

        m_ui->tablewidget_isletme_defteri->setHorizontalHeaderLabels ( QStringList()<<"id"<<"fis_id"<<"satir_order"
                                                                                << tr ( "Belge No" )
                                                                                << tr ( "Açıklama" )<<tr ( "Gelir Türü" )<<tr ( "KDV %" )
                                                                                << tr ( "KDV Hariç Tutar" )<<tr ( "KDV Tutarı" )<<tr ( "KDV Dahil Tutar" )
                                                                                <<tr ( "" )<<tr ( "" ) );
    }
    else {
        m_ui->tablewidget_isletme_defteri->setHorizontalHeaderLabels ( QStringList()<<"id"<<"fis_id"<<"satir_order"
                                                                                << tr ( "Belge No" )
                                                                                << tr ( "Açıklama" )<<tr ( "Gider Türü" )<<tr ( "KDV %" )
                                                                                << tr ( "KDV Hariç Tutar" )<<tr ( "KDV Tutarı" )<<tr ( "KDV Dahil Tutar" )
                                                                                <<tr ( "" ) <<tr ( "" ));
    }

    m_ui->tablewidget_isletme_defteri->setSelectionMode ( QAbstractItemView::NoSelection );
    m_ui->tablewidget_isletme_defteri->setFocus         ( Qt::OtherFocusReason );

    FIS_EKRANINI_DUZENLE ();

    FOCUS_FIRST_WIDGET      ();
    AUTOSTART_FIS_KERNEL    ( m_fis_id );
}

/**************************************************************************************
                   ISL_DEFTERI_FISI::CLEAR_FORM_MEMBERS
***************************************************************************************/

void ISL_DEFTERI_FISI::CLEAR_FORM_MEMBERS ()
{
}

/**************************************************************************************
                   ISL_DEFTERI_FISI::NEW_FIS_RECORD
***************************************************************************************/

void ISL_DEFTERI_FISI::NEW_FIS_RECORD ()
{
    ISL_CLEAR_FIS_STRUCT       (M_ISL_FISI);
    ISL_CLEAR_FIS_SATIRI_STRUCT(M_ISL_FIS_SATIRI);

    m_ui->line_edit_fis_no->setText("*");

    M_ISL_FISI->program_id = E9_PROGRAMI;
    M_ISL_FISI->modul_id   = ISLETME_MODULU;

}

/**************************************************************************************
                   ISL_DEFTERI_FISI::GET_FIS_RECORD
***************************************************************************************/

int ISL_DEFTERI_FISI::GET_FIS_RECORD ( int fis_id )
{
    SQL_QUERY sql_query ( DB );

    ISL_FIS_BILGILERINI_OKU ( M_ISL_FISI , fis_id);

    m_fis_turu = M_ISL_FISI->fis_turu;

    QDate fis_tarihi = QDate::fromString ( M_ISL_FISI->fis_tarihi, "yyyy.MM.dd" );

    m_ui->CommaEdit_kdv_haric_tutar->SET_DOUBLE ( M_ISL_FISI->kdv_haric_toplam );
    m_ui->CommaEdit_kdv_tutari->SET_DOUBLE      ( M_ISL_FISI->kdv_toplam);
    m_ui->CommaEdit_toplam_tutar->SET_DOUBLE    ( M_ISL_FISI->kdv_dahil_toplam );
    m_ui->limitedTextEdit->setText              ( M_ISL_FISI->aciklama );
    m_ui->adakDate_fis_tarihi->SET_DATE        ( fis_tarihi );

    m_ui->line_edit_fis_no->setText( QVariant ( M_ISL_FISI->fis_no ).toString() );

    sql_query.PREPARE_SELECT ("isl_fis_satirlari",
                              "fis_satiri_id, order_number, belge_no, aciklama, gider_turu, "
                             "kdv_orani, satir_kdv_haric_tutar, satir_kdv_tutari, "
                              "satir_kdv_dahil_tutar,kk_sahibi,kk_numarasi,"
                              "pos_id,kredi_karti_id,hesap_no_id",
                             "fis_id = :fis_id " );
    sql_query.SET_VALUE(":fis_id", fis_id );

    if ( sql_query.SELECT("order_number ASC" ) EQ 0 ) {
        return ADAK_OK;
    }

    while ( sql_query.NEXT() EQ true ) {

        int current_row = ADD_NEW_LINE();

        int     fis_satiri_id         = sql_query.VALUE(0).toInt();
        int     order_number          = sql_query.VALUE(1).toInt();
        QString belge_no              = sql_query.VALUE(2).toString();
        QString aciklama              = sql_query.VALUE(3).toString();
        int     gider_turu            = sql_query.VALUE(4).toInt();
        double  kdv_orani             = sql_query.VALUE(5).toInt();
        double  satir_kdv_haric_tutar = sql_query.VALUE(6).toDouble();
        double  satir_kdv_tutari      = sql_query.VALUE(7).toDouble();
        double  satir_kdv_dahil_tutar = sql_query.VALUE(8).toDouble();

        QTableWidgetItem * new_item;

        new_item = new QTableWidgetItem ( tr ( "%1" ).arg ( fis_satiri_id ) );
        m_ui->tablewidget_isletme_defteri->setItem ( current_row, ROW_ID_COLUMN, new_item );

        new_item = new QTableWidgetItem ( tr( "%1" ).arg ( fis_id ) );
        m_ui->tablewidget_isletme_defteri->setItem(current_row, FIS_ID_COLUMN, new_item);

        new_item = new QTableWidgetItem ( tr ( "%1" ).arg ( order_number ) );
        m_ui->tablewidget_isletme_defteri->setItem ( current_row, ORDER_COLUMN, new_item );

        QLineEdit * aciklama_lineEdit = ( QLineEdit * ) m_ui->tablewidget_isletme_defteri->cellWidget ( current_row, ACIKLAMA_COLUMN );
        aciklama_lineEdit->setText ( aciklama );

        QLineEdit * belge_no_lineEdit = ( QLineEdit * ) m_ui->tablewidget_isletme_defteri->cellWidget ( current_row, BELGE_NO_COLUMN );
        belge_no_lineEdit->setText ( belge_no );

        QCommaEdit * comma_edit = ( QCommaEdit * ) m_ui->tablewidget_isletme_defteri->cellWidget ( current_row, KDV_HARIC_TUTAR_COLUMN );
        comma_edit->SET_DOUBLE ( satir_kdv_haric_tutar );

        comma_edit = ( QCommaEdit * ) m_ui->tablewidget_isletme_defteri->cellWidget ( current_row, KDV_TUTARI_COLUMN );
        comma_edit->SET_DOUBLE ( satir_kdv_tutari );

        comma_edit = ( QCommaEdit * ) m_ui->tablewidget_isletme_defteri->cellWidget (current_row, KDV_DAHIL_TUTAR_COLUMN );
        comma_edit->SET_DOUBLE ( satir_kdv_dahil_tutar );

        QComboBox * combobox = ( QComboBox * ) m_ui->tablewidget_isletme_defteri->cellWidget ( current_row, GIDER_TURU_COLUMN );

        QString islem_turu_str = ISL_GET_GIDER_STRING(gider_turu);
        if (m_fis_turu EQ ENUM_GELIR_FISI ) {
            islem_turu_str = ISL_GET_GELIR_STRING(gider_turu);
        }
        FIND_AND_SET_COMBOBOX_TEXT( combobox, islem_turu_str );

        combobox =  ( QComboBox * ) m_ui->tablewidget_isletme_defteri->cellWidget ( current_row, KDV_ORANI_COLUMN );
        combobox->setCurrentIndex ( combobox->findText ( QVariant ( kdv_orani ).toString(), Qt::MatchExactly ) );
    }
    return ADAK_OK;
}

/**************************************************************************************
                   ISL_DEFTERI_FISI::SET_LINE_DEFAULTS
***************************************************************************************/

void ISL_DEFTERI_FISI::SET_LINE_DEFAULTS ( int row_number )
{
    QComboBox  * comboBox_gider_turu = ( QComboBox  * ) m_ui->tablewidget_isletme_defteri->cellWidget ( row_number, GIDER_TURU_COLUMN );
    QComboBox  * comboBox_kdv_orani  = ( QComboBox  * ) m_ui->tablewidget_isletme_defteri->cellWidget ( row_number, KDV_ORANI_COLUMN  );
    QCommaEdit * commaedit           = ( QCommaEdit * ) m_ui->tablewidget_isletme_defteri->cellWidget ( row_number, KDV_TUTARI_COLUMN );
    QLineEdit  * lineEdit_aciklama   = ( QLineEdit  * ) m_ui->tablewidget_isletme_defteri->cellWidget ( row_number, ACIKLAMA_COLUMN   );
    QLineEdit  * lineEdit_belge_no   = ( QLineEdit  * ) m_ui->tablewidget_isletme_defteri->cellWidget ( row_number, BELGE_NO_COLUMN   );

    comboBox_gider_turu->setEditable ( false );

    FILL_ISL_GIDERLERI(comboBox_gider_turu);

    if ( m_fis_turu EQ ENUM_GELIR_FISI ) {
        FILL_ISL_GELIRLERI( comboBox_gider_turu);
    }

    E9_FILL_KDV_ORANLARI_COMBOBOX ( comboBox_kdv_orani );

    comboBox_kdv_orani->setCurrentIndex(comboBox_kdv_orani->findText(QVariant(m_ontanimli_kdv_orani).toString()));

    commaedit->setReadOnly ( true );

    lineEdit_aciklama->setMaxLength ( 128 );
    lineEdit_belge_no->setMaxLength(8);
}

/**************************************************************************************
                   ISL_DEFTERI_FISI::CHECK_FIS_FORM_VAR
***************************************************************************************/

int ISL_DEFTERI_FISI::CHECK_FIS_FORM_VAR ( QObject * object )
{
    if ( object EQ m_ui->line_edit_fis_no ) {
        if ( m_ui->line_edit_fis_no->text() EQ "*" ) {
            return ADAK_OK;
        }
    }

    return ADAK_OK;
}

/**************************************************************************************
                   ISL_DEFTERI_FISI::CHECK_FIS_FORM_EMPTY
***************************************************************************************/

int ISL_DEFTERI_FISI::CHECK_FIS_FORM_EMPTY ()
{
    if ( m_ui->line_edit_fis_no->text().isEmpty() EQ true ) {
         MSG_WARNING( tr ( "Fiş numarası boş bırakılamaz, Yeni fiş kaydı için (*) girmelisiniz!.." ), m_ui->line_edit_fis_no );

        return ADAK_FAIL;
    }
    return ADAK_OK;
}

/**************************************************************************************
                   ISL_DEFTERI_FISI::CHECK_FIS_RECORD_ADD
***************************************************************************************/

int ISL_DEFTERI_FISI::CHECK_FIS_RECORD_ADD ()
{
    if ( QString ( m_ui->line_edit_fis_no->text().at(0) ) NE "*" ) {

        int fis_no = m_ui->line_edit_fis_no->text().toInt();

        SQL_QUERY query ( DB );

        query.PREPARE_SELECT ( "isl_fisi","fis_id","fis_no = :fis_no "
                               "AND fis_tarihi = :fis_tarihi" );

        query.SET_VALUE ( ":fis_no" ,fis_no );
        query.SET_VALUE ( ":fis_tarihi"   ,m_ui->adakDate_fis_tarihi->DATE() );

        if ( query.SELECT() > 0 ) {
            MSG_WARNING( tr ("Aynı numarada başka bir fiş var.Fiş eklenemedi!.." ), m_ui->line_edit_fis_no );

            return ADAK_FAIL;
        }
    }
    return ADAK_OK;
}

/**************************************************************************************
                   ISL_DEFTERI_FISI::ADD_FIS_RECORD
***************************************************************************************/

int ISL_DEFTERI_FISI::ADD_FIS_RECORD ()
{
    int fis_no = 0;
    if  (  QString  (  m_ui->line_edit_fis_no->text().at(0) ) EQ "*" ) {
        fis_no = ISL_SIRADAKI_FIS_NO_AL (  m_ui->adakDate_fis_tarihi->DATE() );
    }
    else {
        fis_no = m_ui->line_edit_fis_no->text().toInt();
    }

    M_ISL_FISI->fis_no              = fis_no;
    M_ISL_FISI->fis_tarihi          = m_ui->adakDate_fis_tarihi->DATE();
    M_ISL_FISI->aciklama            = m_ui->limitedTextEdit->toPlainText();
    M_ISL_FISI->modul_id            = ISLETME_MODULU;
    M_ISL_FISI->program_id          = E9_PROGRAMI;
    M_ISL_FISI->fis_turu            = m_fis_turu;

    int fis_id = ISL_FISI_EKLE(M_ISL_FISI);

    m_ui->line_edit_fis_no->setText ( QVariant ( fis_no ).toString() );

    m_log_detaylari = QObject::tr("İşletme Fişi , ") + ISL_GET_FIS_TURU_STRING(m_fis_turu) + "\n";
    m_log_detaylari.append ( "Fiş Tarihi : " ) + m_ui->adakDate_fis_tarihi->QDATE().toString("dd MMMM YYYY");
    m_log_detaylari.append ( ", Fiş No : " ) + QVariant(fis_no).toString();

    m_log_kaydi_eklensin_mi = true;
    m_kayit_eklendi         = true;

    return fis_id;
}

/**************************************************************************************
                   ISL_DEFTERI_FISI::CHECK_FIS_RECORD_UPDATE
***************************************************************************************/

int ISL_DEFTERI_FISI::CHECK_FIS_RECORD_UPDATE ( int fis_id )
{
    SQL_QUERY sql_query ( DB );
    if ( QString ( m_ui->line_edit_fis_no->text().at(0) ) EQ "*" ) {
         MSG_WARNING(  tr ( "Fiş numarasını güncellemek için fiş numarası kısmına sadece sayı girebilirsiniz." ), m_ui->line_edit_fis_no );

        return ADAK_FAIL;
    }

    int fis_no = m_ui->line_edit_fis_no->text().toInt();

    sql_query.PREPARE_SELECT ( "isl_fisi ","fis_id","fis_no = :fis_no "
                               "AND fis_tarihi = :fis_tarihi" );

    sql_query.SET_VALUE ( ":fis_no" ,fis_no );
    sql_query.SET_VALUE ( ":fis_tarihi", m_ui->adakDate_fis_tarihi->DATE());

    if ( sql_query.SELECT() NE 0 ) {

        sql_query.NEXT();

        if ( sql_query.VALUE(0).toInt() NE fis_id ) {
            MSG_WARNING( tr (  "Aynı numarada başka bir fiş var. Lütfen fiş numarasını değiştiriniz." ), m_ui->line_edit_fis_no );

            return ADAK_FAIL;
        }
    }

    return ADAK_OK;
}

/**************************************************************************************
                   ISL_DEFTERI_FISI::UPDATE_FIS_RECORD
***************************************************************************************/

void ISL_DEFTERI_FISI::UPDATE_FIS_RECORD ( int fis_id )
{

    M_ISL_FISI->fis_no     = m_ui->line_edit_fis_no->text().toInt();
    M_ISL_FISI->fis_tarihi = m_ui->adakDate_fis_tarihi->DATE();
    M_ISL_FISI->aciklama   = m_ui->limitedTextEdit->toPlainText();

    ISL_FISI_GUNCELLE  ( M_ISL_FISI , fis_id);

    m_log_kaydi_eklensin_mi = true;

    m_log_detaylari = QObject::tr("İşletme Fişi , ") + ISL_GET_FIS_TURU_STRING(m_fis_turu) + "\n";
    m_log_detaylari.append ( "Fiş Tarihi : "  + m_ui->adakDate_fis_tarihi->QDATE().toString("dd MMMM YYYY"));
    m_log_detaylari.append ( ", Fiş No : "  + m_ui->line_edit_fis_no->text() + "\n");
    m_log_detaylari.append ( "KDV Hariç Toplam : "  +
    m_ui->CommaEdit_kdv_haric_tutar->GET_TEXT() + ",");
    m_log_detaylari.append ( "KDV Toplam : " +
    m_ui->CommaEdit_kdv_tutari->GET_TEXT() + "," );
    m_log_detaylari.append ( "KDV Dahil Toplam : " + m_ui->CommaEdit_toplam_tutar->GET_TEXT() + "," );

}

/**************************************************************************************
                   ISL_DEFTERI_FISI::CHECK_FIS_RECORD_DELETE
***************************************************************************************/

int ISL_DEFTERI_FISI::CHECK_FIS_RECORD_DELETE ( int fis_id )
{
    Q_UNUSED ( fis_id );

    return ADAK_OK;
}

/**************************************************************************************
                   ISL_DEFTERI_FISI::DELETE_FIS_RECORD
***************************************************************************************/

void ISL_DEFTERI_FISI::DELETE_FIS_RECORD ( int fis_id )
{
    ISL_FISI_SIL(M_ISL_FISI , fis_id);

    m_log_kaydi_eklensin_mi = true;
    m_kayit_silindi         = true;

    m_log_detaylari = QObject::tr("İşletme Fişi , ") + ISL_GET_FIS_TURU_STRING(m_fis_turu) + "\n";
    m_log_detaylari.append ( "Fiş Tarihi : "  + QDate::fromString(M_ISL_FISI->fis_tarihi , "yyyy.MM.dd").toString("dd MMMM yyyy dddd"));
    m_log_detaylari.append ( ", Fiş No : "  + QVariant(M_ISL_FISI->fis_no).toString() + "\n");
    m_log_detaylari.append ( "KDV Hariç Toplam : "  +
    VIRGUL_EKLE ( QVariant( M_ISL_FISI->kdv_haric_toplam).toString() ) + ",");
    m_log_detaylari.append ( "KDV Toplam : " +
            VIRGUL_EKLE ( QVariant( M_ISL_FISI->kdv_toplam).toString()) + ",");
    m_log_detaylari.append ( "KDV Dahil Toplam : " + VIRGUL_EKLE ( QVariant( M_ISL_FISI->kdv_dahil_toplam).toString())
                             + ",");

}

/**************************************************************************************
                   ISL_DEFTERI_FISI::SELECT_FIS_RECORD
***************************************************************************************/

int ISL_DEFTERI_FISI::SELECT_FIS_RECORD ()
{
    return OPEN_ISL_FIS_ARAMA ( this );
}

/**************************************************************************************
                   ISL_DEFTERI_FISI::FIND_FIS_RECORD
***************************************************************************************/

int ISL_DEFTERI_FISI::FIND_FIS_RECORD ()
{
    SQL_QUERY sql_query ( DB );
    sql_query.PREPARE_SELECT ( "isl_fisi","fis_id","fis_no = :fis_no "
                               "AND fis_turu = :fis_turu AND fis_tarihi = :fis_tarihi" );

    sql_query.SET_VALUE      ( ":fis_no"       ,  m_ui->line_edit_fis_no->text().toInt() );
    sql_query.SET_VALUE      ( ":fis_turu"     ,  m_fis_turu );
    sql_query.SET_VALUE      ( ":fis_tarihi"   ,  m_ui->adakDate_fis_tarihi->DATE() );

    if ( sql_query.SELECT() EQ 0 ) {
        MSG_WARNING(  tr ( "Fiş bulunamadı" ),m_ui->line_edit_fis_no );

        return 0;
    }

    sql_query.NEXT();

    return sql_query.VALUE(0).toInt();
}

/**************************************************************************************
                   ISL_DEFTERI_FISI::FIND_FIRST_FIS_RECORD
***************************************************************************************/

int ISL_DEFTERI_FISI::FIND_FIRST_FIS_RECORD ()
{
    SQL_QUERY sql_query  (  DB  );

    sql_query.PREPARE_SELECT ( "isl_fisi","fis_id ","fis_turu = :fis_turu " );
    sql_query.SET_VALUE ( ":fis_turu"   , m_fis_turu );

    if ( sql_query.SELECT("fis_tarihi ASC, fis_no ASC ",0,1) EQ 0 ) {
        return 0;
    }

    sql_query.NEXT();

    return sql_query.VALUE(0).toInt();
}

/**************************************************************************************
                   ISL_DEFTERI_FISI::FIND_LAST_FIS_RECORD
***************************************************************************************/

int ISL_DEFTERI_FISI::FIND_LAST_FIS_RECORD ()
{
    SQL_QUERY sql_query  ( DB );

    sql_query.PREPARE_SELECT ( "isl_fisi", "fis_id", "fis_turu = :fis_turu ");
    sql_query.SET_VALUE      ( ":fis_turu",   m_fis_turu );

    if ( sql_query.SELECT("fis_tarihi DESC, fis_no DESC",0,1) EQ 0 ) {
        return 0;
    }
    sql_query.NEXT();
    return sql_query.VALUE(0).toInt();
}

/**************************************************************************************
                   ISL_DEFTERI_FISI::FIND_PREV_FIS_RECORD
***************************************************************************************/

int ISL_DEFTERI_FISI::FIND_PREV_FIS_RECORD ()
{
    SQL_QUERY sql_query ( DB );

    sql_query.PREPARE_SELECT ( "isl_fisi","fis_id ",
                                "fis_no < :fis_no "
                                "AND fis_turu       = :fis_turu "
                                "AND fis_tarihi     = :fis_tarihi" );

    sql_query.SET_VALUE           ( ":fis_no", m_ui->line_edit_fis_no->text().toInt() );
    sql_query.SET_VALUE           ( ":fis_turu"    , m_fis_turu );
    sql_query.SET_VALUE           ( ":fis_tarihi"  , m_ui->adakDate_fis_tarihi->DATE() );

    if ( sql_query.SELECT("fis_no DESC",0,1) > 0 ) {

        sql_query.NEXT();
        return sql_query.VALUE(0).toInt();
    }

    sql_query.PREPARE_SELECT ( "isl_fisi","fis_id ",
                               "fis_turu   = :fis_turu "
                               "AND  fis_tarihi  < :fis_tarihi");

    sql_query.SET_VALUE           ( ":fis_turu"    , m_fis_turu );
    sql_query.SET_VALUE           ( ":fis_tarihi"  , m_ui->adakDate_fis_tarihi->DATE()  );

    if ( sql_query.SELECT("fis_tarihi DESC,fis_no DESC",0,1) EQ 0 ) {
        return 0;
    }

    sql_query.NEXT();

    return sql_query.VALUE(0).toInt();

}

/**************************************************************************************
                   ISL_DEFTERI_FISI::FIND_NEXT_FIS_RECORD
***************************************************************************************/

int ISL_DEFTERI_FISI::FIND_NEXT_FIS_RECORD ()
{
    SQL_QUERY sql_query ( DB );

    sql_query.PREPARE_SELECT ( "isl_fisi","fis_id",
                               "fis_no > :fis_no "
                               "AND fis_turu       = :fis_turu "
                               "AND fis_tarihi     = :fis_tarihi");

    sql_query.SET_VALUE           ( ":fis_no", m_ui->line_edit_fis_no->text().toInt() );
    sql_query.SET_VALUE           ( ":fis_turu"    , m_fis_turu );
    sql_query.SET_VALUE           ( ":fis_tarihi"  , m_ui->adakDate_fis_tarihi->DATE() );

    if ( sql_query.SELECT("fis_no ASC",0,1) > 0 ) {

        sql_query.NEXT();

        return sql_query.VALUE(0).toInt();

    }

    sql_query.PREPARE_SELECT ( "isl_fisi","fis_id ",
                                "fis_turu   = :fis_turu "
                                "AND  fis_tarihi  > :fis_tarihi" );

    sql_query.SET_VALUE           ( ":fis_turu"    , m_fis_turu );
    sql_query.SET_VALUE           ( ":fis_tarihi"  , m_ui->adakDate_fis_tarihi->DATE() );

    if ( sql_query.SELECT("fis_tarihi ASC,fis_no ASC",0,1) EQ 0 ) {
        return 0;
    }

    sql_query.NEXT();

    return sql_query.VALUE(0).toInt();
}

/**************************************************************************************
                   ISL_DEFTERI_FISI::CHECK_LINE_VAR
***************************************************************************************/

int ISL_DEFTERI_FISI::CHECK_LINE_VAR ( int row_number, QObject * object )
{
    QLineEdit  * lineEdit_belge_no         = ( QLineEdit  * ) m_ui->tablewidget_isletme_defteri->cellWidget ( row_number, BELGE_NO_COLUMN );
    QCommaEdit * commaEdit_kdv_haric_tutar = ( QCommaEdit * ) m_ui->tablewidget_isletme_defteri->cellWidget ( row_number, KDV_HARIC_TUTAR_COLUMN );
    QCommaEdit * commaEdit_kdv_tutari      = ( QCommaEdit * ) m_ui->tablewidget_isletme_defteri->cellWidget ( row_number, KDV_TUTARI_COLUMN );
    QCommaEdit * commaEdit_kdv_dahil_tutar = ( QCommaEdit * ) m_ui->tablewidget_isletme_defteri->cellWidget ( row_number, KDV_DAHIL_TUTAR_COLUMN );
    QComboBox  * combobox_kdv_orani        = ( QComboBox  * ) m_ui->tablewidget_isletme_defteri->cellWidget ( row_number, KDV_ORANI_COLUMN );

    if ( object EQ lineEdit_belge_no ) {
        if ( lineEdit_belge_no->text().size() > 8 ) {
             MSG_WARNING( tr (  "Belge numarası sekiz karakterden daha uzun olamaz" ), lineEdit_belge_no );

            return ADAK_FAIL;
        }
    }
    else if ( ( object EQ combobox_kdv_orani ) OR ( object EQ commaEdit_kdv_dahil_tutar )
        OR ( object EQ commaEdit_kdv_haric_tutar ) ) {

        double kdv_dahil_tutar = 0.00;
        double kdv_haric_tutar = 0.00;
        double kdv_tutari      = 0.00;

        double kdv_orani       = combobox_kdv_orani->currentText().toDouble() / 100;

        if ( object EQ commaEdit_kdv_dahil_tutar ) {
            kdv_dahil_tutar = commaEdit_kdv_dahil_tutar->GET_DOUBLE();
            kdv_haric_tutar = kdv_dahil_tutar / (  1 + kdv_orani  );
        }
        else {

            kdv_haric_tutar = commaEdit_kdv_haric_tutar->GET_DOUBLE();
            kdv_dahil_tutar = ROUND ( kdv_haric_tutar * ( 1 + kdv_orani ) );
        }

        kdv_tutari          = ROUND ( kdv_haric_tutar * kdv_orani );

        commaEdit_kdv_dahil_tutar->SET_DOUBLE (  kdv_dahil_tutar );
        commaEdit_kdv_haric_tutar->SET_DOUBLE (  kdv_haric_tutar );
        commaEdit_kdv_tutari->SET_DOUBLE      (  kdv_tutari      );
    }

    return ADAK_OK;
}

/**************************************************************************************
                   ISL_DEFTERI_FISI::CHECK_LINE_EMPTY
***************************************************************************************/

int ISL_DEFTERI_FISI::CHECK_LINE_EMPTY ( int row_number )
{
    QCommaEdit * commaEdit = ( QCommaEdit * ) m_ui->tablewidget_isletme_defteri->cellWidget ( row_number, KDV_HARIC_TUTAR_COLUMN);
    QLineEdit  * lineEdit  = ( QLineEdit * )  m_ui->tablewidget_isletme_defteri->cellWidget ( row_number, BELGE_NO_COLUMN );

    if ( commaEdit->GET_DOUBLE() EQ 0.0 ) {
        MSG_WARNING(  tr ( " KDV hariç tutarı girmelisiniz" ), commaEdit );

        return ADAK_FAIL;
    }
    if ( lineEdit->text().isEmpty() EQ true ) {
        MSG_WARNING(  tr ( " Belge numarası boş bırakılamaz" ), lineEdit );

        return ADAK_FAIL;
    }
    return ADAK_OK;
}


/**************************************************************************************
                   ISL_DEFTERI_FISI::CHECK_ADD_LINE
***************************************************************************************/

int ISL_DEFTERI_FISI::CHECK_ADD_LINE ( int fis_id, int row_number )
{
    Q_UNUSED ( fis_id );
    Q_UNUSED ( row_number );
    return ADAK_OK;
}

/**************************************************************************************
                   ISL_DEFTERI_FISI::ADD_LINE
***************************************************************************************/

void ISL_DEFTERI_FISI::ADD_LINE ( int fis_id, int row_number )
{

    QString belge_no                 = ((QLineEdit  * ) m_ui->tablewidget_isletme_defteri->cellWidget( row_number, BELGE_NO_COLUMN  ) )->text();
    QString aciklama                 = ((QLineEdit  * ) m_ui->tablewidget_isletme_defteri->cellWidget( row_number, ACIKLAMA_COLUMN  ) )->text();
    double  kdv_orani                = ((QComboBox  * ) m_ui->tablewidget_isletme_defteri->cellWidget( row_number, KDV_ORANI_COLUMN ) )->currentText().toDouble();
    double  satir_kdv_haric_tutar    = ((QCommaEdit * ) m_ui->tablewidget_isletme_defteri->cellWidget( row_number, KDV_HARIC_TUTAR_COLUMN ) )->GET_DOUBLE();
    double  satir_kdv_tutari         = ((QCommaEdit * ) m_ui->tablewidget_isletme_defteri->cellWidget( row_number, KDV_TUTARI_COLUMN ) )->GET_DOUBLE();
    double  satir_kdv_dahil_tutar    = ((QCommaEdit * ) m_ui->tablewidget_isletme_defteri->cellWidget( row_number, KDV_DAHIL_TUTAR_COLUMN ) )->GET_DOUBLE();

    QComboBox * combobox_gider_turu  = ((QComboBox * ) m_ui->tablewidget_isletme_defteri->cellWidget ( row_number, GIDER_TURU_COLUMN ));

    M_ISL_FIS_SATIRI->gider_turu = ISL_GET_GIDER_ENUM(combobox_gider_turu->currentText());

    if ( m_fis_turu EQ ENUM_GELIR_FISI ) {
        M_ISL_FIS_SATIRI->gider_turu = ISL_GET_GELIR_ENUM(combobox_gider_turu->currentText());
    }

    M_ISL_FIS_SATIRI->fis_id                = fis_id;
    M_ISL_FIS_SATIRI->order_number          = 0;
    M_ISL_FIS_SATIRI->belge_no              = belge_no;
    M_ISL_FIS_SATIRI->aciklama              = aciklama;
    M_ISL_FIS_SATIRI->kdv_orani             = kdv_orani;
    M_ISL_FIS_SATIRI->satir_kdv_haric_tutar = satir_kdv_haric_tutar;
    M_ISL_FIS_SATIRI->satir_kdv_tutari      = satir_kdv_tutari;
    M_ISL_FIS_SATIRI->satir_kdv_dahil_tutar = satir_kdv_dahil_tutar;
    M_ISL_FIS_SATIRI->modul_id              = M_ISL_FISI->modul_id;
    M_ISL_FIS_SATIRI->program_id            = M_ISL_FISI->program_id;

    int satir_id = ISL_FIS_SATIRI_EKLE( M_ISL_FISI , M_ISL_FIS_SATIRI);

    QTableWidgetItem * new_item = new QTableWidgetItem ( QVariant ( satir_id ).toString() );
    m_ui->tablewidget_isletme_defteri->setItem ( row_number, ROW_ID_COLUMN, new_item );



    m_ui->CommaEdit_kdv_haric_tutar->SET_DOUBLE ( M_ISL_FISI->kdv_haric_toplam );
    m_ui->CommaEdit_kdv_tutari->SET_DOUBLE      ( M_ISL_FISI->kdv_toplam      );
    m_ui->CommaEdit_toplam_tutar->SET_DOUBLE    ( M_ISL_FISI->kdv_dahil_toplam   );



    m_log_detaylari = QObject::tr("İşletme Fişi , ") + ISL_GET_FIS_TURU_STRING(m_fis_turu) + "\n";
    m_log_detaylari.append ( "Fiş Tarihi : "  + QDate::fromString(M_ISL_FISI->fis_tarihi,"yyyy.MM.dd").toString("dd MMMM yyyy dddd"));
    m_log_detaylari.append ( ", Fiş No : "  + QVariant(M_ISL_FISI->fis_no).toString() + "\n");
    m_log_detaylari.append ( "KDV Hariç Toplam : "  +
    VIRGUL_EKLE ( QVariant( M_ISL_FISI->kdv_haric_toplam ).toString())
    + ",");
    m_log_detaylari.append ( "KDV Toplam : " +
            VIRGUL_EKLE ( QVariant( M_ISL_FISI->kdv_toplam ).toString())
            + ",");
    m_log_detaylari.append ( "KDV Dahil Toplam : " + VIRGUL_EKLE ( QVariant( M_ISL_FISI->kdv_dahil_toplam).toString())
                             + ",");
    m_log_kaydi_eklensin_mi = true;

}

/**************************************************************************************
                   ISL_DEFTERI_FISI::CHECK_UPDATE_LINE
***************************************************************************************/

int ISL_DEFTERI_FISI::CHECK_UPDATE_LINE ( int fis_id, int row_number )
{
    Q_UNUSED ( fis_id );
    Q_UNUSED ( row_number );
    return ADAK_OK;
}

/**************************************************************************************
                   ISL_DEFTERI_FISI::UPDATE_LINE
***************************************************************************************/

void ISL_DEFTERI_FISI::UPDATE_LINE ( int fis_id, int row_number )
{
    int     satir_id                      = m_ui->tablewidget_isletme_defteri->item ( row_number, ROW_ID_COLUMN )->text().toInt();
    QString belge_no                      = ( (QLineEdit  *) m_ui->tablewidget_isletme_defteri->cellWidget ( row_number, BELGE_NO_COLUMN ) )->text();
    QString aciklama                      = ( (QLineEdit  *) m_ui->tablewidget_isletme_defteri->cellWidget ( row_number, ACIKLAMA_COLUMN  ) )->text();
    double  kdv_orani                     = ( (QComboBox  *) m_ui->tablewidget_isletme_defteri->cellWidget ( row_number, KDV_ORANI_COLUMN ) )->currentText().toDouble();
    double  current_satir_kdv_haric_tutar = ( (QCommaEdit *) m_ui->tablewidget_isletme_defteri->cellWidget ( row_number, KDV_HARIC_TUTAR_COLUMN ) )->GET_DOUBLE();
    double  current_satir_kdv_tutari      = ( (QCommaEdit *) m_ui->tablewidget_isletme_defteri->cellWidget ( row_number, KDV_TUTARI_COLUMN ) )->GET_DOUBLE();
    double  current_satir_kdv_dahil_tutar = ( (QCommaEdit *) m_ui->tablewidget_isletme_defteri->cellWidget ( row_number, KDV_DAHIL_TUTAR_COLUMN) )->GET_DOUBLE();

    QComboBox * combobox_gider_turu  = ((QComboBox * ) m_ui->tablewidget_isletme_defteri->cellWidget ( row_number, GIDER_TURU_COLUMN ));

    M_ISL_FIS_SATIRI->gider_turu = ISL_GET_GIDER_ENUM(combobox_gider_turu->currentText());

    if ( m_fis_turu EQ ENUM_GELIR_FISI ) {
        M_ISL_FIS_SATIRI->gider_turu = ISL_GET_GELIR_ENUM(combobox_gider_turu->currentText());
    }

    M_ISL_FIS_SATIRI->fis_id                = fis_id;
    M_ISL_FIS_SATIRI->belge_no              = belge_no;
    M_ISL_FIS_SATIRI->aciklama              = aciklama;
    M_ISL_FIS_SATIRI->kdv_orani             = kdv_orani;
    M_ISL_FIS_SATIRI->satir_kdv_haric_tutar = current_satir_kdv_haric_tutar;
    M_ISL_FIS_SATIRI->satir_kdv_tutari      = current_satir_kdv_tutari;
    M_ISL_FIS_SATIRI->satir_kdv_dahil_tutar = current_satir_kdv_dahil_tutar;

    ISL_FIS_SATIRI_GUNCELLE( M_ISL_FISI , M_ISL_FIS_SATIRI , satir_id);

    m_ui->CommaEdit_kdv_haric_tutar->SET_DOUBLE ( M_ISL_FISI->kdv_haric_toplam);
    m_ui->CommaEdit_kdv_tutari->SET_DOUBLE      ( M_ISL_FISI->kdv_toplam );
    m_ui->CommaEdit_toplam_tutar->SET_DOUBLE    ( M_ISL_FISI->kdv_dahil_toplam );



    m_log_detaylari = QObject::tr("İşletme Fişi , ") + ISL_GET_FIS_TURU_STRING(m_fis_turu) + "\n";
    m_log_detaylari.append ( "Fiş Tarihi : "  + QDate::fromString(M_ISL_FISI->fis_tarihi,"yyyy.MM.dd").toString("dd MMMM yyyy dddd"));
    m_log_detaylari.append ( ", Fiş No : "  + QVariant(M_ISL_FISI->fis_no).toString() + "\n");
    m_log_detaylari.append ( "KDV Hariç Toplam : "  +
    VIRGUL_EKLE ( QVariant( M_ISL_FISI->kdv_haric_toplam).toString())
    + ",");
    m_log_detaylari.append ( "KDV Toplam : " +
            VIRGUL_EKLE ( QVariant( M_ISL_FISI->kdv_toplam).toString())
            + ",");
    m_log_detaylari.append ( "KDV Dahil Toplam : " + VIRGUL_EKLE ( QVariant( M_ISL_FISI->kdv_dahil_toplam).toString())
                             + ",");
    m_log_kaydi_eklensin_mi = true;

}

/**************************************************************************************
                   ISL_DEFTERI_FISI::CHECK_DELETE_LINE
***************************************************************************************/

int ISL_DEFTERI_FISI::CHECK_DELETE_LINE ( int fis_id, int row_number )
{
    Q_UNUSED ( fis_id );
    Q_UNUSED ( row_number );
    return ADAK_OK;
}

/**************************************************************************************
                   ISL_DEFTERI_FISI::DELETE_LINE
***************************************************************************************/

void ISL_DEFTERI_FISI::DELETE_LINE ( int fis_id, int row_number )

{
    Q_UNUSED(fis_id);

    int satir_id  = m_ui->tablewidget_isletme_defteri->item ( row_number, ROW_ID_COLUMN )->text().toInt();

    ISL_FIS_SATIRI_SIL(M_ISL_FISI , satir_id);

    m_ui->CommaEdit_kdv_haric_tutar->SET_DOUBLE ( M_ISL_FISI->kdv_haric_toplam );
    m_ui->CommaEdit_kdv_tutari->SET_DOUBLE      ( M_ISL_FISI->kdv_toplam   );
    m_ui->CommaEdit_toplam_tutar->SET_DOUBLE    ( M_ISL_FISI->kdv_dahil_toplam   );


    m_log_detaylari = QObject::tr("İşletme Fişi , ") + ISL_GET_FIS_TURU_STRING(m_fis_turu) + "\n";
    m_log_detaylari.append ( "Fiş Tarihi : "  + QDate::fromString(M_ISL_FISI->fis_tarihi,"yyyy.MM.dd").toString("dd MMMM yyyy dddd"));
    m_log_detaylari.append ( ", Fiş No : "  + QVariant(M_ISL_FISI->fis_no).toString() + "\n");
    m_log_detaylari.append ( "KDV Hariç Toplam : "  +
    VIRGUL_EKLE ( QVariant( M_ISL_FISI->kdv_haric_toplam).toString())
    + ",");
    m_log_detaylari.append ( "KDV Toplam : " +
            VIRGUL_EKLE ( QVariant( M_ISL_FISI->kdv_toplam).toString())
            + ",");
    m_log_detaylari.append ( "KDV Dahil Toplam : " + VIRGUL_EKLE ( QVariant( M_ISL_FISI->kdv_dahil_toplam ).toString())
                             + ",");
    m_log_kaydi_eklensin_mi = true;
}

/**************************************************************************************
                   ISL_DEFTERI_FISI::LOCK_FIS_RECORD
***************************************************************************************/

int ISL_DEFTERI_FISI::LOCK_FIS_RECORD ( int fis_id )
{
    return DB->LOCK_ROW ( "fis_id","isl_fisi", QString ( "fis_id = %1" ).arg ( fis_id ) );
}

/**************************************************************************************
                   ISL_DEFTERI_FISI::UNLOCK_FIS_RECORD
***************************************************************************************/

void ISL_DEFTERI_FISI::UNLOCK_FIS_RECORD ( int fis_id )
{
    DB->UNLOCK_ROW ( "fis_id","isl_fisi", QString ( "fis_id = %1" ).arg ( fis_id ) );
}

/**************************************************************************************
                   ISL_DEFTERI_FISI::LEAVE_RECORD()
***************************************************************************************/

int ISL_DEFTERI_FISI::LEAVE_RECORD()
{
    if (m_log_kaydi_eklensin_mi EQ true ) {
        DB->START_TRANSACTION();
        if(m_kayit_eklendi EQ true) {
            E9_LOG_KAYDI_EKLE ( ISLETME_MODULU , LOG_ISL_FISLER , LOG_ISLEM_ADD , m_log_detaylari);
        }
        if(m_kayit_silindi EQ true ) {
            E9_LOG_KAYDI_EKLE ( ISLETME_MODULU , LOG_ISL_FISLER , LOG_ISLEM_DELETE , m_log_detaylari);
        }
        if(m_kayit_eklendi EQ false AND m_kayit_silindi EQ false ) {
            E9_LOG_KAYDI_EKLE ( ISLETME_MODULU , LOG_ISL_FISLER , LOG_ISLEM_UPDATE , m_log_detaylari);
        }
        DB->COMMIT_TRANSACTION();
    }
    return ADAK_OK;
}


/**************************************************************************************
                   ISL_DEFTERI_FISI::SAVER_BUTTON_CLICKED
***************************************************************************************/

void ISL_DEFTERI_FISI::SAVER_BUTTON_CLICKED ( QAbstractButton * p_button, int p_record_id )
{
    if ( p_button EQ m_ui->Button_yazdir ) {
        OPEN_REPORT_SHOWER ( GET_ISL_FISI_RAPORU ( p_record_id, m_fis_turu ), nativeParentWidget() );
    }
    else if ( p_button EQ m_ui->button_fis_kopyala ) {
        OPEN_ISL_FIS_KOPYALAMA_BATCH ( p_record_id, this );
    }
}

/**************************************************************************************
                   ISL_DEFTERI_FISI::FIS_EKRANINI_DUZENLE
***************************************************************************************/

void ISL_DEFTERI_FISI::FIS_EKRANINI_DUZENLE ()
{
    m_ui->tablewidget_isletme_defteri->setColumnWidth ( BELGE_NO_COLUMN               , 100  );
    m_ui->tablewidget_isletme_defteri->setColumnWidth ( ACIKLAMA_COLUMN               , 420  );
    m_ui->tablewidget_isletme_defteri->setColumnWidth ( GIDER_TURU_COLUMN             , 170  );
    m_ui->tablewidget_isletme_defteri->setColumnWidth ( KDV_ORANI_COLUMN              ,  75  );
    m_ui->tablewidget_isletme_defteri->setColumnWidth ( KDV_HARIC_TUTAR_COLUMN        , 150  );
    m_ui->tablewidget_isletme_defteri->setColumnWidth ( KDV_TUTARI_COLUMN             , 150  );
    m_ui->tablewidget_isletme_defteri->setColumnWidth ( KDV_DAHIL_TUTAR_COLUMN        , 125  );

    SET_SETTING_NAME ( "ISLETME-FISI" );
}

/**************************************************************************************
                   ISL_DEFTERI_FISI::CHANGER_BUTTON_CLICKED
***************************************************************************************/

int ISL_DEFTERI_FISI::CHANGER_BUTTON_CLICKED(QAbstractButton *button)
{
    Q_UNUSED(button);
    return ADAK_RECORD_UNCHANGED;
}

/**************************************************************************************
                   ISL_DEFTERI_FISI::LINE_CHANGER_BUTTON_CLICKED
***************************************************************************************/

int ISL_DEFTERI_FISI::LINE_CHANGER_BUTTON_CLICKED(QAbstractButton *button, int row_number)
{
    Q_UNUSED(button);
    Q_UNUSED(row_number);
    return ADAK_RECORD_UNCHANGED;
}









