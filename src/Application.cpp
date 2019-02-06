#include "Application.h"

#include <iostream>

namespace Magnum {

Application::Application(const Arguments& arguments): Platform::Application{arguments}, _gfxPlane{}, fileWatcher{}, dw{&fileWatcher} //, vv{"../data/voronoi_2.txt"}
{
  ImageManager imageManager;

  PluginManagerImporter pluginManagerImporter;
  ImageLoader imageLoader{pluginManagerImporter};
  imageManager.setLoader(&imageLoader);

  fileWatcher.addWatch("../data/tmp", &dw);

  //vv.draw_points();
  //vv.draw_segments();
  //vv.draw_edges();
}

void Application::drawEvent() {
  GL::defaultFramebuffer.clear(GL::FramebufferClear::Color);
  //_gfxPlane.drawEvent();

  //vv.draw();

  swapBuffers();

  fileWatcher.update();
  redraw();
  //Debug{} << "Updating";
}

#ifndef CORRADE_TARGET_ANDROID
void Application::keyPressEvent(KeyEvent &event) {
  if(event.key() == KeyEvent::Key::Q)
    exit();
}
#endif

void Application::mousePressEvent(MouseEvent& event) {
  //vv.addPoint(event.position());
}
void Application::mouseReleaseEvent(MouseEvent& event) {
}
void Application::mouseMoveEvent(MouseMoveEvent& event) {
  redraw();
}
}
