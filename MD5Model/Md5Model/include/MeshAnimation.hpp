#pragma once
#include <vector>
#include <string>
#include <vector>
#include "Joint.hpp"
#include "Bound.hpp"
#include "BaseFrame.hpp"
#include "Frame.hpp"
class MeshAnimation
{
public:

	int m_MD5Version;

	unsigned int m_numFrames;
	unsigned int m_numJoints;
	unsigned int m_frameRate;
	unsigned int m_numAnimatedComponents;

	bool AnimActive = false;

	float m_FrameDuration;
	float m_AnimDongusu;
	float m_AnimasyonSuresi;
	
	std::vector<JointHierarchy>		m_hierarchy;
	std::vector<Bound>				m_bounds;
	std::vector<BaseFrame>			m_baseFrames;
	std::vector<Frame>				m_frames;
	std::vector<std::vector<Joint>>	m_skeletons;

	std::vector<Joint>	m_AnimSkelaton;

	bool loadModelAnim(const std::string& fileName);
	void Update(float DeltaTime);
	
	void ClearAll();
private:
	void BuildFrameSkeleton(const Frame& frameData);
	void InterpolateSkeletons(int frame1, int frame2, float fInterpolate);
	
};