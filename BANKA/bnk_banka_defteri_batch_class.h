#ifndef BNK_BANKA_DEFTERI_BATCH_CLASS_H
#define BNK_BANKA_DEFTERI_BATCH_CLASS_H

#include "batch_kernel.h"
 
struct BNK_RAPOR_VARS;

namespace Ui {
    class BNK_BANKA_DEFTERI_BATCH;
}

class BNK_BANKA_DEFTERI_BATCH : public BATCH_KERNEL
{
    Q_OBJECT

public:
                                    BNK_BANKA_DEFTERI_BATCH              ( QWidget * p_parent = 0 );
                                    ~BNK_BANKA_DEFTERI_BATCH             () { }
private :

    int                             m_bnk_hesap_no_id;
    int                             m_banka_hesabi_doviz_id;
    BNK_RAPOR_VARS                  * BNK_RV ;

    Ui::BNK_BANKA_DEFTERI_BATCH *   m_ui;
    int                             CHECK_VAR                            ( QObject * p_object );
    int                             CHECK_RUN                            ();
    void                            RUN_BATCH                            ();

    void                            SETUP_FORM                           ();

};

#endif // BNK_BANKA_DEFTERI_BATCH_H
