#include "stok_urun_listesi_batch_class.h"
#include "ui_stok_urun_listesi_batch.h"
#include "print.h"
#include "adak_gruplar.h"
#include "stok_urun_listesi_raporu_open.h"
#include "stok_urun_arama_open.h"
#include "e9_enum.h"
#include "stok_enum.h"
#include "stok_struct.h"

extern ADAK_SQL * DB;

/**************************************************************************************
                   OPEN_STOK_URUN_LISTESI_BATCH
***************************************************************************************/

void OPEN_STOK_URUN_LISTESI_BATCH ( int p_urun_mu_hizmet_mi, QWidget * parent )
{
    STOK_URUN_LISTESI_BATCH * F = new STOK_URUN_LISTESI_BATCH ( p_urun_mu_hizmet_mi, parent );
    F->EXEC( NOT_FULL_SCREEN );
}

/**************************************************************************************
                   STOK_URUN_LISTESI_BATCH::STOK_URUN_LISTESI_BATCH
***************************************************************************************/

STOK_URUN_LISTESI_BATCH::STOK_URUN_LISTESI_BATCH ( int p_urun_mu_hizmet_mi, QWidget * parent ) :  BATCH_KERNEL ( parent ), m_ui ( new Ui::STOK_URUN_LISTESI_BATCH )
{
    m_urun_mu_hizmet_mi = p_urun_mu_hizmet_mi;

    m_ui->setupUi       (this );
    START_BATCH_KERNEL  (this, DB );
}

/**************************************************************************************
                   STOK_URUN_LISTESI_BATCH::SETUP_FORM
***************************************************************************************/

void STOK_URUN_LISTESI_BATCH::SETUP_FORM()
{

    if ( m_urun_mu_hizmet_mi EQ ENUM_URUN_KARTI ) {
        SET_PAGE_TITLE( tr ( "STK - ÜRÜN LİSTESİ" ) );
        SET_SETTING_NAME( "STOK_URUN_LISTESI_BATCH" );
    }
    else {
        SET_PAGE_TITLE( tr ( "STK - HİZMET LİSTESİ" ) );
        SET_SETTING_NAME( "STOK_HIZMET_LISTESI_BATCH" );
    }

    SET_HELP_PAGE("urun-listesi");

    REGISTER_BUTTON_WIDGET ( m_ui->widget_batch_buttons );
    SET_NAME_OF_RUN_BATCH_BUTTON ( "Rapor Al" );
    SET_FIRST_FOCUS_WIDGET       ( m_ui->searchEdit_bas_urun_kodu );
    SET_ENTER_KEY_FOR_RUN_BATCH  ( true );

    SQL_QUERY query(DB);

    query.PREPARE_SELECT( "stk_urunler", "urun_kodu,urun_adi", "hizmet_mi = :hizmet_mi" );
    query.SET_VALUE(":hizmet_mi", m_urun_mu_hizmet_mi );

    if ( query.SELECT("urun_kodu", 0, 1 ) NE 0 ) {
        query.NEXT();
        m_ui->searchEdit_bas_urun_kodu->SET_TEXT(query.VALUE(0).toString());
        m_ui->lineEdit_bas_urun_adi->setText(query.VALUE(1).toString());
    }

    query.PREPARE_SELECT("stk_urunler","urun_kodu,urun_adi","hizmet_mi = :hizmet_mi");

    query.SET_VALUE(":hizmet_mi", m_urun_mu_hizmet_mi );

    if ( query.SELECT("urun_kodu DESC", 0, 1 ) NE 0 ) {
        query.NEXT();
        m_ui->searchEdit_bts_urun_kodu->SET_TEXT(query.VALUE(0).toString());
        m_ui->lineEdit_bts_urun_adi->setText(query.VALUE(1).toString());
    }

    m_ui->frame_grup_kodu->setDisabled(true);
}

/**************************************************************************************
                   STOK_URUN_LISTESI_BATCH::CHECK_VAR
***************************************************************************************/

int STOK_URUN_LISTESI_BATCH::CHECK_VAR ( QObject * object )
{
    if ( object EQ m_ui->searchEdit_bas_urun_kodu OR object EQ m_ui->searchEdit_bts_urun_kodu ) {

        if ( object EQ m_ui->searchEdit_bas_urun_kodu ) {
            if ( m_ui->searchEdit_bas_urun_kodu->GET_TEXT().isEmpty() EQ true ) {
                m_ui->lineEdit_bas_urun_adi->clear();
                return ADAK_OK;
            }
        }
        else {
            if ( m_ui->searchEdit_bts_urun_kodu->GET_TEXT().isEmpty() EQ true ) {
                m_ui->lineEdit_bts_urun_adi->clear();
                return ADAK_OK;
            }
        }

        SQL_QUERY query(DB);

        query.PREPARE_SELECT("stk_urunler","urun_adi","(urun_kodu = :urun_kodu OR "
                             "barkod_numarasi = :barkod_numarasi) AND hizmet_mi = :hizmet_mi");

        query.SET_VALUE(":hizmet_mi", m_urun_mu_hizmet_mi );

        if ( object EQ m_ui->searchEdit_bas_urun_kodu ) {
            query.SET_VALUE(":urun_kodu"       , m_ui->searchEdit_bas_urun_kodu->GET_TEXT());
            query.SET_VALUE(":barkod_numarasi" , m_ui->searchEdit_bas_urun_kodu->GET_TEXT());
        }
        else {
            query.SET_VALUE(":urun_kodu"       , m_ui->searchEdit_bts_urun_kodu->GET_TEXT());
            query.SET_VALUE(":barkod_numarasi" , m_ui->searchEdit_bts_urun_kodu->GET_TEXT());
        }
        if ( query.SELECT() EQ 0 ) {
            MSG_WARNING(  tr("Girmiş olduğunuz ürün kodu bulunamadı."), m_ui->searchEdit_bas_urun_kodu );

            return ADAK_FAIL_UNDO;
        }
        query.NEXT();
        QString urun_adi = query.VALUE(0).toString();

        if ( object EQ m_ui->searchEdit_bas_urun_kodu ) {
            m_ui->lineEdit_bas_urun_adi->setText(urun_adi);
        }
        else {
            m_ui->lineEdit_bts_urun_adi->setText(urun_adi);
        }
    }

    if ( object EQ m_ui->checkBox_urun_araligi ) {
        if ( m_ui->checkBox_urun_araligi->isChecked() EQ false ) {

            SQL_QUERY query(DB);

            query.PREPARE_SELECT("stk_urunler","urun_kodu,urun_adi","hizmet_mi = :hizmet_mi");
            query.SET_VALUE(":hizmet_mi", m_urun_mu_hizmet_mi );

            if ( query.SELECT("urun_kodu",0,1) NE 0 ) {
                query.NEXT();
                m_ui->searchEdit_bas_urun_kodu->SET_TEXT(query.VALUE(0).toString());
                m_ui->lineEdit_bas_urun_adi->setText(query.VALUE(1).toString());
            }

            query.PREPARE_SELECT("stk_urunler","urun_kodu,urun_adi","hizmet_mi = :hizmet_mi");
            query.SET_VALUE(":hizmet_mi", m_urun_mu_hizmet_mi );

            if ( query.SELECT("urun_kodu DESC",0,1) NE 0 ) {
                query.NEXT();
                m_ui->searchEdit_bts_urun_kodu->SET_TEXT(query.VALUE(0).toString());
                m_ui->lineEdit_bts_urun_adi->setText(query.VALUE(1).toString());
            }

            m_ui->frame_urun_araligi->setDisabled(true);
            m_ui->searchEdit_bas_urun_kodu->setDisabled(true);
            m_ui->lineEdit_bas_urun_adi->setDisabled(true);
            m_ui->searchEdit_bts_urun_kodu->setDisabled(true);
            m_ui->lineEdit_bts_urun_adi->setDisabled(true);

        }
        else {
            m_ui->frame_urun_araligi->setDisabled(false);
            m_ui->searchEdit_bas_urun_kodu->setDisabled(false);
            m_ui->lineEdit_bas_urun_adi->setDisabled(false);
            m_ui->searchEdit_bts_urun_kodu->setDisabled(false);
            m_ui->lineEdit_bts_urun_adi->setDisabled(false);
            SET_FOCUS( m_ui->searchEdit_bas_urun_kodu );
        }
    }

    if ( object EQ m_ui->checkBox_grup_kodu ) {
        if ( m_ui->checkBox_grup_kodu->isChecked() EQ false ) {
            m_ui->frame_grup_kodu->setDisabled(true);
            m_ui->textEdit_gruplar->clear();;
        }
        else {
            m_ui->frame_grup_kodu->setEnabled(true);
            SET_FOCUS( m_ui->pushButton_grup_sec );
        }
    }
    if ( object EQ m_ui->pushButton_grup_sec ) {
        OPEN_GRUP_SECIM( E9_PROGRAMI,STOK_MODULU , &m_grup_idleri, DB , this, true );

        QStringList grup_adlari;
        QStringList grup_kodlari;

        m_ui->textEdit_gruplar->setText(GRP_GRUP_ADLARINI_BUL(&m_grup_idleri , &grup_adlari, &grup_kodlari));
    }

    return ADAK_OK;
}

/**************************************************************************************
                   STOK_URUN_LISTESI_BATCH::CHECK_RUN
***************************************************************************************/

int STOK_URUN_LISTESI_BATCH::CHECK_RUN ()
{

    if ( m_ui->searchEdit_bas_urun_kodu->GET_TEXT().isEmpty() EQ false AND
         m_ui->searchEdit_bts_urun_kodu->GET_TEXT().isEmpty() EQ false ) {

        if ( m_ui->searchEdit_bas_urun_kodu->GET_TEXT() > m_ui->searchEdit_bts_urun_kodu->GET_TEXT() ) {
            MSG_WARNING( tr("Başlangıç ürün kodu , bitiş ürün kodundan büyük olamaz."), m_ui->searchEdit_bas_urun_kodu );
            return ADAK_FAIL;
        }
    }
    if ( m_ui->checkBox_urun_araligi->isChecked() EQ true ) {
        if ( m_ui->searchEdit_bas_urun_kodu->GET_TEXT().isEmpty() EQ true ) {
            MSG_WARNING( tr("Başlangıç ürün Kodunu veya barkod numarasını girmelisiniz."), m_ui->searchEdit_bas_urun_kodu );
            return ADAK_FAIL;
        }

        if ( m_ui->searchEdit_bts_urun_kodu->GET_TEXT().isEmpty() EQ true ) {
            MSG_WARNING(  tr ("Bitiş ürün kodunu veya barkod numarasını girmelisiniz."), m_ui->searchEdit_bts_urun_kodu );
            return ADAK_FAIL;
        }
    }

    if ( m_ui->checkBox_grup_kodu->isChecked() EQ true ) {
        if ( m_ui->textEdit_gruplar->toPlainText().isEmpty() EQ true ) {
            MSG_WARNING(  tr("Stok grup kodunu seçmelisiniz."), m_ui->pushButton_grup_sec );
            return ADAK_FAIL;
        }
    }

    return ADAK_OK;
}

/**************************************************************************************
                   STOK_URUN_LISTESI_BATCH::RUN_BATCH
***************************************************************************************/

void STOK_URUN_LISTESI_BATCH::RUN_BATCH ()
{
    STOK_RAPOR_VARS * STOK_RV = new STOK_RAPOR_VARS;

    STOK_RV->bas_urun_kodu      = m_ui->searchEdit_bas_urun_kodu->GET_TEXT();
    STOK_RV->bts_urun_kodu      = m_ui->searchEdit_bts_urun_kodu->GET_TEXT();
    STOK_RV->grup_idleri        = m_grup_idleri;
    STOK_RV->urun_mu_hizmet_mi  = m_urun_mu_hizmet_mi;

    OPEN_REPORT_SHOWER ( GET_STOK_URUN_LISTESI_RAPORU(STOK_RV), nativeParentWidget() );

    delete STOK_RV;
}


/**************************************************************************************
                   STOK_URUN_LISTESI_BATCH::SEARCH_EDIT_CLICKED()
***************************************************************************************/

void STOK_URUN_LISTESI_BATCH::SEARCH_EDIT_CLICKED(QWidget *widget, QLineEdit *line_edit)
{
    if ( widget EQ m_ui->searchEdit_bas_urun_kodu OR widget EQ m_ui->searchEdit_bts_urun_kodu ) {
        int urun_id = OPEN_STOK_URUN_ARAMA( m_urun_mu_hizmet_mi ,-1, line_edit->text() ,-1,this );

        if ( urun_id NE -1) {
            SQL_QUERY query(DB);

            query.PREPARE_SELECT("stk_urunler","urun_kodu","urun_id = :urun_id");
            query.SET_VALUE(":urun_id" , urun_id);

            if ( query.SELECT() NE 0 ) {
                query.NEXT();
                line_edit->setText(query.VALUE(0).toString());
            }
        }
    }
}
