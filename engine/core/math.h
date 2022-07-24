#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/epsilon.hpp"

namespace Math = glm;

using Vector2 = Math::vec2;
using Vector3 = Math::vec3;
using Vector4 = Math::vec4;
using Matrix4x4 = Math::mat4x4;

namespace Convert
{
constexpr float s_to_ns = 1e9f;
constexpr float ns_to_s = 1.0f / s_to_ns;
};
