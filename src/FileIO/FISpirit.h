/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef FISPIRIT_H
#define FISPIRIT_H
/**	@File	boost::spirit を使うときに，
	パーサの実装ファイル(.cpp)だけでインクルードして使うヘッダ．
	FIPhraseParser と FISkipParserを定義する．
*/


#include <Base/BaseUtility.h>

#if defined(__BORLANDC__)
# pragma option push -pc
# pragma warn -8026
# pragma warn -8027
#endif

#if defined _MSC_VER && _MSC_VER <= 0x1200
#pragma warning (disable:4761 4786 4096)
#endif

#include <boost/spirit/core.hpp>
#include <boost/spirit/core/safe_bool.hpp>
#include <boost/spirit/dynamic/if.hpp>
#include <boost/spirit/dynamic/while.hpp>
#include <boost/spirit/utility/chset.hpp>
#include <boost/spirit/utility/confix.hpp>
#include <boost/spirit/utility/escape_char.hpp>
#include <boost/spirit/utility/functor_parser.hpp>
#include <boost/shared_ptr.hpp>

#if defined(BOOST_MSVC) && (BOOST_MSVC <= 1300)
#define BOOST_SPIRIT_IT_NS boost::spirit::impl
#else
#define BOOST_SPIRIT_IT_NS std
#endif

#undef BOOST_SPIRIT_SAFE_BOOL
#if !defined(__BORLANDC__)
#define BOOST_SPIRIT_SAFE_BOOL(cond)  ((cond) ? &boost::spirit::impl::dummy::nonnull : 0)
#else
#define BOOST_SPIRIT_SAFE_BOOL(cond)  (cond)
#endif

namespace Spr{;

///	FISpiritで使用する入力文字列のイタレータの型
typedef const char* FIIteratorT;



//----------------------------------------------------------------------------
//	アクションによる，パースツリーを作らないパーサ

///	スキャナを固定するためのスキップ用パーサ．他のパーサを代入できる
struct FISkipParser: boost::spirit::parser<FISkipParser>{
	typedef FISkipParser          self_t;
	typedef FISkipParser const&   embed_t;
	typedef boost::spirit::skip_parser_iteration_policy<FISkipParser> iter_policy_t;
	typedef boost::spirit::scanner_policies<iter_policy_t> scanner_policies_t;
	///	本文(Phrase)のためのスキャナ型
	typedef boost::spirit::scanner<FIIteratorT, scanner_policies_t> PhraseScannerT;

	typedef boost::spirit::scanner_policies<
		boost::spirit::no_skipper_iteration_policy<PhraseScannerT::iteration_policy_t>,
		PhraseScannerT::match_policy_t,
		PhraseScannerT::action_policy_t
    > policies_t;
	///	スペース読み飛ばし(Skipper)のためのスキャナ型
    typedef boost::spirit::scanner<PhraseScannerT::iterator_t, policies_t> SkipperScannerT;

	//	SkipperScannerTでパーサを定義していく．
	typedef boost::spirit::match_result<SkipperScannerT, boost::spirit::parser_context<>::context_linker_t::attr_t>::type Result;
    FISkipParser(){}
    FISkipParser(const FISkipParser& wp): ptr_(wp.ptr_? wp.ptr_->clone() : NULL){}
    ///	Scannerがテンプレートのパーサを受け取って，コンクリートなパーサに変換して保存．
	template <class T>
    FISkipParser(const T& p){
		ptr_.reset(DBG_NEW boost::spirit::impl::concrete_parser<T, SkipperScannerT, Result>(p));
	}
    ///	Scannerがテンプレートのパーサを受け取って，コンクリートなパーサに変換して保存．
	template <class T>
    FISkipParser& operator=(const T& p){
	    ptr_.reset(DBG_NEW boost::spirit::impl::concrete_parser<T, SkipperScannerT, Result>(p));
		return *this;
	}
    ///	スキップのためのパース
    Result parse(const SkipperScannerT& scan)const{
		if (ptr_) return ptr_->do_parse_virtual(scan);
		else return scan.no_match();
	}
	///	コンクリートなパーサ
	boost::scoped_ptr<boost::spirit::impl::abstract_parser<SkipperScannerT, Result> > ptr_;
};

///	スキャナを固定するための本文用パーサ．他のパーサを代入できる
struct FIPhraseParser: public boost::spirit::rule<FISkipParser::PhraseScannerT>{
	typedef boost::spirit::rule<FISkipParser::PhraseScannerT> base_type;
	FIPhraseParser(){}
	template <typename ParserT>
		FIPhraseParser(ParserT const& p){ *this=p; }
	template <typename ParserT>
	FIPhraseParser& operator=(ParserT const& p){
        base_type::operator=(p);
        return *this;
	}
	FIPhraseParser& operator=(FIPhraseParser const& p){
        base_type::operator=(p);
		return *this;
	}

};




//----------------------------------------------------------------------------
//	パースツリーのためのクラス

template <typename ValueT = boost::spirit::nil_t>
class FIPTNodeValData {
public:
    typedef typename BOOST_SPIRIT_IT_NS::iterator_traits<FIIteratorT>::value_type value_type;
    typedef std::vector<value_type> container_t;
    typedef typename container_t::iterator iterator_t;
    typedef typename container_t::const_iterator const_iterator_t;

private:
    container_t text;
    bool is_root_;
    boost::spirit::parser_id parser_id_;
    ValueT value_;

public:
    FIPTNodeValData() : text(), is_root_(false){}
    FIPTNodeValData(FIIteratorT const& _first, FIIteratorT const& _last)
        : text(_first, _last), is_root_(false){}
	FIPTNodeValData(FIIteratorT const& _first, FIIteratorT const& _last, bool is_r, boost::spirit::parser_id i)
        : text(_first, _last), is_root_(is_r), parser_id_(i){}
    void swap(FIPTNodeValData& x){
        cp_swap(text, x.text);
        cp_swap(is_root_, x.is_root_);
        cp_swap(parser_id_, x.parser_id_);
		cp_swap(value_, x.value_);
    }
    typename container_t::iterator begin(){ return text.begin(); }
    typename container_t::const_iterator begin() const{ return text.begin(); }
    typename container_t::iterator end(){ return text.end(); }
    typename container_t::const_iterator end() const{ return text.end(); }
	bool is_root() const{ return is_root_; }
    void is_root(bool b){ is_root_ = b; }
	boost::spirit::parser_id id() const { return parser_id_; }
    void id(boost::spirit::parser_id r){ parser_id_ = r; }
    ValueT const& value() const { return value_; }
    void value(ValueT const& v) { value_ = v; }
};


//----------------------------------------------------------------------------
template <typename AttrT = boost::spirit::nil_t> struct FIPTMatch;

template <typename T>
struct FIPTMatchAttr
{
    template <typename MatchT>
    static T get(MatchT const& m) { return T(m.value()); }
	static T get(FIPTMatch<boost::spirit::nil_t> const& /*m*/) { return T(); }
    static T get_default() { return T(); }
};

//////////////////////////////////
template <>
struct FIPTMatchAttr<boost::spirit::nil_t>{
    template <typename MatchT>
	static boost::spirit::nil_t get(MatchT const& /*m*/) { return boost::spirit::nil_t(); }
    static boost::spirit::nil_t get_default() { return boost::spirit::nil_t(); }
};

template <typename T>
inline void cp_swap(T& t1, T& t2) {
	using std::swap;
    using boost::swap;
    swap(t1, t2);
}

struct FIPTNode{
	typedef std::vector<FIPTNode> container_t;
	container_t children;
	UTString text;
	boost::spirit::parser_id id;

	FIPTNode(){}
	template <typename ValueT>
	FIPTNode(const FIPTNodeValData<ValueT>& val){
		id = val.id();
		if (!val.is_root()){
			text.assign(val.begin(), val.end());
		}
//		std::cout << "FIPTNode<" << id.to_long() << "> (" << text << ")" << std::endl;
	}
	void Print(std::ostream& os) const;
	void GetTextR(UTString& t) const;
};

#if defined _MSC_VER && _MSC_VER <= 1200
template <typename AttrT = boost::spirit::nil_t>
#else
template <typename AttrT>
#endif
struct FIPTMatch : public boost::spirit::safe_bool<FIPTMatch<AttrT> >{
    typedef AttrT attr_t;
    typedef typename boost::call_traits<AttrT>::param_type      param_type;
    typedef typename boost::call_traits<AttrT>::reference       reference;
    typedef typename boost::call_traits<AttrT>::const_reference const_reference;
	typedef FIPTNode tree_t;
	typedef std::vector<tree_t> container_t;
	typedef FIPTNodeValData<AttrT> parse_node_t;

    int len;
    mutable container_t trees;
    AttrT   val;
	
	FIPTMatch():len(-1), trees(), val(FIPTMatchAttr<AttrT>::get_default()) {}
	explicit FIPTMatch(unsigned length): len(length), trees(), val(FIPTMatchAttr<AttrT>::get_default()){}
    FIPTMatch(unsigned length, parse_node_t const& n):len(length), trees(), val(FIPTMatchAttr<AttrT>::get_default()){
		trees.push_back(FIPTNode(n));
    }
    FIPTMatch(unsigned length, param_type val_, parse_node_t const& n):len(length), trees(), val(val_){
        trees.push_back(FIPTNode(n));
    }
    template <typename T2>
	FIPTMatch(FIPTMatch<T2> const& other):len(other.length()), trees(), val(FIPTMatchAttr<AttrT>::get(other)){
		cp_swap(trees, other.trees);
	}
    FIPTMatch(FIPTMatch const& x) : len(x.len), trees(), val(FIPTMatchAttr<AttrT>::get_default()){
        // use auto_ptr like ownership for the trees data member
		cp_swap(trees, x.trees);
    }

    template <typename T2>
    FIPTMatch& operator=(FIPTMatch<T2> const& other) {
        len = other.length();
        val = FIPTMatchAttr<AttrT>::get(other);
        cp_swap(trees, other.trees);
        return *this;
    }
    FIPTMatch& operator=(FIPTMatch const& x){
        FIPTMatch tmp(x);
        this->swap(tmp);
		return *this;
    }

    void swap(FIPTMatch& x){
        cp_swap(len, x.len);
        cp_swap(trees, x.trees);
    }

    //operator boost::spirit::impl::safe_bool() const { 
	//	return BOOST_SPIRIT_SAFE_BOOL(len >= 0); 
	//}
	bool operator_bool(){
		return len >= 0;
	}

    bool operator!() const { return len < 0; }

    int length() const              { return len; }
    const_reference value() const   { return val; }
    reference value()               { return val; }

    template <typename MatchT>
    void concat(MatchT const& other){
        BOOST_SPIRIT_ASSERT(*this && other);
        len += other.length();
        std::copy(other.trees.begin(), other.trees.end(),
            std::back_insert_iterator<typename FIPTMatch::container_t>(trees));
    }

};


//	for making parse tree
struct FIPTMatchPolicy{
    template <typename T>
	struct result { typedef FIPTMatch<T> type; };

    const FIPTMatch<boost::spirit::nil_t>
	no_match() const { return FIPTMatch<boost::spirit::nil_t>(); }

    const FIPTMatch<boost::spirit::nil_t>
    empty_match() const { return FIPTMatch<boost::spirit::nil_t>(0); }

    template <typename AttrT>
    FIPTMatch<AttrT>
    create_match(unsigned length, AttrT const& val, FIIteratorT const& first, FIIteratorT const& last) const{
		return FIPTMatch<AttrT>(length, val, create_node(length, val, first, last, true));
	}
	template <typename AttrT>
	FIPTNodeValData<AttrT> create_node(int length, AttrT const& val, FIIteratorT first, FIIteratorT last, bool leaf_node) const{
		if (leaf_node){
			return FIPTNodeValData<AttrT>(first, last);
		}else{
			return FIPTNodeValData<AttrT>(NULL, NULL);
		}
	}

    template <typename MatchT>
    void
    group_match(MatchT& m, boost::spirit::parser_id const& id,
        FIIteratorT const& first, FIIteratorT const& last) const {
        if (!m) return;
		FIPTNodeValData<typename MatchT::attr_t> newVal(first, last, true, id);
		MatchT newmatch(m.length(), newVal);
        std::swap(newmatch.trees.begin()->children, m.trees);
        m = newmatch;
    }

    template <typename Match1T, typename Match2T>
    void
    concat_match(Match1T& l, Match2T const& r) const{
		l.concat(r);
	}
};

///	スキャナを固定するためのツリー用スキップパーサ．他のパーサを代入できる
struct FIPTSkipParser: boost::spirit::parser< FIPTSkipParser >{
	typedef FIPTSkipParser          self_t;
	typedef FIPTSkipParser const&   embed_t;
	typedef boost::spirit::skip_parser_iteration_policy< FIPTSkipParser >
		iter_policy_t;
	typedef boost::spirit::scanner_policies<
		iter_policy_t, FIPTMatchPolicy
	> scanner_policies_t;
	///	本文(Phrase)のためのスキャナ型
    typedef boost::spirit::scanner<FIIteratorT, scanner_policies_t> PhraseScannerT;

	typedef boost::spirit::scanner_policies<
		boost::spirit::no_skipper_iteration_policy<PhraseScannerT::iteration_policy_t>,
		PhraseScannerT::match_policy_t,
		PhraseScannerT::action_policy_t
    > policies_t;
	///	スペース読み飛ばし(Skipper)のためのスキャナ型
    typedef boost::spirit::scanner<FIIteratorT, policies_t> SkipperScannerT;

	//	SkipperScannerTでパーサを定義していく．
	typedef boost::spirit::match_result<SkipperScannerT, boost::spirit::parser_context<>::context_linker_t::attr_t>::type Result;
    FIPTSkipParser(){}
    FIPTSkipParser(const FIPTSkipParser& wp): ptr_(wp.ptr_? wp.ptr_->clone() : NULL){}
    ///	Scannerがテンプレートのパーサを受け取って，コンクリートなパーサに変換して保存．
	template <class T>
    FIPTSkipParser(const T& p){
		ptr_.reset(DBG_NEW boost::spirit::impl::concrete_parser<T, SkipperScannerT, Result>(p));
	}
    ///	Scannerがテンプレートのパーサを受け取って，コンクリートなパーサに変換して保存．
	template <class T>
    FIPTSkipParser& operator=(const T& p){
	    ptr_.reset(DBG_NEW boost::spirit::impl::concrete_parser<T, SkipperScannerT, Result>(p));
		return *this;
    }
    ///	スキップのためのパース
	Result parse(const SkipperScannerT& scan)const{
		if (ptr_) return ptr_->do_parse_virtual(scan);
		else return scan.no_match();
	}
	///	コンクリートなパーサ
	boost::scoped_ptr<boost::spirit::impl::abstract_parser<SkipperScannerT, Result> > ptr_;
};

///	ツリー用本文パーサ．他のパーサを代入できる
struct FIPTPhraseParser: public boost::spirit::parser< FIPTPhraseParser >{
	typedef FIPTPhraseParser          self_t;
	typedef FIPTPhraseParser const&   embed_t;

	///	本文(Phrase)のためのスキャナ型
	typedef FIPTSkipParser::PhraseScannerT PhraseScannerT;
	typedef boost::spirit::match_result<PhraseScannerT, boost::spirit::parser_context<>::context_linker_t::attr_t>::type Result;

	FIPTPhraseParser(){}
	FIPTPhraseParser(FIPTPhraseParser const& fp): ptr_(fp.ptr_? fp.ptr_->clone() : NULL){}
	template <typename ParserT>
	FIPTPhraseParser& operator=(ParserT const& p){
	    ptr_.reset(DBG_NEW boost::spirit::impl::concrete_parser<ParserT, PhraseScannerT, Result>(p));
		return *this;
	}
    ///	パース
	Result parse(const PhraseScannerT& scan)const{
		Result r;
		if (ptr_) r=ptr_->do_parse_virtual(scan);
		else r=scan.no_match();
		return r;
	}
	///	コンクリートなパーサ
	boost::scoped_ptr<boost::spirit::impl::abstract_parser<PhraseScannerT, Result> > ptr_;
};

extern int FIPTPhraseParserWithID_indent;

template <class TagT>
struct FIPTPhraseParserWithID: public boost::spirit::parser< FIPTPhraseParserWithID<TagT> >{
	typedef FIPTPhraseParserWithID<TagT>          self_t;
	typedef FIPTPhraseParserWithID<TagT> const&   embed_t;

	///	本文(Phrase)のためのスキャナ型
	typedef FIPTSkipParser::PhraseScannerT PhraseScannerT;
	typedef boost::spirit::match_result<PhraseScannerT, boost::spirit::parser_context<>::context_linker_t::attr_t>::type Result;

	FIPTPhraseParserWithID(){}
	FIPTPhraseParserWithID(FIPTPhraseParserWithID const& fp): ptr_(fp.ptr_? fp.ptr_->clone() : NULL){}
	template <typename ParserT>
	FIPTPhraseParserWithID& operator=(ParserT const& p){
	    ptr_.reset(DBG_NEW boost::spirit::impl::concrete_parser<ParserT, PhraseScannerT, Result>(p));
		return *this;
	}
	Result parse(const PhraseScannerT& scan)const{
/*		int& w = FIPTPhraseParserWithID_indent;
		std::cout.width(0);
		std::cout << UTPadding(w) << "FIPTPP with ID " << TagT::id(*this).to_long() << std::endl;
		w += 2;
*/
		Result r;

		if (ptr_){
			FIIteratorT f = scan.first;
			r = ptr_->do_parse_virtual(scan);
			FIIteratorT l = scan.first;
			scan.group_match(r, TagT::id(*this), f, l);
		}else{
			r=scan.no_match();
		}
		
		/*
		w -= 2;
		std::cout.width(0);
		std::cout << UTPadding(w) << "-";
		if (!r) std::cout << "failed.";
		std::cout << std::endl;
*/
		return r;
	}
	///	コンクリートなパーサ
	boost::scoped_ptr<boost::spirit::impl::abstract_parser<PhraseScannerT, Result> > ptr_;
};

struct FIPTInfo {
	typedef FIPTPhraseParser ParserT;
    FIIteratorT   stop;
    bool        match;
    bool        full;
    unsigned    length;
    mutable FIPTMatch<ParserT>::container_t trees;

    FIPTInfo(): stop(), match(false), full(false), length(0), trees(){}

    FIPTInfo(FIPTInfo const& pi): stop(pi.stop), match(pi.match), full(pi.full), length(pi.length), trees(){
        using std::swap;
        using boost::swap;
		// use auto_ptr like ownership for the trees data member
		swap(trees, pi.trees);
    }

    FIPTInfo(FIIteratorT stop_, bool match_, bool full_, unsigned length_,
		FIPTMatch<ParserT>::container_t trees_)
		:stop(stop_), match(match_), full(full_), length(length_), trees(){
        using std::swap;
        using boost::swap;
        // use auto_ptr like ownership for the trees data member
		swap(trees, trees_);
	}

	void Print(std::ostream& os) const;
};

FIPTInfo FIPTParse(FIIteratorT const& first_, FIIteratorT const& last,
	FIPTPhraseParser const&  p, FIPTSkipParser const& skip);

}	//	namespace Spr


#undef BOOST_SPIRIT_IT_NS

#if defined(__BORLANDC__)
# pragma option pop
#endif

#endif
