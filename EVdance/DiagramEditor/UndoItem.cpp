#include "stdafx.h"
#include "UndoItem.h"

// Construction/destruction
CUndoItem::CUndoItem()
/* ============================================================
	Function :		CUndoItem::CUndoItem
	Description :	constructor
	Access :		Public

	Return :		void
	Parameters :	none

	Usage :			

   ============================================================*/
{
}

CUndoItem::~CUndoItem()
/* ============================================================
	Function :		CUndoItem::~CUndoItem
	Description :	Destructor
	Access :		Public

	Return :		void
	Parameters :	none

	Usage :			Deletes all associated memory.

   ============================================================*/
{

	int max = arr.GetSize();
	for (int t = 0 ; t < max ; t++ )
		delete arr.GetAt( t );
	arr.RemoveAll();

}
