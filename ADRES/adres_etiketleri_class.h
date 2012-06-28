#ifndef ADRES_ETIKETLERI_H
#define ADRES_ETIKETLERI_H

#include "etiket_kernel.h"
#include "etiket_sablonlari.h"

class ADRES_ETIKETLERI : public ETIKET_KERNEL
{


public:

    ADRES_ETIKETLERI ( etiket_sablonlari etiket_sablonu,float yazi_tipi_boyutu);

    QString ETIKET_TABLOLARINI_OLUSTUR ( QStringList P_ADRES_ETIKET_BILGILERI );

private:

    float   m_yazitipi_boyutu;
    float   m_etiket_genisligi;

    QString m_secenek;

    void    ETIKET_OLUSTUR ( QStringList P_ADRES_ETIKET_BILGILERI, QStringList * etiket_list );

};

#endif // ADRES_ETIKETLERI_H
