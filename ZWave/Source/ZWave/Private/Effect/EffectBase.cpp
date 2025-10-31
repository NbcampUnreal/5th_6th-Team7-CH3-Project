#include "Effect/EffectBase.h"

void BeginDestroy()
{

}

void UEffectBase::RemoveEffect()
{
	if (Target)
	{
		Target = nullptr;
	}

	MarkAsGarbage();
}
