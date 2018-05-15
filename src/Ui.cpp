#include "Ui.h"

Ui::Ui() {

  /*
    _ui.emplace(Vector2{windowSize()}, windowSize(), Ui::mcssDarkStyleConfiguration(), "ƒ₀");
    Interconnect::connect(*_ui, &Ui::UserInterface::inputWidgetFocused, *this, &Application::startTextInput);
  Interconnect::connect(*_ui, &Ui::UserInterface::inputWidgetBlurred, *this, &Application::stopTextInput);

  // Base UI plane 
  _baseUiPlane.emplace(*_ui); 

  //Interconnect::connect(_baseUiPlane->metalness, &Ui::Input::valueChanged, *this, &Application::enableApplyButton);
  //Interconnect::connect(_baseUiPlane->roughness, &Ui::Input::valueChanged, *this, &Application::enableApplyButton);
  //Interconnect::connect(_baseUiPlane->f0, &Ui::Input::valueChanged, *this, &Application::enableApplyButton);
  //Interconnect::connect(_baseUiPlane->apply, &Ui::Button::tapped, *this, &Application::apply);
  //Interconnect::connect(_baseUiPlane->reset, &Ui::Button::tapped, *this, &Application::reset); 
  */
}

void Ui::drawEvent() {
  /*
  // Draw the UI 
  GL::Renderer::enable(GL::Renderer::Feature::Blending);
  GL::Renderer::setBlendFunction(GL::Renderer::BlendFunction::One, GL::Renderer::BlendFunction::OneMinusSourceAlpha);
  _ui->draw();
  GL::Renderer::setBlendFunction(GL::Renderer::BlendFunction::One, GL::Renderer::BlendFunction::One);
  GL::Renderer::disable(GL::Renderer::Feature::Blending);
  */
}

void Ui::mousePressEvent(MouseEvent& event) {
}
void Ui::mouseReleaseEvent(MouseEvent& event) {
}
void Ui::mouseMoveEvent(MouseMoveEvent& event) {
}
