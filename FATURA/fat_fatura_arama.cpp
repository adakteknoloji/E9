#include <adak_utils.h>
#include <adak_sql.h>
#include "ui_fat_fatura_arama.h"
#include "fat_fatura_arama_class.h"
#include "fat_console_utils.h"
#include "fat_gui_utils.h"
#include "cari_console_utils.h"
#include "adak_std_utils.h"
#include "e9_console_utils.h"
#include "e9_gui_utils.h"
#include "e9_enum.h"
#include "fat_enum.h"
#include "stok_console_utils.h"
#include "cari_kart_arama_open.h"

#define     FIS_ID_COLUMN           0
#define     FATURA_TARIHI_COLUMN    1
#define     FIS_NO_COLUMN           2
#define     CARI_HESAP_COLUMN       3
#define     FATURA_SERI_NO_COLUMN   4
#define     FATURA_TUTARI_COLUMN    5


extern ADAK_SQL * DB;

static int g_secilen_id;

/**************************************************/
/*             OPEN_FATURA_ARAMA                 */
/**************************************************/

int OPEN_FATURA_ARAMA ( int p_fatura_alis_satis_turu , int p_iade_faturasi_mi, QWidget * p_parent )
{
    g_secilen_id    = -1;
    FAT_FATURA_ARAMA * F = new FAT_FATURA_ARAMA ( p_fatura_alis_satis_turu, p_iade_faturasi_mi, p_parent );
    F->EXEC( NOT_FULL_SCREEN );
    return g_secilen_id;
}

/****************************************************************/
/*           FAT_FATURA_ARAMA::FAT_FATURA_ARAMA                 */
/****************************************************************/

FAT_FATURA_ARAMA::FAT_FATURA_ARAMA ( int p_fatura_alis_satis_turu, int p_iade_faturasi_mi, QWidget * p_parent ) : ARAMA_KERNEL ( p_parent ), m_ui ( new Ui::FAT_FATURA_ARAMA )
{
    m_fatura_alis_satis_turu = p_fatura_alis_satis_turu;
    m_iade_faturasi_mi       = p_iade_faturasi_mi;

    m_ui->setupUi       (this );
    START_ARAMA_KERNEL  (this, DB );
}

/******************************************************************/
/*               FAT_FATURA_ARAMA::SETUP_FORM                      */
/******************************************************************/

void FAT_FATURA_ARAMA::SETUP_FORM()
{
    SET_HELP_PAGE  ("fatura_arama.html");
    SET_PAGE_TITLE ( tr ( "FATURA ARAMA" ) );

    //m_ui->frame_entegre_filtresi->setVisible( false );
    m_ui->groupBox_entegre_filtresi->setVisible( false );

    m_ui->frame_fatura_turu->setEnabled( false );
    m_ui->frame_acik_kapali_fatura->setEnabled( false );

    FAT_FILL_COMBOBOX_FATURA_TURU( m_ui->comboBox_fatura_turu, m_fatura_alis_satis_turu, -1 , m_iade_faturasi_mi );

    m_ui->comboBox_fatura_turu->setCurrentIndex( -1 );

    if ( E9_KULLANICI_ENT_BILGILERINI_DEGISTIREBILIR_MI() EQ 1 ) {

        //m_ui->frame_entegre_filtresi->setVisible( true );
        m_ui->groupBox_entegre_filtresi->setVisible( false );
    }

    FAT_FILL_ACIK_KAPALI_COMBO_BOX( m_ui->comboBox_acik_kapali_fatura );

    FIND_AND_SET_COMBOBOX_TEXT( m_ui->comboBox_acik_kapali_fatura, FAT_GET_ACIK_KAPALI_FATURA_STRING( ENUM_TUM_FATURALAR ));

    m_ui->commaEdit_min_tutar->SET_PRECISION( 2 );
    m_ui->commaEdit_min_tutar->SET_RETURN_EMPTY( false );
    m_ui->commaEdit_max_tutar->SET_PRECISION( 2 );
    m_ui->commaEdit_max_tutar->SET_RETURN_EMPTY( false );

    m_ui->checkBox_tarih_araligi->setChecked        ( true  );
    m_ui->groupBox_tarih_araligi->setEnabled        ( true  );
    m_ui->groupBox_aciklama_icerigi->setEnabled     ( false );
    m_ui->groupBox_cari_hesap_kodu->setEnabled      ( false );
    m_ui->groupBox_urun->setEnabled                 ( false );
    m_ui->groupBox_tutar_araligi->setEnabled        ( false );

    m_ui->adakDate_ilk_tarih->SET_DATE( QDate::currentDate().addDays( -7 ) );
    m_ui->adakDate_son_tarih->SET_DATE( QDate::currentDate() );

    REGISTER_TABLE_WIDGET         ( m_ui->tableWidget );
    REGISTER_ARAMA_BUTTONS_WIDGET ( m_ui->widget_batch_buttons );

    SET_HEADERS ( QStringList() <<tr("Fatura Id")<<tr("Fatura Tarihi")<<tr("Fiş No")<<tr("Cari Hesap") << tr("Fatura Seri / No")<< tr("Fatura Tutarı"));

    SET_SORTING ( false );

    SET_FIRST_FOCUS_WIDGET ( m_ui->adakDate_ilk_tarih );

    m_ui->tableWidget->setColumnWidth ( FATURA_TARIHI_COLUMN,  120 );
    m_ui->tableWidget->setColumnWidth ( FIS_NO_COLUMN,         60  );
    m_ui->tableWidget->setColumnWidth ( CARI_HESAP_COLUMN,     200 );
    m_ui->tableWidget->setColumnWidth ( FATURA_SERI_NO_COLUMN, 120 );
    m_ui->tableWidget->setColumnWidth ( FATURA_TUTARI_COLUMN,  120 );

    m_ui->tableWidget->hideColumn ( FIS_ID_COLUMN );

    SET_ENTER_KEY_FOR_RUN_BATCH ( true );
    SET_FIND_BUTTON_NAME        ( tr ( "Fatura Bul" ) );
    SET_SETTING_NAME            ( "FATURA_ARAMA" );
}

/******************************************************************/
/*               FAT_FATURA_ARAMA::CHECK_VAR                      */
/******************************************************************/

int FAT_FATURA_ARAMA::CHECK_VAR ( QObject * p_object )
{
    if ( ( p_object EQ m_ui->adakDate_ilk_tarih ) OR ( p_object EQ m_ui->adakDate_son_tarih) ) {
        if (   m_ui->adakDate_ilk_tarih->QDATE() > m_ui->adakDate_son_tarih->QDATE() ) {
            MSG_WARNING( tr("İlk tarih son tarihten büyük olamaz!.."), m_ui->adakDate_ilk_tarih );

            return ADAK_FAIL;
        }
    }
    else if ( p_object EQ m_ui->checkBox_tarih_araligi ) {
        if ( m_ui->checkBox_tarih_araligi->isChecked() EQ true ) {
            //m_ui->frame_tarih_araligi->setEnabled(true);
            m_ui->groupBox_tarih_araligi->setEnabled( true );
            SET_FOCUS( m_ui->adakDate_ilk_tarih );
        }
        else {
            //m_ui->frame_tarih_araligi->setEnabled(false);
            m_ui->groupBox_tarih_araligi->setEnabled(false);
        }
    }
    else if ( p_object EQ m_ui->checkBox_cari_hesap_kodu ) {
        if ( m_ui->checkBox_cari_hesap_kodu->isChecked() EQ true ) {
            m_ui->groupBox_cari_hesap_kodu->setEnabled(true);
            SET_FOCUS( m_ui->searchEdit_cari_hesap_kodu );
        }
        else {
            m_ui->groupBox_cari_hesap_kodu->setEnabled(false);
            m_ui->lineEdit_cari_hesap_ismi->clear();
        }
    }
    else if ( p_object EQ m_ui->checkBox_aciklama_icerigi ) {
        if ( m_ui->checkBox_aciklama_icerigi->isChecked() EQ true ) {
            //m_ui->frame_aciklama_icerigi->setEnabled(true);
            m_ui->groupBox_aciklama_icerigi->setEnabled( true );
            SET_FOCUS( m_ui->lineEdit_aciklama_icerigi );
        }
        else {
            //m_ui->frame_aciklama_icerigi->setEnabled(false);
            m_ui->groupBox_aciklama_icerigi->setEnabled( false );
            m_ui->lineEdit_aciklama_icerigi->clear();
        }
    }
    else if ( p_object EQ m_ui->checkBox_urun ) {
        if ( m_ui->checkBox_urun->isChecked() EQ true ) {
            //m_ui->frame_urun->setEnabled(true);
            m_ui->groupBox_urun->setEnabled( true );
            SET_FOCUS( m_ui->lineEdit_urun_kodu_ismi_icerigi );
        }
        else {
            //m_ui->frame_urun->setEnabled(false);
            m_ui->groupBox_urun->setEnabled( false );
            m_ui->lineEdit_urun_kodu_ismi_icerigi->clear();
        }
    }
    else if ( p_object EQ m_ui->checkBox_tutar_araligi ) {
        if ( m_ui->checkBox_tutar_araligi->isChecked() EQ true ) {
            //m_ui->frame_tutar_araligi->setEnabled(true);
            m_ui->groupBox_tutar_araligi->setEnabled( true );
            m_ui->commaEdit_min_tutar->SET_DOUBLE( 0.00 );
            m_ui->commaEdit_max_tutar->SET_DOUBLE( 10000.00 );
            SET_FOCUS( m_ui->commaEdit_min_tutar );
        }
        else {
            m_ui->commaEdit_min_tutar->clear();
            m_ui->commaEdit_max_tutar->clear();
            //m_ui->frame_tutar_araligi->setEnabled(false);
            m_ui->groupBox_tutar_araligi->setEnabled( false );
        }
    }
    else if ( p_object EQ m_ui->checkBox_fatura_turu ) {
        if ( m_ui->checkBox_fatura_turu->isChecked() EQ true ) {
            m_ui->comboBox_fatura_turu->setCurrentIndex( 0 );

            m_ui->frame_fatura_turu->setEnabled( true );
        }
        else {
           m_ui->comboBox_fatura_turu->setCurrentIndex( -1 );
           m_ui->frame_fatura_turu->setEnabled( false );
        }
    }
    else if ( p_object EQ m_ui->checkBox_acik_kapali_fatura ) {
        if ( m_ui->checkBox_acik_kapali_fatura->isChecked() EQ true ) {

            m_ui->frame_acik_kapali_fatura->setEnabled( true );
        }
        else {
            m_ui->frame_acik_kapali_fatura->setEnabled( false );
        }
    }
    else if ( p_object EQ m_ui->searchEdit_cari_hesap_kodu ) {
        QString cari_hesap_ismi;

        int sonuc = CARI_GET_HESAP_KODU_ISMI_PARA_BIRIM_ID( m_ui->searchEdit_cari_hesap_kodu->GET_TEXT(), &m_cari_hesap_id, &cari_hesap_ismi );

        if ( sonuc EQ 0 ) {
            MSG_WARNING( "Cari Hesap bulunamadı.", m_ui->searchEdit_cari_hesap_kodu );
            m_ui->lineEdit_cari_hesap_ismi->clear();
            return ADAK_FAIL;
        }

        m_ui->lineEdit_cari_hesap_ismi->setText( cari_hesap_ismi );
    }
    return ADAK_OK;
}

/*******************************************************************/
/*                 FAT_FATURA_ARAMA::CHECK_RUN                   */
/*******************************************************************/

int FAT_FATURA_ARAMA::CHECK_RUN ()
{
    if ( ( m_ui->checkBox_tarih_araligi->isChecked() EQ false ) AND
         ( m_ui->checkBox_cari_hesap_kodu->isChecked() EQ false )    AND
         ( m_ui->checkBox_aciklama_icerigi->isChecked() EQ false ) AND
         ( m_ui->checkBox_urun->isChecked() EQ false ) ) {
        MSG_WARNING(  tr("Arama için en az bir kriter seçmelisiniz."), m_ui->searchEdit_cari_hesap_kodu );

        return ADAK_FAIL;
    }

    if ( m_ui->checkBox_cari_hesap_kodu->isChecked() EQ true ) {
        if ( m_ui->searchEdit_cari_hesap_kodu->GET_TEXT().isEmpty() EQ true ) {
           MSG_WARNING(  tr("Cari hesap içeriği boş bırakılamaz."), m_ui->searchEdit_cari_hesap_kodu );

            return ADAK_FAIL;
        }
    }
    if ( m_ui->checkBox_aciklama_icerigi->isChecked() EQ true ) {
        if ( m_ui->lineEdit_aciklama_icerigi->text().isEmpty() EQ true ) {
           MSG_WARNING(  tr("Açıklama içeriği boş bırakılamaz."), m_ui->lineEdit_aciklama_icerigi );

            return ADAK_FAIL;
        }
    }
    if ( m_ui->checkBox_urun->isChecked() EQ true ) {
        if ( m_ui->lineEdit_urun_kodu_ismi_icerigi->text().isEmpty() EQ true ) {
           MSG_WARNING(  tr("Ürün ismi / kodu içeriği boş bırakılamaz."), m_ui->lineEdit_urun_kodu_ismi_icerigi );

            return ADAK_FAIL;
        }
    }

    return ADAK_OK;
}

/********************************************************************/
/*                     FAT_FATURA_ARAMA::SEARCH                     */
/********************************************************************/

void FAT_FATURA_ARAMA::SEARCH()
{
    SQL_QUERY sql_query ( DB );

    QString ilk_tarih     = m_ui->adakDate_ilk_tarih->DATE();
    QString son_tarih     = m_ui->adakDate_son_tarih->DATE();

    QString column_string = "fis_id, fis_no, fatura_tarihi, "
                            "fat_faturalar.cari_hesap_id, fat_faturalar.cari_hesap_ismi, "
                            "yazdirildi_mi,fatura_turu, belge_seri, belge_numarasi, fatura_tutari ";

    QString table_string   = "fat_faturalar ";

    QString urun_kodu_ismi_icerigi       = m_ui->lineEdit_urun_kodu_ismi_icerigi->text();

    sql_query.PREPARE_SELECT ( table_string , column_string );

    sql_query.AND_EKLE  ( "fatura_alis_satis_turu = :fatura_alis_satis_turu" );
    sql_query.SET_VALUE ( ":fatura_alis_satis_turu" , m_fatura_alis_satis_turu );

    if ( m_ui->checkBox_fatura_turu->isChecked() EQ true ) {
        QString fatura_turu = m_ui->comboBox_fatura_turu->currentText();
        switch ( m_fatura_alis_satis_turu ) {
            case ENUM_ALIS_IRSALIYESI_FATURASI :
            default :
                fatura_turu.append( tr(" ( ALIŞ )")  );
                break;
            case ENUM_SATIS_IRSALIYESI_FATURASI :
                fatura_turu.append ( tr(" ( SATIŞ )") );
                break;
        }

        sql_query.AND_EKLE  ( "fatura_turu = :fatura_turu" );
        sql_query.SET_VALUE ( ":fatura_turu", FAT_GET_FATURA_TURLERI_ENUM( fatura_turu  ) );
    }

    if ( m_ui->checkBox_cari_hesap_kodu->isChecked() EQ true ) {
        sql_query.AND_EKLE  ( "cari_hesap_id = :cari_hesap_id" );
        sql_query.SET_VALUE ( ":cari_hesap_id", m_cari_hesap_id );
    }

    if ( m_ui->checkBox_tarih_araligi->isChecked() EQ true ) {
        sql_query.AND_EKLE( "fatura_tarihi BETWEEN :ilk_tarih AND :son_tarih");
        sql_query.SET_VALUE(":ilk_tarih" , ilk_tarih );
        sql_query.SET_VALUE(":son_tarih" , son_tarih );
    }
    if ( m_ui->checkBox_aciklama_icerigi->isChecked() EQ true ) {
        sql_query.AND_EKLE( "aciklama LIKE :aciklama" );
        sql_query.SET_LIKE(":aciklama" , m_ui->lineEdit_aciklama_icerigi->text());
    }

    if ( m_ui->checkBox_tutar_araligi->isChecked() EQ true ) {
        sql_query.AND_EKLE( "fatura_tutari BETWEEN :min_tutar AND :max_tutar" );
        sql_query.SET_VALUE(":min_tutar" , m_ui->commaEdit_min_tutar->GET_DOUBLE() );
        sql_query.SET_VALUE(":max_tutar" , m_ui->commaEdit_max_tutar->GET_DOUBLE() );
    }
    if ( FAT_GET_ACIK_KAPALI_FATURA_ENUM( m_ui->comboBox_acik_kapali_fatura->currentText() ) EQ ENUM_ACIK_FATURA ) {
        sql_query.AND_EKLE  ( "acik_kapali_fatura = :acik_kapali_fatura AND iptal_mi = :iptal_mi" );
        sql_query.SET_VALUE ( ":acik_kapali_fatura", ENUM_ACIK_FATURA );
        sql_query.SET_VALUE ( ":iptal_mi"          , 0 );
    }
    else if ( FAT_GET_ACIK_KAPALI_FATURA_ENUM( m_ui->comboBox_acik_kapali_fatura->currentText() ) EQ ENUM_KAPALI_FATURA ) {
        sql_query.AND_EKLE  ( "acik_kapali_fatura = :acik_kapali_fatura AND iptal_mi = :iptal_mi" );
        sql_query.SET_VALUE ( ":acik_kapali_fatura", ENUM_KAPALI_FATURA );
        sql_query.SET_VALUE ( ":iptal_mi"          , 0 );
    }
    else if ( FAT_GET_ACIK_KAPALI_FATURA_ENUM( m_ui->comboBox_acik_kapali_fatura->currentText() ) EQ ENUM_IPTAL_EDILEN_FATURALAR ) {
        sql_query.AND_EKLE  ( "iptal_mi = :iptal_mi" );
        sql_query.SET_VALUE ( ":iptal_mi", 1 );
    }

    sql_query.AND_EKLE   ( " iade_faturasi_mi = :iade_faturasi_mi ");
    sql_query.SET_VALUE  ( ":iade_faturasi_mi", m_iade_faturasi_mi );


    if ( sql_query.SELECT("fat_faturalar.fatura_tarihi ASC , fat_faturalar.fis_no ASC") EQ 0 ) {
        return;
    }

    int cari_hesap_id;
    QString cari_hesap_kodu, cari_hesap_ismi;

    M_FATURA_BILGILERI = new FATURA_BILGILERI[sql_query.NUM_OF_ROWS()];

    int struct_index = 0;

    while ( sql_query.NEXT() EQ true ) {

        int fis_id = sql_query.VALUE( 0  ).toInt();

        cari_hesap_id = FAT_GET_FATURA_CARI_HESAP_ID( fis_id );

        CARI_GET_HESAP_KODU_ISMI_PARA_BIRIM_ID( cari_hesap_id, &cari_hesap_kodu, &cari_hesap_ismi );

        M_FATURA_BILGILERI[ struct_index ].fis_id                 =   fis_id;
        M_FATURA_BILGILERI[ struct_index ].fis_no                 =   sql_query.VALUE( 1  ).toInt();
        M_FATURA_BILGILERI[ struct_index ].fatura_tarihi          =   sql_query.VALUE( 2  ).toString();
        M_FATURA_BILGILERI[ struct_index ].cari_hesap_id          =   sql_query.VALUE( 3  ).toInt();
        M_FATURA_BILGILERI[ struct_index ].fat_cari_hsp_ismi      =   sql_query.VALUE( 4  ).toString();
        M_FATURA_BILGILERI[ struct_index ].car_cari_hesap_kodu    =   cari_hesap_kodu;
        M_FATURA_BILGILERI[ struct_index ].car_cari_hesap_ismi    =   cari_hesap_ismi;
        M_FATURA_BILGILERI[ struct_index ].yazdirildi_mi          =   sql_query.VALUE( 5  ).toInt();
        M_FATURA_BILGILERI[ struct_index ].gecerli_fatura         =   1;
        M_FATURA_BILGILERI[ struct_index ].belge_seri             =   sql_query.VALUE( 7  ).toString();
        M_FATURA_BILGILERI[ struct_index ].belge_numarasi         =   sql_query.VALUE( 8  ).toString();
        M_FATURA_BILGILERI[ struct_index ].fatura_tutari          =   sql_query.VALUE( 9  ).toDouble();

        struct_index++;
    }

    if ( m_ui->checkBox_urun->isChecked() EQ true ) {
        bool urun_kodu_ismi_eslesti = false;
        for ( int i = 0; i < struct_index; i++ ) {
            urun_kodu_ismi_eslesti = false;
            if ( M_FATURA_BILGILERI[i].gecerli_fatura EQ 0 ) {
                continue;
            }
            sql_query.PREPARE_SELECT("fat_fatura_satirlari ","urun_adi",
                                     "fis_id = :fis_id AND urun_id = :urun_id");
            sql_query.SET_VALUE ( ":fis_id"  , M_FATURA_BILGILERI[i].fis_id );
            sql_query.SET_VALUE ( ":urun_id" , 0);

            if ( sql_query.SELECT() > 0 ) {

                while ( sql_query.NEXT() EQ true ) {
                    if ( sql_query.VALUE(0).toString().contains ( urun_kodu_ismi_icerigi, Qt::CaseInsensitive ) EQ true ) {
                        urun_kodu_ismi_eslesti = true;
                        break;
                    }
                }
            }
            if ( urun_kodu_ismi_eslesti EQ true ) {
                continue;
            }

            sql_query.PREPARE_SELECT("fat_fatura_satirlari" , "urun_id" , "fis_id = :fis_id "
                                     "AND urun_id > :urun_id");
            sql_query.SET_VALUE(":fis_id" , M_FATURA_BILGILERI[i].fis_id );
            sql_query.SET_VALUE(":urun_id", 0);

            if ( sql_query.SELECT() EQ 0 ) {
                continue;
            }

            while ( sql_query.NEXT() EQ true ) {

                int urun_id = sql_query.VALUE(0).toInt();

                QString urun_kodu;
                QString urun_adi;

                STK_GET_URUN_KODU_ADI(urun_id , &urun_kodu , &urun_adi);

                if ( urun_kodu.contains ( urun_kodu_ismi_icerigi, Qt::CaseInsensitive ) EQ true ) {
                    urun_kodu_ismi_eslesti = true;
                    break;
                }
                if ( urun_adi.contains ( urun_kodu_ismi_icerigi, Qt::CaseInsensitive ) EQ true ) {
                    urun_kodu_ismi_eslesti = true;
                    break;
                }
            }
            if ( urun_kodu_ismi_eslesti EQ false ) {
                M_FATURA_BILGILERI[i].gecerli_fatura = 0;
            }
        }
    }

    int current_row = 0;

    for ( int i = 0; i < struct_index; i++ ) {

        if ( M_FATURA_BILGILERI[i].gecerli_fatura EQ 0 ) {
            continue;
        }

        current_row = ADD_NEW_ROW();

        m_ui->tableWidget->item ( current_row, FIS_ID_COLUMN        )->setText ( QVariant( M_FATURA_BILGILERI[i].fis_id ).toString() );
        m_ui->tableWidget->item ( current_row, FIS_NO_COLUMN        )->setText ( QVariant( M_FATURA_BILGILERI[i].fis_no).toString());
        m_ui->tableWidget->item ( current_row, FATURA_TARIHI_COLUMN )->setText ( QDate::fromString( M_FATURA_BILGILERI[i].fatura_tarihi, "yyyy.MM.dd").toString( " dd MMMM yyyy dddd") );
        m_ui->tableWidget->item ( current_row, CARI_HESAP_COLUMN    )->setText ( M_FATURA_BILGILERI[i].car_cari_hesap_kodu
                                                              + " " +
                                                              M_FATURA_BILGILERI[i].car_cari_hesap_ismi );
        m_ui->tableWidget->item( current_row, FATURA_SERI_NO_COLUMN )->setText ( M_FATURA_BILGILERI[i].belge_seri + " " + M_FATURA_BILGILERI[ i ].belge_numarasi );
        m_ui->tableWidget->item( current_row, FATURA_TUTARI_COLUMN  )->setTextAlignment( Qt::AlignRight + Qt::AlignVCenter );
        m_ui->tableWidget->item( current_row, FATURA_TUTARI_COLUMN  )->setText ( VIRGUL_EKLE( QVariant(M_FATURA_BILGILERI[ i ].fatura_tutari).toString(),2)   );

    }
}

/********************************************************************/
/*           FAT_FATURA_ARAMA::SINGLE_LINE_SELECTED                 */
/********************************************************************/

int FAT_FATURA_ARAMA::SINGLE_LINE_SELECTED ( int p_selected_row_number )
{
    g_secilen_id = m_ui->tableWidget->item ( p_selected_row_number, FIS_ID_COLUMN )->text().toInt();
    return ADAK_EXIT;
}

/********************************************************************/
/*           FAT_FATURA_ARAMA::SEARCH_EDIT_CLICKED                 */
/********************************************************************/

void FAT_FATURA_ARAMA::SEARCH_EDIT_CLICKED( QWidget *widget, QLineEdit *line_edit )
{
    Q_UNUSED ( line_edit );

    if ( widget EQ m_ui->searchEdit_cari_hesap_kodu ) {
        m_cari_hesap_id = OPEN_CARI_KART_ARAMA ( "", SAHIS_ARTI_FIRMA , this, 0, E9_PROGRAMI, CARI_MODULU );// SAHIS + FIRMA
        QString cari_hesap_kodu;
        CARI_GET_HESAP_KODU_ISMI_PARA_BIRIM_ID( m_cari_hesap_id, &cari_hesap_kodu );
        line_edit->setText( cari_hesap_kodu );
    }
}
