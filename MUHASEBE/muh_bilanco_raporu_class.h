#ifndef BILANCO_RAPORU_H
#define BILANCO_RAPORU_H

#include "report_kernel.h"

struct MUH_RAPOR_VARS;

class BILANCO_RAPORU : public REPORT_KERNEL
{

public:
    BILANCO_RAPORU(struct MUH_RAPOR_VARS *  );
    ~BILANCO_RAPORU(){}


private:
    #define MAX_BILANCO_HESAPLARI_SAYISI         500
    #define BILANCO_SINIFI_SAYISI                5
    #define BILANCO_HESAPLARI_GRUP_SAYISI        38

    struct MUH_RAPOR_VARS * M_MUH_RV;


    struct MUH_BILANCO_HESAPLARI_STRUCT{

        int         hesap_id;
        QString     hesap_kodu;
        QString     hesap_ismi;
        double      baslangic_borc_bakiyesi;
        double      bitis_borc_bakiyesi;
        double      baslangic_alacak_bakiyesi;
        double      bitis_alacak_bakiyesi;
    };

    struct MUH_HESAP_SINIFLARI_STRUCT {
        QString    hesap_kodu;
        QString    hesap_ismi;
    };

    struct MUH_HESAP_GRUPLARI_STRUCT {
        QString    hesap_kodu;
        QString    hesap_ismi;
    };

    enum BILANCO_SATIRLARI {
        HESAP_SINIF_SATIRI = 0,
        HESAP_GRUP_SATIRI  = -1,
        TOPLAM_SATIRI      = -2
    };


    int                         m_max_yazdirilacak_satir_sayisi;
    int                         m_yazdirilan_kayit_sayisi;
    int                         m_bilanco_hesaplari_indis_no;
    int                         m_bilanco_hesaplari_sayisi;
    int                         m_last_hesap_indisi;

    int                         m_pasif_baslangic_index;

    int                         m_pasif_index;
    int                         m_aktif_index;

    MUH_BILANCO_HESAPLARI_STRUCT  * BILANCO_HESAPLARI;

    QString             GET_HEADER                                  ();
    QString             GET_FOOTER                                  ();
    QString             GET_BODY                                    ();

    bool                m_aktif_finished;
    bool                m_pasif_finished;

    void                FILL_BILANCO_TABLOSU                        ();
    void                GRUBUN_HESAPLARINI_BILANCO_HESAPLARINA_EKLE ( int grup_indisi  , QString grup_kodu);
    void                SINIF_BAKIYESINI_BILANCO_HESAPLARINA_EKLE   ( int sinif_indisi , QString p_sinif_kodu );

    void                BILANCOYA_FARK_BAKIYESINI_EKLE (int indis_no , bool aktif_varlik_satiri_mi);
};

#endif // BILANCO_RAPORU_H
