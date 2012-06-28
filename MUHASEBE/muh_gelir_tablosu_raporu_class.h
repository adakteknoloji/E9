#ifndef GELIR_TABLOSU_RAPORU_H
#define GELIR_TABLOSU_RAPORU_H

#include "report_kernel.h"


class GELIR_TABLOSU_RAPORU : public REPORT_KERNEL
{

public:
    GELIR_TABLOSU_RAPORU(struct MUH_RAPOR_VARS *  M_MUH_RV);
    ~GELIR_TABLOSU_RAPORU() {}

private:
    struct MUH_RAPOR_VARS *  M_MUH_RV;

    #define     GELIR_TABLOSU_GRUP_SAYISI        10
    #define     GELIR_TABLOSU_ARRAY_SIZE         56

    struct MUH_GELIR_TABLOSU_HESAPLARI_STRUCT {
        int         hesap_id;
        char        hesap_kodu[4];
        char        hesap_ismi[61];
        double      bitis_borc_bakiyesi;
        double      bitis_alacak_bakiyesi;
    };

    struct MUH_HESAP_GRUPLARI_STRUCT {
       char         hesap_kodu[4];
       char         hesap_ismi[61];

    };


    enum GELIR_TABLOSU_SATIRLARI {
        GRUP_SATIRI      = 0,
        HESAPLAMA_SATIRI = -1,
    };


    int                         m_max_yazdirilabilir_satir_sayisi;
    int                         m_yazdirilan_kayit_sayisi;
    int                         m_gelir_tablosu_hesap_sayisi;
    int                         m_gelir_hesaplari_indis_no;
    int                         m_last_hesap_indisi;

    MUH_GELIR_TABLOSU_HESAPLARI_STRUCT  * GELIR_TABLOSU_HESAPLARI;



    QString             GET_HEADER                              ();
    QString             GET_FOOTER                              ();
    QString             GET_BODY                                ();

    void                GELIR_TABLOSU_OLUSTUR                   ();
    void                GRUBUN_HESAPLARINI_GELIR_TABLOSUNA_EKLE (int p_grup_indis_no , const char * p_grup_kodu);
    void                GRUP_BAKIYESI                           (int p_grup_indis_no , const char * p_grup_kodu);




};

#endif // GELIR_TABLOSU_RAPORU_H
