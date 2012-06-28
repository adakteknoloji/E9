#ifndef PRS_KARTI_EXPORT_H
#define PRS_KARTI_EXPORT_H

#include "export_kernel.h"

class PRS_KARTI_EXPORT : public EXPORT_KERNEL
{
public:

    PRS_KARTI_EXPORT             ( QList <int> cari_hesap_id_list , QWidget * parent , QByteArray text_codec = "UTF-8" );

private:
    void           EXPORT_ROWS              ();

    QList <int>    m_cari_hesap_id_list;

    QByteArray     m_text_codec ;


};

#endif // PRS_KARTI_EXPORT_H
