#ifndef STOK_URUN_ARAMA_H
#define STOK_URUN_ARAMA_H

#include "arama_kernel.h"
#include "stok_enum.h"

namespace Ui {
    class STOK_URUN_ARAMA;
}

class STOK_URUN_ARAMA: public ARAMA_KERNEL
{
    Q_OBJECT

public:
                           STOK_URUN_ARAMA             ( int p_kart_turu,int p_depo_id,
                                                         QString p_arama_icerigi,int * secilen_id,
                                                         int hizmet_turu,QWidget * p_parent , int islem_turu = STOK_URUN_ARAMA_ISLEMI );
                           ~STOK_URUN_ARAMA            (){}
private:

    Ui::STOK_URUN_ARAMA   * m_ui;

    QMap<QPushButton*, int >m_button_list;

    int *                  m_secilen_id;
    int                    m_entegrasyon_durumu;
    int                    m_islem_turu;
    QList<int>             m_grup_idleri;
    QList<int>             m_urun_id_list;

    QString                m_arama_icerigi;

    int                    m_depo_id;
    int                    m_urun_hizmet_karti_turu;
    int                    m_hizmet_turu;

    void                   SETUP_FORM             ();
    int                    CHECK_VAR              (QObject * p_object);
    int                    CHECK_RUN              ();
    void                   SEARCH                 ();
    int                    SINGLE_LINE_SELECTED   ( int p_selected_row_number );

    void                   HEADER_DUZENLE         ();

    int                    CHANGER_BUTTON_CLICKED (QAbstractButton *button);

};

#endif // STOK_URUN_ARAMA_H
