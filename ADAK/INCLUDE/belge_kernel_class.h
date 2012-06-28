#ifndef BELGE_KERNEL_H
#define BELGE_KERNEL_H

#include <QString>
#include <QStringList>
#include <QSplashScreen>
#include <QPixmap>
#include <belge_struct.h>
#include <QPrinter>
#include <QPainter>
#include <QMap>
#include "belge_kernel_open.h"

class TASARIM_DEGISKENI;

class BELGE_KERNEL
{
public:
    BELGE_KERNEL (QString printer_name = "", BELGE_PRINT_MOD p_mode = PRINT_NORMAL );
    virtual ~BELGE_KERNEL();

    void                            START_PRINT                     ();

protected:
    void                            SET_ENUM_SIZE                   (int header_enum_size,int footer_enum_size,int line_enum_size);

    void                            SET_HEADER_DATA                 (int header_enum , QString value);
    void                            SET_FOOTER_DATA                 (int footer_enum , QString value);
    void                            SET_LINE_DATA                   (int line_enum   , QString value);

    virtual void                    GET_HEADER_INFO                 ();
    virtual void                    GET_FOOTER_INFO                 ();
    virtual void                    GET_LINE_INFO                   ();

private:
    int                             m_row_count;
    int                             m_belge_satir_count;
    int                             m_row_space_mm;

    int                             m_kullanilan_satir_sayisi;

    QList< TASARIM_DEGISKENI *>     m_all_variables;

    int                             m_row_space_satir;
    int                             m_belge_baslangic_satir_sayisi;
    int                             m_belge_satir_sayisi;
    int                             m_belge_bitis_satir_sayisi;
    int                             m_belge_toplam_karakter_sayisi;

    int                             m_printer_type;
    QString                         m_printer_name;

    BELGE_PRINT_MOD                 m_mode;

    QStringList                     m_header_variables;
    QStringList                     m_footer_variables;
    QStringList *                   m_line_variables;

    //QString                         m_print_txt;

    QMap<int, QString *>            m_print_txt;
    QMap<int, QString *>            m_print_code;
    QString                         m_line_print_code;
    QList<QString*>                 m_line_print_txt;
    int                             m_line_begin_row;


    double                          m_device_pixel_rate_x;
    double                          m_device_pixel_rate_y;
    double                          m_pixel_rate;

    double                          m_last_add_line_y_pos;

    QList<int>                      m_variable_order;
    QStringList                     m_eklenen_textler;

    QStringList                     m_satir_bilgileri;

    QPrinter m_printer;
    QPainter m_painter;

    void                            GET_VARIABLES();

    void                            CREATE_LAZER_BELGE();
    void                            ADD_LAZER_VARIABLES(ADAK_BELGE_DEFAULTS_STRUCT * adak_struct,int index,QStringList * variables,int grup);

    void                            CREATE_DOTMATRIX_BELGE();
    void                            ADD_DOTMATRIX_VARIABLES(ADAK_BELGE_DEFAULTS_STRUCT * adak_struct,int index,QStringList * variables, int grup, bool p_is_nlq, int p_printer_mode);
    void                            SET_DOTMATRIX_VALUE (QString value , int satir , int soldan_bosluk, int grup, bool p_is_nlq, int p_printer_mode );

    void                            ADD_VARIABLES ();

    ADAK_BELGE_DEFAULTS_STRUCT *   m_header_struct;
    int                             m_header_struct_size;
    ADAK_BELGE_DEFAULTS_STRUCT *   m_footer_struct;
    int                             m_footer_struct_size;
    ADAK_BELGE_DEFAULTS_STRUCT *   m_line_struct;
    int                             m_line_struct_size;

    void                            CLEAR_VARIABLES_STRING          (QString * variable,int variable_size);
    void                            FILL_VARIABLE_STRINGLIST        (QStringList *,QString *,int variable_size);

    void                            CALCULATE_DEVICE_PIXEL          ();
    void                            INIT_REPORT                     ();

    QString                         GET_LIST_VALUE                  ( QStringList p_list, int p_degisken_id );

    QString                         DOT_MATRIX_PREPARE_VALUE        ( bool p_is_nlq, int p_printer_mode );

    QString                         MERGE_STRING                    ( QString p_str1, QString p_eklenecek_str );

    QString                         MERGE_CODE                      ( QString p_code1,QString p_eklenecek );

    QString                         ALING_STR                       ( QString p_str, int p_str_size, int p_align );

};

#endif // BELGE_KERNEL_H

