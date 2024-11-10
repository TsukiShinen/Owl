#include "opch.h"
#include "SceneSerializer.h"

#include "Components.h"
#include "Entity.h"

#define YAML_CPP_STATIC_DEFINE  
#include <yaml-cpp/yaml.h>

namespace YAML
{
	template <>
	struct convert<glm::vec2>
	{
		static Node encode(const glm::vec2& pRhs)
		{
			Node node;
			node.push_back(pRhs.x);
			node.push_back(pRhs.y);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& pNode, glm::vec2& pRhs)
		{
			if (!pNode.IsSequence() || pNode.size() != 2)
				return false;

			pRhs.x = pNode[0].as<float>();
			pRhs.y = pNode[1].as<float>();
			return true;
		}
	};

	template <>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& pRhs)
		{
			Node node;
			node.push_back(pRhs.x);
			node.push_back(pRhs.y);
			node.push_back(pRhs.z);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& pNode, glm::vec3& pRhs)
		{
			if (!pNode.IsSequence() || pNode.size() != 3)
				return false;

			pRhs.x = pNode[0].as<float>();
			pRhs.y = pNode[1].as<float>();
			pRhs.z = pNode[2].as<float>();
			return true;
		}
	};

	template <>
	struct convert<glm::vec4>
	{
		static Node encode(const glm::vec4& pRhs)
		{
			Node node;
			node.push_back(pRhs.x);
			node.push_back(pRhs.y);
			node.push_back(pRhs.z);
			node.push_back(pRhs.w);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& pNode, glm::vec4& pRhs)
		{
			if (!pNode.IsSequence() || pNode.size() != 4)
				return false;

			pRhs.x = pNode[0].as<float>();
			pRhs.y = pNode[1].as<float>();
			pRhs.z = pNode[2].as<float>();
			pRhs.w = pNode[3].as<float>();
			return true;
		}
	};
}

namespace Owl
{
	YAML::Emitter& operator<<(YAML::Emitter& pOut, const glm::vec2& pVector)
	{
		pOut << YAML::Flow;
		pOut << YAML::BeginSeq << pVector.x << pVector.y << YAML::EndSeq;
		return pOut;
	}

	YAML::Emitter& operator<<(YAML::Emitter& pOut, const glm::vec3& pVector)
	{
		pOut << YAML::Flow;
		pOut << YAML::BeginSeq << pVector.x << pVector.y << pVector.z << YAML::EndSeq;
		return pOut;
	}

	YAML::Emitter& operator<<(YAML::Emitter& pOut, const glm::vec4& pVector)
	{
		pOut << YAML::Flow;
		pOut << YAML::BeginSeq << pVector.x << pVector.y << pVector.z << pVector.w << YAML::EndSeq;
		return pOut;
	}

	static std::string RigidBody2DBodyTypeToString(Rigidbody2DComponent::BodyType bodyType)
	{
		switch (bodyType)
		{
			case Rigidbody2DComponent::BodyType::Static: return "Static";
			case Rigidbody2DComponent::BodyType::Dynamic: return "Dynamic";
			case Rigidbody2DComponent::BodyType::Kinematic: return "Kinematic";
		}

		OWL_CORE_ASSERT(false, "Unknown body type");
		return {};
	}

	static Rigidbody2DComponent::BodyType RigidBody2DBodyTypeFromString(const std::string& bodyTypeString)
	{
		if (bodyTypeString == "Static") return Rigidbody2DComponent::BodyType::Static;
		if (bodyTypeString == "Dynamic") return Rigidbody2DComponent::BodyType::Dynamic;
		if (bodyTypeString == "Kinematic") return Rigidbody2DComponent::BodyType::Kinematic;

		OWL_CORE_ASSERT(false, "Unknown body type");
		return Rigidbody2DComponent::BodyType::Static;
	}

	SceneSerializer::SceneSerializer(const Ref<Scene>& pScene)
		: m_Scene(pScene)
	{
	}

	static void SerializeEntity(YAML::Emitter& pOut, Entity pEntity)
	{
		OWL_CORE_ASSERT(pEntity.HasComponent<IdComponent>())

		pOut << YAML::BeginMap; // Entity
		pOut << YAML::Key << "Entity" << YAML::Value << pEntity.GetUuid();

		if (pEntity.HasComponent<TagComponent>())
		{
			pOut << YAML::Key << "TagComponent";
			pOut << YAML::BeginMap; // TagComponent

			auto& tag = pEntity.GetComponent<TagComponent>().Tag;
			pOut << YAML::Key << "Tag" << YAML::Value << tag;

			pOut << YAML::EndMap; // TagComponent
		}

		if (pEntity.HasComponent<TransformComponent>())
		{
			pOut << YAML::Key << "TransformComponent";
			pOut << YAML::BeginMap; // TransformComponent

			auto& tc = pEntity.GetComponent<TransformComponent>();
			pOut << YAML::Key << "Translation" << YAML::Value << tc.Translation;
			pOut << YAML::Key << "Rotation" << YAML::Value << tc.Rotation;
			pOut << YAML::Key << "Scale" << YAML::Value << tc.Scale;

			pOut << YAML::EndMap; // TransformComponent
		}

		if (pEntity.HasComponent<CameraComponent>())
		{
			pOut << YAML::Key << "CameraComponent";
			pOut << YAML::BeginMap; // CameraComponent

			auto& cameraComponent = pEntity.GetComponent<CameraComponent>();
			auto& camera = cameraComponent.Camera;

			pOut << YAML::Key << "Camera" << YAML::Value;
			pOut << YAML::BeginMap; // Camera
			pOut << YAML::Key << "ProjectionType" << YAML::Value << static_cast<int>(camera.GetProjectionType());
			pOut << YAML::Key << "PerspectiveFOV" << YAML::Value << camera.GetPerspectiveVerticalFov();
			pOut << YAML::Key << "PerspectiveNear" << YAML::Value << camera.GetPerspectiveNearClip();
			pOut << YAML::Key << "PerspectiveFar" << YAML::Value << camera.GetPerspectiveFarClip();
			pOut << YAML::Key << "OrthographicSize" << YAML::Value << camera.GetOrthographicSize();
			pOut << YAML::Key << "OrthographicNear" << YAML::Value << camera.GetOrthographicNearClip();
			pOut << YAML::Key << "OrthographicFar" << YAML::Value << camera.GetOrthographicFarClip();
			pOut << YAML::EndMap; // Camera

			pOut << YAML::Key << "Primary" << YAML::Value << cameraComponent.Primary;
			pOut << YAML::Key << "FixedAspectRatio" << YAML::Value << cameraComponent.FixedAspectRatio;

			pOut << YAML::EndMap; // CameraComponent
		}

		if (pEntity.HasComponent<SpriteRendererComponent>())
		{
			pOut << YAML::Key << "SpriteRendererComponent";
			pOut << YAML::BeginMap; // SpriteRendererComponent

			auto& spriteRendererComponent = pEntity.GetComponent<SpriteRendererComponent>();
			pOut << YAML::Key << "Color" << YAML::Value << spriteRendererComponent.Color;

			pOut << YAML::EndMap; // SpriteRendererComponent
		}

		if (pEntity.HasComponent<CircleRendererComponent>())
		{
			pOut << YAML::Key << "CircleRendererComponent";
			pOut << YAML::BeginMap; // CircleRendererComponent

			auto& circleRendererComponent = pEntity.GetComponent<CircleRendererComponent>();
			pOut << YAML::Key << "Color" << YAML::Value << circleRendererComponent.Color;
			pOut << YAML::Key << "Thickness" << YAML::Value << circleRendererComponent.Thickness;
			pOut << YAML::Key << "Fade" << YAML::Value << circleRendererComponent.Fade;

			pOut << YAML::EndMap; // CircleRendererComponent
		}

		if (pEntity.HasComponent<Rigidbody2DComponent>())
		{
			pOut << YAML::Key << "Rigidbody2DComponent";
			pOut << YAML::BeginMap; // Rigidbody2DComponent
			
			auto& rb2dComponent = pEntity.GetComponent<Rigidbody2DComponent>();
			pOut << YAML::Key << "BodyType" << YAML::Value << RigidBody2DBodyTypeToString(rb2dComponent.Type);
			pOut << YAML::Key << "FixedRotation" << YAML::Value << rb2dComponent.FixedRotation;
			
			pOut << YAML::EndMap; // Rigidbody2DComponent
		}

		if (pEntity.HasComponent<BoxCollider2DComponent>())
		{
			pOut << YAML::Key << "BoxCollider2DComponent";
			pOut << YAML::BeginMap; // BoxCollider2DComponent
			
			auto& bc2dComponent = pEntity.GetComponent<BoxCollider2DComponent>();
			pOut << YAML::Key << "Offset" << YAML::Value << bc2dComponent.Offset;
			pOut << YAML::Key << "Size" << YAML::Value << bc2dComponent.Size;
			pOut << YAML::Key << "Density" << YAML::Value << bc2dComponent.Density;
			pOut << YAML::Key << "Friction" << YAML::Value << bc2dComponent.Friction;
			pOut << YAML::Key << "Restitution" << YAML::Value << bc2dComponent.Restitution;
			pOut << YAML::Key << "RestitutionThreshold" << YAML::Value << bc2dComponent.RestitutionThreshold;
			
			pOut << YAML::EndMap; // BoxCollider2DComponent
		}

		if (pEntity.HasComponent<CircleCollider2DComponent>())
		{
			pOut << YAML::Key << "CircleCollider2DComponent";
			pOut << YAML::BeginMap; // CircleCollider2DComponent
			
			auto& cc2dComponent = pEntity.GetComponent<CircleCollider2DComponent>();
			pOut << YAML::Key << "Offset" << YAML::Value << cc2dComponent.Offset;
			pOut << YAML::Key << "Radius" << YAML::Value << cc2dComponent.Radius;
			pOut << YAML::Key << "Density" << YAML::Value << cc2dComponent.Density;
			pOut << YAML::Key << "Friction" << YAML::Value << cc2dComponent.Friction;
			pOut << YAML::Key << "Restitution" << YAML::Value << cc2dComponent.Restitution;
			pOut << YAML::Key << "RestitutionThreshold" << YAML::Value << cc2dComponent.RestitutionThreshold;
			
			pOut << YAML::EndMap; // CircleCollider2DComponent
		}

		pOut << YAML::EndMap; // Entity
	}

	void SceneSerializer::Serialize(const std::filesystem::path& pFilepath)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << "Untitled";
		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
		m_Scene->m_Registry.each([&](auto entityID)
		{
			Entity entity = {entityID, m_Scene.get()};
			if (!entity)
				return;

			SerializeEntity(out, entity);
		});
		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream fout(pFilepath);
		fout << out.c_str();
	}

	void SceneSerializer::SerializeBinary(const std::filesystem::path& pFilepath)
	{
		// Not Implemented
		OWL_CORE_ASSERT(false)
	}

	bool SceneSerializer::Deserialize(const std::filesystem::path& pFilepath)
	{
		YAML::Node data;
		try
		{
			data = YAML::LoadFile(pFilepath.string());
		}
		catch (YAML::ParserException e)
		{
			return false;
		}

		if (!data["Scene"])
			return false;

		auto sceneName = data["Scene"].as<std::string>();
		OWL_CORE_TRACE("Deserializing scene '{0}'", sceneName);

		if (auto entities = data["Entities"])
		{
			for (auto entity : entities)
			{
				auto uuid = entity["Entity"].as<uint64_t>();

				std::string name;
				if (auto tagComponent = entity["TagComponent"])
					name = tagComponent["Tag"].as<std::string>();

				OWL_CORE_TRACE("Deserialized entity with ID = {0}, name = {1}", uuid, name);

				Entity deserializedEntity = m_Scene->CreateEntityWithUuid(uuid, name);

				if (auto transformComponent = entity["TransformComponent"])
				{
					// Entities always have transforms
					auto& tc = deserializedEntity.GetComponent<TransformComponent>();
					tc.Translation = transformComponent["Translation"].as<glm::vec3>();
					tc.Rotation = transformComponent["Rotation"].as<glm::vec3>();
					tc.Scale = transformComponent["Scale"].as<glm::vec3>();
				}

				if (auto cameraComponent = entity["CameraComponent"])
				{
					auto& cc = deserializedEntity.AddComponent<CameraComponent>();

					auto cameraProps = cameraComponent["Camera"];
					cc.Camera.SetProjectionType(
						static_cast<SceneCamera::ProjectionType>(cameraProps["ProjectionType"].as<int>()));

					cc.Camera.SetPerspectiveVerticalFov(cameraProps["PerspectiveFOV"].as<float>());
					cc.Camera.SetPerspectiveNearClip(cameraProps["PerspectiveNear"].as<float>());
					cc.Camera.SetPerspectiveFarClip(cameraProps["PerspectiveFar"].as<float>());

					cc.Camera.SetOrthographicSize(cameraProps["OrthographicSize"].as<float>());
					cc.Camera.SetOrthographicNearClip(cameraProps["OrthographicNear"].as<float>());
					cc.Camera.SetOrthographicFarClip(cameraProps["OrthographicFar"].as<float>());

					cc.Primary = cameraComponent["Primary"].as<bool>();
					cc.FixedAspectRatio = cameraComponent["FixedAspectRatio"].as<bool>();
				}

				if (auto spriteRendererComponent = entity["SpriteRendererComponent"])
				{
					auto& src = deserializedEntity.AddComponent<SpriteRendererComponent>();
					src.Color = spriteRendererComponent["Color"].as<glm::vec4>();
				}

				if (auto circleRendererComponent = entity["CircleRendererComponent"])
				{
					auto& crc = deserializedEntity.AddComponent<CircleRendererComponent>();
					crc.Color = circleRendererComponent["Color"].as<glm::vec4>();
					crc.Thickness = circleRendererComponent["Thickness"].as<float>();
					crc.Fade = circleRendererComponent["Fade"].as<float>();
				}

				if (auto rigidbody2DComponent = entity["Rigidbody2DComponent"])
				{
					auto& rb2d = deserializedEntity.AddComponent<Rigidbody2DComponent>();
					rb2d.Type = RigidBody2DBodyTypeFromString(rigidbody2DComponent["BodyType"].as<std::string>());
					rb2d.FixedRotation = rigidbody2DComponent["FixedRotation"].as<bool>();
				}

				if (auto boxCollider2DComponent = entity["BoxCollider2DComponent"])
				{
					auto& bc2d = deserializedEntity.AddComponent<BoxCollider2DComponent>();
					bc2d.Offset = boxCollider2DComponent["Offset"].as<glm::vec2>();
					bc2d.Size = boxCollider2DComponent["Size"].as<glm::vec2>();
					bc2d.Density = boxCollider2DComponent["Density"].as<float>();
					bc2d.Friction = boxCollider2DComponent["Friction"].as<float>();
					bc2d.Restitution = boxCollider2DComponent["Restitution"].as<float>();
					bc2d.RestitutionThreshold = boxCollider2DComponent["RestitutionThreshold"].as<float>();
				}

				if (auto circleCollider2DComponent = entity["CircleCollider2DComponent"])
				{
					auto& cc2d = deserializedEntity.AddComponent<CircleCollider2DComponent>();
					cc2d.Offset = circleCollider2DComponent["Offset"].as<glm::vec2>();
					cc2d.Radius = circleCollider2DComponent["Radius"].as<float>();
					cc2d.Density = circleCollider2DComponent["Density"].as<float>();
					cc2d.Friction = circleCollider2DComponent["Friction"].as<float>();
					cc2d.Restitution = circleCollider2DComponent["Restitution"].as<float>();
					cc2d.RestitutionThreshold = circleCollider2DComponent["RestitutionThreshold"].as<float>();
				}
			}
		}

		return true;
	}

	bool SceneSerializer::DeserializeBinary(const std::filesystem::path& pFilepath)
	{
		// Not Implemented
		OWL_CORE_ASSERT(false)
		return false;
	}
}
