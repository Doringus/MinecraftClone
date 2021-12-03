#pragma once

#include <cinttypes>
#include <cstddef>
#include <initializer_list>
#include <vector>



class IInputLayout {
public:
	virtual ~IInputLayout() = default;

	virtual void bind() = 0;
	virtual void unbind() = 0;
};