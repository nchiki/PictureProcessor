#ifndef PICWRAPPER_H
#define PICWRAPPER_H


#include <mutex>
#include <thread>
#include "Picture.hpp"

class PicWrapper {

public:
    Picture pic;
    std::mutex mtex;
    vector<thread> threads;

    PicWrapper(Picture *pic);

    ~PicWrapper() {};
};

#endif //PICWRAPPER_H
