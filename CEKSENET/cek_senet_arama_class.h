#ifndef CEK_SENET_ARAMA_H
#define CEK_SENET_ARAMA_H

#include "arama_kernel.h"



namespace Ui {
    class CEK_SENET_ARAMA;
}

class CEK_SENET_ARAMA : public ARAMA_KERNEL
{
    Q_OBJECT

public:
                           CEK_SENET_ARAMA     (int bordro_islem_turu,int cek_mi_senet_mi
                                               ,int musteri_cek_senedi_mi,int hesap_id,
                                               int para_birimi_id , QWidget * parent = 0);
                           ~CEK_SENET_ARAMA                           () {}

    int *                  m_secilen_id;

private:

    Ui::CEK_SENET_ARAMA *   m_ui;



    int                    m_aranacak_cek_senet_turu;
    int                    m_cek_mi_senet_mi; //CEK,SENET veya HEPSI OLABILIR
    int                    m_musteri_cek_senedi_mi;
    int                    m_hesap_id; //Bordrodaki islem turune gore banka_hesabi veya cari hesabin idsidir.
    int                    m_para_birimi_id; //Aranacak cek senedin ait olmasi gereken para birimi

    void                   SETUP_FORM                           ();
    int                    CHECK_VAR                            ( QObject * object );
    int                    CHECK_RUN                            ();
    void                   SEARCH                               ();
    int                    SINGLE_LINE_SELECTED                 ( int selected_row_number );
    void                   FILL_COMBOBOX_CEKIN_DURUMU();

    QStringList            GET_UYGUN_CEKLER_LIST                ( int hesap_id);


};

#endif // CEK_SENET_ARAMA_H
