#include <iostream>
#include <limits>

#include "Colour.hpp"
#include "Utils.hpp"
#include "Picture.hpp"
#include "PicLibrary.hpp"

using namespace std;

int main(int argc, char **argv) {
    PicLibrary lib = PicLibrary();

    //loads all needed pictures
    for (int i = 1; argv[i] != NULL; ++i) {
        size_t start = string(argv[i]).rfind("/");
        char *name;
        if (start == string::npos) {
            name = argv[i];
        } else {
            name = &argv[i][start + 1];
        }

        lib.loadpicture(argv[i], name);
    }

    //VON NIRANJAN ERKLÄREN LASSEN (Part hier vor)

    string cmd;
    string name;
    string path;
    int angle;
    char plane;
    cin >> cmd;

    while (cmd != "exit" && cin.peek() != EOF) {

        if (cmd == "liststore") {
            lib.print_picturestore();
        }
        if (cmd == "load") {
            cin >> path >> name;
            lib.loadpicture(path, name);
        }
        if (cmd == "unload") {
            cin >> name;
            lib.unloadpicture(name);
        }
        if (cmd == "invert") {
            cin >> name; lib.invert(name);
        }
        if (cmd == "save") {
            cin >> name >> path;
            lib.savepicture(name, path);
        }
        if (cmd == "display") {
            cin >> name; lib.display(name);
        }
        if (cmd == "rotate") {
            cin >> angle >> name;
            lib.rotate(angle, name);
        }
        if (cmd == "grayscale"){
            cin >> name; lib.grayscale(name);
        }
        if (cmd == "flip") {
            cin >> plane >> name;
            lib.flipVH(plane, name);
        }
        if (cmd == "blur") {
            cin >> name;
            lib.blur(name);
        } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        cin >> cmd;
    }
    return 0;
}


