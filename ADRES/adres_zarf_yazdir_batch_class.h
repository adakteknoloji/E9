#ifndef ADRES_ZARF_YAZDIR_BATCH_H
#define ADRES_ZARF_YAZDIR_BATCH_H

#include <QPrinter>
#include <QPainter>
#include "batch_kernel.h"

namespace Ui {
    class ADRES_ZARF_YAZDIR_BATCH;
}

class ADRES_ZARF_YAZDIR_BATCH : public BATCH_KERNEL
{
    Q_OBJECT

private:
    Ui::ADRES_ZARF_YAZDIR_BATCH *   m_ui;

public:
                           ADRES_ZARF_YAZDIR_BATCH              ( int record_id, QString printer_name , QWidget * parent );
                           ~ADRES_ZARF_YAZDIR_BATCH             () {}


private:
    int                    m_record_id;

    int                    m_modul_id;
    int                    m_program_id;

    double                 m_device_pixel_rate_x;
    double                 m_device_pixel_rate_y;

    int                    m_etiket_sol_bosluk;
    int                    m_etiket_ust_bosluk;
    int                    m_etiket_genislik;
    int                    m_etiket_yukseklik;
    int                    m_row_space;
    int                    m_font_size;
    int                    m_last_add_line_y_pos;

    QString                m_adres_string;

    QString                m_printer_name;

    QPrinter               m_printer;
    QPainter               m_painter;

    void                   SETUP_FORM                           ();

    int                    CHECK_VAR                            (QObject * object);
    int                    CHECK_RUN                            ();
    void                   RUN_BATCH                            ();

    void                   CALCULATE_DEVICE_PIXEL               ();
    void                   START_PRINT                          ();
    void                   GET_VARIABLES                        ();
    void                   CREATE_PAGE                          ();

};

#endif // ADRES_ZARF_YAZDIR_BATCH_H
