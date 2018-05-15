#ifndef GUIGFXPLANE_H
#define GUIGFXPLANE_H

#include <Magnum/GL/Buffer.h>
#include <Magnum/GL/Mesh.h>
#include <Magnum/Shaders/VertexColor.h>
#include <Magnum/Math/Vector3.h>
#include <Magnum/SceneGraph/Drawable.h>
#include <Magnum/SceneGraph/Object.h>
#include <Magnum/SceneGraph/Camera.h>
// TODO: Find better solution than namespace in header ( Want to use GL::Buffer etc with GL namespace intact)

using namespace Magnum;
typedef SceneGraph::Object<SceneGraph::MatrixTransformation2D> Object2D;

class GuiGfxPlane
//: public SceneGraph::Drawable2D
{
using Camera3D = SceneGraph::Camera3D; 
public:
      GuiGfxPlane();
  virtual ~GuiGfxPlane();
public:
  void drawEvent();
  //void draw(const Matrix4& transformationMatrix, Camera3D& camera) override; 
private:
      Vector3 _position;

private:
  GL::Buffer _buffer;
  GL::Mesh _mesh;
  Shaders::VertexColor2D _shader;
  };


#endif /* GUIGFXPLANE_H */
