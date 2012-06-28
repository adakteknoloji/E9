#include "prs_isten_ayrilanlar_arama_class.h"
#include "prs_isten_ayrilanlar_arama_open.h"
#include "print.h"
#include "prs_con_utils.h"
#include "cari_console_utils.h"
#include "prs_isten_ayrilma_raporu_open.h"
#include "cari_struct.h"

extern ADAK_SQL * DB;



/**************************************************************************************
                   SHOW_PRS_ISTEN_AYRILANLAR_ARAMA
***************************************************************************************/

int SHOW_PRS_ISTEN_AYRILANLAR_ARAMA( QWidget * parent)
{
    int secilen_id    = -1;

    PRS_ISTEN_AYRILANLAR_ARAMA * A = new PRS_ISTEN_AYRILANLAR_ARAMA( parent);
    A->m_secilen_id = &secilen_id;
    A->EXEC( NOT_FULL_SCREEN );

    return secilen_id;

}

/**************************************************************************************
                   PRS_ISTEN_AYRILANLAR_ARAMA::PRS_ISTEN_AYRILANLAR_ARAMA
**************************************************************************************/

PRS_ISTEN_AYRILANLAR_ARAMA::PRS_ISTEN_AYRILANLAR_ARAMA(QWidget *parent) : ARAMA_KERNEL (parent), m_ui(new Ui::PRS_ISTEN_AYRILANLAR_ARAMA)
{
    m_ui->setupUi       (this);
    START_ARAMA_KERNEL  (this, DB);

}

/**************************************************************************************
                   PRS_ISTEN_AYRILANLAR_ARAMA::SETUP_FORM
***************************************************************************************/

void PRS_ISTEN_AYRILANLAR_ARAMA::SETUP_FORM()
{
    SET_HELP_PAGE ("isten_ayrilanlar_batch.html");
    SET_PAGE_TITLE         ( tr ( "PRS - İŞTEN AYRILAN PERSONEL ARAMA" ) );

    REGISTER_TABLE_WIDGET           (m_ui->table_widget_personeller);
    REGISTER_ARAMA_BUTTONS_WIDGET   (m_ui->widget_batch_buttons);

    SET_HEADERS( QStringList()<<"İşten Ayrılan Id"<<"Personel Id"<<"Sicil No"<<"TC Kimlik No"<<"Personel Adı / Soyadı" );

    SET_FIRST_FOCUS_WIDGET ( m_ui->line_edit_personel_adi );

    m_ui->line_edit_personel_adi->setText("*");

    m_ui->table_widget_personeller->setColumnHidden ( 0, true );
    m_ui->table_widget_personeller->setColumnHidden ( 1, true );

    m_ui->table_widget_personeller->setColumnWidth  ( 2, 200 );
    m_ui->table_widget_personeller->setColumnWidth  ( 3, 150 );
    m_ui->table_widget_personeller->setColumnWidth  ( 4, 160 );

    SET_SETTING_NAME                                ( "PRS_ISTEN_AYR_ARAMA" );

    SET_ENTER_KEY_FOR_RUN_BATCH(true);

    SET_FIND_BUTTON_NAME("İşten Ayrılan Personel Bul");
}

/**************************************************************************************
                   PRS_ISTEN_AYRILANLAR_ARAMA::CHECK_BOX_CHANGED
**************************************************************************************/

void PRS_ISTEN_AYRILANLAR_ARAMA::CHECK_BOX_CHANGED()
{

}

/**************************************************************************************
                   PRS_ISTEN_AYRILANLAR_ARAMA::CHECK_VAR
***************************************************************************************/

int PRS_ISTEN_AYRILANLAR_ARAMA::CHECK_VAR(QObject * object)
{
    Q_UNUSED(object);
    return ADAK_OK;
}

/**************************************************************************************
                 PRS_ISTEN_AYRILANLAR_ARAMA::CHECK_RUN
**************************************************************************************/

int PRS_ISTEN_AYRILANLAR_ARAMA::CHECK_RUN()
{
    if ( (m_ui->line_edit_personel_adi->text() EQ "") ) {

        MSG_INFO( tr("Arama Kriteri Olarak Personel Ad veya Soyad Bilgisi Girmelisiniz!.."), m_ui->line_edit_personel_adi );

        return ADAK_FAIL;
    }

    return ADAK_OK;
}

/**************************************************************************************
                   PRS_ISTEN_AYRILANLAR_ARAMA::SEARCH
***************************************************************************************/

void PRS_ISTEN_AYRILANLAR_ARAMA::SEARCH()
{
    SQL_QUERY       sql_query_nufus( DB );
    QString         where_str;
    int             personel_id;
    int             isten_ayrilan_id;
    QString         sicil_no;
    int             current_row;

    current_row = 0;

    if ( where_str.isEmpty() EQ false ) {
        where_str += " AND ";
    }

    where_str += QString::fromUtf8("prs_personeller.personel_id = prs_personel_kisisel_blgler.personel_id ");

    sql_query_nufus.PREPARE_SELECT("prs_personel_kisisel_blgler, prs_personeller ","prs_personeller.personel_id, cari_hesap_id ", where_str);


    if ( sql_query_nufus.SELECT() EQ 0 ) {
        return;

    }

    while ( sql_query_nufus.NEXT() ) {

        personel_id = sql_query_nufus.VALUE("personel_id").toInt();


        int cari_hesap_id =  sql_query_nufus.VALUE( "cari_hesap_id" ).toInt();

        CARI_HESAP_STRUCT hesap_bilgileri;
        CARI_SET_HESAP_STRUCT_DEFAULTS(&hesap_bilgileri);

        CARI_KART_BILGILERINI_OKU( cari_hesap_id, &hesap_bilgileri );


        if ( PRS_PERSONEL_ISTEN_AYRILDI_MI( personel_id ) EQ ADAK_OK ) {

            sicil_no = PRS_GET_PERSONEL_SICIL_NO( personel_id );

            isten_ayrilan_id = PRS_GET_ISTEN_AYRILAN_ID( personel_id );

            current_row = ADD_NEW_ROW();

            m_ui->table_widget_personeller->item( current_row, 0 )->setText( QVariant( isten_ayrilan_id ).toString() );
            m_ui->table_widget_personeller->item( current_row, 1 )->setText( QVariant( personel_id ).toString() );
            m_ui->table_widget_personeller->item( current_row, 2 )->setText( sicil_no );
            m_ui->table_widget_personeller->item( current_row, 3 )->setText( hesap_bilgileri.kimlik_no);
            m_ui->table_widget_personeller->item( current_row, 4 )->setText( hesap_bilgileri.cari_hesap_ismi) ;

        }
    }
}

/**************************************************************************************
                 PRS_ISTEN_AYRILANLAR_ARAMA::SINGLE_LINE_SELECTED
***************************************************************************************/

int PRS_ISTEN_AYRILANLAR_ARAMA::SINGLE_LINE_SELECTED(int selected_row_number)
{
    int     isten_ayrilan_id;

    *m_secilen_id      =  m_ui->table_widget_personeller->item(selected_row_number, 1)->text().toInt();
    isten_ayrilan_id  =   m_ui->table_widget_personeller->item(selected_row_number, 0)->text().toInt();

    OPEN_REPORT_SHOWER( SHOW_ISTEN_AYRILMA_RAPORU( *m_secilen_id, isten_ayrilan_id ),nativeParentWidget() );

    return ADAK_CONTINUE;
}


