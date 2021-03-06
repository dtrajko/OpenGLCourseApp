#pragma once

#include "FramebufferWater.h"
#include "Renderbuffer.h"
#include "TextureCubemapLite.h"


class Cubemap
{
public:
	Cubemap();
	Cubemap(int width, int height);
	inline TextureCubemapLite* GetTextureCubemap() { return m_TextureCubemap; };
	inline FramebufferWater* GetCaptureFBO() { return m_CaptureFBO; };
	inline Renderbuffer* GetCaptureRBO() { return m_CaptureRBO; };
	~Cubemap();

private:
	unsigned int m_Width;
	unsigned int m_Height;

	FramebufferWater* m_CaptureFBO;
	Renderbuffer* m_CaptureRBO;
	TextureCubemapLite* m_TextureCubemap;

};
