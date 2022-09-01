#include "mpdparser.h"

#include <QMultiHash>
#include <QDebug>

QList<QSharedPointer<MpdObject>> MpdParser::parseData(const QByteArray& data)
{
    QByteArray firstKey = "";
    QMultiHash<QByteArray, QByteArray> entityHash;
    QList<QSharedPointer<MpdObject>> objectList;

    QByteArrayList lines = data.split('\n');

    for (int i = 0 ; i < lines.count() ; i++ ) {
        const QByteArray& line = lines.at(i);
        if (line.isEmpty()) {
            continue;
        }
        int firstColonPos = line.indexOf(':');

        const QByteArray& key = line.left(firstColonPos).toLower();
        const QByteArray& value = line.mid(firstColonPos + 2); // 2 = len(": ")
        // Define the first key to understand the entity we deal with
        if (firstKey.isEmpty()) {
            firstKey = key;
        // If we find the first key again, it means we start a new entity
        // So we first deal with the previous entity then we clear it (= initialization) to loop again
        } else if ((!firstKey.isEmpty() && key == firstKey)) {
            QSharedPointer<MpdObject> mpdObject = this->parseObject(firstKey, entityHash);
            objectList.append(mpdObject);
            entityHash.clear();
        // If we find a key twice (that is not the first key), it means that we are in a group
        // So we first deal with the previous entity. Do not loop as we want to keep the previous elements!
        } else if (entityHash.contains(key)) {
            QSharedPointer<MpdObject> mpdObject = this->parseObject(firstKey, entityHash);
            objectList.append(mpdObject);
        }
        entityHash.insert(key, value);
    }

    if (!entityHash.isEmpty()) {
        QSharedPointer<MpdObject> mpdObject = this->parseObject(firstKey, entityHash);
        objectList.append(mpdObject);
    }
    return objectList;
}

QSharedPointer<MpdObject> MpdParser::parseObject(const QString& mpdObjectKey,
                                                 const QMultiHash<QByteArray, QByteArray>& entityHash
                                                 )
{
    QSharedPointer<MpdObject> object = nullptr;
    if (mpdObjectKey == "file" or mpdObjectKey == "Title") {
        object = QSharedPointer<MpdObject>(new MpdSong(entityHash));
    } else if (mpdObjectKey == "directory") {
        object = QSharedPointer<MpdObject>(new MpdDirectory(entityHash));
    } else if (mpdObjectKey == "playlist") {
        object = QSharedPointer<MpdObject>(new MpdPlaylist(entityHash));
    } else if (mpdObjectKey == "artist") {
        object = QSharedPointer<MpdObject>(new MpdArtist(entityHash));
    } else if (mpdObjectKey == "album") {
        object = QSharedPointer<MpdObject>(new MpdAlbum(entityHash));
    } else if (mpdObjectKey == "id") {
        object = QSharedPointer<MpdObject>(new MpdId(entityHash));
    } else if (entityHash.contains(QByteArray("state"))) {
        object = QSharedPointer<MpdObject>(new MpdStatus(entityHash));
    } else {
        qDebug("Could not create entity from data '%s'", qPrintable(mpdObjectKey));
    }
    return QSharedPointer<MpdObject>(object);
}
