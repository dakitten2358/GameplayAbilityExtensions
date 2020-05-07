#include "AxAbilitySystemGlobals.h"
#include "AxGameplayEffectContext.h"

FGameplayEffectContext* UAxAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FAxGameplayEffectContext();
}
