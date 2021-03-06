#ifndef BABYLON_DEBUG_BONE_AXES_VIEWER_H
#define BABYLON_DEBUG_BONE_AXES_VIEWER_H

#include <babylon/babylon_global.h>
#include <babylon/debug/axes_viewer.h>
#include <babylon/math/vector3.h>

namespace BABYLON {

class BABYLON_SHARED_EXPORT BoneAxesViewer : public AxesViewer {

public:
  BoneAxesViewer(Scene* scene, Bone* bone, Mesh* mesh, float scaleLines);
  ~BoneAxesViewer();

  void update();
  void dispose();

public:
  Mesh* mesh;
  Bone* bone;

  Vector3 pos;
  Vector3 xaxis;
  Vector3 yaxis;
  Vector3 zaxis;

}; // end of class BoneAxesViewer

} // end of namespace BABYLON

#endif // end of BABYLON_DEBUG_BONE_AXES_VIEWER_H
