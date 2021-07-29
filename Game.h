#ifndef GAME_H
#define GAME_H

#include <QObject>
#include <QQmlEngine>
#include <QMediaPlayer>
#include <QThread>
#include <string>
#include <QTextToSpeech>

#include "DeepSpeechC.h"

enum GameState
{
    INTRO,
    GAME,
    QUIT
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
    void speak(QString text);
    void speechDone(QTextToSpeech::State state);

signals:    
    void gameStart();
    void showQsn(QString word, QString img);

private:
    static Game* backend;
    QMediaPlayer* player=nullptr;
    void loadMusic(QString file);
    void nextQsn(bool sayit=false);
    void showNextQsn();

    DeepSpeech ds;
    QThread deepThread;  //Run DeepSpeech in diff thread to prevent UI Lag
    QString listeningFor;
    GameState gameState = INTRO;

    std::vector<Question> qsns;

    QTextToSpeech* speech;

};

#endif // GAME_H
