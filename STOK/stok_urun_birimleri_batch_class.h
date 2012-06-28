#ifndef STOK_URUN_BIRIMLERI_BATCH_H
#define STOK_URUN_BIRIMLERI_BATCH_H

#include "batch_kernel.h"

namespace Ui {
    class STOK_URUN_BIRIMLERI_BATCH;
}

class STOK_URUN_BIRIMLERI_BATCH : public BATCH_KERNEL
{
    Q_OBJECT

public:
                           STOK_URUN_BIRIMLERI_BATCH             ( int p_urun_id, QWidget * p_parent = 0 );
                           ~STOK_URUN_BIRIMLERI_BATCH            () {}


private:

    Ui::STOK_URUN_BIRIMLERI_BATCH *    m_ui;
    int                               m_urun_id;
    QString                           m_temel_urun_birimi;

    QLineEdit    **                   M_URUN_BIRIM_ADI_LINEEDIT;
    QCommaEdit   **                   M_URUN_BIRIM_KATSAYI_COMMAEDIT;
    QComboBox    **                   M_URUN_BIRIM_ALT_BIRIMI_COMBOBOX;
    QCommaEdit   **                   M_URUN_BIRIM_ALIS_FIYATI_COMMAEDIT;
    QCommaEdit   **                   M_URUN_BIRIM_SATIS_FIYATI_COMMAEDIT;
    QCheckBox    **                   M_URUN_KARTINDA_GOSTER_CHECKBOX;
    QPushButton  **                   M_URUN_BIRIMI_TEMIZLE_BUTTON;

    QString      *                    M_URUN_BIRIM_ADLARI;
    double       *                    M_URUN_BIRIM_KATSAYILARI;
    int          *                    M_URUN_CAPRAZ_BIRIMLER;
    double       *                    M_URUN_BIRIM_ALIS_FIYATLARI;
    double       *                    M_URUN_BIRIM_SATIS_FIYATLARI;
    int          *                    M_GORUNECEK_URUN_BIRIMLERI;

    void                             SETUP_FORM                           ();
    int                              CHECK_VAR                            ( QObject * p_object );
    int                              CHECK_RUN                            ();
    void                             RUN_BATCH                            ();

    void                             SET_WIDGET_ARRAYS                    ();
    void                             FILL_ALT_BIRIM_COMBOBOXES            ();
    void                             URUN_BIRIMI_BILGILERINI_OKU          ();





};

#endif // STOK_URUN_BIRIMLERI_BATCH_H
