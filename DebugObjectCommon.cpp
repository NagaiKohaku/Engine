#include "DebugObjectCommon.h"

#include "DirectXCommon.h"

#include "Log.h"

#include "cassert"

DebugObjectCommon* DebugObjectCommon::GetInstance() {
    static DebugObjectCommon instance;
    return &instance;
}

void DebugObjectCommon::Initialize() {

	//DirectX���̃C���X�^���X���擾
	dxCommon_ = DirectXCommon::GetInstance();

	//�O���t�B�b�N�p�C�v���C���̐���
	CreateGraphicsPipeline();

}

void DebugObjectCommon::Update() {

}

void DebugObjectCommon::CommonDrawSetting() {

	//���[�g�V�O�l�`����ݒ�
	dxCommon_->GetCommandList()->SetGraphicsRootSignature(rootSignature_.Get());

	//�O���t�B�b�N�p�C�v���C����ݒ�
	dxCommon_->GetCommandList()->SetPipelineState(graphicsPipelineState_.Get());

	//�v���~�e�B�u�g�|���W�[��ݒ�
	dxCommon_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

}

void DebugObjectCommon::CreateRootSignature() {

	HRESULT hr;

	//���[�g�V�O�l�`��
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};

	descriptionRootSignature.Flags =
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
	descriptorRange[0].BaseShaderRegister = 0; //0����n�܂�
	descriptorRange[0].NumDescriptors = 1; //����1��
	descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; //SRV���g��
	descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND; //Offset�������v�Z

	//RootParameter���쐬
	D3D12_ROOT_PARAMETER rootParameters[2] = {};

	//�}�e���A��
	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;                   //CBV���g��
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;                //PixelShader���g��
	rootParameters[0].Descriptor.ShaderRegister = 0;                                   //���W�X�^�ԍ�0�ƃo�C���h

	//WVP
	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;                   //CBV���g��
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;               //VertexShader���g��
	rootParameters[1].Descriptor.ShaderRegister = 0;                                   //���W�X�^�ԍ�0�ƃo�C���h

	descriptionRootSignature.pParameters = rootParameters;               //���[�g�p�����[�^�z��ւ̃|�C���^
	descriptionRootSignature.NumParameters = _countof(rootParameters);   //�z��̒���

	D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};
	staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;         //�o�C���j�A�t�B���^
	staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;       //0~1�͈̔͊O�����s�[�g
	staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;     //��r���Ȃ�
	staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX;                       //�����������MinMap���g��
	staticSamplers[0].ShaderRegister = 0;                               //���W�X�^�ԍ�0���g��
	staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; //PixelShader�Ŏg��
	descriptionRootSignature.pStaticSamplers = staticSamplers;
	descriptionRootSignature.NumStaticSamplers = _countof(staticSamplers);

	//�V���A���C�Y���ăo�C�i���ɂ���
	Microsoft::WRL::ComPtr<ID3DBlob> signatureBlob = nullptr;

	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob = nullptr;

	hr = D3D12SerializeRootSignature(&descriptionRootSignature,
		D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);

	//�G���[���o����~�߂�
	if (FAILED(hr)) {

		OutPutLog(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));

		assert(false);
	}

	hr = dxCommon_->GetDevice()->CreateRootSignature(0, signatureBlob->GetBufferPointer(),
		signatureBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature_));

	assert(SUCCEEDED(hr));
}

void DebugObjectCommon::CreateGraphicsPipeline() {

	HRESULT hr;

	/// === RootSignature��ݒ肷�� === ///

	CreateRootSignature();

	/// === InputLayout��ݒ肷�� === ///

	//InputLayOut��ݒ肷��
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[1] = {};
	inputElementDescs[0].SemanticName = "POSITION";
	inputElementDescs[0].SemanticIndex = 0;
	inputElementDescs[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputElementDescs[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	//InputLayout���m�肷��
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
	inputLayoutDesc.pInputElementDescs = inputElementDescs;
	inputLayoutDesc.NumElements = _countof(inputElementDescs);

	/// === BlendState��ݒ肷�� === ///

	//BlendState�̐ݒ�
	D3D12_BLEND_DESC blendDesc{};

	//Normal�̃u�����h���[�h�Őݒ�
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;

	/// === RasterizerState��ݒ肷�� === ///

	//RasterizerState�̐ݒ�
	D3D12_RASTERIZER_DESC rasterizerDesc{};

	//����(���v���)��\�����Ȃ�
	rasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;

	//�O�p�`�̒���h��Ԃ�
	rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;

	/// === Shader�̃R���p�C�� === ///

	//VertexShader���R���p�C������
	Microsoft::WRL::ComPtr<IDxcBlob> vertexShaderBlob =
		dxCommon_->CompileShader(
			L"Resource/Shader/DebugObject.VS.hlsl",
			L"vs_6_0"
		);

	assert(vertexShaderBlob != nullptr);

	//PixelShader���R���p�C������
	Microsoft::WRL::ComPtr<IDxcBlob> pixelShaderBlob =
		dxCommon_->CompileShader(
			L"Resource/Shader/DebugObject.PS.hlsl",
			L"ps_6_0"
		);

	assert(pixelShaderBlob != nullptr);

	/// === DepthStencilState�̐ݒ� === ///

	//DepthStencilState�̐ݒ�
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};
	//Depth�̋@�\��L��������
	depthStencilDesc.DepthEnable = true;
	//�������݂��܂�
	depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	//��r�֐���LessEqual�B�܂�A�߂���Ε`�悳���
	depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;

	/// === PSO�𐶐����� === ///

	//PSO�̐ݒ�
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};

	//RootSignature
	graphicsPipelineStateDesc.pRootSignature = rootSignature_.Get();

	//InputLayout
	graphicsPipelineStateDesc.InputLayout = inputLayoutDesc;

	//VertexShader
	graphicsPipelineStateDesc.VS = { vertexShaderBlob->GetBufferPointer(),
	vertexShaderBlob->GetBufferSize() };

	//PixelShader
	graphicsPipelineStateDesc.PS = { pixelShaderBlob->GetBufferPointer(),
	pixelShaderBlob->GetBufferSize() };

	//DepthStencil�̐ݒ�
	graphicsPipelineStateDesc.DepthStencilState = depthStencilDesc;
	graphicsPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	//BlendState
	graphicsPipelineStateDesc.BlendState = blendDesc;

	//RasterizerState
	graphicsPipelineStateDesc.RasterizerState = rasterizerDesc;

	//��������RTV�̏��
	graphicsPipelineStateDesc.NumRenderTargets = 1;

	graphicsPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;

	//���p����g�|���W(�`��)�̃^�C�v�B�O�p�`
	graphicsPipelineStateDesc.PrimitiveTopologyType =
		D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	//�ǂ̂悤�ɉ�ʂɐF��ł����ނ��̐ݒ�
	graphicsPipelineStateDesc.SampleDesc.Count = 1;

	graphicsPipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

	//PSO�𐶐�
	hr = dxCommon_->GetDevice()->CreateGraphicsPipelineState(&graphicsPipelineStateDesc,
		IID_PPV_ARGS(&graphicsPipelineState_));

	//����ɐ����ł��Ă��邩�̊m�F
	assert(SUCCEEDED(hr));
}