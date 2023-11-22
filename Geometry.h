//���ɳ����ļ���������ģ��
#ifndef GEOMETRY_H_
#define GEOMETRY_H_

#include <vector>
#include <string>
#include <map>
#include <functional>
#include "Vertex.h"

namespace Geometry
{
	template<class VertexType = VertexPosNormalTex, class IndexType = DWORD>
	struct MeshData
	{
		std::vector<VertexType> vertexVec; // ��������
		std::vector<IndexType> indexVec;   // ��������

		MeshData()
		{
			//�����������͵ĺϷ���
			static_assert(sizeof(IndexType) == 2 || sizeof(IndexType) == 4, "The size of IndexType must be 2 bytes or 4 bytes!");
			static_assert(std::is_unsigned<IndexType>::value, "IndexType must be unsigned integer!");
		}
	};

	//���������������ݣ�level��slicesԽ�󣬾���Խ��
	template<class VertexType = VertexPosNormalTex, class IndexType = DWORD>
	MeshData<VertexType, IndexType> CreateSphere(float radius = 1.0f, UINT levels = 20, UINT slices = 20, 
												 const DirectX::XMFLOAT4& color = { 1.0f, 1.0f, 1.0f, 1.0f });
	//������������������
	template<class VertexType = VertexPosNormalTex, class IndexType = DWORD>
	MeshData<VertexType, IndexType> CreateBox(float width = 2.0f, float height = 2.0f, float depth = 2.0f, 
											  const DirectX::XMFLOAT4& color = {1.0f, 1.0f, 1.0f, 1.0f});
	//����Բ������������
	template<class VertexType = VertexPosNormalTex, class IndexType = DWORD>
	MeshData<VertexType, IndexType> CreateCylinder(float radius = 1.0f, float height = 2.0f, UINT slices = 20, UINT stacks = 10,
												   float texU = 1.0f, float texV = 1.0f, const DirectX::XMFLOAT4& color = { 1.0f, 1.0f, 1.0f, 1.0f });
	//����ֻ��Բ����������������
	template<class VertexType = VertexPosNormalTex, class IndexType = DWORD>
	MeshData<VertexType, IndexType> CreateCylinderNoCap(float radius = 1.0f, float height = 2.0f, UINT slices = 20, UINT stacks = 10,
														float texU = 1.0f, float texV = 1.0f, const DirectX::XMFLOAT4& color = { 1.0f, 1.0f, 1.0f, 1.0f });
	//����Բ׶����������
	template<class VertexType = VertexPosNormalTex, class IndexType = DWORD>
	MeshData<VertexType, IndexType> CreateCone(float radius = 1.0f, float height = 2.0f, UINT slices = 20,
											   const DirectX::XMFLOAT4& color = { 1.0f, 1.0f, 1.0f, 1.0f });
	//����ֻ��Բ׶��������������
	template<class VertexType = VertexPosNormalTex, class IndexType = DWORD>
	MeshData<VertexType, IndexType> CreateConeNoCap(float radius = 1.0f, float height = 2.0f, UINT slices = 20,
													const DirectX::XMFLOAT4& color = { 1.0f, 1.0f, 1.0f, 1.0f });
	//����һ��ָ��NDC��Ļ������棬Ĭ��ȫ��
	template<class VertexType = VertexPosNormalTex, class IndexType = DWORD>
	MeshData<VertexType, IndexType> Create2DShow(const DirectX::XMFLOAT2& center, const DirectX::XMFLOAT2& scale, 
												 const DirectX::XMFLOAT4& color = { 1.0f, 1.0f, 1.0f, 1.0f });
	template<class VertexType = VertexPosNormalTex, class IndexType = DWORD>
	MeshData<VertexType, IndexType> Create2DShow(float centerX = 0.0f, float centerY = 0.0f, float scaleX = 1.0f, float scaleY = 1.0f, 
												 const DirectX::XMFLOAT4& color = { 1.0f, 1.0f, 1.0f, 1.0f });
	//����һ��ƽ��
	template<class VertexType = VertexPosNormalTex, class IndexType = DWORD>
	MeshData<VertexType, IndexType> CreatePlane(const DirectX::XMFLOAT2& planeSize, const DirectX::XMFLOAT2& maxTexCoord = { 1.0f, 1.0f },
												const DirectX::XMFLOAT4& color = { 1.0f, 1.0f, 1.0f, 1.0f });
	template<class VertexType = VertexPosNormalTex, class IndexType = DWORD>
	MeshData<VertexType, IndexType> CreatePlane(float width = 10.0f, float depth = 10.0f, float texU = 1.0f, float texV = 1.0f, 
												const DirectX::XMFLOAT4& color = { 1.0f, 1.0f, 1.0f, 1.0f });
	//����һ������
	template<class VertexType = VertexPosNormalTex, class IndexType = DWORD>
	MeshData<VertexType, IndexType> CreateTerrain(const DirectX::XMFLOAT2& terrainSize, 
		const DirectX::XMUINT2& slices = { 10,10 }, const DirectX::XMFLOAT2& maxTexCoord = {1.0f, 1.0f},
		const std::function<float(float, float)>& heightFunc = [](float x, float z) {return 0.0f; },
		const std::function<DirectX::XMFLOAT3(float, float)>& normalFunc = [](float x, float z) {return DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f); },
		const std::function<DirectX::XMFLOAT4(float, float)>& colorFunc = [](float x, float z) {return DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f); });
	template<class VertexType = VertexPosNormalTex, class IndexType = DWORD>
	MeshData<VertexType, IndexType> CreateTerrain(float width=10.0f, float depth=10.0f, 
		UINT slicesX=10, UINT slicesZ=10, float texU=1.0f, float texV=1.0f,
		const std::function<float(float, float)>& heightFunc = [](float x, float z) {return 0.0f; },
		const std::function<DirectX::XMFLOAT3(float, float)>& normalFunc = [](float x, float z) {return DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f); },
		const std::function<DirectX::XMFLOAT4(float, float)>& colorFunc = [](float x, float z) {return DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f); });
}



namespace Geometry
{
    namespace Internal
    {
        //
        // ���½ṹ��ͺ��������ڲ�ʵ��ʹ��
        //
        struct VertexData
        {
            DirectX::XMFLOAT3 pos;
            DirectX::XMFLOAT3 normal;
            DirectX::XMFLOAT4 tangent;
            DirectX::XMFLOAT4 color;
            DirectX::XMFLOAT2 tex;
        };

        // ����Ŀ�궥������ѡ���Խ����ݲ���
        template<class VertexType>
        inline void InsertVertexElement(VertexType& vertexDst, const VertexData& vertexSrc)
        {
            static std::string semanticName; // ������
            static const std::map < std::string, std::pair<size_t, size_t>> semanticSizeMap = {
                {"POSITION", std::pair<size_t, size_t>(0, 12)},
                {"NORMAL", std::pair<size_t, size_t>(12, 24)},
                {"TANGENT", std::pair<size_t, size_t>(24, 40)},
                {"COLOR", std::pair<size_t, size_t>(40, 56)},
                {"TEXCOORD", std::pair<size_t, size_t>(56, 64)},
            };

            for (size_t i = 0; i < ARRAYSIZE(VertexType::inputLayout); i++)
            {
                semanticName = VertexType::inputLayout[i].SemanticName;
                const auto& range = semanticSizeMap.at(semanticName);
                memcpy_s(reinterpret_cast<char*>(&vertexDst) + VertexType::inputLayout[i].AlignedByteOffset,
                    range.second - range.first,
                    reinterpret_cast<const char*>(&vertexSrc) + range.first,
                    range.second - range.first);
            }

        }
    }

    //
    // �����巽����ʵ��
    //


    //���������������ݣ�level��slicesԽ�󣬾���Խ��
    template<class VertexType, class IndexType>
    inline MeshData<VertexType, IndexType> CreateSphere(float radius, UINT levels, UINT slices, const DirectX::XMFLOAT4& color)
    {
        using namespace DirectX;

        MeshData<VertexType, IndexType> meshData;
        UINT vertexCount = 2 + (levels - 1) * (slices + 1);
        UINT indexCount = 6 * (levels - 1) * slices;
        meshData.vertexVec.resize(vertexCount);
        meshData.indexVec.resize(indexCount);

        Internal::VertexData vertexData;
        IndexType vIndex = 0, iIndex = 0;

        float phi = 0.0f, theta = 0.0f;
        float per_phi = XM_PI / levels;
        float per_theta = XM_2PI / slices;
        float x, y, z;

        // ���붥�˵�
        vertexData = { XMFLOAT3(0.0f, radius, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), color, XMFLOAT2(0.0f, 0.0f) };
        Internal::InsertVertexElement(meshData.vertexVec[vIndex++], vertexData);

        for (UINT i = 1; i < levels; ++i)
        {
            phi = per_phi * i;
            // ��Ҫslices + 1����������Ϊ �����յ���Ϊͬһ�㣬����������ֵ��һ��
            for (UINT j = 0; j <= slices; ++j)
            {
                theta = per_theta * j;
                x = radius * sinf(phi) * cosf(theta);
                y = radius * cosf(phi);
                z = radius * sinf(phi) * sinf(theta);
                // ������ֲ����ꡢ��������Tangent��������������
                XMFLOAT3 pos = XMFLOAT3(x, y, z), normal;
                XMStoreFloat3(&normal, XMVector3Normalize(XMLoadFloat3(&pos)));

                vertexData = { pos, normal, XMFLOAT4(-sinf(theta), 0.0f, cosf(theta), 1.0f), color, XMFLOAT2(theta / XM_2PI, phi / XM_PI) };
                Internal::InsertVertexElement(meshData.vertexVec[vIndex++], vertexData);
            }
        }

        // ����׶˵�
        vertexData = { XMFLOAT3(0.0f, -radius, 0.0f), XMFLOAT3(0.0f, -1.0f, 0.0f),
            XMFLOAT4(-1.0f, 0.0f, 0.0f, 1.0f), color, XMFLOAT2(0.0f, 1.0f) };
        Internal::InsertVertexElement(meshData.vertexVec[vIndex++], vertexData);


        // ��������
        if (levels > 1)
        {
            for (UINT j = 1; j <= slices; ++j)
            {
                meshData.indexVec[iIndex++] = 0;
                meshData.indexVec[iIndex++] = j % (slices + 1) + 1;
                meshData.indexVec[iIndex++] = j;
            }
        }


        for (UINT i = 1; i < levels - 1; ++i)
        {
            for (UINT j = 1; j <= slices; ++j)
            {
                meshData.indexVec[iIndex++] = (i - 1) * (slices + 1) + j;
                meshData.indexVec[iIndex++] = (i - 1) * (slices + 1) + j % (slices + 1) + 1;
                meshData.indexVec[iIndex++] = i * (slices + 1) + j % (slices + 1) + 1;

                meshData.indexVec[iIndex++] = i * (slices + 1) + j % (slices + 1) + 1;
                meshData.indexVec[iIndex++] = i * (slices + 1) + j;
                meshData.indexVec[iIndex++] = (i - 1) * (slices + 1) + j;
            }
        }

        // �𽥷�������
        if (levels > 1)
        {
            for (UINT j = 1; j <= slices; ++j)
            {
                meshData.indexVec[iIndex++] = (levels - 2) * (slices + 1) + j;
                meshData.indexVec[iIndex++] = (levels - 2) * (slices + 1) + j % (slices + 1) + 1;
                meshData.indexVec[iIndex++] = (levels - 1) * (slices + 1) + 1;
            }
        }


        return meshData;
    }

    //������������������
    template<class VertexType, class IndexType>
    inline MeshData<VertexType, IndexType> CreateBox(float width, float height, float depth, const DirectX::XMFLOAT4& color)
    {
        using namespace DirectX;

        MeshData<VertexType, IndexType> meshData;
        meshData.vertexVec.resize(24);


        Internal::VertexData vertexDataArr[24];
        float w2 = width / 2, h2 = height / 2, d2 = depth / 2;

        // ����(+X��)
        vertexDataArr[0].pos = XMFLOAT3(w2, -h2, -d2);
        vertexDataArr[1].pos = XMFLOAT3(w2, h2, -d2);
        vertexDataArr[2].pos = XMFLOAT3(w2, h2, d2);
        vertexDataArr[3].pos = XMFLOAT3(w2, -h2, d2);
        // ����(-X��)
        vertexDataArr[4].pos = XMFLOAT3(-w2, -h2, d2);
        vertexDataArr[5].pos = XMFLOAT3(-w2, h2, d2);
        vertexDataArr[6].pos = XMFLOAT3(-w2, h2, -d2);
        vertexDataArr[7].pos = XMFLOAT3(-w2, -h2, -d2);
        // ����(+Y��)
        vertexDataArr[8].pos = XMFLOAT3(-w2, h2, -d2);
        vertexDataArr[9].pos = XMFLOAT3(-w2, h2, d2);
        vertexDataArr[10].pos = XMFLOAT3(w2, h2, d2);
        vertexDataArr[11].pos = XMFLOAT3(w2, h2, -d2);
        // ����(-Y��)
        vertexDataArr[12].pos = XMFLOAT3(w2, -h2, -d2);
        vertexDataArr[13].pos = XMFLOAT3(w2, -h2, d2);
        vertexDataArr[14].pos = XMFLOAT3(-w2, -h2, d2);
        vertexDataArr[15].pos = XMFLOAT3(-w2, -h2, -d2);
        // ����(+Z��)
        vertexDataArr[16].pos = XMFLOAT3(w2, -h2, d2);
        vertexDataArr[17].pos = XMFLOAT3(w2, h2, d2);
        vertexDataArr[18].pos = XMFLOAT3(-w2, h2, d2);
        vertexDataArr[19].pos = XMFLOAT3(-w2, -h2, d2);
        // ����(-Z��)
        vertexDataArr[20].pos = XMFLOAT3(-w2, -h2, -d2);
        vertexDataArr[21].pos = XMFLOAT3(-w2, h2, -d2);
        vertexDataArr[22].pos = XMFLOAT3(w2, h2, -d2);
        vertexDataArr[23].pos = XMFLOAT3(w2, -h2, -d2);

        for (UINT i = 0; i < 4; ++i)
        {
            // ����(+X��)
            vertexDataArr[i].normal = XMFLOAT3(1.0f, 0.0f, 0.0f);
            vertexDataArr[i].tangent = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
            vertexDataArr[i].color = color;
            // ����(-X��)
            vertexDataArr[i + 4].normal = XMFLOAT3(-1.0f, 0.0f, 0.0f);
            vertexDataArr[i + 4].tangent = XMFLOAT4(0.0f, 0.0f, -1.0f, 1.0f);
            vertexDataArr[i + 4].color = color;
            // ����(+Y��)
            vertexDataArr[i + 8].normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
            vertexDataArr[i + 8].tangent = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
            vertexDataArr[i + 8].color = color;
            // ����(-Y��)
            vertexDataArr[i + 12].normal = XMFLOAT3(0.0f, -1.0f, 0.0f);
            vertexDataArr[i + 12].tangent = XMFLOAT4(-1.0f, 0.0f, 0.0f, 1.0f);
            vertexDataArr[i + 12].color = color;
            // ����(+Z��)
            vertexDataArr[i + 16].normal = XMFLOAT3(0.0f, 0.0f, 1.0f);
            vertexDataArr[i + 16].tangent = XMFLOAT4(-1.0f, 0.0f, 0.0f, 1.0f);
            vertexDataArr[i + 16].color = color;
            // ����(-Z��)
            vertexDataArr[i + 20].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
            vertexDataArr[i + 20].tangent = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
            vertexDataArr[i + 20].color = color;
        }

        for (UINT i = 0; i < 6; ++i)
        {
            vertexDataArr[i * 4].tex = XMFLOAT2(0.0f, 1.0f);
            vertexDataArr[i * 4 + 1].tex = XMFLOAT2(0.0f, 0.0f);
            vertexDataArr[i * 4 + 2].tex = XMFLOAT2(1.0f, 0.0f);
            vertexDataArr[i * 4 + 3].tex = XMFLOAT2(1.0f, 1.0f);
        }

        for (UINT i = 0; i < 24; ++i)
        {
            Internal::InsertVertexElement(meshData.vertexVec[i], vertexDataArr[i]);
        }

        meshData.indexVec = {
            0, 1, 2, 2, 3, 0,		// ����(+X��)
            4, 5, 6, 6, 7, 4,		// ����(-X��)
            8, 9, 10, 10, 11, 8,	// ����(+Y��)
            12, 13, 14, 14, 15, 12,	// ����(-Y��)
            16, 17, 18, 18, 19, 16, // ����(+Z��)
            20, 21, 22, 22, 23, 20	// ����(-Z��)
        };

        return meshData;
    }

    //����Բ�����������ݣ�slicesԽ�󣬾���Խ��
    template<class VertexType, class IndexType>
    inline MeshData<VertexType, IndexType> CreateCylinder(float radius, float height, UINT slices, UINT stacks,
        float texU, float texV, const DirectX::XMFLOAT4& color)
    {
        using namespace DirectX;

        auto meshData = CreateCylinderNoCap<VertexType, IndexType>(radius, height, slices, stacks, texU, texV, color);
        UINT vertexCount = (slices + 1) * (stacks + 3) + 2;
        UINT indexCount = 6 * slices * (stacks + 1);
        meshData.vertexVec.resize(vertexCount);
        meshData.indexVec.resize(indexCount);

        float h2 = height / 2;
        float theta = 0.0f;
        float per_theta = XM_2PI / slices;

        IndexType vIndex = (slices + 1) * (stacks + 1), iIndex = 6 * slices * stacks;
        IndexType offset = vIndex;
        Internal::VertexData vertexData;

        // ���붥��Բ��
        vertexData = { XMFLOAT3(0.0f, h2, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f),
            XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), color, XMFLOAT2(0.5f, 0.5f) };
        Internal::InsertVertexElement(meshData.vertexVec[vIndex++], vertexData);

        // ���붥��Բ�ϸ���
        for (UINT i = 0; i <= slices; ++i)
        {
            theta = i * per_theta;
            float u = cosf(theta) * radius / height + 0.5f;
            float v = sinf(theta) * radius / height + 0.5f;
            vertexData = { XMFLOAT3(radius * cosf(theta), h2, radius * sinf(theta)), XMFLOAT3(0.0f, 1.0f, 0.0f),
                XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), color, XMFLOAT2(u, v) };
            Internal::InsertVertexElement(meshData.vertexVec[vIndex++], vertexData);
        }

        // ����׶�Բ��
        vertexData = { XMFLOAT3(0.0f, -h2, 0.0f), XMFLOAT3(0.0f, -1.0f, 0.0f),
            XMFLOAT4(-1.0f, 0.0f, 0.0f, 1.0f), color, XMFLOAT2(0.5f, 0.5f) };
        Internal::InsertVertexElement(meshData.vertexVec[vIndex++], vertexData);

        // ����ײ�Բ�ϸ���
        for (UINT i = 0; i <= slices; ++i)
        {
            theta = i * per_theta;
            float u = cosf(theta) * radius / height + 0.5f;
            float v = sinf(theta) * radius / height + 0.5f;
            vertexData = { XMFLOAT3(radius * cosf(theta), -h2, radius * sinf(theta)), XMFLOAT3(0.0f, -1.0f, 0.0f),
                XMFLOAT4(-1.0f, 0.0f, 0.0f, 1.0f), color, XMFLOAT2(u, v) };
            Internal::InsertVertexElement(meshData.vertexVec[vIndex++], vertexData);
        }



        // ���붥������������
        for (UINT i = 1; i <= slices; ++i)
        {
            meshData.indexVec[iIndex++] = offset;
            meshData.indexVec[iIndex++] = offset + i % (slices + 1) + 1;
            meshData.indexVec[iIndex++] = offset + i;
        }

        // ����ײ�����������
        offset += slices + 2;
        for (UINT i = 1; i <= slices; ++i)
        {
            meshData.indexVec[iIndex++] = offset;
            meshData.indexVec[iIndex++] = offset + i;
            meshData.indexVec[iIndex++] = offset + i % (slices + 1) + 1;
        }

        return meshData;
    }

    //����ֻ��Բ���������������ݣ�sliceԽ�󣬾���Խ��
    template<class VertexType, class IndexType>
    inline MeshData<VertexType, IndexType> CreateCylinderNoCap(float radius, float height, UINT slices, UINT stacks,
        float texU, float texV, const DirectX::XMFLOAT4& color)
    {
        using namespace DirectX;

        MeshData<VertexType, IndexType> meshData;
        UINT vertexCount = (slices + 1) * (stacks + 1);
        UINT indexCount = 6 * slices * stacks;
        meshData.vertexVec.resize(vertexCount);
        meshData.indexVec.resize(indexCount);

        float h2 = height / 2;
        float theta = 0.0f;
        float per_theta = XM_2PI / slices;
        float stackHeight = height / stacks;

        Internal::VertexData vertexData;

        // �Ե������������˵�
        UINT vIndex = 0;
        for (UINT i = 0; i < stacks + 1; ++i)
        {
            float y = -h2 + i * stackHeight;
            // ��ǰ�㶥��
            for (UINT j = 0; j <= slices; ++j)
            {
                theta = j * per_theta;
                float u = theta / XM_2PI;
                float v = 1.0f - (float)i / stacks;
                vertexData = { XMFLOAT3(radius * cosf(theta), y, radius * sinf(theta)), XMFLOAT3(cosf(theta), 0.0f, sinf(theta)),
                    XMFLOAT4(-sinf(theta), 0.0f, cosf(theta), 1.0f), color, XMFLOAT2(u * texU, v * texV) };
                Internal::InsertVertexElement(meshData.vertexVec[vIndex++], vertexData);
            }
        }

        // ��������
        UINT iIndex = 0;
        for (UINT i = 0; i < stacks; ++i)
        {
            for (UINT j = 0; j < slices; ++j)
            {
                meshData.indexVec[iIndex++] = i * (slices + 1) + j;
                meshData.indexVec[iIndex++] = (i + 1) * (slices + 1) + j;
                meshData.indexVec[iIndex++] = (i + 1) * (slices + 1) + j + 1;

                meshData.indexVec[iIndex++] = i * (slices + 1) + j;
                meshData.indexVec[iIndex++] = (i + 1) * (slices + 1) + j + 1;
                meshData.indexVec[iIndex++] = i * (slices + 1) + j + 1;
            }
        }



        return meshData;
    }


    //����Բ׶���������ݣ�sliceԽ�󣬾���Խ��
    template<class VertexType, class IndexType>
    MeshData<VertexType, IndexType> CreateCone(float radius, float height, UINT slices, const DirectX::XMFLOAT4& color)
    {
        using namespace DirectX;
        auto meshData = CreateConeNoCap<VertexType, IndexType>(radius, height, slices, color);

        UINT vertexCount = 3 * slices + 1;
        UINT indexCount = 6 * slices;
        meshData.vertexVec.resize(vertexCount);
        meshData.indexVec.resize(indexCount);

        float h2 = height / 2;
        float theta = 0.0f;
        float per_theta = XM_2PI / slices;
        UINT iIndex = 3 * slices;
        UINT vIndex = 2 * slices;
        Internal::VertexData vertexData;

        // ����Բ׶���涥��
        for (UINT i = 0; i < slices; ++i)
        {
            theta = i * per_theta;
            vertexData = { XMFLOAT3(radius * cosf(theta), -h2, radius * sinf(theta)), XMFLOAT3(0.0f, -1.0f, 0.0f),
                XMFLOAT4(-1.0f, 0.0f, 0.0f, 1.0f), color, XMFLOAT2(cosf(theta) / 2 + 0.5f, sinf(theta) / 2 + 0.5f) };
            Internal::InsertVertexElement(meshData.vertexVec[vIndex++], vertexData);
        }
        // ����Բ׶����Բ��
        vertexData = { XMFLOAT3(0.0f, -h2, 0.0f), XMFLOAT3(0.0f, -1.0f, 0.0f),
                XMFLOAT4(-1.0f, 0.0f, 0.0f, 1.0f), color, XMFLOAT2(0.5f, 0.5f) };
        Internal::InsertVertexElement(meshData.vertexVec[vIndex++], vertexData);

        // ��������
        UINT offset = 2 * slices;
        for (UINT i = 0; i < slices; ++i)
        {
            meshData.indexVec[iIndex++] = offset + slices;
            meshData.indexVec[iIndex++] = offset + i % slices;
            meshData.indexVec[iIndex++] = offset + (i + 1) % slices;
        }

        return meshData;
    }

    //����ֻ��Բ׶������������ݣ�sliceԽ�󣬾���Խ��
    template<class VertexType, class IndexType>
    MeshData<VertexType, IndexType> CreateConeNoCap(float radius, float height, UINT slices, const DirectX::XMFLOAT4& color)
    {
        using namespace DirectX;

        MeshData<VertexType, IndexType> meshData;
        UINT vertexCount = 2 * slices;
        UINT indexCount = 3 * slices;
        meshData.vertexVec.resize(vertexCount);
        meshData.indexVec.resize(indexCount);

        float h2 = height / 2;
        float theta = 0.0f;
        float per_theta = XM_2PI / slices;
        float len = sqrtf(height * height + radius * radius);
        UINT iIndex = 0;
        UINT vIndex = 0;
        Internal::VertexData vertexData;

        // ����Բ׶��˶���(ÿ������λ����ͬ����������ͬ�ķ���������������)
        for (UINT i = 0; i < slices; ++i)
        {
            theta = i * per_theta + per_theta / 2;
            vertexData = { XMFLOAT3(0.0f, h2, 0.0f), XMFLOAT3(radius * cosf(theta) / len, height / len, radius * sinf(theta) / len),
                XMFLOAT4(-sinf(theta), 0.0f, cosf(theta), 1.0f), color, XMFLOAT2(0.5f, 0.5f) };
            Internal::InsertVertexElement(meshData.vertexVec[vIndex++], vertexData);
        }

        // ����Բ׶����ײ�����
        for (UINT i = 0; i < slices; ++i)
        {
            theta = i * per_theta;
            vertexData = { XMFLOAT3(radius * cosf(theta), -h2, radius * sinf(theta)), XMFLOAT3(radius * cosf(theta) / len, height / len, radius * sinf(theta) / len),
                XMFLOAT4(-sinf(theta), 0.0f, cosf(theta), 1.0f), color, XMFLOAT2(cosf(theta) / 2 + 0.5f, sinf(theta) / 2 + 0.5f) };
            Internal::InsertVertexElement(meshData.vertexVec[vIndex++], vertexData);
        }

        // ��������
        for (UINT i = 0; i < slices; ++i)
        {
            meshData.indexVec[iIndex++] = i;
            meshData.indexVec[iIndex++] = slices + (i + 1) % slices;
            meshData.indexVec[iIndex++] = slices + i % slices;
        }

        return meshData;
    }

    //����һ��ָ��NDC��Ļ������棨Ĭ��ȫ����
    template<class VertexType, class IndexType>
    inline MeshData<VertexType, IndexType> Create2DShow(const DirectX::XMFLOAT2& center, const DirectX::XMFLOAT2& scale, const DirectX::XMFLOAT4& color)
    {
        return Create2DShow<VertexType, IndexType>(center.x, center.y, scale.x, scale.y, color);
    }

    template<class VertexType, class IndexType>
    inline MeshData<VertexType, IndexType> Create2DShow(float centerX, float centerY, float scaleX, float scaleY, const DirectX::XMFLOAT4& color)
    {
        using namespace DirectX;

        MeshData<VertexType, IndexType> meshData;
        meshData.vertexVec.resize(4);

        Internal::VertexData vertexData;
        UINT vIndex = 0;

        vertexData = { XMFLOAT3(centerX - scaleX, centerY - scaleY, 0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f),
            XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), color, XMFLOAT2(0.0f, 1.0f) };
        Internal::InsertVertexElement(meshData.vertexVec[vIndex++], vertexData);

        vertexData = { XMFLOAT3(centerX - scaleX, centerY + scaleY, 0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f),
            XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), color, XMFLOAT2(0.0f, 0.0f) };
        Internal::InsertVertexElement(meshData.vertexVec[vIndex++], vertexData);

        vertexData = { XMFLOAT3(centerX + scaleX, centerY + scaleY, 0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f),
            XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), color, XMFLOAT2(1.0f, 0.0f) };
        Internal::InsertVertexElement(meshData.vertexVec[vIndex++], vertexData);

        vertexData = { XMFLOAT3(centerX + scaleX, centerY - scaleY, 0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f),
            XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), color, XMFLOAT2(1.0f, 1.0f) };
        Internal::InsertVertexElement(meshData.vertexVec[vIndex++], vertexData);

        meshData.indexVec = { 0, 1, 2, 2, 3, 0 };
        return meshData;
    }

    //����һ��ƽ��
    template<class VertexType, class IndexType>
    inline MeshData<VertexType, IndexType> CreatePlane(const DirectX::XMFLOAT2& planeSize,
        const DirectX::XMFLOAT2& maxTexCoord, const DirectX::XMFLOAT4& color)
    {
        return CreatePlane<VertexType, IndexType>(planeSize.x, planeSize.y, maxTexCoord.x, maxTexCoord.y, color);
    }

    template<class VertexType, class IndexType>
    inline MeshData<VertexType, IndexType> CreatePlane(float width, float depth, float texU, float texV, const DirectX::XMFLOAT4& color)
    {
        using namespace DirectX;

        MeshData<VertexType, IndexType> meshData;
        meshData.vertexVec.resize(4);

        Internal::VertexData vertexData;
        UINT vIndex = 0;

        vertexData = { XMFLOAT3(-width / 2, 0.0f, -depth / 2), XMFLOAT3(0.0f, 1.0f, 0.0f),
            XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), color, XMFLOAT2(0.0f, texV) };
        Internal::InsertVertexElement(meshData.vertexVec[vIndex++], vertexData);

        vertexData = { XMFLOAT3(-width / 2, 0.0f, depth / 2), XMFLOAT3(0.0f, 1.0f, 0.0f),
            XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), color, XMFLOAT2(0.0f, 0.0f) };
        Internal::InsertVertexElement(meshData.vertexVec[vIndex++], vertexData);

        vertexData = { XMFLOAT3(width / 2, 0.0f, depth / 2), XMFLOAT3(0.0f, 1.0f, 0.0f),
            XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), color, XMFLOAT2(texU, 0.0f) };
        Internal::InsertVertexElement(meshData.vertexVec[vIndex++], vertexData);

        vertexData = { XMFLOAT3(width / 2, 0.0f, -depth / 2), XMFLOAT3(0.0f, 1.0f, 0.0f),
            XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), color, XMFLOAT2(texU, texV) };
        Internal::InsertVertexElement(meshData.vertexVec[vIndex++], vertexData);

        meshData.indexVec = { 0, 1, 2, 2, 3, 0 };
        return meshData;
    }

    //����һ������
    template<class VertexType, class IndexType>
    MeshData<VertexType, IndexType> CreateTerrain(const DirectX::XMFLOAT2& terrainSize, const DirectX::XMUINT2& slices,
        const DirectX::XMFLOAT2& maxTexCoord, const std::function<float(float, float)>& heightFunc,
        const std::function<DirectX::XMFLOAT3(float, float)>& normalFunc,
        const std::function<DirectX::XMFLOAT4(float, float)>& colorFunc)
    {
        return CreateTerrain<VertexType, IndexType>(terrainSize.x, terrainSize.y, slices.x, slices.y,
            maxTexCoord.x, maxTexCoord.y, heightFunc, normalFunc, colorFunc);
    }

    template<class VertexType, class IndexType>
    MeshData<VertexType, IndexType> CreateTerrain(float width, float depth, UINT slicesX, UINT slicesZ,
        float texU, float texV, const std::function<float(float, float)>& heightFunc,
        const std::function<DirectX::XMFLOAT3(float, float)>& normalFunc,
        const std::function<DirectX::XMFLOAT4(float, float)>& colorFunc)
    {
        using namespace DirectX;

        MeshData<VertexType, IndexType> meshData;
        UINT vertexCount = (slicesX + 1) * (slicesZ + 1);
        UINT indexCount = 6 * slicesX * slicesZ;
        meshData.vertexVec.resize(vertexCount);
        meshData.indexVec.resize(indexCount);

        Internal::VertexData vertexData;
        UINT vIndex = 0;
        UINT iIndex = 0;

        float sliceWidth = width / slicesX;
        float sliceDepth = depth / slicesZ;
        float leftBottomX = -width / 2;
        float leftBottomZ = -depth / 2;
        float posX, posZ;
        float sliceTexWidth = texU / slicesX;
        float sliceTexDepth = texV / slicesZ;

        XMFLOAT3 normal;
        XMFLOAT4 tangent;
        // �������񶥵�
        //  __ __
        // | /| /|
        // |/_|/_|
        // | /| /| 
        // |/_|/_|
        for (UINT z = 0; z <= slicesZ; ++z)
        {
            posZ = leftBottomZ + z * sliceDepth;
            for (UINT x = 0; x <= slicesX; ++x)
            {
                posX = leftBottomX + x * sliceWidth;
                // ���㷨��������һ��
                normal = normalFunc(posX, posZ);
                XMStoreFloat3(&normal, XMVector3Normalize(XMLoadFloat3(&normal)));
                // ���㷨ƽ����z=posZƽ�湹�ɵ�ֱ�ߵ�λ��������ά��w����Ϊ1.0f
                XMStoreFloat4(&tangent, XMVector3Normalize(XMVectorSet(normal.y, -normal.x, 0.0f, 0.0f)) + g_XMIdentityR3);

                vertexData = { XMFLOAT3(posX, heightFunc(posX, posZ), posZ),
                    normal, tangent, colorFunc(posX, posZ), XMFLOAT2(x * sliceTexWidth, texV - z * sliceTexDepth) };
                Internal::InsertVertexElement(meshData.vertexVec[vIndex++], vertexData);
            }
        }
        // ��������
        for (UINT i = 0; i < slicesZ; ++i)
        {
            for (UINT j = 0; j < slicesX; ++j)
            {
                meshData.indexVec[iIndex++] = i * (slicesX + 1) + j;
                meshData.indexVec[iIndex++] = (i + 1) * (slicesX + 1) + j;
                meshData.indexVec[iIndex++] = (i + 1) * (slicesX + 1) + j + 1;

                meshData.indexVec[iIndex++] = (i + 1) * (slicesX + 1) + j + 1;
                meshData.indexVec[iIndex++] = i * (slicesX + 1) + j + 1;
                meshData.indexVec[iIndex++] = i * (slicesX + 1) + j;
            }
        }

        return meshData;
    }
}


#endif 

