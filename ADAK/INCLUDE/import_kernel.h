#ifndef IMPORT_KERNEL_H
#define IMPORT_KERNEL_H

#include "batch_kernel.h"
#include "arama_kernel.h"
namespace Ui {
    class IMPORT_KERNEL;
}

class IMPORT_KERNEL : public BATCH_KERNEL
{
    Q_OBJECT

public:
             IMPORT_KERNEL ( QString p_help_page, QWidget * parent = 0 );
            ~IMPORT_KERNEL () {}

protected:

    // DEFAULT OLARAK DB ALIR.
    ADAK_SQL              *M_DB;

    void SET_DB( ADAK_SQL *P_DB );

    //Table Widget column headerlari
    //Burdaki headers sirasi ile SAVE_ROW fonksiyonunda gonderilen degerleri indexleri ayni olacaktir.
    //Mesela header;
    //  QStringList() << Semt/Ilce << Il << Ulke
    // ise SAVE ROW da ki values;
    // 0-> Sirinevler 1-> Istanbul 2->Turkiye olur.
    void                   SET_HEADERS       ( QList<QStringList> headers);

    QList<int> *           GET_GRUP_IDLERI   ();

    void                   SET_PROGRAM_ID    (int program_id);
    void                   SET_MODUL_ID      (int modul_id  );

    //Excel deki Row degerlerini tek tek gonderir.QStringList teki her bir index , o rowun column degeridir.
    virtual void           IMPORT_ROW        (QStringList row ) = 0;

private:

    QList<QStringList>     m_import_rows;
    QList<QStringList>     m_header_list;
    bool                   m_kayitlar_onaylandi_mi;
    QList<int>             m_grup_idleri;
    int                    m_program_id;
    int                    m_modul_id;

    QList<int>             m_alan_uzunluklari;
    QStringList            m_zorunlu_alanlar;

    QString                m_help_page;

    Ui::IMPORT_KERNEL *    m_ui;

    void    		   SETUP_FORM           ();
    int                    CHECK_VAR            ( QObject * object );
    int                    CHECK_RUN            ();

    void                   RUN_BATCH            ();

    void                   ONIZLEMEYI_GOSTER    ();

    void                   TABLO_DOLDUR         ( QTableWidget * table_widget, QList<QStringList> list );

    void                   SET_LIST_VALUES      ( int p_row_id, int p_at_id, QString p_value );

    bool                   ROW_DATA_KONTROL     ();

    void                   CHECK_MAX_SIZE_KONTROL( QList<QStringList> p_import_rows );

};

#endif // IMPORT_KERNEL_H
