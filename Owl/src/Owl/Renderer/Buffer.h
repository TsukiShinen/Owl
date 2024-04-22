﻿#pragma once

namespace Owl
{
	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() = default;
		
		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;

		static VertexBuffer* Create(float* pVertices, uint32_t pSize);
	};
	
	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() = default;
		
		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;

		virtual uint32_t GetCount() const = 0;

		static IndexBuffer* Create(uint32_t* pIndices, uint32_t pCount);
	};
}
