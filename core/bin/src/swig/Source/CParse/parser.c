
/*  A Bison parser, made from parser.y
    by GNU Bison version 1.28  */

#define YYBISON 1  /* Identify Bison output.  */

#define	ID	257
#define	HBLOCK	258
#define	POUND	259
#define	STRING	260
#define	INCLUDE	261
#define	IMPORT	262
#define	INSERT	263
#define	CHARCONST	264
#define	NUM_INT	265
#define	NUM_FLOAT	266
#define	NUM_UNSIGNED	267
#define	NUM_LONG	268
#define	NUM_ULONG	269
#define	NUM_LONGLONG	270
#define	NUM_ULONGLONG	271
#define	NUM_BOOL	272
#define	TYPEDEF	273
#define	TYPE_INT	274
#define	TYPE_UNSIGNED	275
#define	TYPE_SHORT	276
#define	TYPE_LONG	277
#define	TYPE_FLOAT	278
#define	TYPE_DOUBLE	279
#define	TYPE_CHAR	280
#define	TYPE_WCHAR	281
#define	TYPE_VOID	282
#define	TYPE_SIGNED	283
#define	TYPE_BOOL	284
#define	TYPE_COMPLEX	285
#define	TYPE_TYPEDEF	286
#define	TYPE_RAW	287
#define	TYPE_NON_ISO_INT8	288
#define	TYPE_NON_ISO_INT16	289
#define	TYPE_NON_ISO_INT32	290
#define	TYPE_NON_ISO_INT64	291
#define	LPAREN	292
#define	RPAREN	293
#define	COMMA	294
#define	SEMI	295
#define	EXTERN	296
#define	INIT	297
#define	LBRACE	298
#define	RBRACE	299
#define	PERIOD	300
#define	CONST_QUAL	301
#define	VOLATILE	302
#define	REGISTER	303
#define	STRUCT	304
#define	UNION	305
#define	EQUAL	306
#define	SIZEOF	307
#define	MODULE	308
#define	LBRACKET	309
#define	RBRACKET	310
#define	ILLEGAL	311
#define	CONSTANT	312
#define	NAME	313
#define	RENAME	314
#define	NAMEWARN	315
#define	EXTEND	316
#define	PRAGMA	317
#define	FEATURE	318
#define	VARARGS	319
#define	ENUM	320
#define	CLASS	321
#define	TYPENAME	322
#define	PRIVATE	323
#define	PUBLIC	324
#define	PROTECTED	325
#define	COLON	326
#define	STATIC	327
#define	VIRTUAL	328
#define	FRIEND	329
#define	THROW	330
#define	CATCH	331
#define	EXPLICIT	332
#define	USING	333
#define	NAMESPACE	334
#define	NATIVE	335
#define	INLINE	336
#define	TYPEMAP	337
#define	EXCEPT	338
#define	ECHO	339
#define	APPLY	340
#define	CLEAR	341
#define	SWIGTEMPLATE	342
#define	FRAGMENT	343
#define	WARN	344
#define	LESSTHAN	345
#define	GREATERTHAN	346
#define	DELETE_KW	347
#define	LESSTHANOREQUALTO	348
#define	GREATERTHANOREQUALTO	349
#define	EQUALTO	350
#define	NOTEQUALTO	351
#define	QUESTIONMARK	352
#define	TYPES	353
#define	PARMS	354
#define	NONID	355
#define	DSTAR	356
#define	DCNOT	357
#define	TEMPLATE	358
#define	OPERATOR	359
#define	COPERATOR	360
#define	PARSETYPE	361
#define	PARSEPARM	362
#define	PARSEPARMS	363
#define	CAST	364
#define	LOR	365
#define	LAND	366
#define	OR	367
#define	XOR	368
#define	AND	369
#define	LSHIFT	370
#define	RSHIFT	371
#define	PLUS	372
#define	MINUS	373
#define	STAR	374
#define	SLASH	375
#define	MODULO	376
#define	UMINUS	377
#define	NOT	378
#define	LNOT	379
#define	DCOLON	380

#line 16 "parser.y"


#define yylex yylex

char cvsroot_parser_y[] = "$Id: parser.y 12666 2011-05-14 00:13:43Z wsfulton $";

#include "swig.h"
#include "cparse.h"
#include "preprocessor.h"
#include <ctype.h>

/* We do this for portability */
#undef alloca
#define alloca malloc

/* -----------------------------------------------------------------------------
 *                               Externals
 * ----------------------------------------------------------------------------- */

int  yyparse();

/* NEW Variables */

static Node    *top = 0;      /* Top of the generated parse tree */
static int      unnamed = 0;  /* Unnamed datatype counter */
static Hash    *extendhash = 0;     /* Hash table of added methods */
static Hash    *classes = 0;        /* Hash table of classes */
static Symtab  *prev_symtab = 0;
static Node    *current_class = 0;
String  *ModuleName = 0;
static Node    *module_node = 0;
static String  *Classprefix = 0;  
static String  *Namespaceprefix = 0;
static int      inclass = 0;
static int      nested_template = 0; /* template class/function definition within a class */
static char    *last_cpptype = 0;
static int      inherit_list = 0;
static Parm    *template_parameters = 0;
static int      extendmode   = 0;
static int      compact_default_args = 0;
static int      template_reduce = 0;
static int      cparse_externc = 0;

static int      max_class_levels = 0;
static int      class_level = 0;
static Node   **class_decl = NULL;

/* -----------------------------------------------------------------------------
 *                            Assist Functions
 * ----------------------------------------------------------------------------- */


 
/* Called by the parser (yyparse) when an error is found.*/
static void yyerror (const char *e) {
  (void)e;
}

static Node *new_node(const_String_or_char_ptr tag) {
  Node *n = NewHash();
  set_nodeType(n,tag);
  Setfile(n,cparse_file);
  Setline(n,cparse_line);
  return n;
}

/* Copies a node.  Does not copy tree links or symbol table data (except for
   sym:name) */

static Node *copy_node(Node *n) {
  Node *nn;
  Iterator k;
  nn = NewHash();
  Setfile(nn,Getfile(n));
  Setline(nn,Getline(n));
  for (k = First(n); k.key; k = Next(k)) {
    String *ci;
    String *key = k.key;
    char *ckey = Char(key);
    if ((strcmp(ckey,"nextSibling") == 0) ||
	(strcmp(ckey,"previousSibling") == 0) ||
	(strcmp(ckey,"parentNode") == 0) ||
	(strcmp(ckey,"lastChild") == 0)) {
      continue;
    }
    if (Strncmp(key,"csym:",5) == 0) continue;
    /* We do copy sym:name.  For templates */
    if ((strcmp(ckey,"sym:name") == 0) || 
	(strcmp(ckey,"sym:weak") == 0) ||
	(strcmp(ckey,"sym:typename") == 0)) {
      String *ci = Copy(k.item);
      Setattr(nn,key, ci);
      Delete(ci);
      continue;
    }
    if (strcmp(ckey,"sym:symtab") == 0) {
      Setattr(nn,"sym:needs_symtab", "1");
    }
    /* We don't copy any other symbol table attributes */
    if (strncmp(ckey,"sym:",4) == 0) {
      continue;
    }
    /* If children.  We copy them recursively using this function */
    if (strcmp(ckey,"firstChild") == 0) {
      /* Copy children */
      Node *cn = k.item;
      while (cn) {
	Node *copy = copy_node(cn);
	appendChild(nn,copy);
	Delete(copy);
	cn = nextSibling(cn);
      }
      continue;
    }
    /* We don't copy the symbol table.  But we drop an attribute 
       requires_symtab so that functions know it needs to be built */

    if (strcmp(ckey,"symtab") == 0) {
      /* Node defined a symbol table. */
      Setattr(nn,"requires_symtab","1");
      continue;
    }
    /* Can't copy nodes */
    if (strcmp(ckey,"node") == 0) {
      continue;
    }
    if ((strcmp(ckey,"parms") == 0) || (strcmp(ckey,"pattern") == 0) || (strcmp(ckey,"throws") == 0)
	|| (strcmp(ckey,"kwargs") == 0)) {
      ParmList *pl = CopyParmList(k.item);
      Setattr(nn,key,pl);
      Delete(pl);
      continue;
    }
    /* Looks okay.  Just copy the data using Copy */
    ci = Copy(k.item);
    Setattr(nn, key, ci);
    Delete(ci);
  }
  return nn;
}

/* -----------------------------------------------------------------------------
 *                              Variables
 * ----------------------------------------------------------------------------- */

static char  *typemap_lang = 0;    /* Current language setting */

static int cplus_mode  = 0;
static String  *class_rename = 0;

/* C++ modes */

#define  CPLUS_PUBLIC    1
#define  CPLUS_PRIVATE   2
#define  CPLUS_PROTECTED 3

/* include types */
static int   import_mode = 0;

void SWIG_typemap_lang(const char *tm_lang) {
  typemap_lang = Swig_copy_string(tm_lang);
}

void SWIG_cparse_set_compact_default_args(int defargs) {
  compact_default_args = defargs;
}

int SWIG_cparse_template_reduce(int treduce) {
  template_reduce = treduce;
  return treduce;  
}

/* -----------------------------------------------------------------------------
 *                           Assist functions
 * ----------------------------------------------------------------------------- */

static int promote_type(int t) {
  if (t <= T_UCHAR || t == T_CHAR) return T_INT;
  return t;
}

/* Perform type-promotion for binary operators */
static int promote(int t1, int t2) {
  t1 = promote_type(t1);
  t2 = promote_type(t2);
  return t1 > t2 ? t1 : t2;
}

static String *yyrename = 0;

/* Forward renaming operator */

static String *resolve_node_scope(String *cname);


Hash *Swig_cparse_features(void) {
  static Hash   *features_hash = 0;
  if (!features_hash) features_hash = NewHash();
  return features_hash;
}

static String *feature_identifier_fix(String *s) {
  String *tp = SwigType_istemplate_templateprefix(s);
  if (tp) {
    String *ts, *ta, *tq;
    ts = SwigType_templatesuffix(s);
    ta = SwigType_templateargs(s);
    tq = Swig_symbol_type_qualify(ta,0);
    Append(tp,tq);
    Append(tp,ts);
    Delete(ts);
    Delete(ta);
    Delete(tq);
    return tp;
  } else {
    return NewString(s);
  }
}

/* Generate the symbol table name for an object */
/* This is a bit of a mess. Need to clean up */
static String *add_oldname = 0;



static String *make_name(Node *n, String *name,SwigType *decl) {
  int destructor = name && (*(Char(name)) == '~');

  if (yyrename) {
    String *s = NewString(yyrename);
    Delete(yyrename);
    yyrename = 0;
    if (destructor  && (*(Char(s)) != '~')) {
      Insert(s,0,"~");
    }
    return s;
  }

  if (!name) return 0;
  return Swig_name_make(n,Namespaceprefix,name,decl,add_oldname);
}

/* Generate an unnamed identifier */
static String *make_unnamed() {
  unnamed++;
  return NewStringf("$unnamed%d$",unnamed);
}

/* Return if the node is a friend declaration */
static int is_friend(Node *n) {
  return Cmp(Getattr(n,"storage"),"friend") == 0;
}

static int is_operator(String *name) {
  return Strncmp(name,"operator ", 9) == 0;
}


/* Add declaration list to symbol table */
static int  add_only_one = 0;

static void add_symbols(Node *n) {
  String *decl;
  String *wrn = 0;

  if (nested_template) {
    if (!(n && Equal(nodeType(n), "template"))) {
      return;
    }
    /* continue if template function, but not template class, declared within a class */
  }

  if (inclass && n) {
    cparse_normalize_void(n);
  }
  while (n) {
    String *symname = 0;
    /* for friends, we need to pop the scope once */
    String *old_prefix = 0;
    Symtab *old_scope = 0;
    int isfriend = inclass && is_friend(n);
    int iscdecl = Cmp(nodeType(n),"cdecl") == 0;
    int only_csymbol = 0;
    if (extendmode) {
      Setattr(n,"isextension","1");
    }
    
    if (inclass) {
      String *name = Getattr(n, "name");
      if (isfriend) {
	/* for friends, we need to add the scopename if needed */
	String *prefix = name ? Swig_scopename_prefix(name) : 0;
	old_prefix = Namespaceprefix;
	old_scope = Swig_symbol_popscope();
	Namespaceprefix = Swig_symbol_qualifiedscopename(0);
	if (!prefix) {
	  if (name && !is_operator(name) && Namespaceprefix) {
	    String *nname = NewStringf("%s::%s", Namespaceprefix, name);
	    Setattr(n,"name",nname);
	    Delete(nname);
	  }
	} else {
	  Symtab *st = Swig_symbol_getscope(prefix);
	  String *ns = st ? Getattr(st,"name") : prefix;
	  String *base  = Swig_scopename_last(name);
	  String *nname = NewStringf("%s::%s", ns, base);
	  Setattr(n,"name",nname);
	  Delete(nname);
	  Delete(base);
	  Delete(prefix);
	}
	Namespaceprefix = 0;
      } else {
	/* for member functions, we need to remove the redundant
	   class scope if provided, as in
	   
	   struct Foo {
	   int Foo::method(int a);
	   };
	   
	*/
	String *prefix = name ? Swig_scopename_prefix(name) : 0;
	if (prefix) {
	  if (Classprefix && (Equal(prefix,Classprefix))) {
	    String *base = Swig_scopename_last(name);
	    Setattr(n,"name",base);
	    Delete(base);
	  }
	  Delete(prefix);
	}

        /*
	if (!Getattr(n,"parentNode") && class_level) set_parentNode(n,class_decl[class_level - 1]);
        */
	Setattr(n,"ismember","1");
      }
    }
    if (!isfriend && inclass) {
      if ((cplus_mode != CPLUS_PUBLIC)) {
	only_csymbol = 1;
	if (cplus_mode == CPLUS_PROTECTED) {
	  Setattr(n,"access", "protected");
	  only_csymbol = !Swig_need_protected(n);
	} else {
	  Setattr(n,"access", "private");
	  /* private are needed only when they are pure virtuals - why? */
	  if ((Cmp(Getattr(n,"storage"),"virtual") == 0) && (Cmp(Getattr(n,"value"),"0") == 0)) {
	    only_csymbol = 0;
	  }
	}
      } else {
	  Setattr(n,"access", "public");
      }
    }
    if (Getattr(n,"sym:name")) {
      n = nextSibling(n);
      continue;
    }
    decl = Getattr(n,"decl");
    if (!SwigType_isfunction(decl)) {
      String *name = Getattr(n,"name");
      String *makename = Getattr(n,"parser:makename");
      if (iscdecl) {	
	String *storage = Getattr(n, "storage");
	if (Cmp(storage,"typedef") == 0) {
	  Setattr(n,"kind","typedef");
	} else {
	  SwigType *type = Getattr(n,"type");
	  String *value = Getattr(n,"value");
	  Setattr(n,"kind","variable");
	  if (value && Len(value)) {
	    Setattr(n,"hasvalue","1");
	  }
	  if (type) {
	    SwigType *ty;
	    SwigType *tmp = 0;
	    if (decl) {
	      ty = tmp = Copy(type);
	      SwigType_push(ty,decl);
	    } else {
	      ty = type;
	    }
	    if (!SwigType_ismutable(ty)) {
	      SetFlag(n,"hasconsttype");
	      SetFlag(n,"feature:immutable");
	    }
	    if (tmp) Delete(tmp);
	  }
	  if (!type) {
	    Printf(stderr,"notype name %s\n", name);
	  }
	}
      }
      Swig_features_get(Swig_cparse_features(), Namespaceprefix, name, 0, n);
      if (makename) {
	symname = make_name(n, makename,0);
        Delattr(n,"parser:makename"); /* temporary information, don't leave it hanging around */
      } else {
        makename = name;
	symname = make_name(n, makename,0);
      }
      
      if (!symname) {
	symname = Copy(Getattr(n,"unnamed"));
      }
      if (symname) {
	wrn = Swig_name_warning(n, Namespaceprefix, symname,0);
      }
    } else {
      String *name = Getattr(n,"name");
      SwigType *fdecl = Copy(decl);
      SwigType *fun = SwigType_pop_function(fdecl);
      if (iscdecl) {	
	Setattr(n,"kind","function");
      }
      
      Swig_features_get(Swig_cparse_features(),Namespaceprefix,name,fun,n);

      symname = make_name(n, name,fun);
      wrn = Swig_name_warning(n, Namespaceprefix,symname,fun);
      
      Delete(fdecl);
      Delete(fun);
      
    }
    if (!symname) {
      n = nextSibling(n);
      continue;
    }
    if (only_csymbol || GetFlag(n,"feature:ignore")) {
      /* Only add to C symbol table and continue */
      Swig_symbol_add(0, n);
    } else if (strncmp(Char(symname),"$ignore",7) == 0) {
      char *c = Char(symname)+7;
      SetFlag(n,"feature:ignore");
      if (strlen(c)) {
	SWIG_WARN_NODE_BEGIN(n);
	Swig_warning(0,Getfile(n), Getline(n), "%s\n",c+1);
	SWIG_WARN_NODE_END(n);
      }
      Swig_symbol_add(0, n);
    } else {
      Node *c;
      if ((wrn) && (Len(wrn))) {
	String *metaname = symname;
	if (!Getmeta(metaname,"already_warned")) {
	  SWIG_WARN_NODE_BEGIN(n);
	  Swig_warning(0,Getfile(n),Getline(n), "%s\n", wrn);
	  SWIG_WARN_NODE_END(n);
	  Setmeta(metaname,"already_warned","1");
	}
      }
      c = Swig_symbol_add(symname,n);

      if (c != n) {
        /* symbol conflict attempting to add in the new symbol */
        if (Getattr(n,"sym:weak")) {
          Setattr(n,"sym:name",symname);
        } else {
          String *e = NewStringEmpty();
          String *en = NewStringEmpty();
          String *ec = NewStringEmpty();
          int redefined = Swig_need_redefined_warn(n,c,inclass);
          if (redefined) {
            Printf(en,"Identifier '%s' redefined (ignored)",symname);
            Printf(ec,"previous definition of '%s'",symname);
          } else {
            Printf(en,"Redundant redeclaration of '%s'",symname);
            Printf(ec,"previous declaration of '%s'",symname);
          }
          if (Cmp(symname,Getattr(n,"name"))) {
            Printf(en," (Renamed from '%s')", SwigType_namestr(Getattr(n,"name")));
          }
          Printf(en,",");
          if (Cmp(symname,Getattr(c,"name"))) {
            Printf(ec," (Renamed from '%s')", SwigType_namestr(Getattr(c,"name")));
          }
          Printf(ec,".");
	  SWIG_WARN_NODE_BEGIN(n);
          if (redefined) {
            Swig_warning(WARN_PARSE_REDEFINED,Getfile(n),Getline(n),"%s\n",en);
            Swig_warning(WARN_PARSE_REDEFINED,Getfile(c),Getline(c),"%s\n",ec);
          } else if (!is_friend(n) && !is_friend(c)) {
            Swig_warning(WARN_PARSE_REDUNDANT,Getfile(n),Getline(n),"%s\n",en);
            Swig_warning(WARN_PARSE_REDUNDANT,Getfile(c),Getline(c),"%s\n",ec);
          }
	  SWIG_WARN_NODE_END(n);
          Printf(e,"%s:%d:%s\n%s:%d:%s\n",Getfile(n),Getline(n),en,
                 Getfile(c),Getline(c),ec);
          Setattr(n,"error",e);
	  Delete(e);
          Delete(en);
          Delete(ec);
        }
      }
    }
    /* restore the class scope if needed */
    if (isfriend) {
      Swig_symbol_setscope(old_scope);
      if (old_prefix) {
	Delete(Namespaceprefix);
	Namespaceprefix = old_prefix;
      }
    }
    Delete(symname);

    if (add_only_one) return;
    n = nextSibling(n);
  }
}


/* add symbols a parse tree node copy */

static void add_symbols_copy(Node *n) {
  String *name;
  int    emode = 0;
  while (n) {
    char *cnodeType = Char(nodeType(n));

    if (strcmp(cnodeType,"access") == 0) {
      String *kind = Getattr(n,"kind");
      if (Strcmp(kind,"public") == 0) {
	cplus_mode = CPLUS_PUBLIC;
      } else if (Strcmp(kind,"private") == 0) {
	cplus_mode = CPLUS_PRIVATE;
      } else if (Strcmp(kind,"protected") == 0) {
	cplus_mode = CPLUS_PROTECTED;
      }
      n = nextSibling(n);
      continue;
    }

    add_oldname = Getattr(n,"sym:name");
    if ((add_oldname) || (Getattr(n,"sym:needs_symtab"))) {
      int old_inclass = -1;
      Node *old_current_class = 0;
      if (add_oldname) {
	DohIncref(add_oldname);
	/*  Disable this, it prevents %rename to work with templates */
	/* If already renamed, we used that name  */
	/*
	if (Strcmp(add_oldname, Getattr(n,"name")) != 0) {
	  Delete(yyrename);
	  yyrename = Copy(add_oldname);
	}
	*/
      }
      Delattr(n,"sym:needs_symtab");
      Delattr(n,"sym:name");

      add_only_one = 1;
      add_symbols(n);

      if (Getattr(n,"partialargs")) {
	Swig_symbol_cadd(Getattr(n,"partialargs"),n);
      }
      add_only_one = 0;
      name = Getattr(n,"name");
      if (Getattr(n,"requires_symtab")) {
	Swig_symbol_newscope();
	Swig_symbol_setscopename(name);
	Delete(Namespaceprefix);
	Namespaceprefix = Swig_symbol_qualifiedscopename(0);
      }
      if (strcmp(cnodeType,"class") == 0) {
	old_inclass = inclass;
	inclass = 1;
	old_current_class = current_class;
	current_class = n;
	if (Strcmp(Getattr(n,"kind"),"class") == 0) {
	  cplus_mode = CPLUS_PRIVATE;
	} else {
	  cplus_mode = CPLUS_PUBLIC;
	}
      }
      if (strcmp(cnodeType,"extend") == 0) {
	emode = cplus_mode;
	cplus_mode = CPLUS_PUBLIC;
      }
      add_symbols_copy(firstChild(n));
      if (strcmp(cnodeType,"extend") == 0) {
	cplus_mode = emode;
      }
      if (Getattr(n,"requires_symtab")) {
	Setattr(n,"symtab", Swig_symbol_popscope());
	Delattr(n,"requires_symtab");
	Delete(Namespaceprefix);
	Namespaceprefix = Swig_symbol_qualifiedscopename(0);
      }
      if (add_oldname) {
	Delete(add_oldname);
	add_oldname = 0;
      }
      if (strcmp(cnodeType,"class") == 0) {
	inclass = old_inclass;
	current_class = old_current_class;
      }
    } else {
      if (strcmp(cnodeType,"extend") == 0) {
	emode = cplus_mode;
	cplus_mode = CPLUS_PUBLIC;
      }
      add_symbols_copy(firstChild(n));
      if (strcmp(cnodeType,"extend") == 0) {
	cplus_mode = emode;
      }
    }
    n = nextSibling(n);
  }
}

/* Extension merge.  This function is used to handle the %extend directive
   when it appears before a class definition.   To handle this, the %extend
   actually needs to take precedence.  Therefore, we will selectively nuke symbols
   from the current symbol table, replacing them with the added methods */

static void merge_extensions(Node *cls, Node *am) {
  Node *n;
  Node *csym;

  n = firstChild(am);
  while (n) {
    String *symname;
    if (Strcmp(nodeType(n),"constructor") == 0) {
      symname = Getattr(n,"sym:name");
      if (symname) {
	if (Strcmp(symname,Getattr(n,"name")) == 0) {
	  /* If the name and the sym:name of a constructor are the same,
             then it hasn't been renamed.  However---the name of the class
             itself might have been renamed so we need to do a consistency
             check here */
	  if (Getattr(cls,"sym:name")) {
	    Setattr(n,"sym:name", Getattr(cls,"sym:name"));
	  }
	}
      } 
    }

    symname = Getattr(n,"sym:name");
    DohIncref(symname);
    if ((symname) && (!Getattr(n,"error"))) {
      /* Remove node from its symbol table */
      Swig_symbol_remove(n);
      csym = Swig_symbol_add(symname,n);
      if (csym != n) {
	/* Conflict with previous definition.  Nuke previous definition */
	String *e = NewStringEmpty();
	String *en = NewStringEmpty();
	String *ec = NewStringEmpty();
	Printf(ec,"Identifier '%s' redefined by %%extend (ignored),",symname);
	Printf(en,"%%extend definition of '%s'.",symname);
	SWIG_WARN_NODE_BEGIN(n);
	Swig_warning(WARN_PARSE_REDEFINED,Getfile(csym),Getline(csym),"%s\n",ec);
	Swig_warning(WARN_PARSE_REDEFINED,Getfile(n),Getline(n),"%s\n",en);
	SWIG_WARN_NODE_END(n);
	Printf(e,"%s:%d:%s\n%s:%d:%s\n",Getfile(csym),Getline(csym),ec, 
	       Getfile(n),Getline(n),en);
	Setattr(csym,"error",e);
	Delete(e);
	Delete(en);
	Delete(ec);
	Swig_symbol_remove(csym);              /* Remove class definition */
	Swig_symbol_add(symname,n);            /* Insert extend definition */
      }
    }
    n = nextSibling(n);
  }
}

static void append_previous_extension(Node *cls, Node *am) {
  Node *n, *ne;
  Node *pe = 0;
  Node *ae = 0;

  if (!am) return;
  
  n = firstChild(am);
  while (n) {
    ne = nextSibling(n);
    set_nextSibling(n,0);
    /* typemaps and fragments need to be prepended */
    if (((Cmp(nodeType(n),"typemap") == 0) || (Cmp(nodeType(n),"fragment") == 0)))  {
      if (!pe) pe = new_node("extend");
      appendChild(pe, n);
    } else {
      if (!ae) ae = new_node("extend");
      appendChild(ae, n);
    }    
    n = ne;
  }
  if (pe) prependChild(cls,pe);
  if (ae) appendChild(cls,ae);
}
 

/* Check for unused %extend.  Special case, don't report unused
   extensions for templates */
 
static void check_extensions() {
  Iterator ki;

  if (!extendhash) return;
  for (ki = First(extendhash); ki.key; ki = Next(ki)) {
    if (!Strchr(ki.key,'<')) {
      SWIG_WARN_NODE_BEGIN(ki.item);
      Swig_warning(WARN_PARSE_EXTEND_UNDEF,Getfile(ki.item), Getline(ki.item), "%%extend defined for an undeclared class %s.\n", ki.key);
      SWIG_WARN_NODE_END(ki.item);
    }
  }
}

/* Check a set of declarations to see if any are pure-abstract */

static List *pure_abstract(Node *n) {
  List *abs = 0;
  while (n) {
    if (Cmp(nodeType(n),"cdecl") == 0) {
      String *decl = Getattr(n,"decl");
      if (SwigType_isfunction(decl)) {
	String *init = Getattr(n,"value");
	if (Cmp(init,"0") == 0) {
	  if (!abs) {
	    abs = NewList();
	  }
	  Append(abs,n);
	  Setattr(n,"abstract","1");
	}
      }
    } else if (Cmp(nodeType(n),"destructor") == 0) {
      if (Cmp(Getattr(n,"value"),"0") == 0) {
	if (!abs) {
	  abs = NewList();
	}
	Append(abs,n);
	Setattr(n,"abstract","1");
      }
    }
    n = nextSibling(n);
  }
  return abs;
}

/* Make a classname */

static String *make_class_name(String *name) {
  String *nname = 0;
  String *prefix;
  if (Namespaceprefix) {
    nname= NewStringf("%s::%s", Namespaceprefix, name);
  } else {
    nname = NewString(name);
  }
  prefix = SwigType_istemplate_templateprefix(nname);
  if (prefix) {
    String *args, *qargs;
    args   = SwigType_templateargs(nname);
    qargs  = Swig_symbol_type_qualify(args,0);
    Append(prefix,qargs);
    Delete(nname);
    Delete(args);
    Delete(qargs);
    nname = prefix;
  }
  return nname;
}

static List *make_inherit_list(String *clsname, List *names) {
  int i, ilen;
  String *derived;
  List *bases = NewList();

  if (Namespaceprefix) derived = NewStringf("%s::%s", Namespaceprefix,clsname);
  else derived = NewString(clsname);

  ilen = Len(names);
  for (i = 0; i < ilen; i++) {
    Node *s;
    String *base;
    String *n = Getitem(names,i);
    /* Try to figure out where this symbol is */
    s = Swig_symbol_clookup(n,0);
    if (s) {
      while (s && (Strcmp(nodeType(s),"class") != 0)) {
	/* Not a class.  Could be a typedef though. */
	String *storage = Getattr(s,"storage");
	if (storage && (Strcmp(storage,"typedef") == 0)) {
	  String *nn = Getattr(s,"type");
	  s = Swig_symbol_clookup(nn,Getattr(s,"sym:symtab"));
	} else {
	  break;
	}
      }
      if (s && ((Strcmp(nodeType(s),"class") == 0) || (Strcmp(nodeType(s),"template") == 0))) {
	String *q = Swig_symbol_qualified(s);
	Append(bases,s);
	if (q) {
	  base = NewStringf("%s::%s", q, Getattr(s,"name"));
	  Delete(q);
	} else {
	  base = NewString(Getattr(s,"name"));
	}
      } else {
	base = NewString(n);
      }
    } else {
      base = NewString(n);
    }
    if (base) {
      Swig_name_inherit(base,derived);
      Delete(base);
    }
  }
  return bases;
}

/* If the class name is qualified.  We need to create or lookup namespace entries */

static Symtab *set_scope_to_global() {
  Symtab *symtab = Swig_symbol_global_scope();
  Swig_symbol_setscope(symtab);
  return symtab;
}
 
/* Remove the block braces, { and }, if the 'noblock' attribute is set.
 * Node *kw can be either a Hash or Parmlist. */
static String *remove_block(Node *kw, const String *inputcode) {
  String *modified_code = 0;
  while (kw) {
   String *name = Getattr(kw,"name");
   if (name && (Cmp(name,"noblock") == 0)) {
     char *cstr = Char(inputcode);
     size_t len = Len(inputcode);
     if (len && cstr[0] == '{') {
       --len; ++cstr; 
       if (len && cstr[len - 1] == '}') { --len; }
       /* we now remove the extra spaces */
       while (len && isspace((int)cstr[0])) { --len; ++cstr; }
       while (len && isspace((int)cstr[len - 1])) { --len; }
       modified_code = NewStringWithSize(cstr, len);
       break;
     }
   }
   kw = nextSibling(kw);
  }
  return modified_code;
}


static Node *nscope = 0;
static Node *nscope_inner = 0;

/* Remove the scope prefix from cname and return the base name without the prefix.
 * The scopes specified in the prefix are found, or created in the current namespace.
 * So ultimately the scope is changed to that required for the base name.
 * For example AA::BB::CC as input returns CC and creates the namespace AA then inner 
 * namespace BB in the current scope. If no scope separator (::) in the input, then nothing happens! */
static String *resolve_node_scope(String *cname) {
  Symtab *gscope = 0;
  nscope = 0;
  nscope_inner = 0;  
  if (Swig_scopename_check(cname)) {
    Node   *ns;
    String *prefix = Swig_scopename_prefix(cname);
    String *base = Swig_scopename_last(cname);
    if (prefix && (Strncmp(prefix,"::",2) == 0)) {
      /* Use the global scope */
      String *nprefix = NewString(Char(prefix)+2);
      Delete(prefix);
      prefix= nprefix;
      gscope = set_scope_to_global();
    }    
    if (!prefix || (Len(prefix) == 0)) {
      /* Use the global scope, but we need to add a 'global' namespace.  */
      if (!gscope) gscope = set_scope_to_global();
      /* note that this namespace is not the "unnamed" one,
	 and we don't use Setattr(nscope,"name", ""),
	 because the unnamed namespace is private */
      nscope = new_node("namespace");
      Setattr(nscope,"symtab", gscope);;
      nscope_inner = nscope;
      return base;
    }
    /* Try to locate the scope */
    ns = Swig_symbol_clookup(prefix,0);
    if (!ns) {
      Swig_error(cparse_file,cparse_line,"Undefined scope '%s'\n", prefix);
    } else {
      Symtab *nstab = Getattr(ns,"symtab");
      if (!nstab) {
	Swig_error(cparse_file,cparse_line,
		   "'%s' is not defined as a valid scope.\n", prefix);
	ns = 0;
      } else {
	/* Check if the node scope is the current scope */
	String *tname = Swig_symbol_qualifiedscopename(0);
	String *nname = Swig_symbol_qualifiedscopename(nstab);
	if (tname && (Strcmp(tname,nname) == 0)) {
	  ns = 0;
	  cname = base;
	}
	Delete(tname);
	Delete(nname);
      }
      if (ns) {
	/* we will try to create a new node using the namespaces we
	   can find in the scope name */
	List *scopes;
	String *sname;
	Iterator si;
	String *name = NewString(prefix);
	scopes = NewList();
	while (name) {
	  String *base = Swig_scopename_last(name);
	  String *tprefix = Swig_scopename_prefix(name);
	  Insert(scopes,0,base);
	  Delete(base);
	  Delete(name);
	  name = tprefix;
	}
	for (si = First(scopes); si.item; si = Next(si)) {
	  Node *ns1,*ns2;
	  sname = si.item;
	  ns1 = Swig_symbol_clookup(sname,0);
	  assert(ns1);
	  if (Strcmp(nodeType(ns1),"namespace") == 0) {
	    if (Getattr(ns1,"alias")) {
	      ns1 = Getattr(ns1,"namespace");
	    }
	  } else {
	    /* now this last part is a class */
	    si = Next(si);
	    ns1 = Swig_symbol_clookup(sname,0);
	    /*  or a nested class tree, which is unrolled here */
	    for (; si.item; si = Next(si)) {
	      if (si.item) {
		Printf(sname,"::%s",si.item);
	      }
	    }
	    /* we get the 'inner' class */
	    nscope_inner = Swig_symbol_clookup(sname,0);
	    /* set the scope to the inner class */
	    Swig_symbol_setscope(Getattr(nscope_inner,"symtab"));
	    /* save the last namespace prefix */
	    Delete(Namespaceprefix);
	    Namespaceprefix = Swig_symbol_qualifiedscopename(0);
	    /* and return the node name, including the inner class prefix */
	    break;
	  }
	  /* here we just populate the namespace tree as usual */
	  ns2 = new_node("namespace");
	  Setattr(ns2,"name",sname);
	  Setattr(ns2,"symtab", Getattr(ns1,"symtab"));
	  add_symbols(ns2);
	  Swig_symbol_setscope(Getattr(ns1,"symtab"));
	  Delete(Namespaceprefix);
	  Namespaceprefix = Swig_symbol_qualifiedscopename(0);
	  if (nscope_inner) {
	    if (Getattr(nscope_inner,"symtab") != Getattr(ns2,"symtab")) {
	      appendChild(nscope_inner,ns2);
	      Delete(ns2);
	    }
	  }
	  nscope_inner = ns2;
	  if (!nscope) nscope = ns2;
	}
	cname = base;
	Delete(scopes);
      }
    }
    Delete(prefix);
  }
  return cname;
}
 


/* Structures for handling code fragments built for nested classes */

typedef struct Nested {
  String   *code;        /* Associated code fragment */
  int      line;         /* line number where it starts */
  const char *name;      /* Name associated with this nested class */
  const char *kind;      /* Kind of class */
  int      unnamed;      /* unnamed class */
  SwigType *type;        /* Datatype associated with the name */
  struct Nested   *next; /* Next code fragment in list */
} Nested;

/* Some internal variables for saving nested class information */

static Nested      *nested_list = 0;

/* Add a function to the nested list */

static void add_nested(Nested *n) {
  if (!nested_list) {
    nested_list = n;
  } else {
    Nested *n1 = nested_list;
    while (n1->next)
      n1 = n1->next;
    n1->next = n;
  }
}

/* -----------------------------------------------------------------------------
 * nested_new_struct()
 *
 * Nested struct handling for C code only creates a global struct from the nested struct.
 *
 * Nested structure. This is a sick "hack". If we encounter
 * a nested structure, we're going to grab the text of its definition and
 * feed it back into the scanner.  In the meantime, we need to grab
 * variable declaration information and generate the associated wrapper
 * code later.  Yikes!
 *
 * This really only works in a limited sense.   Since we use the
 * code attached to the nested class to generate both C code
 * it can't have any SWIG directives in it.  It also needs to be parsable
 * by SWIG or this whole thing is going to puke.
 * ----------------------------------------------------------------------------- */

static void nested_new_struct(const char *kind, String *struct_code, Node *cpp_opt_declarators) {
  String *name;
  String *decl;

  /* Create a new global struct declaration which is just a copy of the nested struct */
  Nested *nested = (Nested *) malloc(sizeof(Nested));
  Nested *n = nested;

  name = Getattr(cpp_opt_declarators, "name");
  decl = Getattr(cpp_opt_declarators, "decl");

  n->code = NewStringEmpty();
  Printv(n->code, "typedef ", kind, " ", struct_code, " $classname_", name, ";\n", NIL);
  n->name = Swig_copy_string(Char(name));
  n->line = cparse_start_line;
  n->type = NewStringEmpty();
  n->kind = kind;
  n->unnamed = 0;
  SwigType_push(n->type, decl);
  n->next = 0;

  /* Repeat for any multiple instances of the nested struct */
  {
    Node *p = cpp_opt_declarators;
    p = nextSibling(p);
    while (p) {
      Nested *nn = (Nested *) malloc(sizeof(Nested));

      name = Getattr(p, "name");
      decl = Getattr(p, "decl");

      nn->code = NewStringEmpty();
      Printv(nn->code, "typedef ", kind, " ", struct_code, " $classname_", name, ";\n", NIL);
      nn->name = Swig_copy_string(Char(name));
      nn->line = cparse_start_line;
      nn->type = NewStringEmpty();
      nn->kind = kind;
      nn->unnamed = 0;
      SwigType_push(nn->type, decl);
      nn->next = 0;
      n->next = nn;
      n = nn;
      p = nextSibling(p);
    }
  }

  add_nested(nested);
}

/* -----------------------------------------------------------------------------
 * nested_forward_declaration()
 * 
 * Nested struct handling for C++ code only.
 *
 * Treat the nested class/struct/union as a forward declaration until a proper 
 * nested class solution is implemented.
 * ----------------------------------------------------------------------------- */

static Node *nested_forward_declaration(const char *storage, const char *kind, String *sname, const char *name, Node *cpp_opt_declarators) {
  Node *nn = 0;
  int warned = 0;

  if (sname) {
    /* Add forward declaration of the nested type */
    Node *n = new_node("classforward");
    Setfile(n, cparse_file);
    Setline(n, cparse_line);
    Setattr(n, "kind", kind);
    Setattr(n, "name", sname);
    Setattr(n, "storage", storage);
    Setattr(n, "sym:weak", "1");
    add_symbols(n);
    nn = n;
  }

  /* Add any variable instances. Also add in any further typedefs of the nested type.
     Note that anonymous typedefs (eg typedef struct {...} a, b;) are treated as class forward declarations */
  if (cpp_opt_declarators) {
    int storage_typedef = (storage && (strcmp(storage, "typedef") == 0));
    int variable_of_anonymous_type = !sname && !storage_typedef;
    if (!variable_of_anonymous_type) {
      int anonymous_typedef = !sname && (storage && (strcmp(storage, "typedef") == 0));
      Node *n = cpp_opt_declarators;
      SwigType *type = NewString(name);
      while (n) {
	Setattr(n, "type", type);
	Setattr(n, "storage", storage);
	if (anonymous_typedef) {
	  Setattr(n, "nodeType", "classforward");
	  Setattr(n, "sym:weak", "1");
	}
	n = nextSibling(n);
      }
      Delete(type);
      add_symbols(cpp_opt_declarators);

      if (nn) {
	set_nextSibling(nn, cpp_opt_declarators);
      } else {
	nn = cpp_opt_declarators;
      }
    }
  }

  if (nn && Equal(nodeType(nn), "classforward")) {
    Node *n = nn;
    if (GetFlag(n, "feature:nestedworkaround")) {
      Swig_symbol_remove(n);
      nn = 0;
      warned = 1;
    } else {
      SWIG_WARN_NODE_BEGIN(n);
      Swig_warning(WARN_PARSE_NAMED_NESTED_CLASS, cparse_file, cparse_line,"Nested %s not currently supported (%s ignored)\n", kind, sname ? sname : name);
      SWIG_WARN_NODE_END(n);
      warned = 1;
    }
  }

  if (!warned)
    Swig_warning(WARN_PARSE_UNNAMED_NESTED_CLASS, cparse_file, cparse_line, "Nested %s not currently supported (ignored).\n", kind);

  return nn;
}

/* Strips C-style and C++-style comments from string in-place. */
static void strip_comments(char *string) {
  int state = 0; /* 
                  * 0 - not in comment
                  * 1 - in c-style comment
                  * 2 - in c++-style comment
                  * 3 - in string
                  * 4 - after reading / not in comments
                  * 5 - after reading * in c-style comments
                  * 6 - after reading \ in strings
                  */
  char * c = string;
  while (*c) {
    switch (state) {
    case 0:
      if (*c == '\"')
        state = 3;
      else if (*c == '/')
        state = 4;
      break;
    case 1:
      if (*c == '*')
        state = 5;
      *c = ' ';
      break;
    case 2:
      if (*c == '\n')
        state = 0;
      else
        *c = ' ';
      break;
    case 3:
      if (*c == '\"')
        state = 0;
      else if (*c == '\\')
        state = 6;
      break;
    case 4:
      if (*c == '/') {
        *(c-1) = ' ';
        *c = ' ';
        state = 2;
      } else if (*c == '*') {
        *(c-1) = ' ';
        *c = ' ';
        state = 1;
      } else
        state = 0;
      break;
    case 5:
      if (*c == '/')
        state = 0;
      else 
        state = 1;
      *c = ' ';
      break;
    case 6:
      state = 3;
      break;
    }
    ++c;
  }
}

/* Dump all of the nested class declarations to the inline processor
 * However.  We need to do a few name replacements and other munging
 * first.  This function must be called before closing a class! */

static Node *dump_nested(const char *parent) {
  Nested *n,*n1;
  Node *ret = 0;
  Node *last = 0;
  n = nested_list;
  if (!parent) {
    nested_list = 0;
    return 0;
  }
  while (n) {
    Node *retx;
    SwigType *nt;
    /* Token replace the name of the parent class */
    Replace(n->code, "$classname", parent, DOH_REPLACE_ANY);

    /* Fix up the name of the datatype (for building typedefs and other stuff) */
    Append(n->type,parent);
    Append(n->type,"_");
    Append(n->type,n->name);

    /* Add the appropriate declaration to the C++ processor */
    retx = new_node("cdecl");
    Setattr(retx,"name",n->name);
    nt = Copy(n->type);
    Setattr(retx,"type",nt);
    Delete(nt);
    Setattr(retx,"nested",parent);
    if (n->unnamed) {
      Setattr(retx,"unnamed","1");
    }
    
    add_symbols(retx);
    if (ret) {
      set_nextSibling(last, retx);
      Delete(retx);
    } else {
      ret = retx;
    }
    last = retx;

    /* Strip comments - further code may break in presence of comments. */
    strip_comments(Char(n->code));

    /* Make all SWIG created typedef structs/unions/classes unnamed else 
       redefinition errors occur - nasty hack alert.*/

    {
      const char* types_array[3] = {"struct", "union", "class"};
      int i;
      for (i=0; i<3; i++) {
	char* code_ptr = Char(n->code);
	while (code_ptr) {
	  /* Replace struct name (as in 'struct name {...}' ) with whitespace
	     name will be between struct and opening brace */
	
	  code_ptr = strstr(code_ptr, types_array[i]);
	  if (code_ptr) {
	    char *open_bracket_pos;
	    code_ptr += strlen(types_array[i]);
	    open_bracket_pos = strchr(code_ptr, '{');
	    if (open_bracket_pos) { 
	      /* Make sure we don't have something like struct A a; */
	      char* semi_colon_pos = strchr(code_ptr, ';');
	      if (!(semi_colon_pos && (semi_colon_pos < open_bracket_pos)))
		while (code_ptr < open_bracket_pos)
		  *code_ptr++ = ' ';
	    }
	  }
	}
      }
    }
    
    {
      /* Remove SWIG directive %constant which may be left in the SWIG created typedefs */
      char* code_ptr = Char(n->code);
      while (code_ptr) {
	code_ptr = strstr(code_ptr, "%constant");
	if (code_ptr) {
	  char* directive_end_pos = strchr(code_ptr, ';');
	  if (directive_end_pos) { 
            while (code_ptr <= directive_end_pos)
              *code_ptr++ = ' ';
	  }
	}
      }
    }
    {
      Node *newnode = new_node("insert");
      String *code = NewStringEmpty();
      Wrapper_pretty_print(n->code, code);
      Setattr(newnode,"code", code);
      Delete(code);
      set_nextSibling(last, newnode);
      Delete(newnode);      
      last = newnode;
    }
      
    /* Dump the code to the scanner */
    start_inline(Char(Getattr(last, "code")),n->line);

    n1 = n->next;
    Delete(n->code);
    free(n);
    n = n1;
  }
  nested_list = 0;
  return ret;
}

Node *Swig_cparse(File *f) {
  scanner_file(f);
  top = 0;
  yyparse();
  return top;
}

static void single_new_feature(const char *featurename, String *val, Hash *featureattribs, char *declaratorid, SwigType *type, ParmList *declaratorparms, String *qualifier) {
  String *fname;
  String *name;
  String *fixname;
  SwigType *t = Copy(type);

  /* Printf(stdout, "single_new_feature: [%s] [%s] [%s] [%s] [%s] [%s]\n", featurename, val, declaratorid, t, ParmList_str_defaultargs(declaratorparms), qualifier); */

  fname = NewStringf("feature:%s",featurename);
  if (declaratorid) {
    fixname = feature_identifier_fix(declaratorid);
  } else {
    fixname = NewStringEmpty();
  }
  if (Namespaceprefix) {
    name = NewStringf("%s::%s",Namespaceprefix, fixname);
  } else {
    name = fixname;
  }

  if (declaratorparms) Setmeta(val,"parms",declaratorparms);
  if (!Len(t)) t = 0;
  if (t) {
    if (qualifier) SwigType_push(t,qualifier);
    if (SwigType_isfunction(t)) {
      SwigType *decl = SwigType_pop_function(t);
      if (SwigType_ispointer(t)) {
	String *nname = NewStringf("*%s",name);
	Swig_feature_set(Swig_cparse_features(), nname, decl, fname, val, featureattribs);
	Delete(nname);
      } else {
	Swig_feature_set(Swig_cparse_features(), name, decl, fname, val, featureattribs);
      }
      Delete(decl);
    } else if (SwigType_ispointer(t)) {
      String *nname = NewStringf("*%s",name);
      Swig_feature_set(Swig_cparse_features(),nname,0,fname,val, featureattribs);
      Delete(nname);
    }
  } else {
    /* Global feature, that is, feature not associated with any particular symbol */
    Swig_feature_set(Swig_cparse_features(),name,0,fname,val, featureattribs);
  }
  Delete(fname);
  Delete(name);
}

/* Add a new feature to the Hash. Additional features are added if the feature has a parameter list (declaratorparms)
 * and one or more of the parameters have a default argument. An extra feature is added for each defaulted parameter,
 * simulating the equivalent overloaded method. */
static void new_feature(const char *featurename, String *val, Hash *featureattribs, char *declaratorid, SwigType *type, ParmList *declaratorparms, String *qualifier) {

  ParmList *declparms = declaratorparms;

  /* remove the { and } braces if the noblock attribute is set */
  String *newval = remove_block(featureattribs, val);
  val = newval ? newval : val;

  /* Add the feature */
  single_new_feature(featurename, val, featureattribs, declaratorid, type, declaratorparms, qualifier);

  /* Add extra features if there are default parameters in the parameter list */
  if (type) {
    while (declparms) {
      if (ParmList_has_defaultargs(declparms)) {

        /* Create a parameter list for the new feature by copying all
           but the last (defaulted) parameter */
        ParmList* newparms = CopyParmListMax(declparms, ParmList_len(declparms)-1);

        /* Create new declaration - with the last parameter removed */
        SwigType *newtype = Copy(type);
        Delete(SwigType_pop_function(newtype)); /* remove the old parameter list from newtype */
        SwigType_add_function(newtype,newparms);

        single_new_feature(featurename, Copy(val), featureattribs, declaratorid, newtype, newparms, qualifier);
        declparms = newparms;
      } else {
        declparms = 0;
      }
    }
  }
}

/* check if a function declaration is a plain C object */
static int is_cfunction(Node *n) {
  if (!cparse_cplusplus || cparse_externc) return 1;
  if (Cmp(Getattr(n,"storage"),"externc") == 0) {
    return 1;
  }
  return 0;
}

/* If the Node is a function with parameters, check to see if any of the parameters
 * have default arguments. If so create a new function for each defaulted argument. 
 * The additional functions form a linked list of nodes with the head being the original Node n. */
static void default_arguments(Node *n) {
  Node *function = n;

  if (function) {
    ParmList *varargs = Getattr(function,"feature:varargs");
    if (varargs) {
      /* Handles the %varargs directive by looking for "feature:varargs" and 
       * substituting ... with an alternative set of arguments.  */
      Parm     *p = Getattr(function,"parms");
      Parm     *pp = 0;
      while (p) {
	SwigType *t = Getattr(p,"type");
	if (Strcmp(t,"v(...)") == 0) {
	  if (pp) {
	    ParmList *cv = Copy(varargs);
	    set_nextSibling(pp,cv);
	    Delete(cv);
	  } else {
	    ParmList *cv =  Copy(varargs);
	    Setattr(function,"parms", cv);
	    Delete(cv);
	  }
	  break;
	}
	pp = p;
	p = nextSibling(p);
      }
    }

    /* Do not add in functions if kwargs is being used or if user wants old default argument wrapping
       (one wrapped method per function irrespective of number of default arguments) */
    if (compact_default_args 
	|| is_cfunction(function) 
	|| GetFlag(function,"feature:compactdefaultargs") 
	|| GetFlag(function,"feature:kwargs")) {
      ParmList *p = Getattr(function,"parms");
      if (p) 
        Setattr(p,"compactdefargs", "1"); /* mark parameters for special handling */
      function = 0; /* don't add in extra methods */
    }
  }

  while (function) {
    ParmList *parms = Getattr(function,"parms");
    if (ParmList_has_defaultargs(parms)) {

      /* Create a parameter list for the new function by copying all
         but the last (defaulted) parameter */
      ParmList* newparms = CopyParmListMax(parms,ParmList_len(parms)-1);

      /* Create new function and add to symbol table */
      {
	SwigType *ntype = Copy(nodeType(function));
	char *cntype = Char(ntype);
        Node *new_function = new_node(ntype);
        SwigType *decl = Copy(Getattr(function,"decl"));
        int constqualifier = SwigType_isconst(decl);
	String *ccode = Copy(Getattr(function,"code"));
	String *cstorage = Copy(Getattr(function,"storage"));
	String *cvalue = Copy(Getattr(function,"value"));
	SwigType *ctype = Copy(Getattr(function,"type"));
	String *cthrow = Copy(Getattr(function,"throw"));

        Delete(SwigType_pop_function(decl)); /* remove the old parameter list from decl */
        SwigType_add_function(decl,newparms);
        if (constqualifier)
          SwigType_add_qualifier(decl,"const");

        Setattr(new_function,"name", Getattr(function,"name"));
        Setattr(new_function,"code", ccode);
        Setattr(new_function,"decl", decl);
        Setattr(new_function,"parms", newparms);
        Setattr(new_function,"storage", cstorage);
        Setattr(new_function,"value", cvalue);
        Setattr(new_function,"type", ctype);
        Setattr(new_function,"throw", cthrow);

	Delete(ccode);
	Delete(cstorage);
	Delete(cvalue);
	Delete(ctype);
	Delete(cthrow);
	Delete(decl);

        {
          Node *throws = Getattr(function,"throws");
	  ParmList *pl = CopyParmList(throws);
          if (throws) Setattr(new_function,"throws",pl);
	  Delete(pl);
        }

        /* copy specific attributes for global (or in a namespace) template functions - these are not templated class methods */
        if (strcmp(cntype,"template") == 0) {
          Node *templatetype = Getattr(function,"templatetype");
          Node *symtypename = Getattr(function,"sym:typename");
          Parm *templateparms = Getattr(function,"templateparms");
          if (templatetype) {
	    Node *tmp = Copy(templatetype);
	    Setattr(new_function,"templatetype",tmp);
	    Delete(tmp);
	  }
          if (symtypename) {
	    Node *tmp = Copy(symtypename);
	    Setattr(new_function,"sym:typename",tmp);
	    Delete(tmp);
	  }
          if (templateparms) {
	    Parm *tmp = CopyParmList(templateparms);
	    Setattr(new_function,"templateparms",tmp);
	    Delete(tmp);
	  }
        } else if (strcmp(cntype,"constructor") == 0) {
          /* only copied for constructors as this is not a user defined feature - it is hard coded in the parser */
          if (GetFlag(function,"feature:new")) SetFlag(new_function,"feature:new");
        }

        add_symbols(new_function);
        /* mark added functions as ones with overloaded parameters and point to the parsed method */
        Setattr(new_function,"defaultargs", n);

        /* Point to the new function, extending the linked list */
        set_nextSibling(function, new_function);
	Delete(new_function);
        function = new_function;
	
	Delete(ntype);
      }
    } else {
      function = 0;
    }
  }
}

/* -----------------------------------------------------------------------------
 * tag_nodes()
 *
 * Used by the parser to mark subtypes with extra information.
 * ----------------------------------------------------------------------------- */

static void tag_nodes(Node *n, const_String_or_char_ptr attrname, DOH *value) {
  while (n) {
    Setattr(n, attrname, value);
    tag_nodes(firstChild(n), attrname, value);
    n = nextSibling(n);
  }
}


#line 1592 "parser.y"
typedef union {
  char  *id;
  List  *bases;
  struct Define {
    String *val;
    String *rawval;
    int     type;
    String *qualifier;
    String *bitfield;
    Parm   *throws;
    String *throwf;
  } dtype;
  struct {
    char *type;
    String *filename;
    int   line;
  } loc;
  struct {
    char      *id;
    SwigType  *type;
    String    *defarg;
    ParmList  *parms;
    short      have_parms;
    ParmList  *throws;
    String    *throwf;
  } decl;
  Parm         *tparms;
  struct {
    String     *method;
    Hash       *kwargs;
  } tmap;
  struct {
    String     *type;
    String     *us;
  } ptype;
  SwigType     *type;
  String       *str;
  Parm         *p;
  ParmList     *pl;
  int           intvalue;
  Node         *node;
} YYSTYPE;
#include <stdio.h>

#ifndef __cplusplus
#ifndef __STDC__
#define const
#endif
#endif



#define	YYFINAL		907
#define	YYFLAG		-32768
#define	YYNTBASE	127

#define YYTRANSLATE(x) ((unsigned)(x) <= 380 ? yytranslate[x] : 274)

static const short yytranslate[] = {     0,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     1,     3,     4,     5,     6,
     7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
    17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
    27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
    37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
    47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
    57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
    67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
    77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
    87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
    97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
   107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
   117,   118,   119,   120,   121,   122,   123,   124,   125,   126
};

#if YYDEBUG != 0
static const short yyprhs[] = {     0,
     0,     2,     6,     9,    13,    16,    22,    26,    29,    31,
    33,    35,    37,    39,    41,    43,    46,    48,    50,    52,
    54,    56,    58,    60,    62,    64,    66,    68,    70,    72,
    74,    76,    78,    80,    82,    84,    86,    88,    89,    97,
   103,   107,   113,   119,   123,   126,   129,   135,   138,   144,
   147,   152,   154,   156,   164,   172,   178,   179,   187,   189,
   191,   194,   197,   199,   205,   211,   217,   221,   226,   230,
   238,   247,   253,   257,   259,   261,   265,   267,   272,   280,
   287,   289,   291,   299,   309,   318,   329,   335,   343,   350,
   359,   361,   363,   369,   374,   380,   388,   390,   394,   401,
   408,   417,   419,   422,   426,   428,   431,   435,   442,   448,
   458,   461,   463,   465,   467,   468,   475,   481,   483,   488,
   490,   492,   495,   501,   508,   513,   521,   531,   538,   540,
   542,   544,   546,   548,   550,   551,   561,   562,   572,   574,
   578,   583,   584,   591,   595,   597,   599,   601,   603,   605,
   607,   609,   612,   614,   616,   618,   622,   624,   628,   633,
   634,   641,   642,   648,   654,   657,   658,   665,   667,   669,
   670,   674,   676,   678,   680,   682,   684,   686,   688,   690,
   694,   696,   698,   700,   702,   704,   706,   708,   710,   712,
   719,   726,   734,   743,   752,   762,   770,   776,   779,   782,
   785,   786,   794,   795,   802,   804,   806,   808,   810,   812,
   814,   816,   818,   820,   822,   824,   827,   830,   833,   838,
   841,   847,   849,   852,   854,   856,   858,   860,   862,   864,
   866,   869,   871,   875,   877,   880,   888,   892,   894,   897,
   899,   903,   905,   907,   909,   912,   918,   921,   924,   926,
   929,   932,   934,   936,   938,   940,   943,   947,   949,   952,
   956,   961,   967,   972,   974,   977,   981,   986,   992,   996,
  1001,  1006,  1008,  1011,  1016,  1021,  1027,  1031,  1036,  1041,
  1043,  1046,  1049,  1053,  1055,  1058,  1060,  1063,  1067,  1072,
  1076,  1081,  1084,  1088,  1092,  1097,  1101,  1105,  1108,  1111,
  1113,  1115,  1118,  1120,  1122,  1124,  1126,  1129,  1131,  1134,
  1138,  1140,  1142,  1144,  1147,  1150,  1152,  1154,  1157,  1159,
  1161,  1164,  1166,  1168,  1170,  1172,  1174,  1176,  1178,  1180,
  1182,  1184,  1186,  1188,  1190,  1192,  1193,  1196,  1198,  1200,
  1204,  1206,  1208,  1212,  1214,  1216,  1218,  1220,  1222,  1224,
  1230,  1232,  1234,  1238,  1243,  1249,  1255,  1262,  1265,  1268,
  1270,  1272,  1274,  1276,  1278,  1280,  1282,  1284,  1288,  1292,
  1296,  1300,  1304,  1308,  1312,  1316,  1320,  1324,  1328,  1332,
  1336,  1340,  1344,  1348,  1354,  1357,  1360,  1363,  1366,  1369,
  1371,  1372,  1376,  1378,  1380,  1384,  1385,  1389,  1390,  1396,
  1398,  1400,  1402,  1404,  1406,  1408,  1410,  1412,  1414,  1416,
  1418,  1423,  1429,  1431,  1435,  1439,  1444,  1449,  1453,  1456,
  1458,  1460,  1464,  1467,  1471,  1473,  1475,  1477,  1479,  1481,
  1484,  1489,  1491,  1495,  1497,  1501,  1505,  1508,  1511,  1514,
  1517,  1520,  1525,  1527,  1531,  1533,  1537,  1541,  1544,  1547,
  1550,  1553,  1555,  1557,  1559,  1561,  1565,  1567,  1571,  1577,
  1579,  1583,  1587,  1593,  1595,  1597
};

static const short yyrhs[] = {   128,
     0,   107,   212,    41,     0,   107,     1,     0,   108,   212,
    41,     0,   108,     1,     0,   109,    38,   209,    39,    41,
     0,   109,     1,    41,     0,   128,   129,     0,   273,     0,
   130,     0,   167,     0,   175,     0,    41,     0,     1,     0,
   174,     0,     1,   106,     0,   131,     0,   133,     0,   134,
     0,   135,     0,   136,     0,   137,     0,   140,     0,   141,
     0,   144,     0,   145,     0,   146,     0,   147,     0,   148,
     0,   149,     0,   152,     0,   154,     0,   157,     0,   159,
     0,   164,     0,   165,     0,   166,     0,     0,    62,   270,
   263,    44,   132,   192,    45,     0,    86,   163,    44,   161,
    45,     0,    87,   161,    41,     0,    58,     3,    52,   234,
    41,     0,    58,   228,   220,   217,    41,     0,    58,     1,
    41,     0,    85,     4,     0,    85,   268,     0,    84,    38,
     3,    39,    44,     0,    84,    44,     0,    84,    38,     3,
    39,    41,     0,    84,    41,     0,   268,    44,   212,    45,
     0,   268,     0,   138,     0,    89,    38,   139,    40,   271,
    39,     4,     0,    89,    38,   139,    40,   271,    39,    44,
     0,    89,    38,   139,    39,    41,     0,     0,   143,   270,
   268,    55,   142,   128,    56,     0,     7,     0,     8,     0,
    82,     4,     0,    82,    44,     0,     4,     0,     9,    38,
   261,    39,   268,     0,     9,    38,   261,    39,     4,     0,
     9,    38,   261,    39,    44,     0,    54,   270,   261,     0,
    59,    38,   261,    39,     0,    59,    38,    39,     0,    81,
    38,     3,    39,   208,     3,    41,     0,    81,    38,     3,
    39,   208,   228,   220,    41,     0,    63,   151,     3,    52,
   150,     0,    63,   151,     3,     0,   268,     0,     4,     0,
    38,     3,    39,     0,   273,     0,   153,   220,   261,    41,
     0,   153,    38,   271,    39,   220,   255,    41,     0,   153,
    38,   271,    39,   268,    41,     0,    60,     0,    61,     0,
    64,    38,   261,    39,   220,   255,   155,     0,    64,    38,
   261,    40,   272,    39,   220,   255,    41,     0,    64,    38,
   261,   156,    39,   220,   255,   155,     0,    64,    38,   261,
    40,   272,   156,    39,   220,   255,    41,     0,    64,    38,
   261,    39,   155,     0,    64,    38,   261,    40,   272,    39,
    41,     0,    64,    38,   261,   156,    39,   155,     0,    64,
    38,   261,    40,   272,   156,    39,    41,     0,   269,     0,
    41,     0,   100,    38,   209,    39,    41,     0,    40,   261,
    52,   272,     0,    40,   261,    52,   272,   156,     0,    65,
    38,   158,    39,   220,   255,    41,     0,   209,     0,    11,
    40,   212,     0,    83,    38,   160,    39,   161,   269,     0,
    83,    38,   160,    39,   161,    41,     0,    83,    38,   160,
    39,   161,    52,   163,    41,     0,   271,     0,   163,   162,
     0,    40,   163,   162,     0,   273,     0,   228,   219,     0,
    38,   209,    39,     0,    38,   209,    39,    38,   209,    39,
     0,    99,    38,   209,    39,   155,     0,    88,    38,   262,
    39,   266,    91,   213,    92,    41,     0,    90,   268,     0,
   169,     0,   173,     0,   172,     0,     0,    42,   268,    44,
   168,   128,    45,     0,   208,   228,   220,   171,   170,     0,
    41,     0,    40,   220,   171,   170,     0,    44,     0,   217,
     0,   226,   217,     0,    76,    38,   209,    39,   217,     0,
   226,    76,    38,   209,    39,   217,     0,   208,    66,     3,
    41,     0,   208,    66,   236,    44,   237,    45,    41,     0,
   208,    66,   236,    44,   237,    45,   220,   171,   170,     0,
   208,   228,    38,   209,    39,   256,     0,   176,     0,   180,
     0,   181,     0,   188,     0,   189,     0,   199,     0,     0,
   208,   253,   263,   244,    44,   177,   192,    45,   179,     0,
     0,   208,   253,    44,   178,   192,    45,   220,   171,   170,
     0,    41,     0,   220,   171,   170,     0,   208,   253,   263,
    41,     0,     0,   104,    91,   184,    92,   182,   183,     0,
   104,   253,   263,     0,   169,     0,   176,     0,   196,     0,
   181,     0,   180,     0,   198,     0,   185,     0,   186,   187,
     0,   273,     0,   252,     0,   212,     0,    40,   186,   187,
     0,   273,     0,    79,   263,    41,     0,    79,    80,   263,
    41,     0,     0,    80,   263,    44,   190,   128,    45,     0,
     0,    80,    44,   191,   128,    45,     0,    80,     3,    52,
   263,    41,     0,   195,   192,     0,     0,    62,    44,   193,
   192,    45,   192,     0,   141,     0,   273,     0,     0,     1,
   194,   192,     0,   167,     0,   196,     0,   197,     0,   200,
     0,   204,     0,   198,     0,   180,     0,   201,     0,   208,
   263,    41,     0,   188,     0,   181,     0,   199,     0,   165,
     0,   166,     0,   207,     0,   140,     0,   164,     0,    41,
     0,   208,   228,    38,   209,    39,   256,     0,   124,   265,
    38,   209,    39,   205,     0,    74,   124,   265,    38,   209,
    39,   206,     0,   208,   106,   228,   225,    38,   209,    39,
   206,     0,   208,   106,   228,   115,    38,   209,    39,   206,
     0,   208,   106,   228,   225,   115,    38,   209,    39,   206,
     0,   208,   106,   228,    38,   209,    39,   206,     0,    77,
    38,   209,    39,    44,     0,    70,    72,     0,    69,    72,
     0,    71,    72,     0,     0,   208,   253,   263,   244,    44,
   202,   179,     0,     0,   208,   253,   244,    44,   203,   179,
     0,   149,     0,   135,     0,   147,     0,   152,     0,   154,
     0,   157,     0,   145,     0,   159,     0,   133,     0,   134,
     0,   136,     0,   255,    41,     0,   255,    44,     0,   255,
    41,     0,   255,    52,   234,    41,     0,   255,    44,     0,
   208,   228,    72,   240,    41,     0,    42,     0,    42,   268,
     0,    73,     0,    19,     0,    74,     0,    75,     0,    78,
     0,   273,     0,   210,     0,   212,   211,     0,   273,     0,
    40,   212,   211,     0,   273,     0,   229,   218,     0,   104,
    91,   253,    92,   253,   263,   217,     0,    46,    46,    46,
     0,   214,     0,   216,   215,     0,   273,     0,    40,   216,
   215,     0,   273,     0,   212,     0,   241,     0,    52,   234,
     0,    52,   234,    55,   240,    56,     0,    52,    44,     0,
    72,   240,     0,   273,     0,   220,   217,     0,   223,   217,
     0,   217,     0,   220,     0,   223,     0,   273,     0,   225,
   221,     0,   225,   115,   221,     0,   222,     0,   115,   221,
     0,   263,   102,   221,     0,   225,   263,   102,   221,     0,
   225,   263,   102,   115,   221,     0,   263,   102,   115,   221,
     0,   263,     0,   124,   263,     0,    38,   263,    39,     0,
    38,   225,   221,    39,     0,    38,   263,   102,   221,    39,
     0,   221,    55,    56,     0,   221,    55,   240,    56,     0,
   221,    38,   209,    39,     0,   263,     0,   124,   263,     0,
    38,   225,   222,    39,     0,    38,   115,   222,    39,     0,
    38,   263,   102,   222,    39,     0,   222,    55,    56,     0,
   222,    55,   240,    56,     0,   222,    38,   209,    39,     0,
   225,     0,   225,   224,     0,   225,   115,     0,   225,   115,
   224,     0,   224,     0,   115,   224,     0,   115,     0,   263,
   102,     0,   225,   263,   102,     0,   225,   263,   102,   224,
     0,   224,    55,    56,     0,   224,    55,   240,    56,     0,
    55,    56,     0,    55,   240,    56,     0,    38,   223,    39,
     0,   224,    38,   209,    39,     0,    38,   209,    39,     0,
   120,   226,   225,     0,   120,   225,     0,   120,   226,     0,
   120,     0,   227,     0,   227,   226,     0,    47,     0,    48,
     0,    49,     0,   229,     0,   226,   230,     0,   230,     0,
   230,   226,     0,   226,   230,   226,     0,   231,     0,    30,
     0,    28,     0,    32,   260,     0,    66,   263,     0,    33,
     0,   263,     0,   253,   263,     0,   232,     0,   233,     0,
   233,   232,     0,    20,     0,    22,     0,    23,     0,    26,
     0,    27,     0,    24,     0,    25,     0,    29,     0,    21,
     0,    31,     0,    34,     0,    35,     0,    36,     0,    37,
     0,     0,   235,   240,     0,     3,     0,   273,     0,   237,
    40,   238,     0,   238,     0,     3,     0,     3,    52,   239,
     0,   273,     0,   240,     0,   241,     0,   228,     0,   242,
     0,   268,     0,    53,    38,   228,   218,    39,     0,   243,
     0,    10,     0,    38,   240,    39,     0,    38,   240,    39,
   240,     0,    38,   240,   225,    39,   240,     0,    38,   240,
   115,    39,   240,     0,    38,   240,   225,   115,    39,   240,
     0,   115,   240,     0,   120,   240,     0,    11,     0,    12,
     0,    13,     0,    14,     0,    15,     0,    16,     0,    17,
     0,    18,     0,   240,   118,   240,     0,   240,   119,   240,
     0,   240,   120,   240,     0,   240,   121,   240,     0,   240,
   122,   240,     0,   240,   115,   240,     0,   240,   113,   240,
     0,   240,   114,   240,     0,   240,   116,   240,     0,   240,
   117,   240,     0,   240,   112,   240,     0,   240,   111,   240,
     0,   240,    96,   240,     0,   240,    97,   240,     0,   240,
    95,   240,     0,   240,    94,   240,     0,   240,    98,   240,
    72,   240,     0,   119,   240,     0,   118,   240,     0,   124,
   240,     0,   125,   240,     0,   228,    38,     0,   245,     0,
     0,    72,   246,   247,     0,   273,     0,   248,     0,   247,
    40,   248,     0,     0,   254,   249,   263,     0,     0,   254,
   251,   250,   254,   263,     0,    70,     0,    69,     0,    71,
     0,    67,     0,    68,     0,   252,     0,    50,     0,    51,
     0,    74,     0,   273,     0,   226,     0,    76,    38,   209,
    39,     0,   226,    76,    38,   209,    39,     0,   273,     0,
   255,   257,    41,     0,   255,   257,    44,     0,    38,   209,
    39,    41,     0,    38,   209,    39,    44,     0,    52,   234,
    41,     0,    72,   258,     0,   273,     0,   259,     0,   258,
    40,   259,     0,   263,    38,     0,    91,   213,    92,     0,
   273,     0,     3,     0,   268,     0,   261,     0,   273,     0,
   265,   264,     0,   101,   126,   265,   264,     0,   265,     0,
   101,   126,   265,     0,   105,     0,   101,   126,   105,     0,
   126,   265,   264,     0,   126,   265,     0,   126,   105,     0,
   103,   265,     0,     3,   260,     0,     3,   267,     0,   101,
   126,     3,   267,     0,     3,     0,   101,   126,     3,     0,
   105,     0,   101,   126,   105,     0,   126,     3,   267,     0,
   126,     3,     0,   126,   105,     0,   103,     3,     0,   268,
     6,     0,     6,     0,   268,     0,    44,     0,     4,     0,
    38,   271,    39,     0,   273,     0,   261,    52,   272,     0,
   261,    52,   272,    40,   271,     0,   261,     0,   261,    40,
   271,     0,   261,    52,   138,     0,   261,    52,   138,    40,
   271,     0,   268,     0,   242,     0,     0
};

#endif

#if YYDEBUG != 0
static const short yyrline[] = { 0,
  1745,  1758,  1762,  1765,  1768,  1771,  1774,  1779,  1784,  1789,
  1790,  1791,  1792,  1793,  1799,  1815,  1825,  1826,  1827,  1828,
  1829,  1830,  1831,  1832,  1833,  1834,  1835,  1836,  1837,  1838,
  1839,  1840,  1841,  1842,  1843,  1844,  1845,  1852,  1880,  1924,
  1934,  1945,  1966,  1988,  1999,  2008,  2027,  2033,  2039,  2044,
  2051,  2058,  2062,  2075,  2084,  2099,  2112,  2121,  2167,  2168,
  2175,  2194,  2225,  2229,  2239,  2244,  2262,  2302,  2308,  2321,
  2327,  2353,  2359,  2366,  2367,  2370,  2371,  2379,  2425,  2471,
  2482,  2485,  2512,  2518,  2524,  2530,  2538,  2544,  2550,  2556,
  2564,  2565,  2566,  2569,  2574,  2584,  2620,  2621,  2656,  2673,
  2681,  2694,  2719,  2725,  2729,  2732,  2743,  2748,  2761,  2773,
  3047,  3057,  3064,  3065,  3069,  3073,  3100,  3161,  3165,  3187,
  3193,  3199,  3205,  3211,  3224,  3239,  3249,  3327,  3378,  3379,
  3380,  3381,  3382,  3383,  3388,  3505,  3631,  3665,  3754,  3755,
  3767,  3787,  3792,  4076,  4082,  4085,  4088,  4091,  4094,  4097,
  4102,  4132,  4136,  4139,  4142,  4147,  4151,  4156,  4166,  4197,
  4215,  4226,  4238,  4248,  4275,  4290,  4294,  4300,  4301,  4302,
  4307,  4318,  4319,  4336,  4337,  4338,  4339,  4340,  4341,  4342,
  4343,  4344,  4345,  4346,  4347,  4348,  4349,  4350,  4351,  4360,
  4385,  4409,  4450,  4465,  4483,  4502,  4521,  4528,  4535,  4543,
  4564,  4568,  4590,  4594,  4626,  4629,  4633,  4636,  4637,  4638,
  4639,  4640,  4641,  4642,  4643,  4646,  4651,  4658,  4666,  4674,
  4685,  4691,  4692,  4700,  4701,  4702,  4703,  4704,  4705,  4712,
  4723,  4727,  4730,  4734,  4738,  4748,  4756,  4764,  4777,  4781,
  4784,  4788,  4792,  4820,  4828,  4839,  4853,  4862,  4870,  4880,
  4884,  4888,  4895,  4912,  4929,  4937,  4945,  4954,  4958,  4967,
  4978,  4990,  5000,  5013,  5020,  5028,  5044,  5052,  5063,  5074,
  5085,  5104,  5112,  5129,  5137,  5144,  5155,  5166,  5177,  5196,
  5202,  5208,  5215,  5224,  5227,  5236,  5243,  5250,  5260,  5271,
  5282,  5293,  5300,  5307,  5310,  5327,  5337,  5344,  5350,  5355,
  5361,  5365,  5371,  5372,  5373,  5379,  5385,  5389,  5390,  5394,
  5401,  5404,  5405,  5406,  5407,  5408,  5410,  5413,  5418,  5443,
  5446,  5500,  5504,  5508,  5512,  5516,  5520,  5524,  5528,  5532,
  5536,  5540,  5544,  5548,  5552,  5558,  5558,  5584,  5585,  5588,
  5601,  5609,  5617,  5627,  5630,  5644,  5645,  5664,  5665,  5669,
  5674,  5675,  5689,  5696,  5713,  5720,  5727,  5735,  5739,  5745,
  5746,  5747,  5748,  5749,  5750,  5751,  5752,  5755,  5759,  5763,
  5767,  5771,  5775,  5779,  5783,  5787,  5791,  5795,  5799,  5803,
  5807,  5821,  5825,  5829,  5835,  5839,  5843,  5847,  5851,  5867,
  5872,  5872,  5873,  5876,  5893,  5902,  5904,  5918,  5920,  5934,
  5935,  5936,  5940,  5944,  5950,  5953,  5957,  5963,  5964,  5967,
  5972,  5977,  5982,  5989,  5996,  6003,  6011,  6019,  6027,  6028,
  6031,  6032,  6035,  6041,  6047,  6050,  6051,  6054,  6055,  6058,
  6063,  6067,  6070,  6073,  6076,  6081,  6085,  6088,  6095,  6101,
  6110,  6115,  6119,  6122,  6125,  6128,  6133,  6137,  6140,  6143,
  6149,  6154,  6157,  6160,  6164,  6169,  6182,  6186,  6191,  6197,
  6201,  6206,  6210,  6217,  6220,  6225
};
#endif


#if YYDEBUG != 0 || defined (YYERROR_VERBOSE)

static const char * const yytname[] = {   "$","error","$undefined.","ID","HBLOCK",
"POUND","STRING","INCLUDE","IMPORT","INSERT","CHARCONST","NUM_INT","NUM_FLOAT",
"NUM_UNSIGNED","NUM_LONG","NUM_ULONG","NUM_LONGLONG","NUM_ULONGLONG","NUM_BOOL",
"TYPEDEF","TYPE_INT","TYPE_UNSIGNED","TYPE_SHORT","TYPE_LONG","TYPE_FLOAT","TYPE_DOUBLE",
"TYPE_CHAR","TYPE_WCHAR","TYPE_VOID","TYPE_SIGNED","TYPE_BOOL","TYPE_COMPLEX",
"TYPE_TYPEDEF","TYPE_RAW","TYPE_NON_ISO_INT8","TYPE_NON_ISO_INT16","TYPE_NON_ISO_INT32",
"TYPE_NON_ISO_INT64","LPAREN","RPAREN","COMMA","SEMI","EXTERN","INIT","LBRACE",
"RBRACE","PERIOD","CONST_QUAL","VOLATILE","REGISTER","STRUCT","UNION","EQUAL",
"SIZEOF","MODULE","LBRACKET","RBRACKET","ILLEGAL","CONSTANT","NAME","RENAME",
"NAMEWARN","EXTEND","PRAGMA","FEATURE","VARARGS","ENUM","CLASS","TYPENAME","PRIVATE",
"PUBLIC","PROTECTED","COLON","STATIC","VIRTUAL","FRIEND","THROW","CATCH","EXPLICIT",
"USING","NAMESPACE","NATIVE","INLINE","TYPEMAP","EXCEPT","ECHO","APPLY","CLEAR",
"SWIGTEMPLATE","FRAGMENT","WARN","LESSTHAN","GREATERTHAN","DELETE_KW","LESSTHANOREQUALTO",
"GREATERTHANOREQUALTO","EQUALTO","NOTEQUALTO","QUESTIONMARK","TYPES","PARMS",
"NONID","DSTAR","DCNOT","TEMPLATE","OPERATOR","COPERATOR","PARSETYPE","PARSEPARM",
"PARSEPARMS","CAST","LOR","LAND","OR","XOR","AND","LSHIFT","RSHIFT","PLUS","MINUS",
"STAR","SLASH","MODULO","UMINUS","NOT","LNOT","DCOLON","program","interface",
"declaration","swig_directive","extend_directive","@1","apply_directive","clear_directive",
"constant_directive","echo_directive","except_directive","stringtype","fname",
"fragment_directive","include_directive","@2","includetype","inline_directive",
"insert_directive","module_directive","name_directive","native_directive","pragma_directive",
"pragma_arg","pragma_lang","rename_directive","rename_namewarn","feature_directive",
"stringbracesemi","featattr","varargs_directive","varargs_parms","typemap_directive",
"typemap_type","tm_list","tm_tail","typemap_parm","types_directive","template_directive",
"warn_directive","c_declaration","@3","c_decl","c_decl_tail","initializer","c_enum_forward_decl",
"c_enum_decl","c_constructor_decl","cpp_declaration","cpp_class_decl","@4","@5",
"cpp_opt_declarators","cpp_forward_class_decl","cpp_template_decl","@6","cpp_temp_possible",
"template_parms","templateparameters","templateparameter","templateparameterstail",
"cpp_using_decl","cpp_namespace_decl","@7","@8","cpp_members","@9","@10","cpp_member",
"cpp_constructor_decl","cpp_destructor_decl","cpp_conversion_operator","cpp_catch_decl",
"cpp_protection_decl","cpp_nested","@11","@12","cpp_swig_directive","cpp_end",
"cpp_vend","anonymous_bitfield","storage_class","parms","rawparms","ptail","parm",
"valparms","rawvalparms","valptail","valparm","def_args","parameter_declarator",
"typemap_parameter_declarator","declarator","notso_direct_declarator","direct_declarator",
"abstract_declarator","direct_abstract_declarator","pointer","type_qualifier",
"type_qualifier_raw","type","rawtype","type_right","primitive_type","primitive_type_list",
"type_specifier","definetype","@13","ename","enumlist","edecl","etype","expr",
"valexpr","exprnum","exprcompound","inherit","raw_inherit","@14","base_list",
"base_specifier","@15","@16","access_specifier","templcpptype","cpptype","opt_virtual",
"cpp_const","ctor_end","ctor_initializer","mem_initializer_list","mem_initializer",
"template_decl","idstring","idstringopt","idcolon","idcolontail","idtemplate",
"idcolonnt","idcolontailnt","string","stringbrace","options","kwargs","stringnum",
"empty", NULL
};
#endif

static const short yyr1[] = {     0,
   127,   127,   127,   127,   127,   127,   127,   128,   128,   129,
   129,   129,   129,   129,   129,   129,   130,   130,   130,   130,
   130,   130,   130,   130,   130,   130,   130,   130,   130,   130,
   130,   130,   130,   130,   130,   130,   130,   132,   131,   133,
   134,   135,   135,   135,   136,   136,   137,   137,   137,   137,
   138,   139,   139,   140,   140,   140,   142,   141,   143,   143,
   144,   144,   145,   145,   145,   145,   146,   147,   147,   148,
   148,   149,   149,   150,   150,   151,   151,   152,   152,   152,
   153,   153,   154,   154,   154,   154,   154,   154,   154,   154,
   155,   155,   155,   156,   156,   157,   158,   158,   159,   159,
   159,   160,   161,   162,   162,   163,   163,   163,   164,   165,
   166,   167,   167,   167,   168,   167,   169,   170,   170,   170,
   171,   171,   171,   171,   172,   173,   173,   174,   175,   175,
   175,   175,   175,   175,   177,   176,   178,   176,   179,   179,
   180,   182,   181,   181,   183,   183,   183,   183,   183,   183,
   184,   185,   185,   186,   186,   187,   187,   188,   188,   190,
   189,   191,   189,   189,   192,   193,   192,   192,   192,   194,
   192,   195,   195,   195,   195,   195,   195,   195,   195,   195,
   195,   195,   195,   195,   195,   195,   195,   195,   195,   196,
   197,   197,   198,   198,   198,   198,   199,   200,   200,   200,
   202,   201,   203,   201,   204,   204,   204,   204,   204,   204,
   204,   204,   204,   204,   204,   205,   205,   206,   206,   206,
   207,   208,   208,   208,   208,   208,   208,   208,   208,   209,
   210,   210,   211,   211,   212,   212,   212,   213,   214,   214,
   215,   215,   216,   216,   217,   217,   217,   217,   217,   218,
   218,   218,   219,   219,   219,   220,   220,   220,   220,   220,
   220,   220,   220,   221,   221,   221,   221,   221,   221,   221,
   221,   222,   222,   222,   222,   222,   222,   222,   222,   223,
   223,   223,   223,   223,   223,   223,   223,   223,   223,   224,
   224,   224,   224,   224,   224,   224,   225,   225,   225,   225,
   226,   226,   227,   227,   227,   228,   229,   229,   229,   229,
   230,   230,   230,   230,   230,   230,   230,   230,   231,   232,
   232,   233,   233,   233,   233,   233,   233,   233,   233,   233,
   233,   233,   233,   233,   233,   235,   234,   236,   236,   237,
   237,   238,   238,   238,   239,   240,   240,   241,   241,   241,
   241,   241,   241,   241,   241,   241,   241,   241,   241,   242,
   242,   242,   242,   242,   242,   242,   242,   243,   243,   243,
   243,   243,   243,   243,   243,   243,   243,   243,   243,   243,
   243,   243,   243,   243,   243,   243,   243,   243,   243,   244,
   246,   245,   245,   247,   247,   249,   248,   250,   248,   251,
   251,   251,   252,   252,   253,   253,   253,   254,   254,   255,
   255,   255,   255,   256,   256,   256,   256,   256,   257,   257,
   258,   258,   259,   260,   260,   261,   261,   262,   262,   263,
   263,   263,   263,   263,   263,   264,   264,   264,   264,   265,
   266,   266,   266,   266,   266,   266,   267,   267,   267,   267,
   268,   268,   269,   269,   269,   270,   270,   271,   271,   271,
   271,   271,   271,   272,   272,   273
};

static const short yyr2[] = {     0,
     1,     3,     2,     3,     2,     5,     3,     2,     1,     1,
     1,     1,     1,     1,     1,     2,     1,     1,     1,     1,
     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     1,     1,     1,     0,     7,     5,
     3,     5,     5,     3,     2,     2,     5,     2,     5,     2,
     4,     1,     1,     7,     7,     5,     0,     7,     1,     1,
     2,     2,     1,     5,     5,     5,     3,     4,     3,     7,
     8,     5,     3,     1,     1,     3,     1,     4,     7,     6,
     1,     1,     7,     9,     8,    10,     5,     7,     6,     8,
     1,     1,     5,     4,     5,     7,     1,     3,     6,     6,
     8,     1,     2,     3,     1,     2,     3,     6,     5,     9,
     2,     1,     1,     1,     0,     6,     5,     1,     4,     1,
     1,     2,     5,     6,     4,     7,     9,     6,     1,     1,
     1,     1,     1,     1,     0,     9,     0,     9,     1,     3,
     4,     0,     6,     3,     1,     1,     1,     1,     1,     1,
     1,     2,     1,     1,     1,     3,     1,     3,     4,     0,
     6,     0,     5,     5,     2,     0,     6,     1,     1,     0,
     3,     1,     1,     1,     1,     1,     1,     1,     1,     3,
     1,     1,     1,     1,     1,     1,     1,     1,     1,     6,
     6,     7,     8,     8,     9,     7,     5,     2,     2,     2,
     0,     7,     0,     6,     1,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     1,     2,     2,     2,     4,     2,
     5,     1,     2,     1,     1,     1,     1,     1,     1,     1,
     2,     1,     3,     1,     2,     7,     3,     1,     2,     1,
     3,     1,     1,     1,     2,     5,     2,     2,     1,     2,
     2,     1,     1,     1,     1,     2,     3,     1,     2,     3,
     4,     5,     4,     1,     2,     3,     4,     5,     3,     4,
     4,     1,     2,     4,     4,     5,     3,     4,     4,     1,
     2,     2,     3,     1,     2,     1,     2,     3,     4,     3,
     4,     2,     3,     3,     4,     3,     3,     2,     2,     1,
     1,     2,     1,     1,     1,     1,     2,     1,     2,     3,
     1,     1,     1,     2,     2,     1,     1,     2,     1,     1,
     2,     1,     1,     1,     1,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     1,     0,     2,     1,     1,     3,
     1,     1,     3,     1,     1,     1,     1,     1,     1,     5,
     1,     1,     3,     4,     5,     5,     6,     2,     2,     1,
     1,     1,     1,     1,     1,     1,     1,     3,     3,     3,
     3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
     3,     3,     3,     5,     2,     2,     2,     2,     2,     1,
     0,     3,     1,     1,     3,     0,     3,     0,     5,     1,
     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
     4,     5,     1,     3,     3,     4,     4,     3,     2,     1,
     1,     3,     2,     3,     1,     1,     1,     1,     1,     2,
     4,     1,     3,     1,     3,     3,     2,     2,     2,     2,
     2,     4,     1,     3,     1,     3,     3,     2,     2,     2,
     2,     1,     1,     1,     1,     3,     1,     3,     5,     1,
     3,     3,     5,     1,     1,     0
};

static const short yydefact[] = {   466,
     0,     0,     0,     0,     9,     3,   466,   322,   330,   323,
   324,   327,   328,   325,   326,   313,   329,   312,   331,   466,
   316,   332,   333,   334,   335,     0,   303,   304,   305,   406,
   407,     0,   403,   404,     0,     0,   434,     0,     0,   301,
   466,   308,   311,   319,   320,   405,     0,   317,   432,     5,
     0,     0,   466,    14,    63,    59,    60,     0,   225,    13,
   222,   466,     0,     0,    81,    82,   466,   466,     0,     0,
   224,   226,   227,     0,   228,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     8,
    10,    17,    18,    19,    20,    21,    22,    23,    24,   466,
    25,    26,    27,    28,    29,    30,    31,     0,    32,    33,
    34,    35,    36,    37,    11,   112,   114,   113,    15,    12,
   129,   130,   131,   132,   133,   134,     0,   229,   466,   440,
   425,   314,     0,   315,     0,     0,     2,   307,   302,   466,
   336,     0,     0,   286,   300,     0,   252,   235,   466,   258,
   466,   284,   280,   272,   249,   309,   321,   318,     0,     0,
   430,     4,     7,     0,   230,   466,   232,    16,     0,   452,
   223,     0,     0,   457,     0,   466,     0,   306,     0,     0,
     0,     0,    77,     0,   466,   466,     0,     0,   466,   162,
     0,     0,    61,    62,     0,     0,    50,    48,    45,    46,
   466,     0,   466,     0,   466,   466,     0,   111,   466,   466,
     0,     0,     0,     0,     0,     0,   272,   466,     0,     0,
   352,   360,   361,   362,   363,   364,   365,   366,   367,     0,
     0,     0,     0,     0,     0,     0,     0,   243,     0,   238,
   466,   347,   306,     0,   346,   348,   351,   349,   240,   237,
   435,   433,     0,   310,   466,   286,     0,     0,   280,   317,
   247,   245,     0,   292,     0,   346,   248,   466,     0,   259,
   285,   264,   298,   299,   273,   250,   466,     0,   251,   466,
     0,   282,   256,   281,   264,   287,   439,   438,   437,     0,
     0,   231,   234,   426,     0,   427,   451,   115,   460,     0,
    67,    44,   336,     0,   466,    69,     0,     0,     0,    73,
     0,     0,     0,    97,     0,     0,   158,     0,   466,   160,
     0,     0,   102,     0,     0,     0,   106,   253,   254,   255,
    41,     0,   103,   105,   428,     0,   429,    53,     0,    52,
     0,     0,   151,   466,   155,   405,   153,   144,     0,   426,
     0,     0,     0,     0,     0,     0,     0,   264,     0,   466,
     0,   339,   466,   466,   137,   318,     0,     0,   358,   386,
   385,   359,   387,   388,   424,     0,   239,   242,   389,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,   431,     0,   286,   280,
   317,     0,   272,   296,   294,   282,     0,   272,   287,     0,
   337,   293,   280,   317,   265,   466,     0,   297,     0,   277,
     0,     0,   290,     0,   257,   283,   288,     0,   260,   436,
     6,   466,     0,   466,     0,     0,   456,     0,     0,    68,
    38,    76,     0,     0,     0,     0,     0,     0,     0,   159,
     0,     0,   466,   466,     0,     0,   107,     0,   466,     0,
     0,     0,     0,     0,   142,     0,   152,   157,    57,     0,
     0,     0,     0,    78,     0,   125,   466,     0,   317,     0,
     0,   121,   466,     0,   141,   391,     0,   390,   393,   353,
     0,   300,     0,   466,   466,   383,   382,   380,   381,     0,
   379,   378,   374,   375,   373,   376,   377,   368,   369,   370,
   371,   372,     0,     0,   287,   275,   274,   288,     0,     0,
     0,   264,   266,   287,     0,   269,     0,   279,   278,   295,
   291,     0,   261,   289,   263,   233,    65,    66,    64,     0,
   461,   462,   465,   464,   458,    42,    43,     0,    75,    72,
    74,   455,    92,   454,     0,    87,   466,   453,    91,     0,
   464,     0,     0,    98,   466,   197,   164,   163,     0,   222,
     0,     0,    49,    47,   466,    40,   104,   443,     0,   445,
     0,    56,     0,     0,   109,   466,   466,   466,   466,     0,
     0,   342,     0,   341,   344,   466,   466,     0,   118,   120,
   117,     0,   122,   170,   189,     0,     0,     0,     0,   226,
     0,   213,   214,   206,   215,   187,   168,   211,   207,   205,
   208,   209,   210,   212,   188,   184,   185,   172,   178,   182,
   181,     0,     0,   173,   174,   177,   183,   175,   179,   176,
   186,     0,   229,   466,   135,   354,     0,   300,   299,     0,
     0,     0,   241,     0,   466,   276,   246,   267,     0,   271,
   270,   262,   116,     0,     0,     0,   466,     0,   410,     0,
   413,     0,     0,     0,     0,    89,   466,     0,   161,   223,
   466,     0,   100,     0,    99,     0,     0,     0,   441,     0,
   466,     0,    51,   145,   146,   149,   148,   143,   147,   150,
     0,   156,     0,     0,    80,     0,   466,     0,   466,   336,
   466,   128,     0,   466,   466,     0,   166,   199,   198,   200,
     0,     0,     0,   165,     0,     0,   466,   317,   408,   392,
   394,   396,   409,     0,   356,   355,     0,   350,   384,   236,
   268,   463,   459,    39,     0,   466,     0,    83,   464,    94,
    88,   466,     0,     0,    96,    70,     0,     0,   108,   450,
   448,   449,   444,   446,     0,    54,    55,     0,    58,    79,
   343,   345,   340,   126,   466,     0,     0,     0,     0,   420,
   466,     0,     0,   171,     0,     0,   466,   466,     0,   466,
     0,     0,   318,   180,   466,   401,   400,   402,     0,   398,
     0,   357,     0,     0,   466,    95,     0,    90,   466,    85,
    71,   101,   447,   442,     0,     0,     0,   418,   419,   421,
     0,   414,   415,   123,   119,   466,     0,   466,     0,     0,
   466,     0,     0,     0,     0,   203,     0,   395,   397,   466,
     0,    93,   411,     0,    84,     0,   110,   127,   416,   417,
     0,   423,   124,     0,     0,   466,   138,     0,   466,   466,
     0,   466,   221,     0,   201,     0,   139,   136,   466,   412,
    86,   422,   167,   466,   191,     0,   466,     0,     0,   466,
   190,   204,     0,   399,     0,   192,     0,   216,   217,   196,
   466,   466,     0,   202,   140,   218,   220,   336,   194,   193,
   466,     0,   195,   219,     0,     0,     0
};

static const short yydefgoto[] = {   905,
     4,    90,    91,    92,   548,   612,   613,   614,   615,    97,
   338,   339,   616,   617,   588,   100,   101,   618,   103,   619,
   105,   620,   550,   182,   621,   108,   622,   556,   446,   623,
   313,   624,   322,   204,   333,   205,   625,   626,   627,   628,
   434,   116,   601,   481,   117,   118,   119,   120,   121,   734,
   484,   868,   629,   630,   586,   698,   342,   343,   344,   467,
   631,   125,   453,   319,   632,   785,   716,   633,   634,   635,
   636,   637,   638,   639,   883,   864,   640,   875,   886,   641,
   642,   257,   165,   292,   166,   239,   240,   377,   241,   482,
   148,   327,   149,   270,   150,   151,   152,   216,    39,    40,
   242,   178,    42,    43,    44,    45,   262,   263,   361,   593,
   594,   771,   244,   266,   246,   247,   487,   488,   644,   730,
   731,   799,   840,   800,    46,    47,   732,   887,   712,   779,
   819,   820,   130,   299,   336,    48,   161,    49,   581,   689,
   248,   559,   173,   300,   545,   167
};

static const short yypact[] = {   652,
  1928,  2048,   187,  2694,-32768,-32768,   -48,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,   -48,
-32768,-32768,-32768,-32768,-32768,   123,-32768,-32768,-32768,-32768,
-32768,   158,-32768,-32768,   -53,   142,-32768,   193,  3496,   719,
   299,   719,-32768,-32768,  2650,-32768,   158,-32768,   177,-32768,
   226,   235,  2528,   222,-32768,-32768,-32768,   277,-32768,-32768,
   369,   323,  2168,   343,-32768,-32768,   323,   363,   364,   389,
-32768,-32768,-32768,   404,-32768,    90,   220,   411,   173,   413,
   483,   308,  3287,  3287,   417,   429,   369,   433,   435,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,   323,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,   944,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,  3336,-32768,  1747,-32768,
-32768,-32768,   430,-32768,    32,   465,-32768,   719,-32768,  1364,
   434,  1867,  2467,   103,   202,   158,-32768,-32768,   129,   224,
   129,   361,   795,   385,-32768,-32768,-32768,-32768,   486,    39,
-32768,-32768,-32768,   457,-32768,   469,-32768,-32768,   285,-32768,
    57,   285,   285,-32768,   464,     3,  1010,-32768,   250,   158,
   519,   527,-32768,   285,  2408,  2528,   158,   495,   144,-32768,
   497,   539,-32768,-32768,   285,   543,-32768,-32768,-32768,   541,
  2528,   512,   703,   518,   522,   285,   369,   541,  2528,  2528,
   158,   369,    33,   268,   285,  1159,   462,   172,  1035,   269,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,  2467,
   528,  2467,  2467,  2467,  2467,  2467,  2467,-32768,   479,-32768,
   537,   561,   271,  3608,   -21,-32768,-32768,   541,-32768,-32768,
-32768,   177,   487,-32768,  1473,   228,   542,   573,  1093,   513,
-32768,   563,  2467,-32768,  3457,-32768,  3608,  1473,   158,   383,
   361,-32768,-32768,   500,-32768,-32768,  2528,  1987,-32768,  2528,
  2107,   103,   383,   361,   526,  1185,-32768,-32768,   177,   591,
  2528,-32768,-32768,-32768,   595,   541,-32768,-32768,    34,   597,
-32768,-32768,-32768,    79,   129,-32768,   602,   606,   614,   609,
   207,   623,   626,-32768,   633,   635,-32768,   158,-32768,-32768,
   641,   644,-32768,   648,   650,  3287,-32768,-32768,-32768,-32768,
-32768,  3287,-32768,-32768,-32768,   651,-32768,-32768,   300,   234,
   656,   582,-32768,   670,-32768,     7,-32768,-32768,   156,   239,
   355,   355,   594,   674,    82,   676,   268,   617,  1185,   102,
   688,-32768,  2288,  1030,-32768,   365,  1339,  3395,  1516,-32768,
-32768,-32768,-32768,-32768,-32768,  1747,-32768,-32768,-32768,  2467,
  2467,  2467,  2467,  2467,  2467,  2467,  2467,  2467,  2467,  2467,
  2467,  2467,  2467,  2467,  2467,  2467,-32768,   465,   408,   324,
   631,   373,-32768,-32768,-32768,   408,   443,   636,   355,  2467,
  3608,-32768,  1109,    12,-32768,  2528,  2227,-32768,   698,-32768,
  3492,   701,-32768,  3524,   383,   361,  1193,   268,   383,-32768,
-32768,   469,   216,-32768,   285,  1410,-32768,   705,   706,-32768,
-32768,-32768,   427,   952,  1806,   712,  2528,  1010,   700,-32768,
   713,  2793,-32768,   624,  3287,   421,   715,   711,   522,   252,
   731,   285,  2528,   124,-32768,  2528,-32768,-32768,-32768,   355,
   911,   268,    14,-32768,  1200,-32768,   779,   745,   594,   748,
   681,-32768,   209,  1642,-32768,-32768,   743,-32768,-32768,  2467,
  2347,  2587,    -5,   299,   537,  1149,  1149,  1134,  1134,  3596,
  3648,  3677,  1451,  1441,  1516,   967,   967,   657,   657,-32768,
-32768,-32768,   158,   636,-32768,-32768,-32768,   408,   618,  3536,
   620,   636,-32768,   268,   753,-32768,  3567,-32768,-32768,-32768,
-32768,   268,   383,   361,   383,-32768,-32768,-32768,   541,  2892,
-32768,   749,-32768,   234,   759,-32768,-32768,  1642,-32768,-32768,
   541,-32768,-32768,-32768,   762,-32768,   578,   541,-32768,   755,
    72,   328,   952,-32768,   578,-32768,-32768,-32768,  2991,   369,
  3444,   374,-32768,-32768,  2528,-32768,-32768,   184,   679,-32768,
   720,-32768,   771,   768,-32768,   574,   670,-32768,   578,    85,
   268,   763,   395,-32768,-32768,   506,  2528,  1010,-32768,-32768,
-32768,   786,-32768,-32768,-32768,   781,   756,   757,   767,   707,
   486,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,   790,  1642,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,  3178,   792,   770,-32768,  3608,  2467,  2587,  1808,  2467,
   806,   807,-32768,  2467,   129,-32768,-32768,-32768,   654,-32768,
-32768,   383,-32768,   285,   285,   802,  2528,   814,   783,   124,
-32768,  1410,   293,   285,   818,-32768,   578,   819,-32768,   541,
    51,  1010,-32768,  3287,-32768,   824,   861,    49,-32768,    58,
  1747,   194,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  3227,-32768,  3090,   826,-32768,  2467,   779,   887,  2528,-32768,
   796,-32768,   833,  1030,  2528,  1642,-32768,-32768,-32768,-32768,
   486,   838,  1010,-32768,  3395,   899,   114,   842,-32768,   839,
-32768,   750,-32768,  1642,  3608,  3608,  2467,-32768,  3637,-32768,
-32768,-32768,-32768,-32768,   845,  2528,   847,-32768,   541,   849,
-32768,   578,   960,   124,-32768,-32768,   854,   857,-32768,-32768,
   184,-32768,   184,-32768,   811,-32768,-32768,  1036,-32768,-32768,
-32768,  3608,-32768,-32768,  1030,   860,   865,   158,   428,-32768,
   129,   681,   869,-32768,  1642,   875,  2528,  1030,    70,  2288,
  2467,   876,   365,-32768,   770,-32768,-32768,-32768,   158,-32768,
   878,  3608,   885,   890,  2528,-32768,   889,-32768,   578,-32768,
-32768,-32768,-32768,-32768,   891,   681,   473,-32768,   894,-32768,
   893,-32768,-32768,-32768,-32768,   129,   895,  2528,   897,   681,
  2528,   901,    16,   902,  2547,-32768,   900,-32768,-32768,   770,
  1003,-32768,-32768,   904,-32768,   905,-32768,-32768,-32768,-32768,
   158,-32768,-32768,  1642,   909,   578,-32768,   912,  2528,  2528,
   915,   506,-32768,  1003,-32768,   158,-32768,-32768,  1030,-32768,
-32768,-32768,-32768,   578,-32768,   493,   578,   918,   920,  2528,
-32768,-32768,  1003,-32768,   681,-32768,   467,-32768,-32768,-32768,
   578,   578,   921,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
   578,   913,-32768,-32768,   950,   961,-32768
};

static const short yypgoto[] = {-32768,
  -313,-32768,-32768,-32768,-32768,     9,    28,    36,    40,-32768,
   529,-32768,    45,    52,-32768,-32768,-32768,    54,-32768,    60,
-32768,    61,-32768,-32768,    66,-32768,    68,  -456,  -541,    73,
-32768,    77,-32768,  -308,   503,   -80,    91,    98,    99,   100,
-32768,   378,  -707,  -677,-32768,-32768,-32768,-32768,   384,-32768,
-32768,  -476,    -2,     5,-32768,-32768,-32768,-32768,   507,   387,
   131,-32768,-32768,-32768,  -537,-32768,-32768,-32768,   386,-32768,
   391,   146,-32768,-32768,-32768,-32768,-32768,-32768,  -545,-32768,
    21,   107,-32768,   548,    44,   284,-32768,   491,   613,   -36,
   501,-32768,    -3,  -127,  -180,  -116,   -17,    81,   -26,-32768,
   601,    27,   -38,-32768,   946,-32768,  -288,-32768,-32768,-32768,
   287,-32768,   949,  -119,  -414,-32768,  -661,-32768,-32768,-32768,
   204,-32768,-32768,-32768,  -198,   -39,   157,  -432,   141,-32768,
-32768,   154,   989,  -146,-32768,   765,  -107,   -76,-32768,  -643,
   709,   438,   -10,  -165,  -428,     0
};


#define	YYLAST		3799


static const short yytable[] = {     5,
   138,   122,   202,   128,   147,   452,   131,   585,   123,   245,
   666,   346,    93,   139,   438,   156,   562,   458,  -244,   131,
   675,   543,   295,   258,   127,   283,   301,    41,    41,   323,
   543,    94,   307,   650,     7,   350,   782,   311,   170,    95,
   155,     7,   129,    96,    38,    51,  -154,   354,    98,   211,
   523,   761,   523,   860,   303,    99,   180,   102,   252,   335,
   763,   174,   297,   104,   106,   792,   174,   183,   356,   107,
  -244,   109,   135,   435,   825,   402,   110,   297,   407,    41,
   111,     7,   287,   289,     7,   436,   329,   220,   283,   212,
   297,   756,     7,   129,   112,   724,   253,   816,  -154,   174,
   298,   113,   114,   115,   215,     7,   676,   831,   848,   651,
   830,   254,   276,   524,   279,   591,     7,   813,   274,   814,
   540,   153,   857,  -427,   670,   705,   271,   552,   249,   170,
   861,   837,   678,    35,   124,   284,   251,    37,   258,   569,
   268,   129,   476,   288,   397,  -338,   572,   351,   155,   126,
   155,   258,   145,   762,   425,   243,   704,   142,   429,   164,
     7,   297,   764,   711,   553,   293,    41,   554,   133,   187,
   402,   407,   238,   305,   360,   131,   193,   895,   784,    35,
   141,   430,    35,    37,   832,   486,    37,    52,   131,   145,
    35,   885,   129,   351,    37,   318,   801,   766,   145,   328,
   143,   145,   330,    35,   334,   337,   147,    37,   806,   347,
   469,    41,    41,   748,    35,   364,   194,   362,    37,   537,
   259,   170,   189,   555,    53,   273,   269,    41,   519,   425,
     7,   429,   136,   137,   129,    41,    41,   767,   271,   297,
   378,   284,   155,   750,   754,   444,   445,   827,    27,    28,
    29,   459,   294,   345,   578,   170,   245,   543,    35,   538,
   141,   277,    37,   190,   426,   140,   162,   346,   439,   541,
     7,     7,    35,     7,   703,   163,    37,   463,   278,   159,
   143,    41,   142,   153,   602,   521,   687,   294,   306,   519,
   170,   314,   315,   352,    41,     7,   583,   810,   560,   533,
   535,     7,   160,    41,   155,   355,    41,   325,   140,   688,
  -466,   199,   365,   170,   169,   341,   873,    41,     5,   807,
    35,   145,   141,   153,    37,   142,     7,   168,    35,   129,
   304,   890,    37,   751,   432,   400,   140,   483,   461,   462,
  -466,  -466,   143,   468,   521,   899,   900,   533,   413,   131,
   141,   146,   579,   142,   418,   903,   580,     7,   513,   131,
   172,   255,  -466,   155,  -466,   489,   673,   674,    35,    35,
   143,    35,    37,    37,   170,    37,   846,   552,   142,   170,
   179,   271,   284,   419,   352,   144,   422,   882,   426,    41,
   145,   269,   304,    35,   146,   284,   659,    37,   280,    35,
   181,   184,   243,    37,   662,   485,   894,   214,  -466,   534,
   277,   516,   145,   144,   683,   281,   146,   554,   145,   238,
   416,   777,   146,   876,    35,   684,   185,   278,    37,   711,
   549,   293,   170,     5,   707,   472,   486,   417,   406,   708,
   557,   186,    41,   352,   565,   255,   603,   493,   192,   122,
   195,   128,     5,   128,   206,    35,   123,   147,   334,    37,
    93,   573,   142,   659,   574,   649,   207,   589,   822,   478,
   209,   823,   127,    41,   571,   250,   595,   261,   146,    94,
   277,   517,   155,   643,    30,    31,   286,    95,     7,    41,
   564,    96,    41,   155,   378,   290,    98,   278,   742,   743,
   534,    33,    34,    99,   302,   102,   584,   896,   291,   345,
   897,   104,   106,   849,    30,    31,   850,   107,   898,   109,
   196,   309,   525,   197,   110,   210,   198,   560,   111,   310,
   669,    33,    34,   888,   722,   317,   889,   122,   669,   128,
   320,   321,   112,   709,   123,   324,   297,   643,    93,   113,
   114,   115,    27,    28,    29,   326,   671,   710,   331,   677,
   127,   332,   669,   359,   671,   368,   122,    94,   128,   669,
   375,   245,   273,   123,   153,    95,   376,    93,   398,    96,
   404,   668,   124,   696,    98,   128,   468,     5,   671,   127,
   697,    99,    59,   102,   714,   671,    94,   126,   379,   104,
   106,    41,   727,   758,    95,   107,   701,   109,    96,   902,
   138,   405,   110,    98,   409,   570,   111,   410,   740,   145,
    99,   649,   102,    41,    27,    28,    29,   427,   104,   106,
   112,   431,   643,   433,   107,   437,   109,   113,   114,   115,
   440,   110,    59,   733,   786,   111,    71,    72,    73,   441,
   669,    75,   442,   668,   155,   277,   656,   416,   658,   112,
   443,   220,   447,   177,   448,   570,   113,   114,   115,   752,
   124,   449,   278,   465,   417,   450,   671,    89,   757,   454,
   131,   686,   455,   203,   203,   126,   456,   483,   457,   460,
   249,   416,   741,    41,   464,   470,    71,    72,    73,   124,
   122,    75,   128,   713,   775,     7,   595,   123,   417,   466,
   780,    93,   471,   155,   126,   643,   474,   243,   475,   788,
   598,   599,   364,   127,   600,   669,   489,   219,   273,   418,
    94,   477,   515,   643,   238,    41,   528,   518,    95,   530,
   140,    41,    96,   566,   824,   546,   547,    98,   483,   809,
   563,   671,   575,   567,    99,   576,   102,   142,     1,     2,
     3,   483,   104,   106,   364,    27,    28,    29,   107,   171,
   109,   582,    41,   745,   155,   110,   394,   395,   396,   111,
   155,   592,   669,   596,   643,   597,   645,   155,   664,   853,
   200,   660,   489,   112,   733,   208,   134,     7,   665,   667,
   113,   114,   115,    35,   690,   154,   672,    37,   671,   692,
   691,   158,   693,    41,   706,   776,    41,   144,   796,   797,
   798,   783,   145,   715,   717,   155,   146,   718,   719,   669,
   721,    41,   268,   124,   723,   669,  -169,   869,   720,   733,
   188,   191,   483,   729,   737,   738,   744,   669,   126,   142,
   669,   746,   804,   643,    41,   671,   753,    41,   747,   755,
   869,   671,   759,   760,   669,   669,   770,   778,   155,   833,
   352,   781,   217,   671,   669,   787,   671,   296,   795,   869,
   296,   296,   794,   803,   805,    41,    41,   296,   674,     7,
   671,   671,   296,   829,   811,    35,   834,   812,   817,    37,
   671,     7,   815,   296,   260,   818,    41,   826,   272,   282,
   275,   844,   828,     7,   296,   340,   170,   285,   269,   836,
   349,   296,   841,   296,   304,   842,   203,   774,   843,   845,
   852,   847,   203,   851,   855,   856,   790,   858,   859,   854,
   862,   217,   870,   865,   308,   871,     7,   874,   304,   906,
   877,   316,   880,   904,     7,   552,   891,   170,   892,   901,
   907,   577,     7,   694,   542,   878,   879,   154,   494,   695,
   791,   699,   587,   702,   765,   348,   700,   353,   272,   536,
   358,   213,   134,   217,   366,   653,   893,    35,   495,   304,
   157,    37,   553,   773,   652,   554,   866,   304,   838,    35,
   808,   214,   881,    37,   872,     7,   145,   154,   132,   685,
   146,    35,     7,   214,     0,    37,     0,     0,   145,   401,
   403,     0,   146,   408,     0,   214,     0,     0,     0,     0,
   145,     0,   414,   415,   146,     0,     0,     7,     7,     0,
   304,     0,     0,   867,    35,     0,   272,   304,    37,     0,
   272,   555,    35,     0,     0,   203,    37,     0,   214,     0,
    35,     0,     0,   145,    37,     0,   214,   146,   353,     0,
     0,   145,   363,   790,   214,   146,    27,    28,    29,   145,
     0,   141,   451,   146,   392,   393,   394,   395,   396,     0,
   265,   267,     0,     0,     0,     7,     0,     0,     0,     0,
     0,   143,     0,    35,     0,   480,     0,    37,     0,     0,
    35,     7,     0,     0,    37,   403,   403,   214,     0,   473,
     0,   272,   145,   272,   214,     0,   146,   479,     0,   145,
   140,     0,     0,   146,     0,    35,    35,     0,     0,    37,
    37,   539,     0,   296,   544,     0,   268,   142,     0,   214,
   214,   551,   558,   561,   145,   145,     0,     0,   146,   146,
     0,     7,     0,   142,   514,     0,     0,     0,     0,     0,
   296,   682,   558,   403,     0,     0,     0,   522,   367,   590,
   369,   370,   371,   372,   373,   374,     0,     7,     0,     0,
     0,   272,   272,    35,     0,     7,   355,    37,     0,     0,
     0,     0,     7,     0,     0,     0,     0,   406,   217,    35,
     0,   411,   217,    37,     0,     0,   146,     0,     0,     0,
     0,     0,   355,   406,     0,     0,   421,   380,   381,   424,
   268,     0,   269,     0,   403,   217,   272,   355,     0,   272,
     0,     0,   726,     0,     0,     0,     0,   142,     0,   390,
   391,   392,   393,   394,   395,   396,     0,     0,   154,    35,
     0,     0,     0,    37,   390,   391,   392,   393,   394,   395,
   396,   558,     0,   357,     0,     0,     0,   655,   680,     0,
   558,     0,   269,     0,   203,    35,     0,     0,   272,    37,
     0,     0,     0,    35,     0,     0,   272,    37,     0,   428,
    35,   768,     0,     0,    37,     0,     0,   532,   269,     0,
     0,     0,     0,     0,   532,     0,   269,     0,     0,     0,
     0,     0,     0,   269,     0,   789,     0,   217,   496,   497,
   498,   499,   500,   501,   502,   503,   504,   505,   506,   507,
   508,   509,   510,   511,   512,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,   272,     0,     0,   520,     0,
     0,     0,   217,     0,     0,   527,     7,     0,     0,     0,
     0,     0,   296,   296,     0,     0,     0,   490,   558,     0,
   749,     0,   296,     8,     9,    10,    11,    12,    13,    14,
    15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
    25,   255,     0,     0,     0,     0,   728,     0,     0,    26,
    27,    28,    29,    30,    31,   170,     0,     0,   142,     0,
   222,   223,   224,   225,   226,   227,   228,   229,     0,    32,
    33,    34,   380,   381,   382,   383,   384,   217,   646,   505,
   510,     0,     0,     0,     0,     0,   217,     0,     0,   385,
   386,   387,   388,   491,   390,   391,   392,   393,   492,   395,
   396,     0,   558,     0,    35,     0,     0,    36,    37,     0,
     0,     0,   217,     0,     0,     7,     0,     0,   256,     0,
     0,     0,     0,   145,     0,     0,     0,   217,     0,     0,
   217,   793,     8,     9,    10,    11,    12,    13,    14,    15,
    16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
   255,     0,     0,     0,     0,     0,     0,   217,    26,    27,
    28,    29,    30,    31,     0,     0,     0,   142,     0,     0,
     0,     0,   217,     0,   380,   381,   382,   383,    32,    33,
    34,     0,   821,     0,   380,   381,   382,   383,     0,     0,
     0,     0,     0,     0,   479,   389,   390,   391,   392,   393,
   394,   395,   396,   839,   388,   389,   390,   391,   392,   393,
   394,   395,   396,    35,     0,     0,    36,    37,     0,     0,
     0,     0,     0,     0,     0,     0,     0,   399,     0,     0,
     0,     0,   145,     0,     0,   735,   372,     0,   736,     0,
     0,     0,   739,     0,     0,   217,     0,     0,     0,   380,
   381,   382,   383,     0,     0,   821,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,   217,     0,
   884,   390,   391,   392,   393,   394,   395,   396,     0,     0,
     0,     0,   604,     0,  -466,    55,     0,   217,    56,    57,
    58,     0,     0,     0,   772,     0,     0,     0,     0,     0,
    59,  -466,  -466,  -466,  -466,  -466,  -466,  -466,  -466,  -466,
  -466,  -466,  -466,  -466,  -466,  -466,  -466,  -466,  -466,     0,
     0,     0,   605,    61,     0,   802,  -466,     0,  -466,  -466,
  -466,  -466,  -466,     0,     0,     0,     0,     0,     0,    63,
    64,    65,    66,   606,    68,    69,    70,  -466,  -466,  -466,
   607,   608,   609,     0,    71,   610,    73,     0,    74,    75,
    76,     0,     0,     0,    80,     0,    82,    83,    84,    85,
    86,    87,     0,     0,     0,     0,     0,     0,     0,   835,
    88,     0,  -466,     0,     0,    89,  -466,  -466,     0,     7,
     0,     0,   170,     0,     0,     0,   221,   222,   223,   224,
   225,   226,   227,   228,   229,   611,     8,     9,    10,    11,
    12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
    22,    23,    24,    25,   230,     0,     0,     0,     0,     0,
     0,     0,    26,    27,    28,    29,    30,    31,     0,   231,
     0,     0,     0,     0,     0,     0,     0,     0,   294,     0,
     7,   170,    32,    33,    34,     0,   222,   223,   224,   225,
   226,   227,   228,   229,     0,     0,     0,     8,     9,    10,
    11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
    21,    22,    23,    24,    25,     0,     0,    35,     0,     0,
    36,    37,     0,     0,     0,     0,     0,    30,    31,     0,
     0,   232,     0,     0,   233,   234,   235,     0,     0,     7,
   236,   237,   170,    32,    33,    34,   221,   222,   223,   224,
   225,   226,   227,   228,   229,     0,     8,     9,    10,    11,
    12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
    22,    23,    24,    25,   230,     0,     0,     0,    35,     0,
     0,     0,    37,    27,    28,    29,    30,    31,     0,   231,
     0,     0,   264,     0,     0,     0,     0,   145,     6,     0,
     7,     0,    32,    33,    34,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     8,     9,    10,
    11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
    21,    22,    23,    24,    25,     0,     0,    35,     0,     0,
     0,    37,     0,    26,    27,    28,    29,    30,    31,     0,
     0,   232,     0,     0,   233,   234,   235,     0,     0,     7,
   236,   237,   170,    32,    33,    34,   221,   222,   223,   224,
   225,   226,   227,   228,   229,     0,     8,     9,    10,    11,
    12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
    22,    23,    24,    25,   230,     0,     0,     0,    35,     0,
     0,    36,    37,    27,    28,    29,    30,    31,     0,   231,
     0,     0,   420,     0,     0,     0,     0,     0,    50,     0,
     7,     0,    32,    33,    34,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     8,     9,    10,
    11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
    21,    22,    23,    24,    25,     0,     0,    35,     0,     0,
     0,    37,     0,    26,    27,    28,    29,    30,    31,     0,
     0,   232,     0,     0,   233,   234,   235,     0,     0,     7,
   236,   237,   170,    32,    33,    34,   221,   222,   223,   224,
   225,   226,   227,   228,   229,     0,     8,     9,    10,    11,
    12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
    22,    23,    24,    25,   230,     0,     0,     0,    35,     0,
     0,    36,    37,    27,    28,    29,    30,    31,     0,   231,
     0,     0,   423,     0,     0,     0,     0,     0,   175,     0,
   176,     0,    32,    33,    34,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     8,     9,    10,
    11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
    21,    22,    23,    24,    25,     0,     0,    35,     0,     0,
     0,    37,     0,     0,    27,    28,    29,    30,    31,     0,
     0,   232,     0,     0,   233,   234,   235,     0,     0,     7,
   236,   237,   170,    32,    33,    34,   221,   222,   223,   224,
   225,   226,   227,   228,   229,     0,     8,     9,    10,    11,
    12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
    22,    23,    24,    25,   230,     0,     0,     0,    35,     0,
     0,     0,    37,    27,    28,    29,    30,    31,     0,   231,
     0,     0,   526,     0,     0,     0,     0,     0,     0,     0,
     7,     0,    32,    33,    34,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     8,     9,    10,
    11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
    21,    22,    23,    24,    25,     0,     0,    35,     0,     0,
     0,    37,     0,    26,    27,    28,    29,    30,    31,     0,
     0,   232,     0,     0,   233,   234,   235,     0,     0,     7,
   236,   237,   170,    32,    33,    34,   221,   222,   223,   224,
   225,   226,   227,   228,   229,     0,     8,     9,    10,    11,
    12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
    22,    23,    24,    25,   230,   647,     0,     0,    35,     0,
     0,    36,    37,    27,    28,    29,    30,    31,     0,   231,
     0,     0,   351,     0,     0,     0,     0,   145,     0,     0,
     7,     0,    32,    33,    34,     0,     0,     0,   312,     0,
     0,     0,     0,     0,     0,     0,     0,     8,     9,    10,
    11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
    21,    22,    23,    24,    25,     0,     0,    35,     0,     0,
     0,    37,     0,    26,    27,    28,    29,    30,    31,     0,
     0,   232,     0,     0,   233,   234,   235,     0,     0,     7,
   236,   237,   170,    32,    33,    34,   221,   222,   223,   224,
   225,   226,   227,   228,   229,     0,     8,     9,    10,    11,
    12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
    22,    23,    24,    25,   230,     0,     0,     0,    35,     0,
     0,    36,    37,    27,    28,    29,    30,    31,     0,   231,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     7,     0,    32,    33,    34,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     8,     9,    10,
    11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
    21,    22,    23,    24,    25,     0,     0,    35,     0,     0,
     0,    37,     0,    26,    27,    28,    29,    30,    31,     0,
     0,   232,     0,     0,   233,   234,   235,   863,     0,     7,
   236,   237,   170,    32,    33,    34,   221,   222,   223,   224,
   225,   226,   227,   228,   229,     0,     8,     9,    10,    11,
    12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
    22,    23,    24,    25,   230,     0,     0,     0,    35,     0,
     0,    36,    37,    27,    28,    29,    30,    31,     0,   231,
   380,   381,   382,   383,   384,     0,     0,     0,     0,     0,
     0,     0,    32,    33,    34,     0,     0,   385,   386,   387,
   388,   389,   390,   391,   392,   393,   394,   395,   396,     8,
     9,    10,    11,    12,    13,    14,    15,     0,    17,     0,
    19,     0,     0,    22,    23,    24,    25,    35,     0,     0,
     0,    37,     0,    -1,    54,     0,  -466,    55,     0,     0,
    56,    57,    58,     0,   233,   234,   648,     0,     0,     0,
   236,   237,    59,  -466,  -466,  -466,  -466,  -466,  -466,  -466,
  -466,  -466,  -466,  -466,  -466,  -466,  -466,  -466,  -466,  -466,
  -466,     0,     0,     0,    60,    61,     0,     0,     0,     0,
  -466,  -466,  -466,  -466,  -466,     0,     0,    62,     0,     0,
     0,    63,    64,    65,    66,    67,    68,    69,    70,  -466,
  -466,  -466,     0,     0,     0,     0,    71,    72,    73,     0,
    74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
    84,    85,    86,    87,     0,     0,     0,     0,     0,     0,
     0,     0,    88,    54,  -466,  -466,    55,    89,  -466,    56,
    57,    58,     0,     0,     0,     0,     0,     0,     0,     0,
     0,    59,  -466,  -466,  -466,  -466,  -466,  -466,  -466,  -466,
  -466,  -466,  -466,  -466,  -466,  -466,  -466,  -466,  -466,  -466,
     0,     0,     0,    60,    61,     0,     0,   568,     0,  -466,
  -466,  -466,  -466,  -466,     0,     0,    62,     0,     0,     0,
    63,    64,    65,    66,    67,    68,    69,    70,  -466,  -466,
  -466,     0,     0,     0,     0,    71,    72,    73,     0,    74,
    75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
    85,    86,    87,     0,     0,     0,     0,     0,     0,     0,
     0,    88,    54,  -466,  -466,    55,    89,  -466,    56,    57,
    58,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    59,  -466,  -466,  -466,  -466,  -466,  -466,  -466,  -466,  -466,
  -466,  -466,  -466,  -466,  -466,  -466,  -466,  -466,  -466,     0,
     0,     0,    60,    61,     0,     0,   663,     0,  -466,  -466,
  -466,  -466,  -466,     0,     0,    62,     0,     0,     0,    63,
    64,    65,    66,    67,    68,    69,    70,  -466,  -466,  -466,
     0,     0,     0,     0,    71,    72,    73,     0,    74,    75,
    76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
    86,    87,     0,     0,     0,     0,     0,     0,     0,     0,
    88,    54,  -466,  -466,    55,    89,  -466,    56,    57,    58,
     0,     0,     0,     0,     0,     0,     0,     0,     0,    59,
  -466,  -466,  -466,  -466,  -466,  -466,  -466,  -466,  -466,  -466,
  -466,  -466,  -466,  -466,  -466,  -466,  -466,  -466,     0,     0,
     0,    60,    61,     0,     0,   679,     0,  -466,  -466,  -466,
  -466,  -466,     0,     0,    62,     0,     0,     0,    63,    64,
    65,    66,    67,    68,    69,    70,  -466,  -466,  -466,     0,
     0,     0,     0,    71,    72,    73,     0,    74,    75,    76,
    77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
    87,     0,     0,     0,     0,     0,     0,     0,     0,    88,
    54,  -466,  -466,    55,    89,  -466,    56,    57,    58,     0,
     0,     0,     0,     0,     0,     0,     0,     0,    59,  -466,
  -466,  -466,  -466,  -466,  -466,  -466,  -466,  -466,  -466,  -466,
  -466,  -466,  -466,  -466,  -466,  -466,  -466,     0,     0,     0,
    60,    61,     0,     0,     0,     0,  -466,  -466,  -466,  -466,
  -466,     0,     0,    62,     0,   769,     0,    63,    64,    65,
    66,    67,    68,    69,    70,  -466,  -466,  -466,     0,     0,
     0,     0,    71,    72,    73,     0,    74,    75,    76,    77,
    78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
     7,     0,     0,     0,     0,     0,     0,     0,    88,     0,
  -466,     0,     0,    89,  -466,     0,     0,     8,     9,    10,
    11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
    21,    22,    23,    24,    25,     0,     0,     0,     0,     0,
     0,     0,     0,     0,    27,    28,    29,    30,    31,     7,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,   218,    33,    34,     8,     9,    10,    11,
    12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
    22,    23,    24,    25,     0,     0,     0,     0,     0,     0,
     0,     0,     0,    27,    28,    29,    30,    31,    35,     0,
     0,     0,    37,   725,     0,     0,     0,     0,     0,     7,
     0,     0,    32,    33,    34,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     8,     9,    10,    11,
    12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
    22,    23,    24,    25,   201,     0,     0,    35,     0,     0,
     0,    37,   725,    27,    28,    29,    30,    31,     7,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,    32,    33,    34,     8,     9,    10,    11,    12,
    13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
    23,    24,    25,     0,     0,     0,     0,     0,     0,     0,
     0,     0,    27,    28,    29,    30,    31,    35,     0,     0,
     0,    37,     0,     0,     0,     0,     0,     7,     0,     0,
     0,   218,    33,    34,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     8,     9,    10,    11,    12,    13,
    14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
    24,    25,     0,     0,     0,     0,    35,     0,     0,     0,
    37,    27,    28,    29,    30,    31,   681,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    32,    33,    34,     8,     9,    10,    11,    12,    13,    14,
    15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
    25,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    27,    28,    29,    30,    31,    35,     0,     0,     7,    37,
     0,     0,     0,     0,     0,     0,     0,     0,     0,    32,
    33,    34,   412,     0,     0,     8,     9,    10,    11,    12,
    13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
    23,    24,    25,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,    35,    30,    31,   529,    37,     0,
   380,   381,   382,   383,   384,     0,     0,     0,     0,     0,
     0,    32,    33,    34,     0,     0,     0,   385,   386,   387,
   388,   389,   390,   391,   392,   393,   394,   395,   396,   531,
     0,     0,     0,     0,     0,   380,   381,   382,   383,   384,
     0,   657,     0,     0,     0,     0,    35,     0,     0,     0,
    37,     0,   385,   386,   387,   388,   389,   390,   391,   392,
   393,   394,   395,   396,     0,     0,     0,   380,   381,   382,
   383,   384,   661,     0,     0,     0,     0,     0,     0,   380,
   381,   382,   383,   384,   385,   386,   387,   388,   389,   390,
   391,   392,   393,   394,   395,   396,   385,   386,   387,   388,
   389,   390,   391,   392,   393,   394,   395,   396,     0,     0,
   380,   381,   382,   383,   384,     0,     0,   654,     0,     0,
     0,     0,     0,     0,     0,     0,     0,   385,   386,   387,
   388,   389,   390,   391,   392,   393,   394,   395,   396,   380,
   381,   382,   383,   384,     0,     0,     0,     0,     0,     0,
     0,   380,   381,   382,   383,   384,   385,   386,   387,   388,
   389,   390,   391,   392,   393,   394,   395,   396,   385,   386,
   387,   388,   389,   390,   391,   392,   393,   394,   395,   396,
   380,   381,   382,   383,     0,     0,     0,     0,     0,     0,
     0,   380,   381,   382,   383,     0,     0,   385,   386,   387,
   388,   389,   390,   391,   392,   393,   394,   395,   396,   386,
   387,   388,   389,   390,   391,   392,   393,   394,   395,   396,
   380,   381,   382,   383,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,   387,
   388,   389,   390,   391,   392,   393,   394,   395,   396
};

static const short yycheck[] = {     0,
    39,     4,    83,     4,    41,   319,     7,   464,     4,   129,
   548,   210,     4,    40,   303,    42,   445,   326,    40,    20,
   562,   436,   169,   140,     4,   153,   173,     1,     2,   195,
   445,     4,   179,    39,     3,     3,   714,   184,     6,     4,
    41,     3,    91,     4,     1,     2,    40,   213,     4,    89,
    39,     3,    39,    38,    52,     4,    67,     4,   135,   206,
     3,    62,     6,     4,     4,   727,    67,    68,   215,     4,
    92,     4,   126,    40,   782,   256,     4,     6,   259,    53,
     4,     3,   159,   160,     3,    52,   203,   127,   216,   100,
     6,    41,     3,    91,     4,   633,   136,   775,    92,   100,
    44,     4,     4,     4,   108,     3,   563,    38,   816,   115,
   788,   138,   149,   102,   151,   102,     3,   761,   145,   763,
   434,    41,   830,    52,   557,    41,   144,     4,   129,     6,
   115,   793,   565,   101,     4,   153,   105,   105,   255,   453,
    38,    91,    41,   105,   252,    44,   455,   115,   149,     4,
   151,   268,   120,   105,   282,   129,   589,    55,   286,    53,
     3,     6,   105,   596,    41,   166,   140,    44,    46,    80,
   351,   352,   129,   177,     3,   176,     4,   885,   716,   101,
    52,   289,   101,   105,   115,    72,   105,     1,   189,   120,
   101,   869,    91,   115,   105,    52,   734,     4,   120,   203,
    72,   120,   203,   101,   205,   206,   243,   105,   750,   210,
    55,   185,   186,   670,   101,   219,    44,   218,   105,     4,
   140,     6,     3,   100,    38,   145,   124,   201,   409,   357,
     3,   359,    91,    41,    91,   209,   210,    44,   256,     6,
   241,   259,   243,   672,   677,    39,    40,   785,    47,    48,
    49,   332,     3,   210,     3,     6,   376,   672,   101,    44,
    52,    38,   105,    44,   282,    38,    41,   466,   305,   435,
     3,     3,   101,     3,   588,    41,   105,    44,    55,   103,
    72,   255,    55,   203,    76,   413,   103,     3,    39,   470,
     6,   185,   186,   213,   268,     3,   462,   754,   445,   427,
   428,     3,   126,   277,   305,    38,   280,   201,    38,   126,
    40,     4,    44,     6,    38,   209,   854,   291,   319,   752,
   101,   120,    52,   243,   105,    55,     3,   106,   101,    91,
    38,   877,   105,    41,   291,   255,    38,   364,    39,    40,
   102,   103,    72,   344,   472,   891,   892,   475,   268,   350,
    52,   124,   101,    55,   274,   901,   105,     3,   398,   360,
    38,    38,    92,   364,   126,   366,    39,    40,   101,   101,
    72,   101,   105,   105,     6,   105,   809,     4,    55,     6,
    38,   399,   400,   277,   304,   115,   280,   864,   406,   363,
   120,   124,    38,   101,   124,   413,   524,   105,    38,   101,
    38,    38,   376,   105,   532,    41,   883,   115,    44,   427,
    38,    39,   120,   115,    41,    55,   124,    44,   120,   376,
    38,   710,   124,   856,   101,    52,    38,    55,   105,   862,
     4,   432,     6,   434,    40,   355,    72,    55,   115,    45,
   444,    38,   416,   363,   448,    38,   483,   367,    38,   452,
    38,   452,   453,   454,    38,   101,   452,   494,   459,   105,
   452,    41,    55,   591,    44,   492,    38,   471,    41,   363,
    38,    44,   452,   447,   454,    46,   477,    44,   124,   452,
    38,    39,   483,   484,    50,    51,   102,   452,     3,   463,
   447,   452,   466,   494,   495,    39,   452,    55,   664,   665,
   518,    67,    68,   452,    41,   452,   463,    41,    40,   466,
    44,   452,   452,    41,    50,    51,    44,   452,    52,   452,
    38,     3,   416,    41,   452,    91,    44,   674,   452,     3,
   557,    67,    68,    41,   611,    41,    44,   540,   565,   540,
    44,     3,   452,    38,   540,     3,     6,   548,   540,   452,
   452,   452,    47,    48,    49,    44,   557,    52,    41,   563,
   540,    40,   589,   102,   565,    38,   569,   540,   569,   596,
    92,   691,   492,   569,   494,   540,    40,   569,    92,   540,
    39,    76,   452,   586,   540,   586,   587,   588,   589,   569,
   586,   540,    19,   540,   598,   596,   569,   452,    38,   540,
   540,   575,   642,   684,   569,   540,   586,   540,   569,   898,
   649,    39,   540,   569,   102,    42,   540,    55,   655,   120,
   569,   648,   569,   597,    47,    48,    49,   102,   569,   569,
   540,    41,   633,    39,   569,    39,   569,   540,   540,   540,
    39,   569,    19,   644,   721,   569,    73,    74,    75,    44,
   677,    78,    39,    76,   655,    38,    39,    38,    39,   569,
    52,   701,    40,    63,    39,    42,   569,   569,   569,   673,
   540,    39,    55,    92,    55,    41,   677,   104,   682,    39,
   681,   575,    39,    83,    84,   540,    39,   714,    39,    39,
   691,    38,    39,   667,    39,   102,    73,    74,    75,   569,
   703,    78,   703,   597,   708,     3,   707,   703,    55,    40,
   711,   703,    39,   714,   569,   716,    41,   691,   102,   723,
    40,    41,   726,   703,    44,   752,   727,   127,   648,   649,
   703,    44,   102,   734,   691,   709,    39,   102,   703,    39,
    38,   715,   703,    44,   781,    41,    41,   703,   775,   753,
    39,   752,    38,    41,   703,    45,   703,    55,   107,   108,
   109,   788,   703,   703,   768,    47,    48,    49,   703,    61,
   703,    41,   746,   667,   775,   703,   120,   121,   122,   703,
   781,     3,   809,    39,   785,    38,    44,   788,    40,   826,
    82,    39,   793,   703,   795,    87,    32,     3,    40,    38,
   703,   703,   703,   101,   126,    41,    52,   105,   809,    39,
    91,    47,    45,   787,    52,   709,   790,   115,    69,    70,
    71,   715,   120,    38,    44,   826,   124,    72,    72,   856,
   124,   805,    38,   703,    45,   862,    45,   841,    72,   840,
    76,    77,   869,    74,    39,    39,    45,   874,   703,    55,
   877,    38,   746,   854,   828,   856,    39,   831,    76,    41,
   864,   862,    39,     3,   891,   892,    41,    72,   869,   789,
   790,    39,   108,   874,   901,    38,   877,   169,    40,   883,
   172,   173,    41,    39,    38,   859,   860,   179,    40,     3,
   891,   892,   184,   787,    41,   101,   790,    41,    39,   105,
   901,     3,    92,   195,   140,    41,   880,    39,   144,   115,
   146,   805,    38,     3,   206,   207,     6,   153,   124,    44,
   212,   213,    45,   215,    38,    41,   326,    41,    39,    41,
    38,    41,   332,    40,   828,    39,    38,   831,    38,    45,
    39,   177,    39,    44,   180,    41,     3,    39,    38,     0,
    39,   187,    38,    41,     3,     4,    39,     6,    39,    39,
     0,   459,     3,   586,   436,   859,   860,   203,   368,   586,
    72,   586,   466,   587,   691,   211,   586,   213,   214,   432,
   216,    38,   218,   219,   220,   495,   880,   101,   376,    38,
    45,   105,    41,   707,   494,    44,   840,    38,   795,   101,
    41,   115,   862,   105,   851,     3,   120,   243,    20,   572,
   124,   101,     3,   115,    -1,   105,    -1,    -1,   120,   255,
   256,    -1,   124,   259,    -1,   115,    -1,    -1,    -1,    -1,
   120,    -1,   268,   269,   124,    -1,    -1,     3,     3,    -1,
    38,    -1,    -1,    41,   101,    -1,   282,    38,   105,    -1,
   286,   100,   101,    -1,    -1,   455,   105,    -1,   115,    -1,
   101,    -1,    -1,   120,   105,    -1,   115,   124,   304,    -1,
    -1,   120,    38,    38,   115,   124,    47,    48,    49,   120,
    -1,    52,   318,   124,   118,   119,   120,   121,   122,    -1,
   142,   143,    -1,    -1,    -1,     3,    -1,    -1,    -1,    -1,
    -1,    72,    -1,   101,    -1,    76,    -1,   105,    -1,    -1,
   101,     3,    -1,    -1,   105,   351,   352,   115,    -1,   355,
    -1,   357,   120,   359,   115,    -1,   124,   363,    -1,   120,
    38,    -1,    -1,   124,    -1,   101,   101,    -1,    -1,   105,
   105,   433,    -1,   435,   436,    -1,    38,    55,    -1,   115,
   115,   443,   444,   445,   120,   120,    -1,    -1,   124,   124,
    -1,     3,    -1,    55,   400,    -1,    -1,    -1,    -1,    -1,
   462,   571,   464,   409,    -1,    -1,    -1,   413,   230,   471,
   232,   233,   234,   235,   236,   237,    -1,     3,    -1,    -1,
    -1,   427,   428,   101,    -1,     3,    38,   105,    -1,    -1,
    -1,    -1,     3,    -1,    -1,    -1,    -1,   115,   444,   101,
    -1,   263,   448,   105,    -1,    -1,   124,    -1,    -1,    -1,
    -1,    -1,    38,   115,    -1,    -1,   278,    94,    95,   281,
    38,    -1,   124,    -1,   470,   471,   472,    38,    -1,   475,
    -1,    -1,   642,    -1,    -1,    -1,    -1,    55,    -1,   116,
   117,   118,   119,   120,   121,   122,    -1,    -1,   494,   101,
    -1,    -1,    -1,   105,   116,   117,   118,   119,   120,   121,
   122,   563,    -1,   115,    -1,    -1,    -1,   513,   570,    -1,
   572,    -1,   124,    -1,   684,   101,    -1,    -1,   524,   105,
    -1,    -1,    -1,   101,    -1,    -1,   532,   105,    -1,   115,
   101,   701,    -1,    -1,   105,    -1,    -1,   115,   124,    -1,
    -1,    -1,    -1,    -1,   115,    -1,   124,    -1,    -1,    -1,
    -1,    -1,    -1,   124,    -1,   725,    -1,   563,   380,   381,
   382,   383,   384,   385,   386,   387,   388,   389,   390,   391,
   392,   393,   394,   395,   396,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,   591,    -1,    -1,   410,    -1,
    -1,    -1,   598,    -1,    -1,   417,     3,    -1,    -1,    -1,
    -1,    -1,   664,   665,    -1,    -1,    -1,    39,   670,    -1,
   672,    -1,   674,    20,    21,    22,    23,    24,    25,    26,
    27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
    37,    38,    -1,    -1,    -1,    -1,   642,    -1,    -1,    46,
    47,    48,    49,    50,    51,     6,    -1,    -1,    55,    -1,
    11,    12,    13,    14,    15,    16,    17,    18,    -1,    66,
    67,    68,    94,    95,    96,    97,    98,   673,   490,   491,
   492,    -1,    -1,    -1,    -1,    -1,   682,    -1,    -1,   111,
   112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
   122,    -1,   754,    -1,   101,    -1,    -1,   104,   105,    -1,
    -1,    -1,   708,    -1,    -1,     3,    -1,    -1,   115,    -1,
    -1,    -1,    -1,   120,    -1,    -1,    -1,   723,    -1,    -1,
   726,   727,    20,    21,    22,    23,    24,    25,    26,    27,
    28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
    38,    -1,    -1,    -1,    -1,    -1,    -1,   753,    46,    47,
    48,    49,    50,    51,    -1,    -1,    -1,    55,    -1,    -1,
    -1,    -1,   768,    -1,    94,    95,    96,    97,    66,    67,
    68,    -1,   778,    -1,    94,    95,    96,    97,    -1,    -1,
    -1,    -1,    -1,    -1,   790,   115,   116,   117,   118,   119,
   120,   121,   122,   799,   114,   115,   116,   117,   118,   119,
   120,   121,   122,   101,    -1,    -1,   104,   105,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,    -1,    -1,
    -1,    -1,   120,    -1,    -1,   647,   648,    -1,   650,    -1,
    -1,    -1,   654,    -1,    -1,   841,    -1,    -1,    -1,    94,
    95,    96,    97,    -1,    -1,   851,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   864,    -1,
   866,   116,   117,   118,   119,   120,   121,   122,    -1,    -1,
    -1,    -1,     1,    -1,     3,     4,    -1,   883,     7,     8,
     9,    -1,    -1,    -1,   706,    -1,    -1,    -1,    -1,    -1,
    19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
    29,    30,    31,    32,    33,    34,    35,    36,    37,    -1,
    -1,    -1,    41,    42,    -1,   737,    45,    -1,    47,    48,
    49,    50,    51,    -1,    -1,    -1,    -1,    -1,    -1,    58,
    59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
    69,    70,    71,    -1,    73,    74,    75,    -1,    77,    78,
    79,    -1,    -1,    -1,    83,    -1,    85,    86,    87,    88,
    89,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   791,
    99,    -1,   101,    -1,    -1,   104,   105,   106,    -1,     3,
    -1,    -1,     6,    -1,    -1,    -1,    10,    11,    12,    13,
    14,    15,    16,    17,    18,   124,    20,    21,    22,    23,
    24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
    34,    35,    36,    37,    38,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    46,    47,    48,    49,    50,    51,    -1,    53,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,    -1,
     3,     6,    66,    67,    68,    -1,    11,    12,    13,    14,
    15,    16,    17,    18,    -1,    -1,    -1,    20,    21,    22,
    23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
    33,    34,    35,    36,    37,    -1,    -1,   101,    -1,    -1,
   104,   105,    -1,    -1,    -1,    -1,    -1,    50,    51,    -1,
    -1,   115,    -1,    -1,   118,   119,   120,    -1,    -1,     3,
   124,   125,     6,    66,    67,    68,    10,    11,    12,    13,
    14,    15,    16,    17,    18,    -1,    20,    21,    22,    23,
    24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
    34,    35,    36,    37,    38,    -1,    -1,    -1,   101,    -1,
    -1,    -1,   105,    47,    48,    49,    50,    51,    -1,    53,
    -1,    -1,    56,    -1,    -1,    -1,    -1,   120,     1,    -1,
     3,    -1,    66,    67,    68,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    20,    21,    22,
    23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
    33,    34,    35,    36,    37,    -1,    -1,   101,    -1,    -1,
    -1,   105,    -1,    46,    47,    48,    49,    50,    51,    -1,
    -1,   115,    -1,    -1,   118,   119,   120,    -1,    -1,     3,
   124,   125,     6,    66,    67,    68,    10,    11,    12,    13,
    14,    15,    16,    17,    18,    -1,    20,    21,    22,    23,
    24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
    34,    35,    36,    37,    38,    -1,    -1,    -1,   101,    -1,
    -1,   104,   105,    47,    48,    49,    50,    51,    -1,    53,
    -1,    -1,    56,    -1,    -1,    -1,    -1,    -1,     1,    -1,
     3,    -1,    66,    67,    68,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    20,    21,    22,
    23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
    33,    34,    35,    36,    37,    -1,    -1,   101,    -1,    -1,
    -1,   105,    -1,    46,    47,    48,    49,    50,    51,    -1,
    -1,   115,    -1,    -1,   118,   119,   120,    -1,    -1,     3,
   124,   125,     6,    66,    67,    68,    10,    11,    12,    13,
    14,    15,    16,    17,    18,    -1,    20,    21,    22,    23,
    24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
    34,    35,    36,    37,    38,    -1,    -1,    -1,   101,    -1,
    -1,   104,   105,    47,    48,    49,    50,    51,    -1,    53,
    -1,    -1,    56,    -1,    -1,    -1,    -1,    -1,     1,    -1,
     3,    -1,    66,    67,    68,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    20,    21,    22,
    23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
    33,    34,    35,    36,    37,    -1,    -1,   101,    -1,    -1,
    -1,   105,    -1,    -1,    47,    48,    49,    50,    51,    -1,
    -1,   115,    -1,    -1,   118,   119,   120,    -1,    -1,     3,
   124,   125,     6,    66,    67,    68,    10,    11,    12,    13,
    14,    15,    16,    17,    18,    -1,    20,    21,    22,    23,
    24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
    34,    35,    36,    37,    38,    -1,    -1,    -1,   101,    -1,
    -1,    -1,   105,    47,    48,    49,    50,    51,    -1,    53,
    -1,    -1,    56,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     3,    -1,    66,    67,    68,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    20,    21,    22,
    23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
    33,    34,    35,    36,    37,    -1,    -1,   101,    -1,    -1,
    -1,   105,    -1,    46,    47,    48,    49,    50,    51,    -1,
    -1,   115,    -1,    -1,   118,   119,   120,    -1,    -1,     3,
   124,   125,     6,    66,    67,    68,    10,    11,    12,    13,
    14,    15,    16,    17,    18,    -1,    20,    21,    22,    23,
    24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
    34,    35,    36,    37,    38,    39,    -1,    -1,   101,    -1,
    -1,   104,   105,    47,    48,    49,    50,    51,    -1,    53,
    -1,    -1,   115,    -1,    -1,    -1,    -1,   120,    -1,    -1,
     3,    -1,    66,    67,    68,    -1,    -1,    -1,    11,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    20,    21,    22,
    23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
    33,    34,    35,    36,    37,    -1,    -1,   101,    -1,    -1,
    -1,   105,    -1,    46,    47,    48,    49,    50,    51,    -1,
    -1,   115,    -1,    -1,   118,   119,   120,    -1,    -1,     3,
   124,   125,     6,    66,    67,    68,    10,    11,    12,    13,
    14,    15,    16,    17,    18,    -1,    20,    21,    22,    23,
    24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
    34,    35,    36,    37,    38,    -1,    -1,    -1,   101,    -1,
    -1,   104,   105,    47,    48,    49,    50,    51,    -1,    53,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     3,    -1,    66,    67,    68,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    20,    21,    22,
    23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
    33,    34,    35,    36,    37,    -1,    -1,   101,    -1,    -1,
    -1,   105,    -1,    46,    47,    48,    49,    50,    51,    -1,
    -1,   115,    -1,    -1,   118,   119,   120,    41,    -1,     3,
   124,   125,     6,    66,    67,    68,    10,    11,    12,    13,
    14,    15,    16,    17,    18,    -1,    20,    21,    22,    23,
    24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
    34,    35,    36,    37,    38,    -1,    -1,    -1,   101,    -1,
    -1,   104,   105,    47,    48,    49,    50,    51,    -1,    53,
    94,    95,    96,    97,    98,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    66,    67,    68,    -1,    -1,   111,   112,   113,
   114,   115,   116,   117,   118,   119,   120,   121,   122,    20,
    21,    22,    23,    24,    25,    26,    27,    -1,    29,    -1,
    31,    -1,    -1,    34,    35,    36,    37,   101,    -1,    -1,
    -1,   105,    -1,     0,     1,    -1,     3,     4,    -1,    -1,
     7,     8,     9,    -1,   118,   119,   120,    -1,    -1,    -1,
   124,   125,    19,    20,    21,    22,    23,    24,    25,    26,
    27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
    37,    -1,    -1,    -1,    41,    42,    -1,    -1,    -1,    -1,
    47,    48,    49,    50,    51,    -1,    -1,    54,    -1,    -1,
    -1,    58,    59,    60,    61,    62,    63,    64,    65,    66,
    67,    68,    -1,    -1,    -1,    -1,    73,    74,    75,    -1,
    77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
    87,    88,    89,    90,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    99,     1,   101,     3,     4,   104,   105,     7,
     8,     9,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    19,    20,    21,    22,    23,    24,    25,    26,    27,
    28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
    -1,    -1,    -1,    41,    42,    -1,    -1,    45,    -1,    47,
    48,    49,    50,    51,    -1,    -1,    54,    -1,    -1,    -1,
    58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
    68,    -1,    -1,    -1,    -1,    73,    74,    75,    -1,    77,
    78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
    88,    89,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    99,     1,   101,     3,     4,   104,   105,     7,     8,
     9,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
    29,    30,    31,    32,    33,    34,    35,    36,    37,    -1,
    -1,    -1,    41,    42,    -1,    -1,    45,    -1,    47,    48,
    49,    50,    51,    -1,    -1,    54,    -1,    -1,    -1,    58,
    59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
    -1,    -1,    -1,    -1,    73,    74,    75,    -1,    77,    78,
    79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
    89,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    99,     1,   101,     3,     4,   104,   105,     7,     8,     9,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    19,
    20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
    30,    31,    32,    33,    34,    35,    36,    37,    -1,    -1,
    -1,    41,    42,    -1,    -1,    45,    -1,    47,    48,    49,
    50,    51,    -1,    -1,    54,    -1,    -1,    -1,    58,    59,
    60,    61,    62,    63,    64,    65,    66,    67,    68,    -1,
    -1,    -1,    -1,    73,    74,    75,    -1,    77,    78,    79,
    80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,
     1,   101,     3,     4,   104,   105,     7,     8,     9,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    19,    20,
    21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
    31,    32,    33,    34,    35,    36,    37,    -1,    -1,    -1,
    41,    42,    -1,    -1,    -1,    -1,    47,    48,    49,    50,
    51,    -1,    -1,    54,    -1,    56,    -1,    58,    59,    60,
    61,    62,    63,    64,    65,    66,    67,    68,    -1,    -1,
    -1,    -1,    73,    74,    75,    -1,    77,    78,    79,    80,
    81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
     3,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,
   101,    -1,    -1,   104,   105,    -1,    -1,    20,    21,    22,
    23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
    33,    34,    35,    36,    37,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    47,    48,    49,    50,    51,     3,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    66,    67,    68,    20,    21,    22,    23,
    24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
    34,    35,    36,    37,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    47,    48,    49,    50,    51,   101,    -1,
    -1,    -1,   105,   106,    -1,    -1,    -1,    -1,    -1,     3,
    -1,    -1,    66,    67,    68,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    20,    21,    22,    23,
    24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
    34,    35,    36,    37,    38,    -1,    -1,   101,    -1,    -1,
    -1,   105,   106,    47,    48,    49,    50,    51,     3,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    66,    67,    68,    20,    21,    22,    23,    24,
    25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
    35,    36,    37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    47,    48,    49,    50,    51,   101,    -1,    -1,
    -1,   105,    -1,    -1,    -1,    -1,    -1,     3,    -1,    -1,
    -1,    66,    67,    68,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    20,    21,    22,    23,    24,    25,
    26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
    36,    37,    -1,    -1,    -1,    -1,   101,    -1,    -1,    -1,
   105,    47,    48,    49,    50,    51,     3,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    66,    67,    68,    20,    21,    22,    23,    24,    25,    26,
    27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
    37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    47,    48,    49,    50,    51,   101,    -1,    -1,     3,   105,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    66,
    67,    68,    56,    -1,    -1,    20,    21,    22,    23,    24,
    25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
    35,    36,    37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,   101,    50,    51,    56,   105,    -1,
    94,    95,    96,    97,    98,    -1,    -1,    -1,    -1,    -1,
    -1,    66,    67,    68,    -1,    -1,    -1,   111,   112,   113,
   114,   115,   116,   117,   118,   119,   120,   121,   122,    56,
    -1,    -1,    -1,    -1,    -1,    94,    95,    96,    97,    98,
    -1,    56,    -1,    -1,    -1,    -1,   101,    -1,    -1,    -1,
   105,    -1,   111,   112,   113,   114,   115,   116,   117,   118,
   119,   120,   121,   122,    -1,    -1,    -1,    94,    95,    96,
    97,    98,    56,    -1,    -1,    -1,    -1,    -1,    -1,    94,
    95,    96,    97,    98,   111,   112,   113,   114,   115,   116,
   117,   118,   119,   120,   121,   122,   111,   112,   113,   114,
   115,   116,   117,   118,   119,   120,   121,   122,    -1,    -1,
    94,    95,    96,    97,    98,    -1,    -1,    72,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,   111,   112,   113,
   114,   115,   116,   117,   118,   119,   120,   121,   122,    94,
    95,    96,    97,    98,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    94,    95,    96,    97,    98,   111,   112,   113,   114,
   115,   116,   117,   118,   119,   120,   121,   122,   111,   112,
   113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
    94,    95,    96,    97,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    94,    95,    96,    97,    -1,    -1,   111,   112,   113,
   114,   115,   116,   117,   118,   119,   120,   121,   122,   112,
   113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
    94,    95,    96,    97,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,
   114,   115,   116,   117,   118,   119,   120,   121,   122
};
/* -*-C-*-  Note some compilers choke on comments on `#line' lines.  */
#line 3 "bison.simple"
/* This file comes from bison-1.28.  */

/* Skeleton output parser for bison,
   Copyright (C) 1984, 1989, 1990 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

/* This is the parser code that is written into each bison parser
  when the %semantic_parser declaration is not specified in the grammar.
  It was written by Richard Stallman by simplifying the hairy parser
  used when %semantic_parser is specified.  */

#ifndef YYSTACK_USE_ALLOCA
#ifdef alloca
#define YYSTACK_USE_ALLOCA
#else /* alloca not defined */
#ifdef __GNUC__
#define YYSTACK_USE_ALLOCA
#define alloca __builtin_alloca
#else /* not GNU C.  */
#if (!defined (__STDC__) && defined (sparc)) || defined (__sparc__) || defined (__sparc) || defined (__sgi) || (defined (__sun) && defined (__i386))
#define YYSTACK_USE_ALLOCA
#include <alloca.h>
#else /* not sparc */
/* We think this test detects Watcom and Microsoft C.  */
/* This used to test MSDOS, but that is a bad idea
   since that symbol is in the user namespace.  */
#if (defined (_MSDOS) || defined (_MSDOS_)) && !defined (__TURBOC__)
#if 0 /* No need for malloc.h, which pollutes the namespace;
	 instead, just don't use alloca.  */
#include <malloc.h>
#endif
#else /* not MSDOS, or __TURBOC__ */
#if defined(_AIX)
/* I don't know what this was needed for, but it pollutes the namespace.
   So I turned it off.   rms, 2 May 1997.  */
/* #include <malloc.h>  */
 #pragma alloca
#define YYSTACK_USE_ALLOCA
#else /* not MSDOS, or __TURBOC__, or _AIX */
#if 0
#ifdef __hpux /* haible@ilog.fr says this works for HPUX 9.05 and up,
		 and on HPUX 10.  Eventually we can turn this on.  */
#define YYSTACK_USE_ALLOCA
#define alloca __builtin_alloca
#endif /* __hpux */
#endif
#endif /* not _AIX */
#endif /* not MSDOS, or __TURBOC__ */
#endif /* not sparc */
#endif /* not GNU C */
#endif /* alloca not defined */
#endif /* YYSTACK_USE_ALLOCA not defined */

#ifdef YYSTACK_USE_ALLOCA
#define YYSTACK_ALLOC alloca
#else
#define YYSTACK_ALLOC malloc
#endif

/* Note: there must be only one dollar sign in this file.
   It is replaced by the list of actions, each action
   as one case of the switch.  */

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		-2
#define YYEOF		0
#define YYACCEPT	goto yyacceptlab
#define YYABORT 	goto yyabortlab
#define YYERROR		goto yyerrlab1
/* Like YYERROR except do call yyerror.
   This remains here temporarily to ease the
   transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */
#define YYFAIL		goto yyerrlab
#define YYRECOVERING()  (!!yyerrstatus)
#define YYBACKUP(token, value) \
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    { yychar = (token), yylval = (value);			\
      yychar1 = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { yyerror ("syntax error: cannot back up"); YYERROR; }	\
while (0)

#define YYTERROR	1
#define YYERRCODE	256

#ifndef YYPURE
#define YYLEX		yylex()
#endif

#ifdef YYPURE
#ifdef YYLSP_NEEDED
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, &yylloc, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval, &yylloc)
#endif
#else /* not YYLSP_NEEDED */
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval)
#endif
#endif /* not YYLSP_NEEDED */
#endif

/* If nonreentrant, generate the variables here */

#ifndef YYPURE

int	yychar;			/*  the lookahead symbol		*/
YYSTYPE	yylval;			/*  the semantic value of the		*/
				/*  lookahead symbol			*/

#ifdef YYLSP_NEEDED
YYLTYPE yylloc;			/*  location data for the lookahead	*/
				/*  symbol				*/
#endif

int yynerrs;			/*  number of parse errors so far       */
#endif  /* not YYPURE */

#if YYDEBUG != 0
int yydebug;			/*  nonzero means print parse trace	*/
/* Since this is uninitialized, it does not stop multiple parsers
   from coexisting.  */
#endif

/*  YYINITDEPTH indicates the initial size of the parser's stacks	*/

#ifndef	YYINITDEPTH
#define YYINITDEPTH 200
#endif

/*  YYMAXDEPTH is the maximum size the stacks can grow to
    (effective only if the built-in stack extension method is used).  */

#if YYMAXDEPTH == 0
#undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
#define YYMAXDEPTH 10000
#endif

/* Define __yy_memcpy.  Note that the size argument
   should be passed with type unsigned int, because that is what the non-GCC
   definitions require.  With GCC, __builtin_memcpy takes an arg
   of type size_t, but it can handle unsigned int.  */

#if __GNUC__ > 1		/* GNU C and GNU C++ define this.  */
#define __yy_memcpy(TO,FROM,COUNT)	__builtin_memcpy(TO,FROM,COUNT)
#else				/* not GNU C or C++ */
#ifndef __cplusplus

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_memcpy (to, from, count)
     char *to;
     char *from;
     unsigned int count;
{
  register char *f = from;
  register char *t = to;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#else /* __cplusplus */

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_memcpy (char *to, char *from, unsigned int count)
{
  register char *t = to;
  register char *f = from;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#endif
#endif

#line 217 "bison.simple"

/* The user can define YYPARSE_PARAM as the name of an argument to be passed
   into yyparse.  The argument should have type void *.
   It should actually point to an object.
   Grammar actions can access the variable by casting it
   to the proper pointer type.  */

#ifdef YYPARSE_PARAM
#ifdef __cplusplus
#define YYPARSE_PARAM_ARG void *YYPARSE_PARAM
#define YYPARSE_PARAM_DECL
#else /* not __cplusplus */
#define YYPARSE_PARAM_ARG YYPARSE_PARAM
#define YYPARSE_PARAM_DECL void *YYPARSE_PARAM;
#endif /* not __cplusplus */
#else /* not YYPARSE_PARAM */
#define YYPARSE_PARAM_ARG
#define YYPARSE_PARAM_DECL
#endif /* not YYPARSE_PARAM */

/* Prevent warning if -Wstrict-prototypes.  */
#ifdef __GNUC__
#ifdef YYPARSE_PARAM
int yyparse (void *);
#else
int yyparse (void);
#endif
#endif

int
yyparse(YYPARSE_PARAM_ARG)
     YYPARSE_PARAM_DECL
{
  register int yystate;
  register int yyn;
  register short *yyssp;
  register YYSTYPE *yyvsp;
  int yyerrstatus;	/*  number of tokens to shift before error messages enabled */
  int yychar1 = 0;		/*  lookahead token as an internal (translated) token number */

  short	yyssa[YYINITDEPTH];	/*  the state stack			*/
  YYSTYPE yyvsa[YYINITDEPTH];	/*  the semantic value stack		*/

  short *yyss = yyssa;		/*  refer to the stacks thru separate pointers */
  YYSTYPE *yyvs = yyvsa;	/*  to allow yyoverflow to reallocate them elsewhere */

#ifdef YYLSP_NEEDED
  YYLTYPE yylsa[YYINITDEPTH];	/*  the location stack			*/
  YYLTYPE *yyls = yylsa;
  YYLTYPE *yylsp;

#define YYPOPSTACK   (yyvsp--, yyssp--, yylsp--)
#else
#define YYPOPSTACK   (yyvsp--, yyssp--)
#endif

  int yystacksize = YYINITDEPTH;
  int yyfree_stacks = 0;

#ifdef YYPURE
  int yychar;
  YYSTYPE yylval;
  int yynerrs;
#ifdef YYLSP_NEEDED
  YYLTYPE yylloc;
#endif
#endif

  YYSTYPE yyval;		/*  the variable used to return		*/
				/*  semantic values from the action	*/
				/*  routines				*/

  int yylen;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Starting parse\n");
#endif

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss - 1;
  yyvsp = yyvs;
#ifdef YYLSP_NEEDED
  yylsp = yyls;
#endif

/* Push a new state, which is found in  yystate  .  */
/* In all cases, when you get here, the value and location stacks
   have just been pushed. so pushing a state here evens the stacks.  */
yynewstate:

  *++yyssp = yystate;

  if (yyssp >= yyss + yystacksize - 1)
    {
      /* Give user a chance to reallocate the stack */
      /* Use copies of these so that the &'s don't force the real ones into memory. */
      YYSTYPE *yyvs1 = yyvs;
      short *yyss1 = yyss;
#ifdef YYLSP_NEEDED
      YYLTYPE *yyls1 = yyls;
#endif

      /* Get the current used size of the three stacks, in elements.  */
      int size = yyssp - yyss + 1;

#ifdef yyoverflow
      /* Each stack pointer address is followed by the size of
	 the data in use in that stack, in bytes.  */
#ifdef YYLSP_NEEDED
      /* This used to be a conditional around just the two extra args,
	 but that might be undefined if yyoverflow is a macro.  */
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yyls1, size * sizeof (*yylsp),
		 &yystacksize);
#else
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yystacksize);
#endif

      yyss = yyss1; yyvs = yyvs1;
#ifdef YYLSP_NEEDED
      yyls = yyls1;
#endif
#else /* no yyoverflow */
      /* Extend the stack our own way.  */
      if (yystacksize >= YYMAXDEPTH)
	{
	  yyerror("parser stack overflow");
	  if (yyfree_stacks)
	    {
	      free (yyss);
	      free (yyvs);
#ifdef YYLSP_NEEDED
	      free (yyls);
#endif
	    }
	  return 2;
	}
      yystacksize *= 2;
      if (yystacksize > YYMAXDEPTH)
	yystacksize = YYMAXDEPTH;
#ifndef YYSTACK_USE_ALLOCA
      yyfree_stacks = 1;
#endif
      yyss = (short *) YYSTACK_ALLOC (yystacksize * sizeof (*yyssp));
      __yy_memcpy ((char *)yyss, (char *)yyss1,
		   size * (unsigned int) sizeof (*yyssp));
      yyvs = (YYSTYPE *) YYSTACK_ALLOC (yystacksize * sizeof (*yyvsp));
      __yy_memcpy ((char *)yyvs, (char *)yyvs1,
		   size * (unsigned int) sizeof (*yyvsp));
#ifdef YYLSP_NEEDED
      yyls = (YYLTYPE *) YYSTACK_ALLOC (yystacksize * sizeof (*yylsp));
      __yy_memcpy ((char *)yyls, (char *)yyls1,
		   size * (unsigned int) sizeof (*yylsp));
#endif
#endif /* no yyoverflow */

      yyssp = yyss + size - 1;
      yyvsp = yyvs + size - 1;
#ifdef YYLSP_NEEDED
      yylsp = yyls + size - 1;
#endif

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Stack size increased to %d\n", yystacksize);
#endif

      if (yyssp >= yyss + yystacksize - 1)
	YYABORT;
    }

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Entering state %d\n", yystate);
#endif

  goto yybackup;
 yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* yychar is either YYEMPTY or YYEOF
     or a valid token in external form.  */

  if (yychar == YYEMPTY)
    {
#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Reading a token: ");
#endif
      yychar = YYLEX;
    }

  /* Convert token to internal form (in yychar1) for indexing tables with */

  if (yychar <= 0)		/* This means end of input. */
    {
      yychar1 = 0;
      yychar = YYEOF;		/* Don't call YYLEX any more */

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Now at end of input.\n");
#endif
    }
  else
    {
      yychar1 = YYTRANSLATE(yychar);

#if YYDEBUG != 0
      if (yydebug)
	{
	  fprintf (stderr, "Next token is %d (%s", yychar, yytname[yychar1]);
	  /* Give the individual parser a way to print the precise meaning
	     of a token, for further debugging info.  */
#ifdef YYPRINT
	  YYPRINT (stderr, yychar, yylval);
#endif
	  fprintf (stderr, ")\n");
	}
#endif
    }

  yyn += yychar1;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != yychar1)
    goto yydefault;

  yyn = yytable[yyn];

  /* yyn is what to do for this token type in this state.
     Negative => reduce, -yyn is rule number.
     Positive => shift, yyn is new state.
       New state is final state => don't bother to shift,
       just return success.
     0, or most negative number => error.  */

  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrlab;

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting token %d (%s), ", yychar, yytname[yychar1]);
#endif

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  /* count tokens shifted since error; after three, turn off error status.  */
  if (yyerrstatus) yyerrstatus--;

  yystate = yyn;
  goto yynewstate;

/* Do the default action for the current state.  */
yydefault:

  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;

/* Do a reduction.  yyn is the number of a rule to reduce with.  */
yyreduce:
  yylen = yyr2[yyn];
  if (yylen > 0)
    yyval = yyvsp[1-yylen]; /* implement default value of the action */

#if YYDEBUG != 0
  if (yydebug)
    {
      int i;

      fprintf (stderr, "Reducing via rule %d (line %d), ",
	       yyn, yyrline[yyn]);

      /* Print the symbols being reduced, and their result.  */
      for (i = yyprhs[yyn]; yyrhs[i] > 0; i++)
	fprintf (stderr, "%s ", yytname[yyrhs[i]]);
      fprintf (stderr, " -> %s\n", yytname[yyr1[yyn]]);
    }
#endif


  switch (yyn) {

case 1:
#line 1745 "parser.y"
{
                   if (!classes) classes = NewHash();
		   Setattr(yyvsp[0].node,"classes",classes); 
		   Setattr(yyvsp[0].node,"name",ModuleName);
		   
		   if ((!module_node) && ModuleName) {
		     module_node = new_node("module");
		     Setattr(module_node,"name",ModuleName);
		   }
		   Setattr(yyvsp[0].node,"module",module_node);
		   check_extensions();
	           top = yyvsp[0].node;
               ;
    break;}
case 2:
#line 1758 "parser.y"
{
                 top = Copy(Getattr(yyvsp[-1].p,"type"));
		 Delete(yyvsp[-1].p);
               ;
    break;}
case 3:
#line 1762 "parser.y"
{
                 top = 0;
               ;
    break;}
case 4:
#line 1765 "parser.y"
{
                 top = yyvsp[-1].p;
               ;
    break;}
case 5:
#line 1768 "parser.y"
{
                 top = 0;
               ;
    break;}
case 6:
#line 1771 "parser.y"
{
                 top = yyvsp[-2].pl;
               ;
    break;}
case 7:
#line 1774 "parser.y"
{
                 top = 0;
               ;
    break;}
case 8:
#line 1779 "parser.y"
{  
                   /* add declaration to end of linked list (the declaration isn't always a single declaration, sometimes it is a linked list itself) */
                   appendChild(yyvsp[-1].node,yyvsp[0].node);
                   yyval.node = yyvsp[-1].node;
               ;
    break;}
case 9:
#line 1784 "parser.y"
{
                   yyval.node = new_node("top");
               ;
    break;}
case 10:
#line 1789 "parser.y"
{ yyval.node = yyvsp[0].node; ;
    break;}
case 11:
#line 1790 "parser.y"
{ yyval.node = yyvsp[0].node; ;
    break;}
case 12:
#line 1791 "parser.y"
{ yyval.node = yyvsp[0].node; ;
    break;}
case 13:
#line 1792 "parser.y"
{ yyval.node = 0; ;
    break;}
case 14:
#line 1793 "parser.y"
{
                  yyval.node = 0;
		  Swig_error(cparse_file, cparse_line,"Syntax error in input(1).\n");
		  exit(1);
               ;
    break;}
case 15:
#line 1799 "parser.y"
{ 
                  if (yyval.node) {
   		      add_symbols(yyval.node);
                  }
                  yyval.node = yyvsp[0].node; 
	       ;
    break;}
case 16:
#line 1815 "parser.y"
{
                  yyval.node = 0;
                  skip_decl();
               ;
    break;}
case 17:
#line 1825 "parser.y"
{ yyval.node = yyvsp[0].node; ;
    break;}
case 18:
#line 1826 "parser.y"
{ yyval.node = yyvsp[0].node; ;
    break;}
case 19:
#line 1827 "parser.y"
{ yyval.node = yyvsp[0].node; ;
    break;}
case 20:
#line 1828 "parser.y"
{ yyval.node = yyvsp[0].node; ;
    break;}
case 21:
#line 1829 "parser.y"
{ yyval.node = yyvsp[0].node; ;
    break;}
case 22:
#line 1830 "parser.y"
{ yyval.node = yyvsp[0].node; ;
    break;}
case 23:
#line 1831 "parser.y"
{ yyval.node = yyvsp[0].node; ;
    break;}
case 24:
#line 1832 "parser.y"
{ yyval.node = yyvsp[0].node; ;
    break;}
case 25:
#line 1833 "parser.y"
{ yyval.node = yyvsp[0].node; ;
    break;}
case 26:
#line 1834 "parser.y"
{ yyval.node = yyvsp[0].node; ;
    break;}
case 27:
#line 1835 "parser.y"
{ yyval.node = yyvsp[0].node; ;
    break;}
case 28:
#line 1836 "parser.y"
{ yyval.node = yyvsp[0].node; ;
    break;}
case 29:
#line 1837 "parser.y"
{ yyval.node = yyvsp[0].node; ;
    break;}
case 30:
#line 1838 "parser.y"
{ yyval.node = yyvsp[0].node; ;
    break;}
case 31:
#line 1839 "parser.y"
{ yyval.node = yyvsp[0].node; ;
    break;}
case 32:
#line 1840 "parser.y"
{ yyval.node = yyvsp[0].node; ;
    break;}
case 33:
#line 1841 "parser.y"
{ yyval.node = yyvsp[0].node; ;
    break;}
case 34:
#line 1842 "parser.y"
{ yyval.node = yyvsp[0].node; ;
    break;}
case 35:
#line 1843 "parser.y"
{ yyval.node = yyvsp[0].node; ;
    break;}
case 36:
#line 1844 "parser.y"
{ yyval.node = yyvsp[0].node; ;
    break;}
case 37:
#line 1845 "parser.y"
{ yyval.node = yyvsp[0].node; ;
    break;}
case 38:
#line 1852 "parser.y"
{
               Node *cls;
	       String *clsname;
	       cplus_mode = CPLUS_PUBLIC;
	       if (!classes) classes = NewHash();
	       if (!extendhash) extendhash = NewHash();
	       clsname = make_class_name(yyvsp[-1].str);
	       cls = Getattr(classes,clsname);
	       if (!cls) {
		 /* No previous definition. Create a new scope */
		 Node *am = Getattr(extendhash,clsname);
		 if (!am) {
		   Swig_symbol_newscope();
		   Swig_symbol_setscopename(yyvsp[-1].str);
		   prev_symtab = 0;
		 } else {
		   prev_symtab = Swig_symbol_setscope(Getattr(am,"symtab"));
		 }
		 current_class = 0;
	       } else {
		 /* Previous class definition.  Use its symbol table */
		 prev_symtab = Swig_symbol_setscope(Getattr(cls,"symtab"));
		 current_class = cls;
		 extendmode = 1;
	       }
	       Classprefix = NewString(yyvsp[-1].str);
	       Namespaceprefix= Swig_symbol_qualifiedscopename(0);
	       Delete(clsname);
	     ;
    break;}
case 39:
#line 1880 "parser.y"
{
               String *clsname;
	       extendmode = 0;
               yyval.node = new_node("extend");
	       Setattr(yyval.node,"symtab",Swig_symbol_popscope());
	       if (prev_symtab) {
		 Swig_symbol_setscope(prev_symtab);
	       }
	       Namespaceprefix = Swig_symbol_qualifiedscopename(0);
               clsname = make_class_name(yyvsp[-4].str);
	       Setattr(yyval.node,"name",clsname);

	       /* Mark members as extend */

	       tag_nodes(yyvsp[-1].node,"feature:extend",(char*) "1");
	       if (current_class) {
		 /* We add the extension to the previously defined class */
		 appendChild(yyval.node,yyvsp[-1].node);
		 appendChild(current_class,yyval.node);
	       } else {
		 /* We store the extensions in the extensions hash */
		 Node *am = Getattr(extendhash,clsname);
		 if (am) {
		   /* Append the members to the previous extend methods */
		   appendChild(am,yyvsp[-1].node);
		 } else {
		   appendChild(yyval.node,yyvsp[-1].node);
		   Setattr(extendhash,clsname,yyval.node);
		 }
	       }
	       current_class = 0;
	       Delete(Classprefix);
	       Delete(clsname);
	       Classprefix = 0;
	       prev_symtab = 0;
	       yyval.node = 0;

	     ;
    break;}
case 40:
#line 1924 "parser.y"
{
                    yyval.node = new_node("apply");
                    Setattr(yyval.node,"pattern",Getattr(yyvsp[-3].p,"pattern"));
		    appendChild(yyval.node,yyvsp[-1].p);
               ;
    break;}
case 41:
#line 1934 "parser.y"
{
		 yyval.node = new_node("clear");
		 appendChild(yyval.node,yyvsp[-1].p);
               ;
    break;}
case 42:
#line 1945 "parser.y"
{
		   if ((yyvsp[-1].dtype.type != T_ERROR) && (yyvsp[-1].dtype.type != T_SYMBOL)) {
		     SwigType *type = NewSwigType(yyvsp[-1].dtype.type);
		     yyval.node = new_node("constant");
		     Setattr(yyval.node,"name",yyvsp[-3].id);
		     Setattr(yyval.node,"type",type);
		     Setattr(yyval.node,"value",yyvsp[-1].dtype.val);
		     if (yyvsp[-1].dtype.rawval) Setattr(yyval.node,"rawval", yyvsp[-1].dtype.rawval);
		     Setattr(yyval.node,"storage","%constant");
		     SetFlag(yyval.node,"feature:immutable");
		     add_symbols(yyval.node);
		     Delete(type);
		   } else {
		     if (yyvsp[-1].dtype.type == T_ERROR) {
		       Swig_warning(WARN_PARSE_UNSUPPORTED_VALUE,cparse_file,cparse_line,"Unsupported constant value (ignored)\n");
		     }
		     yyval.node = 0;
		   }

	       ;
    break;}
case 43:
#line 1966 "parser.y"
{
		 if ((yyvsp[-1].dtype.type != T_ERROR) && (yyvsp[-1].dtype.type != T_SYMBOL)) {
		   SwigType_push(yyvsp[-3].type,yyvsp[-2].decl.type);
		   /* Sneaky callback function trick */
		   if (SwigType_isfunction(yyvsp[-3].type)) {
		     SwigType_add_pointer(yyvsp[-3].type);
		   }
		   yyval.node = new_node("constant");
		   Setattr(yyval.node,"name",yyvsp[-2].decl.id);
		   Setattr(yyval.node,"type",yyvsp[-3].type);
		   Setattr(yyval.node,"value",yyvsp[-1].dtype.val);
		   if (yyvsp[-1].dtype.rawval) Setattr(yyval.node,"rawval", yyvsp[-1].dtype.rawval);
		   Setattr(yyval.node,"storage","%constant");
		   SetFlag(yyval.node,"feature:immutable");
		   add_symbols(yyval.node);
		 } else {
		     if (yyvsp[-1].dtype.type == T_ERROR) {
		       Swig_warning(WARN_PARSE_UNSUPPORTED_VALUE,cparse_file,cparse_line,"Unsupported constant value\n");
		     }
		   yyval.node = 0;
		 }
               ;
    break;}
case 44:
#line 1988 "parser.y"
{
		 Swig_warning(WARN_PARSE_BAD_VALUE,cparse_file,cparse_line,"Bad constant value (ignored).\n");
		 yyval.node = 0;
	       ;
    break;}
case 45:
#line 1999 "parser.y"
{
		 char temp[64];
		 Replace(yyvsp[0].str,"$file",cparse_file, DOH_REPLACE_ANY);
		 sprintf(temp,"%d", cparse_line);
		 Replace(yyvsp[0].str,"$line",temp,DOH_REPLACE_ANY);
		 Printf(stderr,"%s\n", yyvsp[0].str);
		 Delete(yyvsp[0].str);
                 yyval.node = 0;
	       ;
    break;}
case 46:
#line 2008 "parser.y"
{
		 char temp[64];
		 String *s = NewString(yyvsp[0].id);
		 Replace(s,"$file",cparse_file, DOH_REPLACE_ANY);
		 sprintf(temp,"%d", cparse_line);
		 Replace(s,"$line",temp,DOH_REPLACE_ANY);
		 Printf(stderr,"%s\n", s);
		 Delete(s);
                 yyval.node = 0;
               ;
    break;}
case 47:
#line 2027 "parser.y"
{
                    skip_balanced('{','}');
		    yyval.node = 0;
		    Swig_warning(WARN_DEPRECATED_EXCEPT,cparse_file, cparse_line, "%%except is deprecated.  Use %%exception instead.\n");
	       ;
    break;}
case 48:
#line 2033 "parser.y"
{
                    skip_balanced('{','}');
		    yyval.node = 0;
		    Swig_warning(WARN_DEPRECATED_EXCEPT,cparse_file, cparse_line, "%%except is deprecated.  Use %%exception instead.\n");
               ;
    break;}
case 49:
#line 2039 "parser.y"
{
		 yyval.node = 0;
		 Swig_warning(WARN_DEPRECATED_EXCEPT,cparse_file, cparse_line, "%%except is deprecated.  Use %%exception instead.\n");
               ;
    break;}
case 50:
#line 2044 "parser.y"
{
		 yyval.node = 0;
		 Swig_warning(WARN_DEPRECATED_EXCEPT,cparse_file, cparse_line, "%%except is deprecated.  Use %%exception instead.\n");
	       ;
    break;}
case 51:
#line 2051 "parser.y"
{		 
                 yyval.node = NewHash();
                 Setattr(yyval.node,"value",yyvsp[-3].id);
		 Setattr(yyval.node,"type",Getattr(yyvsp[-1].p,"type"));
               ;
    break;}
case 52:
#line 2058 "parser.y"
{
                 yyval.node = NewHash();
                 Setattr(yyval.node,"value",yyvsp[0].id);
              ;
    break;}
case 53:
#line 2062 "parser.y"
{
                yyval.node = yyvsp[0].node;
              ;
    break;}
case 54:
#line 2075 "parser.y"
{
                   Hash *p = yyvsp[-2].node;
		   yyval.node = new_node("fragment");
		   Setattr(yyval.node,"value",Getattr(yyvsp[-4].node,"value"));
		   Setattr(yyval.node,"type",Getattr(yyvsp[-4].node,"type"));
		   Setattr(yyval.node,"section",Getattr(p,"name"));
		   Setattr(yyval.node,"kwargs",nextSibling(p));
		   Setattr(yyval.node,"code",yyvsp[0].str);
                 ;
    break;}
case 55:
#line 2084 "parser.y"
{
		   Hash *p = yyvsp[-2].node;
		   String *code;
                   skip_balanced('{','}');
		   yyval.node = new_node("fragment");
		   Setattr(yyval.node,"value",Getattr(yyvsp[-4].node,"value"));
		   Setattr(yyval.node,"type",Getattr(yyvsp[-4].node,"type"));
		   Setattr(yyval.node,"section",Getattr(p,"name"));
		   Setattr(yyval.node,"kwargs",nextSibling(p));
		   Delitem(scanner_ccode,0);
		   Delitem(scanner_ccode,DOH_END);
		   code = Copy(scanner_ccode);
		   Setattr(yyval.node,"code",code);
		   Delete(code);
                 ;
    break;}
case 56:
#line 2099 "parser.y"
{
		   yyval.node = new_node("fragment");
		   Setattr(yyval.node,"value",Getattr(yyvsp[-2].node,"value"));
		   Setattr(yyval.node,"type",Getattr(yyvsp[-2].node,"type"));
		   Setattr(yyval.node,"emitonly","1");
		 ;
    break;}
case 57:
#line 2112 "parser.y"
{
                     yyvsp[-3].loc.filename = Copy(cparse_file);
		     yyvsp[-3].loc.line = cparse_line;
		     scanner_set_location(NewString(yyvsp[-1].id),1);
                     if (yyvsp[-2].node) { 
		       String *maininput = Getattr(yyvsp[-2].node, "maininput");
		       if (maininput)
		         scanner_set_main_input_file(NewString(maininput));
		     }
               ;
    break;}
case 58:
#line 2121 "parser.y"
{
                     String *mname = 0;
                     yyval.node = yyvsp[-1].node;
		     scanner_set_location(yyvsp[-6].loc.filename,yyvsp[-6].loc.line+1);
		     if (strcmp(yyvsp[-6].loc.type,"include") == 0) set_nodeType(yyval.node,"include");
		     if (strcmp(yyvsp[-6].loc.type,"import") == 0) {
		       mname = yyvsp[-5].node ? Getattr(yyvsp[-5].node,"module") : 0;
		       set_nodeType(yyval.node,"import");
		       if (import_mode) --import_mode;
		     }
		     
		     Setattr(yyval.node,"name",yyvsp[-4].id);
		     /* Search for the module (if any) */
		     {
			 Node *n = firstChild(yyval.node);
			 while (n) {
			     if (Strcmp(nodeType(n),"module") == 0) {
			         if (mname) {
				   Setattr(n,"name", mname);
				   mname = 0;
				 }
				 Setattr(yyval.node,"module",Getattr(n,"name"));
				 break;
			     }
			     n = nextSibling(n);
			 }
			 if (mname) {
			   /* There is no module node in the import
			      node, ie, you imported a .h file
			      directly.  We are forced then to create
			      a new import node with a module node.
			   */			      
			   Node *nint = new_node("import");
			   Node *mnode = new_node("module");
			   Setattr(mnode,"name", mname);
			   appendChild(nint,mnode);
			   Delete(mnode);
			   appendChild(nint,firstChild(yyval.node));
			   yyval.node = nint;
			   Setattr(yyval.node,"module",mname);
			 }
		     }
		     Setattr(yyval.node,"options",yyvsp[-5].node);
               ;
    break;}
case 59:
#line 2167 "parser.y"
{ yyval.loc.type = (char *) "include"; ;
    break;}
case 60:
#line 2168 "parser.y"
{ yyval.loc.type = (char *) "import"; ++import_mode;;
    break;}
case 61:
#line 2175 "parser.y"
{
                 String *cpps;
		 if (Namespaceprefix) {
		   Swig_error(cparse_file, cparse_start_line, "%%inline directive inside a namespace is disallowed.\n");
		   yyval.node = 0;
		 } else {
		   yyval.node = new_node("insert");
		   Setattr(yyval.node,"code",yyvsp[0].str);
		   /* Need to run through the preprocessor */
		   Seek(yyvsp[0].str,0,SEEK_SET);
		   Setline(yyvsp[0].str,cparse_start_line);
		   Setfile(yyvsp[0].str,cparse_file);
		   cpps = Preprocessor_parse(yyvsp[0].str);
		   start_inline(Char(cpps), cparse_start_line);
		   Delete(yyvsp[0].str);
		   Delete(cpps);
		 }
		 
	       ;
    break;}
case 62:
#line 2194 "parser.y"
{
                 String *cpps;
		 int start_line = cparse_line;
		 skip_balanced('{','}');
		 if (Namespaceprefix) {
		   Swig_error(cparse_file, cparse_start_line, "%%inline directive inside a namespace is disallowed.\n");
		   
		   yyval.node = 0;
		 } else {
		   String *code;
                   yyval.node = new_node("insert");
		   Delitem(scanner_ccode,0);
		   Delitem(scanner_ccode,DOH_END);
		   code = Copy(scanner_ccode);
		   Setattr(yyval.node,"code", code);
		   Delete(code);		   
		   cpps=Copy(scanner_ccode);
		   start_inline(Char(cpps), start_line);
		   Delete(cpps);
		 }
               ;
    break;}
case 63:
#line 2225 "parser.y"
{
                 yyval.node = new_node("insert");
		 Setattr(yyval.node,"code",yyvsp[0].str);
	       ;
    break;}
case 64:
#line 2229 "parser.y"
{
		 String *code = NewStringEmpty();
		 yyval.node = new_node("insert");
		 Setattr(yyval.node,"section",yyvsp[-2].id);
		 Setattr(yyval.node,"code",code);
		 if (Swig_insert_file(yyvsp[0].id,code) < 0) {
		   Swig_error(cparse_file, cparse_line, "Couldn't find '%s'.\n", yyvsp[0].id);
		   yyval.node = 0;
		 } 
               ;
    break;}
case 65:
#line 2239 "parser.y"
{
		 yyval.node = new_node("insert");
		 Setattr(yyval.node,"section",yyvsp[-2].id);
		 Setattr(yyval.node,"code",yyvsp[0].str);
               ;
    break;}
case 66:
#line 2244 "parser.y"
{
		 String *code;
                 skip_balanced('{','}');
		 yyval.node = new_node("insert");
		 Setattr(yyval.node,"section",yyvsp[-2].id);
		 Delitem(scanner_ccode,0);
		 Delitem(scanner_ccode,DOH_END);
		 code = Copy(scanner_ccode);
		 Setattr(yyval.node,"code", code);
		 Delete(code);
	       ;
    break;}
case 67:
#line 2262 "parser.y"
{
                 yyval.node = new_node("module");
		 if (yyvsp[-1].node) {
		   Setattr(yyval.node,"options",yyvsp[-1].node);
		   if (Getattr(yyvsp[-1].node,"directors")) {
		     Wrapper_director_mode_set(1);
		   } 
		   if (Getattr(yyvsp[-1].node,"dirprot")) {
		     Wrapper_director_protected_mode_set(1);
		   } 
		   if (Getattr(yyvsp[-1].node,"allprotected")) {
		     Wrapper_all_protected_mode_set(1);
		   } 
		   if (Getattr(yyvsp[-1].node,"templatereduce")) {
		     template_reduce = 1;
		   }
		   if (Getattr(yyvsp[-1].node,"notemplatereduce")) {
		     template_reduce = 0;
		   }
		 }
		 if (!ModuleName) ModuleName = NewString(yyvsp[0].id);
		 if (!import_mode) {
		   /* first module included, we apply global
		      ModuleName, which can be modify by -module */
		   String *mname = Copy(ModuleName);
		   Setattr(yyval.node,"name",mname);
		   Delete(mname);
		 } else { 
		   /* import mode, we just pass the idstring */
		   Setattr(yyval.node,"name",yyvsp[0].id);   
		 }		 
		 if (!module_node) module_node = yyval.node;
	       ;
    break;}
case 68:
#line 2302 "parser.y"
{
                 Swig_warning(WARN_DEPRECATED_NAME,cparse_file,cparse_line, "%%name is deprecated.  Use %%rename instead.\n");
		 Delete(yyrename);
                 yyrename = NewString(yyvsp[-1].id);
		 yyval.node = 0;
               ;
    break;}
case 69:
#line 2308 "parser.y"
{
		 Swig_warning(WARN_DEPRECATED_NAME,cparse_file,cparse_line, "%%name is deprecated.  Use %%rename instead.\n");
		 yyval.node = 0;
		 Swig_error(cparse_file,cparse_line,"Missing argument to %%name directive.\n");
	       ;
    break;}
case 70:
#line 2321 "parser.y"
{
                 yyval.node = new_node("native");
		 Setattr(yyval.node,"name",yyvsp[-4].id);
		 Setattr(yyval.node,"wrap:name",yyvsp[-1].id);
	         add_symbols(yyval.node);
	       ;
    break;}
case 71:
#line 2327 "parser.y"
{
		 if (!SwigType_isfunction(yyvsp[-1].decl.type)) {
		   Swig_error(cparse_file,cparse_line,"%%native declaration '%s' is not a function.\n", yyvsp[-1].decl.id);
		   yyval.node = 0;
		 } else {
		     Delete(SwigType_pop_function(yyvsp[-1].decl.type));
		     /* Need check for function here */
		     SwigType_push(yyvsp[-2].type,yyvsp[-1].decl.type);
		     yyval.node = new_node("native");
	             Setattr(yyval.node,"name",yyvsp[-5].id);
		     Setattr(yyval.node,"wrap:name",yyvsp[-1].decl.id);
		     Setattr(yyval.node,"type",yyvsp[-2].type);
		     Setattr(yyval.node,"parms",yyvsp[-1].decl.parms);
		     Setattr(yyval.node,"decl",yyvsp[-1].decl.type);
		 }
	         add_symbols(yyval.node);
	       ;
    break;}
case 72:
#line 2353 "parser.y"
{
                 yyval.node = new_node("pragma");
		 Setattr(yyval.node,"lang",yyvsp[-3].id);
		 Setattr(yyval.node,"name",yyvsp[-2].id);
		 Setattr(yyval.node,"value",yyvsp[0].str);
	       ;
    break;}
case 73:
#line 2359 "parser.y"
{
		yyval.node = new_node("pragma");
		Setattr(yyval.node,"lang",yyvsp[-1].id);
		Setattr(yyval.node,"name",yyvsp[0].id);
	      ;
    break;}
case 74:
#line 2366 "parser.y"
{ yyval.str = NewString(yyvsp[0].id); ;
    break;}
case 75:
#line 2367 "parser.y"
{ yyval.str = yyvsp[0].str; ;
    break;}
case 76:
#line 2370 "parser.y"
{ yyval.id = yyvsp[-1].id; ;
    break;}
case 77:
#line 2371 "parser.y"
{ yyval.id = (char *) "swig"; ;
    break;}
case 78:
#line 2379 "parser.y"
{
                SwigType *t = yyvsp[-2].decl.type;
		Hash *kws = NewHash();
		String *fixname;
		fixname = feature_identifier_fix(yyvsp[-2].decl.id);
		Setattr(kws,"name",yyvsp[-1].id);
		if (!Len(t)) t = 0;
		/* Special declarator check */
		if (t) {
		  if (SwigType_isfunction(t)) {
		    SwigType *decl = SwigType_pop_function(t);
		    if (SwigType_ispointer(t)) {
		      String *nname = NewStringf("*%s",fixname);
		      if (yyvsp[-3].intvalue) {
			Swig_name_rename_add(Namespaceprefix, nname,decl,kws,yyvsp[-2].decl.parms);
		      } else {
			Swig_name_namewarn_add(Namespaceprefix,nname,decl,kws);
		      }
		      Delete(nname);
		    } else {
		      if (yyvsp[-3].intvalue) {
			Swig_name_rename_add(Namespaceprefix,(fixname),decl,kws,yyvsp[-2].decl.parms);
		      } else {
			Swig_name_namewarn_add(Namespaceprefix,(fixname),decl,kws);
		      }
		    }
		    Delete(decl);
		  } else if (SwigType_ispointer(t)) {
		    String *nname = NewStringf("*%s",fixname);
		    if (yyvsp[-3].intvalue) {
		      Swig_name_rename_add(Namespaceprefix,(nname),0,kws,yyvsp[-2].decl.parms);
		    } else {
		      Swig_name_namewarn_add(Namespaceprefix,(nname),0,kws);
		    }
		    Delete(nname);
		  }
		} else {
		  if (yyvsp[-3].intvalue) {
		    Swig_name_rename_add(Namespaceprefix,(fixname),0,kws,yyvsp[-2].decl.parms);
		  } else {
		    Swig_name_namewarn_add(Namespaceprefix,(fixname),0,kws);
		  }
		}
                yyval.node = 0;
		scanner_clear_rename();
              ;
    break;}
case 79:
#line 2425 "parser.y"
{
		String *fixname;
		Hash *kws = yyvsp[-4].node;
		SwigType *t = yyvsp[-2].decl.type;
		fixname = feature_identifier_fix(yyvsp[-2].decl.id);
		if (!Len(t)) t = 0;
		/* Special declarator check */
		if (t) {
		  if (yyvsp[-1].dtype.qualifier) SwigType_push(t,yyvsp[-1].dtype.qualifier);
		  if (SwigType_isfunction(t)) {
		    SwigType *decl = SwigType_pop_function(t);
		    if (SwigType_ispointer(t)) {
		      String *nname = NewStringf("*%s",fixname);
		      if (yyvsp[-6].intvalue) {
			Swig_name_rename_add(Namespaceprefix, nname,decl,kws,yyvsp[-2].decl.parms);
		      } else {
			Swig_name_namewarn_add(Namespaceprefix,nname,decl,kws);
		      }
		      Delete(nname);
		    } else {
		      if (yyvsp[-6].intvalue) {
			Swig_name_rename_add(Namespaceprefix,(fixname),decl,kws,yyvsp[-2].decl.parms);
		      } else {
			Swig_name_namewarn_add(Namespaceprefix,(fixname),decl,kws);
		      }
		    }
		    Delete(decl);
		  } else if (SwigType_ispointer(t)) {
		    String *nname = NewStringf("*%s",fixname);
		    if (yyvsp[-6].intvalue) {
		      Swig_name_rename_add(Namespaceprefix,(nname),0,kws,yyvsp[-2].decl.parms);
		    } else {
		      Swig_name_namewarn_add(Namespaceprefix,(nname),0,kws);
		    }
		    Delete(nname);
		  }
		} else {
		  if (yyvsp[-6].intvalue) {
		    Swig_name_rename_add(Namespaceprefix,(fixname),0,kws,yyvsp[-2].decl.parms);
		  } else {
		    Swig_name_namewarn_add(Namespaceprefix,(fixname),0,kws);
		  }
		}
                yyval.node = 0;
		scanner_clear_rename();
              ;
    break;}
case 80:
#line 2471 "parser.y"
{
		if (yyvsp[-5].intvalue) {
		  Swig_name_rename_add(Namespaceprefix,yyvsp[-1].id,0,yyvsp[-3].node,0);
		} else {
		  Swig_name_namewarn_add(Namespaceprefix,yyvsp[-1].id,0,yyvsp[-3].node);
		}
		yyval.node = 0;
		scanner_clear_rename();
              ;
    break;}
case 81:
#line 2482 "parser.y"
{
		    yyval.intvalue = 1;
                ;
    break;}
case 82:
#line 2485 "parser.y"
{
                    yyval.intvalue = 0;
                ;
    break;}
case 83:
#line 2512 "parser.y"
{
                    String *val = yyvsp[0].str ? NewString(yyvsp[0].str) : NewString("1");
                    new_feature(yyvsp[-4].id, val, 0, yyvsp[-2].decl.id, yyvsp[-2].decl.type, yyvsp[-2].decl.parms, yyvsp[-1].dtype.qualifier);
                    yyval.node = 0;
                    scanner_clear_rename();
                  ;
    break;}
case 84:
#line 2518 "parser.y"
{
                    String *val = Len(yyvsp[-4].id) ? NewString(yyvsp[-4].id) : 0;
                    new_feature(yyvsp[-6].id, val, 0, yyvsp[-2].decl.id, yyvsp[-2].decl.type, yyvsp[-2].decl.parms, yyvsp[-1].dtype.qualifier);
                    yyval.node = 0;
                    scanner_clear_rename();
                  ;
    break;}
case 85:
#line 2524 "parser.y"
{
                    String *val = yyvsp[0].str ? NewString(yyvsp[0].str) : NewString("1");
                    new_feature(yyvsp[-5].id, val, yyvsp[-4].node, yyvsp[-2].decl.id, yyvsp[-2].decl.type, yyvsp[-2].decl.parms, yyvsp[-1].dtype.qualifier);
                    yyval.node = 0;
                    scanner_clear_rename();
                  ;
    break;}
case 86:
#line 2530 "parser.y"
{
                    String *val = Len(yyvsp[-5].id) ? NewString(yyvsp[-5].id) : 0;
                    new_feature(yyvsp[-7].id, val, yyvsp[-4].node, yyvsp[-2].decl.id, yyvsp[-2].decl.type, yyvsp[-2].decl.parms, yyvsp[-1].dtype.qualifier);
                    yyval.node = 0;
                    scanner_clear_rename();
                  ;
    break;}
case 87:
#line 2538 "parser.y"
{
                    String *val = yyvsp[0].str ? NewString(yyvsp[0].str) : NewString("1");
                    new_feature(yyvsp[-2].id, val, 0, 0, 0, 0, 0);
                    yyval.node = 0;
                    scanner_clear_rename();
                  ;
    break;}
case 88:
#line 2544 "parser.y"
{
                    String *val = Len(yyvsp[-2].id) ? NewString(yyvsp[-2].id) : 0;
                    new_feature(yyvsp[-4].id, val, 0, 0, 0, 0, 0);
                    yyval.node = 0;
                    scanner_clear_rename();
                  ;
    break;}
case 89:
#line 2550 "parser.y"
{
                    String *val = yyvsp[0].str ? NewString(yyvsp[0].str) : NewString("1");
                    new_feature(yyvsp[-3].id, val, yyvsp[-2].node, 0, 0, 0, 0);
                    yyval.node = 0;
                    scanner_clear_rename();
                  ;
    break;}
case 90:
#line 2556 "parser.y"
{
                    String *val = Len(yyvsp[-3].id) ? NewString(yyvsp[-3].id) : 0;
                    new_feature(yyvsp[-5].id, val, yyvsp[-2].node, 0, 0, 0, 0);
                    yyval.node = 0;
                    scanner_clear_rename();
                  ;
    break;}
case 91:
#line 2564 "parser.y"
{ yyval.str = yyvsp[0].str; ;
    break;}
case 92:
#line 2565 "parser.y"
{ yyval.str = 0; ;
    break;}
case 93:
#line 2566 "parser.y"
{ yyval.str = yyvsp[-2].pl; ;
    break;}
case 94:
#line 2569 "parser.y"
{
		  yyval.node = NewHash();
		  Setattr(yyval.node,"name",yyvsp[-2].id);
		  Setattr(yyval.node,"value",yyvsp[0].id);
                ;
    break;}
case 95:
#line 2574 "parser.y"
{
		  yyval.node = NewHash();
		  Setattr(yyval.node,"name",yyvsp[-3].id);
		  Setattr(yyval.node,"value",yyvsp[-1].id);
                  set_nextSibling(yyval.node,yyvsp[0].node);
                ;
    break;}
case 96:
#line 2584 "parser.y"
{
                 Parm *val;
		 String *name;
		 SwigType *t;
		 if (Namespaceprefix) name = NewStringf("%s::%s", Namespaceprefix, yyvsp[-2].decl.id);
		 else name = NewString(yyvsp[-2].decl.id);
		 val = yyvsp[-4].pl;
		 if (yyvsp[-2].decl.parms) {
		   Setmeta(val,"parms",yyvsp[-2].decl.parms);
		 }
		 t = yyvsp[-2].decl.type;
		 if (!Len(t)) t = 0;
		 if (t) {
		   if (yyvsp[-1].dtype.qualifier) SwigType_push(t,yyvsp[-1].dtype.qualifier);
		   if (SwigType_isfunction(t)) {
		     SwigType *decl = SwigType_pop_function(t);
		     if (SwigType_ispointer(t)) {
		       String *nname = NewStringf("*%s",name);
		       Swig_feature_set(Swig_cparse_features(), nname, decl, "feature:varargs", val, 0);
		       Delete(nname);
		     } else {
		       Swig_feature_set(Swig_cparse_features(), name, decl, "feature:varargs", val, 0);
		     }
		     Delete(decl);
		   } else if (SwigType_ispointer(t)) {
		     String *nname = NewStringf("*%s",name);
		     Swig_feature_set(Swig_cparse_features(),nname,0,"feature:varargs",val, 0);
		     Delete(nname);
		   }
		 } else {
		   Swig_feature_set(Swig_cparse_features(),name,0,"feature:varargs",val, 0);
		 }
		 Delete(name);
		 yyval.node = 0;
              ;
    break;}
case 97:
#line 2620 "parser.y"
{ yyval.pl = yyvsp[0].pl; ;
    break;}
case 98:
#line 2621 "parser.y"
{ 
		  int i;
		  int n;
		  Parm *p;
		  n = atoi(Char(yyvsp[-2].dtype.val));
		  if (n <= 0) {
		    Swig_error(cparse_file, cparse_line,"Argument count in %%varargs must be positive.\n");
		    yyval.pl = 0;
		  } else {
		    String *name = Getattr(yyvsp[0].p, "name");
		    yyval.pl = Copy(yyvsp[0].p);
		    if (name)
		      Setattr(yyval.pl, "name", NewStringf("%s%d", name, n));
		    for (i = 1; i < n; i++) {
		      p = Copy(yyvsp[0].p);
		      name = Getattr(p, "name");
		      if (name)
		        Setattr(p, "name", NewStringf("%s%d", name, n-i));
		      set_nextSibling(p,yyval.pl);
		      Delete(yyval.pl);
		      yyval.pl = p;
		    }
		  }
                ;
    break;}
case 99:
#line 2656 "parser.y"
{
		   yyval.node = 0;
		   if (yyvsp[-3].tmap.method) {
		     String *code = 0;
		     yyval.node = new_node("typemap");
		     Setattr(yyval.node,"method",yyvsp[-3].tmap.method);
		     if (yyvsp[-3].tmap.kwargs) {
		       ParmList *kw = yyvsp[-3].tmap.kwargs;
                       code = remove_block(kw, yyvsp[0].str);
		       Setattr(yyval.node,"kwargs", yyvsp[-3].tmap.kwargs);
		     }
		     code = code ? code : NewString(yyvsp[0].str);
		     Setattr(yyval.node,"code", code);
		     Delete(code);
		     appendChild(yyval.node,yyvsp[-1].p);
		   }
	       ;
    break;}
case 100:
#line 2673 "parser.y"
{
		 yyval.node = 0;
		 if (yyvsp[-3].tmap.method) {
		   yyval.node = new_node("typemap");
		   Setattr(yyval.node,"method",yyvsp[-3].tmap.method);
		   appendChild(yyval.node,yyvsp[-1].p);
		 }
	       ;
    break;}
case 101:
#line 2681 "parser.y"
{
		   yyval.node = 0;
		   if (yyvsp[-5].tmap.method) {
		     yyval.node = new_node("typemapcopy");
		     Setattr(yyval.node,"method",yyvsp[-5].tmap.method);
		     Setattr(yyval.node,"pattern", Getattr(yyvsp[-1].p,"pattern"));
		     appendChild(yyval.node,yyvsp[-3].p);
		   }
	       ;
    break;}
case 102:
#line 2694 "parser.y"
{
		 Hash *p;
		 String *name;
		 p = nextSibling(yyvsp[0].node);
		 if (p && (!Getattr(p,"value"))) {
 		   /* this is the deprecated two argument typemap form */
 		   Swig_warning(WARN_DEPRECATED_TYPEMAP_LANG,cparse_file, cparse_line,
				"Specifying the language name in %%typemap is deprecated - use #ifdef SWIG<LANG> instead.\n");
		   /* two argument typemap form */
		   name = Getattr(yyvsp[0].node,"name");
		   if (!name || (Strcmp(name,typemap_lang))) {
		     yyval.tmap.method = 0;
		     yyval.tmap.kwargs = 0;
		   } else {
		     yyval.tmap.method = Getattr(p,"name");
		     yyval.tmap.kwargs = nextSibling(p);
		   }
		 } else {
		   /* one-argument typemap-form */
		   yyval.tmap.method = Getattr(yyvsp[0].node,"name");
		   yyval.tmap.kwargs = p;
		 }
                ;
    break;}
case 103:
#line 2719 "parser.y"
{
                 yyval.p = yyvsp[-1].p;
		 set_nextSibling(yyval.p,yyvsp[0].p);
		;
    break;}
case 104:
#line 2725 "parser.y"
{
                 yyval.p = yyvsp[-1].p;
		 set_nextSibling(yyval.p,yyvsp[0].p);
                ;
    break;}
case 105:
#line 2729 "parser.y"
{ yyval.p = 0;;
    break;}
case 106:
#line 2732 "parser.y"
{
                  Parm *parm;
		  SwigType_push(yyvsp[-1].type,yyvsp[0].decl.type);
		  yyval.p = new_node("typemapitem");
		  parm = NewParmWithoutFileLineInfo(yyvsp[-1].type,yyvsp[0].decl.id);
		  Setattr(yyval.p,"pattern",parm);
		  Setattr(yyval.p,"parms", yyvsp[0].decl.parms);
		  Delete(parm);
		  /*		  $$ = NewParmWithoutFileLineInfo($1,$2.id);
				  Setattr($$,"parms",$2.parms); */
                ;
    break;}
case 107:
#line 2743 "parser.y"
{
                  yyval.p = new_node("typemapitem");
		  Setattr(yyval.p,"pattern",yyvsp[-1].pl);
		  /*		  Setattr($$,"multitype",$2); */
               ;
    break;}
case 108:
#line 2748 "parser.y"
{
		 yyval.p = new_node("typemapitem");
		 Setattr(yyval.p,"pattern", yyvsp[-4].pl);
		 /*                 Setattr($$,"multitype",$2); */
		 Setattr(yyval.p,"parms",yyvsp[-1].pl);
               ;
    break;}
case 109:
#line 2761 "parser.y"
{
                   yyval.node = new_node("types");
		   Setattr(yyval.node,"parms",yyvsp[-2].pl);
                   if (yyvsp[0].str)
		     Setattr(yyval.node,"convcode",NewString(yyvsp[0].str));
               ;
    break;}
case 110:
#line 2773 "parser.y"
{
                  Parm *p, *tp;
		  Node *n;
		  Symtab *tscope = 0;
		  int     specialized = 0;

		  yyval.node = 0;

		  tscope = Swig_symbol_current();          /* Get the current scope */

		  /* If the class name is qualified, we need to create or lookup namespace entries */
		  if (!inclass) {
		    yyvsp[-4].str = resolve_node_scope(yyvsp[-4].str);
		  }

		  /*
		    We use the new namespace entry 'nscope' only to
		    emit the template node. The template parameters are
		    resolved in the current 'tscope'.

		    This is closer to the C++ (typedef) behavior.
		  */
		  n = Swig_cparse_template_locate(yyvsp[-4].str,yyvsp[-2].p,tscope);

		  /* Patch the argument types to respect namespaces */
		  p = yyvsp[-2].p;
		  while (p) {
		    SwigType *value = Getattr(p,"value");
		    if (!value) {
		      SwigType *ty = Getattr(p,"type");
		      if (ty) {
			SwigType *rty = 0;
			int reduce = template_reduce;
			if (reduce || !SwigType_ispointer(ty)) {
			  rty = Swig_symbol_typedef_reduce(ty,tscope);
			  if (!reduce) reduce = SwigType_ispointer(rty);
			}
			ty = reduce ? Swig_symbol_type_qualify(rty,tscope) : Swig_symbol_type_qualify(ty,tscope);
			Setattr(p,"type",ty);
			Delete(ty);
			Delete(rty);
		      }
		    } else {
		      value = Swig_symbol_type_qualify(value,tscope);
		      Setattr(p,"value",value);
		      Delete(value);
		    }

		    p = nextSibling(p);
		  }

		  /* Look for the template */
		  {
                    Node *nn = n;
                    Node *linklistend = 0;
                    while (nn) {
                      Node *templnode = 0;
                      if (Strcmp(nodeType(nn),"template") == 0) {
                        int nnisclass = (Strcmp(Getattr(nn,"templatetype"),"class") == 0); /* if not a templated class it is a templated function */
                        Parm *tparms = Getattr(nn,"templateparms");
                        if (!tparms) {
                          specialized = 1;
                        }
                        if (nnisclass && !specialized && ((ParmList_len(yyvsp[-2].p) > ParmList_len(tparms)))) {
                          Swig_error(cparse_file, cparse_line, "Too many template parameters. Maximum of %d.\n", ParmList_len(tparms));
                        } else if (nnisclass && !specialized && ((ParmList_len(yyvsp[-2].p) < ParmList_numrequired(tparms)))) {
                          Swig_error(cparse_file, cparse_line, "Not enough template parameters specified. %d required.\n", ParmList_numrequired(tparms));
                        } else if (!nnisclass && ((ParmList_len(yyvsp[-2].p) != ParmList_len(tparms)))) {
                          /* must be an overloaded templated method - ignore it as it is overloaded with a different number of template parameters */
                          nn = Getattr(nn,"sym:nextSibling"); /* repeat for overloaded templated functions */
                          continue;
                        } else {
			  String *tname = Copy(yyvsp[-4].str);
                          int def_supplied = 0;
                          /* Expand the template */
			  Node *templ = Swig_symbol_clookup(yyvsp[-4].str,0);
			  Parm *targs = templ ? Getattr(templ,"templateparms") : 0;

                          ParmList *temparms;
                          if (specialized) temparms = CopyParmList(yyvsp[-2].p);
                          else temparms = CopyParmList(tparms);

                          /* Create typedef's and arguments */
                          p = yyvsp[-2].p;
                          tp = temparms;
                          if (!p && ParmList_len(p) != ParmList_len(temparms)) {
                            /* we have no template parameters supplied in %template for a template that has default args*/
                            p = tp;
                            def_supplied = 1;
                          }

                          while (p) {
                            String *value = Getattr(p,"value");
                            if (def_supplied) {
                              Setattr(p,"default","1");
                            }
                            if (value) {
                              Setattr(tp,"value",value);
                            } else {
                              SwigType *ty = Getattr(p,"type");
                              if (ty) {
                                Setattr(tp,"type",ty);
                              }
                              Delattr(tp,"value");
                            }
			    /* fix default arg values */
			    if (targs) {
			      Parm *pi = temparms;
			      Parm *ti = targs;
			      String *tv = Getattr(tp,"value");
			      if (!tv) tv = Getattr(tp,"type");
			      while(pi != tp && ti && pi) {
				String *name = Getattr(ti,"name");
				String *value = Getattr(pi,"value");
				if (!value) value = Getattr(pi,"type");
				Replaceid(tv, name, value);
				pi = nextSibling(pi);
				ti = nextSibling(ti);
			      }
			    }
                            p = nextSibling(p);
                            tp = nextSibling(tp);
                            if (!p && tp) {
                              p = tp;
                              def_supplied = 1;
                            }
                          }

                          templnode = copy_node(nn);
                          /* We need to set the node name based on name used to instantiate */
                          Setattr(templnode,"name",tname);
			  Delete(tname);
                          if (!specialized) {
                            Delattr(templnode,"sym:typename");
                          } else {
                            Setattr(templnode,"sym:typename","1");
                          }
                          if (yyvsp[-6].id && !inclass) {
			    /*
			       Comment this out for 1.3.28. We need to
			       re-enable it later but first we need to
			       move %ignore from using %rename to use
			       %feature(ignore).

			       String *symname = Swig_name_make(templnode,0,$3,0,0);
			    */
			    String *symname = yyvsp[-6].id;
                            Swig_cparse_template_expand(templnode,symname,temparms,tscope);
                            Setattr(templnode,"sym:name",symname);
                          } else {
                            static int cnt = 0;
                            String *nname = NewStringf("__dummy_%d__", cnt++);
                            Swig_cparse_template_expand(templnode,nname,temparms,tscope);
                            Setattr(templnode,"sym:name",nname);
			    Delete(nname);
                            Setattr(templnode,"feature:onlychildren", "typemap,typemapitem,typemapcopy,typedef,types,fragment");

			    if (yyvsp[-6].id) {
			      Swig_warning(WARN_PARSE_NESTED_TEMPLATE, cparse_file, cparse_line, "Named nested template instantiations not supported. Processing as if no name was given to %%template().\n");
			    }
                          }
                          Delattr(templnode,"templatetype");
                          Setattr(templnode,"template",nn);
                          Setfile(templnode,cparse_file);
                          Setline(templnode,cparse_line);
                          Delete(temparms);

                          add_symbols_copy(templnode);

                          if (Strcmp(nodeType(templnode),"class") == 0) {

                            /* Identify pure abstract methods */
                            Setattr(templnode,"abstract", pure_abstract(firstChild(templnode)));

                            /* Set up inheritance in symbol table */
                            {
                              Symtab  *csyms;
                              List *baselist = Getattr(templnode,"baselist");
                              csyms = Swig_symbol_current();
                              Swig_symbol_setscope(Getattr(templnode,"symtab"));
                              if (baselist) {
                                List *bases = make_inherit_list(Getattr(templnode,"name"),baselist);
                                if (bases) {
                                  Iterator s;
                                  for (s = First(bases); s.item; s = Next(s)) {
                                    Symtab *st = Getattr(s.item,"symtab");
                                    if (st) {
				      Setfile(st,Getfile(s.item));
				      Setline(st,Getline(s.item));
                                      Swig_symbol_inherit(st);
                                    }
                                  }
				  Delete(bases);
                                }
                              }
                              Swig_symbol_setscope(csyms);
                            }

                            /* Merge in %extend methods for this class */

			    /* !!! This may be broken.  We may have to add the
			       %extend methods at the beginning of the class */

                            if (extendhash) {
                              String *stmp = 0;
                              String *clsname;
                              Node *am;
                              if (Namespaceprefix) {
                                clsname = stmp = NewStringf("%s::%s", Namespaceprefix, Getattr(templnode,"name"));
                              } else {
                                clsname = Getattr(templnode,"name");
                              }
                              am = Getattr(extendhash,clsname);
                              if (am) {
                                Symtab *st = Swig_symbol_current();
                                Swig_symbol_setscope(Getattr(templnode,"symtab"));
                                /*			    Printf(stdout,"%s: %s %x %x\n", Getattr(templnode,"name"), clsname, Swig_symbol_current(), Getattr(templnode,"symtab")); */
                                merge_extensions(templnode,am);
                                Swig_symbol_setscope(st);
				append_previous_extension(templnode,am);
                                Delattr(extendhash,clsname);
                              }
			      if (stmp) Delete(stmp);
                            }
                            /* Add to classes hash */
                            if (!classes) classes = NewHash();

                            {
                              if (Namespaceprefix) {
                                String *temp = NewStringf("%s::%s", Namespaceprefix, Getattr(templnode,"name"));
                                Setattr(classes,temp,templnode);
				Delete(temp);
                              } else {
				String *qs = Swig_symbol_qualifiedscopename(templnode);
                                Setattr(classes, qs,templnode);
				Delete(qs);
                              }
                            }
                          }
                        }

                        /* all the overloaded templated functions are added into a linked list */
                        if (nscope_inner) {
                          /* non-global namespace */
                          if (templnode) {
                            appendChild(nscope_inner,templnode);
			    Delete(templnode);
                            if (nscope) yyval.node = nscope;
                          }
                        } else {
                          /* global namespace */
                          if (!linklistend) {
                            yyval.node = templnode;
                          } else {
                            set_nextSibling(linklistend,templnode);
			    Delete(templnode);
                          }
                          linklistend = templnode;
                        }
                      }
                      nn = Getattr(nn,"sym:nextSibling"); /* repeat for overloaded templated functions. If a templated class there will never be a sibling. */
                    }
		  }
	          Swig_symbol_setscope(tscope);
		  Delete(Namespaceprefix);
		  Namespaceprefix = Swig_symbol_qualifiedscopename(0);
                ;
    break;}
case 111:
#line 3047 "parser.y"
{
		  Swig_warning(0,cparse_file, cparse_line,"%s\n", yyvsp[0].id);
		  yyval.node = 0;
               ;
    break;}
case 112:
#line 3057 "parser.y"
{
                    yyval.node = yyvsp[0].node; 
                    if (yyval.node) {
   		      add_symbols(yyval.node);
                      default_arguments(yyval.node);
   	            }
                ;
    break;}
case 113:
#line 3064 "parser.y"
{ yyval.node = yyvsp[0].node; ;
    break;}
case 114:
#line 3065 "parser.y"
{ yyval.node = yyvsp[0].node; ;
    break;}
case 115:
#line 3069 "parser.y"
{
		  if (Strcmp(yyvsp[-1].id,"C") == 0) {
		    cparse_externc = 1;
		  }
		;
    break;}
case 116:
#line 3073 "parser.y"
{
		  cparse_externc = 0;
		  if (Strcmp(yyvsp[-4].id,"C") == 0) {
		    Node *n = firstChild(yyvsp[-1].node);
		    yyval.node = new_node("extern");
		    Setattr(yyval.node,"name",yyvsp[-4].id);
		    appendChild(yyval.node,n);
		    while (n) {
		      SwigType *decl = Getattr(n,"decl");
		      if (SwigType_isfunction(decl) && Strcmp(Getattr(n, "storage"), "typedef") != 0) {
			Setattr(n,"storage","externc");
		      }
		      n = nextSibling(n);
		    }
		  } else {
		     Swig_warning(WARN_PARSE_UNDEFINED_EXTERN,cparse_file, cparse_line,"Unrecognized extern type \"%s\".\n", yyvsp[-4].id);
		    yyval.node = new_node("extern");
		    Setattr(yyval.node,"name",yyvsp[-4].id);
		    appendChild(yyval.node,firstChild(yyvsp[-1].node));
		  }
                ;
    break;}
case 117:
#line 3100 "parser.y"
{
              yyval.node = new_node("cdecl");
	      if (yyvsp[-1].dtype.qualifier) SwigType_push(yyvsp[-2].decl.type,yyvsp[-1].dtype.qualifier);
	      Setattr(yyval.node,"type",yyvsp[-3].type);
	      Setattr(yyval.node,"storage",yyvsp[-4].id);
	      Setattr(yyval.node,"name",yyvsp[-2].decl.id);
	      Setattr(yyval.node,"decl",yyvsp[-2].decl.type);
	      Setattr(yyval.node,"parms",yyvsp[-2].decl.parms);
	      Setattr(yyval.node,"value",yyvsp[-1].dtype.val);
	      Setattr(yyval.node,"throws",yyvsp[-1].dtype.throws);
	      Setattr(yyval.node,"throw",yyvsp[-1].dtype.throwf);
	      if (!yyvsp[0].node) {
		if (Len(scanner_ccode)) {
		  String *code = Copy(scanner_ccode);
		  Setattr(yyval.node,"code",code);
		  Delete(code);
		}
	      } else {
		Node *n = yyvsp[0].node;
		/* Inherit attributes */
		while (n) {
		  String *type = Copy(yyvsp[-3].type);
		  Setattr(n,"type",type);
		  Setattr(n,"storage",yyvsp[-4].id);
		  n = nextSibling(n);
		  Delete(type);
		}
	      }
	      if (yyvsp[-1].dtype.bitfield) {
		Setattr(yyval.node,"bitfield", yyvsp[-1].dtype.bitfield);
	      }

	      /* Look for "::" declarations (ignored) */
	      if (Strstr(yyvsp[-2].decl.id,"::")) {
                /* This is a special case. If the scope name of the declaration exactly
                   matches that of the declaration, then we will allow it. Otherwise, delete. */
                String *p = Swig_scopename_prefix(yyvsp[-2].decl.id);
		if (p) {
		  if ((Namespaceprefix && Strcmp(p,Namespaceprefix) == 0) ||
		      (inclass && Strcmp(p,Classprefix) == 0)) {
		    String *lstr = Swig_scopename_last(yyvsp[-2].decl.id);
		    Setattr(yyval.node,"name",lstr);
		    Delete(lstr);
		    set_nextSibling(yyval.node,yyvsp[0].node);
		  } else {
		    Delete(yyval.node);
		    yyval.node = yyvsp[0].node;
		  }
		  Delete(p);
		} else {
		  Delete(yyval.node);
		  yyval.node = yyvsp[0].node;
		}
	      } else {
		set_nextSibling(yyval.node,yyvsp[0].node);
	      }
           ;
    break;}
case 118:
#line 3161 "parser.y"
{ 
                   yyval.node = 0;
                   Clear(scanner_ccode); 
               ;
    break;}
case 119:
#line 3165 "parser.y"
{
		 yyval.node = new_node("cdecl");
		 if (yyvsp[-1].dtype.qualifier) SwigType_push(yyvsp[-2].decl.type,yyvsp[-1].dtype.qualifier);
		 Setattr(yyval.node,"name",yyvsp[-2].decl.id);
		 Setattr(yyval.node,"decl",yyvsp[-2].decl.type);
		 Setattr(yyval.node,"parms",yyvsp[-2].decl.parms);
		 Setattr(yyval.node,"value",yyvsp[-1].dtype.val);
		 Setattr(yyval.node,"throws",yyvsp[-1].dtype.throws);
		 Setattr(yyval.node,"throw",yyvsp[-1].dtype.throwf);
		 if (yyvsp[-1].dtype.bitfield) {
		   Setattr(yyval.node,"bitfield", yyvsp[-1].dtype.bitfield);
		 }
		 if (!yyvsp[0].node) {
		   if (Len(scanner_ccode)) {
		     String *code = Copy(scanner_ccode);
		     Setattr(yyval.node,"code",code);
		     Delete(code);
		   }
		 } else {
		   set_nextSibling(yyval.node,yyvsp[0].node);
		 }
	       ;
    break;}
case 120:
#line 3187 "parser.y"
{ 
                   skip_balanced('{','}');
                   yyval.node = 0;
               ;
    break;}
case 121:
#line 3193 "parser.y"
{ 
                   yyval.dtype = yyvsp[0].dtype; 
                   yyval.dtype.qualifier = 0;
		   yyval.dtype.throws = 0;
		   yyval.dtype.throwf = 0;
              ;
    break;}
case 122:
#line 3199 "parser.y"
{ 
                   yyval.dtype = yyvsp[0].dtype; 
		   yyval.dtype.qualifier = yyvsp[-1].str;
		   yyval.dtype.throws = 0;
		   yyval.dtype.throwf = 0;
	      ;
    break;}
case 123:
#line 3205 "parser.y"
{ 
		   yyval.dtype = yyvsp[0].dtype; 
                   yyval.dtype.qualifier = 0;
		   yyval.dtype.throws = yyvsp[-2].pl;
		   yyval.dtype.throwf = NewString("1");
              ;
    break;}
case 124:
#line 3211 "parser.y"
{ 
                   yyval.dtype = yyvsp[0].dtype; 
                   yyval.dtype.qualifier = yyvsp[-5].str;
		   yyval.dtype.throws = yyvsp[-2].pl;
		   yyval.dtype.throwf = NewString("1");
              ;
    break;}
case 125:
#line 3224 "parser.y"
{
		   SwigType *ty = 0;
		   yyval.node = new_node("enumforward");
		   ty = NewStringf("enum %s", yyvsp[-1].id);
		   Setattr(yyval.node,"name",yyvsp[-1].id);
		   Setattr(yyval.node,"type",ty);
		   Setattr(yyval.node,"sym:weak", "1");
		   add_symbols(yyval.node);
	      ;
    break;}
case 126:
#line 3239 "parser.y"
{
		  SwigType *ty = 0;
                  yyval.node = new_node("enum");
		  ty = NewStringf("enum %s", yyvsp[-4].id);
		  Setattr(yyval.node,"name",yyvsp[-4].id);
		  Setattr(yyval.node,"type",ty);
		  appendChild(yyval.node,yyvsp[-2].node);
		  add_symbols(yyval.node);       /* Add to tag space */
		  add_symbols(yyvsp[-2].node);       /* Add enum values to id space */
               ;
    break;}
case 127:
#line 3249 "parser.y"
{
		 Node *n;
		 SwigType *ty = 0;
		 String   *unnamed = 0;
		 int       unnamedinstance = 0;

		 yyval.node = new_node("enum");
		 if (yyvsp[-6].id) {
		   Setattr(yyval.node,"name",yyvsp[-6].id);
		   ty = NewStringf("enum %s", yyvsp[-6].id);
		 } else if (yyvsp[-2].decl.id) {
		   unnamed = make_unnamed();
		   ty = NewStringf("enum %s", unnamed);
		   Setattr(yyval.node,"unnamed",unnamed);
                   /* name is not set for unnamed enum instances, e.g. enum { foo } Instance; */
		   if (yyvsp[-8].id && Cmp(yyvsp[-8].id,"typedef") == 0) {
		     Setattr(yyval.node,"name",yyvsp[-2].decl.id);
                   } else {
                     unnamedinstance = 1;
                   }
		   Setattr(yyval.node,"storage",yyvsp[-8].id);
		 }
		 if (yyvsp[-2].decl.id && Cmp(yyvsp[-8].id,"typedef") == 0) {
		   Setattr(yyval.node,"tdname",yyvsp[-2].decl.id);
                   Setattr(yyval.node,"allows_typedef","1");
                 }
		 appendChild(yyval.node,yyvsp[-4].node);
		 n = new_node("cdecl");
		 Setattr(n,"type",ty);
		 Setattr(n,"name",yyvsp[-2].decl.id);
		 Setattr(n,"storage",yyvsp[-8].id);
		 Setattr(n,"decl",yyvsp[-2].decl.type);
		 Setattr(n,"parms",yyvsp[-2].decl.parms);
		 Setattr(n,"unnamed",unnamed);

                 if (unnamedinstance) {
		   SwigType *cty = NewString("enum ");
		   Setattr(yyval.node,"type",cty);
		   SetFlag(yyval.node,"unnamedinstance");
		   SetFlag(n,"unnamedinstance");
		   Delete(cty);
                 }
		 if (yyvsp[0].node) {
		   Node *p = yyvsp[0].node;
		   set_nextSibling(n,p);
		   while (p) {
		     SwigType *cty = Copy(ty);
		     Setattr(p,"type",cty);
		     Setattr(p,"unnamed",unnamed);
		     Setattr(p,"storage",yyvsp[-8].id);
		     Delete(cty);
		     p = nextSibling(p);
		   }
		 } else {
		   if (Len(scanner_ccode)) {
		     String *code = Copy(scanner_ccode);
		     Setattr(n,"code",code);
		     Delete(code);
		   }
		 }

                 /* Ensure that typedef enum ABC {foo} XYZ; uses XYZ for sym:name, like structs.
                  * Note that class_rename/yyrename are bit of a mess so used this simple approach to change the name. */
                 if (yyvsp[-2].decl.id && yyvsp[-6].id && Cmp(yyvsp[-8].id,"typedef") == 0) {
		   String *name = NewString(yyvsp[-2].decl.id);
                   Setattr(yyval.node, "parser:makename", name);
		   Delete(name);
                 }

		 add_symbols(yyval.node);       /* Add enum to tag space */
		 set_nextSibling(yyval.node,n);
		 Delete(n);
		 add_symbols(yyvsp[-4].node);       /* Add enum values to id space */
	         add_symbols(n);
		 Delete(unnamed);
	       ;
    break;}
case 128:
#line 3327 "parser.y"
{
                   /* This is a sick hack.  If the ctor_end has parameters,
                      and the parms parameter only has 1 parameter, this
                      could be a declaration of the form:

                         type (id)(parms)

			 Otherwise it's an error. */
                    int err = 0;
                    yyval.node = 0;

		    if ((ParmList_len(yyvsp[-2].pl) == 1) && (!Swig_scopename_check(yyvsp[-4].type))) {
		      SwigType *ty = Getattr(yyvsp[-2].pl,"type");
		      String *name = Getattr(yyvsp[-2].pl,"name");
		      err = 1;
		      if (!name) {
			yyval.node = new_node("cdecl");
			Setattr(yyval.node,"type",yyvsp[-4].type);
			Setattr(yyval.node,"storage",yyvsp[-5].id);
			Setattr(yyval.node,"name",ty);

			if (yyvsp[0].decl.have_parms) {
			  SwigType *decl = NewStringEmpty();
			  SwigType_add_function(decl,yyvsp[0].decl.parms);
			  Setattr(yyval.node,"decl",decl);
			  Setattr(yyval.node,"parms",yyvsp[0].decl.parms);
			  if (Len(scanner_ccode)) {
			    String *code = Copy(scanner_ccode);
			    Setattr(yyval.node,"code",code);
			    Delete(code);
			  }
			}
			if (yyvsp[0].decl.defarg) {
			  Setattr(yyval.node,"value",yyvsp[0].decl.defarg);
			}
			Setattr(yyval.node,"throws",yyvsp[0].decl.throws);
			Setattr(yyval.node,"throw",yyvsp[0].decl.throwf);
			err = 0;
		      }
		    }
		    if (err) {
		      Swig_error(cparse_file,cparse_line,"Syntax error in input(2).\n");
		      exit(1);
		    }
                ;
    break;}
case 129:
#line 3378 "parser.y"
{  yyval.node = yyvsp[0].node; ;
    break;}
case 130:
#line 3379 "parser.y"
{ yyval.node = yyvsp[0].node; ;
    break;}
case 131:
#line 3380 "parser.y"
{ yyval.node = yyvsp[0].node; ;
    break;}
case 132:
#line 3381 "parser.y"
{ yyval.node = yyvsp[0].node; ;
    break;}
case 133:
#line 3382 "parser.y"
{ yyval.node = yyvsp[0].node; ;
    break;}
case 134:
#line 3383 "parser.y"
{ yyval.node = 0; ;
    break;}
case 135:
#line 3388 "parser.y"
{
                 if (nested_template == 0) {
                   String *prefix;
                   List *bases = 0;
		   Node *scope = 0;
		   yyval.node = new_node("class");
		   Setline(yyval.node,cparse_start_line);
		   Setattr(yyval.node,"kind",yyvsp[-3].id);
		   if (yyvsp[-1].bases) {
		     Setattr(yyval.node,"baselist", Getattr(yyvsp[-1].bases,"public"));
		     Setattr(yyval.node,"protectedbaselist", Getattr(yyvsp[-1].bases,"protected"));
		     Setattr(yyval.node,"privatebaselist", Getattr(yyvsp[-1].bases,"private"));
		   }
		   Setattr(yyval.node,"allows_typedef","1");

		   /* preserve the current scope */
		   prev_symtab = Swig_symbol_current();
		  
		   /* If the class name is qualified.  We need to create or lookup namespace/scope entries */
		   scope = resolve_node_scope(yyvsp[-2].str);
		   Setfile(scope,cparse_file);
		   Setline(scope,cparse_line);
		   yyvsp[-2].str = scope;
		   
		   /* support for old nested classes "pseudo" support, such as:

		         %rename(Ala__Ola) Ala::Ola;
			class Ala::Ola {
			public:
			    Ola() {}
		         };

		      this should disappear when a proper implementation is added.
		   */
		   if (nscope_inner && Strcmp(nodeType(nscope_inner),"namespace") != 0) {
		     if (Namespaceprefix) {
		       String *name = NewStringf("%s::%s", Namespaceprefix, yyvsp[-2].str);		       
		       yyvsp[-2].str = name;
		       Namespaceprefix = 0;
		       nscope_inner = 0;
		     }
		   }
		   Setattr(yyval.node,"name",yyvsp[-2].str);

		   Delete(class_rename);
                   class_rename = make_name(yyval.node,yyvsp[-2].str,0);
		   Classprefix = NewString(yyvsp[-2].str);
		   /* Deal with inheritance  */
		   if (yyvsp[-1].bases) {
		     bases = make_inherit_list(yyvsp[-2].str,Getattr(yyvsp[-1].bases,"public"));
		   }
		   prefix = SwigType_istemplate_templateprefix(yyvsp[-2].str);
		   if (prefix) {
		     String *fbase, *tbase;
		     if (Namespaceprefix) {
		       fbase = NewStringf("%s::%s", Namespaceprefix,yyvsp[-2].str);
		       tbase = NewStringf("%s::%s", Namespaceprefix, prefix);
		     } else {
		       fbase = Copy(yyvsp[-2].str);
		       tbase = Copy(prefix);
		     }
		     Swig_name_inherit(tbase,fbase);
		     Delete(fbase);
		     Delete(tbase);
		   }
                   if (strcmp(yyvsp[-3].id,"class") == 0) {
		     cplus_mode = CPLUS_PRIVATE;
		   } else {
		     cplus_mode = CPLUS_PUBLIC;
		   }
		   Swig_symbol_newscope();
		   Swig_symbol_setscopename(yyvsp[-2].str);
		   if (bases) {
		     Iterator s;
		     for (s = First(bases); s.item; s = Next(s)) {
		       Symtab *st = Getattr(s.item,"symtab");
		       if (st) {
			 Setfile(st,Getfile(s.item));
			 Setline(st,Getline(s.item));
			 Swig_symbol_inherit(st); 
		       }
		     }
		     Delete(bases);
		   }
		   Delete(Namespaceprefix);
		   Namespaceprefix = Swig_symbol_qualifiedscopename(0);
		   cparse_start_line = cparse_line;

		   /* If there are active template parameters, we need to make sure they are
                      placed in the class symbol table so we can catch shadows */

		   if (template_parameters) {
		     Parm *tp = template_parameters;
		     while(tp) {
		       String *tpname = Copy(Getattr(tp,"name"));
		       Node *tn = new_node("templateparm");
		       Setattr(tn,"name",tpname);
		       Swig_symbol_cadd(tpname,tn);
		       tp = nextSibling(tp);
		       Delete(tpname);
		     }
		   }
		   if (class_level >= max_class_levels) {
		       if (!max_class_levels) {
			   max_class_levels = 16;
		       } else {
			   max_class_levels *= 2;
		       }
		       class_decl = (Node**) realloc(class_decl, sizeof(Node*) * max_class_levels);
		       if (!class_decl) {
			   Swig_error(cparse_file, cparse_line, "realloc() failed\n");
		       }
		   }
		   class_decl[class_level++] = yyval.node;
		   Delete(prefix);
		   inclass = 1;
		 }
               ;
    break;}
case 136:
#line 3505 "parser.y"
{
	         (void) yyvsp[-3].node;
		 if (nested_template == 0) {
		   Node *p;
		   SwigType *ty;
		   Symtab *cscope = prev_symtab;
		   Node *am = 0;
		   String *scpname = 0;
		   yyval.node = class_decl[--class_level];
		   inclass = 0;
		   
		   /* Check for pure-abstract class */
		   Setattr(yyval.node,"abstract", pure_abstract(yyvsp[-2].node));
		   
		   /* This bit of code merges in a previously defined %extend directive (if any) */
		   
		   if (extendhash) {
		     String *clsname = Swig_symbol_qualifiedscopename(0);
		     am = Getattr(extendhash,clsname);
		     if (am) {
		       merge_extensions(yyval.node,am);
		       Delattr(extendhash,clsname);
		     }
		     Delete(clsname);
		   }
		   if (!classes) classes = NewHash();
		   scpname = Swig_symbol_qualifiedscopename(0);
		   Setattr(classes,scpname,yyval.node);
		   Delete(scpname);

		   appendChild(yyval.node,yyvsp[-2].node);
		   
		   if (am) append_previous_extension(yyval.node,am);

		   p = yyvsp[0].node;
		   if (p) {
		     set_nextSibling(yyval.node,p);
		   }
		   
		   if (cparse_cplusplus && !cparse_externc) {
		     ty = NewString(yyvsp[-6].str);
		   } else {
		     ty = NewStringf("%s %s", yyvsp[-7].id,yyvsp[-6].str);
		   }
		   while (p) {
		     Setattr(p,"storage",yyvsp[-8].id);
		     Setattr(p,"type",ty);
		     p = nextSibling(p);
		   }
		   /* Dump nested classes */
		   {
		     String *name = yyvsp[-6].str;
		     if (yyvsp[0].node) {
		       SwigType *decltype = Getattr(yyvsp[0].node,"decl");
		       if (Cmp(yyvsp[-8].id,"typedef") == 0) {
			 if (!decltype || !Len(decltype)) {
			   String *cname;
			   name = Getattr(yyvsp[0].node,"name");
			   cname = Copy(name);
			   Setattr(yyval.node,"tdname",cname);
			   Delete(cname);

			   /* Use typedef name as class name */
			   if (class_rename && (Strcmp(class_rename,yyvsp[-6].str) == 0)) {
			     Delete(class_rename);
			     class_rename = NewString(name);
			   }
			   if (!Getattr(classes,name)) {
			     Setattr(classes,name,yyval.node);
			   }
			   Setattr(yyval.node,"decl",decltype);
			 }
		       }
		     }
		     appendChild(yyval.node,dump_nested(Char(name)));
		   }

		   if (cplus_mode != CPLUS_PUBLIC) {
		   /* we 'open' the class at the end, to allow %template
		      to add new members */
		     Node *pa = new_node("access");
		     Setattr(pa,"kind","public");
		     cplus_mode = CPLUS_PUBLIC;
		     appendChild(yyval.node,pa);
		     Delete(pa);
		   }

		   Setattr(yyval.node,"symtab",Swig_symbol_popscope());

		   Classprefix = 0;
		   if (nscope_inner) {
		     /* this is tricky */
		     /* we add the declaration in the original namespace */
		     appendChild(nscope_inner,yyval.node);
		     Swig_symbol_setscope(Getattr(nscope_inner,"symtab"));
		     Delete(Namespaceprefix);
		     Namespaceprefix = Swig_symbol_qualifiedscopename(0);
		     add_symbols(yyval.node);
		     if (nscope) yyval.node = nscope;
		     /* but the variable definition in the current scope */
		     Swig_symbol_setscope(cscope);
		     Delete(Namespaceprefix);
		     Namespaceprefix = Swig_symbol_qualifiedscopename(0);
		     add_symbols(yyvsp[0].node);
		   } else {
		     Delete(yyrename);
		     yyrename = Copy(class_rename);
		     Delete(Namespaceprefix);
		     Namespaceprefix = Swig_symbol_qualifiedscopename(0);

		     add_symbols(yyval.node);
		     add_symbols(yyvsp[0].node);
		   }
		   Swig_symbol_setscope(cscope);
		   Delete(Namespaceprefix);
		   Namespaceprefix = Swig_symbol_qualifiedscopename(0);
		 } else {
		    yyval.node = new_node("class");
		    Setattr(yyval.node,"kind",yyvsp[-7].id);
		    Setattr(yyval.node,"name",NewString(yyvsp[-6].str));
		    SetFlag(yyval.node,"nestedtemplateclass");
		 }
	       ;
    break;}
case 137:
#line 3631 "parser.y"
{
	       String *unnamed;
	       unnamed = make_unnamed();
	       yyval.node = new_node("class");
	       Setline(yyval.node,cparse_start_line);
	       Setattr(yyval.node,"kind",yyvsp[-1].id);
	       Setattr(yyval.node,"storage",yyvsp[-2].id);
	       Setattr(yyval.node,"unnamed",unnamed);
	       Setattr(yyval.node,"allows_typedef","1");
	       Delete(class_rename);
	       class_rename = make_name(yyval.node,0,0);
	       if (strcmp(yyvsp[-1].id,"class") == 0) {
		 cplus_mode = CPLUS_PRIVATE;
	       } else {
		 cplus_mode = CPLUS_PUBLIC;
	       }
	       Swig_symbol_newscope();
	       cparse_start_line = cparse_line;
	       if (class_level >= max_class_levels) {
		   if (!max_class_levels) {
		       max_class_levels = 16;
		   } else {
		       max_class_levels *= 2;
		   }
		   class_decl = (Node**) realloc(class_decl, sizeof(Node*) * max_class_levels);
		   if (!class_decl) {
		       Swig_error(cparse_file, cparse_line, "realloc() failed\n");
		   }
	       }
	       class_decl[class_level++] = yyval.node;
	       inclass = 1;
	       Classprefix = NewStringEmpty();
	       Delete(Namespaceprefix);
	       Namespaceprefix = Swig_symbol_qualifiedscopename(0);
             ;
    break;}
case 138:
#line 3665 "parser.y"
{
	       String *unnamed;
	       Node *n;
	       (void) yyvsp[-5].node;
	       Classprefix = 0;
	       yyval.node = class_decl[--class_level];
	       inclass = 0;
	       unnamed = Getattr(yyval.node,"unnamed");

	       /* Check for pure-abstract class */
	       Setattr(yyval.node,"abstract", pure_abstract(yyvsp[-4].node));

	       n = new_node("cdecl");
	       Setattr(n,"name",yyvsp[-2].decl.id);
	       Setattr(n,"unnamed",unnamed);
	       Setattr(n,"type",unnamed);
	       Setattr(n,"decl",yyvsp[-2].decl.type);
	       Setattr(n,"parms",yyvsp[-2].decl.parms);
	       Setattr(n,"storage",yyvsp[-8].id);
	       if (yyvsp[0].node) {
		 Node *p = yyvsp[0].node;
		 set_nextSibling(n,p);
		 while (p) {
		   String *type = Copy(unnamed);
		   Setattr(p,"name",yyvsp[-2].decl.id);
		   Setattr(p,"unnamed",unnamed);
		   Setattr(p,"type",type);
		   Delete(type);
		   Setattr(p,"storage",yyvsp[-8].id);
		   p = nextSibling(p);
		 }
	       }
	       set_nextSibling(yyval.node,n);
	       Delete(n);
	       {
		 /* If a proper typedef name was given, we'll use it to set the scope name */
		 String *name = 0;
		 if (yyvsp[-8].id && (strcmp(yyvsp[-8].id,"typedef") == 0)) {
		   if (!Len(yyvsp[-2].decl.type)) {	
		     String *scpname = 0;
		     name = yyvsp[-2].decl.id;
		     Setattr(yyval.node,"tdname",name);
		     Setattr(yyval.node,"name",name);
		     Swig_symbol_setscopename(name);

		     /* If a proper name was given, we use that as the typedef, not unnamed */
		     Clear(unnamed);
		     Append(unnamed, name);
		     
		     n = nextSibling(n);
		     set_nextSibling(yyval.node,n);

		     /* Check for previous extensions */
		     if (extendhash) {
		       String *clsname = Swig_symbol_qualifiedscopename(0);
		       Node *am = Getattr(extendhash,clsname);
		       if (am) {
			 /* Merge the extension into the symbol table */
			 merge_extensions(yyval.node,am);
			 append_previous_extension(yyval.node,am);
			 Delattr(extendhash,clsname);
		       }
		       Delete(clsname);
		     }
		     if (!classes) classes = NewHash();
		     scpname = Swig_symbol_qualifiedscopename(0);
		     Setattr(classes,scpname,yyval.node);
		     Delete(scpname);
		   } else {
		     Swig_symbol_setscopename("<unnamed>");
		   }
		 }
		 appendChild(yyval.node,yyvsp[-4].node);
		 appendChild(yyval.node,dump_nested(Char(name)));
	       }
	       /* Pop the scope */
	       Setattr(yyval.node,"symtab",Swig_symbol_popscope());
	       if (class_rename) {
		 Delete(yyrename);
		 yyrename = NewString(class_rename);
	       }
	       Delete(Namespaceprefix);
	       Namespaceprefix = Swig_symbol_qualifiedscopename(0);
	       add_symbols(yyval.node);
	       add_symbols(n);
	       Delete(unnamed);
              ;
    break;}
case 139:
#line 3754 "parser.y"
{ yyval.node = 0; ;
    break;}
case 140:
#line 3755 "parser.y"
{
                        yyval.node = new_node("cdecl");
                        Setattr(yyval.node,"name",yyvsp[-2].decl.id);
                        Setattr(yyval.node,"decl",yyvsp[-2].decl.type);
                        Setattr(yyval.node,"parms",yyvsp[-2].decl.parms);
			set_nextSibling(yyval.node,yyvsp[0].node);
                    ;
    break;}
case 141:
#line 3767 "parser.y"
{
              if (yyvsp[-3].id && (Strcmp(yyvsp[-3].id,"friend") == 0)) {
		/* Ignore */
                yyval.node = 0; 
	      } else {
		yyval.node = new_node("classforward");
		Setfile(yyval.node,cparse_file);
		Setline(yyval.node,cparse_line);
		Setattr(yyval.node,"kind",yyvsp[-2].id);
		Setattr(yyval.node,"name",yyvsp[-1].str);
		Setattr(yyval.node,"sym:weak", "1");
		add_symbols(yyval.node);
	      }
             ;
    break;}
case 142:
#line 3787 "parser.y"
{ 
		    template_parameters = yyvsp[-1].tparms; 
		    if (inclass)
		      nested_template++;

		  ;
    break;}
case 143:
#line 3792 "parser.y"
{

		    /* Don't ignore templated functions declared within a class, unless the templated function is within a nested class */
		    if (nested_template <= 1) {
		      int is_nested_template_class = yyvsp[0].node && GetFlag(yyvsp[0].node, "nestedtemplateclass");
		      if (is_nested_template_class) {
			yyval.node = 0;
			/* Nested template classes would probably better be ignored like ordinary nested classes using cpp_nested, but that introduces shift/reduce conflicts */
			if (cplus_mode == CPLUS_PUBLIC) {
			  /* Treat the nested class/struct/union as a forward declaration until a proper nested class solution is implemented */
			  String *kind = Getattr(yyvsp[0].node, "kind");
			  String *name = Getattr(yyvsp[0].node, "name");
			  yyval.node = new_node("template");
			  Setattr(yyval.node,"kind",kind);
			  Setattr(yyval.node,"name",name);
			  Setattr(yyval.node,"sym:weak", "1");
			  Setattr(yyval.node,"templatetype","classforward");
			  Setattr(yyval.node,"templateparms", yyvsp[-3].tparms);
			  add_symbols(yyval.node);

			  if (GetFlag(yyval.node, "feature:nestedworkaround")) {
			    Swig_symbol_remove(yyval.node);
			    yyval.node = 0;
			  } else {
			    SWIG_WARN_NODE_BEGIN(yyval.node);
			    Swig_warning(WARN_PARSE_NAMED_NESTED_CLASS, cparse_file, cparse_line, "Nested template %s not currently supported (%s ignored).\n", kind, name);
			    SWIG_WARN_NODE_END(yyval.node);
			  }
			}
			Delete(yyvsp[0].node);
		      } else {
			String *tname = 0;
			int     error = 0;

			/* check if we get a namespace node with a class declaration, and retrieve the class */
			Symtab *cscope = Swig_symbol_current();
			Symtab *sti = 0;
			Node *ntop = yyvsp[0].node;
			Node *ni = ntop;
			SwigType *ntype = ni ? nodeType(ni) : 0;
			while (ni && Strcmp(ntype,"namespace") == 0) {
			  sti = Getattr(ni,"symtab");
			  ni = firstChild(ni);
			  ntype = nodeType(ni);
			}
			if (sti) {
			  Swig_symbol_setscope(sti);
			  Delete(Namespaceprefix);
			  Namespaceprefix = Swig_symbol_qualifiedscopename(0);
			  yyvsp[0].node = ni;
			}

			yyval.node = yyvsp[0].node;
			if (yyval.node) tname = Getattr(yyval.node,"name");
			
			/* Check if the class is a template specialization */
			if ((yyval.node) && (Strchr(tname,'<')) && (!is_operator(tname))) {
			  /* If a specialization.  Check if defined. */
			  Node *tempn = 0;
			  {
			    String *tbase = SwigType_templateprefix(tname);
			    tempn = Swig_symbol_clookup_local(tbase,0);
			    if (!tempn || (Strcmp(nodeType(tempn),"template") != 0)) {
			      SWIG_WARN_NODE_BEGIN(tempn);
			      Swig_warning(WARN_PARSE_TEMPLATE_SP_UNDEF, Getfile(yyval.node),Getline(yyval.node),"Specialization of non-template '%s'.\n", tbase);
			      SWIG_WARN_NODE_END(tempn);
			      tempn = 0;
			      error = 1;
			    }
			    Delete(tbase);
			  }
			  Setattr(yyval.node,"specialization","1");
			  Setattr(yyval.node,"templatetype",nodeType(yyval.node));
			  set_nodeType(yyval.node,"template");
			  /* Template partial specialization */
			  if (tempn && (yyvsp[-3].tparms) && (yyvsp[0].node)) {
			    List   *tlist;
			    String *targs = SwigType_templateargs(tname);
			    tlist = SwigType_parmlist(targs);
			    /*			  Printf(stdout,"targs = '%s' %s\n", targs, tlist); */
			    if (!Getattr(yyval.node,"sym:weak")) {
			      Setattr(yyval.node,"sym:typename","1");
			    }
			    
			    if (Len(tlist) != ParmList_len(Getattr(tempn,"templateparms"))) {
			      Swig_error(Getfile(yyval.node),Getline(yyval.node),"Inconsistent argument count in template partial specialization. %d %d\n", Len(tlist), ParmList_len(Getattr(tempn,"templateparms")));
			      
			    } else {

			    /* This code builds the argument list for the partial template
			       specialization.  This is a little hairy, but the idea is as
			       follows:

			       $3 contains a list of arguments supplied for the template.
			       For example template<class T>.

			       tlist is a list of the specialization arguments--which may be
			       different.  For example class<int,T>.

			       tp is a copy of the arguments in the original template definition.
       
			       The patching algorithm walks through the list of supplied
			       arguments ($3), finds the position in the specialization arguments
			       (tlist), and then patches the name in the argument list of the
			       original template.
			    */

			    {
			      String *pn;
			      Parm *p, *p1;
			      int i, nargs;
			      Parm *tp = CopyParmList(Getattr(tempn,"templateparms"));
			      nargs = Len(tlist);
			      p = yyvsp[-3].tparms;
			      while (p) {
				for (i = 0; i < nargs; i++){
				  pn = Getattr(p,"name");
				  if (Strcmp(pn,SwigType_base(Getitem(tlist,i))) == 0) {
				    int j;
				    Parm *p1 = tp;
				    for (j = 0; j < i; j++) {
				      p1 = nextSibling(p1);
				    }
				    Setattr(p1,"name",pn);
				    Setattr(p1,"partialarg","1");
				  }
				}
				p = nextSibling(p);
			      }
			      p1 = tp;
			      i = 0;
			      while (p1) {
				if (!Getattr(p1,"partialarg")) {
				  Delattr(p1,"name");
				  Setattr(p1,"type", Getitem(tlist,i));
				} 
				i++;
				p1 = nextSibling(p1);
			      }
			      Setattr(yyval.node,"templateparms",tp);
			      Delete(tp);
			    }
  #if 0
			    /* Patch the parameter list */
			    if (tempn) {
			      Parm *p,*p1;
			      ParmList *tp = CopyParmList(Getattr(tempn,"templateparms"));
			      p = yyvsp[-3].tparms;
			      p1 = tp;
			      while (p && p1) {
				String *pn = Getattr(p,"name");
				Printf(stdout,"pn = '%s'\n", pn);
				if (pn) Setattr(p1,"name",pn);
				else Delattr(p1,"name");
				pn = Getattr(p,"type");
				if (pn) Setattr(p1,"type",pn);
				p = nextSibling(p);
				p1 = nextSibling(p1);
			      }
			      Setattr(yyval.node,"templateparms",tp);
			      Delete(tp);
			    } else {
			      Setattr(yyval.node,"templateparms",yyvsp[-3].tparms);
			    }
  #endif
			    Delattr(yyval.node,"specialization");
			    Setattr(yyval.node,"partialspecialization","1");
			    /* Create a specialized name for matching */
			    {
			      Parm *p = yyvsp[-3].tparms;
			      String *fname = NewString(Getattr(yyval.node,"name"));
			      String *ffname = 0;
			      ParmList *partialparms = 0;

			      char   tmp[32];
			      int    i, ilen;
			      while (p) {
				String *n = Getattr(p,"name");
				if (!n) {
				  p = nextSibling(p);
				  continue;
				}
				ilen = Len(tlist);
				for (i = 0; i < ilen; i++) {
				  if (Strstr(Getitem(tlist,i),n)) {
				    sprintf(tmp,"$%d",i+1);
				    Replaceid(fname,n,tmp);
				  }
				}
				p = nextSibling(p);
			      }
			      /* Patch argument names with typedef */
			      {
				Iterator tt;
				Parm *parm_current = 0;
				List *tparms = SwigType_parmlist(fname);
				ffname = SwigType_templateprefix(fname);
				Append(ffname,"<(");
				for (tt = First(tparms); tt.item; ) {
				  SwigType *rtt = Swig_symbol_typedef_reduce(tt.item,0);
				  SwigType *ttr = Swig_symbol_type_qualify(rtt,0);

				  Parm *newp = NewParmWithoutFileLineInfo(ttr, 0);
				  if (partialparms)
				    set_nextSibling(parm_current, newp);
				  else
				    partialparms = newp;
				  parm_current = newp;

				  Append(ffname,ttr);
				  tt = Next(tt);
				  if (tt.item) Putc(',',ffname);
				  Delete(rtt);
				  Delete(ttr);
				}
				Delete(tparms);
				Append(ffname,")>");
			      }
			      {
				Node *new_partial = NewHash();
				String *partials = Getattr(tempn,"partials");
				if (!partials) {
				  partials = NewList();
				  Setattr(tempn,"partials",partials);
				  Delete(partials);
				}
				/*			      Printf(stdout,"partial: fname = '%s', '%s'\n", fname, Swig_symbol_typedef_reduce(fname,0)); */
				Setattr(new_partial, "partialparms", partialparms);
				Setattr(new_partial, "templcsymname", ffname);
				Append(partials, new_partial);
			      }
			      Setattr(yyval.node,"partialargs",ffname);
			      Swig_symbol_cadd(ffname,yyval.node);
			    }
			    }
			    Delete(tlist);
			    Delete(targs);
			  } else {
			    /* An explicit template specialization */
			    /* add default args from primary (unspecialized) template */
			    String *ty = Swig_symbol_template_deftype(tname,0);
			    String *fname = Swig_symbol_type_qualify(ty,0);
			    Swig_symbol_cadd(fname,yyval.node);
			    Delete(ty);
			    Delete(fname);
			  }
			}  else if (yyval.node) {
			  Setattr(yyval.node,"templatetype",nodeType(yyvsp[0].node));
			  set_nodeType(yyval.node,"template");
			  Setattr(yyval.node,"templateparms", yyvsp[-3].tparms);
			  if (!Getattr(yyval.node,"sym:weak")) {
			    Setattr(yyval.node,"sym:typename","1");
			  }
			  add_symbols(yyval.node);
			  default_arguments(yyval.node);
			  /* We also place a fully parameterized version in the symbol table */
			  {
			    Parm *p;
			    String *fname = NewStringf("%s<(", Getattr(yyval.node,"name"));
			    p = yyvsp[-3].tparms;
			    while (p) {
			      String *n = Getattr(p,"name");
			      if (!n) n = Getattr(p,"type");
			      Append(fname,n);
			      p = nextSibling(p);
			      if (p) Putc(',',fname);
			    }
			    Append(fname,")>");
			    Swig_symbol_cadd(fname,yyval.node);
			  }
			}
			yyval.node = ntop;
			Swig_symbol_setscope(cscope);
			Delete(Namespaceprefix);
			Namespaceprefix = Swig_symbol_qualifiedscopename(0);
			if (error) yyval.node = 0;
		      }
		    } else {
		      yyval.node = 0;
		    }
		    template_parameters = 0;
		    if (inclass)
		      nested_template--;
                  ;
    break;}
case 144:
#line 4076 "parser.y"
{
		  Swig_warning(WARN_PARSE_EXPLICIT_TEMPLATE, cparse_file, cparse_line, "Explicit template instantiation ignored.\n");
                   yyval.node = 0; 
                ;
    break;}
case 145:
#line 4082 "parser.y"
{
		  yyval.node = yyvsp[0].node;
                ;
    break;}
case 146:
#line 4085 "parser.y"
{
                   yyval.node = yyvsp[0].node;
                ;
    break;}
case 147:
#line 4088 "parser.y"
{
                   yyval.node = yyvsp[0].node;
                ;
    break;}
case 148:
#line 4091 "parser.y"
{
		  yyval.node = 0;
                ;
    break;}
case 149:
#line 4094 "parser.y"
{
                  yyval.node = yyvsp[0].node;
                ;
    break;}
case 150:
#line 4097 "parser.y"
{
                  yyval.node = yyvsp[0].node;
                ;
    break;}
case 151:
#line 4102 "parser.y"
{
		   /* Rip out the parameter names */
		  Parm *p = yyvsp[0].pl;
		  yyval.tparms = yyvsp[0].pl;

		  while (p) {
		    String *name = Getattr(p,"name");
		    if (!name) {
		      /* Hmmm. Maybe it's a 'class T' parameter */
		      char *type = Char(Getattr(p,"type"));
		      /* Template template parameter */
		      if (strncmp(type,"template<class> ",16) == 0) {
			type += 16;
		      }
		      if ((strncmp(type,"class ",6) == 0) || (strncmp(type,"typename ", 9) == 0)) {
			char *t = strchr(type,' ');
			Setattr(p,"name", t+1);
		      } else {
			/*
			 Swig_error(cparse_file, cparse_line, "Missing template parameter name\n");
			 $$.rparms = 0;
			 $$.parms = 0;
			 break; */
		      }
		    }
		    p = nextSibling(p);
		  }
                 ;
    break;}
case 152:
#line 4132 "parser.y"
{
                      set_nextSibling(yyvsp[-1].p,yyvsp[0].pl);
                      yyval.pl = yyvsp[-1].p;
                   ;
    break;}
case 153:
#line 4136 "parser.y"
{ yyval.pl = 0; ;
    break;}
case 154:
#line 4139 "parser.y"
{
		    yyval.p = NewParmWithoutFileLineInfo(NewString(yyvsp[0].id), 0);
                  ;
    break;}
case 155:
#line 4142 "parser.y"
{
                    yyval.p = yyvsp[0].p;
                  ;
    break;}
case 156:
#line 4147 "parser.y"
{
                         set_nextSibling(yyvsp[-1].p,yyvsp[0].pl);
                         yyval.pl = yyvsp[-1].p;
                       ;
    break;}
case 157:
#line 4151 "parser.y"
{ yyval.pl = 0; ;
    break;}
case 158:
#line 4156 "parser.y"
{
                  String *uname = Swig_symbol_type_qualify(yyvsp[-1].str,0);
		  String *name = Swig_scopename_last(yyvsp[-1].str);
                  yyval.node = new_node("using");
		  Setattr(yyval.node,"uname",uname);
		  Setattr(yyval.node,"name", name);
		  Delete(uname);
		  Delete(name);
		  add_symbols(yyval.node);
             ;
    break;}
case 159:
#line 4166 "parser.y"
{
	       Node *n = Swig_symbol_clookup(yyvsp[-1].str,0);
	       if (!n) {
		 Swig_error(cparse_file, cparse_line, "Nothing known about namespace '%s'\n", yyvsp[-1].str);
		 yyval.node = 0;
	       } else {

		 while (Strcmp(nodeType(n),"using") == 0) {
		   n = Getattr(n,"node");
		 }
		 if (n) {
		   if (Strcmp(nodeType(n),"namespace") == 0) {
		     Symtab *current = Swig_symbol_current();
		     Symtab *symtab = Getattr(n,"symtab");
		     yyval.node = new_node("using");
		     Setattr(yyval.node,"node",n);
		     Setattr(yyval.node,"namespace", yyvsp[-1].str);
		     if (current != symtab) {
		       Swig_symbol_inherit(symtab);
		     }
		   } else {
		     Swig_error(cparse_file, cparse_line, "'%s' is not a namespace.\n", yyvsp[-1].str);
		     yyval.node = 0;
		   }
		 } else {
		   yyval.node = 0;
		 }
	       }
             ;
    break;}
case 160:
#line 4197 "parser.y"
{ 
                Hash *h;
                yyvsp[-2].node = Swig_symbol_current();
		h = Swig_symbol_clookup(yyvsp[-1].str,0);
		if (h && (yyvsp[-2].node == Getattr(h,"sym:symtab")) && (Strcmp(nodeType(h),"namespace") == 0)) {
		  if (Getattr(h,"alias")) {
		    h = Getattr(h,"namespace");
		    Swig_warning(WARN_PARSE_NAMESPACE_ALIAS, cparse_file, cparse_line, "Namespace alias '%s' not allowed here. Assuming '%s'\n",
				 yyvsp[-1].str, Getattr(h,"name"));
		    yyvsp[-1].str = Getattr(h,"name");
		  }
		  Swig_symbol_setscope(Getattr(h,"symtab"));
		} else {
		  Swig_symbol_newscope();
		  Swig_symbol_setscopename(yyvsp[-1].str);
		}
		Delete(Namespaceprefix);
		Namespaceprefix = Swig_symbol_qualifiedscopename(0);
             ;
    break;}
case 161:
#line 4215 "parser.y"
{
                Node *n = yyvsp[-1].node;
		set_nodeType(n,"namespace");
		Setattr(n,"name",yyvsp[-4].str);
                Setattr(n,"symtab", Swig_symbol_popscope());
		Swig_symbol_setscope(yyvsp[-5].node);
		yyval.node = n;
		Delete(Namespaceprefix);
		Namespaceprefix = Swig_symbol_qualifiedscopename(0);
		add_symbols(yyval.node);
             ;
    break;}
case 162:
#line 4226 "parser.y"
{
	       Hash *h;
	       yyvsp[-1].node = Swig_symbol_current();
	       h = Swig_symbol_clookup((char *)"    ",0);
	       if (h && (Strcmp(nodeType(h),"namespace") == 0)) {
		 Swig_symbol_setscope(Getattr(h,"symtab"));
	       } else {
		 Swig_symbol_newscope();
		 /* we don't use "__unnamed__", but a long 'empty' name */
		 Swig_symbol_setscopename("    ");
	       }
	       Namespaceprefix = 0;
             ;
    break;}
case 163:
#line 4238 "parser.y"
{
	       yyval.node = yyvsp[-1].node;
	       set_nodeType(yyval.node,"namespace");
	       Setattr(yyval.node,"unnamed","1");
	       Setattr(yyval.node,"symtab", Swig_symbol_popscope());
	       Swig_symbol_setscope(yyvsp[-4].node);
	       Delete(Namespaceprefix);
	       Namespaceprefix = Swig_symbol_qualifiedscopename(0);
	       add_symbols(yyval.node);
             ;
    break;}
case 164:
#line 4248 "parser.y"
{
	       /* Namespace alias */
	       Node *n;
	       yyval.node = new_node("namespace");
	       Setattr(yyval.node,"name",yyvsp[-3].id);
	       Setattr(yyval.node,"alias",yyvsp[-1].str);
	       n = Swig_symbol_clookup(yyvsp[-1].str,0);
	       if (!n) {
		 Swig_error(cparse_file, cparse_line, "Unknown namespace '%s'\n", yyvsp[-1].str);
		 yyval.node = 0;
	       } else {
		 if (Strcmp(nodeType(n),"namespace") != 0) {
		   Swig_error(cparse_file, cparse_line, "'%s' is not a namespace\n",yyvsp[-1].str);
		   yyval.node = 0;
		 } else {
		   while (Getattr(n,"alias")) {
		     n = Getattr(n,"namespace");
		   }
		   Setattr(yyval.node,"namespace",n);
		   add_symbols(yyval.node);
		   /* Set up a scope alias */
		   Swig_symbol_alias(yyvsp[-3].id,Getattr(n,"symtab"));
		 }
	       }
             ;
    break;}
case 165:
#line 4275 "parser.y"
{
                   yyval.node = yyvsp[-1].node;
                   /* Insert cpp_member (including any siblings) to the front of the cpp_members linked list */
		   if (yyval.node) {
		     Node *p = yyval.node;
		     Node *pp =0;
		     while (p) {
		       pp = p;
		       p = nextSibling(p);
		     }
		     set_nextSibling(pp,yyvsp[0].node);
		   } else {
		     yyval.node = yyvsp[0].node;
		   }
             ;
    break;}
case 166:
#line 4290 "parser.y"
{ 
                  if (cplus_mode != CPLUS_PUBLIC) {
		     Swig_error(cparse_file,cparse_line,"%%extend can only be used in a public section\n");
		  }
             ;
    break;}
case 167:
#line 4294 "parser.y"
{
	       yyval.node = new_node("extend");
	       tag_nodes(yyvsp[-2].node,"feature:extend",(char*) "1");
	       appendChild(yyval.node,yyvsp[-2].node);
	       set_nextSibling(yyval.node,yyvsp[0].node);
	     ;
    break;}
case 168:
#line 4300 "parser.y"
{ yyval.node = yyvsp[0].node; ;
    break;}
case 169:
#line 4301 "parser.y"
{ yyval.node = 0;;
    break;}
case 170:
#line 4302 "parser.y"
{
	       int start_line = cparse_line;
	       skip_decl();
	       Swig_error(cparse_file,start_line,"Syntax error in input(3).\n");
	       exit(1);
	       ;
    break;}
case 171:
#line 4307 "parser.y"
{ 
		 yyval.node = yyvsp[0].node;
   	     ;
    break;}
case 172:
#line 4318 "parser.y"
{ yyval.node = yyvsp[0].node; ;
    break;}
case 173:
#line 4319 "parser.y"
{ 
                 yyval.node = yyvsp[0].node; 
		 if (extendmode) {
		   String *symname;
		   symname= make_name(yyval.node,Getattr(yyval.node,"name"), Getattr(yyval.node,"decl"));
		   if (Strcmp(symname,Getattr(yyval.node,"name")) == 0) {
		     /* No renaming operation.  Set name to class name */
		     Delete(yyrename);
		     yyrename = NewString(Getattr(current_class,"sym:name"));
		   } else {
		     Delete(yyrename);
		     yyrename = symname;
		   }
		 }
		 add_symbols(yyval.node);
                 default_arguments(yyval.node);
             ;
    break;}
case 174:
#line 4336 "parser.y"
{ yyval.node = yyvsp[0].node; ;
    break;}
case 175:
#line 4337 "parser.y"
{ yyval.node = yyvsp[0].node; ;
    break;}
case 176:
#line 4338 "parser.y"
{ yyval.node = yyvsp[0].node; ;
    break;}
case 177:
#line 4339 "parser.y"
{ yyval.node = yyvsp[0].node; ;
    break;}
case 178:
#line 4340 "parser.y"
{ yyval.node = yyvsp[0].node; ;
    break;}
case 179:
#line 4341 "parser.y"
{ yyval.node = yyvsp[0].node; ;
    break;}
case 180:
#line 4342 "parser.y"
{ yyval.node = 0; ;
    break;}
case 181:
#line 4343 "parser.y"
{ yyval.node = yyvsp[0].node; ;
    break;}
case 182:
#line 4344 "parser.y"
{ yyval.node = yyvsp[0].node; ;
    break;}
case 183:
#line 4345 "parser.y"
{ yyval.node = 0; ;
    break;}
case 184:
#line 4346 "parser.y"
{ yyval.node = yyvsp[0].node; ;
    break;}
case 185:
#line 4347 "parser.y"
{ yyval.node = yyvsp[0].node; ;
    break;}
case 186:
#line 4348 "parser.y"
{ yyval.node = 0; ;
    break;}
case 187:
#line 4349 "parser.y"
{yyval.node = yyvsp[0].node; ;
    break;}
case 188:
#line 4350 "parser.y"
{yyval.node = yyvsp[0].node; ;
    break;}
case 189:
#line 4351 "parser.y"
{ yyval.node = 0; ;
    break;}
case 190:
#line 4360 "parser.y"
{
              if (Classprefix) {
		 SwigType *decl = NewStringEmpty();
		 yyval.node = new_node("constructor");
		 Setattr(yyval.node,"storage",yyvsp[-5].id);
		 Setattr(yyval.node,"name",yyvsp[-4].type);
		 Setattr(yyval.node,"parms",yyvsp[-2].pl);
		 SwigType_add_function(decl,yyvsp[-2].pl);
		 Setattr(yyval.node,"decl",decl);
		 Setattr(yyval.node,"throws",yyvsp[0].decl.throws);
		 Setattr(yyval.node,"throw",yyvsp[0].decl.throwf);
		 if (Len(scanner_ccode)) {
		   String *code = Copy(scanner_ccode);
		   Setattr(yyval.node,"code",code);
		   Delete(code);
		 }
		 SetFlag(yyval.node,"feature:new");
	      } else {
		yyval.node = 0;
              }
              ;
    break;}
case 191:
#line 4385 "parser.y"
{
               String *name = NewStringf("%s",yyvsp[-4].str);
	       if (*(Char(name)) != '~') Insert(name,0,"~");
               yyval.node = new_node("destructor");
	       Setattr(yyval.node,"name",name);
	       Delete(name);
	       if (Len(scanner_ccode)) {
		 String *code = Copy(scanner_ccode);
		 Setattr(yyval.node,"code",code);
		 Delete(code);
	       }
	       {
		 String *decl = NewStringEmpty();
		 SwigType_add_function(decl,yyvsp[-2].pl);
		 Setattr(yyval.node,"decl",decl);
		 Delete(decl);
	       }
	       Setattr(yyval.node,"throws",yyvsp[0].dtype.throws);
	       Setattr(yyval.node,"throw",yyvsp[0].dtype.throwf);
	       add_symbols(yyval.node);
	      ;
    break;}
case 192:
#line 4409 "parser.y"
{
		String *name;
		char *c = 0;
		yyval.node = new_node("destructor");
	       /* Check for template names.  If the class is a template
		  and the constructor is missing the template part, we
		  add it */
	        if (Classprefix) {
                  c = strchr(Char(Classprefix),'<');
                  if (c && !Strchr(yyvsp[-4].str,'<')) {
                    yyvsp[-4].str = NewStringf("%s%s",yyvsp[-4].str,c);
                  }
		}
		Setattr(yyval.node,"storage","virtual");
	        name = NewStringf("%s",yyvsp[-4].str);
		if (*(Char(name)) != '~') Insert(name,0,"~");
		Setattr(yyval.node,"name",name);
		Delete(name);
		Setattr(yyval.node,"throws",yyvsp[0].dtype.throws);
		Setattr(yyval.node,"throw",yyvsp[0].dtype.throwf);
		if (yyvsp[0].dtype.val) {
		  Setattr(yyval.node,"value","0");
		}
		if (Len(scanner_ccode)) {
		  String *code = Copy(scanner_ccode);
		  Setattr(yyval.node,"code",code);
		  Delete(code);
		}
		{
		  String *decl = NewStringEmpty();
		  SwigType_add_function(decl,yyvsp[-2].pl);
		  Setattr(yyval.node,"decl",decl);
		  Delete(decl);
		}

		add_symbols(yyval.node);
	      ;
    break;}
case 193:
#line 4450 "parser.y"
{
                 yyval.node = new_node("cdecl");
                 Setattr(yyval.node,"type",yyvsp[-5].type);
		 Setattr(yyval.node,"name",yyvsp[-6].str);
		 Setattr(yyval.node,"storage",yyvsp[-7].id);

		 SwigType_add_function(yyvsp[-4].type,yyvsp[-2].pl);
		 if (yyvsp[0].dtype.qualifier) {
		   SwigType_push(yyvsp[-4].type,yyvsp[0].dtype.qualifier);
		 }
		 Setattr(yyval.node,"decl",yyvsp[-4].type);
		 Setattr(yyval.node,"parms",yyvsp[-2].pl);
		 Setattr(yyval.node,"conversion_operator","1");
		 add_symbols(yyval.node);
              ;
    break;}
case 194:
#line 4465 "parser.y"
{
		 SwigType *decl;
                 yyval.node = new_node("cdecl");
                 Setattr(yyval.node,"type",yyvsp[-5].type);
		 Setattr(yyval.node,"name",yyvsp[-6].str);
		 Setattr(yyval.node,"storage",yyvsp[-7].id);
		 decl = NewStringEmpty();
		 SwigType_add_reference(decl);
		 SwigType_add_function(decl,yyvsp[-2].pl);
		 if (yyvsp[0].dtype.qualifier) {
		   SwigType_push(decl,yyvsp[0].dtype.qualifier);
		 }
		 Setattr(yyval.node,"decl",decl);
		 Setattr(yyval.node,"parms",yyvsp[-2].pl);
		 Setattr(yyval.node,"conversion_operator","1");
		 add_symbols(yyval.node);
	       ;
    break;}
case 195:
#line 4483 "parser.y"
{
		 SwigType *decl;
                 yyval.node = new_node("cdecl");
                 Setattr(yyval.node,"type",yyvsp[-6].type);
		 Setattr(yyval.node,"name",yyvsp[-7].str);
		 Setattr(yyval.node,"storage",yyvsp[-8].id);
		 decl = NewStringEmpty();
		 SwigType_add_pointer(decl);
		 SwigType_add_reference(decl);
		 SwigType_add_function(decl,yyvsp[-2].pl);
		 if (yyvsp[0].dtype.qualifier) {
		   SwigType_push(decl,yyvsp[0].dtype.qualifier);
		 }
		 Setattr(yyval.node,"decl",decl);
		 Setattr(yyval.node,"parms",yyvsp[-2].pl);
		 Setattr(yyval.node,"conversion_operator","1");
		 add_symbols(yyval.node);
	       ;
    break;}
case 196:
#line 4502 "parser.y"
{
		String *t = NewStringEmpty();
		yyval.node = new_node("cdecl");
		Setattr(yyval.node,"type",yyvsp[-4].type);
		Setattr(yyval.node,"name",yyvsp[-5].str);
		 Setattr(yyval.node,"storage",yyvsp[-6].id);
		SwigType_add_function(t,yyvsp[-2].pl);
		if (yyvsp[0].dtype.qualifier) {
		  SwigType_push(t,yyvsp[0].dtype.qualifier);
		}
		Setattr(yyval.node,"decl",t);
		Setattr(yyval.node,"parms",yyvsp[-2].pl);
		Setattr(yyval.node,"conversion_operator","1");
		add_symbols(yyval.node);
              ;
    break;}
case 197:
#line 4521 "parser.y"
{
                 skip_balanced('{','}');
                 yyval.node = 0;
               ;
    break;}
case 198:
#line 4528 "parser.y"
{ 
                yyval.node = new_node("access");
		Setattr(yyval.node,"kind","public");
                cplus_mode = CPLUS_PUBLIC;
              ;
    break;}
case 199:
#line 4535 "parser.y"
{ 
                yyval.node = new_node("access");
                Setattr(yyval.node,"kind","private");
		cplus_mode = CPLUS_PRIVATE;
	      ;
    break;}
case 200:
#line 4543 "parser.y"
{ 
		yyval.node = new_node("access");
		Setattr(yyval.node,"kind","protected");
		cplus_mode = CPLUS_PROTECTED;
	      ;
    break;}
case 201:
#line 4564 "parser.y"
{
		cparse_start_line = cparse_line;
		skip_balanced('{','}');
		yyval.str = NewString(scanner_ccode); /* copied as initializers overwrite scanner_ccode */
	      ;
    break;}
case 202:
#line 4568 "parser.y"
{
	        yyval.node = 0;
		if (cplus_mode == CPLUS_PUBLIC) {
		  if (cparse_cplusplus) {
		    yyval.node = nested_forward_declaration(yyvsp[-6].id, yyvsp[-5].id, yyvsp[-4].str, yyvsp[-4].str, yyvsp[0].node);
		  } else if (yyvsp[0].node) {
		    nested_new_struct(yyvsp[-5].id, yyvsp[-1].str, yyvsp[0].node);
		  }
		}
		Delete(yyvsp[-1].str);
	      ;
    break;}
case 203:
#line 4590 "parser.y"
{
		cparse_start_line = cparse_line;
		skip_balanced('{','}');
		yyval.str = NewString(scanner_ccode); /* copied as initializers overwrite scanner_ccode */
	      ;
    break;}
case 204:
#line 4594 "parser.y"
{
	        yyval.node = 0;
		if (cplus_mode == CPLUS_PUBLIC) {
		  if (cparse_cplusplus) {
		    const char *name = yyvsp[0].node ? Getattr(yyvsp[0].node, "name") : 0;
		    yyval.node = nested_forward_declaration(yyvsp[-5].id, yyvsp[-4].id, 0, name, yyvsp[0].node);
		  } else {
		    if (yyvsp[0].node) {
		      nested_new_struct(yyvsp[-4].id, yyvsp[-1].str, yyvsp[0].node);
		    } else {
		      Swig_warning(WARN_PARSE_UNNAMED_NESTED_CLASS, cparse_file, cparse_line, "Nested %s not currently supported (ignored).\n", yyvsp[-4].id);
		    }
		  }
		}
		Delete(yyvsp[-1].str);
	      ;
    break;}
case 205:
#line 4626 "parser.y"
{ yyval.node = yyvsp[0].node; ;
    break;}
case 206:
#line 4629 "parser.y"
{ yyval.node = yyvsp[0].node; ;
    break;}
case 207:
#line 4633 "parser.y"
{ yyval.node = yyvsp[0].node; ;
    break;}
case 208:
#line 4636 "parser.y"
{ yyval.node = yyvsp[0].node; ;
    break;}
case 209:
#line 4637 "parser.y"
{ yyval.node = yyvsp[0].node; ;
    break;}
case 210:
#line 4638 "parser.y"
{ yyval.node = yyvsp[0].node; ;
    break;}
case 211:
#line 4639 "parser.y"
{ yyval.node = yyvsp[0].node; ;
    break;}
case 212:
#line 4640 "parser.y"
{ yyval.node = yyvsp[0].node; ;
    break;}
case 213:
#line 4641 "parser.y"
{ yyval.node = yyvsp[0].node; ;
    break;}
case 214:
#line 4642 "parser.y"
{ yyval.node = yyvsp[0].node; ;
    break;}
case 215:
#line 4643 "parser.y"
{ yyval.node = yyvsp[0].node; ;
    break;}
case 216:
#line 4646 "parser.y"
{
	            Clear(scanner_ccode);
		    yyval.dtype.throws = yyvsp[-1].dtype.throws;
		    yyval.dtype.throwf = yyvsp[-1].dtype.throwf;
               ;
    break;}
case 217:
#line 4651 "parser.y"
{ 
		    skip_balanced('{','}'); 
		    yyval.dtype.throws = yyvsp[-1].dtype.throws;
		    yyval.dtype.throwf = yyvsp[-1].dtype.throwf;
	       ;
    break;}
case 218:
#line 4658 "parser.y"
{ 
                     Clear(scanner_ccode);
                     yyval.dtype.val = 0;
                     yyval.dtype.qualifier = yyvsp[-1].dtype.qualifier;
                     yyval.dtype.bitfield = 0;
                     yyval.dtype.throws = yyvsp[-1].dtype.throws;
                     yyval.dtype.throwf = yyvsp[-1].dtype.throwf;
                ;
    break;}
case 219:
#line 4666 "parser.y"
{ 
                     Clear(scanner_ccode);
                     yyval.dtype.val = yyvsp[-1].dtype.val;
                     yyval.dtype.qualifier = yyvsp[-3].dtype.qualifier;
                     yyval.dtype.bitfield = 0;
                     yyval.dtype.throws = yyvsp[-3].dtype.throws; 
                     yyval.dtype.throwf = yyvsp[-3].dtype.throwf; 
               ;
    break;}
case 220:
#line 4674 "parser.y"
{ 
                     skip_balanced('{','}');
                     yyval.dtype.val = 0;
                     yyval.dtype.qualifier = yyvsp[-1].dtype.qualifier;
                     yyval.dtype.bitfield = 0;
                     yyval.dtype.throws = yyvsp[-1].dtype.throws; 
                     yyval.dtype.throwf = yyvsp[-1].dtype.throwf; 
               ;
    break;}
case 221:
#line 4685 "parser.y"
{ ;
    break;}
case 222:
#line 4691 "parser.y"
{ yyval.id = "extern"; ;
    break;}
case 223:
#line 4692 "parser.y"
{ 
                   if (strcmp(yyvsp[0].id,"C") == 0) {
		     yyval.id = "externc";
		   } else {
		     Swig_warning(WARN_PARSE_UNDEFINED_EXTERN,cparse_file, cparse_line,"Unrecognized extern type \"%s\".\n", yyvsp[0].id);
		     yyval.id = 0;
		   }
               ;
    break;}
case 224:
#line 4700 "parser.y"
{ yyval.id = "static"; ;
    break;}
case 225:
#line 4701 "parser.y"
{ yyval.id = "typedef"; ;
    break;}
case 226:
#line 4702 "parser.y"
{ yyval.id = "virtual"; ;
    break;}
case 227:
#line 4703 "parser.y"
{ yyval.id = "friend"; ;
    break;}
case 228:
#line 4704 "parser.y"
{ yyval.id = "explicit"; ;
    break;}
case 229:
#line 4705 "parser.y"
{ yyval.id = 0; ;
    break;}
case 230:
#line 4712 "parser.y"
{
                 Parm *p;
		 yyval.pl = yyvsp[0].pl;
		 p = yyvsp[0].pl;
                 while (p) {
		   Replace(Getattr(p,"type"),"typename ", "", DOH_REPLACE_ANY);
		   p = nextSibling(p);
                 }
               ;
    break;}
case 231:
#line 4723 "parser.y"
{
                  set_nextSibling(yyvsp[-1].p,yyvsp[0].pl);
                  yyval.pl = yyvsp[-1].p;
		;
    break;}
case 232:
#line 4727 "parser.y"
{ yyval.pl = 0; ;
    break;}
case 233:
#line 4730 "parser.y"
{
                 set_nextSibling(yyvsp[-1].p,yyvsp[0].pl);
		 yyval.pl = yyvsp[-1].p;
                ;
    break;}
case 234:
#line 4734 "parser.y"
{ yyval.pl = 0; ;
    break;}
case 235:
#line 4738 "parser.y"
{
                   SwigType_push(yyvsp[-1].type,yyvsp[0].decl.type);
		   yyval.p = NewParmWithoutFileLineInfo(yyvsp[-1].type,yyvsp[0].decl.id);
		   Setfile(yyval.p,cparse_file);
		   Setline(yyval.p,cparse_line);
		   if (yyvsp[0].decl.defarg) {
		     Setattr(yyval.p,"value",yyvsp[0].decl.defarg);
		   }
		;
    break;}
case 236:
#line 4748 "parser.y"
{
                  yyval.p = NewParmWithoutFileLineInfo(NewStringf("template<class> %s %s", yyvsp[-2].id,yyvsp[-1].str), 0);
		  Setfile(yyval.p,cparse_file);
		  Setline(yyval.p,cparse_line);
                  if (yyvsp[0].dtype.val) {
                    Setattr(yyval.p,"value",yyvsp[0].dtype.val);
                  }
                ;
    break;}
case 237:
#line 4756 "parser.y"
{
		  SwigType *t = NewString("v(...)");
		  yyval.p = NewParmWithoutFileLineInfo(t, 0);
		  Setfile(yyval.p,cparse_file);
		  Setline(yyval.p,cparse_line);
		;
    break;}
case 238:
#line 4764 "parser.y"
{
                 Parm *p;
		 yyval.p = yyvsp[0].p;
		 p = yyvsp[0].p;
                 while (p) {
		   if (Getattr(p,"type")) {
		     Replace(Getattr(p,"type"),"typename ", "", DOH_REPLACE_ANY);
		   }
		   p = nextSibling(p);
                 }
               ;
    break;}
case 239:
#line 4777 "parser.y"
{
                  set_nextSibling(yyvsp[-1].p,yyvsp[0].p);
                  yyval.p = yyvsp[-1].p;
		;
    break;}
case 240:
#line 4781 "parser.y"
{ yyval.p = 0; ;
    break;}
case 241:
#line 4784 "parser.y"
{
                 set_nextSibling(yyvsp[-1].p,yyvsp[0].p);
		 yyval.p = yyvsp[-1].p;
                ;
    break;}
case 242:
#line 4788 "parser.y"
{ yyval.p = 0; ;
    break;}
case 243:
#line 4792 "parser.y"
{
		  yyval.p = yyvsp[0].p;
		  {
		    /* We need to make a possible adjustment for integer parameters. */
		    SwigType *type;
		    Node     *n = 0;

		    while (!n) {
		      type = Getattr(yyvsp[0].p,"type");
		      n = Swig_symbol_clookup(type,0);     /* See if we can find a node that matches the typename */
		      if ((n) && (Strcmp(nodeType(n),"cdecl") == 0)) {
			SwigType *decl = Getattr(n,"decl");
			if (!SwigType_isfunction(decl)) {
			  String *value = Getattr(n,"value");
			  if (value) {
			    String *v = Copy(value);
			    Setattr(yyvsp[0].p,"type",v);
			    Delete(v);
			    n = 0;
			  }
			}
		      } else {
			break;
		      }
		    }
		  }

               ;
    break;}
case 244:
#line 4820 "parser.y"
{
                  yyval.p = NewParmWithoutFileLineInfo(0,0);
                  Setfile(yyval.p,cparse_file);
		  Setline(yyval.p,cparse_line);
		  Setattr(yyval.p,"value",yyvsp[0].dtype.val);
               ;
    break;}
case 245:
#line 4828 "parser.y"
{ 
                  yyval.dtype = yyvsp[0].dtype; 
		  if (yyvsp[0].dtype.type == T_ERROR) {
		    Swig_warning(WARN_PARSE_BAD_DEFAULT,cparse_file, cparse_line, "Can't set default argument (ignored)\n");
		    yyval.dtype.val = 0;
		    yyval.dtype.rawval = 0;
		    yyval.dtype.bitfield = 0;
		    yyval.dtype.throws = 0;
		    yyval.dtype.throwf = 0;
		  }
               ;
    break;}
case 246:
#line 4839 "parser.y"
{ 
		  yyval.dtype = yyvsp[-3].dtype;
		  if (yyvsp[-3].dtype.type == T_ERROR) {
		    Swig_warning(WARN_PARSE_BAD_DEFAULT,cparse_file, cparse_line, "Can't set default argument (ignored)\n");
		    yyval.dtype = yyvsp[-3].dtype;
		    yyval.dtype.val = 0;
		    yyval.dtype.rawval = 0;
		    yyval.dtype.bitfield = 0;
		    yyval.dtype.throws = 0;
		    yyval.dtype.throwf = 0;
		  } else {
		    yyval.dtype.val = NewStringf("%s[%s]",yyvsp[-3].dtype.val,yyvsp[-1].dtype.val); 
		  }		  
               ;
    break;}
case 247:
#line 4853 "parser.y"
{
		 skip_balanced('{','}');
		 yyval.dtype.val = 0;
		 yyval.dtype.rawval = 0;
                 yyval.dtype.type = T_INT;
		 yyval.dtype.bitfield = 0;
		 yyval.dtype.throws = 0;
		 yyval.dtype.throwf = 0;
	       ;
    break;}
case 248:
#line 4862 "parser.y"
{ 
		 yyval.dtype.val = 0;
		 yyval.dtype.rawval = 0;
		 yyval.dtype.type = 0;
		 yyval.dtype.bitfield = yyvsp[0].dtype.val;
		 yyval.dtype.throws = 0;
		 yyval.dtype.throwf = 0;
	       ;
    break;}
case 249:
#line 4870 "parser.y"
{
                 yyval.dtype.val = 0;
                 yyval.dtype.rawval = 0;
                 yyval.dtype.type = T_INT;
		 yyval.dtype.bitfield = 0;
		 yyval.dtype.throws = 0;
		 yyval.dtype.throwf = 0;
               ;
    break;}
case 250:
#line 4880 "parser.y"
{
                 yyval.decl = yyvsp[-1].decl;
		 yyval.decl.defarg = yyvsp[0].dtype.rawval ? yyvsp[0].dtype.rawval : yyvsp[0].dtype.val;
            ;
    break;}
case 251:
#line 4884 "parser.y"
{
              yyval.decl = yyvsp[-1].decl;
	      yyval.decl.defarg = yyvsp[0].dtype.rawval ? yyvsp[0].dtype.rawval : yyvsp[0].dtype.val;
            ;
    break;}
case 252:
#line 4888 "parser.y"
{
   	      yyval.decl.type = 0;
              yyval.decl.id = 0;
	      yyval.decl.defarg = yyvsp[0].dtype.rawval ? yyvsp[0].dtype.rawval : yyvsp[0].dtype.val;
            ;
    break;}
case 253:
#line 4895 "parser.y"
{
                 yyval.decl = yyvsp[0].decl;
		 if (SwigType_isfunction(yyvsp[0].decl.type)) {
		   Delete(SwigType_pop_function(yyvsp[0].decl.type));
		 } else if (SwigType_isarray(yyvsp[0].decl.type)) {
		   SwigType *ta = SwigType_pop_arrays(yyvsp[0].decl.type);
		   if (SwigType_isfunction(yyvsp[0].decl.type)) {
		     Delete(SwigType_pop_function(yyvsp[0].decl.type));
		   } else {
		     yyval.decl.parms = 0;
		   }
		   SwigType_push(yyvsp[0].decl.type,ta);
		   Delete(ta);
		 } else {
		   yyval.decl.parms = 0;
		 }
            ;
    break;}
case 254:
#line 4912 "parser.y"
{
              yyval.decl = yyvsp[0].decl;
	      if (SwigType_isfunction(yyvsp[0].decl.type)) {
		Delete(SwigType_pop_function(yyvsp[0].decl.type));
	      } else if (SwigType_isarray(yyvsp[0].decl.type)) {
		SwigType *ta = SwigType_pop_arrays(yyvsp[0].decl.type);
		if (SwigType_isfunction(yyvsp[0].decl.type)) {
		  Delete(SwigType_pop_function(yyvsp[0].decl.type));
		} else {
		  yyval.decl.parms = 0;
		}
		SwigType_push(yyvsp[0].decl.type,ta);
		Delete(ta);
	      } else {
		yyval.decl.parms = 0;
	      }
            ;
    break;}
case 255:
#line 4929 "parser.y"
{
   	      yyval.decl.type = 0;
              yyval.decl.id = 0;
	      yyval.decl.parms = 0;
	      ;
    break;}
case 256:
#line 4937 "parser.y"
{
              yyval.decl = yyvsp[0].decl;
	      if (yyval.decl.type) {
		SwigType_push(yyvsp[-1].type,yyval.decl.type);
		Delete(yyval.decl.type);
	      }
	      yyval.decl.type = yyvsp[-1].type;
           ;
    break;}
case 257:
#line 4945 "parser.y"
{
              yyval.decl = yyvsp[0].decl;
	      SwigType_add_reference(yyvsp[-2].type);
              if (yyval.decl.type) {
		SwigType_push(yyvsp[-2].type,yyval.decl.type);
		Delete(yyval.decl.type);
	      }
	      yyval.decl.type = yyvsp[-2].type;
           ;
    break;}
case 258:
#line 4954 "parser.y"
{
              yyval.decl = yyvsp[0].decl;
	      if (!yyval.decl.type) yyval.decl.type = NewStringEmpty();
           ;
    break;}
case 259:
#line 4958 "parser.y"
{ 
	     yyval.decl = yyvsp[0].decl;
	     yyval.decl.type = NewStringEmpty();
	     SwigType_add_reference(yyval.decl.type);
	     if (yyvsp[0].decl.type) {
	       SwigType_push(yyval.decl.type,yyvsp[0].decl.type);
	       Delete(yyvsp[0].decl.type);
	     }
           ;
    break;}
case 260:
#line 4967 "parser.y"
{ 
	     SwigType *t = NewStringEmpty();

	     yyval.decl = yyvsp[0].decl;
	     SwigType_add_memberpointer(t,yyvsp[-2].str);
	     if (yyval.decl.type) {
	       SwigType_push(t,yyval.decl.type);
	       Delete(yyval.decl.type);
	     }
	     yyval.decl.type = t;
	     ;
    break;}
case 261:
#line 4978 "parser.y"
{ 
	     SwigType *t = NewStringEmpty();
	     yyval.decl = yyvsp[0].decl;
	     SwigType_add_memberpointer(t,yyvsp[-2].str);
	     SwigType_push(yyvsp[-3].type,t);
	     if (yyval.decl.type) {
	       SwigType_push(yyvsp[-3].type,yyval.decl.type);
	       Delete(yyval.decl.type);
	     }
	     yyval.decl.type = yyvsp[-3].type;
	     Delete(t);
	   ;
    break;}
case 262:
#line 4990 "parser.y"
{ 
	     yyval.decl = yyvsp[0].decl;
	     SwigType_add_memberpointer(yyvsp[-4].type,yyvsp[-3].str);
	     SwigType_add_reference(yyvsp[-4].type);
	     if (yyval.decl.type) {
	       SwigType_push(yyvsp[-4].type,yyval.decl.type);
	       Delete(yyval.decl.type);
	     }
	     yyval.decl.type = yyvsp[-4].type;
	   ;
    break;}
case 263:
#line 5000 "parser.y"
{ 
	     SwigType *t = NewStringEmpty();
	     yyval.decl = yyvsp[0].decl;
	     SwigType_add_memberpointer(t,yyvsp[-3].str);
	     SwigType_add_reference(t);
	     if (yyval.decl.type) {
	       SwigType_push(t,yyval.decl.type);
	       Delete(yyval.decl.type);
	     } 
	     yyval.decl.type = t;
	   ;
    break;}
case 264:
#line 5013 "parser.y"
{
                /* Note: This is non-standard C.  Template declarator is allowed to follow an identifier */
                 yyval.decl.id = Char(yyvsp[0].str);
		 yyval.decl.type = 0;
		 yyval.decl.parms = 0;
		 yyval.decl.have_parms = 0;
                  ;
    break;}
case 265:
#line 5020 "parser.y"
{
                  yyval.decl.id = Char(NewStringf("~%s",yyvsp[0].str));
                  yyval.decl.type = 0;
                  yyval.decl.parms = 0;
                  yyval.decl.have_parms = 0;
                  ;
    break;}
case 266:
#line 5028 "parser.y"
{
                  yyval.decl.id = Char(yyvsp[-1].str);
                  yyval.decl.type = 0;
                  yyval.decl.parms = 0;
                  yyval.decl.have_parms = 0;
                  ;
    break;}
case 267:
#line 5044 "parser.y"
{
		    yyval.decl = yyvsp[-1].decl;
		    if (yyval.decl.type) {
		      SwigType_push(yyvsp[-2].type,yyval.decl.type);
		      Delete(yyval.decl.type);
		    }
		    yyval.decl.type = yyvsp[-2].type;
                  ;
    break;}
case 268:
#line 5052 "parser.y"
{
		    SwigType *t;
		    yyval.decl = yyvsp[-1].decl;
		    t = NewStringEmpty();
		    SwigType_add_memberpointer(t,yyvsp[-3].str);
		    if (yyval.decl.type) {
		      SwigType_push(t,yyval.decl.type);
		      Delete(yyval.decl.type);
		    }
		    yyval.decl.type = t;
		    ;
    break;}
case 269:
#line 5063 "parser.y"
{ 
		    SwigType *t;
		    yyval.decl = yyvsp[-2].decl;
		    t = NewStringEmpty();
		    SwigType_add_array(t,(char*)"");
		    if (yyval.decl.type) {
		      SwigType_push(t,yyval.decl.type);
		      Delete(yyval.decl.type);
		    }
		    yyval.decl.type = t;
                  ;
    break;}
case 270:
#line 5074 "parser.y"
{ 
		    SwigType *t;
		    yyval.decl = yyvsp[-3].decl;
		    t = NewStringEmpty();
		    SwigType_add_array(t,yyvsp[-1].dtype.val);
		    if (yyval.decl.type) {
		      SwigType_push(t,yyval.decl.type);
		      Delete(yyval.decl.type);
		    }
		    yyval.decl.type = t;
                  ;
    break;}
case 271:
#line 5085 "parser.y"
{
		    SwigType *t;
                    yyval.decl = yyvsp[-3].decl;
		    t = NewStringEmpty();
		    SwigType_add_function(t,yyvsp[-1].pl);
		    if (!yyval.decl.have_parms) {
		      yyval.decl.parms = yyvsp[-1].pl;
		      yyval.decl.have_parms = 1;
		    }
		    if (!yyval.decl.type) {
		      yyval.decl.type = t;
		    } else {
		      SwigType_push(t, yyval.decl.type);
		      Delete(yyval.decl.type);
		      yyval.decl.type = t;
		    }
		  ;
    break;}
case 272:
#line 5104 "parser.y"
{
                /* Note: This is non-standard C.  Template declarator is allowed to follow an identifier */
                 yyval.decl.id = Char(yyvsp[0].str);
		 yyval.decl.type = 0;
		 yyval.decl.parms = 0;
		 yyval.decl.have_parms = 0;
                  ;
    break;}
case 273:
#line 5112 "parser.y"
{
                  yyval.decl.id = Char(NewStringf("~%s",yyvsp[0].str));
                  yyval.decl.type = 0;
                  yyval.decl.parms = 0;
                  yyval.decl.have_parms = 0;
                  ;
    break;}
case 274:
#line 5129 "parser.y"
{
		    yyval.decl = yyvsp[-1].decl;
		    if (yyval.decl.type) {
		      SwigType_push(yyvsp[-2].type,yyval.decl.type);
		      Delete(yyval.decl.type);
		    }
		    yyval.decl.type = yyvsp[-2].type;
                  ;
    break;}
case 275:
#line 5137 "parser.y"
{
                    yyval.decl = yyvsp[-1].decl;
		    if (!yyval.decl.type) {
		      yyval.decl.type = NewStringEmpty();
		    }
		    SwigType_add_reference(yyval.decl.type);
                  ;
    break;}
case 276:
#line 5144 "parser.y"
{
		    SwigType *t;
		    yyval.decl = yyvsp[-1].decl;
		    t = NewStringEmpty();
		    SwigType_add_memberpointer(t,yyvsp[-3].str);
		    if (yyval.decl.type) {
		      SwigType_push(t,yyval.decl.type);
		      Delete(yyval.decl.type);
		    }
		    yyval.decl.type = t;
		    ;
    break;}
case 277:
#line 5155 "parser.y"
{ 
		    SwigType *t;
		    yyval.decl = yyvsp[-2].decl;
		    t = NewStringEmpty();
		    SwigType_add_array(t,(char*)"");
		    if (yyval.decl.type) {
		      SwigType_push(t,yyval.decl.type);
		      Delete(yyval.decl.type);
		    }
		    yyval.decl.type = t;
                  ;
    break;}
case 278:
#line 5166 "parser.y"
{ 
		    SwigType *t;
		    yyval.decl = yyvsp[-3].decl;
		    t = NewStringEmpty();
		    SwigType_add_array(t,yyvsp[-1].dtype.val);
		    if (yyval.decl.type) {
		      SwigType_push(t,yyval.decl.type);
		      Delete(yyval.decl.type);
		    }
		    yyval.decl.type = t;
                  ;
    break;}
case 279:
#line 5177 "parser.y"
{
		    SwigType *t;
                    yyval.decl = yyvsp[-3].decl;
		    t = NewStringEmpty();
		    SwigType_add_function(t,yyvsp[-1].pl);
		    if (!yyval.decl.have_parms) {
		      yyval.decl.parms = yyvsp[-1].pl;
		      yyval.decl.have_parms = 1;
		    }
		    if (!yyval.decl.type) {
		      yyval.decl.type = t;
		    } else {
		      SwigType_push(t, yyval.decl.type);
		      Delete(yyval.decl.type);
		      yyval.decl.type = t;
		    }
		  ;
    break;}
case 280:
#line 5196 "parser.y"
{
		    yyval.decl.type = yyvsp[0].type;
                    yyval.decl.id = 0;
		    yyval.decl.parms = 0;
		    yyval.decl.have_parms = 0;
                  ;
    break;}
case 281:
#line 5202 "parser.y"
{ 
                     yyval.decl = yyvsp[0].decl;
                     SwigType_push(yyvsp[-1].type,yyvsp[0].decl.type);
		     yyval.decl.type = yyvsp[-1].type;
		     Delete(yyvsp[0].decl.type);
                  ;
    break;}
case 282:
#line 5208 "parser.y"
{
		    yyval.decl.type = yyvsp[-1].type;
		    SwigType_add_reference(yyval.decl.type);
		    yyval.decl.id = 0;
		    yyval.decl.parms = 0;
		    yyval.decl.have_parms = 0;
		  ;
    break;}
case 283:
#line 5215 "parser.y"
{
		    yyval.decl = yyvsp[0].decl;
		    SwigType_add_reference(yyvsp[-2].type);
		    if (yyval.decl.type) {
		      SwigType_push(yyvsp[-2].type,yyval.decl.type);
		      Delete(yyval.decl.type);
		    }
		    yyval.decl.type = yyvsp[-2].type;
                  ;
    break;}
case 284:
#line 5224 "parser.y"
{
		    yyval.decl = yyvsp[0].decl;
                  ;
    break;}
case 285:
#line 5227 "parser.y"
{
		    yyval.decl = yyvsp[0].decl;
		    yyval.decl.type = NewStringEmpty();
		    SwigType_add_reference(yyval.decl.type);
		    if (yyvsp[0].decl.type) {
		      SwigType_push(yyval.decl.type,yyvsp[0].decl.type);
		      Delete(yyvsp[0].decl.type);
		    }
                  ;
    break;}
case 286:
#line 5236 "parser.y"
{ 
                    yyval.decl.id = 0;
                    yyval.decl.parms = 0;
		    yyval.decl.have_parms = 0;
                    yyval.decl.type = NewStringEmpty();
		    SwigType_add_reference(yyval.decl.type);
                  ;
    break;}
case 287:
#line 5243 "parser.y"
{ 
		    yyval.decl.type = NewStringEmpty();
                    SwigType_add_memberpointer(yyval.decl.type,yyvsp[-1].str);
                    yyval.decl.id = 0;
                    yyval.decl.parms = 0;
		    yyval.decl.have_parms = 0;
      	          ;
    break;}
case 288:
#line 5250 "parser.y"
{ 
		    SwigType *t = NewStringEmpty();
                    yyval.decl.type = yyvsp[-2].type;
		    yyval.decl.id = 0;
		    yyval.decl.parms = 0;
		    yyval.decl.have_parms = 0;
		    SwigType_add_memberpointer(t,yyvsp[-1].str);
		    SwigType_push(yyval.decl.type,t);
		    Delete(t);
                  ;
    break;}
case 289:
#line 5260 "parser.y"
{ 
		    yyval.decl = yyvsp[0].decl;
		    SwigType_add_memberpointer(yyvsp[-3].type,yyvsp[-2].str);
		    if (yyval.decl.type) {
		      SwigType_push(yyvsp[-3].type,yyval.decl.type);
		      Delete(yyval.decl.type);
		    }
		    yyval.decl.type = yyvsp[-3].type;
                  ;
    break;}
case 290:
#line 5271 "parser.y"
{ 
		    SwigType *t;
		    yyval.decl = yyvsp[-2].decl;
		    t = NewStringEmpty();
		    SwigType_add_array(t,(char*)"");
		    if (yyval.decl.type) {
		      SwigType_push(t,yyval.decl.type);
		      Delete(yyval.decl.type);
		    }
		    yyval.decl.type = t;
                  ;
    break;}
case 291:
#line 5282 "parser.y"
{ 
		    SwigType *t;
		    yyval.decl = yyvsp[-3].decl;
		    t = NewStringEmpty();
		    SwigType_add_array(t,yyvsp[-1].dtype.val);
		    if (yyval.decl.type) {
		      SwigType_push(t,yyval.decl.type);
		      Delete(yyval.decl.type);
		    }
		    yyval.decl.type = t;
                  ;
    break;}
case 292:
#line 5293 "parser.y"
{ 
		    yyval.decl.type = NewStringEmpty();
		    yyval.decl.id = 0;
		    yyval.decl.parms = 0;
		    yyval.decl.have_parms = 0;
		    SwigType_add_array(yyval.decl.type,(char*)"");
                  ;
    break;}
case 293:
#line 5300 "parser.y"
{ 
		    yyval.decl.type = NewStringEmpty();
		    yyval.decl.id = 0;
		    yyval.decl.parms = 0;
		    yyval.decl.have_parms = 0;
		    SwigType_add_array(yyval.decl.type,yyvsp[-1].dtype.val);
		  ;
    break;}
case 294:
#line 5307 "parser.y"
{
                    yyval.decl = yyvsp[-1].decl;
		  ;
    break;}
case 295:
#line 5310 "parser.y"
{
		    SwigType *t;
                    yyval.decl = yyvsp[-3].decl;
		    t = NewStringEmpty();
                    SwigType_add_function(t,yyvsp[-1].pl);
		    if (!yyval.decl.type) {
		      yyval.decl.type = t;
		    } else {
		      SwigType_push(t,yyval.decl.type);
		      Delete(yyval.decl.type);
		      yyval.decl.type = t;
		    }
		    if (!yyval.decl.have_parms) {
		      yyval.decl.parms = yyvsp[-1].pl;
		      yyval.decl.have_parms = 1;
		    }
		  ;
    break;}
case 296:
#line 5327 "parser.y"
{
                    yyval.decl.type = NewStringEmpty();
                    SwigType_add_function(yyval.decl.type,yyvsp[-1].pl);
		    yyval.decl.parms = yyvsp[-1].pl;
		    yyval.decl.have_parms = 1;
		    yyval.decl.id = 0;
                  ;
    break;}
case 297:
#line 5337 "parser.y"
{ 
               yyval.type = NewStringEmpty();
               SwigType_add_pointer(yyval.type);
	       SwigType_push(yyval.type,yyvsp[-1].str);
	       SwigType_push(yyval.type,yyvsp[0].type);
	       Delete(yyvsp[0].type);
           ;
    break;}
case 298:
#line 5344 "parser.y"
{
	     yyval.type = NewStringEmpty();
	     SwigType_add_pointer(yyval.type);
	     SwigType_push(yyval.type,yyvsp[0].type);
	     Delete(yyvsp[0].type);
	     ;
    break;}
case 299:
#line 5350 "parser.y"
{ 
	     	yyval.type = NewStringEmpty();	
		SwigType_add_pointer(yyval.type);
	        SwigType_push(yyval.type,yyvsp[0].str);
           ;
    break;}
case 300:
#line 5355 "parser.y"
{
	      yyval.type = NewStringEmpty();
	      SwigType_add_pointer(yyval.type);
           ;
    break;}
case 301:
#line 5361 "parser.y"
{
	          yyval.str = NewStringEmpty();
	          if (yyvsp[0].id) SwigType_add_qualifier(yyval.str,yyvsp[0].id);
               ;
    break;}
case 302:
#line 5365 "parser.y"
{
		  yyval.str = yyvsp[0].str;
	          if (yyvsp[-1].id) SwigType_add_qualifier(yyval.str,yyvsp[-1].id);
               ;
    break;}
case 303:
#line 5371 "parser.y"
{ yyval.id = "const"; ;
    break;}
case 304:
#line 5372 "parser.y"
{ yyval.id = "volatile"; ;
    break;}
case 305:
#line 5373 "parser.y"
{ yyval.id = 0; ;
    break;}
case 306:
#line 5379 "parser.y"
{
                   yyval.type = yyvsp[0].type;
                   Replace(yyval.type,"typename ","", DOH_REPLACE_ANY);
                ;
    break;}
case 307:
#line 5385 "parser.y"
{
                   yyval.type = yyvsp[0].type;
	           SwigType_push(yyval.type,yyvsp[-1].str);
               ;
    break;}
case 308:
#line 5389 "parser.y"
{ yyval.type = yyvsp[0].type; ;
    break;}
case 309:
#line 5390 "parser.y"
{
		  yyval.type = yyvsp[-1].type;
	          SwigType_push(yyval.type,yyvsp[0].str);
	       ;
    break;}
case 310:
#line 5394 "parser.y"
{
		  yyval.type = yyvsp[-1].type;
	          SwigType_push(yyval.type,yyvsp[0].str);
	          SwigType_push(yyval.type,yyvsp[-2].str);
	       ;
    break;}
case 311:
#line 5401 "parser.y"
{ yyval.type = yyvsp[0].type;
                  /* Printf(stdout,"primitive = '%s'\n", $$);*/
                ;
    break;}
case 312:
#line 5404 "parser.y"
{ yyval.type = yyvsp[0].type; ;
    break;}
case 313:
#line 5405 "parser.y"
{ yyval.type = yyvsp[0].type; ;
    break;}
case 314:
#line 5406 "parser.y"
{ yyval.type = NewStringf("%s%s",yyvsp[-1].type,yyvsp[0].id); ;
    break;}
case 315:
#line 5407 "parser.y"
{ yyval.type = NewStringf("enum %s", yyvsp[0].str); ;
    break;}
case 316:
#line 5408 "parser.y"
{ yyval.type = yyvsp[0].type; ;
    break;}
case 317:
#line 5410 "parser.y"
{
		  yyval.type = yyvsp[0].str;
               ;
    break;}
case 318:
#line 5413 "parser.y"
{ 
		 yyval.type = NewStringf("%s %s", yyvsp[-1].id, yyvsp[0].str);
               ;
    break;}
case 319:
#line 5418 "parser.y"
{
		 if (!yyvsp[0].ptype.type) yyvsp[0].ptype.type = NewString("int");
		 if (yyvsp[0].ptype.us) {
		   yyval.type = NewStringf("%s %s", yyvsp[0].ptype.us, yyvsp[0].ptype.type);
		   Delete(yyvsp[0].ptype.us);
                   Delete(yyvsp[0].ptype.type);
		 } else {
                   yyval.type = yyvsp[0].ptype.type;
		 }
		 if (Cmp(yyval.type,"signed int") == 0) {
		   Delete(yyval.type);
		   yyval.type = NewString("int");
                 } else if (Cmp(yyval.type,"signed long") == 0) {
		   Delete(yyval.type);
                   yyval.type = NewString("long");
                 } else if (Cmp(yyval.type,"signed short") == 0) {
		   Delete(yyval.type);
		   yyval.type = NewString("short");
		 } else if (Cmp(yyval.type,"signed long long") == 0) {
		   Delete(yyval.type);
		   yyval.type = NewString("long long");
		 }
               ;
    break;}
case 320:
#line 5443 "parser.y"
{ 
                 yyval.ptype = yyvsp[0].ptype;
               ;
    break;}
case 321:
#line 5446 "parser.y"
{
                    if (yyvsp[-1].ptype.us && yyvsp[0].ptype.us) {
		      Swig_error(cparse_file, cparse_line, "Extra %s specifier.\n", yyvsp[0].ptype.us);
		    }
                    yyval.ptype = yyvsp[0].ptype;
                    if (yyvsp[-1].ptype.us) yyval.ptype.us = yyvsp[-1].ptype.us;
		    if (yyvsp[-1].ptype.type) {
		      if (!yyvsp[0].ptype.type) yyval.ptype.type = yyvsp[-1].ptype.type;
		      else {
			int err = 0;
			if ((Cmp(yyvsp[-1].ptype.type,"long") == 0)) {
			  if ((Cmp(yyvsp[0].ptype.type,"long") == 0) || (Strncmp(yyvsp[0].ptype.type,"double",6) == 0)) {
			    yyval.ptype.type = NewStringf("long %s", yyvsp[0].ptype.type);
			  } else if (Cmp(yyvsp[0].ptype.type,"int") == 0) {
			    yyval.ptype.type = yyvsp[-1].ptype.type;
			  } else {
			    err = 1;
			  }
			} else if ((Cmp(yyvsp[-1].ptype.type,"short")) == 0) {
			  if (Cmp(yyvsp[0].ptype.type,"int") == 0) {
			    yyval.ptype.type = yyvsp[-1].ptype.type;
			  } else {
			    err = 1;
			  }
			} else if (Cmp(yyvsp[-1].ptype.type,"int") == 0) {
			  yyval.ptype.type = yyvsp[0].ptype.type;
			} else if (Cmp(yyvsp[-1].ptype.type,"double") == 0) {
			  if (Cmp(yyvsp[0].ptype.type,"long") == 0) {
			    yyval.ptype.type = NewString("long double");
			  } else if (Cmp(yyvsp[0].ptype.type,"complex") == 0) {
			    yyval.ptype.type = NewString("double complex");
			  } else {
			    err = 1;
			  }
			} else if (Cmp(yyvsp[-1].ptype.type,"float") == 0) {
			  if (Cmp(yyvsp[0].ptype.type,"complex") == 0) {
			    yyval.ptype.type = NewString("float complex");
			  } else {
			    err = 1;
			  }
			} else if (Cmp(yyvsp[-1].ptype.type,"complex") == 0) {
			  yyval.ptype.type = NewStringf("%s complex", yyvsp[0].ptype.type);
			} else {
			  err = 1;
			}
			if (err) {
			  Swig_error(cparse_file, cparse_line, "Extra %s specifier.\n", yyvsp[-1].ptype.type);
			}
		      }
		    }
               ;
    break;}
case 322:
#line 5500 "parser.y"
{ 
		    yyval.ptype.type = NewString("int");
                    yyval.ptype.us = 0;
               ;
    break;}
case 323:
#line 5504 "parser.y"
{ 
                    yyval.ptype.type = NewString("short");
                    yyval.ptype.us = 0;
                ;
    break;}
case 324:
#line 5508 "parser.y"
{ 
                    yyval.ptype.type = NewString("long");
                    yyval.ptype.us = 0;
                ;
    break;}
case 325:
#line 5512 "parser.y"
{ 
                    yyval.ptype.type = NewString("char");
                    yyval.ptype.us = 0;
                ;
    break;}
case 326:
#line 5516 "parser.y"
{ 
                    yyval.ptype.type = NewString("wchar_t");
                    yyval.ptype.us = 0;
                ;
    break;}
case 327:
#line 5520 "parser.y"
{ 
                    yyval.ptype.type = NewString("float");
                    yyval.ptype.us = 0;
                ;
    break;}
case 328:
#line 5524 "parser.y"
{ 
                    yyval.ptype.type = NewString("double");
                    yyval.ptype.us = 0;
                ;
    break;}
case 329:
#line 5528 "parser.y"
{ 
                    yyval.ptype.us = NewString("signed");
                    yyval.ptype.type = 0;
                ;
    break;}
case 330:
#line 5532 "parser.y"
{ 
                    yyval.ptype.us = NewString("unsigned");
                    yyval.ptype.type = 0;
                ;
    break;}
case 331:
#line 5536 "parser.y"
{ 
                    yyval.ptype.type = NewString("complex");
                    yyval.ptype.us = 0;
                ;
    break;}
case 332:
#line 5540 "parser.y"
{ 
                    yyval.ptype.type = NewString("__int8");
                    yyval.ptype.us = 0;
                ;
    break;}
case 333:
#line 5544 "parser.y"
{ 
                    yyval.ptype.type = NewString("__int16");
                    yyval.ptype.us = 0;
                ;
    break;}
case 334:
#line 5548 "parser.y"
{ 
                    yyval.ptype.type = NewString("__int32");
                    yyval.ptype.us = 0;
                ;
    break;}
case 335:
#line 5552 "parser.y"
{ 
                    yyval.ptype.type = NewString("__int64");
                    yyval.ptype.us = 0;
                ;
    break;}
case 336:
#line 5558 "parser.y"
{ /* scanner_check_typedef(); */ ;
    break;}
case 337:
#line 5558 "parser.y"
{
                   yyval.dtype = yyvsp[0].dtype;
		   if (yyval.dtype.type == T_STRING) {
		     yyval.dtype.rawval = NewStringf("\"%(escape)s\"",yyval.dtype.val);
		   } else if (yyval.dtype.type != T_CHAR) {
		     yyval.dtype.rawval = 0;
		   }
		   yyval.dtype.bitfield = 0;
		   yyval.dtype.throws = 0;
		   yyval.dtype.throwf = 0;
		   scanner_ignore_typedef();
                ;
    break;}
case 338:
#line 5584 "parser.y"
{ yyval.id = yyvsp[0].id; ;
    break;}
case 339:
#line 5585 "parser.y"
{ yyval.id = (char *) 0;;
    break;}
case 340:
#line 5588 "parser.y"
{ 

                  /* Ignore if there is a trailing comma in the enum list */
                  if (yyvsp[0].node) {
                    Node *leftSibling = Getattr(yyvsp[-2].node,"_last");
                    if (!leftSibling) {
                      leftSibling=yyvsp[-2].node;
                    }
                    set_nextSibling(leftSibling,yyvsp[0].node);
                    Setattr(yyvsp[-2].node,"_last",yyvsp[0].node);
                  }
		  yyval.node = yyvsp[-2].node;
               ;
    break;}
case 341:
#line 5601 "parser.y"
{ 
                   yyval.node = yyvsp[0].node; 
                   if (yyvsp[0].node) {
                     Setattr(yyvsp[0].node,"_last",yyvsp[0].node);
                   }
               ;
    break;}
case 342:
#line 5609 "parser.y"
{
		   SwigType *type = NewSwigType(T_INT);
		   yyval.node = new_node("enumitem");
		   Setattr(yyval.node,"name",yyvsp[0].id);
		   Setattr(yyval.node,"type",type);
		   SetFlag(yyval.node,"feature:immutable");
		   Delete(type);
		 ;
    break;}
case 343:
#line 5617 "parser.y"
{
		   SwigType *type = NewSwigType(yyvsp[0].dtype.type == T_BOOL ? T_BOOL : (yyvsp[0].dtype.type == T_CHAR ? T_CHAR : T_INT));
		   yyval.node = new_node("enumitem");
		   Setattr(yyval.node,"name",yyvsp[-2].id);
		   Setattr(yyval.node,"type",type);
		   SetFlag(yyval.node,"feature:immutable");
		   Setattr(yyval.node,"enumvalue", yyvsp[0].dtype.val);
		   Setattr(yyval.node,"value",yyvsp[-2].id);
		   Delete(type);
                 ;
    break;}
case 344:
#line 5627 "parser.y"
{ yyval.node = 0; ;
    break;}
case 345:
#line 5630 "parser.y"
{
                   yyval.dtype = yyvsp[0].dtype;
		   if ((yyval.dtype.type != T_INT) && (yyval.dtype.type != T_UINT) &&
		       (yyval.dtype.type != T_LONG) && (yyval.dtype.type != T_ULONG) &&
		       (yyval.dtype.type != T_SHORT) && (yyval.dtype.type != T_USHORT) &&
		       (yyval.dtype.type != T_SCHAR) && (yyval.dtype.type != T_UCHAR) &&
		       (yyval.dtype.type != T_CHAR) && (yyval.dtype.type != T_BOOL)) {
		     Swig_error(cparse_file,cparse_line,"Type error. Expecting an integral type\n");
		   }
                ;
    break;}
case 346:
#line 5644 "parser.y"
{ yyval.dtype = yyvsp[0].dtype; ;
    break;}
case 347:
#line 5645 "parser.y"
{
		 Node *n;
		 yyval.dtype.val = yyvsp[0].type;
		 yyval.dtype.type = T_INT;
		 /* Check if value is in scope */
		 n = Swig_symbol_clookup(yyvsp[0].type,0);
		 if (n) {
                   /* A band-aid for enum values used in expressions. */
                   if (Strcmp(nodeType(n),"enumitem") == 0) {
                     String *q = Swig_symbol_qualified(n);
                     if (q) {
                       yyval.dtype.val = NewStringf("%s::%s", q, Getattr(n,"name"));
                       Delete(q);
                     }
                   }
		 }
               ;
    break;}
case 348:
#line 5664 "parser.y"
{ yyval.dtype = yyvsp[0].dtype; ;
    break;}
case 349:
#line 5665 "parser.y"
{
		    yyval.dtype.val = NewString(yyvsp[0].id);
                    yyval.dtype.type = T_STRING;
               ;
    break;}
case 350:
#line 5669 "parser.y"
{
		  SwigType_push(yyvsp[-2].type,yyvsp[-1].decl.type);
		  yyval.dtype.val = NewStringf("sizeof(%s)",SwigType_str(yyvsp[-2].type,0));
		  yyval.dtype.type = T_ULONG;
               ;
    break;}
case 351:
#line 5674 "parser.y"
{ yyval.dtype = yyvsp[0].dtype; ;
    break;}
case 352:
#line 5675 "parser.y"
{
		  yyval.dtype.val = NewString(yyvsp[0].str);
		  if (Len(yyval.dtype.val)) {
		    yyval.dtype.rawval = NewStringf("'%(escape)s'", yyval.dtype.val);
		  } else {
		    yyval.dtype.rawval = NewString("'\\0'");
		  }
		  yyval.dtype.type = T_CHAR;
		  yyval.dtype.bitfield = 0;
		  yyval.dtype.throws = 0;
		  yyval.dtype.throwf = 0;
	       ;
    break;}
case 353:
#line 5689 "parser.y"
{
   	            yyval.dtype.val = NewStringf("(%s)",yyvsp[-1].dtype.val);
		    yyval.dtype.type = yyvsp[-1].dtype.type;
   	       ;
    break;}
case 354:
#line 5696 "parser.y"
{
                 yyval.dtype = yyvsp[0].dtype;
		 if (yyvsp[0].dtype.type != T_STRING) {
		   switch (yyvsp[-2].dtype.type) {
		     case T_FLOAT:
		     case T_DOUBLE:
		     case T_LONGDOUBLE:
		     case T_FLTCPLX:
		     case T_DBLCPLX:
		       yyval.dtype.val = NewStringf("(%s)%s", yyvsp[-2].dtype.val, yyvsp[0].dtype.val); /* SwigType_str and decimal points don't mix! */
		       break;
		     default:
		       yyval.dtype.val = NewStringf("(%s) %s", SwigType_str(yyvsp[-2].dtype.val,0), yyvsp[0].dtype.val);
		       break;
		   }
		 }
 	       ;
    break;}
case 355:
#line 5713 "parser.y"
{
                 yyval.dtype = yyvsp[0].dtype;
		 if (yyvsp[0].dtype.type != T_STRING) {
		   SwigType_push(yyvsp[-3].dtype.val,yyvsp[-2].type);
		   yyval.dtype.val = NewStringf("(%s) %s", SwigType_str(yyvsp[-3].dtype.val,0), yyvsp[0].dtype.val);
		 }
 	       ;
    break;}
case 356:
#line 5720 "parser.y"
{
                 yyval.dtype = yyvsp[0].dtype;
		 if (yyvsp[0].dtype.type != T_STRING) {
		   SwigType_add_reference(yyvsp[-3].dtype.val);
		   yyval.dtype.val = NewStringf("(%s) %s", SwigType_str(yyvsp[-3].dtype.val,0), yyvsp[0].dtype.val);
		 }
 	       ;
    break;}
case 357:
#line 5727 "parser.y"
{
                 yyval.dtype = yyvsp[0].dtype;
		 if (yyvsp[0].dtype.type != T_STRING) {
		   SwigType_push(yyvsp[-4].dtype.val,yyvsp[-3].type);
		   SwigType_add_reference(yyvsp[-4].dtype.val);
		   yyval.dtype.val = NewStringf("(%s) %s", SwigType_str(yyvsp[-4].dtype.val,0), yyvsp[0].dtype.val);
		 }
 	       ;
    break;}
case 358:
#line 5735 "parser.y"
{
		 yyval.dtype = yyvsp[0].dtype;
                 yyval.dtype.val = NewStringf("&%s",yyvsp[0].dtype.val);
	       ;
    break;}
case 359:
#line 5739 "parser.y"
{
		 yyval.dtype = yyvsp[0].dtype;
                 yyval.dtype.val = NewStringf("*%s",yyvsp[0].dtype.val);
	       ;
    break;}
case 360:
#line 5745 "parser.y"
{ yyval.dtype = yyvsp[0].dtype; ;
    break;}
case 361:
#line 5746 "parser.y"
{ yyval.dtype = yyvsp[0].dtype; ;
    break;}
case 362:
#line 5747 "parser.y"
{ yyval.dtype = yyvsp[0].dtype; ;
    break;}
case 363:
#line 5748 "parser.y"
{ yyval.dtype = yyvsp[0].dtype; ;
    break;}
case 364:
#line 5749 "parser.y"
{ yyval.dtype = yyvsp[0].dtype; ;
    break;}
case 365:
#line 5750 "parser.y"
{ yyval.dtype = yyvsp[0].dtype; ;
    break;}
case 366:
#line 5751 "parser.y"
{ yyval.dtype = yyvsp[0].dtype; ;
    break;}
case 367:
#line 5752 "parser.y"
{ yyval.dtype = yyvsp[0].dtype; ;
    break;}
case 368:
#line 5755 "parser.y"
{
		 yyval.dtype.val = NewStringf("%s+%s",yyvsp[-2].dtype.val,yyvsp[0].dtype.val);
		 yyval.dtype.type = promote(yyvsp[-2].dtype.type,yyvsp[0].dtype.type);
	       ;
    break;}
case 369:
#line 5759 "parser.y"
{
		 yyval.dtype.val = NewStringf("%s-%s",yyvsp[-2].dtype.val,yyvsp[0].dtype.val);
		 yyval.dtype.type = promote(yyvsp[-2].dtype.type,yyvsp[0].dtype.type);
	       ;
    break;}
case 370:
#line 5763 "parser.y"
{
		 yyval.dtype.val = NewStringf("%s*%s",yyvsp[-2].dtype.val,yyvsp[0].dtype.val);
		 yyval.dtype.type = promote(yyvsp[-2].dtype.type,yyvsp[0].dtype.type);
	       ;
    break;}
case 371:
#line 5767 "parser.y"
{
		 yyval.dtype.val = NewStringf("%s/%s",yyvsp[-2].dtype.val,yyvsp[0].dtype.val);
		 yyval.dtype.type = promote(yyvsp[-2].dtype.type,yyvsp[0].dtype.type);
	       ;
    break;}
case 372:
#line 5771 "parser.y"
{
		 yyval.dtype.val = NewStringf("%s%%%s",yyvsp[-2].dtype.val,yyvsp[0].dtype.val);
		 yyval.dtype.type = promote(yyvsp[-2].dtype.type,yyvsp[0].dtype.type);
	       ;
    break;}
case 373:
#line 5775 "parser.y"
{
		 yyval.dtype.val = NewStringf("%s&%s",yyvsp[-2].dtype.val,yyvsp[0].dtype.val);
		 yyval.dtype.type = promote(yyvsp[-2].dtype.type,yyvsp[0].dtype.type);
	       ;
    break;}
case 374:
#line 5779 "parser.y"
{
		 yyval.dtype.val = NewStringf("%s|%s",yyvsp[-2].dtype.val,yyvsp[0].dtype.val);
		 yyval.dtype.type = promote(yyvsp[-2].dtype.type,yyvsp[0].dtype.type);
	       ;
    break;}
case 375:
#line 5783 "parser.y"
{
		 yyval.dtype.val = NewStringf("%s^%s",yyvsp[-2].dtype.val,yyvsp[0].dtype.val);
		 yyval.dtype.type = promote(yyvsp[-2].dtype.type,yyvsp[0].dtype.type);
	       ;
    break;}
case 376:
#line 5787 "parser.y"
{
		 yyval.dtype.val = NewStringf("%s << %s",yyvsp[-2].dtype.val,yyvsp[0].dtype.val);
		 yyval.dtype.type = promote_type(yyvsp[-2].dtype.type);
	       ;
    break;}
case 377:
#line 5791 "parser.y"
{
		 yyval.dtype.val = NewStringf("%s >> %s",yyvsp[-2].dtype.val,yyvsp[0].dtype.val);
		 yyval.dtype.type = promote_type(yyvsp[-2].dtype.type);
	       ;
    break;}
case 378:
#line 5795 "parser.y"
{
		 yyval.dtype.val = NewStringf("%s&&%s",yyvsp[-2].dtype.val,yyvsp[0].dtype.val);
		 yyval.dtype.type = cparse_cplusplus ? T_BOOL : T_INT;
	       ;
    break;}
case 379:
#line 5799 "parser.y"
{
		 yyval.dtype.val = NewStringf("%s||%s",yyvsp[-2].dtype.val,yyvsp[0].dtype.val);
		 yyval.dtype.type = cparse_cplusplus ? T_BOOL : T_INT;
	       ;
    break;}
case 380:
#line 5803 "parser.y"
{
		 yyval.dtype.val = NewStringf("%s==%s",yyvsp[-2].dtype.val,yyvsp[0].dtype.val);
		 yyval.dtype.type = cparse_cplusplus ? T_BOOL : T_INT;
	       ;
    break;}
case 381:
#line 5807 "parser.y"
{
		 yyval.dtype.val = NewStringf("%s!=%s",yyvsp[-2].dtype.val,yyvsp[0].dtype.val);
		 yyval.dtype.type = cparse_cplusplus ? T_BOOL : T_INT;
	       ;
    break;}
case 382:
#line 5821 "parser.y"
{
		 yyval.dtype.val = NewStringf("%s >= %s", yyvsp[-2].dtype.val, yyvsp[0].dtype.val);
		 yyval.dtype.type = cparse_cplusplus ? T_BOOL : T_INT;
	       ;
    break;}
case 383:
#line 5825 "parser.y"
{
		 yyval.dtype.val = NewStringf("%s <= %s", yyvsp[-2].dtype.val, yyvsp[0].dtype.val);
		 yyval.dtype.type = cparse_cplusplus ? T_BOOL : T_INT;
	       ;
    break;}
case 384:
#line 5829 "parser.y"
{
		 yyval.dtype.val = NewStringf("%s?%s:%s", yyvsp[-4].dtype.val, yyvsp[-2].dtype.val, yyvsp[0].dtype.val);
		 /* This may not be exactly right, but is probably good enough
		  * for the purposes of parsing constant expressions. */
		 yyval.dtype.type = promote(yyvsp[-2].dtype.type, yyvsp[0].dtype.type);
	       ;
    break;}
case 385:
#line 5835 "parser.y"
{
		 yyval.dtype.val = NewStringf("-%s",yyvsp[0].dtype.val);
		 yyval.dtype.type = yyvsp[0].dtype.type;
	       ;
    break;}
case 386:
#line 5839 "parser.y"
{
                 yyval.dtype.val = NewStringf("+%s",yyvsp[0].dtype.val);
		 yyval.dtype.type = yyvsp[0].dtype.type;
	       ;
    break;}
case 387:
#line 5843 "parser.y"
{
		 yyval.dtype.val = NewStringf("~%s",yyvsp[0].dtype.val);
		 yyval.dtype.type = yyvsp[0].dtype.type;
	       ;
    break;}
case 388:
#line 5847 "parser.y"
{
                 yyval.dtype.val = NewStringf("!%s",yyvsp[0].dtype.val);
		 yyval.dtype.type = T_INT;
	       ;
    break;}
case 389:
#line 5851 "parser.y"
{
		 String *qty;
                 skip_balanced('(',')');
		 qty = Swig_symbol_type_qualify(yyvsp[-1].type,0);
		 if (SwigType_istemplate(qty)) {
		   String *nstr = SwigType_namestr(qty);
		   Delete(qty);
		   qty = nstr;
		 }
		 yyval.dtype.val = NewStringf("%s%s",qty,scanner_ccode);
		 Clear(scanner_ccode);
		 yyval.dtype.type = T_INT;
		 Delete(qty);
               ;
    break;}
case 390:
#line 5867 "parser.y"
{
		 yyval.bases = yyvsp[0].bases;
               ;
    break;}
case 391:
#line 5872 "parser.y"
{ inherit_list = 1; ;
    break;}
case 392:
#line 5872 "parser.y"
{ yyval.bases = yyvsp[0].bases; inherit_list = 0; ;
    break;}
case 393:
#line 5873 "parser.y"
{ yyval.bases = 0; ;
    break;}
case 394:
#line 5876 "parser.y"
{
		   Hash *list = NewHash();
		   Node *base = yyvsp[0].node;
		   Node *name = Getattr(base,"name");
		   List *lpublic = NewList();
		   List *lprotected = NewList();
		   List *lprivate = NewList();
		   Setattr(list,"public",lpublic);
		   Setattr(list,"protected",lprotected);
		   Setattr(list,"private",lprivate);
		   Delete(lpublic);
		   Delete(lprotected);
		   Delete(lprivate);
		   Append(Getattr(list,Getattr(base,"access")),name);
	           yyval.bases = list;
               ;
    break;}
case 395:
#line 5893 "parser.y"
{
		   Hash *list = yyvsp[-2].bases;
		   Node *base = yyvsp[0].node;
		   Node *name = Getattr(base,"name");
		   Append(Getattr(list,Getattr(base,"access")),name);
                   yyval.bases = list;
               ;
    break;}
case 396:
#line 5902 "parser.y"
{
		 yyval.intvalue = cparse_line;
	       ;
    break;}
case 397:
#line 5904 "parser.y"
{
		 yyval.node = NewHash();
		 Setfile(yyval.node,cparse_file);
		 Setline(yyval.node,yyvsp[-1].intvalue);
		 Setattr(yyval.node,"name",yyvsp[0].str);
		 Setfile(yyvsp[0].str,cparse_file);
		 Setline(yyvsp[0].str,yyvsp[-1].intvalue);
                 if (last_cpptype && (Strcmp(last_cpptype,"struct") != 0)) {
		   Setattr(yyval.node,"access","private");
		   Swig_warning(WARN_PARSE_NO_ACCESS, Getfile(yyval.node), Getline(yyval.node), "No access specifier given for base class '%s' (ignored).\n", SwigType_namestr(yyvsp[0].str));
                 } else {
		   Setattr(yyval.node,"access","public");
		 }
               ;
    break;}
case 398:
#line 5918 "parser.y"
{
		 yyval.intvalue = cparse_line;
	       ;
    break;}
case 399:
#line 5920 "parser.y"
{
		 yyval.node = NewHash();
		 Setfile(yyval.node,cparse_file);
		 Setline(yyval.node,yyvsp[-2].intvalue);
		 Setattr(yyval.node,"name",yyvsp[0].str);
		 Setfile(yyvsp[0].str,cparse_file);
		 Setline(yyvsp[0].str,yyvsp[-2].intvalue);
		 Setattr(yyval.node,"access",yyvsp[-3].id);
	         if (Strcmp(yyvsp[-3].id,"public") != 0) {
		   Swig_warning(WARN_PARSE_PRIVATE_INHERIT, Getfile(yyval.node), Getline(yyval.node), "%s inheritance from base '%s' (ignored).\n", yyvsp[-3].id, SwigType_namestr(yyvsp[0].str));
		 }
               ;
    break;}
case 400:
#line 5934 "parser.y"
{ yyval.id = (char*)"public"; ;
    break;}
case 401:
#line 5935 "parser.y"
{ yyval.id = (char*)"private"; ;
    break;}
case 402:
#line 5936 "parser.y"
{ yyval.id = (char*)"protected"; ;
    break;}
case 403:
#line 5940 "parser.y"
{ 
                   yyval.id = (char*)"class"; 
		   if (!inherit_list) last_cpptype = yyval.id;
               ;
    break;}
case 404:
#line 5944 "parser.y"
{ 
                   yyval.id = (char *)"typename"; 
		   if (!inherit_list) last_cpptype = yyval.id;
               ;
    break;}
case 405:
#line 5950 "parser.y"
{
                 yyval.id = yyvsp[0].id;
               ;
    break;}
case 406:
#line 5953 "parser.y"
{ 
                   yyval.id = (char*)"struct"; 
		   if (!inherit_list) last_cpptype = yyval.id;
               ;
    break;}
case 407:
#line 5957 "parser.y"
{
                   yyval.id = (char*)"union"; 
		   if (!inherit_list) last_cpptype = yyval.id;
               ;
    break;}
case 410:
#line 5967 "parser.y"
{
                    yyval.dtype.qualifier = yyvsp[0].str;
                    yyval.dtype.throws = 0;
                    yyval.dtype.throwf = 0;
               ;
    break;}
case 411:
#line 5972 "parser.y"
{
                    yyval.dtype.qualifier = 0;
                    yyval.dtype.throws = yyvsp[-1].pl;
                    yyval.dtype.throwf = NewString("1");
               ;
    break;}
case 412:
#line 5977 "parser.y"
{
                    yyval.dtype.qualifier = yyvsp[-4].str;
                    yyval.dtype.throws = yyvsp[-1].pl;
                    yyval.dtype.throwf = NewString("1");
               ;
    break;}
case 413:
#line 5982 "parser.y"
{ 
                    yyval.dtype.qualifier = 0; 
                    yyval.dtype.throws = 0;
                    yyval.dtype.throwf = 0;
               ;
    break;}
case 414:
#line 5989 "parser.y"
{ 
                    Clear(scanner_ccode); 
                    yyval.decl.have_parms = 0; 
                    yyval.decl.defarg = 0; 
		    yyval.decl.throws = yyvsp[-2].dtype.throws;
		    yyval.decl.throwf = yyvsp[-2].dtype.throwf;
               ;
    break;}
case 415:
#line 5996 "parser.y"
{ 
                    skip_balanced('{','}'); 
                    yyval.decl.have_parms = 0; 
                    yyval.decl.defarg = 0; 
                    yyval.decl.throws = yyvsp[-2].dtype.throws;
                    yyval.decl.throwf = yyvsp[-2].dtype.throwf;
               ;
    break;}
case 416:
#line 6003 "parser.y"
{ 
                    Clear(scanner_ccode); 
                    yyval.decl.parms = yyvsp[-2].pl; 
                    yyval.decl.have_parms = 1; 
                    yyval.decl.defarg = 0; 
		    yyval.decl.throws = 0;
		    yyval.decl.throwf = 0;
               ;
    break;}
case 417:
#line 6011 "parser.y"
{
                    skip_balanced('{','}'); 
                    yyval.decl.parms = yyvsp[-2].pl; 
                    yyval.decl.have_parms = 1; 
                    yyval.decl.defarg = 0; 
                    yyval.decl.throws = 0;
                    yyval.decl.throwf = 0;
               ;
    break;}
case 418:
#line 6019 "parser.y"
{ 
                    yyval.decl.have_parms = 0; 
                    yyval.decl.defarg = yyvsp[-1].dtype.val; 
                    yyval.decl.throws = 0;
                    yyval.decl.throwf = 0;
               ;
    break;}
case 423:
#line 6035 "parser.y"
{
	            skip_balanced('(',')');
                    Clear(scanner_ccode);
            	;
    break;}
case 424:
#line 6041 "parser.y"
{ 
                     String *s = NewStringEmpty();
                     SwigType_add_template(s,yyvsp[-1].p);
                     yyval.id = Char(s);
		     scanner_last_id(1);
                 ;
    break;}
case 425:
#line 6047 "parser.y"
{ yyval.id = (char*)"";  ;
    break;}
case 426:
#line 6050 "parser.y"
{ yyval.id = yyvsp[0].id; ;
    break;}
case 427:
#line 6051 "parser.y"
{ yyval.id = yyvsp[0].id; ;
    break;}
case 428:
#line 6054 "parser.y"
{ yyval.id = yyvsp[0].id; ;
    break;}
case 429:
#line 6055 "parser.y"
{ yyval.id = 0; ;
    break;}
case 430:
#line 6058 "parser.y"
{ 
                  yyval.str = 0;
		  if (!yyval.str) yyval.str = NewStringf("%s%s", yyvsp[-1].str,yyvsp[0].str);
      	          Delete(yyvsp[0].str);
               ;
    break;}
case 431:
#line 6063 "parser.y"
{ 
		 yyval.str = NewStringf("::%s%s",yyvsp[-1].str,yyvsp[0].str);
                 Delete(yyvsp[0].str);
               ;
    break;}
case 432:
#line 6067 "parser.y"
{
		 yyval.str = NewString(yyvsp[0].str);
   	       ;
    break;}
case 433:
#line 6070 "parser.y"
{
		 yyval.str = NewStringf("::%s",yyvsp[0].str);
               ;
    break;}
case 434:
#line 6073 "parser.y"
{
                 yyval.str = NewString(yyvsp[0].str);
	       ;
    break;}
case 435:
#line 6076 "parser.y"
{
                 yyval.str = NewStringf("::%s",yyvsp[0].str);
               ;
    break;}
case 436:
#line 6081 "parser.y"
{
                   yyval.str = NewStringf("::%s%s",yyvsp[-1].str,yyvsp[0].str);
		   Delete(yyvsp[0].str);
               ;
    break;}
case 437:
#line 6085 "parser.y"
{
                   yyval.str = NewStringf("::%s",yyvsp[0].str);
               ;
    break;}
case 438:
#line 6088 "parser.y"
{
                   yyval.str = NewStringf("::%s",yyvsp[0].str);
               ;
    break;}
case 439:
#line 6095 "parser.y"
{
		 yyval.str = NewStringf("::~%s",yyvsp[0].str);
               ;
    break;}
case 440:
#line 6101 "parser.y"
{
                  yyval.str = NewStringf("%s%s",yyvsp[-1].id,yyvsp[0].id);
		  /*		  if (Len($2)) {
		    scanner_last_id(1);
		    } */
              ;
    break;}
case 441:
#line 6110 "parser.y"
{ 
                  yyval.str = 0;
		  if (!yyval.str) yyval.str = NewStringf("%s%s", yyvsp[-1].id,yyvsp[0].str);
      	          Delete(yyvsp[0].str);
               ;
    break;}
case 442:
#line 6115 "parser.y"
{ 
		 yyval.str = NewStringf("::%s%s",yyvsp[-1].id,yyvsp[0].str);
                 Delete(yyvsp[0].str);
               ;
    break;}
case 443:
#line 6119 "parser.y"
{
		 yyval.str = NewString(yyvsp[0].id);
   	       ;
    break;}
case 444:
#line 6122 "parser.y"
{
		 yyval.str = NewStringf("::%s",yyvsp[0].id);
               ;
    break;}
case 445:
#line 6125 "parser.y"
{
                 yyval.str = NewString(yyvsp[0].str);
	       ;
    break;}
case 446:
#line 6128 "parser.y"
{
                 yyval.str = NewStringf("::%s",yyvsp[0].str);
               ;
    break;}
case 447:
#line 6133 "parser.y"
{
                   yyval.str = NewStringf("::%s%s",yyvsp[-1].id,yyvsp[0].str);
		   Delete(yyvsp[0].str);
               ;
    break;}
case 448:
#line 6137 "parser.y"
{
                   yyval.str = NewStringf("::%s",yyvsp[0].id);
               ;
    break;}
case 449:
#line 6140 "parser.y"
{
                   yyval.str = NewStringf("::%s",yyvsp[0].str);
               ;
    break;}
case 450:
#line 6143 "parser.y"
{
		 yyval.str = NewStringf("::~%s",yyvsp[0].id);
               ;
    break;}
case 451:
#line 6149 "parser.y"
{ 
                   yyval.id = (char *) malloc(strlen(yyvsp[-1].id)+strlen(yyvsp[0].id)+1);
                   strcpy(yyval.id,yyvsp[-1].id);
                   strcat(yyval.id,yyvsp[0].id);
               ;
    break;}
case 452:
#line 6154 "parser.y"
{ yyval.id = yyvsp[0].id;;
    break;}
case 453:
#line 6157 "parser.y"
{
		 yyval.str = NewString(yyvsp[0].id);
               ;
    break;}
case 454:
#line 6160 "parser.y"
{
                  skip_balanced('{','}');
		  yyval.str = NewString(scanner_ccode);
               ;
    break;}
case 455:
#line 6164 "parser.y"
{
		 yyval.str = yyvsp[0].str;
              ;
    break;}
case 456:
#line 6169 "parser.y"
{
                  Hash *n;
                  yyval.node = NewHash();
                  n = yyvsp[-1].node;
                  while(n) {
                     String *name, *value;
                     name = Getattr(n,"name");
                     value = Getattr(n,"value");
		     if (!value) value = (String *) "1";
                     Setattr(yyval.node,name, value);
		     n = nextSibling(n);
		  }
               ;
    break;}
case 457:
#line 6182 "parser.y"
{ yyval.node = 0; ;
    break;}
case 458:
#line 6186 "parser.y"
{
		 yyval.node = NewHash();
		 Setattr(yyval.node,"name",yyvsp[-2].id);
		 Setattr(yyval.node,"value",yyvsp[0].id);
               ;
    break;}
case 459:
#line 6191 "parser.y"
{
		 yyval.node = NewHash();
		 Setattr(yyval.node,"name",yyvsp[-4].id);
		 Setattr(yyval.node,"value",yyvsp[-2].id);
		 set_nextSibling(yyval.node,yyvsp[0].node);
               ;
    break;}
case 460:
#line 6197 "parser.y"
{
                 yyval.node = NewHash();
                 Setattr(yyval.node,"name",yyvsp[0].id);
	       ;
    break;}
case 461:
#line 6201 "parser.y"
{
                 yyval.node = NewHash();
                 Setattr(yyval.node,"name",yyvsp[-2].id);
                 set_nextSibling(yyval.node,yyvsp[0].node);
               ;
    break;}
case 462:
#line 6206 "parser.y"
{
                 yyval.node = yyvsp[0].node;
		 Setattr(yyval.node,"name",yyvsp[-2].id);
               ;
    break;}
case 463:
#line 6210 "parser.y"
{
                 yyval.node = yyvsp[-2].node;
		 Setattr(yyval.node,"name",yyvsp[-4].id);
		 set_nextSibling(yyval.node,yyvsp[0].node);
               ;
    break;}
case 464:
#line 6217 "parser.y"
{
		 yyval.id = yyvsp[0].id;
               ;
    break;}
case 465:
#line 6220 "parser.y"
{
                 yyval.id = Char(yyvsp[0].dtype.val);
               ;
    break;}
}
   /* the action file gets copied in in place of this dollarsign */
#line 543 "bison.simple"

  yyvsp -= yylen;
  yyssp -= yylen;
#ifdef YYLSP_NEEDED
  yylsp -= yylen;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

  *++yyvsp = yyval;

#ifdef YYLSP_NEEDED
  yylsp++;
  if (yylen == 0)
    {
      yylsp->first_line = yylloc.first_line;
      yylsp->first_column = yylloc.first_column;
      yylsp->last_line = (yylsp-1)->last_line;
      yylsp->last_column = (yylsp-1)->last_column;
      yylsp->text = 0;
    }
  else
    {
      yylsp->last_line = (yylsp+yylen-1)->last_line;
      yylsp->last_column = (yylsp+yylen-1)->last_column;
    }
#endif

  /* Now "shift" the result of the reduction.
     Determine what state that goes to,
     based on the state we popped back to
     and the rule number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTBASE] + *yyssp;
  if (yystate >= 0 && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTBASE];

  goto yynewstate;

yyerrlab:   /* here on detecting error */

  if (! yyerrstatus)
    /* If not already recovering from an error, report this error.  */
    {
      ++yynerrs;

#ifdef YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (yyn > YYFLAG && yyn < YYLAST)
	{
	  int size = 0;
	  char *msg;
	  int x, count;

	  count = 0;
	  /* Start X at -yyn if nec to avoid negative indexes in yycheck.  */
	  for (x = (yyn < 0 ? -yyn : 0);
	       x < (sizeof(yytname) / sizeof(char *)); x++)
	    if (yycheck[x + yyn] == x)
	      size += strlen(yytname[x]) + 15, count++;
	  msg = (char *) malloc(size + 15);
	  if (msg != 0)
	    {
	      strcpy(msg, "parse error");

	      if (count < 5)
		{
		  count = 0;
		  for (x = (yyn < 0 ? -yyn : 0);
		       x < (sizeof(yytname) / sizeof(char *)); x++)
		    if (yycheck[x + yyn] == x)
		      {
			strcat(msg, count == 0 ? ", expecting `" : " or `");
			strcat(msg, yytname[x]);
			strcat(msg, "'");
			count++;
		      }
		}
	      yyerror(msg);
	      free(msg);
	    }
	  else
	    yyerror ("parse error; also virtual memory exceeded");
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror("parse error");
    }

  goto yyerrlab1;
yyerrlab1:   /* here on error raised explicitly by an action */

  if (yyerrstatus == 3)
    {
      /* if just tried and failed to reuse lookahead token after an error, discard it.  */

      /* return failure if at end of input */
      if (yychar == YYEOF)
	YYABORT;

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Discarding token %d (%s).\n", yychar, yytname[yychar1]);
#endif

      yychar = YYEMPTY;
    }

  /* Else will try to reuse lookahead token
     after shifting the error token.  */

  yyerrstatus = 3;		/* Each real token shifted decrements this */

  goto yyerrhandle;

yyerrdefault:  /* current state does not do anything special for the error token. */

#if 0
  /* This is wrong; only states that explicitly want error tokens
     should shift them.  */
  yyn = yydefact[yystate];  /* If its default is to accept any token, ok.  Otherwise pop it.*/
  if (yyn) goto yydefault;
#endif

yyerrpop:   /* pop the current state because it cannot handle the error token */

  if (yyssp == yyss) YYABORT;
  yyvsp--;
  yystate = *--yyssp;
#ifdef YYLSP_NEEDED
  yylsp--;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "Error: state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

yyerrhandle:

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yyerrdefault;

  yyn += YYTERROR;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != YYTERROR)
    goto yyerrdefault;

  yyn = yytable[yyn];
  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrpop;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrpop;

  if (yyn == YYFINAL)
    YYACCEPT;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting error token, ");
#endif

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  yystate = yyn;
  goto yynewstate;

 yyacceptlab:
  /* YYACCEPT comes here.  */
  if (yyfree_stacks)
    {
      free (yyss);
      free (yyvs);
#ifdef YYLSP_NEEDED
      free (yyls);
#endif
    }
  return 0;

 yyabortlab:
  /* YYABORT comes here.  */
  if (yyfree_stacks)
    {
      free (yyss);
      free (yyvs);
#ifdef YYLSP_NEEDED
      free (yyls);
#endif
    }
  return 1;
}
#line 6227 "parser.y"


SwigType *Swig_cparse_type(String *s) {
   String *ns;
   ns = NewStringf("%s;",s);
   Seek(ns,0,SEEK_SET);
   scanner_file(ns);
   top = 0;
   scanner_next_token(PARSETYPE);
   yyparse();
   /*   Printf(stdout,"typeparse: '%s' ---> '%s'\n", s, top); */
   return top;
}


Parm *Swig_cparse_parm(String *s) {
   String *ns;
   ns = NewStringf("%s;",s);
   Seek(ns,0,SEEK_SET);
   scanner_file(ns);
   top = 0;
   scanner_next_token(PARSEPARM);
   yyparse();
   /*   Printf(stdout,"typeparse: '%s' ---> '%s'\n", s, top); */
   Delete(ns);
   return top;
}


ParmList *Swig_cparse_parms(String *s, Node *file_line_node) {
   String *ns;
   char *cs = Char(s);
   if (cs && cs[0] != '(') {
     ns = NewStringf("(%s);",s);
   } else {
     ns = NewStringf("%s;",s);
   }
   Setfile(ns, Getfile(file_line_node));
   Setline(ns, Getline(file_line_node));
   Seek(ns,0,SEEK_SET);
   scanner_file(ns);
   top = 0;
   scanner_next_token(PARSEPARMS);
   yyparse();
   /*   Printf(stdout,"typeparse: '%s' ---> '%s'\n", s, top); */
   return top;
}

