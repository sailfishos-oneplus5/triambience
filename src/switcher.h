#ifndef SWITCHER_H
#define SWITCHER_H

#include <QObject>
#include <mlite5/MGConfItem>
#include <QtDBus/QtDBus>
#include <QTimer>

class switcher : public QObject
{
    Q_OBJECT
public:
    explicit switcher(QObject *parent = 0);

signals:

public slots:
    void switchTo(QString keypos);
    void handleAmbienceSettingsChanged();
    void changeAmbience();

private:
    MGConfItem *ambience_top;
    MGConfItem *ambience_middle;
    MGConfItem *ambience_bottom;
    QMap<QString, QString> ambiences;

    QTimer *filter;

    QString currentKeypos;
};

#endif // SWITCHER_H
