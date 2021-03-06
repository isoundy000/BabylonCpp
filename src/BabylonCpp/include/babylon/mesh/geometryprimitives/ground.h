#ifndef BABYLON_MESH_PRIMITIVES_GROUND_H
#define BABYLON_MESH_PRIMITIVES_GROUND_H

#include <babylon/babylon_global.h>

#include <babylon/mesh/geometryprimitives/_primitive.h>
#include <babylon/mesh/mesh.h>

namespace BABYLON {
namespace GeometryPrimitives {

/**
 * @brief
 */
class BABYLON_SHARED_EXPORT Ground : public _Primitive {

public:
  template <typename... Ts>
  static Ground* New(Ts&&... args)
  {
    auto ground = new Ground(std::forward<Ts>(args)...);
    ground->addToScene(static_cast<std::unique_ptr<Geometry>>(ground));

    return ground;
  }
  ~Ground();

  std::unique_ptr<VertexData> _regenerateVertexData() override;
  Geometry* copy(const std::string& id) override;

  // Statics
  static Ground* Parse(const Json::value& parsedGround, Scene* scene);

protected:
  Ground(const std::string& id, Scene* scene, unsigned int width,
         unsigned int height, unsigned int subdivisions, bool canBeRegenerated,
         Mesh* mesh);

public:
  unsigned int width;
  unsigned int height;
  unsigned int subdivisions;

}; // end of class Ground

} // end of namespace GeometryPrimitives
} // end of namespace BABYLON

#endif // end of BABYLON_MESH_PRIMITIVES_GROUND_H
