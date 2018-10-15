#include <algorithm>
#include "PicLibrary.hpp"
#include "Colour.hpp"

inline bool PicLibrary::isJPG(string const &filename, string const &ending)
{
    if(ending.size() > filename.size()) {
        return false;
    } else {
        return std::equal(ending.rbegin(), ending.rend(), filename.rbegin());
    }
}

void PicLibrary::print_picturestore()
{
    vector<string> keys;
    keys.reserve(loadedPictures.size());
    for(auto &it : loadedPictures) {
        keys.push_back(it.first);
    }
    std::sort(keys.begin(), keys.end());
    for(auto &it : keys) {
        cout << it << endl; 
    }
}

void PicLibrary::grayscale(string key)
{

}

void PicLibrary::loadpicture(string path, string filename)
{
    string jpg = ".jpg";
    if(!PicLibrary::isJPG(filename, "jpg")) {
        cerr << "Picture is not a .jpg file" << endl;
    } else if(loadedPictures.find(filename) == loadedPictures.end()) {
        Picture toAdd = Picture(path);
        if(toAdd.getheight() != 0) {
            loadedPictures.insert({filename, Picture(path)});
            cout << filename << " saved!" << endl;
        } else {
            cout << path << " doesn't exist!" << endl;
        }
    } else {
        cerr << "Picture already exists" << endl;
    }
}