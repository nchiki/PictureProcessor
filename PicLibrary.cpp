#include <algorithm>
#include "PicLibrary.hpp"
#include "Colour.hpp"


//NOCH SEQUENTIAL SACHEN HINZUFÜGEN UND DANN AUSKOMMENTIEREN UND TESTEN
//ANDERE DATEI MIT ERKLÄRUNGEN HINZUFÜGEN



void PicLibrary::joinPicThreads(string filename) {
    PicWrapper* pic = loadedPictures[filename];
    for(auto t : pic->threads) {
        t.join(); 
    }
    pic->threads.clear();
}

void PicLibrary::joinAllThreads() {
    for (unordered_map<std::string, PicWrapper *>::const_iterator it = loadedPictures.begin();
         it != loadedPictures.end(); ++it) {
        joinPicThreads(it->first);
    }
}

bool PicLibrary::checkMapforFile(string filename) {
    for (unordered_map<std::string, PicWrapper *>::const_iterator it = loadedPictures.begin();
         it != loadedPictures.end(); ++it) {
        if(it->first == filename) {
            return true;
        }
    }
    return false; 
}

PicWrapper* PicLibrary::getWrapper(string filename) {
    for (unordered_map<std::string, PicWrapper *>::const_iterator it = loadedPictures.begin();
         it != loadedPictures.end(); ++it) {
        if(it->first == filename) {
            return it->second;
        }
    }
    return NULL;
}

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

/* SEQUENTIAL IMPLEMENTATION
void PicLibrary::loadpicture(string path, string filename)
{
    string jpg = ".jpg";
    if(!PicLibrary::isJPG(path, jpg)) {
        cerr << "Picture is not a .jpg file" << endl;
    } else if(loadedPictures.end() == loadedPictures.find(filename)) {
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
*/

//MODIFIED
void PicLibrary::loadpicture(string path, string filename)
{
    string jpg = ".jpg";
    if(!PicLibrary::isJPG(path, jpg)) {
        cerr << "Picture is not a .jpg file" << endl;
    } else if(loadedPictures.end() == loadedPictures.find(filename)) {
        Picture toAdd = Picture(path);
        if(toAdd.getheight() != 0) {
            PicWrapper* toA = new PicWrapper(&toAdd);
            loadedPictures.insert({filename, toA});
            cout << filename << " saved!" << endl;
        } else {
            cout << path << " doesn't exist!" << endl;
        }
    } else {
        cerr << "Picture already exists" << endl;
    }
}

//SEQUENTIAL IS THE SAME
void PicLibrary::unloadpicture(string filename) {
    if(loadedPictures.find(filename) != loadedPictures.end()) {
        loadedPictures.erase(filename);
        cout << filename << " successfully removed!" << endl;
    } else {
        cerr << filename << " does not exist in the picture library's internal picture store!" << endl;
    }
}

void PicLibrary::savepicture(string filename, string path) {
    if(loadedPictures.find(filename) != loadedPictures.end()) {
        auto picture = loadedPictures.find(filename)->second->pic;
        Utils util;
        util.saveimage(picture.getimage(), path);
        cout << filename << " has been saved!" << endl;
    } else {
        cerr << filename << " does not exist in the picture library's internal picture store!" << endl;
    }
}

void PicLibrary::display(string filename) {
    if(loadedPictures.find(filename) != loadedPictures.end()) {
        auto picture = loadedPictures.find(filename)->second->pic;
        Utils util;
        util.displayimage(picture.getimage());
    } else {
        cerr << filename << " does not exist in the picture library's internal picture store!" << endl;
    }
}

void PicLibrary::invert(string filename) {
    loadedPictures[filename]->mtex.lock();
    if(loadedPictures.find(filename) != loadedPictures.end()) {
        Picture pic = loadedPictures[filename]->pic;
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
    loadedPictures[filename]->mtex.unlock();
}

void PicLibrary::grayscale(string key) {
    loadedPictures[key]->mtex.lock();
    Picture pic = loadedPictures[key]->pic;
    for(int i = 0; i < pic.getwidth(); i++) {
        for(int j = 0; j < pic.getheight(); j++) {
            Colour temp = pic.getpixel(i, j);
            int average = (temp.getred() + temp.getgreen() + temp.getblue())/3;
            pic.setpixel(i, j, Colour(average, average, average));
        }
    }
    loadedPictures[key]->mtex.unlock();
}

void PicLibrary::flipVH(char plane, string filename) {
    switch(plane) {
        case('V'): flipV(filename); break;
        case('H'): flipH(filename); break;
    }
}

void PicLibrary::flipV(string filename){
    loadedPictures[filename]->mtex.lock();
    Picture oldPic = loadedPictures[filename]->pic;
    Picture newPic = Picture(oldPic.getwidth(), oldPic.getheight());
    for(int i = 0; i < oldPic.getheight(); i++) {
        for(int j = 0; j < oldPic.getwidth(); j++) {
            newPic.setpixel(j, i, Colour(oldPic.getpixel(j, ((oldPic.getheight() - 1) - i))));
        }
    }
    oldPic.setimage(newPic.getimage());
    loadedPictures[filename]->mtex.unlock();
}

void PicLibrary::flipH(string filename) {
    loadedPictures[filename]->mtex.lock();
    Picture oldPic = loadedPictures[filename]->pic;
    Picture newPic = Picture(oldPic.getwidth(), oldPic.getheight());
    for (int i = 0; i < oldPic.getheight(); i++) {
        for (int j = 0; j < oldPic.getwidth(); j++) {
            newPic.setpixel(j, i, Colour(oldPic.getpixel(((oldPic.getwidth() - 1) - j), i)));
        }
    }
    oldPic.setimage(newPic.getimage());
    loadedPictures[filename]->mtex.unlock();
}

void PicLibrary::rotate(int angle, string filename) {
    switch(angle) {
        case(90): rotate90(filename); break;
        case(180): rotate180(filename); break;
        case(270): rotate270(filename); break;
    }
}

void PicLibrary::rotate270(string filename) {
    rotate90(filename);
    rotate90(filename);
    rotate90(filename);
}

void PicLibrary::rotate180(string filename) {
    rotate90(filename);
    rotate90(filename);
}

void PicLibrary::rotate90(string filename) {
    loadedPictures[filename]->mtex.lock();
    auto oldPic = loadedPictures[filename]->pic;
    int width = oldPic.getwidth();
    int height = oldPic.getheight();
    Picture newPic = Picture(height, width);

    for(int x = 0; x < width; x++){
        for(int y = 0; y < height; y++){
            newPic.setpixel(y, x, oldPic.getpixel(x, (oldPic.getheight() - y - 1)));
        }
    }
    oldPic.setimage(newPic.getimage());
    loadedPictures[filename]->mtex.unlock();
}


void PicLibrary::blur(string filename) {
    loadedPictures[filename]->mtex.lock();
    auto oldPic = loadedPictures[filename]->pic;
    Picture newPic = Picture(oldPic.getwidth(), oldPic.getheight());
    for(int i = 0; i < (oldPic.getheight()); i++) {
        for(int j = 0; j < (oldPic.getwidth()); j++) {
            if((i != 0) && (j != 0) && (i != (oldPic.getheight() - 1)) && (j != (oldPic.getwidth() - 1))) {
                int red = 0;
                int blue = 0;
                int green = 0;
                for(int k = (i - 1); k <= (i + 1); ++k) {
                    for(int l = (j - 1); l <= (j + 1); ++l) {
                        Colour colour = oldPic.getpixel(l, k);
                        red += colour.getred();
                        green += colour.getgreen();
                        blue += colour.getblue();
            }
        }
        newPic.setpixel(j, i, Colour(red/9, green/9, blue/9));
    } else {
        newPic.setpixel(j, i, oldPic.getpixel(j, i));
            }
        }
    }
    oldPic.setimage(newPic.getimage());
    loadedPictures[filename]->mtex.unlock();
}

