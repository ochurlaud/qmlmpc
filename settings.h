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

#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QSettings>

struct MpdConnectionDetails {
    Q_GADGET
    Q_PROPERTY(QString host MEMBER host)
    Q_PROPERTY(int port MEMBER port)
    Q_PROPERTY(QString password MEMBER password)

public:
    QString host;
    int port;
    QString password;
};

class Settings : public QObject
{
    Q_OBJECT
public:
    explicit Settings(QObject *parent = 0);

    Q_INVOKABLE void setValue(const QString & key, const QVariant & value);
    Q_INVOKABLE QVariant value(const QString & key, const QVariant & defaultValue = QVariant()) const;
    Q_INVOKABLE const MpdConnectionDetails mpdConnectionDetails(const QString& serverName) const;
    Q_INVOKABLE void setMpdConnectionDetails(const QString& serverName, const MpdConnectionDetails& mpdConnectionDetails);

    const QMap<QString, MpdConnectionDetails> allMpdConnectionDetails() const;


private:
    void setDefaultSetting();
    QSettings* p_settings;
};

#endif // SETTINGS_H
