#include "ui_dmr_amortisman_fisi.h"
#include "dmr_amortisman_fisi_class.h"
#include "dmr_con_utils.h"
#include "e9_console_utils.h"
#include "e9_gui_utils.h"
 
#include "e9_log.h"
#include "dmr_fis_utils.h"
#include "fat_console_utils.h"
#include "dmr_fis_arama_open.h"
#include "dmr_demirbas_karti_arama_open.h"
#include "fat_struct.h"

extern ADAK_SQL *              DB;


#define ROW_ID_COLUMN                     0
#define FIS_ID_COLUMN                     1
#define ORDER_COLUMN                      2
#define DEMIRBAS_ID_COLUMN                3
#define DEMIRBAS_KODU_COLUMN              4
#define DEMIRBAS_ADI_COLUMN               5
#define YENI_DEMIRBAS_DEGERI_COLUMN       6
#define YENI_BIR_AMORTISMAN_COLUMN        7
#define NET_DEGER_COLUMN                  8
#define AMORTISMAN_ORANI_COLUMN           9
#define AMORTISMAN_YONTEMI_COLUMN         10
#define ACIKLAMA_COLUMN                   11
#define AMORTISMAN_TUTARI_COLUMN          12



/**************************************************************************************
                   OPEN_DMR_AMORTISMAN_FISI
***************************************************************************************/

void OPEN_DMR_AMORTISMAN_FISI ( int record_id, QWidget * parent )
{
    DMR_AMORTISMAN_FISI * F = new DMR_AMORTISMAN_FISI ( record_id,parent );
    F->SHOW( FULL_SCREEN );
}

/**************************************************************************************
                   DMR_AMORTISMAN_FISI::DMR_AMORTISMAN_FISI
***************************************************************************************/

DMR_AMORTISMAN_FISI::DMR_AMORTISMAN_FISI ( int record_id, QWidget * parent ) :FIS_KERNEL ( parent ), m_ui ( new Ui::DMR_AMORTISMAN_FISI )
{
    m_start_record_id = record_id;

    m_ui->setupUi    ( this );
    START_FIS_KERNEL ( this, DB );
 }

/**************************************************************************************
                   DMR_AMORTISMAN_FISI::SETUP_FORM
***************************************************************************************/

void DMR_AMORTISMAN_FISI::SETUP_FORM()
{
    M_FIS        = new DMR_FIS_STRUCT;
    M_FIS_SATIRI = new DMR_FIS_SATIRI_STRUCT;

    SET_HELP_PAGE  ( "amortisman-fisi" );
    SET_PAGE_TITLE ( tr ( "DMR - AMORTİSMAN FİŞİ") );

    REGISTER_BUTTONS_WIDGET           ( m_ui->navigation_button_widget );

    QRegExp rx ( "(\\*?)(\\d+)" );
    QRegExpValidator * validator = new QRegExpValidator ( rx, this );
    m_ui->lineedit_fis_no->setValidator ( validator );
     

    m_ui->lineedit_fis_no->setMaxLength(10);

    m_ui->lineedit_fis_no->setText("*");
    DISABLE_CLEAR_ON_WIDGET(m_ui->lineedit_fis_no);

    REGISTER_TABLE_WIDGET             ( m_ui->tablewidget_fis_satirlari, 13);

    SET_FIS_ORDER_COLUMN_NUMBER       ( ORDER_COLUMN );
    SET_FORM_ID_COLUMN_POSITION       ( FIS_ID_COLUMN );
    SET_ROW_ID_COLUMN_POSITION        ( ROW_ID_COLUMN );
    SET_NUMBER_OF_HIDDEN_COLUMN       ( 4 );

    m_ui->limitedTextEdit_aciklama->SET_MAX_STRING_LENGTH(512);

    SET_FIS_ORDER_DATABASE ("dmr_fis_satirlari", "order_number", "fis_satiri_id" );

    SET_TABLE_ROW_WIDGETS ( DEMIRBAS_KODU_COLUMN           , WIDGET_SEARCH_EDIT );
    SET_TABLE_ROW_WIDGETS ( DEMIRBAS_ADI_COLUMN            , WIDGET_LINE_EDIT   );
    SET_TABLE_ROW_WIDGETS ( YENI_DEMIRBAS_DEGERI_COLUMN    , WIDGET_COMMA_EDIT  );
    SET_TABLE_ROW_WIDGETS ( YENI_BIR_AMORTISMAN_COLUMN     , WIDGET_COMMA_EDIT  );
    SET_TABLE_ROW_WIDGETS ( NET_DEGER_COLUMN               , WIDGET_COMMA_EDIT  );
    SET_TABLE_ROW_WIDGETS ( AMORTISMAN_ORANI_COLUMN        , WIDGET_COMMA_EDIT  );
    SET_TABLE_ROW_WIDGETS ( ACIKLAMA_COLUMN                , WIDGET_LINE_EDIT   );
    SET_TABLE_ROW_WIDGETS ( AMORTISMAN_TUTARI_COLUMN       , WIDGET_COMMA_EDIT  );
    SET_TABLE_ROW_WIDGETS ( AMORTISMAN_YONTEMI_COLUMN      , WIDGET_LINE_EDIT   );
    SET_TABLE_ROW_WIDGETS ( DEMIRBAS_ID_COLUMN             , WIDGET_LINE_EDIT    );

    SET_FIRST_FOCUS_WIDGET ( m_ui->lineedit_fis_no );

    m_ui->tablewidget_fis_satirlari->setHorizontalHeaderLabels ( QStringList()<<"row_id"<<"fis_id"<<"order_column"<<"demirbas id"
                                                                              <<tr ( "Sabit Kıymet Kodu")<<tr ("Sabit Kıymet Adı")
                                                                              <<tr ( "Yeni Değeri")<<tr ( "Birikmiş Amortisman\nYeni Değeri")
                                                                              <<tr ( "Net Değer")<< tr ( "Amortisman \nOranı")<<
                                                                              ( "Amortisman \nYöntemi")
                                                                              <<tr ( "Açıklama")<< tr ( "Amortisman Tutarı") <<tr("") << tr(""));

    m_ui->tablewidget_fis_satirlari->setColumnWidth ( DEMIRBAS_KODU_COLUMN           , 128 );
    m_ui->tablewidget_fis_satirlari->setColumnWidth ( DEMIRBAS_ADI_COLUMN            , 162 );
    m_ui->tablewidget_fis_satirlari->setColumnWidth ( YENI_DEMIRBAS_DEGERI_COLUMN    , 132 );
    m_ui->tablewidget_fis_satirlari->setColumnWidth ( YENI_BIR_AMORTISMAN_COLUMN     , 161 );
    m_ui->tablewidget_fis_satirlari->setColumnWidth ( NET_DEGER_COLUMN               , 128 );
    m_ui->tablewidget_fis_satirlari->setColumnWidth ( AMORTISMAN_ORANI_COLUMN        , 86  );
    m_ui->tablewidget_fis_satirlari->setColumnWidth ( AMORTISMAN_YONTEMI_COLUMN      , 104 );
    m_ui->tablewidget_fis_satirlari->setColumnWidth ( ACIKLAMA_COLUMN                , 138 );
    m_ui->tablewidget_fis_satirlari->setColumnWidth ( AMORTISMAN_TUTARI_COLUMN       , 128 );

    SET_SETTING_NAME                                ( "DMR_AMORT_FISI" );


    m_ui->tablewidget_fis_satirlari->setSelectionMode ( QAbstractItemView::NoSelection );
    m_ui->tablewidget_fis_satirlari->setFocus( Qt::OtherFocusReason );

    FOCUS_FIRST_WIDGET   ();
    AUTOSTART_FIS_KERNEL ( m_start_record_id );
}

/**************************************************************************************
                   DMR_AMORTISMAN_FISI::CLEAR_FORM_MEMBERS();
***************************************************************************************/

void DMR_AMORTISMAN_FISI::CLEAR_FORM_MEMBERS()
{
    DMR_CLEAR_FIS_STRUCT(M_FIS);
    DMR_CLEAR_FIS_SATIRI_STRUCT(M_FIS_SATIRI);

    M_FIS->modul_id   = DEMIRBAS_MODULU;
    M_FIS->program_id = E9_PROGRAMI;

    m_log_kaydi_eklenecek_mi = false;
    m_kayit_eklendi          = false;
    m_kayit_silindi          = false;
}


/**************************************************************************************
                   DMR_AMORTISMAN_FISI::NEW_FIS_RECORD
***************************************************************************************/

void DMR_AMORTISMAN_FISI::NEW_FIS_RECORD()
{
    m_ui->lineedit_fis_no->setText("*");
}

/**************************************************************************************
                   DMR_AMORTISMAN_FISI::GET_FIS_RECORD
***************************************************************************************/

int DMR_AMORTISMAN_FISI::GET_FIS_RECORD ( int record_id )
{

    DMR_FIS_BILGILERINI_OKU(M_FIS , record_id);

    m_ui->lineedit_fis_no->setText ( QVariant(M_FIS->fis_no).toString() );
    m_ui->adakDate_fis_tarihi->SET_DATE(M_FIS->fis_tarihi);
    m_ui->limitedTextEdit_aciklama->setPlainText( M_FIS->aciklama );
    m_ui->commaedit_toplam_tutar->SET_DOUBLE ( M_FIS->toplam_tutar );
    m_ui->commaEdit_amortisman_yuzdesi->SET_DOUBLE ( M_FIS->dusulecek_amor_yuzdesi );


    SQL_QUERY select_query ( DB );

    select_query.PREPARE_SELECT ( "dmr_fis_satirlari","fis_satiri_id,demirbas_id,aciklama,satir_tutari ",
                                  "fis_id = :fis_id " );
    select_query.SET_VALUE      ( ":fis_id" , record_id );

    if ( select_query.SELECT("order_number ASC") EQ 0 ) {
        return ADAK_OK;
    }
    int current_row;
    QTableWidgetItem * new_item;

    while ( select_query.NEXT() ) {
        current_row = ADD_NEW_LINE();

        new_item = new QTableWidgetItem    ( select_query.VALUE(0).toString() );
        m_ui->tablewidget_fis_satirlari->setItem ( current_row, ROW_ID_COLUMN, new_item );

        new_item = new QTableWidgetItem ( QVariant ( record_id ).toString() );
        m_ui->tablewidget_fis_satirlari->setItem ( current_row,FIS_ID_COLUMN,new_item );

        int demirbas_id         = select_query.VALUE("demirbas_id").toInt();

        double satir_tutari     = select_query.VALUE("satir_tutari").toDouble();

        QSearchEdit * searchedit_demirbas_kodu                  = ( QSearchEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget ( current_row, DEMIRBAS_KODU_COLUMN        );
        QLineEdit   * lineedit_demirbas_ismi                    = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget ( current_row, DEMIRBAS_ADI_COLUMN         );
        QCommaEdit  * commaedit_demirbas_yeni_deger             = ( QCommaEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( current_row, YENI_DEMIRBAS_DEGERI_COLUMN );
        QCommaEdit  * commaedit_yeni_brkms_amortisman_tutari    = ( QCommaEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( current_row, YENI_BIR_AMORTISMAN_COLUMN  );
        QCommaEdit  * commaeedit_amortisman_orani               = ( QCommaEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( current_row, AMORTISMAN_ORANI_COLUMN     );
        QCommaEdit  * commaedit_net_deger                       = ( QCommaEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( current_row, NET_DEGER_COLUMN            );
        QCommaEdit  * commaedit_amortisman_tutari               = ( QCommaEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( current_row, AMORTISMAN_TUTARI_COLUMN    );
        QLineEdit   * lineedit_aciklama                         = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget ( current_row, ACIKLAMA_COLUMN             );
        QLineEdit   * lineedit_amortisman_yontemi               = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget ( current_row, AMORTISMAN_YONTEMI_COLUMN   );
        QLineEdit   * lineedit_demirbas_id                      = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget ( current_row , DEMIRBAS_ID_COLUMN);

        lineedit_aciklama->setText(select_query.VALUE("aciklama").toString() ) ;
        lineedit_demirbas_id->setText(QVariant(demirbas_id).toString());

        SQL_QUERY sql_query ( DB );

        sql_query.PREPARE_SELECT ( "dmr_demirbaslar","demirbas_adi,demirbas_kodu,demirbasin_yeni_degeri,yeni_brkms_amortisman_tutari, "
                                   "amortisman_orani,amortisman_yontemi",
                                   "demirbas_id = :demirbas_id" );
        sql_query.SET_VALUE      ( ":demirbas_id" , demirbas_id );

        if ( sql_query.SELECT() NE 0 ) {
             sql_query.NEXT();
            lineedit_demirbas_ismi->setText(sql_query.VALUE(0).toString());
            searchedit_demirbas_kodu->SET_TEXT(sql_query.VALUE(1).toString());

            commaedit_demirbas_yeni_deger->SET_DOUBLE ( sql_query.VALUE(2).toDouble() );
            commaedit_yeni_brkms_amortisman_tutari->SET_DOUBLE ( sql_query.VALUE(3).toDouble() );
            commaeedit_amortisman_orani->SET_DOUBLE ( sql_query.VALUE(4).toDouble() );
            lineedit_amortisman_yontemi->setText ( DMR_GET_AMORTISMAN_SEKLI_STRING ( sql_query.VALUE(5).toInt() ) );
        }
        double net_deger = sql_query.VALUE(2).toDouble() - sql_query.VALUE(3).toDouble();
        commaedit_net_deger->SET_DOUBLE ( net_deger );
        commaedit_amortisman_tutari->SET_DOUBLE ( satir_tutari );

    }

    return ADAK_OK;
}

/**************************************************************************************
                   DMR_AMORTISMAN_FISI::SET_LINE_DEFAULTS
***************************************************************************************/

void DMR_AMORTISMAN_FISI::SET_LINE_DEFAULTS ( int row_number )
{
    QLineEdit   * lineedit_demirbas_ismi                    = ( QLineEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number , DEMIRBAS_ADI_COLUMN         );
    QCommaEdit  * commaedit_demirbas_yeni_deger             = ( QCommaEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number , YENI_DEMIRBAS_DEGERI_COLUMN );
    QCommaEdit  * commaedit_yeni_brkms_amortisman_tutari    = ( QCommaEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number , YENI_BIR_AMORTISMAN_COLUMN  );
    QCommaEdit  * commaeedit_amortisman_orani               = ( QCommaEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number , AMORTISMAN_ORANI_COLUMN     );
    QCommaEdit  * commaedit_net_deger                       = ( QCommaEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number , NET_DEGER_COLUMN            );
    QCommaEdit  * commaedit_amortisman_tutari               = ( QCommaEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number , AMORTISMAN_TUTARI_COLUMN    );
    QLineEdit   * lineedit_amortisman_yontemi               = ( QLineEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number , AMORTISMAN_YONTEMI_COLUMN   );

    lineedit_demirbas_ismi->setReadOnly ( true );
    commaedit_demirbas_yeni_deger->setReadOnly ( true );
    commaedit_yeni_brkms_amortisman_tutari->setReadOnly ( true );
    commaedit_net_deger->setReadOnly ( true );
    commaeedit_amortisman_orani->setReadOnly ( true ) ;
    commaedit_amortisman_tutari->setReadOnly ( true );
    lineedit_amortisman_yontemi->setReadOnly ( true );

    QLineEdit * lineedit_aciklama = ( QLineEdit *) m_ui->tablewidget_fis_satirlari->cellWidget(row_number,ACIKLAMA_COLUMN);

    lineedit_aciklama->setMaxLength(128);
    lineedit_demirbas_ismi->setMaxLength(128);
    lineedit_amortisman_yontemi->setMaxLength(20);
}

/**************************************************************************************
                   DMR_AMORTISMAN_FISI::CHECK_FIS_FORM_VAR
***************************************************************************************/

int DMR_AMORTISMAN_FISI::CHECK_FIS_FORM_VAR ( QObject * object )
{
    if ( object EQ m_ui->commaEdit_amortisman_yuzdesi ) {
        if ( m_ui->commaEdit_amortisman_yuzdesi->GET_DOUBLE() > 100 ) {
            MSG_WARNING( tr ( "Düşülecek yıllık amortisman yüzdesi, 100 'den fazla olamaz!.."), m_ui->commaEdit_amortisman_yuzdesi );

            return ADAK_FAIL_UNDO;
        }
        SQL_QUERY select_query ( DB );
        select_query.PREPARE_SELECT ( "dmr_fis_satirlari,dmr_fisler","fis_satiri_id","dmr_fis_satirlari.fis_id = dmr_fisler.fis_id AND fis_no = :fis_no");
        select_query.SET_VALUE      ( ":fis_no" , m_ui->lineedit_fis_no->text().toInt());

        if(select_query.SELECT() NE 0 ) {
            MSG_WARNING( tr ( "Düşülecek yıllık amortisman yüzdesini değiştiremezsiniz!..Fişi silip,tekrardan oluşturabilirsiniz!.."), m_ui->commaEdit_amortisman_yuzdesi );

            return ADAK_FAIL;
        }
    }

    return ADAK_OK;    
}

/**************************************************************************************
                   DMR_AMORTISMAN_FISI::CHECK_FIS_FORM_EMPTY
***************************************************************************************/

int DMR_AMORTISMAN_FISI::CHECK_FIS_FORM_EMPTY()
{
    if ( m_ui->lineedit_fis_no->text().isEmpty() EQ true ) {
        MSG_WARNING( tr ( "Yeni Fiş Kaydı Yapabilmek için Fiş No değeri olarak (*) girmelisiniz!.."), m_ui->lineedit_fis_no );

        return ADAK_FAIL;
    }
    if ( m_ui->commaEdit_amortisman_yuzdesi->GET_DOUBLE() EQ 0.0 ) {
        MSG_WARNING( tr ( "Ayrılacak amortisman yüzdesini girmelisiniz!.." ), m_ui->commaEdit_amortisman_yuzdesi );

        return ADAK_FAIL;
    }

    return ADAK_OK;
}

/**************************************************************************************
                   DMR_AMORTISMAN_FISI::CHECK_FIS_RECORD_ADD
***************************************************************************************/

int DMR_AMORTISMAN_FISI::CHECK_FIS_RECORD_ADD()
{
    if ( QString ( m_ui->lineedit_fis_no->text().at(0)) NE "*" ) {
        if ( DMR_FIS_NO_KULLANILDI_MI ( m_ui->lineedit_fis_no->text().toInt(), m_ui->adakDate_fis_tarihi->DATE()) EQ true ) {
            MSG_WARNING( tr ( "Aynı Fiş Numarası daha önce kullanılmış.Lütfen fiş numarasını değiştiriniz!.."), m_ui->lineedit_fis_no );

            return ADAK_FAIL;
        }
    }

    return ADAK_OK;
}

/**************************************************************************************
                   DMR_AMORTISMAN_FISI::ADD_FIS_RECORD
***************************************************************************************/

int DMR_AMORTISMAN_FISI::ADD_FIS_RECORD()
{
    int fis_no = 0;

    if ( QString ( m_ui->lineedit_fis_no->text().at(0) ) EQ "*") {
        fis_no = DMR_SIRADAKI_FIS_NO_AL ( m_ui->adakDate_fis_tarihi->DATE());
    }
    else {
        fis_no = m_ui->lineedit_fis_no->text().toInt();
    }


    M_FIS->program_id             = E9_PROGRAMI;
    M_FIS->modul_id               = DEMIRBAS_MODULU;
    M_FIS->fis_no                 = fis_no;
    M_FIS->fis_tarihi             = m_ui->adakDate_fis_tarihi->DATE();
    M_FIS->aciklama               = m_ui->limitedTextEdit_aciklama->toPlainText();
    M_FIS->fis_turu               = AMORTISMAN_AYIRMA_FISI;
    M_FIS->dusulecek_amor_yuzdesi = m_ui->commaEdit_amortisman_yuzdesi->GET_DOUBLE();


    int fis_id = DMR_FIS_EKLE(M_FIS);

    m_ui->lineedit_fis_no->setText ( QVariant(M_FIS->fis_no).toString());

    m_log_kaydi_eklenecek_mi = true;
    m_kayit_eklendi          = true;

    m_log_detaylari = QObject::tr ( "Amortisman Fişi , Düşülecek Yıllık Amortisman Yüzdesi : " )
                      + m_ui->commaEdit_amortisman_yuzdesi->GET_TEXT();


    m_log_detaylari.append("\nFiş Tarihi  : " + m_ui->adakDate_fis_tarihi->QDATE().toString("dd MMMM yyyy")
                           + " , " + "Fiş No : " + QVariant ( fis_no ).toString() );


    return fis_id;
}

/**************************************************************************************
                   DMR_AMORTISMAN_FISI::CHECK_FIS_RECORD_UPDATE
***************************************************************************************/

int DMR_AMORTISMAN_FISI::CHECK_FIS_RECORD_UPDATE ( int record_id )
{
    if ( QString ( m_ui->lineedit_fis_no->text().at(0)) EQ "*" ) {
        MSG_WARNING(  tr ( "Fiş numarasını güncellemek için fiş numarası kısmına sadece sayı girebilirsiniz."), m_ui->lineedit_fis_no );

        return ADAK_FAIL;
    }

    int fis_no = m_ui->lineedit_fis_no->text().toInt();

    SQL_QUERY select_query ( DB );

    select_query.PREPARE_SELECT ( "dmr_fisler","fis_id ",
                                  "fis_no = :fis_no "
                                  "AND fis_tarihi = :fis_tarihi");

    select_query.SET_VALUE ( ":fis_no",fis_no );
    select_query.SET_VALUE ( ":fis_tarihi",  m_ui->adakDate_fis_tarihi->DATE());

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
                   DMR_AMORTISMAN_FISI::UPDATE_FIS_RECORD
***************************************************************************************/

void DMR_AMORTISMAN_FISI::UPDATE_FIS_RECORD ( int record_id )
{

    DMR_FIS_BILGILERINI_OKU(M_FIS , record_id);

    M_FIS->aciklama               = m_ui->limitedTextEdit_aciklama->toPlainText();
    M_FIS->fis_no                 = m_ui->lineedit_fis_no->text().toInt();
    M_FIS->fis_tarihi             = m_ui->adakDate_fis_tarihi->DATE();
    M_FIS->dusulecek_amor_yuzdesi = m_ui->commaEdit_amortisman_yuzdesi->GET_DOUBLE();

    DMR_FIS_GUNCELLE ( M_FIS , record_id);

    m_log_kaydi_eklenecek_mi = true;

    m_log_detaylari = QObject::tr("Amortisman Fişi , Düşülecek Yıllık Amortisman Yüzdesi : ")
                      + m_ui->commaEdit_amortisman_yuzdesi->GET_TEXT();

    m_log_detaylari.append("\nFiş Tarihi  : " + m_ui->adakDate_fis_tarihi->QDATE().toString("dd MMMM YYYY")
                           + " , " + "Fiş No : " + QVariant ( M_FIS->fis_no).toString() + "Ayrılacak Amortisman Tutarı : " +
                           VIRGUL_EKLE ( QVariant ( ROUND(M_FIS->toplam_tutar) ).toString(),2));

}

/**************************************************************************************
                   DMR_AMORTISMAN_FISI::CHECK_FIS_RECORD_DELETE
***************************************************************************************/

int DMR_AMORTISMAN_FISI::CHECK_FIS_RECORD_DELETE ( int record_id )
{
    Q_UNUSED ( record_id );


    return ADAK_OK;
}

/**************************************************************************************
                   DMR_AMORTISMAN_FISI::DELETE_FIS_RECORD
***************************************************************************************/

void DMR_AMORTISMAN_FISI::DELETE_FIS_RECORD ( int record_id )
{
    DMR_FIS_SIL(M_FIS , record_id);

    m_log_kaydi_eklenecek_mi = true;
    m_kayit_silindi          = true;

    m_log_detaylari = QObject::tr ( "Amortisman Fişi , Düşülecek Yıllık Amortisman Yüzdesi : " )
                      + VIRGUL_EKLE ( QVariant ( ROUND ( M_FIS->dusulecek_amor_yuzdesi)).toString());
    m_log_detaylari.append ( "\nFiş Tarihi  : " + M_FIS->fis_tarihi
                           + " , " + "Fiş No : " + QVariant(M_FIS->fis_no).toString() + "Ayrılacak Amortisman Tutarı : " +
                           VIRGUL_EKLE ( QVariant (  ROUND(M_FIS->toplam_tutar ) ).toString(),2 ) );

}

/**************************************************************************************
                   DMR_AMORTISMAN_FISI::SELECT_FIS_RECORD
***************************************************************************************/

int DMR_AMORTISMAN_FISI::SELECT_FIS_RECORD()
{
    return OPEN_DMR_FIS_ARAMA ( AMORTISMAN_AYIRMA_FISI,this );
}

/**************************************************************************************
                   DMR_AMORTISMAN_FISI::FIND_FIS_RECORD
***************************************************************************************/

int DMR_AMORTISMAN_FISI::FIND_FIS_RECORD()
{
    SQL_QUERY select_query ( DB );

    select_query.PREPARE_SELECT ( "dmr_fisler","fis_id ",
                                  "fis_no = :fis_no "
                                  "AND fis_turu       = :fis_turu "
                                  "AND fis_tarihi     = :fis_tarihi");

    select_query.SET_VALUE      ( ":fis_no"       , m_ui->lineedit_fis_no->text().toInt() );
    select_query.SET_VALUE      ( ":fis_turu"     , AMORTISMAN_AYIRMA_FISI );
    select_query.SET_VALUE      ( ":fis_tarihi"   , m_ui->adakDate_fis_tarihi->DATE() );

    if ( select_query.SELECT() EQ 0 ) {
        MSG_WARNING( tr ( "Aradığınız Amortisman Fişi Bulunamadı!.." ) , NULL );
        return ADAK_FAIL;
    }
    select_query.NEXT();

    return select_query.VALUE(0).toInt();
}

/**************************************************************************************
                   DMR_AMORTISMAN_FISI::FIND_FIRST_FIS_RECORD
***************************************************************************************/

int DMR_AMORTISMAN_FISI::FIND_FIRST_FIS_RECORD()
{
    SQL_QUERY select_query ( DB );

    select_query.PREPARE_SELECT ( "dmr_fisler","fis_id ",
                                   "fis_turu = :fis_turu ");


    select_query.SET_VALUE       ( ":fis_turu",   AMORTISMAN_AYIRMA_FISI );

    if ( select_query.SELECT("fis_tarihi ASC ,fis_no ASC",0,1) EQ 0 ) {
        return 0;
    }

    select_query.NEXT();

    return select_query.VALUE(0).toInt();
}

/**************************************************************************************
                   DMR_AMORTISMAN_FISI::FIND_LAST_FIS_RECORD
***************************************************************************************/

int DMR_AMORTISMAN_FISI::FIND_LAST_FIS_RECORD()
{
    SQL_QUERY select_query ( DB );

    select_query.PREPARE_SELECT  (  "dmr_fisler","fis_id ",
                                    "fis_turu = :fis_turu ");


    select_query.SET_VALUE       ( ":fis_turu" , AMORTISMAN_AYIRMA_FISI );

    if ( select_query.SELECT("fis_tarihi DESC, fis_no DESC",0,1) EQ 0 ) {
        return 0;
    }

    select_query.NEXT();

    return select_query.VALUE(0).toInt();
}

/**************************************************************************************
                   DMR_AMORTISMAN_FISI::FIND_PREV_FIS_RECORD
***************************************************************************************/

int DMR_AMORTISMAN_FISI::FIND_PREV_FIS_RECORD()
{
    SQL_QUERY select_query ( DB );

    select_query.PREPARE_SELECT ( "dmr_fisler","fis_id ",
                                        "fis_no < :fis_no "
                                        "AND fis_turu   = :fis_turu "
                                        "AND fis_tarihi = :fis_tarihi");


    select_query.SET_VALUE            ( ":fis_no" , m_ui->lineedit_fis_no->text().toInt() );
    select_query.SET_VALUE            ( ":fis_turu"     , AMORTISMAN_AYIRMA_FISI );
    select_query.SET_VALUE            ( ":fis_tarihi"   , m_ui->adakDate_fis_tarihi->DATE());

    if ( select_query.SELECT("fis_no DESC",0,1) NE 0 ) {
        select_query.NEXT();
        return select_query.VALUE(0).toInt();
    }

    select_query.PREPARE_SELECT ( "dmr_fisler","fis_id ",
                                  "fis_turu   = :fis_turu "
                                  "AND   fis_tarihi < :fis_tarihi");


    select_query.SET_VALUE       ( ":fis_turu"     , AMORTISMAN_AYIRMA_FISI);
    select_query.SET_VALUE       ( ":fis_tarihi"   , m_ui->adakDate_fis_tarihi->DATE());

    if ( select_query.SELECT("fis_tarihi DESC, fis_no DESC",0,1) EQ 0 ) {
        return 0;
    }
    select_query.NEXT();
    return select_query.VALUE(0).toInt();
}

/**************************************************************************************
                   DMR_AMORTISMAN_FISI::FIND_NEXT_FIS_RECORD
***************************************************************************************/

int DMR_AMORTISMAN_FISI::FIND_NEXT_FIS_RECORD()
{
    SQL_QUERY select_query ( DB );

    select_query.PREPARE_SELECT ( "dmr_fisler","fis_id ",
                                  "fis_no > :fis_no "
                                  "AND fis_turu       = :fis_turu "
                                  "AND fis_tarihi     = :fis_tarihi");


    select_query.SET_VALUE            ( ":fis_no" , m_ui->lineedit_fis_no->text().toInt() );
    select_query.SET_VALUE            ( ":fis_turu"     , AMORTISMAN_AYIRMA_FISI );
    select_query.SET_VALUE            ( ":fis_tarihi"   , m_ui->adakDate_fis_tarihi->DATE() );

    if ( select_query.SELECT("fis_no ASC",0,1) NE 0 ) {
        select_query.NEXT();
        return select_query.VALUE(0).toInt();
    }

    select_query.PREPARE_SELECT ( "dmr_fisler","fis_id","fis_turu   = :fis_turu AND fis_tarihi > :fis_tarihi");


    select_query.SET_VALUE      ( ":fis_turu"     , AMORTISMAN_AYIRMA_FISI );
    select_query.SET_VALUE      ( ":fis_tarihi"   , m_ui->adakDate_fis_tarihi->DATE());

    if ( select_query.SELECT("fis_tarihi ASC, fis_no ASC",0,1 ) EQ 0 ) {
        return 0;
    }
    select_query.NEXT();
    return select_query.VALUE(0).toInt();
}

/**************************************************************************************
                   DMR_AMORTISMAN_FISI::CHECK_LINE_VAR
***************************************************************************************/

int DMR_AMORTISMAN_FISI::CHECK_LINE_VAR ( int row_number, QObject * object )
{
    QSearchEdit * searchedit_demirbas_kodu                  = ( QSearchEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, DEMIRBAS_KODU_COLUMN        );
    QCommaEdit  * commaedit_demirbas_yeni_deger             = ( QCommaEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, YENI_DEMIRBAS_DEGERI_COLUMN );
    QCommaEdit  * commaedit_yeni_brkms_amortisman_tutari    = ( QCommaEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, YENI_BIR_AMORTISMAN_COLUMN  );
    QCommaEdit  * commaeedit_amortisman_orani               = ( QCommaEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, AMORTISMAN_ORANI_COLUMN     );
    QCommaEdit  * commaedit_net_deger                       = ( QCommaEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, NET_DEGER_COLUMN            );
    QLineEdit   * lineedit_amortisman_yontemi               = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, AMORTISMAN_YONTEMI_COLUMN   );
    QLineEdit   * lineedit_demirbas_id                      = ( QLineEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number , DEMIRBAS_ID_COLUMN);
    QLineEdit   * lineedit_demirbas_ismi                    = ( QLineEdit *) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number,DEMIRBAS_ADI_COLUMN);

    if ( object EQ searchedit_demirbas_kodu ) {

        if ( searchedit_demirbas_kodu->GET_TEXT().isEmpty() EQ true ) {
            lineedit_demirbas_ismi->clear();
            lineedit_demirbas_id->clear();
            return ADAK_OK;
        }

        SQL_QUERY select_query ( DB  );

        select_query.PREPARE_SELECT ( "dmr_demirbaslar" , "yeniden_degerleme_yapildi , yeniden_degerleme_yapilacak_mi , "
                                      "demirbas_adi , demirbas_id , demirbasin_yeni_degeri , yeni_brkms_amortisman_tutari, "
                                      "amortisman_orani , alis_tarihi , amortisman_yontemi,"
                                      "yillik_ayrilan_amor_yuzdesi , muh_yili_ayrilacak_amor_tutari,"
                                      "kist_amortisman,fatura_id,amortisman_ayrilacak_mi ,"
                                      "muh_devir_yili_ayrilacak_amor,amortisman_suresi",
                                      "demirbas_kodu = :demirbas_kodu" );
        select_query.SET_VALUE      ( ":demirbas_kodu" , searchedit_demirbas_kodu->GET_TEXT() );

        if ( select_query.SELECT() EQ 0 ) {
            MSG_WARNING( tr ( "Aradığınız sabit kıymet kodu bulunamadı!.." ), searchedit_demirbas_kodu );

            return ADAK_FAIL;
        }
        select_query.NEXT();

        int amortisman_ayrilacak_mi = select_query.VALUE("amortisman_ayrilacak_mi").toInt();

        if ( amortisman_ayrilacak_mi EQ 0 ) {
            MSG_WARNING( tr ( " Sabit kıymet için amortisman ayrılmayacaktır.Lütfen değiştiriniz." ), searchedit_demirbas_kodu );

            return ADAK_FAIL;
        }

        int yeniden_degerleme_yapildi           = select_query.VALUE(0).toInt();
        int yeniden_degerleme_yapilacak_mi      = select_query.VALUE(1).toInt();

        QString demirbas_ismi                   = select_query.VALUE(2).toString();
        int     demirbas_id                     = select_query.VALUE(3).toInt();

        double  demirbasin_yeni_degeri          = select_query.VALUE(4).toDouble();
        double  yeni_birikmis_amortisman_tutari = select_query.VALUE(5).toDouble();

        double amortisman_orani                 = select_query.VALUE(6).toDouble();
        QDate  alis_tarihi                      = QDate::fromString(select_query.VALUE(7).toString(),"yyyy.MM.dd");

        int    amortisman_yontemi               = select_query.VALUE(8).toInt();

        double yillik_ayrilan_amortisman_yuzdesi = select_query.VALUE(9).toDouble();

        double muhasebe_yili_ayrilacak_toplam_amortisman_tutari = select_query.VALUE(10).toDouble();

        int kist_amortisman_uygulanacak_mi  = select_query.VALUE(11).toInt();

        int fatura_id = select_query.VALUE("fatura_id").toInt();

        double muh_devir_yili_ayrilacak_amor_tutari = select_query.VALUE("muh_devir_yili_ayrilacak_amor").toDouble();

        int amortisman_suresi = select_query.VALUE("amortisman_suresi").toInt();

        if ( yillik_ayrilan_amortisman_yuzdesi EQ 100 ) {
             MSG_WARNING( tr ( "Girilen demirbaşın yıllık amortismanı ayrılmıştır.Tekrar aynı işlemi gerçekleştiremezsiniz!.."), NULL );
             return ADAK_FAIL;
        }

        double max_ayrilacak_amortisman_yuzdesi = 100 - yillik_ayrilan_amortisman_yuzdesi;


        yillik_ayrilan_amortisman_yuzdesi += m_ui->commaEdit_amortisman_yuzdesi->GET_DOUBLE();

        if ( yillik_ayrilan_amortisman_yuzdesi > 100 ) {
             MSG_WARNING( tr ( "Düşülecek amortisman miktarı çok fazla,demirbaştan en fazla % %1 oranında amortisman ayırabilirsiniz!..").arg(max_ayrilacak_amortisman_yuzdesi), commaeedit_amortisman_orani );

             return ADAK_FAIL;
        }



        int demirbas_alim_yili        = alis_tarihi.year();
        int current_year              = m_ui->adakDate_fis_tarihi->QDATE().year();

        if ( current_year NE demirbas_alim_yili ) {

            if ( yeniden_degerleme_yapildi EQ 0 ) {
                if ( yeniden_degerleme_yapilacak_mi EQ 1 ) {
                    MSG_WARNING( tr ( "Girdiğiniz demirbaş için yeniden değerleme yapılmamıştır.Önce yeniden değerleme işlemi yapmalısınız!.."), NULL );
                    return ADAK_FAIL;
                }
            }
        }
        else {
            //demirbasin alindigi aya gore ayrilmasi gereken amortisman oranina bakilir.

            int demirbasin_alindigi_ay = alis_tarihi.month();
            int amortisman_ayi         = m_ui->adakDate_fis_tarihi->QDATE().month();

            if ( demirbasin_alindigi_ay > amortisman_ayi ) {
                MSG_WARNING(QObject::tr("Amortisman ayırmak istediğiniz dönem ile seçilen sabit kıymetin alındığı ay birbirini tutmuyor.Seçilen sabit kıymet için amortisman ayıramazsınız."),NULL);
                return ADAK_FAIL;
            }


            double ayrilacak_amor_orani = DMR_GET_AYRILACAK_AMORTISMAN_ORANI(demirbasin_alindigi_ay);

            if ( ayrilacak_amor_orani NE m_ui->commaEdit_amortisman_yuzdesi->GET_DOUBLE() ) {

                MSG_WARNING(QObject::tr("Seçilen sabit kıymet için % %1 oranında amortisman ayırmak zorundasınız.Amortisman oranı ile sabit kıymet için ayrılması gereken oran birbirini tutmuyor.").arg(ayrilacak_amor_orani),NULL);
                return ADAK_FAIL;
            }

        }


        lineedit_demirbas_ismi->setText (demirbas_ismi );
        commaedit_demirbas_yeni_deger->SET_DOUBLE ( demirbasin_yeni_degeri );
        commaedit_yeni_brkms_amortisman_tutari->SET_DOUBLE ( yeni_birikmis_amortisman_tutari );
        commaeedit_amortisman_orani->SET_DOUBLE ( amortisman_orani);
        lineedit_amortisman_yontemi->setText ( DMR_GET_AMORTISMAN_SEKLI_STRING ( amortisman_yontemi ) );

        //yeniden degerleme sonrasi

        double net_deger = demirbasin_yeni_degeri - yeni_birikmis_amortisman_tutari;
        commaedit_net_deger->SET_DOUBLE ( net_deger );

        lineedit_demirbas_id->setText(QVariant(demirbas_id).toString());

        if ( muhasebe_yili_ayrilacak_toplam_amortisman_tutari EQ 0 OR
             muh_devir_yili_ayrilacak_amor_tutari EQ 0) {

            if ( fatura_id > 0 ) {

                FATURA_TEMEL_BILGILER_STRUCT P_FAT;

                FAT_GET_FAT_TEMEL_BILGILER( &P_FAT , fatura_id );


                double fatura_tutari    = P_FAT.fatura_tutari;
                double kdv_tutari       = P_FAT.isk_sonrasi_kdv_tutari;
                double kdv_haric_tutar  = fatura_tutari - kdv_tutari;

                if (  amortisman_yontemi EQ NORMAL_BAKIYE ) {
                     muhasebe_yili_ayrilacak_toplam_amortisman_tutari = kdv_haric_tutar * ( amortisman_orani/100);
                }
                else {
                    muhasebe_yili_ayrilacak_toplam_amortisman_tutari = ( fatura_tutari - yeni_birikmis_amortisman_tutari)
                                                                       * ( ( amortisman_orani * 2)/100);
                }
                muh_devir_yili_ayrilacak_amor_tutari = muhasebe_yili_ayrilacak_toplam_amortisman_tutari;
            }
            else {
                if (  amortisman_yontemi EQ NORMAL_BAKIYE ) {
                     muhasebe_yili_ayrilacak_toplam_amortisman_tutari = demirbasin_yeni_degeri * ( amortisman_orani/100);
                }
                else {
                    muhasebe_yili_ayrilacak_toplam_amortisman_tutari = net_deger * ( ( amortisman_orani * 2)/100);
                }
                muh_devir_yili_ayrilacak_amor_tutari = muhasebe_yili_ayrilacak_toplam_amortisman_tutari;
            }

            //KIST AMORTISMAN VAR ISE,ILK YIL BINEK OTOMOBILIN FIRMAYA ALINDIGI GUNDEN ALINDIGI AY  DAHIL OLMAK UZERE YIL SONUNA KADAR KALAN TUM AYLAR ICIN,
            //SON YIL ISE ILK YILDAN AMORTISMAN AYRILMAYAN AYLAR VE
            //SON YILIN TAMAMI ICIN AMORTISMAN AYRILIR.



            if ( kist_amortisman_uygulanacak_mi EQ 1) {

                int demirbasin_alindigi_ay = alis_tarihi.month();

                if ( alis_tarihi.year() EQ m_ui->adakDate_fis_tarihi->QDATE().year() ) {
                     muhasebe_yili_ayrilacak_toplam_amortisman_tutari = ( muhasebe_yili_ayrilacak_toplam_amortisman_tutari / 12) * ((12 - demirbasin_alindigi_ay ) + 1);

                }

                if ( ( alis_tarihi.year() + amortisman_suresi) EQ m_ui->adakDate_fis_tarihi->QDATE().year() ) {
                     double ayrilacak_amortisman_tutari = (muhasebe_yili_ayrilacak_toplam_amortisman_tutari/12) * (demirbasin_alindigi_ay - 1);

                     muhasebe_yili_ayrilacak_toplam_amortisman_tutari += ayrilacak_amortisman_tutari;

                }

                if ( ( alis_tarihi.year() + amortisman_suresi) EQ (m_ui->adakDate_fis_tarihi->QDATE().year() + 1 )) {
                    double ayrilacak_amortisman_tutari = (muhasebe_yili_ayrilacak_toplam_amortisman_tutari/12) * (demirbasin_alindigi_ay - 1);

                    muhasebe_yili_ayrilacak_toplam_amortisman_tutari += ayrilacak_amortisman_tutari;
                }


            }
            DB->START_TRANSACTION();

            SQL_QUERY update_query ( DB );

            update_query.PREPARE_UPDATE ( "dmr_demirbaslar","demirbas_id","muh_yili_ayrilacak_amor_tutari,"
                                          "muh_devir_yili_ayrilacak_amor",
                                          "demirbas_id = :demirbas_id" );
            update_query.SET_VALUE      ( ":muh_yili_ayrilacak_amor_tutari" , ROUND(muhasebe_yili_ayrilacak_toplam_amortisman_tutari,-2));
            update_query.SET_VALUE      ( ":muh_devir_yili_ayrilacak_amor"  , ROUND(muh_devir_yili_ayrilacak_amor_tutari ,-2));
            update_query.SET_VALUE      ( ":demirbas_id"                    , demirbas_id ) ;
            update_query.UPDATE();

            DB->COMMIT_TRANSACTION();
        }


        //Ayrilacak amortisman yuzdesi yillik amortismanin yuzde kacinin ayrilacagini gosterir.
        double ayrilacak_amortisman_yuzdesi = m_ui->commaEdit_amortisman_yuzdesi->GET_DOUBLE();

        double ayrilan_amortisman_tutari = 0.0;

        if ( m_ui->adakDate_fis_tarihi->QDATE().year() EQ MALI_YIL_FIRST_DATE().year() ) {
            ayrilan_amortisman_tutari = muhasebe_yili_ayrilacak_toplam_amortisman_tutari *  ( ayrilacak_amortisman_yuzdesi/100);
        }
        else {
            ayrilan_amortisman_tutari = muh_devir_yili_ayrilacak_amor_tutari *  ( ayrilacak_amortisman_yuzdesi/100);
        }

        QCommaEdit * commaedit_ayrilan_amortisman_tutari = ( QCommaEdit* ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number,AMORTISMAN_TUTARI_COLUMN);
        commaedit_ayrilan_amortisman_tutari->SET_DOUBLE ( ayrilan_amortisman_tutari );
    }
    return ADAK_OK;
}

/**************************************************************************************
                   DMR_AMORTISMAN_FISI::CHECK_LINE_EMPTY
***************************************************************************************/

int DMR_AMORTISMAN_FISI::CHECK_LINE_EMPTY(int row_number)
{
    QSearchEdit * searchedit_demirbas_kodu = ( QSearchEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number,DEMIRBAS_KODU_COLUMN);

    if ( searchedit_demirbas_kodu->GET_TEXT().isEmpty() EQ true ) {
        MSG_WARNING( tr ( "Demirbaş kodunu boş bırakamazsınız!.." ), searchedit_demirbas_kodu );

        return ADAK_FAIL;
    }

    QCommaEdit * commaedit_amortisman_orani = ( QCommaEdit *) m_ui->tablewidget_fis_satirlari->cellWidget(row_number,AMORTISMAN_ORANI_COLUMN);

    if ( commaedit_amortisman_orani->GET_DOUBLE() EQ 0.0 ) {
        MSG_WARNING( tr ( "Amortisman Oranını Boş Bırakamazsınız!.." ), commaedit_amortisman_orani );

        return ADAK_FAIL;
    }
    return ADAK_OK;
}


/**************************************************************************************
                   DMR_AMORTISMAN_FISI::CHECK_ADD_LINE
***************************************************************************************/

int DMR_AMORTISMAN_FISI::CHECK_ADD_LINE ( int record_id, int row_number)
{
    Q_UNUSED ( record_id );
    Q_UNUSED ( row_number ) ;
    return ADAK_OK;
}

/**************************************************************************************
                   DMR_AMORTISMAN_FISI::ADD_LINE
***************************************************************************************/

void DMR_AMORTISMAN_FISI::ADD_LINE(int record_id, int row_number)
{
    QLineEdit   * lineedit_aciklama           = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, ACIKLAMA_COLUMN           );
    QCommaEdit  * commaedit_amortisman_tutari = ( QCommaEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, AMORTISMAN_TUTARI_COLUMN  );
    QLineEdit   * lineedit_demirbas_id        = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, DEMIRBAS_ID_COLUMN);

    DMR_CLEAR_FIS_SATIRI_STRUCT(M_FIS_SATIRI);

    M_FIS_SATIRI->program_id         = E9_PROGRAMI;
    M_FIS_SATIRI->modul_id           = DEMIRBAS_MODULU;
    M_FIS_SATIRI->fis_id             = record_id;
    M_FIS_SATIRI->demirbas_id        = lineedit_demirbas_id->text().toInt();
    M_FIS_SATIRI->aciklama           = lineedit_aciklama->text();
    M_FIS_SATIRI->satir_tutari       = commaedit_amortisman_tutari->GET_DOUBLE();

    int row_id = DMR_FIS_SATIRI_EKLE(M_FIS , M_FIS_SATIRI);

    QTableWidgetItem * item = new QTableWidgetItem ( QVariant ( row_id ).toString() );
    m_ui->tablewidget_fis_satirlari->setItem ( row_number,ROW_ID_COLUMN,item );

    item = new QTableWidgetItem ( QVariant ( record_id ).toString() );
    m_ui->tablewidget_fis_satirlari->setItem(row_number,FIS_ID_COLUMN,item);

    m_ui->commaedit_toplam_tutar->SET_DOUBLE(M_FIS->toplam_tutar);

    m_log_kaydi_eklenecek_mi = true;

    m_log_detaylari = QObject::tr ( "Amortisman Fişi , Düşülecek Yıllık Amortisman Yüzdesi : ")
                      + VIRGUL_EKLE ( QVariant ( ROUND ( M_FIS->dusulecek_amor_yuzdesi ) ).toString() );
    m_log_detaylari.append ( "\nFiş Tarihi  : " + M_FIS->fis_tarihi
                           + " , " + "Fiş No : " + QVariant(M_FIS->fis_no).toString() + "Ayrılacak Amortisman Tutarı : " +
                           VIRGUL_EKLE ( QVariant (M_FIS->toplam_tutar).toString() ));
}

/**************************************************************************************
                   DMR_AMORTISMAN_FISI::CHECK_UPDATE_LINE
***************************************************************************************/

int DMR_AMORTISMAN_FISI::CHECK_UPDATE_LINE ( int record_id, int row_number )
{
    Q_UNUSED ( record_id );
    Q_UNUSED ( row_number );
    return ADAK_OK;
}

/**************************************************************************************
                   DMR_AMORTISMAN_FISI::UPDATE_LINE
***************************************************************************************/

void DMR_AMORTISMAN_FISI::UPDATE_LINE ( int record_id, int row_number )
{
    Q_UNUSED(record_id);

    QCommaEdit  * commaedit_amortisman_tutari  = ( QCommaEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, AMORTISMAN_TUTARI_COLUMN  );
    QLineEdit   * lineedit_aciklama            = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, ACIKLAMA_COLUMN           );
    QLineEdit   * lineedit_demirbas_id         = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, DEMIRBAS_ID_COLUMN);

    int fis_satiri_id = m_ui->tablewidget_fis_satirlari->item(row_number , ROW_ID_COLUMN)->text().toInt();

    DMR_FIS_SATIRINI_OKU(M_FIS_SATIRI , fis_satiri_id);

    M_FIS_SATIRI->demirbas_id        = lineedit_demirbas_id->text().toInt();
    M_FIS_SATIRI->aciklama           = lineedit_aciklama->text();
    M_FIS_SATIRI->satir_tutari       = commaedit_amortisman_tutari->GET_DOUBLE();

    DMR_FIS_SATIRI_GUNCELLE(M_FIS , M_FIS_SATIRI , fis_satiri_id);


    m_ui->commaedit_toplam_tutar->SET_DOUBLE ( M_FIS->toplam_tutar);


    m_log_kaydi_eklenecek_mi = true;

    m_log_detaylari = QObject::tr ( "Amortisman Fişi , Düşülecek Yıllık Amortisman Yüzdesi : " )
                      + VIRGUL_EKLE ( QVariant ( ROUND ( M_FIS->dusulecek_amor_yuzdesi ) ).toString() );
    m_log_detaylari.append ( "\nFiş Tarihi  : " + M_FIS->fis_tarihi
                           + " , " + "Fiş No : " + QVariant(M_FIS->fis_no).toString() + "Ayrılacak Amortisman Tutarı : " +
                           VIRGUL_EKLE ( QVariant( ROUND(M_FIS->toplam_tutar) ).toString(),2));
}

/**************************************************************************************
                   DMR_AMORTISMAN_FISI::CHECK_DELETE_LINE
***************************************************************************************/

int DMR_AMORTISMAN_FISI::CHECK_DELETE_LINE ( int record_id, int row_number )
{
    Q_UNUSED ( record_id );
    Q_UNUSED ( row_number );
    return ADAK_OK;
}

/**************************************************************************************
                   DMR_AMORTISMAN_FISI::DELETE_LINE
***************************************************************************************/

void DMR_AMORTISMAN_FISI::DELETE_LINE ( int record_id, int row_number )
{
    Q_UNUSED(record_id);

    int fis_satiri_id = m_ui->tablewidget_fis_satirlari->item(row_number , ROW_ID_COLUMN)->text().toInt();

    DMR_FIS_SATIRI_SIL(M_FIS , fis_satiri_id);

    m_ui->commaedit_toplam_tutar->SET_DOUBLE ( M_FIS->toplam_tutar );


    m_log_kaydi_eklenecek_mi = true;

    m_log_detaylari = QObject::tr ( "Amortisman Fişi , Düşülecek Yıllık Amortisman Yüzdesi : " )
                      + VIRGUL_EKLE ( QVariant (M_FIS->dusulecek_amor_yuzdesi).toString() );
    m_log_detaylari.append("\nFiş Tarihi  : " + M_FIS->fis_tarihi
                           + " , " + "Fiş No : " + QVariant( M_FIS->fis_no).toString() + "Ayrılacak Amortisman Tutarı : " +
                           VIRGUL_EKLE ( QVariant( M_FIS->toplam_tutar ).toString(),2 ) );
}

/**************************************************************************************
                   DMR_AMORTISMAN_FISI::LOCK_FIS_RECORD
***************************************************************************************/

int DMR_AMORTISMAN_FISI::LOCK_FIS_RECORD (int record_id )
{
    return DB->LOCK_ROW( "fis_id","dmr_fisler", QString ( "fis_id = %1" ).arg ( record_id ));
}

/**************************************************************************************
                   DMR_AMORTISMAN_FISI::UNLOCK_FIS_RECORD
***************************************************************************************/

void DMR_AMORTISMAN_FISI::UNLOCK_FIS_RECORD ( int record_id )
{
    DB->UNLOCK_ROW ( "fis_id","dmr_fisler", QString ( "fis_id = %1" ).arg ( record_id ) );
}

int DMR_AMORTISMAN_FISI::CHECK_EXIT()
{
    return ADAK_EXIT;
}
/**************************************************************************************
                   DMR_YENIDEN_DEGERLEME_FISI::LINE_SEARCH_EDIT_CLICKED
***************************************************************************************/

void DMR_AMORTISMAN_FISI::LINE_SEARCH_EDIT_CLICKED(int row_number,QWidget * , QLineEdit * lineedit)
{
    Q_UNUSED(row_number);
    int demirbas_id = OPEN_DMR_DEMIRBAS_KARTI_ARAMA( lineedit->text(), this);

    if(demirbas_id NE -1) {
        SQL_QUERY select_query(DB);

        select_query.PREPARE_SELECT("dmr_demirbaslar","demirbas_kodu,amortisman_ayrilacak_mi","demirbas_id = :demirbas_id");
        select_query.SET_VALUE(":demirbas_id" , demirbas_id);

        if (select_query.SELECT() NE 0 ) {
            select_query.NEXT();

            if ( select_query.VALUE("amortisman_ayrilacak_mi").toInt() EQ 0 ) {
                MSG_WARNING("Sabit kıymet için amortisman ayrılmayacaktır.Lütfen değiştiriniz." , NULL);
                return;
            }

            lineedit->setText(select_query.VALUE(0).toString());
        }
    }
}

/**************************************************************************************
                   DMR_AMORTISMAN_FISI::LEAVE_RECORD
***************************************************************************************/

int DMR_AMORTISMAN_FISI::LEAVE_RECORD()
{
    if ( m_log_kaydi_eklenecek_mi EQ true ) {

        DB->START_TRANSACTION();

        if ( m_kayit_eklendi EQ true ) {
            E9_LOG_KAYDI_EKLE (DEMIRBAS_MODULU , LOG_DMR_FISLER , LOG_ISLEM_ADD , m_log_detaylari);
        }
        if ( m_kayit_silindi EQ true ) {
            E9_LOG_KAYDI_EKLE (DEMIRBAS_MODULU , LOG_DMR_FISLER , LOG_ISLEM_DELETE , m_log_detaylari);
        }
        if ( m_kayit_eklendi EQ false AND m_kayit_silindi EQ false ) {
            E9_LOG_KAYDI_EKLE (DEMIRBAS_MODULU , LOG_DMR_FISLER , LOG_ISLEM_UPDATE , m_log_detaylari);
        }
        DB->COMMIT_TRANSACTION();

    }
    return ADAK_OK;
}









