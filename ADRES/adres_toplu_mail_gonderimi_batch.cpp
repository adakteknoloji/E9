#include <QMessageBox>
#include "adres_toplu_mail_gonderimi_batch_class.h"
#include "adres_toplu_mail_gonderimi_batch_open.h"
#include "ui_adres_toplu_mail_gonderimi_batch.h"
#include "email.h"
#include "yonetim.h"
#include "e9_log.h"

extern ADAK_SQL * DB;
extern ADAK_SQL * G_YONETIM_DB;

/**************************************************************************************
                   OPEN_ADRES_TOPLU_MAIL_GONDERIMI_BATCH
***************************************************************************************/

void OPEN_ADRES_TOPLU_MAIL_GONDERIMI_BATCH ( QStringList P_TOPLU_MAIL_BILGILERI, QWidget * parent )
{
    if ( P_TOPLU_MAIL_BILGILERI.size() EQ 0 ) {
        ADAK_INFO( QObject::tr ( "Gönderilecek email adresi bulunamadı"), NULL, parent );
        return;
    }

    ADRES_TOPLU_MAIL_GONDERIMI_BATCH * B = new ADRES_TOPLU_MAIL_GONDERIMI_BATCH ( P_TOPLU_MAIL_BILGILERI, parent );
    B->EXEC( NOT_FULL_SCREEN );
}

/**************************************************************************************
                   ADRES_TOPLU_MAIL_GONDERIMI_BATCH::ADRES_TOPLU_MAIL_GONDERIMI_BATCH
***************************************************************************************/

ADRES_TOPLU_MAIL_GONDERIMI_BATCH::ADRES_TOPLU_MAIL_GONDERIMI_BATCH( QStringList P_TOPLU_MAIL_BILGILERI, QWidget * parent ) :  BATCH_KERNEL ( parent ), m_ui ( new Ui::ADRES_TOPLU_MAIL_GONDERIMI_BATCH )
{
    M_TOPLU_MAIL_BILGILERI = P_TOPLU_MAIL_BILGILERI;

    m_ui->setupUi       ( this );
    START_BATCH_KERNEL  ( this, DB );
}

/**************************************************************************************
                   ADRES_TOPLU_MAIL_GONDERIMI_BATCH::SETUP_FORM
***************************************************************************************/

void ADRES_TOPLU_MAIL_GONDERIMI_BATCH::SETUP_FORM()
{
    SET_HELP_PAGE       ("tadres-toplu-mail-sms_toplu-mail-gonderimi");
    SET_PAGE_TITLE      ( tr ( "ADR - TOPLU MAIL GÖNDERİMİ" ) );
    SET_SETTING_NAME    ( "ADRES_TOPLU_MAIL_GONDERIMI_BATCH" );
    REGISTER_BUTTON_WIDGET          ( m_ui->widget_batch_buttons );

    for ( int i = 0; i < M_TOPLU_MAIL_BILGILERI.size(); i++ ) {
        if ( m_to_email_address.isEmpty() EQ false ) {
            m_to_email_address.append(",");
        }
        if ( m_to_email_address_with_record_id.isEmpty() EQ false ) {
            m_to_email_address_with_record_id.append(",");
        }

        QStringList email_info = M_TOPLU_MAIL_BILGILERI.at(i).split(":",QString::SkipEmptyParts,Qt::CaseInsensitive);

        if (email_info.size() EQ 0) {
            m_to_email_address.append                ( email_info.at(0) );
            m_to_email_address_with_record_id.append ( email_info.at(0) );

        }
        else {
            m_to_email_address.append ( email_info.at(1) );
            m_to_email_address_with_record_id.append ( email_info.at(0) + ":" + email_info.at(1) );
        }
    }

    m_to_email_address.replace("\n","");
    m_to_email_address.replace("\r","");
    m_to_email_address.replace("\t","");
    m_to_email_address.replace(" ","");

    SQL_QUERY yonetim_query ( G_YONETIM_DB );

    m_ui->frame_posta_sunucusu_ayarlari->setEnabled( false );
    yonetim_query.PREPARE_SELECT ( "ynt_posta_sunucusu","email_sunucusu, email_port, email_baglanti_guvenligi " );

    if ( yonetim_query.SELECT() > 0 ) {

        yonetim_query.NEXT();

        m_ui->lineEdit_mail_server->setText ( yonetim_query.VALUE(0).toString());
        m_ui->lineEdit_port->setText        ( yonetim_query.VALUE(1).toString());
        m_ui->comboBox_mail_baglanti_guvenligi->setCurrentIndex (
        m_ui->comboBox_mail_baglanti_guvenligi->findText ( yonetim_query.VALUE(2).toString() ) );
    }

    yonetim_query.PREPARE_SELECT ( "ynt_kullanicilar","email_adresi, email_kullanici_adi","kullanici_id = :kullanici_id" );

    yonetim_query.SET_VALUE ( ":kullanici_id",KULLANICI_ID() );

    if ( yonetim_query.SELECT() > 0 ) {
        yonetim_query.NEXT();
        m_ui->lineEdit_kimden->setText        ( yonetim_query.VALUE(0).toString() );
        m_ui->lineEdit_kullanici_adi->setText ( yonetim_query.VALUE(1).toString() );
    }

    SET_NAME_OF_RUN_BATCH_BUTTON    ( tr ("Mail At") );

    SET_FIRST_FOCUS_WIDGET          ( m_ui->lineEdit_konu );
}

/**************************************************************************************
                   ADRES_TOPLU_MAIL_GONDERIMI_BATCH::CHECK_VAR
***************************************************************************************/

int ADRES_TOPLU_MAIL_GONDERIMI_BATCH::CHECK_VAR ( QObject * p_object )
{
    if ( p_object EQ m_ui->lineEdit_mail_server ) {
        if ( m_ui->lineEdit_mail_server->text().size() > 100 ) {
            MSG_WARNING( tr ( "Posta sunucusu uzunluuğu 100 karakterden uzun olamaz." ), m_ui->lineEdit_mail_server );

            return ADAK_FAIL_UNDO;
        }
    }
    else if ( p_object EQ m_ui->lineEdit_port ) {
         if ( m_ui->lineEdit_port->text().size() > 10 ) {
            MSG_WARNING( tr ( "Port bilgisi uzunluuğu 10 karakterden uzun olamaz." ), m_ui->lineEdit_port );

            return ADAK_FAIL_UNDO;
        }
    }
    return ADAK_OK;
}

/**************************************************************************************
                   ADRES_TOPLU_MAIL_GONDERIMI_BATCH::CHECK_RUN
***************************************************************************************/

int ADRES_TOPLU_MAIL_GONDERIMI_BATCH::CHECK_RUN()
{
    if ( m_ui->lineEdit_kimden->text().isEmpty() EQ true) {
        MSG_WARNING( tr ( "Kullanıcı mail adresi kayıtlı değil. Lütfen sistem menüsü kullanıcılar ekranından giriş yaptığınız kullanıcıya ait email adresini kaydediniz"), m_ui->lineEdit_kimden );

        return ADAK_FAIL;
    }
    if ( m_ui->lineEdit_mail_server->text().isEmpty() EQ true OR m_ui->lineEdit_mail_server->text() EQ tr("Tanımlı Değil")) {
        MSG_WARNING( tr ( "Posta sunucusu tanımlanmamış" ), m_ui->lineEdit_mail_server );

        return ADAK_FAIL;
    }
    if ( m_ui->lineEdit_port->text().isEmpty() EQ true ) {
        MSG_WARNING( tr ( "Port numarası girilmemiş"), m_ui->lineEdit_port );

        return ADAK_FAIL;
    }
    return ADAK_OK;
}

/**************************************************************************************
                   ADRES_TOPLU_MAIL_GONDERIMI_BATCH::RUN_BATCH
***************************************************************************************/

void ADRES_TOPLU_MAIL_GONDERIMI_BATCH::RUN_BATCH()
{

//    QString mail_server    = "mail.harran.edu.tr";
//    int     port           = 587;
//    QString gonderen_mail  = "bilisim@harran.edu.tr";
//    QString alici_mail     = "c.savur@gmail.com";

//    EMAIL_SET_EMAIL_SERVER( mail_server, port, GET_SSL_PROTOCOL_ID( "STARTTLS") );

//    EMAIL_SET_USER_EMAIL_INFO( "bilisim", "isyanatesi5852", "Celal", true );

//    EMAIL_SET_TOPLU_GONDER_FLAG(false);

//    SEND_MAIL ( gonderen_mail, gonderen_mail, alici_mail , "test", "test maili" , E9_PROGRAMI,ADRES_MODULU, "Mesajınız Gönderildi..." );
//    EMAIL_SET_TOPLU_GONDER_FLAG(true);

    EMAIL_SET_EMAIL_SERVER ( m_ui->lineEdit_mail_server->text(), m_ui->lineEdit_port->text().toInt(), GET_SSL_PROTOCOL_ID ( m_ui->comboBox_mail_baglanti_guvenligi->currentText() ) );

    int clicked_button = QMessageBox::information ( this, tr ( "Bilgi" ), tr ( "Gönderilecek email adresleri \n" + m_to_email_address.toUtf8() ) , QMessageBox::Ok, QMessageBox::Cancel );

    if ( clicked_button EQ QMessageBox::Cancel ) {
        return;
    }

    QString kullanici_adi;

    SQL_QUERY query ( G_YONETIM_DB );

    query.PREPARE_SELECT ( "ynt_kullanicilar","kullanici_adi","kullanici_id = :kullanici_id" );

    query.SET_VALUE      ( ":kullanici_id", KULLANICI_ID() );

    if( query.SELECT() NE 0 ){
        query.NEXT();
        kullanici_adi = query.VALUE(0).toString();
    }

    EMAIL_SET_USER_EMAIL_INFO ( m_ui->lineEdit_kullanici_adi->text(), m_ui->lineEdit_parola->text(), kullanici_adi, m_ui->checkBox_parola_kullan->isChecked() );

    if ( m_ui->combo_box_gonderim_sekli->currentIndex() EQ 0 ) {
        EMAIL_SET_TOPLU_GONDER_FLAG(true);
        SEND_MAIL ( m_ui->lineEdit_kimden->text(), kullanici_adi, m_to_email_address_with_record_id , m_ui->lineEdit_konu->text(), m_ui->textEdit_icerik->toPlainText(),E9_PROGRAMI,ADRES_MODULU, "Mesajınız Gönderildi..." );
    }
    else {
        EMAIL_SET_TOPLU_GONDER_FLAG(false);
        SEND_MAIL ( m_ui->lineEdit_kimden->text(), kullanici_adi, m_to_email_address_with_record_id , m_ui->lineEdit_konu->text(), m_ui->textEdit_icerik->toPlainText() , E9_PROGRAMI,ADRES_MODULU, "Mesajınız Gönderildi..." );
        EMAIL_SET_TOPLU_GONDER_FLAG(true);
    }

    QString log_detaylari;
    log_detaylari.append ( " Log Türü: E-MAIL GÖNDERİMİ "    );
    log_detaylari.append ( " # Email Gönderen Adres : "      );
    log_detaylari.append ( m_ui->lineEdit_kimden->text()     );
    log_detaylari.append ( " # Email Gönderilen Adresler : " );
    log_detaylari.append ( m_to_email_address   );
    log_detaylari.append ( " # Email Server: "  );
    log_detaylari.append ( m_ui->lineEdit_mail_server->text()      );
    log_detaylari.append ( " # Email Konusu : " );
    log_detaylari.append ( m_ui->lineEdit_konu->text());
    log_detaylari.append ( " # Email İçeriği : " );
    log_detaylari.append ( m_ui->textEdit_icerik->toPlainText()       );

    DB->START_TRANSACTION();

    SQL_QUERY sql_query ( DB );
    sql_query.PREPARE_UPDATE ( "e9_sabit_degerler","sabit_deger_id","adr_mail_server_adi,"
                               "adr_mail_port,adr_mail_baglanti_guvenligi","" );

    sql_query.SET_VALUE     ( ":adr_mail_server_adi",         m_ui->lineEdit_mail_server->text() );
    sql_query.SET_VALUE     ( ":adr_mail_port",               m_ui->lineEdit_port->text());
    sql_query.SET_VALUE     ( ":adr_mail_baglanti_guvenligi", m_ui->comboBox_mail_baglanti_guvenligi->currentText() );
    sql_query.UPDATE();

    E9_LOG_KAYDI_EKLE(ADRES_MODULU,LOG_ADR_MAIL_GONDERIMI,-1,log_detaylari);

    DB->COMMIT_TRANSACTION();
}
