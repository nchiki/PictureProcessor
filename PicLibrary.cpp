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

void PicLibrary::grayscale(string key) {
    Picture pic = loadedPictures[key];
    for(int i = 0; i < pic.getwidth(); i++) {
        for(int j = 0; j < pic.getheight(); j++) {
            Colour temp = pic.getpixel(i, j);
            int average = (temp.getred() + temp.getgreen() + temp.getblue())/3;
            pic.setpixel(i, j, Colour(average, average, average));
        }
    }
}

void PicLibrary::flipVH(char plane, string filename) {
    switch(plane) {
        case('V'): flipV(filename);
        case('H'): flipH(filename);
    }
}

void PicLibrary::flipV(string filename){
    Picture oldPic = loadedPictures[filename];
    Picture newPic = Picture(oldPic.getwidth(), oldPic.getheight());
    for(int i = 0; i < oldPic.getheight(); i++) {
        for(int j = 0; j < oldPic.getwidth(); j++) {
            newPic.setpixel(j, i, Colour(oldPic.getpixel(j, ((oldPic.getheight() - 1) - i))));
        }
    }
}

void PicLibrary::flipH(string filename) {
    Picture oldPic = loadedPictures[filename];
    Picture newPic = Picture(oldPic.getwidth(), oldPic.getheight());
    for (int i = 0; i < oldPic.getheight(); i++) {
        for (int j = 0; j < oldPic.getwidth(); j++) {
            newPic.setpixel(j, i, Colour(oldPic.getpixel(((oldPic.getwidth() - 1) - j), i)));
        }

    }
}

void PicLibrary::rotate(int angle, string filename) {
    Picture oldPic = loadedPictures[filename];
    Picture newPic = Picture(oldPic.getwidth(), oldPic.getheight());
    switch(angle) {
        case(90): rotate90(filename, &newPic, &oldPic);
        case(180): rotate180(filename, &newPic, &oldPic);
        case(270): rotate270(filename, &newPic, &oldPic);
    }
}

void PicLibrary::rotate270(string filename, Picture *newPic, Picture *oldPic) {
    for(int i = 0; i < oldPic->getheight(); i++){
        for(int j = 0; j < oldPic->getwidth(); j++){
            newPic->setpixel(i, oldPic->getwidth() - j, oldPic->getpixel(j, i));
        }
    }
}

void PicLibrary::rotate180(string filename, Picture *newPic, Picture *oldPic) {
     for(int j = 0; j < oldPic->getwidth(); j++){
            for(int i = 0; i < oldPic->getheight(); i++){
                newPic->setpixel(((newPic->getwidth()) - j), newPic->getheight() - i, newPic->getpixel(j, i));
            }
        }
}

void PicLibrary::rotate90(string filename, Picture *newPic, Picture *oldPic) {
    for(int i = 0; i < oldPic->getheight(); i++) {
        for(int j = 0; j < newPic->getwidth(); j++) {
            newPic->setpixel(((newPic->getwidth() - 1) - i), j, Colour(oldPic->getpixel(j, i)));
        }
    }
}


void PicLibrary::blur(string filename) {
    Picture oldPic = loadedPictures[filename];
    Picture newPic = Picture(oldPic.getwidth(), oldPic.getheight());
    for(int i = 0; i < (oldPic.getheight()); i++) {
        for(int j = 0; j < (oldPic.getheight()); j++) {
            if((i != 0) && (j != 0) && (i != (oldPic.getheight() - 1)) && (j != (oldPic.getwidth() - 1))) {
                int red = 0;
                int blue = 0;
                int green = 0;
                for(int k = (i - 1); k <= (i + 1); ++k) {
                    for(int l = (j - 1); l <= (j + 1); ++l) {
                        Colour colour = newPic.getpixel(l, k);
                        red += colour.getred();
                        green += colour.getgreen();
                        blue += colour.getblue();
            }
        }
        newPic.setpixel(j, i, Colour (red/9, green/9, blue/9));
    } else {
        newPic.setpixel(j, i, oldPic.getpixel(j, i));
            }
        }
    }
}

/*
void PicLibrary::exit() {
    delete(loadedPictures);
} */