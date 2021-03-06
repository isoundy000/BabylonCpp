#ifndef BABYLON_RENDERING_BOUNDING_BOX_RENDERER_H
#define BABYLON_RENDERING_BOUNDING_BOX_RENDERER_H

#include <babylon/babylon_global.h>
#include <babylon/interfaces/idisposable.h>
#include <babylon/math/color3.h>

namespace BABYLON {

class BABYLON_SHARED_EXPORT BoundingBoxRenderer : public IDisposable {

public:
  BoundingBoxRenderer(Scene* scene);
  virtual ~BoundingBoxRenderer();

  void reset();
  void render();
  void dispose(bool doNotRecurse = false) override;

private:
  void _prepareResources();

public:
  Color3 frontColor;
  Color3 backColor;
  bool showBackLines;
  std::vector<BoundingBox> renderList;

private:
  Scene* _scene;
  ShaderMaterial* _colorShader;
  std::vector<std::unique_ptr<VertexBuffer>> _vertexBuffers;
  std::unordered_map<std::string, VertexBuffer*> _vertexBuffersMap;
  std::unique_ptr<GL::IGLBuffer> _indexBuffer;

}; // end of class BoundingBoxRenderer

} // end of namespace BABYLON

#endif // end of BABYLON_RENDERING_BOUNDING_BOX_RENDERER_H
