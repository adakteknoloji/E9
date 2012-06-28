#ifndef ETIKET_KERNEL_H
#define ETIKET_KERNEL_H

#include <QStringList>
#include <report_kernel.h>
#include "etiket_sablonlari.h"

//NOT - 1: Tum degerlerin birimleri milimetre olmalidir.

//NOT - 2: Etiketlerin preview ekranina gonderirken kullandigimiz ekranda, gerekiyorsa printer marginlerin ayarlanamasi lazim su anda tanimli fonsiyon;
//  void SHOW_PRINT_PREVIEW(QString string_html, QWidget * parent, qreal left_margin = 0.0,qreal top_margin = 0.0,qreal bottom_margin=0.0,qreal right_margin=0.0);
//Burda marginler default olarak 0 gonderiliyor.Bu sekilde printerin default marginleri kullanilir.Ancak etiketlerde
//bu degerler kullanici tanimli oldugundan veya struct dan geldiginden, kullanicin degerleri yada struct dan bir deger seciliyse de bu degerler
//gonderilmelidir.Struct da bu degerler double page_left_space = left_margin iken , double page_top_space = top_margin dir. Bottom ve right margin daima 0 dir cunku
//etiket duzgun ayarlanmissa zaten etiket tam olarak sayfaya oturacagindan bottom ve right margin direk olarak hesaplanir.
//
//Dikkat structdaki double margin_right,double margin_bottom degerleri,sayfadaki her bir etiket arasindaki degerlerdir.Printer marginleri
//ile herhangi bir baglantisi yoktur. Bu yuzden SHOW_PRINT_PREVIEW fonksiyonu ile kullanilmamalidir.



class ETIKET_KERNEL : public REPORT_KERNEL
{
public:
    ETIKET_KERNEL();
    QList<etiket_sablonlari> p_sablonlar;
    void ETIKET_SABLONLARINI_OLUSTUR();

protected:
    //etiketin boyutlari
    void SET_ETIKET_SIZE(double etiket_width,double etiket_height);

    //etiketin sagdan ve alt tarafindan birakilacak bosluk
    void SET_ETIKET_MARGIN(double margin_right,double margin_bottom);

    //Sayfanin en ust ve sag tarafindan birakilacak bosluk
    //Bu opsiyon kullanilirsa yazici margin 0 lanmalidir.
    void SET_PAGE_SPACE(double top_space,double left_space);

    //Bir sayfada bulunacak etkiket sayisi
    void SET_SAYFADAKI_SATIR_SAYISI(int satir_sayisi);

    //Bir sayfaya konacak etiket sayisi
    void SET_SATIRDAKI_ETIKET_SAYISI(int etiket_sayisi);

    //gonderilen stringlist gore sayfalari ve sayfa duzenini olusturur.
    //QStringlist sadece etiket tablolarini icermelidir.
    QString ETIKETLERI_OLUSTUR(QStringList etiketler);

private:
    double p_etiket_width;
    double p_etiket_height;
    double p_page_left_space;
    double p_page_top_space;
    double p_margin_right;
    double p_margin_bottom;
    int p_satirdaki_etiket_sayisi;
    int p_eklenen_etiket_sayisi;
    int p_toplam_etiket_sayisi;
    int p_max_line_count;

    bool p_finish_report;

    int CONVERT_MM_TO_PT(double mm);

    QStringList p_etiketler;

    QString GET_BODY();
    QString GET_FOOTER();
    QString GET_HEADER();

};

#endif // ETIKET_KERNEL_H
