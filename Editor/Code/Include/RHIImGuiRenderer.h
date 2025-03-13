#pragma once

/*
virtual class for any 3d renderer-specific functions to comply with the RHI
*/
class RHIImGuiRenderer
{
public:
	virtual void Init() = 0;

	virtual void NewFrame() = 0;
	virtual void Render() = 0;
};

