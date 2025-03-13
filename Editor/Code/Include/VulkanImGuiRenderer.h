#pragma once
#include "RHIImGuiRenderer.h"

//why the fuck is RHIImGuiRenderer not recognised
class VulkanImGuiRenderer : RHIImGuiRenderer
{
public:
	void Init() override;

	void NewFrame() override;
	void Render() override;
};