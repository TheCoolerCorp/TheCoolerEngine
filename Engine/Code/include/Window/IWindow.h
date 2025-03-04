#ifndef IWINDOW_H
#define IWINDOW_H

class GLwindow;

class IWindow
{
public:
	~IWindow() = default;
	virtual GLwindow* CastGLFW() { return nullptr; }

	virtual void Create(int a_width, int a_height) = 0;
	virtual void Destroy() = 0;

	virtual void PollEvents() = 0;
	virtual bool ShouldClose() = 0;
	virtual void ResizeFramebuffer(int a_width, int a_height) = 0;
	virtual void GetFramebufferSize(int& a_width, int& a_height) = 0;
};










#endif
