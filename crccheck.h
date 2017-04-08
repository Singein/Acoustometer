#ifndef CRCCHECK_H
#define CRCCHECK_H
#include "QString"
#include "QByteArray"

class CrcCheck
{
public:
    CrcCheck();
    QString crcChecksix(QString noCRCCode);
};

#endif // CRCCHECK_H
