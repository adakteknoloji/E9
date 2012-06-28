#ifndef ISL_FIS_ARAMA_CLASS_H
#define ISL_FIS_ARAMA_CLASS_H

#include <QtGui/QDialog>
#include "arama_kernel.h"
#include "ui_isl_fis_arama.h"

class ISL_FIS_ARAMA : public ARAMA_KERNEL
{
    Q_OBJECT

public:
                           ISL_FIS_ARAMA                        (QWidget * parent = 0);
                           ~ISL_FIS_ARAMA                       () {}

    int *                  m_secilen_id;
private:
    Ui::ISL_FIS_ARAMA *    m_ui;
    int                    m_fis_turu;

    void                   SETUP_FORM                           ();
    int                    CHECK_VAR                            (QObject * object);
    int                    CHECK_RUN                            ();
    void                   SEARCH                               ();
    int                    SINGLE_LINE_SELECTED                 (int selected_row_number);

};

#endif // ISL_FIS_ARAMA_H
