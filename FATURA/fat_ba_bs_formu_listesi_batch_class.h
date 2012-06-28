#ifndef FAT_BA_BS_FORMU_LISTESI_BATCH_H
#define FAT_BA_BS_FORMU_LISTESI_BATCH_H


#include "batch_kernel.h"

namespace Ui {
    class FAT_BA_BS_FORMU_LISTESI_BATCH;
}

class FAT_BA_BS_FORMU_LISTESI_BATCH : public BATCH_KERNEL
{
    Q_OBJECT


public:
                           FAT_BA_BS_FORMU_LISTESI_BATCH                            ( QWidget * parent = 0 );
                           ~FAT_BA_BS_FORMU_LISTESI_BATCH                           () {}
private:

    Ui::FAT_BA_BS_FORMU_LISTESI_BATCH *          m_ui;

    int                    m_cari_hesap_id;
    int                    m_depo_id;
    int                    m_ent_filtresi_turu;

    void                   SETUP_FORM                           ();
    int                    CHECK_VAR                            ( QObject * object );
    int                    CHECK_RUN                            ();
    int                    CHECK_EXIT                           ();
    void                   RUN_BATCH                            ();

    void                   SEARCH_EDIT_CLICKED                  (QWidget *widget, QLineEdit *line_edit);

};

#endif // FAT_BA_BS_FORMU_LISTESI_BATCH_H
