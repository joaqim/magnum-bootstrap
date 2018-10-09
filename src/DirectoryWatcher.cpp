#include "DirectoryWatcher.h"


DirectoryWatcher::DirectoryWatcher(FW::FileWatcher *fileWatcher) :  _fileWatcher(fileWatcher){};



bool DirectoryWatcher::handleFileAction(FW::WatchID watchid, const FW::String& dir, const FW::String& filename, FW::Action action) {
  Corrade::Utility::Debug{} << filename;
  return true;
}

