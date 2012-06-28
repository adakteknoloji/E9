#ifndef HATALI_FIS_SECIMI_H
#define HATALI_FIS_SECIMI_H

#include "secim_kerneli.h"

class HATALI_FIS_SECIMI : public SECIM_KERNELI
{
public:
    HATALI_FIS_SECIMI(QWidget* parent = 0);
    ~HATALI_FIS_SECIMI() {}

private:

    QStringList          m_hesap_id_list;

    void                 FILL_TABLE              ();
    int                  SINGLE_LINE_SELECTED    (int selected_row_number);

    bool                 FIS_HATALI_MI           (int fis_id , double toplam_borc,
                                                  double toplam_alacak);

};

#endif // HATALI_FIS_SECIMI_H


