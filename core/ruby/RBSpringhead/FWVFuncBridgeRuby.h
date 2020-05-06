#include <Framework/SprFWApp.h>
#include <ruby.h>

namespace Spr{;

class FWVFuncBridgeRuby : public FWVFuncBridge{
	VALUE self;
public:
	virtual void Link(void* pObj){
		self = (VALUE)pObj;
	}
	virtual bool Display(){
		int id = rb_intern("Display");
		if(rb_respond_to(self, id)){
			rb_funcall2(self, id, 0, NULL);
			return true;
		}
		return false;
	}
	virtual bool Reshape(int w, int h){
		int id = rb_intern("Reshape");
		if(rb_respond_to(self, id)){
			VALUE args[2];
			args[0] = INT2FIX(w);
			args[1] = INT2FIX(h);
			rb_funcall2(self, id, 2, args);
			return true;
		}
		return false;
	}
	virtual bool Keyboard(unsigned char key, int x, int y){
		int id = rb_intern("Reshape");
		if(rb_respond_to(self, id)){
			VALUE args[3];
			args[0] = CHR2FIX(key);
			args[1] = INT2FIX(x);
			args[2] = INT2FIX(y);
			rb_funcall2(self, id, 3, args);
			return true;
		}
		return false;
	}
	virtual bool MouseButton(int button, int state, int x, int y){
		int id = rb_intern("MouseButton");
		if(rb_respond_to(self, id)){
			VALUE args[4];
			args[0] = INT2FIX(button);
			args[1] = INT2FIX(state);
			args[2] = INT2FIX(x);
			args[3] = INT2FIX(y);
			rb_funcall2(self, id, 4, args);
			return true;
		}
		return false;
	}
	virtual bool MouseMove(int x, int y){
		int id = rb_intern("MouseMove");
		if(rb_respond_to(self, id)){
			VALUE args[2];
			args[0] = INT2FIX(x);
			args[1] = INT2FIX(y);
			rb_funcall2(self, id, 2, args);
			return true;
		}
		return false;
	}
	virtual bool Step(){
		int id = rb_intern("Step");
		if(rb_respond_to(self, id)){
			rb_funcall2(self, id, 0, NULL);
			return true;
		}
		return false;
	}
	virtual void AtExit(){
		rb_eval_string("exit");
	}
};

}