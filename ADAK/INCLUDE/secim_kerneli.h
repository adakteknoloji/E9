#ifndef SECIM_KERNELI_H
#define SECIM_KERNELI_H

#include <QDialog>
#include <QHBoxLayout>
#include <QToolButton>
#include <adak_sql.h>
#include "base_kernel.h"

class QTableWidget;
class QTableWidgetItem;
class QWidget;

class SECIM_KERNELI : public BASE_KERNEL
{
    Q_OBJECT

public:
    SECIM_KERNELI(QWidget * parent = 0);
    virtual ~SECIM_KERNELI();

protected:
    int                         ADD_NEW_ROW                          ();
    QTableWidget *              GET_TABLE_WIDGET                     ();
    void                        SET_HEADERS                          (QStringList headers);
    void                        SET_WINDOW_SIZE                      (int height, int width);
    void                        INIT_KERNEL                          (ADAK_SQL * database);
    void                        HIDE_COLUMN_HEADER                   (bool hide);

    //otomatik siralamayi kapatir.default true dir.false gonderilirse otomatik siralamaz.
    //INIT_KERNEL dan once kullanilmali.
    void                        SET_SORTING                          (bool value,int sorting_column = 1);

    //Secim kernelinda kullanilan table ki verileri yenilemek icin kullanilir.
    void                        REFRESH_TABLE                        ();

    //Bu fonksiyonla birden fazla row secimine izin verilir o zaman int yerine qlist donecektir.
    void                        ENABLE_MULTI_ROW_SELECTION           ();
    void                        DISABLE_MULTI_ROW_SELECTION          ();

    //Bu fonksiyonla table widgetin altina buttonlar eklenebilir.Button text gonderdigimizde fonksiyon
    //bize o button text e sahip push button in adresini dondurur.Bunu kullanarak DOUBLE_CLICKED fonksiyonunda
    //hangi buttona tiklandigini bulabiliriz.Icon ve size bilgileri istenirse gonderilebilir.Default olarak
    //Gelecek icon ekle ikonudur.Default size da layouta uygun olarak ayarlanir.
    QToolButton *               ADD_BUTTON                           (QString button_text, Qt::ToolButtonStyle style = Qt::ToolButtonTextUnderIcon,
                                                                      QString icon = ":/kernel_icons/kernel_ekle_ikonu.png",int size_height=-1,int size_width=-1);

    virtual void                BUTTON_CLICKED                       (QToolButton * button);

    //Bu fonksiyonla sonuclardan secilen degerler programciya doner.Eger programci ADAK_CONTINUE dondururse
    //kernel calismaya devam eder aksi takdirde ADAK_EXIT dondururse program sonlanir.
    virtual int                 SINGLE_LINE_SELECTED                 (int selected_row        ) = 0;
    virtual int                 MULTI_LINE_SELECTED                  (QList<int> selected_rows);


    //virtual olan bu fonksiyon programciya secilen row un idsini gonderir.
    virtual void                SELECTED_LINE                        (int selected_row_number);

    // SECIM EKRANI ACILDIGINDA SECENEKLER SECILI HALDE GELMESINI SAGLAR
    void                        SELECT_LINES                         ( QList< int > p_line_row_list );

private:
    int                         p_num_of_columns;
    int                         p_height;
    int                         p_width;
    int                         m_sorting_column;
    bool                        m_is_multi_select;
    bool                        p_sorting_value;
    QHBoxLayout *               p_h_box_layout;
    QTableWidget *              p_table_widget;
    QStringList                 p_headers;
    QLabel *                    m_label_status;
    QString                     p_window_title;

    void                        SET_TABLE_SELECTION_MODE       ();
    void                        SET_TABLE_CHECK_BOX_COLUMN     ();

private slots:
    void                        FIND_SELECTED_ROWS             ();
    void                        SLOT_ROW_CLICKED               ();
    void                        SLOT_BUTTON_CLICKED            ();

public:
    virtual void                FILL_TABLE() = 0;
};

#endif // SECIM_KERNELI_H
