#include "DirectoryWatcher.h"

using Signal = DirectoryWatcher::Signal; 

DirectoryWatcher::DirectoryWatcher(const std::string path) :  _path{path} {
}

DirectoryWatcher::~DirectoryWatcher() {
}

Signal DirectoryWatcher::onFileChanged(const std::string &filename) {
  return emit(&DirectoryWatcher::onFileChanged, filename ); 
}
