#ifndef CPP_PIC_PROC_NC1517_PICWRAPPER_H
#define CPP_PIC_PROC_NC1517_PICWRAPPER_H


#include <mutex>
#include <thread>
#include "Picture.hpp"

class PicWrapper {

public:
    Picture pic;
    std::mutex mtex;
    vector<thread> threads;
    PicWrapper(Picture* pic) {};
    ~PicWrapper() {};

};

#endif //CPP_PIC_PROC_NC1517_PICWRAPPER_H
