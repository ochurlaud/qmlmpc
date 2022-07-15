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

#include "settings.h"

static const char mpdGroup[] = "mpd";
static const char defaultServerName[] = "default";
static const char hostKey[] = "host";
static const char portKey[] = "port";
static const char passwordKey[] = "password";

static const QString getHostKey(const QString& serverName) {
    return QStringLiteral("%1/%2/%3").arg(mpdGroup, serverName, hostKey);
}
static const QString getPortKey(const QString& serverName) {
    return QStringLiteral("%1/%2/%3").arg(mpdGroup, serverName, portKey);
}
static const QString getPasswordKey(const QString& serverName) {
    return QStringLiteral("%1/%2/%3").arg(mpdGroup, serverName, passwordKey);
}
static const QString decodePassword(const QByteArray& encodedPassword) {
    return QString(QByteArray::fromBase64(encodedPassword));
}
static const QString encodePassword(const QString& password) {
    return password.toUtf8().toBase64();
}

Settings::Settings(QObject *parent) :
    QObject(parent)
{
    p_settings = new QSettings("qmlmpc", "qmlmpc", this);
    // mpd connection
    setDefaultSetting();
    p_settings->sync();
}

void Settings::setValue(const QString &key, const QVariant &value)
{
    p_settings->setValue(key, value);
    p_settings->sync();
}

QVariant Settings::value(const QString &key, const QVariant &defaultValue) const
{
    return p_settings->value(key, defaultValue);
}

const MpdConnectionDetails Settings::mpdConnectionDetails(const QString& serverName) const
{
    MpdConnectionDetails mpdConnectionDetails;
    mpdConnectionDetails.host = p_settings->value(getHostKey(serverName)).toString();
    mpdConnectionDetails.port = p_settings->value(getPortKey(serverName)).toInt();
    mpdConnectionDetails.password = decodePassword(p_settings->value(getPasswordKey(serverName)).toByteArray());
    return mpdConnectionDetails;
}

void Settings::setMpdConnectionDetails(const QString& serverName, const MpdConnectionDetails& mpdConnectionDetails)
{
    p_settings->setValue(getHostKey(serverName), mpdConnectionDetails.host);
    p_settings->setValue(getPortKey(serverName), mpdConnectionDetails.port);
    p_settings->setValue(getPasswordKey(serverName), encodePassword(mpdConnectionDetails.password));
    p_settings->sync();
}


const QMap<QString, MpdConnectionDetails> Settings::allMpdConnectionDetails() const
{
    QMap<QString, MpdConnectionDetails> connectionDetailsMap;
    p_settings->beginGroup(mpdGroup);
    const QStringList& serverNames = p_settings->childGroups();
    p_settings->endGroup();
    for (const QString& serverName : serverNames) {
        connectionDetailsMap[serverName] = this->mpdConnectionDetails(serverName);
    }
    return connectionDetailsMap;
}

void Settings::setDefaultSetting()
{
    bool aServerIsDefined = false;

    if (p_settings->childGroups().contains(mpdGroup)) {
        p_settings->beginGroup(mpdGroup);
        const QStringList& mpdServers = p_settings->childGroups();
        if (!mpdServers.isEmpty()) {
            p_settings->beginGroup(mpdServers.first());
            const QStringList& serverConfigKeys = p_settings->childKeys();
            if (serverConfigKeys.contains(hostKey) &&
                    serverConfigKeys.contains(portKey) &&
                    serverConfigKeys.contains(passwordKey)) {
                // Everything is fine, we have at least a "correct" settings
                aServerIsDefined = true;
            }
            p_settings->endGroup();
        }
        p_settings->endGroup();
    }

    if (!aServerIsDefined) {
        p_settings->setValue(QStringLiteral("%1/%2/%3").arg(mpdGroup, defaultServerName, hostKey), "localhost");
        p_settings->setValue(QStringLiteral("%1/%2/%3").arg(mpdGroup, defaultServerName, portKey), 6600);
        p_settings->setValue(QStringLiteral("%1/%2/%3").arg(mpdGroup, defaultServerName, passwordKey), "");
    }
    // If we get to this part, it means that the settings are not setup correctly
}
