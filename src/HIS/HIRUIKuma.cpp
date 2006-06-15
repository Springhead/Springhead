#include "HIS.h"
//#include "HIRUIKuma.h"
//#include <iomanip>
#include <time.h>
//#include "HIOrientation.h"

using namespace Spr;

HIOBJECTIMPABST(HIRuiBase, HIHapticDevice);
HIOBJECTIMP(HIRuiKuma, HIRuiBase);

bool HIRuiKuma::Init(){
	comSt.boudRate = 57600;//38400;
	comSt.flowControl = WBComStatus::FC_NONE;
	com.init(comSt);

	com.put('A');
	com.flush();

	int i = 0;
	while(i<1000){
		if(com.avail() >=6 ){
			com.read((char*)receiveBuffer,1);
	//		DSTR << "Read: " << receiveBuffer[0];
			if(receiveBuffer[0] == '\x41'){
				com.read((char*)receiveBuffer,5);
				return true;

			}
		}
		i++;
	}
	Update(0.01f);
	return false;
}

float HIRuiKuma::GetJointAngle(int jn){
	float ang = (float)(data[jn]/1024.0*M_PI*4.0/3.0 - M_PI*2.0/3.0);
		return ang;
}

void HIRuiKuma::SetTorque(int jn, float torque){

/*	senddata[0] = data[0] ;//- torqueR[2];
	senddata[1] = data[1] ;//+ torqueR[0];
	senddata[2] = data[2] ;//- torqueL[2]; 
	senddata[3] = data[3] ;//+ torqueL[0];
*/

/*	for( int i=0; i < jn; i++){
		senddata[i] = data[i] + torque;
		if(senddata[i]>1023)senddata[i] = 1023;
		else if(senddata[i]<0)senddata[i] = 0;
	}
*/
	senddata[jn] = (int)(data[jn] - torque);
	if(senddata[jn]>1023)senddata[jn] = 1023;
	else if(senddata[jn]<0)senddata[jn] = 0;

}



void HIRuiKuma::Update(float dt){

	sendBuffer[0]='A';
	sendBuffer[1]=(senddata[0]>>2)&0xff;
	sendBuffer[2]=(senddata[1]>>2)&0xff;
	sendBuffer[3]=(senddata[2]>>2)&0xff;
	sendBuffer[4]=(senddata[3]>>2)&0xff;
	sendBuffer[5]=((senddata[0]<<6)&0xc0)+((senddata[1]<<4)&0x30)+((senddata[2]<<2)&0x0c)+(senddata[3]&0x03);

	com.write((char*)sendBuffer,6);
//	com.put('A');
	com.flush();
	while(com.avail() >=6 ){
		com.read((char*)receiveBuffer,1);
//		DSTR << "Read: " << receiveBuffer[0];
		if(receiveBuffer[0] == '\x41')
		{
			com.read((char*)receiveBuffer,5);
//			DSTR << std::setbase(16) << (unsigned)buff[0] << (unsigned)buff[1] << (unsigned)buff[2] << (unsigned)buff[3] << (unsigned)buff[4] << std::endl;
//			DSTR.flush();

			data[0]=((receiveBuffer[0]<<2) + ((receiveBuffer[4]>>6) & 0x03));
			data[1]=((receiveBuffer[1]<<2) + ((receiveBuffer[4]>>4) & 0x03));
			data[2]=((receiveBuffer[2]<<2) + ((receiveBuffer[4]>>2) & 0x03));
			data[3]=((receiveBuffer[3]<<2) + ((receiveBuffer[4]>>0) & 0x03));
			
/*			for(int j = 0; j < 4; j++)
			{
				DSTR << data[j] << " ";
			}
			DSTR << std::endl;
*/
		}
	}

}

