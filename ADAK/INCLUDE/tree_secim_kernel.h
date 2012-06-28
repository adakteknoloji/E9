#ifndef TREE_SECIM_KERNEL_H
#define TREE_SECIM_KERNEL_H

#include <QTreeWidget>
#include <QAbstractButton>
#include <QEvent>
#include <QCloseEvent>
#include <QDialog>
#include <QStatusBar>
#include <QToolButton>
#include <QHBoxLayout>
#include <adak_sql.h>
#include "base_tree_kernel.h"

class TREE_SECIM_KERNEL : public BASE_TREE_KERNEL
{
    Q_OBJECT

public:
    TREE_SECIM_KERNEL(QWidget * parent = 0);
    virtual ~TREE_SECIM_KERNEL();

protected:
    bool                        m_is_multi_row;

    QTreeWidget *               GET_TREE_VIEW                    ();

    void                        SET_WINDOW_SIZE                  (int height, int width);

    void                        INIT_KERNEL                      (QObject * parent,ADAK_SQL * database);

    //Bu fonksiyonla table widgetin altina buttonlar eklenebilir.Button text gonderdigimizde fonksiyon
    //bize o button text e sahip push button in adresini dondurur.Bunu kullanarak DOUBLE_CLICKED fonksiyonunda
    //hangi buttona tiklandigini bulabiliriz.Icon ve size bilgileri istenirse gonderilebilir.Default olarak
    //Gelecek icon ekle ikonudur.Default size da layouta uygun olarak ayarlanir.
    QToolButton *               ADD_BUTTON                       (QString button_text, Qt::ToolButtonStyle style = Qt::ToolButtonTextUnderIcon,
                                                                  QString icon = ":/kernel_icons/kernel_ekle_ikonu.png",int size_height=-1,int size_width=-1);

    virtual void                BUTTON_CLICKED                   (QAbstractButton * button,QStringList column_datas);

    //Bu fonksiyonla sonuclardan secilen degerler programciya doner.Eger programci ADAK_CONTINUE dondururse
    //kernel calismaya devam eder aksi takdirde ADAK_EXIT dondururse program sonlanir.
    virtual int                 SINGLE_LINE_SELECTED             ( QStringList        column_datas ) = 0;

    //Sirasi ile QList secilen rowlari verir.
    //Yani selected_datas.at(0)      -> 0. Secili olan(!!!) ilk Row bilgisi
    //Yani selected_datas.at(0).at(1)-> Secili olan(!!!) ilk rowun 1. Column daki degeri

    virtual int                 MULTI_LINE_SELECTED              ( QList<QStringList> selected_datas );

    //virtual olan bu fonksiyon programciya secilen row un idsini gonderir.
    virtual void                SELECTED_LINE                    ( QStringList        column_datas  );

    void                        SET_ID_COLUMN_NUM                (int column);

    void                        ENABLE_MULTI_ROW_SELECTION       ();
    void                        DISABLE_MULTI_ROW_SELECTION      ();

    void                        ADD_PARENT_ITEM                  (QStringList column_datas,int number_of_child, bool select_item_if_multi_row_enable = false);
    void                        ADD_CHILD_ITEM                   (QStringList column_datas,int number_of_child, bool select_item_if_multi_row_enable = false);
    void                        UPDATE_SELECTED_ITEM             (QStringList column_datas);

private:
    QList<QTreeWidgetItem *>    m_selected_items;

    int                         m_secilen_check_box_column;    

    int                         p_height;
    int                         p_width;

    void                        SET_TREE_WIDGET_DEFAULTS       ();
    void                        ADD_PARENT_ITEMS               ();
    void                        FIND_SELECTED_ITEMS            (QTreeWidgetItem * parent_item ,QList<QTreeWidgetItem *> * old_selected_items );

    QHBoxLayout *               p_h_box_layout;

private slots:
    void                        FIND_SELECTED_ROW              ();
    void                        SLOT_ROW_CLICKED               ();
    void                        SLOT_BUTTON_CLICKED            ();

public:
    virtual void                FILL_TREE_TABLE() = 0;
};

#endif // TREE_SECIM_KERNEL_H
