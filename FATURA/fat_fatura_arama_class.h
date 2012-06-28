#ifndef FAT_FATURA_ARAMA_H
#define FAT_FATURA_ARAMA_H

#include "arama_kernel.h"

namespace Ui {
    class FAT_FATURA_ARAMA;
}

class FAT_FATURA_ARAMA: public ARAMA_KERNEL
{
    Q_OBJECT

public:
                           FAT_FATURA_ARAMA             ( int p_fatura_alis_satis_turu, int p_iade_faturasi_mi, QWidget * p_parent = 0 );
                           ~FAT_FATURA_ARAMA            (){}
private:

   struct FATURA_BILGILERI {

       int          fis_id;
       int          fis_no;
       int          fatura_turu;
       QString      fatura_tarihi;
       int          cari_hesap_id;
       QString      fat_cari_hsp_ismi;
       QString      car_cari_hesap_kodu;
       QString      car_cari_hesap_ismi;
       int          yazdirildi_mi;
       int          gecerli_fatura;
       QString      belge_seri;
       QString      belge_numarasi;
       double       fatura_tutari;
   };

   FATURA_BILGILERI *  M_FATURA_BILGILERI;

   Ui::FAT_FATURA_ARAMA      *        m_ui;

   int                    m_fatura_alis_satis_turu;
   int                    m_entegre_filtresi_turu;
   int                    m_iade_faturasi_mi;
   int                    m_cari_hesap_id;

   void                   SETUP_FORM                                 ();
   int                    CHECK_VAR                                  ( QObject * p_object );
   int                    CHECK_RUN                                  ();
   void                   SEARCH                                     ();
   int                    SINGLE_LINE_SELECTED                       ( int p_selected_row_number );

   void                   SEARCH_EDIT_CLICKED                       ( QWidget *widget, QLineEdit *line_edit );

};

#endif // FAT_FATURA_ARAMA_H
