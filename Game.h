#ifndef GAME_H
#define GAME_H

#include <QObject>
#include <QQmlEngine>
#include <QMediaPlayer>
#include <QThread>
#include <string>

#include "DeepSpeechC.h"

enum GameState
{
    INTRO,
    GAME
};

struct Question
{
    QString name;
    QString img;
};

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
    void gameStart();
    void showQsn(QString word, QString img);

private:
    static Game* backend;
    QMediaPlayer* player=nullptr;
    void loadMusic(QString file);
    void nextQsn();

    DeepSpeech ds;
    QThread deepThread;  //Run DeepSpeech in diff thread to prevent UI Lag
    QString listeningFor;
    GameState gameState = INTRO;

    std::vector<Question> qsns;

};

#endif // GAME_H
