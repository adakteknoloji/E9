#ifndef ADRES_EMAIL_RAPORU_H
#define ADRES_EMAIL_RAPORU_H

#include "report_kernel.h"

struct EMAIL_RAPOR_STRUCT {
    QString     bas_tarihi;
    QString     bts_tarihi;
    QString     email_adresi;
    QString     kullanici_adi;
};

class ADRES_EMAIL_RAPORU : public REPORT_KERNEL
{
public:
    ADRES_EMAIL_RAPORU                          ( EMAIL_RAPOR_STRUCT * raport_struct , QString rapor_ismi );
    ~ADRES_EMAIL_RAPORU                         () {}

private:

    int                         m_sayfa_numarasi;

    int                         m_max_line_count;

    EMAIL_RAPOR_STRUCT *        m_email_struct;

    void                        CREATE_REPORT_BODY      ();

    QString                     GET_HEADER              ();
    QString                     GET_FOOTER              ();
    QString                     GET_BODY                ();

    QStringList                 m_report_lines;

};

QString GET_EMAIL_LOG_RAPORU(EMAIL_RAPOR_STRUCT * report_struct);

#endif // ADRES_EMAIL_RAPORU_H
