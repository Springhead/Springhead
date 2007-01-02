extern "C"
{
	__declspec(dllexport) short __stdcall Adapio_Device_Open(short id);
	__declspec(dllexport) void __stdcall Adapio_Device_Close(short id);
	__declspec(dllexport) short __stdcall Adapio_Pio_LowDirection(short id, unsigned char dir);
	__declspec(dllexport) short __stdcall Adapio_Pio_HighDirection(short id,unsigned char dir);
	__declspec(dllexport) short __stdcall Adapio_Pio_LowByteWrite(short id,unsigned char dat);
	__declspec(dllexport) short __stdcall Adapio_Pio_HighByteWrite(short id,unsigned char dat);
	__declspec(dllexport) short __stdcall Adapio_Pio_LowByteRead(short id,unsigned char *dat);
	__declspec(dllexport) short __stdcall Adapio_Pio_HighByteRead(short id,unsigned char *dat);
	__declspec(dllexport) short __stdcall Adapio_Pio_WordWrite(short id,int dat);
	__declspec(dllexport) short __stdcall Adapio_Pio_WordRead(short id,int *dat);
	__declspec(dllexport) short __stdcall Adapio_Pio_Clock_Out(short id,int count );
	__declspec(dllexport) short __stdcall Adapio_Pio_Clock_Stop(short id);
	__declspec(dllexport) short __stdcall Adapio_Dac_Out(short id,short ch,unsigned char dat);
	__declspec(dllexport) short __stdcall Adapio_Adc_SingleSample(short id,short ch,short *dat);
	__declspec(dllexport) short __stdcall Adapio_Adc_DigitalTrg(short id,short EndCh,short BufSize);
	__declspec(dllexport) short __stdcall Adapio_Adc_AnalogTrg(short id,short EndCh,short BufSize, short Threshold,short TrgCh ,short UpOrDown);
	__declspec(dllexport) short __stdcall Adapio_Adc_GetStatus(short id,short *Running, short *SampledNum);
	__declspec(dllexport) short __stdcall Adapio_Adc_GetDatas(short id,short *databuf, short Len );
}
