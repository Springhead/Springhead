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


extern YYSTYPE yylval;
