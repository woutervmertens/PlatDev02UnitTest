#pragma once

#include "ContentLoader.h"
#include "DdsTextureResource.h"

class DdsTextureLoader : public ContentLoader<DdsTextureResource>
{
public:
	DdsTextureLoader(void);
	~DdsTextureLoader(void);

protected:

	virtual DdsTextureResource* LoadContent(wstring assetFile);
	virtual void Destroy(DdsTextureResource* objToDestroy);

private:
// -------------------------
// Disabling default copy constructor and default 
// assignment operator.
// -------------------------
DdsTextureLoader(const DdsTextureLoader& yRef);									
DdsTextureLoader& operator=(const DdsTextureLoader& yRef);
};

