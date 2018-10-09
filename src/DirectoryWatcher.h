#ifndef DIRECTORYWATCHER_H
#define DIRECTORYWATCHER_H

#include <FileWatcher/FileWatcher.h>

#include <Corrade/Utility/Debug.h>

#include <sys/types.h> // needed by:
#include <unistd.h> // For impl::getuid()
#include <pwd.h> // for getpwuid()



class DirectoryWatcher : public FW::FileWatchListener {
public:
  DirectoryWatcher(FW::FileWatcher *fileWatcher);
  virtual ~DirectoryWatcher() = default;
public:
  bool handleFileAction(FW::WatchID watchid, const FW::String& dir, const FW::String& filename, FW::Action action);

private:
  FW::FileWatcher *_fileWatcher;
};

#endif /* DIRECTORYWATCHER_H */
