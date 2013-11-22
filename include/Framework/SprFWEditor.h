#ifndef SPR_FWEDITOR_H
#define SPR_FWEDITOR_H

/**
	ObjectIfのAPIを介した表示・編集を行うクラス
 */

#include <Foundation/SprObject.h>
#include <vector>
#include <string>

namespace Spr{;
/** \addtogroup gpFramework */
//@{

/** FWEditor
	basic functions for viewing and editting descriptors
 */

class UTTypeDesc;
struct GRRenderIf;

class FWEditor{
protected:
	enum PrimitiveType{
		TYPE_UNKNOWN, TYPE_BOOL,
		TYPE_UINT8, TYPE_INT8, TYPE_UINT16, TYPE_INT16, TYPE_UINT32, TYPE_INT32,
		TYPE_FLOAT, TYPE_DOUBLE,
	};
	struct FieldInfo{
		//const UTTypeDesc::Field*	field;
		const void*					field;
		size_t						nElements;
		PrimitiveType				type;

		//FieldInfo(const UTTypeDesc::Field* f);
		FieldInfo(const void* f);
	};
	
	NamedObjectIf*		curObj;		///< reference to object
	const UTTypeDesc*	typeDesc;	///< typedesc of current object
	std::vector<char>	descData;	///< buffer to store descriptor
	std::vector<FieldInfo> fieldInfos;			///< array of field infos
	std::vector<NamedObjectIf*>	childObjects;	///< array of child object names
	std::vector<NamedObjectIf*>	hist;	///< history of displayed objects

	int			curChild;			///< currently selected child object
	int			curField;			///< currently selected field
	int			curElement;			///< currently selected element (used for vectors)
	
	/// returns number of elements for primitive typedesc
	size_t NElements(std::string typeName);

	/// convert composite typedesc into array of fields
	//void FlattenTypeDesc(const UTTypeDesc::Field* field);
	void FlattenTypeDesc(const void* field);

	/// increment/decrement selected field[element]
	void Increment(bool mode);
	void Decrement(bool mode);
	double Increase(double val, bool mode);
	double Decrease(double val, bool mode);

public:
	/// select object
	void SetObject(NamedObjectIf* obj, bool clearHist = true);

	/// reload descriptor of selected object
	void Update();

	FWEditor();
};

/**	Validator
 */
class FWValidator{

};

/** DescViewOverlay
	shows information on graphics window using text overlay
 */
class FWEditorOverlay : public FWEditor{
protected:
	std::ostringstream	oss;

	Vec2f		margin;				///< left and top margin
	float		columnWidth;		///< column with in pixels
	float		lineSpace;			///< line spacing in pixels
	Vec3f		drawPos;			///< current position to draw text

	/// draw field
	virtual void DrawField(GRRenderIf* render, size_t index);

public:
	/// keyboard event handler. returns true if handled
	virtual bool Key(int key);

	/// draw
	virtual void Draw(GRRenderIf* render);

	FWEditorOverlay();
};
//@}
}

#endif