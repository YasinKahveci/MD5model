#include "MeshAnimation.hpp"
#include <iostream>
#include <fstream>
#include "Helpers.hpp"


bool MeshAnimation::loadModelAnim(const std::string& fileName)
{
	
	std::ifstream file(fileName);
	if (file.fail())
	{
		std::cout << fileName + " couldn't open" << std::endl;
		return false;
	}

	std::string Key;
	std::string Skip;

	int LengthofFile = GetFileLength(file);
	file >> Key;

	while (!file.eof())
	{
		if (Key == "MD5Version")
		{
			file >> m_MD5Version;
			if (m_MD5Version != 10)
			{
				return false;
			}
		}
		else if (Key == "commandline" || Key == "//")
		{
			ignoreLine(file, LengthofFile);
		}
		else if (Key == "numFrames")
		{
			file >> m_numFrames;
		}
		else if (Key == "numJoints")
		{
			file >> m_numJoints;
		}
		else if (Key == "frameRate")
		{
			file >> m_frameRate;
		}
		else if (Key == "numAnimatedComponents")
		{
			file >> m_numAnimatedComponents;
		}
		else if (Key == "hierarchy")
		{
			file >> Skip;
			for (size_t i = 0; i < m_numJoints; i++)
			{
				JointHierarchy joint;
				file >> joint.m_jointName >> joint.m_parent >>
					joint.flags >> joint.startIndex;
				removeQuatos(joint.m_jointName);

				m_hierarchy.push_back(joint);

				ignoreLine(file, LengthofFile);
			}
			file >> Key;
		}
		else if (Key == "bounds")
		{
			file >> Skip;
			for (size_t i = 0; i < m_numFrames; i++)
			{
				Bound bound;
				file >> Skip >> bound.m_Min.x >> bound.m_Min.y >> bound.m_Min.z;
				file >> Skip >> Skip >> bound.m_Max.x >> bound.m_Max.y >> bound.m_Max.z;

				m_bounds.push_back(bound);

				ignoreLine(file, LengthofFile);
			}
			file >> Skip;
		}
		else if (Key == "baseframe")
		{
			file >> Skip;
			for (size_t i = 0; i < m_numJoints; i++)
			{
				BaseFrame bframe;
				file >> Skip >> bframe.m_Pos.x >> bframe.m_Pos.y >> bframe.m_Pos.z >> Skip;
				file >> Skip >> bframe.m_Orient.x >> bframe.m_Orient.y >> bframe.m_Orient.z;

				m_baseFrames.push_back(bframe);

				ignoreLine(file, LengthofFile);
			}
			file >> Skip;
		}
		else if (Key == "frame")
		{
			Frame frame;
			file >> frame.m_ID >> Skip;
			for (size_t i = 0; i < m_numAnimatedComponents; i++)
			{
				float frameFloat;
				file >> frameFloat;
				frame.m_FrameFloats.push_back(frameFloat);
			}
			m_frames.push_back(frame);

			BuildFrameSkeleton(frame);

			file >> Skip;
			ignoreLine(file, LengthofFile);
		}
		file >> Key;
	}

	m_FrameDuration = 1.0f / (float)m_frameRate;
	m_AnimDongusu = (m_FrameDuration * (float)m_numFrames);
	m_AnimasyonSuresi = 0.0f;

	return true;
}
void MeshAnimation::BuildFrameSkeleton(const Frame& frameData)
{
	std::vector<Joint> skeleton;
	for (size_t i = 0; i < m_numJoints; i++)
	{
		BaseFrame* baseJoint = &m_baseFrames[i];
		Joint skelJoint;

		int j = 0;

		skelJoint.m_Position = baseJoint->m_Pos;
		skelJoint.m_Orient = baseJoint->m_Orient;

		if (m_hierarchy[i].flags & 1) 
		{
			skelJoint.m_Position.x = frameData.m_FrameFloats[m_hierarchy[i].startIndex + j];
			++j;
		}

		if (m_hierarchy[i].flags & 2) 
		{
			skelJoint.m_Position.y = frameData.m_FrameFloats[m_hierarchy[i].startIndex + j];
			++j;
		}

		if (m_hierarchy[i].flags & 4) 
		{
			skelJoint.m_Position.z = frameData.m_FrameFloats[m_hierarchy[i].startIndex + j];
			++j;
		}

		if (m_hierarchy[i].flags & 8) 
		{
			skelJoint.m_Orient.x = frameData.m_FrameFloats[m_hierarchy[i].startIndex + j];
			++j;
		}

		if (m_hierarchy[i].flags & 16) 
		{
			skelJoint.m_Orient.y = frameData.m_FrameFloats[m_hierarchy[i].startIndex + j];
			++j;
		}

		if (m_hierarchy[i].flags & 32) 
		{
			skelJoint.m_Orient.z = frameData.m_FrameFloats[m_hierarchy[i].startIndex + j];
			++j;
		}

		skelJoint.ComputeQuatW();
		
		skelJoint.m_Parent = m_hierarchy[i].m_parent;
		skelJoint.m_Name = m_hierarchy[i].m_jointName;

		if (skelJoint.m_Parent > -1)
		{
			Joint* parentJoint = &skeleton[skelJoint.m_Parent];
			glm::vec3 rpos;

			rpos = parentJoint->m_Orient * skelJoint.m_Position;

			skelJoint.m_Position = rpos + parentJoint->m_Position;
			skelJoint.m_Orient = parentJoint->m_Orient * skelJoint.m_Orient;

			skelJoint.m_Orient = glm::normalize(skelJoint.m_Orient);
		}
		skeleton.push_back(skelJoint);
	}
	m_skeletons.push_back(skeleton);
}
void MeshAnimation::Update(float DeltaTime)
{
	m_AnimasyonSuresi += DeltaTime;
	if (m_AnimasyonSuresi > m_AnimDongusu)
	{
		m_AnimasyonSuresi -= m_AnimDongusu;
		AnimActive = false;
		return;
	}
	float fFramNum = m_AnimasyonSuresi * (float)m_frameRate;
	int iFrame0 = (int)floorf(fFramNum);
	int iFrame1 = (int)ceilf(fFramNum);
	iFrame0 = iFrame0 % m_numFrames;
	iFrame1 = iFrame1 % m_numFrames;

	float fInterpolate = fmodf(m_AnimasyonSuresi, m_FrameDuration) / m_FrameDuration;
	InterpolateSkeletons(iFrame0, iFrame1, fInterpolate);
}

void MeshAnimation::InterpolateSkeletons(int frame0, int frame1, float fInterpolate)
{
	m_AnimSkelaton.clear();
	for (size_t i = 0; i < m_numJoints; i++)
	{
		Joint joint0 = m_skeletons.at(frame0).at(i);
		Joint joint1 = m_skeletons.at(frame1).at(i);

		Joint animJoint;
		animJoint.m_Parent = joint0.m_Parent;


		animJoint.m_Position = joint0.m_Position + fInterpolate * (joint1.m_Position - joint0.m_Position);
		animJoint.m_Orient = glm::mix(joint0.m_Orient, joint1.m_Orient, fInterpolate);


		m_AnimSkelaton.push_back(animJoint);
	}
}

void MeshAnimation::ClearAll()
{
	m_hierarchy.clear();
	m_bounds.clear();
	m_baseFrames.clear();
	m_frames.clear();
	for (size_t i = 0; i < m_skeletons.size(); i++)
	{
		m_skeletons.at(i).clear();
	}
	m_skeletons.clear();
	m_AnimSkelaton.clear();
}
