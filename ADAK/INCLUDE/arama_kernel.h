#ifndef ARAMA_KERNEL_H
#define ARAMA_KERNEL_H

#include <QDialog>
#include <QStatusBar>
#include <QHeaderView>
#include <QVBoxLayout>
#include "batch_kernel.h"

class ARAMA_KERNEL : public BATCH_KERNEL
{
    Q_OBJECT

public:
    ARAMA_KERNEL(QWidget *parent = 0);
    ~ARAMA_KERNEL();

protected:
    void                   START_ARAMA_KERNEL                   (QObject * parent, ADAK_SQL * database);

    int                    ADD_NEW_ROW();

    //Arama sonuclarini gosterilmesi istenen table widget bu fonksiyonla setlenir.
    //Setlenen table widget kernel tarafindan otomatik olarak temizlenecek ayrica otomatik olarak
    //kernel tarafindan hidelanip visible yapilacak.Bu yuzden programcinin sadece table widget i setlemesi yeterli.
    void                   REGISTER_TABLE_WIDGET                (QTableWidget * table_widget);

    //Arama yi baslatacak olan butonun textini atamak icin bu fonksiyon kullanilabilir.Bu fonksiyon cagrilmadikca
    //default olarak buton texti "Ara" dir.
    void                   SET_FIND_BUTTON_NAME                 (QString find_button_name);

    //Arama yapildiginda filtreleme amacli kullanilan tum widgetlarin disable edilmesi gerek.Boylelikle
    //arama sonucuyla widgettaki degerler yeni bir arama istenene kadar birbiri ile uyusur.Bunun icin tum bu
    //widgetlari frame icine yerlestirilirek kernel a bu fonksiyonla setlenir.Kernel da otomatik olarak disable
    //yada enable eder.
    void                   REGISTER_ARAMA_BUTTONS_WIDGET        (QWidget * widget,bool is_yardim_button_visible = true);

    //Table widgettaki column headerlar atanir.QStringList olarak gonderilmelidir.
    //
    //Ornek; SET_HEADERS(QStringList()<<"Column Header 1"<<"Column Header 2");
    void                   SET_HEADERS                          (QStringList headers);

    //Ara butonuna basildiginda cagrilacak olan fonksiyon bu butonla gerekli sonuclar bulunup table widgeta
    //eklenmelidir.
    virtual void           SEARCH                               () = 0;

    //Bu fonksiyonla sonuclardan secilen degerler programciya doner.Eger programci ADAK_CONTINUE dondururse
    //kernel calismaya devam eder aksi takdirde ADAK_EXIT dondururse program sonlanir.
    virtual int            SINGLE_LINE_SELECTED                 (int selected_row        ) = 0;
    virtual int            MULTI_LINE_SELECTED                  (QList<int> selected_rows);

    //Bu fonksiyonla birden fazla row secimine izin verilir o zaman int yerine qlist donecektir.
    void                  ENABLE_MULTI_ROW_SELECTION           ();
    void                  DISABLE_MULTI_ROW_SELECTION          ();

    //virtual olan bu fonksiyon programciya secilen row u gonderir.
    virtual void           SELECTED_LINE                        (int selected_row_number);

    //otomatik siralamayi kapatir.default true dir.false gonderilirse otomatik siralamaz.
    //INIT_KERNEL dan once kullanilmali.
    void                   SET_SORTING                          (bool value , int sorting_column = 1);

    // Mevcut kriterlere gore aramayi yeniden yapar. 
    void                   REFRESH_ARAMA                        ( void );

    //Default degerlerle aramayi baslatmak icin kullanilir. INIT_KERNEL ve default degerler
    //atandiktan sonra en son bu fonksiyon cagrilmali.
    void                   AUTO_START                           ();

    QToolButton *          ADD_BUTTON                           (QString button_text, Qt::ToolButtonStyle style = Qt::ToolButtonTextUnderIcon,
                                                                      QString icon = ":/kernel_icons/kernel_ekle_ikonu.png",int size_height=-1,int size_width=-1);


    virtual bool           eventFilter                          (QObject * obj, QEvent * event);


private:
    int                    p_num_of_columns;
    int                    m_sorting_column;
    bool                   m_is_multi_select;
    bool                   p_sorting_value;
    bool                   m_auto_start;
    QString                p_kayit_sayisi;
    QVBoxLayout *          p_vertical_layout;
    QLabel *               m_label;
    QString                p_find_button_name;
    QWidget *              m_arama_button_widget;
    QWidget *              m_batch_kernel_buttons;
    QTableWidget *         p_table_widget;
    QStringList            p_headers;
    QString                p_window_title;
    void                   RUN_BATCH                      ();

    void                   SET_TABLE_SELECTION_MODE       ();
    void                   SET_TABLE_CHECK_BOX_COLUMN     ();
    void                   INIT_KERNEL                    ();


private slots:
    void                   FIND_SELECTED_ROWS             ();
    void                   SLOT_CLICKED                   ();

};

#endif // ARAMA_KERNEL_H
