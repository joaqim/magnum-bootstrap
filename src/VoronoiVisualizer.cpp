#include <iostream>
#include <vector>
#include <fstream>


#include <Magnum/GL/Buffer.h>
#include <Magnum/GL/Mesh.h>
#include <Magnum/Shaders/VertexColor.h>
#include <Magnum/Math/Vector3.h>

#include <Magnum/SceneGraph/MatrixTransformation3D.h>
#include <Magnum/SceneGraph/Scene.h>
#include <Magnum/SceneGraph/SceneGraph.h>
#include <Magnum/SceneGraph/Drawable.h>
#include <Magnum/SceneGraph/Camera.h>

#include <Magnum/Trade/MeshData3D.h>
#include <Magnum/Trade/MeshObjectData3D.h>
#include <Magnum/Shaders/Phong.h>

#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/GL/Renderer.h>

#include <boost/polygon/polygon.hpp>
#include <boost/polygon/voronoi.hpp>

#include "voronoi_visual_utils.hpp"

namespace boost { namespace polygon {
using namespace Magnum;

typedef SceneGraph::Object<SceneGraph::MatrixTransformation3D> Object3D;
typedef SceneGraph::Scene<SceneGraph::MatrixTransformation3D> Scene3D;

using namespace Math::Literals;

class ColoredDrawable: public SceneGraph::Drawable3D {
 public:
  explicit ColoredDrawable(Object3D& object, Shaders::Phong& shader, GL::Mesh& mesh, const Color4& color, SceneGraph::DrawableGroup3D& group): SceneGraph::Drawable3D{object, &group}, _shader(shader), _mesh(mesh), _color{color} {}

 private:
  //void draw(const Matrix4& transformationMatrix, SceneGraph::Camera3D& camera) override;
  void draw(const Matrix4& transformationMatrix, SceneGraph::Camera3D& camera) {
    _shader
        .setDiffuseColor(_color)
        .setLightPosition(camera.cameraMatrix().transformPoint({-3.0f, 10.0f, 10.0f}))
        .setTransformationMatrix(transformationMatrix)
        .setNormalMatrix(transformationMatrix.rotationScaling())
        .setProjectionMatrix(camera.projectionMatrix());

    _mesh.draw(_shader);
  }
  Shaders::Phong& _shader;
  GL::Mesh& _mesh;
  Color4 _color;
};

class VoronoiVisualizer {
 public:
  typedef double coordinate_type;

  typedef point_data<coordinate_type> point_type;
  typedef segment_data<coordinate_type> segment_type;
  typedef rectangle_data<coordinate_type> rect_type;
  typedef voronoi_builder<int> VB;
  typedef voronoi_diagram<coordinate_type> VD;
  typedef VD::cell_type cell_type;
  typedef VD::cell_type::source_index_type source_index_type;
  typedef VD::cell_type::source_category_type source_category_type;
  typedef VD::edge_type edge_type;
  typedef VD::cell_container_type cell_container_type;
  typedef VD::cell_container_type vertex_container_type;
  typedef VD::edge_container_type edge_container_type;
  typedef VD::const_cell_iterator const_cell_iterator;
  typedef VD::const_vertex_iterator const_vertex_iterator;
  typedef VD::const_edge_iterator const_edge_iterator;

  struct Vertex {
    Vector2 position;
    Color3 color;
  };

  Vector3 _position;

  GL::Buffer _buffer;
  GL::Mesh _mesh;
  Shaders::VertexColor2D _shader;
  Scene3D _scene;
  Object3D _manipulator, _cameraObject;
  SceneGraph::Camera3D* _camera;
  SceneGraph::DrawableGroup3D _drawables;
  Vector3 _previousPosition;
  Shaders::Phong _coloredShader;

  static const std::size_t EXTERNAL_COLOR = 1;
  void update_brect(const point_type& point) {
    if (brect_initialized_) {
      encompass(brect_, point);
    } else {
      set_points(brect_, point, point);
      brect_initialized_ = true;
    }
  }
  void construct_brect() {
    double side = (std::max)(xh(brect_) - xl(brect_), yh(brect_) - yl(brect_));
    std::cout << "Side: "  << side << std::endl;
    center(shift_, brect_);
    set_points(brect_, shift_, shift_);
    bloat(brect_, side * 1.2);
  }

  void InitScene() {
    /* Every scene needs a camera */
    _cameraObject
        .setParent(&_scene)
        .translate(Vector3::zAxis(5.0f));
    (*(_camera = new SceneGraph::Camera3D{_cameraObject}))
        .setAspectRatioPolicy(SceneGraph::AspectRatioPolicy::Extend)
        .setProjectionMatrix(Matrix4::perspectiveProjection(35.0_degf, 1.0f, 0.01f, 1000.0f))
        .setViewport(GL::defaultFramebuffer.viewport().size());

    /* Base object, parent of all (for easy manipulation) */
    _manipulator.setParent(&_scene);

    /* Setup renderer and shader defaults */
    GL::Renderer::enable(GL::Renderer::Feature::DepthTest);
    GL::Renderer::enable(GL::Renderer::Feature::FaceCulling);
    _coloredShader
        .setAmbientColor(0x111111_rgbf)
        .setSpecularColor(0xffffff_rgbf)
        .setShininess(80.0f);
  }

  void clear() {
    brect_initialized_ = false;
    point_data_.clear();
    segment_data_.clear();
    vd_.clear();
  }

  void read_data(std::string const &file_path) {
    std::ifstream in_stream (file_path);

#if 0
    int i = 0;
    while(in_stream >> i) {
      std::cout << i << std::endl;
    }
    return;
#endif

    std::size_t num_points, num_segments;
    int x1, y1, x2, y2;
    in_stream >> num_points;
    for (std::size_t i = 0; i < num_points; ++i) {
      in_stream >> x1 >> y1;
      point_type p(x1, y1);
      update_brect(p);
      point_data_.push_back(p);
    }
    in_stream >> num_segments;
    for (std::size_t i = 0; i < num_segments; ++i) {
      in_stream >> x1 >> y1 >> x2 >> y2;
      point_type lp(x1, y1);
      point_type hp(x2, y2);
      update_brect(lp);
      update_brect(hp);
      segment_data_.push_back(segment_type(lp, hp));
    }

    in_stream.close();
  }

  void color_exterior(const VD::edge_type* edge) {
    if (edge->color() == EXTERNAL_COLOR) {
      return;
    }
    edge->color(EXTERNAL_COLOR);
    edge->twin()->color(EXTERNAL_COLOR);
    const VD::vertex_type* v = edge->vertex1();
    if (v == NULL || !edge->is_primary()) {
      return;
    }
    v->color(EXTERNAL_COLOR);
    const VD::edge_type* e = v->incident_edge();
    do {
      color_exterior(e);
      e = e->rot_next();
    } while (e != v->incident_edge());
  }
  void clip_infinite_edge(
      const edge_type& edge, std::vector<point_type>* clipped_edge) {
    //const cell_type& cell1 = *edge.cell();
    const auto& cell1 = *edge.cell();
    //const cell_type cell1 = *edge.cell();
    //const cell_type& cell2 = *edge.twin()->cell();
    const auto& cell2 = *edge.twin()->cell();

    point_type origin, direction;
    // Infinite edges could not be created by two segment sites.
    if (cell1.contains_point() && cell2.contains_point()) {

      point_type p1 = retrieve_point(cell1);
      point_type p2 = retrieve_point(cell2);

      origin.x((p1.x() + p2.x()) * 0.5);
      origin.y((p1.y() + p2.y()) * 0.5);
      direction.x(p1.y() - p2.y());
      direction.y(p2.x() - p1.x());
    } else {
      origin = cell1.contains_segment() ?
          retrieve_point(cell2) :
          retrieve_point(cell1);
      segment_type segment = cell1.contains_segment() ?
          retrieve_segment(cell1) :
          retrieve_segment(cell2);
      coordinate_type dx = high(segment).x() - low(segment).x();
      coordinate_type dy = high(segment).y() - low(segment).y();
      if ((low(segment) == origin) ^ cell1.contains_point()) {
        direction.x(dy);
        direction.y(-dx);
      } else {
        direction.x(-dy);
        direction.y(dx);
      }
    }
    coordinate_type side = xh(brect_) - xl(brect_);
    coordinate_type koef =
        side / (std::max)(fabs(direction.x()), fabs(direction.y()));
    if (edge.vertex0() == NULL) {
      clipped_edge->push_back(point_type(
          origin.x() - direction.x() * koef,
          origin.y() - direction.y() * koef));
    } else {
      clipped_edge->push_back(
          point_type(edge.vertex0()->x(), edge.vertex0()->y()));
    }
    if (edge.vertex1() == NULL) {
      clipped_edge->push_back(point_type(
          origin.x() + direction.x() * koef,
          origin.y() + direction.y() * koef));
    } else {
      clipped_edge->push_back(
          point_type(edge.vertex1()->x(), edge.vertex1()->y()));
    }
  }
#if 1
  void sample_curved_edge(
      const edge_type& edge,
      std::vector<point_type>* sampled_edge) {
    coordinate_type max_dist = 1E-3 * (xh(brect_) - xl(brect_));
    point_type point = edge.cell()->contains_point() ?
        retrieve_point(*edge.cell()) :
        retrieve_point(*edge.twin()->cell());
    segment_type segment = edge.cell()->contains_point() ?
        retrieve_segment(*edge.twin()->cell()) :
        retrieve_segment(*edge.cell());
    voronoi_visual_utils<coordinate_type>::discretize(
        point, segment, max_dist, sampled_edge);
  }

  point_type retrieve_point(const cell_type& cell) {
    source_index_type index = cell.source_index();
    source_category_type category = cell.source_category();
    if (category == SOURCE_CATEGORY_SINGLE_POINT) {
      return point_data_[index];
    }
    index -= point_data_.size();
    if (category == SOURCE_CATEGORY_SEGMENT_START_POINT) {
      return low(segment_data_[index]);
    } else {
      return high(segment_data_[index]);
    }
  }

  segment_type retrieve_segment(const cell_type& cell) {
    source_index_type index = cell.source_index() - point_data_.size();
    return segment_data_[index];
  }
#endif

  void draw_points() {
    Vertex data[256];

    for (std::size_t i = 0; i < point_data_.size(); ++i) {
      point_type point = point_data_[i];
      deconvolve(point, shift_);
      data[i] = Vertex{{float(point.x()), float(point.y())}};
    }
    assert(point_data_.size() > 0);

    _buffer.setData(data, GL::BufferUsage::StaticDraw);
    _mesh.setPrimitive(GL::MeshPrimitive::TriangleStrip)
        .setCount(point_data_.size()*8)
        .addVertexBuffer(_buffer, 0,
                         Shaders::VertexColor2D::Position{},
                         Shaders::VertexColor2D::Color{Shaders::VertexColor2D::Color::Components::Three});

  };

  void draw_edges() {
    // Draw voronoi edges.
    Vertex data[256];

    for (const_edge_iterator it = vd_.edges().begin();
         it != vd_.edges().end(); ++it) {
      if (primary_edges_only_ && !it->is_primary()) {
        continue;
      }
      if (internal_edges_only_ && (it->color() == EXTERNAL_COLOR)) {
        continue;
      }
      std::vector<point_type> samples;
      if (!it->is_finite()) {
        clip_infinite_edge(*it, &samples);
      } else {
        point_type vertex0(it->vertex0()->x(), it->vertex0()->y());
        samples.push_back(vertex0);
        point_type vertex1(it->vertex1()->x(), it->vertex1()->y());
        samples.push_back(vertex1);
        if (it->is_curved()) {
          sample_curved_edge(*it, &samples);
        }
      }
      for (std::size_t i = 0; i < samples.size(); ++i) {
        point_type vertex = deconvolve(samples[i], shift_);
        // glVertex2f(vertex.x(), vertex.y());
        data[i] = Vertex{Vector2{static_cast<float>(vertex.x()/10), static_cast<float>(vertex.y()/10)}, Color3{static_cast<float>(it->color())}};
      }
    }

    _buffer.setData(data, GL::BufferUsage::StaticDraw);
    _mesh.setPrimitive(GL::MeshPrimitive::LineStrip)
        .setCount(256)
        .addVertexBuffer(_buffer, 0,
                         Shaders::VertexColor2D::Position{},
                         Shaders::VertexColor2D::Color{Shaders::VertexColor2D::Color::Components::Three});


  };

  void addPoint(Vector2i const &pos) {
    data_.emplace_back(Vertex{{Vector2(pos.x()/60.f, pos.y()/60.f)}, {0,255,0}});
    _buffer.setData(data_, GL::BufferUsage::DynamicDraw);

    std::cout << pos.x()/600.f << std::endl;
    //std::cout << pos.y()/600.f << std::endl;
    //float x = -10.0 - (600 / (pos.x() + 1) * 20);
    //std::cout << x << std::endl;
    //_mesh.release();

    _mesh.setPrimitive(GL::MeshPrimitive::Lines)
        .setCount(data_.size())
        .addVertexBuffer(_buffer, 0,
                         Shaders::VertexColor2D::Position{},
                         Shaders::VertexColor2D::Color{Shaders::VertexColor2D::Color::Components::Three});

  }

  void update_view_port() {
    rect_type view_rect = brect_;
    deconvolve(view_rect, shift_);
  };


  void draw() {
    _mesh.draw(_shader);
    //if(!_drawables.isEmpty())
    //_camera->draw(_drawables);
  };

  std::vector<Vertex> data_;
  void draw_segments() {
    //Vertex data[256];
    //std::vector<Vertex> data;

    data_.resize(segment_data_.size()*2);

    for (std::size_t i = 0; i < segment_data_.size(); i+=1) {
      point_type lp = low(segment_data_[i]);
      lp = deconvolve(lp, shift_);
      data_[i] = Vertex{Vector2{static_cast<float>(lp.x()/10), static_cast<float>(lp.y()/10)}};
      point_type hp = high(segment_data_[i]);
      hp = deconvolve(hp, shift_);

      data_[i+1] = Vertex{Vector2{static_cast<float>(hp.x()/10), static_cast<float>(hp.y()/10)}};
      auto &c = data_[i];
      std::cout << c.position.x() << ", " << c.position.y() << std::endl;
      c.color = Color3{255,0,0};
      //assert(point_data_.size() > 0);
    }

    _buffer.setData(data_, GL::BufferUsage::DynamicDraw);
    _mesh.setPrimitive(GL::MeshPrimitive::Lines)
        .setCount(data_.size())
        .addVertexBuffer(_buffer, 0,
                         Shaders::VertexColor2D::Position{},
                         Shaders::VertexColor2D::Color{Shaders::VertexColor2D::Color::Components::Three});

    //new ColoredDrawable{_manipulator, _coloredShader, _mesh, 0xffffff_rgbf, _drawables};
  };

  void draw_vertices() {};

  VoronoiVisualizer(std::string const &file_path) :
      primary_edges_only_(false),
      internal_edges_only_(false)
  {
    // Clear all containers.
    clear();

    // Read data.
    read_data(file_path);

    // No data, don't proceed.
    if (!brect_initialized_) {
      std::cout << "Failed to read data in file: " << file_path << std::endl;
      return;
    }

    // Construct bounding rectangle.
    construct_brect();

    // Construct voronoi diagram.
    construct_voronoi(
        point_data_.begin(), point_data_.end(),
        segment_data_.begin(), segment_data_.end(),
        &vd_);

    //assert(point_data_.size() > 0);
    //assert(segment_data_.size() > 0);
    // Color exterior edges.
    for (const_edge_iterator it = vd_.edges().begin();
         it != vd_.edges().end(); ++it) {
      if (!it->is_finite()) {
        color_exterior(&(*it));
      }
    }
  }

  ~VoronoiVisualizer() {
    _mesh.release();
    _buffer.release();
  }


  point_type shift_;
  std::vector<point_type> point_data_;
  std::vector<segment_type> segment_data_;
  rect_type brect_;
  VB vb_;
  VD vd_;
  bool brect_initialized_;
  bool primary_edges_only_;
  bool internal_edges_only_;
};
} }
