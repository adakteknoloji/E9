#ifndef ADRES_ETIKETLERI_BATCH_H
#define ADRES_ETIKETLERI_BATCH_H

#include "batch_kernel.h"
#include "etiket_sablonlari.h"

namespace Ui {
    class ADRES_ETIKETLERI_BATCH;
}

class ADRES_ETIKETLERI_BATCH : public BATCH_KERNEL
{
    Q_OBJECT


public:
                                ADRES_ETIKETLERI_BATCH           ( QStringList P_ADRES_ETIKET_BILGILERI, QWidget * parent = 0 );
                                ~ADRES_ETIKETLERI_BATCH          () {}
private:

   Ui::ADRES_ETIKETLERI_BATCH *   m_ui;

   QList <etiket_sablonlari>    sablonlar;
   QStringList                  M_ADRES_ETIKET_BILGILERI;

   void                        SETUP_FORM                           ();

   int                         CHECK_VAR                            (QObject * object);
   int                         CHECK_RUN                            ();
   void                        RUN_BATCH                            ();

   void                        ETIKET_TURUNE_GORE_BASLIK_OLUSTUR    ();



};

#endif // ADRES_ETIKETLERI_BATCH_H
