#ifndef STRUCTDEF_H
#define STRUCTDEF_H
#include <string>
#include <vector>

// 报文头内容定义
struct structMESSAGE_HEAD
{
	char MESSAGE_ID[20 + 1];	// 报文编号
	char FUNCTION_CODE[2 + 1];	// 功能代码 2:增加, 3:删除
	char MESSAGE_TYPE[10 + 1];	// 报文类型
	char SENDER[20 + 1];		// 发送者
	char RECEIVE[20 + 1];		// 接受者
	char MESSAGE_CREATE_TIME[30 + 1];	// 创建时间
	char VERSION[10 + 1];		// 版本号
};

// 报文体内容定义
struct structBILL_INFO
{
	char CUSTOMS_CODE[4 + 1];	// 海关代码
	char I_E_FLAG[1 + 1];		// 进/出口
	char SHIP_NAME_EN[255 + 1];	// 运输工具名称
	char VOYAGE_NO[36 + 1];		// 航次/班
	char NOTE[255 + 1];			// 备注
};


struct structCONTAINER_INFO
{
	char CONTA_NO[11 + 1];		// 集装箱号
	char CONTA_MODEL[4 + 1];	// 规格类型
	char CONTA_SIZE[4 + 1];		// 箱尺寸
	char SEAL_NO[30 + 1];		// 封志号
	char NOTE[255 + 1];			// 备注
};

struct structRESPOND_INFO
{
	char MESSAGE_ID[20 + 1];	// 报文编号
	char OP_FLAG[1 + 1];		// 入库标志 T:成功, F:失败
	char NOTE[255 + 1];			// 备注
};

//
struct structBILL_INFO_CLM10011
{
	std::string CUSTOMS_CODE;	// 海关代码
	std::string I_E_FLAG;		// 进/出口
	std::string SHIP_NAME_EN;	// 运输工具名称
	std::string VOYAGE_NO;		// 航次/班
	std::string BILL_NO;		// 提运单号
	std::string NOTE;			// 备注
};

struct structCONTAINER_INFO_CLM10011
{
	char CONTA_NO[11 + 1];		// 集装箱号
	char CONTA_MODEL[4 + 1];	// 规格类型
	char CONTA_SIZE[4 + 1];		// 箱尺寸
	char NOTE[255 + 1];			// 备注
};

struct structBILL_INFO_CLM10021
{
	char CUSTOMS_CODE[4 + 1];	// 海关代码
	char I_E_FLAG[1 + 1];		// 进/出口
	char SHIP_NAME_EN[255 + 1];	// 运输工具名称
	char VOYAGE_NO[36 + 1];		// 航次/班
	char BILL_NO[36 + 1];		// 提运单号
	char AGENT_NAME[255 + 1];
	char ENTRY_ID[18 + 1];
	char NOTE[255 + 1];			// 备注
};

struct structCONTAINER_INFO_CLM10021
{
	char CONTA_NO[11 + 1];		// 集装箱号
	char EXAM_TYPE[2 + 1];		// 
	char NOTE[255 + 1];			// 备注
};

struct structBILL_INFO_CLM10031
{
	char CUSTOMS_CODE[4 + 1];	// 海关代码
	char I_E_FLAG[1 + 1];		// 进/出口
	char SHIP_NAME_EN[255 + 1];	// 运输工具名称
	char VOYAGE_NO[36 + 1];		// 航次/班
	char BILL_NO[36 + 1];		// 提运单号
	char NOTE[255 + 1];			// 备注
};

struct structCONTAINER_INFO_CLM10031
{
	char CONTA_NO[11 + 1];		// 集装箱号
	char CONTA_MODEL[4 + 1];	// 规格类型
	char CONTA_SIZE[4 + 1];		// 箱尺寸
	char NOTE[255 + 1];			// 备注
};


struct structBILL_INFO_CLM10041
{
	char CUSTOMS_CODE[4 + 1];	// 海关代码
	char I_E_FLAG[1 + 1];		// 进/出口
	char SHIP_NAME_EN[255 + 1];	// 运输工具名称
	char VOYAGE_NO[36 + 1];		// 航次/班
	char NOTE[255 + 1];			// 备注
};

struct structCONTAINER_INFO_CLM10041
{
	char CONTA_NO[11 + 1];		// 集装箱号
	char CONTA_MODEL[4 + 1];	// 规格类型
	char CONTA_SIZE[4 + 1];		// 箱尺寸
	char SEAL_NO[30 + 1];
	char NOTE[255 + 1];			// 备注
};


struct structBILL_INFO_CLM10051
{
	char CUSTOMS_CODE[4 + 1];	// 海关代码
	char I_E_FLAG[1 + 1];		// 进/出口
	char SHIP_NAME_EN[255 + 1];	// 运输工具名称
	char VOYAGE_NO[36 + 1];		// 航次/班
	char NOTE[255 + 1];			// 备注
};

struct structCONTAINER_INFO_CLM10051
{
	char CONTA_NO[11 + 1];		// 集装箱号
	char CONTA_MODEL[4 + 1];	// 规格类型
	char CONTA_SIZE[4 + 1];		// 箱尺寸
	char NOTE[255 + 1];			// 备注
};


struct structBILL_INFO_CLM10061
{
	char CUSTOMS_CODE[4 + 1];	// 海关代码
	char I_E_FLAG[1 + 1];		// 进/出口
	char SHIP_NAME_EN[255 + 1];	// 运输工具名称
	char VOYAGE_NO[36 + 1];		// 航次/班
	char NOTE[255 + 1];			// 备注
};

struct structCONTAINER_INFO_CLM10061
{
	char CONTA_NO[11 + 1];		// 集装箱号
	char CONTA_MODEL[4 + 1];	// 规格类型
	char CONTA_SIZE[4 + 1];		// 箱尺寸
	char NOTE[255 + 1];			// 备注
};



struct structSHIP_INFO_CLM20011
{
	char CUSTOMS_CODE[4 + 1];	// 海关代码
	char I_E_FLAG[1 + 1];		// 进/出口
	char NATIVE_SHIP_NAME_EN[255 + 1];	// 运输工具名称
	char NATIVE_SHIP_NAME_CN[255 + 1];	// 运输工具名称
	char NATIVE_VOYAGE_NO[36 + 1];		// 航次/班
	char START_CUSTOMS_CODE[4 + 1];		// 
	char END_CUSTOMS_CODE[4 + 1];		// 
	char IMO[255 + 1];
	char SHIP_ID[16 + 1];
	char NOTE[255 + 1];			// 备注
};

struct structCONTAINER_INFO_CLM20011
{
	std::string CONTA_NO;		// 集装箱号
	std::string CONTA_MODEL;	// 规格类型
	std::string CONTA_SIZE;		// 箱尺寸
	std::string NOTE;			// 备注
};

struct structBILL_INFO_CLM20011
{
	std::string BILL_NO;
	std::vector<structCONTAINER_INFO_CLM20011> CONTAINER_INFO;
	std::string NOTE;			// 备注
};

struct structRESPOND_INFO_CLM20013
{
	char MESSAGE_ID[20 + 1];
	char OP_FLAG[1 + 1];
	char NOTE[255 + 1];
};



struct structSHIP_INFO_CLM20021
{
	char CUSTOMS_CODE[4 + 1];	// 海关代码
	char I_E_FLAG[1 + 1];		// 进/出口
	char NATIVE_SHIP_NAME_EN[255 + 1];	// 运输工具名称
	char NATIVE_SHIP_NAME_CN[255 + 1];	// 运输工具名称
	char NATIVE_VOYAGE_NO[36 + 1];		// 航次/班
	char CHANGE_SHIP_NAME_EN[255 + 1];
	char CHANGE_SHIP_NAME_CN[255 + 1];
	char CHANGE_VOYAGE_NO[36 + 1];

	char START_CUSTOMS_CODE[4 + 1];		// 
	char END_CUSTOMS_CODE[4 + 1];		// 
	char IMO[255 + 1];
	char SHIP_ID[16 + 1];
	char NOTE[255 + 1];			// 备注
};

struct structCONTAINER_INFO_CLM20021
{
	std::string CONTA_NO;		// 集装箱号
	std::string CONTA_MODEL;	// 规格类型
	std::string CONTA_SIZE;		// 箱尺寸
	std::string NOTE;			// 备注
};

struct structBILL_INFO_CLM20021
{
	std::string BILL_NO;
	std::vector<structCONTAINER_INFO_CLM20021> CONTAINER_INFO;
	std::string NOTE;			// 备注
};


struct structRESPOND_INFO_CLM20022
{
	char MESSAGE_ID[20 + 1];
	char OP_FLAG[1 + 1];
	char NOTE[255 + 1];
};


struct structBILL_INFO_CLM30011
{
	char CUSTOMS_CODE[4 + 1];	// 海关代码
	char I_E_FLAG[1 + 1];		// 进/出口
	char SHIP_NAME_EN[255 + 1];	// 运输工具名称
	char VOYAGE_NO[36 + 1];		// 航次/班
	char BILL_NO[36 + 1];
	char NOTE[255 + 1];			// 备注
};

struct structCONTAINER_INFO_CLM30011
{
	char CONTA_NO[11 + 1];		// 集装箱号
	char CONTA_MODEL[4 + 1];	// 规格类型
	char CONTA_SIZE[4 + 1];		// 箱尺寸
	char CONTA_LOCATION[20 + 1];
	char NOTE[255 + 1];			// 备注
};


struct structCONTAINER_ARRIVE_INFO_CLM40011
{
	char CUSTOMS_CODE[4 + 1];	// 海关代码
	char AREA_CODE[20 + 1];
	char VE_NAME[15 + 1];
	char ARRIVE_TIME[30 + 1];
	char CONTA_NO[11 + 1];
	char F_CONTA_NO[11 + 1];
	char F_CONTA_MODEL[4 + 1];
	char F_CONTA_SIZE[4 + 1];

	char B_CONTA_NO[11 + 1];
	char B_CONTA_MODEL[4 + 1];
	char B_CONTA_SIZE[4 + 1];
	char NOTE[255 + 1];			// 备注
};


struct structVEHICLES_INFO_CLM40021
{
	std::string VE_NAME;
	std::string F_CONTA_NO;
	std::string B_CONTA_NO;
	std::string VE_PROPERTY;
	std::string VE_MODEL;
	std::string VE_WT;
	std::string ENGINE_NO;
	std::string VE_TYPE;
	std::string VE_COLOR;
	std::string VE_PERMIT;
	std::string VE_CHARATER;
	std::string VE_OWNER;
	std::string DRIVER_PHONE;
	std::string VE_CUSTOMS_NO;
	std::string NOTE;
};



struct structBILL_INFO_CLM40031
{
	char CUSTOMS_CODE[4 + 1];	// 海关代码
	char I_E_FLAG[1 + 1];		// 进/出口
	char SHIP_NAME_EN[255 + 1];	// 运输工具名称
	char VOYAGE_NO[36 + 1];		// 航次/班
	char BILL_NO[36 + 1];
	char MOVE_ARR_TIME[30 + 1];
	char NOTE[255 + 1];			// 备注
};


struct structCONTAINER_INFO_CLM40031
{
	char CONTA_NO[11 + 1];		// 集装箱号
	char CONTA_MODEL[4 + 1];	// 规格类型
	char CONTA_SIZE[4 + 1];		// 箱尺寸
	char NOTE[255 + 1];			// 备注
};



struct structVEHICLES_INFO_CLM50011
{
	std::string AREA_ID;
	std::string CHNL_NO;
	std::string SEQ_NO;
	std::string I_E_TYPE;

	std::string CAR_EC_NO;
	std::string DR_IC_NO;
	std::string VE_NAME;

	std::string F_CONTA_NO;
	std::string B_CONTA_NO;

	std::string GROSS_WT;
	std::string NOTE;
};


struct structTRANS_INFO_CLM50031
{
	std::string CUSTOMS_CODE;
	std::string I_E_FLAG;
	std::string TRANS_NAME;
	std::string TRANS_NO;

	std::string LOADING_PORT;
	std::string DESTINATION_PORT;
	std::string UNLOAD_TIME;
	std::string NOTE;
};

struct structCONTAINER_INFO_CLM50031
{
	std::string CONTA_NO;		// 集装箱号
	std::string CONTA_MODEL;	// 规格类型
	std::string CONTA_SIZE;		// 箱尺寸
	std::string SEAL_NO;
	std::string NOTE;			// 备注
};


struct structRESPOND_INFO_CLM50033
{
	std::string MESSAGE_ID;		// 集装箱号
	std::string OP_FLAG;	// 规格类型
	std::string NOTE;			// 备注
};

struct structLOAD_INFO_CLM50041
{
	std::string CUSTOMS_CODE;		// 集装箱号
	std::string I_E_FLAG;	// 规格类型
	std::string TRANS_NAME;		// 箱尺寸
	std::string F_CONTA_NO;
	std::string B_CONTA_NO;			// 备注
	std::string LOADING_PORT;
	std::string DESTINATION_PORT;
	std::string LOAD_TIME;
	std::string NOTE;
};

struct structBILL_INFO_CLM60011
{
	std::string CUSTOMS_CODE;	// 海关代码
	std::string I_E_FLAG;		// 进/出口
	std::string SHIP_NAME_EN;	// 运输工具名称
	std::string VOYAGE_NO;		// 航次/班
	std::string NOTE;			// 备注
};

struct structCONTAINER_INFO_CLM60011
{
	std::string CONTA_NO;		// 集装箱号
	std::string CONTA_MODEL;	// 规格类型
	std::string CONTA_SIZE;		// 箱尺寸
	std::string NOTE;			// 备注
};

struct structRESPOND_INFO_CLM60013
{
	std::string MESSAGE_ID;		// 集装箱号
	std::string OP_FLAG;	// 规格类型
	std::string NOTE;			// 备注
};


struct structVEHICLES_INFO_CLM90011
{
	std::string CAR_EC_NO;
	std::string VE_NAME;
	std::string VE_PROPERTY;
	std::string VE_MODEL;
	std::string VE_WT;
	std::string ENGINE_NO;
	std::string VE_TYPE;
	std::string VE_COLOR;
	std::string VE_PERMIT;
	std::string VE_CHARATER;
	std::string VE_OWNER;
	std::string DRIVER_PHONE;
	std::string VE_CUSTOMS_NO;
	std::string NOTE;
};

struct structTRAILER_INFO_CLM90021
{
	std::string TR_EC_NO;
	std::string TR_NAME;
	std::string TR_WT;
	std::string TR_COLOR;
	std::string TR_CHARATER;
	std::string TR_OWNER;
	std::string DRIVER_PHONE;
	std::string NOTE;
};


struct structCOMPANY_INFO_CLM90031
{
	std::string COMPANY_ID;
	std::string COMPANY_NAME;
	std::string COMPANY_ADDRESS;
	std::string LEGAL;
	std::string LEGAL_PHONE;
	std::string NOTE;
};


struct structVEHICLES_INFO_CLM90041
{
	std::string CAR_IC_NO;
	std::string VE_NAME;
	std::string VE_PROPERTY;
	std::string TIME_LINESS;
	std::string PERIOD;
	std::string VE_WT;
	std::string VE_TYPE;
	std::string VE_CHARATER;
	std::string VE_OWNER;
	std::string DRIVER_PHONE;
	std::string VE_CUSTOMS_NO;
	std::string NOTE;
};




struct structMT2010_Head			// 报文头
{
	std::string MessageID;			// 报文编号
	std::string FunctionCode;		// 报文功能代码
	std::string MessageType;		// 报文类型代码
	std::string SenderID;			// 发送方代码
	std::string ReceiverID;		// 接受方代码
	std::string SendTime;			// 发送时间
	std::string Version;			// 报文版本
};

struct structMT2010_BorderTransportMeans	// 运输工具信息
{
	std::string JourneyID;					// 航次航班编号
	std::string TypeCode;					// 运输方式代码
	std::string ID;							// 运输工具代码
	std::string Name;						// 运输工具名称
	std::string FirstArrivalLocationID;		// 运输工具抵达关境内第一个目的港代码
	std::string ArrivalDateTime;			// 输工具抵达关境内第一个目的港的日期和时间
	std::string DepartureDateTime;			// 输工具启运日期和时间
};

struct structMT2010_ChangeReasonCode
{
	std::string ChangeReasonCode;			// 更改原因代码
};

struct structMT2010_TransportContractDocument	// 运输合同信息
{
	std::string ID;												// 总提运单号
	std::vector<structMT2010_ChangeReasonCode> ChangeReasonCode;			// 更改原因代码
	std::string ConditionCode;										// 运输条款代码
	std::string ConsolidatorID;									// 拼箱人代码
};

struct structMT2010_DeliveryDestination
{
	std::string Line;
	std::string CityName;
	std::string CountrySubEntityID;
	std::string CountrySubEntityName;
	std::string PostcodeID;
	std::string CountryCode;
};

struct structMT2010_Communication
{
	std::string ID;
	std::string TypeID;
};

struct structMT2010_IntermediateCarrier
{
	std::string ID;											// 中间承运人代码
	std::vector<structMT2010_Communication> Communication;	// 中间承运人联系方式
};

struct structMT2010_Consignee_Address
{
	std::string Line;
	std::string CityName;
	std::string CountrySubEntityID;
	std::string CountrySubEntityName;
	std::string PostcodeID;
	std::string CountryCode;
};

struct structMT2010_Contact	// 具体联系人信息
{
	std::string Name;
	std::vector<structMT2010_Communication> Communication;	// 中间承运人联系方式
};

struct structMT2010_Consignee		// 收货人信息
{
	std::string ID;
	std::string Name;
	structMT2010_Consignee_Address Addresss;			// 收货人地址信息
	std::vector<structMT2010_Communication> Communication;	// 收货人联系方式
	structMT2010_Contact	Contact;					// 具体联系人信息
};

struct structMT2010_Consignor		// 发货人信息
{
	std::string ID;
	std::string Name;
	structMT2010_Consignee_Address Addresss;			// 发货人地址信息
	std::vector<structMT2010_Communication> Communication;	// 发货人联系方式
};

struct structMT2010_NotifyParty		// 通知人信息
{
	std::string ID;
	std::string Name;
	structMT2010_Consignee_Address Addresss;			// 发货人地址信息
	std::vector<structMT2010_Communication> Communication;	// 发货人联系方式
};

struct structMT2010_UNDGContact		// 危险品联系人信息
{
	std::string Name;
	std::vector<structMT2010_Communication> Communication;	// 发货人联系方式
};

struct structMT2010_SealID
{
	std::string SealID;
	std::string AgencyCode;	// 施加封志人
};

struct structMT2010_TransportEquipment	// 集装箱（器）信息
{
	std::string EquipmentIdentificationID;
	std::string CharacteristicCode;
	std::string SupplierPartyTypeCode;
	std::string FullnessCode;
	std::vector<structMT2010_SealID> SealID; 
};

struct structMT2010_EquipmentIdentification
{
	std::string ID;
};

struct structMT2010_UCR
{
	std::string ID;
};

struct structMT2010_ConsignmentItem
{
	std::string SequenceNumeric;
	std::string QuantityQuantity;
	std::string TypeCode;
	std::string MarksNumbers;
	std::string CargoDescription;
	std::string UNDGCode;
	std::string TariffClassificationCode;
	std::string Content;
	std::string GrossMassMeasure;
	std::vector<structMT2010_EquipmentIdentification> EquipmentIdentification;
	std::string CurrentCode;
	std::vector<structMT2010_UCR> UCR;
	std::string OriginCountryCode;

};

struct structMT2010_Consignment			// 提运单信息
{
	structMT2010_TransportContractDocument TransportContractDocument;
	std::string AssociatedTransportDocumentID;				// 分提运单号
	std::string GrossVolumeMeasure;						// 货物体积
	std::string ValueAmount;
	std::string CurrencyTypeCode;							// 金额类型代码
	std::string LoadingLocationID;							// 装货地代码
	std::string LoadingLocationLoadingDate;				// 货物装载运输工具时间
	std::string UnloadingLocationID;						// 卸货地代码
	std::string UnloadingLocationArrivalDate;				// 到达卸货地日期
	std::string GoodsReceiptPlaceID;						// 收货地代码
	std::string GoodsReceiptPlaceName;						// 收货地名称
	std::string TranshipmentLocationID;					// 中转地代码
	std::string TransitDestinationID;						// 中转目的地代码
	std::string RoutingCountryCode;						// 途经的国家代码
	std::string GoodsConsignedPlaceID;						// 货物托运地点或国家代码
	std::string CustomsStatusCode;							// 货物海关状态代码
	std::string TransportSplitIndicator;					// 承运人货物分批到/发货标识
	std::string FreightPaymentMethodCode;							// 运费支付信息
	std::string ConsignmentPackagingQuantityQuantity;			// 托运货物件数
	std::string ConsignmentPackagingTypeCode;					// 包装种类代码
	std::string TotalGrossMassMeasure;							// 货物总毛重
	std::string PreviousCustomsDocumentID;						// 前一海关代码
	std::string PreviousCustomsDocumentTypeCode;					// 前一海关单证类型代码
	structMT2010_DeliveryDestination DeliveryDestination;	// 货物交付信息
	std::string HandlingInstructionsCodeCode;					// 码头作业指令代码
	structMT2010_IntermediateCarrier IntermediateCarrier;	// 中间承运人信息
	structMT2010_Consignee	Consignee;						// 收货人信息
	structMT2010_Consignor	Consignor;						// 发货人信息
	structMT2010_NotifyParty	NotifyParty;				// 通知人信息
	structMT2010_UNDGContact	UNDGContact;				// 危险品联系人信息
	std::vector<structMT2010_TransportEquipment>	TransportEquipment;		// 集装箱（器）信息
	std::vector<structMT2010_ConsignmentItem>	ConsignmentItem;		// 托运货物详细信息

};

struct structMT2010_Declaration								// 报文体
{
	std::string RepresentativePersonName;						// 舱单传输人名称
	std::string ExitCustomsOfficeID;								// 运输工具离境地海关代码
	std::string AgentID;											// 运输工具代理企业代码
	std::string CarrierID;										// 承运人代码
	structMT2010_BorderTransportMeans BorderTransportMeans;	// 运输工具信息
	std::vector<structMT2010_Consignment>	Consignment;				// 提运单信息
	std::string AdditionalInformationContent;					// 备注
};

struct structMT2010
{
	structMT2010_Head		 Head;
	structMT2010_Declaration Declaration;
};

struct structConfigInfo
{
	std::string strQueueIP;
	std::string strQueueName;
	std::string strQueueLabel;

	std::string strWebQueueIP;
	std::string strWebQueueName;
	std::string strWebQueueLabel;

	std::string strFrontQueueIP;
	std::string strFrontQueueName;
	std::string strFrontQueueLabel;

	std::string strBackerQueueIP;
	std::string strBackerQueueName;
	std::string strBackerQueueLabel;



	std::string strSqlServerDataUser;
	std::string strSqlServerDataPwd;
	std::string strSqlServerDataName;
	std::string strSqlServerDataServer;
	std::string strFilePath;
	std::string strErrorFilePath;
};

#endif