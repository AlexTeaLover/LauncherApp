#ifndef TESTERPLUGIN_GLOBAL_H
#define TESTERPLUGIN_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(TESTERPLUGIN_LIBRARY)
#  define TESTERPLUGIN_EXPORT Q_DECL_EXPORT
#else
#  define TESTERPLUGIN_EXPORT Q_DECL_IMPORT
#endif

#endif // TESTERPLUGIN_GLOBAL_H
