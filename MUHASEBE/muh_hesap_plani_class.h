#ifndef MUH_HESAP_PLANI_H
#define MUH_HESAP_PLANI_H

#include <QtGui/QDialog>
#include <tree_kernel.h>


namespace Ui {
    class MUH_HESAP_PLANI;
}

class MUH_HESAP_PLANI : public TREE_KERNEL {
    Q_OBJECT

public:
    MUH_HESAP_PLANI(QWidget *parent = 0);
    ~MUH_HESAP_PLANI();

    QStringList       m_added_record;
    QStringList       m_updated_record;
    QStringList       m_updated_line;

    QString           m_tum_hesaplar_tables;

    bool              m_record_updated;
    bool              m_line_added;
    bool              m_line_updated;

private:

    Ui::MUH_HESAP_PLANI*    m_ui;
    QString                 p_delimeter;
    bool                    p_hesabin_parentini_aktar;

    void                    ADD_CHILD_ITEMS(QStringList column_datas);
    void                    ADD_PARENT_ITEMS();
    void                    ADD_ITEM(QObject * button,QStringList column_datas);
    void                    UPDATE_ITEM(QStringList column_datas);
    int                     CHECK_DELETE_ITEM(QStringList column_datas);
    void                    DELETE_ITEM(QStringList column_datas);

private slots:
    void                    BAKIYE_TURU_COMBO_BOX_CHANGED();

public:
    void                SHOW_REKLAM();
};

#endif // MUH_HESAP_PLANI_H
