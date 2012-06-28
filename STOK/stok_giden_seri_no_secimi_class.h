#ifndef STOK_GIDEN_SERI_NO_SECIMI_CLASS_H
#define STOK_GIDEN_SERI_NO_SECIMI_CLASS_H

#include "secim_kerneli.h"
#include "stok_enum.h"

class STOK_GIDEN_SERI_NO_SECIMI : public SECIM_KERNELI
{
public:
    STOK_GIDEN_SERI_NO_SECIMI( int p_depo_id, int p_urun_id, QStringList *p_seri_no_list,
                               URUN_DEPO_DURMU p_urun_durumu,  bool p_sadece_liste, int p_fatura_satiri_id, QWidget *  parent = 0 );
    ~STOK_GIDEN_SERI_NO_SECIMI() {}

public:
    QStringList *m_secilen_seri_nolar;

private:

    URUN_DEPO_DURMU m_urun_durumu;
    int         m_fatura_satiri_id;

    int         m_depo_id;
    int         m_urun_id;
    bool        m_sadece_liste; // db den okumaz sadece gelen liste sectirir iade isleminde kullanilir.

    QList < int > m_secili_olacak_satirlar;

    void        FILL_TABLE           ();
    int         SINGLE_LINE_SELECTED ( int selected_row_number );

    int         MULTI_LINE_SELECTED  ( QList<int> selected_rows );
};

#endif // STOK_GIDEN_SERI_NO_SECIMI_CLASS_H


