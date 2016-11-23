#pragma once

#include "ContentLoader.h"
#include <unordered_map>

class EffectLoader : public ContentLoader<ID3DX11Effect>
{
public:
	EffectLoader(void);
	~EffectLoader(void);

protected:
	virtual ID3DX11Effect* LoadContent(wstring assetFile);
	virtual void Destroy(ID3DX11Effect* objToDestroy);

private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	EffectLoader( const EffectLoader &obj);
	EffectLoader& operator=( const EffectLoader& obj);
};

