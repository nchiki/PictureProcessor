#ifndef CPP_PIC_PROC_NC1517_PICWRAPPER_H
#define CPP_PIC_PROC_NC1517_PICWRAPPER_H


#include <mutex>
#include <thread>
#include "Picture.hpp"

class PicWrapper {

public:
    Picture _pic;
    std::mutex _mtx;
    vector<thread> _threads;
    PicWrapper(Picture* pic) {};
    ~PicWrapper() {};

};

#endif //CPP_PIC_PROC_NC1517_PICWRAPPER_H
