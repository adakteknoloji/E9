#ifndef MUH_FIS_KOPYALAMA_BATCH_H
#define MUH_FIS_KOPYALAMA_BATCH_H

#include <QtGui/QDialog>
#include "batch_kernel.h"
#include "MUHASEBE/muh_fis_utils.h"

namespace Ui {
    class MUH_FIS_KOPYALAMA_BATCH;
}

class MUH_FIS_KOPYALAMA_BATCH : public BATCH_KERNEL
{
    Q_OBJECT

public:
                           MUH_FIS_KOPYALAMA_BATCH                                   (int fis_id, QWidget * parent = 0 );
                           ~MUH_FIS_KOPYALAMA_BATCH                                  (){
                               delete M_FIS;
                               delete M_FIS_SATIRI;
                           }
private:

    Ui::MUH_FIS_KOPYALAMA_BATCH *   m_ui;
    int                             m_fis_id;
    int                             m_fis_turu;
    QWidget *                       m_parent;

    MUH_FIS_SATIRI_STRUCT       *   M_FIS_SATIRI;
    MUH_FIS_STRUCT              *   M_FIS;



    void                            SETUP_FORM                                       ();
    int                             CHECK_VAR                                        (QObject * object);
    int                             CHECK_RUN                                        ();
    void                            RUN_BATCH                                        ();
    void                            SEARCH_EDIT_CLICKED                              (QWidget *, QLineEdit *);
};


#endif // MUH_FIS_KOPYALAMA_BATCH_H
