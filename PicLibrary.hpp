
#ifndef PICLIB_H
#define PICLIB_H

#include<unordered_map>
#include "Picture.hpp"
#include "Utils.hpp"
#include "PicWrapper.hpp"
#include <atomic>
#include <memory>
#include <mutex>

class PicLibrary {

private:
    unordered_map<std::string, PicWrapper*> loadedPictures;

    atomic<int> _currentThreadCount;




    // function to check if a string ends with .jpg
    static inline bool ends_with(string const & value, string const & ending );

    // function to rotate once by 90, can be used multiple times
    void rotate90(string filename);


    void doHFlip(string filename);
    void doVFlip(string filename);

    static Colour avgColor(int i, int j, Picture *given);
public:
    // defaiult constructor/deconstructor
    PicLibrary(){};
    ~PicLibrary(){};

    // command-line interpreter routines
    void print_picturestore();
    void loadpicture(string path, string filename);
    void unloadpicture(string filename);
    void savepicture(string filename, string path);
    void display(string filename);

    // picture transformation routines
    void invert(string filename);
    void grayscale(string filename);
    void rotate(int angle, string filename);
    void flipVH(char plane, string filename);
    void blur(string filename);


    void _incrementThreadCount();
    void _decrementThreadCount();
    int _getCurrentThreadCount();

    bool checkIfExists(string filename);

    PicWrapper* getWrap(string filename);


    //functions to join all the active threads on each/all pictures before load/save/unload
    void joinAllThreads();
    void joinPicThreads(string filename);

};

#endif

