#include "ImageManager.h"


namespace Magnum {

ImageLoader::ImageLoader(PluginManagerImporter &pluginManager) {
  _tgaImporter = pluginManager.loadAndInstantiate("TgaImporter");
};


void ImageLoader::doLoad(ResourceKey key) {
  Debug{} << "ImageLoader::doLoad()";

  const Utility::Resource rs{"data-textures"};

  if(!_tgaImporter->openData(rs.getRaw("stone.tga"))) {
    setNotFound(key);
    Debug{} << "Image not found.";
    return;
  }
  Containers::Optional<Trade::ImageData2D> image = _tgaImporter->image2D(0);

  CORRADE_INTERNAL_ASSERT(image);
  //set(key, *image, ResourceDataState::Final, ResourcePolicy::Manual);
}

ImageManager::ImageManager() {
}

void ImageManager::onFileChanged(const std::string &filename) {
  Debug{} << filename << " changed!";
  //imageLoader->doLoad(filename);
}

} // namespace magnum


