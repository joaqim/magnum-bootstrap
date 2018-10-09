#include "Application.h"

#include <iostream>

namespace Magnum {
Application::Application(const Arguments& arguments): Platform::Application{arguments}, _gfxPlane{}, fileWatcher{}, dw{&fileWatcher}
{
  ImageManager imageManager;

  PluginManagerImporter pluginManagerImporter;
  ImageLoader imageLoader{pluginManagerImporter};
  imageManager.setLoader(&imageLoader);

  fileWatcher.addWatch("../data/tmp", &dw);
}

void Application::drawEvent() {
  GL::defaultFramebuffer.clear(GL::FramebufferClear::Color);
  _gfxPlane.drawEvent();
  swapBuffers();

  fileWatcher.update();
  //Debug{} << "Updating";
}

#ifndef CORRADE_TARGET_ANDROID
void Application::keyPressEvent(KeyEvent &event) {
  if(event.key() == KeyEvent::Key::Q)
    exit();
}
#endif

void Application::mousePressEvent(MouseEvent& event) {
}
void Application::mouseReleaseEvent(MouseEvent& event) {
}
void Application::mouseMoveEvent(MouseMoveEvent& event) {
  redraw();
}
}
