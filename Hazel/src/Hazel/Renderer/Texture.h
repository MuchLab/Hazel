#pragma once

namespace Hazel {

	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual int GetHeight() const = 0;
		virtual int GetWidth() const = 0;

		virtual void SetData(void* data, uint32_t size) = 0;
		virtual const uint32_t GetRendererID() const = 0;

		virtual void Bind(const uint32_t slot = 0) const = 0;

		virtual bool operator==(Texture& other) = 0;
	};

	class Texture2D : public Texture
	{
	public:
		static Ref<Texture2D> Create(const uint32_t width, const uint32_t height);
		static Ref<Texture2D> Create(const std::string& path);
	};
}


