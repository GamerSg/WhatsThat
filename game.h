#ifndef GAME_H
#define GAME_H

#include <QObject>
#include <QQmlEngine>
#include <QMediaPlayer>
#include <QThread>
#include <string>

#include "DeepSpeechC.h"

class Game : public QObject
{
    Q_OBJECT
public:
    explicit Game(QObject *parent = nullptr);
    static Game* getGame(QQmlEngine *engine, QJSEngine *scriptEngine);
    static void destroy();

public slots:
    void openingMenu();
    void userSpeech(QString text);
    void listenFor(QString word);

signals:
    void wordDetected(QString word);

private:
    static Game* backend;
    QMediaPlayer* player=nullptr;
    void loadMusic(QString file);

    DeepSpeech ds;
    QThread deepThread;  //Run DeepSpeech in diff thread to prevent UI Lag
    QString listeningFor;

};

#endif // GAME_H
