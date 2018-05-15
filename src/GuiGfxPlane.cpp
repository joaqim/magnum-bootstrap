#include "GuiGfxPlane.h"

  GuiGfxPlane::GuiGfxPlane() {
    using namespace Math::Literals;

    struct Vertex {
      Vector2 position;
      Color3 color;
    };
    const Vertex data[] {
      {{-0.5f, -0.5f}, 0xff0000_rgbf},    // Bottom-Left vertex, red color
      {{ 0.5f, -0.5f}, 0x00ff00_rgbf},    // Bottom-Right vertex, green color
      {{ 0.5f,  0.5f}, 0x0000ff_rgbf},    // Top-Right vertex, blue color
      {{ -0.5f,  0.5f}, 0xffffff_rgbf}    // Top-Left vertex, white color
    };

    _buffer.setData(data, GL::BufferUsage::StaticDraw);
    _mesh.setPrimitive(GL::MeshPrimitive::TriangleFan)
.setCount(4)
.addVertexBuffer(_buffer, 0,
                 Shaders::VertexColor2D::Position{},
                 Shaders::VertexColor2D::Color{Shaders::VertexColor2D::Color::Components::Three});
}

//void GuiGfxPlane::draw(const Matrix4& transformationMatrix, Camera3D& camera) {}

GuiGfxPlane::~GuiGfxPlane() {
  _mesh.release();
}


void GuiGfxPlane::drawEvent() {
  _mesh.draw(_shader);
}
