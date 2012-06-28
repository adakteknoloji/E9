#ifndef SMS_H
#define SMS_H

#include <QString>
#include <QWidget>

void SEND_SMS       (const QString from_gsm_no, const QString to_gms_no, const QString message);
void SHOW_SMS_GUI   (const QString &from, const QString &to, const QString &message, QWidget * parent);

//Burada sms server bilgileri setlenir. id olarak tanimlanan degisken genellikle kullanici adi ve passwordun yaninda
//Ekstra dan kanal kodu,sirket kodu vs gibisinde bir kod verilir. Bu kod id olarak gonderilmelidir.
void SET_SMS_USER   (const QString username,const QString password,const QString id );

#endif // end of SMS_H
