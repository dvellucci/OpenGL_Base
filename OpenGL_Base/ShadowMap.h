#pragma once
#include <../glad\glad.h>
#include <../GLFW\glfw3.h>
#include <iostream>

class ShadowMap
{
public:
	ShadowMap(int shadowWidth, int shadowHeight);
	~ShadowMap();
	void set(int shadowWidth, int shadowHeight);
	void bindForReading(GLenum textureId);
	void shadowMapPass();
	const unsigned int getDepthMap() { return m_depthMap; }
	const unsigned int getFBO() { return m_fbo; }

	int m_shadowWidth, m_shadowHeight;

private:
	unsigned int m_depthMap;
	unsigned int m_fbo;
};