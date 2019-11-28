/*
 * (C) 2016 Kimmo Lindholm <kimmo.lindholm@gmail.com> Kimmoli
 * (C) 2019 Jami Kettunen <jami.kettunen@protonmail.com>
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

    printf("triambience: Starting daemon version %s\n", APPVERSION);

    switcher *sw;
    UinputEvPoll *uin;
    QThread *uinThread;

    uin = new UinputEvPoll();
    sw = new switcher();
    uinThread = new QThread();

    int fd = uin->openDevice("/dev/input/event3");
    if (fd == -1)
    {
        printf("triambience: error opening input device\n");
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
