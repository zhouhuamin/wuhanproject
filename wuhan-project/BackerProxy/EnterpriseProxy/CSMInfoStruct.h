#ifndef CSM_INFO_STRUCT_H
#define CSM_INFO_STRUCT_H
#include <string>

struct structCSM001_MessageHead
{
	std::string MessageID;			// 
	std::string FunctionCode;		// 
	std::string MessageType;		// 
	std::string SenderID;			// 
	std::string ReceiverID;			// 
	std::string SendTime;			//
	std::string Version;			// 
};

struct structCSM001_Archive
{
	std::string VesselNameEn;
	std::string VesselNameCn;
	std::string IMO;
	std::string CallSign;
	std::string Nationality;
	std::string CertificateNo;
	std::string CertificateDate;
	std::string BuildDate;
	std::string HailingPort;
	std::string VesselSort;
	std::string LineType;
	std::string CommunicationID;
	std::string CommunicationType;
	std::string GrossTonnage;
	std::string NetTonnage;
	std::string DeadWeightTonnage;
	std::string VesselLength;
	std::string VesselBreadth;
	std::string VesselDeapth;
	std::string ControlType;
	std::string CoCode;
	std::string VesselCorpNameEn;
	std::string VesselCorpNameCn;
	std::string RecordDateTime;
	std::string ClassLetter;
	std::string ClassificationNo;
	std::string VesselColor;
	std::string FunnelColor;
	std::string Email;
	std::string SatellitePhone;
	std::string HorsePower;
	std::string Speed;
	std::string TPI_TPC;
	std::string DeepDraft;
	std::string LightDraft;
	std::string CarbinWindowNum;
	std::string CargoDear;
	std::string BaleCapacity;
	std::string GrainCapacity;
	std::string CeilingTEU;
	std::string DeckTEU;
	std::string CrewLimit;
	std::string PassengerLimit;
	std::string Memo;	
};

struct structCSM001_Declaration
{
	struct structCSM001_Archive Archive;
};

struct structCSM001_MessageBody
{
	struct structCSM001_Declaration Declaration;
};

struct structCSM001
{
	structCSM001_MessageHead		 	MessageHead;
	structCSM001_MessageBody 			MessageBody;
};

struct structCSM301_REC
{
	std::string MessageID;
	std::string IMO;
	std::string Voyage;
	std::string RecType;
	std::string RecContent;
	std::string PortCode;
	std::string CustomsName;
	std::string SendTime;
};

struct structCSM301_Declaration
{
	struct structCSM301_REC REC;
};

struct structCSM301_MessageBody
{
	struct structCSM301_Declaration Declaration;
};

struct structCSM301
{
	structCSM001_MessageHead		 	MessageHead;
	structCSM301_MessageBody 			MessageBody;
};


struct structCSM002_Itinerary
{
	std::string ArrivalDateTime;
};

struct structCSM002_DepartureConveyanceFacility
{
	std::string ID;
};

struct structCSM002_ArrivalConveyanceFacility
{
	std::string ID;
};

struct structCSM002_BorderTransPortMeans
{
	std::string ID;
	std::string Name;
	std::string JourneyID;
	std::string JourneyID2;
	structCSM002_Itinerary Itinerary;
	std::string StayID;
	structCSM002_DepartureConveyanceFacility	DepartureConveyanceFacility;
	structCSM002_ArrivalConveyanceFacility		ArrivalConveyanceFacility;
};

struct structCSM002_LoadingLocation
{
	std::string LoadingDate;
};

struct structCSM002_Contact
{
	std::string Name;
};

struct structCSM002_Agent
{
	std::string ID;
	structCSM002_Contact	Contact;
};

struct structCSM002_AdditionalInformation
{
	std::string Content;
};

struct structCSM002_Declaration
{
	struct structCSM002_BorderTransPortMeans		BorderTransPortMeans;
	struct structCSM002_LoadingLocation				LoadingLocation;
	struct structCSM002_Agent						Agent;
	std::string IssueDate;
	struct structCSM002_AdditionalInformation		AdditionalInformation;
};

struct structCSM002_MessageBody
{
	struct structCSM002_Declaration Declaration;
};

struct structCSM002
{
	structCSM001_MessageHead		 	MessageHead;
	structCSM002_MessageBody 			MessageBody;
};

struct structCSM003_Commodity
{
	std::string CargoDescription;
};

struct structCSM003_ConsignmentItem
{
	structCSM003_Commodity	Commodity;
};

struct structCSM003_Consignment
{
	structCSM003_ConsignmentItem	ConsignmentItem;
};

struct structCSM003_Declaration
{
	struct structCSM002_BorderTransPortMeans		BorderTransPortMeans;
	struct structCSM003_Consignment					Consignment;
	struct structCSM002_Agent						Agent;
	std::string IssueDate;
	struct structCSM002_AdditionalInformation		AdditionalInformation;
};

struct structCSM003_MessageBody
{
	struct structCSM003_Declaration Declaration;
};

struct structCSM003
{
	structCSM001_MessageHead		 	MessageHead;
	structCSM003_MessageBody 			MessageBody;
};


struct structCSM004_Declaration
{
	struct structCSM002_BorderTransPortMeans		BorderTransPortMeans;
	struct structCSM002_Agent						Agent;
	std::string IssueDate;
	struct structCSM002_AdditionalInformation		AdditionalInformation;
};

struct structCSM004_MessageBody
{
	struct structCSM004_Declaration Declaration;
};

struct structCSM004
{
	structCSM001_MessageHead		 	MessageHead;
	structCSM004_MessageBody 			MessageBody;
};

struct structCSM005_BorderTransPortMeans
{
	std::string ID;
	std::string Name;
	std::string JourneyID;
	std::string JourneyID2;
	structCSM002_Itinerary Itinerary;
	std::string StayID;
	std::string StayID2;
	std::string DataTime;
	structCSM002_DepartureConveyanceFacility	DepartureConveyanceFacility;
	structCSM002_ArrivalConveyanceFacility		ArrivalConveyanceFacility;
};

struct structCSM005_Declaration
{
	struct structCSM005_BorderTransPortMeans		BorderTransPortMeans;
	struct structCSM002_Agent						Agent;
	std::string IssueDate;
	struct structCSM002_AdditionalInformation		AdditionalInformation;
};

struct structCSM005_MessageBody
{
	struct structCSM005_Declaration Declaration;
};

struct structCSM005
{
	structCSM001_MessageHead		 	MessageHead;
	structCSM005_MessageBody 			MessageBody;
};

struct structCSM007_Itinerary
{
	std::string ArrivalDateTime;
	std::string DepartureDateTime;
};

struct structCSM007_BorderTransPortMeans
{
	std::string ID;
	std::string Name;
	std::string JourneyID;
	std::string JourneyID2;
	structCSM007_Itinerary Itinerary;
	std::string StayID;
	structCSM002_DepartureConveyanceFacility	DepartureConveyanceFacility;
	structCSM002_ArrivalConveyanceFacility		ArrivalConveyanceFacility;
};

struct structCSM007_Declaration
{
	struct structCSM007_BorderTransPortMeans		BorderTransPortMeans;
	struct structCSM002_Agent						Agent;
	std::string IssueDate;
	struct structCSM002_AdditionalInformation		AdditionalInformation;
};

struct structCSM007_MessageBody
{
	struct structCSM007_Declaration Declaration;
};

struct structCSM007
{
	structCSM001_MessageHead		 	MessageHead;
	structCSM007_MessageBody 			MessageBody;
};

#endif