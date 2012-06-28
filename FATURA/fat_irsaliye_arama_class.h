#ifndef FAT_IRSALIYE_ARAMA_CLASS_H
#define FAT_IRSALIYE_ARAMA_CLASS_H

#include "arama_kernel.h"

namespace Ui {
    class FAT_IRSALIYE_ARAMA;
}

class FAT_IRSALIYE_ARAMA: public ARAMA_KERNEL
{
    Q_OBJECT
public:
    FAT_IRSALIYE_ARAMA( QString p_fatura_tarihi, int p_cari_hesap_id, int p_fatura_alis_satis_turu,
                        QList< int > *p_secilen_irs_list, QWidget * p_parent );
    ~FAT_IRSALIYE_ARAMA() {}


private:

    QString  m_irsaliye_tarihi_bitis;
    QString  m_irsaliye_tarihi_baslangic;
    int      m_cari_hesap_id;
    int      m_irsaliye_alis_satis_turu;

    QList< int > *  m_secilen_irs_list;

private:
    Ui::FAT_IRSALIYE_ARAMA  *m_ui;
    
    void SETUP_FORM            ();
    int  CHECK_VAR             ( QObject * object );
    int  CHECK_RUN             ();
    void SEARCH                ();
    int  SINGLE_LINE_SELECTED  ( int selected_row_number );
    void SELECTED_LINE         ( int selected_row_number );
    int  MULTI_LINE_SELECTED   ( QList<int> selected_rows);

};

#endif // FAT_IRSALIYE_ARAMA_CLASS_H
