#ifndef COMMAND_TEMPLATE_H
#define COMMAND_TEMPLATE_H
#include "env.h"
#include "fixed.h"

#ifdef _MSC_VER
#define START_PACKED	__pragma( pack(push, 1) )
#define END_PACKED		__pragma( pack(pop) )
#define PACKED
#else
#define START_PACKED
#define END_PACKED
#define PACKED			__attribute__((__packed__))
#endif

#define sizeof_field(s,m) (sizeof((((s*)0)->m)))

#define NOTCT_0(x)	x
#define NOTCT_N(x)
#define CT_0(x)
#define CT_N(x)	x

#ifdef __cplusplus
#define BOARDINFOFUNCS(BOARD)	\
	static const char* GetBoardName(){ return #BOARD;}				\
	static int GetModelNumber(){ return BOARD##_MODEL_NUMBER;}		\
	static int GetNTarget(){ return BOARD##_NTARGET;}				\
	static int GetNMotor(){ return BOARD##_NMOTOR;}					\
	static int GetNCurrent(){ return BOARD##_NCURRENT;}				\
	static int GetNForce(){ return BOARD##_NFORCE;}					\
	static int GetNTouch(){ return BOARD##_NTOUCH;}					\

#else
#define BOARDINFOFUNCS(BOARD)
#endif

#define MAXBOARDID	7
#define BORADIDBITS 3

START_PACKED
union CommandHeader{
	unsigned char header;
	struct {
		unsigned char boardId : 3;
		unsigned char commandId : 5;
	} PACKED;
} PACKED;
END_PACKED

#define DEFINE_CommandPacket(BOARD, CURRENT, FORCE, TOUCH)			\
START_PACKED														\
struct MotorHeatLimit##BOARD{                                       \
    unsigned short limit[BOARD##_NMOTOR];                           \
    SDEC release[BOARD##_NMOTOR];                                   \
} PACKED;															\
struct SetGetParam##BOARD{    /* CI_SETPARAM / CI_GETPARAM */       \
    unsigned char type;                                             \
    union {                                                         \
        struct {                                                    \
            SDEC k[BOARD##_NMOTOR];	/* P */                         \
            SDEC b[BOARD##_NMOTOR];	/* D */                         \
        }PACKED pd;													\
        struct {                                                    \
            SDEC min[BOARD##_NMOTOR];	/* Tq min */                \
            SDEC max[BOARD##_NMOTOR];	/* Tq max */                \
        }PACKED torque;												\
        SDEC a[BOARD##_NMOTOR];	/* Current */                       \
        unsigned char boardId;	/* boardId */                       \
        unsigned long baudrate[2];	/* baudrate */                  \
        struct MotorHeatLimit##BOARD heat;                          \
    } PACKED;														\
} PACKED;															\
union CommandPacket##BOARD {										\
	BOARDINFOFUNCS(BOARD)											\
	struct {														\
		unsigned char bytes_pad[3];									\
		unsigned char bytes[1 + BOARD##_NMOTOR*2 * 2];      		\
	} PACKED;														\
	struct {														\
		unsigned char header_pad[3];								\
		union {														\
			unsigned char header;									\
			struct {												\
				unsigned char boardId : 3;							\
				unsigned char commandId : 5;						\
			} PACKED;												\
		} PACKED;													\
		union {														\
			struct {/*  CI_SET_CMDLEN	*/							\
				unsigned char len[CI_NCOMMAND];						\
			} PACKED cmdLen;										\
			struct { /*	 CI_ALL */									\
				unsigned char controlMode;							\
				unsigned char targetCountWrite; 					\
				short period;		/*	period to interpolate */	\
				union {                                             \
                    SDEC current[BOARD##_NMOTOR];					\
                    struct{                                         \
                        SDEC pos[BOARD##_NMOTOR];					\
                        union {                                     \
                            SDEC vel[BOARD##_NMOTOR];				\
							SDEC jacob[BOARD##_NFORCE][BOARD##_NMOTOR];\
                        }PACKED;									\
                    } PACKED;										\
                }PACKED;											\
			} PACKED all;											\
			struct {/*  CI_DIRECT	*/								\
				SDEC pos[BOARD##_NMOTOR];							\
				SDEC vel[BOARD##_NMOTOR];							\
			} PACKED direct;										\
			struct {/*  CI_CURRENT  */								\
				SDEC current[BOARD##_NMOTOR];						\
			} PACKED current;										\
			struct { /*	 CI_INTERPOLATE */							\
				SDEC pos[BOARD##_NMOTOR];							\
				short period;		/*	period to interpolate */	\
				unsigned char targetCountWrite;						\
			} PACKED interpolate;									\
			struct { /*	 CI_FORCE_CONTROL */						\
				union {												\
					SDEC pos[BOARD##_NMOTOR];						\
					NOT##FORCE(SDEC jacob[1][BOARD##_NMOTOR];)		\
				} PACKED;											\
				FORCE(SDEC jacob[BOARD##_NFORCE][BOARD##_NMOTOR];)	\
				short period;		/*	period to interpolate */	\
				unsigned char targetCountWrite;						\
			} PACKED forceControl;									\
			struct SetGetParam##BOARD param; /* CI_SETPARAM  */     \
			struct {				 /*	 CI_RESET_SENSOR	 */		\
                short flags;                                        \
			} PACKED resetSensor;									\
		};															\
	};																\
};																	\
END_PACKED															\
enum BOARD##CommandLenEnum{																	\
	BOARD##_CLEN_NONE = 1,																	\
	BOARD##_CLEN_BOARD_INFO = 1,															\
	BOARD##_CLEN_SET_CMDLEN = 1+sizeof_field(union CommandPacket##BOARD, cmdLen),			\
	BOARD##_CLEN_ALL = 1+sizeof_field(union CommandPacket##BOARD, all),						\
    BOARD##_CLEN_SENSOR = 1,																\
	BOARD##_CLEN_DIRECT = 1+sizeof_field(union CommandPacket##BOARD, direct),				\
	BOARD##_CLEN_CURRENT = 1+sizeof_field(union CommandPacket##BOARD, current),				\
    BOARD##_CLEN_INTERPOLATE = 1+sizeof_field(union CommandPacket##BOARD, interpolate),		\
    BOARD##_CLEN_FORCE_CONTROL = 1+sizeof_field(union CommandPacket##BOARD, forceControl),	\
    BOARD##_CLEN_SET_PARAM = 1+sizeof_field(union CommandPacket##BOARD, param),             \
	BOARD##_CLEN_RESET_SENSOR = 1+sizeof_field(union CommandPacket##BOARD, resetSensor),	\
    BOARD##_CLEN_GET_PARAM = 1 + 1,															\
};																	\
const unsigned char cmdPacketLen##BOARD[CI_NCOMMAND] = {			\
    BOARD##_CLEN_NONE,												\
    BOARD##_CLEN_BOARD_INFO,										\
	BOARD##_CLEN_SET_CMDLEN,										\
	BOARD##_CLEN_ALL,												\
	BOARD##_CLEN_SENSOR,											\
    BOARD##_CLEN_DIRECT,											\
    BOARD##_CLEN_CURRENT,											\
    BOARD##_CLEN_INTERPOLATE,										\
    BOARD##_CLEN_FORCE_CONTROL,										\
    BOARD##_CLEN_SET_PARAM,											\
	BOARD##_CLEN_RESET_SENSOR,  									\
    BOARD##_CLEN_GET_PARAM,                                         \
};																	\


#define DEFINE_ReturnPacket(BOARD, CURRENT, FORCE, TOUCH)		\
START_PACKED													\
union ReturnPacket##BOARD {										\
	BOARDINFOFUNCS(BOARD)										\
	struct {													\
		unsigned char bytes_pad[3];								\
		unsigned char bytes[1 + BOARD##_NMOTOR*2*2 + BOARD##_NCURRENT*2 + BOARD##_NFORCE*2];		\
	}PACKED;													\
	struct {													\
		unsigned char header_pad[3];							\
		union {													\
			unsigned char header;								\
			struct {											\
				unsigned char boardId : 3;						\
				unsigned char commandId : 5;					\
			}PACKED;											\
		}PACKED;												\
		union {													\
			struct {		 /*	 CI_BOARD_INFO */				\
				unsigned char modelNumber;						\
				unsigned char nTarget;							\
				unsigned char nMotor;							\
				unsigned char nCurrent;							\
				unsigned char nForce;							\
				unsigned char nTouch;							\
			}PACKED boardInfo;									\
			struct {		 /*	 CI_ALL */						\
				unsigned char controlMode;						\
				unsigned char targetCountRead;					\
				unsigned short tick;							\
				SDEC pos[BOARD##_NMOTOR];						\
				union {											\
					SDEC vel[BOARD##_NMOTOR];					\
					NOT##CURRENT(SDEC current[1];)				\
					NOT##FORCE(SDEC force[1];)					\
					NOT##TOUCH(SDEC touch[1];)					\
				} PACKED;										\
				CURRENT(SDEC current[BOARD##_NCURRENT];)		\
				FORCE(SDEC force[BOARD##_NFORCE];)				\
				TOUCH(SDEC touch[BOARD##_NTOUCH];)				\
			}PACKED all;										\
			struct {		 /*	 CI_SENSOR */					\
				union {											\
					SDEC pos[BOARD##_NMOTOR];					\
					NOT##CURRENT(SDEC current[1];)				\
					NOT##FORCE(SDEC force[1];)					\
					NOT##TOUCH(SDEC touch[1];)					\
				} PACKED;										\
				CURRENT(SDEC current[BOARD##_NCURRENT];)		\
				FORCE(SDEC force[BOARD##_NFORCE];)				\
				TOUCH(SDEC touch[BOARD##_NTOUCH];)				\
			}PACKED sensor;										\
			struct {		 /*	 CI_DIRECT */					\
				SDEC pos[BOARD##_NMOTOR];						\
				SDEC vel[BOARD##_NMOTOR];						\
			}PACKED direct;										\
			struct {		 /*	 CI_CURRENT	*/					\
				SDEC pos[BOARD##_NMOTOR];						\
				union {											\
					SDEC vel[BOARD##_NMOTOR];					\
					NOT##CURRENT(SDEC current[1];)				\
				} PACKED;										\
				CURRENT(SDEC current[BOARD##_NCURRENT];)		\
			}PACKED current;									\
			struct {    /* CI_INTERPOLATE, CI_FORCE_CONTROL */	\
				SDEC pos[BOARD##_NMOTOR];						\
				unsigned short tick;							\
				unsigned char targetCountRead;					\
			}PACKED interpolate;								\
			struct SetGetParam##BOARD param; /* CI_GETPARAM  */ \
		};														\
	};															\
};																\
END_PACKED														\
enum BOARD##ReturnLenEnum{										\
	BOARD##_RLEN_NONE = 0,										\
	BOARD##_RLEN_BOARD_INFO = 1+sizeof_field(union ReturnPacket##BOARD, boardInfo),		\
	BOARD##_RLEN_ALL = 1 + sizeof_field(union ReturnPacket##BOARD, all),				\
	BOARD##_RLEN_SENSOR = 1 + sizeof_field(union ReturnPacket##BOARD, sensor),			\
	BOARD##_RLEN_DIRECT = 1+sizeof_field(union ReturnPacket##BOARD, direct),			\
	BOARD##_RLEN_CURRENT = 1+sizeof_field(union ReturnPacket##BOARD, current),			\
    BOARD##_RLEN_INTERPOLATE = 1+sizeof_field(union ReturnPacket##BOARD, interpolate),	\
    BOARD##_RLEN_FORCE_CONTROL = 1+sizeof_field(union ReturnPacket##BOARD, interpolate),\
    BOARD##_RLEN_GET_PARAM = 1+sizeof_field(union ReturnPacket##BOARD, param),          \
    BOARD##_RLEN_NO_RETURN = 0,															\
};																						\
const unsigned char retPacketLen##BOARD[CI_NCOMMAND]={									\
    BOARD##_RLEN_NONE,																	\
    BOARD##_RLEN_BOARD_INFO,															\
    BOARD##_RLEN_NONE,	/* CI_SET_CMDLEN */												\
	BOARD##_RLEN_ALL,																	\
	BOARD##_RLEN_SENSOR,																\
    BOARD##_RLEN_DIRECT,																\
    BOARD##_RLEN_CURRENT,																\
    BOARD##_RLEN_INTERPOLATE,															\
    BOARD##_RLEN_FORCE_CONTROL,															\
    BOARD##_RLEN_NO_RETURN,		/* CI_SET_PARAM */										\
    BOARD##_RLEN_NO_RETURN,		/* CI_RESET_SENSOR */									\
    BOARD##_RLEN_GET_PARAM,                                                             \
};


#define DEFINE_Packets(BOARD, CURRENT, FORCE, TOUCH)			\
DEFINE_CommandPacket(BOARD, CURRENT, FORCE, TOUCH)				\
DEFINE_ReturnPacket(BOARD, CURRENT, FORCE, TOUCH)				\


#define CHOOSE_BoardInfo(BOARD)     							\
enum BoardInfo{                     							\
    MODEL_NUMBER = BOARD##_MODEL_NUMBER,					 	\
    NTARGET = BOARD##_NTARGET,        							\
    NMOTOR = BOARD##_NMOTOR,        							\
    NFORCE = BOARD##_NFORCE,         							\
    NCURRENT = BOARD##_NCURRENT,       							\
    NTOUCH = BOARD##_NTOUCH        								\
};																\
typedef union CommandPacket##BOARD CommandPacket;				\
typedef union ReturnPacket##BOARD ReturnPacket;					\
const unsigned char* const cmdPacketLen = cmdPacketLen##BOARD;	\
const unsigned char* const retPacketLen = retPacketLen##BOARD;	\
typedef struct MotorHeatLimit##BOARD MotorHeatLimit;            \

#endif
