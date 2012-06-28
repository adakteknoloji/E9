#ifndef BATCH_KERNEL_H
#define BATCH_KERNEL_H

#include <adak_sql.h>
#include "base_event_kernel.h"

class BATCH_KERNEL : public BASE_EVENT_KERNEL
{
    Q_OBJECT

public:
    BATCH_KERNEL (QWidget * parent = 0 );
    virtual ~BATCH_KERNEL();

protected :
    QToolButton *          run_batch_button;
    virtual bool           eventFilter                  (QObject * obj, QEvent * event);

    //Bu fonksiyonla programci tarafindan SET lenen buttonlar CHECK_VAR ve CHECK_EMPTY fonksiyonlari tarafindan
    //degerler kontrol edildikten sonra aktif olurlar.
    void                   REGISTER_SAVER_BUTTON        (QAbstractButton * button);

    void                   SET_ENTER_KEY_FOR_RUN_BATCH  (bool enable);
    //enter tusuna basildiginda RUN_BATCH in cagrilip cagrilmayacagini belirler.Default olarak
    //bu event aktif degildir.Aktif edilmek istenirse SET_ENTER_KEY_FOR_RUN_BATCH(true) cagrilmali.

    void                   REGISTER_BUTTON_WIDGET       (QWidget* batch_buttons_widget,bool is_yardim_button_visible = true,Qt::ToolButtonStyle style = Qt::ToolButtonTextUnderIcon);
    //Gondermis oldugumuz widgeta start butonunun ekler

    void                   SET_NAME_OF_RUN_BATCH_BUTTON (QString name_of_start_button);
    //Bu fonksiyonla run batch buttoninin ismi atanabilir eger bu fonksiyon buton ismi atanmazsa
    //kernel otomatik olarak "Tamam" degerini atar.

    void                   SET_ICON_OF_BATCH_BUTTON     (QString filename);
    //Bu fonksiyonla RUN_BATCH buttonuna icon atanabilir.Eger setlenmezse default olarak getir iconu gelir.
    //
    //Ornek; SET_ICON_OF_BATCH_BUTTON(":/kernel_icons/kernel_getir_ikonu.png");

    // RUN BATCH METODUNDAN SONRA BATCH OTOMATIK KAPATIR
    void                   SET_AUTO_EXIT_BATCH          ( bool p_auto_exit );

    void                   RESET_FOCUS                  ();

    void                   START_BATCH_KERNEL           (QObject * parent, ADAK_SQL * database);

    virtual void           SETUP_FORM                   ()                                 = 0;

    virtual int            CHECK_RUN                    ()                                 = 0;
    virtual void           RUN_BATCH                    ()                                 = 0;
    void                   PROCESS_CHECK_VAR_RETURN_VALUE (int return_value);

    virtual void           SAVER_BUTTON_CLICKED         (QAbstractButton * button);
    virtual void           INIT_KERNEL                  ();

private:
    QString                p_name_of_run_batch_button;
    QString                p_icon_file_name;
    bool                   p_run_batch_on_enter;
    bool                   close_event_started;
    bool                   m_auto_exit;

private slots:
    void                   SLOT_RUN_BATCH_BUTTON        ();
    void                   SLOT_CHECK_BUTTON            ();

};

#endif // BATCH_KERNEL_H
