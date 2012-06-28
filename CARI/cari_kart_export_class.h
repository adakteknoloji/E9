#ifndef CARI_KART_EXPORT_CLASS_H
#define CARI_KART_EXPORT_CLASS_H

#include "export_kernel.h"

class CARI_KART_EXPORT : public EXPORT_KERNEL
{
public:

    CARI_KART_EXPORT( QList <int> cari_hesap_id_list , QWidget * parent , QByteArray text_codec);

private :

    void           EXPORT_ROWS ();

    QList <int>    m_cari_hesap_id_list;

    QByteArray     m_text_codec_name ;
};

#endif // CARI_KART_EXPORT_CLASS_H
