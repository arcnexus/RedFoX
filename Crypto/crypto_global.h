#ifndef CRYPTO_GLOBAL_H
#define CRYPTO_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(CRYPTO_LIBRARY)
#  define CRYPTOSHARED_EXPORT Q_DECL_EXPORT
#else
#  define CRYPTOSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // CRYPTO_GLOBAL_H
