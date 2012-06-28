#include <QMessageBox>
#include "adak_std_utils.h"
#include "bnk_makro_fisi_arama_class.h"
#include "bnk_makro_fisi_arama_open.h"
#include "ui_bnk_makro_fisi_arama.h"
#include "e9_console_utils.h"
#include "e9_gui_utils.h"
#include "e9_enum.h"
#include "banka_enum.h"

#define     FIS_ID_COLUMN           0
#define     FIS_TARIHI_COLUMN       1
#define     FIS_NO_COLUMN           2
#define     ACIKLAMA_COLUMN         3
#define     MAKRO_TUTARI_COLUMN     4
#define     MAKRO_TUTARI_COLUMN_2   5

extern ADAK_SQL * DB;

/**************************************************/
/*               F_BNK_MAKRO_FISI_ARAMA           */
/**************************************************/

int OPEN_BNK_MAKRO_FISI_ARAMA( int banka_modulu_fis_turu , int fis_kaynagi , QWidget * parent)
{
    int secilen_id    = -1;

    BNK_MAKRO_FISI_ARAMA * F = new BNK_MAKRO_FISI_ARAMA( banka_modulu_fis_turu,fis_kaynagi, parent);
    F->m_secilen_id = &secilen_id;
    F->EXEC( NOT_FULL_SCREEN );

    return secilen_id;
}

/**********************************************************/
/*       BNK_MAKRO_FISI_ARAMA::BNK_MAKRO_FISI_ARAMA       */
/**********************************************************/

BNK_MAKRO_FISI_ARAMA::BNK_MAKRO_FISI_ARAMA(int banka_modulu_fis_turu,int fis_kaynagi, QWidget *parent) : ARAMA_KERNEL (parent), m_ui(new Ui::BNK_MAKRO_FISI_ARAMA)
{
    m_base_fisi_turu = banka_modulu_fis_turu;
    m_modul_id              = fis_kaynagi;

    m_ui->setupUi      (this);
    START_ARAMA_KERNEL (this,DB);
}

/*****************************************************/
/*         BNK_MAKRO_FISI_ARAMA::SETUP_FORM           */
/*****************************************************/

void BNK_MAKRO_FISI_ARAMA::SETUP_FORM()
{
    SET_HELP_PAGE      ( "banka-makro-fisi" );
    SET_PAGE_TITLE( tr ( "BNK - BANKA MAKRO FİŞİ ARAMA" ) );

    REGISTER_TABLE_WIDGET         (m_ui->tableWidget);
    REGISTER_ARAMA_BUTTONS_WIDGET (m_ui->widget_batch_buttons);

    QStringList headers;
    if (m_base_fisi_turu EQ ENUM_BANKA_MAKRO_FISI) {
        headers <<tr("Fiş Id")<<tr("Fiş Tarihi")<<tr("Fiş No")<<tr("Açıklama")<<tr("İşlem Tutarı");
    }
    else {
        headers <<tr("Fiş Id")<<tr("Fiş Tarihi")<<tr("Fiş No")<<tr("Açıklama")<<tr("Toplam Hesaba Giren")<<tr("Toplam Hesaptan Çıkan");
    }

    SET_HEADERS ( headers );
    SET_SORTING ( false   );

    m_ui->adakDate_first_date->SET_DATE(MALI_YIL_FIRST_DATE());

    m_ui->frame_banka->setEnabled ( false );
    m_ui->frame_tarih->setEnabled ( false );

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
    m_ui->tableWidget->setColumnWidth ( FIS_NO_COLUMN,          50  );
    if (m_base_fisi_turu EQ ENUM_BANKA_MAKRO_FISI) {
        m_ui->tableWidget->setColumnWidth ( ACIKLAMA_COLUMN,        350 );
        m_ui->tableWidget->setColumnWidth ( MAKRO_TUTARI_COLUMN,    170 );
        SET_SETTING_NAME("BANKA_MAKRO_FISI");
    }
    else {
        m_ui->tableWidget->setColumnWidth ( FIS_TARIHI_COLUMN,      200 );
        m_ui->tableWidget->setColumnWidth ( ACIKLAMA_COLUMN,        250 );
        m_ui->tableWidget->setColumnWidth ( MAKRO_TUTARI_COLUMN,    155 );
        m_ui->tableWidget->setColumnWidth ( MAKRO_TUTARI_COLUMN_2,  155 );
        SET_SETTING_NAME("BANKA_HAREKET_FISI");
    }


    SET_ENTER_KEY_FOR_RUN_BATCH ( true );
    SET_FIND_BUTTON_NAME        ( tr ( "Fişleri Bul" ) );
}

/*****************************************************/
/*         BNK_MAKRO_FISI_ARAMA::CHECK_VAR           */
/*****************************************************/

int BNK_MAKRO_FISI_ARAMA::CHECK_VAR(QObject * object)
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

    return ADAK_OK;
}

/*****************************************************/
/*          BNK_MAKRO_FISI_ARAMA::CHECK_RUN        */
/*****************************************************/

int BNK_MAKRO_FISI_ARAMA::CHECK_RUN()
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
/*             BNK_MAKRO_FISI_ARAMA::SEARCH          */
/*****************************************************/

void BNK_MAKRO_FISI_ARAMA::SEARCH()
{
    QStringList banka_hesaplari_id_list;
    bool fisler_banka_ismine_gore_aranacak = false;

    SQL_QUERY select_query(DB);

    select_query.PREPARE_SELECT("bnk_makro_fisler","fis_id,fis_no,fis_tarihi,aciklama,toplam_makro_tutari,hesap_no_id,"
                                "toplam_hesaba_giren, toplam_hesaptan_cikan","modul_id = :modul_id" );

    select_query.SET_VALUE(":modul_id",m_modul_id);

    if (m_ui->checkbox_tarih_araligi->isChecked() EQ true ) {
        select_query.AND_EKLE ("fis_tarihi BETWEEN :fis_tarihi_1 AND :fis_tarihi_2");
        select_query.SET_VALUE(":fis_tarihi_1",m_ui->adakDate_first_date->DATE());
        select_query.SET_VALUE(":fis_tarihi_2",m_ui->adakDate_second_date->DATE());
    }

    if (m_ui->checkbox_banka_ismi->isChecked() EQ true ) {
        banka_hesaplari_id_list = GET_UYGUN_BANKA_HESAPLARI_ID_LIST(m_ui->combobox_banka_ismi->currentText());
        fisler_banka_ismine_gore_aranacak = true;
    }

    if (select_query.SELECT("fis_tarihi, fis_no") NE 0 ) {
        int     current_row;

        while (select_query.NEXT()) {
            int hesap_no_id = select_query.VALUE(5).toInt();

            if ( fisler_banka_ismine_gore_aranacak EQ true ) {
                if ( banka_hesaplari_id_list.contains ( QVariant ( hesap_no_id ).toString()) EQ false ) {
                    continue;
                }
            }

            current_row = ADD_NEW_ROW();

            m_ui->tableWidget->item ( current_row, FIS_ID_COLUMN       )->setText ( select_query.VALUE(0).toString());
            m_ui->tableWidget->item ( current_row, FIS_NO_COLUMN       )->setText ( select_query.VALUE(1).toString());
            m_ui->tableWidget->item ( current_row, FIS_TARIHI_COLUMN   )->setText ( select_query.VALUE(2).toDate().toString("dd MMMM yyyy dddd"));
            m_ui->tableWidget->item ( current_row, ACIKLAMA_COLUMN     )->setText ( select_query.VALUE(3).toString());
            if (m_base_fisi_turu EQ ENUM_BANKA_MAKRO_FISI) {
                m_ui->tableWidget->item ( current_row, MAKRO_TUTARI_COLUMN )->setText ( VIRGUL_EKLE ( select_query.VALUE(4).toString() ) );
                m_ui->tableWidget->item ( current_row, MAKRO_TUTARI_COLUMN )->setTextAlignment ( Qt::AlignRight + Qt::AlignVCenter );
            }
            else {
                m_ui->tableWidget->item ( current_row, MAKRO_TUTARI_COLUMN   )->setText ( VIRGUL_EKLE ( select_query.VALUE(6).toString() ) );
                m_ui->tableWidget->item ( current_row, MAKRO_TUTARI_COLUMN   )->setTextAlignment ( Qt::AlignRight + Qt::AlignVCenter );
                m_ui->tableWidget->item ( current_row, MAKRO_TUTARI_COLUMN_2 )->setText ( VIRGUL_EKLE ( select_query.VALUE(7).toString() ) );
                m_ui->tableWidget->item ( current_row, MAKRO_TUTARI_COLUMN_2 )->setTextAlignment ( Qt::AlignRight + Qt::AlignVCenter );
            }
        }
    }
}

/**************************************************/
/*   BNK_MAKRO_FISI_ARAMA::SINGLE_LINE_SELECTED   */
/**************************************************/

int BNK_MAKRO_FISI_ARAMA::SINGLE_LINE_SELECTED(int selected_row_number)
{
    *m_secilen_id = m_ui->tableWidget->item(selected_row_number,FIS_ID_COLUMN )->text().toInt();
    return ADAK_EXIT;
}

/****************************************************************/
/*  BNK_MAKRO_FISI_ARAMA::GET_UYGUN_BANKA_HESAPLARI_ID_LIST()   */
/****************************************************************/

QStringList BNK_MAKRO_FISI_ARAMA::GET_UYGUN_BANKA_HESAPLARI_ID_LIST(QString banka_ismi)
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
