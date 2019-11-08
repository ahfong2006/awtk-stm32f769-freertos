/**
  ******************************************************************************
  * @file    gtxx.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   i2c��������������gt9157/gt911/gt5688оƬ
  ******************************************************************************
  */ 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "bsp_touch_gtxx.h"
#include "bsp_i2c_touch.h"
#include "sys.h"

// 5����GT9157��������
const uint8_t CTP_CFG_GT9157[] ={ 
	0x00,0x20,0x03,0xE0,0x01,0x05,0x3C,0x00,0x01,0x08,
	0x28,0x0C,0x50,0x32,0x03,0x05,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x17,0x19,0x1E,0x14,0x8B,0x2B,0x0D,
	0x33,0x35,0x0C,0x08,0x00,0x00,0x00,0x9A,0x03,0x11,
	0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x32,0x00,0x00,
	0x00,0x20,0x58,0x94,0xC5,0x02,0x00,0x00,0x00,0x04,
	0xB0,0x23,0x00,0x93,0x2B,0x00,0x7B,0x35,0x00,0x69,
	0x41,0x00,0x5B,0x4F,0x00,0x5B,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x02,0x04,0x06,0x08,0x0A,0x0C,0x0E,0x10,
	0x12,0x14,0x16,0x18,0x1A,0xFF,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x02,0x04,0x06,0x08,0x0A,0x0C,0x0F,
	0x10,0x12,0x13,0x16,0x18,0x1C,0x1D,0x1E,0x1F,0x20,
	0x21,0x22,0x24,0x26,0xFF,0xFF,0xFF,0xFF,0x00,0x00,
	0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
	0xFF,0xFF,0xFF,0xFF,0x48,0x01
};

// 7����GT911��������
const uint8_t CTP_CFG_GT911[] =  {
	0xFF,              // �Ĵ�����ַ��0x8047�����ܣ����ð汾��
	0x00,0x04,         // �Ĵ�����ַ��0x8048~0x8049�����ܣ�X�������ֵ����λ��ǰ
	0x58,0x02,         // �Ĵ�����ַ��0x804A~0x804B�����ܣ�Y�������ֵ����λ��ǰ
	0x05,              // �Ĵ�����ַ��0x804C�����ܣ����������������1~5��
	0x0D,              // �Ĵ�����ַ��0x804D�����ܣ�����INT������ʽ��XY���꽻��
	0x00,              // �Ĵ�����ַ��0x804E���üĴ�����������
	0x03,              // �Ĵ�����ַ��0x804F�����ܣ����»��ɿ�ȥ������
	0x0F,              // �Ĵ�����ַ��0x8050�����ܣ�ԭʼ���괰���˲�ֵ

	0x1E,              // �Ĵ�����ַ��0x8051�����ܣ���������������
	0x0F,              // �Ĵ�����ַ��0x8052�����ܣ���������ֵ��ϵ��Ϊ1,0-15��Ч��
	0x5A,              // �Ĵ�����ַ��0x8053�����ܣ����ϴ�������޵��е���ֵ
	0x32,              // �Ĵ�����ַ��0x8054�����ܣ����ϴ�������е��޵���ֵ
	0x03,              // �Ĵ�����ַ��0x8055�����ܣ����͹���ʱ��(0~15s)
	0x05,              // �Ĵ�����ַ��0x8056�����ܣ������ϱ���(����Ϊ5+N ms)
	0x00,0x00,         // �Ĵ�����ַ��0x8057-0x8058�����ܣ��üĴ�����������	
	0x00,0x00,         // �Ĵ�����ַ��0x8059-0x805A�����ܣ��üĴ�����������

	0x00,              // �Ĵ�����ַ��0x805B�����ܣ���4bit�ϱ߿�Ŀհ�������32Ϊϵ����
	                   //                           ��4bit�±߿�Ŀհ�������32Ϊϵ����
	0x00,              // �Ĵ�����ַ��0x805C�����ܣ���4bit��߿�Ŀհ�������32Ϊϵ���� 
	                   //                           ��4bit�ұ߿�Ŀհ�������32Ϊϵ����
	0x0F,              // �Ĵ�����ַ��0x805D�����ܣ������������̶ȣ����� x/16 Pitch��
                     //	��beta��ռ�ã���������Ч��
	0x18,              // �Ĵ�����ַ��0x805E�����ܣ�����1ϵ��
	0x1A,              // �Ĵ�����ַ��0x805F�����ܣ�����2ϵ��
	0x1E,              // �Ĵ�����ַ��0x8060�����ܣ�����3ϵ��
	0x14,              // �Ĵ�����ַ��0x8061�����ܣ����������
	0x8C,              // �Ĵ�����ַ��0x8062�����ܣ�A����������
	0x2E,              // �Ĵ�����ַ��0x8063�����ܣ�B����������
	0x0E,              // �Ĵ�����ַ��0x8064�����ܣ���4bit Sensor_Group_B_Number 
	                   //                           ��4bit Sensor_Group_A_Number
	
	0x20,              // �Ĵ�����ַ��0x8065�����ܣ�������A������Ƶ�ʱ�Ƶϵ�� 
	                   // GroupA_Frequence = ��Ƶϵ�� * ��Ƶ
	0x22,              // �Ĵ�����ַ��0x8066�����ܣ�������B������Ƶ�ʱ�Ƶϵ��
                     //	GroupA_Frequence = ��Ƶϵ�� * ��Ƶ
	0x70,0x11,         // �Ĵ�����ַ��0x8067-0x8068�����ܣ�������A��B�Ļ�Ƶ(1526HZ<��Ƶ<14600Hz)
	0x00,0x00,         // �Ĵ�����ַ��0x8069-0x806A�����ܣ��������������ź����ʱ����
	                   // ����usΪ��λ��,Reserved��beta��ռ�ã���������Ч��
	0x02,              // �Ĵ�����ַ��0x806B�����ܣ�A����������
	0x02,              // �Ĵ�����ַ��0x806C�����ܣ�A����������
	0x03,              // �Ĵ�����ַ��0x806D�����ܣ�A����������
	0x1D,              // �Ĵ�����ַ��0x806E�����ܣ�A����������
	
	0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,		// 0x806F ~ 0x8078
	0x00,              // �Ĵ�����ַ��0x8079
	0x23,              // �Ĵ�����ַ��0x807A����Ƶ��Χ�����Ƶ�� 
	0x53,							 // �Ĵ�����ַ��0x807B����Ƶ��Χ���յ�Ƶ��
	0x94,							 // �Ĵ�����ַ��0x807C�������������ȷ����������1-63��Ч
	0xC5,							 // �Ĵ�����ַ��0x807D��������ⳬʱʱ��
	0x02,							 // �Ĵ�����ַ��0x807E���б��и��ŵ�����
	0x07,							 // �Ĵ�����ַ��0x807F
	0x00,0x00,				 // �Ĵ�����ַ��0x8080 - 0x8081
	0x04,							 // �Ĵ�����ַ��0x8082����Ƶ�������Ƶ��1���ĵ��Ƶ������������A��B��
	0x6D,							 // �Ĵ�����ַ��0x8083
	0x26,							 // �Ĵ�����ַ��0x8084  ��Ƶ�������Ƶ��1���ĵ㱶Ƶϵ��
	0x00, 						 // �Ĵ�����ַ��0x8085  ��Ƶ�������Ƶ��2���ĵ��Ƶ(����A��B�ڴ˻����ϻ���) 
	0x5F,							 // �Ĵ�����ַ��0x8086  
	0x2D,							 // �Ĵ�����ַ��0x8087  ��Ƶ�������Ƶ��2���ĵ㱶Ƶϵ��
	0x00,							 // �Ĵ�����ַ��0x8088  ��Ƶ�������Ƶ��3���ĵ��Ƶ������������A��B�� 
	0x52,							 // �Ĵ�����ַ��0x8089
	0x36,							 // �Ĵ�����ַ��0x808A  ��Ƶ�������Ƶ��3���ĵ㱶Ƶϵ��
	0x00,							 // �Ĵ�����ַ��0x808B  ��Ƶ�������Ƶ��4���ĵ��Ƶ������������A��B��
	0x4B,							 // �Ĵ�����ַ��0x808C
	0x40,							 // �Ĵ�����ַ��0x808D  ��Ƶ�������Ƶ��4���ĵ㱶Ƶϵ��
	0x00,							 // �Ĵ�����ַ��0x808E  ��Ƶ�������Ƶ��5���ĵ��Ƶ������������A��B��
	0x43,							 // �Ĵ�����ַ��0x808F
	0x4C,							 // �Ĵ�����ַ��0x8090  ��Ƶ�������Ƶ��5���ĵ㱶Ƶϵ��
	0x00,0x43,         // �Ĵ�����ַ��0x8091 - 0x8092
	0x00,							 // �Ĵ�����ַ��0x8093  key 1:λ�� 0:�ް��� 
	0x00,							 // �Ĵ�����ַ��0x8094  key 1:λ�� 0:�ް��� 
	0x00,							 // �Ĵ�����ַ��0x8095  key 1:λ�� 0:�ް��� 
	0x00, 						 // �Ĵ�����ַ��0x8096  key 1:λ�� 0:�ް��� 
	0x00,							 // �Ĵ�����ַ��0x8097
	0x00,							 // �Ĵ�����ַ��0x8098
	0x00,							 // �Ĵ�����ַ��0x8099
	0x00,							 // �Ĵ�����ַ��0x809A
	0x00,							 // �Ĵ�����ַ��0x809B
	0x00,							 // �Ĵ�����ַ��0x809C
	0x00,0x00,0x00,0x00,// �Ĵ�����ַ��0x809D - 0x80A0
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,		// 0x80A1 - 0x80AA
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,              // 0x80AB - 0x80B2
	0x00,              // �Ĵ�����ַ��0x80B3
	0x00,		           // �Ĵ�����ַ��0x80B4
	0x00,0x00,         // �Ĵ�����ַ��0x80B5 - 0x80B6

	/******************************************************************************************
	*	�Ĵ�����ַ:	0x80B7~0X80C4
	*	����˵��  :	�޸ĸ�Ӧͨ����Ӧ��оƬͨ���ţ����Ըı䴥�����Ĵ�ֱɨ�跽��
	*******************************************************************************************/

	0x02,0x04,0x06,0x08,0x0A,0x0C,0x0E,0x10,0x12,0x14,      // ɨ�跽��� �� �� ��
	0x16,0x18,0x1A,0x1C,

	/******************************************************************************************/

	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,		  // �����üĴ�������������
	0x00,0x00,0x00,0x00,0x00,0x00,                    		  // �����üĴ�������������

	/*******************************************************************************************
	*	�Ĵ�����ַ:	0x80D5~0x80EE
	*	����˵��  :	�޸�����ͨ����Ӧ��оƬͨ���ţ����Ըı䴥������ˮƽɨ�跽��
	********************************************************************************************/

	0x00,0x02,0x04,0x06,0x08,0x0A,0x0C,0x0F,0x10,0x12,		  // ɨ�跽��� �� �� ��
	0x13,0x14,0x16,0x18,0x1C,0x1D,0x1E,0x1F,0x20,0x21,
	0x22,0x24,0x26,0x28,0x29,0x2A,

	/*******************************************************************************************/	

	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	    // ͨ������ϵ���Ĵ����������޸�
	0x00,0x00,0x00,0x00,0x00,0x00,                          // ͨ������ϵ���Ĵ����������޸�
};

// 4.5����GT5688��������

const uint8_t CTP_CFG_GT5688[] =  {
0x97,0xE0,0x01,0x10,0x01,0x05,0x0D,0x00,0x01,0x00,
0x00,0x05,0x50,0x3C,0x53,0x11,0x00,0x00,0x11,0x11,
0x14,0x14,0x14,0x22,0x0A,0x04,0x00,0x00,0x00,0x00,
0x00,0x00,0x53,0x00,0x14,0x00,0x00,0x84,0x00,0x00,
0x3C,0x00,0x00,0x64,0x1E,0x28,0x87,0x27,0x08,0x32,
0x34,0x05,0x0D,0x20,0x33,0x60,0x13,0x02,0x24,0x00,
0x00,0x64,0x80,0x80,0x14,0x02,0x00,0x00,0x54,0xC5,
0x26,0xA5,0x2E,0x90,0x36,0x80,0x3F,0x75,0x47,0x6C,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF0,0x50,0x3C,
0xFF,0xFF,0x07,0x00,0x00,0x00,0x02,0x14,0x14,0x03,
0x04,0x00,0x21,0x64,0x0A,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x32,0x20,0x50,0x3C,0x3C,0x00,0x00,0x00,0x00,0x00,
0x0D,0x06,0x0C,0x05,0x0B,0x04,0x0A,0x03,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0x00,0x01,0x02,0x03,0x04,0x05,
0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x3C,0x00,0x05,0x1E,0x00,0x02,
0x2A,0x1E,0x19,0x14,0x02,0x00,0x03,0x0A,0x05,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x01,0xFF,0xFF,0x86,
0x22,0x03,0x00,0x00,0x33,0x00,0x0F,0x00,0x00,0x00,
0x50,0x3C,0x50,0x00,0x00,0x00,0x2F,0xE1,0x01
};


//uint8_t config[GTP_CONFIG_MAX_LENGTH + GTP_ADDR_LENGTH]
//                = {GTP_REG_CONFIG_DATA >> 8, GTP_REG_CONFIG_DATA & 0xff};

/* ����IC����Ĭ��Ϊ5������ic */
TOUCH_IC touchIC = GT911;		

const TOUCH_PARAM_TypeDef touch_param[3] = 
{
  /* GT9157,5���� */
  {
    .max_width = 800,
    .max_height = 480,
    .config_reg_addr = 0x8047,
  },
  /* GT911,7���� */
  {
    .max_width = 800,
    .max_height = 480,
    .config_reg_addr = 0x8047,
  },
  /* GT5688,4.3���� */
  {
    .max_width = 480,
    .max_height = 272,
    .config_reg_addr = 0x8050,
  }
};

uint8_t Touch_Pres_Down = 0;
GT911_T g_GT911;
GT911_Point g_GT911_Point;

static int8_t GTP_I2C_Test(void);
//static void GT91xx_Config_Read_Proc(void);

//static void Delay(__IO uint32_t nCount)	 //�򵥵���ʱ����
//{
//	for(; nCount != 0; nCount--);
//}


/**
  * @brief   ʹ��IIC�������ݴ���
  * @param
  *		@arg i2c_msg:���ݴ���ṹ��
  *		@arg num:���ݴ���ṹ��ĸ���
  * @retval  ������ɵĴ���ṹ��������������������0xff
  */
static int I2C_Transfer( struct i2c_msg *msgs,int num)
{
	int im = 0;
	int ret = 0;

	for (im = 0; ret == 0 && im != num; im++)
	{
		if ((msgs[im].flags&I2C_M_RD))																//����flag�ж��Ƕ����ݻ���д����
		{
			ret = I2C_ReadBytes(msgs[im].addr, msgs[im].buf, msgs[im].len);		//IIC��ȡ����
		} else
		{
			ret = I2C_WriteBytes(msgs[im].addr,  msgs[im].buf, msgs[im].len);	//IICд������
		}
	}

	if(ret)
		return ret;

	return im;   													//������ɵĴ���ṹ����
}

/**
  * @brief   ��IIC�豸�ж�ȡ����
  * @param
  *		@arg client_addr:�豸��ַ
  *		@arg  buf[0~1]: ��ȡ���ݼĴ�������ʼ��ַ
  *		@arg buf[2~len-1]: �洢���������ݵĻ���buffer
  *		@arg len:    GTP_ADDR_LENGTH + read bytes count���Ĵ�����ַ����+��ȡ�������ֽ�����
  * @retval  i2c_msgs����ṹ��ĸ�����2Ϊ�ɹ�������Ϊʧ��
  */
static int32_t GTP_I2C_Read(uint8_t client_addr, uint8_t *buf, int32_t len)
{
    struct i2c_msg msgs[2];
    int32_t ret=-1;
    int32_t retries = 0;
    /*һ�������ݵĹ��̿��Է�Ϊ�����������:
     * 1. IIC  д�� Ҫ��ȡ�ļĴ�����ַ
     * 2. IIC  ��ȡ  ����
     * */

    msgs[0].flags = !I2C_M_RD;					//д��
    msgs[0].addr  = client_addr;					//IIC�豸��ַ
    msgs[0].len   = GTP_ADDR_LENGTH;	//�Ĵ�����ַΪ2�ֽ�(��д�����ֽڵ�����)
    msgs[0].buf   = &buf[0];						//buf[0~1]�洢����Ҫ��ȡ�ļĴ�����ַ
    
    msgs[1].flags = I2C_M_RD;					//��ȡ
    msgs[1].addr  = client_addr;					//IIC�豸��ַ
    msgs[1].len   = len - GTP_ADDR_LENGTH;	//Ҫ��ȡ�����ݳ���
    msgs[1].buf   = &buf[GTP_ADDR_LENGTH];	//buf[GTP_ADDR_LENGTH]֮��Ļ������洢����������

    while(retries < 5)
    {
        ret = I2C_Transfer( msgs, 2);					//����IIC���ݴ�����̺�������2���������
        if(ret == 2)break;
        retries++;
    }
    if((retries >= 5))
    {
      
    }
    return ret;
}



/**
  * @brief   ��IIC�豸д������
  * @param
  *		@arg client_addr:�豸��ַ
  *		@arg  buf[0~1]: Ҫд������ݼĴ�������ʼ��ַ
  *		@arg buf[2~len-1]: Ҫд�������
  *		@arg len:    GTP_ADDR_LENGTH + write bytes count���Ĵ�����ַ����+д��������ֽ�����
  * @retval  i2c_msgs����ṹ��ĸ�����1Ϊ�ɹ�������Ϊʧ��
  */
static int32_t GTP_I2C_Write(uint8_t client_addr,uint8_t *buf,int32_t len)
{
    struct i2c_msg msg;
    int32_t ret = -1;
    int32_t retries = 0;
    /*һ��д���ݵĹ���ֻ��Ҫһ���������:
     * 1. IIC���� д�� ���ݼĴ�����ַ������
     * */
    msg.flags = !I2C_M_RD;			//д��
    msg.addr  = client_addr;			//���豸��ַ
    msg.len   = len;							//����ֱ�ӵ���(�Ĵ�����ַ����+д��������ֽ���)
    msg.buf   = buf;						//ֱ������д�뻺�����е�����(�����˼Ĵ�����ַ)

    while(retries < 5)
    {
        ret = I2C_Transfer(&msg, 1);	//����IIC���ݴ�����̺�����1���������
        if (ret == 1)break;
        retries++;
    }
    if((retries >= 5))
    {

    }
    return ret;
}



/**
  * @brief   ʹ��IIC��ȡ�ٴ����ݣ������Ƿ�����
  * @param
  *		@arg client:�豸��ַ
  *		@arg  addr: �Ĵ�����ַ
  *		@arg rxbuf: �洢����������
  *		@arg len:    ��ȡ���ֽ���
  * @retval
  * 	@arg FAIL
  * 	@arg SUCCESS
  */
 int32_t GTP_I2C_Read_dbl_check(uint8_t client_addr, uint16_t addr, uint8_t *rxbuf, int len)
{
    uint8_t buf[16] = {0};
    uint8_t confirm_buf[16] = {0};
    uint8_t retry = 0;

    while (retry++ < 3)
    {
        memset(buf, 0xAA, 16);
        buf[0] = (uint8_t)(addr >> 8);
        buf[1] = (uint8_t)(addr & 0xFF);
        GTP_I2C_Read(client_addr, buf, len + 2);
        
        memset(confirm_buf, 0xAB, 16);
        confirm_buf[0] = (uint8_t)(addr >> 8);
        confirm_buf[1] = (uint8_t)(addr & 0xFF);
        GTP_I2C_Read(client_addr, confirm_buf, len + 2);

      
        if (!memcmp(buf, confirm_buf, len+2))
        {
            memcpy(rxbuf, confirm_buf+2, len);
            return SUCCESS;
        }
    }    
    return FAIL;
}


/**
  * @brief   �ر�GT91xx�ж�
  * @param ��
  * @retval ��
  */
void GTP_IRQ_Disable(void)
{
    I2C_GTP_IRQDisable();
}

/**
  * @brief   ʹ��GT91xx�ж�
  * @param ��
  * @retval ��
  */
void GTP_IRQ_Enable(void)
{
	I2C_GTP_IRQEnable();    
}


/**
  * @brief   ���ڴ���򱨸津����⵽����
  * @param
  *    @arg     id: ����˳��trackID
  *    @arg     x:  ������ x ����
  *    @arg     y:  ������ y ����
  *    @arg     w:  ������ ��С
  * @retval ��
  */
/*���ڼ�¼��������ʱ(����)����һ�δ���λ�ã�����ֵ��ʾ��һ���޴�������*/
//static int16_t pre_x[GTP_MAX_TOUCH] ={-1,-1,-1,-1,-1};
//static int16_t pre_y[GTP_MAX_TOUCH] ={-1,-1,-1,-1,-1};

static void GTP_Touch_Down(int32_t id,int32_t x,int32_t y,int32_t w)
{

}


/**
  * @brief   ���ڴ���򱨸津���ͷ�
  * @param �ͷŵ��id��
  * @retval ��
  */
static void GTP_Touch_Up( int32_t id)
{

}


/**
  * @brief   ��������������ѯ�����ڴ����жϵ���
  * @param ��
  * @retval ��
  */
static void Goodix_TS_Work_Func(void)
{
    uint8_t  end_cmd[3] = {GTP_READ_COOR_ADDR >> 8, GTP_READ_COOR_ADDR & 0xFF, 0};
    uint8_t  point_data[2 + 1 + 8 * GTP_MAX_TOUCH + 1]={GTP_READ_COOR_ADDR >> 8, GTP_READ_COOR_ADDR & 0xFF};
    uint8_t  touch_num = 0;
    uint8_t  finger = 0;
    static uint16_t pre_touch = 0;
    static uint8_t pre_id[GTP_MAX_TOUCH] = {0};

    uint8_t client_addr=GTP_ADDRESS;
    uint8_t* coor_data = NULL;
    int32_t input_x = 0;
    int32_t input_y = 0;
    int32_t input_w = 0;
    uint8_t id = 0;
 
    int32_t i  = 0;
    int32_t ret = -1;

    ret = GTP_I2C_Read(client_addr, point_data, 12);//10�ֽڼĴ�����2�ֽڵ�ַ
    if (ret < 0)
    {
        return;
    }
    
    finger = point_data[GTP_ADDR_LENGTH];//״̬�Ĵ�������

    if (finger == 0x00)		//û�����ݣ��˳�
    {
        return;
    }

    if((finger & 0x80) == 0)//�ж�buffer statusλ
    {
        goto exit_work_func;//����δ������������Ч
    }

    touch_num = finger & 0x0f;//�������
    if (touch_num > GTP_MAX_TOUCH)
    {
        goto exit_work_func;//�������֧�ֵ����������˳�
    }

    if (touch_num > 1)//��ֹһ����
    {
        uint8_t buf[8 * GTP_MAX_TOUCH] = {(GTP_READ_COOR_ADDR + 10) >> 8, (GTP_READ_COOR_ADDR + 10) & 0xff};

        ret = GTP_I2C_Read(client_addr, buf, 2 + 8 * (touch_num - 1));
        memcpy(&point_data[12], &buf[2], 8 * (touch_num - 1));			//����������������ݵ�point_data
    }

    
    
    if (pre_touch>touch_num)				//pre_touch>touch_num,��ʾ�еĵ��ͷ���
    {
        for (i = 0; i < pre_touch; i++)						//һ����һ���㴦��
         {
            uint8_t j;
           for(j=0; j<touch_num; j++)
           {
               coor_data = &point_data[j * 8 + 3];
               id = coor_data[0] & 0x0F;									//track id
              if(pre_id[i] == id)
                break;

              if(j >= touch_num-1)											//������ǰ����id���Ҳ���pre_id[i]����ʾ���ͷ�
              {
                 GTP_Touch_Up( pre_id[i]);
              }
           }
       }
    }


    if (touch_num)
    {
        for (i = 0; i < touch_num; i++)						//һ����һ���㴦��
        {
            coor_data = &point_data[i * 8 + 3];

            id = coor_data[0] & 0x0F;									//track id
            pre_id[i] = id;

            input_x  = coor_data[1] | (coor_data[2] << 8);	//x����
            input_y  = coor_data[3] | (coor_data[4] << 8);	//y����
            input_w  = coor_data[5] | (coor_data[6] << 8);	//size
        
            {
                GTP_Touch_Down( id, input_x, input_y, input_w);//���ݴ���
            }
        }
    }
    else if (pre_touch)		//touch_ num=0 ��pre_touch��=0
    {
      for(i=0;i<pre_touch;i++)
      {
          GTP_Touch_Up(pre_id[i]);
      }
    }


    pre_touch = touch_num;


exit_work_func:
    {
        ret = GTP_I2C_Write(client_addr, end_cmd, 3);
        if (ret < 0)
        {
					
        }
    }

}


/**
  * @brief   ������оƬ���¸�λ
  * @param ��
  * @retval ��
  */
int8_t GTP_Reset_Guitar(void)
{
#if 1
    I2C_ResetChip();
    return 0;
#else 		//�����λ
    int8_t ret = -1;
    int8_t retry = 0;
    uint8_t reset_command[3]={(uint8_t)GTP_REG_COMMAND>>8,(uint8_t)GTP_REG_COMMAND&0xFF,2};

    //д�븴λ����
    while(retry++ < 5)
    {
        ret = GTP_I2C_Write(GTP_ADDRESS, reset_command, 3);
        if (ret > 0)
        {
            return ret;
        }

    }
    return ret;
#endif

}



 /**
   * @brief   ����˯��ģʽ
   * @param ��
   * @retval 1Ϊ�ɹ�������Ϊʧ��
   */
//int8_t GTP_Enter_Sleep(void)
//{
//    int8_t ret = -1;
//    int8_t retry = 0;
//    uint8_t reset_comment[3] = {(uint8_t)(GTP_REG_COMMENT >> 8), (uint8_t)GTP_REG_COMMENT&0xFF, 5};//5
//
//    GTP_DEBUG_FUNC();
//
//    while(retry++ < 5)
//    {
//        ret = GTP_I2C_Write(GTP_ADDRESS, reset_comment, 3);
//        if (ret > 0)
//        {
//            GTP_INFO("GTP enter sleep!");
//
//            return ret;
//        }
//
//    }
//    GTP_ERROR("GTP send sleep cmd failed.");
//    return ret;
//}


int8_t GTP_Send_Command(uint8_t command)
{
    int8_t ret = -1;
    int8_t retry = 0;
    uint8_t command_buf[3] = {(uint8_t)(GTP_REG_COMMAND >> 8), (uint8_t)GTP_REG_COMMAND&0xFF, GTP_COMMAND_READSTATUS};

    while(retry++ < 5)
    {
        ret = GTP_I2C_Write(GTP_ADDRESS, command_buf, 3);
        if (ret > 0)
        {
            return ret;
        }

    }
    return ret;
}

/**
  * @brief   ���Ѵ�����
  * @param ��
  * @retval 0Ϊ�ɹ�������Ϊʧ��
  */
int8_t GTP_WakeUp_Sleep(void)
{
    uint8_t retry = 0;
    int8_t ret = -1;

    while(retry++ < 10)
    {
        ret = GTP_I2C_Test();
        if (ret > 0)
        {
            return ret;
        }
        GTP_Reset_Guitar();
    }
    return ret;
}

static int32_t GTP_Get_Info(void)
{
    uint8_t opr_buf[10] = {0};
    int32_t ret = 0;

//    uint16_t abs_x_max = GTP_MAX_WIDTH;
//    uint16_t abs_y_max = GTP_MAX_HEIGHT;
//    uint8_t int_trigger_type = GTP_INT_TRIGGER;
        
    opr_buf[0] = (uint8_t)((GTP_REG_CONFIG_DATA+1) >> 8);
    opr_buf[1] = (uint8_t)((GTP_REG_CONFIG_DATA+1) & 0xFF);
    
    ret = GTP_I2C_Read(GTP_ADDRESS, opr_buf, 10);
    if (ret < 0)
    {
        return FAIL;
    }
    
//    abs_x_max = (opr_buf[3] << 8) + opr_buf[2];
//    abs_y_max = (opr_buf[5] << 8) + opr_buf[4];

    opr_buf[0] = (uint8_t)((GTP_REG_CONFIG_DATA+6) >> 8);
    opr_buf[1] = (uint8_t)((GTP_REG_CONFIG_DATA+6) & 0xFF);
    
    ret = GTP_I2C_Read(GTP_ADDRESS, opr_buf, 3);
    if (ret < 0)
    {
        return FAIL;
    }
//    int_trigger_type = opr_buf[2] & 0x03;
    
    return SUCCESS;    
}

/*******************************************************
Function:
    Initialize gtp.
Input:
    ts: goodix private data
Output:
    Executive outcomes.
        0: succeed, otherwise: failed
*******************************************************/
 int32_t GTP_Init_Panel(void)
{
    int32_t ret = -1;

    int32_t i = 0;
    uint16_t check_sum = 0;
    int32_t retry = 0;

    const uint8_t* cfg_info;
    uint8_t cfg_info_len  ;
		uint8_t* config;

    uint8_t cfg_num =0 ;		//��Ҫ���õļĴ�������

    I2C_Touch_Init();

    ret = GTP_I2C_Test();
    if (ret < 0)
    {
				return ret;
    } 
		
		//��ȡ����IC���ͺ�
    GTP_Read_Version(); 

		config = (uint8_t *)malloc (GTP_CONFIG_MAX_LENGTH + GTP_ADDR_LENGTH);

		config[0] = GTP_REG_CONFIG_DATA >> 8;
		config[1] =  GTP_REG_CONFIG_DATA & 0xff;
		
		//����IC���ͺ�ָ��ͬ������
		if(touchIC == GT9157)
		{
			cfg_info =  CTP_CFG_GT9157; //ָ��Ĵ�������
			cfg_info_len = CFG_GROUP_LEN(CTP_CFG_GT9157);//�������ñ�Ĵ�С
		}
		else if(touchIC == GT911)
		{
			cfg_info =  CTP_CFG_GT911;//ָ��Ĵ�������
			cfg_info_len = CFG_GROUP_LEN(CTP_CFG_GT911) ;//�������ñ�Ĵ�С
		}
		else if(touchIC == GT5688)			
		{
			cfg_info =  CTP_CFG_GT5688; //ָ��Ĵ�������
			cfg_info_len = CFG_GROUP_LEN(CTP_CFG_GT5688);//�������ñ�Ĵ�С
		}
		
    memset(&config[GTP_ADDR_LENGTH], 0, GTP_CONFIG_MAX_LENGTH);
    memcpy(&config[GTP_ADDR_LENGTH], cfg_info, cfg_info_len);
		
		cfg_num = cfg_info_len;
		
		/*����LCD��ɨ�跽�����÷ֱ���*/
		config[GTP_ADDR_LENGTH+1] = LCD_PIXEL_WIDTH & 0xFF;
		config[GTP_ADDR_LENGTH+2] = LCD_PIXEL_WIDTH >> 8;
		config[GTP_ADDR_LENGTH+3] = LCD_PIXEL_HEIGHT & 0xFF;
		config[GTP_ADDR_LENGTH+4] = LCD_PIXEL_HEIGHT >> 8;
		
		/*����ģʽ����X2Y����*/

    //������
    //				config[GTP_ADDR_LENGTH+6] &= ~(X2Y_LOC);

    //����
    config[GTP_ADDR_LENGTH+6] |= (X2Y_LOC);

    //����Ҫд��checksum�Ĵ�����ֵ
    check_sum = 0;

    /* ����check sumУ��ֵ */
    if(touchIC == GT911 || touchIC == GT9157)
    {
        for (i = GTP_ADDR_LENGTH; i < cfg_num+GTP_ADDR_LENGTH; i++)
        {
            check_sum += (config[i] & 0xFF);
        }
        config[ cfg_num+GTP_ADDR_LENGTH] = (~(check_sum & 0xFF)) + 1; 	//checksum
        config[ cfg_num+GTP_ADDR_LENGTH+1] =  1; 						//refresh ���ø��±�־
    }
    else if(touchIC == GT5688) 
    {
      for (i = GTP_ADDR_LENGTH; i < (cfg_num+GTP_ADDR_LENGTH -3); i += 2) 
      {
        check_sum += (config[i] << 8) + config[i + 1];
      }
      
      check_sum = 0 - check_sum;
      //����checksum
      config[(cfg_num+GTP_ADDR_LENGTH -3)] = (check_sum >> 8) & 0xFF;
      config[(cfg_num+GTP_ADDR_LENGTH -2)] = check_sum & 0xFF;
      config[(cfg_num+GTP_ADDR_LENGTH -1)] = 0x01;
    }

    //д��������Ϣ
    for (retry = 0; retry < 5; retry++)
    {
        ret = GTP_I2C_Write(GTP_ADDRESS, config , cfg_num + GTP_ADDR_LENGTH+2);
        if (ret > 0)
        {
            break;
        }
    }
    //Delay(0xffffff);				//�ӳٵȴ�оƬ����
		HAL_Delay(50);

		
#if 1	//����д������ݣ�����Ƿ�����д��
    //���������������д����Ƿ���ͬ
	{
		uint16_t i;
		uint8_t buf[300];
		buf[0] = config[0];
		buf[1] =config[1];    //�Ĵ�����ַ

		ret = GTP_I2C_Read(GTP_ADDRESS, buf, sizeof(buf));

		//���ԱȰ汾��
		for(i=3;i<cfg_num+GTP_ADDR_LENGTH-3;i++)
		{

			if(config[i] != buf[i])
			{
				free(config);
				return -1;
			}
		}
		if(i==cfg_num+GTP_ADDR_LENGTH-3){}
}
#endif
	
		
	 /* emXGUIʾ���в�ʹ���ж� */
		I2C_GTP_IRQDisable();
	
    GTP_Get_Info();
		
		free(config);

    return 0;
}


/*******************************************************
Function:
    Read chip version.
Input:
    client:  i2c device
    version: buffer to keep ic firmware version
Output:
    read operation return.
        2: succeed, otherwise: failed
*******************************************************/
int32_t GTP_Read_Version(void)
{
	int32_t ret = -1;
	uint8_t buf[8] = {GTP_REG_VERSION >> 8, GTP_REG_VERSION & 0xff};    //�Ĵ�����ַ

	ret = GTP_I2C_Read(GTP_ADDRESS, buf, sizeof(buf));
	if (ret < 0)
	{
			return ret;
	}

	if (buf[2] == '9')
	{				
		//GT911оƬ
		if(buf[2] == '9' && buf[3] == '1' && buf[4] == '1')
		{
			touchIC = GT911;
		}
		//GT9157оƬ
		else if( buf[2] == '9' && buf[3] == '1' && buf[4] == '5' && buf[5] == '7')
		{
			touchIC = GT9157;
		}

	}    
	else if (buf[2] == '5')
	{	
		//GT5688оƬ
		if(buf[2] == '5' && buf[3] == '6' && buf[4] == '8' && buf[5] == '8')
		{
			touchIC = GT5688;
		}
	}
	return ret;
}

/*******************************************************
Function:
    I2c test Function.
Input:
    client:i2c client.
Output:
    Executive outcomes.
        2: succeed, otherwise failed.
*******************************************************/
static int8_t GTP_I2C_Test( void)
{
	uint8_t test[3] = {GTP_REG_CONFIG_DATA >> 8, GTP_REG_CONFIG_DATA & 0xff};
	uint8_t retry = 0;
	int8_t ret = -1;

	while(retry++ < 5)
	{
		ret = GTP_I2C_Read(GTP_ADDRESS, test, 3);
		if (ret > 0)
		{
				return ret;
		}
	}
	return ret;
}

//��⵽�����ж�ʱ���ã�
void GTP_TouchProcess(void)
{
  Goodix_TS_Work_Func();
}

#if 0//û�е��Ĳ��Ժ���
/*******************************************************
Function:
    Request gpio(INT & RST) ports.
Input:
    ts: private data.
Output:
    Executive outcomes.
        >= 0: succeed, < 0: failed
*******************************************************/
static int8_t GTP_Request_IO_Port(struct goodix_ts_data *ts)
{
}

/*******************************************************
Function:
    Request interrupt.
Input:
    ts: private data.
Output:
    Executive outcomes.
        0: succeed, -1: failed.
*******************************************************/
static int8_t GTP_Request_IRQ(struct goodix_ts_data *ts)
{
}

//���Ҫ��ʼ�������ݼ�оƬ�е���ʵ����
static void GT91xx_Config_Read_Proc(void)
{
    char temp_data[GTP_CONFIG_MAX_LENGTH + 2] = {0x80, 0x47};
    int i;

    GTP_INFO("==== GT9XX config init value====\n");

    for (i = 0 ; i < GTP_CONFIG_MAX_LENGTH ; i++)
    {
        printf("reg0x%x = 0x%02X ", i+0x8047, config[i + 2]);

        if (i % 10 == 9)
            printf("\n");
    }


    GTP_INFO("==== GT9XX config real value====\n");
    GTP_I2C_Read(GTP_ADDRESS, (uint8_t *)temp_data, GTP_CONFIG_MAX_LENGTH + 2);
    for (i = 0 ; i < GTP_CONFIG_MAX_LENGTH ; i++)
    {
        printf("reg0x%x = 0x%02X ", i+0x8047,temp_data[i+2]);

        if (i % 10 == 9)
            printf("\n");
    }

}

//��оƬд����������
static int32_t GT91xx_Config_Write_Proc(void)
{
    int32_t ret = -1;

    int32_t i = 0;
    uint8_t check_sum = 0;
    int32_t retry = 0;
    uint8_t cfg_num =0x80FE-0x8047+1 ;		//��Ҫ���õļĴ�������

    uint8_t cfg_info[] = CTP_CFG_GROUP1;
    uint8_t cfg_info_len =CFG_GROUP_LEN(cfg_info) ;

    GTP_INFO("==== GT9XX send config====\n");

    memset(&config[GTP_ADDR_LENGTH], 0, GTP_CONFIG_MAX_LENGTH);
    memcpy(&config[GTP_ADDR_LENGTH], cfg_info,cfg_info_len);

    //����Ҫд��checksum�Ĵ�����ֵ
    check_sum = 0;
    for (i = GTP_ADDR_LENGTH; i < cfg_num+GTP_ADDR_LENGTH; i++)
    {
        check_sum += config[i];
    }
    config[ cfg_num+GTP_ADDR_LENGTH] = (~check_sum) + 1; 	//checksum
    config[ cfg_num+GTP_ADDR_LENGTH+1] =  1; 						//refresh ���ø��±�־

    //д��������Ϣ
    for (retry = 0; retry < 5; retry++)
    {
        ret = GTP_I2C_Write(GTP_ADDRESS, config , cfg_num + GTP_ADDR_LENGTH+2);
        if (ret > 0)
        {
            break;
        }
    }


    return ret;
}

#endif

/**
  * @brief  �����жϷ�������emXGUIʾ����û��ʹ���ж�
  * @param ��
  * @retval ��
  */
void GTP_IRQHandler(void)
{
	if(__HAL_GPIO_EXTI_GET_IT(GTP_INT_GPIO_PIN) != RESET) //ȷ���Ƿ������EXTI Line�ж�
	{
		//LED2_TOGGLE;
		GTP_TouchProcess();    
		__HAL_GPIO_EXTI_CLEAR_IT(GTP_INT_GPIO_PIN);     //����жϱ�־λ
	}  
}

/**
  * @brief  ������⺯������������ΪemXGUI�Ķ��Ƽ�⺯����
   *        �ο�Goodix_TS_Work_Func�޸Ķ����� ֻ��ȡ��������������
  * @param x[out] y[out] ��ȡ��������
  * @retval ������Ч����1�����򷵻�0
  */
int	GTP_Execu( int *x,int *y)
{
	uint8_t  end_cmd[3] = {GTP_READ_COOR_ADDR >> 8, GTP_READ_COOR_ADDR & 0xFF, 0};
	//2-�Ĵ�����ַ 1-״̬�Ĵ��� 8*1-ÿ��������ʹ��8���Ĵ��� 
	uint8_t  point_data[2 + 1 + 8 * 1 + 1]={GTP_READ_COOR_ADDR >> 8, GTP_READ_COOR_ADDR & 0xFF};
	uint8_t  touch_num = 0;
	uint8_t  finger = 0;

	uint8_t client_addr=GTP_ADDRESS;
	int32_t input_x = 0;
	int32_t input_y = 0;

	int32_t ret = -1;

	ret = GTP_I2C_Read(client_addr, point_data, 12);//10�ֽڼĴ�����2�ֽڵ�ַ
	if (ret < 0)
	{
			return 0;
	}

	finger = point_data[GTP_ADDR_LENGTH];//״̬�Ĵ�������

	if (finger == 0x00)		//û�����ݣ��˳�
	{
			return 0;
	}

	if((finger & 0x80) == 0)//�ж�buffer statusλ
	{
			goto exit_work_func;//����δ������������Ч
	}

	touch_num = finger & 0x0f;//�������
	if (touch_num > GTP_MAX_TOUCH)
	{
			goto exit_work_func;//�������֧�ֵ����������˳�
	}    

	if (touch_num)
	{
		input_x  = point_data[3+1] | (point_data[3+2] << 8);	//x����
		input_y  = point_data[3+3] | (point_data[3+4] << 8);	//y����

		if(input_x < GTP_MAX_WIDTH && input_y < GTP_MAX_HEIGHT)  
		{
			*x = input_x;
			*y = input_y;
			Touch_Pres_Down = 1;
		}
		else
		{
				//������Χ�������˳�
			 Touch_Pres_Down = 0;
			 goto exit_work_func;
		}
	}

exit_work_func:
	{
			
		  //��ձ�־
			ret = GTP_I2C_Write(client_addr, end_cmd, 3);
			if (ret < 0)
			{
					return 0;
			}
	}
	return touch_num;
}

/*
*********************************************************************************************************
*	�� �� ��: GT911_OnePiontScan
*	����˵��: ��ȡGT911�������ݣ��������ȡһ�������㡣
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void GT911_OnePiontScan(void)
{
	static uint8_t s_tp_down = 0;
	uint16_t x, y;
	uint8_t res;
	static uint32_t count = 0;
	uint8_t  point_data[2 + 1 + 8 * 1 + 1]={GTP_READ_COOR_ADDR >> 8, GTP_READ_COOR_ADDR & 0xFF};
	uint8_t  end_cmd[3] = {GTP_READ_COOR_ADDR >> 8, GTP_READ_COOR_ADDR & 0xFF, 0};
	
	res = GTP_I2C_Read(GTP_ADDRESS, point_data, 12);//10�ֽڼĴ�����2�ֽڵ�ַ
	//GTP_I2C_Write(GTP_ADDRESS, end_cmd, 3);
	if(res != 2)
	{
			return;
	}
	/* �ж��Ƿ��£�û�а��£�ֱ���˳� */
	if((point_data[GTP_ADDR_LENGTH] & 0x0F) == 0)
	{
		GTP_I2C_Write(GTP_ADDRESS, end_cmd, 3);
		
		if(s_tp_down == 1)
		{
			count++;
			/* State.x��State.y����ֵ������£�State��ȫ�ֱ���������ľ������һ�ε���ֵ */
			/* �����⵽2�����ϲſ������� */
			if(count > 2)
			{   
				s_tp_down = 0;
				count = 0;
				g_GT911_Point.x = 0xFFFF;
		    g_GT911_Point.y = 0XFFFF;
				g_GT911_Point.Pressed = 0;
			}
		}
		return;
	}
	if((point_data[GTP_ADDR_LENGTH] & 0x80) && ((point_data[GTP_ADDR_LENGTH] & 0x0F) < 6))
	{
		GTP_I2C_Write(GTP_ADDRESS, end_cmd, 3);
		
		g_GT911.X0  = point_data[3+1] | (point_data[3+2] << 8);	//x����
		g_GT911.Y0  = point_data[3+3] | (point_data[3+4] << 8);	//y����

		/* ��ⰴ�� */
		/* ����ת�� :
			���ݴ��������½��� (0��0);  ���Ͻ��� (479��799)
			��Ҫת��LCD���������� (���Ͻ��� (0��0), ���½��� (799��479)
		*/
		x = g_GT911.X0;
		y = g_GT911.Y0;
		
		if (x > 799)
		{
			x = 799;
		}
		
		if (y > 479)
		{
			y = 479;
		}
		
		if(s_tp_down == 0)
		{
			s_tp_down = 1;
			g_GT911_Point.x = x;
			g_GT911_Point.y = y;
			g_GT911_Point.Pressed = 1;
		}
		else
		{
			g_GT911_Point.x = x;
			g_GT911_Point.y = y;
			g_GT911_Point.Pressed = 1;
		}
  }
	/* ʵʱ���� */
	count = 0;	
}

//MODULE_DESCRIPTION("GTP Series Driver");
//MODULE_LICENSE("GPL");
