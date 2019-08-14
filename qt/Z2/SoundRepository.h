#ifndef SOUNDREPOSITORY_H
#define SOUNDREPOSITORY_H
#include <config/RepositoryTemplate.h>
#include <QMediaPlayer>
#include <QSound>
namespace z2 {

using SoundPtr = shared_ptr<QSoundEffect>;

class SoundRepository : public RepositoryTemplate<SoundRepository>
{

private:
    qreal volumnEffect;
    int volumnBGM;
    shared_ptr<QMediaPlayer> player;
    map<string,SoundPtr> soundMap;

    QMediaPlaylist* playListMainMenu;
    QMediaPlaylist* playListInGame;


    void setCurrentPlayList(QMediaPlaylist* list);

    void initBGMs(ancono::File& dir);

public:
    SoundRepository();

    void playMainMenuBGM();

    void playInGameBGM();

    void setVolumn(int volBGM,int volEff);

    void updateConfig();

    void addSound(const string& str, const SoundPtr& sound);

    bool containsSound(const string& key);

    const SoundPtr& getSound(const string& key);

    void playSound(const string& key);

    void playClick();

public:

    void initFromFolder(const ancono::File &dir);

};
}


#endif // SOUNDREPOSITORY_H
