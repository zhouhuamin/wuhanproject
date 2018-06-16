#ifndef MT_INFO_STRUCT_H_H
#define MT_INFO_STRUCT_H_H
#include <string>
#include <vector>

struct structMT3101_Head			// 
{
	std::string MessageID;			// 
	std::string FunctionCode;		// 
	std::string MessageType;		// 
	std::string SenderID;			// 
	std::string ReceiverID;		// 
	std::string SendTime;			//
	std::string Version;			// 
};

struct structMT3101_BorderTransportMeans	// 
{
	std::string JourneyID;					// 
	std::string TypeCode;					// 
	std::string ID;							// 
	std::string Name;						// 
};

struct structMT3101_UnloadingLocation
{
	std::string ID;							// 
	std::string ArrivalDate;				// 
};

struct structMT3101_TransportContractDocument	// 
{
	std::string ID;												// 
};

struct structMT3101_AssociatedTransportDocument					// 
{
	std::string ID;												// 
};

struct structMT3101_ConsignmentPackaging						// 
{
	std::string QuantityQuantity;								// 
	std::string TypeCode;										// 
};

struct structMT3101_SealID
{
	std::string SealID;
	std::string AgencyCode;	// 
};

struct structMT3101_EquipmentIdentification
{
	std::string ID;	// 
};

struct structMT3101_TransportEquipment	// 
{
	structMT3101_EquipmentIdentification EquipmentIdentification;
	std::string CharacteristicCode;
	std::string FullnessCode;
	std::vector<structMT3101_SealID> SealID; 
};

struct structMT3101_ConsignmentItemPackaging
{
	std::string MarksNumbers;
};

struct structMT3101_Commodity
{
	std::string CargoDescription;
};

struct structMT3101_AdditionalInformation
{
	std::string Content;
};

struct structMT3101_ConsignmentItem
{
	std::string SequenceNumeric;
	structMT3101_ConsignmentItemPackaging 	ConsignmentItemPackaging;
	structMT3101_Commodity 					Commodity;
	structMT3101_AdditionalInformation 		AdditionalInformation;
};

struct structMT3101_Consignment			// 
{
	structMT3101_TransportContractDocument 		TransportContractDocument;
	structMT3101_AssociatedTransportDocument 	AssociatedTransportDocument;				// 
	structMT3101_ConsignmentPackaging 			ConsignmentPackaging;						// 
	std::string TotalGrossMassMeasure;
	std::vector<structMT3101_ConsignmentItem>	ConsignmentItem;		// 

};

struct structMT3101_Declaration											// 
{
	std::string DeclarationOfficeID;									// 
	structMT3101_BorderTransportMeans 		BorderTransportMeans;		// 
	structMT3101_UnloadingLocation			UnloadingLocation;
	std::vector<structMT3101_Consignment>	Consignment;				// 
	std::vector<structMT3101_TransportEquipment> TransportEquipment;
	structMT3101_AdditionalInformation 		AdditionalInformation;					// 
};

struct structMT3101
{
	structMT3101_Head		 Head;
	structMT3101_Declaration Declaration;
};

struct structMT4101_Head			// 
{
	std::string MessageID;			// 
	std::string FunctionCode;		// 
	std::string MessageType;		// 
	std::string SenderID;			// 
	std::string ReceiverID;		// 
	std::string SendTime;			//
	std::string Version;			// 
};

struct structMT4101_Carrier
{
	std::string ID;
};

struct structMT4101_BorderTransportMeans	// 
{
	std::string JourneyID;					// 
	std::string TypeCode;					// 
	std::string IEFlag;
	std::string ID;							// 
	std::string Name;						// 
};

struct structMT4101_AdditionalInformation
{
	std::string Content;
};

struct structMT4101_EquipmentIdentification
{
	std::string ID;	// 
};

struct structMT4101_SealID
{
	std::string SealID;
	std::string AgencyCode;	// 
};

struct structMT4101_TransportEquipment
{
	structMT4101_EquipmentIdentification EquipmentIdentification;
	std::string TypeFlag;
	std::string OperationType;
	std::string CharacteristicCode;
	std::string FullnessCode;
	std::vector<structMT4101_SealID> SealID; 
};

struct structMT4101_Commodity
{
	std::string CargoDescription;
};

struct structMT4101_ConsignmentItem
{
	structMT4101_Commodity 					Commodity;
};

struct structMT4101_TransportContractDocument	// 
{
	std::string ID;												// 
};

struct structMT4101_LoadingLocation
{
	std::string ID;							// 
	std::string LoadingDate;				// 
};

struct structMT4101_ConsignmentPackaging						// 
{
	std::string QuantityQuantity;								// 
	std::string TypeCode;										// 
};

struct structMT4101_Consignment			// 
{
	structMT4101_TransportContractDocument 		TransportContractDocument;
	structMT4101_LoadingLocation 				LoadingLocation;
	structMT4101_ConsignmentPackaging 			ConsignmentPackaging;						// 
	std::string TotalGrossMassMeasure;
	std::string TransportSplitIndicator;
	std::vector<structMT4101_TransportEquipment> TransportEquipment;
	std::vector<structMT4101_ConsignmentItem>	ConsignmentItem;
};

struct structMT4101_Declaration											// 
{
	structMT4101_Carrier 					Carrier;		// 
	structMT4101_BorderTransportMeans 		BorderTransportMeans;		// 
	std::vector<structMT4101_Consignment>	Consignment;				// 
	structMT4101_AdditionalInformation 		AdditionalInformation;					// 
};

struct structMT4101
{
	structMT4101_Head		 Head;
	structMT4101_Declaration Declaration;
};

struct structMT5101_Head			// 
{
	std::string MessageID;			// 
	std::string FunctionCode;		// 
	std::string MessageType;		// 
	std::string SenderID;			// 
	std::string ReceiverID;		// 
	std::string SendTime;			//
	std::string Version;			// 
};

struct structMT5101_LoadingLocation
{
	std::string ID;
};

struct structMT5101_UnloadingLocation
{
	std::string ID;
};

struct structMT5101_Communication
{
	std::string ID;
	std::string TypeID;
};

struct structMT5101_Master
{
	std::string 							Name;
	std::vector<structMT5101_Communication>	Communication;
	
};

struct structMT5101_Contact
{
	std::string								Name;
	std::vector<structMT5101_Communication>	Communication;
};

struct structMT5101_TallyParty
{
	std::string ID;
	structMT5101_Contact	Contact;
};

struct structMT5101_BorderTransportMeans	// 
{
	std::string JourneyID;					// 
	std::string TypeCode;					// 
	std::string ID;							// 
	std::string Name;						// 
	std::string ActualDateTime;
	std::string CompletedDateTime;
	structMT5101_LoadingLocation	LoadingLocation;
	structMT5101_UnloadingLocation	UnloadingLocation;
	structMT5101_Master				Master;
};

struct structMT5101_TransportContractDocument	// 
{
	std::string ID;												// 
};

struct structMT5101_AssociatedTransportDocument					// 
{
	std::string ID;												// 
};

struct structMT5101_ConsignmentPackaging						// 
{
	std::string QuantityQuantity;								// 
	std::string TypeCode;										// 
};

struct structMT5101_Consignment			// 
{
	structMT5101_TransportContractDocument 		TransportContractDocument;
	structMT5101_AssociatedTransportDocument 	AssociatedTransportDocument;
	structMT5101_ConsignmentPackaging 			ConsignmentPackaging;						// 
	std::string TotalGrossMassMeasure;
	std::string GrossVolumeMeasure;
};

struct structMT5101_EquipmentIdentification
{
	std::string ID;	// 
};

struct structMT5101_SealID
{
	std::string SealID;
	std::string AgencyCode;	// 
};

struct structMT5101_DamageType
{
	std::string Code;
	std::string Text;
};

struct structMT5101_DamageArea
{
	std::string Code;
	std::string Text;
};

struct structMT5101_Damage
{
	structMT5101_DamageType DamageType;
	structMT5101_DamageArea DamageArea;
	std::string Severity;
};

struct structMT5101_TransportMeans
{
	std::string StowageLocationID;
};

struct structMT5101_TransportEquipment
{
	structMT5101_EquipmentIdentification EquipmentIdentification;
	std::string CharacteristicCode;
	std::string FullnessCode;
	std::vector<structMT5101_SealID> SealID; 
	structMT5101_Damage Damage;
	structMT5101_TransportMeans TransportMeans;
};

struct structMT5101_AdditionalInformation
{
	std::string Content;
};

struct structMT5101_Declaration											// 
{
	std::string 									DeclarationOfficeID;
	structMT5101_BorderTransportMeans 				BorderTransportMeans;		// 
	structMT5101_TallyParty							TallyParty;
	std::vector<structMT5101_Consignment>			Consignment;				// 
	std::vector<structMT5101_TransportEquipment>	TransportEquipment;
	structMT5101_AdditionalInformation 				AdditionalInformation;					// 
};

struct structMT5101
{
	structMT5101_Head		 Head;
	structMT5101_Declaration Declaration;
};


struct structMTXXXX_Head			// 
{
	std::string MessageID;			// 
	std::string FunctionCode;		// 
	std::string MessageType;		// 
	std::string SenderID;			// 
	std::string ReceiverID;		// 
	std::string SendTime;			//
	std::string Version;			// 
};

struct structMTXXXX_BorderTransportMeans	// 
{
	std::string JourneyID;					// 
	std::string ID;					// 
};

struct structMTXXXX_ResponseType
{
	std::string Code;					// 
	std::string Text;					// 
};

struct structMTXXXX_Consignment			// 
{
	structMT3101_TransportContractDocument 		TransportContractDocument;
	structMT3101_AssociatedTransportDocument 	AssociatedTransportDocument;				// 
	structMTXXXX_ResponseType 					ResponseType;						// 
};

struct structMTXXXX_TransportEquipment	// 
{
	structMT3101_EquipmentIdentification EquipmentIdentification;
	structMTXXXX_ResponseType 					ResponseType;
};

struct structMTXXXX_Response											// 
{									// 
	structMTXXXX_BorderTransportMeans 		BorderTransportMeans;		// 
	std::vector<structMTXXXX_Consignment>	Consignment;				// 
	std::vector<structMTXXXX_TransportEquipment> TransportEquipment;
	structMTXXXX_ResponseType 					ResponseType;					// 
};

struct structMT_RESPONSE
{
	structMTXXXX_Head		 Head;
	structMTXXXX_Response Declaration;
};

#endif