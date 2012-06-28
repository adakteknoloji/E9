#ifndef ADRES_FIHRIST_RAPORU_H
#define ADRES_FIHRIST_RAPORU_H

#include "report_kernel.h"

class ADRES_FIHRIST_RAPORU : public REPORT_KERNEL
{


public:
                                ADRES_FIHRIST_RAPORU                      ( int baslangic_harf_index, int bitis_harf_index, QList<int> gruba_gore_kayit_idleri );
                                ~ADRES_FIHRIST_RAPORU                     () {}

private:

    int                         m_baslangic_harf_index;
    int                         m_bitis_harf_index;

    int                         m_max_line_count;
    int                         m_sayfa_numarasi;
    int                         m_satir_numarasi;
    int                         m_harf_indexi;

    bool                        m_body_basligi_olusturuldu_mu;
    bool                        m_indexteki_harf_yazimi_bitti_mi;

    int                         m_adres_kayitlari_index;

    QList<int>                  m_gruba_gore_kayit_idleri;

    QStringList                 M_ADRES_KAYITLARI;
    QStringList                 M_FIRMA_YETKILILERI;

    QString                     GET_HEADER                          ();
    QString                     GET_FOOTER                          ();
    QString                     GET_BODY                            ();

    QString                     BODY_BASLIGI_OLUSTUR                ();
    void                        BODY_TABLOSU_OLUSTUR                ();
    bool                        ADRES_KAYIT_BILGILERINI_AL          ( QString fihrist_harfi );
    bool                        FIRMA_YETKILI_BILGILERINI_AL        ( int firma_id );
};

#endif // ADRES_FIHRIST_RAPORU_H
