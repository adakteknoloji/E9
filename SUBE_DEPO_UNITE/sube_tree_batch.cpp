#include "sube_tree_batch_open.h"
#include "sube_tree_batch_class.h"
#include "ui_sube_tree_batch.h"
#include "print.h"
#include "sube_fisi_open.h"

extern ADAK_SQL *   DB;

/**************************************************************************************
                   OPEN_SUBE_TREE_BATCH
***************************************************************************************/

void OPEN_SUBE_TREE_BATCH ( QWidget * parent )
{
    SUBE_TREE_BATCH * F = new SUBE_TREE_BATCH ( parent );
    F->EXEC( NOT_FULL_SCREEN );
}

/**************************************************************************************
                   SUBE_TREE_BATCH::SUBE_TREE_BATCH
***************************************************************************************/

SUBE_TREE_BATCH::SUBE_TREE_BATCH ( QWidget * parent ) :  BATCH_KERNEL ( parent ), m_ui ( new Ui::SUBE_TREE_BATCH )
{

    m_ui->setupUi      ( this );
    START_BATCH_KERNEL ( this, DB );
}

/**************************************************************************************
                   SUBE_TREE_BATCH::SETUP_FORM
***************************************************************************************/

void SUBE_TREE_BATCH::SETUP_FORM ()
{
    SET_PAGE_TITLE    ( tr ( "ŞUBE - Organizasyon Şeması" ) );
    SET_HELP_PAGE     ( "organizasyon_semasi" );

    SET_AUTO_EXIT_BATCH             ( true );

    REGISTER_BUTTON_WIDGET ( m_ui->widget_batch_buttons );

    SET_FIRST_FOCUS_WIDGET( m_ui->widget_batch_buttons );
    // Buton ismini setliyoruz.
    SET_NAME_OF_RUN_BATCH_BUTTON ( "Kapat" );

    SET_SETTING_NAME  ("ORGANIZASYON_SEMASI");

    REFRESH_SUBE_SCHEMA();
}

/**************************************************************************************
                   SUBE_TREE_BATCH::CHECK_VAR
***************************************************************************************/

int SUBE_TREE_BATCH::CHECK_VAR ( QObject * object )
{
     Q_UNUSED ( object );
     return ADAK_OK;
}

/**************************************************************************************
                   SUBE_TREE_BATCH::CHECK_RUN
***************************************************************************************/

int SUBE_TREE_BATCH::CHECK_RUN ()
{
    return ADAK_OK;
}

/**************************************************************************************
                   SUBE_TREE_BATCH::RUN_BATCH
***************************************************************************************/

void SUBE_TREE_BATCH::RUN_BATCH ()
{
}

/**************************************************************************************
                   SUBE_TREE_BATCH::CHECK_EXIT
***************************************************************************************/

int SUBE_TREE_BATCH::CHECK_EXIT ()
{
    return ADAK_OK;
}

/**************************************************************************************
                   SUBE_TREE_BATCH::REFRESH_SUBE_SCHEMA
****************************************************************************************/

void SUBE_TREE_BATCH::REFRESH_SUBE_SCHEMA()
{
    SQL_QUERY f_query (DB);
    SQL_QUERY s_query (DB);
    SQL_QUERY t_query (DB);

    QString schema = "";

    f_query.PREPARE_SELECT( "sub_subeler","sube_id,sube_kodu,sube_adi" );

    if ( f_query.SELECT() EQ 0 ) {
        m_ui->text_edit_sube_semasi->clear();
        return;
    }

    while ( f_query.NEXT() EQ true ) {

        schema.append(QString("<p style=\"margin:0; -qt-block-indent:0; text-indent:0px;\">"
                      "<span style=\"font-size:20px; font-weight:600;\">Şube: %1 %2</span></p>")
                      .arg(f_query.VALUE(1).toString()).arg(f_query.VALUE(2).toString()));

        s_query.PREPARE_SELECT("sub_depolar","depo_id,depo_kodu,depo_adi","sube_id=:sube_id");
        s_query.SET_VALUE     (":sube_id" , f_query.VALUE(0).toInt());
        if ( s_query.SELECT() NE 0 ) {

            while ( s_query.NEXT() EQ true ) {
                schema.append(QString("<p style=\"margin:0; -qt-block-indent:0; text-indent:30px;\">"
                              "<span style=\"font-size:16px; font-weight:600;\">Depo: %1 %2</span></p>")
                              .arg(s_query.VALUE(1).toString()).arg(s_query.VALUE(2).toString()));

                t_query.PREPARE_SELECT("sub_uniteler","unite_adi","sube_id=:sube_id AND depo_id=:depo_id");
                t_query.SET_VALUE     (":sube_id", f_query.VALUE(0).toInt());
                t_query.SET_VALUE     (":depo_id", s_query.VALUE(0).toInt());
                if ( t_query.SELECT() NE 0 ) {
                    while ( t_query.NEXT() EQ true ) {
                        schema.append(QString("<p style=\"margin:0; -qt-block-indent:0; text-indent:60px;\">"
                                      "<span style=\"font-size:13px; font-weight:600;\">Ünite: %1</span></p>")
                                      .arg(t_query.VALUE(0).toString()));
                    }
                }
            }
        }
        schema.append("<br><br>");
    }

    m_ui->text_edit_sube_semasi->setHtml(schema);
}
