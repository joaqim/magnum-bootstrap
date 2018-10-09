
#include <Corrade/PluginManager/PluginManager.h>
#include <Magnum/Trade/AbstractImporter.h>
#include <Magnum/Trade/ImageData.h>
#include <Magnum/GL/TextureFormat.h>

#include <Magnum/ResourceManager.h>

#include <Corrade/Interconnect/Emitter.h>
#include <Corrade/Interconnect/Receiver.h>

namespace Magnum {

typedef PluginManager::Manager<Trade::AbstractImporter> PluginManagerImporter;

class ImageLoader : public AbstractResourceLoader<Trade::ImageData2D> {
 public:
    std::unique_ptr<Trade::AbstractImporter> _tgaImporter;

    ImageLoader(PluginManagerImporter &pluginManager);
    void doLoad(ResourceKey key) override;
};

class ImageManager : public ResourceManager<Trade::ImageData2D>, public Interconnect::Receiver {
public:
  ImageManager();
  void onFileChanged(const std::string &filename);
public:
  ImageLoader *imageLoader;
};

} // namespace magnum



