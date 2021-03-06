#ifndef BABYLON_MATERIALS_LIBRARY_GRID_GRID_MATERIAL_H
#define BABYLON_MATERIALS_LIBRARY_GRID_GRID_MATERIAL_H

#include <babylon/babylon_global.h>
#include <babylon/materials/push_material.h>
#include <babylon/materialslibrary/grid/grid_material_defines.h>
#include <babylon/math/color3.h>
#include <babylon/math/vector4.h>

namespace BABYLON {
namespace MaterialsLibrary {

/**
 * @brief The grid materials allows you to wrap any shape with a grid.
 * Colors are customizable.
 */
class BABYLON_SHARED_EXPORT GridMaterial : public PushMaterial {

public:
  using GMD = GridMaterialDefines;

public:
  /**
   * @brief constructor
   * @param name The name given to the material in order to identify it
   * afterwards.
   * @param scene The scene the material is used in.
   */
  GridMaterial(const std::string& name, Scene* scene);
  ~GridMaterial();

  /**
   * Returns whether or not the grid requires alpha blending.
   */
  bool needAlphaBlending() override;
  bool isReadyForSubMesh(AbstractMesh* mesh, SubMesh* subMesh,
                         bool useInstances = false) override;
  void bindForSubMesh(Matrix* world, Mesh* mesh, SubMesh* subMesh) override;
  virtual void dispose(bool forceDisposeEffect   = false,
                       bool forceDisposeTextures = false) override;
  Material* clone(const std::string& name,
                  bool cloneChildren = false) const override;
  Json::object serialize() const;

  /** Statics **/
  static GridMaterial* Parse(const Json::value& source, Scene* scene,
                             const std::string& rootUrl);

public:
  /**
   * Main color of the grid (e.g. between lines)
   */
  Color3 mainColor;

  /**
   * Color of the grid lines.
   */
  Color3 lineColor;

  /**
   * The scale of the grid compared to unit.
   */
  float gridRatio;

  /**
   * The frequency of thicker lines.
   */
  float majorUnitFrequency;

  /**
   * The visibility of minor units in the grid.
   */
  float minorUnitVisibility;

  /**
   * The grid opacity outside of the lines.
   */
  float opacity;

private:
  Vector4 _gridControl;
  int _renderId;

}; // end of class GridMaterial

} // end of namespace MaterialsLibrary
} // end of namespace BABYLON

#endif // end of BABYLON_MATERIALS_LIBRARY_GRID_GRID_MATERIAL_H
