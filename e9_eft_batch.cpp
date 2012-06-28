#include <QMessageBox>
#include "e9_eft_batch_class.h"
#include "ui_e9_eft_batch.h"
#include "cari_enum.h"
#include "bnk_gui_utils.h"
#include "bnk_hesaplari_arama_open.h"
#include "dvz_enum.h"
#include "dvz_console_utils.h"
#include "adak_std_utils.h"
#include "e9_enum.h"
#include "e9_struct.h"

extern ADAK_SQL *           DB;

/**************************************************************************************
                   OPEN_E9_EFT_BATCH
***************************************************************************************/

bool OPEN_E9_EFT_BATCH (int p_fis_turu , int p_islem_turu,int p_fis_satiri_id ,int p_makbuz_para_birimi_id,
                        E9_ISLEM_DETAYLARI * P_ISLEM_DETAYLARI,int p_modul_id,QWidget * parent )
{
    bool kayit_degistirildi_mi = false;

    E9_EFT_BATCH * F = new E9_EFT_BATCH ( p_fis_turu , p_islem_turu , p_fis_satiri_id ,
                                          p_makbuz_para_birimi_id, P_ISLEM_DETAYLARI,p_modul_id, parent );

    F->m_kayit_degistirildi_mi = &kayit_degistirildi_mi;

    F->EXEC( NOT_FULL_SCREEN );

    return kayit_degistirildi_mi;
}

/**************************************************************************************
                   E9_EFT_BATCH::E9_EFT_BATCH
***************************************************************************************/

E9_EFT_BATCH::E9_EFT_BATCH ( int fis_turu , int islem_turu ,int fis_satiri_id ,
                                 int makbuz_para_birimi_id, E9_ISLEM_DETAYLARI * P_ISLEM_DETAYLARI ,int p_modul_id,
                                 QWidget * parent ) :  BATCH_KERNEL ( parent ), m_ui ( new Ui::E9_EFT_BATCH )
{
    m_fis_turu              = fis_turu;
    m_islem_turu            = islem_turu;
    m_fis_satiri_id         = fis_satiri_id;
    m_modul_id              = p_modul_id;
    m_makbuz_para_birimi_id = makbuz_para_birimi_id;

    M_ISLEM_DETAYLARI       = P_ISLEM_DETAYLARI;

    m_ui->setupUi      (this );
    START_BATCH_KERNEL (this, DB );
}

/**************************************************************************************
                   E9_EFT_BATCH::SETUP_FORM
***************************************************************************************/

void E9_EFT_BATCH::SETUP_FORM()
{
    SET_PAGE_TITLE    ( tr ( "HAVALE / EFT DETAYLARI" ) );
    SET_HELP_PAGE     ( "e9-eft-havele" );

    REGISTER_BUTTON_WIDGET ( m_ui->widget_batch_buttons );

    SET_FIRST_FOCUS_WIDGET        ( m_ui->comboBox_banka_hesap_kodu );
    SET_NAME_OF_RUN_BATCH_BUTTON  ( "Tamam" );
    SET_ENTER_KEY_FOR_RUN_BATCH   ( true );
    SET_SETTING_NAME              ( "E9_EFT_BATCH" );

    SET_AUTO_EXIT_BATCH ( true );

    BNK_FILL_COMBO_BOX_BANKA_HESAPLARI( m_ui->comboBox_banka_hesap_kodu );

    DISABLE_CLEAR_ON_WIDGET(m_ui->combobox_banka_ismi);

    m_hesap_no_id           = 0;


    m_ui->frame_musteri_banka_bilgisi->setHidden(true);

    if ( m_fis_turu EQ ENUM_CARI_ODEME_FISI ){
        GET_BANKALAR ( m_ui->combobox_banka_ismi );
        if ( m_islem_turu EQ ENUM_HAVALE ) {
            m_ui->combobox_banka_ismi->setDisabled(true);
        }
        m_ui->frame_musteri_banka_bilgisi->setHidden(false);
    }

    if ( m_modul_id EQ PERAKENDE_MODULU OR m_modul_id EQ SMM_MODULU OR m_modul_id
             EQ ISLETME_MODULU ) {
        if ( m_islem_turu EQ ENUM_HAVALE ) {
            GET_BANKALAR ( m_ui->combobox_banka_ismi );
            m_ui->combobox_banka_ismi->setDisabled(true);
        }
    }

    if ( m_fis_satiri_id EQ -1 ) {

        if ( m_modul_id EQ CARI_MODULU ) {
            m_ui->lineedit_hesap_no->setText(M_ISLEM_DETAYLARI->hesap_no);
            m_ui->combobox_banka_ismi->setEditText(M_ISLEM_DETAYLARI->sube_adi);
            m_ui->lineedit_sube_kodu->setText(M_ISLEM_DETAYLARI->sube_kodu);
            m_ui->combobox_sube_ismi->setEditText(M_ISLEM_DETAYLARI->sube_adi);
            m_ui->combobox_banka_ismi->setEditText(M_ISLEM_DETAYLARI->banka_ismi);
        }

        m_hesap_no_id = M_ISLEM_DETAYLARI->bnk_hesap_no_id;

        SQL_QUERY query(DB);

        query.PREPARE_SELECT("bnk_hesaplar",
                             "banka_hesap_kodu,banka_ismi,sube_kodu,sube_ismi,hesap_numarasi,doviz_id",
                             "hesap_no_id = :hesap_no_id AND doviz_id = :doviz_id");

        query.SET_VALUE(":hesap_no_id" , m_hesap_no_id);
        query.SET_VALUE(":doviz_id"    , m_makbuz_para_birimi_id);

        if ( query.SELECT() NE 0 ) {
            query.NEXT();
            m_ui->comboBox_banka_hesap_kodu->setCurrentIndex(m_ui->comboBox_banka_hesap_kodu->findText(query.VALUE(0).toString()));
            QString banka_bilgisi = query.VALUE(1).toString() + " / " + query.VALUE(2).toString() + " " + query.VALUE(3).toString()
                                    + " " +  query.VALUE(4).toString();
            m_ui->lineEdit_bnk_sube_bilgisi->setText(banka_bilgisi);
            m_ui->lineEdit_doviz_cinsi->setText(DVZ_GET_DOVIZ_KODU(query.VALUE(5).toInt()));

        }
    }
    else {
        SQL_QUERY sql_query(DB);

        if ( m_modul_id EQ CARI_MODULU ) {

            sql_query.PREPARE_SELECT("car_fis_satirlari",
                                     "hesap_no_id , musteri_banka_ismi , musteri_sube_kodu , musteri_sube_ismi,musteri_hesap_no",
                                     "fis_satiri_id = :fis_satiri_id");


        }
        else if ( m_modul_id EQ PERAKENDE_MODULU ) {//Perakende modulu

            sql_query.PREPARE_SELECT("prk_fis_satirlari",
                                     "hesap_no_id",
                                     "fis_satiri_id = :fis_satiri_id");


        }
        else if ( m_modul_id EQ ISLETME_MODULU ) {
            sql_query.PREPARE_SELECT("isl_fis_satirlari",
                                     "hesap_no_id",
                                     "fis_satiri_id = :fis_satiri_id");
        }

        sql_query.SET_VALUE(":fis_satiri_id" , m_fis_satiri_id);

        if ( sql_query.SELECT() NE 0 ) {
            sql_query.NEXT();
            m_hesap_no_id =  sql_query.VALUE(0).toInt();

            if ( m_fis_turu EQ ENUM_CARI_ODEME_FISI ) {

                GET_BANKA_SUBE_ADLARI(m_ui->combobox_sube_ismi,sql_query.VALUE(1).toString());

                if ( sql_query.VALUE(1).toString() EQ NULL) {
                    m_ui->combobox_banka_ismi->setEditText(M_ISLEM_DETAYLARI->banka_ismi);
                }
                else {
                    m_ui->combobox_banka_ismi->setEditText(sql_query.VALUE(1).toString());
                }
                if ( sql_query.VALUE(2).toString() EQ NULL ) {
                    m_ui->lineedit_sube_kodu->setText(M_ISLEM_DETAYLARI->sube_kodu);
                }
                else {
                    m_ui->lineedit_sube_kodu->setText(sql_query.VALUE(2).toString());
                }
                if ( sql_query.VALUE(3).toString() EQ NULL ) {
                    m_ui->combobox_sube_ismi->setEditText(M_ISLEM_DETAYLARI->sube_adi);
                }
                else {
                    m_ui->combobox_sube_ismi->setEditText(sql_query.VALUE(3).toString());
                }

                if ( sql_query.VALUE(4).toString() EQ NULL ) {
                    m_ui->lineedit_hesap_no->setText(M_ISLEM_DETAYLARI->hesap_no);
                }
                else {
                    m_ui->lineedit_hesap_no->setText(sql_query.VALUE(4).toString());
                }
            }


            int row = BNK_HESAPLARI_MODEL_INDEX_BUL( m_hesap_no_id, m_ui->comboBox_banka_hesap_kodu );

            if ( row > -1) {

                m_ui->comboBox_banka_hesap_kodu->setCurrentIndex(m_ui->comboBox_banka_hesap_kodu->findText(BNK_GET_DATA_FROM_BNK_HESAPLARI_MODEL(m_ui->comboBox_banka_hesap_kodu,row,2).toString()));

                m_ui->lineEdit_doviz_cinsi->setText ( DVZ_GET_DOVIZ_KODU ( BNK_GET_DATA_FROM_BNK_HESAPLARI_MODEL ( m_ui->comboBox_banka_hesap_kodu, row, 1 ).toInt()) );


                QString banka_bilgisi = BNK_GET_DATA_FROM_BNK_HESAPLARI_MODEL ( m_ui->comboBox_banka_hesap_kodu, row, 3 ).toString() + " / " +
                                        BNK_GET_DATA_FROM_BNK_HESAPLARI_MODEL ( m_ui->comboBox_banka_hesap_kodu, row, 4 ).toString() + " " +
                                        BNK_GET_DATA_FROM_BNK_HESAPLARI_MODEL ( m_ui->comboBox_banka_hesap_kodu, row, 5 ).toString() + " " +
                                        BNK_GET_DATA_FROM_BNK_HESAPLARI_MODEL ( m_ui->comboBox_banka_hesap_kodu, row, 6 ).toString();


                m_ui->lineEdit_bnk_sube_bilgisi->setText ( banka_bilgisi );
            }
        }
    }
}

/**************************************************************************************
                   E9_EFT_BATCH::CHECK_VAR
***************************************************************************************/

int E9_EFT_BATCH::CHECK_VAR ( QObject * object )
{
    if ( object EQ m_ui->comboBox_banka_hesap_kodu  ) {
        int row = -1;

        m_ui->lineEdit_bnk_sube_bilgisi->clear();
        m_ui->lineEdit_doviz_cinsi->clear();
        m_ui->comboBox_banka_hesap_kodu->setCurrentIndex(-1);

        if ( m_makbuz_para_birimi_id EQ 0 ) {
            MSG_WARNING(tr("Önce makbuz para birimini seçmelisiniz."), NULL );
            return ADAK_FAIL;
        }

        if ( object EQ m_ui->comboBox_banka_hesap_kodu ) {
            row = m_ui->comboBox_banka_hesap_kodu->view()->currentIndex().row();
        }
        else {
            int bnk_hesap_id = OPEN_BNK_HESAPLARI_ARAMA(this);

            SQL_QUERY query(DB);

            query.PREPARE_SELECT("bnk_hesaplar", "doviz_id","hesap_no_id = :hesap_no_id");
            query.SET_VALUE(":hesap_no_id" , bnk_hesap_id);

            if ( query.SELECT() NE 0 ) {
                query.NEXT();
                int doviz_id = query.VALUE(0).toInt();
                if ( doviz_id NE m_makbuz_para_birimi_id ) {
                    MSG_WARNING( tr("Makbuz para birimi ile banka hesabının para birimi aynı değil.\n"
                                                        "Lütfen banka hesabını değiştiriniz. "), m_ui->comboBox_banka_hesap_kodu );
                    return ADAK_FAIL;
                }

            }


            row = BNK_HESAPLARI_MODEL_INDEX_BUL(bnk_hesap_id,m_ui->comboBox_banka_hesap_kodu);

            if ( row NE -1 ) {
                m_ui->comboBox_banka_hesap_kodu->setCurrentIndex(row);
            }
        }
        if ( row EQ -1 ) {
            return ADAK_FAIL;
        }
        m_hesap_no_id = BNK_GET_DATA_FROM_BNK_HESAPLARI_MODEL(m_ui->comboBox_banka_hesap_kodu , row , 0).toInt();

        SQL_QUERY query(DB);

        query.PREPARE_SELECT("bnk_hesaplar","doviz_id", "hesap_no_id = :hesap_no_id" );
        query.SET_VALUE(":hesap_no_id" , m_hesap_no_id);

        if ( query.SELECT() NE 0 ) {
            query.NEXT();
            int doviz_id = query.VALUE(0).toInt();
            if ( doviz_id NE m_makbuz_para_birimi_id ) {
                MSG_WARNING( tr("Makbuz para birimi ile banka hesabının para birimi aynı değil.\n"
                                                    "Lütfen banka hesabını değiştiriniz. "), m_ui->comboBox_banka_hesap_kodu );
                return ADAK_FAIL;
            }

        }

        m_ui->comboBox_banka_hesap_kodu->setCurrentIndex(m_ui->comboBox_banka_hesap_kodu->findText(BNK_GET_DATA_FROM_BNK_HESAPLARI_MODEL(m_ui->comboBox_banka_hesap_kodu,row,2).toString()));

        m_ui->lineEdit_doviz_cinsi->setText(DVZ_GET_DOVIZ_KODU(BNK_GET_DATA_FROM_BNK_HESAPLARI_MODEL(m_ui->comboBox_banka_hesap_kodu,row,1).toInt()));

        QString banka_sube_bilgisi = BNK_GET_DATA_FROM_BNK_HESAPLARI_MODEL(m_ui->comboBox_banka_hesap_kodu,row,3).toString() +
                                     " / " + BNK_GET_DATA_FROM_BNK_HESAPLARI_MODEL(m_ui->comboBox_banka_hesap_kodu,row,4).toString()
                                     +" " + BNK_GET_DATA_FROM_BNK_HESAPLARI_MODEL(m_ui->comboBox_banka_hesap_kodu,row,5).toString()
                                     + " " + BNK_GET_DATA_FROM_BNK_HESAPLARI_MODEL(m_ui->comboBox_banka_hesap_kodu,row,6).toString();
        m_ui->lineEdit_bnk_sube_bilgisi->setText(banka_sube_bilgisi);
        m_ui->comboBox_banka_hesap_kodu->setCurrentIndex(row);

        if ( m_islem_turu EQ ENUM_HAVALE ) {
            if ( m_fis_turu EQ ENUM_CARI_ODEME_FISI ) {
                m_ui->lineedit_sube_kodu->clear();
                m_ui->lineedit_hesap_no->clear();

                QString banka_ismi = BNK_GET_DATA_FROM_BNK_HESAPLARI_MODEL(m_ui->comboBox_banka_hesap_kodu,row,4).toString();

                m_ui->combobox_banka_ismi->setCurrentIndex(m_ui->combobox_banka_ismi->findText(banka_ismi));

                GET_BANKA_SUBE_ADLARI(m_ui->combobox_sube_ismi, m_ui->combobox_banka_ismi->currentText());
            }
        }
    }

    else if ( object EQ m_ui->combobox_banka_ismi ) {

        if ( m_fis_turu EQ ENUM_CARI_ODEME_FISI AND m_islem_turu EQ ENUM_EFT ) {

            m_ui->combobox_sube_ismi->clear();
            m_ui->lineedit_sube_kodu->clear();

            GET_BANKA_SUBE_ADLARI(m_ui->combobox_sube_ismi, m_ui->combobox_banka_ismi->currentText());
        }

    }


    else if ( object EQ m_ui->combobox_sube_ismi) {

        QString sube_ismi = m_ui->combobox_sube_ismi->currentText();
        m_ui->combobox_sube_ismi->setCurrentIndex ( m_ui->combobox_sube_ismi->findText ( ( sube_ismi ) ) );
        m_ui->lineedit_sube_kodu->setText ( GET_BANKA_SUBE_KODU( m_ui->combobox_banka_ismi->currentText(),sube_ismi ) );
    }


    if ( object EQ m_ui->comboBox_banka_hesap_kodu OR object EQ m_ui->combobox_banka_ismi OR
         object EQ m_ui->combobox_sube_ismi OR object EQ m_ui->lineedit_hesap_no ) {
         *m_kayit_degistirildi_mi = true;
    }

    return ADAK_OK;
}

/**************************************************************************************
                   E9_EFT_BATCH::CHECK_RUN
***************************************************************************************/

int E9_EFT_BATCH::CHECK_RUN ()
{
    if ( m_ui->comboBox_banka_hesap_kodu->currentIndex() EQ -1 ) {
        MSG_WARNING(  tr("Banka hesap kodunu boş bırakamazsınız."), m_ui->comboBox_banka_hesap_kodu );

        return ADAK_FAIL;
    }
    return ADAK_OK;
}

/**************************************************************************************
                   E9_EFT_BATCH::RUN_BATCH
***************************************************************************************/

void E9_EFT_BATCH::RUN_BATCH ()
{
    if ( *m_kayit_degistirildi_mi EQ true ) {
        M_ISLEM_DETAYLARI->banka_ismi      = m_ui->combobox_banka_ismi->currentText();
        M_ISLEM_DETAYLARI->sube_adi        = m_ui->combobox_sube_ismi->currentText();
        M_ISLEM_DETAYLARI->sube_kodu       = m_ui->lineedit_sube_kodu->text();
        M_ISLEM_DETAYLARI->bnk_hesap_no_id = m_hesap_no_id;
        M_ISLEM_DETAYLARI->hesap_no        = m_ui->lineedit_hesap_no->text();
    }
}




