#ifndef STOK_URUN_KARTI_EXPORT_CLASS_H
#define STOK_URUN_KARTI_EXPORT_CLASS_H

#include "export_kernel.h"

class STOK_URUN_KARTI_EXPORT : public EXPORT_KERNEL
{
public:
    STOK_URUN_KARTI_EXPORT( QList <int> urun_id_list , QWidget * parent , QByteArray text_codec = "UTF-8" ) ;

private:
    void EXPORT_ROWS ();

    QList <int>    m_urun_id_list;
    QByteArray     m_text_codec ;

};

#endif // STOK_URUN_KARTI_EXPORT_CLASS_H
