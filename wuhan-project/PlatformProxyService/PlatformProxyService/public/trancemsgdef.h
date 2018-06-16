#if !defined _TRANSMESSAGE_DEF_H_
#define _TRANSMESSAGE_DEF_H_

#define _PACKETHEAD_FINDFLAG_SIZE    4      //����ʶ���ʶ  4λ
#define _PACKETHEAD_LEN_SIZE         4      //�����ܳ��ĳ���
#define _PACKETHEAD_MSGTYPE_SIZE     1      //��Ϣ���ͳ���
#define _PACKETHEAD_AREARID_SIZE    10     //��վ�ų���
#define _PACKETHEAD_CHANNEL_SIZE     10     //ͨ���ų���
#define _PACKETHEAD_IETYPE_SIZE      1      //ietype����
#define _PACKETHEAD_IDFLAG_SIZE      4      //��ݱ�ʶ  ���������أ�ͨ����0xff
#define _PACKETHEAD_LEN              34     //������ͷ����


#define _PACKET_XMLLEN_SIZE          4      //XML�����ȵĴ�С
#define _PACKETTAIL_LEN              2      //��β����

#define _PACKETHEAD_IDFLAG_POS       30     //��ݱ�ʶ��λ��

#define _UDPSNDMAXSIZE				1024*3
typedef enum _LoginResualtErrType
{
	_LOGIN_RESUALT_OK	   =	0,			//	��ʾ�ɹ�		
	_LOGIN_RESUALT_AREA_ERR=	1,			//��ʾ��վ����
	_LOGIN_RESUALT_CHNL_ERR=	2,			//��ʾͨ������	
	_LOGIN_RESUALT_IE_ERR  =	3,			//��ʾ�������ڱ�־����
	_LOGIN_RESUALT_IP_ERR  =	4,			//��ʾ����֤IP��ַ
	_LOGIN_RESUALT_USER_ERR  =	5,			//��ʾ�û�������
	_LOGIN_RESUALT_PASS_ERR  =	6,			//��ʾ�������
	_LOGIN_RESUALT_RIGHT_ERR  =	7,			//��ʾû��Ȩ��
	_LOGIN_RESUALT_NOINFO  =	8,			//��ʾ�޸�ͨ����Ϣ
	_LOGIN_RESUALT_FORBIDDEN=	9,			//��ʾΪ��ͨ������
	_LOGIN_RESUALT_KEY_ERR=		10,			//��Կ��֤����
	_LOGIN_RESUALT_SYS_ERR=		99			//��ʾϵͳ����
		
}LOGIN_RESUALT_ERR;
 
typedef enum _TransMessageType
{
	_MonitorLoginCmd=0x11,					//��ʾԶ�̼�ض˵�¼ָ��
	_MonitorLoginResult=0x12,				//��ʾԶ�̼�ض˵�¼���
	_MonitorLoginOutCmd=0x13,				//��ʾΪԶ�̼�ض�ע��ָ��

	_MonitorQueryChannelCmd=0x14,			//��ʾΪ��ѯ���Լ�ص�ͨ����Ϣ
	_MonitorGetChannelResult=0x15,          //��ʾԶ�̼�ض˻�ȡ���ͨ����Ϣ

	_MonitorSubChannelCmd=0x16,             //��ʾԶ�̼�ض��ύ��ص�ָ��
	_MonitorUnSubChannelCmd=0x17,           //��ʾԶ�̼�ض˳�����ص�ָ��
	_DockSubChannelCmd=0x18,				//��ʾ��ͷƽ̨�����ύ��ص�ָ��

	_ChannelGatherInfo=0x21,				//��ʾΪ�ɼ����ݴ���
	_WLJKChannlRelCmd=0x22,					//��ʾΪƽ̨���ط���ָ��

	_MonitorManGatherInfo=0x23,				//��ʾΪ���Ͳ������ݲ���

	_MonitorSubGrossWT=0x24,                //����������Ϣ

	_MonitorCallPictCmd=0x25,	            //��ʾΪȡͼ��ָ��
	_ChannelRetPicInfo=0x26,                //��ʾΪ����ͼ�����ݲ���
	_ChannelSndPicInfo=0x27,				//��ʾΪ����ͼ�����ݲ���

	_WLJKChannlDeviceExcuteCmd=0x31,        //��ʾƽ̨���ؿ����豸ָ��
	_ChannelDeviceExcuteBack=0x32,	        //��ʾͨ�������豸ִ�н��

	_MonitorCtrlDeviceStatusCmd=0x33,       //��ʾ�����豸����״̬����
	_ChannelDeviceControlFeedBack=0x34,	    //��ʾ�����豸����״̬����ִ�н������
   
	_ManCtrlCmd=0x35,						//�˹�����ָ��
	_ExcuteManManCtrlCmdResualt=0x36,        //��ʾͨ��ִ���˹������صĽ��

	_ChannelConnStatusInfo=0x41,            //��ʾͨ������״̬��Ϣ

	_ChannelDeviceStatusInfo=0x42,          //��ʾͨ���豸״̬��Ϣ

	_ChannelExceptionInfo=0x43,             //��ʾͨ���쳣��Ϣ

	_MonitorCtrlChannelResetCmd=0x44,		//��ʾ����ͨ����λָ��

	_MonitorCtrlChannelBeginCmd=0x45,		//��ʾ����ͨ����ʼ����

	_MonitorCtrlChannelPauseCmd=0x46,		//��ʾ����ͨ����ͣ����
	
	_MonitorQueryRecordCmd=0x51,            //��ʾΪ��ѯ�ɼ���ʷ��¼ָ��
    _MonitorQueryRecordInfo=0x52,           //��ʾΪ��ѯ��¼�Ľ��

	_ChannelQueryESealKeyCmd=0x53,          //��ѯ���ӹ�����Կָ��
	_ChannelESealKeyInfo=0x54,              //���ӹ�����Կ

	_ChannelDownEsealInfo=0x56,				//���ӹ���������Ϣ����ָ��

	_ChannelRegCmd=0x61,					//��ʾΪ�����ƻ�ע��ָ��
	_ChannelRegResault=0x62,				//��ʾΪ���ػ�ע����
	_ChannelUnRegCmd=0x63,					//��ʾΪ�����ƻ�ע��ָ��

	_ManErrDealInfo=0x64,                         //�˹��쳣�����¼

	_ChannelHeartBeatCmd=0x71,				//��ʾͨ���������ĵ����������ָ��
	_MonitorHeartBeatCmd=0x72,				//��ʾ��ض˵����ĵ�����������ָ��

	_DockProxyLogoutCmd=0x73,					//��ʾ��ͷƽ̨����ע��ָ��

	//��������չ�Ķ���
	_CheckChannelIsLoginOut=0xc1,            //��ʾ���ͨ���Ƿ����
	_ReLoginCmd	=	0xc2,					//��ʾ֪ͨ�ͻ������µ�½ָ��
	_CheckIsHaveESealInfo=0xc3,				//��ʾ���ͨ���Ƿ��й�����Ϣ����

	_MonitorSubNatureCmd=0xa0,				//�豸��ض��ύ������ݼ��м��������������
	_MonitorUnSubNamtureCmd=0xa1,			//�豸��ض˳���������ݼ��м��������������
	_CenterNatureInfo=0xa2,					//���м��������������
	_TestCenterActiveCmd=0xa3,				//���ݼ��м�����Ļ����ָ��
	_ResealtOfTestCenter=0xa4,				//���ݼ��м�����Ļ���Խ��
	_NatureAlarm=0xa5,						//���ܱ���
	_RemoteCtrlCmd=0xa6,					//����ָ��
	_ResualtOfRemoteCtrl=0xa7				//���ƽ��
}TRANSMESSAGETYPE;

#endif