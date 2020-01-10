
enum CommandId {
	CI_NONE,				//0	As reset makes 0 and 0xFF, must avoid use of them in header. 
	CI_BOARD_INFO,          //1 Board information.
	CI_SET_CMDLEN,			//2	Set command length for each board id.
	CI_ALL,					//3 Send all data and return all status.
	CI_SENSOR,				//4	Return sensor data
	CI_DIRECT,              //5 Directly set servo targets (positions and velicities).
	CI_CURRENT,             //6 Set currents as servo targets.
	CI_INTERPOLATE,         //7 Send new frame for interpolation.
	CI_FORCE_CONTROL,		//8	Position and force control with interpolation.
	CI_SET_PARAM,           //9 Set parameter.
    CI_RESET_SENSOR,        //10 Reset sensor.
    CI_GET_PARAM,           //11 Get parameter.
    CI_NCOMMAND,
    CI_NCOMMAND_MAX=32,     //0 to 31 can be used for UART command, because command ID has 5 bits.
    CIU_TEXT = CI_NCOMMAND_MAX,	
                            //32 return text message: cmd, type, length, bytes
	CIU_SET_IPADDRESS,		//33 Set ip address to return the packet
    CIU_GET_IPADDRESS,      //34 Get ip address to return the packet
    CIU_GET_SUBBOARD_INFO,  //35 Get sub board info
	CIU_MOVEMENT,			//36 movement command
	CIU_NCOMMAND,           //37 number of commands
	CIU_NONE = -1           //  no command is in receiving state.
};
enum SetParamType{
	PT_PD,          //  PD param (k, b)
    PT_CURRENT,     //  PD param (a)
	PT_TORQUE_LIMIT,
	PT_BOARD_ID,
	PT_BAUDRATE,
    PT_MOTOR_HEAT,
};
enum ResetSensorFlags {
	RSF_NONE=0,
	RSF_MOTOR=1,
	RSF_FORCE=2,
};
enum CommandIdMovement {
	CI_M_NONE = 0,
	CI_M_ADD_KEYFRAME = 1,
	CI_M_PAUSE_MOV = 2,
	CI_M_RESUME_MOV = 3,
	CI_M_PAUSE_INTERPOLATE = 4,
	CI_M_RESUME_INTERPOLATE = 5,
	CI_M_CLEAR_MOV = 6,
	CI_M_CLEAR_PAUSED = 7,
	CI_M_CLEAR_ALL = 8,
	CI_M_QUERY = 9,
	CI_M_COUNT = 9
};

// define the source of command packet, for return packet to know where to return
enum CommandSource {
	CS_WEBSOCKET = 0,
	CS_DUKTAPE = 1,
	CS_MOVEMENT_MANAGER = 2
};
