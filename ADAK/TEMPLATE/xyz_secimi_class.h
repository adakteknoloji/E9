#ifndef XYZ_SECIMI_CLASS_H
#define XYZ_SECIMI_CLASS_H

#include "secim_kerneli.h"

class XYZ_SECIMI : public SECIM_KERNELI
{
public:
    XYZ_SECIMI                                ( QWidget *  parent = 0 );
    ~XYZ_SECIMI                               () {}

private:
    void                 FILL_TABLE           ();

    QToolButton *        m_button_yeni_xxyyzz;
    int         *        m_secilen_id;
    //Donus degerleri;
    //- ADAK_EXIT cikmak icin
    //- ADAK_CONTINUE devam etmek icin
    int                  SINGLE_LINE_SELECTED ( int selected_row_number );
};

#endif // XYZ_SECIMI_CLASS_H


