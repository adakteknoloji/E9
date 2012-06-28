#ifndef ADRES_EXPORT_H
#define ADRES_EXPORT_H

#include "export_kernel.h"

class ADRES_EXPORT : public EXPORT_KERNEL
{
public:
    ADRES_EXPORT             (QList <int> adres_id_list , QWidget * parent , QByteArray text_codec = "UTF-8");


private:
    void           EXPORT_ROWS              ();

    QList <int>    m_adres_id_list;

    QByteArray     m_text_codec ;


};

#endif // ADRES_EXPORT_H
