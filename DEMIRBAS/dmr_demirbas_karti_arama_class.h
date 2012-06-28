#ifndef DMR_DEMIRBAS_KARTI_ARAMA_BATCH_H
#define DMR_DEMIRBAS_KARTI_ARAMA_BATCH_H

#include <QtGui/QDialog>
#include "arama_kernel.h"

namespace Ui {
    class DMR_DEMIRBAS_KARTI_ARAMA;
}

class DMR_DEMIRBAS_KARTI_ARAMA: public ARAMA_KERNEL
{
    Q_OBJECT

public:
                           DMR_DEMIRBAS_KARTI_ARAMA             ( QString demirbas_kodu, QWidget * parent = 0 ,
                                                                  int demirbas_satildi_mi = 0);
                           ~DMR_DEMIRBAS_KARTI_ARAMA            (){}

    int     *              m_secilen_id;
private:
    QString                m_demirbas_kodu;

    QList <int>            m_grup_idler_list;

    int                    m_sube_id;
    int                    m_aranacak_demirbaslar;

    Ui::DMR_DEMIRBAS_KARTI_ARAMA *        m_ui;

    void                   SETUP_FORM                                 ();
    int                    CHECK_VAR                                  (QObject * object);
    int                    CHECK_RUN                                  ();
    void                   SEARCH                                     ();
    int                    SINGLE_LINE_SELECTED                       (int selected_row_number);

    void                   SEARCH_EDIT_CLICKED                        (QWidget *widget, QLineEdit *line_edit);

};

#endif // DMR_DEMIRBAS_KARTI_ARAMA_H
