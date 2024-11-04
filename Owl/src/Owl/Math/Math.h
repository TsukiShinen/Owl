#pragma once
#include "glm/glm.hpp"

namespace Owl::Math
{
	bool DecomposeTransform(const glm::mat4& pTransform, glm::vec3& pTranslation, glm::vec3& pRotation,
	                        glm::vec3& pScale);
}
