#include "FISpirit.h"

#if defined(__BORLANDC__)
# pragma option push -pc
#endif

namespace Spr{;

int FIPTPhraseParserWithID_indent;

FIPTInfo FIPTParse(FIIteratorT const& first_, FIIteratorT const& last, 
	FIPTPhraseParser const&  p, FIPTSkipParser const& skip){
	FIPTSkipParser::iter_policy_t iter_policy(skip);
	FIPTSkipParser::scanner_policies_t policies(iter_policy);
	FIIteratorT first = first_;
	FIPTSkipParser::PhraseScannerT scan(first, last, policies);
	FIPTPhraseParser::Result hit = p.parse(scan);
    scan.skip(scan);
    return FIPTInfo(first, (bool)hit, hit && (first == last), hit.length(), hit.trees);
}


void FIPTInfo::Print(std::ostream& os) const{
	for(unsigned i=0; i<trees.size(); ++i){
		trees[i].Print(os);
	}
}
void FIPTNode::Print(std::ostream& os) const {
	static int w;
	os.width(0);
	os << UTPadding(w);
	os << text << ": " << id.to_long() << std::endl;
	w += 2;
	for(unsigned i=0; i< children.size(); ++i){
		children[i].Print(os);
	}
	w -= 2;
}
void FIPTNode::GetTextR(FIString& t) const {
	t.append(text);
	for(unsigned i=0; i< children.size(); ++i){
		children[i].GetTextR(t);
	}
}


}

#if defined(__BORLANDC__)
# pragma option pop
#endif


