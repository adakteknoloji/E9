#include "prs_ibraname_maddeleri_batch_class.h"
#include "prs_ibraname_maddeleri_batch_open.h"
#include "e9_log.h"

extern ADAK_SQL *      DB;

/**************************************************************************************
                   SHOW_PRS_IBRANAME_MADDELERI_BATCH
***************************************************************************************/

void SHOW_PRS_IBRANAME_MADDELERI_BATCH (int firma_id, QWidget * parent)
{
    PRS_IBRANAME_MADDELERI_BATCH * B = new PRS_IBRANAME_MADDELERI_BATCH ( firma_id, parent );

    B->EXEC( NOT_FULL_SCREEN );
}

/**************************************************************************************
                   PRS_IBRANAME_MADDELERI_BATCH::PRS_IBRANAME_MADDELERI_BATCH
***************************************************************************************/

PRS_IBRANAME_MADDELERI_BATCH::PRS_IBRANAME_MADDELERI_BATCH(int firma_id, QWidget * parent) : BATCH_KERNEL (parent), m_ui(new Ui::PRS_IBRANAME_MADDELERI_BATCH)
{
    m_firma_id = firma_id;

    m_ui->setupUi       (this);
    START_BATCH_KERNEL  (this, DB );
}

/**************************************************************************************
                   PRS_IBRANAME_MADDELERI_BATCH::SETUP_FORM
***************************************************************************************/

void PRS_IBRANAME_MADDELERI_BATCH::SETUP_FORM()
{
    SET_HELP_PAGE     ("sabit-degerler");
    SET_SETTING_NAME  ( "PRS - PRS_IBRANAME_MADDELERI_BATCH" );
    SET_PAGE_TITLE    ( tr("SÖZLEŞME") );

    SET_AUTO_EXIT_BATCH             ( true );

    REGISTER_BUTTON_WIDGET          ( m_ui->widget_batch_buttons );

    SQL_QUERY       sql_query( DB );

    SET_NAME_OF_RUN_BATCH_BUTTON    (tr("Kaydet"));

    SET_ICON_OF_BATCH_BUTTON        (":/e9_icons/E9_ICONS/kaydet_ikonu.png");

    m_ui->text_edit_ibraname_maddeleri->SET_MAX_STRING_LENGTH( 950 );

    DISABLE_CLEAR_ON_WIDGET( m_ui->text_edit_ibraname_maddeleri );

    SET_FIRST_FOCUS_WIDGET          ( m_ui->text_edit_ibraname_maddeleri );


    sql_query.PREPARE_SELECT("prs_sabit_degerler ","ibraname_maddeleri ",
                             "sabit_degerler_id =:sabit_degerler");
    sql_query.SET_VALUE(":sabit_degerler",1);

    if ( sql_query.SELECT() > 0 ) {
        sql_query.NEXT();
        m_ui->text_edit_ibraname_maddeleri->setText( sql_query.VALUE(0).toString() );
    }

    sql_query.PREPARE_SELECT("e9_sabit_degerler ","firma_ismi", "sabit_deger_id =:sabit_degerler");
    sql_query.SET_VALUE(":sabit_deger",1);
    if(sql_query.SELECT() EQ 0 ) {
        m_ui->label_firma_adi->clear();
    }
    else {
        sql_query.NEXT();
        m_ui->label_firma_adi->setText(sql_query.VALUE(0).toString());
    }


}

/**************************************************************************************
                   PRS_IBRANAME_MADDELERI_BATCH::CHECK_VAR
***************************************************************************************/

int PRS_IBRANAME_MADDELERI_BATCH::CHECK_VAR ( QObject * object )
{
    if ( object EQ m_ui->push_button_standart_ibraname_maddeleri_olustur ) {
        STANDART_IBRANAME_MADDELERI_OLUSTUR();
    }
    return ADAK_OK;
}

/**************************************************************************************
                   PRS_IBRANAME_MADDELERI_BATCH::CHECK_RUN
**************************************************************************************/

int PRS_IBRANAME_MADDELERI_BATCH::CHECK_RUN ()
{
    return ADAK_OK;
}

/**************************************************************************************
                   PRS_IBRANAME_MADDELERI_BATCH::RUN_BATCH
***************************************************************************************/

void PRS_IBRANAME_MADDELERI_BATCH::RUN_BATCH ()
{
    SQL_QUERY       sql_query( DB );
    QString         log_detaylari;

    DB->START_TRANSACTION();

    sql_query.PREPARE_UPDATE("prs_sabit_degerler","sabit_degerler_id", "ibraname_maddeleri ",
                             "sabit_degerler_id =:sabit_degerler_id");
    sql_query.SET_VALUE(":sabit_degerler_id",1);

    sql_query.SET_VALUE( ":ibraname_maddeleri" , m_ui->text_edit_ibraname_maddeleri->toPlainText() );

    sql_query.UPDATE();

    log_detaylari = "Ibraname Maddeleri Aşağıdaki Gibi Güncellenmiştir.\n" +
                    m_ui->text_edit_ibraname_maddeleri->toPlainText();

    E9_LOG_KAYDI_EKLE(PERSONEL_MODULU , LOG_PRS_IBRANAME_MADDELERI , LOG_ISLEM_UPDATE , log_detaylari);

    DB->COMMIT_TRANSACTION();

    close();
}

/**************************************************************************************
                   PRS_IBRANAME_MADDELERI_BATCH::STANDART_IBRANAME_MADDELERI_OLUSTUR
**************************************************************************************/

void PRS_IBRANAME_MADDELERI_BATCH::STANDART_IBRANAME_MADDELERI_OLUSTUR ()
{
    QString         standart_ibraname_maddeleri;

    standart_ibraname_maddeleri = "tarihinden beri çalıştığım işyerinden yukarıda gösterilen şekilde ayrıldım.\n"
                                  "Çalışmaya başladığım tarihten bugüne kadar geçen çalışma sürem içinde bütün ücretlerimi \n"
                                  "ve her türlü sosyal hakkım ile hafta ve genel tatil günlerine ait ücretlerimi ayrıca \n"
                                  "yapmış olduğum fazla mesai ücretlerim ile yıllık izin ücretlerimi ve ayrılış esnasında \n"
                                  "hak etmiş olduğum ve yukarıda belirtilen istihkaklarımı eksiksiz aldım.\n"
                                  "\t\t\t İşyerinden, Kanun ve Hizmet akdinden doğan hiçbir şekil ve nam altında herhangi bir \n"
                                  "alacağım kalmadığını maddi ve manevi herhangi bir hak talebinde bulunmayacağımı beyanla \n"
                                  "işyerini geçmişe dönük olarak da ibra ederim."
                                  "\n";

    m_ui->text_edit_ibraname_maddeleri->setText( standart_ibraname_maddeleri );
}



