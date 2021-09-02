#pragma once

class IRenderingContext {
public:
	virtual void init() const = 0;
	virtual void swapBuffers() const = 0;
};