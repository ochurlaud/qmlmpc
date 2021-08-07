/*
 * Copyright 2015, Maarten Sebregts <maartensebregts AT gmail DOT com>
 *
 * This file is part of qmlmpc.
 *
 * qmlmpc is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * qmlmpc is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with qmlmpc. If not, see <http://www.gnu.org/licenses/>.
 */

#include "musicplayerconnection.h"

#include <QTcpSocket>
#include <QTimer>
#include <QStringList>

#include "mpdrequest.h"

MusicPlayerConnection::MusicPlayerConnection(const QString& host, int port, const QString& password, QObject *parent) :
    QObject(parent),
    p_waitingRequest(0),
    m_connected(false)
{
    m_host = host;
    m_port = port;
    m_password = password;
    p_socket = new QTcpSocket(this);

    connect(p_socket, SIGNAL(readyRead()), this, SLOT(dataReady()));
    connect(p_socket, SIGNAL(connected()), SLOT(onConnected()));
    connect(p_socket, SIGNAL(disconnected()), SLOT(onDisconnected()));
    connect(p_socket, SIGNAL(errorOccurred(QAbstractSocket::SocketError)), SLOT(onSocketError(QAbstractSocket::SocketError)));

    p_timer = new QTimer(this);
    connect(p_timer, SIGNAL(timeout()), this, SLOT(renewStatus()));
    p_timer->setInterval(100);

    QTimer *sendMessagesTimer = new QTimer(this);
    connect(sendMessagesTimer, SIGNAL(timeout()), SLOT(sendNextRequest()));
    sendMessagesTimer->setInterval(1000); // if a sendNextRequest had failed, it won't be resend without this timer
    sendMessagesTimer->start();

    p_socket->connectToHost(host, port);
}

MpdRequest *MusicPlayerConnection::request(const QString &mpdCommand)
{
    if (mpdCommand != "status") {
        qDebug() << "MPD Command:" << mpdCommand;
    }
    MpdRequest *request = new MpdRequest(mpdCommand);
    enqueueRequest(request);
    return request;
}

MpdRequest *MusicPlayerConnection::getStatus()
{
    QString mpdCommand = QStringLiteral("status");
    return this->request(mpdCommand);
}

MpdRequest *MusicPlayerConnection::play()
{
    QString mpdCommand = QStringLiteral("play");
    return this->request(mpdCommand);
}

MpdRequest *MusicPlayerConnection::pause()
{
    QString mpdCommand = QStringLiteral("pause");
    return this->request(mpdCommand);
}

MpdRequest *MusicPlayerConnection::stop()
{
    QString mpdCommand = QStringLiteral("stop");
    return this->request(mpdCommand);
}

MpdRequest *MusicPlayerConnection::next()
{
    QString mpdCommand = QStringLiteral("next");
    return this->request(mpdCommand);
}

MpdRequest *MusicPlayerConnection::previous()
{
    QString mpdCommand = QStringLiteral("previous");
    return this->request(mpdCommand);
}

MpdRequest *MusicPlayerConnection::repeat(bool repeat)
{
    QString mpdCommand = QStringLiteral("repeat %1").arg(repeat ? 1 : 0);
    return this->request(mpdCommand);
}

MpdRequest *MusicPlayerConnection::random(bool random)
{
    QString mpdCommand = QStringLiteral("random %1").arg(random ? 1 : 0);
    return this->request(mpdCommand);
}

MpdRequest *MusicPlayerConnection::move(MpdSong *song, MoveDirection direction)
{
    int pos = -1;
    switch(direction){
    case MoveToFirst:
        pos = 0;
        break;
    case MoveToLast:
        pos = p_status->playlistLength() - 1;
        break;
    case MoveAfterCurrent:
        pos = p_status->currentSongPos();
        if (pos != -1 && pos < song->getPlaylistPosition()) {
            pos ++;
        }
        break;
    case MoveOneUp:
        pos = qMax(0, song->getPlaylistPosition() - 1);
        break;
    case MoveOneDown:
        pos = qMin(song->getPlaylistPosition() + 1, p_status->playlistLength() - 1);
        break;
    }
    if (pos < 0) {
        return 0;
    }
    QString mpdCommand = QStringLiteral("moveid \"%1\" \"%2\"").arg(song->getPlaylistId(), pos);
    return this->request(mpdCommand);
}

MpdRequest *MusicPlayerConnection::playSong(unsigned int songId)
{
    QString mpdCommand = QStringLiteral("playid %1").arg(songId);
    return this->request(mpdCommand);
}

MpdRequest *MusicPlayerConnection::removeSong(unsigned int songId)
{
    QString mpdCommand = QStringLiteral("deleteid %1").arg(songId);
    return this->request(mpdCommand);
}

MpdRequest *MusicPlayerConnection::clearQueue()
{
    QString mpdCommand = QStringLiteral("clear");
    return this->request(mpdCommand);
}

MpdRequest *MusicPlayerConnection::savePlaylist(const QString& playlistName)
{
    QString mpdCommand = QStringLiteral("save \"%1\"").arg(playlistName);
    return this->request(mpdCommand);
}

MpdRequest *MusicPlayerConnection::renamePlaylist(const QString& oldPlaylistName, const QString& newPlaylistName)
{
    QString mpdCommand = QStringLiteral("rename \"%1\" \"%2\"").arg(oldPlaylistName, newPlaylistName);
    return this->request(mpdCommand);
}

MpdRequest *MusicPlayerConnection::removePlaylist(const QString& playlistName)
{
    QString mpdCommand = QStringLiteral("rm \"%1\"").arg(playlistName);
    return this->request(mpdCommand);
}

MpdRequest *MusicPlayerConnection::appendPlaylist(const QString& playlistName)
{
    QString mpdCommand = QStringLiteral("load \"%1\"").arg(playlistName);
    return this->request(mpdCommand);
}

MpdRequest *MusicPlayerConnection::playPlaylist(const QString& playlistName)
{
    QString mpdCommand = QStringLiteral("command_list_begin\n"
                                        "clear\n"
                                        "load \"%1\"\n"
                                        "play\n"
                                        "command_list_end"
                                        ).arg(playlistName);
    return this->request(mpdCommand);
}


MpdRequest *MusicPlayerConnection::insertSong(const QString& songPath)
{
    QString mpdCommand = QStringLiteral("addid \"%1\" -1").arg(songPath);
    return this->request(mpdCommand);
}

MpdRequest *MusicPlayerConnection::appendSong(const QString& songPath)
{
    QString mpdCommand = QStringLiteral("addid \"%1\"").arg(songPath);
    return this->request(mpdCommand);
}

MpdRequest *MusicPlayerConnection::prependSong(const QString& songPath)
{
    QString mpdCommand = QStringLiteral("addid \"%1\" 0").arg(songPath);
    return this->request(mpdCommand);
}

MpdRequest *MusicPlayerConnection::addSongs(const QStringList& songPaths)
{
    QString mpdCommand;
    if (songPaths.isEmpty()) {
        mpdCommand = QStringLiteral("ping");
    } else {
        mpdCommand = QStringLiteral("command_list_begin\n"
                                            "add \"%1\"\n"
                                            "command_list_end"
                                            ).arg(songPaths.join("\"\nadd \""));
    }
    return this->request(mpdCommand);
}

MpdRequest *MusicPlayerConnection::seek(int songId, int time)
{
    QString mpdCommand = QStringLiteral("seekid \"%1\" \"%2\"").arg(songId, time);
    return this->request(mpdCommand);
}

MpdRequest *MusicPlayerConnection::listDirectory(const QString& path)
{
    QString mpdCommand = QStringLiteral("lsinfo \"%1\"").arg(path);
    return this->request(mpdCommand);
}

MpdRequest *MusicPlayerConnection::listAlbums(const QString& artist)
{
    if (artist == "") {
        return this->listAlbums();
    }
    QString mpdCommand = QStringLiteral("list album \"(Artist == \\\"%1\\\" )\" ").arg(artist);
    return this->request(mpdCommand);
}

MpdRequest *MusicPlayerConnection::listAlbums()
{
    QString mpdCommand = QStringLiteral("list album");
    return this->request(mpdCommand);
}

MpdRequest *MusicPlayerConnection::listSongsByArtistAndAlbum(const QString& artist, const QString& album)
{
    QString mpdCommand = QStringLiteral("find \"((Artist == \\\"%1\\\") AND (Album == \\\"%2\\\"))\" ")
            .arg(artist, album);
    return this->request(mpdCommand);
}

MpdRequest *MusicPlayerConnection::listArtists()
{
    QString mpdCommand = QStringLiteral("list artist");
    return this->request(mpdCommand);
}

MpdRequest *MusicPlayerConnection::listPlaylists()
{
    QString mpdCommand = QStringLiteral("listplaylists");
    return this->request(mpdCommand);
}

MpdRequest *MusicPlayerConnection::getQueue()
{
    QString mpdCommand = QStringLiteral("playlistinfo");
    return this->request(mpdCommand);
}

MpdRequest *MusicPlayerConnection::getPlaylistSongs(const QString& playlist)
{
    QString mpdCommand = QStringLiteral("listplaylistinfo \"%1\"").arg(playlist);
    return this->request(mpdCommand);
}

MpdRequest *MusicPlayerConnection::search(const QString& query, const QString& scope)
{
    QString mpdCommand = QStringLiteral("search \"%1\" \"%2\"").arg(scope, query);
    return this->request(mpdCommand);
}

void MusicPlayerConnection::debugAndDelete()
{
    MpdRequest *request = qobject_cast<MpdRequest*>(sender());
    if (!request) return; //ignore...
    if (!request->succesfull())
        qDebug("Got an ACK: %s", qPrintable(request->getAck()));
    request->deleteLater();
}

void MusicPlayerConnection::reconnect()
{
    if (!p_socket->isOpen())
        p_socket->connectToHost(m_host, m_port);
}

void MusicPlayerConnection::reconnect(const QString& host, int port, const QString& password)
{
    if (p_socket->isOpen())
        disconnect();
    m_host = host;
    m_port = port;
    m_password = password;
    qDebug() << host << port;
    p_socket->connectToHost(host, port);
}

void MusicPlayerConnection::disconnect()
{
    p_socket->abort();
}

void MusicPlayerConnection::dataReady()
{
    if (!p_socket->canReadLine())
        return;
    QByteArray data;
    if ( !m_connected ) {
        // this should be MPD's welcome message
        data = p_socket->readLine();
        if (data.startsWith("OK MPD")) {
            m_connected = true;
            emit connectedChanged();
            qDebug() << "Connected: OK MPD";
            p_timer->start();
        } else {
            qDebug("Expected MPD welcome message, but got '%s'", data.constData());
            p_socket->disconnectFromHost();
            return;
        }
    }
    if (!p_waitingRequest) {
        qDebug("%s", p_socket->readAll().constData());
        qDebug("Got a message from the server, but no waiting request");
    }
    while (p_socket->canReadLine()) {
        data = p_socket->readLine();
        if (data.startsWith("OK")) {
            p_waitingRequest->setOk();
            p_waitingRequest = nullptr;
            sendNextRequest();
        } else if (data.startsWith("ACK")) {
            p_waitingRequest->setAck(QString::fromUtf8(data.constData()));
            p_waitingRequest = nullptr;
            sendNextRequest();
        } else {
            p_waitingRequest->feedData(data);
        }
    }
}

void MusicPlayerConnection::renewStatus()
{
    if (m_connected) {
        MpdRequest *req = getStatus();
        connect(req, SIGNAL(resultReady()), SLOT(statusReady()));
    }
}

void MusicPlayerConnection::statusReady()
{
    MpdRequest *req = qobject_cast<MpdRequest*>(sender());
    QList<QSharedPointer<MpdObject>> response = req->getResponse();
    if (response.length() !=1) {
        qDebug() << "Incorrect Status, response has" << response.length() << "elements.";
        req->deleteLater();
        return;
    }
    QSharedPointer<MpdStatus> status = response.first().dynamicCast<MpdStatus>();
    if (p_status.data()==0 || *status != *p_status.data()) {
        p_status = QSharedPointer<MpdStatus>(status);
        emit statusChanged(p_status);
    }
    req->deleteLater();
}

void MusicPlayerConnection::onSocketError(QAbstractSocket::SocketError error)
{
    qDebug("MPD Socket Error: %d", error);
    p_socket->close();
}

void MusicPlayerConnection::onConnected()
{
    qDebug("MPD connected");
    if (!m_password.isEmpty()) {
        MpdRequest *req = new MpdRequest(QString("password \"%1\"").arg(m_password));
        enqueueRequest(req);
        connect(req, SIGNAL(resultReady()), req, SLOT(deleteLater()));
    }
}

void MusicPlayerConnection::onDisconnected()
{
    qDebug("MPD disconnected");
    m_connected = false;
    emit connectedChanged();
    // discard waiting requests, they won't be answered...
    if (p_waitingRequest != 0)
        delete p_waitingRequest;
    p_waitingRequest = 0;
    qDeleteAll(m_requestQueue);
    m_requestQueue.clear();
}

void MusicPlayerConnection::enqueueRequest(MpdRequest *request)
{
    m_requestQueue.append(request);
    sendNextRequest();
}

void MusicPlayerConnection::sendNextRequest()
{
    if (p_waitingRequest != 0 || m_requestQueue.isEmpty())
        return;
    if (p_socket->state() != QAbstractSocket::ConnectedState)
        return; // qDebug("MPC: Socket is not open");
    p_waitingRequest = m_requestQueue.takeFirst();
    p_socket->write(p_waitingRequest->m_requestMessage.toUtf8()+'\n');
    p_socket->flush();
}

