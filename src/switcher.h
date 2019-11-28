/*
 * (C) 2016 Kimmo Lindholm <kimmo.lindholm@gmail.com> Kimmoli
 * (C) 2019 Jami Kettunen <jami.kettunen@protonmail.com>
 *
 * Triambience daemon
 *
 */

#ifndef SWITCHER_H
#define SWITCHER_H

#include <QObject>
#include <QtDBus>
#include <QTimer>
#include <mlite5/MGConfItem>


class switcher : public QObject
{
    Q_OBJECT

public:
    explicit switcher(QObject *parent = 0);

public slots:
    void switchTo(QString keypos);
    void handleAmbienceSettingsChanged();
    void changeAmbience();

private:
    MGConfItem *ambience_top;
    MGConfItem *ambience_middle;
    MGConfItem *ambience_bottom;

    QMap<QString, QString> ambiences;
    QString currentKeypos;
    QTimer *filter;
};

#endif // SWITCHER_H
