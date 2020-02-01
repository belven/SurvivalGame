#include "Group.h"
#include "SurvivalGameCharacter.h"

void UGroup::AddMember(ASurvivalGameCharacter* memberToAdd)
{
	if (memberToAdd->GetGroup() != NULL && memberToAdd->GetGroup() != this)
		members.Add(memberToAdd);
}

void UGroup::RemoveMember(ASurvivalGameCharacter* memberToRemove)
{
	members.Remove(memberToRemove);
}
