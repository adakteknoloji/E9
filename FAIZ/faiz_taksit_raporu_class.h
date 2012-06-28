#ifndef FAIZ_TAKSIT_RAPORU_H
#define FAIZ_TAKSIT_RAPORU_H

#include "report_kernel.h"


class FAIZ_TAKSIT_RAPORU : public REPORT_KERNEL
{

public:
    FAIZ_TAKSIT_RAPORU(double anapara,double aylik_faiz_orani,double toplam_tutar,QList<int> odeme_gunleri,QList<double> odeme_tutarlari, QList< QString > odeme_tarihleri , QString page_title, int p_gunluk_aylik );
    ~FAIZ_TAKSIT_RAPORU() {}

protected:
    virtual QString             GET_HEADER                      ();
    virtual QString             GET_FOOTER                      ();
    virtual QString             GET_BODY                        ();

private:
    bool                        p_raporu_bitir;

    QString                     text_gunluk_aylik;

    QString                     p_page_title;
    double                      p_anapara;
    double                      p_aylik_faiz_orani;
    double                      p_toplam_tutar;

    QList<int>                  p_odeme_gunleri;
    QList<double>               p_odeme_tutarlari;
    QList< QString >            p_odeme_tarihleri;


    int                         p_taksit_sayisi;
    int                         p_max_line_count;
    int                         p_eklenen_kayit_sayisi;
};



#endif // FAIZ_TAKSIT_RAPORU_H
