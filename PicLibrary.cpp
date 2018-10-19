#include <algorithm>
#include "PicLibrary.hpp"
#include "Colour.hpp"


void PicLibrary::joinPicThreads(string filename) {
    if (checkMapforFile(filename)) {
        std::for_each(loadedPictures[filename]->threads.begin(), loadedPictures[filename]->threads.end(),
                      [](thread &thr) { thr.join(); });
        loadedPictures[filename]->threads.clear();
    }
}

void PicLibrary::joinAllThreads() {
    for (auto item : loadedPictures) {
        std::for_each(item.second->threads.begin(), item.second->threads.end(), [](thread &thr) { thr.join(); });
        item.second->threads.clear();
    }
}

bool PicLibrary::checkMapforFile(string filename) {
    return loadedPictures.find(filename) != loadedPictures.end();
}

PicWrapper *PicLibrary::getWrapper(string filename) {
    if (checkMapforFile(filename)) {
        return loadedPictures[filename];
    }
}

inline bool PicLibrary::isJPG(string const &filename, string const &ending) {
    if (ending.size() > filename.size()) {
        return false;
    } else {
        return std::equal(ending.rbegin(), ending.rend(), filename.rbegin());
    }
}

void PicLibrary::print_picturestore() {
    vector<string> keys;
    keys.reserve(loadedPictures.size());
    for (auto &item : loadedPictures) {
        keys.push_back(item.first);
    }
    std::sort(keys.begin(), keys.end());
    for (auto &item : keys) {
        cout << item << endl;
    }
}


void PicLibrary::loadpicture(string path, string filename) {
    string jpg = ".jpg";
    if (!PicLibrary::isJPG(path, jpg)) {
        cerr << "Picture is not a .jpg file" << endl;
    }
    if (loadedPictures.end() == loadedPictures.find(filename)) {
        cout << path << endl;
        Picture *toAdd = new Picture(path);
        if (toAdd->getheight() != 0) {
            PicWrapper *toA = new PicWrapper(toAdd);

            loadedPictures.insert({filename, toA});
            cout << filename << " saved!" << endl;
        } else {
            cout << path << " doesn't exist!" << endl;
        }
    } else {
        cerr << "Picture already exists" << endl;
    }
}

void PicLibrary::unloadpicture(string filename) {
    if (loadedPictures.find(filename) != loadedPictures.end()) {
        loadedPictures.erase(filename);
        cout << filename << " successfully removed!" << endl;
    } else {
        cerr << filename << " does not exist in the picture library's internal picture store!" << endl;
    }
}

void PicLibrary::savepicture(string filename, string path) {
    if (loadedPictures.find(filename) != loadedPictures.end()) {
        auto wrapper = loadedPictures[filename];
        Utils util;
        if (wrapper->pic.getheight() != 0) {
            util.saveimage(wrapper->pic.getimage(), path);
            cout << filename << " has been saved!" << endl;
        }
    } else {
        cout << filename << " does not exist in the picture library's internal picture store!" << endl;
    }
}

void PicLibrary::display(string filename) {
    if (loadedPictures.find(filename) != loadedPictures.end()) {
        auto wrapper = loadedPictures[filename];
        Utils util;
        util.displayimage(wrapper->pic.getimage());
    } else {
        cerr << filename << " does not exist in the picture library's internal picture store!" << endl;
    }
}

void PicLibrary::invert(string filename) {
    auto wrapper = loadedPictures[filename];
    wrapper->mtex.lock();
    for (int i = 0; i < wrapper->pic.getheight(); i++) {
        for (int j = 0; j < wrapper->pic.getwidth(); j++) {
            Colour temp = wrapper->pic.getpixel(j, i);
            Colour newC = Colour(255 - temp.getred(), 255 - temp.getgreen(), 255 - temp.getblue());
            wrapper->pic.setpixel(j, i, newC);
        }
    }
    cout << filename << "has been inverted" << endl;
    wrapper->mtex.unlock();
}

void PicLibrary::grayscale(string filename) {
    auto wrapper = loadedPictures[filename];
    wrapper->mtex.lock();
    for (int i = 0; i < wrapper->pic.getwidth(); i++) {
        for (int j = 0; j < wrapper->pic.getheight(); j++) {
            Colour temp = wrapper->pic.getpixel(i, j);
            wrapper->pic.setpixel(i, j, Colour(average(temp.getblue(), temp.getred(), temp.getgreen()),
                                               average(temp.getblue(), temp.getred(), temp.getgreen()),
                                               average(temp.getblue(), temp.getred(), temp.getgreen())));
        }
    }
    wrapper->mtex.unlock();
}

int PicLibrary::average(int blue, int red, int green) {
    return ((blue + red + green) / 3);
}

void PicLibrary::flipVH(char plane, string filename) {
    switch (plane) {
        case ('V'):
            flipV(filename);
            break;
        case ('H'):
            flipH(filename);
            break;
    }
}


void PicLibrary::flipV(string filename) {
    auto wrapper = loadedPictures[filename];
    loadedPictures[filename]->mtex.lock();
    Picture newPic(wrapper->pic.getwidth(), wrapper->pic.getheight());
    for (int i = 0; i < wrapper->pic.getheight(); i++) {
        for (int j = 0; j < wrapper->pic.getwidth(); j++) {
            newPic.setpixel(j, i, Colour(wrapper->pic.getpixel(j, ((wrapper->pic.getheight() - 1) - i))));
        }
    }
    wrapper->pic.setimage(newPic.getimage());
    wrapper->mtex.unlock();
}

void PicLibrary::flipH(string filename) {
    auto wrapper = loadedPictures[filename];
    wrapper->mtex.lock();
    Picture newPic(wrapper->pic.getwidth(), wrapper->pic.getheight());
    for (int i = 0; i < wrapper->pic.getheight(); i++) {
        for (int j = 0; j < wrapper->pic.getwidth(); j++) {
            newPic.setpixel(j, i, Colour(wrapper->pic.getpixel(((wrapper->pic.getwidth() - 1) - j), i)));
        }
    }
    wrapper->pic.setimage(newPic.getimage());
    wrapper->mtex.unlock();
}


void PicLibrary::rotate(int angle, string filename) {
    switch (angle) {
        case (90):
            rotate90(filename);
            break;
        case (180):
            rotate180(filename);
            break;
        case (270):
            rotate270(filename);
            break;
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
    auto wrapper = loadedPictures[filename];
    wrapper->mtex.lock();
    int width = wrapper->pic.getwidth();
    int height = wrapper->pic.getheight();
    Picture newPic = Picture(height, width);

    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            newPic.setpixel(y, x, wrapper->pic.getpixel(x, wrapper->pic.getheight() - y - 1));
        }
    }
    wrapper->pic.setimage(newPic.getimage());
    wrapper->mtex.unlock();
}


void PicLibrary::blur(string filename) {
    auto wrapper = loadedPictures[filename];
    wrapper->mtex.lock();
    Picture newPic(wrapper->pic.getwidth(), wrapper->pic.getheight());
    for (int i = 0; i < (wrapper->pic.getheight()); i++) {
        for (int j = 0; j < (wrapper->pic.getwidth()); j++) {
            if ((i != 0) && (j != 0) && (i != (wrapper->pic.getheight() - 1)) && (j != (wrapper->pic.getwidth() - 1))) {
                int red = 0;
                int blue = 0;
                int green = 0;
                for (int k = (i - 1); k <= (i + 1); ++k) {
                    for (int l = (j - 1); l <= (j + 1); ++l) {
                        Colour colour = wrapper->pic.getpixel(l, k);
                        red += colour.getred();
                        green += colour.getgreen();
                        blue += colour.getblue();
                    }
                }
                newPic.setpixel(j, i, Colour(red / 9, green / 9, blue / 9));
            } else {
                newPic.setpixel(j, i, wrapper->pic.getpixel(j, i));
            }
        }
    }
    wrapper->pic.setimage(newPic.getimage());
    wrapper->mtex.unlock();
}


// BLUR OPTIMISED IMPLEMENTATIONS
/*
 *
 * ROW-BY-ROW IMPLEMENTATION
 *
static void blur_rowHelper(int y, Picture *originalPic, Picture *newPic) {
    for (int x = 0; x < originalPic->getwidth(); x++) {
        if ((y != 0) && (x != 0) && (y != (originalPic->getheight() - 1)) && (x != (originalPic->getwidth() - 1))) {
            int red = 0;
            int blue = 0;
            int green = 0;
            for (int i = (y - 1); i <= (i + 1); ++i) {
                for (int j = (x - 1); j <= (x + 1); ++j) {
                    Colour colour = originalPic->getpixel(j, i);
                    red += colour.getred();
                    green += colour.getgreen();
                    blue += colour.getblue();
                }
            }
            newPic->setpixel(x, y, Colour(red / 9, green / 9, blue / 9));
        } else {
            newPic->setpixel(x, y, originalPic->getpixel(x, y));
        }
    }
}

void PicLibrary::blur_row(string filename) {
    auto wrapper = loadedPictures[filename];
    Picture originalPic = wrapper->pic;
    Picture newPic = Picture(originalPic.getwidth(), originalPic.getheight());
    vector<thread> threads;
    for (int numberOfRows = 0; numberOfRows < originalPic.getheight(); numberOfRows++) {
        threads.push_back(thread(blur_rowHelper, numberOfRows, &originalPic, &newPic));
    }
    std::for_each(threads.begin(), threads.end(), [](thread &t) { t.join(); });
    wrapper->pic.setimage(newPic.getimage());
}

 COLUMN-BY-COLUMN IMPLEMENTATION

static void blur_columnHelper(int x, Picture *originalPic, Picture *newPic) {
    for (int y = 0; y < originalPic->getheigth(); y++) {
        if ((x != 0) && (x != 0) && (x != (originalPic->getwidth() - 1)) && (y != (originalPic->getheigth() - 1))) {
            int red = 0;
            int blue = 0;
            int green = 0;
            for (int i = (x - 1); i <= (i + 1); ++i) {
                for (int j = (y - 1); j <= (y + 1); ++j) {
                    Colour colour = originalPic->getpixel(j, i);
                    red += colour.getred();
                    green += colour.getgreen();
                    blue += colour.getblue();
                }
            }
            newPic->setpixel(x, y, Colour(red / 9, green / 9, blue / 9));
        } else {
            newPic->setpixel(x, y, originalPic->getpixel(x, y));
        }
    }
}

void PicLibrary::blur_column(string filename) {
    auto wrapper = loadedPictures[filename];
    Picture originalPic = wrapper->pic;
    Picture newPic = Picture(originalPic.getwidth(), originalPic.getheight());
    vector<thread> threads;
    for (int numberOfColumns = 0; numberOfColumns < originalPic.getwidth(); numberOfRows++) {
        threads.push_back(thread(blur_rowHelper, numberOfColumns, &originalPic, &newPic));
    }
    std::for_each(threads.begin(), threads.end(), [](thread &t) { t.join(); });
    wrapper->pic.setimage(newPic.getimage());
}


 PIXEL-BY-PIXEL IMPLEMENTATION

void pixel_blurHelper(Picture* originalPic, Picture* newPic) {
    auto wrapper = loadedPictures[filename];
    Picture newPic(wrapper->pic.getwidth(), wrapper->pic.getheight());
    for (int i = 0; i < (wrapper->pic.getheight()); i++) {
        for (int j = 0; j < (wrapper->pic.getwidth()); j++) {
            if ((i != 0) && (j != 0) && (i != (wrapper->pic.getheight() - 1)) && (j != (wrapper->pic.getwidth() - 1))) {
                int red = 0;
                int blue = 0;
                int green = 0;
                for (int k = (i - 1); k <= (i + 1); ++k) {
                    for (int l = (j - 1); l <= (j + 1); ++l) {
                        Colour colour = wrapper->pic.getpixel(l, k);
                        red += colour.getred();
                        green += colour.getgreen();
                        blue += colour.getblue();
                    }
                }
                newPic.setpixel(j, i, Colour(red / 9, green / 9, blue / 9));
            } else {
                newPic.setpixel(j, i, wrapper->pic.getpixel(j, i));
            }
        }
    }
    wrapper->pic.setimage(newPic.getimage());
}


void PicLibrary::pixel_blur(string filename) {
    auto wrapper = loadedPictures[filename];
    Picture originalPic = wrapper->pic;
    Picture newPic = Picture(originalPic.getwidth(), originalPic.getheight());
    vector<thread> threads;
    for (int numberOfPixels = 0; numberOfPixels < (originalPic.getwidth() * originalPic.getheight()); numberOfPixels++) {
        threads.push_back(thread(blur_rowHelper, numberOfPixels, &originalPic, &newPic));
    }
    std::for_each(threads.begin(), threads.end(), [](thread &t) { t.join(); });
    wrapper->pic.setimage(newPic.getimage());
}

*/