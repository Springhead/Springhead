#ifndef VRMLPARSER_H
#define VRMLPARSER_H
#include "FileIO.h"
#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

#include "FIFileVRML.h"
#include <Foundation/UTLoadHandler.h>
#include <fstream>
#include <sstream>
#include <Springhead.h>
#include <map>

namespace Spr{;

class FIFileVRML:public FIFile{
public:
	SPR_OBJECTDEF(FIFileVRML);
protected:
	FIPhraseParser vrmlScene, statements, statement, nodeStatement, protoStatement, 
		nodeParser, protoStatements, proto, protoBody, interfaceDeclarations, 
		restrictedInterfaceDeclaration, interfaceDeclaration, externproto, 
		externInterfaceDeclarations, externInterfaceDeclaration, routeStatement, 
		URLList, node, scriptBody, scriptBodyElement, nodeBodyElement,  id, 
		fieldType, fieldValue, sfboolValue, sfcolorValue, sffloatValue, sfimageValue, 
		sfint32Value, sfnodeValue, sfrotationValue, sfstringValue, sftimeValue, mftimeValue, 
		sftimeValues, sfvec2fValue, sfvec3fValue, mfcolorValue, sfcolorValues, mffloatValue, 
		sffloatValues, mfint32Value, sfint32Values, mfnodeValue, nodeStatements, mfrotationValue,
		sfrotationValues, mfstringValue, sfstringValues, mfvec2fValue, sfvec2fValues, 
		mfvec3fValue, sfvec3fValues, NodeRule, NodeBodyRule, NodeBodyElementRule;
	FIPhraseParser decParser;
	FISkipParser cmt;
	std::map<std::string, FIPhraseParser> valueParsers;

public:
	FIFileVRML();
	void LoadImp(FILoadContext* fc);
	virtual void PushLoaderContext(FILoadContext* fc);
	virtual void PopLoaderContext();

protected:
	void Init();
	//	ï€ë∂èàóùÇÃÉnÉìÉhÉâÅ[
/*
	virtual void OnSaveFileStart(FISaveContext* sc);
	virtual void OnSaveNodeStart(FISaveContext* sc);
	virtual void OnSaveNodeEnd(FISaveContext* sc);
	virtual void OnSaveDataEnd(FISaveContext* sc);
	virtual void OnSaveBool(FISaveContext* sc, bool val);
	virtual void OnSaveInt(FISaveContext* sc, int val);
	virtual void OnSaveReal(FISaveContext* sc, double val);
	virtual void OnSaveRef(FISaveContext* sc);
	virtual void OnSaveString(FISaveContext* sc, UTString val);
	virtual void OnSaveFieldStart(FISaveContext* sc, int nElements);
	virtual void OnSaveFieldEnd(FISaveContext* sc, int nElements);
	virtual void OnSaveElementEnd(FISaveContext* sc, int pos, bool last);
*/
	friend void OnEndProto(const char* first, const char* last);
};

} //	namespace Spr
//---------------------------------------------------------------------------
#endif