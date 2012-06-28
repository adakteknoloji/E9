#ifndef FAT_TOPLU_FATURALASTIR_ARAMA_H
#define FAT_TOPLU_FATURALASTIR_ARAMA_H

#include <QtGui/QDialog>
#include "arama_kernel.h"

namespace Ui {
    class FAT_TOPLU_FATURALASTIR_ARAMA;
}

class FAT_TOPLU_FATURALASTIR_ARAMA : public ARAMA_KERNEL
{
    Q_OBJECT

public :

    FAT_TOPLU_FATURALASTIR_ARAMA                            (QWidget * parent = 0);
    ~FAT_TOPLU_FATURALASTIR_ARAMA                           () {}

private :

    Ui::FAT_TOPLU_FATURALASTIR_ARAMA *   m_ui;
    int                              m_irs_faturalandirilacak_gun_sayisi;
    QStringList                      M_FATURA_SATIRLARI;
    QStringList                      M_FATURA_SATIRININ_IRSALIYE_SATIRLARI;
    QStringList                      M_FATURALASTIRILMIS_IRSALIYE_LISTESI;

    QList < int >                    m_grup_idleri;
    QList < int >                    m_irsaliye_fis_id_list;


    int                              m_sube_id;

    void                    SETUP_FORM                                      ();
    int                     CHECK_VAR                                       ( QObject * object );
    int                     CHECK_RUN                                       ();
    void                    FATURALASTIR                                    ( int p_irsaliye_fis_id );
    void                    SEARCH_EDIT_CLICKED                             ( QWidget * widget, QLineEdit * line_edit );

    void                    SAVER_BUTTON_CLICKED                            ( QAbstractButton * p_button );

    int                     SINGLE_LINE_SELECTED                            ( int selected_row  );
    void                    SEARCH                                          ();

};

#endif // FAT_TOPLU_FATURALASTIR_ARAMA_H
