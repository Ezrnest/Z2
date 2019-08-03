/*
 * Created by liyicheng on 2019/7/10.
 * Completed by chenzc18 on 2019/7/28.
 * Revised by liyicheng on 2019/8/1.
 */

#include "File.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <io.h>
#include <functional>

using namespace std;
using namespace ancono;

void getFiles(const string &, vector<File> &);

File::File(const string &path) : path(path) {
    const char *p = path.c_str();
    if (_access(p, 0) == 0) {
        handle = _findfirst(p, &fileinfo);
    }else{
        handle = 0;
    }
//    else // file does not exist
//    {
//        handle = _creat(p, 0);
//    }
}

File::File(const File &file) = default;

File::~File() = default;

File &File::operator=(const File &file) = default;

long File::getHandle() const {
    return handle;
}

string File::getPath() const {
    return path;
}

string File::getFileName() const {
    return fileinfo.name;
}

string File::getFileNameWithoutExtension() const {
    auto name = getFileName();
    int pos = name.find_last_of('.');
    return name.substr(0, pos);
}


File File::parent() const {
    string p;
    File parent(p.assign(path).append("\\.."));
    return parent;
}

File File::subFile(const string &name) const {
    string p;
    p.assign(path).append("\\").append(name);
//    long exist = open(p.c_str(), 2);
//    if (exist == -1) // file does not exist
//    {
    return File(p);
//    }
}

vector<File> File::listSubfiles() const {
    vector<File> files;
    getFiles(path, files);
    return files;
}

void File::forEachSubfile(const function<void(File &)> &f) const {
    vector<File> subfile = this->listSubfiles();
    for (auto &i : subfile) {
        f(i);
    }
}

bool File::isDirectory() const {
    return fileinfo.attrib == _A_SUBDIR;
}

void File::outStream(ofstream &out) {
    out.open(path.c_str());
}

void File::inStream(ifstream &in) {
    in.open(path.c_str());
}

void File::deleteFile() const {
    remove(path.c_str());
}

void File::mkdir() const {
    ::mkdir(path.c_str());
}

#include <windows.h>

namespace ancono {
string exePath() {
    char buffer[MAX_PATH];
    GetModuleFileName(nullptr, buffer, MAX_PATH);
    string s(buffer);
    cout << s << endl;
    cout << "-----" << endl;
    string::size_type pos = s.find_last_of('\\');
    return s.substr(0, pos);
}
}


File File::currentDirectory() {
    return File(ancono::exePath());
}

bool File::exists() const {
    return (access(path.c_str(), F_OK) != -1);
}


void getFiles(const string &path, vector<File> &files) {
    long tmpHandle = 0;
    struct _finddata_t fileinfo{};
    string p;
    if ((tmpHandle = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1) {
        do {
            if (fileinfo.attrib == _A_SUBDIR) {
                if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0) {
                    getFiles(p.assign(path).append("\\").append(fileinfo.name), files);
                }
            } else {
                File tmp(p.assign(path).append("\\").append(fileinfo.name));
                files.push_back(tmp);
            }
        } while (_findnext(tmpHandle, &fileinfo) == 0);
        _findclose(tmpHandle);
    }
}
