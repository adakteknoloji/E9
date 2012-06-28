#ifndef ETIKET_SABLONLARI_H
#define ETIKET_SABLONLARI_H

//etiket sablonlari asagidaki struct daki gibi tutulur. Bu degerler etiket kernel da doldurulur.
//Eger ki bu struct a erismek istersek QList<etiket_sablonlari> yaratip bunu GET_ETIKET_SABLONLARI()
//fonksiyonu ile doldurmamiz lazim.Daha sonra buradan asagidaki degerlere erisebiliriz.

struct etiket_sablonlari
{
    //Etiket tanimlamak ve kullaniciya gosterilecek ismi
    QString name;

    //etiket genislik ve yuksekligi
    double etiket_width;
    double etiket_height;

    //etiketlerin bulundugu sayfadaki sol ve ust bosluklar
    double page_left_space;
    double page_top_space;

    //etiketin sol ve alt tarafindan birakilacak bosluklar.Etiketler arasi mesafe
    double margin_right;
    double margin_bottom;

    int satirdaki_etiket_sayisi;
    int sayfadaki_satir_sayisi;
    QString icon_path;
};

//Kernel da tanimli etiketlere ulasmak icin bu fonksiyon kullanilir.
QList<etiket_sablonlari> GET_ETIKET_SABLONLARI();

#endif // ETIKET_SABLONLARI_H
