#include "GameApp.h"
#include "d3dUtil.h"
#include "DXTrace.h"
using namespace DirectX;

const D3D11_INPUT_ELEMENT_DESC GameApp::VertexPosColor::inputLayout[2] = {
    {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
    {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
};

GameApp::GameApp(HINSTANCE hInstance, const std::wstring& windowName, int initWidth, int initHeight)
    : D3DApp(hInstance, windowName, initWidth, initHeight), m_CBuffer()
{
}

GameApp::~GameApp()
{
}

bool GameApp::Init()
{
    if (!D3DApp::Init())
        return false;
    if (!InitEffect())
        return false;
    if (!InitResource())
        return false;

    return true;
}

void GameApp::OnResize()
{
    D3DApp::OnResize();
}

void GameApp::UpdateScene(float dt)
{
    // ImGui�ڲ�ʾ������
    //ImGui::ShowAboutWindow();
    //ImGui::ShowDemoWindow();
    //ImGui::ShowUserGuide();

    //��ȡIO�¼�
    ImGuiIO& io = ImGui::GetIO();
    
    //
    // �Զ��崰�������
    //
    static float tx = 0.0f, ty = 0.0f, phi = 0.0f, theta = 0.0f, scale = 1.0f, fov = XM_PIDIV2;
    static bool animateCube = true, customColor = false;
    if (animateCube)
    {
        phi += 0.3f * dt, theta += 0.37f * dt;
        phi = XMScalarModAngle(phi);
        theta = XMScalarModAngle(theta);
    }
    if (ImGui::Begin("Use ImGui"))
    {
        ImGui::Checkbox("Animate Cube", &animateCube);   // ��ѡ��
        ImGui::SameLine(0.0f, 25.0f);                    // ��һ���ؼ���ͬһ������25���ص�λ
        if (ImGui::Button("Reset Params"))               // ��ť
        {
            tx = ty = phi = theta = 0.0f;
            scale = 1.0f;
            fov = XM_PIDIV2;
        }
        ImGui::SliderFloat("Scale", &scale, 0.2f, 2.0f);  // �϶����������С

        ImGui::Text("Phi: %.2f degrees", XMConvertToDegrees(phi));     // ��ʾ���֣�������������Ŀؼ� 
        ImGui::SliderFloat("##1", &phi, -XM_PI, XM_PI, "");            // ����ʾ�ؼ����⣬��ʹ��##�������ǩ�ظ�
        // ���ַ���������ʾ����
        ImGui::Text("Theta: %.2f degrees", XMConvertToDegrees(theta));
        // ��һ��д����ImGui::PushID(2);
        // �������##2ɾȥ
        ImGui::SliderFloat("##2", &theta, -XM_PI, XM_PI, "");
        // Ȼ�����ImGui::PopID(2);

        ImGui::Text("Position: (%.1f, %.1f, 0.0)", tx, ty);

        ImGui::Text("FOV: %.2f degrees", XMConvertToDegrees(fov));
        ImGui::SliderFloat("##3", &fov, XM_PIDIV4, XM_PI / 3 * 2, "");

        if (ImGui::Checkbox("Use Custom Color", &customColor))
            m_CBuffer.useCustomColor = customColor;
        // ����Ŀؼ�������ĸ�ѡ��Ӱ��
        if (customColor)
        {
            ImGui::ColorEdit3("Color", reinterpret_cast<float*>(&m_CBuffer.color));  // �༭��ɫ
        }
    }
    ImGui::End();

    //�������ڲ���UIʱ��������
    if (!ImGui::IsAnyItemActive())
    {
        // �������϶�ƽ��
        if (ImGui::IsMouseDragging(ImGuiMouseButton_Left))
        {
            tx += io.MouseDelta.x * 0.01f;
            ty -= io.MouseDelta.y * 0.01f;
        }
        // ����Ҽ��϶���ת
        else if (ImGui::IsMouseDragging(ImGuiMouseButton_Right))
        {
            phi -= io.MouseDelta.y * 0.01f;
            theta -= io.MouseDelta.x * 0.01f;
            phi = XMScalarModAngle(phi);
            theta = XMScalarModAngle(theta);
        }
        // ����������
        else if (io.MouseWheel != 0.0f)
        {
            scale += 0.02f * io.MouseWheel;
            if (scale > 2.0f)
                scale = 2.0f;
            else if (scale < 0.2f)
                scale = 0.2f;
        }
    }
    m_CBuffer.world = XMMatrixTranspose(
        XMMatrixScalingFromVector(XMVectorReplicate(scale)) *
        XMMatrixRotationX(phi) * XMMatrixRotationY(theta) *
        XMMatrixTranslation(tx, ty, 0.0f));
    m_CBuffer.proj = XMMatrixTranspose(XMMatrixPerspectiveFovLH(fov, AspectRatio(), 1.0f, 1000.0f));
    // ���³���������
    D3D11_MAPPED_SUBRESOURCE mappedData;
    HR(m_pd3dImmediateContext->Map(m_pConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));
    memcpy_s(mappedData.pData, sizeof(m_CBuffer), &m_CBuffer, sizeof(m_CBuffer));
    m_pd3dImmediateContext->Unmap(m_pConstantBuffer.Get(), 0);

}

void GameApp::DrawScene()
{
    assert(m_pd3dImmediateContext);
    assert(m_pSwapChain);
    static float blue[4] = { 0.0f, 0.0f, 1.0f, 1.0f };  // RGBA = (0,0,255,255)
    m_pd3dImmediateContext->ClearRenderTargetView(m_pRenderTargetView.Get(), blue);
    m_pd3dImmediateContext->ClearDepthStencilView(m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    //����������
    //m_pd3dImmediateContext->Draw(3, 0);
    //HR(m_pSwapChain->Present(0, 0));

    //����������
    m_pd3dImmediateContext->DrawIndexed(36, 0, 0);

    ImGui::Render();
    // ������仰�ᴥ��ImGui��Direct3D�Ļ���
    // �����Ҫ�ڴ�֮ǰ���󱸻������󶨵���Ⱦ������
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    HR(m_pSwapChain->Present(0, 0));
}

bool GameApp::InitEffect()
{
    ComPtr<ID3DBlob> blob;

    // ����������ɫ��
    HR(CreateShaderFromFile(L"Cube_VS.cso", L"Cube_VS.hlsl", "main", "vs_5_0", blob.ReleaseAndGetAddressOf()));
    HR(m_pd3dDevice->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, m_pVertexShader.GetAddressOf()));
    //  �������󶨶��㲼��
    HR(m_pd3dDevice->CreateInputLayout(VertexPosColor::inputLayout, ARRAYSIZE(VertexPosColor::inputLayout),
        blob->GetBufferPointer(), blob->GetBufferSize(), m_pVertexLayout.GetAddressOf()));
    //����������ɫ��
    HR(CreateShaderFromFile(L"Cube_PS.cso", L"Cube_PS.hlsl", "main", "ps_5_0", blob.ReleaseAndGetAddressOf()));
    HR(m_pd3dDevice->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, m_pPixelShader.GetAddressOf()));

    return true;
}

bool GameApp::InitResource()
{
    // ���������嶥��
    VertexPosColor vertices[] =
    {
        { XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) },
        { XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
        { XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f) },
        { XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
        { XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
        { XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f) },
        { XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
        { XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f) },
    };
    // ���ö��㻺��������
    D3D11_BUFFER_DESC vbd;
    ZeroMemory(&vbd, sizeof(vbd));
    vbd.Usage = D3D11_USAGE_IMMUTABLE;
    vbd.ByteWidth = sizeof vertices;
    vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vbd.CPUAccessFlags = 0;
    // �½����㻺����
    D3D11_SUBRESOURCE_DATA InitData;
    ZeroMemory(&InitData, sizeof(InitData));
    InitData.pSysMem = vertices;
    HR(m_pd3dDevice->CreateBuffer(&vbd, &InitData, m_pVertexBuffer.GetAddressOf()));

    // ******************
    // ��������
    //
    DWORD indices[] = {
        //front
        0, 1, 2,
        2, 3, 0,
        //left
        4, 5, 1,
        1, 0, 4,
        //top
        1, 5, 6,
        6, 2, 1,
        //rear
        7, 6, 5,
        5, 4, 7,
        //right
        3, 2, 6,
        6, 7, 3,
        //bottom
        4, 0, 3,
        3, 7, 4
    };
    //������������������
    D3D11_BUFFER_DESC ibd;
    ZeroMemory(&ibd, sizeof(ibd));
    ibd.Usage = D3D11_USAGE_IMMUTABLE;
    ibd.ByteWidth = sizeof indices;
    ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    ibd.CPUAccessFlags = 0;
    //��������������
    InitData.pSysMem = indices;
    HR(m_pd3dDevice->CreateBuffer(&ibd, &InitData, m_pIndexBuffer.GetAddressOf()));
    //����װ��׶ε���������������
    m_pd3dImmediateContext->IASetIndexBuffer(m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

    //**********************
    // ���ó�������������
    // 
    // 
    D3D11_BUFFER_DESC cbd;
    ZeroMemory(&cbd, sizeof(cbd));
    cbd.Usage = D3D11_USAGE_DYNAMIC;
    cbd.ByteWidth = sizeof(ConstantBuffer);
    cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    //�½���������������ʹ�ó�ʼ����
    HR(m_pd3dDevice->CreateBuffer(&cbd, nullptr, m_pConstantBuffer.GetAddressOf()));


    //��ʼ��������������ֵ
    m_CBuffer.world = XMMatrixIdentity();
    m_CBuffer.view = XMMatrixTranspose(XMMatrixLookAtLH(
        XMVectorSet(0.0f, 0.0f, -5.0f, 0.0f),
        XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
        XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
    ));
    m_CBuffer.proj = XMMatrixTranspose(XMMatrixPerspectiveFovLH(XM_PIDIV2, AspectRatio(), 1.0f, 1000.0f));

    // ******************
    // ����Ⱦ���߸����׶ΰ󶨺�������Դ
    //

    // ����װ��׶εĶ��㻺��������
    UINT stride = sizeof(VertexPosColor);	// ��Խ�ֽ���
    UINT offset = 0;						// ��ʼƫ����

    m_pd3dImmediateContext->IASetVertexBuffers(0, 1, m_pVertexBuffer.GetAddressOf(), &stride, &offset);
    // ����ͼԪ���ͣ��趨���벼��
    m_pd3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    m_pd3dImmediateContext->IASetInputLayout(m_pVertexLayout.Get());
    // ����ɫ���󶨵���Ⱦ����
    m_pd3dImmediateContext->VSSetShader(m_pVertexShader.Get(), nullptr, 0);
    // �����ºõĳ����������󶨵�������ɫ����������ɫ��
    m_pd3dImmediateContext->VSSetConstantBuffers(0, 1, m_pConstantBuffer.GetAddressOf());
    m_pd3dImmediateContext->PSSetConstantBuffers(0, 1, m_pConstantBuffer.GetAddressOf());

    m_pd3dImmediateContext->PSSetShader(m_pPixelShader.Get(), nullptr, 0);

    // ******************
    // ���õ��Զ�����
    //
    D3D11SetDebugObjectName(m_pVertexLayout.Get(), "VertexPosColorLayout");
    D3D11SetDebugObjectName(m_pVertexBuffer.Get(), "VertexBuffer");
    D3D11SetDebugObjectName(m_pIndexBuffer.Get(), "IndexBuffer");
    D3D11SetDebugObjectName(m_pConstantBuffer.Get(), "ConstantBuffer");
    D3D11SetDebugObjectName(m_pVertexShader.Get(), "Cube_VS");
    D3D11SetDebugObjectName(m_pPixelShader.Get(), "Cube_PS");



    return true;
}