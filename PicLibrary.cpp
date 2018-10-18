#include <algorithm>
#include "PicLibrary.hpp"
#include "Colour.hpp"


inline bool PicLibrary::ends_with(string const &value, string const &ending)
{
    if ( ending.size() > value.size()) return false;
    return std::equal(ending.rbegin(),ending.rend(), value.rbegin());
}

void PicLibrary::print_picturestore()
{


    vector<string> keys;
    keys.reserve(loadedPictures.size());
    for (auto &it : loadedPictures)
    {
        keys.push_back(it.first);
    }
    std::sort(keys.begin(), keys.end());

    for (auto &it : keys)
    {
        cout << it << std::flush << endl;
    }


}

void PicLibrary::unloadpicture(string filename)
{
    if (loadedPictures.find(filename) != loadedPictures.end())
    {
        loadedPictures.erase(filename);
    }
    else
    {
        cerr << "Given filename does not exist in internal storage" <<endl;
    }
}

void PicLibrary::doHFlip(string filename)
{
    auto toBeModded = loadedPictures[filename];

    std::lock_guard<std::mutex> lock(toBeModded->_mtx);

    Picture newPic(toBeModded->_pic.getwidth(),toBeModded->_pic.getheight());
    for ( int x = 0 ; x < toBeModded->_pic.getwidth(); ++x)
    {
        for ( int y = 0 ; y < toBeModded->_pic.getheight(); ++y)
        {
            newPic.setpixel(toBeModded->_pic.getwidth()-1-x,y, toBeModded->_pic.getpixel(x,y) );
        }
    }

    toBeModded->_pic.setimage(newPic.getimage());
    //loadedPictures.erase(filename);
    //loadedPictures.insert({filename,newPic});
}

void PicLibrary::doVFlip(string filename)
{

    auto toBeModded = loadedPictures[filename];
    std::lock_guard<std::mutex> lock(toBeModded->_mtx);
    Picture newPic(toBeModded->_pic.getwidth(),toBeModded->_pic.getheight());
    for ( int x = 0 ; x < toBeModded->_pic.getwidth(); ++x)
    {
        for ( int y = 0 ; y < toBeModded->_pic.getheight(); ++y)
        {
            newPic.setpixel(x,toBeModded->_pic.getheight()-1-y, toBeModded->_pic.getpixel(x,y) );
        }
    }
    toBeModded->_pic.setimage(newPic.getimage());
    // loadedPictures.erase(filename);
    //loadedPictures.insert({filename,&newPic});

}
void PicLibrary::flipVH(char plane, string filename)
{


    switch(plane)
    {
        case 'V': doVFlip(filename); break;
        case 'H': doHFlip(filename); break;
        default: cerr << "Bad input, try again";
    }
}

void PicLibrary::grayscale(string filename)
{
    auto toBeModded = loadedPictures[filename];
    std::lock_guard<std::mutex> lock(toBeModded->_mtx);

    // existence of picture is guaranteed by the load function
    // but what if an unload function is called?
    // check existence of picture before call to this function


    for ( int i = 0 ; i < toBeModded->_pic.getwidth(); ++i )
    {
        for ( int j = 0 ; j < toBeModded->_pic.getheight(); ++j )
        {
            Colour temp = toBeModded->_pic.getpixel(i,j);
            toBeModded->_pic.setpixel(i,j,Colour((temp.getblue()+temp.getred()+temp.getgreen())/3,(temp.getblue()+temp.getred()+temp.getgreen())/3,(temp.getblue()+temp.getred()+temp.getgreen())/3));
        }
    }
}

void PicLibrary::invert(string filename)
{

    auto given = loadedPictures[filename];
    std::lock_guard<std::mutex> lock(given->_mtx);
    for (int i = 0; i < given->_pic.getwidth(); i++) {
        for (int j = 0; j < given->_pic.getheight(); j++) {
            Colour temp = given->_pic.getpixel(i, j);
            Colour newColor =Colour(255 - temp.getred(), 255 - temp.getgreen(), 255 - temp.getblue());
            given->_pic.setpixel(i, j, newColor);
        }
    }
    /*
    loadedPictures.erase(filename);
    loadedPictures.insert({filename,newPic});*/
}


Colour PicLibrary::avgColor(int i, int j, Picture *given)
{
    Colour upperLeft = given->getpixel(i-1,j-1);
    Colour upperCentre = given->getpixel(i,j-1);
    Colour upperRight = given->getpixel(i+1,j-1);
    Colour midLeft = given->getpixel(i-1,j);
    Colour midCentre = given->getpixel(i,j);
    Colour midRight = given->getpixel(i+1,j);
    Colour lowerLeft = given->getpixel(i-1,j+1);
    Colour lowerCentre = given->getpixel(i,j+1);
    Colour lowerRight = given->getpixel(i+1,j+1);



    int sumRed = 0, sumGreen = 0, sumBlue = 0 ;
    sumRed = upperLeft.getred() + upperCentre.getred() + upperRight.getred() + midCentre.getred() + midLeft.getred() + midRight.getred() + lowerCentre.getred() + lowerLeft.getred() + lowerRight.getred();
    sumGreen = upperCentre.getgreen() + upperLeft.getgreen() + upperRight.getgreen() + midCentre.getgreen() + midLeft.getgreen() + midRight.getgreen() + lowerCentre.getgreen() + lowerLeft.getgreen() + lowerRight.getgreen();
    sumBlue = upperCentre.getblue() + upperLeft.getblue() + upperRight.getblue() + midCentre.getblue() + midLeft.getblue() + midRight.getblue() + lowerCentre.getblue() + lowerLeft.getblue() + lowerRight.getblue();

    int avgRed = sumRed/9;
    int avgBlue = sumBlue/9;
    int avgGreen = sumGreen/9;

    Colour returnVariable = Colour(avgRed,avgGreen,avgBlue);
    return returnVariable;

}

void PicLibrary::blur(string filename)
{
    auto given = loadedPictures[filename];
    std::lock_guard<std::mutex> lock(given->_mtx);
    int x = given->_pic.getwidth();
    int y = given->_pic.getheight();

    Picture newPic(x, y);

    // parts of the picture which need to be copied as is

    for (int i = 0; i < x; ++i) {
        newPic.setpixel(i, 0, given->_pic.getpixel(i, 0));
    }

    for (int j = 0; j < y; ++j) {
        newPic.setpixel(0, j, given->_pic.getpixel(0, j));
    }

    for (int i = 0; i < x; ++i) {
        newPic.setpixel(i, y-1, given->_pic.getpixel(i, y-1));
    }

    for (int j = 0; j < y; ++j) {
        newPic.setpixel(x-1, j, given->_pic.getpixel(x-1, j));
    }

    // ok the real stuff starts here

    for (int i = 1; i < x - 1; ++i) {
        for (int j = 1; j < y - 1; ++j) {
            Colour avg =  PicLibrary::avgColor(i, j, &(given->_pic));
            newPic.setpixel(i, j, avg);
        }
    }

    given->_pic.setimage(newPic.getimage());
    /*loadedPictures.erase(filename);
    loadedPictures.insert({filename,&newPic});
*/

}


void PicLibrary::rotate90(string filename)
{
    auto toBeModded = loadedPictures[filename];
    std::lock_guard<std::mutex> lock(toBeModded->_mtx);
    int oWidth = toBeModded->_pic.getwidth();
    int oHeight = toBeModded->_pic.getheight();

    Picture newPic(oHeight,oWidth);

    for ( int x = 0 ; x < oWidth; ++x )
    {
        for ( int y = 0 ; y < oHeight ; ++y )
        {
            newPic.setpixel(y,x,toBeModded->_pic.getpixel(x,toBeModded->_pic.getheight()-y-1));
        }
    }

    toBeModded->_pic.setimage(newPic.getimage());

    // loadedPictures.erase(filename);
    //loadedPictures.insert({filename,&newPic});

}


void PicLibrary::rotate(int angle, string filename)
{

    switch(angle)
    {
        case 90:  rotate90(filename); break;
        case 180: rotate90(filename); rotate90(filename); break;
        case 270: rotate90(filename);rotate90(filename);rotate90(filename); break;
        default: cerr << "You've passed a bad number " << endl;
    }

}

void PicLibrary::display(string filename)
{
    if (loadedPictures.find(filename) != loadedPictures.end() )
    {
        auto toBeDisplayed = loadedPictures[filename];
        Utils dummy;
        dummy.displayimage(toBeDisplayed->_pic.getimage());
    }

}

void PicLibrary::savepicture(string filename, string path)
{

    if (loadedPictures.find(filename) != loadedPictures.end())
    {
        auto pic = loadedPictures[filename];
        Utils dummy;
        dummy.saveimage(pic->_pic.getimage(),path);
        cout << filename << " saved" << endl ;
    }
    else
    {
        cout << filename << " not saved" << endl;
    }
}

void PicLibrary::loadpicture(string path, string filename)
{
    string jpg = ".jpg";
    if ( !PicLibrary::ends_with(path,jpg) )
    {
        cerr << "Path name does not end with a .jpg, invalid path name" << endl;
    }
    if ( loadedPictures.find(filename) == loadedPictures.end() )
    {

        Picture* toBeAdded = new Picture(path);
        if (toBeAdded->getheight() != 0)
        {
            PicWrapper *pw = new PicWrapper(toBeAdded);
            //loadedPictures[filename] = toBeAdded;
            loadedPictures.insert({filename,pw});
            cout << filename << "added" << endl;
        }
        else
        {
            cout << path << "doesnt exist" <<endl;
        }
    }
    else
    {
        cerr << "Picture at path already exists in the picture library's intern"
                "al picture storage" << endl;
    }

}
bool PicLibrary::checkIfExists(string filename)
{
    return (loadedPictures.find(filename) != loadedPictures.end());

}

PicWrapper* PicLibrary::getWrap(string filename)
{
    return loadedPictures[filename];
}

void PicLibrary::joinAllThreads()
{
    for (auto i : loadedPictures )
    {
        std::for_each(i.second->_threads.begin(), i.second->_threads.end(), [](thread &t){t.join();});

    }
    for (auto i : loadedPictures )
    {
        i.second->_threads.clear();
    }


}
void PicLibrary::joinPicThreads(string filename)
{
    if (checkIfExists(filename))
    {
        std::for_each(loadedPictures[filename]->_threads.begin(), loadedPictures[filename]->_threads.end(), [](thread &t){t.join();});
        loadedPictures[filename]->_threads.clear();
    }
}