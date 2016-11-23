#pragma once


class DdsTextureResource
{
public:
	DdsTextureResource(ID3D11Resource* pTexture, ID3D11ShaderResourceView* pTextureShaderResourceView);
	~DdsTextureResource(void);

	ID3D11Resource* GetResource() const { return m_pTexture; }
	ID3D11ShaderResourceView* GetShaderResourceView() const { return m_pTextureShaderResourceView; }

private:

	ID3D11Resource *m_pTexture;
	ID3D11ShaderResourceView *m_pTextureShaderResourceView;

private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	DdsTextureResource(const DdsTextureResource& t);
	DdsTextureResource& operator=(const DdsTextureResource& t);
};

