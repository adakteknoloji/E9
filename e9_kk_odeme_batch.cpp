#include <QMessageBox>
#include "e9_kk_odeme_batch_class.h"
#include "ui_e9_kk_odeme_batch.h"
#include "dvz_enum.h"
#include "dvz_console_utils.h"
#include "bnk_gui_utils.h"
#include "e9_struct.h"
#include "bnk_kredi_karti_secimi_open.h"
#include "bnk_console_utils.h"

extern ADAK_SQL *           DB;


/**************************************************************************************
                   OPEN_E9_KK_ODEME_BATCH
***************************************************************************************/

bool OPEN_E9_KK_ODEME_BATCH ( int p_fis_satiri_id ,E9_ISLEM_DETAYLARI * P_ISLEM_DETAYLARI,int p_modul_id, QWidget * p_parent )
{
    bool kayit_degisti_mi = false;

    E9_KK_ODEME_BATCH * F = new E9_KK_ODEME_BATCH ( p_fis_satiri_id , P_ISLEM_DETAYLARI , p_modul_id , p_parent  );

    F->m_kayit_degisti_mi = &kayit_degisti_mi;

    F->EXEC( NOT_FULL_SCREEN );

    return kayit_degisti_mi;
}

/**************************************************************************************
                   E9_KK_ODEME_BATCH::E9_KK_ODEME_BATCH
***************************************************************************************/

E9_KK_ODEME_BATCH::E9_KK_ODEME_BATCH ( int fis_satiri_id , E9_ISLEM_DETAYLARI * P_ISLEM_DETAYLARI
                                       ,int p_modul_id, QWidget * parent ) :  BATCH_KERNEL ( parent ), m_ui ( new Ui::E9_KK_ODEME_BATCH )
{
    m_fis_satiri_id   = fis_satiri_id;
    m_modul_id        = p_modul_id;
    M_ISLEM_DETAYLARI = P_ISLEM_DETAYLARI;

    m_ui->setupUi      ( this );
    START_BATCH_KERNEL ( this, DB );
}

/**************************************************************************************
                   E9_KK_ODEME_BATCH::SETUP_FORM
***************************************************************************************/

void E9_KK_ODEME_BATCH::SETUP_FORM()
{
    SET_PAGE_TITLE    ( tr ( "KREDİ KARTI DETAYLARI" ) );
    SET_HELP_PAGE     ( "e9-odeme-kk"                  );

    REGISTER_BUTTON_WIDGET ( m_ui->widget_batch_buttons );

    SET_NAME_OF_RUN_BATCH_BUTTON ( "Tamam" );
    SET_FIRST_FOCUS_WIDGET       ( m_ui->searchEdit_kk_numarasi );
    SET_ENTER_KEY_FOR_RUN_BATCH  ( true );
    SET_SETTING_NAME             ( "E9_KK_ODEME_BATCH"            );

    SET_AUTO_EXIT_BATCH ( true );

    BNK_FILL_COMBO_BOX_BANKA_HESAPLARI(m_ui->comboBox_banka_hesap_kodu);

    m_kredi_karti_id = 0;
    m_hesap_no_id    = 0;

    m_ui->comboBox_banka_hesap_kodu->setDisabled(true);

    if ( m_fis_satiri_id EQ -1 ) {

        m_kredi_karti_id = M_ISLEM_DETAYLARI->kredi_karti_id;

        SQL_QUERY sql_query(DB);

        sql_query.PREPARE_SELECT("bnk_hesabin_kredi_kartlari, bnk_hesaplar ",
                                 "kredi_karti_numarasi, kredi_karti_sahibi, banka_hesap_kodu, banka_ismi,  "
                                 "sube_kodu,sube_ismi,hesap_numarasi,doviz_id,bnk_hesaplar.hesap_no_id  ",
                                 "kredi_karti_id = :kredi_karti_id AND  "
                                 "bnk_hesaplar.hesap_no_id = bnk_hesabin_kredi_kartlari.hesap_no_id     " );

        sql_query.SET_VALUE(":kredi_karti_id" , m_kredi_karti_id );

        if ( sql_query.SELECT() NE 0 ) {
            sql_query.NEXT();

            m_ui->searchEdit_kk_numarasi->SET_TEXT(sql_query.VALUE(0).toString());
            m_ui->lineEdit_kredi_karti_sahibi->setText(sql_query.VALUE(1).toString());
            m_ui->comboBox_banka_hesap_kodu->setCurrentIndex(m_ui->comboBox_banka_hesap_kodu->findText(sql_query.VALUE(2).toString()));

            QString banka_bilgisi = sql_query.VALUE(3).toString() + " / " + sql_query.VALUE(4).toString() + " " +
                                    sql_query.VALUE(5).toString() + "  " + sql_query.VALUE(6).toString();

            m_ui->lineEdit_banka_bilgisi->setText(banka_bilgisi);
            m_ui->lineEdit_doviz_cinsi->setText(DVZ_GET_DOVIZ_KODU(sql_query.VALUE(7).toInt()));
            m_hesap_no_id = sql_query.VALUE(8).toInt();

        }


    }
    else {
        SQL_QUERY sql_query(DB);

        if ( m_modul_id EQ CARI_MODULU ) {

            sql_query.PREPARE_SELECT("car_fis_satirlari","kredi_karti_id","fis_satiri_id = :fis_satiri_id");
            sql_query.SET_VALUE(":fis_satiri_id" , m_fis_satiri_id);
        }
        else if ( m_modul_id EQ PERAKENDE_MODULU ){
            sql_query.PREPARE_SELECT("prk_fis_satirlari","kredi_karti_id ","fis_satiri_id = :fis_satiri_id");
            sql_query.SET_VALUE(":fis_satiri_id" , m_fis_satiri_id );
        }
        else if ( m_modul_id EQ ISLETME_MODULU ) {
            sql_query.PREPARE_SELECT("isl_fis_satirlari","kredi_karti_id ","fis_satiri_id = :fis_satiri_id");
            sql_query.SET_VALUE(":fis_satiri_id" , m_fis_satiri_id );
        }

        if ( sql_query.SELECT() NE 0 ) {
            sql_query.NEXT();
            m_kredi_karti_id = sql_query.VALUE(0).toInt();
        }

        if ( m_kredi_karti_id NE 0 ) {
            sql_query.PREPARE_SELECT("bnk_hesabin_kredi_kartlari, bnk_hesaplar ",
                                     "kredi_karti_numarasi,kredi_karti_sahibi,banka_hesap_kodu,banka_ismi,"
                                     "sube_kodu,sube_ismi,hesap_numarasi,doviz_id,bnk_hesaplar.hesap_no_id",
                                     "kredi_karti_id = :kredi_karti_id AND "
                                     "bnk_hesaplar.hesap_no_id = bnk_hesabin_kredi_kartlari.hesap_no_id");
            sql_query.SET_VALUE(":kredi_karti_id" , m_kredi_karti_id );

            if ( sql_query.SELECT() NE 0 ) {
                sql_query.NEXT();

                m_ui->searchEdit_kk_numarasi->SET_TEXT(sql_query.VALUE(0).toString());
                m_ui->lineEdit_kredi_karti_sahibi->setText(sql_query.VALUE(1).toString());
                m_ui->comboBox_banka_hesap_kodu->setCurrentIndex(m_ui->comboBox_banka_hesap_kodu->findText(sql_query.VALUE(2).toString()));

                QString banka_bilgisi = sql_query.VALUE(3).toString() + " / " + sql_query.VALUE(4).toString() + " " +
                                        sql_query.VALUE(5).toString() + "  " + sql_query.VALUE(6).toString();

                m_ui->lineEdit_banka_bilgisi->setText(banka_bilgisi);
                m_ui->lineEdit_doviz_cinsi->setText(DVZ_GET_DOVIZ_KODU(sql_query.VALUE(7).toInt()));

                m_hesap_no_id = sql_query.VALUE(8).toInt();

            }
        }

    }
}

/**************************************************************************************
                   E9_KK_ODEME_BATCH::CHECK_VAR
***************************************************************************************/

int E9_KK_ODEME_BATCH::CHECK_VAR ( QObject * object )
{
    if ( object EQ m_ui->searchEdit_kk_numarasi ) {
        m_ui->lineEdit_doviz_cinsi->clear();

        SQL_QUERY select_query(DB);

        select_query.PREPARE_SELECT ( "bnk_hesabin_kredi_kartlari, bnk_hesaplar ",
                                      "kredi_karti_id,kredi_karti_sahibi,banka_hesap_kodu,banka_ismi,"
                                      "sube_kodu,sube_ismi,hesap_numarasi,doviz_id,bnk_hesaplar.hesap_no_id",
                                      "kredi_karti_numarasi = :kredi_karti_numarasi AND "
                                      "bnk_hesaplar.hesap_no_id = bnk_hesabin_kredi_kartlari.hesap_no_id");
        select_query.SET_VALUE      ( ":kredi_karti_numarasi" , m_ui->searchEdit_kk_numarasi->GET_TEXT());

        if( select_query.SELECT() EQ 0 ) {
            MSG_WARNING( tr ( "Aradığınız Kredi Kartı bulunamadı."), m_ui->searchEdit_kk_numarasi );
            return ADAK_FAIL;
        }
        select_query.NEXT();
        m_kredi_karti_id = select_query.VALUE(0).toInt();


        m_ui->lineEdit_kredi_karti_sahibi->setText(select_query.VALUE(1).toString());
        m_ui->comboBox_banka_hesap_kodu->setCurrentIndex(m_ui->comboBox_banka_hesap_kodu->findText(select_query.VALUE(2).toString()));

        QString banka_bilgisi = select_query.VALUE(3).toString() + " / " + select_query.VALUE(4).toString() + " " +
                                select_query.VALUE(5).toString() + "  " + select_query.VALUE(6).toString();

        m_ui->lineEdit_banka_bilgisi->setText(banka_bilgisi);
        m_ui->lineEdit_doviz_cinsi->setText(DVZ_GET_DOVIZ_KODU(select_query.VALUE(7).toInt()));
        *m_kayit_degisti_mi = true;
        m_hesap_no_id = select_query.VALUE(8).toInt();

    }

    return ADAK_OK;
}

/**************************************************************************************
                   E9_KK_ODEME_BATCH::CHECK_RUN
***************************************************************************************/

int E9_KK_ODEME_BATCH::CHECK_RUN ()
{
    if ( m_ui->searchEdit_kk_numarasi->GET_TEXT().isEmpty() EQ true ) {
        MSG_WARNING( tr("Ödeme Yapılacak Kredi Kartını Seçmelisiniz."), m_ui->searchEdit_kk_numarasi );

        return ADAK_FAIL;
    }
    return ADAK_OK;
}

/**************************************************************************************
                   E9_KK_ODEME_BATCH::RUN_BATCH
***************************************************************************************/

void E9_KK_ODEME_BATCH::RUN_BATCH ()
{
    if ( *m_kayit_degisti_mi EQ true ) {
        M_ISLEM_DETAYLARI->kredi_karti_id = m_kredi_karti_id;
        M_ISLEM_DETAYLARI->bnk_hesap_no_id= m_hesap_no_id;
    }
}


/**************************************************************************************
                   E9_KK_ODEME_BATCH::SEARCH_EDIT_CLICKED
***************************************************************************************/

void E9_KK_ODEME_BATCH::SEARCH_EDIT_CLICKED ( QWidget * , QLineEdit * line_edit)
{
    m_ui->lineEdit_doviz_cinsi->clear();
    int kredi_karti_id = OPEN_BNK_KREDI_KARTI_SEC(-1,this);//Kredi Karti secimi ekranina hesap_no_id olarak -1 gonderiyoruz,hesap numarasina bakilmaksizin
                                                           //kayitli olan tum kredi kartlari listeleniyor.

    m_kredi_karti_id = kredi_karti_id;
    line_edit->setText(BNK_GET_KREDI_KARTI_NUMARASI(kredi_karti_id));

}
