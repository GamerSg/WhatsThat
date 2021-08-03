#ifndef DEEPSPEECHC_H
#define DEEPSPEECHC_H

#include <QObject>
#include <QQmlEngine>
#include <QtMultimedia/QAudioInput>
#include <QThread>
#include <mutex>

#include "deepspeech.h"

//Wraps Deepspeech library and also handles all AudioInput recording and passing recorded buffers to deepspeech via callbacks
class DeepSpeech : public QObject
{
    Q_OBJECT
public:
    explicit DeepSpeech(QObject* parent = nullptr);
    ~DeepSpeech();

public slots:
    void init();
    void transcribeAudio();     //Takes recorded buffers and pass to deepspeech to transcribe
    void setHotword(QString hotword);
    void stopRecording();
    void startRecording();
signals:
    void transcript(QString text);      //Signal to inform listeners that new transribed text is ready
private:
    QAudioInput* recorder = nullptr;    //Records audio from mic
    ModelState* model = nullptr;        //deepspeech library model
    StreamingState* state = nullptr;    //deepspeech state

    int samplesCollected = 0;

    std::mutex lock;                    //Mutex to prevent race conditions and sync access to deepspeech from other threads
};





#endif // DEEPSPEECHC_H
