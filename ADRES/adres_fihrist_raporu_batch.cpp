#include <QMessageBox>
#include "print.h"
#include "adres_fihrist_raporu_batch_class.h"
#include "adres_fihrist_raporu_batch_open.h"
#include "ui_adres_fihrist_raporu_batch.h"
#include "adres_fihrist_raporu_open.h"

#include "adak_gruplar.h"

extern ADAK_SQL * DB;

/**************************************************************************************
                   OPEN_ADRES_FIHRIST_RAPORU_BATCH
***************************************************************************************/

void OPEN_ADRES_FIHRIST_RAPORU_BATCH (QWidget * parent)
{
    ADRES_FIHRIST_RAPORU_BATCH * B = new ADRES_FIHRIST_RAPORU_BATCH ( parent );
    B->EXEC( NOT_FULL_SCREEN );

}

/**************************************************************************************
                   ADRES_FIHRIST_RAPORU_BATCH::ADRES_FIHRIST_RAPORU_BATCH
***************************************************************************************/

ADRES_FIHRIST_RAPORU_BATCH::ADRES_FIHRIST_RAPORU_BATCH ( QWidget *parent ) :  BATCH_KERNEL ( parent), m_ui ( new Ui::ADRES_FIHRIST_RAPORU_BATCH )
{
    m_ui->setupUi       ( this );
    START_BATCH_KERNEL  (this,DB );
}

/**************************************************************************************
                   ADRES_FIHRIST_RAPORU_BATCH::SETUP_FORM
***************************************************************************************/

void ADRES_FIHRIST_RAPORU_BATCH::SETUP_FORM ()
{
    SET_HELP_PAGE           ("adres-toplu-mail-sms_fihrist-raporu") ;
    SET_PAGE_TITLE          ( tr ( "ADR - FİHRİST RAPORU" ) );

    SET_SETTING_NAME        ( "ADRES_FIHRIST_RAPORU_BATCH" );
    REGISTER_BUTTON_WIDGET  ( m_ui->widget_run_batch_button );

    QStringList harf_dizisi;
    harf_dizisi << "0"<<"1"<<"2"<<"3"<<"4"<<"5"<<"6"<<"7"<<"8"<<"9"
                << "a"<<"b"<<"c"<<"Ç"<<"d"<<"e"<<"f"<<"g"<<"h"<<"ı"<<"İ"<<"j"
                <<"k"<<"l"<<"m"<<"n"<<"o"<<"Ö"<<"p"<<"r"<<"s"<<"Ş"<<"t"<<"u"<<"Ü"<<"v"<<"y"<<"z";

    SET_NAME_OF_RUN_BATCH_BUTTON ( "Fihrist Oluştur" );

    SET_FIRST_FOCUS_WIDGET ( m_ui->combo_box_baslangic );

    m_ui->combo_box_baslangic->addItems ( harf_dizisi );
    m_ui->combo_box_bitis->addItems ( harf_dizisi );

    m_ui->combo_box_bitis->setCurrentIndex ( harf_dizisi.size() -1 );    

    m_grup_idleri.clear();
}

/**************************************************************************************
                   ADRES_FIHRIST_RAPORU_BATCH::CHECK_VAR
***************************************************************************************/

int ADRES_FIHRIST_RAPORU_BATCH::CHECK_VAR ( QObject * object )
{
    if ( object EQ m_ui->push_button_grup_ekle ) {
        OPEN_GRUP_SECIM(E9_PROGRAMI , ADRES_MODULU , &m_grup_idleri,DB,this,true);
        m_ui->text_edit_gruplar->setText(GRP_GRUP_ADLARINI_BUL(&m_grup_idleri , new QStringList(),new QStringList()));
    }

    return ADAK_OK;
}

/**************************************************************************************
                   ADRES_FIHRIST_RAPORU_BATCH::CHECK_RUN
***************************************************************************************/

int ADRES_FIHRIST_RAPORU_BATCH::CHECK_RUN()
{
    return ADAK_OK;
}

/**************************************************************************************
                   ADRES_FIHRIST_RAPORU_BATCH::RUN_BATCH
***************************************************************************************/

void ADRES_FIHRIST_RAPORU_BATCH::RUN_BATCH()
{
    QList<int> gruba_gore_idler;

    if ( m_grup_idleri.size() > 0 ) {
        gruba_gore_idler = GRP_GRUBA_EKLI_KAYITLARI_BUL(E9_PROGRAMI,ADRES_MODULU,m_grup_idleri);
    }

    OPEN_REPORT_SHOWER ( OPEN_ADRES_FIHRIST_RAPORU ( m_ui->combo_box_baslangic->currentIndex(), ( m_ui->combo_box_bitis->currentIndex() ) + 1 , gruba_gore_idler),nativeParentWidget() );
}
