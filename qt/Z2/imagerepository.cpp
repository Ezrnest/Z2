#include "ImageRepository.h"
#include <plog/Log.h>
#include <util/File.h>
#include <util/Properties.h>
using namespace ancono;
namespace z2 {


void ImageRepository::addImage(const string &str, const ImagePtr &image)
{
    imageMap.insert(make_pair(str,image));
    PLOG_INFO << "[ImageRepository] Loaded image: " << str;
}

bool ImageRepository::containsImage(const string &key)
{
    return imageMap.find(key) != imageMap.end();
}

const ImagePtr& ImageRepository::getImage(const string &key)
{
    return imageMap.at(key);
}

void ImageRepository::initFromFolder(const ancono::File &dir)
{
    if(!dir.exists()){
        LOG_WARNING << "Unable to load images from: " << dir.getPath();
        return;
    }

    auto& repo = instance();
    auto entityFiles = dir.listSubfiles();
    for(File& f : entityFiles){
        string name = f.getFileName();
        ImagePtr image(new QImage(QString::fromStdString(f.getPath())));
        repo.addImage(name,image);
    }
}

//ImageRepository &ImageRepository::instance()
//{
//    static ImageRepository repo;
//    return repo;
//}
}

