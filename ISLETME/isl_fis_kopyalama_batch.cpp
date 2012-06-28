#include "isl_fis_kopyalama_batch_class.h"
#include "isl_fis_kopyalama_batch_open.h"
#include "ui_isl_fis_kopyalama_batch.h"
#include "isl_console_utils.h"
#include "e9_console_utils.h"
#include "adak_utils.h"
#include "e9_log.h"
#include "isl_defteri_fisi_open.h"
#include "isl_fis_arama_open.h"
#include "isl_enum.h"
#include "e9_gui_utils.h"

extern ADAK_SQL * DB;

/**************************************************************************************
                   OPEN_ISL_FIS_KOPYALAMA_BATCH
***************************************************************************************/

void OPEN_ISL_FIS_KOPYALAMA_BATCH ( int fis_id, QWidget * parent )
{
    ISL_FIS_KOPYALAMA_BATCH * F = new ISL_FIS_KOPYALAMA_BATCH ( fis_id, parent );
    F->EXEC( NOT_FULL_SCREEN );
}

/**************************************************************************************
                   ISL_FIS_KOPYALAMA_BATCH::ISL_FIS_KOPYALAMA_BATCH
***************************************************************************************/

ISL_FIS_KOPYALAMA_BATCH::ISL_FIS_KOPYALAMA_BATCH ( int fis_id, QWidget * parent ) : BATCH_KERNEL ( parent ), m_ui ( new Ui::ISL_FIS_KOPYALAMA_BATCH ), m_fis_id ( fis_id )
{
    m_parent = parent;
    m_fis_id = fis_id;

    m_ui->setupUi      ( this );
    START_BATCH_KERNEL ( this, DB );
}

/**************************************************************************************
                   ISL_FIS_KOPYALAMA_BATCH::SETUP_FORM
***************************************************************************************/

void ISL_FIS_KOPYALAMA_BATCH::SETUP_FORM()
{
    SET_HELP_PAGE  ( tr ( "isl-fis-kopyalama" ) );
    SET_PAGE_TITLE ( tr ( "İŞL - İŞLETME DEFTERİ FİŞİ KOPYALAMA" ) );

    SET_AUTO_EXIT_BATCH ( true );

    REGISTER_BUTTON_WIDGET       ( m_ui->widget_batch_buttons );

    if ( m_fis_id > 0 ) {

        SQL_QUERY sql_query ( DB );

        sql_query.PREPARE_SELECT ( "isl_fisi","fis_no, fis_turu, aciklama, fis_tarihi ",
                                   "fis_id = :fis_id" );
        sql_query.SET_VALUE      ( ":fis_id", m_fis_id );
        if ( sql_query.SELECT() EQ 0 ) {
            return;
        }
        sql_query.NEXT();
        m_ui->search_edit_fis_no->SET_TEXT ( sql_query.VALUE(0).toString() );
        m_fis_turu = sql_query.VALUE(1).toInt();
        m_ui->lineEdit_fis_turu->setText ( ISL_GET_FIS_TURU_STRING( m_fis_turu) );
        m_ui->textedit_aciklama->setText ( sql_query.VALUE(2).toString() );
        m_ui->adakDate_kpylnck_fis_tarihi->SET_DATE(  sql_query.VALUE(3).toString() );
    }

    m_ui->lineEdit_yeni_fis_no->setText          ( "*" );

    SET_NAME_OF_RUN_BATCH_BUTTON ( tr ("Kopyala" ) );
    SET_FIRST_FOCUS_WIDGET       ( m_ui->search_edit_fis_no );
    SET_SETTING_NAME             ( "ISL_FIS_KOPYALAMA_BATCH" );


    m_ui->adakDate_kpylnck_fis_tarihi->setEnabled( false );
    m_ui->search_edit_fis_no->setEnabled    ( false );
    m_ui->textedit_aciklama->setEnabled     ( false );
    m_ui->lineEdit_fis_turu->setEnabled     ( false );
}

/**************************************************************************************
                   ISL_FIS_KOPYALAMA_BATCH::CHECK_VAR
***************************************************************************************/

int ISL_FIS_KOPYALAMA_BATCH::CHECK_VAR ( QObject * object )
{

    SQL_QUERY sql_query ( DB );

    if ( object EQ m_ui->adakDate_yeni_fis_tarihi ) {
        int siradaki_fis_no = ISL_SIRADAKI_FIS_NO_AL( m_ui->adakDate_yeni_fis_tarihi->DATE() );
        m_ui->lineEdit_yeni_fis_no->setText ( QVariant ( siradaki_fis_no ).toString() );
    }

    else if ( object EQ m_ui->search_edit_fis_no ) {

        sql_query.PREPARE_SELECT ( "isl_fisi","fis_id, fis_turu, aciklama, fis_tarihi ",
                                   "fis_no = :fis_no AND fis_tarihi = :fis_tarihi" );

        sql_query.SET_VALUE ( ":fis_no", m_ui->search_edit_fis_no->GET_TEXT().toInt());
        sql_query.SET_VALUE ( ":fis_tarihi", m_ui->adakDate_kpylnck_fis_tarihi->DATE() );

        if ( sql_query.SELECT() EQ 0 ) {
            MSG_WARNING(  tr ( "Bu fiş numarası ile fiş bulunamadı." ), m_ui->search_edit_fis_no );

            return ADAK_FAIL;
        }
        sql_query.NEXT();

        m_fis_id   = sql_query.VALUE(0).toInt();

        m_fis_turu = sql_query.VALUE(1).toInt();

        m_ui->lineEdit_fis_turu->setText ( ISL_GET_FIS_TURU_STRING(m_fis_turu) );

        m_ui->textedit_aciklama->setText ( sql_query.VALUE(2).toString() );

        m_ui->adakDate_kpylnck_fis_tarihi->SET_DATE(  sql_query.VALUE(3).toString());

        int siradaki_fis_no = ISL_SIRADAKI_FIS_NO_AL ( m_ui->adakDate_yeni_fis_tarihi->DATE() );
        m_ui->lineEdit_yeni_fis_no->setText ( QVariant ( siradaki_fis_no ).toString() );

    }
    else if ( object EQ m_ui->lineEdit_yeni_fis_no ) {
        sql_query.PREPARE_SELECT ( "isl_fisi","fis_id","fis_no = :fis_no AND fis_tarihi = :fis_tarihi" );

        sql_query.SET_VALUE ( ":fis_no",m_ui->lineEdit_yeni_fis_no->text() );
        sql_query.SET_VALUE ( ":fis_tarihi",  m_ui->adakDate_yeni_fis_tarihi->DATE() );

        if ( sql_query.SELECT() NE 0 ) {
            MSG_WARNING( tr ( "Bu fiş numarasına sahip başka bir fiş vardır. "
                                                       "Bu numara ile fiş kopyalayamazsınız!.." ),m_ui->lineEdit_yeni_fis_no );


            return ADAK_FAIL;
        }
    }

    return ADAK_OK;
}

/**************************************************************************************
                   ISL_FIS_KOPYALAMA_BATCH::CHECK_RUN
***************************************************************************************/

int ISL_FIS_KOPYALAMA_BATCH::CHECK_RUN()
{
    if ( m_ui->lineEdit_yeni_fis_no->text().isEmpty() EQ true ) {
        MSG_WARNING( tr ( "Yeni fiş numarası boş bırakılamaz." ), m_ui->lineEdit_yeni_fis_no );
        return ADAK_FAIL;
    }

    return ADAK_OK;
}

/**************************************************************************************
                   ISL_FIS_KOPYALAMA_BATCH::RUN_BATCH
***************************************************************************************/

void ISL_FIS_KOPYALAMA_BATCH::RUN_BATCH()
{
    SQL_QUERY sql_query     ( DB );
    SQL_QUERY insert_query  ( DB );

    sql_query.PREPARE_SELECT ( "isl_fisi","kdv_haric_toplam, kdv_toplam, kdv_dahil_toplam,fis_tarihi,fis_no,modul_id,program_id ",
                               "fis_id = :fis_id" );
    sql_query.SET_VALUE ( ":fis_id", m_fis_id  );

    if ( sql_query.SELECT() EQ 0 ) {
        MSG_WARNING( tr ( "Kopyalanacak fiş bilgileri alınamadı!" ), NULL );
        return;
    }
    sql_query.NEXT();

    double kdv_haric_toplam   = sql_query.VALUE(0).toDouble();
    double kdv_toplam         = sql_query.VALUE(1).toDouble();
    double kdv_dahil_toplam   = sql_query.VALUE(2).toDouble();
    QDate  fis_tarihi         = QDate::fromString(sql_query.VALUE(3).toString(),"yyyy.MM.dd");
    QString fis_no            = sql_query.VALUE(4).toString();
    int modul_id              = sql_query.VALUE(5).toInt();
    int program_id            = sql_query.VALUE(6).toInt();

    int siradaki_fis_no = -1;

    if (m_ui->lineEdit_yeni_fis_no->text() EQ "*") {
        siradaki_fis_no = ISL_SIRADAKI_FIS_NO_AL ( m_ui->adakDate_yeni_fis_tarihi->DATE() );
        m_ui->lineEdit_yeni_fis_no->setText ( QVariant (  siradaki_fis_no ).toString() );
    }
    else {
        siradaki_fis_no = m_ui->lineEdit_yeni_fis_no->text().toInt();
    }

    if ( siradaki_fis_no <= 0 ) {
        return;
    }

    DB->START_TRANSACTION();

    sql_query.PREPARE_INSERT ( "isl_fisi","fis_id", "fis_no, fis_turu, fis_tarihi, "
                             "aciklama, kdv_haric_toplam, kdv_toplam, kdv_dahil_toplam,program_id,modul_id");

    sql_query.SET_VALUE ( ":fis_no"           , m_ui->lineEdit_yeni_fis_no->text().toInt() );
    sql_query.SET_VALUE ( ":fis_turu"         , m_fis_turu );
    sql_query.SET_VALUE ( ":fis_tarihi"       , m_ui->adakDate_yeni_fis_tarihi->DATE() );
    sql_query.SET_VALUE ( ":aciklama"         , m_ui->textedit_yeni_aciklama->toPlainText() );
    sql_query.SET_VALUE ( ":kdv_haric_toplam"  , kdv_haric_toplam );
    sql_query.SET_VALUE ( ":kdv_toplam"       , kdv_toplam );
    sql_query.SET_VALUE ( ":kdv_dahil_toplam" , kdv_dahil_toplam );
    sql_query.SET_VALUE ( ":program_id"       , program_id );
    sql_query.SET_VALUE ( ":modul_id"         , modul_id );

    int kopyalanan_fis_id = sql_query.INSERT();

    sql_query.PREPARE_SELECT ( "isl_fis_satirlari","order_number, belge_no, aciklama, gider_turu, kdv_orani, "
                               "satir_kdv_haric_tutar, satir_kdv_tutari, satir_kdv_dahil_tutar,modul_id,program_id ",
                               "fis_id = :fis_id" );
    sql_query.SET_VALUE(":fis_id", m_fis_id );
    if ( sql_query.SELECT() EQ 0 ) {
        DB->COMMIT_TRANSACTION();
        return;
    }
    while ( sql_query.NEXT() EQ true ) {

        int         order_number         = sql_query.VALUE(0).toInt();
        QString     belge_no                = sql_query.VALUE(1).toString();
        QString     aciklama                = sql_query.VALUE(2).toString();
        int         gider_turu              = sql_query.VALUE(3).toInt();
        int         kdv_orani               = sql_query.VALUE(4).toInt();
        double      satir_kdv_haric_tutar   = sql_query.VALUE(5).toDouble();
        double      satir_kdv_tutari        = sql_query.VALUE(6).toDouble();
        double      satir_kdv_dahil_tutar   = sql_query.VALUE(7).toDouble();
        modul_id                            = sql_query.VALUE(8).toInt();
        program_id                          = sql_query.VALUE(9).toInt();


        insert_query.PREPARE_INSERT ( "isl_fis_satirlari","fis_satiri_id", "fis_id, "
                                      "order_number, belge_no, aciklama, gider_turu, kdv_orani, "
                                      "satir_kdv_haric_tutar, satir_kdv_tutari, satir_kdv_dahil_tutar,modul_id,program_id");


        insert_query.SET_VALUE ( ":fis_id",                   kopyalanan_fis_id );
        insert_query.SET_VALUE ( ":order_number",             order_number );
        insert_query.SET_VALUE ( ":belge_no",                 belge_no );
        insert_query.SET_VALUE ( ":aciklama",                 aciklama );
        insert_query.SET_VALUE ( ":gider_turu",               gider_turu );
        insert_query.SET_VALUE ( ":kdv_orani",                kdv_orani );
        insert_query.SET_VALUE ( ":satir_kdv_haric_tutar",    satir_kdv_haric_tutar );
        insert_query.SET_VALUE ( ":satir_kdv_tutari",         satir_kdv_tutari );
        insert_query.SET_VALUE ( ":satir_kdv_dahil_tutar",    satir_kdv_dahil_tutar );
        insert_query.SET_VALUE ( ":modul_id",                 modul_id );
        insert_query.SET_VALUE ( ":program_id",               program_id);
        insert_query.INSERT();

    }
    QString fis_turu;
    if ( m_fis_turu EQ ENUM_GELIR_FISI ) {
        fis_turu = QObject::tr("Gelir Fişi");
    }
    else {
        fis_turu = QObject::tr("Gider Fişi");
    }

    QString log_detaylari = QObject::tr("Log Türü - İşletme Fişi Kopyalama ,");
    log_detaylari.append(fis_turu + "," + fis_tarihi.toString("dd MMMM yyyy dddd") + ",Fiş No : "  +
                         fis_no + ",Kdv Hariç Tutar : " +
                         VIRGUL_EKLE (QVariant (ROUND (kdv_haric_toplam )).toString()) + ",Kdv Tutarı : " +
                         VIRGUL_EKLE (QVariant (ROUND (kdv_toplam      )).toString()) + ",Kdv Dahil Tutar : " +
                         VIRGUL_EKLE (QVariant (ROUND (kdv_dahil_toplam    )).toString()));

    log_detaylari.append("--Fişi Kopyalandı");

    E9_LOG_KAYDI_EKLE (ISLETME_MODULU , LOG_ISL_FIS_KOPYALAMA , LOG_ISLEM_ADD ,log_detaylari);

    DB->COMMIT_TRANSACTION();

    MSG_INFO( tr ( "Kopyalama işlemi başarıyla gerçekleştirildi." ), NULL );

    this->hide();

    OPEN_ISL_ISLETME_FISI( kopyalanan_fis_id,m_fis_turu , m_parent );
}

/**************************************************************************************
                   ISL_FIS_KOPYALAMA_BATCH::SEARCH_EDIT_CLICKED
***************************************************************************************/

void ISL_FIS_KOPYALAMA_BATCH::SEARCH_EDIT_CLICKED ( QWidget * widget, QLineEdit * lineEdit )
{
    Q_UNUSED ( widget   );

    SQL_QUERY sql_query ( DB );

    int fis_id = OPEN_ISL_FIS_ARAMA ( this );
    if ( fis_id < 1 ) {
        return;
    }
    m_fis_id = fis_id;

    sql_query.PREPARE_SELECT ( "isl_fisi","fis_no","fis_id = :fis_id" );
    sql_query.SET_VALUE      ( ":fis_id", m_fis_id );
    if ( sql_query.SELECT() EQ 0 ) {
        return;
    }
    sql_query.NEXT();
    lineEdit->setText ( sql_query.VALUE(0).toString() );

}
