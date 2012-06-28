#ifndef FAIZ_TAKSIT_RAPORU_OPEN_H
#define FAIZ_TAKSIT_RAPORU_OPEN_H

class QString;

QString F_FAIZ_TAKSIT_RAPORU(double anapara,double aylik_faiz_orani,double toplam_tutar,QList<int> odeme_gunleri,QList<double> odeme_tutarlari, QList< QString > odeme_tarihleri, QString page_title , int p_gunluk_aylik );

#endif // FAIZ_TAKSIT_RAPORU_OPEN_H
