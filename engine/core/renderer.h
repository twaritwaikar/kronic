#pragma once

class Renderer
{
public:
	Renderer() = default;
	virtual ~Renderer() = default;

	virtual void draw() = 0;
};
