#include "stok_miktarlari_batch_class.h"
#include "ui_stok_miktarlari_batch.h"
#include "print.h"
#include "e9_enum.h"
#include "stok_enum.h"
#include "stok_struct.h"
#include "stok_console_utils.h"
#include "sube_console_utils.h"
#include "adak_gruplar.h"
#include "e9_console_utils.h"
#include "e9_gui_utils.h"
#include "stok_miktarlari_raporu_open.h"
#include "stok_urun_arama_open.h"
#include "sube_depo_secimi_open.h"
#include "sube_enum.h"


extern ADAK_SQL *           DB;

/**************************************************************************************
                   OPEN_STOK_MIKTARLARI_BATCH
***************************************************************************************/

void OPEN_STOK_MIKTARLARI_BATCH ( QWidget * parent )
{
    STOK_MIKTARLARI_BATCH * F = new STOK_MIKTARLARI_BATCH ( parent );
    F->EXEC( NOT_FULL_SCREEN );
}

/**************************************************************************************
                   STOK_MIKTARLARI_BATCH::STOK_MIKTARLARI_BATCH
***************************************************************************************/

STOK_MIKTARLARI_BATCH::STOK_MIKTARLARI_BATCH ( QWidget * parent ) :  BATCH_KERNEL ( parent ), m_ui ( new Ui::STOK_MIKTARLARI_BATCH )
{
    m_ui->setupUi       ( this );
    START_BATCH_KERNEL  ( this, DB );
}

/**************************************************************************************
                   STOK_MIKTARLARI_BATCH::SETUP_FORM
***************************************************************************************/

void STOK_MIKTARLARI_BATCH::SETUP_FORM()
{
    SET_PAGE_TITLE    ( tr ( "STK - STOK MİKTARI RAPORU" ) );
    SET_HELP_PAGE     ("stok-islemleri_stok-miktarlari-raporu");

    REGISTER_BUTTON_WIDGET ( m_ui->widget_batch_buttons );

    m_ui->adakDate_stok_tarihi->SET_DATE( QDate::currentDate() );

    m_depo_id     = -1;

    m_ui->frame_sirket->setHidden(true);

    if ( E9_KULLANICI_SIRKET_DEGISTIRE_BILIR_MI() EQ EVET ) {
        m_ui->frame_sirket->setHidden(false);
    }

    SET_NAME_OF_RUN_BATCH_BUTTON ( "Rapor Al" );
    SET_ENTER_KEY_FOR_RUN_BATCH(true);

    SET_FIRST_FOCUS_WIDGET ( m_ui->searchEdit_bas_urun_kodu );

    SET_SETTING_NAME  ("STOK_MIKTARLARI_BATCH");

    SQL_QUERY query(DB);

    query.PREPARE_SELECT("stk_urunler","urun_kodu,urun_adi" ,"hizmet_mi = :hizmet_mi" );

    query.SET_VALUE(":hizmet_mi" , 0);

    if ( query.SELECT("urun_kodu ASC",0,1) NE 0 ) {
        query.NEXT();
        m_ui->searchEdit_bas_urun_kodu->SET_TEXT(query.VALUE(0).toString());
        m_ui->lineEdit_bas_urun_adi->setText(query.VALUE(1).toString());
    }

    query.PREPARE_SELECT("stk_urunler","urun_kodu,urun_adi" ,"hizmet_mi = :hizmet_mi" );
    query.SET_VALUE(":hizmet_mi" , 0);

    if ( query.SELECT("urun_kodu DESC",0,1) NE 0 ) {
        query.NEXT();
        m_ui->searchEdit_bts_urun_kodu->SET_TEXT(query.VALUE(0).toString());
        m_ui->lineEdit_bts_urun_adi->setText(query.VALUE(1).toString());
    }

    m_depo_id = SUBE_GET_UNITENIN_DEPOSU();

    if ( SUBE_GET_DEPO_ISLEM_YETKISI()  EQ SADECE_UNITE_DEPOSUNDA ) {
        m_ui->frame_depo_araligi->setHidden(true);
        m_ui->checkBox_depo_kodu->setHidden(true);
    }
    else {
        QString depo_kodu;
        QString depo_adi;

        SUBE_GET_DEPO_KODU_ADI(m_depo_id, &depo_kodu, &depo_adi);

        m_ui->searchEdit_depo_kodu->SET_TEXT(depo_kodu);
        m_ui->lineEdit_depo_ismi->setText(depo_adi);
    }

    m_ui->frame_depo_araligi->setDisabled(true);
    m_ui->frame_grup_araligi->setDisabled(true);

    m_ui->frame_stok_tarihi->setEnabled( false );

}

/**************************************************************************************
                   STOK_MIKTARLARI_BATCH::CHECK_VAR
***************************************************************************************/

int STOK_MIKTARLARI_BATCH::CHECK_VAR ( QObject * object )
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

        query.PREPARE_SELECT("stk_urunler" , "urun_adi" , "(urun_kodu = :urun_kodu OR "
                             "barkod_numarasi = :barkod_numarasi) AND hizmet_mi = :hizmet_mi");

        if ( object EQ m_ui->searchEdit_bas_urun_kodu ) {
            query.SET_VALUE(":urun_kodu"       , m_ui->searchEdit_bas_urun_kodu->GET_TEXT());
            query.SET_VALUE(":barkod_numarasi" , m_ui->searchEdit_bas_urun_kodu->GET_TEXT());
        }
        else {
            query.SET_VALUE(":urun_kodu"       , m_ui->searchEdit_bts_urun_kodu->GET_TEXT());
            query.SET_VALUE(":barkod_numarasi" , m_ui->searchEdit_bts_urun_kodu->GET_TEXT());
        }

        query.SET_VALUE(":hizmet_mi" , 0);

        if ( query.SELECT() EQ 0 ) {
            if ( object EQ m_ui->searchEdit_bas_urun_kodu ) {
                MSG_WARNING(  tr("Girmiş olduğunuz başlangıç ürün kodu bulunamadı."), m_ui->searchEdit_bas_urun_kodu );
                return ADAK_FAIL_UNDO;
            }
            else {
                MSG_WARNING(  tr("Girmiş olduğunuz bitiş ürün kodu bulunamadı."), m_ui->searchEdit_bts_urun_kodu );
                return ADAK_FAIL;
            }
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

    else if ( object EQ m_ui->searchEdit_depo_kodu ) {

        if ( m_ui->searchEdit_depo_kodu->GET_TEXT().isEmpty() EQ true ) {
            m_ui->lineEdit_depo_ismi->clear();
            m_depo_id = -1;
            return ADAK_OK;
        }

        QString depo_adi;
        int     depo_id;

        int result = SUBE_GET_DEPO_ID_ADI( m_ui->searchEdit_depo_kodu->GET_TEXT(), &depo_id, &depo_adi );

        if ( result EQ 0 ) {
            MSG_WARNING( tr("Aradığınız depo kodu bulunamadı."), m_ui->searchEdit_depo_kodu );

            return ADAK_FAIL;
        }
        m_ui->lineEdit_depo_ismi->setText( depo_adi );
        m_depo_id = depo_id;
    }

    else if ( object EQ m_ui->checkBox_urun_araligi ) {

        if ( m_ui->checkBox_urun_araligi->isChecked() EQ true ) {

            URUN_FRAME_AYARLA( true  );
            SQL_QUERY query(DB);

            query.PREPARE_SELECT("stk_urunler","urun_kodu,urun_adi" ,"hizmet_mi = :hizmet_mi");
            query.SET_VALUE(":hizmet_mi" , 0);

            if ( query.SELECT("urun_kodu ASC" , 0 , 1) NE 0 ) {
                query.NEXT();
                m_ui->searchEdit_bas_urun_kodu->SET_TEXT(query.VALUE(0).toString());
                m_ui->lineEdit_bas_urun_adi->setText(query.VALUE(1).toString());
            }

            query.PREPARE_SELECT("stk_urunler","urun_kodu,urun_adi" ," hizmet_mi = :hizmet_mi");
            query.SET_VALUE     (":hizmet_mi" , 0);


            if ( query.SELECT("urun_kodu DESC" , 0 , 1) NE 0 ) {
                query.NEXT();
                m_ui->searchEdit_bts_urun_kodu->SET_TEXT(query.VALUE(0).toString());
                m_ui->lineEdit_bts_urun_adi->setText(query.VALUE(1).toString());
            }
        }
        else {
            URUN_FRAME_AYARLA( false );
        }
    }

    else if ( object EQ m_ui->checkBox_grup_araligi) {
        if ( m_ui->checkBox_grup_araligi->isChecked() EQ true ) {
            GRUP_FRAME_AYARLA( true  );
        }
        else {
            GRUP_FRAME_AYARLA( false );
        }

    }

    else if ( object EQ m_ui->checkBox_depo_kodu ) {
        if ( m_ui->checkBox_depo_kodu->isChecked() EQ false ) {
            m_ui->frame_depo_araligi->setDisabled(true);
            QString depo_kodu;
            QString depo_adi;

            m_depo_id = SUBE_GET_UNITENIN_DEPOSU();
            SUBE_GET_DEPO_KODU_ADI(m_depo_id, &depo_kodu, &depo_adi);

            m_ui->searchEdit_depo_kodu->SET_TEXT(depo_kodu);
            m_ui->lineEdit_depo_ismi->setText(depo_adi);
        }
        else {
            m_ui->frame_depo_araligi->setEnabled(true);
            SET_FOCUS( m_ui->searchEdit_depo_kodu );

        }
    }
    else if ( object EQ m_ui->pushButton_grup_sec ) {
        OPEN_GRUP_SECIM( E9_PROGRAMI,STOK_MODULU, &m_grup_idleri, DB , this , true );
        QStringList grup_adlari;
        QStringList grup_kodlari;

        m_ui->textEdit_gruplar->setText(GRP_GRUP_ADLARINI_BUL(&m_grup_idleri, &grup_adlari, &grup_kodlari));
    }
    else if ( object EQ m_ui->checkBox_stok_tarihi ) {
        if ( m_ui->checkBox_stok_tarihi->isChecked() EQ true ) {
            m_ui->frame_stok_tarihi->setEnabled( true );
        }
        else {
            m_ui->frame_stok_tarihi->setEnabled( false );
        }
    }

    return ADAK_OK;
}

/**************************************************************************************
                   STOK_MIKTARLARI_BATCH::CHECK_RUN
***************************************************************************************/

int STOK_MIKTARLARI_BATCH::CHECK_RUN ()
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
    if ( m_ui->checkBox_grup_araligi->isChecked() EQ true ) {
        if ( m_ui->textEdit_gruplar->toPlainText().isEmpty() EQ true ) {
            MSG_WARNING( tr("Grup kodu girmelisiniz."), m_ui->pushButton_grup_sec );
            return ADAK_FAIL;
        }
    }

    if ( m_ui->checkBox_depo_kodu->isChecked() EQ true ) {
        if ( m_ui->searchEdit_depo_kodu->GET_TEXT().isEmpty() EQ true ) {
            MSG_WARNING( tr ("Depo kodunu boş bırakamazsınız."), m_ui->searchEdit_depo_kodu );
            return ADAK_FAIL;
        }
    }

    return ADAK_OK;
}

/**************************************************************************************
                   STOK_MIKTARLARI_BATCH::RUN_BATCH
***************************************************************************************/

void STOK_MIKTARLARI_BATCH::RUN_BATCH ()
{
    STOK_RAPOR_VARS * STOK_RV = new STOK_RAPOR_VARS;

    STOK_RV->grup_idleri.clear();
    STOK_RV->bas_urun_kodu.clear();
    STOK_RV->bts_urun_kodu.clear();

    if ( m_ui->checkBox_grup_araligi->isChecked() EQ true ) {
        STOK_RV->grup_idleri   = m_grup_idleri;
    }
    if ( m_ui->checkBox_urun_araligi->isChecked() EQ true) {
        STOK_RV->bas_urun_kodu = m_ui->searchEdit_bas_urun_kodu->GET_TEXT();
        STOK_RV->bts_urun_kodu = m_ui->searchEdit_bts_urun_kodu->GET_TEXT();
    }

    STOK_RV->baslangic_tarihi = m_ui->adakDate_stok_tarihi->QDATE();

    STOK_RV->depo_id          = m_depo_id;
    STOK_RV->ent_filtresi     = m_entegrasyon_durumu;

    OPEN_REPORT_SHOWER ( GET_STOK_MIKTARLARI_RAPORU(STOK_RV), nativeParentWidget() );

    delete STOK_RV;
}

/**************************************************************************************
                   STOK_MIKTARLARI_BATCH::CHECK_EXIT
***************************************************************************************/

int STOK_MIKTARLARI_BATCH::CHECK_EXIT ()
{
    return ADAK_OK;
}


/**************************************************************************************
                   STOK_MIKTARLARI_BATCH::SEARCH_EDIT_CLICKED
***************************************************************************************/

void STOK_MIKTARLARI_BATCH::SEARCH_EDIT_CLICKED ( QWidget * widget, QLineEdit * line_edit)
{
    if ( widget EQ m_ui->searchEdit_bas_urun_kodu OR widget EQ m_ui->searchEdit_bts_urun_kodu ) {
        int urun_id = OPEN_STOK_URUN_ARAMA(ENUM_URUN_KARTI ,-1, line_edit->text(),-1,this );

        if ( urun_id NE -1) {
            SQL_QUERY query(DB);

            query.PREPARE_SELECT("stk_urunler","urun_kodu" ,"urun_id = :urun_id");
            query.SET_VALUE(":urun_id" , urun_id);

            if ( query.SELECT() NE 0 ) {
                query.NEXT();
                line_edit->setText(query.VALUE(0).toString());
            }
        }
    }
    if ( widget EQ m_ui->searchEdit_depo_kodu ) {
        int depo_id = SUBE_DEPO_SEC ( SUBE_GET_SUBE_ID(),this );

        if ( depo_id NE -1 ) {
            line_edit->setText(SUBE_GET_DEPO_KODU(depo_id));
            m_depo_id = depo_id;
        }
    }
}

/**************************************************************************************
                   STOK_MIKTARLARI_BATCH::GRUP_FRAME_AYARLA
***************************************************************************************/

void STOK_MIKTARLARI_BATCH::GRUP_FRAME_AYARLA(bool p_state )
{
    m_ui->checkBox_grup_araligi->setChecked( p_state );
    m_ui->frame_grup_araligi->setEnabled   ( p_state );

    m_grup_idleri.clear();
    m_ui->textEdit_gruplar->clear();
}

/**************************************************************************************
                   STOK_MIKTARLARI_BATCH::URUN_FRAME_AYARLA
***************************************************************************************/

void STOK_MIKTARLARI_BATCH::URUN_FRAME_AYARLA( bool p_state )
{
    m_ui->checkBox_urun_araligi->setChecked( p_state );
    m_ui->frame_urun_araligi->setEnabled   ( p_state );

    m_ui->searchEdit_bas_urun_kodu->CLEAR();
    m_ui->searchEdit_bts_urun_kodu->CLEAR();

    m_ui->lineEdit_bas_urun_adi->clear();
    m_ui->lineEdit_bts_urun_adi->clear();
}

