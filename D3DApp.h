#ifndef D3DAPP_H
#define D3DAPP_H

#include <wrl/client.h>
#include <string>
#include  "windows.h"
#include <d2d1.h>
#include <dwrite.h>
#include <d3d11_1.h>
#include <DirectXMath.h>
#include "Mouse.h"
#include "Keyboard.h"
#include "CpuTimer.h"

class D3DApp
{
public:
    D3DApp(HINSTANCE hInstance, const std::wstring& windowName, int initWidth, int initHeight);
    virtual ~D3DApp();

    HINSTANCE AppInst()const;                   // ��ȡӦ��ʵ���ľ��
    HWND      MainWnd()const;                   // ��ȡ�����ھ��
    float     AspectRatio()const;               // ��ȡ��Ļ��߱�

    int Run();                                  // ���г��򣬽�����Ϸ��ѭ��

    // ��ܷ������ͻ���������Ҫ������Щ������ʵ���ض���Ӧ������
    virtual bool Init();                        // �ø��෽����Ҫ��ʼ�����ں�Direct3D����
    virtual void OnResize();                    // �ø��෽����Ҫ�ڴ��ڴ�С�䶯��ʱ�����
    virtual void UpdateScene(float dt) = 0;     // ������Ҫʵ�ָ÷��������ÿһ֡�ĸ���
    virtual void DrawScene() = 0;               // ������Ҫʵ�ָ÷��������ÿһ֡�Ļ���
    virtual LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
    // ���ڵ���Ϣ�ص�����
protected:
    bool InitMainWindow();      // ���ڳ�ʼ��
    bool InitDirect2D();        // Direct2D��ʼ��
    bool InitDirect3D();        // Direct3D��ʼ��

    void CalculateFrameStats(); // ����ÿ��֡�����ڴ�����ʾ

protected:

    HINSTANCE m_hAppInst;        // Ӧ��ʵ�����
    HWND      m_hMainWnd;        // �����ھ��
    bool      m_AppPaused;       // Ӧ���Ƿ���ͣ
    bool      m_Minimized;       // Ӧ���Ƿ���С��
    bool      m_Maximized;       // Ӧ���Ƿ����
    bool      m_Resizing;        // ���ڴ�С�Ƿ�仯
    bool      m_Enable4xMsaa;    // �Ƿ���4�����ز���
    UINT      m_4xMsaaQuality;   // MSAA֧�ֵ������ȼ�


    CpuTimer m_Timer;            // ��ʱ��

    // ʹ��ģ�����(C++11)��������
    template <class T>
    using ComPtr = Microsoft::WRL::ComPtr<T>;
    // Direct2D
    ComPtr<ID2D1Factory> m_pd2dFactory;                   //D2D����
    ComPtr<ID2D1RenderTarget> m_pd2dRenderTarget;         //D2D��ȾĿ��
    ComPtr<IDWriteFactory> m_pdwriteFactory;              //DWrite����
    // Direct3D 11
    ComPtr<ID3D11Device> m_pd3dDevice;                    // D3D11�豸
    ComPtr<ID3D11DeviceContext> m_pd3dImmediateContext;   // D3D11�豸������
    ComPtr<IDXGISwapChain> m_pSwapChain;                  // D3D11������
    // Direct3D 11.1
    ComPtr<ID3D11Device1> m_pd3dDevice1;                  // D3D11.1�豸
    ComPtr<ID3D11DeviceContext1> m_pd3dImmediateContext1; // D3D11.1�豸������
    ComPtr<IDXGISwapChain1> m_pSwapChain1;                // D3D11.1������
    // ������Դ
    ComPtr<ID3D11Texture2D> m_pDepthStencilBuffer;        // ���ģ�建����
    ComPtr<ID3D11RenderTargetView> m_pRenderTargetView;   // ��ȾĿ����ͼ
    ComPtr<ID3D11DepthStencilView> m_pDepthStencilView;   // ���ģ����ͼ
    D3D11_VIEWPORT m_ScreenViewport;                      // �ӿ�
    // ��������
    std::unique_ptr<DirectX::Mouse> m_pMouse;                 //���
    DirectX::Mouse::ButtonStateTracker m_MouseTracker;        //���״̬׷����
    std::unique_ptr<DirectX::Keyboard> m_pKeyboard;           //����
    DirectX::Keyboard::KeyboardStateTracker m_KeyboardTracker;//����״̬׷����
    // ������Ӧ���ڹ��캯�����ú���Щ�Զ���ĳ�ʼ����
    std::wstring m_MainWndCaption;                       // �����ڱ���
    int m_ClientWidth;                                   // �ӿڿ��
    int m_ClientHeight;                                  // �ӿڸ߶�
};

#endif // D3DAPP_H