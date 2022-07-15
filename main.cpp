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

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>

#include "settings.h"
#include "mpc/mpdconnector.h"
#include "mpc/musicplayerconnection.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    Settings* settings = new Settings(&app);
    QMap<QString, MpdConnectionDetails> allMpdConnectionDetails = settings->allMpdConnectionDetails();
    MpdConnector *mpdConnector = new MpdConnector();

    for (QMap<QString, MpdConnectionDetails>::iterator i = allMpdConnectionDetails.begin() ; i != allMpdConnectionDetails.end() ; ++i) {
        mpdConnector->getConnection2()->addConnection(i.key(), i.value());
    }
    mpdConnector->getConnection2()->connectServer("default");

    QQuickStyle::setStyle("Material");

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("mpdConnector", mpdConnector);
    engine.rootContext()->setContextProperty("settings", settings);
    engine.load(QUrl(QStringLiteral("qrc:///main.qml")));

    return app.exec();
}
