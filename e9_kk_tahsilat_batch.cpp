#include <QMessageBox>
#include "e9_kk_tahsilat_batch_class.h"
#include "ui_e9_kk_tahsilat_batch.h"
#include "dvz_enum.h"
#include "dvz_console_utils.h"
#include "e9_struct.h"
#include "e9_enum.h"
#include "bnk_console_utils.h"
#include "bnk_pos_secimi_open.h"

extern ADAK_SQL *           DB;


/**************************************************************************************
                   OPEN_E9_KK_TAHSILAT_BATCH
***************************************************************************************/

bool OPEN_E9_KK_TAHSILAT_BATCH ( int fis_satiri_id , E9_ISLEM_DETAYLARI * P_ISLEM_DETAYLARI ,int p_modul_id,QWidget * parent )
{
    bool kayit_degisti_mi = false;
    E9_KK_TAHSILAT_BATCH * F = new E9_KK_TAHSILAT_BATCH ( fis_satiri_id , P_ISLEM_DETAYLARI , p_modul_id ,parent );

    F->m_kayit_degisti_mi = &kayit_degisti_mi;

    F->EXEC( NOT_FULL_SCREEN );

    return kayit_degisti_mi;
}

/**************************************************************************************
                   E9_KK_TAHSILAT_BATCH::E9_KK_TAHSILAT_BATCH
***************************************************************************************/

E9_KK_TAHSILAT_BATCH::E9_KK_TAHSILAT_BATCH ( int fis_satiri_id , E9_ISLEM_DETAYLARI * P_ISLEM_DETAYLARI ,int p_modul_id,
                                                 QWidget * parent ) :  BATCH_KERNEL ( parent ), m_ui ( new Ui::E9_KK_TAHSILAT_BATCH )
{
    m_fis_satiri_id   = fis_satiri_id ;

    m_modul_id        = p_modul_id;

    M_ISLEM_DETAYLARI = P_ISLEM_DETAYLARI;

    m_ui->setupUi     ( this );
    START_BATCH_KERNEL( this, DB );
}

/**************************************************************************************
                   E9_KK_TAHSILAT_BATCH::SETUP_FORM
***************************************************************************************/

void E9_KK_TAHSILAT_BATCH::SETUP_FORM()
{
    SET_PAGE_TITLE    ( tr ( "KREDİ KARTI DETAYLARI" ) );
    SET_HELP_PAGE     ( "e9-kk-tahsilat"               );

    REGISTER_BUTTON_WIDGET       ( m_ui->widget_batch_buttons );

    SET_NAME_OF_RUN_BATCH_BUTTON ( "Tamam" );
    SET_ENTER_KEY_FOR_RUN_BATCH  ( true );
    SET_FIRST_FOCUS_WIDGET       ( m_ui->searchEdit_pos_numarasi );
    SET_SETTING_NAME             ( "E9_KK_TAHSILAT_BATCH"         );

    SET_AUTO_EXIT_BATCH ( true );

    m_pos_id          = 0;
    m_ui->lineEdit_kk_numarasi->setMaxLength(16);

    if ( m_fis_satiri_id EQ -1 ) {

        m_ui->lineEdit_kk_numarasi->setText ( M_ISLEM_DETAYLARI->kredi_karti_numarasi   );
        m_ui->lineEdit_kk_sahibi->setText   ( M_ISLEM_DETAYLARI->kredi_karti_sahibi     );

        m_pos_id = M_ISLEM_DETAYLARI->pos_id;

        SQL_QUERY query(DB);

        query.PREPARE_SELECT("bnk_hesabin_poslari,bnk_hesaplar ",
                             "pos_numarasi , doviz_id ",
                             "pos_id = :pos_id "
                             "AND bnk_hesabin_poslari.hesap_no_id = bnk_hesaplar.hesap_no_id ");

        query.SET_VALUE(":pos_id" , M_ISLEM_DETAYLARI->pos_id);

        if(query.SELECT() EQ 0){
            return ;
        }

          query.NEXT();
          m_ui->searchEdit_pos_numarasi->SET_TEXT(query.VALUE(0).toString());
          m_ui->lineEdit_doviz_cinsi->setText(DVZ_GET_DOVIZ_KODU(query.VALUE(1).toInt()));

    }
    else {
        SQL_QUERY sql_query(DB);

        if ( m_modul_id EQ CARI_MODULU ) {

            sql_query.PREPARE_SELECT("bnk_hesabin_poslari,car_fis_satirlari,bnk_hesaplar",
                                     "car_fis_satirlari.pos_id,pos_numarasi ,kredi_karti_numarasi,kredi_karti_sahibi, "
                                     "bnk_hesaplar.doviz_id",
                                     "fis_satiri_id = :fis_satiri_id AND "
                                     "bnk_hesabin_poslari.pos_id = car_fis_satirlari.pos_id AND "
                                     "bnk_hesaplar.hesap_no_id = bnk_hesabin_poslari.hesap_no_id");
        }
        else if ( m_modul_id EQ PERAKENDE_MODULU ){
            sql_query.PREPARE_SELECT("bnk_hesabin_poslari,prk_fis_satirlari,bnk_hesaplar",
                                     "prk_fis_satirlari.pos_id,pos_numarasi ,kredi_karti_numarasi,kredi_karti_sahibi, "
                                     "bnk_hesaplar.doviz_id ",
                                     "fis_satiri_id = :fis_satiri_id AND "
                                     "bnk_hesabin_poslari.pos_id = prk_fis_satirlari.pos_id AND "
                                     "bnk_hesaplar.hesap_no_id = bnk_hesabin_poslari.hesap_no_id");
        }
        else if ( m_modul_id EQ ISLETME_MODULU ) {
            sql_query.PREPARE_SELECT("bnk_hesabin_poslari,isl_fis_satirlari,bnk_hesaplar, bnk_hesabin_kredi_kartlari",
                                     "isl_fis_satirlari.pos_id, pos_numarasi ,kredi_karti_numarasi,kredi_karti_sahibi, "
                                     "bnk_hesaplar.doviz_id ",
                                     "fis_satiri_id = :fis_satiri_id AND "
                                     "bnk_hesabin_poslari.pos_id = isl_fis_satirlari.pos_id AND "
                                     "bnk_hesaplar.hesap_no_id = bnk_hesabin_poslari.hesap_no_id AND "
                                     "bnk_hesaplar.hesap_no_id = bnk_hesabin_kredi_kartlari.hesap_no_id");
        }
        sql_query.SET_VALUE(":fis_satiri_id" , m_fis_satiri_id);

        if ( sql_query.SELECT() NE 0 ) {
            sql_query.NEXT();

            m_pos_id = sql_query.VALUE(0).toInt();
            m_ui->searchEdit_pos_numarasi->SET_TEXT ( sql_query.VALUE(1).toString() );
            m_ui->lineEdit_kk_numarasi->setText     ( sql_query.VALUE(2).toString() );
            m_ui->lineEdit_kk_sahibi->setText       ( sql_query.VALUE(3).toString() );
            m_ui->lineEdit_doviz_cinsi->setText     ( DVZ_GET_DOVIZ_KODU(sql_query.VALUE(4).toInt()));
        }
    }

    SET_SETTING_NAME  ( "E9_KK_TAHSILAT_BATCH" );
}

/**************************************************************************************
                   E9_KK_TAHSILAT_BATCH::CHECK_VAR
***************************************************************************************/

int E9_KK_TAHSILAT_BATCH::CHECK_VAR ( QObject * object )
{
    if ( object EQ m_ui->searchEdit_pos_numarasi ) {
        SQL_QUERY select_query ( DB );

        select_query.PREPARE_SELECT("bnk_hesabin_poslari,bnk_hesaplar",
                                    "pos_id,doviz_id",
                                    "pos_numarasi = :pos_numarasi AND "
                                    "bnk_hesaplar.hesap_no_id = bnk_hesabin_poslari.hesap_no_id");
        select_query.SET_VALUE(":pos_numarasi" , m_ui->searchEdit_pos_numarasi->GET_TEXT());

        if ( select_query.SELECT() EQ 0 ) {
            QMessageBox::warning(this,"UYARI",tr("Aradığınız pos numarası bulunamadı."));
            SET_FOCUS( m_ui->searchEdit_pos_numarasi );
            return ADAK_FAIL;
        }

        select_query.NEXT();

        m_pos_id        = select_query.VALUE(0).toInt();
        m_ui->lineEdit_doviz_cinsi->setText(DVZ_GET_DOVIZ_KODU ( select_query.VALUE(1).toInt()));
    }

    if ( object EQ m_ui->searchEdit_pos_numarasi OR object EQ m_ui->lineEdit_kk_numarasi
         OR object EQ m_ui->lineEdit_kk_sahibi) {
        *m_kayit_degisti_mi = true;
    }

    return ADAK_OK;
}

/**************************************************************************************
                   E9_KK_TAHSILAT_BATCH::CHECK_RUN
***************************************************************************************/

int E9_KK_TAHSILAT_BATCH::CHECK_RUN ()
{
    if ( m_ui->searchEdit_pos_numarasi->GET_TEXT().isEmpty() EQ true ) {
        QMessageBox::warning(this,"UYARI" ,tr ("Pos numarasını boş bırakamazsınız."));
        SET_FOCUS( m_ui->searchEdit_pos_numarasi );
        return ADAK_FAIL;
    }

    if ( m_ui->lineEdit_kk_numarasi->text().isEmpty() EQ true ) {
        QMessageBox::warning(this,"UYARI",tr("Müşteri kredi kartı numarasını boş bırakamazsınız."));
        SET_FOCUS( m_ui->lineEdit_kk_numarasi );
        return ADAK_FAIL;
    }

    if ( m_ui->lineEdit_kk_sahibi->text().isEmpty() EQ true ) {
        QMessageBox::warning(this,"UYARI" , tr ("Kredi Kartı sahibini boş bırakamazsınız."));
        SET_FOCUS( m_ui->lineEdit_kk_sahibi );
        return ADAK_FAIL;
    }

    return ADAK_OK;
}

/**************************************************************************************
                   E9_KK_TAHSILAT_BATCH::RUN_BATCH
***************************************************************************************/

void E9_KK_TAHSILAT_BATCH::RUN_BATCH ()
{
    if ( *m_kayit_degisti_mi EQ true ) {

        QString kredi_karti_numarasi = m_ui->lineEdit_kk_numarasi->text().mid(0,4);
        kredi_karti_numarasi.append ( "XXXXYYYY" );
        kredi_karti_numarasi.append ( m_ui->lineEdit_kk_numarasi->text().mid(12, 4));

        M_ISLEM_DETAYLARI->pos_id               = m_pos_id;
        M_ISLEM_DETAYLARI->kredi_karti_numarasi = kredi_karti_numarasi;
        M_ISLEM_DETAYLARI->kredi_karti_sahibi   = m_ui->lineEdit_kk_sahibi->text();

        SQL_QUERY query( DB );

        query.PREPARE_SELECT("bnk_hesabin_poslari",
                             "hesap_no_id",
                             "pos_id =:pos_id");
        query.SET_VALUE( ":pos_id", m_pos_id );

        if( query.SELECT() NE 0 ) {

            query.NEXT();
            M_ISLEM_DETAYLARI->bnk_hesap_no_id  = query.VALUE( 0 ).toInt();
        }

    }
}

/**************************************************************************************
                   E9_KK_TAHSILAT_BATCH::SEARCH_EDIT_CLICKED
***************************************************************************************/

void E9_KK_TAHSILAT_BATCH::SEARCH_EDIT_CLICKED ( QWidget * , QLineEdit * line_edit)
{
    m_ui->lineEdit_doviz_cinsi->clear();
    int pos_id = OPEN_BNK_POS_SEC ( -1,this );//hesap no id degerine -1 veriyoruz.Pos Tablosunda kayitli olan tum poslari listeliyor.
    if ( pos_id NE -1 ) {
        line_edit->setText(BNK_GET_POS_NUMARASI(pos_id));
    }

}
