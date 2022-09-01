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

#ifndef MPDCONNECTOR_H
#define MPDCONNECTOR_H

#include <QObject>
#include <QSharedPointer>
#include <QStringList>

#include "types.h"

class MusicPlayerConnection;
class MpdSong;
class MpdSongListModel;
class MpdEntityListModel;

class MpdConnector : public QObject
{
    Q_OBJECT
    // status properties
    Q_PROPERTY( bool repeating READ getRepeat NOTIFY statusChanged )
    Q_PROPERTY( bool shuffling READ getRandom NOTIFY statusChanged )
    Q_PROPERTY( bool playing READ isPlaying NOTIFY statusChanged )
    Q_PROPERTY( bool paused READ isPaused NOTIFY statusChanged )
    Q_PROPERTY( bool stopped READ isStopped NOTIFY statusChanged )
    Q_PROPERTY( int currentSongId READ getCurrentSongId NOTIFY statusChanged )
    Q_PROPERTY( int currentSongPos READ getCurrentSongPos NOTIFY statusChanged )
    Q_PROPERTY( unsigned int elapsedTime READ getElapsedTime NOTIFY statusChanged )
    Q_PROPERTY( unsigned int totalTime READ getTotalTime NOTIFY statusChanged )
    Q_PROPERTY( unsigned int numSongs READ getNumSongs NOTIFY statusChanged )
    // now playing
    Q_PROPERTY( QString nowPlaying READ getNowPlaying NOTIFY currentSongChanged )
    // collection path and search string
    Q_PROPERTY( QString currentCollectionPath READ getCurrentCollectionPath NOTIFY currentCollectionPathChanged )
    // connection
    Q_PROPERTY( QObject *connection READ getConnection CONSTANT )
    // data models
    Q_PROPERTY( QObject *queueModel READ getQueueModel CONSTANT )
    Q_PROPERTY( QObject *playlistSongsModel READ getPlaylistSongsModel CONSTANT )
    Q_PROPERTY( QObject *searchResultModel READ getSearchResultModel CONSTANT)
public:
    MpdConnector(QObject *parent = 0);

    bool getRepeat() { return p_status->repeat(); }
    bool getRandom() { return p_status->random(); }
    bool isPlaying() { return p_status->state() == MpdStatus::Play; }
    bool isPaused() { return p_status->state() == MpdStatus::Pause; }
    bool isStopped() { return p_status->state() == MpdStatus::Stop; }
    int getCurrentSongId() { return p_status->currentSongId(); }
    int getCurrentSongPos() { return p_status->currentSongPos(); }
    unsigned int getElapsedTime() { return p_status->elapsedTime(); }
    unsigned int getTotalTime() { return p_status->totalTime(); }
    unsigned int getNumSongs() { return p_status->playlistLength(); }

    QString getNowPlaying();
    QString getCurrentCollectionPath() { return m_currentCollectionPath; }

    QObject *getConnection() { return (QObject*) p_connection; }
    MusicPlayerConnection *getConnection2() { return p_connection; }

    QObject *getQueueModel() { return (QObject*) p_queueModel; }
    QObject *getPlaylistSongsModel() { return (QObject*) p_playlistSongsModel; }
    QObject *getSearchResultModel() { return (QObject*) p_searchResultModel; }
    
signals:
    void statusChanged();
    void currentSongChanged();
    void currentCollectionPathChanged();
    
public slots:

    // Media management
    void play();
    void pause();
    void stop();
    void next();
    void previous();
    void repeat(bool repeat);
    void random(bool random);
    void playSong(int songId);
    void removeSong( int songId);
    void clearQueue();

    // Playlists
    void savePlaylist(const QString& name);
    void renamePlaylist(const QString& playlist, const QString& newName);
    void removePlaylist(const QString& playlist);
    void appendPlaylist(const QString& playlist);
    void playPlaylist(const QString& playlist);

    // Albums
    void appendAlbum(const QString& artist, const QString& album);

    // Songs
    void insertSong(const QString& path);
    void appendSong(const QString& path);
    void addSongs(QStringList paths);
    void prependSong(const QString& path);
    void moveSongUp(int songId);
    void moveSongDown(int songId);
    void moveSongFirst(int songId);
    void moveSongLast(int songId);
    void moveSongAfterCurrent(int songId);
    void seek(int time);
    /*void listDirectory(const QString& path);
    void listAlbums();
    void listAlbums(const QString& artist);
    void listSongsByArtistAndAlbum(const QString& artist, const QString& album);
    void listPlaylists();
    void listSongsByPlaylist(const QString& playlist);*/
    void getPlaylistSongs(const QString& playlist);
    void search(const QString& query, const QString& scope);
    
private slots:
    void statusUpdated(QSharedPointer<MpdStatus> status);
    void queueReady();
/*    void playlistsReady();*/
    void playlistSongsReady();
    void songsToAppendReady();
/*    void directoryListingReady();
    void artistListingReady();
    void albumListingReady();*/
 //   void songListingReady();
    void searchResultsReady();

private:
    MpdSong *getSong(int songId);

private:
    MusicPlayerConnection *p_connection;
    QSharedPointer<MpdStatus> p_status;
    int m_lastPlaylistVersion;
    MpdSongList m_queue;
    MpdSongListModel *p_queueModel;
    MpdSongListModel *p_playlistSongsModel;
    QString m_currentCollectionPath;
    MpdSongListModel *p_searchResultModel;
};

#endif // MPDCONNECTOR_H
