#pragma once

//#ifndef CORRADE_TARGET_ANDROID
#ifndef ANDROID
#include <Magnum/Platform/Sdl2Application.h>
#else
#include <Magnum/Platform/AndroidApplication.h>
#endif

#include <Corrade/Containers/Optional.h>
#include <Corrade/Interconnect/Receiver.h>

using Corrade::Interconnect::Receiver; 

class Ui : public Receiver {
  using MouseMoveEvent = Magnum::Platform::Application::MouseMoveEvent;
  using MouseEvent = Magnum::Platform::Application::MouseEvent;
public:
  Ui();
public:
  void drawEvent();
public:
  void mousePressEvent(MouseEvent& event);
  void mouseReleaseEvent(MouseEvent& event);
  void mouseMoveEvent(MouseMoveEvent& event);

#ifndef CORRADE_TARGET_ANDROID
  using KeyEvent = Magnum::Platform::Application::KeyEvent;
  void keyPressEvent(KeyEvent& event);
  void keyReleaseEvent(KeyEvent& event);
#endif

};

