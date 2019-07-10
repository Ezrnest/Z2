/*
 * Created by liyicheng on 2019/7/10.
 */

#ifndef Z2_FILE_H
#define Z2_FILE_H
#include <memory>
#include <string>
#include <fstream>
#include <vector>

using namespace std;

namespace ancono {
class File {

public:
    /**
     * Creates a file from a string representing the path of the file.
     */
    explicit File(const string&);

    File(const File& file);

    File& operator=(const File& file);

    /**
     * Gets the path of this file as a string.
     */
    string getPath() const;

    /**
     * Gets the name of the file/directory without the prefix.
     *
     */
    string getFileName() const;

    File parent() const;

    File subfile(const string&) const;



    /**
     * Returns a vector of sub-files. If this file is not a directory, return an empty vector.
     */
    vector<File> listSubfiles() const;

    /**
     * Applies the given function to each sub-file.
     */
    void forEachSubfile(const function<File&>& f) const;

    bool isDirectory() const;

    void outStream(ofstream& out);

    void inStream(ifstream& in);

    /**
     * Deletes this file.
     */
    void deleteFile() const;

    /**
     * Creates the directory named by this abstract pathname, including any
     * necessary but nonexistent parent directories.  Note that if this
     * operation fails it may have succeeded in creating some of the necessary
     * parent directories.
     */
    void mkdir() const;
};

};


#endif //Z2_FILE_H
