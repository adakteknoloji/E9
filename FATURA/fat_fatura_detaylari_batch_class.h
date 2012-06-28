#ifndef FAT_FATURA_DETAYLARI_BATCH_H
#define FAT_FATURA_DETAYLARI_BATCH_H

#include "batch_kernel.h"

struct FATURA_DETAYLARI_STRUCT;

namespace Ui {
    class FAT_FATURA_DETAYLARI_BATCH;
}

class FAT_FATURA_DETAYLARI_BATCH : public BATCH_KERNEL
{
    Q_OBJECT


public:
    bool * m_return_value;

    FAT_FATURA_DETAYLARI_BATCH ( FATURA_DETAYLARI_STRUCT * P_FATURA_DETAYLARI, int p_fis_id, int p_fatura_alis_satis_iade_turu, QWidget * p_parent, int p_iade_faturasi_mi = 0 );
    ~FAT_FATURA_DETAYLARI_BATCH() {}

private:

    FATURA_DETAYLARI_STRUCT *        M_FATURA_DETAYLARI;
    Ui::FAT_FATURA_DETAYLARI_BATCH * m_ui;

    int  m_fis_id;

    int  m_sube_yurtici_satis_hsp_id;
    int  m_sube_hizmet_urt_maliyet_hsp_id;

    int  m_satistan_iade_hesap_id;
    int  m_fatura_alis_satis_iade_turu;
    int  m_iade_faturasi_mi;

    int  MUHASEBE_HESAP_KONTROL ( QSearchEdit *p_search_edit, QLineEdit *p_line_edit, int * p_hesap_degiskeni );

    void SETUP_FORM             ();
    int  CHECK_VAR              ( QObject * p_object );
    int  CHECK_RUN              ();
    void RUN_BATCH              ();
    void SEARCH_EDIT_CLICKED    ( QWidget * p_widget, QLineEdit * p_line_edit );

};

#endif // FAT_FATURA_DETAYLARI_BATCH_H
