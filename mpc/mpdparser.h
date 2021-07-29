#ifndef MPDPARSER_H
#define MPDPARSER_H

#include <QByteArray>
#include <QList>
#include <QSharedPointer>


#include "types.h"

class MpdParser
{
public:
    MpdParser(QByteArray data=QByteArray()) { parseData(data); }
    QList<QSharedPointer<MpdObject> > parseData(const QByteArray& data);
private:
    QSharedPointer<MpdObject> parseObject(const QString& mpdObjectKey,
                                          const QMultiHash<QByteArray, QByteArray>& entityHash);
};


#endif // MPDPARSER_H
