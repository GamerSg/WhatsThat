#include <QFileInfo>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <QCoreApplication>


#include "Game.h"
#include "DeepSpeechC.h"

Game* Game::backend = nullptr;

Game::Game(QObject *parent) : QObject(parent)
{
   qInfo()<<"Creating Game Object";
   player = new QMediaPlayer(this);
   loadMusic("audio/theme.mp3");    //To cache opening music for faster playback

   speech = new QTextToSpeech(this);

   qInfo()<<"Game created on thread "<<this->thread();
   ds.moveToThread(&deepThread);
   deepThread.start();
   ds.init();
   //Subscribe to all transcribed text
   connect(&ds,&DeepSpeech::transcript, this, &Game::userSpeech);
   connect(speech,&QTextToSpeech::stateChanged, this, &Game::speechDone);
   //Create questions
   qsns.push_back( {"cat", "qrc:/img/cat.png"});
   qsns.push_back( {"bus", "qrc:/img/bus.png"});
   qsns.push_back( {"dog", "qrc:/img/dog.png"});
   qsns.push_back( {"train", "qrc:/img/train.png"});

   //Random seed
   srand(time(0));
}

Game* Game::getGame(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)
    if(!backend)
    {
        backend = new Game();
        QQmlEngine::setObjectOwnership(backend, QQmlEngine::CppOwnership);
    }

    return backend;
}

void Game::listenFor(QString word)
{
    qInfo()<<"Game listenFor "<<word<<" on "<<this->thread();
    listeningFor = word;  
    ds.startRecording();
    ds.setHotword("quit");
    ds.setHotword(word);
}

void Game::loadMusic(QString file)
{
//#ifdef Q_OS_ANDROID
//    QFileInfo f("assets:/"+file);
//    qInfo()<<"Loading "<<f.absoluteFilePath();
//    player->setMedia(QUrl::fromLocalFile(f.absoluteFilePath()));
//    qInfo()<<"Media Status "<<player->mediaStatus();
//#else
//    QFileInfo f("./"+file);
//    player->setMedia(QUrl::fromLocalFile(f.absoluteFilePath()));
//#endif
    player->setMedia(QUrl("qrc:/"+file));
}

void Game::speechDone(QTextToSpeech::State state)
{
    if(state == QTextToSpeech::State::Ready)
    {
        if(gameState == GAME)
        {
            showNextQsn();
        }
        else if(gameState == QUIT)
        {
            QCoreApplication::quit();
        }
    }
}

void Game::openingMenu()
{
    qInfo()<<"Opening Menu";
    loadMusic("audio/theme.mp3");
    player->play();
    listenFor("start");
}

void Game::speak(QString text)
{
    speech->say(text);
}

void Game::nextQsn(bool sayit)
{
    if(sayit)
    {
        ds.stopRecording();    //Clear stream and recoup memory
        speak("Correct, this is a " + listeningFor);
    }
    else
    {
        showNextQsn();
    }
}

void Game::showNextQsn()
{
    int qsnNum =rand() % qsns.size();
    listenFor(qsns[qsnNum].name);
    emit showQsn(qsns[qsnNum].name, qsns[qsnNum].img);
}

void Game::userSpeech(QString text)
{
    qInfo()<<"Game received user speech "<<text;
    if(text.contains(listeningFor))
    {//Word detected
        if(gameState == INTRO)
        {
            gameState = GAME;
            emit gameStart();
            nextQsn();
        }
        else
        {           
            qInfo()<<"Correct, moving on to next question";
            nextQsn(true);
        }
    }
    else if(text.contains("quit"))
    {
        ds.stopRecording();
        if(gameState == GAME)
        {
            speak("This was a "+listeningFor+". Thank you for playing What's That, try again next time! Bye bye");
            gameState = QUIT;
        }
        else
        {
            QCoreApplication::quit();
        }


    }
}

void Game::destroy()
{
    if(backend)
        delete backend;
    //No need to delete QT classes, QT will automatically delete them
}
