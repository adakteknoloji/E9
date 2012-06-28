#ifndef EMAIL_H
#define EMAIL_H

#include <QString>
#include <QWidget>
#include "adak_defines.h"

void SEND_MAIL         ( const QString subject, const QString message_to_send , const int program_id , const int module_id , const QString p_uyari_mesaji, const bool add_to_log = true );
void SEND_MAIL         ( const QString email_adresi_from,const QString gonderenin_gercek_ismi, const QString email_adresi_to,  const QString subject,
                         const QString message_to_send, const int program_id , const int module_id, const QString p_uyari_mesaji, const bool add_to_log = true );

void ADAK_EMAIL_SENDER(const QString &email_adresi_from,const QString &gonderenin_gercek_ismi, const QString &email_adresi_to, 
		        const QString &subject, const QString &message_to_send, QWidget * parent = 0);



/////////  SET FONKSIYONLARI
void         EMAIL_SET_PORT(int port);
void         EMAIL_SET_EMAIL_ADRESI_FROM(QString email_adresi_from);
void         EMAIL_SET_EMAIL_ADRESI_TO(QString email_adresi_to);
void         EMAIL_SET_GONDERENIN_GERCEK_ISMI(QString gonderenin_gercek_ismi);
void         EMAIL_SET_EMAIL_SERVER_ADRESS(QString mail_server_adress);
void         EMAIL_SET_LOGIN_NAME(QString login_name);
void         EMAIL_SET_LOGIN_PASSWORD(QString login_password);
void         EMAIL_SET_PROTOCOL(SSL_PROTOCOL protocol);
void         EMAIL_SET_TOPLU_GONDER_FLAG(bool value);
void         EMAIL_SET_EMAIL_SERVER (QString mail_server_adress, int port, SSL_PROTOCOL protocol);
void         EMAIL_SET_USER_EMAIL_INFO(QString login_name, QString login_password,QString gonderenin_gercek_ismi, bool need_authentication);

/////////  GET_FONKSIYONLARI
QString      EMAIL_GET_EMAIL_ADRESI_FROM();
QString      EMAIL_GET_EMAIL_ADRESI_TO();
QString      EMAIL_GET_GONDERENIN_GERCEK_ISMI();
QString      EMAIL_GET_EMAIL_SERVER_ADRESS();
QString      EMAIL_GET_LOGIN_NAME();
QString      EMAIL_GET_LOGIN_PASSWORD();
SSL_PROTOCOL EMAIL_GET_PROTOCOL();
int          EMAIL_GET_PORT();



/////////  SSL_FONKSIYONLARI
SSL_PROTOCOL GET_SSL_PROTOCOL_ID(const QString protocol);


#endif // end of EMAIL_H
