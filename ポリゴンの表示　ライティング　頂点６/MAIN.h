#pragma once
//�w�b�_�[�t�@�C���̃C���N���[�h
#include <windows.h>
#include <d3d11.h>
#include <d3dx10.h>
#include <d3dx11.h>
#include <d3dCompiler.h>
//�K�v�ȃ��C�u�����t�@�C���̃��[�h
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"d3dx10.lib")
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dx11.lib")
#pragma comment(lib,"d3dCompiler.lib")
//�x����\��
// double����float�ɒl���i�荞�ނ��ƂɊւ���x���B
#pragma warning(disable : 4305)
//�萔��`
#define WINDOW_WIDTH 640 //�E�B���h�E��
#define WINDOW_HEIGHT 480 //�E�B���h�E����
#define APP_NAME L"�|���S���̕\�� D3d11"//�^�C�g���o�[�̖��O
//�}�N�� �R�[�h�̏d������������
#define SAFE_RELEASE(x) if(x){x->Release(); x;}

//���_�̍\����
struct SimpleVertex
{
	D3DXVECTOR3 Pos; //�ʒu Vec�^ X,Y,Z
	D3DXVECTOR3 Normal;//�@���@�V�F�[�f�B���O
};
//Simple�V�F�[�_�[�p�̃R���X�^���g�o�b�t�@�[�̃A�v�����\���� 
//�������V�F�[�_�[���̃R���X�^���g�o�b�t�@�[�ƈ�v���Ă���K�v����
struct SIMPLESHADER_CONSTANT_BUFFER
{
	D3DXMATRIX mW;      //���[���h�ϊ�
	D3DXMATRIX mWVP;    //�ϊ��s��
	D3DXVECTOR4 vLightDir;//���C�g����
	D3DXVECTOR4 vColor; //�f�B�t���[�Y�F

};
//
//
//�N���X��`
class MAIN
{
public:
	MAIN();//�R���X�g���N�^
	~MAIN();//�f�X�g���N�^
	HRESULT InitWindow(HINSTANCE,INT,INT,INT,INT,LPCWSTR);//�E�B���h�E�̍쐬
	LRESULT MsgProc(HWND,UINT,WPARAM,LPARAM);//�E�B���h�E�v���V�[�W���[
	HRESULT InitD3D();//������
	HRESULT InitPolygon();//�|���S���̏�����
	HRESULT InitShader();//�V�F�[�_�[�̏�����(�o�[�e�b�N�X�V�F�[�_�[�ƃs�N�Z���V�F�[�_�[�̏�����)
	void Loop();          //���b�Z�[�W���[�v
	void App();           //�A�v���P�[�V��������
	void Render();    //�|���S���`�揈��
	void DestroyD3D();//�I�u�W�F�N�g�̔j��
	//���A�v���ɂЂƂ�
	HWND m_hWnd;//�E�B���h�E�̃n���h��
	ID3D11Device* m_pDevice;                    //�f�o�C�X�C���^�[�t�F�[�X
	ID3D11DeviceContext* m_pDeviceContext;      //�����_�����O�R�}���h�𐶐�����f�o�C�X�R���e�L�X�g
	IDXGISwapChain* m_pSwapChain;               //�����_�����O���ꂽ�f�[�^���o�͂ɒ񎦂���O�Ɋi�[���邽�߂�1�ȏ�̃T�[�t�F�X������
	ID3D11RenderTargetView* m_pRenderTargetView;//�����_�����O���ɃA�N�Z�X�ł��郌���_�[�^�[�Q�b�g�T�u���\�[�X������
	ID3D11DepthStencilView* m_pDepthStencilView;//�[�x�X�e���V���e�X�g���Ƀe�N�X�`�����\�[�X�ɃA�N�Z�X
	ID3D11Texture2D* m_pDepthStencil;           //�e�N�Z���f�[�^���Ǘ�,�e�N�Z���f�[�^�͍\����������
	D3DXVECTOR3 m_vLight;
	//�����f���̎�ނ���(���f���̍\�����S�ē���Ȃ�A�v���ɂЂƂj
	//���|���S���`��ɕK�v�ȕ�
	//���̂S�͓����^�C�v�̃|���S����
	//�������P�ő����
	ID3D11InputLayout* m_pVertexLayout;  //���_���C�A�E�g�̃|�C���^�A���W�݂̂̃��C�A�E�g�̏�����
	ID3D11VertexShader* m_pVertexShader; //�o�[�e�b�N�X�V�F�[�_�[�̃|�C���^
	ID3D11GeometryShader* m_pGeometryShader; //�V�F�[�_�[�̃|�C���^
	ID3D11PixelShader* m_pPixelShader;   //�s�N�Z���V�F�[�_�[�̃|�C���^
	ID3D11Buffer* m_pConstantBuffer;     //�R���X�^���X�ƃo�b�t�@�[
	//�����f�����ƂɕK�v	
	ID3D11Buffer* m_pVertexBuffer;       //�o�[�e�b�N�X�o�b�t�@�[(���_�o�b�t�@�[),���_��ۑ����Ă����������[�̈�
};
