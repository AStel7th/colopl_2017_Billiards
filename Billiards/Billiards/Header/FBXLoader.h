#pragma once

#include <vector>
#include <string>
#include <memory>
#include <unordered_map>
#include <wrl/client.h>
#include "func.h"

#ifndef FBXSDK_NEW_API
#define FBXSDK_NEW_API	// �V�����o�[�W�����g���Ƃ��p
#endif

#include <fbxsdk.h>
#include <Windows.h>

#include "FBXInitialize.h"

using namespace std;
using namespace Microsoft::WRL;

// UVSet��, ���_����UV�Z�b�g����
typedef std::tr1::unordered_map<std::string, int> UVsetID;
// UVSet��, �e�N�X�`���p�X��(�P��UVSet�ɕ����̃e�N�X�`�����Ԃ牺�����Ă邱�Ƃ�����)
typedef std::tr1::unordered_map<std::string, std::vector<std::string>> TextureSet;

struct FBX_VERTEX_DATA
{
	FbxVector4 pos;
	FbxVector4 nor;
	FbxVector2 uv;
	UINT boneIndex[4];	//�{�[���ԍ�
	float boneWeight[4];  //�{
};

struct FBX_MATRIAL_ELEMENT
{
	enum MATERIAL_ELEMENT_TYPE
	{
		ELEMENT_NONE = 0,
		ELEMENT_COLOR,
		ELEMENT_TEXTURE,
		ELEMENT_BOTH,
		ELEMENT_MAX,
	};

	MATERIAL_ELEMENT_TYPE type;
	float r, g, b, a;
	TextureSet			textureSetArray;

	FBX_MATRIAL_ELEMENT() :r(), g(), b(), a()
	{
		type = ELEMENT_NONE;
		textureSetArray.clear();
	}

	~FBX_MATRIAL_ELEMENT()
	{
		Release();
	}

	void Release()
	{
		for (TextureSet::iterator it = textureSetArray.begin(); it != textureSetArray.end(); ++it)
		{
			it->second.clear();
		}

		textureSetArray.clear();
	}
};

struct FBX_MATERIAL_NODE
{
	// FBX�̃}�e���A����Lambert��Phong�����Ȃ�
	enum eMATERIAL_TYPE
	{
		MATERIAL_LAMBERT = 0,
		MATERIAL_PHONG,
	};

	eMATERIAL_TYPE type;
	FBX_MATRIAL_ELEMENT ambient;
	FBX_MATRIAL_ELEMENT diffuse;
	FBX_MATRIAL_ELEMENT emmisive;
	FBX_MATRIAL_ELEMENT specular;

	float shininess;
	float TransparencyFactor;		// ���ߓx

	int indexCount;
	vector<int> pIndex;

	FBX_MATERIAL_NODE() :ambient(), diffuse(), emmisive(), specular(), shininess(), TransparencyFactor()
	{
		type = MATERIAL_LAMBERT;
		indexCount = 0;
	}
};

// ���b�V���\���v�f
struct MESH_ELEMENTS
{
	unsigned int	numPosition;		// ���_���W�̃Z�b�g����������
	unsigned int	numNormal;			//
	unsigned int	numUVSet;			// UV�Z�b�g��

	MESH_ELEMENTS() :numPosition(), numNormal(), numUVSet()
	{

	}
};

//�P���_�̋��L�@�ő�20�|���S���܂�
struct POLY_TABLE
{
	int PolyIndex[20];//�|���S���ԍ� 
	int Index123[20];//3�̒��_�̂����A���Ԗڂ�
	int NumRef;//�����Ă���|���S����

	POLY_TABLE()
	{
		ZeroMemory(this, sizeof(POLY_TABLE));
	}
};

//
struct FBX_MESH_NODE
{
	std::string		name;			// �m�[�h��
	std::string		parentName;		// �e�m�[�h��(�e�����Ȃ��Ȃ�"null"�Ƃ������̂�����.root�m�[�h�̑Ή�)

	FbxMesh* m_pMesh;

	MESH_ELEMENTS	elements;		// ���b�V�����ێ�����f�[�^�\��
	std::vector<FBX_MATERIAL_NODE> m_materialArray;		// �}�e���A��
	UVsetID		uvsetID;

	std::vector<FBX_VERTEX_DATA>	m_vertexDataArray;
	std::vector<unsigned int>		m_indexArray;				// �C���f�b�N�X�z��
	std::vector<FbxCluster*>		m_boneArray;			// �{�[���z��

	DWORD faceCount;//���̃}�e���A���ł���|���S����
	DWORD UVCount;
	DWORD vertexCount;

	std::vector<POLY_TABLE> polyTable;

	float	mat4x4[16];	// Matrix

	bool anim;

	FBX_MESH_NODE() :name(), parentName(), elements(), faceCount(), UVCount(), vertexCount()
	{
		m_pMesh = nullptr;
		Release();
	}

	~FBX_MESH_NODE()
	{
		Release();
	}

	void Release()
	{
		polyTable.clear();
		uvsetID.clear();
		m_vertexDataArray.clear();
		m_materialArray.clear();
		m_indexArray.clear();
	}
};

class FBXLoader
{
public:
	
protected:
	unique_ptr<FBX> fbx;

	std::vector<FBX_MESH_NODE>		m_meshNodeArray;

	void SetupNode(FbxNode* pNode, std::string parentName);
	void Setup();

	void CopyVertexData(FbxMesh*	pMesh, FBX_MESH_NODE* meshNode, FbxDouble3 translation, FbxDouble3 scale, FbxDouble3 rotation);
	void CopyMatrialData(FbxSurfaceMaterial* mat, FBX_MATERIAL_NODE* destMat);
	void CopyBoneData(FbxMesh*	pMesh, FBX_MESH_NODE* meshNode);

	void ComputeNodeMatrix(FbxNode* pNode, FBX_MESH_NODE* meshNode);

	void SetFbxColor(FBX_MATRIAL_ELEMENT& destColor, const FbxDouble3 srcColor);
	FbxDouble3 GetMaterialProperty(
		const FbxSurfaceMaterial * pMaterial,
		const char * pPropertyName,
		const char * pFactorPropertyName,
		FBX_MATRIAL_ELEMENT*			pElement);

public:
	FBXLoader();
	~FBXLoader();

	void Release();

	// �ǂݍ���
	HRESULT LoadFBX(const char* filename);
	FbxNode&	GetRootNode();

	size_t GetNodesCount() { return m_meshNodeArray.size(); };		// �m�[�h���̎擾

	FBX_MESH_NODE&	GetNode(const unsigned int id);

	//void SetNewPoseMatrices(int frame);
};