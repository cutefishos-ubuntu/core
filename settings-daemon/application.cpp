/*
 * Copyright (C) 2021 CutefishOS Team.
 *
 * Author:     revenmartin <revenmartin@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "application.h"
#include "dbusadaptor.h"
#include <QStandardPaths>
#include <QProcess>
#include <QTimer>
#include <QFile>
#include <QDebug>
#include <QDir>
#include <QTranslator>
#include <QLocale>
#include <QTimer>

Application::Application(int &argc, char **argv)
    : QApplication(argc, argv)
    , m_audioManager(new AudioManager(this))
    , m_themeManager(new ThemeManager(this))
    , m_brightnessManager(new BrightnessManager(this))
    , m_upowerManager(new UPowerManager(this))
    , m_language(new Language(this))
    , m_mouse(new Mouse)
    , m_touchpad(new TouchpadManager)
//    , m_kwinTimer(new QTimer(this))
{
    new DBusAdaptor(this);
    // connect to D-Bus and register as an object:
    QDBusConnection::sessionBus().registerService(QStringLiteral("com.cutefish.Settings"));

//    m_kwinTimer->setSingleShot(false);
//    m_kwinTimer->setInterval(50);
//    connect(m_kwinTimer, &QTimer::timeout, this, &Application::initKWin);
//    m_kwinTimer->start();

    // Translations
    QLocale locale;
    QString qmFilePath = QString("%1/%2.qm").arg("/usr/share/cutefish-settings-daemon/translations/").arg(locale.name());
    if (QFile::exists(qmFilePath)) {
        QTranslator *translator = new QTranslator(QApplication::instance());
        if (translator->load(qmFilePath)) {
            QApplication::installTranslator(translator);
        } else {
            translator->deleteLater();
        }
    }

    QTimer::singleShot(10, this, &Application::invokeDesktopProcess);
}

void Application::invokeDesktopProcess()
{
    // Start desktop UI component.
    QDBusInterface sessionInterface("com.cutefish.Session", "/Session", "com.cutefish.Session",
                                    QDBusConnection::sessionBus());

    if (sessionInterface.isValid()) {
        sessionInterface.call("startDesktopProcess");
    }
}

//void Application::initKWin()
//{
//    QDBusInterface effect("org.kde.KWin", "/Effects", "org.kde.kwin.Effects",
//                           QDBusConnection::sessionBus());

//    if (effect.isValid() && !effect.lastError().isValid()) {
//        // KWin
//        effect.call("loadEffect", "kwin4_effect_dialogparent");

//        effect.call("unloadEffect", "kwin4_effect_fadingpopups");
//        effect.call("unloadEffect", "kwin4_effect_fade");
//        effect.call("unloadEffect", "kwin4_effect_scale");
//        effect.call("unloadEffect", "kwin4_effect_grayscale");
//        effect.call("unloadEffect", "kwin4_effect_squash");
//        effect.call("unloadEffect", "kwin4_effect_translucency");
//        effect.call("unloadEffect", "magiclamp");

//        effect.call("loadEffect", "cutefish_popups");
//        effect.call("loadEffect", "cutefish_scale");
//        effect.call("loadEffect", "cutefish_squash");

//        m_kwinTimer->stop();
//    }
//}
