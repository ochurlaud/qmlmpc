#ifndef MPDREQUEST_H
#define MPDREQUEST_H

#include <QObject>
#include <QSharedPointer>

#include "mpdparser.h"

class QString;

class MpdRequest : public QObject
{
    Q_OBJECT
public:
    MpdRequest(QString requestMessage);
    bool isResultReady() const;
    bool succesfull() const;
    QString getAck() const;
    QList<QSharedPointer<MpdObject>> getResponse();

    friend class MusicPlayerConnection;

signals:
    void resultReady();

protected:
    virtual void feedData(QByteArray data) { m_buffer += data; }
    void setOk();
    void setAck(const QString& msg);

    QString m_requestMessage;
    QByteArray m_buffer;
    QList<QSharedPointer<MpdObject>> m_response;

private:
    bool m_ready;
    QString m_ack;
};


#endif // MPDREQUEST_H
