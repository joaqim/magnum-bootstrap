#ifndef DIRECTORYWATCHER_H
#define DIRECTORYWATCHER_H

#include <Corrade/Interconnect/Receiver.h>
#include <Corrade/Interconnect/Emitter.h>

class DirectoryWatcher : public Corrade::Interconnect::Emitter {
public:
  DirectoryWatcher(const std::string path); 
  virtual ~DirectoryWatcher();
public:
  Signal onFileChanged(const std::string &filename);
private:
  std::string _path;
}; 

#endif /* DIRECTORYWATCHER_H */
