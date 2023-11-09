#ifndef GAMEAPP_H
#define GAMEAPP_H

#include "d3dApp.h"
class GameApp : public D3DApp
{
public:
    struct VertexPosColor
    {
        DirectX::XMFLOAT3 pos;
        DirectX::XMFLOAT4 color;
        static const D3D11_INPUT_ELEMENT_DESC inputLayout[2];
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

private:
    ComPtr<ID3D11InputLayout> m_pVertexLayout;      //�������벼��
    ComPtr<ID3D11Buffer> m_pVertexBuffer;           //���㻺����
    ComPtr<ID3D11VertexShader> m_pVertexShader;     //������ɫ��
    ComPtr<ID3D11PixelShader> m_pPixelShader;       //������ɫ��
};


#endif