#include <QMessageBox>
#include "adak_std_utils.h"
#include "bnk_hareket_fisi_arama_class.h"
#include "bnk_hareket_fisi_arama_open.h"
#include "ui_bnk_hareket_fisi_arama.h"
#include "e9_console_utils.h"
#include "e9_enum.h"
#include "e9_gui_utils.h"
#include "banka_enum.h"
#include "bnk_gui_utils.h"
#include "bnk_console_utils.h"
#include "bnk_hareket_fisi_open.h"

#define     FIS_ID_COLUMN           0
#define     FIS_TARIHI_COLUMN       1
#define     FIS_NO_COLUMN           2
#define     ACIKLAMA_COLUMN         3
#define     MAKRO_TUTARI_COLUMN     4
#define     MAKRO_TUTARI_COLUMN_2   5
#define     MAKRO_TUTARI_COLUMN_3   6

extern ADAK_SQL * DB;

/**************************************************/
/*               F_BNK_HAREKET_FISI_ARAMA           */
/**************************************************/

int OPEN_BNK_HAREKET_FISI_ARAMA( int banka_modulu_fis_turu , int fis_kaynagi , BANKA_FIS_TURU p_fis_turu, QWidget * parent)
{
    int secilen_id    = -1;

    BNK_HAREKET_FISI_ARAMA * F = new BNK_HAREKET_FISI_ARAMA( banka_modulu_fis_turu, fis_kaynagi, p_fis_turu,  parent);
    F->m_secilen_id = &secilen_id;
    F->EXEC( NOT_FULL_SCREEN );

    return secilen_id;
}

/**********************************************************/
/*       BNK_HAREKET_FISI_ARAMA::BNK_HAREKET_FISI_ARAMA       */
/**********************************************************/

BNK_HAREKET_FISI_ARAMA::BNK_HAREKET_FISI_ARAMA(int banka_modulu_fis_turu, int fis_kaynagi, BANKA_FIS_TURU p_fis_turu, QWidget *parent) : ARAMA_KERNEL (parent), m_ui(new Ui::BNK_HAREKET_FISI_ARAMA)
{
    m_base_fisi_turu = banka_modulu_fis_turu;
    m_modul_id       = fis_kaynagi;
    m_fis_turu       = p_fis_turu;

    m_ui->setupUi      (this);
    START_ARAMA_KERNEL (this,DB);
}

/*****************************************************
         BNK_HAREKET_FISI_ARAMA::SETUP_FORM
*****************************************************/

void BNK_HAREKET_FISI_ARAMA::SETUP_FORM()
{
    SET_HELP_PAGE  ( "banka-islemleri" );
    // Ekran Entegras
    if ( m_fis_turu EQ ENUM_BANKA_ENT_FISI ) {
        SET_PAGE_TITLE( tr ( "BNK - BANKA ENTEGRASYON FİŞLERİ" ) );
        SET_HELP_PAGE( "Banka-islemleri_banka-entegrasyon-fisleri");
        SET_FIRST_FOCUS_WIDGET( m_ui->comboBox_ent_fis_turu );
        m_ui->checkbox_banka_ismi->setVisible( false );
        m_ui->frame_banka->setVisible(  false );

        m_ui->checkBox_ent_fis_turu->setChecked( true );
    }
    else {
        SET_PAGE_TITLE( tr ( "BNK - BANKA FİŞ ARAMA" ) );

        SET_FIRST_FOCUS_WIDGET( m_ui->adakDate_first_date )
                ;
        m_ui->checkBox_ent_fis_turu->setVisible(  false );
        m_ui->frame_ent_fis_turu->setVisible( false );
    }

    REGISTER_TABLE_WIDGET         (m_ui->tableWidget);
    REGISTER_ARAMA_BUTTONS_WIDGET (m_ui->widget_batch_buttons);

    m_ui->frame_entegre_filtresi->setVisible( false );
    m_ui->checkBox->setVisible( false );
    m_ui->label_fis_filtresi->setVisible(false);

    if ( E9_KULLANICI_SIRKET_DEGISTIRE_BILIR_MI() EQ EVET ) {
        m_ui->comboBox_fis_filtresi->setVisible( true );
        m_ui->label_fis_filtresi->setVisible(true);

        m_ui->frame_entegre_filtresi->setVisible( true );
        m_ui->checkBox->setVisible( true );
    }

    QStringList headers;

    if (  m_base_fisi_turu EQ ENUM_BANKA_ACILIS_FISI ) {
         headers <<tr("Fiş Id")<<tr("Fiş Tarihi")<<tr("Fiş No")<<tr("Açıklama")<<tr("Banka \n Hesabı Tutarı") << tr("Kredi Kartı \n Hesabı Tutarı") << tr("Pos \n Hesabı Tutarı");
    }
    else {
        headers <<tr("Fiş Id")<<tr("Fiş Tarihi")<<tr("Fiş No")<<tr("Açıklama")<<tr("Toplam \n Hesaba Giren")<<tr("Toplam \n Hesaptan Çıkan");
    }

    SET_HEADERS(headers);

    SET_SORTING(false);

    m_ui->adakDate_first_date->SET_DATE(MALI_YIL_FIRST_DATE());

    m_ui->frame_banka->setEnabled(false);
    m_ui->frame_tarih->setEnabled(false);

    SQL_QUERY select_query(DB);
    select_query.PREPARE_SELECT("bnk_hesaplar","banka_ismi");
    if(select_query.SELECT("banka_ismi") NE 0 ) {
        while (select_query.NEXT()) {
            m_ui->combobox_banka_ismi->addItem(select_query.VALUE(0).toString());
        }
        m_ui->combobox_banka_ismi->setCurrentIndex(-1);
    }

    m_ui->tableWidget->hideColumn ( FIS_ID_COLUMN );

    m_ui->tableWidget->setColumnWidth ( FIS_TARIHI_COLUMN,      240 );
    m_ui->tableWidget->setColumnWidth ( FIS_NO_COLUMN,          70  );

    if( m_base_fisi_turu EQ ENUM_BANKA_VIRMAN_FISI ) {
            m_ui->tableWidget->setColumnWidth ( FIS_TARIHI_COLUMN,      200 );
            m_ui->tableWidget->setColumnWidth ( ACIKLAMA_COLUMN,        400 );
            SET_SETTING_NAME("BANKA_VIRMAN_FISI");

    }
    else if ( m_base_fisi_turu EQ ENUM_BANKA_ACILIS_FISI ) {
        m_ui->tableWidget->setColumnWidth ( FIS_TARIHI_COLUMN,      200 );
        m_ui->tableWidget->setColumnWidth ( ACIKLAMA_COLUMN,        250 );
        m_ui->tableWidget->setColumnWidth ( MAKRO_TUTARI_COLUMN,    120 );
        m_ui->tableWidget->setColumnWidth ( MAKRO_TUTARI_COLUMN_2,  120 );
        m_ui->tableWidget->setColumnWidth ( MAKRO_TUTARI_COLUMN_2,  120 );
        SET_SETTING_NAME("BANKA_ACILIS_FISI");
    }
    else {
        m_ui->tableWidget->setColumnWidth ( FIS_TARIHI_COLUMN,      200 );
        m_ui->tableWidget->setColumnWidth ( ACIKLAMA_COLUMN,        250 );
        m_ui->tableWidget->setColumnWidth ( MAKRO_TUTARI_COLUMN,    120 );
        m_ui->tableWidget->setColumnWidth ( MAKRO_TUTARI_COLUMN_2,  120 );
        SET_SETTING_NAME("BANKA_HAREKET_FISI");
    }

    SET_ENTER_KEY_FOR_RUN_BATCH ( true );
    SET_FIND_BUTTON_NAME        ( tr ( "Fişleri Bul" ) );
}

/*****************************************************/
/*         BNK_HAREKET_FISI_ARAMA::CHECK_VAR           */
/*****************************************************/

int BNK_HAREKET_FISI_ARAMA::CHECK_VAR(QObject * object)
{

    if ( object EQ m_ui->checkbox_tarih_araligi ) {

        if ( m_ui->checkbox_tarih_araligi->isChecked() EQ true ) {
            m_ui->frame_tarih->setEnabled(true);
        }
        else {
            m_ui->frame_tarih->setDisabled(true);
            m_ui->adakDate_first_date->SET_DATE(MALI_YIL_FIRST_DATE());
        }
    }
    else if ( object EQ m_ui->checkbox_banka_ismi ) {
        if ( m_ui->checkbox_banka_ismi->isChecked() EQ true ) {
            m_ui->frame_banka->setEnabled(true);
        }
        else {
            m_ui->frame_banka->setEnabled(false);
            m_ui->combobox_banka_ismi->setCurrentIndex(-1);
        }
    }
    else if ( object EQ m_ui->comboBox_ent_fis_turu ) {
        m_modul_id = BNK_GET_ENT_FIS_MODUL_ID( m_ui->comboBox_ent_fis_turu->currentText() );
    }
    else if ( object EQ m_ui->checkBox_ent_fis_turu ) {
        if( m_ui->checkBox_ent_fis_turu->isChecked() EQ true ) {
            m_ui->frame_ent_fis_turu->setEnabled( true );
        }
        else {
            m_ui->frame_ent_fis_turu->setEnabled( false );
        }
    }
    return ADAK_OK;
}

/*****************************************************/
/*          BNK_HAREKET_FISI_ARAMA::CHECK_RUN        */
/*****************************************************/

int BNK_HAREKET_FISI_ARAMA::CHECK_RUN()
{
    if (m_ui->checkbox_banka_ismi->isChecked() EQ true) {
        if (m_ui->combobox_banka_ismi->currentIndex() EQ -1 ) {
            MSG_WARNING( tr("Banka ismini seçmelisiniz!.."), m_ui->combobox_banka_ismi );

            return ADAK_FAIL;
        }
    }
    return ADAK_OK;
}

/*****************************************************/
//             BNK_HAREKET_FISI_ARAMA::SEARCH
/*****************************************************/

void BNK_HAREKET_FISI_ARAMA::SEARCH()
{
    QStringList banka_hesaplari_id_list;

    bool fisler_banka_ismine_gore_aranacak = false;

    QString column_names = "";
    QString table_names  = "";
    QString where_str    = "";

    if ( m_base_fisi_turu EQ ENUM_BANKA_ACILIS_FISI  ) {
        column_names = "fis_id,fis_no,fis_tarihi,aciklama,hesap_no_id";
        table_names   = "bnk_hareket_fisler";
    }
    else if ( m_base_fisi_turu EQ ENUM_BANKA_PARA_YAT_CEKME_FISI ) {
        column_names = "fis_id,fis_no,fis_tarihi,aciklama,hesap_no_id, "
                       "toplam_hesaba_giren, toplam_hesaptan_cikan";
        table_names   = "bnk_hareket_fisler";
    }
    else if( m_base_fisi_turu EQ ENUM_BANKA_VIRMAN_FISI ) {
        column_names = "fis_id,fis_no,fis_tarihi,aciklama, hesap_no_id ,"
                       "toplam_hesaba_giren, toplam_hesaptan_cikan ";
        table_names  = "bnk_virman_fisler";
    } // ent_fisleri
    else {
        column_names = "fis_id,fis_no,fis_tarihi,aciklama,hesap_no_id, "
                       "toplam_hesaba_giren, toplam_hesaptan_cikan";
        table_names   = "bnk_hareket_fisler";
    }

    SQL_QUERY f_query (DB);

    f_query.PREPARE_SELECT( table_names, column_names , where_str );


    // banka modulunden gelmistir
    if ( m_modul_id EQ BANKA_MODULU ) {
        f_query.AND_EKLE ("program_id = :program_id");
        f_query.SET_VALUE(":program_id",E9_PROGRAMI);

        f_query.AND_EKLE ("modul_id = :modul_id");
        f_query.SET_VALUE(":modul_id", m_modul_id);

        if( m_base_fisi_turu NE 0 ) {
            if ( m_base_fisi_turu NE ENUM_BANKA_VIRMAN_FISI  ) {
                f_query.AND_EKLE ("base_fis_turu = :base_fis_turu");
                f_query.SET_VALUE(":base_fis_turu", m_base_fisi_turu );
            }
        }
    }
    // entegrasyon fisidir
    else if ( m_ui->checkBox_ent_fis_turu->isChecked() EQ true ){
        if ( m_modul_id EQ -1 ) {
            f_query.AND_EKLE ("modul_id != :modul_id");
            f_query.SET_VALUE(":modul_id", BANKA_MODULU );
        }
        else if ( m_modul_id EQ 0 ) {
            f_query.AND_EKLE ("program_id != :program_id");
            f_query.SET_VALUE(":program_id", E9_PROGRAMI );
        }
        else {
            f_query.AND_EKLE ("modul_id = :modul_id");
            f_query.SET_VALUE(":modul_id", m_modul_id );
        }
    }

    if (m_ui->checkbox_tarih_araligi->isChecked() EQ true ) {
        f_query.AND_EKLE ("fis_tarihi BETWEEN :baslangic_tarihi AND :bitis_tarihi");
        f_query.SET_VALUE(":baslangic_tarihi",m_ui->adakDate_first_date->DATE());
        f_query.SET_VALUE(":bitis_tarihi"    ,m_ui->adakDate_second_date->DATE());
    }

    if (m_ui->checkbox_banka_ismi->isChecked() EQ true ) {
        banka_hesaplari_id_list = GET_UYGUN_BANKA_HESAPLARI_ID_LIST(m_ui->combobox_banka_ismi->currentText());
        fisler_banka_ismine_gore_aranacak = true;
    }

    if (f_query.SELECT("fis_tarihi ASC, fis_no ASC") NE 0 ) {
        int     current_row;

        while (f_query.NEXT()) {
            int hesap_no_id = f_query.VALUE(4).toInt();

            if ( fisler_banka_ismine_gore_aranacak EQ true ) {
                if ( banka_hesaplari_id_list.contains ( QVariant ( hesap_no_id ).toString()) EQ false ) {
                    continue;
                }
            }

            current_row = ADD_NEW_ROW();

            m_ui->tableWidget->item ( current_row, FIS_ID_COLUMN       )->setText ( f_query.VALUE(0).toString());
            m_ui->tableWidget->item ( current_row, FIS_NO_COLUMN       )->setText ( f_query.VALUE(1).toString());
            m_ui->tableWidget->item ( current_row, FIS_TARIHI_COLUMN   )->setText ( f_query.VALUE(2).toDate().toString("dd MMMM yyyy dddd"));
            m_ui->tableWidget->item ( current_row, ACIKLAMA_COLUMN     )->setText ( f_query.VALUE(3).toString());
            if (m_base_fisi_turu EQ ENUM_BANKA_MAKRO_FISI) {
                m_ui->tableWidget->item ( current_row, MAKRO_TUTARI_COLUMN )->setText ( VIRGUL_EKLE ( f_query.VALUE(5).toString() ) );
                m_ui->tableWidget->item ( current_row, MAKRO_TUTARI_COLUMN )->setTextAlignment ( Qt::AlignRight + Qt::AlignVCenter );
            }
            else if ( m_base_fisi_turu EQ ENUM_BANKA_ACILIS_FISI ) {

                SQL_QUERY s_query( DB );

                s_query.PREPARE_SELECT( "bnk_hareket_fis_satirlari","etkilenecek_hesap_turu, hesaba_giren, hesaptan_cikan",
                                      "fis_id = :fis_id ");
                s_query.SET_VALUE     ( ":fis_id", f_query.VALUE(0).toString() );

                double banka_hesabi = 0.00;
                double kk_hesabi    = 0.00;
                double pos_hesabi   = 0.00;

                int hesap_turu = 0;

                if ( s_query.SELECT() NE 0 ) {

                    while ( s_query.NEXT() EQ true ) {

                        hesap_turu = s_query.VALUE( 0 ).toInt();
                        switch ( hesap_turu ) {
                            case ENUM_BNK_BANKA_HESABI: {
                                    banka_hesabi += s_query.VALUE( 1 ).toDouble() + s_query.VALUE( 2 ).toDouble();
                                    break;
                                }
                            case ENUM_BNK_POS_HESABI: {
                                    pos_hesabi  += s_query.VALUE( 1 ).toDouble();
                                    break;
                                }
                            case ENUM_BNK_KREDI_KARTI_HESABI: {
                                    kk_hesabi   += s_query.VALUE( 1 ).toDouble() + s_query.VALUE( 2 ).toDouble();

                                break;
                            }
                        }
                    }
                }

                m_ui->tableWidget->item ( current_row, MAKRO_TUTARI_COLUMN   )->setText ( VIRGUL_EKLE ( QVariant ( banka_hesabi ).toString() )  );
                m_ui->tableWidget->item ( current_row, MAKRO_TUTARI_COLUMN   )->setTextAlignment ( Qt::AlignRight + Qt::AlignVCenter            );
                m_ui->tableWidget->item ( current_row, MAKRO_TUTARI_COLUMN_2 )->setText ( VIRGUL_EKLE ( QVariant ( kk_hesabi ).toString() )     );
                m_ui->tableWidget->item ( current_row, MAKRO_TUTARI_COLUMN_2 )->setTextAlignment ( Qt::AlignRight + Qt::AlignVCenter            );
                m_ui->tableWidget->item ( current_row, MAKRO_TUTARI_COLUMN_3 )->setText ( VIRGUL_EKLE ( QVariant ( pos_hesabi ).toString() )    );
                m_ui->tableWidget->item ( current_row, MAKRO_TUTARI_COLUMN_3 )->setTextAlignment ( Qt::AlignRight + Qt::AlignVCenter            );
            }
            else if ( m_base_fisi_turu EQ ENUM_BANKA_VIRMAN_FISI ) {
                m_ui->tableWidget->item ( current_row, MAKRO_TUTARI_COLUMN   )->setText ( VIRGUL_EKLE ( f_query.VALUE(5).toString() )   );
                m_ui->tableWidget->item ( current_row, MAKRO_TUTARI_COLUMN   )->setTextAlignment ( Qt::AlignRight + Qt::AlignVCenter    );
                m_ui->tableWidget->item ( current_row, MAKRO_TUTARI_COLUMN_2 )->setText ( VIRGUL_EKLE ( f_query.VALUE(6).toString() )   );
                m_ui->tableWidget->item ( current_row, MAKRO_TUTARI_COLUMN_2 )->setTextAlignment ( Qt::AlignRight + Qt::AlignVCenter    );
            }
            else if ( m_base_fisi_turu EQ ENUM_BANKA_PARA_YAT_CEKME_FISI  ) {
                m_ui->tableWidget->item ( current_row, MAKRO_TUTARI_COLUMN   )->setText ( VIRGUL_EKLE ( f_query.VALUE(5).toString() )   );
                m_ui->tableWidget->item ( current_row, MAKRO_TUTARI_COLUMN   )->setTextAlignment ( Qt::AlignRight + Qt::AlignVCenter    );
                m_ui->tableWidget->item ( current_row, MAKRO_TUTARI_COLUMN_2 )->setText ( VIRGUL_EKLE ( f_query.VALUE(6).toString() )   );
                m_ui->tableWidget->item ( current_row, MAKRO_TUTARI_COLUMN_2 )->setTextAlignment ( Qt::AlignRight + Qt::AlignVCenter    );
            }
            else {
                m_ui->tableWidget->item ( current_row, MAKRO_TUTARI_COLUMN   )->setText ( VIRGUL_EKLE ( f_query.VALUE(5).toString() )   );
                m_ui->tableWidget->item ( current_row, MAKRO_TUTARI_COLUMN   )->setTextAlignment ( Qt::AlignRight + Qt::AlignVCenter    );
                m_ui->tableWidget->item ( current_row, MAKRO_TUTARI_COLUMN_2 )->setText ( VIRGUL_EKLE ( f_query.VALUE(6).toString() )   );
                m_ui->tableWidget->item ( current_row, MAKRO_TUTARI_COLUMN_2 )->setTextAlignment ( Qt::AlignRight + Qt::AlignVCenter    );
            }
        }
    }
}

/**************************************************/
/*   BNK_HAREKET_FISI_ARAMA::SINGLE_LINE_SELECTED   */
/**************************************************/

int BNK_HAREKET_FISI_ARAMA::SINGLE_LINE_SELECTED(int selected_row_number)
{
    *m_secilen_id = m_ui->tableWidget->item(selected_row_number,FIS_ID_COLUMN )->text().toInt();

    if ( m_fis_turu EQ ENUM_BANKA_FISI ) {
        return ADAK_EXIT;
    }

    OPEN_BNK_HAREKET_FISI ( ENUM_BANKA_HAREKET_FISI, *m_secilen_id, m_modul_id , nativeParentWidget() );

    return ADAK_OK;
}

/****************************************************************/
/*  BNK_HAREKET_FISI_ARAMA::GET_UYGUN_BANKA_HESAPLARI_ID_LIST()   */
/****************************************************************/

QStringList BNK_HAREKET_FISI_ARAMA::GET_UYGUN_BANKA_HESAPLARI_ID_LIST(QString banka_ismi)
{
    SQL_QUERY select_query(DB);

    select_query.PREPARE_SELECT("bnk_hesaplar","hesap_no_id","banka_ismi = :banka_ismi");
    select_query.SET_VALUE(":banka_ismi" , banka_ismi);

    if(select_query.SELECT() EQ 0){
        return QStringList();
    }
    QStringList banka_hesaplari_id_list;

    while (select_query.NEXT()) {
        banka_hesaplari_id_list << select_query.VALUE(0).toString();
    }
    return banka_hesaplari_id_list;
}
