#include "mpdrequest.h"

#include <QString>

MpdRequest::MpdRequest(QString requestMessage) : m_requestMessage(requestMessage), m_ready(false)
{

}

bool MpdRequest::isResultReady() const
{
    return m_ready;
}

bool MpdRequest::succesfull() const
{
    if (m_ready) {
        return m_ack.isEmpty();
    } else {
        return false;
    }
}

QString MpdRequest::getAck() const
{
    return m_ack;
}

QList<QSharedPointer<MpdObject>> MpdRequest::getResponse()
{
    return m_response;
}

