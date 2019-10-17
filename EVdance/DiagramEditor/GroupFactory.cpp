#include "stdafx.h"
#include "GroupFactory.h"


int CGroupFactory::GetNewGroup()
/* ============================================================
	Function :		CGroupFactory::GetNewGroup()
	Description :	Gets a new unique group-id.
	Access :		Public
					
	Return :		int		-	Unique group id.
	Parameters :	none

	Usage :			Call to get a new unique group id.

   ============================================================*/
{

	CGroupFactory::m_sCurrentGroup++;
	return CGroupFactory::m_sCurrentGroup;

}

int CGroupFactory::m_sCurrentGroup = 0;
