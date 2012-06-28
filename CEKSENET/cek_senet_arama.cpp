#include "cek_senet_arama_class.h"
#include "ui_cek_senet_arama.h"
#include "adak_utils.h"
#include "adak_std_utils.h"
#include "e9_console_utils.h"
#include "e9_gui_utils.h"
#include "dvz_enum.h"
#include "dvz_console_utils.h"
#include "e9_enum.h"
#include "cari_console_utils.h"
#include "bnk_console_utils.h"
#include "cek_enum.h"
#include "cek_console_utils.h"


extern ADAK_SQL * DB;

/**************************************************************************************
                   OPEN_CEK_SENET_ARAMA
***************************************************************************************/

int OPEN_CEK_SENET_ARAMA ( int aranacak_cek_senet_turu , int cek_mi_senet_mi, int musteri_cek_senedi_mi,
                           int hesap_id, int para_birimi_id , QWidget * parent )
{
    int secilen_id = -1;

    CEK_SENET_ARAMA * F = new CEK_SENET_ARAMA ( aranacak_cek_senet_turu,cek_mi_senet_mi, musteri_cek_senedi_mi,hesap_id,
                                                para_birimi_id , parent);
    F->m_secilen_id = &secilen_id;
    F->EXEC( NOT_FULL_SCREEN );
    return secilen_id;
}

/**************************************************************************************
                   CEK_SENET_ARAMA::CEK_SENET_ARAMA
***************************************************************************************/

CEK_SENET_ARAMA::CEK_SENET_ARAMA ( int aranacak_cek_senet_turu, int cek_mi_senet_mi, int musteri_cek_senedi_mi,
                                   int hesap_id, int para_birimi_id , QWidget *parent ) : ARAMA_KERNEL ( parent ), m_ui ( new Ui::CEK_SENET_ARAMA )
{
    m_aranacak_cek_senet_turu = aranacak_cek_senet_turu;
    m_cek_mi_senet_mi         = cek_mi_senet_mi; //cek,senet veya her ikiside olabilir.
    m_musteri_cek_senedi_mi   = musteri_cek_senedi_mi;
    m_hesap_id                = hesap_id;
    m_para_birimi_id          = para_birimi_id;

    m_ui->setupUi      (this ) ;

    START_ARAMA_KERNEL (this , DB );
}

/**************************************************************************************
                   CEK_SENET_ARAMA::SETUP_FORM
***************************************************************************************/

void CEK_SENET_ARAMA::SETUP_FORM()
{
    REGISTER_TABLE_WIDGET         ( m_ui->tableWidget );
    REGISTER_ARAMA_BUTTONS_WIDGET ( m_ui->widget_batch_buttons );

    DISABLE_CLEAR_ON_WIDGET(m_ui->combobox_sirket_filtresi);

    m_ui->adak_date_vade_bas_tarihi->MALI_YIL_KISITI_YOK( true );
    m_ui->adak_date_vade_bts_tarihi->MALI_YIL_KISITI_YOK( true );

    m_ui->adak_date_vade_bas_tarihi->SET_DATE( MALI_YIL_FIRST_DATE()  );
    m_ui->adak_date_vade_bts_tarihi->SET_DATE( MALI_ARRAY_LAST_DATE() );

    SET_HELP_PAGE( "cek-senet-islemleri");

    SET_FIRST_FOCUS_WIDGET( m_ui->combobox_cekin_durumu );

    if ( E9_KULLANICI_SIRKET_DEGISTIRE_BILIR_MI() EQ HAYIR ) {
        m_ui->combobox_sirket_filtresi->setHidden(true);
        m_ui->label_sirket->setHidden(true);
    }

    if (m_cek_mi_senet_mi EQ ENUM_CEK OR m_cek_mi_senet_mi EQ ENUM_HEPSI ) {
        SET_HEADERS(QStringList()<<tr ( "Çek Id" )<<tr ( "Portföy No" )<<tr("Çek Numarası")<<tr ( "Vade Tarihi" )<<tr ( "Düzenleme Tarihi" )<<
                    tr ( "Banka İsmi")<<tr ( "Şube İsmi" )<<tr("Hesap No" )<<tr("Tutar")<<tr ( "Para Birimi" ) );
    }
    else {
        SET_HEADERS ( QStringList()<<tr ( "Senet Id")<<tr ( "Portföy  Numarası" )<<tr ( "Senet Numarası" )<<tr ( "Vade Tarihi" )
                                   <<tr ( "Düzenleme Tarihi" )<<tr("Tutar") << tr( "Para Birimi" ) );
    }

    SET_ENTER_KEY_FOR_RUN_BATCH ( true );

    if ( m_cek_mi_senet_mi EQ ENUM_CEK OR m_cek_mi_senet_mi EQ ENUM_HEPSI ) {

        m_ui->tableWidget->setColumnWidth( 1, 110 );
        m_ui->tableWidget->setColumnWidth( 2, 110 );
        m_ui->tableWidget->setColumnWidth( 3, 140 );
        m_ui->tableWidget->setColumnWidth( 4, 130 );
        m_ui->tableWidget->setColumnWidth( 5, 110 );
        m_ui->tableWidget->setColumnWidth( 6, 110 );
        m_ui->tableWidget->setColumnWidth( 7, 110 );
        m_ui->tableWidget->setColumnWidth( 8, 80  );
        m_ui->tableWidget->setColumnWidth( 9, 70  );
        SET_SETTING_NAME( "CEK_SENET_ARAMA_CEK" );

        if ( m_cek_mi_senet_mi EQ ENUM_CEK ) {
             SET_FIND_BUTTON_NAME ( tr ( "Çekleri Bul" ) );
             SET_PAGE_TITLE ( tr ( "ÇEK-SNT - ÇEK ARAMA" ) );
        }
        else { //Hepsi
            SET_FIND_BUTTON_NAME ( tr ( "Bul" ) );
            SET_PAGE_TITLE ( tr ( "ÇEK-SNT - ÇEK / SENET ARAMA" ) );
        }

        if ( m_musteri_cek_senedi_mi EQ 1 ) {
             GET_BANKALAR(m_ui->combobox_banka_ismi );
        }
        else {
            QStringList kullanilan_banka_list = BNK_GET_BANKA_ISIMLERI();
            for( int i = 0; i < kullanilan_banka_list.size(); i++ ) {
                m_ui->combobox_banka_ismi->addItem ( kullanilan_banka_list.at( i ) );
            }
            m_ui->combobox_banka_ismi->setCurrentIndex(-1);
        }
    }
    else { //senetler
        m_ui->tableWidget->setColumnWidth( 1, 130 );
        m_ui->tableWidget->setColumnWidth( 2, 180 );
        m_ui->tableWidget->setColumnWidth( 3, 200 );
        m_ui->tableWidget->setColumnWidth( 4, 200 );
        m_ui->tableWidget->setColumnWidth( 5, 140 );
        m_ui->tableWidget->setColumnWidth( 6, 70  );

        SET_SETTING_NAME( "CEK_SENET_ARAMA_SENET" );

        SET_FIND_BUTTON_NAME( tr ( "Senetleri Bul" ) );
        SET_PAGE_TITLE( tr ( "ÇEK-SNT - SENET ARAMA" ) );

        m_ui->banka_bilgisi_frame->hide     ();
        m_ui->checkbox_banka_bilgisi->hide  ();
    }

    m_ui->tableWidget->hideColumn( 0 );

    m_ui->date_frame->setEnabled( false );
    m_ui->banka_bilgisi_frame->setEnabled( false );
    m_ui->cekin_durumu_frame->setEnabled( false );

    if ( m_aranacak_cek_senet_turu EQ ENUM_CEK_IADE_ALINDI_BANKADAN OR
        m_aranacak_cek_senet_turu EQ ENUM_CEK_ODENDI_HESAPTAN) {
        m_ui->checkbox_banka_bilgisi->hide();
        m_ui->banka_bilgisi_frame->hide();
    }

    if ( m_aranacak_cek_senet_turu NE ENUM_CEK_TAHSIL_EDILDI_HESABA AND
         m_aranacak_cek_senet_turu NE ENUM_CEK_TAHSIL_EDILDI_NKT AND
         m_aranacak_cek_senet_turu NE ENUM_CEK_IADE_ALINDI_BANKADAN ) {

       m_ui->cekin_durumu_frame->hide();
       m_ui->checkbox_cekin_durumu->hide();
    }
    else {
       FILL_COMBOBOX_CEKIN_DURUMU();
    }
}

/**************************************************************************************
                   CEK_SENET_ARAMA::CHECK_VAR
***************************************************************************************/

int CEK_SENET_ARAMA::CHECK_VAR(QObject * object)
{
    if ( object EQ m_ui->checkbox_vade_tarihi ) {

        if ( m_ui->checkbox_vade_tarihi->isChecked() EQ true ) {
            m_ui->date_frame->setEnabled(true);
            m_ui->adak_date_vade_bas_tarihi->SET_DATE(MALI_YIL_FIRST_DATE());
            SET_FOCUS( m_ui->adak_date_vade_bas_tarihi);
        }
        else {
            m_ui->date_frame->setEnabled(false);
        }
    }
    else if ( object EQ  m_ui->checkbox_banka_bilgisi ) {

        if ( m_ui->checkbox_banka_bilgisi->isChecked() EQ true ) {
            m_ui->banka_bilgisi_frame->setEnabled ( true );
            SET_FOCUS( m_ui->combobox_banka_ismi );
        }
        else {
            m_ui->banka_bilgisi_frame->setEnabled ( false );
            m_ui->combobox_banka_ismi->setCurrentIndex(-1);
        }
    }
    else if ( object EQ m_ui->checkbox_cekin_durumu ) {

        if ( m_ui->checkbox_cekin_durumu->isChecked() EQ true ) {
            m_ui->cekin_durumu_frame->setEnabled ( true );
            m_ui->combobox_cekin_durumu->setCurrentIndex(-1);
            SET_FOCUS( m_ui->combobox_cekin_durumu );
        }
        else {
            m_ui->cekin_durumu_frame->setEnabled ( false );
            m_ui->combobox_cekin_durumu->setCurrentIndex(-1);
        }
    }
    return ADAK_OK;
}

/**************************************************************************************
                   CEK_SENET_ARAMA::CHECK_RUN
***************************************************************************************/

int CEK_SENET_ARAMA::CHECK_RUN()
{
    if ( m_ui->checkbox_cekin_durumu->isChecked() EQ true) {
        if ( m_ui->combobox_cekin_durumu->currentIndex() EQ -1) {
             MSG_WARNING( tr ( "Aranacak çek/senedin durumunu seçmelisiniz1.."), m_ui->combobox_cekin_durumu );

             return ADAK_FAIL;
        }
    }
    if  ( m_ui->checkbox_banka_bilgisi->isChecked() EQ true ) {
        if ( m_ui->combobox_banka_ismi->currentIndex() EQ -1 ) {
             MSG_WARNING( tr ( "Banka İsmini seçmelisiniz!.."), m_ui->combobox_banka_ismi );

             return ADAK_FAIL;
        }
    }
    return ADAK_OK;
}

/**************************************************************************************
                   CEK_SENET_ARAMA::SEARCH
***************************************************************************************/

void CEK_SENET_ARAMA::SEARCH()
{
    SQL_QUERY f_query( DB );

    f_query.PREPARE_SELECT ( "cek_cekler_senetler",
                             "cek_senet_id, portfoy_numarasi, cek_senet_numarasi, vade_tarihi, keside_tarihi, "
                             "cek_senet_tutari, banka_ismi, sube_ismi, hesap_no_id, "
                             "musteri_hesap_numarasi,musteri_ceki_mi,doviz_id,cek_senet_turu", "" );

    QStringList uygun_cekler_list;

    QString vade_bas_tarihi = m_ui->adak_date_vade_bas_tarihi->DATE();
    QString vade_bts_tarihi = m_ui->adak_date_vade_bts_tarihi->DATE();

    if ( m_para_birimi_id > 0 ) {
        f_query.AND_EKLE ("doviz_id = :doviz_id");
        f_query.SET_VALUE(":doviz_id",m_para_birimi_id);
    }

    switch (m_aranacak_cek_senet_turu) { //CEK ARAMA CARI ODEME FISINDEN CAGIRILMISSA
        case ENUM_KENDI_CEKIMIZ         :
        case ENUM_KENDI_SENEDIMIZ       :
            f_query.AND_EKLE ("musteri_ceki_mi = :musteri_ceki_mi AND cek_senet_son_durumu = :cek_senet_son_durumu");
            f_query.SET_VALUE(":musteri_ceki_mi"     ,0);
            f_query.SET_VALUE(":cek_senet_son_durumu",ENUM_PORTFOYDE);
            break;
        case ENUM_MUSTERI_CEKI:
        case ENUM_MUSTERI_SENEDI:
            f_query.AND_EKLE ("musteri_ceki_mi = :musteri_ceki_mi AND cek_senet_son_durumu = :cek_senet_son_durumu");
            f_query.SET_VALUE(":musteri_ceki_mi"     ,1);
            f_query.SET_VALUE(":cek_senet_son_durumu",ENUM_PORTFOYDE);
            break;
        default :
            break;
    }

    if ( m_ui->checkbox_vade_tarihi->isChecked() EQ true ) {
        f_query.AND_EKLE ("vade_tarihi BETWEEN :baslangic_tarihi AND :bitis_tarihi");
        f_query.SET_VALUE(":baslangic_tarihi", vade_bas_tarihi );
        f_query.SET_VALUE(":bitis_tarihi",     vade_bts_tarihi );
    }

    if ( m_ui->checkbox_banka_bilgisi->isChecked() EQ true) {
        f_query.AND_EKLE ("banka_ismi = :banka_ismi" );
        f_query.SET_VALUE(":banka_ismi", m_ui->combobox_banka_ismi->currentText());
    }
     
    if( m_ui->checkbox_cekin_durumu->isChecked() EQ true ) {
        f_query.AND_EKLE ("cek_senet_son_durumu = :cek_senet_durumu" );
        f_query.SET_VALUE(":cek_senet_son_durumu",CEK_GET_CEK_SENET_POZISYONU_ENUM(m_ui->combobox_cekin_durumu->currentText()));
    }

    if ( m_aranacak_cek_senet_turu NE ENUM_KENDI_CEKIMIZ AND
         m_aranacak_cek_senet_turu NE ENUM_MUSTERI_CEKI AND
         m_aranacak_cek_senet_turu NE ENUM_KENDI_SENEDIMIZ AND
         m_aranacak_cek_senet_turu NE ENUM_MUSTERI_SENEDI) {   //CEK ARAMA,CEK BORDROSUNDAN CAGIRILMISSA

        //Çek arama çek/senet bordrosundaki search_editten çağrılmıştır.O yüzden bordrodaki işlem türünde bulunabilecek  çekler/senetler
        //aranır ve uygun olanlar listelenir.

        int bordro_islem_turu = m_aranacak_cek_senet_turu;

        switch (bordro_islem_turu) {

            case ENUM_CEK_BNK_TAHSILATA_VERILDI :
            case ENUM_CEK_KARSILIKSIZ           :
            case ENUM_CEK_PROTESTOLU            :
            case ENUM_CEK_BNK_TEMINATA_VERILDI  :
                f_query.AND_EKLE ("cek_senet_son_durumu = :cek_senet_son_durumu AND musteri_ceki_mi = :musteri_ceki_mi");
                f_query.SET_VALUE(":cek_senet_son_durumu",ENUM_PORTFOYDE);
                f_query.SET_VALUE(":musteri_ceki_mi"     ,EVET);
                break;

            case ENUM_CEK_SATICIYA_VERILDI           :
                f_query.AND_EKLE ("cek_senet_son_durumu = :cek_senet_son_durumu");
                f_query.SET_VALUE(":cek_senet_son_durumu",ENUM_PORTFOYDE);
                if ( m_musteri_cek_senedi_mi EQ 1 )  { //Bordroda secilen cari hesaba ait olmayan cekler listelenir.
                    uygun_cekler_list = GET_UYGUN_CEKLER_LIST(m_hesap_id);
                }
                break;
            case ENUM_CEK_IPTAL                 :
                f_query.AND_EKLE ("cek_senet_son_durumu = :cek_senet_son_durumu AND musteri_ceki_mi = :musteri_ceki_mi");
                f_query.SET_VALUE(":cek_senet_son_durumu",ENUM_PORTFOYDE);
                f_query.SET_VALUE(":musteri_ceki_mi"     ,HAYIR);
                break;
            case ENUM_CEK_TAHSIL_EDILDI_NKT     :
                f_query.AND_EKLE ("(cek_senet_son_durumu = :cek_senet_son_durumu_1 OR "
                                  " cek_senet_son_durumu = :cek_senet_son_durumu_2 OR "
                                  " cek_senet_son_durumu = :cek_senet_son_durumu_3 OR "
                                  " cek_senet_son_durumu = :cek_senet_son_durumu_4 ) AND "
                                  " musteri_ceki_mi = :musteri_ceki_mi");

                f_query.SET_VALUE(":cek_senet_son_durumu_1",ENUM_PORTFOYDE);
                f_query.SET_VALUE(":cek_senet_son_durumu_2",ENUM_KARSILIKSIZ);
                f_query.SET_VALUE(":cek_senet_son_durumu_3",ENUM_PROTESTOLU);
                f_query.SET_VALUE(":cek_senet_son_durumu_4",ENUM_SUPHELI_ALACAK);
                f_query.SET_VALUE(":musteri_ceki_mi",       EVET );

                break;
            case ENUM_CEK_MUS_IADE_EDILDI       :
                f_query.AND_EKLE ("(cek_senet_son_durumu = :cek_senet_son_durumu_1 OR "
                                  " cek_senet_son_durumu = :cek_senet_son_durumu_2 OR "
                                  " cek_senet_son_durumu = :cek_senet_son_durumu_3 OR "
                                  " cek_senet_son_durumu = :cek_senet_son_durumu_4 ) AND "
                                  " musteri_ceki_mi = :musteri_ceki_mi");

                f_query.SET_VALUE(":cek_senet_son_durumu_1", ENUM_PORTFOYDE  );
                f_query.SET_VALUE(":cek_senet_son_durumu_2", ENUM_KARSILIKSIZ);
                f_query.SET_VALUE(":cek_senet_son_durumu_3", ENUM_PROTESTOLU );
                f_query.SET_VALUE(":cek_senet_son_durumu_4", ENUM_SUPHELI_ALACAK);
                f_query.SET_VALUE(":musteri_ceki_mi",        EVET );

                uygun_cekler_list = GET_UYGUN_CEKLER_LIST(m_hesap_id); //Bordroda secilen cari hesaba ait olan cekler listelenir.
                break;
            case ENUM_CEK_TAHSIL_EDILDI_HESABA :
                f_query.AND_EKLE("(cek_senet_son_durumu = :cek_senet_son_durumu_1 OR cek_senet_son_durumu = :cek_senet_son_durumu_2) AND "
                                 "musteri_ceki_mi = :musteri_ceki_mi");

                f_query.SET_VALUE(":cek_senet_son_durumu_1",ENUM_BNK_TAHSILAT_VERILDI);
                f_query.SET_VALUE(":cek_senet_son_durumu_2",ENUM_BNK_TEMINAT_VERILDI);
                f_query.SET_VALUE(":musteri_ceki_mi",       EVET );
                if ( m_musteri_cek_senedi_mi EQ 1 ) {
                   uygun_cekler_list = GET_UYGUN_CEKLER_LIST(m_hesap_id);//Bordroda secilen banka hesabindan tahsil edilebilecek durumda olan cekler listelenir.
                }
                break;
            case ENUM_CEK_ODENDI_HESAPTAN       :
                f_query.AND_EKLE ("cek_senet_son_durumu=:cek_senet_son_durumu AND musteri_ceki_mi=:musteri_ceki_mi AND hesap_no_id=:hesap_no_id");
                f_query.SET_VALUE(":cek_senet_son_durumu",ENUM_SATICIYA_VERILDI);
                f_query.SET_VALUE(":musteri_ceki_mi"     ,HAYIR );
                f_query.SET_VALUE(":hesap_no_id"         ,m_hesap_id);
                if ( m_musteri_cek_senedi_mi EQ 0 ) {
                   uygun_cekler_list = GET_UYGUN_CEKLER_LIST(m_hesap_id); //Bordroda secilen banka hesabina ait olan cekler listelenir.
                }
                break;
            case ENUM_CEK_ODENDI_NKT            :
                f_query.AND_EKLE ("cek_senet_son_durumu = :cek_senet_son_durumu AND musteri_ceki_mi = :musteri_ceki_mi");
                f_query.SET_VALUE(":cek_senet_son_durumu",ENUM_SATICIYA_VERILDI);
                f_query.SET_VALUE(":musteri_ceki_mi"     ,HAYIR );
                break;
            case ENUM_CEK_IADE_ALINDI_SATICIDAN           :
                f_query.AND_EKLE ("cek_senet_son_durumu = :cek_senet_son_durumu");
                f_query.SET_VALUE(":cek_senet_son_durumu",ENUM_SATICIYA_VERILDI);
                uygun_cekler_list = GET_UYGUN_CEKLER_LIST(m_hesap_id); //Bodroda secilen cari hesaba verdigimiz cekler listelenir.
                break;
            case ENUM_CEK_IADE_ALINDI_BANKADAN             :
                f_query.AND_EKLE ("(cek_senet_son_durumu = :cek_senet_son_durumu_1 OR cek_senet_son_durumu = :cek_senet_son_durumu_2 ) AND "
                                  "musteri_ceki_mi = :musteri_ceki_mi");
                f_query.SET_VALUE(":cek_senet_son_durumu_1",ENUM_BNK_TAHSILAT_VERILDI);
                f_query.SET_VALUE(":cek_senet_son_durumu_2",ENUM_BNK_TEMINAT_VERILDI);
                f_query.SET_VALUE(":musteri_ceki_mi",1);
                uygun_cekler_list = GET_UYGUN_CEKLER_LIST(m_hesap_id); //Bordroda secilen banka hesabinda bulunan cekler listelenir.
                break;
            case ENUM_CEK_SUPHELI_ALACAK        :
                f_query.AND_EKLE ("(cek_senet_son_durumu = :cek_senet_son_durumu_1 OR cek_senet_son_durumu = :cek_senet_son_durumu_2 ) AND "
                                  "musteri_ceki_mi = :musteri_ceki_mi");
                f_query.SET_VALUE(":cek_senet_son_durumu_1",ENUM_KARSILIKSIZ);
                f_query.SET_VALUE(":cek_senet_son_durumu_2",ENUM_PROTESTOLU);
                f_query.SET_VALUE(":musteri_ceki_mi",       EVET );
                break;
             case ENUM_CEK_TAHSIL_EDILEMEYEN     :
                f_query.AND_EKLE ("(cek_senet_son_durumu = :cek_senet_son_durumu AND musteri_ceki_mi = :musteri_ceki_mi");
                f_query.SET_VALUE(":cek_senet_son_durumu",ENUM_SUPHELI_ALACAK);
                f_query.SET_VALUE(":musteri_ceki_mi",     EVET );
                break;
             default :
                break;
        };
    }

    if ( m_cek_mi_senet_mi EQ ENUM_HEPSI ) {
        f_query.AND_EKLE ("musteri_ceki_mi=:musteri_ceki_mi");
        f_query.SET_VALUE(":musteri_ceki_mi" , m_musteri_cek_senedi_mi);
    }
    else {
        f_query.AND_EKLE ("cek_senet_turu=:cek_senet_turu");
        f_query.SET_VALUE(":cek_senet_turu", m_cek_mi_senet_mi );
    }

    if ( f_query.SELECT() EQ 0 ) {
        return;
    }
    int current_row;

    while ( f_query.NEXT() EQ true ) {

        //okunan cek senedin turune bakilir.Cek araniyorsa cek,senet araniyorsa senet olmalidir
        if ( f_query.VALUE("cek_senet_turu").toInt() NE m_cek_mi_senet_mi) {
            continue;
        }
        //aranan cek senedin mustericek ve senedi ise yalnizca musteriye ait olan,firma cek senedi ise yalnizca firmaya ait olanlar gelmelidir.
        if ( f_query.VALUE("musteri_ceki_mi").toInt() NE m_musteri_cek_senedi_mi ) {
            continue;
        }

        if ( uygun_cekler_list.size() NE 0) {
            if ( uygun_cekler_list.indexOf(f_query.VALUE(0).toString()) EQ -1) {
                 continue;
            }
        }

        if ( m_cek_mi_senet_mi EQ ENUM_CEK OR m_cek_mi_senet_mi EQ ENUM_HEPSI ) {

            current_row = ADD_NEW_ROW();
            m_ui->tableWidget->item(current_row,0)->setText(f_query.VALUE(0).toString() );

            m_ui->tableWidget->item(current_row,1)->setText ( f_query.VALUE(1).toString() );
            m_ui->tableWidget->item(current_row,2)->setText ( f_query.VALUE(2).toString() );
            m_ui->tableWidget->item(current_row,3)->setText ( f_query.VALUE(3).toDate().toString("dd MMMM yyyy dddd"));
            m_ui->tableWidget->item(current_row,4)->setText ( f_query.VALUE(4).toDate().toString("dd MMMM yyyy dddd"));
            m_ui->tableWidget->item(current_row,5)->setText ( f_query.VALUE(6).toString());
            m_ui->tableWidget->item(current_row,6)->setText ( f_query.VALUE(7).toString());

            int musteri_ceki_mi = f_query.VALUE( "musteri_ceki_mi" ).toInt();

            if ( musteri_ceki_mi EQ EVET ) {
                m_ui->tableWidget->item(current_row,7)->setText ( f_query.VALUE(9).toString());
            }
            else {
                m_ui->tableWidget->item(current_row,7)->setText ( BNK_GET_HESAP_NUMARASI(f_query.VALUE("hesap_no_id").toInt()));
            }
            m_ui->tableWidget->item(current_row,8)->setText ( VIRGUL_EKLE ( f_query.VALUE(5).toString()));
            m_ui->tableWidget->item(current_row,9)->setText (DVZ_GET_DOVIZ_KODU(f_query.VALUE(11).toInt()));
            m_ui->tableWidget->item(current_row,8)->setTextAlignment ( Qt::AlignRight + Qt::AlignVCenter);
        }

        else {
            current_row = ADD_NEW_ROW();

            m_ui->tableWidget->item(current_row,0)->setText( f_query.VALUE(0).toString());
            m_ui->tableWidget->item(current_row,1)->setText( f_query.VALUE(1).toString());
            m_ui->tableWidget->item(current_row,2)->setText( f_query.VALUE(2).toString() );
            m_ui->tableWidget->item(current_row,3)->setText( f_query.VALUE(3).toDate().toString ( "dd MMMM yyyy dddd"));
            m_ui->tableWidget->item(current_row,4)->setText( f_query.VALUE(4).toDate().toString ( "dd MMMM yyyy dddd"));
            m_ui->tableWidget->item(current_row,5)->setText( VIRGUL_EKLE ( f_query.VALUE(5).toString() ) );
            m_ui->tableWidget->item(current_row,6)->setText( DVZ_GET_DOVIZ_KODU(f_query.VALUE(11).toInt() ) );

            m_ui->tableWidget->item ( current_row,5 )->setTextAlignment ( Qt::AlignRight + Qt::AlignVCenter );
        }
    }
}

/**************************************************************************************
                   CEK_SENET_ARAMA::SINGLE_LINE_SELECTED
***************************************************************************************/

int CEK_SENET_ARAMA::SINGLE_LINE_SELECTED ( int selected_row_number )
{
    *m_secilen_id = m_ui->tableWidget->item ( selected_row_number,0 )->text().toInt();
    return ADAK_EXIT;
}

/**************************************************************************************
                   CEK_SENET_ARAMA::FILL_COMBOBOX_CEKIN_DURUMU
***************************************************************************************/

void CEK_SENET_ARAMA::FILL_COMBOBOX_CEKIN_DURUMU()
{
    QStringList cek_status;
    int bordro_islem_turu = m_aranacak_cek_senet_turu;

    switch ( bordro_islem_turu ) {
        case ENUM_CEK_IADE_ALINDI_BANKADAN   :
        case ENUM_CEK_TAHSIL_EDILDI_HESABA   :
            cek_status << tr ( "Bankaya Teminata Verildi" );
            cek_status << tr ( "Bankaya Tahsilata Verildi" );
            break;
        case ENUM_CEK_TAHSIL_EDILDI_NKT   :
            cek_status << tr ( "Portföyde" );
            cek_status << tr ( "Karşılıksız / Protestolu");
            cek_status << tr ( "Şüpheli Alacaklar" );
            break;
        default :
                break;
    }
    m_ui->combobox_cekin_durumu->addItems ( cek_status );
    m_ui->combobox_cekin_durumu->setCurrentIndex(-1);

}


/**************************************************************************************
                   CEK_SENET_ARAMA::GET_UYGUN_CEKLER_LIST
***************************************************************************************/

QStringList CEK_SENET_ARAMA::GET_UYGUN_CEKLER_LIST ( int hesap_id)
{
    QString where_query;
    QStringList uygun_cekler_list;

    SQL_QUERY select_query ( DB );

    switch  ( m_aranacak_cek_senet_turu)
    {
        case ENUM_CEK_SATICIYA_VERILDI :
        case ENUM_MUSTERI_CEKI    :
        case ENUM_MUSTERI_SENEDI  : {

            QList <int> cek_senet_id_list = CARI_GET_CEK_SENET_ID_LIST( -1, CARI_MODULU );

            if ( cek_senet_id_list.size() EQ 0 ) {
                 uygun_cekler_list << QVariant(-1).toString();

                return uygun_cekler_list;
            }
            for ( int i = 0; i < cek_senet_id_list.size(); i++ ) {
                uygun_cekler_list << QVariant ( cek_senet_id_list.at( i )).toString();
            }
            return uygun_cekler_list;

            break;
        }
        case ENUM_CEK_MUS_IADE_EDILDI        :
        case ENUM_CEK_IADE_ALINDI_SATICIDAN :
            where_query = QString ( "cek_bordrolar.bordro_id = cek_bordro_satirlari.bordro_id AND "
                                    "cari_hesap_id = :cari_hesap_id" );

            if ( m_aranacak_cek_senet_turu EQ ENUM_CEK_IADE_ALINDI_SATICIDAN ) {

                where_query += QString  ( " AND bordro_islem_turu = %1" ).arg ( ENUM_CEK_SATICIYA_VERILDI );
            }
            if ( m_aranacak_cek_senet_turu EQ ENUM_CEK_MUS_IADE_EDILDI ) {

                where_query += QString ( " AND bordro_islem_turu = %1" ).arg ( ENUM_CEK_MUSTERIDEN_ALINDI );
            }

            select_query.PREPARE_SELECT ( "cek_bordrolar,cek_bordro_satirlari", "cek_senet_id", where_query );

            select_query.SET_VALUE      ( ":cari_hesap_id",  hesap_id );

            if ( select_query.SELECT() EQ 0 ) {
                 uygun_cekler_list << QVariant(-1).toString();

                return uygun_cekler_list;
            }
            while  ( select_query.NEXT() ) {
                uygun_cekler_list << select_query.VALUE(0).toString();
            }
            return uygun_cekler_list;

            break;
        case ENUM_CEK_TAHSIL_EDILDI_HESABA   :
        case ENUM_CEK_IADE_ALINDI_BANKADAN   :
            where_query = QString ( "cek_bordrolar.bordro_id = cek_bordro_satirlari.bordro_id AND "
                                    "hesap_no_id = :hesap_no_id AND ( bordro_islem_turu = :bordro_islem_turu "
                                    "OR bordro_islem_turu = :bordro_islem_turu_2 )" );

            select_query.PREPARE_SELECT ( "cek_bordrolar,cek_bordro_satirlari", "cek_senet_id", where_query );

            select_query.SET_VALUE      ( ":hesap_no_id"        ,  hesap_id );
            select_query.SET_VALUE      ( ":bordro_islem_turu"  ,  ENUM_CEK_BNK_TAHSILATA_VERILDI );
            select_query.SET_VALUE      ( ":bordro_islem_turu_2",  ENUM_CEK_BNK_TEMINATA_VERILDI );

            if ( select_query.SELECT() EQ 0 ) {
                 uygun_cekler_list << QVariant(-1).toString();

                return uygun_cekler_list;
            }
            while  ( select_query.NEXT() ) {
                uygun_cekler_list << select_query.VALUE(0).toString();
            }
            return uygun_cekler_list;

            break;
        case ENUM_CEK_ODENDI_HESAPTAN :
            where_query = QString ( "cek_senet_son_durumu = :cek_senet_son_durumu AND "
                                    "hesap_no_id = :hesap_no_id " );


            select_query.PREPARE_SELECT ( "cek_cekler_senetler", "cek_senet_id", where_query );

            select_query.SET_VALUE      ( ":cek_senet_son_durumu"   ,  ENUM_SATICIYA_VERILDI );
            select_query.SET_VALUE      ( ":hesap_no_id"            ,  hesap_id );

            if ( select_query.SELECT() EQ 0 ) {
                 uygun_cekler_list << QVariant(-1).toString();

                return uygun_cekler_list;
            }
            while  ( select_query.NEXT() ) {
                uygun_cekler_list << select_query.VALUE(0).toString();
            }
            return uygun_cekler_list;

            break;
    }


    return uygun_cekler_list;
}
