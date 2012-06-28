#ifndef BELGE_KERENEL_DEFINES_H
#define BELGE_KERENEL_DEFINES_H

enum BELGE_ELEMENTS {
    HEADER      = 10,
    LINE        = 20,
    FOOTER      = 30,
    USER_TEXT   = 40,
    USER_IMG    = 50
};

enum ALIGN_ELEMENTS {
    LEFT_ALIGN      = 10,
    RIGHT_ALIGN     = 20,
    CENTER_ALIGN    = 30
};

enum PRINTER_TYPE {
    IBM     = 0,
    EPSON   = 1
};

enum PRINTER_QUALITY {
    NORMAL_QUALITY = 0,
    NLQ_QUALITY    = 1
};

enum PRINTER_MODE {
    NORMAL_MODE     = 0,
    CONDENSED_MODE  = 1,
    LARGE_MODE      = 2
};


#endif // BELGE_KERENEL_DEFINES_H
