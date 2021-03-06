﻿#ifndef BABYLON_MATERIALS_LIBRARY_LAVA_LAVA_FRAGMENT_FX_H
#define BABYLON_MATERIALS_LIBRARY_LAVA_LAVA_FRAGMENT_FX_H

namespace BABYLON {

extern const char* lavaPixelShader;

const char* lavaPixelShader
  = "#ifdef GL_ES\n"
    "precision highp float;\n"
    "#endif\n"
    "\n"
    "// Constants\n"
    "uniform vec3 vEyePosition;\n"
    "uniform vec4 vDiffuseColor;\n"
    "\n"
    "// Input\n"
    "varying vec3 vPositionW;\n"
    "\n"
    "// MAGMAAAA\n"
    "uniform float time;\n"
    "uniform float speed;\n"
    "uniform float movingSpeed;\n"
    "uniform vec3 fogColor;\n"
    "uniform sampler2D noiseTexture;\n"
    "uniform float fogDensity;\n"
    "\n"
    "// Varying\n"
    "varying float noise;\n"
    "\n"
    "#ifdef NORMAL\n"
    "varying vec3 vNormalW;\n"
    "#endif\n"
    "\n"
    "#ifdef VERTEXCOLOR\n"
    "varying vec4 vColor;\n"
    "#endif\n"
    "\n"
    "// Lights\n"
    "#include<__decl__lightFragment>[0]\n"
    "#include<__decl__lightFragment>[1]\n"
    "#include<__decl__lightFragment>[2]\n"
    "#include<__decl__lightFragment>[3]\n"
    "\n"
    "\n"
    "#include<lightsFragmentFunctions>\n"
    "#include<shadowsFragmentFunctions>\n"
    "\n"
    "// Samplers\n"
    "#ifdef DIFFUSE\n"
    "varying vec2 vDiffuseUV;\n"
    "uniform sampler2D diffuseSampler;\n"
    "uniform vec2 vDiffuseInfos;\n"
    "#endif\n"
    "\n"
    "#include<clipPlaneFragmentDeclaration>\n"
    "\n"
    "// Fog\n"
    "#include<fogFragmentDeclaration>\n"
    "\n"
    "\n"
    "float random( vec3 scale, float seed ){\n"
    "  return fract( sin( dot( gl_FragCoord.xyz + seed, scale ) ) * 43758.5453 + seed ) ;\n"
    "}\n"
    "\n"
    "\n"
    "void main(void) {\n"
    "#include<clipPlaneFragment>\n"
    "\n"
    "  vec3 viewDirectionW = normalize(vEyePosition - vPositionW);\n"
    "\n"
    "  // Base color\n"
    "  vec4 baseColor = vec4(1., 1., 1., 1.);\n"
    "  vec3 diffuseColor = vDiffuseColor.rgb;\n"
    "\n"
    "  // Alpha\n"
    "  float alpha = vDiffuseColor.a;\n"
    "\n"
    "\n"
    "\n"
    "#ifdef DIFFUSE\n"
    "  ////// MAGMA ///\n"
    "\n"
    "  vec4 noiseTex = texture2D( noiseTexture, vDiffuseUV );\n"
    "  vec2 T1 = vDiffuseUV + vec2( 1.5, -1.5 ) * time  * 0.02;\n"
    "  vec2 T2 = vDiffuseUV + vec2( -0.5, 2.0 ) * time * 0.01 * speed;\n"
    "\n"
    "  T1.x += noiseTex.x * 2.0;\n"
    "  T1.y += noiseTex.y * 2.0;\n"
    "  T2.x -= noiseTex.y * 0.2 + time*0.001*movingSpeed;\n"
    "  T2.y += noiseTex.z * 0.2 + time*0.002*movingSpeed;\n"
    "\n"
    "  float p = texture2D( noiseTexture, T1 * 3.0 ).a;\n"
    "\n"
    "  vec4 lavaColor = texture2D( diffuseSampler, T2 * 4.0);\n"
    "  vec4 temp = lavaColor * ( vec4( p, p, p, p ) * 2. ) + ( lavaColor * lavaColor - 0.1 );\n"
    "\n"
    "  baseColor = temp;\n"
    "\n"
    "  float depth = gl_FragCoord.z * 4.0;\n"
    "  const float LOG2 = 1.442695;\n"
    "  float fogFactor = exp2( - fogDensity * fogDensity * depth * depth * LOG2 );\n"
    "  fogFactor = 1.0 - clamp( fogFactor, 0.0, 1.0 );\n"
    "\n"
    "  baseColor = mix( baseColor, vec4( fogColor, baseColor.w ), fogFactor );\n"
    "  diffuseColor = baseColor.rgb;\n"
    "  ///// END MAGMA ////\n"
    "\n"
    "\n"
    "\n"
    "//  baseColor = texture2D(diffuseSampler, vDiffuseUV);\n"
    "\n"
    "#ifdef ALPHATEST\n"
    "  if (baseColor.a < 0.4)\n"
    "  discard;\n"
    "#endif\n"
    "\n"
    "  baseColor.rgb *= vDiffuseInfos.y;\n"
    "#endif\n"
    "\n"
    "#ifdef VERTEXCOLOR\n"
    "  baseColor.rgb *= vColor.rgb;\n"
    "#endif\n"
    "\n"
    "  // Bump\n"
    "#ifdef NORMAL\n"
    "  vec3 normalW = normalize(vNormalW);\n"
    "#else\n"
    "  vec3 normalW = vec3(1.0, 1.0, 1.0);\n"
    "#endif\n"
    "\n"
    "  // Lighting\n"
    "  vec3 diffuseBase = vec3(0., 0., 0.);\n"
    "  lightingInfo info;\n"
    "  float shadow = 1.;\n"
    "  float glossiness = 0.;\n"
    "  \n"
    "#include<lightFragment>[0]\n"
    "#include<lightFragment>[1]\n"
    "#include<lightFragment>[2]\n"
    "#include<lightFragment>[3]\n"
    "\n"
    "\n"
    "#ifdef VERTEXALPHA\n"
    "  alpha *= vColor.a;\n"
    "#endif\n"
    "\n"
    "  vec3 finalDiffuse = clamp(diffuseBase * diffuseColor, 0.0, 1.0) * baseColor.rgb;\n"
    "\n"
    "  // Composition\n"
    "  vec4 color = vec4(finalDiffuse, alpha);\n"
    "\n"
    "#include<fogFragment>\n"
    "\n"
    "  gl_FragColor = color;\n"
    "}\n";

} // end of namespace BABYLON

#endif // end of BABYLON_MATERIALS_LIBRARY_LAVA_LAVA_FRAGMENT_FX_H
