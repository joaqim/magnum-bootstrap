#include <Magnum/GL/DefaultFramebuffer.h>
#ifndef CORRADE_TARGET_ANDROID
#include <Magnum/Platform/Sdl2Application.h>
#else
#include <Magnum/Platform/AndroidApplication.h>
#endif

#include <Magnum/GL/Renderer.h>
#include <Magnum/GL/Buffer.h>
#include <Magnum/GL/Mesh.h>
#include <Magnum/Shaders/VertexColor.h>

#ifndef CORRADE_TARGET_ANDROID
#include <imgui.h>
#include <imgui_impl_sdl_gl2.h>
#endif

/*
  #include <iomanip>
  #include <sstream>

  #include <Corrade/Containers/Optional.h>
  #include <Corrade/Interconnect/Receiver.h>

  #include <Magnum/Text/Alignment.h>

  #include <Magnum/Ui/Anchor.h>
  #include <Magnum/Ui/Button.h>
  #include <Magnum/Ui/Label.h>
  #include <Magnum/Ui/Plane.h>
  #include <Magnum/Ui/UserInterface.h>
  #include <Magnum/Ui/ValidatedInput.h>
*/

using namespace Magnum;
#include "MyApplication.h"

class MyApplication: public Platform::Application {
 public:
  explicit MyApplication(const Arguments& arguments);
  ~MyApplication();

 private:
  void drawEvent() override;

  void keyPressEvent(KeyEvent &event) override;
 private:

  GL::Buffer _buffer;
  GL::Mesh _mesh;
  Shaders::VertexColor2D _shader;

  /* Material properties */
  Float _metalness = 0.5f;
  Float _roughness = 0.25f;
  Float _f0 = 0.5f; /* Specular reflection coefficient */


  /* UI */
  //Containers::Optional<Ui::UserInterface> _ui;
  //Containers::Optional<BaseUiPlane> _baseUiPlane;
};

void MyApplication::keyPressEvent(KeyEvent &event) {
  if(event.key() == KeyEvent::Key::Q)
    exit();
}

MyApplication::MyApplication(const Arguments& arguments): Platform::Application{arguments} //,public Interconnect::Receiver
{
  /* TODO: Add your initialization code here */

  using namespace Math::Literals;

  struct TriangleVertex {
    Vector2 position;
    Color3 color;
  };
  const TriangleVertex data[]{
    {{-0.5f, -0.5f}, 0xff0000_rgbf},    // Left vertex, red color
    {{ 0.5f, -0.5f}, 0x00ff00_rgbf},    // Right vertex, green color
    {{ 0.0f,  0.5f}, 0x0000ff_rgbf}     // Top vertex, blue color
  };

  _buffer.setData(data, GL::BufferUsage::StaticDraw);
  _mesh.setPrimitive(GL::MeshPrimitive::Triangles)
      .setCount(3)
      .addVertexBuffer(_buffer, 0,
                       Shaders::VertexColor2D::Position{},
                       Shaders::VertexColor2D::Color{Shaders::VertexColor2D::Color::Components::Three});

#ifndef CORRADE_TARGET_ANDROID
  // Setup Dear ImGui binding
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;
  //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
  ImGui_ImplSdlGL2_Init(this->window());
  // Setup style
  ImGui::StyleColorsDark();
  //ImGui::StyleColorsClassic();
  redraw();
#endif

      /*
  // Create the UI 
  _ui.emplace(Vector2{windowSize()}, windowSize(), Ui::mcssDarkStyleConfiguration(), "ƒ₀");
  Interconnect::connect(*_ui, &Ui::UserInterface::inputWidgetFocused, *this, &MyApplication::startTextInput);
  Interconnect::connect(*_ui, &Ui::UserInterface::inputWidgetBlurred, *this, &MyApplication::stopTextInput);

  // Base UI plane 
  _baseUiPlane.emplace(*_ui); 

  //Interconnect::connect(_baseUiPlane->metalness, &Ui::Input::valueChanged, *this, &MyApplication::enableApplyButton);
  //Interconnect::connect(_baseUiPlane->roughness, &Ui::Input::valueChanged, *this, &MyApplication::enableApplyButton);
  //Interconnect::connect(_baseUiPlane->f0, &Ui::Input::valueChanged, *this, &MyApplication::enableApplyButton);
  //Interconnect::connect(_baseUiPlane->apply, &Ui::Button::tapped, *this, &MyApplication::apply);
  //Interconnect::connect(_baseUiPlane->reset, &Ui::Button::tapped, *this, &MyApplication::reset); 
  //*/
};

void MyApplication::drawEvent() {
  GL::defaultFramebuffer.clear(GL::FramebufferClear::Color);

  _mesh.draw(_shader);
#ifndef CORRADE_TARGET_ANDROID
  ImGui_ImplSdlGL2_NewFrame(this->window());

  // 1. Show a simple window.
  // Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets automatically appears in a window called "Debug".
  if(true){
    ImGui::Begin("");
    static float f = 0.0f;
    static int counter = 0;
    ImGui::Text("Hello, world!");                           // Display some text (you can use a format string too)
    ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f


    if (ImGui::Button("Button"))                            // Buttons return true when clicked (NB: most widgets return true when edited/activated)
      counter++;
    ImGui::SameLine();
    ImGui::Text("counter = %d", counter);

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();
  }
  ImGui::Render();
  ImGui_ImplSdlGL2_RenderDrawData(ImGui::GetDrawData());
#endif



  /*
  // Draw the UI 
  GL::Renderer::enable(GL::Renderer::Feature::Blending);
  GL::Renderer::setBlendFunction(GL::Renderer::BlendFunction::One, GL::Renderer::BlendFunction::OneMinusSourceAlpha);
  _ui->draw();
  GL::Renderer::setBlendFunction(GL::Renderer::BlendFunction::One, GL::Renderer::BlendFunction::One);
  GL::Renderer::disable(GL::Renderer::Feature::Blending);
*/
  swapBuffers();
}

MyApplication::~MyApplication() {
  ImGui_ImplSdlGL2_Shutdown();
}

MAGNUM_APPLICATION_MAIN(MyApplication)
