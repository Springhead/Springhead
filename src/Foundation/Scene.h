#ifndef SCENE_H
#define SCENE_H
#include <Foundation/Object.h>
#include <set>
#include <map>

namespace Spr {;


class ObjectNamesLess{
public:
	bool operator () (Object* o1, Object* o2) const;
};
/**	名前とシーングラフのオブジェクトの対応表
	名前をキーにしたセットを用意し，名前の一意性の保証とオブジェクトの
	検索を実現する．	*/
class ObjectNames:public std::set<UTRef<Object>, ObjectNamesLess >{
public:
	typedef std::map<UTString, UTString> TNameMap;
	class ObjectKey:public Object{
	public:
		ObjectKey();
		~ObjectKey();
		DEF_UTTYPEINFODEF(ObjectKey);
	};
	TNameMap nameMap;
	static ObjectKey key;
	void Print(std::ostream& os) const;

	///	名前からオブジェクトを取得
	typedef std::pair<iterator, iterator> range_type;
	Object* Find(UTString name, UTString ns="", UTString cn="") const {
		key.name = name;
		key.nameSpace = ns;
		UTString className = cn;
		key.typeInfo.className = className.c_str();
		const_iterator lb = lower_bound(&key);
		if (lb==end()) return NULL;
		ObjectNamesLess less;
		if (less(*lb, &key)) return NULL;		//	等しいものがない場合
		const_iterator it = lb;
		++it;
		if (it == end()) return *lb;
		if(less(&key, *it)) return *lb;		//	等しいものは1つ
		return NULL;						//	等しいものが複数有る場合
	}
	range_type Range(UTString name, UTString ns="", UTString cn=""){
		key.name = name;
		key.nameSpace = ns;
		UTString className = cn;
		key.typeInfo.className = className.c_str();
		iterator lb = lower_bound(&key);
		if (lb==end()) return range_type(end(), end());
		ObjectNamesLess less;
		iterator it = lb;
		while(it != end() && !less(*it, &key)){
			++it;
		}
		return range_type(lb, it);
	}
	/**	オブジェクトの追加，
		名前のないオブジェクトは追加できない．この場合 false を返す．
		追加に成功すると true． すでに登録されていた場合は false を返す．
		名前がぶつかる場合は，追加するオブジェクトの名前が変更される．	*/
	bool Add(Object* obj);
	///	オブジェクトの削除
	bool Del(Object* key){
		iterator it = find(key);
		if (it==end()) return false;
		erase(it);
		return true;
	}
	///	オブジェクトの削除
	bool Del(UTString name){
		Object key;
		key.name = name;
		key.AddRef();
		iterator it = find(&key);
		key.DelRef();
		if (it==end()) return false;
		erase(it);
		return true;
	}
};
inline std::ostream& operator << (std::ostream& os, const ObjectNames& ns){
	ns.Print(os); return os;
}

/**	シーングラフのトップノード．光源・視点を持つ．
	レンダラとシーングラフの関係が深いため，
	レンダラが違うとシーングラフはそのまま使用できない．
	シーングラフは，たとえばレンダラがOpenGLなら，displayList
	を持っているし，D3Dならば ID3DXMeshを持っている．
	OpenGLのシーングラフをD3Dに変換するためには，一度Documentに
	セーブして，D3D形式でロードしなければならない．	*/
class SPR_DLL Scene:public Object{
public:
	OBJECTDEF(Scene);
protected:
	///	名前とオブジェクトの対応表	
	ObjectNames names;
public:
	///	コンストラクタ
	Scene();
	virtual ~Scene(){Clear();}

	///	名前からオブジェクトを取得
	Object* FindObject(UTString name, UTString ns=""){ return names.Find(name, ns); }
	///	型と名前からオブジェクトを取得
	template <class T> void FindObject(UTRef<T>& t, UTString name, UTString ns=""){
		T* p;
		FindObject(p, name, ns);
		t = p;
	}
	template <class T> void FindObject(T*& t, UTString name, UTString ns=""){
		Object* p = names.Find(name, ns, GETCLASSNAMES(T));
		t = DCAST(T, p);
	}
	typedef ObjectNames::iterator SetIt;
	typedef std::pair<SetIt, SetIt> SetRange;
	SetRange RangeObject(UTString n){ return names.Range(n); }
	
	ObjectNames::TNameMap& GetNameMap(){ return names.nameMap; }

	///	シーンを空にする．
	void Clear();
	///	名前表から，参照されていないオブジェクトを削除する．
	void ClearName();
	///	デバッグ用
	void Print(std::ostream& os) const { names.Print(os); }

	friend class Object;
};

}
#endif
