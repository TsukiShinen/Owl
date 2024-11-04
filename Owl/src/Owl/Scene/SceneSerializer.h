#pragma once
#include "Scene.h"

namespace Owl
{
	class SceneSerializer
	{
	public:
		SceneSerializer(const Ref<Scene>& pScene);

		void Serialize(const std::filesystem::path& pFilepath);
		void SerializeBinary(const std::filesystem::path& pFilepath);

		bool Deserialize(const std::filesystem::path& pFilepath);
		bool DeserializeBinary(const std::filesystem::path& pFilepath);

	private:
		Ref<Scene> m_Scene;
	};
}
