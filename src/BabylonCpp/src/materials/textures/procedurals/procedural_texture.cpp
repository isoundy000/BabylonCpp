#include <babylon/materials/textures/procedurals/procedural_texture.h>

#include <babylon/babylon_stl_util.h>
#include <babylon/engine/engine.h>
#include <babylon/engine/scene.h>
#include <babylon/materials/effect.h>
#include <babylon/materials/effect_creation_options.h>
#include <babylon/materials/effect_fallbacks.h>
#include <babylon/materials/textures/irender_target_options.h>
#include <babylon/mesh/vertex_buffer.h>

namespace BABYLON {

ProceduralTexture::ProceduralTexture(
  const std::string& _name, const Size& size,
  const std::unordered_map<std::string, std::string>& fragment, Scene* scene,
  Texture* fallbackTexture, bool generateMipMaps)
    : Texture("", scene, !generateMipMaps)
    , _generateMipMaps{generateMipMaps}
    , isEnabled{true}
    , _size{size}
    , _currentRefreshId{-1}
    , _refreshRate{1}
    , _fallbackTexture{fallbackTexture}
    , _fallbackTextureUsed{false}
{
  // scene->_proceduralTextures.emplace_back(
  //  std::make_unique<ProceduralTexture>(this));

  name           = _name;
  isRenderTarget = true;

  setFragment(fragment);

  auto engine = scene->getEngine();

  if (isCube) {
    IRenderTargetOptions options;
    options.generateMipMaps = generateMipMaps;
    _texture = engine->createRenderTargetCubeTexture(size, options);
    setFloat("face", 0);
  }
  else {
    IRenderTargetOptions options;
    options.generateMipMaps = generateMipMaps;
    _texture                = engine->createRenderTargetTexture(size, options);
  }

  // VBO
  Float32Array vertices{1.f,  1.f,  //
                        -1.f, 1.f,  //
                        -1.f, -1.f, //
                        1.f,  -1.f};

  _vertexBuffers[VertexBuffer::PositionKindChars]
    = std::make_unique<VertexBuffer>(
      engine, vertices, VertexBuffer::PositionKind, false, false, 2);

  // Indices
  Uint32Array indices{0, 1, 2, 0, 2, 3};

  _indexBuffer = engine->createIndexBuffer(indices);
}

ProceduralTexture::ProceduralTexture(const std::string& _name, const Size& size,
                                     const std::string& fragment, Scene* scene,
                                     Texture* fallbackTexture,
                                     bool generateMipMaps)
    : Texture("", scene, !generateMipMaps)
    , _generateMipMaps{generateMipMaps}
    , isEnabled{true}
    , _size{size}
    , _currentRefreshId{-1}
    , _refreshRate{1}
    , _fallbackTexture{fallbackTexture}
    , _fallbackTextureUsed{false}
{
  scene->_proceduralTextures.emplace_back(this);

  name           = _name;
  isRenderTarget = true;

  setFragment(fragment);

  auto engine = scene->getEngine();

  if (isCube) {
    IRenderTargetOptions options;
    options.generateMipMaps = generateMipMaps;
    _texture = engine->createRenderTargetCubeTexture(size, options);
    setFloat("face", 0);
  }
  else {
    IRenderTargetOptions options;
    options.generateMipMaps = generateMipMaps;
    _texture                = engine->createRenderTargetTexture(size, options);
  }

  // VBO
  Float32Array vertices{1.f,  1.f,  //
                        -1.f, 1.f,  //
                        -1.f, -1.f, //
                        1.f,  -1.f};

  _vertexBuffers[VertexBuffer::PositionKindChars]
    = std::make_unique<VertexBuffer>(
      engine, vertices, VertexBuffer::PositionKind, false, false, 2);

  // Indices
  Uint32Array indices{0, 1, 2, 0, 2, 3};

  _indexBuffer = engine->createIndexBuffer(indices);
}

ProceduralTexture::~ProceduralTexture()
{
}

void ProceduralTexture::reset()
{
  if (_effect == nullptr) {
    return;
  }
  auto engine = getScene()->getEngine();
  engine->_releaseEffect(_effect);
}

bool ProceduralTexture::isReady()
{
  auto engine = getScene()->getEngine();
  std::unordered_map<std::string, std::string> shaders;

  if (_fragment.empty()) {
    return false;
  }

  if (_fallbackTextureUsed) {
    return true;
  }

  if (stl_util::contains(_fragment, "fragmentElement")) {
    shaders["vertex"]          = "procedural";
    shaders["fragmentElement"] = _fragment["fragmentElement"];
  }
  else {
    return false;
  }

  EffectCreationOptions options;
  options.attributes    = {VertexBuffer::PositionKindChars};
  options.uniformsNames = _uniforms;
  options.samplers      = _samplers;
  options.onError
    = [this](const Effect* /*effect*/, const std::string& /*errors*/) {
        releaseInternalTexture();

        if (_fallbackTexture) {
          _texture = _fallbackTexture->_texture;
          ++_texture->references;
        }

        _fallbackTextureUsed = true;
      };

  _effect = engine->createEffect(shaders, options, getScene()->getEngine());

  return _effect->isReady();
}

void ProceduralTexture::resetRefreshCounter()
{
  _currentRefreshId = -1;
}

void ProceduralTexture::setFragment(
  const std::unordered_map<std::string, std::string>& fragment)
{
  _fragment = fragment;
}

void ProceduralTexture::setFragment(const std::string& fragment)
{
  _fragment["fragmentElement"] = fragment;
}

// Use 0 to render just once, 1 to render on every frame, 2 to render every two
// frames and so on...
int ProceduralTexture::refreshRate() const
{
  return _refreshRate;
}

void ProceduralTexture::setRefreshRate(int value)
{
  _refreshRate = value;
  resetRefreshCounter();
}

bool ProceduralTexture::_shouldRender()
{
  if (!isEnabled || !isReady() || !_texture) {
    return false;
  }

  if (_fallbackTextureUsed) {
    return false;
  }

  if (_currentRefreshId == -1) { // At least render once
    _currentRefreshId = 1;
    return true;
  }

  if (refreshRate() == _currentRefreshId) {
    _currentRefreshId = 1;
    return true;
  }

  ++_currentRefreshId;
  return false;
}

Size& ProceduralTexture::getRenderSize()
{
  return _size;
}

const Size& ProceduralTexture::getRenderSize() const
{
  return _size;
}

void ProceduralTexture::resize(const Size& size, bool generateMipMaps)
{
  if (_fallbackTextureUsed) {
    return;
  }

  releaseInternalTexture();
  IRenderTargetOptions options;
  options.generateMipMaps = generateMipMaps;
  _texture = getScene()->getEngine()->createRenderTargetTexture(size, options);
}

void ProceduralTexture::_checkUniform(const std::string& uniformName)
{
  auto it = std::find(_uniforms.begin(), _uniforms.end(), uniformName);
  if (it == _uniforms.end()) {
    _uniforms.emplace_back(uniformName);
  }
}

ProceduralTexture& ProceduralTexture::setTexture(const std::string& iName,
                                                 Texture* texture)
{
  auto it = std::find(_samplers.begin(), _samplers.end(), iName);
  if (it == _samplers.end()) {
    _samplers.emplace_back(iName);
  }
  _textures[iName] = texture;

  return *this;
}

ProceduralTexture& ProceduralTexture::setFloat(const std::string& _name,
                                               float value)
{
  _checkUniform(_name);
  _floats[_name] = value;

  return *this;
}

ProceduralTexture& ProceduralTexture::setFloats(const std::string& _name,
                                                std::vector<float> value)
{
  _checkUniform(_name);
  _floatsArrays[_name] = value;

  return *this;
}

ProceduralTexture& ProceduralTexture::setColor3(const std::string& _name,
                                                const Color3& value)
{
  _checkUniform(_name);
  _colors3[_name] = value;

  return *this;
}

ProceduralTexture& ProceduralTexture::setColor4(const std::string& _name,
                                                const Color4& value)
{
  _checkUniform(_name);
  _colors4[_name] = value;

  return *this;
}

ProceduralTexture& ProceduralTexture::setVector2(const std::string& _name,
                                                 const Vector2& value)
{
  _checkUniform(_name);
  _vectors2[_name] = value;

  return *this;
}

ProceduralTexture& ProceduralTexture::setVector3(const std::string& _name,
                                                 const Vector3& value)
{
  _checkUniform(_name);
  _vectors3[_name] = value;

  return *this;
}

ProceduralTexture& ProceduralTexture::setMatrix(const std::string& _name,
                                                const Matrix& value)
{
  _checkUniform(_name);
  _matrices[_name] = value;

  return *this;
}

void ProceduralTexture::render(bool /*useCameraPostProcess*/)
{
  auto scene  = getScene();
  auto engine = scene->getEngine();

  // Render
  engine->enableEffect(_effect);
  engine->setState(false);

  // Texture
  for (auto& item : _textures) {
    _effect->setTexture(item.first, item.second);
  }

  // Float
  for (auto& item : _floats) {
    _effect->setFloat(item.first, item.second);
  }

  // Floats
  for (auto& item : _floatsArrays) {
    _effect->setArray(item.first, item.second);
  }

  // Color3
  for (auto& item : _colors3) {
    _effect->setColor3(item.first, item.second);
  }

  // Color4
  for (auto& item : _colors4) {
    const Color4& color = item.second;
    _effect->setFloat4(item.first, color.r, color.g, color.b, color.a);
  }

  // Vector2
  for (auto& item : _vectors2) {
    _effect->setVector2(item.first, item.second);
  }

  // Vector3
  for (auto& item : _vectors3) {
    _effect->setVector3(item.first, item.second);
  }

  // Matrix
  for (auto& item : _matrices) {
    _effect->setMatrix(item.first, item.second);
  }

  // VBOs
  std::unordered_map<std::string, VertexBuffer*> vertexBuffersTmp;
  vertexBuffersTmp.reserve(_vertexBuffers.size());
  for (const auto& item : _vertexBuffers) {
    vertexBuffersTmp[item.first] = item.second.get();
  }

  if (isCube) {
    for (unsigned int face = 0; face < 6; ++face) {
      engine->bindFramebuffer(_texture, face);

      // VBOs
      engine->bindBuffers(vertexBuffersTmp, _indexBuffer.get(), _effect);

      _effect->setFloat("face", static_cast<float>(face));

      // Clear
      engine->clear(scene->clearColor, true, true, true);

      // Draw order
      engine->draw(true, 0, 6);

      // Mipmaps
      if (face == 5) {
        engine->generateMipMapsForCubemap(_texture);
      }
    }
  }
  else {
    engine->bindFramebuffer(_texture);

    // VBOs
    engine->bindBuffers(vertexBuffersTmp, _indexBuffer.get(), _effect);

    // Clear
    engine->clear(scene->clearColor, true, true, true);

    // Draw order
    engine->draw(true, 0, 6);
  }

  // Unbind
  engine->unBindFramebuffer(_texture, isCube);

  if (onGenerated) {
    onGenerated();
  }
}

std::unique_ptr<ProceduralTexture> ProceduralTexture::clone() const
{
  ISize textureSize = getSize();
  auto newTexture   = std::make_unique<ProceduralTexture>(
    name, textureSize.width, _fragment, getScene(), _fallbackTexture,
    _generateMipMaps);

  // Base texture
  newTexture->setHasAlpha(hasAlpha());
  newTexture->level = level;

  // RenderTarget Texture
  newTexture->setCoordinatesMode(coordinatesMode());

  return newTexture;
}

void ProceduralTexture::dispose(bool /*doNotRecurse*/)
{
  getScene()->_proceduralTextures.erase(
    std::remove_if(
      getScene()->_proceduralTextures.begin(),
      getScene()->_proceduralTextures.end(),
      [this](const std::unique_ptr<ProceduralTexture>& proceduralTexture) {
        return proceduralTexture.get() == this;
      }),
    getScene()->_proceduralTextures.end());

  auto& vertexBuffer = _vertexBuffers[VertexBuffer::PositionKindChars];
  if (vertexBuffer) {
    vertexBuffer->dispose();
    vertexBuffer.reset(nullptr);
    _vertexBuffers.erase(VertexBuffer::PositionKindChars);
  }

  if (_indexBuffer
      && getScene()->getEngine()->_releaseBuffer(_indexBuffer.get())) {
    _indexBuffer.reset(nullptr);
  }

  Texture::dispose();
}

} // end of _namespace BABYLON
