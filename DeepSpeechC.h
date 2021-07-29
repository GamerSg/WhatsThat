#ifndef DEEPSPEECHC_H
#define DEEPSPEECHC_H

#include <QObject>
#include <QQmlEngine>
#include <QtMultimedia/QAudioInput>
#include <QThread>
#include <mutex>

#include "deepspeech.h"

class DeepSpeech : public QObject
{
    Q_OBJECT
public:
    explicit DeepSpeech(QObject* parent = nullptr);
    ~DeepSpeech();

public slots:
    void init();
    void transcribeAudio();
    void setHotword(QString hotword);
    void stopRecording();
    void startRecording();
signals:
    void transcript(QString text);
private:
    QAudioInput* recorder = nullptr;
    ModelState* model = nullptr;
    StreamingState* state = nullptr;
    int samplesCollected = 0;
    void doTranscription(QIODevice* device);
    void createRecorder();

    std::mutex lock;
    bool recording = false;
    int idealSample = 0;
};





#endif // DEEPSPEECHC_H
