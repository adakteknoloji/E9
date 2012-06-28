#ifndef EXPORT_KERNEL_H
#define EXPORT_KERNEL_H

#include "batch_kernel.h"

namespace Ui {
    class EXPORT_KERNEL;
}

class EXPORT_KERNEL : public BATCH_KERNEL
{
    Q_OBJECT

public:
                           EXPORT_KERNEL( QString p_help_page, QWidget * parent = 0 );
                           ~EXPORT_KERNEL() {}

    void                   START_EXPORT();

protected:
    void                   SET_ROW_TITLE( QStringList row_title );
    void                   ADD_ROW( QStringList rows);

    //Sirasi ile satirlar ADD_ROW la eklenir.Stringlist in her bir column i bir row a denk gelir.
    virtual void           EXPORT_ROWS ()      = 0;

private:
    QList<QStringList>     m_export_rows;

    QString                m_help_page;

    void    		   SETUP_FORM();
    int                    CHECK_VAR ( QObject * object );
    int                    CHECK_RUN ();

    void                   RUN_BATCH ();
};

#endif // EXPORT_KERNEL_H
