#include "SoundRepository.h"
#include <QSoundEffect>
#include <plog/Log.h>
#include <util/File.h>
#include <util/Properties.h>
#include <QFile>
#include <QFileInfo>
#include <QMediaPlaylist>
#include <config/GameConfiguration.h>
using namespace ancono;
namespace z2 {
void SoundRepository::setCurrentPlayList(QMediaPlaylist *list)
{
    player->stop();
    player->setPlaylist(list);
    player->play();
    if(player->error() != QMediaPlayer::NoError){
        player->stop();
        return;
    }
}

QUrl getMedia(File& f){
    QFileInfo fi(QString::fromStdString(f.getPath()));
    if(!fi.exists()){
        PLOG_WARNING << "Failed to load file: " << fi.absoluteFilePath();
    }else{
        PLOG_INFO << "Loaded music: " << fi.fileName();
    }
    return QUrl::fromLocalFile(fi.absoluteFilePath());
}


void SoundRepository::initBGMs(File &dir)
{
    File mm= dir.subFile("mainmenu.mp3");
    playListMainMenu = new QMediaPlaylist(player.get());
    playListMainMenu->addMedia(getMedia(mm));
    playListMainMenu->setCurrentIndex(0);
    playListMainMenu->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);


    const int count = 22;
    playListInGame = new QMediaPlaylist(player.get());
    for(int i=1;i<=count;i++){
        stringstream ss;
        ss << "xmusic" << i << ".mp3";
        File f = dir.subFile(ss.str());
        playListInGame->addMedia(getMedia(f));
    }
    playListInGame->setPlaybackMode(QMediaPlaylist::Random);
    playListInGame->shuffle();
}



SoundRepository::SoundRepository()
{
    player.reset(new QMediaPlayer);
    updateConfig();
}

void SoundRepository::playMainMenuBGM()
{
    setCurrentPlayList(playListMainMenu);
}

void SoundRepository::playInGameBGM()
{
    playListInGame->shuffle();
    setCurrentPlayList(playListInGame);
}


void SoundRepository::setVolumn(int volBGM, int volEff)
{
    volumnBGM = volBGM;
    volumnEffect = volEff / qreal(100);
    player->setVolume(volBGM);
}

void SoundRepository::updateConfig()
{
    auto& prop = z2::GameConfiguration::instance().getProp();

    int volBGM = prop.getInt("volumnBGM",100);
    int volEff = prop.getInt("volumnEff",100);
    setVolumn(volBGM,volEff);
}



void SoundRepository::addSound(const string &str, const SoundPtr &image)
{
    soundMap.insert(make_pair(str,image));
    PLOG_INFO << "[ImageRepository] Loaded sound: " << str;
}

bool SoundRepository::containsSound(const string &key)
{
    return soundMap.find(key) != soundMap.end();
}

const SoundPtr& SoundRepository::getSound(const string &key)
{
    return soundMap.at(key);
}

void SoundRepository::playSound(const string &key)
{
    if(containsSound(key)){
        auto& sound = getSound(key);
        sound->setVolume(volumnEffect);
        sound->play();
    }else{
        PLOG_WARNING << "Unable to play sound: " << key;
    }
}

void SoundRepository::playClick()
{
    playSound("Click.wav");
}

void SoundRepository::initFromFolder(const ancono::File &dir)
{
    if(!dir.exists()){
        PLOG_WARNING << "Unable to load sounds from: " << dir.getPath();
        return;
    }

    auto& repo = instance();
    auto entityFiles = dir.listSubfiles();
    for(File& f : entityFiles){
        string name = f.getFileName();
        QString qName = QString::fromStdString(f.getPath());
        if(!qName.endsWith(".wav")){
            continue;
        }
        SoundPtr sound(new QSoundEffect);
        QFileInfo qf(qName);
//        PLOG_INFO << qf.absoluteFilePath();
        sound->setSource(QUrl::fromLocalFile(qf.absoluteFilePath()));
        repo.addSound(name,sound);
    }
    File musicDir = dir.subFile("music");
    initBGMs(musicDir);
}
}

