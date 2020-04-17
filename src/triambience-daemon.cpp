/*
 * (C) 2016 Kimmo Lindholm <kimmo.lindholm@gmail.com> Kimmoli
 * (C) 2019-2020 Jami Kettunen <jami.kettunen@protonmail.com>
 *
 * Triambience daemon
 *
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <QCoreApplication>
#include <QObject>
#include <QThread>
#include "switcher.h"
#include "uinputevpoll.h"


//
//  Daemon init
//

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);

    setlinebuf(stdout);
    setlinebuf(stderr);

    printf("triambienced: Starting daemon version %s\n", APPVERSION);

    switcher *sw;
    UinputEvPoll *uin;
    QThread *uinThread;
    MGConfItem *dconfSliderDevice;

    uin = new UinputEvPoll();
    sw = new switcher();
    uinThread = new QThread();
    dconfSliderDevice = new MGConfItem("/apps/onyxtristate/sliderDevice");

    const char* sliderDevice = dconfSliderDevice->value(QString("/dev/input/event3")).toString().toUtf8().constData();
    int fd = uin->openDevice(sliderDevice);
    if (fd == -1)
    {
        printf("triambienced: error opening input device %s!\n", sliderDevice);
        return EXIT_FAILURE;
    }

    uin->moveToThread(uinThread);

    QObject::connect(uin, SIGNAL(tristateChanged(QString)), sw, SLOT(switchTo(QString)));
    QObject::connect(uin, SIGNAL(pollingRequested()), uinThread, SLOT(start()));
    QObject::connect(uinThread, SIGNAL(started()), uin, SLOT(doPoll()));
    QObject::connect(uin, SIGNAL(finished()), uinThread, SLOT(quit()), Qt::DirectConnection);

    uin->requestPolling(fd);

    return app.exec();
}
