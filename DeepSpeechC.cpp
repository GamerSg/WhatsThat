#include <QtDebug>
#include <QFile>
#include <QDir>
#include <QStandardPaths>

#include <thread>
#include "DeepSpeechC.h"
#include "Game.h"

//Convenience function to debug file path issues
void listFiles(QString dir)
{
    QDir d(dir);
    QStringList list = d.entryList();
    qInfo()<<"Listing files in "<<dir;
    for( int i = 0; i< list.size(); ++i)
    {
        qInfo()<<list[i];
    }
}


DeepSpeech::DeepSpeech(QObject *parent) : QObject(parent)
{

}

void DeepSpeech::init()
{
#ifdef Q_OS_ANDROID
    //Copy model file from apk to accessible location
    QFile dfile("assets:/models/ds93.tflite");
    QString filePath = QStandardPaths::writableLocation( QStandardPaths::StandardLocation::AppLocalDataLocation );
    qInfo()<<"Data write location"<<filePath;
    filePath.append( "/ds93.tflite");
    if (dfile.exists()) {  //If model file exists
        qInfo()<<"Model file exists @"<<dfile.fileName();
        if(!QFile::exists( filePath ) )
        {//Check if it exists in this location already
            qInfo()<<"Model files does not exist at "<<filePath;
            if( dfile.copy( filePath ) )
            {//Copy over if it doesnt
                qInfo()<<"Copying model to : "<<filePath;
                QFile::setPermissions( filePath, QFile::WriteOwner | QFile::ReadOwner );
            }
        }
    }
#else //For Windows/Mac/Linux
    QString filePath("./models/ds93.pbmm");
#endif
    std::string modelPath = filePath.toStdString();

    int result = 0;
    result = DS_CreateModel(modelPath.c_str(), &model);

    qInfo()<<"Creating DS Model : "<<result<<" on thread "<<this->thread();
    int idealSample = 0;
    idealSample = DS_GetModelSampleRate(model);
    qInfo()<<"Ideal Sample Rate : "<<idealSample;

    QAudioFormat recFormat;
    recFormat.setSampleRate(idealSample);
    recFormat.setByteOrder(QAudioFormat::LittleEndian);
    recFormat.setChannelCount(1);
    recFormat.setSampleSize(16);
    recFormat.setCodec("audio/pcm");
    recFormat.setSampleType(QAudioFormat::SignedInt);
    recorder = new QAudioInput (recFormat);

    QIODevice* device = recorder->start();
    qInfo()<<"Started recording audio on "<<device;
    QObject::connect(device, &QIODevice::readyRead, this, &DeepSpeech::transcribeAudio, Qt::QueuedConnection );
}


DeepSpeech::~DeepSpeech()
{
    if(recorder)
    {
        delete recorder;
        recorder = nullptr;
    }
}

void DeepSpeech::startRecording()
{
    std::lock_guard<std::mutex> guard(lock);
    int ret = DS_CreateStream(model, &state);
    qInfo()<<"Setting up Model Stream : "<<ret;
}

void DeepSpeech::stopRecording()
{
    std::lock_guard<std::mutex> guard(lock);
    qInfo()<<"Clearing Hotwords and Freeing stream"<<this->thread();
    DS_ClearHotWords(model);
    if(state)
    {
        DS_FreeStream(state);
    }
    state = nullptr;
}

void DeepSpeech::setHotword(QString hotword)
{
    std::string word = hotword.toStdString();
   DS_AddHotWord(model, word.c_str(), 5.0f );
}

void DeepSpeech::clearHotwords()
{
 //   DS_ClearHotWords(model);
}


void DeepSpeech::transcribeAudio()
{
    try {
        std::lock_guard<std::mutex> guard(lock);
        if(state)
        {
            QIODevice* device = static_cast<QIODevice*>(sender());
            QByteArray data = device->readAll();
            qInfo()<<"Received audio bytes "<<data.size();
            DS_FeedAudioContent(state, reinterpret_cast<const short*>(data.data()), data.size()/2);
            //  qInfo()<<"Transcribed "<<data.size()<<" bytes...";
            qInfo()<<"Transcribing on thread "<<this->thread();
            samplesCollected += data.size()/2;

            if(samplesCollected > 16000)
            {
                std::string qtext;
                char* text = DS_IntermediateDecode(state);
                qInfo()<<text;
                qtext = text;
                DS_FreeString(text);

                //Inform Game of new speech by user
                emit transcript(QString::fromStdString(qtext));
                samplesCollected = 0;
            }
        }
    }  catch (std::exception& e) {
        qInfo()<<e.what();
    }
}
