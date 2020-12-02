#  GameplayAbility Extensions

*Fair Warning:  This is a C++ plugin for UE4*

Helpers for working with the GameplayAbilities plugin

## AnimNotify_SendGameplayEvent

Animation notify for sending a gameplay event to the owning actor.

![Animation notify exampel](Documentation/images/anim-notify.png)

> When prototyping, I have a GameplayAbility that forwards the event to the actor's blueprint for easy testing.  In my actual game, I use this for things like equip/unequip.

