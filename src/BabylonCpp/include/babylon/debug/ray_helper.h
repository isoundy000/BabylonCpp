#ifndef BABYLON_DEBUG_RAY_HELPER_H
#define BABYLON_DEBUG_RAY_HELPER_H

#include <babylon/babylon_global.h>
#include <babylon/culling/ray.h>
#include <babylon/math/vector3.h>

namespace BABYLON {

class BABYLON_SHARED_EXPORT RayHelper {

public:
  /** Statics **/
  static std::unique_ptr<RayHelper> CreateAndShow(const Ray& ray, Scene* scene,
                                                  const Color3& color);

public:
  RayHelper(const Ray& ray);
  ~RayHelper();

  void show(Scene* scene, const Color3& color);
  void hide();
  void attachToMesh(AbstractMesh* mesh,
                    const Vector3& meshSpaceDirection = Vector3::Zero(),
                    const Vector3& meshSpaceOrigin    = Vector3(0.f, 0.f, -1.f),
                    float length                      = 0.f);
  void detachFromMesh();

private:
  void _render();
  void _updateToMesh();
  void dispose();

public:
  Ray ray;

private:
  std::vector<Vector3> _renderPoints;
  LinesMesh* _renderLine;
  std::function<void()> _renderFunction;
  Scene* _scene;

  std::function<void()> _updateToMeshFunction;
  AbstractMesh* _attachedToMesh;
  Vector3 _meshSpaceDirection;
  Vector3 _meshSpaceOrigin;

}; // end of class RayHelper

} // end of namespace BABYLON

#endif // end of BABYLON_DEBUG_RAY_HELPER_H
