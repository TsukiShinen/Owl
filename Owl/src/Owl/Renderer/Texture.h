#pragma once

#include "string"
#include "Owl/Core/Base.h"

namespace Owl
{
	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual void SetData(void* pData, uint32_t pSize) = 0;

		virtual void Bind(uint32_t pSlot = 0) const = 0;
	};

	class Texture2D : public Texture
	{
	public:
		static Ref<Texture2D> Create(uint32_t pWidth, uint32_t pHeight);
		static Ref<Texture2D> Create(const std::string& pPath);
	};
}
