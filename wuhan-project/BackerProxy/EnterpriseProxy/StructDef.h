#ifndef STRUCTDEF_H
#define STRUCTDEF_H
#include <string>
#include <vector>

// ����ͷ���ݶ���
struct structMESSAGE_HEAD
{
	char MESSAGE_ID[20 + 1];	// ���ı��
	char FUNCTION_CODE[2 + 1];	// ���ܴ��� 2:����, 3:ɾ��
	char MESSAGE_TYPE[10 + 1];	// ��������
	char SENDER[20 + 1];		// ������
	char RECEIVE[20 + 1];		// ������
	char MESSAGE_CREATE_TIME[30 + 1];	// ����ʱ��
	char VERSION[10 + 1];		// �汾��
};

// ���������ݶ���
struct structBILL_INFO
{
	char CUSTOMS_CODE[4 + 1];	// ���ش���
	char I_E_FLAG[1 + 1];		// ��/����
	char SHIP_NAME_EN[255 + 1];	// ���乤������
	char VOYAGE_NO[36 + 1];		// ����/��
	char NOTE[255 + 1];			// ��ע
};


struct structCONTAINER_INFO
{
	char CONTA_NO[11 + 1];		// ��װ���
	char CONTA_MODEL[4 + 1];	// �������
	char CONTA_SIZE[4 + 1];		// ��ߴ�
	char SEAL_NO[30 + 1];		// ��־��
	char NOTE[255 + 1];			// ��ע
};

struct structRESPOND_INFO
{
	char MESSAGE_ID[20 + 1];	// ���ı��
	char OP_FLAG[1 + 1];		// ����־ T:�ɹ�, F:ʧ��
	char NOTE[255 + 1];			// ��ע
};

//
struct structBILL_INFO_CLM10011
{
	std::string CUSTOMS_CODE;	// ���ش���
	std::string I_E_FLAG;		// ��/����
	std::string SHIP_NAME_EN;	// ���乤������
	std::string VOYAGE_NO;		// ����/��
	std::string BILL_NO;		// ���˵���
	std::string NOTE;			// ��ע
};

struct structCONTAINER_INFO_CLM10011
{
	char CONTA_NO[11 + 1];		// ��װ���
	char CONTA_MODEL[4 + 1];	// �������
	char CONTA_SIZE[4 + 1];		// ��ߴ�
	char NOTE[255 + 1];			// ��ע
};

struct structBILL_INFO_CLM10021
{
	char CUSTOMS_CODE[4 + 1];	// ���ش���
	char I_E_FLAG[1 + 1];		// ��/����
	char SHIP_NAME_EN[255 + 1];	// ���乤������
	char VOYAGE_NO[36 + 1];		// ����/��
	char BILL_NO[36 + 1];		// ���˵���
	char AGENT_NAME[255 + 1];
	char ENTRY_ID[18 + 1];
	char NOTE[255 + 1];			// ��ע
};

struct structCONTAINER_INFO_CLM10021
{
	char CONTA_NO[11 + 1];		// ��װ���
	char EXAM_TYPE[2 + 1];		// 
	char NOTE[255 + 1];			// ��ע
};

struct structBILL_INFO_CLM10031
{
	char CUSTOMS_CODE[4 + 1];	// ���ش���
	char I_E_FLAG[1 + 1];		// ��/����
	char SHIP_NAME_EN[255 + 1];	// ���乤������
	char VOYAGE_NO[36 + 1];		// ����/��
	char BILL_NO[36 + 1];		// ���˵���
	char NOTE[255 + 1];			// ��ע
};

struct structCONTAINER_INFO_CLM10031
{
	char CONTA_NO[11 + 1];		// ��װ���
	char CONTA_MODEL[4 + 1];	// �������
	char CONTA_SIZE[4 + 1];		// ��ߴ�
	char NOTE[255 + 1];			// ��ע
};


struct structBILL_INFO_CLM10041
{
	char CUSTOMS_CODE[4 + 1];	// ���ش���
	char I_E_FLAG[1 + 1];		// ��/����
	char SHIP_NAME_EN[255 + 1];	// ���乤������
	char VOYAGE_NO[36 + 1];		// ����/��
	char NOTE[255 + 1];			// ��ע
};

struct structCONTAINER_INFO_CLM10041
{
	char CONTA_NO[11 + 1];		// ��װ���
	char CONTA_MODEL[4 + 1];	// �������
	char CONTA_SIZE[4 + 1];		// ��ߴ�
	char SEAL_NO[30 + 1];
	char NOTE[255 + 1];			// ��ע
};


struct structBILL_INFO_CLM10051
{
	char CUSTOMS_CODE[4 + 1];	// ���ش���
	char I_E_FLAG[1 + 1];		// ��/����
	char SHIP_NAME_EN[255 + 1];	// ���乤������
	char VOYAGE_NO[36 + 1];		// ����/��
	char NOTE[255 + 1];			// ��ע
};

struct structCONTAINER_INFO_CLM10051
{
	char CONTA_NO[11 + 1];		// ��װ���
	char CONTA_MODEL[4 + 1];	// �������
	char CONTA_SIZE[4 + 1];		// ��ߴ�
	char NOTE[255 + 1];			// ��ע
};


struct structBILL_INFO_CLM10061
{
	char CUSTOMS_CODE[4 + 1];	// ���ش���
	char I_E_FLAG[1 + 1];		// ��/����
	char SHIP_NAME_EN[255 + 1];	// ���乤������
	char VOYAGE_NO[36 + 1];		// ����/��
	char NOTE[255 + 1];			// ��ע
};

struct structCONTAINER_INFO_CLM10061
{
	char CONTA_NO[11 + 1];		// ��װ���
	char CONTA_MODEL[4 + 1];	// �������
	char CONTA_SIZE[4 + 1];		// ��ߴ�
	char NOTE[255 + 1];			// ��ע
};



struct structSHIP_INFO_CLM20011
{
	char CUSTOMS_CODE[4 + 1];	// ���ش���
	char I_E_FLAG[1 + 1];		// ��/����
	char NATIVE_SHIP_NAME_EN[255 + 1];	// ���乤������
	char NATIVE_SHIP_NAME_CN[255 + 1];	// ���乤������
	char NATIVE_VOYAGE_NO[36 + 1];		// ����/��
	char START_CUSTOMS_CODE[4 + 1];		// 
	char END_CUSTOMS_CODE[4 + 1];		// 
	char IMO[255 + 1];
	char SHIP_ID[16 + 1];
	char NOTE[255 + 1];			// ��ע
};

struct structCONTAINER_INFO_CLM20011
{
	std::string CONTA_NO;		// ��װ���
	std::string CONTA_MODEL;	// �������
	std::string CONTA_SIZE;		// ��ߴ�
	std::string NOTE;			// ��ע
};

struct structBILL_INFO_CLM20011
{
	std::string BILL_NO;
	std::vector<structCONTAINER_INFO_CLM20011> CONTAINER_INFO;
	std::string NOTE;			// ��ע
};

struct structRESPOND_INFO_CLM20013
{
	char MESSAGE_ID[20 + 1];
	char OP_FLAG[1 + 1];
	char NOTE[255 + 1];
};



struct structSHIP_INFO_CLM20021
{
	char CUSTOMS_CODE[4 + 1];	// ���ش���
	char I_E_FLAG[1 + 1];		// ��/����
	char NATIVE_SHIP_NAME_EN[255 + 1];	// ���乤������
	char NATIVE_SHIP_NAME_CN[255 + 1];	// ���乤������
	char NATIVE_VOYAGE_NO[36 + 1];		// ����/��
	char CHANGE_SHIP_NAME_EN[255 + 1];
	char CHANGE_SHIP_NAME_CN[255 + 1];
	char CHANGE_VOYAGE_NO[36 + 1];

	char START_CUSTOMS_CODE[4 + 1];		// 
	char END_CUSTOMS_CODE[4 + 1];		// 
	char IMO[255 + 1];
	char SHIP_ID[16 + 1];
	char NOTE[255 + 1];			// ��ע
};

struct structCONTAINER_INFO_CLM20021
{
	std::string CONTA_NO;		// ��װ���
	std::string CONTA_MODEL;	// �������
	std::string CONTA_SIZE;		// ��ߴ�
	std::string NOTE;			// ��ע
};

struct structBILL_INFO_CLM20021
{
	std::string BILL_NO;
	std::vector<structCONTAINER_INFO_CLM20021> CONTAINER_INFO;
	std::string NOTE;			// ��ע
};


struct structRESPOND_INFO_CLM20022
{
	char MESSAGE_ID[20 + 1];
	char OP_FLAG[1 + 1];
	char NOTE[255 + 1];
};


struct structBILL_INFO_CLM30011
{
	char CUSTOMS_CODE[4 + 1];	// ���ش���
	char I_E_FLAG[1 + 1];		// ��/����
	char SHIP_NAME_EN[255 + 1];	// ���乤������
	char VOYAGE_NO[36 + 1];		// ����/��
	char BILL_NO[36 + 1];
	char NOTE[255 + 1];			// ��ע
};

struct structCONTAINER_INFO_CLM30011
{
	char CONTA_NO[11 + 1];		// ��װ���
	char CONTA_MODEL[4 + 1];	// �������
	char CONTA_SIZE[4 + 1];		// ��ߴ�
	char CONTA_LOCATION[20 + 1];
	char NOTE[255 + 1];			// ��ע
};


struct structCONTAINER_ARRIVE_INFO_CLM40011
{
	char CUSTOMS_CODE[4 + 1];	// ���ش���
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
	char NOTE[255 + 1];			// ��ע
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
	char CUSTOMS_CODE[4 + 1];	// ���ش���
	char I_E_FLAG[1 + 1];		// ��/����
	char SHIP_NAME_EN[255 + 1];	// ���乤������
	char VOYAGE_NO[36 + 1];		// ����/��
	char BILL_NO[36 + 1];
	char MOVE_ARR_TIME[30 + 1];
	char NOTE[255 + 1];			// ��ע
};


struct structCONTAINER_INFO_CLM40031
{
	char CONTA_NO[11 + 1];		// ��װ���
	char CONTA_MODEL[4 + 1];	// �������
	char CONTA_SIZE[4 + 1];		// ��ߴ�
	char NOTE[255 + 1];			// ��ע
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
	std::string CONTA_NO;		// ��װ���
	std::string CONTA_MODEL;	// �������
	std::string CONTA_SIZE;		// ��ߴ�
	std::string SEAL_NO;
	std::string NOTE;			// ��ע
};


struct structRESPOND_INFO_CLM50033
{
	std::string MESSAGE_ID;		// ��װ���
	std::string OP_FLAG;	// �������
	std::string NOTE;			// ��ע
};

struct structLOAD_INFO_CLM50041
{
	std::string CUSTOMS_CODE;		// ��װ���
	std::string I_E_FLAG;	// �������
	std::string TRANS_NAME;		// ��ߴ�
	std::string F_CONTA_NO;
	std::string B_CONTA_NO;			// ��ע
	std::string LOADING_PORT;
	std::string DESTINATION_PORT;
	std::string LOAD_TIME;
	std::string NOTE;
};

struct structBILL_INFO_CLM60011
{
	std::string CUSTOMS_CODE;	// ���ش���
	std::string I_E_FLAG;		// ��/����
	std::string SHIP_NAME_EN;	// ���乤������
	std::string VOYAGE_NO;		// ����/��
	std::string NOTE;			// ��ע
};

struct structCONTAINER_INFO_CLM60011
{
	std::string CONTA_NO;		// ��װ���
	std::string CONTA_MODEL;	// �������
	std::string CONTA_SIZE;		// ��ߴ�
	std::string NOTE;			// ��ע
};

struct structRESPOND_INFO_CLM60013
{
	std::string MESSAGE_ID;		// ��װ���
	std::string OP_FLAG;	// �������
	std::string NOTE;			// ��ע
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




struct structMT2010_Head			// ����ͷ
{
	std::string MessageID;			// ���ı��
	std::string FunctionCode;		// ���Ĺ��ܴ���
	std::string MessageType;		// �������ʹ���
	std::string SenderID;			// ���ͷ�����
	std::string ReceiverID;		// ���ܷ�����
	std::string SendTime;			// ����ʱ��
	std::string Version;			// ���İ汾
};

struct structMT2010_BorderTransportMeans	// ���乤����Ϣ
{
	std::string JourneyID;					// ���κ�����
	std::string TypeCode;					// ���䷽ʽ����
	std::string ID;							// ���乤�ߴ���
	std::string Name;						// ���乤������
	std::string FirstArrivalLocationID;		// ���乤�ߵִ�ؾ��ڵ�һ��Ŀ�ĸ۴���
	std::string ArrivalDateTime;			// �乤�ߵִ�ؾ��ڵ�һ��Ŀ�ĸ۵����ں�ʱ��
	std::string DepartureDateTime;			// �乤���������ں�ʱ��
};

struct structMT2010_ChangeReasonCode
{
	std::string ChangeReasonCode;			// ����ԭ�����
};

struct structMT2010_TransportContractDocument	// �����ͬ��Ϣ
{
	std::string ID;												// �����˵���
	std::vector<structMT2010_ChangeReasonCode> ChangeReasonCode;			// ����ԭ�����
	std::string ConditionCode;										// �����������
	std::string ConsolidatorID;									// ƴ���˴���
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
	std::string ID;											// �м�����˴���
	std::vector<structMT2010_Communication> Communication;	// �м��������ϵ��ʽ
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

struct structMT2010_Contact	// ������ϵ����Ϣ
{
	std::string Name;
	std::vector<structMT2010_Communication> Communication;	// �м��������ϵ��ʽ
};

struct structMT2010_Consignee		// �ջ�����Ϣ
{
	std::string ID;
	std::string Name;
	structMT2010_Consignee_Address Addresss;			// �ջ��˵�ַ��Ϣ
	std::vector<structMT2010_Communication> Communication;	// �ջ�����ϵ��ʽ
	structMT2010_Contact	Contact;					// ������ϵ����Ϣ
};

struct structMT2010_Consignor		// ��������Ϣ
{
	std::string ID;
	std::string Name;
	structMT2010_Consignee_Address Addresss;			// �����˵�ַ��Ϣ
	std::vector<structMT2010_Communication> Communication;	// ��������ϵ��ʽ
};

struct structMT2010_NotifyParty		// ֪ͨ����Ϣ
{
	std::string ID;
	std::string Name;
	structMT2010_Consignee_Address Addresss;			// �����˵�ַ��Ϣ
	std::vector<structMT2010_Communication> Communication;	// ��������ϵ��ʽ
};

struct structMT2010_UNDGContact		// Σ��Ʒ��ϵ����Ϣ
{
	std::string Name;
	std::vector<structMT2010_Communication> Communication;	// ��������ϵ��ʽ
};

struct structMT2010_SealID
{
	std::string SealID;
	std::string AgencyCode;	// ʩ�ӷ�־��
};

struct structMT2010_TransportEquipment	// ��װ�䣨������Ϣ
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

struct structMT2010_Consignment			// ���˵���Ϣ
{
	structMT2010_TransportContractDocument TransportContractDocument;
	std::string AssociatedTransportDocumentID;				// �����˵���
	std::string GrossVolumeMeasure;						// �������
	std::string ValueAmount;
	std::string CurrencyTypeCode;							// ������ʹ���
	std::string LoadingLocationID;							// װ���ش���
	std::string LoadingLocationLoadingDate;				// ����װ�����乤��ʱ��
	std::string UnloadingLocationID;						// ж���ش���
	std::string UnloadingLocationArrivalDate;				// ����ж��������
	std::string GoodsReceiptPlaceID;						// �ջ��ش���
	std::string GoodsReceiptPlaceName;						// �ջ�������
	std::string TranshipmentLocationID;					// ��ת�ش���
	std::string TransitDestinationID;						// ��תĿ�ĵش���
	std::string RoutingCountryCode;						// ;���Ĺ��Ҵ���
	std::string GoodsConsignedPlaceID;						// �������˵ص����Ҵ���
	std::string CustomsStatusCode;							// ���ﺣ��״̬����
	std::string TransportSplitIndicator;					// �����˻��������/������ʶ
	std::string FreightPaymentMethodCode;							// �˷�֧����Ϣ
	std::string ConsignmentPackagingQuantityQuantity;			// ���˻������
	std::string ConsignmentPackagingTypeCode;					// ��װ�������
	std::string TotalGrossMassMeasure;							// ������ë��
	std::string PreviousCustomsDocumentID;						// ǰһ���ش���
	std::string PreviousCustomsDocumentTypeCode;					// ǰһ���ص�֤���ʹ���
	structMT2010_DeliveryDestination DeliveryDestination;	// ���ｻ����Ϣ
	std::string HandlingInstructionsCodeCode;					// ��ͷ��ҵָ�����
	structMT2010_IntermediateCarrier IntermediateCarrier;	// �м��������Ϣ
	structMT2010_Consignee	Consignee;						// �ջ�����Ϣ
	structMT2010_Consignor	Consignor;						// ��������Ϣ
	structMT2010_NotifyParty	NotifyParty;				// ֪ͨ����Ϣ
	structMT2010_UNDGContact	UNDGContact;				// Σ��Ʒ��ϵ����Ϣ
	std::vector<structMT2010_TransportEquipment>	TransportEquipment;		// ��װ�䣨������Ϣ
	std::vector<structMT2010_ConsignmentItem>	ConsignmentItem;		// ���˻�����ϸ��Ϣ

};

struct structMT2010_Declaration								// ������
{
	std::string RepresentativePersonName;						// �յ�����������
	std::string ExitCustomsOfficeID;								// ���乤���뾳�غ��ش���
	std::string AgentID;											// ���乤�ߴ�����ҵ����
	std::string CarrierID;										// �����˴���
	structMT2010_BorderTransportMeans BorderTransportMeans;	// ���乤����Ϣ
	std::vector<structMT2010_Consignment>	Consignment;				// ���˵���Ϣ
	std::string AdditionalInformationContent;					// ��ע
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