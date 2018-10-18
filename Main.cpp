#include <iostream>

#include "Colour.hpp"
#include "Utils.hpp"
#include "Picture.hpp"
#include "PicLibrary.hpp"
#include <thread>

using namespace std;


int main(int argc, char ** argv)
{
    PicLibrary lib;

    for ( int i = 1 ; argv[i] != NULL; ++i)
    {
        char *name;
        size_t name_start = string(argv[i]).rfind("/")+1;

        if (name_start == string::npos)
        {
            name = argv[i];
        }
        else
        {
            name = &argv[i][name_start];
        }

        lib.loadpicture(argv[i], name);
    }

    string command, name, filepath;
    int angle;
    char plane;



    while ( cin >> command && command != "exit")
    {
        if (command == "liststore")
        {

            lib.print_picturestore();
        }

        if (command == "load")
        {
            cin >> filepath >> name;
            lib.loadpicture(filepath,name);
            // t1 = thread(&PicLibrary::loadpicture, &lib, filepath, name);
            //threadVector.emplace_back(&PicLibrary::loadpicture, &lib, filepath, name);
        }

        if (command == "unload")
        {
            cin >> name;
            lib.joinPicThreads(name);
            lib.unloadpicture(name);
            //threadVector.emplace_back(thread(&PicLibrary::unloadpicture, &lib, name));
        }

        if (command == "save")
        {
            cin >> name >> filepath;
            lib.joinPicThreads(name);
            lib.savepicture(name,filepath);
            //threadVector.emplace_back(thread(&PicLibrary::savepicture, &lib, name, filepath));
        }

        if (command == "display")
        {
            cin >> name;
            lib.joinPicThreads(name);
            lib.display(name);
            //threadVector.emplace_back(thread(&PicLibrary::display, &lib, name));
        }

        if (command == "grayscale")
        {
            cin >> name;
            if(lib.checkIfExists(name)) {
                auto something = lib.getWrap(name);
                something->_threads.push_back(thread(&PicLibrary::grayscale, &lib, name));
                // lib.grayscale(name);
                //threadVector.emplace_back(thread(&PicLibrary::grayscale, &lib, name));
            }
        }

        if (command == "invert")
        {
            cin >> name;
            if(lib.checkIfExists(name)) {
                auto something = lib.getWrap(name);
                something->_threads.push_back(thread(&PicLibrary::invert, &lib, name));
                // t = new thread(&PicLibrary::invert, &lib, name);
            }
            // lib.invert(name);
            //threadVector.emplace_back(thread(&PicLibrary::invert, &lib, name));
        }

        if (command == "rotate")
        {
            cin >> angle >> name;
            if (lib.checkIfExists(name)) {
                auto something = lib.getWrap(name);
                something->_threads.push_back(thread(&PicLibrary::rotate, &lib, angle, name));

                // lib.rotate(angle, name);
            }
            //threadVector.emplace_back(thread(&PicLibrary::rotate, &lib, angle, name));
        }

        if (command == "flip")
        {
            cin >> plane >> name;
            if(lib.checkIfExists(name)) {
                auto something = lib.getWrap(name);
                something->_threads.push_back(thread(&PicLibrary::flipVH, &lib, plane, name));
                // lib.flipVH(plane, name);
                //threadVector.emplace_back(thread(&PicLibrary::flipVH, &lib, plane, name));
            }

        }

        if (command == "blur")
        {
            cin >> name;
            if (lib.checkIfExists(name)) {
                auto something = lib.getWrap(name);
                something->_threads.push_back(thread(&PicLibrary::blur, &lib, name));
                // lib.blur(name);
                //threadVector.emplace_back(thread(&PicLibrary::blur, &lib, name));
            }
        }
        if ( cin.peek() == EOF)
        {
            break;
        }
        else
        {
            cin.ignore(numeric_limits<streamsize>::max(),'\n');
        }


    }
    lib.joinAllThreads();




    //for_each(threadVector.begin(), threadVector.end(), [](thread &t){t.join();});
    return 0;

}


