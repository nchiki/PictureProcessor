#ifndef PICLIB_H
#define PICLIB_H

#include <unordered_map>
#include "Picture.hpp"
#include "Utils.hpp"


class PicLibrary {

  private:
    unordered_map<std::string, PicWrapper*> loadedPictures;

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
  inline bool isJPG(string const &filename, string const &ending);
  bool checkMapforFile(string filename);
  PicWrapper* getWrapper(string filename);
  void exit();
  void rotate90(string filename);
  void rotate180(string filename);
  void rotate270(string filename);
  void flipV(string filename);
  void flipH(string filename);
  void joinPicThreads(string filename);
  void joinAllThreads();


      // picture transformation routines
  void invert(string filename);
  void grayscale(string filename);
  void rotate(int angle, string filename);
  void flipVH(char plane, string filename);
  void blur(string filename);
};

#endif

