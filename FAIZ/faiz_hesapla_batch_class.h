#ifndef FAIZ_HESAPLA_BATCH_H
#define FAIZ_HESAPLA_BATCH_H

#include <QtGui/QDialog>
#include <QVBoxLayout>
#include <QLabel>
#include "batch_kernel.h"

namespace Ui {
    class FAIZ_HESAPLA_BATCH;
}

class FAIZ_HESAPLA_BATCH : public BATCH_KERNEL
{
    Q_OBJECT



public:
                           FAIZ_HESAPLA_BATCH                            (int islem_turu, QWidget * parent = 0);
                           ~FAIZ_HESAPLA_BATCH                           () {}


private:
   bool                   event_started;
   Ui::FAIZ_HESAPLA_BATCH *   m_ui;

   QString                 page_title;
   int                     m_islem_turu;
   int                     m_faiz_hesaplama_sekli;

   QList < QCommaEdit *>   m_comma_edit_gunler;
   QList < QAdakDate *>   m_date_edit_tarih;
   QList < QCommaEdit *>   m_comma_edit_miktarlar;
   QList < QComboBox  *>   m_combo_box_odeme;
   QList < QFrame     *>   m_frame_container_list;

   QList<QLabel* >         m_tl_label_list;
   QList<QLabel* >         m_taksit_label_list;
   QList<QGridLayout *>    m_grid_layout_list;



protected:
    void                   SETUP_FORM                           ();

    int                    CHECK_VAR                            (QObject * object);
    int                    CHECK_RUN                            ();
    void                   RUN_BATCH                            ();
    void                   WIDGETLARI_EKLE                      ();


    double                 AYLIK_ODEME_MIKTARINI_HESAPLA        (double anapara,double aylik_faiz,int taksit_sayisi);
    double                 FAIZ_ORANINI_BUL                     ();


    void                   SET_WIDGET_COLORS                    ();


    double                 ANA_PARA_HESAPLA                     ();
    void                   TUM_GUNLERI_SIFIRLA                  ();

    void                   TUM_GUN_VE_TARIHLERI_AYARLA          ();

    void                   TAKSITLERI_HESAPLA_FUNC              ();
    void                   SET_TARIHLER_ENABLE                  ( bool durum );

private slots:


    void                   TAKSITLERI_HESAPLA();
    void                   TOPLAM_TUTARI_GOSTER();
    void                   WIDGETLARI_DUZENLE();
    void                   SLOT_ESIT_TAKSITLERI_AYARLA();
    void                   SLOT_TAKSITLERI_HESAPLA();
    void                   REMOVE_COMBO_BOX_COLOR();
    void                   TARIHLERI_AYARLA();
    void                   FAIZ_ORANLARINI_AYARLA();

public:
    void                SHOW_REKLAM();

};

#endif // FAIZ_HESABI_BATCH_H
