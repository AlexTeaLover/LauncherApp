#ifndef DBHANDLER_GLOBAL_H
#define DBHANDLER_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(DBHANDLER_LIBRARY)
#  define DBHANDLER_EXPORT Q_DECL_EXPORT
#else
#  define DBHANDLER_EXPORT Q_DECL_IMPORT
#endif

#endif // DBHANDLER_GLOBAL_H
