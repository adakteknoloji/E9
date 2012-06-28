#include <QHeaderView>
#include <QScrollBar>
#include <QDebug>
#include "adak_defines.h"
#include "adak_utils.h"
#include "muh_fis_utils.h"
#include "ui_muh_muhasebe_fisi.h"
#include "muh_muhasebe_fisi_class.h"
#include "muh_muhasebe_fisi_open.h"
#include "print.h"
#include "muh_enum.h"
#include "muh_struct.h"
#include "e9_enum.h"
#include "e9_console_utils.h"
#include "e9_gui_utils.h"
#include "e9_log.h"
#include "muh_fis_utils.h"
#include "adak_std_utils.h"
#include "e9_yetki.h"
#include "muh_fis_arama_open.h"
#include "muh_fis_kopyalama_batch_open.h"
#include "muh_hesap_arama_open.h"
#include "muh_kdv_ayirma_batch_open.h"
#include "muh_toplu_fis_raporu_open.h"
#include "muh_console_utils.h"
#include "muh_gui_utils.h"
#include "sube_console_utils.h"

extern ADAK_SQL   *            DB;

#define     ROW_ID_COLUMN                    0
#define     FIS_ID_COLUMN                    1
#define     ORDER_COLUMN                     2
#define     HESAP_ID_COLUMN                  3
#define     HESAP_KODU_COLUMN                4
#define     HESAP_ISMI_COLUMN                5
#define     ACIKLAMA_COLUMN                  6
#define     BORC_COLUMN                      7
#define     ALACAK_COLUMN                    8
#define     KDV_AYIRMA_BUTON_COLUMN          9
#define     SATIR_TAMAMLA_BUTTON_COLUMN      10

/**************************************************************************************
                   OPEN_MUHASEBE_FISI
***************************************************************************************/

void OPEN_MUHASEBE_FISI ( int p_fis_id, int p_fis_turu,int p_modul_id, QWidget * parent )
{
    MUH_MUHASEBE_FISI * F = new MUH_MUHASEBE_FISI ( p_fis_id, p_fis_turu, p_modul_id, parent );
    F->SHOW( FULL_SCREEN );

}

/**************************************************************************************
                   MUH_MUHASEBE_FISI::MUH_MUHASEBE_FISI
***************************************************************************************/

MUH_MUHASEBE_FISI::MUH_MUHASEBE_FISI ( int fis_id, int fis_turu,int modul_id, QWidget * parent )
: FIS_KERNEL(parent), m_ui ( new Ui::MUH_MUHASEBE_FISI )
{
    m_fis_turu        = fis_turu;
    m_fis_id          = fis_id;
    m_modul_id        = modul_id;

    m_ui->setupUi    ( this );

    START_FIS_KERNEL ( this, DB );
 }

/**************************************************************************************
                   MUH_MUHASEBE_FISI::SETUP_FORM           
***************************************************************************************/

void MUH_MUHASEBE_FISI::SETUP_FORM()
{

    bool is_entegrasyon_fisi = false;

    if ( m_modul_id NE MUHASEBE_MODULU ) {
        is_entegrasyon_fisi = true;
    }

    REGISTER_BUTTONS_WIDGET           ( m_ui->kernel_buttons_widget , false , is_entegrasyon_fisi );
    REGISTER_TABLE_WIDGET             ( m_ui->tablewidget_fis_satirlari,11 );

    m_ui->line_edit_fis_no->setText("*");

    DISABLE_CLEAR_ON_WIDGET ( m_ui->line_edit_fis_no );

    DISABLE_CLEAR_ON_WIDGET ( m_ui->comboBox_fis_filtresi);
    DISABLE_CLEAR_ON_WIDGET ( m_ui->comboBox_sirket_turu);

    M_FIS        = new MUH_FIS_STRUCT;
    M_FIS_SATIRI = new MUH_FIS_SATIRI_STRUCT;

    M_HESABIN_BORC_BAKIYESI   = new double [ E9_ARRAY_SIZE ];
    M_HESABIN_ALACAK_BAKIYESI = new double [ E9_ARRAY_SIZE ];

    if ( E9_KULLANICI_SIRKET_DEGISTIRE_BILIR_MI() EQ EVET ) {

       m_ui->comboBox_fis_filtresi->setHidden( false );
       m_ui->comboBox_sirket_turu->setHidden( false );
       m_ui->label_sirket_turu->setHidden(false);
       m_ui->label_fis_filtresi->setHidden(false);
    }

    QRegExp rx ( "(\\*?)(\\d+)" );
    QRegExpValidator * validator = new QRegExpValidator ( rx, this );
      

    m_ui->line_edit_fis_no->setValidator ( validator );

    // DEFAULT KASA HESABI SETLENIYOR
    DISABLE_CLEAR_ON_WIDGET( m_ui->search_edit_kasa_hesabi );
    DISABLE_CLEAR_ON_WIDGET( m_ui->lineedit_kasa_hesap_ismi );

    if ( m_fis_turu EQ ENUM_TAHSIL_FISI OR m_fis_turu EQ ENUM_TEDIYE_FISI ) {
        m_ui->search_edit_kasa_hesabi->SET_TEXT( MUH_GET_HESAP_KODU( SUBE_GET_UNITE_MUH_KASA_HESABI_ID()) );
        m_ui->lineedit_kasa_hesap_ismi->setText( MUH_GET_HESAP_ISMI( SUBE_GET_UNITE_MUH_KASA_HESABI_ID()) );
    }

    m_kayit_eklendi         = false;
    m_kayit_silindi         = false;
    m_log_kaydi_eklensin_mi = false;

    SET_FIS_ORDER_COLUMN_NUMBER ( ORDER_COLUMN );
    SET_FORM_ID_COLUMN_POSITION ( FIS_ID_COLUMN );
    SET_ROW_ID_COLUMN_POSITION  ( ROW_ID_COLUMN );
    SET_NUMBER_OF_HIDDEN_COLUMN ( 4 );

    SET_FIRST_FOCUS_WIDGET      ( m_ui->line_edit_fis_no );
    SET_LINE_BUTTON_ICON        ( QStringList()<<":/kernel_icons/kernel_ara_ikonu.png" );
    SET_FIS_ORDER_DATABASE      ( "muh_fis_satirlari", "order_number", "fis_satiri_id" );

    SET_TABLE_ROW_WIDGETS ( HESAP_KODU_COLUMN             ,  WIDGET_SEARCH_EDIT  );
    SET_TABLE_ROW_WIDGETS ( HESAP_ISMI_COLUMN             ,  WIDGET_LINE_EDIT    );
    SET_TABLE_ROW_WIDGETS ( BORC_COLUMN                   ,  WIDGET_COMMA_EDIT   );
    SET_TABLE_ROW_WIDGETS ( ALACAK_COLUMN                 ,  WIDGET_COMMA_EDIT   );
    SET_TABLE_ROW_WIDGETS ( ACIKLAMA_COLUMN               ,  WIDGET_LINE_EDIT    );
    SET_TABLE_ROW_WIDGETS ( KDV_AYIRMA_BUTON_COLUMN       ,  WIDGET_PUSH_BUTTON  );
    SET_TABLE_ROW_WIDGETS ( SATIR_TAMAMLA_BUTTON_COLUMN   ,  WIDGET_PUSH_BUTTON  );
    SET_TABLE_ROW_WIDGETS ( HESAP_ID_COLUMN               ,  WIDGET_LINE_EDIT    );

    m_ui->comma_edit_hesap_alacak->SET_RETURN_EMPTY ( true );
    m_ui->comma_hesap_borc->SET_RETURN_EMPTY        ( true );

    REGISTER_SAVER_BUTTON  ( m_ui->button_yazdir      );
    REGISTER_SAVER_BUTTON  ( m_ui->button_fis_kopyala );

    SET_LINE_BUTTON_ICON   ( QStringList () <<""<<"" );

    m_ui->textedit_aciklama->SET_MAX_STRING_LENGTH( 512 );

    REGISTER_BUTTON_SHORT_CUT ( KDV_AYIRMA_BUTON_COLUMN     , QKeySequence ( Qt::CTRL + Qt::Key_R ) );


    QString borc_degisken_adi   = QString("Borç");
    QString alacak_degisken_adi = QString("Alacak");

    if ( m_fis_turu EQ ENUM_TAHSIL_FISI OR m_fis_turu  EQ ENUM_TEDIYE_FISI ) {
        borc_degisken_adi   = QString("Ödeme Tutarı");
        alacak_degisken_adi = QString("Tahsil Edilen");
    }

    m_ui->tablewidget_fis_satirlari->setHorizontalHeaderLabels ( QStringList()<<"id"<<"fis_id"<<"Column Order"
                                                                              <<tr ( "hesap_id")   << tr ( "Hesap Kodu" )
                                                                              <<tr ( "Hesap İsmi" )<< tr ( "Açıklama" )
                                                                              << borc_degisken_adi << alacak_degisken_adi
                                                                              <<tr ( "" )<<tr ( "" )
                                                                              <<tr ( "" )<<tr ( "" ) );


    m_ui->comma_edit_yevmiye_no->setHidden ( true );
    m_ui->label_yevmiye_no->setHidden      ( true );
    m_ui->line_edit_fis_no->setMaxLength   ( 10 );

    m_ui->comma_edit_yevmiye_no->SET_PRECISION(0);

    FIS_EKRANINI_DUZENLE ( m_fis_turu );

    FOCUS_FIRST_WIDGET   ();

    AUTOSTART_FIS_KERNEL ( m_fis_id );
}

/**************************************************************************************
                   MUH_MUHASEBE_FISI::CLEAR_FORM_MEMBERS
***************************************************************************************/

void MUH_MUHASEBE_FISI::CLEAR_FORM_MEMBERS()
{
   m_kasa_hesabi_id = 0;

   MUH_CLEAR_FIS_STRUCT        ( M_FIS );
   MUH_CLEAR_FIS_SATIRI_STRUCT ( M_FIS_SATIRI );

   M_FIS->program_id     = E9_PROGRAMI;
   M_FIS->modul_id       = MUHASEBE_MODULU;

}

/**************************************************************************************
                   MUH_MUHASEBE_FISI::NEW_FIS_RECORD
***************************************************************************************/

void MUH_MUHASEBE_FISI::NEW_FIS_RECORD ()
{
    m_ui->line_edit_fis_no->setText("*");

    FIS_EKRANINI_DUZENLE ( m_fis_turu );
}

/**************************************************************************************
                   MUH_MUHASEBE_FISI::GET_FIS_RECORD
***************************************************************************************/

int MUH_MUHASEBE_FISI::GET_FIS_RECORD ( int fis_id )
{

    MUH_CLEAR_FIS_STRUCT ( M_FIS);

    MUH_FIS_BILGILERINI_OKU(M_FIS , fis_id);

    m_fis_turu = M_FIS->fis_turu;

    FIS_EKRANINI_DUZENLE( M_FIS->fis_turu );

    m_ui->adakDate_fis_tarihi->SET_DATE( M_FIS->fis_tarihi );

    m_muh_fis_tarihi = M_FIS->fis_tarihi;

    m_ui->textedit_aciklama->setText( M_FIS->aciklama );

    m_ui->commaedit_toplam_alacak->SET_DOUBLE ( M_FIS->toplam_alacak );
    m_ui->commaedit_toplam_borc  ->SET_DOUBLE ( M_FIS->toplam_borc );

    m_kasa_hesabi_id     = M_FIS->kasa_hesabi_id;

    if ( M_FIS->yevmiye_no EQ 0 ) {
        m_ui->comma_edit_yevmiye_no->setHidden ( true );
        m_ui->label_yevmiye_no->setHidden ( true );
    }
    else {
        m_ui->comma_edit_yevmiye_no->setHidden ( false );
        m_ui->label_yevmiye_no->setHidden ( false );
        m_ui->comma_edit_yevmiye_no->SET_INTEGER (  M_FIS->yevmiye_no );
    }

    m_ui->line_edit_fis_no->setText ( QVariant(M_FIS->fis_no).toString() );

    SQL_QUERY query (DB);

    if ( m_kasa_hesabi_id > 0 ) {

        query.PREPARE_SELECT("muh_hesaplar" , "tam_hesap_kodu, hesap_ismi" , "hesap_id = :hesap_id");

        query.SET_VALUE      ( ":hesap_id", m_kasa_hesabi_id );

        if ( query.SELECT() EQ 1 ) {
            query.NEXT();
            m_ui->search_edit_kasa_hesabi->SET_TEXT ( query.VALUE(0).toString() );
            m_ui->lineedit_kasa_hesap_ismi->setText ( query.VALUE(1).toString() );
        }
    }


    query.PREPARE_SELECT("muh_fis_satirlari" , "fis_satiri_id, fis_id, hesap_id, aciklama, borc_tutari, "
                         "alacak_tutari,order_number" , "fis_id = :fis_id AND gizli_satir_mi = :gizli_satir_mi");

    query.SET_VALUE      ( ":fis_id", fis_id );
    query.SET_VALUE      ( ":gizli_satir_mi" , 0);

    if ( query.SELECT("order_number ASC") EQ 0 ) {
        return ADAK_OK;
    }

    int p_current_row = 0;

    while ( query.NEXT() EQ true ) {

        p_current_row = ADD_NEW_LINE ();

        double  satir_borc_tutari    = ROUND ( query.VALUE(4).toDouble()  );
        double  satir_alacak_tutari  = ROUND ( query.VALUE(5).toDouble()  );

        QSearchEdit *  search_edit_hesap_kodu = (QSearchEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(p_current_row, HESAP_KODU_COLUMN);
        QLineEdit   * lineedit_hesap_id       = (QLineEdit*) m_ui->tablewidget_fis_satirlari->cellWidget(p_current_row , HESAP_ID_COLUMN);

        lineedit_hesap_id->setText(query.VALUE(2).toString());

        QString tam_hesap_kodu,hesap_ismi;

        MUH_GET_HESAP_KODU_HESAP_ISMI(query.VALUE(2).toInt(),tam_hesap_kodu,hesap_ismi);
        search_edit_hesap_kodu->SET_TEXT (tam_hesap_kodu );

        QCommaEdit  *  comma_edit;

        if ( satir_borc_tutari NE 0 ) {
            comma_edit  =  ( QCommaEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget (  p_current_row, BORC_COLUMN );
            comma_edit->SET_DOUBLE (  QVariant ( satir_borc_tutari ).toDouble() );
        }
        else if ( satir_alacak_tutari NE 0 ) {

            if ( m_fis_turu NE ENUM_TEDIYE_FISI ) {
                comma_edit = ( QCommaEdit *) m_ui->tablewidget_fis_satirlari->cellWidget ( p_current_row,ALACAK_COLUMN );
                comma_edit->SET_DOUBLE(QVariant(satir_alacak_tutari).toDouble() );
            }
        }

        QString fis_satiri_id = query.VALUE(0).toString();
        QString fis_id        = query.VALUE(1).toString();
        QString aciklama      = query.VALUE(3).toString();
        QString order_number  = query.VALUE(6).toString();


        QLineEdit * aciklama_lineEdit = ( QLineEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget( p_current_row, ACIKLAMA_COLUMN );
        aciklama_lineEdit->setText ( aciklama );

        QLineEdit * lineedit_hesap_ismi = ( QLineEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(p_current_row , HESAP_ISMI_COLUMN);
        lineedit_hesap_ismi->setText(hesap_ismi);

        QTableWidgetItem * new_item;

        new_item = new QTableWidgetItem ( tr ( "%1" ).arg ( fis_satiri_id ) );
        m_ui->tablewidget_fis_satirlari->setItem ( p_current_row, ROW_ID_COLUMN, new_item );

        new_item = new QTableWidgetItem ( tr ( "%1").arg ( fis_id ) );
        m_ui->tablewidget_fis_satirlari->setItem ( p_current_row, FIS_ID_COLUMN, new_item );

        new_item = new QTableWidgetItem ( tr ( "%1" ).arg ( order_number ) );
        m_ui->tablewidget_fis_satirlari->setItem ( p_current_row, ORDER_COLUMN, new_item );
    }
    return ADAK_OK;
}

/**************************************************************************************
                   MUH_MUHASEBE_FISI::SET_LINE_DEFAULTS
***************************************************************************************/

void MUH_MUHASEBE_FISI::SET_LINE_DEFAULTS ( int row_number )
{
   QLineEdit   * lineedit_aciklama    = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, ACIKLAMA_COLUMN             );
   QPushButton * button_kdv_ayirma    = ( QPushButton * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, KDV_AYIRMA_BUTON_COLUMN     );
   QPushButton * button_satir_tamamla = ( QPushButton * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, SATIR_TAMAMLA_BUTTON_COLUMN );
   QLineEdit   * lineedit_hesap_ismi  = ( QLineEdit *   ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, HESAP_ISMI_COLUMN           );


   lineedit_hesap_ismi->setDisabled(true);
   lineedit_hesap_ismi->setMaxLength( 128 );
   lineedit_aciklama->setMaxLength ( 128 );

   REGISTER_LINE_CHANGER_BUTTON ( row_number, SATIR_TAMAMLA_BUTTON_COLUMN );
   REGISTER_LINE_SAVER_BUTTON   ( row_number, KDV_AYIRMA_BUTON_COLUMN     );

   button_satir_tamamla->setText ( "T" );
   button_kdv_ayirma->setText ( "KDV" );

   button_satir_tamamla->setToolTip ( tr ( "Fişi dengeleyecek yeni bir satır ekler." ) );
   button_kdv_ayirma->setToolTip ( tr ( "Kdv ayırma işlemi için tıklayınız." ) );
   QFont font = button_kdv_ayirma->font();
   font.setPointSize(7);
   button_kdv_ayirma->setFont(font);

   QCommaEdit  * alacak_commaEdit  = static_cast < QCommaEdit * > (  m_ui->tablewidget_fis_satirlari->cellWidget ( row_number,ALACAK_COLUMN ) );
   QCommaEdit  * borc_commaEdit    = static_cast < QCommaEdit * > (  m_ui->tablewidget_fis_satirlari->cellWidget ( row_number,BORC_COLUMN  ) );

   if (m_fis_turu EQ ENUM_TEDIYE_FISI) {
       alacak_commaEdit->setReadOnly(true);
   }
   else if (m_fis_turu EQ  ENUM_TAHSIL_FISI) {
       borc_commaEdit->setReadOnly(true);
   }

   alacak_commaEdit->setMaxLength (20);
   borc_commaEdit  ->setMaxLength (20);

   QSearchEdit * search_edit  = static_cast < QSearchEdit * > ( m_ui->tablewidget_fis_satirlari->cellWidget ( row_number,HESAP_KODU_COLUMN ) );
   search_edit->SET_MAX_LENGTH (30);
}

/**************************************************************************************
                   MUH_MUHASEBE_FISI::CHECK_FIS_FORM_VAR
***************************************************************************************/

int MUH_MUHASEBE_FISI::CHECK_FIS_FORM_VAR ( QObject * object )
{
    SQL_QUERY sql_query ( DB );

    if ( object EQ m_ui->search_edit_kasa_hesabi ) {

        if ( m_ui->search_edit_kasa_hesabi->GET_TEXT().isEmpty() EQ true ) {
            m_ui->lineedit_kasa_hesap_ismi->clear();
            m_kasa_hesabi_id = 0;
            return ADAK_OK;
        }

        sql_query.PREPARE_SELECT("muh_hesaplar" , "hesap_ismi, tali_hesap, hesap_id" , "tam_hesap_kodu = :tam_hesap_kodu");

        sql_query.SET_VALUE      ( ":tam_hesap_kodu", m_ui->search_edit_kasa_hesabi->GET_TEXT() );

        if ( sql_query.SELECT() EQ 0 ) {
            MSG_WARNING(  "Muhasebe hesabı bulunamadı", m_ui->textedit_aciklama);

            return ADAK_FAIL_UNDO;
        }
        sql_query.NEXT();

        if ( sql_query.VALUE(1).toInt() EQ 0 ) {
            MSG_WARNING( tr("Seçtiğiniz hesap tali değil , işlem yapamazsınız."), m_ui->search_edit_kasa_hesabi );

            return ADAK_FAIL_UNDO;
        }

        m_ui->lineedit_kasa_hesap_ismi->setText ( sql_query.VALUE(0).toString() );
        m_kasa_hesabi_id = sql_query.VALUE(2).toInt();
    }
    else if ( object EQ m_ui->textedit_aciklama ) {
        if ( m_ui->textedit_aciklama->toPlainText().size() > 512 ) {
            MSG_WARNING( tr ( "Açıklama 512 karakterden daha uzun olamaz!..." ), m_ui->textedit_aciklama );

            return ADAK_FAIL;
        }
    }

    return ADAK_OK;
}

/**************************************************************************************
                   MUH_MUHASEBE_FISI::CHECK_FIS_FORM_EMPTY
***************************************************************************************/

int MUH_MUHASEBE_FISI::CHECK_FIS_FORM_EMPTY ()
{

    if ( m_ui->line_edit_fis_no->text().isEmpty() EQ true ) {
        MSG_WARNING( tr ( "Yeni fiş kaydı için (*) girmelisiniz!.." ), m_ui->line_edit_fis_no );

        return ADAK_FAIL;
    }
    if ( m_fis_turu EQ ENUM_TAHSIL_FISI OR m_fis_turu EQ ENUM_TEDIYE_FISI ) {

        if ( m_ui->lineedit_kasa_hesap_ismi->text().isEmpty() EQ true ) {
            MSG_WARNING( tr ( "Kasa hesabını boş bırakamazsınız!.." ), m_ui->lineedit_kasa_hesap_ismi );

            return ADAK_FAIL;
        }
    }

    return ADAK_OK;
}

/**************************************************************************************
                   MUH_MUHASEBE_FISI::CHECK_FIS_RECORD_ADD
***************************************************************************************/

int MUH_MUHASEBE_FISI::CHECK_FIS_RECORD_ADD ()
{

    if ( QString ( m_ui->line_edit_fis_no->text().at(0)) NE "*" ) {
        int fis_no = m_ui->line_edit_fis_no->text().toInt();
        SQL_QUERY query ( DB );

        query.PREPARE_SELECT("muh_fisler" , "fis_id" , "fis_no = :fis_no "
                             "AND fis_tarihi = :fis_tarihi");

        query.SET_VALUE      ( ":fis_no"       ,fis_no );
        query.SET_VALUE      ( ":fis_tarihi"   ,m_ui->adakDate_fis_tarihi->DATE() );

        if ( query.SELECT() NE 0 ) {
            MSG_WARNING( tr ( "Aynı numarada başka bir fiş var.Fiş eklenemedi!.." ), m_ui->line_edit_fis_no );
            return ADAK_FAIL;
        }
    }

    return ADAK_OK;
}

/**************************************************************************************
                   MUH_MUHASEBE_FISI::ADD_FIS_RECORD
***************************************************************************************/

int MUH_MUHASEBE_FISI::ADD_FIS_RECORD ()
{
    int          fis_no       = 0;

    QString fis_tarihi = m_ui->adakDate_fis_tarihi->DATE();

    if ( QString ( m_ui->line_edit_fis_no->text().at(0) ) EQ "*") {
        fis_no = MUH_SIRADAKI_FIS_NO_AL ( fis_tarihi );
    }
    else {
        fis_no = m_ui->line_edit_fis_no->text().toInt();
    }

    M_FIS->program_id     = E9_PROGRAMI;
    M_FIS->modul_id       = MUHASEBE_MODULU;
    M_FIS->fis_no         = fis_no;
    M_FIS->fis_tarihi     = m_ui->adakDate_fis_tarihi->DATE();
    M_FIS->aciklama       = m_ui->textedit_aciklama->toPlainText();
    M_FIS->kasa_hesabi_id = m_kasa_hesabi_id;
    M_FIS->fis_turu       = m_fis_turu;


    int fis_id = MUH_FIS_EKLE ( M_FIS );

    m_ui->line_edit_fis_no->setText ( QVariant(fis_no).toString() );

    m_log_kaydi_eklensin_mi = true;
    m_kayit_eklendi         = true;

    m_log_detaylari = QObject::tr ( "Log Türü - Muhasebe Fişleri," );

    m_log_detaylari.append ( MUH_GET_FIS_TURLERI_STRING(m_fis_turu) + "," +
                             m_ui->adakDate_fis_tarihi->QDATE().toString("dd MMMM YYYY") +
                             ",Fiş No : "  + QVariant ( fis_no ).toString() );

    return fis_id;
}

/**************************************************************************************
                   MUH_MUHASEBE_FISI::CHECK_FIS_RECORD_UPDATE
***************************************************************************************/

int MUH_MUHASEBE_FISI::CHECK_FIS_RECORD_UPDATE ( int fis_id )
{

    if ( m_ui->line_edit_fis_no->text() NE "*") {
        int fis_no = m_ui->line_edit_fis_no->text().toInt();

        SQL_QUERY query ( DB );

        query.PREPARE_SELECT("muh_fisler" , "fis_id" , "fis_no = :fis_no "
                             "AND fis_tarihi = :fis_tarihi");

        query.SET_VALUE      ( ":fis_no"       , fis_no );
        query.SET_VALUE      ( ":fis_tarihi"   , m_ui->adakDate_fis_tarihi->DATE() );

        if ( query.SELECT() > 0 ) {
            query.NEXT();

            if ( query.VALUE(0).toInt() NE fis_id ) {
                MSG_WARNING( tr("Aynı fiş numarası ile kayıtlı fiş bulundu.Fiş kaydı yapılamadı!.."), m_ui->line_edit_fis_no );

                return ADAK_FAIL;
            }
        }
    }

    if( m_muh_fis_tarihi NE m_ui->adakDate_fis_tarihi->DATE() ) {
        int secim = MSG_YES_NO( "Tarih değişti kaydedilsin mi ?", m_ui->adakDate_fis_tarihi );
        if ( secim EQ ADAK_NO ) {
            return ADAK_FAIL;
        }
    }


    return ADAK_OK;
}

/**************************************************************************************
                   MUH_MUHASEBE_FISI::UPDATE_FIS_RECORD
***************************************************************************************/

void MUH_MUHASEBE_FISI::UPDATE_FIS_RECORD ( int fis_id )
{
    int fis_no = 0;

    if ( QString ( m_ui->line_edit_fis_no->text().at(0) ) EQ "*") {
        fis_no = MUH_SIRADAKI_FIS_NO_AL ( m_ui->adakDate_fis_tarihi->DATE() );
    }
    else {
        fis_no = m_ui->line_edit_fis_no->text().toInt();
    }

    M_FIS->fis_no           = fis_no;
    M_FIS->fis_tarihi       = m_ui->adakDate_fis_tarihi->DATE();
    M_FIS->kasa_hesabi_id   = m_kasa_hesabi_id;
    M_FIS->aciklama         = m_ui->textedit_aciklama->toPlainText();
    M_FIS->program_id       = E9_PROGRAMI;
    M_FIS->modul_id         = MUHASEBE_MODULU;

    MUH_FISI_GUNCELLE ( M_FIS , fis_id);

    m_log_kaydi_eklensin_mi = true;

    m_log_detaylari = QObject::tr ( "Log Türü - Muhasebe Fişleri," );

    m_log_detaylari.append ( MUH_GET_FIS_TURLERI_STRING ( m_fis_turu ) + "," + m_ui->adakDate_fis_tarihi->QDATE().toString("dd MMMM YYYY") + ",Fiş No : "  +
                         QVariant ( fis_no ).toString() + ",Toplam Borç : " +
                         m_ui->commaedit_toplam_borc->GET_TEXT() + ",Toplam Alacak : " +
                         m_ui->commaedit_toplam_alacak->GET_TEXT() );

}

/**************************************************************************************
                   MUH_MUHASEBE_FISI::CHECK_FIS_RECORD_DELETE
***************************************************************************************/

int MUH_MUHASEBE_FISI::CHECK_FIS_RECORD_DELETE ( int fis_id )
{
    Q_UNUSED(fis_id);

    return ADAK_OK;
}

/**************************************************************************************
                     MUH_MUHASEBE_FISI::DELETE_FIS_RECORD
***************************************************************************************/

void MUH_MUHASEBE_FISI::DELETE_FIS_RECORD ( int fis_id )
{

    M_FIS->program_id     = E9_PROGRAMI;
    M_FIS->modul_id       = MUHASEBE_MODULU;
    MUH_FIS_SIL ( M_FIS , fis_id );

    m_log_kaydi_eklensin_mi = true;
    m_kayit_silindi         = true;

    QDate fis_tarihi = QDate::fromString(M_FIS->fis_tarihi , "yyyy.MM.dd");

    m_log_detaylari = QObject::tr ( "Log Türü - Muhasebe Fişleri," );

    m_log_detaylari.append ( MUH_GET_FIS_TURLERI_STRING ( m_fis_turu ) + "," + fis_tarihi.toString ( "dd MMMM yyyy dddd" )
                           + ",Fiş No : " + QVariant(M_FIS->fis_no).toString()
                           + ",Toplam Borç : "  + VIRGUL_EKLE ( QVariant ( ROUND  ( M_FIS->toplam_borc    ) ).toString() )
                           + ",Toplam Alacak :" + VIRGUL_EKLE ( QVariant ( ROUND ( M_FIS->toplam_alacak  ) ).toString()));


}

/**************************************************************************************
                   MUH_MUHASEBE_FISI::SELECT_FIS_RECORD
***************************************************************************************/

int MUH_MUHASEBE_FISI::SELECT_FIS_RECORD ()
{
    return OPEN_MUH_FIS_ARAMA ( 0, m_modul_id, m_fis_turu, ENUM_MUHASEBE_FISI, this );
}

/**************************************************************************************
                   MUH_MUHASEBE_FISI::FIND_FIS_RECORD
***************************************************************************************/

int MUH_MUHASEBE_FISI::FIND_FIS_RECORD ()
{
    SQL_QUERY query ( DB );

    if ( m_ui->line_edit_fis_no->text() EQ "*" ) {
        MSG_WARNING( tr ( "Fiş No (*) olamaz !.." ), m_ui->line_edit_fis_no );
        return ADAK_FAIL;
    }


    query.PREPARE_SELECT  ( "muh_fisler" , "fis_id" ,"fis_no = :fis_no AND "
                           "fis_tarihi  = :fis_tarihi AND fis_turu = :fis_turu" );

    query.SET_VALUE      (  ":fis_no"          , m_ui->line_edit_fis_no->text() );
    query.SET_VALUE      (  ":fis_tarihi"      , m_ui->adakDate_fis_tarihi->DATE() );
    query.SET_VALUE      (  ":fis_turu"        , m_fis_turu);

    if (  m_modul_id NE 0 ) {
        query.AND_EKLE("modul_id = :modul_id");
        query.SET_VALUE      ( ":modul_id"        , m_modul_id);
    }
    else {
        query.AND_EKLE("program_id != :program_id");
        query.SET_VALUE      ( ":program_id"     , E9_PROGRAMI);
    }

    if ( query.SELECT() EQ 0 ) {
        MSG_WARNING( tr ( "Fiş bulunamadı!.." ), m_ui->line_edit_fis_no );

        return 0;
    }
    query.NEXT();

    return query.VALUE(0).toInt();
}

/**************************************************************************************
                   MUH_MUHASEBE_FISI::FIND_FIRST_FIS_RECORD
***************************************************************************************/

int MUH_MUHASEBE_FISI::FIND_FIRST_FIS_RECORD ()
{
    SQL_QUERY query(DB);

    query.PREPARE_SELECT("muh_fisler" ,"fis_id" , "fis_turu = :fis_turu");
    query.SET_VALUE(":fis_turu" , m_fis_turu);

    if ( m_modul_id NE 0 ) {
        query.AND_EKLE("modul_id = :modul_id");
        query.SET_VALUE(":modul_id" , m_modul_id);
    }
    else {
        query.AND_EKLE("program_id != :program_id");
        query.SET_VALUE(":program_id" , E9_PROGRAMI);
    }

    if ( query.SELECT("fis_tarihi ASC , fis_no ASC" , 0 , 1) EQ 0 ) {
        return 0;
    }
    query.NEXT();

    return query.VALUE(0).toInt();
}

/**************************************************************************************
                   MUH_MUHASEBE_FISI::FIND_LAST_FIS_RECORD
***************************************************************************************/

int MUH_MUHASEBE_FISI::FIND_LAST_FIS_RECORD ()
{
    SQL_QUERY query(DB);

    query.PREPARE_SELECT("muh_fisler" ,"fis_id" , "fis_turu = :fis_turu");
    query.SET_VALUE(":fis_turu" , m_fis_turu);

    if ( m_modul_id NE 0 ) {
        query.AND_EKLE("modul_id = :modul_id");
        query.SET_VALUE(":modul_id" , m_modul_id);
    }
    else {
        query.AND_EKLE("program_id != :program_id");
        query.SET_VALUE(":program_id" , E9_PROGRAMI);
    }

    if ( query.SELECT("fis_tarihi DESC , fis_no DESC" , 0 , 1) EQ 0 ) {
        return 0;
    }
    query.NEXT();

    return query.VALUE(0).toInt();
}

/**************************************************************************************
                   MUH_MUHASEBE_FISI::FIND_PREV_FIS_RECORD
***************************************************************************************/

int MUH_MUHASEBE_FISI::FIND_PREV_FIS_RECORD ()
{
    SQL_QUERY  query ( DB );

    if ( m_is_entegrasyon_fisi EQ true ) {

        query.PREPARE_SELECT("muh_fisler" , "fis_id" , "fis_tarihi = :fis_tarihi "
                             "AND fis_no < :fis_no AND fis_turu = :fis_turu");

        query.SET_VALUE ( ":fis_tarihi"  , m_ui->adakDate_fis_tarihi->DATE() );
        query.SET_VALUE ( ":fis_no"      , m_ui->line_edit_fis_no->text().toInt() );
        query.SET_VALUE ( ":fis_turu"    , m_fis_turu);

        if ( m_modul_id NE 0 ) {
            query.AND_EKLE ( "modul_id = :modul_id");
            query.SET_VALUE( ":modul_id" , m_modul_id  );
        }
        else {
            query.AND_EKLE ("program_id != :program_id");
            query.SET_VALUE( ":program_id" , E9_PROGRAMI);
        }

    }
    else {

        query.PREPARE_SELECT("muh_fisler" , "fis_id,fis_turu" ,"fis_turu           = :fis_turu "
                             "AND modul_id       = :modul_id "
                             "AND fis_tarihi     = :fis_tarihi "
                             "AND fis_no < :fis_no ");

        query.SET_VALUE            ( ":fis_turu"       , m_fis_turu );
        query.SET_VALUE            ( ":modul_id"       , m_modul_id  );
        query.SET_VALUE            ( ":fis_tarihi"     , m_ui->adakDate_fis_tarihi->DATE() );
        query.SET_VALUE            ( ":fis_no"         , m_ui->line_edit_fis_no->text().toInt()                      );

    }

    if ( query.SELECT("fis_no DESC" , 0 , 1) > 0 ) {

        query.NEXT();

        return query.VALUE(0).toInt();
    }


    if ( m_is_entegrasyon_fisi EQ true ) {

        query.PREPARE_SELECT("muh_fisler" , "fis_id" , " fis_tarihi   < :fis_tarihi "
                             "AND fis_turu = :fis_turu");
        query.SET_VALUE     (  ":fis_tarihi"    , m_ui->adakDate_fis_tarihi->DATE() );
        query.SET_VALUE     (  ":fis_turu"      , m_fis_turu);

        if ( m_modul_id NE 0 ) {
            query.AND_EKLE ( "modul_id = :modul_id");
            query.SET_VALUE ( ":modul_id"     , m_modul_id );
        }
        else {
            query.AND_EKLE ( "program_id != :program_id");
            query.SET_VALUE ( ":program_id"   , E9_PROGRAMI);
        }
    }
    else {

        query.PREPARE_SELECT("muh_fisler" , "fis_id,fis_turu" ,"fis_turu   = :fis_turu "
                             "AND modul_id  = :modul_id "
                             "AND fis_tarihi   < :fis_tarihi");

        query.SET_VALUE     (  ":fis_turu"      , m_fis_turu );
        query.SET_VALUE     (  ":modul_id"      , m_modul_id );
        query.SET_VALUE     (  ":fis_tarihi"    , m_ui->adakDate_fis_tarihi->DATE() );
    }

    if ( query.SELECT("fis_tarihi DESC,fis_no DESC" , 0, 1) EQ 0 ) {
        return 0;
    }
    query.NEXT();

    return query.VALUE(0).toInt();

}

/**************************************************************************************
                   MUH_MUHASEBE_FISI::FIND_NEXT_FIS_RECORD
***************************************************************************************/

int MUH_MUHASEBE_FISI::FIND_NEXT_FIS_RECORD ()
{
    SQL_QUERY  query ( DB );

    if ( m_is_entegrasyon_fisi EQ true ) {

        query.PREPARE_SELECT("muh_fisler" , "fis_id" , "fis_tarihi   = :fis_tarihi "
                             "AND fis_no > :fis_no AND fis_turu = :fis_turu");

        query.SET_VALUE ( ":fis_tarihi"  , m_ui->adakDate_fis_tarihi->DATE() );
        query.SET_VALUE ( ":fis_no"      , m_ui->line_edit_fis_no->text().toInt() );
        query.SET_VALUE ( ":fis_turu"    , m_fis_turu);

        if ( m_modul_id NE 0 ) {
            query.AND_EKLE ( "modul_id = :modul_id");
            query.SET_VALUE( ":modul_id"    , m_modul_id  );
        }
        else {
            query.AND_EKLE ( "program_id != :program_id");
            query.SET_VALUE( ":program_id" , E9_PROGRAMI);
        }

    }
    else {

        query.PREPARE_SELECT("muh_fisler" , "fis_id,fis_turu" , "fis_turu   = :fis_turu "
                             "AND modul_id  = :modul_id "
                             "AND fis_tarihi   = :fis_tarihi "
                             "AND fis_no > :fis_no ");

        query.SET_VALUE            ( ":fis_turu"       , m_fis_turu );
        query.SET_VALUE            ( ":modul_id"       , m_modul_id  );
        query.SET_VALUE            ( ":fis_tarihi"     , m_ui->adakDate_fis_tarihi->DATE());
        query.SET_VALUE            ( ":fis_no"         , m_ui->line_edit_fis_no->text().toInt()                      );

    }

    if ( query.SELECT("fis_no" , 0 , 1) > 0 ) {

        query.NEXT();

        return query.VALUE(0).toInt();
    }

    if ( m_is_entegrasyon_fisi EQ true ) {

        query.PREPARE_SELECT("muh_fisler" , "fis_id" ," fis_tarihi   > :fis_tarihi AND "
                             "fis_turu = :fis_turu");
        query.SET_VALUE (  ":fis_tarihi"    , m_ui->adakDate_fis_tarihi->DATE() );
        query.SET_VALUE ( ":fis_turu" , m_fis_turu);

        if ( m_modul_id NE 0 ) {
            query.AND_EKLE   ("modul_id = :modul_id");
            query.SET_VALUE ( ":modul_id"     , m_modul_id );
        }
        else {
            query.AND_EKLE("program_id != :program_id");
            query.SET_VALUE ( ":program_id"   , E9_PROGRAMI);
        }
    }
    else {

        query.PREPARE_SELECT("muh_fisler" , "fis_id,fis_turu" ,"fis_turu   = :fis_turu "
                             "AND modul_id  = :modul_id "
                             "AND fis_tarihi  > :fis_tarihi");

        query.SET_VALUE     (  ":fis_turu"      , m_fis_turu );
        query.SET_VALUE     (  ":modul_id"      , m_modul_id );
        query.SET_VALUE     (  ":fis_tarihi"    , m_ui->adakDate_fis_tarihi->DATE() );
    }

    if ( query.SELECT("fis_tarihi,fis_no" , 0, 1) EQ 0 ) {
        return 0;
    }
    query.NEXT();

    return query.VALUE(0).toInt();
}

/**************************************************************************************
                   MUH_MUHASEBE_FISI::CHECK_LINE_VAR
***************************************************************************************/

int MUH_MUHASEBE_FISI::CHECK_LINE_VAR ( int row_number, QObject * object )
{
    QSearchEdit * searchedit_hesap_kodu  = ( QSearchEdit *) m_ui->tablewidget_fis_satirlari->cellWidget(row_number , HESAP_KODU_COLUMN );
    QCommaEdit * commaedit_alacak_tutari = ( QCommaEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number , ALACAK_COLUMN     );
    QCommaEdit * commaedit_borc_tutari   = ( QCommaEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number , BORC_COLUMN       );
    QLineEdit * lineedit_hesap_id        = ( QLineEdit *  ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number , HESAP_ID_COLUMN   );
    QLineEdit * lineedit_hesap_ismi      = ( QLineEdit *  ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number , HESAP_ISMI_COLUMN );

    SQL_QUERY query ( DB );


    if ( object EQ searchedit_hesap_kodu ) {

        if ( searchedit_hesap_kodu->GET_TEXT().isEmpty() EQ true ) {

            lineedit_hesap_ismi->clear();
            searchedit_hesap_kodu->SET_TEXT("");
            searchedit_hesap_kodu->setFocus();
            lineedit_hesap_id->setText("0");
            return ADAK_OK;

        }

        query.PREPARE_SELECT("muh_hesaplar" , "tali_hesap , hesap_ismi , hesap_id, toplam_borc_array , toplam_alacak_array" ,
                             "tam_hesap_kodu = :tam_hesap_kodu");

        query.SET_VALUE(":tam_hesap_kodu" , searchedit_hesap_kodu->GET_TEXT());

        if ( query.SELECT() EQ 0 ) {
            MSG_WARNING(  tr("Aradığınız muhasebe hesabı bulunamadı."), searchedit_hesap_kodu );

            return ADAK_FAIL;
        }
        query.NEXT();

        int tali_hesap_mi = query.VALUE(0).toInt();

        if ( tali_hesap_mi EQ 0 ) {
            MSG_WARNING(  tr("Seçtiğiniz hesap tali değil,işlem yapamazsınız!.."), searchedit_hesap_kodu );

            return ADAK_FAIL;
        }

        lineedit_hesap_ismi->setText(query.VALUE(1).toString());
        lineedit_hesap_id->setText(query.VALUE(2).toString());

        UNPACK_DOUBLE_ARRAY ( query.VALUE(3).toString(),M_HESABIN_BORC_BAKIYESI   , E9_ARRAY_SIZE );
        UNPACK_DOUBLE_ARRAY ( query.VALUE(4).toString(),M_HESABIN_ALACAK_BAKIYESI , E9_ARRAY_SIZE );

        double hesap_toplam_borc   = M_HESABIN_BORC_BAKIYESI   [ E9_SON_BAKIYE ];
        double hesap_toplam_alacak = M_HESABIN_ALACAK_BAKIYESI [ E9_SON_BAKIYE ];

        double borc_bakiye   = 0.0;
        double alacak_bakiye = 0.0;

        if ( hesap_toplam_borc > hesap_toplam_alacak ) {
            borc_bakiye   = hesap_toplam_borc - hesap_toplam_alacak;

        }
        else {

            alacak_bakiye = hesap_toplam_alacak - hesap_toplam_borc;
        }

        m_ui->comma_hesap_borc->SET_DOUBLE(borc_bakiye);
        m_ui->comma_edit_hesap_alacak->SET_DOUBLE(alacak_bakiye);

        return ADAK_OK;
    }
    else if ( object EQ commaedit_borc_tutari ) {
        if  ( m_fis_turu EQ ENUM_TAHSIL_FISI ) {
            commaedit_borc_tutari->clear();
            MSG_WARNING(  tr ( "Tahsil Fişinde borç bilgisi girilemez." ), NULL );
            return ADAK_OK;
        }
    }
    else if ( object EQ commaedit_alacak_tutari ) {
        if  ( m_fis_turu EQ ENUM_TEDIYE_FISI ) {
            commaedit_alacak_tutari->clear();
            MSG_WARNING(  tr ( "Tediye Fişinde alacak bilgisi girilemez." ), NULL );
            return ADAK_OK;
        }
    }


    return ADAK_OK;
}

/**************************************************************************************
                   MUH_MUHASEBE_FISI::SELECTED_LINE
***************************************************************************************/

void MUH_MUHASEBE_FISI::SELECTED_LINE ( int row_number, int row_save_status )
{
    Q_UNUSED ( row_save_status );

    QSearchEdit * hesap_kodu_search_edit  = ( QSearchEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget ( row_number, HESAP_KODU_COLUMN );

    SQL_QUERY query ( DB );

    m_ui->comma_edit_hesap_alacak->setHidden ( false );
    m_ui->comma_hesap_borc->setHidden        ( false );
    m_ui->label_alacak_bakiye->setHidden     ( false );
    m_ui->label_borc_bakiye->setHidden       ( false );

    double borc_bakiye   = 0.00;
    double alacak_bakiye = 0.00;

    QString hesap_kodu = hesap_kodu_search_edit->GET_TEXT();

    query.PREPARE_SELECT("muh_hesaplar" , "toplam_borc_array,toplam_alacak_array ",
                        "tam_hesap_kodu = :tam_hesap_kodu");

    query.SET_VALUE      ( ":tam_hesap_kodu",hesap_kodu );

    if ( query.SELECT() NE 0 ) {

        query.NEXT();

        UNPACK_DOUBLE_ARRAY ( query.VALUE(2).toString(),M_HESABIN_BORC_BAKIYESI   , E9_ARRAY_SIZE );
        UNPACK_DOUBLE_ARRAY ( query.VALUE(3).toString(),M_HESABIN_ALACAK_BAKIYESI , E9_ARRAY_SIZE );

        int     bts_gun_no          = m_ui->adakDate_fis_tarihi->MALI_YIL_ARRAY_INDIS();

        double  hesap_toplam_borc   = M_HESABIN_BORC_BAKIYESI   [ bts_gun_no ];
        double  hesap_toplam_alacak = M_HESABIN_ALACAK_BAKIYESI [ bts_gun_no ];

        if (hesap_toplam_borc > hesap_toplam_alacak) {
            borc_bakiye   = hesap_toplam_borc - hesap_toplam_alacak;
        }
        else {
            alacak_bakiye = hesap_toplam_alacak - hesap_toplam_borc;
        }

    }

    m_ui->comma_hesap_borc->SET_DOUBLE        ( borc_bakiye   );
    m_ui->comma_edit_hesap_alacak->SET_DOUBLE ( alacak_bakiye );
}


/**************************************************************************************
                   MUH_MUHASEBE_FISI::CHECK_LINE_EMPTY
***************************************************************************************/

int MUH_MUHASEBE_FISI::CHECK_LINE_EMPTY ( int row_number )
{
    QWidget   * cell_widget   = m_ui->tablewidget_fis_satirlari->cellWidget ( row_number,HESAP_KODU_COLUMN );
    QSearchEdit * cell_search_edit = static_cast<QSearchEdit*> ( cell_widget );

    cell_widget               = m_ui->tablewidget_fis_satirlari->cellWidget ( row_number,BORC_COLUMN );
    QCommaEdit* comma_edit    = static_cast<QCommaEdit *> ( cell_widget );
    double borc_tutari        = comma_edit->GET_DOUBLE();

    cell_widget               = m_ui->tablewidget_fis_satirlari->cellWidget ( row_number,ALACAK_COLUMN );
    comma_edit                = static_cast<QCommaEdit *> ( cell_widget );
    double alacak_tutari      = comma_edit->GET_DOUBLE();

    if ( cell_search_edit->GET_TEXT().isEmpty() EQ true ) {
        MSG_WARNING(  tr ( "Hesap kodunu boş bırakamazsınız!.." ), cell_search_edit );

        return ADAK_FAIL;
    }

    switch ( m_fis_turu)  {
        case ENUM_MAHSUP_FISI  :
        case ENUM_ACILIS_FISI  :
        case ENUM_KAPANIS_FISI :
            if ( borc_tutari EQ 0 AND alacak_tutari EQ 0 ) {

                MSG_WARNING( tr ( "Borç veya Alacak tutarı girmelisiniz!.." ), comma_edit );

                return ADAK_FAIL;
            }
            if ( borc_tutari NE 0 AND alacak_tutari NE 0 ) {

                MSG_WARNING( tr ( "Satıra hem borç,hem de alacak tutarı giremezsiniz!.."), comma_edit );

                return ADAK_FAIL;
            }
            break;
       case  ENUM_TAHSIL_FISI  :
            if ( alacak_tutari EQ 0 ) {

                MSG_WARNING( tr ( "Alacak tutarını girmelisiniz!.." ), comma_edit );

                return ADAK_FAIL;
            }
            break;
       case  ENUM_TEDIYE_FISI  :
            if ( borc_tutari EQ 0 ) {

                MSG_WARNING( tr ( "Borç tutarını girmelisiniz!.." ), comma_edit );

                return ADAK_FAIL;
            }
            break;
    }

    return ADAK_OK;
}

/**************************************************************************************
                   MUH_MUHASEBE_FISI::CHECK_ADD_LINE
***************************************************************************************/

int MUH_MUHASEBE_FISI::CHECK_ADD_LINE ( int fis_id, int row_number )
{
    Q_UNUSED ( fis_id );

    if ( m_fis_turu EQ ENUM_TEDIYE_FISI ) {

        if ( KASA_ALACAK_BAKIYE_VERIYOR_MU(row_number) EQ true ) {


            int msg_secim = MSG_YES_NO( tr ("Kasa hesabı alacak bakiye veriyor. \n Emin misiniz?" ), NULL );

            if ( msg_secim EQ ADAK_NO ) {

                return ADAK_FAIL;
            }
        }
    }

    return CHECK_ALARMLAR(row_number);
}

/**************************************************************************************
                   MUH_MUHASEBE_FISI::ADD_LINE
***************************************************************************************/

void MUH_MUHASEBE_FISI::ADD_LINE ( int fis_id, int row_number )
{

    QCommaEdit  * commaEdit_satir_borc_tutari   = ( QCommaEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number,BORC_COLUMN          );
    QCommaEdit  * commaEdit_satir_alacak_tutari = ( QCommaEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number,ALACAK_COLUMN        );
    QLineEdit   * lineEdit_aciklama             = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget( row_number, ACIKLAMA_COLUMN    );
    QLineEdit   * lineedit_hesap_id             = ( QLineEdit *) m_ui->tablewidget_fis_satirlari->cellWidget(row_number , HESAP_ID_COLUMN);

    M_FIS_SATIRI->fis_id               = fis_id;
    M_FIS_SATIRI->aciklama             = lineEdit_aciklama->text();
    M_FIS_SATIRI->hesap_id             = lineedit_hesap_id->text().toInt();
    M_FIS_SATIRI->borc_tutari          = commaEdit_satir_borc_tutari->GET_DOUBLE();
    M_FIS_SATIRI->alacak_tutari        = commaEdit_satir_alacak_tutari->GET_DOUBLE();
    M_FIS_SATIRI->order_number         = m_ui->tablewidget_fis_satirlari->item ( row_number, ORDER_COLUMN )->text().toInt();
    M_FIS_SATIRI->base_record_id       = 0;
    M_FIS_SATIRI->modul_id             = M_FIS->modul_id;
    M_FIS_SATIRI->program_id           = M_FIS->program_id;

    int fis_satiri_id = MUH_FIS_SATIRI_EKLE ( M_FIS , M_FIS_SATIRI);

    QTableWidgetItem * new_item;

    new_item = new QTableWidgetItem ( QVariant ( fis_satiri_id ).toString() );
    m_ui->tablewidget_fis_satirlari->setItem ( row_number, ROW_ID_COLUMN, new_item );

    new_item = new QTableWidgetItem( QVariant ( fis_id ).toString() );
    m_ui->tablewidget_fis_satirlari->setItem( row_number, FIS_ID_COLUMN, new_item );

    m_ui->commaedit_toplam_borc->SET_DOUBLE   ( M_FIS->toplam_borc   );
    m_ui->commaedit_toplam_alacak->SET_DOUBLE ( M_FIS->toplam_alacak );

    m_ui->comma_edit_hesap_alacak->clear();
    m_ui->comma_hesap_borc->clear();

    m_log_kaydi_eklensin_mi = true;

    m_log_detaylari = QObject::tr("Log Türü - Muhasebe Fişleri,");

    QDate fis_tarihi = QDate::fromString(M_FIS->fis_tarihi , "yyyy.MM.dd");
    m_log_detaylari.append(MUH_GET_FIS_TURLERI_STRING(M_FIS->fis_turu) + "," + fis_tarihi.toString("dd MMMM yyyy dddd")
                           + ",Fiş No : "  + QVariant(M_FIS->fis_no).toString()
                           + ",Toplam Borç : "   + VIRGUL_EKLE (QVariant ( M_FIS->toplam_borc   ).toString())
                           + ",Toplam Alacak : " + VIRGUL_EKLE (QVariant ( M_FIS->toplam_alacak ).toString()));

}

/**************************************************************************************
                   MUH_MUHASEBE_FISI::CHECK_UPDATE_LINE
***************************************************************************************/

int MUH_MUHASEBE_FISI::CHECK_UPDATE_LINE ( int fis_id, int row_number )
{
    Q_UNUSED(fis_id);

    if ( m_fis_turu EQ ENUM_TEDIYE_FISI ) {

        if ( KASA_ALACAK_BAKIYE_VERIYOR_MU(row_number) EQ true ) {


            int msg_secim = MSG_YES_NO( tr ("Kasa hesabı alacak bakiye veriyor. \n Emin misiniz? " ), NULL );

            if ( msg_secim EQ ADAK_NO ) {

                return ADAK_FAIL;
            }
        }
    }

    return CHECK_ALARMLAR(row_number);
}

/**************************************************************************************
                   MUH_MUHASEBE_FISI::UPDATE_LINE
***************************************************************************************/

void MUH_MUHASEBE_FISI::UPDATE_LINE ( int fis_id, int row_number )
{

    QCommaEdit  * borc_CommaEdit         = ( QCommaEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget( row_number, BORC_COLUMN       );
    QCommaEdit  * alacak_CommaEdit       = ( QCommaEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget( row_number, ALACAK_COLUMN     );
    QLineEdit * lineedit_hesap_id        = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number,HESAP_ID_COLUMN);
    QLineEdit * line_edit_aciklama       = ( QLineEdit   * ) m_ui->tablewidget_fis_satirlari->cellWidget( row_number, ACIKLAMA_COLUMN );

    int fis_satiri_id    = m_ui->tablewidget_fis_satirlari->item ( row_number, ROW_ID_COLUMN )->text().toInt();

    M_FIS_SATIRI->fis_id                = fis_id;
    M_FIS_SATIRI->borc_tutari           = borc_CommaEdit->GET_DOUBLE();
    M_FIS_SATIRI->alacak_tutari         = alacak_CommaEdit->GET_DOUBLE();
    M_FIS_SATIRI->hesap_id              = lineedit_hesap_id->text().toInt();
    M_FIS_SATIRI->aciklama              = line_edit_aciklama->text();
    M_FIS_SATIRI->modul_id              = M_FIS->modul_id;
    M_FIS_SATIRI->program_id            = M_FIS->program_id;


    MUH_FIS_SATIRINI_GUNCELLE ( M_FIS , M_FIS_SATIRI , fis_satiri_id);


    m_ui->commaedit_toplam_borc->SET_DOUBLE   ( M_FIS->toplam_borc  );
    m_ui->commaedit_toplam_alacak->SET_DOUBLE ( M_FIS->toplam_alacak );


    m_log_kaydi_eklensin_mi = true;

    m_log_detaylari = QObject::tr("Log Türü - Muhasebe Fişleri,");

    QDate fis_tarihi = QDate::fromString(M_FIS->fis_tarihi , "yyyy.MM.dd");

    m_log_detaylari.append(MUH_GET_FIS_TURLERI_STRING( M_FIS->fis_turu ) + "," + fis_tarihi.toString("dd MMMM yyyy dddd")
                           + ",Fiş No : "  + QVariant(M_FIS->fis_no).toString()
                           + ",Toplam Borç : " + VIRGUL_EKLE (QVariant   ( M_FIS->toplam_borc   ).toString())
                           + ",Toplam Alacak : " + VIRGUL_EKLE (QVariant ( M_FIS->toplam_alacak ).toString()));
}

/*************************************************************************************
                     MUH_MUHASEBE_FISI::CHECK_DELETE_LINE
**************************************************************************************/

int MUH_MUHASEBE_FISI::CHECK_DELETE_LINE ( int fis_id, int row_number )
{
    Q_UNUSED(fis_id);
    Q_UNUSED(row_number);

    return CHECK_ALARMLAR(row_number);
}

/*************************************************************************************
                     MUH_MUHASEBE_FISI::DELETE_LINE
**************************************************************************************/

void MUH_MUHASEBE_FISI::DELETE_LINE ( int fis_id, int row_number )
{
    Q_UNUSED(fis_id);

    int fis_satiri_id = m_ui->tablewidget_fis_satirlari->item( row_number, ROW_ID_COLUMN )->text().toInt();

    MUH_FIS_SATIRINI_SIL ( M_FIS , fis_satiri_id);


    m_ui->commaedit_toplam_borc->SET_DOUBLE   ( M_FIS->toplam_borc   );
    m_ui->commaedit_toplam_alacak->SET_DOUBLE ( M_FIS->toplam_alacak );


    m_log_kaydi_eklensin_mi = true;

    m_log_detaylari = QObject::tr("Log Türü - Muhasebe Fişleri,");

    QDate fis_tarihi = QDate::fromString(M_FIS->fis_tarihi , "yyyy.MM.dd");

    m_log_detaylari.append(MUH_GET_FIS_TURLERI_STRING( M_FIS->fis_turu) + "," + fis_tarihi.toString("dd MMMM yyyy dddd")
                           + ",Fiş No : "  + M_FIS->fis_no
                           + ",Toplam Borç : " + VIRGUL_EKLE (QVariant   ( M_FIS->toplam_borc   ).toString())
                           + ",Toplam Alacak : " + VIRGUL_EKLE (QVariant ( M_FIS->toplam_alacak ).toString()));

}

/*************************************************************************************
                     MUH_MUHASEBE_FISI::LOCK_FIS_RECORD
**************************************************************************************/

int MUH_MUHASEBE_FISI::LOCK_FIS_RECORD ( int fis_id )
{
    return DB->LOCK_ROW( "fis_id","muh_fisler", QString ( "fis_id = %1" ).arg ( fis_id ));
}

/*************************************************************************************
                     MUH_MUHASEBE_FISI::UNLOCK_FIS_RECORD
**************************************************************************************/

void MUH_MUHASEBE_FISI::UNLOCK_FIS_RECORD ( int fis_id )
{
    DB->UNLOCK_ROW( "fis_id","muh_fisler", QString ( "fis_id = %1" ).arg ( fis_id ));
}

/*************************************************************************************
                     MUH_MUHASEBE_FISI::LEAVE_RECORD
**************************************************************************************/

int MUH_MUHASEBE_FISI::LEAVE_RECORD ()
{
    if ( m_fis_turu NE ENUM_TEDIYE_FISI AND m_fis_turu NE ENUM_TAHSIL_FISI ) {
        if ( m_ui->commaedit_toplam_alacak->GET_DOUBLE()  NE m_ui->commaedit_toplam_borc->GET_DOUBLE() )   {
            MSG_WARNING(  "Borç ve alacak toplamı eşit olmalı" , NULL );
            return ADAK_FAIL;
        }
    }
    if ( m_log_kaydi_eklensin_mi EQ true ) {
        DB->START_TRANSACTION();
        if ( m_kayit_eklendi EQ true) {
            E9_LOG_KAYDI_EKLE ( MUHASEBE_MODULU , LOG_MUH_FISLER , LOG_ISLEM_ADD , m_log_detaylari);
        }
        if ( m_kayit_silindi EQ true ) {
            E9_LOG_KAYDI_EKLE ( MUHASEBE_MODULU , LOG_MUH_FISLER , LOG_ISLEM_DELETE , m_log_detaylari);
        }
        if ( m_kayit_eklendi EQ false AND m_kayit_silindi EQ false ) {
            E9_LOG_KAYDI_EKLE ( MUHASEBE_MODULU , LOG_MUH_FISLER , LOG_ISLEM_UPDATE , m_log_detaylari);
        }
        DB->COMMIT_TRANSACTION();
    }
    return ADAK_OK;
}



/*************************************************************************************
                     MUH_MUHASEBE_FISI::KASA_ALACAK_BAKIYE_VERIYOR_MU
**************************************************************************************/

bool MUH_MUHASEBE_FISI::KASA_ALACAK_BAKIYE_VERIYOR_MU ( int row_number)
{
    SQL_QUERY query (DB);

    query.PREPARE_SELECT("muh_hesaplar" , "toplam_borc_array,toplam_alacak_array",
                         "tam_hesap_kodu = :tam_hesap_kodu");

    query.SET_VALUE(":tam_hesap_kodu",m_ui->search_edit_kasa_hesabi->GET_TEXT());

    if ( query.SELECT() EQ 0 ) {
        MSG_WARNING( tr("Hesap bilgileri alınamadı."), NULL );

        return false;
    }

    query.NEXT();

    UNPACK_DOUBLE_ARRAY ( query.VALUE("toplam_borc_array").toString(),M_HESABIN_BORC_BAKIYESI     ,E9_ARRAY_SIZE );
    UNPACK_DOUBLE_ARRAY ( query.VALUE("toplam_alacak_array").toString(),M_HESABIN_ALACAK_BAKIYESI   ,E9_ARRAY_SIZE );

    double kasa_hesabi_toplam_borc    = M_HESABIN_BORC_BAKIYESI   [ E9_SON_BAKIYE ];
    double kasa_hesabi_toplam_alacak  = M_HESABIN_ALACAK_BAKIYESI [ E9_SON_BAKIYE ];

    QWidget    * borc_widget          = m_ui->tablewidget_fis_satirlari->cellWidget( row_number, BORC_COLUMN);
    QCommaEdit * borc_commaedit       = static_cast<QCommaEdit*>(borc_widget);

    double borc_tutari                = borc_commaedit->GET_DOUBLE();
    kasa_hesabi_toplam_alacak        += borc_tutari;

    if ( kasa_hesabi_toplam_alacak > kasa_hesabi_toplam_borc ) {
        return true;
    }
    return false;
}

/**************************************************************************************
                     MUH_MUHASEBE_FISI::FIS_EKRANINI_DUZENLE
***************************************************************************************/

void MUH_MUHASEBE_FISI::FIS_EKRANINI_DUZENLE ( int fis_turu )
{
    if ( m_modul_id EQ MUHASEBE_MODULU ) {
        switch ( m_fis_turu ) {
            case ENUM_MAHSUP_FISI :
            default :
                SET_PAGE_TITLE( tr ( "MUH - MAHSUP FİŞİ" ) );
                SET_HELP_PAGE ( tr("genel-muhasebe_mahsup-fisi") );
                break;
            case ENUM_TAHSIL_FISI :
                SET_PAGE_TITLE( tr ( "MUH - TAHSİL FİŞİ" ) );
                SET_HELP_PAGE ( tr("genel-muhasebe_tahsil-fisi") );
                break;
            case ENUM_TEDIYE_FISI :
                SET_PAGE_TITLE( tr ( "MUH - TEDİYE FİŞİ" ) );
                SET_HELP_PAGE ( tr("genel-muhasebe_tediye-fisi") );
                break;
            case ENUM_ACILIS_FISI :
                SET_PAGE_TITLE( tr ( "MUH - AÇILIŞ FİŞİ" ) );
                SET_HELP_PAGE ( tr("genel-muhasebe_acilis-fisi") );
                break;
            case ENUM_KAPANIS_FISI :
                SET_PAGE_TITLE( tr ( "MUH - KAPANIŞ FİŞİ" ) );
                SET_HELP_PAGE ( tr("genel-muhasebe_kapanis-fisi") );
                break;
        }
    }
    else {
        SET_HELP_PAGE ( tr("genel-muhasebe_entegrasyon") );

        switch ( m_modul_id ) {
            case CARI_MODULU :
                SET_PAGE_TITLE( tr ( "MUH - CARİ ENT. FİŞİ" ) );
                break;
            case BANKA_MODULU :
                SET_PAGE_TITLE( tr ( "MUH - BANKA ENT. FİŞİ" ) );
                break;
            case CEKSENET_MODULU :
                SET_PAGE_TITLE( tr ( "MUH - ÇEK-SENET ENT. FİŞİ" ) );
                break;
            case DEMIRBAS_MODULU :
                SET_PAGE_TITLE( tr ( "MUH - SABİT KIYMET ENT. FİŞİ" ) );
                break;
            case FATURA_MODULU :
                SET_PAGE_TITLE( tr ( "MUH - FATURA ENT. FİŞİ" ) );
                break;
            case PERAKENDE_MODULU :
                SET_PAGE_TITLE( tr ( "MUH - PERAKENDE ENT. FİŞİ" ) );
                break;
            case PERSONEL_MODULU :
                SET_PAGE_TITLE( tr ( "MUH - PERSONEL ENT. FİŞİ" ) );
                break;
            case STOK_MODULU :
                SET_PAGE_TITLE( tr ( "MUH - STOK ENT. FİŞİ" ) );
                break;
            case IRSALIYE_MODULU :
                SET_PAGE_TITLE( tr ( "MUH - İRSALİYE ENT. FİŞİ" ) );
                break;
            case SMM_MODULU :
                SET_PAGE_TITLE( tr ("MUH - SM MAKBUZ ENT. FİŞİ"));
                break;
            case ISLETME_MODULU :
                SET_PAGE_TITLE( tr ("MUH - İŞLETME ENT. FİŞİ"));
                break;
            case -1 :
                SET_PAGE_TITLE( tr ("MUH - TÜM. ENT. FİŞLERİ"));
                break;
            default:
                SET_PAGE_TITLE( tr ("MUH - DİĞER ENT. FİŞLERİ"));
                break;
        }
        m_ui->button_fis_kopyala->setHidden(true);
    }

    switch ( fis_turu ) {
        case ENUM_MAHSUP_FISI :
        {
            REGISTER_BUTTON_SHORT_CUT ( SATIR_TAMAMLA_BUTTON_COLUMN , QKeySequence ( Qt::CTRL + Qt::Key_T ) );

            m_ui->labelKasaHesabi->setHidden            (true );
            m_ui->search_edit_kasa_hesabi->setHidden    (true );
            m_ui->lineedit_kasa_hesap_ismi->setHidden   (true );
            m_ui->commaedit_toplam_borc->setVisible     (true );
            m_ui->commaedit_toplam_alacak->setVisible   (true );

            m_ui->adakDate_fis_tarihi->setDisabled     (false);
            m_ui->label_fis_tarihi->setHidden           (false);
            m_ui->adakDate_fis_tarihi->setHidden       (false);

            m_ui->tablewidget_fis_satirlari->showColumn     ( ACIKLAMA_COLUMN );
            m_ui->tablewidget_fis_satirlari->showColumn     ( BORC_COLUMN );
            m_ui->tablewidget_fis_satirlari->showColumn     ( ALACAK_COLUMN );
            m_ui->tablewidget_fis_satirlari->showColumn     ( KDV_AYIRMA_BUTON_COLUMN );

            m_ui->tablewidget_fis_satirlari->setColumnWidth (  HESAP_KODU_COLUMN               ,   185 );
            m_ui->tablewidget_fis_satirlari->setColumnWidth (  HESAP_ISMI_COLUMN               ,   345 );
            m_ui->tablewidget_fis_satirlari->setColumnWidth (  ACIKLAMA_COLUMN                 ,   367 );
            m_ui->tablewidget_fis_satirlari->setColumnWidth (  BORC_COLUMN                     ,   152 );
            m_ui->tablewidget_fis_satirlari->setColumnWidth (  ALACAK_COLUMN                   ,   152 );
            m_ui->tablewidget_fis_satirlari->setColumnWidth (  ALACAK_COLUMN                   ,   152 );
            m_ui->tablewidget_fis_satirlari->setColumnWidth (  KDV_AYIRMA_BUTON_COLUMN         ,   30  );
            m_ui->tablewidget_fis_satirlari->setColumnWidth (  SATIR_TAMAMLA_BUTTON_COLUMN     ,   30  );

            SET_SETTING_NAME ( "MUH_MAHSUP_FISI" );
            break;
        }
        case ENUM_TAHSIL_FISI :
        {
            REMOVE_BUTTON_SHORT_CUT ( SATIR_TAMAMLA_BUTTON_COLUMN );

            m_ui->commaedit_toplam_borc->setHidden ( true );
            m_ui->commaedit_toplam_alacak->setVisible ( true );
            m_ui->search_edit_kasa_hesabi->setVisible ( true );
            m_ui->lineedit_kasa_hesap_ismi->setVisible ( true );
            m_ui->labelKasaHesabi->setVisible ( true );

            m_ui->tablewidget_fis_satirlari->showColumn ( ACIKLAMA_COLUMN );
            m_ui->tablewidget_fis_satirlari->hideColumn ( BORC_COLUMN );
            m_ui->tablewidget_fis_satirlari->hideColumn ( KDV_AYIRMA_BUTON_COLUMN );
            m_ui->tablewidget_fis_satirlari->showColumn ( ALACAK_COLUMN );
            m_ui->tablewidget_fis_satirlari->hideColumn ( SATIR_TAMAMLA_BUTTON_COLUMN );
            m_ui->adakDate_fis_tarihi->setDisabled( false );

            m_ui->tablewidget_fis_satirlari->setColumnWidth (  HESAP_KODU_COLUMN           ,   185 );
            m_ui->tablewidget_fis_satirlari->setColumnWidth (  HESAP_ISMI_COLUMN           ,   395 );
            m_ui->tablewidget_fis_satirlari->setColumnWidth (  ACIKLAMA_COLUMN             ,   474 );
            m_ui->tablewidget_fis_satirlari->setColumnWidth (  ALACAK_COLUMN               ,   182 );
            //m_ui->tablewidget_fis_satirlari->setColumnWidth (  SATIR_TAMAMLA_BUTTON_COLUMN ,   30  );
            SET_SETTING_NAME ( "MUH_TAHSILAT_FISI" );
            m_ui->label_fis_tarihi->setHidden(false);
            m_ui->adakDate_fis_tarihi->setHidden(false);
            break;
        }
        case ENUM_TEDIYE_FISI :
        {
            REMOVE_BUTTON_SHORT_CUT ( SATIR_TAMAMLA_BUTTON_COLUMN );

            m_ui->commaedit_toplam_alacak->setHidden ( true );
            m_ui->commaedit_toplam_borc->setHidden ( false );
            m_ui->search_edit_kasa_hesabi->setVisible ( true );
            m_ui->lineedit_kasa_hesap_ismi->setVisible ( true );
            m_ui->labelKasaHesabi->setVisible ( true );


            m_ui->tablewidget_fis_satirlari->showColumn ( ACIKLAMA_COLUMN );
            m_ui->tablewidget_fis_satirlari->hideColumn ( ALACAK_COLUMN );
            m_ui->tablewidget_fis_satirlari->hideColumn ( KDV_AYIRMA_BUTON_COLUMN );
            m_ui->tablewidget_fis_satirlari->hideColumn ( SATIR_TAMAMLA_BUTTON_COLUMN );
            m_ui->tablewidget_fis_satirlari->showColumn ( BORC_COLUMN );

            m_ui->adakDate_fis_tarihi->setDisabled( false );

            m_ui->tablewidget_fis_satirlari->setColumnWidth (  HESAP_KODU_COLUMN           ,   185 );
            m_ui->tablewidget_fis_satirlari->setColumnWidth (  HESAP_ISMI_COLUMN           ,   395 );
            m_ui->tablewidget_fis_satirlari->setColumnWidth (  ACIKLAMA_COLUMN             ,   474 );
            m_ui->tablewidget_fis_satirlari->setColumnWidth (  BORC_COLUMN                 ,   182 );
            SET_SETTING_NAME ( "MUH_TEDIYE_FISI" );
            m_ui->label_fis_tarihi->setHidden(false);
            m_ui->adakDate_fis_tarihi->setHidden(false);
            break;
        }
        case ENUM_ACILIS_FISI :
        {
            m_is_entegrasyon_fisi = false;
            REGISTER_BUTTON_SHORT_CUT ( SATIR_TAMAMLA_BUTTON_COLUMN , QKeySequence ( Qt::CTRL + Qt::Key_T ) );

            m_ui->labelKasaHesabi->setHidden ( true );
            m_ui->search_edit_kasa_hesabi->setHidden ( true );
            m_ui->lineedit_kasa_hesap_ismi->setHidden ( true );
            m_ui->commaedit_toplam_borc->setVisible ( true );
            m_ui->commaedit_toplam_alacak->setVisible ( true );

            m_ui->adakDate_fis_tarihi->SET_DATE( MALI_YIL_FIRST_DATE() );

            m_ui->adakDate_fis_tarihi->setDisabled( true );
            m_ui->tablewidget_fis_satirlari->hideColumn ( KDV_AYIRMA_BUTON_COLUMN );
            m_ui->tablewidget_fis_satirlari->showColumn ( BORC_COLUMN );
            m_ui->tablewidget_fis_satirlari->showColumn ( ALACAK_COLUMN );
            m_ui->tablewidget_fis_satirlari->setColumnWidth (  HESAP_KODU_COLUMN           ,   245 );
            m_ui->tablewidget_fis_satirlari->setColumnWidth (  HESAP_ISMI_COLUMN           ,   286 );
            m_ui->tablewidget_fis_satirlari->setColumnWidth (  BORC_COLUMN                 ,   200 );
            m_ui->tablewidget_fis_satirlari->setColumnWidth (  ALACAK_COLUMN               ,   200 );
            m_ui->tablewidget_fis_satirlari->setColumnWidth (  SATIR_TAMAMLA_BUTTON_COLUMN ,   30  );
            m_ui->tablewidget_fis_satirlari->setColumnWidth (  ACIKLAMA_COLUMN             ,   300);

            SET_SETTING_NAME ( "MUH_ACILIS_FISI" );
            m_ui->label_fis_tarihi->setDisabled(true);
            m_ui->adakDate_fis_tarihi->setDisabled(true);
            break;
        }
        case ENUM_KAPANIS_FISI :
        {
            REGISTER_BUTTON_SHORT_CUT ( SATIR_TAMAMLA_BUTTON_COLUMN , QKeySequence ( Qt::CTRL + Qt::Key_T ) );
            m_fis_turu = ENUM_KAPANIS_FISI;
            m_ui->labelKasaHesabi->setHidden ( true );
            m_ui->search_edit_kasa_hesabi->setHidden ( true );
            m_ui->lineedit_kasa_hesap_ismi->setHidden ( true );
            m_ui->commaedit_toplam_borc->setVisible ( true );
            m_ui->commaedit_toplam_alacak->setVisible ( true );
            m_ui->adakDate_fis_tarihi->SET_DATE(MALI_YIL_FIRST_DATE() );
            m_ui->adakDate_fis_tarihi->setDisabled( true );
            m_ui->tablewidget_fis_satirlari->hideColumn ( KDV_AYIRMA_BUTON_COLUMN );
            m_ui->tablewidget_fis_satirlari->showColumn ( BORC_COLUMN );
            m_ui->tablewidget_fis_satirlari->showColumn ( ALACAK_COLUMN );
            m_ui->tablewidget_fis_satirlari->setColumnWidth (  HESAP_KODU_COLUMN           ,   245 );
            m_ui->tablewidget_fis_satirlari->setColumnWidth (  HESAP_ISMI_COLUMN           ,   286 );
            m_ui->tablewidget_fis_satirlari->setColumnWidth (  BORC_COLUMN                 ,   200 );
            m_ui->tablewidget_fis_satirlari->setColumnWidth (  ALACAK_COLUMN               ,   200 );
            m_ui->tablewidget_fis_satirlari->setColumnWidth (  SATIR_TAMAMLA_BUTTON_COLUMN ,   30  );
            m_ui->tablewidget_fis_satirlari->setColumnWidth (  ACIKLAMA_COLUMN             ,   300);
            SET_SETTING_NAME ( "MUH_KAPANIS_FISI" );
            m_ui->label_fis_tarihi->setHidden(false);
            m_ui->adakDate_fis_tarihi->setHidden(false);
            break;
        }
        default :
               break;
    }

}

/**************************************************************************************
                   MUH_MUHASEBE_FISI::SAVER_BUTTON_CLICKED
***************************************************************************************/

void MUH_MUHASEBE_FISI::SAVER_BUTTON_CLICKED ( QAbstractButton * p_button, int p_record_id )
{
    if ( p_button EQ m_ui->button_yazdir ) {
        MUH_RAPOR_VARS * MUH_RV   = new MUH_RAPOR_VARS;

        MUH_RV->baslangic_tarihi   =  m_ui->adakDate_fis_tarihi->QDATE();
        MUH_RV->bitis_tarihi       =  m_ui->adakDate_fis_tarihi->QDATE();

        OPEN_REPORT_SHOWER ( GET_TOPLU_FIS_RAPORU ( p_record_id , MUH_RV ), nativeParentWidget() );

        delete MUH_RV;
    }
    else if ( p_button EQ m_ui->button_fis_kopyala ) {
        OPEN_MUH_FIS_KOPYALAMA_BATCH ( p_record_id, NULL );
    }
}

/**************************************************************************************
                   MUH_MUHASEBE_FISI::SEARCH_EDIT_CLICKED
***************************************************************************************/

void MUH_MUHASEBE_FISI::SEARCH_EDIT_CLICKED ( QWidget * p_widget , QLineEdit * p_lineEdit )
{
    Q_UNUSED ( p_widget );

    int kasa_hesap_id = OPEN_MUH_HESAP_ARAMA ( p_lineEdit->text(), this, 1 );

    if ( kasa_hesap_id NE -1 ) {
        p_lineEdit->setText ( MUH_GET_HESAP_KODU ( kasa_hesap_id ) );
    }
}

/**************************************************************************************
                   MUH_MUHASEBE_FISI::LINE_SEARCH_EDIT_CLICKED
***************************************************************************************/

void MUH_MUHASEBE_FISI::LINE_SEARCH_EDIT_CLICKED ( int current_row, QWidget * widget, QLineEdit * line_edit )
{
    Q_UNUSED ( widget );
    Q_UNUSED ( current_row );

    if ( m_modul_id NE MUHASEBE_MODULU ) {
        MSG_WARNING( tr ( "Fiş  Entegrasyon Fişidir.Fişte değişiklik yapamazsınız!.."), NULL );
        return;
    }

    int satir_hesap_id = OPEN_MUH_HESAP_ARAMA ( line_edit->text(), this, 1 );
    line_edit->setText ( MUH_GET_HESAP_KODU ( satir_hesap_id ) );
 }

/**************************************************************************************
                   MUH_MUHASEBE_FISI::LINE_CHANGER_BUTTON_CLICKED
***************************************************************************************/

int MUH_MUHASEBE_FISI::LINE_CHANGER_BUTTON_CLICKED ( QAbstractButton * p_button, int p_row_number )
{
    Q_UNUSED ( p_button );

    return SATIR_TAMAMLAMA (  p_row_number );
}

/*************************************************************************************
                     MUH_MUHASEBE_FISI::LINE_SAVER_BUTTON_CLICKED
**************************************************************************************/

void MUH_MUHASEBE_FISI::LINE_SAVER_BUTTON_CLICKED ( QAbstractButton * p_button, int p_row_id, int p_record_id, int p_row_number )
{
    Q_UNUSED ( p_button );

    KDV_AYIRMA ( p_row_number, p_row_id, p_record_id );

    int last_row = m_ui->tablewidget_fis_satirlari->rowCount() - 1;

    if ( last_row EQ p_row_number) {
        return;
    }

    QLineEdit * line_edit = static_cast<QLineEdit *>(m_ui->tablewidget_fis_satirlari->cellWidget(last_row,HESAP_KODU_COLUMN));
    if (line_edit NE NULL) {
        line_edit->setFocus();
        m_ui->tablewidget_fis_satirlari->setCurrentCell(last_row,HESAP_KODU_COLUMN);
    }
}

/**************************************************************************************
                   MUH_MUHASEBE_FISI::SATIR_TAMAMLAMA
***************************************************************************************/

int MUH_MUHASEBE_FISI::SATIR_TAMAMLAMA ( int p_row_number )
{
    switch ( m_fis_turu ) {
        case ENUM_TAHSIL_FISI :
        case ENUM_TEDIYE_FISI :
            return ADAK_RECORD_UNCHANGED;
        default :
            break;
    }


    if ( m_ui->tablewidget_fis_satirlari->rowCount() < 2 ) {
        return ADAK_RECORD_UNCHANGED;
    }

    if ( p_row_number EQ 0 ) {
        return ADAK_RECORD_UNCHANGED;
    }

    QSearchEdit * search_Edit_hesap_kodu   = ( QSearchEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget ( p_row_number, HESAP_KODU_COLUMN );

    SQL_QUERY query(DB);

    query.PREPARE_SELECT("muh_hesaplar" , "hesap_id,hesap_ismi" , "tam_hesap_kodu = :tam_hesap_kodu");

    query.SET_VALUE(":tam_hesap_kodu" , search_Edit_hesap_kodu->GET_TEXT());

    if ( query.SELECT() EQ 0) {
        MSG_WARNING(  tr ( "Hesap kodu bulunamadı" ), search_Edit_hesap_kodu );

        return ADAK_RECORD_UNCHANGED;
    }
    query.NEXT();
    int hesap_id       = query.VALUE(0).toInt();
    QString hesap_ismi = query.VALUE(1).toString();

    QLineEdit * aciklama_lineEdit = ( QLineEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget ( p_row_number, ACIKLAMA_COLUMN );

    if ( aciklama_lineEdit->text().isEmpty() EQ true ) {
        QLineEdit * previous_row_lineEdit = ( QLineEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget ( p_row_number -1, ACIKLAMA_COLUMN );
        aciklama_lineEdit->setText ( previous_row_lineEdit->text() );
    }
    QLineEdit * lineedit_hesap_ismi = ( QLineEdit *) m_ui->tablewidget_fis_satirlari->cellWidget(p_row_number , HESAP_ISMI_COLUMN);
    lineedit_hesap_ismi->setText(hesap_ismi);

    QLineEdit * lineedit_hesap_id = ( QLineEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(p_row_number , HESAP_ID_COLUMN);
    lineedit_hesap_id->setText(QVariant(hesap_id).toString());

    double  borc_bakiye      = 0.00;
    double  alacak_bakiye    = 0.00;
    double  borc_tutari      = 0.00;
    double  alacak_tutari    = 0.00;

    for ( int i = 0 ; i < m_ui->tablewidget_fis_satirlari->rowCount() ; i++ ) {
        if ( i EQ p_row_number ) {
            continue;
        }

        QCommaEdit * borc_widget    = static_cast<QCommaEdit *>(m_ui->tablewidget_fis_satirlari->cellWidget(i,BORC_COLUMN));
        QCommaEdit * alacak_widget  = static_cast<QCommaEdit *>(m_ui->tablewidget_fis_satirlari->cellWidget(i,ALACAK_COLUMN));

        borc_bakiye    += borc_widget->GET_DOUBLE();
        alacak_bakiye  += alacak_widget->GET_DOUBLE();
    }

    if ( borc_bakiye > alacak_bakiye ) {
        borc_tutari   = borc_bakiye - alacak_bakiye;
    }
    else if ( alacak_bakiye > borc_bakiye ) {
        alacak_tutari = alacak_bakiye - borc_bakiye;
    }
    else {
        MSG_WARNING( tr ( "Fişteki borç bakiye ve alacak bakiye eşit!.." ), m_ui-> comma_edit_hesap_alacak );

        return ADAK_RECORD_UNCHANGED;
    }

    if ( alacak_tutari > 0.00 ) {
        QCommaEdit * commaEdit_borc_tutari = ( QCommaEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget ( p_row_number, BORC_COLUMN );
        commaEdit_borc_tutari->SET_DOUBLE ( QVariant ( alacak_tutari ).toDouble() );        
        m_ui->tablewidget_fis_satirlari->setCurrentCell(p_row_number,BORC_COLUMN);
    }

    if ( borc_tutari > 0.00 ) {
        QCommaEdit * commaEdit_alacak_tutari  = ( QCommaEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget ( p_row_number, ALACAK_COLUMN );
        commaEdit_alacak_tutari->SET_DOUBLE ( QVariant ( borc_tutari ).toDouble() );
        m_ui->tablewidget_fis_satirlari->setCurrentCell(p_row_number,ALACAK_COLUMN);
    }

    return ADAK_RECORD_CHANGED;
}

/**************************************************************************************
                   MUH_MUHASEBE_FISI::KDV_AYIRMA
***************************************************************************************/

void MUH_MUHASEBE_FISI::KDV_AYIRMA ( int p_current_row, int p_row_id, int p_fis_id )
{
    MUH_KDV_AYIRMA_STRUCT * KDV_AYIRMA_STRUCT = new MUH_KDV_AYIRMA_STRUCT;

    SQL_QUERY   sql_query ( DB );

    sql_query.PREPARE_SELECT("muh_fisler" , "fis_tarihi" , "fis_id = :fis_id");

    sql_query.SET_VALUE      ( ":fis_id", p_fis_id );

    if ( sql_query.SELECT() EQ 0 ) {
        return;
    }

    sql_query.NEXT();

    QDate        fis_tarihi             =  QDate::fromString    ( sql_query.VALUE(0).toString(), "yyyy.MM.dd" );
    int          gun_no                 =  MALI_YIL_ARRAY_INDIS  ( fis_tarihi );
    
    QCommaEdit * commaEdit_borc         =  ( QCommaEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget ( p_current_row, BORC_COLUMN     );
    QCommaEdit * commaEdit_alacak       =  ( QCommaEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget ( p_current_row, ALACAK_COLUMN   );
    QLineEdit  * lineEdit_aciklama      =  ( QLineEdit  * ) m_ui->tablewidget_fis_satirlari->cellWidget ( p_current_row, ACIKLAMA_COLUMN );
    
    double       borc_tutari            =  commaEdit_borc->GET_DOUBLE();
    double       alacak_tutari          =  commaEdit_alacak->GET_DOUBLE();
    QString      aciklama               =  lineEdit_aciklama->text();

    int          kdvsi_ayrilan_hesap_id =  0;

    if ( borc_tutari NE 0 ) {
        if ( OPEN_KDV_AYIRMA ( p_row_id, borc_tutari ,aciklama , KDV_AYIRMA_STRUCT,  this ) EQ false ) {
            return;
        };
    }
    else {

        if ( OPEN_KDV_AYIRMA ( p_row_id, alacak_tutari , aciklama , KDV_AYIRMA_STRUCT, this ) EQ false ) {
            return;
        };
    }

    DB->START_TRANSACTION();

    //! struct ile eklendiginde fis toplamlari artmamasi gerektigi icin satir manuel olarak eklenmistir.
    sql_query.PREPARE_INSERT("muh_fis_satirlari" , "fis_satiri_id" , "fis_id, hesap_id, aciklama, borc_tutari, "
                             "alacak_tutari, order_number, gizli_satir_mi, gizli_satir_id, program_id, modul_id, base_record_id ");

    sql_query.SET_VALUE      ( ":fis_id"          , p_fis_id                                           );
    sql_query.SET_VALUE      ( ":hesap_id"        , KDV_AYIRMA_STRUCT->kdv_hesap_id                    );
    sql_query.SET_VALUE      ( ":aciklama"        , KDV_AYIRMA_STRUCT->aciklama                        );
    sql_query.SET_VALUE      ( ":borc_tutari"     , ROUND ( KDV_AYIRMA_STRUCT->kdv_borc_tutari )       );
    sql_query.SET_VALUE      ( ":alacak_tutari"   , ROUND ( KDV_AYIRMA_STRUCT->kdv_alacak_tutari )     );
    sql_query.SET_VALUE      ( ":order_number"    , KDV_AYIRMA_STRUCT->order_number                    );
    sql_query.SET_VALUE      ( ":gizli_satir_mi"  , 0                                                  );
    sql_query.SET_VALUE      ( ":gizli_satir_id"  , 0                                                  );
    sql_query.SET_VALUE      ( ":program_id"      , M_FIS->program_id                                  );
    sql_query.SET_VALUE      ( ":modul_id"        , M_FIS->modul_id                                    );
    sql_query.SET_VALUE      ( ":base_record_id"  , 0                                                  );
    sql_query.INSERT();

    // entegere olsun mu gececi olrak 1 gonderildi
    MUH_HESABININ_BAKIYELERINI_ARTTIR ( KDV_AYIRMA_STRUCT->kdv_hesap_id, gun_no, KDV_AYIRMA_STRUCT->kdv_borc_tutari, KDV_AYIRMA_STRUCT->kdv_alacak_tutari);


    sql_query.PREPARE_UPDATE  ( "muh_fis_satirlari ","fis_satiri_id",
                               "borc_tutari , alacak_tutari ",
                               "fis_satiri_id = :fis_satiri_id" );

    sql_query.SET_VALUE       (  ":fis_satiri_id"     , p_row_id );
    sql_query.SET_VALUE       (  ":alacak_tutari"     , ROUND (  KDV_AYIRMA_STRUCT->satir_kdv_haric_alacak_tutari )  );
    sql_query.SET_VALUE       (  ":borc_tutari"       , ROUND (  KDV_AYIRMA_STRUCT->satir_kdv_haric_borc_tutari   )  );
    sql_query.UPDATE();

    sql_query.PREPARE_SELECT("muh_fis_satirlari" , "hesap_id" , "fis_satiri_id = :fis_satiri_id");


    sql_query.SET_VALUE       ( ":fis_satiri_id", p_row_id );

    if ( sql_query.SELECT() > 0 ) {
        sql_query.NEXT();
        kdvsi_ayrilan_hesap_id = sql_query.VALUE(0).toInt();
    }


    // entegere olsun mu gececi olrak 1 gonderildi
    MUH_HESABININ_BAKIYELERINI_AZALT ( kdvsi_ayrilan_hesap_id, gun_no, KDV_AYIRMA_STRUCT->kdv_borc_tutari, KDV_AYIRMA_STRUCT->kdv_alacak_tutari );

    DB->COMMIT_TRANSACTION();

    REFRESH_FIS_RECORD ( );

    delete KDV_AYIRMA_STRUCT;

    KDV_AYIRMA_STRUCT = NULL;
}

/**************************************************************************************
                   MUH_MUHASEBE_FISI::CHECK_ALARMLAR
***************************************************************************************/

int MUH_MUHASEBE_FISI::CHECK_ALARMLAR(int row_number)
{
    QCommaEdit * comma_edit_borc   = static_cast<QCommaEdit *>
                                     (m_ui->tablewidget_fis_satirlari->cellWidget(row_number,BORC_COLUMN));

    QCommaEdit * comma_edit_alacak = static_cast<QCommaEdit *>
                                     (m_ui->tablewidget_fis_satirlari->cellWidget(row_number,ALACAK_COLUMN));

    QSearchEdit * searchedit_tam_hesap_kodu = ( QSearchEdit * ) m_ui->tablewidget_fis_satirlari->cellWidget(row_number , HESAP_KODU_COLUMN);

    return MUH_CHECK_MUHASEBE_ALARMLARI(searchedit_tam_hesap_kodu->GET_TEXT(),comma_edit_borc->GET_DOUBLE(),comma_edit_alacak->GET_DOUBLE());
}

/**************************************************************************************
                   MUH_MUHASEBE_FISI::CHECK_EXIT
***************************************************************************************/

int MUH_MUHASEBE_FISI::CHECK_EXIT()
{
    return ADAK_OK;
}
