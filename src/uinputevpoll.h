/*
 * (C) 2016 Kimmo Lindholm <kimmo.lindholm@gmail.com> Kimmoli
 * (C) 2019-2020 Jami Kettunen <jami.kettunen@protonmail.com>
 *
 * Triambience daemon
 *
 */

#ifndef UINPUTEVPOLL_H
#define UINPUTEVPOLL_H

#include <QMutex>
#include <QObject>
#include <mlite5/MGConfItem>


class UinputEvPoll : public QObject
{
    Q_OBJECT

public:
    explicit UinputEvPoll(QObject *parent = 0);

    void requestPolling(int fd);
    void abort();
    int openDevice(const char *path);

signals:
    void tristateChanged(QString);
    void pollingRequested();
    void finished();

public slots:
    void doPoll();

private:
    void readKeyboard(int fd);

    QMutex mutex;
    MGConfItem *dconfSliderCodes;
    QVariantList defSliderCodes = { 0x259, 0x25a, 0x25b };

    bool _polling;
    bool _abort;
    int _uinputfd;
    int _sliderCodes[3];
};

#endif // UINPUTEVPOLL_H
