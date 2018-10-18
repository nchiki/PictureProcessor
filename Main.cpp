#include <iostream>
#include <limits>

#include "Colour.hpp"
#include "Utils.hpp"
#include "Picture.hpp"
#include "PicLibrary.hpp"

using namespace std;

int main(int argc, char **argv) {
    PicLibrary lib;

    //loads all needed pictures
    for (int i = 1; argv[i] != NULL; ++i) {
        size_t start = string(argv[i]).rfind("/")+1;
        char *name;
        if (start == string::npos) {
            name = argv[i];
        } else {
            name = &argv[i][start];
        }

        lib.loadpicture(argv[i], name);
    }

    string cmd;
    string name;
    string path;
    int angle;
    char plane;

    while (cin >> cmd && cmd != "exit" && cin.peek() != EOF) {

        if (cmd == "liststore") {
            lib.print_picturestore();
        }
        if (cmd == "load") {
            cin >> path >> name;
            lib.loadpicture(path, name);
        }
        if (cmd == "unload") {
            cin >> name;
            lib.joinPicThreads(name);
            lib.unloadpicture(name);
        }
        if (cmd == "invert") {
            cin >> name;
            if(lib.checkMapforFile(name)) {
                lib.getWrapper(name)->threads.push_back(thread(&PicLibrary::invert, &lib, name));
            }
        }
        if (cmd == "save") {
            cin >> name >> path;
            lib.joinPicThreads(name);
            lib.savepicture(name, path);
        }
        if (cmd == "display") {
            cin >> name;
            lib.joinPicThreads(name);
            lib.display(name);
        }
        if (cmd == "rotate") {
            cin >> angle >> name;
            if(lib.checkMapforFile(name)) {
                lib.getWrapper(name)->threads.push_back(thread(&PicLibrary::rotate, &lib, angle, name));
            }
        }
        if (cmd == "grayscale"){
            cin >> name;
            if(lib.checkMapforFile(name)) {
                lib.getWrapper(name)->threads.push_back(thread(&PicLibrary::grayscale, &lib, name));
            }
        }
        if (cmd == "flip") {
            cin >> plane >> name;
            if(lib.checkMapforFile(name)) {
                lib.getWrapper(name)->threads.push_back(thread(&PicLibrary::flipVH, &lib, plane, name));
            }
        }
        if (cmd == "blur") {
            cin >> name;
            if(lib.checkMapforFile(name)) {
                lib.getWrapper(name)->threads.push_back(thread(&PicLibrary::blur, &lib, name));
            }
        } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
    lib.joinAllThreads();
    return 0;
}


