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
public:
    enum MoveDirection {
        MoveToFirst,
        MoveToLast,
        MoveAfterCurrent,
        MoveOneUp,
        MoveOneDown
    };

    MusicPlayerConnection(QString host, int port, QString password=QString(), QObject *parent = 0);

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
    MpdRequest* savePlaylist(QString name);
    MpdRequest* renamePlaylist(QString playlist, QString newName);
    MpdRequest* removePlaylist(QString playlist);
    MpdRequest* appendPlaylist(QString playlist);
    MpdRequest* playPlaylist(QString playlist);
    MpdRequest* insertSong(QString path);
    MpdRequest* appendSong(QString path);
    MpdRequest* prependSong(QString path);
    MpdRequest* addSongs(QStringList paths);
    MpdRequest* seek(int songId, int time);
    MpdRequest* listDirectory(QString path);
    MpdRequest* listArtists();
    MpdRequest* listAlbums(QString& artist);
    MpdRequest* listAlbums();
    MpdRequest* listPlaylists();
    MpdRequest* getQueue();
    MpdRequest* getPlaylistSongs(QString playlist);
    MpdRequest* search(QString query, QString scope);

    bool isConnected() { return m_connected; }

signals:
    void statusChanged(QSharedPointer<MpdStatus> status);
    void connectedChanged();

public slots:
    void debugAndDelete(); //!< if an error occurred, display the error and finally delete the MpdRequest

    void reconnect();
    void reconnect(QString host, int port, QString password=QString());
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
    MpdRequest* request(const QString &mpdCommand);

private:
    QString m_host;
    int m_port;
    QString m_password;
    QTcpSocket *p_socket;

    MpdRequest* p_waitingRequest;
    QList<MpdRequest*> m_requestQueue;

    bool m_connected; //!< "OK MPD <version>" received?
    QSharedPointer<MpdStatus> p_status;

    QTimer* p_timer;
    
};


#endif // MUSICPLAYERCONNECTION_H
