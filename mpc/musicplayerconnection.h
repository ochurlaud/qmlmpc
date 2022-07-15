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

#ifndef MUSICPLAYERCONNECTION_H
#define MUSICPLAYERCONNECTION_H

#include <QObject>
#include <QAbstractSocket>
#include "types.h"
#include "settings.h"

class QTcpSocket;
class QTimer;
class MpdRequest;
class MpdStatusRequest;
class MpdSongListRequest;
class MpdEntityListRequest;

class MusicPlayerConnection : public QObject
{
    Q_OBJECT

    Q_PROPERTY( bool connected READ isConnected NOTIFY connectedChanged )
    Q_PROPERTY( QString connectedServer READ connectedServer NOTIFY connectedServerChanged )

public:
    enum MoveDirection {
        MoveToFirst,
        MoveToLast,
        MoveAfterCurrent,
        MoveOneUp,
        MoveOneDown
    };

    MusicPlayerConnection(QObject *parent = 0);

    Q_INVOKABLE void addConnection(const QString& name, const QString& host, int port, const QString& password);
    void addConnection(const QString& name, const MpdConnectionDetails& connectionDetails);
    Q_INVOKABLE void connectServer(const QString& serverName);
    MpdRequest* getStatus();
    MpdRequest* play();
    MpdRequest* pause();
    MpdRequest* stop();
    MpdRequest* next();
    MpdRequest* previous();
    MpdRequest* repeat(bool repeat);
    MpdRequest* random(bool random);
    MpdRequest* move(MpdSong *song, MoveDirection direction);
    MpdRequest* playSong(unsigned int songId);
    MpdRequest* removeSong(unsigned int songId);
    MpdRequest* clearQueue();
    MpdRequest* savePlaylist(const QString& name);
    MpdRequest* renamePlaylist(const QString& playlist, const QString& newName);
    MpdRequest* removePlaylist(const QString& playlist);
    MpdRequest* appendPlaylist(const QString& playlist);
    MpdRequest* playPlaylist(const QString& playlist);
    MpdRequest* insertSong(const QString& path);
    MpdRequest* appendSong(const QString& path);
    MpdRequest* prependSong(const QString& path);
    MpdRequest* addSongs(const QStringList& paths);
    MpdRequest* seek(int songId, int time);
    MpdRequest* listDirectory(const QString& path);
    MpdRequest* listArtists();
    MpdRequest* listAlbums(const QString& artist);
    MpdRequest* listAlbums();
    MpdRequest* listSongsByArtistAndAlbum(const QString& artist, const QString& album);
    MpdRequest* listSongsByAlbum(const QString& album);

    MpdRequest* listPlaylists();
    MpdRequest* getQueue();
    MpdRequest* getPlaylistSongs(const QString& playlist);
    MpdRequest* search(const QString& query, const QString& scope);

    bool isConnected() { return m_connected; }
    QString connectedServer() const { return m_connectedServer; };

signals:
    void statusChanged(QSharedPointer<MpdStatus> status);
    void connectedChanged();
    void connectedServerChanged();

public slots:
    void debugAndDelete(); //!< if an error occurred, display the error and finally delete the MpdRequest

    void reconnect();
    void reconnect(const QString& serverName);
    void disconnect();

private slots:
    void dataReady();
    void renewStatus();
    void statusReady();

    void onSocketError(QAbstractSocket::SocketError error);
    void onConnected();
    void onDisconnected();
    void sendNextRequest();

private:
    void enqueueRequest(MpdRequest* request);
    MpdRequest* request(const QString& mpdCommand);

private:
    QMap<QString, MpdConnectionDetails> m_connectionDetailsStore;
    MpdConnectionDetails m_currentConnectionDetails;
    QString m_connectedServer;

    QString m_protocolVersion;
    QTcpSocket *p_socket;

    MpdRequest* p_waitingRequest;
    QList<MpdRequest*> m_requestQueue;

    bool m_connected; //!< "OK MPD <version>" received?
    QSharedPointer<MpdStatus> p_status;

    QTimer* p_timer;
    
};


#endif // MUSICPLAYERCONNECTION_H
