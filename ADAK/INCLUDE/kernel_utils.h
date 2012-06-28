#ifndef KERNEL_UTILS_H
#define KERNEL_UTILS_H

class QMdiArea;
class QStringList;
class QPixmap;
class QDialog;
#include <QSplashScreen>

enum KERNEL_DEFINES {
  KERNEL_YES     = 1,
  KERNEL_NO      = 2,
  KERNEL_CANCEL  = 3
};

//Minimun height for kernel buttons
#define KERNEL_BUTTON_MINIMUN_HEIGHT  32
//Default max size
#define KERNEL_BUTTON_MAXIMUN_HEIGHT  32


void SET_MDI_WINDOW                                (QMdiArea * mdi_area,QWidget * sub_window);

//Bu fonksiyonlar gonderilen dialog icin standart windows flags(Maximize,minimize button gibi) lari eklenir.
void SET_WINDOW_FLAGS                              (QDialog * dialog);

//Standart bir splash ekrani yaratir. Kullanim sekli ise;
//    QSplashScreen * splash = CREATE_SPLASH_SCREEN();
//    splash->show();
//    splash->showMessage((QString("Rapor Hazırlanıyor.Lütfen Bekleyiniz...")),Qt::AlignCenter,Qt::white);
//
//    Kapatmak istedigimiz de ise;
//    splash->finish(QWidget *);
QSplashScreen * CREATE_SPLASH_SCREEN               ();

#endif // KERNEL_UTILS_H
