#ifndef IMAGEREPOSITORY_H
#define IMAGEREPOSITORY_H
#include <QImage>
#include <map>
#include <memory>
#include <config/RepositoryTemplate.h>

using namespace std;
namespace z2 {
using ImagePtr = shared_ptr<QImage>;
class ImageRepository : public RepositoryTemplate<ImageRepository>
{
private:
    map<string,ImagePtr> imageMap;
public:


    void addImage(const string& str, const ImagePtr& image);

    bool containsImage(const string& key);

    const ImagePtr& getImage(const string& key);

    // RepositoryTemplate interface
public:
    void initFromFolder(const ancono::File &dir);
};
}


#endif // IMAGEREPOSITORY_H
