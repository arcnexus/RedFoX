#ifndef CRYPTO_H
#define CRYPTO_H

#include "crypto_global.h"

class CRYPTOSHARED_EXPORT Crypto
{

public:
    Crypto();
    static QString Crypt(QString input);
    static QString DeCrypt(QString input);
    static QString SHA256HashString(QString input);
private:
    static unsigned char key[ ];
    static unsigned char iv [ ];
    static QString Pass;
    static void setUpKeys();
};

#endif // CRYPTO_H
