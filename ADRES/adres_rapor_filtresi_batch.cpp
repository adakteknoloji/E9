#include <QMessageBox>
#include "adres_console_utils.h"
#include "adres_enum.h"
#include "sms.h"
#include "adres_rapor_filtresi_batch_class.h"
#include "adres_rapor_filtresi_batch_open.h"
#include "ui_adres_rapor_filtresi_batch.h"
#include "print.h"
#include "kernel_utils.h"
#include "adres_arama_open.h"
#include "adres_kayitlari_raporu_open.h"
#include "adres_etiketleri_batch_open.h"
#include "adres_toplu_mail_gonderimi_batch_open.h"

#include "adak_gruplar.h"

extern ADAK_SQL * DB;

/**************************************************************************************
                     OPEN_ADRES_RAPOR_FILTRESI_BATCH
***************************************************************************************/

void OPEN_ADRES_RAPOR_FILTRESI_BATCH ( int p_rapor_turu, QWidget * p_parent )
{
    ADRES_RAPOR_FILTRESI_BATCH * F = new ADRES_RAPOR_FILTRESI_BATCH ( p_rapor_turu, p_parent );

    switch ( p_rapor_turu ) {
        default :
            break;
        case TOPLU_MAIL_GONDERIMI :
            if ( ADR_GET_USER_MAIL().isEmpty() EQ true ) {
                ADAK_WARNING( QObject::tr ( "<p ALIGN=CENTER> Kullanıcı mail adresi kayıtlı değil. <br> Lütfen <b>Sistem->Kullanıcılar</b> ekranından, giriş yaptığınız kullanıcıya ait email adresini kaydediniz.  </p>" ), NULL, p_parent );
                return ;
            }
            break;
    }

    F->EXEC( NOT_FULL_SCREEN );
}

/**************************************************************************************
                     ADRES_RAPOR_FILTRESI_BATCH::ADRES_RAPOR_FILTRESI_BATCH
***************************************************************************************/

ADRES_RAPOR_FILTRESI_BATCH::ADRES_RAPOR_FILTRESI_BATCH ( int p_rapor_turu, QWidget * p_parent ) :  BATCH_KERNEL ( p_parent ), m_ui ( new Ui::ADRES_RAPOR_FILTRESI_BATCH )
{
    m_adres_rapor_turu = p_rapor_turu;

    m_ui->setupUi      ( this );
    START_BATCH_KERNEL ( this, DB );
}

/**************************************************************************************
                     ADRES_RAPOR_FILTRESI_BATCH::SETUP_FORM
***************************************************************************************/

void ADRES_RAPOR_FILTRESI_BATCH::SETUP_FORM ()
{
    //SET_HELP_PAGE        ("adres-raporlari");
    SET_SETTING_NAME     ( "ADRES_RAPOR_FILTRESI_BATCH" );

    switch ( m_adres_rapor_turu ) {
        case ADRES_ILETISIM_RAPORU :
        default :
            SET_PAGE_TITLE ( tr ( "ADR - TELEFON / WEB / E-POSTA KAYITLARI" ) );
            SET_NAME_OF_RUN_BATCH_BUTTON ( tr ("Rapor Al" ) );
            SET_HELP_PAGE        ("adres-toplu-mail-sms_telefonwebe-posta-kayitlari");
            break;
        case ADRES_ETIKETLERI_RAPORU :
            SET_PAGE_TITLE ( tr ( "ADR - ADRES ETİKETLERİ YAZDIR" ) );
            SET_NAME_OF_RUN_BATCH_BUTTON ( tr ( "Etiket Yazdır" ) );
            SET_HELP_PAGE("adres-toplu-mail-smsm_adres-etiketleri-yazdir");
            break;
        case TOPLU_MAIL_GONDERIMI :
            SET_PAGE_TITLE ( tr ( "ADR - TOPLU MAIL GÖNDERİMİ" ) );
            SET_NAME_OF_RUN_BATCH_BUTTON ( tr ( "Mail Gönder" ) );
            SET_HELP_PAGE("adres-toplu-mail-sms_toplu-mail-gonderimi");
            break;
        case TOPLU_SMS_GONDERIMI:
            SET_PAGE_TITLE ( tr ( "ADR - TOPLU SMS GÖNDERİMİ" ) );
            SET_NAME_OF_RUN_BATCH_BUTTON ( tr ( "Kayıtları Seç" ) );
            SET_HELP_PAGE("adres-toplu-mail-sms_toplu-sms-gonderimi");
            break;
        case ADRES_BILGILERI_RAPORU :
            SET_PAGE_TITLE ( tr ( "ADR - ADRES KAYITLARI" ) );
            SET_NAME_OF_RUN_BATCH_BUTTON ( tr ("Rapor Al" ) );
            SET_HELP_PAGE        ("Adres-toplu-mail-sms_adres-kayitlari");
            break;
    }
    REGISTER_BUTTON_WIDGET ( m_ui->widget_batch_buttons );

    m_ui->frame_gruplar->setEnabled ( false );
    m_ui->frame_adres_kayitlari->setEnabled ( false );

    m_ui->radioButton_firma_sahis->setChecked ( true );
    m_adres_kayit_turu = SAHIS_ARTI_FIRMA;

    m_ui->tableWidget_adres_kayitlari->hideColumn(0);

    m_ui->radioButton_firma_sahis->setEnabled( true );
    SET_FIRST_FOCUS_WIDGET( m_ui->radioButton_firma_sahis );
}

/**************************************************************************************
                     ADRES_RAPOR_FILTRESI_BATCH::CHECK_VAR
***************************************************************************************/

int ADRES_RAPOR_FILTRESI_BATCH::CHECK_VAR ( QObject * p_object )
{

    SQL_QUERY sql_query ( DB );

    if ( p_object EQ m_ui->radioButton_firmalar ) {
        if ( m_ui->radioButton_firmalar->isChecked() EQ true ) {
            m_adres_kayit_turu = FIRMA_KAYDI;
        }
    }
    else if ( p_object EQ m_ui->radioButton_sahislar ) {
         if ( m_ui->radioButton_sahislar->isChecked() EQ true ) {
             m_adres_kayit_turu = SAHIS_KAYDI;
        }
    }
    else if ( p_object EQ m_ui->radioButton_firma_sahis ) {
         if ( m_ui->radioButton_firma_sahis->isChecked() EQ true ) {
            m_adres_kayit_turu = SAHIS_ARTI_FIRMA;
        }
    }
    else if ( p_object EQ m_ui->checkBox_grup_secimi ) {
         if ( m_ui->checkBox_grup_secimi->isChecked() EQ true ) {
            m_ui->frame_gruplar->setEnabled ( true );
            SET_FOCUS( m_ui->button_grup_ekle );
        }
        else {
            m_ui->frame_gruplar->setEnabled ( false );
            m_ui->textEdit_gruplar->clear();
        }
    }
    else if ( p_object EQ m_ui->checkBox_adres_kaydi_secimi ) {
         if ( m_ui->checkBox_adres_kaydi_secimi->isChecked() EQ true ) {
           m_ui->frame_adres_kayitlari->setEnabled ( true );
           SET_FOCUS( m_ui->button_adres_kaydi_ekle );
        }
        else {
            m_ui->frame_adres_kayitlari->setEnabled ( false );
        }
    }
    else if ( p_object EQ m_ui->button_grup_ekle ) {


        if ( OPEN_GRUP_SECIM( E9_PROGRAMI,ADRES_MODULU , &m_grup_idleri , DB , this , true) EQ true ) {
            m_ui->textEdit_gruplar->setText ( GRP_GRUP_ADLARINI_BUL(&m_grup_idleri , new QStringList() , new QStringList()) );
        }

    }

    else if ( p_object EQ m_ui->button_adres_kaydi_ekle ) {

        QList<int> adres_id = OPEN_ADRES_ARAMA_LIST( m_adres_kayit_turu,true, this );

        if ( adres_id.size() EQ 0 ) {
            return ADAK_OK;
        }

        QString adres_id_str = "";

        for ( int i = 0 ; i < adres_id.size() ; i++ ) {
            if ( adres_id_str.isEmpty() EQ false ) {
                adres_id_str.append(" OR ");
            }

            adres_id_str.append(QString(" adres_id=" + QVariant(adres_id.at(i)).toString()));
        }

        sql_query.PREPARE_SELECT ( "adr_adresler","firma_sahis_adi,email,adres_id",adres_id_str );

        if ( sql_query.SELECT() EQ 0 ) {
            ADAK_WARNING( tr ( "Seçili kayıt bulunamadı"), NULL, this );
            return ADAK_FAIL;
        }

        bool email_bulundu           = false;
        bool table_widget_temizlendi = false;

        while ( sql_query.NEXT() EQ true ) {
            if ( m_adres_rapor_turu EQ TOPLU_MAIL_GONDERIMI ) {
                if (sql_query.VALUE(1).toString().isEmpty() EQ true ) {
                    SQL_QUERY query (DB);
                    query.PREPARE_SELECT("adr_emailler","email_adresi","gizli_kayit=:gizli_kayit AND adres_kayit_id=:adres_kayit_id");
                    query.SET_VALUE(":gizli_kayit"   ,0 );
                    query.SET_VALUE(":adres_kayit_id",sql_query.VALUE(2).toInt());
                    if ( query.SELECT() EQ 0 ) {
                        continue;
                    }
                }
                email_bulundu = true;
            }

            QString firma_sahis_adi = sql_query.VALUE(0).toString();

            if ( table_widget_temizlendi EQ false ) {
                for ( int i = m_ui->tableWidget_adres_kayitlari->rowCount() ; i > 0 ; i-- ) {
                    m_ui->tableWidget_adres_kayitlari->removeRow(i-1);
                }

                table_widget_temizlendi = true;
            }

            m_ui->tableWidget_adres_kayitlari->insertRow ( m_ui->tableWidget_adres_kayitlari->rowCount() );
            m_ui->tableWidget_adres_kayitlari->setItem   ( m_ui->tableWidget_adres_kayitlari->rowCount() -1, 0, new QTableWidgetItem ( QVariant( sql_query.VALUE(2) ).toString()));
            m_ui->tableWidget_adres_kayitlari->setItem   ( m_ui->tableWidget_adres_kayitlari->rowCount() -1, 1, new QTableWidgetItem ( firma_sahis_adi ));
        }

        if ( m_adres_rapor_turu EQ TOPLU_MAIL_GONDERIMI ) {
            if (email_bulundu EQ false ) {
                ADAK_WARNING( tr ( "Seçili kayıtta email adresi bulunamadı."), NULL, this );
                return ADAK_FAIL;
            }
        }
    }
    else if ( p_object EQ m_ui->tableWidget_adres_kayitlari ) {
         if ( p_object->objectName() EQ "ITEM_DOUBLE_CLICKED" ) {
            int clicked_row = m_ui->tableWidget_adres_kayitlari->currentRow();
            m_ui->tableWidget_adres_kayitlari->removeRow ( clicked_row );
        }
    }

    return ADAK_OK;
}

/**************************************************************************************
                     ADRES_RAPOR_FILTRESI_BATCH::CHECK_RUN
***************************************************************************************/

int ADRES_RAPOR_FILTRESI_BATCH::CHECK_RUN()
{
    if ( m_ui->checkBox_grup_secimi->isChecked() EQ true ) {
         if ( m_ui->textEdit_gruplar->toPlainText().isEmpty() EQ true ) {
            MSG_WARNING( tr ( "Grup Seç işaretli ise en az bir grup seçimi yapmalısınz" ), m_ui->button_grup_ekle );

            return ADAK_FAIL;
        }
    }
    if ( m_ui->checkBox_adres_kaydi_secimi->isChecked() EQ true ) {
        if ( m_ui->tableWidget_adres_kayitlari->rowCount() EQ 0 ) {
            MSG_WARNING( tr ( "Kayıt Seç işaretli ise en az bir adres kaydı seçmelisiniz" ), m_ui->button_adres_kaydi_ekle  );

            return ADAK_FAIL;
        }
    }
    return ADAK_OK;
}

/**************************************************************************************
                     ADRES_RAPOR_FILTRESI_BATCH::RUN_BATCH
***************************************************************************************/

void ADRES_RAPOR_FILTRESI_BATCH::RUN_BATCH()
{

    switch ( m_adres_rapor_turu ) {
        case ADRES_ILETISIM_RAPORU :
        default                 :
            if ( RAPOR_VERILERINI_OLUSTUR() EQ false ) {
                return;
            }
            OPEN_REPORT_SHOWER ( OPEN_ADRES_KAYITLARI_RAPORU ( ADRES_ILETISIM_RAPORU, m_adres_kayit_turu, M_ADRES_KAYIT_RAPORU_BILGILERI), nativeParentWidget() );
            break;
        case ADRES_BILGILERI_RAPORU:
            if ( RAPOR_VERILERINI_OLUSTUR() EQ false ) {
                return;
            }
            OPEN_REPORT_SHOWER ( OPEN_ADRES_KAYITLARI_RAPORU ( ADRES_BILGILERI_RAPORU,m_adres_kayit_turu, M_ADRES_ETIKET_BILGILERI ), nativeParentWidget() );
            break;
        case ADRES_ETIKETLERI_RAPORU :
            if ( RAPOR_VERILERINI_OLUSTUR() EQ false ) {
                return;
            }
            OPEN_ADRES_ETIKETLERI_BATCH ( M_ADRES_ETIKET_BILGILERI, this );
            break;

        case TOPLU_MAIL_GONDERIMI :
            if ( RAPOR_VERILERINI_OLUSTUR() EQ false ) {
                return;
            }
            if ( M_TOPLU_MAIL_BILGILERI.isEmpty() EQ true ) {
                MSG_ERROR ( "E-Posta gönderilebilecek kayıt bulunamadı." , NULL );
                return;
            }
            OPEN_ADRES_TOPLU_MAIL_GONDERIMI_BATCH ( M_TOPLU_MAIL_BILGILERI, this );
            break;
        case TOPLU_SMS_GONDERIMI:
            if ( RAPOR_VERILERINI_OLUSTUR() EQ false ) {
                return;
            }
            if ( M_SMS_BILGILERI.isEmpty() EQ true ) {
                MSG_ERROR ( "SMS gönderilebilecek kayıtlı bir cep telefon bulunamadı." , NULL );
                return;
            }
            else {
                MSG_INFO (QString("SMS Gönderilecek Numaralar:\n%1").arg(M_SMS_BILGILERI),NULL);
            }

            SHOW_SMS_GUI("",M_SMS_BILGILERI,"",this);
            break;
    }
}

/**************************************************************************************
                     ADRES_RAPOR_FILTRESI_BATCH::RAPOR_VERILERINI_OLUSTUR
***************************************************************************************/

bool ADRES_RAPOR_FILTRESI_BATCH::RAPOR_VERILERINI_OLUSTUR ()
{

    M_ADRES_KAYIT_RAPORU_BILGILERI.clear();
    M_ADRES_ETIKET_BILGILERI.clear();
    M_TOPLU_MAIL_BILGILERI.clear();
    M_SMS_BILGILERI.clear();

    QList<int>  kayit_id_listesi;
    QStringList ADRES_KAYIT_LISTESI;

    SQL_QUERY second_query ( DB );


    if ( m_ui->checkBox_grup_secimi->isChecked() EQ true ) {
        kayit_id_listesi.clear();
        kayit_id_listesi = GRP_GRUBA_EKLI_KAYITLARI_BUL( E9_PROGRAMI , ADRES_MODULU , m_grup_idleri  );
        if ( kayit_id_listesi.isEmpty() EQ true ) {
            MSG_ERROR("Gruba ekli kayıt bulunamadı.",NULL);
            return false;
        }
    }

    SQL_QUERY sql_query ( DB );

    QString where_str = "";

    if ( m_ui->checkBox_adres_kaydi_secimi->isChecked() EQ true ) {

        int   listedeki_kayit_sayisi = m_ui->tableWidget_adres_kayitlari->rowCount();

        for ( int i = 0; i < listedeki_kayit_sayisi; i++ ) {
            ADRES_KAYIT_LISTESI.append ( m_ui->tableWidget_adres_kayitlari->item ( i, 0 )->text());
        }

        if ( where_str.isEmpty() EQ false ) {
            where_str.append (  " OR " );
        }

        where_str.append (  "adres_id = :adres_id_0 " );

        if ( listedeki_kayit_sayisi > 1 ) {
            for ( int i = 1 ; i < listedeki_kayit_sayisi ; i++ ) {
                where_str.append(" OR adres_id = :adres_id_"+ QVariant(i).toString());
            }
        }
    }

    if ( m_ui->checkBox_grup_secimi->isChecked() EQ true ) {
        for ( int i = 0; i < kayit_id_listesi.size() ; i++ ) {
            if ( where_str.isEmpty() EQ false ) {
                where_str.append (  " OR " );
            }

            where_str.append ( QString(" adres_id = %1").arg(QVariant(kayit_id_listesi.at(i)).toString()));
        }
    }

    if ( m_adres_kayit_turu NE SAHIS_ARTI_FIRMA ) {

        if ( where_str.isEmpty() EQ false ) {
            where_str.append (  " AND " );
        }

        where_str.append (  "adres_kart_turu = :adres_kart_turu ");
    }

    sql_query.PREPARE_SELECT ("adr_adresler","adres_id, firma_sahis_adi, adres_satiri_1, adres_satiri_2, adres_satiri_3,"
                              "ilce, semt, ulke, sehir, website, email, tel_ulke_kodu, "
                              "tel_alan_kodu, telefon,adres_kart_turu,cep_tel_ulke_kodu,"
                              "cep_tel_gsm_kodu,cep_telefonu,is_tel_ulke_kodu,"
                              "is_tel_alan_kodu,is_telefonu", where_str );

    if ( ADRES_KAYIT_LISTESI.size() > 0 ) {

        sql_query.SET_VALUE ( ":adres_id_0", ADRES_KAYIT_LISTESI.at(0) );
        for ( int i = 1; i < ADRES_KAYIT_LISTESI.size(); i++ ) {
            sql_query.SET_VALUE ( ":adres_id_" + QVariant(i).toString(), ADRES_KAYIT_LISTESI.at (i) );
        }
    }

    if ( m_adres_kayit_turu NE SAHIS_ARTI_FIRMA ) {
        sql_query.SET_VALUE ( ":adres_kart_turu", m_adres_kayit_turu );
    }

    if ( sql_query.SELECT("firma_sahis_adi") EQ 0 ) {
        return true;
    }

    QString telefon_bilgisi_1;
    QString telefon_bilgisi_2;
    QString cep_telefonu;

    while ( sql_query.NEXT() EQ true ) {

        telefon_bilgisi_1.clear();
        telefon_bilgisi_1 = sql_query.VALUE ( "tel_ulke_kodu" ).toString() + "-" +
                          sql_query.VALUE ( "tel_alan_kodu" ).toString() + "-" +
                          sql_query.VALUE ( "telefon" ).toString();

        telefon_bilgisi_2.clear();
        telefon_bilgisi_2 = sql_query.VALUE ( "is_tel_ulke_kodu" ).toString() + "-" +
                sql_query.VALUE ( "is_tel_alan_kodu" ).toString() + "-" +
                sql_query.VALUE ( "is_telefonu" ).toString();

        cep_telefonu.clear();

        cep_telefonu = sql_query.VALUE ( "cep_tel_ulke_kodu" ).toString() + "-" +
                sql_query.VALUE ( "cep_tel_gsm_kodu" ).toString() + "-" +
                sql_query.VALUE ( "cep_telefonu" ).toString();



        if ( sql_query.VALUE("cep_telefonu").toString().isEmpty() EQ false AND
             sql_query.VALUE("cep_tel_gsm_kodu").toString().isEmpty() EQ false AND
             sql_query.VALUE("cep_tel_ulke_kodu").toString().isEmpty() EQ false) {
            if ( M_SMS_BILGILERI.isEmpty() EQ false ) {
                M_SMS_BILGILERI.append(",");
            }

            M_SMS_BILGILERI .append(QString( sql_query.VALUE("cep_tel_ulke_kodu").toString() + sql_query.VALUE("cep_tel_gsm_kodu").toString() +
                                   sql_query.VALUE("cep_telefonu").toString()));
        }

        QString adres_kayit_raporu_satiri = sql_query.VALUE ( "firma_sahis_adi" ).toString() + "\t" +
                                            sql_query.VALUE ( "website" ).toString() + "\t" + sql_query.VALUE ( "email" ).toString() + "\t" + telefon_bilgisi_1
                + "\t" + telefon_bilgisi_2 + "\t" + cep_telefonu;

        M_ADRES_KAYIT_RAPORU_BILGILERI.append ( adres_kayit_raporu_satiri );

        QString adres_etiketleri_satiri = sql_query.VALUE ( "firma_sahis_adi" ).toString() + "\t" +
                                          sql_query.VALUE ( "adres_satiri_1" ).toString() + "\t" +
                                          sql_query.VALUE ( "adres_satiri_2" ).toString() + "\t" +
                                          sql_query.VALUE ( "adres_satiri_3" ).toString() + "\t" +
                                          sql_query.VALUE ( "semt" ).toString() + "\t" +
                                          sql_query.VALUE ( "ilce" ).toString() + "\t" +
                                          sql_query.VALUE ( "sehir" ).toString() + "\t" +
                                          sql_query.VALUE ( "ulke" ).toString() + "\t" +
                                          sql_query.VALUE ( "email" ).toString() + "\t";

        //int         adres_kart_turu =   sql_query.VALUE("adres_kart_turu").toInt();

        if ( M_ADRES_ETIKET_BILGILERI.contains(adres_etiketleri_satiri) EQ false ) {
            M_ADRES_ETIKET_BILGILERI.append ( adres_etiketleri_satiri );
        }


        second_query.PREPARE_SELECT ( "adr_emailler","adres_kayit_id,email_adresi","adres_kayit_id = :adres_kayit_id" );
        second_query.SET_VALUE ( ":adres_kayit_id", sql_query.VALUE( "adres_id" ).toInt() );
        if ( second_query.SELECT() EQ 0 ) {
            continue;
        }
        while ( second_query.NEXT() EQ true ) {
            if( second_query.VALUE(0).toString().isEmpty() EQ false AND second_query.VALUE(1).toString().isEmpty() EQ false ) {
                M_TOPLU_MAIL_BILGILERI.append ( QString(second_query.VALUE(0).toString() + ":" + second_query.VALUE(1).toString()) );
            }
        }
    }

    return true;
}
