#if !defined _TRANSMESSAGE_DEF_H_
#define _TRANSMESSAGE_DEF_H_

#define _PACKETHEAD_FINDFLAG_SIZE    4      //报文识别标识  4位
#define _PACKETHEAD_LEN_SIZE         4      //包的总长的长度
#define _PACKETHEAD_MSGTYPE_SIZE     1      //消息类型长度
#define _PACKETHEAD_AREARID_SIZE    10     //场站号长度
#define _PACKETHEAD_CHANNEL_SIZE     10     //通道号长度
#define _PACKETHEAD_IETYPE_SIZE      1      //ietype长度
#define _PACKETHEAD_IDFLAG_SIZE      4      //身份标识  用于区别监控，通常是0xff
#define _PACKETHEAD_LEN              34     //整个包头长度


#define _PACKET_XMLLEN_SIZE          4      //XML流长度的大小
#define _PACKETTAIL_LEN              2      //包尾长度

#define _PACKETHEAD_IDFLAG_POS       30     //身份标识的位置

#define _UDPSNDMAXSIZE				1024*3
typedef enum _LoginResualtErrType
{
	_LOGIN_RESUALT_OK	   =	0,			//	表示成功		
	_LOGIN_RESUALT_AREA_ERR=	1,			//表示场站错误
	_LOGIN_RESUALT_CHNL_ERR=	2,			//表示通道错误	
	_LOGIN_RESUALT_IE_ERR  =	3,			//表示进出卡口标志错误
	_LOGIN_RESUALT_IP_ERR  =	4,			//表示非认证IP地址
	_LOGIN_RESUALT_USER_ERR  =	5,			//表示用户名错误
	_LOGIN_RESUALT_PASS_ERR  =	6,			//表示密码错误
	_LOGIN_RESUALT_RIGHT_ERR  =	7,			//表示没有权限
	_LOGIN_RESUALT_NOINFO  =	8,			//表示无该通道信息
	_LOGIN_RESUALT_FORBIDDEN=	9,			//表示为该通道禁用
	_LOGIN_RESUALT_KEY_ERR=		10,			//密钥验证出错
	_LOGIN_RESUALT_SYS_ERR=		99			//表示系统错误
		
}LOGIN_RESUALT_ERR;
 
typedef enum _TransMessageType
{
	_MonitorLoginCmd=0x11,					//表示远程监控端登录指令
	_MonitorLoginResult=0x12,				//表示远程监控端登录结果
	_MonitorLoginOutCmd=0x13,				//表示为远程监控端注销指令

	_MonitorQueryChannelCmd=0x14,			//表示为查询可以监控的通道信息
	_MonitorGetChannelResult=0x15,          //表示远程监控端获取监控通道信息

	_MonitorSubChannelCmd=0x16,             //表示远程监控端提交监控点指令
	_MonitorUnSubChannelCmd=0x17,           //表示远程监控端撤消监控点指令
	_DockSubChannelCmd=0x18,				//表示码头平台代理提交监控点指令

	_ChannelGatherInfo=0x21,				//表示为采集数据传输
	_WLJKChannlRelCmd=0x22,					//表示为平台返回放行指令

	_MonitorManGatherInfo=0x23,				//表示为传送补采数据操作

	_MonitorSubGrossWT=0x24,                //车辆称重信息

	_MonitorCallPictCmd=0x25,	            //表示为取图像指令
	_ChannelRetPicInfo=0x26,                //表示为传送图像数据操作
	_ChannelSndPicInfo=0x27,				//表示为发送图像数据操作

	_WLJKChannlDeviceExcuteCmd=0x31,        //表示平台返回控制设备指令
	_ChannelDeviceExcuteBack=0x32,	        //表示通道控制设备执行结果

	_MonitorCtrlDeviceStatusCmd=0x33,       //表示卡口设备控制状态命令
	_ChannelDeviceControlFeedBack=0x34,	    //表示卡口设备控制状态命令执行结果反馈
   
	_ManCtrlCmd=0x35,						//人工处理指令
	_ExcuteManManCtrlCmdResualt=0x36,        //表示通道执行人工处理返回的结果

	_ChannelConnStatusInfo=0x41,            //表示通道连接状态信息

	_ChannelDeviceStatusInfo=0x42,          //表示通道设备状态信息

	_ChannelExceptionInfo=0x43,             //表示通道异常信息

	_MonitorCtrlChannelResetCmd=0x44,		//表示控制通道复位指令

	_MonitorCtrlChannelBeginCmd=0x45,		//表示控制通道开始工作

	_MonitorCtrlChannelPauseCmd=0x46,		//表示控制通道暂停工作
	
	_MonitorQueryRecordCmd=0x51,            //表示为查询采集历史记录指令
    _MonitorQueryRecordInfo=0x52,           //表示为查询记录的结果

	_ChannelQueryESealKeyCmd=0x53,          //查询电子关锁密钥指令
	_ChannelESealKeyInfo=0x54,              //电子关锁密钥

	_ChannelDownEsealInfo=0x56,				//电子关锁备案信息下载指令

	_ChannelRegCmd=0x61,					//表示为工控制机注册指令
	_ChannelRegResault=0x62,				//表示为工控机注册结果
	_ChannelUnRegCmd=0x63,					//表示为工控制机注销指令

	_ManErrDealInfo=0x64,                         //人工异常处理记录

	_ChannelHeartBeatCmd=0x71,				//表示通道到中心心的跳检测网络指令
	_MonitorHeartBeatCmd=0x72,				//表示监控端到中心的心跳网络检测指令

	_DockProxyLogoutCmd=0x73,					//表示码头平台代理注销指令

	//下面是扩展的定义
	_CheckChannelIsLoginOut=0xc1,            //表示检查通道是否掉线
	_ReLoginCmd	=	0xc2,					//表示通知客户端重新登陆指令
	_CheckIsHaveESealInfo=0xc3,				//表示检查通道是否有关锁信息更新

	_MonitorSubNatureCmd=0xa0,				//设备监控端提交监控数据集中监控中心性能请求
	_MonitorUnSubNamtureCmd=0xa1,			//设备监控端撤消监控数据集中监控中心性能请求
	_CenterNatureInfo=0xa2,					//集中监控中心性能数据
	_TestCenterActiveCmd=0xa3,				//数据集中监控中心活动测试指令
	_ResealtOfTestCenter=0xa4,				//数据集中监控中心活动测试结果
	_NatureAlarm=0xa5,						//性能报警
	_RemoteCtrlCmd=0xa6,					//控制指令
	_ResualtOfRemoteCtrl=0xa7				//控制结果
}TRANSMESSAGETYPE;

#endif