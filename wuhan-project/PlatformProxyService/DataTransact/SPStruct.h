#ifndef SP_STRUCT_H_HH
#define SP_STRUCT_H_HH

//总署新协议数据采集包
struct _CHNL_GATHER_DATA
{
	char		szAREA_ID[11];			//场站代码
	char		szCHNL_NO[11];			//通道号
	BYTE		byI_E_TYPE;				//进出场标志
	char		szSEQ_NO[21];			//序列号
	BYTE        byDataReco;
	char        szDR_IC_NO[11];         //驾驶员IC卡号
	char		szIC_DR_CUSTOMS_NO[11];	//司机信息（不能为空）
	char		szIC_CO_CUSTOMS_NO[11];	//车辆海关编号
	char        szIC_BILL_NO[200];       //IC卡记载单证号
	BYTE		byIC_FORM_TYPE;			 //单证类型 
	char		szIC_REG_DATETIME[20];	//IC卡备案时间
	int			nIC_PER_DAY_DUE;		//有效时间（天数）
	float       IC_GROSS_WT ;           //卡内重量（标签必须有，但是值可以为空）
	char        szIC_VE_CUSTOMS_NO[11]; //车辆海关编号
	char        szIC_VE_NAME[33];       //卡内车牌号（标签必须有，但是值可以为空）

	char        szIC_CONTA_ID[50];		//卡内集装箱号
	char        szIC_CONTA_ID_F[17];      //卡内集装箱号
	char        szIC_CONTA_ID_B[17]; 
	char        szIC_ESEAL_ID[121];     //卡内关锁号（标签必须有，但是值可以为空）
	char		szIC_BUSS_TYPE[10 + 1]; // 业务类型
	char        szIC_EX_DATA[200 + 1];  // IC卡扩展数据
	char        szBAR_CODE[50 + 1];

	float		GROSS_WT;				//称重
	//   把车牌名33-70，车牌ID号17-70，
	char		szVE_NAME[70];			// 运输工具名称（车牌号）
	char 		szCAR_EC_NO[70];		// 电子车牌号
	char		szCAR_EC_NO2[17];		//电子车牌号

	char        szVE_CUSTOMS_NO[11];        //海关车辆编号（值可以为空）
	float		VE_WT;					//车自重

	char        szTR_EC_NO[50 + 1];		// 拖车电子车牌号
	char		szTR_NAME[50 + 1];			// 拖车号
	float       fTR_WT;					// 重量

	char        szVE_TYPE[10];			// 电子车牌车辆类型
	//char       szIC_WARE_TYPE[10];		// IC卡货物类型
	char		szIDENTIFY_VE_NAME[20 + 1]; // 光学图像识别出的车牌号


	int			nCONTA_NUM;				//箱数
	BYTE		byCONTA_RECO;			//箱号识别正确标示
	char		szCONTA_ID_F[17];		//前箱号
	char		szCONTA_ID_B[17];		//后箱号
	char		szCONTA_MODEL_F[10];	//前箱箱型
	char		szCONTA_MODEL_B[10];	//后箱箱型
	char		szOrder_Num[50];		//预约号
	BYTE		byDamageLeave_F;			//残损程度
	BYTE		byDamageLeave_B;			//残损程度
	char		szESEAL_ID[121];		//关锁号
	char        szESEAL_IC_NO[10 + 1];  // 关锁号对应IC卡号

	char		szESEAL_KEY[21];		//关锁操作密钥
	char		szGPS_ID[21];			//GPS ID号
	BYTE		byMODI_FLAG;			//人工修改标志
	char		szI_E_TIME[21];			//进出场时间
	char		byIC_RW_FLAG;			//是否写IC卡标志
	char		szEXC_CMD[50];			//执行指令
	char		szEXC_RESULT[30];		//各设备的放行指令执行结果
	char		szCHK_RESULT[50];		//采集数据比对反馈结果
	char		byREL_TYPE;				//放行方式

	char        szUSER_ID[21];          //操作员代号

	char 			pPicB[100];   //后-抓拍图像
	char 			pPicF[100];   //前-抓拍图像
	char 			pPicRB[100];  //右后-抓拍图像
	char 			pPicRF[100];  //右前-抓拍图像
	char 			pPicLB[100];  //左后-抓拍图像
	char 			pPicLF[100];  //左前-抓拍图像
	char			pVehiPicF[100]; //前车牌图像
	char			pVehiPicB[100]; //后车牌图像

};

struct _structCommandInfo
{
	char		AREA_ID[10 + 1];			// 场站代码
	char		CHNL_NO[10 + 1];			// 通道号
	BYTE		I_E_TYPE;					// 进出场标志
	char		SEQ_NO[20 + 1];				// 序列号
	char		CHECK_RESULT[20 + 1];		// 返回指令
	char		VE_NAME[70 + 1];			// 海关车辆编号
	char		GPS_ID[20 + 1];				// GPS ID号
	char		ORIGIN_CUSTOMS[20 + 1];		// 
	char		DEST_CUSTOMS[20 + 1];		// 
	char		ESEAL_ID[50 + 1];			// 
	char		SEAL_KEY[100 + 1];			// 
	char		FORM_ID[50 + 1];			// 
	char		OP_HINT[100 + 1];			// 
};


#endif