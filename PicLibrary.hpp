#ifndef PICLIB_H
#define PICLIB_H

#include <unordered_map>
#include "Utils.hpp"
#include "PicWrapper.hpp"

class PicLibrary {

private:
    unordered_map<std::string, PicWrapper *> loadedPictures;

public:
    // defaiult constructor/deconstructor
    PicLibrary() {};

    ~PicLibrary() {};

    // command-line interpreter routines
    void print_picturestore();

    void loadpicture(string path, string filename);

    void unloadpicture(string filename);

    void savepicture(string filename, string path);

    void display(string filename);

    inline bool isJPG(string const &filename, string const &ending);

    bool checkMapforFile(string filename);

    PicWrapper *getWrapper(string filename);

    void exit();

    void rotate90(string filename);

    void rotate180(string filename);

    void rotate270(string filename);

    void flipV(string filename);

    void flipH(string filename);

    void joinPicThreads(string filename);

    void joinAllThreads();

    int average(int blue, int red, int green);


    //transformation functions
    void invert(string filename);

    void grayscale(string filename);

    void rotate(int angle, string filename);

    void flipVH(char plane, string filename);

    void blur(string filename);

    void blur_row(string filename);

    void blur_rowHelper(int y, Picture *originalPic, Picture *newPic);

    void blur_column(string filename);

    void blur_columnHelper(int y, Picture *originalPic, Picture *newPic);

    void pixel_blur(string filename);
    };

#endif

/*                              Brief description of my synchronization strategy:
 *
 * I added a new class to the cpp_pic_proc_nc1517 project called PicWrapper. This PicWrapper class contains a mutex to
 * lock the respective PicWrapper, a Picture pic and a vector of threads.
 * In the PicLibrary class, I added an unordered map called "loadedpictures" to keep track of all the pictures loaded
 * into internal storage. Every picture that was loaded can be identified by its key, the filename, and value,
 * a PicWrapper containing that particular picture.
 * In the main function, I check which command was entered and then push back a newly created thread with the task of
 * executing the command that was entered into the vector of threads of the PicWrapper belonging to the also
 * entered filename in "loadedpictures". That way, every thread in the vector of threads of each picture has its
 * own task to execute. In order to make sure that these commands are executed in the right order, I implemented helper
 * functions iterate over the vector of threads and joins the threads. Joining the threads ensures that each thread has
 * to wait to run until the currently running thread is done executing its task, so all the commands are executed in
 * order. When performing transformations on any picture, I lock the PicWrapper that contains the picture that is being
 * transformed before doing any operation on it. At the end of any operation that transforms a picture, e.g. invert,
 * I unlock the PicWrapper belonging to that picture using its mutex.
 * */

