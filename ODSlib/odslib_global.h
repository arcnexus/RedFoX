#ifndef ODSLIB_GLOBAL_H
#define ODSLIB_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(ODSLIB_LIBRARY)
#  define ODSLIBSHARED_EXPORT Q_DECL_EXPORT
#else
#  define ODSLIBSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // ODSLIB_GLOBAL_H
