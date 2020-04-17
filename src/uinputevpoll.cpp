/*
 * (C) 2016 Kimmo Lindholm <kimmo.lindholm@gmail.com> Kimmoli
 * (C) 2019-2020 Jami Kettunen <jami.kettunen@protonmail.com>
 *
 * Triambience daemon
 *
 */

#include <stdio.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <errno.h>
#include <linux/input.h>
#include "uinputevpoll.h"


UinputEvPoll::UinputEvPoll(QObject *parent) : QObject(parent)
{
    _polling = false;
    _abort = false;
    _uinputfd = -1;
}

void UinputEvPoll::abort()
{
    mutex.lock();
    if (_polling)
        _abort = true;
    mutex.unlock();
}

void UinputEvPoll::requestPolling(int fd)
{
    mutex.lock();
    _polling = true;
    _abort = false;
    _uinputfd = fd;
    mutex.unlock();

    emit pollingRequested();
}

void UinputEvPoll::doPoll()
{
    int epfd;
    int ret;
    int i;
    struct epoll_event ev;
    struct epoll_event evs[16];

    epfd = epoll_create1(0);

    if (epfd < 0)
    {
        printf("triambienced: Failed to create epoll instance\n");

        emit finished();
        return;
    }

    memset(&ev, 0, sizeof(ev));
    ev.events = EPOLLIN;
    ev.data.fd = _uinputfd;

    ret = epoll_ctl(epfd, EPOLL_CTL_ADD, _uinputfd, &ev);
    if (ret)
    {
        printf("triambienced: Couldn't add to epoll\n");
        close(epfd);

        emit finished();
        return;
    }

    printf("triambienced: Starting tristate key polling.\n");

    for (;;)
    {
        // Checks if the process should be aborted
        mutex.lock();
        bool abort = _abort;
        mutex.unlock();

        if (abort) break;

        ret = epoll_wait(epfd, evs, 16, -1);

        if (ret < 0)
        {
            if (errno == EINTR) continue;
            else break;
        }

        for (i = 0 ; i<ret ; i++)
            readKeyboard(evs[i].data.fd);
    }

    close(epfd);

    emit finished();
}

void UinputEvPoll::readKeyboard(int fd)
{
    ssize_t len;
    struct input_event evs[16];

    while ((len = read(fd, &evs, sizeof(evs))) > 0)
    {
        const size_t nevs = len / sizeof(struct input_event);
        size_t i;
        for (i = 0; i < nevs; i++)
        {
            if (evs[i].type == EV_KEY && evs[i].value == 1)
            {
                if (evs[i].code == 0x259)
                    emit tristateChanged("top");
                else if (evs[i].code == 0x25a)
                    emit tristateChanged("middle");
                else if (evs[i].code == 0x25b)
                    emit tristateChanged("bottom");
            }
        }
    }

    if (len < 0 && errno != EWOULDBLOCK)
    {
        printf("triambienced: Couldn't read, %s\n", strerror(errno));
        return;
    }
}

int UinputEvPoll::openDevice(const char *path)
{
    int fd;

    if( (fd = open(path, O_RDONLY)) != -1 )
    {
        int vers = 0;
        if( ioctl(fd, EVIOCGVERSION, &vers) == -1)
        {
            close(fd);
            fd = -1;
        }
    }

    return fd;
}
