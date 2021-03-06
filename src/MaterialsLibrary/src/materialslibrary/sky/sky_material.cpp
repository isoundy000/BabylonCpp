#include <babylon/materialslibrary/sky/sky_material.h>

#include <babylon/cameras/camera.h>
#include <babylon/core/json.h>
#include <babylon/engine/engine.h>
#include <babylon/engine/scene.h>
#include <babylon/materials/effect.h>
#include <babylon/materials/effect_creation_options.h>
#include <babylon/materials/effect_fallbacks.h>
#include <babylon/materials/material_helper.h>
#include <babylon/mesh/abstract_mesh.h>
#include <babylon/mesh/mesh.h>
#include <babylon/mesh/sub_mesh.h>
#include <babylon/mesh/vertex_buffer.h>

namespace BABYLON {
namespace MaterialsLibrary {

SkyMaterial::SkyMaterial(const std::string& iName, Scene* scene)
    : PushMaterial{iName, scene}
    , luminance{1.f}
    , turbidity{10.f}
    , rayleigh{2.f}
    , mieCoefficient{0.005f}
    , mieDirectionalG{0.8f}
    , distance{500.f}
    , inclination{0.49f}
    , azimuth{0.25f}
    , sunPosition{Vector3(0.f, 100.f, 0.f)}
    , useSunPosition{false}
    , _cameraPosition{Vector3::Zero()}
    , _renderId{-1}
{
}

SkyMaterial::~SkyMaterial()
{
}

bool SkyMaterial::needAlphaBlending()
{
  return (alpha < 1.f);
}

bool SkyMaterial::needAlphaTesting()
{
  return false;
}

BaseTexture* SkyMaterial::getAlphaTestTexture()
{
  return nullptr;
}

bool SkyMaterial::isReadyForSubMesh(AbstractMesh* mesh, SubMesh* subMesh,
                                    bool /*useInstances*/)
{
  if (isFrozen()) {
    if (_wasPreviouslyReady && subMesh->effect()) {
      return true;
    }
  }

  if (!subMesh->_materialDefines) {
    subMesh->_materialDefines = std::make_unique<SkyMaterialDefines>();
  }

  auto defines
    = *(static_cast<SkyMaterialDefines*>(subMesh->_materialDefines.get()));
  auto scene = getScene();

  if (!checkReadyOnEveryCall && subMesh->effect()) {
    if (_renderId == scene->getRenderId()) {
      return true;
    }
  }

  auto engine = scene->getEngine();

  MaterialHelper::PrepareDefinesForMisc(
    mesh, scene, false, pointsCloud(), fogEnabled(), defines,
    SMD::LOGARITHMICDEPTH, SMD::POINTSIZE, SMD::FOG);

  // Attribs
  MaterialHelper::PrepareDefinesForAttributes(
    mesh, defines, true, false, false, SMD::NORMAL, SMD::UV1, SMD::UV2,
    SMD::VERTEXCOLOR, SMD::VERTEXALPHA);

  // Get correct effect
  if (defines.isDirty()) {
    defines.markAsProcessed();

    scene->resetCachedMaterial();

    // Fallbacks
    auto fallbacks = std::make_unique<EffectFallbacks>();
    if (defines[SMD::FOG]) {
      fallbacks->addFallback(1, "FOG");
    }

    // Attributes
    std::vector<std::string> attribs{VertexBuffer::PositionKindChars};

    if (defines[SMD::VERTEXCOLOR]) {
      attribs.emplace_back(VertexBuffer::ColorKindChars);
    }

    const std::string shaderName{"sky"};
    auto join = defines.toString();
    const std::vector<std::string> uniforms{
      "world",       "viewProjection", "view",           "vFogInfos",
      "vFogColor",   "pointSize",      "vClipPlane",     "luminance",
      "turbidity",   "rayleigh",       "mieCoefficient", "mieDirectionalG",
      "sunPosition", "cameraPosition"};
    const std::vector<std::string> samplers{};
    const std::vector<std::string> uniformBuffers{};

    EffectCreationOptions options;
    options.attributes          = std::move(attribs);
    options.uniformsNames       = std::move(uniforms);
    options.uniformBuffersNames = std::move(uniformBuffers);
    options.samplers            = std::move(samplers);
    options.materialDefines     = &defines;
    options.defines             = std::move(join);
    options.fallbacks           = std::move(fallbacks);
    options.onCompiled          = onCompiled;
    options.onError             = onError;

    subMesh->setEffect(
      scene->getEngine()->createEffect(shaderName, options, engine), defines);
  }

  if (!subMesh->effect()->isReady()) {
    return false;
  }

  _renderId           = scene->getRenderId();
  _wasPreviouslyReady = true;

  return true;
}

void SkyMaterial::bindForSubMesh(Matrix* world, Mesh* mesh, SubMesh* subMesh)
{
  auto scene = getScene();

  auto defines
    = static_cast<SkyMaterialDefines*>(subMesh->_materialDefines.get());
  if (!defines) {
    return;
  }

  auto effect   = subMesh->effect();
  _activeEffect = effect;

  // Matrices
  bindOnlyWorldMatrix(*world);
  _activeEffect->setMatrix("viewProjection", scene->getTransformMatrix());

  if (_mustRebind(scene, effect)) {
    // Clip plane
    if (scene->clipPlane()) {
      auto clipPlane = scene->clipPlane();
      _activeEffect->setFloat4("vClipPlane", clipPlane->normal.x,
                               clipPlane->normal.y, clipPlane->normal.z,
                               clipPlane->d);
    }

    // Point size
    if (pointsCloud()) {
      _activeEffect->setFloat("pointSize", pointSize);
    }
  }

  // View
  if (scene->fogEnabled() && mesh->applyFog()
      && scene->fogMode() != Scene::FOGMODE_NONE) {
    _activeEffect->setMatrix("view", scene->getViewMatrix());
  }

  // Fog
  MaterialHelper::BindFogParameters(scene, mesh, _activeEffect);

  // Sky
  auto camera = scene->activeCamera;
  if (camera) {
    auto cameraWorldMatrix = *camera->getWorldMatrix();
    _cameraPosition.x      = cameraWorldMatrix.m[12];
    _cameraPosition.y      = cameraWorldMatrix.m[13];
    _cameraPosition.z      = cameraWorldMatrix.m[14];
    _activeEffect->setVector3("cameraPosition", _cameraPosition);
  }

  if (luminance > 0) {
    _activeEffect->setFloat("luminance", luminance);
  }

  _activeEffect->setFloat("turbidity", turbidity);
  _activeEffect->setFloat("rayleigh", rayleigh);
  _activeEffect->setFloat("mieCoefficient", mieCoefficient);
  _activeEffect->setFloat("mieDirectionalG", mieDirectionalG);

  if (!useSunPosition) {
    auto theta = Math::PI * (inclination - 0.5f);
    auto phi   = 2 * Math::PI * (azimuth - 0.5f);

    sunPosition.x = distance * std::cos(phi);
    sunPosition.y = distance * std::sin(phi) * std::sin(theta);
    sunPosition.z = distance * std::sin(phi) * std::cos(theta);
  }

  _activeEffect->setVector3("sunPosition", sunPosition);

  _afterBind(mesh, _activeEffect);
}

std::vector<IAnimatable*> SkyMaterial::getAnimatables()
{
  return std::vector<IAnimatable*>();
}

void SkyMaterial::dispose(bool forceDisposeEffect, bool forceDisposeTextures)
{
  Material::dispose(forceDisposeEffect, forceDisposeTextures);
}

Material* SkyMaterial::clone(const std::string& /*name*/,
                             bool /*cloneChildren*/) const
{
  return nullptr;
}

Json::object SkyMaterial::serialize() const
{
  return Json::object();
}

SkyMaterial* SkyMaterial::Parse(const Json::value& /*source*/, Scene* /*scene*/,
                                const std::string& /*rootUrl*/)
{
  return nullptr;
}

} // end of namespace MaterialsLibrary
} // end of namespace BABYLON
