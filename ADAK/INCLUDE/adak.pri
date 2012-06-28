CONFIG += qt debug
DEFINES += USING_PCH
OBJ_ext=o
libADAK=libADAK.a
LIB_ext  = a
SO_ext   = so
win32 {
    OBJDIR = MINGW
    DEFINES += MINGW
    win32-msvc2005|win32-msvc2008 { 
        OBJDIR = MSVC
        DEFINES -= MINGW
        DEFINES += MSVC
        LIB_ext  = lib
        OBJ_ext=obj
        libADAK=ADAK.lib
    }
    SO_ext   = dll
    DEFINES += WIN32
}
!win32 {
    macx {
        OBJDIR   = MACOSX
        SO_ext   = dylib
        DEFINES += MACOSX
    }
    !macx { 
        ADAK_ARCH=$$system('uname -m')
        !contains(ADAK_ARCH , x86_64) {
            OBJDIR   = LNX32
            DEFINES += LNX32
        }
        contains(ADAK_ARCH , x86_64) {
            OBJDIR   = LNX64
            DEFINES += LNX64
        }
        DEFINES += LINUX
    }
}

release_mode=$$(ADAK_RELEASE)
!isEmpty(release_mode) {
    CONFIG  -= debug
    CONFIG  += release
}

OBJECTS_DIR = $$OBJDIR
MOC_DIR     = $$OBJDIR
RCC_DIR     = $$OBJDIR
UI_DIR      = $$OBJDIR
DESTDIR     = $$OBJDIR
