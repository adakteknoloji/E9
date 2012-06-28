#ifndef CARI_KART_ARAMA_H
#define CARI_KART_ARAMA_H

#include "arama_kernel.h"
#include "cari_enum.h"

namespace Ui {
    class CARI_KART_ARAMA;
}

class CARI_KART_ARAMA: public ARAMA_KERNEL
{
    Q_OBJECT

public:
    CARI_KART_ARAMA             ( QString cari_hesap_ismi_hesap_kodu, int cari_kart_turu, int show_add_button,  QWidget * parent = 0 ,
                                  int prog_id = -1,int modul_id=-1,QString ekran_ismi = QObject::tr("CARİ KART ARAMA"), QString filtre_str = "" , int islem_turu = CARI_KART_ARAMA_ISLEMI );
                           ~CARI_KART_ARAMA            (){}
    int *                  m_secilen_id;
private:

    Ui::CARI_KART_ARAMA *        m_ui;

    QMap<QPushButton *, int>m_button_list;

    int                    m_islem_turu ;
    QString                m_cari_hesap_ismi_hesap_kodu;
    int                    m_cari_kart_turu;
    int                    m_show_add_button;
    int                    m_modul_id;
    int                    m_program_id;
    QString                m_ekran_ismi;
    QString                m_filtre_str;

    QList<int>             m_grup_idleri;
    QList <int>            m_cari_hesap_id_list;

    void                   SETUP_FORM                                 ();

    int                    CHECK_VAR                                  ( QObject * p_object );
    int                    CHECK_RUN                                  ();
    void                   SEARCH                                     ();
    int                    SINGLE_LINE_SELECTED                       ( int p_selected_row_number );

    QStringList            FIND_UYGUN_ADRES_ID_LIST ( QStringList list_1,QStringList list_2 , QStringList list_3);

    int                    CHANGER_BUTTON_CLICKED(QAbstractButton *button);

};

#endif // CARI_KART_ARAMA_H
