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

void PicLibrary::unloadpicture(string filename) {
    if(!(loadedPictures.find(filename) == loadedPictures.end())) {
        loadedPictures.erase(filename);
        cout << filename << " successfully removed!" << endl;
    } else {
        cerr << filename << " does not exist in the picture library's internal picture store!" << endl;
    }
}

void PicLibrary::savepicture(string filename, string path) {
    if(!(loadedPictures.find(filename) == loadedPictures.end())) {
        auto picture = loadedPictures.find(filename)->second;
        Utils util;
        util.saveimage(picture.getimage(), path);
        cout << filename << " has been saved!" << endl;
    } else {
        cerr << filename << " does not exist in the picture library's internal picture store!" << endl;
    }
}

void PicLibrary::display(string filename) {
    if(!(loadedPictures.find(filename) == loadedPictures.end())) {
        auto picture = loadedPictures.find(filename)->second;
        Utils util;
        util.displayimage(picture.getimage());
    } else {
        cerr << filename << " does not exist in the picture library's internal picture store!" << endl;
    }
}

void PicLibrary::invert(string filename) {
    if(loadedPictures.find(filename) != loadedPictures.end()) {
        Picture pic = loadedPictures[filename];
        for(int i = 0; i < pic.getheight(); i++) {
            for(int j = 0; j < pic.getwidth(); j++) {
                Colour temp = pic.getpixel(j, i);
                Colour newC = Colour(255-temp.getred(), 255-temp.getgreen(), 255-temp.getblue());
                pic.setpixel(j, i, newC);
            }
        }
    } else {
        cout << filename << " is not in the picture library's internal picture store" << endl;
    }
}

void PicLibrary::exit() {
    delete(loadedPictures);
}