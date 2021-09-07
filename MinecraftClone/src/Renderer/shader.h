#pragma once

#include <filesystem>

class Matrix4x4 {
public:
	virtual ~Matrix4x4() {}
};

class ITexture;

class IShader {
public:
	IShader(const std::filesystem::path& vertexShaderPath, const std::filesystem::path& fragmentShaderPath) :
		m_VertexShaderPath(vertexShaderPath), m_FragmentShaderPath(fragmentShaderPath) {}
	virtual ~IShader() {}

	virtual bool create() noexcept = 0;
	virtual void use() const noexcept = 0;
	virtual void setMatrix(const Matrix4x4& matrix) noexcept = 0;
	virtual void setTexture(const ITexture& texture) noexcept = 0;

protected:
	std::filesystem::path m_VertexShaderPath, m_FragmentShaderPath;
};