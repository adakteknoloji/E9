#ifndef ADAK_GRUPLAR_H
#define ADAK_GRUPLAR_H

#include <QList>
#include <QString>
#include <QStringList>

class ADAK_SQL;

struct ADAK_GRUP_STRUCT {
    int             grup_parent_id;
    int             modul_id;
    int             program_id;
    QString         grup_kodu;
    QString         grup_adi;
};



bool            OPEN_GRUP_SECIM                 (int program_id, int module_id, QList<int> *secilen_idler, ADAK_SQL *db, QWidget *parent,bool ana_gruplar_secilebilir_mi = false);
void            OPEN_GRUPLAR                    ( int program_id , int module_id , QString p_win_title, QString p_help_page, ADAK_SQL * db , QWidget * parent);

// Structu gore grubu ekler ve eklenen grubun idsini dondurur. Bu id yi bu grubu alt grup eklemek icin kullanabiliriz.
// Grubun, alt grup sayisini kernel otomatik olarak arttirir.
int             GRP_GRUP_EKLE                   ( ADAK_GRUP_STRUCT * grup );

// Program ve module ait grup tablosunda kayit olup olmadigina bakar.
// Bununla gerekli gruplarin eklenip eklenmedigine bakilabilir.
bool            GRP_GRUP_EKLENDIMI              ( int program_id , int module_id );

void            GRP_GRUBUN_ALT_GRUPLARINI_BUL   ( QList<int> gruplar, QList<int> * alt_grup_idleri );
void            GRP_KAYDIN_GRUPLARINI_SIL       ( int program_id , int module_id ,int record_id );
void            GRP_KAYDI_GRUPLARA_EKLE         ( int program_id , int module_id ,int record_id , QList<int> * grup_idleri );
QList<int>      GRP_GRUBA_EKLI_KAYITLARI_BUL    ( int program_id , int module_id ,QList<int> gruplar);
QList<int>      GRP_KAYDIN_GRUP_IDLERINI_BUL    ( int program_id , int module_id ,int record_id );
QString         GRP_KAYDIN_GRUPLARINI_BUL       ( int program_id , int module_id ,int record_id ,QList<int> * grup_idleri , QStringList * grup_adlari , QStringList * grup_kodlari = NULL);
QString         GRP_GRUP_ADLARINI_BUL           ( QList<int> * grup_idleri , QStringList * grup_adlari , QStringList * grup_kodlari);

#endif // ADAK_GRUPLAR_H
