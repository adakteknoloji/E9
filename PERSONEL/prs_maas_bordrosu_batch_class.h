#ifndef PRS_MAAS_BORDROSU_BATCH_CLASS_H
#define PRS_MAAS_BORDROSU_BATCH_CLASS_H

#include <batch_kernel.h>
#include "ui_prs_maas_bordrosu_batch.h"

struct PRS_BORDRO_STRUCT;
struct PRS_PERSONEL_STRUCT;
struct PRS_SABITLER;
struct PRS_FIRMA_ODENEKLERI;

namespace Ui {
    class PRS_MAAS_BORDROSU_BATCH;
}

class PRS_MAAS_BORDROSU_BATCH : public BATCH_KERNEL
{
    Q_OBJECT

private:
    Ui::PRS_MAAS_BORDROSU_BATCH *   m_ui;

public:
                           PRS_MAAS_BORDROSU_BATCH              ( QWidget * parent = 0 );
                           ~PRS_MAAS_BORDROSU_BATCH             () {}

protected:

    void                   SETUP_FORM                           ();
    int                    CHECK_VAR                            ( QObject * object );
    int                    CHECK_RUN                            ();
    int                    CHECK_EXIT                           ();
    void                   RUN_BATCH                            ();

private:

    int                    m_sube_id;
    int                    m_personel_cari_hesap_id;


    void                   BORDRO_HAZIRLA                       ( PRS_BORDRO_STRUCT * BORDRO_HESAP, int ay , int yil );
  //  void                 PERSONEL_ODENEKLERINI_STRUCTA_EKLE   ( PRS_BORDRO_STRUCT * BORDRO_HESAP, PRS_PERSONEL_STRUCT * PERSONEL_HESAP );
    void                   PERSONEL_ODENEKLERINI_STRUCTA_EKLE   ( PRS_BORDRO_STRUCT * bordro_bilgileri ,PRS_PERSONEL_STRUCT*personel_bilgileri);
    void                   ODENEK_BILGILERINI_HESAPLA           ( PRS_BORDRO_STRUCT * bordro_bilgileri ,PRS_PERSONEL_STRUCT*personel_bilgileri);
    void                   PERSONEL_KESINTILERINI_STRUCTA_EKLE  ( PRS_BORDRO_STRUCT * BORDRO_HESAP, PRS_PERSONEL_STRUCT * PERSONEL_HESAP );
    void                   MAASI_NETTEN_BRUTE_CEVIR             ( PRS_BORDRO_STRUCT * BORDRO_HESAP, PRS_PERSONEL_STRUCT * PERSONEL_HESAP );
    void                   BORDROYA_PERSONEL_ODENEKLERINI_EKLE  ( PRS_BORDRO_STRUCT * BORDRO_HESAP, PRS_PERSONEL_STRUCT * PERSONEL_HESAP, int bordro_personel_id );
    void                   BORDROYA_PERSONEL_KESINTILERINI_EKLE ( PRS_BORDRO_STRUCT * BORDRO_HESAP, PRS_PERSONEL_STRUCT * PERSONEL_HESAP, int bordro_personel_id );
   // void                   ADD_TO_ODENEKLER_TABLE               ( PRS_BORDRO_STRUCT * BORDRO_HESAP, PRS_ODENEK_STRUCT * ODENEK, int bordro_personel_id );
   // void                   ADD_TO_KESINTILER_TABLE              ( PRS_BORDRO_STRUCT * BORDRO_HESAP, PRS_KESINTI_STRUCT * KESINTI, int bordro_personel_id );
    void                   COMBO_BOX_BORDRO_YILI_DOLDUR         ();




    int                    GET_BORDRO_KESILECEK_PERSONEL_SAYISI ( int p_bordro_ayi, int p_bordro_yili );
    QString                GET_BORDRO_DONEMI                    ( int p_bordro_ayi, int p_bordro_yili );


    void                   BORDRO_BILGILERINI_TOPLA             ( PRS_BORDRO_STRUCT * BORDRO_BILGILERI , int bordro_ay , int bordro_yil  );
    void                   ODENEKLERI_TOPLA                     ( PRS_BORDRO_STRUCT * BORDRO_BILGILERI , PRS_PERSONEL_STRUCT* PERSONEL_BILGILERI );
    void                   KESINTILERI_TOPLA                    ( PRS_BORDRO_STRUCT * BORDRO_BILGILERI , PRS_PERSONEL_STRUCT* PERSONEL_BILGILERI );
    void                   BORDRO_BILGILERINI_HESAPLA           ( PRS_BORDRO_STRUCT * BORDRO_BILGILERI );
    void                   MAASI_BRUTTEN_NETE_CEVIR             ( PRS_BORDRO_STRUCT * BORDRO_HESAP, PRS_PERSONEL_STRUCT* PERSONEL_HESAP );

    void                   BORDRO_BILGILERINI_DB_YAZ            ( PRS_BORDRO_STRUCT * bordro_bilgileri ) ;

    int                    BORDRO_ALABILIRMI                    ();


};

#endif  // PRS_MAAS_BORDROSU_BATCH_H
