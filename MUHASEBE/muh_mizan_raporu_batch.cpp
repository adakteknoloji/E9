#include <QDate>
#include "muh_mizan_raporu_batch_class.h"
#include "muh_mizan_raporu_batch_open.h"
#include "ui_muh_mizan_raporu_batch.h"
#include "print.h"
#include "muh_enum.h"
#include "muh_struct.h"
#include "e9_console_utils.h"
#include "e9_gui_utils.h"
#include "e9_enum.h"
#include "e9_log.h"
#include "yonetim.h"
#include "adak_gruplar.h"
#include "muh_hesap_arama_open.h"
#include "muh_mizan_raporu_open.h"
#include "muh_console_utils.h"

extern ADAK_SQL * DB;

/**************************************************************************************
                   OPEN_MIZAN_RAPORU_BATCH
***************************************************************************************/

void OPEN_MUH_MIZAN_RAPORU_BATCH ( QWidget * parent)
{
    MUH_MIZAN_RAPORU_BATCH * B = new MUH_MIZAN_RAPORU_BATCH ( parent );
    B->EXEC( NOT_FULL_SCREEN );
}

/**************************************************************************************
                   MUH_MIZAN_RAPORU_BATCH::MUH_MIZAN_RAPORU_BATCH
***************************************************************************************/

MUH_MIZAN_RAPORU_BATCH::MUH_MIZAN_RAPORU_BATCH ( QWidget *parent ) : BATCH_KERNEL ( parent ) , m_ui ( new Ui::MUH_MIZAN_RAPORU_BATCH )
{
    m_ui->setupUi               ( this );
    START_BATCH_KERNEL          ( this, DB );
}

/**************************************************************************************
                   MUH_MIZAN_RAPORU_BATCH::SETUP_FORM
***************************************************************************************/

void MUH_MIZAN_RAPORU_BATCH::SETUP_FORM()
{
    SET_HELP_PAGE       ( "mizan" );
    SET_PAGE_TITLE      ( "MUH - MİZAN" );

    REGISTER_BUTTON_WIDGET   ( m_ui->widget_batch_buttons );

    SET_SETTING_NAME             ( "MUH_MIZAN_RAPORU_BATCH" );
    SET_NAME_OF_RUN_BATCH_BUTTON ( tr ( "Rapor Al" ) );
    SET_ENTER_KEY_FOR_RUN_BATCH  ( true );

    SET_FIRST_FOCUS_WIDGET( m_ui->comboBox_mizan_donemi );
    m_grup_idleri.clear();

    SQL_QUERY sql_query ( DB );

    sql_query.PREPARE_SELECT("e9_kullanici_yetkileri" , "mizan_nakli_yekun" ,
                             "kullanici_id = :kullanici_id");
    sql_query.SET_VALUE(":kullanici_id" , KULLANICI_ID());

    int nakli_yekun = 1;

    if ( sql_query.SELECT() NE 0 ) {
        sql_query.NEXT();

        nakli_yekun = sql_query.VALUE(0).toInt();
    }

    m_ui->checkbox_nakli_yekun->setChecked(QVariant(nakli_yekun).toBool());

    m_ui->frame_tarih_araligi->setDisabled(true);



    sql_query.PREPARE_SELECT("muh_hesaplar" , "tam_hesap_kodu,hesap_ismi" );

    if (sql_query.SELECT("tam_hesap_kodu ASC" , 0,1) NE 0 ) {
        sql_query.NEXT();
        m_ui->search_edit_bas_hesap_kodu->SET_TEXT ( sql_query.VALUE(0).toString() );
        m_ui->lineedit_bas_hesap_ismi->setText ( sql_query.VALUE(1).toString() );

    }

    sql_query.PREPARE_SELECT("muh_hesaplar" , "tam_hesap_kodu,hesap_ismi" );

    if ( sql_query.SELECT("tam_hesap_kodu DESC" , 0,1) NE 0 ) {
        sql_query.NEXT();

        m_ui->search_edit_bts_hesap_kodu->SET_TEXT ( sql_query.VALUE(0).toString() );
        m_ui->lineedit_bts_hesap_ismi->setText ( sql_query.VALUE(1).toString() );
    }

    m_ui->spinbox_mizan_seviyesi->setMinimum(1);
    m_ui->spinbox_mizan_seviyesi->setMaximum(5);


    if ( E9_KULLANICI_SIRKET_DEGISTIRE_BILIR_MI() EQ EVET ) {
        m_ui->comboBox_sirket->setVisible(true);
        m_ui->label_sirket->setVisible(true);
    }


    DISABLE_CLEAR_ON_WIDGET(m_ui->comboBox_mizan_donemi);


    FILL_COMBOBOX_MIZAN_DONEMI(m_ui->comboBox_mizan_donemi);



    if ( QDate::currentDate().year() EQ MALI_YIL_FIRST_DATE().year() OR
         QDate::currentDate().year() EQ MALI_ARRAY_LAST_DATE().year() ) {
        m_current_mizan_donemi = GET_MZN_DONEMI_AY_STR(QDate::currentDate().month()) +
                               QVariant(QDate::currentDate().year()).toString();
    }
    else {
        m_current_mizan_donemi = GET_MZN_DONEMI_AY_STR(MALI_YIL_365_DATE().month()) +
                               QVariant(MALI_YIL_365_DATE().year()).toString();
    }



    m_ui->comboBox_mizan_donemi->setCurrentIndex(m_ui->comboBox_mizan_donemi->findText( m_current_mizan_donemi));

    GET_MZN_DONEMINE_GORE_BAS_BTS_TARIHI(m_current_mizan_donemi , m_ui->adakDate_baslangic_tarihi,
                                         m_ui->adakDate_bts_tarihi);

    if ( nakli_yekun EQ 1 ) {
        m_ui->adakDate_baslangic_tarihi->SET_DATE(MALI_YIL_FIRST_DATE());
        m_ui->adakDate_baslangic_tarihi->setDisabled(true);
    }


}

/**************************************************************************************
                   MUH_MIZAN_RAPORU_BATCH::CHECK_VAR
***************************************************************************************/

int MUH_MIZAN_RAPORU_BATCH::CHECK_VAR ( QObject * object )
{
    SQL_QUERY   sql_query ( DB );

    if ( object EQ m_ui->adakDate_baslangic_tarihi OR object EQ m_ui->adakDate_bts_tarihi ) {

        int current_year = 0;

        if ( QDate::currentDate().year() EQ MALI_YIL_FIRST_DATE().year() OR
             QDate::currentDate().year() EQ MALI_YIL_365_DATE().year() ) {
            current_year  = m_ui->adakDate_bts_tarihi->QDATE().year();
        }
        else {
            current_year  = MALI_YIL_FIRST_DATE().year();
        }

         m_ui->comboBox_mizan_donemi->setCurrentIndex(m_ui->comboBox_mizan_donemi->findText(GET_MZN_DONEMI_AY_STR(m_ui->adakDate_bts_tarihi->QDATE().month())+ QVariant(current_year).toString()));
    }
    else if ( object EQ m_ui->search_edit_bas_hesap_kodu OR object EQ m_ui->search_edit_bts_hesap_kodu ) {

        if ( object EQ m_ui->search_edit_bas_hesap_kodu ) {
            if ( m_ui->search_edit_bas_hesap_kodu->GET_TEXT().isEmpty() EQ true ) {
                m_ui->lineedit_bas_hesap_ismi->clear();
                return ADAK_OK;
            }
        }
        else {
            if ( m_ui->search_edit_bts_hesap_kodu->GET_TEXT().isEmpty() EQ true ) {
                m_ui->lineedit_bts_hesap_ismi->clear();
                return ADAK_OK;
            }
        }


        sql_query.PREPARE_SELECT("muh_hesaplar" , "hesap_ismi" , "tam_hesap_kodu = :tam_hesap_kodu");

        if ( object EQ m_ui->search_edit_bas_hesap_kodu ) {
            sql_query.SET_VALUE  ( ":tam_hesap_kodu", m_ui->search_edit_bas_hesap_kodu->GET_TEXT() );
        }
        else {
            sql_query.SET_VALUE  ( ":tam_hesap_kodu", m_ui->search_edit_bts_hesap_kodu->GET_TEXT() );
        }

        if ( sql_query.SELECT() EQ 0 ) {
           MSG_WARNING( tr ( "Hesap Kodu Bulunamadı!..." ), NULL );

            return ADAK_FAIL;
        }
        sql_query.NEXT();

        if ( object EQ m_ui->search_edit_bas_hesap_kodu ) {
            m_ui->lineedit_bas_hesap_ismi->setText ( sql_query.VALUE(0).toString() );
        }
        else {
            m_ui->lineedit_bts_hesap_ismi->setText ( sql_query.VALUE(0).toString() );
        }
    }
    else if ( object EQ m_ui->checkbox_hesap_araligi ) {

         if ( m_ui->checkbox_hesap_araligi->isChecked() EQ false ) {

            sql_query.PREPARE_SELECT("muh_hesaplar" , "tam_hesap_kodu, hesap_ismi" , "parent_id = :parent_id");
            sql_query.SET_VALUE(":parent_id" , 0);

            if ( sql_query.SELECT("tam_hesap_kodu ASC" , 0,1) NE 0 ) {
                sql_query.NEXT();
                m_ui->search_edit_bas_hesap_kodu->SET_TEXT(sql_query.VALUE(0).toString() ) ;
                m_ui->lineedit_bas_hesap_ismi->setText ( sql_query.VALUE(1).toString() );

            }
            sql_query.PREPARE_SELECT("muh_hesaplar" , "tam_hesap_kodu, hesap_ismi" , "parent_id = :parent_id");
            sql_query.SET_VALUE(":parent_id" , 0);

            if ( sql_query.SELECT("tam_hesap_kodu DESC" , 0,1) NE 0 ) {
                sql_query.NEXT();
                m_ui->search_edit_bts_hesap_kodu->SET_TEXT ( sql_query.VALUE(0).toString() );
                m_ui->lineedit_bts_hesap_ismi->setText ( sql_query.VALUE(1).toString() );
            }
            m_ui->search_edit_bas_hesap_kodu->setDisabled ( true );
            m_ui->search_edit_bts_hesap_kodu->setDisabled ( true );
            m_ui->lineedit_bas_hesap_ismi->setDisabled ( true );
            m_ui->lineedit_bts_hesap_ismi->setDisabled ( true );
        }
        else {
            m_ui->search_edit_bas_hesap_kodu->setEnabled ( true );
            m_ui->search_edit_bts_hesap_kodu->setEnabled ( true );
            m_ui->lineedit_bas_hesap_ismi->setEnabled ( true );
            m_ui->lineedit_bts_hesap_ismi->setEnabled ( true );
            SET_FOCUS( m_ui->search_edit_bas_hesap_kodu );
        }
    }
    else if ( object EQ m_ui->checkbox_tarih_araligi ) {

        if ( m_ui->checkbox_tarih_araligi->isChecked() EQ false ) {
            m_ui->frame_tarih_araligi->setDisabled(true);
            m_ui->comboBox_mizan_donemi->setCurrentIndex(m_ui->comboBox_mizan_donemi->findText(m_current_mizan_donemi));
            m_ui->frame_mizan_donemi->setEnabled(true);
            m_ui->checkbox_mizan_donemi->setChecked(true);

        }
        else {
            m_ui->frame_tarih_araligi->setEnabled(true);
            SET_FOCUS( m_ui->adakDate_baslangic_tarihi );
            m_ui->frame_mizan_donemi->setDisabled(true);
            m_ui->checkbox_mizan_donemi->setChecked(false);
            m_ui->adakDate_baslangic_tarihi->setEnabled( true );

            m_ui->comboBox_mizan_donemi->setCurrentIndex(m_ui->comboBox_mizan_donemi->findText(m_current_mizan_donemi));
            GET_MZN_DONEMINE_GORE_BAS_BTS_TARIHI(m_ui->comboBox_mizan_donemi->currentText() ,
                                                 m_ui->adakDate_baslangic_tarihi, m_ui->adakDate_bts_tarihi);
            if ( m_ui->checkbox_nakli_yekun->isChecked() EQ true ) {
                m_ui->adakDate_baslangic_tarihi->SET_DATE(MALI_YIL_FIRST_DATE());
                m_ui->adakDate_baslangic_tarihi->setDisabled(true);
            }
            else {
                m_ui->adakDate_baslangic_tarihi->setEnabled(true);
            }
        }
    }
    else if ( object EQ m_ui->checkbox_mizan_donemi ) {
        if ( m_ui->checkbox_mizan_donemi->isChecked() EQ true ) {
            m_ui->checkbox_tarih_araligi->setChecked(false);
            m_ui->frame_tarih_araligi->setDisabled(true);
            SET_FOCUS(m_ui->comboBox_mizan_donemi);
            m_ui->frame_mizan_donemi->setEnabled(true);

            GET_MZN_DONEMINE_GORE_BAS_BTS_TARIHI(m_ui->comboBox_mizan_donemi->currentText() ,
                                                 m_ui->adakDate_baslangic_tarihi, m_ui->adakDate_bts_tarihi);

            if ( m_ui->checkbox_nakli_yekun->isChecked() EQ true ) {
                m_ui->adakDate_baslangic_tarihi->SET_DATE(MALI_YIL_FIRST_DATE());
                m_ui->adakDate_baslangic_tarihi->setDisabled(true);
            }
        }
        else {
            m_ui->checkbox_tarih_araligi->setChecked(true);
            m_ui->frame_tarih_araligi->setEnabled(true);
            SET_FOCUS(m_ui->adakDate_baslangic_tarihi);
            m_ui->checkbox_mizan_donemi->setChecked(false);
            m_ui->frame_mizan_donemi->setDisabled(true);

            m_ui->comboBox_mizan_donemi->setCurrentIndex(m_ui->comboBox_mizan_donemi->findText(m_current_mizan_donemi));

        }
    }
    else if ( object EQ m_ui->comboBox_mizan_donemi ) {

        GET_MZN_DONEMINE_GORE_BAS_BTS_TARIHI(m_ui->comboBox_mizan_donemi->currentText() ,
                                             m_ui->adakDate_baslangic_tarihi, m_ui->adakDate_bts_tarihi);
        if ( m_ui->checkbox_nakli_yekun->isChecked() EQ true ) {
            m_ui->adakDate_baslangic_tarihi->SET_DATE(MALI_YIL_FIRST_DATE());
            m_ui->adakDate_baslangic_tarihi->setDisabled(true);
        }
    }
    else if ( object EQ m_ui->checkbox_bakiyesiz_hesaplar ) {

        if ( m_ui->checkbox_bakiyesiz_hesaplar->isChecked() EQ false ) {
            m_ui->label_bakiyesiz_hesaplar->setDisabled ( true );
        }
        else {
            m_ui->label_bakiyesiz_hesaplar->setEnabled ( true );
        }
    }
    else if ( object EQ m_ui->checkbox_calismamis_hesaplar ) {

        if ( m_ui->checkbox_calismamis_hesaplar->isChecked() EQ false ) {
            m_ui->label_calismamis_hesaplar->setDisabled ( true );
        }
        else {
            m_ui->label_calismamis_hesaplar->setEnabled ( true );
        }
    }
    else if ( object EQ m_ui->checkbox_nakli_yekun ) {

        if ( m_ui->checkbox_nakli_yekun->isChecked() EQ false ) {
            m_ui->label_nakli_yekun->setDisabled ( true );
            m_ui->adakDate_baslangic_tarihi->setEnabled(true);

            int current_year = 0;
            int current_month = 0;

            if ( QDate::currentDate().year() EQ MALI_YIL_FIRST_DATE().year() OR
                 QDate::currentDate().year() EQ MALI_YIL_365_DATE().year() ) {
                current_month = QDate::currentDate().month();
                current_year  = QDate::currentDate().year();
            }
            else {
                current_month = MALI_YIL_FIRST_DATE().month();
                current_year  = MALI_YIL_FIRST_DATE().year();
            }



            int mizan_donemi_ay = current_month;

            if ( m_ui->adakDate_bts_tarihi->QDATE().month() NE current_month) {
                mizan_donemi_ay = m_ui->adakDate_bts_tarihi->QDATE().month();

                m_ui->adakDate_baslangic_tarihi->SET_DATE(QDate(m_ui->adakDate_bts_tarihi->QDATE().year() , mizan_donemi_ay , 01));

            }
            else {

                GET_MZN_DONEMINE_GORE_BAS_BTS_TARIHI(m_ui->comboBox_mizan_donemi->currentText() ,
                                                 m_ui->adakDate_baslangic_tarihi, m_ui->adakDate_bts_tarihi);
            }

            m_ui->comboBox_mizan_donemi->setCurrentIndex(m_ui->comboBox_mizan_donemi->findText(
                    GET_MZN_DONEMI_AY_STR(mizan_donemi_ay)
                    + QVariant(current_year).toString()));

        }
        else {
            m_ui->label_nakli_yekun->setEnabled ( true );
            m_ui->adakDate_baslangic_tarihi->SET_DATE(MALI_YIL_FIRST_DATE());
            m_ui->adakDate_baslangic_tarihi->setDisabled(true);
        }
    }
    else if ( object EQ m_ui->check_box_hesap_grubu ) {

        if ( m_ui->check_box_hesap_grubu->isChecked() EQ false) {
            m_ui->frame_hesap_grubu->setEnabled ( false );
            m_ui->textEdit_gruplar->clear();
            m_grup_idleri.clear();

        }
        else {
            m_ui->frame_hesap_grubu->setEnabled ( true );
            m_grup_idleri.clear();
            SET_FOCUS( m_ui->pushButton_grup_sec );
        }
    }
    else if ( object EQ m_ui->pushButton_grup_sec ) {
        OPEN_GRUP_SECIM(E9_PROGRAMI,MUHASEBE_MODULU,&m_grup_idleri,DB,this,true);
        QStringList grup_kodlari;
        QStringList grup_adlari;
        m_ui->textEdit_gruplar->setText(GRP_GRUP_ADLARINI_BUL(&m_grup_idleri,&grup_adlari,&grup_kodlari));
    }

    return ADAK_OK;
}

/**************************************************************************************
                   MUH_MIZAN_RAPORU_BATCH::CHECK_RUN
***************************************************************************************/

int MUH_MIZAN_RAPORU_BATCH::CHECK_RUN()
{
    if ( m_ui->search_edit_bas_hesap_kodu->GET_TEXT().isEmpty() EQ false AND
         m_ui->search_edit_bts_hesap_kodu->GET_TEXT().isEmpty() EQ false ) {
        if ( m_ui->search_edit_bas_hesap_kodu->GET_TEXT() > m_ui->search_edit_bts_hesap_kodu->GET_TEXT()  ) {
            MSG_WARNING( tr ( "Başlangıç hesap kodu ,bitiş hesap kodundan büyük olamaz!..."), m_ui->search_edit_bas_hesap_kodu );
            return ADAK_FAIL;
        }
    }
    if ( E9_MALI_YIL_TARIH_ARALIGI_KONTROLU( m_ui->adakDate_baslangic_tarihi, m_ui->adakDate_bts_tarihi ) NE ADAK_OK ) {
        return ADAK_FAIL;
    }

    if(m_ui->check_box_hesap_grubu->isChecked() EQ true ) {
        if(m_ui->textEdit_gruplar->toPlainText().isEmpty() EQ true ) {
            MSG_WARNING( tr("Hesap grubunu boş bırakamazsınız."), m_ui->pushButton_grup_sec );

            return ADAK_FAIL;
        }
    }

    return ADAK_OK;
}

/**************************************************************************************
                   MUH_MIZAN_RAPORU_BATCH::RUN_BATCH
***************************************************************************************/

void MUH_MIZAN_RAPORU_BATCH::RUN_BATCH()
{
    MUH_RAPOR_VARS * MUH_RV   = new MUH_RAPOR_VARS;

    MUH_RV->bas_hesap_kodu      =  m_ui->search_edit_bas_hesap_kodu->GET_TEXT();
    MUH_RV->bts_hesap_kodu      =  m_ui->search_edit_bts_hesap_kodu->GET_TEXT();
    MUH_RV->mizan_seviyesi      =  m_ui->spinbox_mizan_seviyesi->value();
    MUH_RV->nakli_yekun         =  m_ui->checkbox_nakli_yekun->isChecked();
    MUH_RV->bas_gun_no          =  m_ui->adakDate_baslangic_tarihi->MALI_YIL_ARRAY_INDIS();
    MUH_RV->bts_gun_no          =  m_ui->adakDate_bts_tarihi->MALI_YIL_ARRAY_INDIS();
    MUH_RV->calismamis_hesaplar =  m_ui->checkbox_calismamis_hesaplar->isChecked();
    MUH_RV->bakiyesiz_hesaplar  =  m_ui->checkbox_bakiyesiz_hesaplar->isChecked();
    MUH_RV->baslangic_tarihi    =  m_ui->adakDate_baslangic_tarihi->QDATE();
    MUH_RV->bitis_tarihi        =  m_ui->adakDate_bts_tarihi->QDATE();

    QString log_detaylari = QObject::tr ( "Mizan , \nHesap Aralığı : " ) +
                            m_ui->search_edit_bas_hesap_kodu->GET_TEXT() + " " + m_ui->lineedit_bas_hesap_ismi->text()
                            + " - " + m_ui->search_edit_bts_hesap_kodu->GET_TEXT() + " " +
                            m_ui->lineedit_bts_hesap_ismi->text() + "\n" + QObject::tr("Tarih Aralığı    : " ) +
                            m_ui->adakDate_baslangic_tarihi->QDATE().toString("dd MMMM yyyy") + " - " +
                            m_ui->adakDate_bts_tarihi->QDATE().toString("dd MMMM yyyy") + "\n";

    if ( MUH_RV->nakli_yekun EQ true ) {
        log_detaylari.append ( "Nakli Yekün " );
    }

    if ( MUH_RV->calismamis_hesaplar NE 0 ) {
        if ( MUH_RV->nakli_yekun EQ true ) {
            log_detaylari.append ( " , " );
        }
        log_detaylari.append ( "Çalışmamış Hesaplar " );
    }
    if ( MUH_RV->bakiyesiz_hesaplar EQ true ) {
        if ( MUH_RV->calismamis_hesaplar NE 0 OR MUH_RV->nakli_yekun EQ true ) {
            log_detaylari.append ( " , " );
        }
        log_detaylari.append ( "Bakiyesiz Hesaplar " );
    }
    DB->START_TRANSACTION   ();
    E9_LOG_KAYDI_EKLE ( MUHASEBE_MODULU , LOG_MUH_RAPORLAR ,LOG_ISLEM_RAPOR_ALINDI, log_detaylari );
    DB->COMMIT_TRANSACTION  ();

    OPEN_REPORT_SHOWER  ( GET_MIZAN_RAPORU ( m_grup_idleri , MUH_RV ), nativeParentWidget() );

    delete MUH_RV;
}

/**************************************************************************************
                   MUH_MIZAN_RAPORU_BATCH::SEARCH_EDIT_CLICKED
***************************************************************************************/

void MUH_MIZAN_RAPORU_BATCH::SEARCH_EDIT_CLICKED ( QWidget * p_widget, QLineEdit * p_lineEdit )
{
    Q_UNUSED(p_widget);

    SQL_QUERY sql_query ( DB );

    int hesap_id = OPEN_MUH_HESAP_ARAMA ( p_lineEdit->text(), this );

    if ( hesap_id EQ -1 ) {
        return;
    }
    p_lineEdit->setText ( MUH_GET_HESAP_KODU(hesap_id) );
}

/**************************************************************************************
                   MUH_MIZAN_RAPORU_BATCH:GET_MZN_DONEMINE_GORE_BAS_BTS_TARIHI
***************************************************************************************/

void MUH_MIZAN_RAPORU_BATCH::GET_MZN_DONEMINE_GORE_BAS_BTS_TARIHI(QString mizan_donemi,
                                                                  QAdakDate *bas_tarihi, QAdakDate *bts_tarihi)
{

    int muh_donem_yili        = MALI_YIL_FIRST_DATE().year();
    int muh_donem_devir_yili  = MALI_YIL_365_DATE().year();

    if ( mizan_donemi EQ QObject::tr("OCAK ") + QVariant(muh_donem_yili).toString()) {
        int gun_sayisi = MALI_YIL_FIRST_DATE().daysInMonth();

        int ay         = MALI_YIL_FIRST_DATE().month();

        bas_tarihi->SET_DATE(QDate(muh_donem_yili,ay,01));
        bts_tarihi->SET_DATE(QDate(muh_donem_yili,ay,gun_sayisi));
    }
    else if ( mizan_donemi EQ QObject::tr("ŞUBAT ") + QVariant(muh_donem_yili).toString()) {

        int gun_sayisi = MALI_YIL_FIRST_DATE().addMonths(1).daysInMonth();

        int ay         = MALI_YIL_FIRST_DATE().addMonths(1).month();

        bas_tarihi->SET_DATE(QDate(muh_donem_yili,ay,01));
        bts_tarihi->SET_DATE(QDate(muh_donem_yili,ay,gun_sayisi));
    }
    else if ( mizan_donemi EQ QObject::tr("MART ") + QVariant(muh_donem_yili).toString()) {
        int gun_sayisi = MALI_YIL_FIRST_DATE().addMonths(2).daysInMonth();

        int ay         = MALI_YIL_FIRST_DATE().addMonths(2).month();

        bas_tarihi->SET_DATE(QDate(muh_donem_yili,ay,01));
        bts_tarihi->SET_DATE(QDate(muh_donem_yili,ay,gun_sayisi));
    }
    else if ( mizan_donemi EQ QObject::tr("NİSAN ") + QVariant(muh_donem_yili).toString()) {
        int gun_sayisi = MALI_YIL_FIRST_DATE().addMonths(3).daysInMonth();

        int ay         = MALI_YIL_FIRST_DATE().addMonths(3).month();

        bas_tarihi->SET_DATE(QDate(muh_donem_yili,ay,01));
        bts_tarihi->SET_DATE(QDate(muh_donem_yili,ay,gun_sayisi));
    }
    else if ( mizan_donemi EQ QObject::tr("MAYIS ") + QVariant(muh_donem_yili).toString()) {
        int gun_sayisi = MALI_YIL_FIRST_DATE().addMonths(4).daysInMonth();

        int ay         = MALI_YIL_FIRST_DATE().addMonths(4).month();

        bas_tarihi->SET_DATE(QDate(muh_donem_yili,ay,01));
        bts_tarihi->SET_DATE(QDate(muh_donem_yili,ay,gun_sayisi));
    }
    else if ( mizan_donemi EQ QObject::tr("HAZİRAN ") + QVariant(muh_donem_yili).toString()) {
        int gun_sayisi = MALI_YIL_FIRST_DATE().addMonths(5).daysInMonth();

        int ay         = MALI_YIL_FIRST_DATE().addMonths(5).month();

        bas_tarihi->SET_DATE(QDate(muh_donem_yili,ay,01));
        bts_tarihi->SET_DATE(QDate(muh_donem_yili,ay,gun_sayisi));
    }
    else if ( mizan_donemi EQ QObject::tr("TEMMUZ ") + QVariant(muh_donem_yili).toString()) {
        int gun_sayisi = MALI_YIL_FIRST_DATE().addMonths(6).daysInMonth();

        int ay         = MALI_YIL_FIRST_DATE().addMonths(6).month();

        bas_tarihi->SET_DATE(QDate(muh_donem_yili,ay,01));
        bts_tarihi->SET_DATE(QDate(muh_donem_yili,ay,gun_sayisi));
    }
    else if ( mizan_donemi EQ QObject::tr("AĞUSTOS ") + QVariant(muh_donem_yili).toString()) {
        int gun_sayisi = MALI_YIL_FIRST_DATE().addMonths(7).daysInMonth();

        int ay         = MALI_YIL_FIRST_DATE().addMonths(7).month();

        bas_tarihi->SET_DATE(QDate(muh_donem_yili,ay,01));
        bts_tarihi->SET_DATE(QDate(muh_donem_yili,ay,gun_sayisi));
    }
    else if ( mizan_donemi EQ QObject::tr("EYLÜL ") + QVariant(muh_donem_yili).toString()) {
        int gun_sayisi = MALI_YIL_FIRST_DATE().addMonths(8).daysInMonth();

        int ay         = MALI_YIL_FIRST_DATE().addMonths(8).month();

        bas_tarihi->SET_DATE(QDate(muh_donem_yili,ay,01));
        bts_tarihi->SET_DATE(QDate(muh_donem_yili,ay,gun_sayisi));
    }
    else if ( mizan_donemi EQ QObject::tr("EKİM ") + QVariant(muh_donem_yili).toString()) {
        int gun_sayisi = MALI_YIL_FIRST_DATE().addMonths(9).daysInMonth();

        int ay         = MALI_YIL_FIRST_DATE().addMonths(9).month();

        bas_tarihi->SET_DATE(QDate(muh_donem_yili,ay,01));
        bts_tarihi->SET_DATE(QDate(muh_donem_yili,ay,gun_sayisi));
    }
    else if ( mizan_donemi EQ QObject::tr("KASIM ") + QVariant(muh_donem_yili).toString()) {
        int gun_sayisi = MALI_YIL_FIRST_DATE().addMonths(10).daysInMonth();

        int ay         = MALI_YIL_FIRST_DATE().addMonths(10).month();

        bas_tarihi->SET_DATE(QDate(muh_donem_yili,ay,01));
        bts_tarihi->SET_DATE(QDate(muh_donem_yili,ay,gun_sayisi));
    }
    else if ( mizan_donemi EQ QObject::tr("ARALIK ") + QVariant(muh_donem_yili).toString()) {
        int gun_sayisi = MALI_YIL_FIRST_DATE().addMonths(11).daysInMonth();

        int ay         = MALI_YIL_FIRST_DATE().addMonths(11).month();

        bas_tarihi->SET_DATE(QDate(muh_donem_yili,ay,01));
        bts_tarihi->SET_DATE(QDate(muh_donem_yili,ay,gun_sayisi));
    }
    else if ( mizan_donemi EQ QObject::tr("OCAK ") + QVariant(muh_donem_devir_yili).toString()) {
        int gun_sayisi = MALI_YIL_365_DATE().addMonths(-4).daysInMonth();

        int ay         = MALI_YIL_365_DATE().addMonths(-4).month();

        bas_tarihi->SET_DATE(QDate(muh_donem_devir_yili,ay,01));
        bts_tarihi->SET_DATE(QDate(muh_donem_devir_yili,ay,gun_sayisi));
    }
    else if ( mizan_donemi EQ QObject::tr("ŞUBAT ") + QVariant(muh_donem_devir_yili).toString()) {
        int gun_sayisi = MALI_YIL_365_DATE().addMonths(-3).daysInMonth();

        int ay         = MALI_YIL_365_DATE().addMonths(-3).month();

        bas_tarihi->SET_DATE(QDate(muh_donem_devir_yili,ay,01));
        bts_tarihi->SET_DATE(QDate(muh_donem_devir_yili,ay,gun_sayisi));
    }
    else if ( mizan_donemi EQ QObject::tr("MART ") + QVariant(muh_donem_devir_yili).toString()) {
        int gun_sayisi = MALI_YIL_365_DATE().addMonths(-2).daysInMonth();

        int ay         = MALI_YIL_365_DATE().addMonths(-2).month();

        bas_tarihi->SET_DATE(QDate(muh_donem_devir_yili,ay,01));
        bts_tarihi->SET_DATE(QDate(muh_donem_devir_yili,ay,gun_sayisi));
    }
    else if ( mizan_donemi EQ QObject::tr("NİSAN ") + QVariant(muh_donem_devir_yili).toString()) {
        int gun_sayisi = MALI_YIL_365_DATE().addMonths(-1).daysInMonth();

        int ay         = MALI_YIL_365_DATE().addMonths(-1).month();

        bas_tarihi->SET_DATE(QDate(muh_donem_devir_yili,ay,01));
        bts_tarihi->SET_DATE(QDate(muh_donem_devir_yili,ay,gun_sayisi));
    }
    else if ( mizan_donemi EQ QObject::tr("MAYIS ") + QVariant(muh_donem_devir_yili).toString()) {
        int gun_sayisi = MALI_YIL_365_DATE().day();

        int ay         = MALI_YIL_365_DATE().month();

        bas_tarihi->SET_DATE(QDate(muh_donem_devir_yili,ay,01));
        bts_tarihi->SET_DATE(QDate(muh_donem_devir_yili,ay,gun_sayisi));
    }
}

/**************************************************************************************
                   MUH_MIZAN_RAPORU_BATCH:GET_MZN_DONEMI_FROM_BAS_TARIHI
***************************************************************************************/

QString MUH_MIZAN_RAPORU_BATCH::GET_MZN_DONEMI_AY_STR(int month)
{
    QString mizan_donemi_ay = "";

    switch (month) {
        case 1 :
            mizan_donemi_ay = QObject::tr("OCAK ");
            break;
        case 2 :
            mizan_donemi_ay = QObject::tr("ŞUBAT ");
            break;
        case 3 :
            mizan_donemi_ay = QObject::tr("MART ");
            break;
        case 4 :
            mizan_donemi_ay = QObject::tr("NİSAN ");
            break;
        case 5 :
            mizan_donemi_ay = QObject::tr("MAYIS ");
            break;
        case 6 :
            mizan_donemi_ay = QObject::tr("HAZİRAN ");
            break;
        case 7 :
            mizan_donemi_ay = QObject::tr("TEMMUZ ");
            break;
        case 8 :
            mizan_donemi_ay = QObject::tr("AĞUSTOS ");
            break;
        case 9 :
            mizan_donemi_ay = QObject::tr("EYLÜL ");
            break;
        case 10 :
            mizan_donemi_ay = QObject::tr("EKİM ");
            break;
        case 11 :
            mizan_donemi_ay = QObject::tr("KASIM ");
            break;
        case 12 :
            mizan_donemi_ay = QObject::tr("ARALIK ");
            break;

    }
    return mizan_donemi_ay;
}
/**************************************************************************************
                   MUH_MIZAN_RAPORU_BATCH:FILL_COMBOBOX_MIZAN_DONEMI
***************************************************************************************/

void MUH_MIZAN_RAPORU_BATCH::FILL_COMBOBOX_MIZAN_DONEMI(QComboBox *combobox_mizan_donemi)
{
    QString muh_donem_yili        = QVariant(MALI_YIL_FIRST_DATE().year()).toString();
    QString muh_donem_devir_yili  = QVariant(MALI_YIL_365_DATE().year()).toString();

    QStringList mizan_donemleri;

    mizan_donemleri << QObject::tr("OCAK ")    + muh_donem_yili;
    mizan_donemleri << QObject::tr("ŞUBAT ")   + muh_donem_yili;
    mizan_donemleri << QObject::tr("MART ")    + muh_donem_yili;
    mizan_donemleri << QObject::tr("NİSAN ")   + muh_donem_yili;
    mizan_donemleri << QObject::tr("MAYIS ")   + muh_donem_yili;
    mizan_donemleri << QObject::tr("HAZİRAN ") + muh_donem_yili;
    mizan_donemleri << QObject::tr("TEMMUZ ")  + muh_donem_yili;
    mizan_donemleri << QObject::tr("AĞUSTOS ") + muh_donem_yili;
    mizan_donemleri << QObject::tr("EYLÜL ")   + muh_donem_yili;
    mizan_donemleri << QObject::tr("EKİM ")    + muh_donem_yili;
    mizan_donemleri << QObject::tr("KASIM ")   + muh_donem_yili;
    mizan_donemleri << QObject::tr("ARALIK ")  + muh_donem_yili;

    if ( QDate::currentDate().year() EQ MALI_YIL_FIRST_DATE().year() OR
         QDate::currentDate().year() EQ MALI_YIL_365_DATE().year() OR
         QDate::currentDate().year() + 1 EQ MALI_YIL_FIRST_DATE().year()) {

        mizan_donemleri << QObject::tr("OCAK ")    + muh_donem_devir_yili;
        mizan_donemleri << QObject::tr("ŞUBAT ")   + muh_donem_devir_yili;
        mizan_donemleri << QObject::tr("MART ")    + muh_donem_devir_yili;
        mizan_donemleri << QObject::tr("NİSAN ")   + muh_donem_devir_yili;
        mizan_donemleri << QObject::tr("MAYIS ")   + muh_donem_devir_yili;
    }

    combobox_mizan_donemi->addItems(mizan_donemleri);
}



