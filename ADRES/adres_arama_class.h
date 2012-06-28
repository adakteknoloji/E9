#ifndef ADRES_ARAMA_H
#define ADRES_ARAMA_H

#include <QList>

#include "arama_kernel.h"
#include "adres_enum.h"

namespace Ui {
    class ADRES_ARAMA;
}

class ADRES_ARAMA : public ARAMA_KERNEL
{
    Q_OBJECT

public:
                                    ADRES_ARAMA                ( int p_kayit_turu,int islem_turu = ADRES_ARAMA_ISLEMI, bool is_multi_line = false,QWidget * p_parent = 0);
                                    ~ADRES_ARAMA               () {}

    int *                           m_secilen_id;
    QList<int>  *                   m_secilen_id_list;

    QList < int >  *                m_grup_idleri;

private:

    Ui::ADRES_ARAMA *      m_ui;
    int                             p_kayit_turu;
    bool                            m_is_multi_line;
    int                             m_islem_turu;
    QList <int>                     m_adr_export_id_list;


    void                            SETUP_FORM                           ();
    int                             CHECK_VAR                            ( QObject * p_object );
    int                             CHECK_RUN                            ();
    void                            SEARCH                               ();
    int                             SINGLE_LINE_SELECTED                 ( int p_selected_row_number );
    int                             MULTI_LINE_SELECTED                  ( QList<int> selected_rows  );

};


#endif // ADRES_ARAMA_H
