#include <QPushButton>
#include "prs_bordro_inceleme_tree_secimi_class.h"
#include "prs_bordro_inceleme_tree_secimi_open.h"
#include "cari_console_utils.h"
#include "e9_gui_utils.h"
#include "prs_enum.h"
#include "prs_struct.h"
#include "prs_con_utils.h"
#include "e9_console_utils.h"
#include "e9_uretilen_fis_secimi_open.h"

extern ADAK_SQL * DB;


/****************************************************************************/
/*              SHOW_BORDRO_INCELEME_TREE_SECIMI                            */
/****************************************************************************/

int SHOW_BORDRO_INCELEME_TREE_SECIMI (QWidget * parent)
{
    int secilen_id = -1;

    BORDRO_INCELEME_TREE_SECIMI * F = new BORDRO_INCELEME_TREE_SECIMI (parent);
    F->m_secilen_id =&secilen_id ;
    F->EXEC( FULL_SCREEN );

    return secilen_id;
}


/****************************************************************************/
/*           BORDRO_INCELEME_TREE_SECIMI::BORDRO_INCELEME_TREE_SECIMI       */
/****************************************************************************/

BORDRO_INCELEME_TREE_SECIMI::BORDRO_INCELEME_TREE_SECIMI ( QWidget * parent )
{
    SET_HELP_PAGE       ( "personel-bordrosu_bordro-incele" );

    SET_SETTING_NAME    ( "PRS_BRD_INC_TREE_SECIMI" );

    Q_UNUSED(parent);

    SET_WINDOW_SIZE(800,1200);

    SET_COLUMN_HEADERS ( QStringList()<<("")<<tr("")<<("")<<("")<<tr("") );

    SET_PAGE_TITLE( tr ( "PRS - BORDRO İNCELEME" ) );

    INIT_KERNEL ( this, DB );
    
    QFont font("Monospace");
    setFont(font);

    //init kerneldan sonra kullanilmali columni hidelar
    HIDE_COLUMN(1);
    HIDE_COLUMN(2);
    HIDE_COLUMN(3);

    QTreeWidget * tree_widget =  GET_TREE_VIEW();

    tree_widget->setColumnWidth( 0, 10 );
    tree_widget->setColumnWidth( 4, 550 );
    //tree_widget->setColumnWidth( 5, 20 );

}

/****************************************************************************/
/*           BORDRO_INCELEME_TREE_SECIMI::~BORDRO_INCELEME_TREE_SECIMI      */
/****************************************************************************/

BORDRO_INCELEME_TREE_SECIMI::~BORDRO_INCELEME_TREE_SECIMI()
{

}

/****************************************************************************/
/*          BORDRO_INCELEME_TREE_SECIMI::FILL_TREE_TABLE                    */
/****************************************************************************/

void BORDRO_INCELEME_TREE_SECIMI::FILL_TREE_TABLE()
{    
    SQL_QUERY     sql_query          ( DB );
    SQL_QUERY     select_child_query ( DB );
    int           bordro_id;
    int           tree_row_type;
    QStringList   tree_row;
    QString       first_string = " ";
    QString       bordro_donemi;
    QString       bordro_kesim_tarihi;


    //parent itemlarimizi query ile buluyoruz.
    sql_query.PREPARE_SELECT ("prs_bordrolar" ,"bordro_id, bordro_donemi_ay, bordro_donemi_yil, "
                               "bordro_kesim_tarihi, toplam_brut_ucret, toplam_net_ucret, "
                               "toplam_brut_odenek_tutari, toplam_net_odenek_tutari, "
                               "toplam_kesinti_tutari ");

    if ( sql_query.SELECT() NE 0 ) {

        while ( sql_query.NEXT() EQ true ) {

           bordro_id = sql_query.VALUE(0).toInt();

           select_child_query.PREPARE_SELECT ( "prs_bordro_personelleri","bordro_personel_id",
                                               "bordro_id =:bordro_id");

           select_child_query.SET_VALUE      ( ":bordro_id", bordro_id);

           int  bordro_donemi_ay = sql_query.VALUE("bordro_donemi_ay").toInt();

           tree_row_type               = 0;
           bordro_donemi               = E9_GET_AYLAR_STRING( bordro_donemi_ay ) + " / "+
                                         sql_query.VALUE("bordro_donemi_yil").toString() + " BORDROSU  ";

           bordro_kesim_tarihi         = "  (Kesim Tarihi: " +
                                         QDate::fromString( sql_query.VALUE("bordro_kesim_tarihi").toString(),
                                         "yyyy.MM.dd" ).toString("dd MMMM yyyy") +") " ;

           tree_row << first_string <<
                      QVariant( bordro_id ).toString() <<
                      QVariant(0).toString() <<
                      QVariant( tree_row_type ).toString() <<
                      bordro_donemi + bordro_kesim_tarihi +
                      "Top. Brüt Ücr: "      + sql_query.VALUE("toplam_brut_ucret").toString()        + " " +
                      "Top. Net Ücr: "       + sql_query.VALUE("toplam_net_ucret").toString()         + " " +
                      "Top. Ödnk Tutarı: "   + sql_query.VALUE("toplam_brut_odenek_tutari").toString() + " " +
                      "Top. Kesinti Tutarı: "+ sql_query.VALUE("toplam_kesinti_tutari").toString();

	   //ve bu row u tree ye ekleriz.
           ADD_PARENT_ITEM ( tree_row , select_child_query.SELECT());

           //! FIXME
           //! BUTTON EKLENDI TIKLANDIGINDA ENTEGRE OLDUGU FISLERI ACACAK
           //! AMA SUANDA CALISMIYOR.
//           QTreeWidgetItem * tree_item = GET_LAST_ADDED_ITEM ();
//           QTreeWidget * tree_widget =  GET_TREE_VIEW();

//           QPushButton * push_button_urettigi_fisler = new QPushButton( "Ürettiği Fişler", this );

//           push_button_urettigi_fisler->setObjectName( QString::number( bordro_id ) );

//           connect( push_button_urettigi_fisler, SIGNAL(clicked()), this, SLOT( SLOT_URETTIGI_FISLER()));

//           tree_widget->setItemWidget( tree_item, 5, push_button_urettigi_fisler  );

           bordro_donemi.clear();
           tree_row.clear();

       }
    }
}

/****************************************************************************/
/*          BORDRO_INCELEME_TREE_SECIMI::ADD_CHILD_ITEMS                    */
/****************************************************************************/

void BORDRO_INCELEME_TREE_SECIMI::ADD_CHILD_ITEMS(QStringList column_datas)
{
    SQL_QUERY     sql_query           ( DB );
    SQL_QUERY     sql_query_odenek    ( DB );
    SQL_QUERY     sql_query_kesinti   ( DB );
    SQL_QUERY     select_child_query  ( DB );
    int           bordro_id;
    int           bordro_personel_id;
    int           tree_row_type;
    QStringList   tree_row;
    QString       first_string = "---";
    QString       personel_adi_soyadi;
    QString       odenek_ismi;
    QString       odenek_tutari;
    QString       kesinti_ismi;
    QString       kesinti_tutari;



    bordro_id          = column_datas.at( 1 ).toInt();
    bordro_personel_id = column_datas.at( 2 ).toInt();
    tree_row_type      = column_datas.at( 3 ).toInt();

    if ( tree_row_type EQ 0 ){

        sql_query.PREPARE_SELECT ("prs_bordro_personelleri",
                                  "bordro_personel_id, bordro_id, personel_id, "
                                  "calisilan_gun_sayisi, brut_ucret, "
                                  "net_ucret, brd_oncesi_kum_vergi_matrahi, "
                                  "brd_sonrasi_kum_vergi_matrahi, sigorta_primi, "
                                  "issizlik_primi, gelir_vergi_matrahi, gelir_vergisi, "
                                  "asg_gec_indirimi, damga_vergisi, kesintiler_toplami, "
                                  "odenekler_toplami ",
                                  "bordro_id = :bordro_id");

        sql_query.SET_VALUE      ( ":bordro_id", bordro_id);

        if ( sql_query.SELECT() NE 0 ) {

            while ( sql_query.NEXT() ) {

                bordro_personel_id              = sql_query.VALUE("bordro_personel_id").toInt();

                select_child_query.PREPARE_SELECT ( " prs_brd_kesinti_odenekleri ","brd_kesinti_odenek_id ",
                                                     "bordro_personel_id = :bordro_personel_id");

                select_child_query.SET_VALUE      ( ":bordro_personel_id", bordro_personel_id );

                tree_row_type       = 1;

                int cari_hesap_id = PRS_GET_PERSONEL_CARI_HESAP_ID( sql_query.VALUE(2).toInt() );

                QString cari_hesap_kodu, cari_hesap_ismi;
                int result = CARI_GET_HESAP_KODU_ISMI_PARA_BIRIM_ID( cari_hesap_id, &cari_hesap_kodu, &cari_hesap_ismi );

                if ( result NE 0 ) {
                    personel_adi_soyadi = "Personel : " + cari_hesap_ismi.prepend("     ").toUpper();
                }

                tree_row.clear();

                tree_row            << first_string << QVariant( bordro_id ).toString() <<
                                      QVariant( bordro_personel_id ).toString() <<
                                      QVariant( tree_row_type ).toString() <<
                                      personel_adi_soyadi + "  " +
                                      "Brüt Ücr: "        + sql_query.VALUE("brut_ucret").toString()          + " " +
                                      "Sig. Primi: "      + sql_query.VALUE("sigorta_primi").toString()       + " " +
                                      "İşszlk Primi: "    + sql_query.VALUE("issizlik_primi").toString()      + " " +
                                      "Gelir Vergi Mat: " + sql_query.VALUE("gelir_vergi_matrahi").toString() + " " +
                                      "Gelir Vergisi: "   + sql_query.VALUE("gelir_vergisi").toString()       + " " +
                                      "Asg.Geç.İnd: "     + sql_query.VALUE("asg_gec_indirimi").toString()    + " " +
                                      "Gelir Vergisi: "   + sql_query.VALUE("gelir_vergisi").toString()       + " " +
                                      "Damga Vergisi: "   + sql_query.VALUE("damga_vergisi").toString()       + " " +
                                      "Kesntlr Top: "     + sql_query.VALUE("kesintiler_toplami").toString()  + " " +
                                      "Ödnklr Top: "      + sql_query.VALUE("odenekler_toplami").toString()   + " " +
                                      "Net Ödeme: "       + sql_query.VALUE("net_ucret").toString();


                ADD_CHILD_ITEM ( tree_row, select_child_query.SELECT() );
            }
        }
    }

    else if ( tree_row_type EQ 1 ) {

        sql_query_odenek.PREPARE_SELECT ("prs_brd_kesinti_odenekleri",
                                         "kesinti_odenek_ismi, tutari ",
                                         "bordro_personel_id    = :bordro_personel_id "
                                         "AND bordro_id         = :bordro_id AND "
                                         "kesinti_odenek_turu   = :kesinti_odenek_turu");

        sql_query_odenek.SET_VALUE      ( ":bordro_personel_id"  , bordro_personel_id );
        sql_query_odenek.SET_VALUE      ( ":bordro_id"           , bordro_id );
        sql_query_odenek.SET_VALUE      ( ":kesinti_odenek_turu" , ENUM_PRS_ODENEK);

        if ( sql_query_odenek.SELECT() > 0 ) {

            while ( sql_query_odenek.NEXT() ) {

                tree_row_type   = 2;
                first_string    = " ";
                odenek_ismi     = sql_query_odenek.VALUE("kesinti_odenek_ismi").toString().prepend( "----  " );
                odenek_ismi     = odenek_ismi.leftJustified(25, ' ' );
                odenek_tutari   = sql_query_odenek.VALUE("tutari").toString();
                odenek_tutari   = odenek_tutari.rightJustified(10, ' ');

                tree_row.clear();
                // eger odenek yoksa gosterilmiyor.
                if ( QVariant ( odenek_tutari ).toDouble() EQ 0.00 ) {
                    continue;
                }

                tree_row        << first_string <<
                                  QVariant( bordro_id ).toString() <<
                                  QVariant( bordro_personel_id ).toString() <<
                                  QVariant( tree_row_type ).toString() <<
                                  odenek_ismi +
                                  odenek_tutari;
                ADD_CHILD_ITEM ( tree_row, 0 );
           }
        }

        tree_row.clear();

        sql_query_kesinti.PREPARE_SELECT ( "prs_brd_kesinti_odenekleri",
                                           "kesinti_odenek_ismi, tutari ",
                                           "bordro_personel_id = :bordro_personel_id "
                                           "AND bordro_id = :bordro_id AND "
                                           "kesinti_odenek_turu = :kesinti_odenek_turu");


        sql_query_kesinti.SET_VALUE      ( ":bordro_personel_id"  , bordro_personel_id );
        sql_query_kesinti.SET_VALUE      ( ":bordro_id"           , bordro_id );
        sql_query_kesinti.SET_VALUE      ( ":kesinti_odenek_turu" , ENUM_PRS_KESINTI);

        if ( sql_query_kesinti.SELECT() > 0 ) {

            while ( sql_query_kesinti.NEXT() ) {

                tree_row_type   = 2;
                first_string    = " ";
                kesinti_ismi    = sql_query_kesinti.VALUE("kesinti_odenek_ismi").toString().prepend( "----  " );
                kesinti_ismi    = kesinti_ismi.leftJustified(25, ' ' );
                kesinti_tutari  = sql_query_kesinti.VALUE("tutari").toString();
                kesinti_tutari  = kesinti_tutari.rightJustified(10, ' ');

                tree_row.clear();

                tree_row        << first_string <<
                                  QVariant( bordro_id ).toString() <<
                                  QVariant( bordro_personel_id ).toString() <<
                                  QVariant( tree_row_type ).toString() <<
                                  kesinti_ismi +
                                  kesinti_tutari;

                ADD_CHILD_ITEM ( tree_row, 0 );
           }
        }
    }
    else {

        return;
    }
}

/****************************************************************************/
/*         BORDRO_INCELEME_TREE_SECIMI::SINGLE_LINE_SELECTED                */
/****************************************************************************/

int BORDRO_INCELEME_TREE_SECIMI::SINGLE_LINE_SELECTED(QStringList column_datas)
{
    *m_secilen_id = column_datas.at(0).toInt();
        
    return ADAK_OK;
}

/****************************************************************************/
/*         BORDRO_INCELEME_TREE_SECIMI::SLOT_URETTIGI_FISLER                */
/****************************************************************************/

void BORDRO_INCELEME_TREE_SECIMI::SLOT_URETTIGI_FISLER()
{
    QPushButton * push_button_urt_fisler = static_cast< QPushButton * >( sender() );

    E9_URETILEN_FIS_SEC( E9_PROGRAMI, PERSONEL_MODULU, push_button_urt_fisler->objectName().toInt(), nativeParentWidget() );
}



