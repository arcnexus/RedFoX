#include "crypto.h"

#include <QString>
//CryptoPP headers
#ifdef Q_OS_WIN32
#include <C:/CriptoCpp/modes.h>
#include <C:/CriptoCpp/filters.h>
#include <C:/CriptoCpp/aes.h>
#include <C:/CriptoCpp/sha.h>
#include <C:/CriptoCpp/base64.h>
#include <C:/CriptoCpp/hex.h>
#else
#include </usr/include/cryptopp/modes.h>
#include </usr/include/cryptopp/filters.h>
#include </usr/include/cryptopp/aes.h>
#include </usr/include/cryptopp/sha.h>
#include </usr/include/cryptopp/base64.h>
#include </usr/include/cryptopp/hex.h>
#endif


unsigned char Crypto::key[ CryptoPP::AES::DEFAULT_KEYLENGTH ];
unsigned char Crypto::iv[ CryptoPP::AES::BLOCKSIZE ];
QString Crypto::Pass = "RedFox";

using namespace CryptoPP;

Crypto::Crypto()
{
}

QString Crypto::Crypt(QString plaintext)
{
    setUpKeys();

    std::string text = plaintext.toStdString();
    std::string pass = Pass.toStdString();
    std::string CipherText;

    StringSource( reinterpret_cast<const char*>(pass.c_str()), true, new HashFilter(*(new SHA256), new ArraySink(key, AES::DEFAULT_KEYLENGTH)) );

    CBC_Mode<AES>::Encryption Encryptor( key, sizeof(key), iv );
    StringSource( text, true, new StreamTransformationFilter( Encryptor,new HexEncoder(new StringSink( CipherText ) ) ) );

    return QString::fromStdString(CipherText);
}

QString Crypto::DeCrypt(QString ciphertext)
{
    setUpKeys();

    std::string text = ciphertext.toStdString();
    std::string pass = Pass.toStdString();
    std::string RecoveredText;

    StringSource( reinterpret_cast<const char*>(pass.c_str()), true, new HashFilter(*(new SHA256), new ArraySink(key, AES::DEFAULT_KEYLENGTH)) );

    try
    {
        CBC_Mode<AES>::Decryption Decryptor( key, sizeof(key), iv );
        StringSource( text, true, new HexDecoder(new StreamTransformationFilter( Decryptor, new StringSink( RecoveredText ) ) ) );
    }
    catch ( Exception& e)
    {
        return e.what();
    }
    catch (...)
    {
        return "Unknown Error";
    }

    return QString::fromStdString(RecoveredText);
}

QString Crypto::SHA256HashString(QString input)
{
    std::string digest;
    std::string aString = input.toStdString();
    CryptoPP::SHA256 hash;

    CryptoPP::StringSource foo(aString, true,
    new CryptoPP::HashFilter(hash,
      new CryptoPP::Base64Encoder (
         new CryptoPP::StringSink(digest))));

    return QString::fromStdString(digest).trimmed();
}

void Crypto::setUpKeys()
{
    key[0] = 0x10;
    key[1] = 0x14;
    key[2] = 0x12;
    key[3] = 0x8;
    key[4] = 0x10;
    key[5] = 0x19;
    key[6] = 0x10;
    key[7] = 0x3;
    key[8] = 0x10;
    key[9] = 0x10;
    key[10] = 0x7;
    key[11] = 0x55;
    key[12] = 0x10;
    key[13] = 0x43;
    key[14] = 0x10;
    key[15] = 0x17;

    iv[0] = 0x10;
    iv[1] = 0x23;
    iv[2] = 0x10;
    iv[3] = 0x10;
    iv[4] = 0x67;
    iv[5] = 0xee;
    iv[6] = 0x10;
    iv[7] = 0xf5;
    iv[8] = 0x10;
    iv[9] = 0x89;
    iv[10] = 0x10;
    iv[11] = 0xdd;
    iv[12] = 0x10;
    iv[13] = 0x67;
    iv[14] = 0x10;
    iv[15] = 0xa3;
}
