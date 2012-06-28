#ifndef E9_URETILEN_FIS_SECIMI_CLASS_H
#define E9_URETILEN_FIS_SECIMI_CLASS_H

#include "secim_kerneli.h"

class E9_URETILEN_FIS_SECIMI : public SECIM_KERNELI
{
public:
     E9_URETILEN_FIS_SECIMI( int p_program_id, int p_modul_id, int p_fis_id, QWidget *  parent = 0 );
    ~E9_URETILEN_FIS_SECIMI() {}

private:
    void FILL_TABLE();
    int  SINGLE_LINE_SELECTED ( int selected_row_number );

    void CARI_ENT_OLMUS_MU();
    void BNK_ENT_OLMUS_MU();
    void MUH_ENT_OLMUS_MU();
    void CEKSENET_ENT_OLMUS_MU();


private:
    int  m_program_id;
    int  m_modul_id;
    int  m_fis_id;

    QList< QStringList > tum_ent_fis_listesi;
};

#endif // E9_URETILEN_FIS_SECIMI_CLASS_H


