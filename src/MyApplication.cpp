#include <Magnum/GL/DefaultFramebuffer.h>
#ifndef CORRADE_TARGET_ANDROID
#include <Magnum/Platform/Sdl2Application.h>
#else
#include <Magnum/Platform/AndroidApplication.h>
#endif

#include <Magnum/GL/Buffer.h>
#include <Magnum/GL/Mesh.h>
#include <Magnum/Shaders/VertexColor.h>

#ifndef CORRADE_TARGET_ANDROID
#include <imgui.h>
#include <imgui_impl_sdl_gl2.h>
#endif

using namespace Magnum;

class MyApplication: public Platform::Application {
    public:
        explicit MyApplication(const Arguments& arguments);

    private:
        void drawEvent() override;
    private:

    GL::Buffer _buffer;
    GL::Mesh _mesh;
    Shaders::VertexColor2D _shader;
};

MyApplication::MyApplication(const Arguments& arguments): Platform::Application{arguments} {
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
#endif

};

void MyApplication::drawEvent() {
    GL::defaultFramebuffer.clear(GL::FramebufferClear::Color);

    _mesh.draw(_shader);
#ifndef CORRADE_TARGET_ANDROID
    ImGui_ImplSdlGL2_NewFrame(this->window());

    // 1. Show a simple window.
    // Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets automatically appears in a window called "Debug".
    {
        static float f = 0.0f;
        static int counter = 0;
        ImGui::Text("Hello, world!");                           // Display some text (you can use a format string too)
        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f


        if (ImGui::Button("Button"))                            // Buttons return true when clicked (NB: most widgets return true when edited/activated)
            counter++;
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }
#endif

    swapBuffers();
}

MAGNUM_APPLICATION_MAIN(MyApplication)
