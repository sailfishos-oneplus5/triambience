/*
 * (C) 2016 Kimmo Lindholm <kimmo.lindholm@gmail.com> Kimmoli
 * (C) 2019 Jami Kettunen <jami.kettunen@protonmail.com>
 *
 * Triambience daemon
 *
 */

#include <stdio.h>
#include "switcher.h"


switcher::switcher(QObject *parent) : QObject(parent)
{
    if (!QDBusConnection::sessionBus().isConnected())
        printf("triambience: Cannot connect to the D-Bus sessionBus\n%s\n", qPrintable(QDBusConnection::sessionBus().lastError().message()));

    currentKeypos = QString();

    ambience_top = new MGConfItem("/apps/onyxtristate/top");
    ambience_middle = new MGConfItem("/apps/onyxtristate/middle");
    ambience_bottom = new MGConfItem("/apps/onyxtristate/bottom");

    handleAmbienceSettingsChanged();

    connect(ambience_top, SIGNAL(valueChanged()), this, SLOT(handleAmbienceSettingsChanged()));
    connect(ambience_middle, SIGNAL(valueChanged()), this, SLOT(handleAmbienceSettingsChanged()));
    connect(ambience_bottom, SIGNAL(valueChanged()), this, SLOT(handleAmbienceSettingsChanged()));

    filter = new QTimer();
    filter->setSingleShot(true);
    filter->setInterval(500);
    connect(filter, SIGNAL(timeout()), this, SLOT(changeAmbience()));
}

void switcher::handleAmbienceSettingsChanged()
{
    printf("triambience: ambience configuration changed\n");

    ambiences.insert("top", ambience_top->value(QString("silent")).toString());
    ambiences.insert("middle", ambience_middle->value(QString("sailing")).toString());
    ambiences.insert("bottom", ambience_bottom->value(QString("origami")).toString());
}

void switcher::switchTo(QString keypos)
{
    if (keypos == currentKeypos) return;

    printf("triambience: Key in %s position, changing to %s\n", qPrintable(keypos), qPrintable(ambiences.value(keypos)));
    currentKeypos = keypos;

    filter->start();
}

void switcher::changeAmbience()
{
    QDBusInterface ambienced("com.jolla.ambienced", "/com/jolla/ambienced", "");

    QList<QVariant> args;
    if (ambiences.value(currentKeypos).contains(";"))
        args.append(QString("file://%1").arg(ambiences.value(currentKeypos).split(";").at(1)));
    else
        args.append(QString("file:///usr/share/ambience/%1/%1.ambience").arg(ambiences.value(currentKeypos)));

    ambienced.callWithArgumentList(QDBus::AutoDetect, "setAmbience", args);
}
