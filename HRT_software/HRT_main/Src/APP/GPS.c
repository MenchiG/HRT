#include "includes.h"


void GPRMC_DAT()
{
   unsigned char i,i1=0,uf=0;

   for(i=0;i<Rec_Len;i++){
   	  if(GPSRxBuffer[i]==0x2c){				                         //�ж��Ƿ��Ƕ���
	  	i1++;
		uf=0;
	  }
	  if(i1==1&&uf==0){	                                             //GPRMC ʱ��
	  	GPS_Data.Hour=(GPSRxBuffer[i+1]-0x30)*10+GPSRxBuffer[i+2]-0x30+8;		 //ʱ
   	  	GPS_Data.Min=(GPSRxBuffer[i+3]-0x30)*10+GPSRxBuffer[i+4]-0x30;		     //��
      	GPS_Data.Sec=(GPSRxBuffer[i+5]-0x30)*10+GPSRxBuffer[i+6]-0x30;		     //��
		i=i+6;
		uf=1;
   	  }
	  else if(i1==2&&uf==0){	                                     //GPRMC ״̬��Ч��
	  	if(GPSRxBuffer[i+1]=='A')	GPS_Data.GPS_VA=1; 		                     //�ɹ���λ
		else GPS_Data.GPS_VA=0;												 //��λδ�ɹ�
		i++;
		uf=1;
   	  }
	  else if(i1==3&&uf==0){	                                     //GPRMC γ��
	  	if(GPSRxBuffer[i+1]==0x2c) GPS_Data.latitude_h=0;
	  	else {
			GPS_Data.latitude_h=(GPSRxBuffer[i+1]-0x30)*10+GPSRxBuffer[i+2]-0x30;
			GPS_Data.latitude_m=(GPSRxBuffer[i+3]-0x30)*10+GPSRxBuffer[i+4]-0x30;
			GPS_Data.latitude_s=(GPSRxBuffer[i+6]-0x30)*10+GPSRxBuffer[i+7]-0x30;
			GPS_Data.latitude_ss=(GPSRxBuffer[i+8]-0x30)*10+GPSRxBuffer[i+9]-0x30;

			i=i+9;
		}
		uf=1;
	  }
	  else if(i1==4&&uf==0){	                                     //GPRMC γ��	�ϱ������ʾ
	  	if(GPSRxBuffer[i+1]==0x2c) GPS_Data.latitude_dir=0;
		else if(GPSRxBuffer[i+1]=='N') GPS_Data.latitude_dir=0;                    //��γ
		else if(GPSRxBuffer[i+1]=='S') GPS_Data.latitude_dir=1;                    //��γ
		i++;
		uf=1;
	  }
	  else if(i1==5&&uf==0){	                                     //GPRMC ����
	  	if(GPSRxBuffer[i+1]==0x2c) GPS_Data.longitude_h=0;
		else{
			GPS_Data.longitude_h=(GPSRxBuffer[i+1]-0x30)*100+(GPSRxBuffer[i+2]-0x30)*10+GPSRxBuffer[i+3]-0x30;
			GPS_Data.longitude_m=(GPSRxBuffer[i+4]-0x30)*10+(GPSRxBuffer[i+5]-0x30);
			GPS_Data.longitude_s=(GPSRxBuffer[i+7]-0x30)*10+(GPSRxBuffer[i+8]-0x30);
			GPS_Data.longitude_ss=(GPSRxBuffer[i+9]-0x30)*10+(GPSRxBuffer[i+10]-0x30);
			i=i+10;
		}
		uf=1;
	  }
	  else if(i1==6&&uf==0){	                                     //GPRMC ����	��������
	  	if(GPSRxBuffer[i+1]==0x2c) GPS_Data.longitude_dir=0;
		else if(GPSRxBuffer[i+1]=='E') GPS_Data.longitude_dir=0;                   //����
		else if(GPSRxBuffer[i+1]=='W') GPS_Data.longitude_dir=1;                   //����
		i++;
		uf=1;
	  }
	  else if(i1==7&&uf==0){	                                     //GPRMC �����ٶ�
	    GPS_Data.Speed=(GPSRxBuffer[i+1]-0x30)*100+(GPSRxBuffer[i+3]-0x30)*10+	   //������ٶ�ֵ����������
			GPSRxBuffer[i+4]-0x30;
			i=i+4;
	  	uf=1;
	  }
	  else if(i1==8&&uf==0){	                                     //GPRMC �ٶȷ���
	  	GPS_Data.Speed_dir=(GPSRxBuffer[i+1]-0x30)*100+(GPSRxBuffer[i+2]-0x30)*10+	   //������ٶ�ֵ����������
			GPSRxBuffer[i+3]-0x30;
			i=i+5;

	  	uf=1;
	  }
	  else if(i1==9&&uf==0){	                                     //GPRMC ����
	  	GPS_Data.Day=(GPSRxBuffer[i+1]-0x30)*10+GPSRxBuffer[i+2]-0x30;			 //��
   		GPS_Data.Mouth=(GPSRxBuffer[i+3]-0x30)*10+GPSRxBuffer[i+4]-0x30;		 //��
   		GPS_Data.Year=(GPSRxBuffer[i+5]-0x30)*10+GPSRxBuffer[i+6]-0x30;		 //��
		i=i+6;
		uf=1;
   	  }
   }
}
