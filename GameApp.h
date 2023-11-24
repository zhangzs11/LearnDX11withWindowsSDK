#ifndef GAMEAPP_H
#define GAMEAPP_H

#include "d3dApp.h"
#include "LightHelper.h"
#include "Geometry.h"

class GameApp : public D3DApp
{
public:
    //struct VertexPosColor
    //{
    //    DirectX::XMFLOAT3 pos;
    //    DirectX::XMFLOAT4 color;
    //    static const D3D11_INPUT_ELEMENT_DESC inputLayout[2];
    //};

    struct VSConstantBuffer
    {
        DirectX::XMMATRIX world;
        DirectX::XMMATRIX view;
        DirectX::XMMATRIX proj;
        DirectX::XMMATRIX worldInvTranspose;
    };

    struct PSConstantBuffer
    {
        DirectionalLight dirLight;
        PointLight pointLight;
        SpotLight spotLight;
        Material material;
        DirectX::XMFLOAT4 eyePos;
    };

public:
    GameApp(HINSTANCE hInstance, const std::wstring& windowName, int initWidth, int initHeight);
    ~GameApp();

    bool Init();
    void OnResize();
    void UpdateScene(float dt);
    void DrawScene();

private:
    bool InitEffect();      //��ɫ������Ч��س�ʼ��
    bool InitResource();    //��Դ��ʼ��
    bool ResetMesh(const Geometry::MeshData<VertexPosNormalColor>& meshData);

private:
    ComPtr<ID2D1SolidColorBrush> m_pColorBrush;	    // ��ɫ��ˢ
    ComPtr<IDWriteFont> m_pFont;					// ����
    ComPtr<IDWriteTextFormat> m_pTextFormat;		// �ı���ʽ

    ComPtr<ID3D11InputLayout> m_pVertexLayout;      //�������벼��
    ComPtr<ID3D11Buffer> m_pVertexBuffer;           //���㻺����
    ComPtr<ID3D11Buffer> m_pIndexBuffer;            //����������
    ComPtr<ID3D11Buffer> m_pConstantBuffers[2];      //����������
    UINT m_IndexCount;                              //������������������С


    ComPtr<ID3D11VertexShader> m_pVertexShader;     //������ɫ��
    ComPtr<ID3D11PixelShader> m_pPixelShader;       //������ɫ��
    VSConstantBuffer m_VSConstantBuffer;            //�����޸�����VS��GPU�����������ı���
    PSConstantBuffer m_PSConstantBuffer;            //�����޸�����PS��GPU�����������ı���

    DirectionalLight m_DirLight;                    //Ĭ�ϻ�����
    PointLight m_PointLight;                        //Ĭ�ϵ��
    SpotLight m_SpotLight;                          //Ĭ�ϻ�۹�

    ComPtr<ID3D11RasterizerState> m_pRSWireframe;   //��դ��״̬���߿�ģʽ
    bool m_IsWireframeMode;                         //��ǰ�Ƿ�Ϊ�߿�ģʽ
};


#endif