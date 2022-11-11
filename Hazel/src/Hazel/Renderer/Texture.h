#pragma once

namespace Hazel {

	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual int GetHeight() const = 0;
		virtual int GetWidth() const = 0;

		virtual void Bind(const uint32_t slot = 0) const = 0;
	};

	class Texture2D : public Texture
	{
	public:
		static Ref<Texture2D> Create(const std::string& path);
	};
}


