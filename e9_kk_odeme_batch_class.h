#ifndef E9_KK_ODEME_BATCH_H
#define E9_KK_ODEME_BATCH_H

#include "batch_kernel.h"

struct E9_ISLEM_DETAYLARI;

namespace Ui {
    class E9_KK_ODEME_BATCH;
}

class E9_KK_ODEME_BATCH : public BATCH_KERNEL
{
    Q_OBJECT


public:
                           E9_KK_ODEME_BATCH                            ( int p_fis_satiri_id , E9_ISLEM_DETAYLARI * P_ISLEM_DETAYLARI , int p_modul_id,QWidget * parent = 0 );
                           ~E9_KK_ODEME_BATCH                           () {}

    bool           *       m_kayit_degisti_mi;
private:

    Ui::E9_KK_ODEME_BATCH *          m_ui;
    int                              m_kredi_karti_id;
    int                              m_fis_satiri_id;
    int                              m_hesap_no_id;
    int                              m_modul_id;
    E9_ISLEM_DETAYLARI *             M_ISLEM_DETAYLARI;

    void                             SETUP_FORM                           ();
    int                              CHECK_VAR                            ( QObject * object );
    int                              CHECK_RUN                            ();
    void                             RUN_BATCH                            ();
    void                             SEARCH_EDIT_CLICKED                 (QWidget *widget, QLineEdit *line_edit);

};

#endif // E9_KK_ODEME_BATCH_H
