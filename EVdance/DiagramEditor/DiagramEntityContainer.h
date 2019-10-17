#ifndef _DIAGRAMENTITYCONTAINER_H_
#define _DIAGRAMENTITYCONTAINER_H_
class CDiagramEntity;
#include "DiagramClipboardHandler.h"
#include "UndoItem.h"

class CDiagramEntityContainer {

public:

	// Construction/destruction/initialization
	CDiagramEntityContainer( CDiagramClipboardHandler* clip = NULL );
	virtual ~CDiagramEntityContainer();
	void Clear();
	virtual CString	GetString() const;
	virtual BOOL FromString( const CString& str );
	virtual void Export( CStringArray& stra, UINT format = 0 ) const;
	virtual void SetClipboardHandler( CDiagramClipboardHandler* clip );
	virtual CDiagramClipboardHandler* GetClipboardHandler();

	// Data access
	virtual CDiagramEntity* GetAt( int index ) const;
	CObArray*		GetData();
	int				GetSize() const;
	virtual void	Add( CDiagramEntity* obj );
	virtual void	RemoveAt( int index );
	virtual void	RemoveAll();
	virtual void	RemoveAllSelected();
	virtual void	Remove( CDiagramEntity* obj );

	void			SetVirtualSize( CSize size );
	CSize			GetVirtualSize() const;

	void			SetModified( BOOL dirty );
	BOOL			IsModified() const;

	virtual void	SelectAll();
	virtual void	UnselectAll();
	int				GetSelectCount() const;

	// Undo handling
	virtual void	Undo();
	virtual void	Snapshot();
	BOOL			IsUndoPossible();
	virtual void	ClearUndo();
	void			SetUndoStackSize( int maxstacksize );
	int				GetUndoStackSize() const;

	// Group handling
	virtual void	Group();
	virtual void	Ungroup();

	// Single object handlers
	virtual void			Duplicate( CDiagramEntity* obj );
	virtual void			Cut( CDiagramEntity* obj );
	virtual void			Copy( CDiagramEntity* obj );
	virtual void			Up( CDiagramEntity* obj );
	virtual void			Down( CDiagramEntity* obj );
	virtual void			Front( CDiagramEntity* obj );
	virtual void			Bottom( CDiagramEntity* obj );
	virtual void			Paste();

	// Copy/paste
	virtual void			CopyAllSelected();
	virtual int				ObjectsInPaste();
	virtual void			ClearPaste();

	// Message handling
	virtual void			SendMessageToObjects( int command, BOOL selected = TRUE, CDiagramEntity* sender = NULL, CWnd* from = NULL );

	// Positional information
	CSize					GetTotalSize();
	CPoint					GetStartPoint();

protected:
	CObArray*				GetPaste();
	CObArray*				GetUndo();
	int						Find( CDiagramEntity* obj );

private:

	// Data
	CObArray		m_objs;
	CObArray		m_undo;
	int				m_maxstacksize;
	CSize			m_virtualSize;

	CDiagramClipboardHandler*	m_clip;
	CDiagramClipboardHandler	m_internalClip;

	// State
	BOOL			m_dirty;

	// Helpers
	void			Swap( int index1, int index2 );
	void			SetAt( int index, CDiagramEntity* obj );

};

#endif // _DIAGRAMENTITYCONTAINER_H_
