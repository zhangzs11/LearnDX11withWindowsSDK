﻿#include "GameApp.h"
#include "d3dUtil.h"
#include "DXTrace.h"
using namespace DirectX;



GameApp::GameApp(HINSTANCE hInstance, const std::wstring& windowName, int initWidth, int initHeight)
    : D3DApp(hInstance, windowName, initWidth, initHeight),
    m_CameraMode(CameraMode::FirstPerson),
    m_CBFrame(),
    m_CBOnResize(),
    m_CBRarely()
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

    // 摄像机变更显示
    if (m_pCamera != nullptr)
    {
        m_pCamera->SetFrustum(XM_PI / 3, AspectRatio(), 0.5f, 1000.0f);
        m_pCamera->SetViewPort(0.0f, 0.0f, (float)m_ClientWidth, (float)m_ClientHeight);
        m_CBOnResize.proj = XMMatrixTranspose(m_pCamera->GetProjXM());

        D3D11_MAPPED_SUBRESOURCE mappedData;
        HR(m_pd3dImmediateContext->Map(m_pConstantBuffers[2].Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));
        memcpy_s(mappedData.pData, sizeof(CBChangesOnResize), &m_CBOnResize, sizeof(CBChangesOnResize));
        m_pd3dImmediateContext->Unmap(m_pConstantBuffers[2].Get(), 0);
    }
}

void GameApp::UpdateScene(float dt)
{
    // 获取子类
    auto cam1st = std::dynamic_pointer_cast<FirstPersonCamera>(m_pCamera);
    auto cam3rd = std::dynamic_pointer_cast<ThirdPersonCamera>(m_pCamera);

    Transform& woodCrateTransform = m_WoodCrate.GetTransform();

    ImGuiIO& io = ImGui::GetIO();
    if (m_CameraMode == CameraMode::FirstPerson || m_CameraMode == CameraMode::Free)
    {
        // 第一人称/自由摄像机的操作
        float d1 = 0.0f, d2 = 0.0f;
        if (ImGui::IsKeyDown(ImGuiKey_W))
            d1 += dt;
        if (ImGui::IsKeyDown(ImGuiKey_S))
            d1 -= dt;
        if (ImGui::IsKeyDown(ImGuiKey_A))
            d2 -= dt;
        if (ImGui::IsKeyDown(ImGuiKey_D))
            d2 += dt;

        if (m_CameraMode == CameraMode::FirstPerson)
            cam1st->Walk(d1 * 6.0f);
        else
            cam1st->MoveForward(d1 * 6.0f);
        cam1st->Strafe(d2 * 6.0f);

        // 将摄像机位置限制在[-8.9, 8.9]x[-8.9, 8.9]x[0.0, 8.9]的区域内
        // 不允许穿地
        XMFLOAT3 adjustedPos;
        XMStoreFloat3(&adjustedPos, XMVectorClamp(cam1st->GetPositionXM(), XMVectorSet(-8.9f, 0.0f, -8.9f, 0.0f), XMVectorReplicate(8.9f)));
        cam1st->SetPosition(adjustedPos);

        // 仅在第一人称模式移动摄像机的同时移动箱子
        if (m_CameraMode == CameraMode::FirstPerson)
            woodCrateTransform.SetPosition(adjustedPos);

        if (ImGui::IsMouseDragging(ImGuiMouseButton_Right))
        {
            cam1st->Pitch(io.MouseDelta.y * 0.01f);
            cam1st->RotateY(io.MouseDelta.x * 0.01f);
        }
    }
    else if (m_CameraMode == CameraMode::ThirdPerson)
    {
        // 第三人称摄像机的操作
        cam3rd->SetTarget(woodCrateTransform.GetPosition());

        // 绕物体旋转
        if (ImGui::IsMouseDragging(ImGuiMouseButton_Right))
        {
            cam3rd->RotateX(io.MouseDelta.y * 0.01f);
            cam3rd->RotateY(io.MouseDelta.x * 0.01f);
        }
        cam3rd->Approach(-io.MouseWheel * 1.0f);
    }

    // 更新观察矩阵
    XMStoreFloat4(&m_CBFrame.eyePos, m_pCamera->GetPositionXM());
    m_CBFrame.view = XMMatrixTranspose(m_pCamera->GetViewXM());

    if (ImGui::Begin("Camera"))
    {
        ImGui::Text("W/S/A/D in FPS/Free camera");
        ImGui::Text("Hold the right mouse button and drag the view");
        ImGui::Text("The box moves only at First Person mode");

        static int curr_item = 0;
        static const char* modes[] = {
            "First Person",
            "Third Person",
            "Free Camera"
        };
        if (ImGui::Combo("Camera Mode", &curr_item, modes, ARRAYSIZE(modes)))
        {
            if (curr_item == 0 && m_CameraMode != CameraMode::FirstPerson)
            {
                if (!cam1st)
                {
                    cam1st = std::make_shared<FirstPersonCamera>();
                    cam1st->SetFrustum(XM_PI / 3, AspectRatio(), 0.5f, 1000.0f);
                    m_pCamera = cam1st;
                }

                cam1st->LookTo(woodCrateTransform.GetPosition(),
                    XMFLOAT3(0.0f, 0.0f, 1.0f),
                    XMFLOAT3(0.0f, 1.0f, 0.0f));

                m_CameraMode = CameraMode::FirstPerson;
            }
            else if (curr_item == 1 && m_CameraMode != CameraMode::ThirdPerson)
            {
                if (!cam3rd)
                {
                    cam3rd = std::make_shared<ThirdPersonCamera>();
                    cam3rd->SetFrustum(XM_PI / 3, AspectRatio(), 0.5f, 1000.0f);
                    m_pCamera = cam3rd;
                }
                XMFLOAT3 target = woodCrateTransform.GetPosition();
                cam3rd->SetTarget(target);
                cam3rd->SetDistance(8.0f);
                cam3rd->SetDistanceMinMax(3.0f, 20.0f);

                m_CameraMode = CameraMode::ThirdPerson;
            }
            else if (curr_item == 2 && m_CameraMode != CameraMode::Free)
            {
                if (!cam1st)
                {
                    cam1st = std::make_shared<FirstPersonCamera>();
                    cam1st->SetFrustum(XM_PI / 3, AspectRatio(), 0.5f, 1000.0f);
                    m_pCamera = cam1st;
                }
                // 从箱子上方开始
                XMFLOAT3 pos = woodCrateTransform.GetPosition();
                XMFLOAT3 to = XMFLOAT3(0.0f, 0.0f, 1.0f);
                XMFLOAT3 up = XMFLOAT3(0.0f, 1.0f, 0.0f);
                pos.y += 3;
                cam1st->LookTo(pos, to, up);

                m_CameraMode = CameraMode::Free;
            }
        }
        auto woodPos = woodCrateTransform.GetPosition();
        ImGui::Text("Box Position\n%.2f %.2f %.2f", woodPos.x, woodPos.y, woodPos.z);
        auto cameraPos = m_pCamera->GetPosition();
        ImGui::Text("Camera Position\n%.2f %.2f %.2f", cameraPos.x, cameraPos.y, cameraPos.z);
    }
    ImGui::End();
    ImGui::Render();

    D3D11_MAPPED_SUBRESOURCE mappedData;
    HR(m_pd3dImmediateContext->Map(m_pConstantBuffers[1].Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));
    memcpy_s(mappedData.pData, sizeof(CBChangesEveryFrame), &m_CBFrame, sizeof(CBChangesEveryFrame));
    m_pd3dImmediateContext->Unmap(m_pConstantBuffers[1].Get(), 0);
}

void GameApp::DrawScene()
{
    assert(m_pd3dImmediateContext);
    assert(m_pSwapChain);

    m_pd3dImmediateContext->ClearRenderTargetView(m_pRenderTargetView.Get(), reinterpret_cast<const float*>(&Colors::Black));
    m_pd3dImmediateContext->ClearDepthStencilView(m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    //
    // 绘制几何模型
    //
    m_WoodCrate.Draw(m_pd3dImmediateContext.Get());
    m_Floor.Draw(m_pd3dImmediateContext.Get());
    for (auto& wall : m_Walls)
        wall.Draw(m_pd3dImmediateContext.Get());

    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    HR(m_pSwapChain->Present(0, 0));
}


bool GameApp::InitEffect()
{
    ComPtr<ID3DBlob> blob;

    // 创建顶点着色器(2D)
    HR(CreateShaderFromFile(L"Basic_2D_VS.cso", L"HLSL\\Basic_2D_VS.hlsl", "main", "vs_5_0", blob.ReleaseAndGetAddressOf()));
    HR(m_pd3dDevice->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, m_pVertexShader2D.GetAddressOf()));
    // 创建顶点布局(2D)
    HR(m_pd3dDevice->CreateInputLayout(VertexPosTex::inputLayout, ARRAYSIZE(VertexPosTex::inputLayout),
        blob->GetBufferPointer(), blob->GetBufferSize(), m_pVertexLayout2D.GetAddressOf()));

    // 创建像素着色器(2D)
    HR(CreateShaderFromFile(L"Basic_2D_PS.cso", L"HLSL\\Basic_2D_PS.hlsl", "main", "ps_5_0", blob.ReleaseAndGetAddressOf()));
    HR(m_pd3dDevice->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, m_pPixelShader2D.GetAddressOf()));

    // 创建顶点着色器(3D)
    HR(CreateShaderFromFile(L"Basic_3D_VS.cso", L"HLSL\\Basic_3D_VS.hlsl", "main", "vs_5_0", blob.ReleaseAndGetAddressOf()));
    HR(m_pd3dDevice->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, m_pVertexShader3D.GetAddressOf()));
    // 创建顶点布局(3D)
    HR(m_pd3dDevice->CreateInputLayout(VertexPosNormalTex::inputLayout, ARRAYSIZE(VertexPosNormalTex::inputLayout),
        blob->GetBufferPointer(), blob->GetBufferSize(), m_pVertexLayout3D.GetAddressOf()));

    // 创建像素着色器(3D)
    HR(CreateShaderFromFile(L"Basic_3D_PS.cso", L"HLSL\\Basic_3D_PS.hlsl", "main", "ps_5_0", blob.ReleaseAndGetAddressOf()));
    HR(m_pd3dDevice->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, m_pPixelShader3D.GetAddressOf()));

    return true;
}

bool GameApp::InitResource()
{
    // ******************
    // 设置常量缓冲区描述
    D3D11_BUFFER_DESC cbd;
    ZeroMemory(&cbd, sizeof(cbd));
    cbd.Usage = D3D11_USAGE_DYNAMIC;
    cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    // 新建用于VS和PS的常量缓冲区
    cbd.ByteWidth = sizeof(CBChangesEveryDrawing);
    HR(m_pd3dDevice->CreateBuffer(&cbd, nullptr, m_pConstantBuffers[0].GetAddressOf()));
    cbd.ByteWidth = sizeof(CBChangesEveryFrame);
    HR(m_pd3dDevice->CreateBuffer(&cbd, nullptr, m_pConstantBuffers[1].GetAddressOf()));
    cbd.ByteWidth = sizeof(CBChangesOnResize);
    HR(m_pd3dDevice->CreateBuffer(&cbd, nullptr, m_pConstantBuffers[2].GetAddressOf()));
    cbd.ByteWidth = sizeof(CBChangesRarely);
    HR(m_pd3dDevice->CreateBuffer(&cbd, nullptr, m_pConstantBuffers[3].GetAddressOf()));
    // ******************
    // 初始化游戏对象
    ComPtr<ID3D11ShaderResourceView> texture;
    // 初始化木箱
    HR(CreateDDSTextureFromFile(m_pd3dDevice.Get(), L"Texture\\WoodCrate.dds", nullptr, texture.GetAddressOf()));
    m_WoodCrate.SetBuffer(m_pd3dDevice.Get(), Geometry::CreateSphere());
    m_WoodCrate.SetTexture(texture.Get());

    // 初始化地板
    HR(CreateDDSTextureFromFile(m_pd3dDevice.Get(), L"Texture\\floor.dds", nullptr, texture.ReleaseAndGetAddressOf()));
    m_Floor.SetBuffer(m_pd3dDevice.Get(),
        Geometry::CreatePlane(XMFLOAT2(20.0f, 20.0f), XMFLOAT2(5.0f, 5.0f)));
    m_Floor.SetTexture(texture.Get());
    m_Floor.GetTransform().SetPosition(0.0f, -1.0f, 0.0f);


    // 初始化墙体
    m_Walls.resize(4);
    HR(CreateDDSTextureFromFile(m_pd3dDevice.Get(), L"Texture\\brick.dds", nullptr, texture.ReleaseAndGetAddressOf()));
    // 这里控制墙体四个面的生成
    for (int i = 0; i < 4; ++i)
    {
        m_Walls[i].SetBuffer(m_pd3dDevice.Get(),
            Geometry::CreatePlane(XMFLOAT2(20.0f, 8.0f), XMFLOAT2(5.0f, 1.5f)));
        Transform& transform = m_Walls[i].GetTransform();
        transform.SetRotation(-XM_PIDIV2, XM_PIDIV2 * i, 0.0f);
        transform.SetPosition(i % 2 ? -10.0f * (i - 2) : 0.0f, 3.0f, i % 2 == 0 ? -10.0f * (i - 1) : 0.0f);
        m_Walls[i].SetTexture(texture.Get());
    }

    // 初始化采样器状态
    D3D11_SAMPLER_DESC sampDesc;
    ZeroMemory(&sampDesc, sizeof(sampDesc));
    sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    sampDesc.MinLOD = 0;
    sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
    HR(m_pd3dDevice->CreateSamplerState(&sampDesc, m_pSamplerState.GetAddressOf()));


    // ******************
    // 初始化常量缓冲区的值
    // 初始化每帧可能会变化的值
    m_CameraMode = CameraMode::FirstPerson;
    auto camera = std::make_shared<FirstPersonCamera>();
    m_pCamera = camera;
    camera->SetViewPort(0.0f, 0.0f, (float)m_ClientWidth, (float)m_ClientHeight);
    camera->LookAt(XMFLOAT3(), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f));

    // 初始化仅在窗口大小变动时修改的值
    m_pCamera->SetFrustum(XM_PI / 3, AspectRatio(), 0.5f, 1000.0f);
    m_CBOnResize.proj = XMMatrixTranspose(m_pCamera->GetProjXM());

    // 初始化不会变化的值
    // 环境光
    m_CBRarely.dirLight[0].ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
    m_CBRarely.dirLight[0].diffuse = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
    m_CBRarely.dirLight[0].specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
    m_CBRarely.dirLight[0].direction = XMFLOAT3(0.0f, -1.0f, 0.0f);
    // 灯光
    m_CBRarely.pointLight[0].position = XMFLOAT3(0.0f, 10.0f, 0.0f);
    m_CBRarely.pointLight[0].ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
    m_CBRarely.pointLight[0].diffuse = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
    m_CBRarely.pointLight[0].specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
    m_CBRarely.pointLight[0].att = XMFLOAT3(0.0f, 0.1f, 0.0f);
    m_CBRarely.pointLight[0].range = 25.0f;
    m_CBRarely.numDirLight = 1;
    m_CBRarely.numPointLight = 1;
    m_CBRarely.numSpotLight = 0;
    // 初始化材质
    m_CBRarely.material.ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
    m_CBRarely.material.diffuse = XMFLOAT4(0.6f, 0.6f, 0.6f, 1.0f);
    m_CBRarely.material.specular = XMFLOAT4(0.1f, 0.1f, 0.1f, 50.0f);


    // 更新不容易被修改的常量缓冲区资源
    D3D11_MAPPED_SUBRESOURCE mappedData;
    HR(m_pd3dImmediateContext->Map(m_pConstantBuffers[2].Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));
    memcpy_s(mappedData.pData, sizeof(CBChangesOnResize), &m_CBOnResize, sizeof(CBChangesOnResize));
    m_pd3dImmediateContext->Unmap(m_pConstantBuffers[2].Get(), 0);

    HR(m_pd3dImmediateContext->Map(m_pConstantBuffers[3].Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));
    memcpy_s(mappedData.pData, sizeof(CBChangesRarely), &m_CBRarely, sizeof(CBChangesRarely));
    m_pd3dImmediateContext->Unmap(m_pConstantBuffers[3].Get(), 0);

    // ******************
    // 给渲染管线各个阶段绑定好所需资源
    // 设置图元类型，设定输入布局
    m_pd3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    m_pd3dImmediateContext->IASetInputLayout(m_pVertexLayout3D.Get());
    // 默认绑定3D着色器
    m_pd3dImmediateContext->VSSetShader(m_pVertexShader3D.Get(), nullptr, 0);
    // 预先绑定各自所需的缓冲区，其中每帧更新的缓冲区需要绑定到两个缓冲区上
    m_pd3dImmediateContext->VSSetConstantBuffers(0, 1, m_pConstantBuffers[0].GetAddressOf());
    m_pd3dImmediateContext->VSSetConstantBuffers(1, 1, m_pConstantBuffers[1].GetAddressOf());
    m_pd3dImmediateContext->VSSetConstantBuffers(2, 1, m_pConstantBuffers[2].GetAddressOf());

    m_pd3dImmediateContext->PSSetConstantBuffers(1, 1, m_pConstantBuffers[1].GetAddressOf());
    m_pd3dImmediateContext->PSSetConstantBuffers(3, 1, m_pConstantBuffers[3].GetAddressOf());
    m_pd3dImmediateContext->PSSetShader(m_pPixelShader3D.Get(), nullptr, 0);
    m_pd3dImmediateContext->PSSetSamplers(0, 1, m_pSamplerState.GetAddressOf());

    // ******************
    // 设置调试对象名
    //
    D3D11SetDebugObjectName(m_pVertexLayout2D.Get(), "VertexPosTexLayout");
    D3D11SetDebugObjectName(m_pVertexLayout3D.Get(), "VertexPosNormalTexLayout");
    D3D11SetDebugObjectName(m_pConstantBuffers[0].Get(), "CBDrawing");
    D3D11SetDebugObjectName(m_pConstantBuffers[1].Get(), "CBFrame");
    D3D11SetDebugObjectName(m_pConstantBuffers[2].Get(), "CBOnResize");
    D3D11SetDebugObjectName(m_pConstantBuffers[3].Get(), "CBRarely");
    D3D11SetDebugObjectName(m_pVertexShader2D.Get(), "Basic_2D_VS");
    D3D11SetDebugObjectName(m_pVertexShader3D.Get(), "Basic_3D_VS");
    D3D11SetDebugObjectName(m_pPixelShader2D.Get(), "Basic_2D_PS");
    D3D11SetDebugObjectName(m_pPixelShader3D.Get(), "Basic_3D_PS");
    D3D11SetDebugObjectName(m_pSamplerState.Get(), "SSLinearWrap");
    m_Floor.SetDebugObjectName("Floor");
    m_WoodCrate.SetDebugObjectName("WoodCrate");
    m_Walls[0].SetDebugObjectName("Walls[0]");
    m_Walls[1].SetDebugObjectName("Walls[1]");
    m_Walls[2].SetDebugObjectName("Walls[2]");
    m_Walls[3].SetDebugObjectName("Walls[3]");


    return true;
}

GameApp::GameObject::GameObject()
    : m_IndexCount(), m_VertexStride()
{
}

Transform& GameApp::GameObject::GetTransform()
{
    return m_Transform;
}

const Transform& GameApp::GameObject::GetTransform() const
{
    return m_Transform;
}

template<class VertexType, class IndexType>
void GameApp::GameObject::SetBuffer(ID3D11Device* device, const Geometry::MeshData<VertexType, IndexType>& meshData)
{
    // 释放旧资源
    m_pVertexBuffer.Reset();
    m_pIndexBuffer.Reset();

    // 设置顶点缓冲区描述
    m_VertexStride = sizeof(VertexType);
    D3D11_BUFFER_DESC vbd;
    ZeroMemory(&vbd, sizeof(vbd));
    vbd.Usage = D3D11_USAGE_IMMUTABLE;
    vbd.ByteWidth = (UINT)meshData.vertexVec.size() * m_VertexStride;
    vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vbd.CPUAccessFlags = 0;
    // 新建顶点缓冲区
    D3D11_SUBRESOURCE_DATA InitData;
    ZeroMemory(&InitData, sizeof(InitData));
    InitData.pSysMem = meshData.vertexVec.data();
    HR(device->CreateBuffer(&vbd, &InitData, m_pVertexBuffer.GetAddressOf()));


    // 设置索引缓冲区描述
    m_IndexCount = (UINT)meshData.indexVec.size();
    D3D11_BUFFER_DESC ibd;
    ZeroMemory(&ibd, sizeof(ibd));
    ibd.Usage = D3D11_USAGE_IMMUTABLE;
    ibd.ByteWidth = m_IndexCount * sizeof(IndexType);
    ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    ibd.CPUAccessFlags = 0;
    // 新建索引缓冲区
    InitData.pSysMem = meshData.indexVec.data();
    HR(device->CreateBuffer(&ibd, &InitData, m_pIndexBuffer.GetAddressOf()));



}

void GameApp::GameObject::SetTexture(ID3D11ShaderResourceView* texture)
{
    m_pTexture = texture;
}

void GameApp::GameObject::Draw(ID3D11DeviceContext* deviceContext)
{
    // 设置顶点/索引缓冲区
    UINT strides = m_VertexStride;
    UINT offsets = 0;
    deviceContext->IASetVertexBuffers(0, 1, m_pVertexBuffer.GetAddressOf(), &strides, &offsets);
    deviceContext->IASetIndexBuffer(m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

    // 获取之前已经绑定到渲染管线上的常量缓冲区并进行修改
    ComPtr<ID3D11Buffer> cBuffer = nullptr;
    deviceContext->VSGetConstantBuffers(0, 1, cBuffer.GetAddressOf());
    CBChangesEveryDrawing cbDrawing;

    // 内部进行转置
    XMMATRIX W = m_Transform.GetLocalToWorldMatrixXM();
    cbDrawing.world = XMMatrixTranspose(W);
    cbDrawing.worldInvTranspose = XMMatrixTranspose(InverseTranspose(W));

    // 更新常量缓冲区
    D3D11_MAPPED_SUBRESOURCE mappedData;
    HR(deviceContext->Map(cBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));
    memcpy_s(mappedData.pData, sizeof(CBChangesEveryDrawing), &cbDrawing, sizeof(CBChangesEveryDrawing));
    deviceContext->Unmap(cBuffer.Get(), 0);

    // 设置纹理
    deviceContext->PSSetShaderResources(0, 1, m_pTexture.GetAddressOf());
    // 可以开始绘制
    deviceContext->DrawIndexed(m_IndexCount, 0, 0);
}

void GameApp::GameObject::SetDebugObjectName(const std::string& name)
{
#if (defined(DEBUG) || defined(_DEBUG)) && (GRAPHICS_DEBUGGER_OBJECT_NAME)
    D3D11SetDebugObjectName(m_pVertexBuffer.Get(), name + ".VertexBuffer");
    D3D11SetDebugObjectName(m_pIndexBuffer.Get(), name + ".IndexBuffer");
#else
    UNREFERENCED_PARAMETER(name);
#endif
}