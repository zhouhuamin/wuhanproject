// GatherInfoTransact.cpp: implementation of the CGatherInfoTransact class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DataTransact.h"
#include "GatherInfoTransact.h"
#include "tinyxml/tinyxml.h"
#include "DataInterviewClass.h"
#include <boost/shared_array.hpp>
#include "CxLog.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGatherInfoTransact::CGatherInfoTransact():m_ChnlGatherData(),m_CommandInfo()
{
		
}

CGatherInfoTransact::~CGatherInfoTransact()
{

}

BOOL CGatherInfoTransact::OnDealData(CBasicConfigInfo &PackInfo)
{
	CStringArray messageIDArray;
	CStringArray contaNOArray;

	OnParse(PackInfo);
	OnSaveInfo(PackInfo);
	OnGetRelInfo(PackInfo, messageIDArray, contaNOArray);
	OnSendRelToClient(PackInfo, messageIDArray, contaNOArray);
	OnUpdateRelInfo(PackInfo);
	return TRUE;
}

BOOL CGatherInfoTransact::OnParse(CBasicConfigInfo &PackInfo)
{
	// 1. 解析
/*
<?xml version="1.0" encoding="GB2312"?>
<ChnlConfigInfo Opera="Add">	-- Add,Del,Mod,Que
<CustumsName></CustumsName>
<AreaName></AreaName>
<ChnlName></ChnlName>
<IEType></IEType>
<SealTag></SealTag>	-- 有无
<RecordCode></RecordCode>
<ChnlType></ChnlType>
<BusiType></BusiType>
<ChnlStatus></ChnlStatus> -- 1:使用,0:不使用
<ChnlBuCai>	
<VehicleTag></VehicleTag>	-- 1:补采,0:不补采
<ContainerTag></ContainerTag>
<WeightTag></WeightTag>
<ICTag></ICTag>
<ESealTag></ESealTag>
<GPSTag></GPSTag>
</ChnlBuCai>
<ChnlMode>
<ModeType></ModeType>
<ModeName></ModeName>
<DeviceDescript></DeviceDescript>
<SequenceDescript></SequenceDescript>
</ChnlMode>
<OpID></OpID>
<OpDate></OpDate>
</ChnlConfigInfo>

*/
	int iXmlLen = 0;
	if (PackInfo.nAllLength > 38)
	{
		memcpy(&iXmlLen, PackInfo.pPacket + 34, sizeof(iXmlLen));
	}
	else
	{
		return FALSE;
	}

	if (iXmlLen <= 0)
		return FALSE;

	char *pXmlBuffer = new char[iXmlLen + 1];
	if (pXmlBuffer == NULL)
	{
		return FALSE;
	}
	memcpy(pXmlBuffer, PackInfo.pPacket + 38, iXmlLen);
	pXmlBuffer[iXmlLen] = '\0';
	CString strXmlData(pXmlBuffer);
	delete []pXmlBuffer;

	TiXmlDocument doc;
	doc.Parse(LPCTSTR(strXmlData));

	TiXmlHandle docHandle(&doc);
	TiXmlHandle GATHER_INFOHandle		= docHandle.FirstChildElement("GATHER_INFO");
	TiXmlHandle DR_IC_NOHandle			= docHandle.FirstChildElement("GATHER_INFO").FirstChildElement("IC").ChildElement("DR_IC_NO", 0).FirstChild();
	TiXmlHandle IC_DR_CUSTOMS_NOHandle	= docHandle.FirstChildElement("GATHER_INFO").FirstChildElement("IC").ChildElement("IC_DR_CUSTOMS_NO", 0).FirstChild();
	TiXmlHandle IC_CO_CUSTOMS_NOHandle	= docHandle.FirstChildElement("GATHER_INFO").FirstChildElement("IC").ChildElement("IC_CO_CUSTOMS_NO", 0).FirstChild();
	TiXmlHandle IC_BILL_NOHandle		= docHandle.FirstChildElement("GATHER_INFO").FirstChildElement("IC").ChildElement("IC_BILL_NO", 0).FirstChild();
	TiXmlHandle IC_GROSS_WTHandle		= docHandle.FirstChildElement("GATHER_INFO").FirstChildElement("IC").ChildElement("IC_GROSS_WT", 0).FirstChild();
	TiXmlHandle IC_VE_CUSTOMS_NOHandle	= docHandle.FirstChildElement("GATHER_INFO").FirstChildElement("IC").ChildElement("IC_VE_CUSTOMS_NO", 0).FirstChild();
	TiXmlHandle IC_VE_NAMEHandle		= docHandle.FirstChildElement("GATHER_INFO").FirstChildElement("IC").ChildElement("IC_VE_NAME", 0).FirstChild();
	TiXmlHandle IC_CONTA_IDHandle		= docHandle.FirstChildElement("GATHER_INFO").FirstChildElement("IC").ChildElement("IC_CONTA_ID", 0).FirstChild();
	TiXmlHandle IC_ESEAL_IDHandle		= docHandle.FirstChildElement("GATHER_INFO").FirstChildElement("IC").ChildElement("IC_ESEAL_ID", 0).FirstChild();
	TiXmlHandle IC_BUSS_TYPEHandle		= docHandle.FirstChildElement("GATHER_INFO").FirstChildElement("IC").ChildElement("IC_BUSS_TYPE", 0).FirstChild();
	TiXmlHandle IC_EX_DATAHandle		= docHandle.FirstChildElement("GATHER_INFO").FirstChildElement("IC").ChildElement("IC_EX_DATA", 0).FirstChild();

	TiXmlHandle GROSS_WTHandle			= docHandle.FirstChildElement("GATHER_INFO").FirstChildElement("WEIGHT").ChildElement("GROSS_WT", 0).FirstChild();

	TiXmlHandle VE_NAMEHandle			= docHandle.FirstChildElement("GATHER_INFO").FirstChildElement("CAR").ChildElement("VE_NAME", 0).FirstChild();
	TiXmlHandle CAR_EC_NOHandle			= docHandle.FirstChildElement("GATHER_INFO").FirstChildElement("CAR").ChildElement("CAR_EC_NO", 0).FirstChild();
	TiXmlHandle CAR_EC_NO2Handle		= docHandle.FirstChildElement("GATHER_INFO").FirstChildElement("CAR").ChildElement("CAR_EC_NO2", 0).FirstChild();
	TiXmlHandle VE_CUSTOMS_NOHandle		= docHandle.FirstChildElement("GATHER_INFO").FirstChildElement("CAR").ChildElement("VE_CUSTOMS_NO", 0).FirstChild();
	TiXmlHandle VE_WTHandle				= docHandle.FirstChildElement("GATHER_INFO").FirstChildElement("CAR").ChildElement("VE_WT", 0).FirstChild();


	TiXmlHandle CONTA_NUMHandle			= docHandle.FirstChildElement("GATHER_INFO").FirstChildElement("CONTA").ChildElement("CONTA_NUM", 0).FirstChild();
	TiXmlHandle CONTA_RECOHandle		= docHandle.FirstChildElement("GATHER_INFO").FirstChildElement("CONTA").ChildElement("CONTA_RECO", 0).FirstChild();
	TiXmlHandle CONTA_ID_FHandle		= docHandle.FirstChildElement("GATHER_INFO").FirstChildElement("CONTA").ChildElement("CONTA_ID_F", 0).FirstChild();
	TiXmlHandle CONTA_ID_BHandle		= docHandle.FirstChildElement("GATHER_INFO").FirstChildElement("CONTA").ChildElement("CONTA_ID_B", 0).FirstChild();

	TiXmlHandle ESEAL_IDHandle			= docHandle.FirstChildElement("GATHER_INFO").FirstChildElement("SEAL").ChildElement("ESEAL_ID", 0).FirstChild();
	TiXmlHandle BAR_CODEHandle			= docHandle.FirstChildElement("GATHER_INFO").ChildElement("BAR_CODE", 0).FirstChild();

	CString AREA_ID		= "";
	CString CHNL_NO		= "";
	CString I_E_TYPE	= "";
	CString SEQ_NO		= "";

	CString DR_IC_NO			= "";
	CString IC_DR_CUSTOMS_NO	= "";
	CString IC_CO_CUSTOMS_NO	= "";
	CString IC_BILL_NO			= "";
	CString IC_GROSS_WT			= "";
	CString IC_VE_CUSTOMS_NO	= "";
	CString IC_VE_NAME			= "";
	CString IC_CONTA_ID			= "";
	CString IC_ESEAL_ID			= "";
	CString IC_BUSS_TYPE		= "";
	CString IC_EX_DATA			= "";
	CString GROSS_WT			= "";
	CString VE_NAME				= "";
	CString CAR_EC_NO			= "";
	CString CAR_EC_NO2			= "";
	CString VE_CUSTOMS_NO		= "";
	CString VE_WT				= "";
	CString CONTA_NUM			= "";
	CString CONTA_RECO			= "";
	CString CONTA_ID_F			= "";
	CString CONTA_ID_B			= "";
	CString ESEAL_ID			= "";
	CString BAR_CODE			= "";


	if (GATHER_INFOHandle.Node() != NULL)
		AREA_ID 		= GATHER_INFOHandle.Element()->Attribute("AREA_ID");
	if (GATHER_INFOHandle.Node() != NULL)
		CHNL_NO 		= GATHER_INFOHandle.Element()->Attribute("CHNL_NO");
	if (GATHER_INFOHandle.Node() != NULL)
		I_E_TYPE 		= GATHER_INFOHandle.Element()->Attribute("I_E_TYPE");
	if (GATHER_INFOHandle.Node() != NULL)
		SEQ_NO 		= GATHER_INFOHandle.Element()->Attribute("SEQ_NO");

	if (DR_IC_NOHandle.Node() != NULL)	
		DR_IC_NO 			= DR_IC_NOHandle.Text()->Value();
	if (IC_DR_CUSTOMS_NOHandle.Node() != NULL)		
		IC_DR_CUSTOMS_NO	= IC_DR_CUSTOMS_NOHandle.Text()->Value();
	if (IC_CO_CUSTOMS_NOHandle.Node() != NULL)		
		IC_CO_CUSTOMS_NO 		= IC_CO_CUSTOMS_NOHandle.Text()->Value();
	if (IC_BILL_NOHandle.Node() != NULL)		
		IC_BILL_NO		= IC_BILL_NOHandle.Text()->Value();
	if (IC_GROSS_WTHandle.Node() != NULL)		
		IC_GROSS_WT		= IC_GROSS_WTHandle.Text()->Value();
	if (IC_VE_CUSTOMS_NOHandle.Node() != NULL)			
		IC_VE_CUSTOMS_NO 		= IC_VE_CUSTOMS_NOHandle.Text()->Value();
	if (IC_VE_NAMEHandle.Node() != NULL)		
		IC_VE_NAME 		= IC_VE_NAMEHandle.Text()->Value();
	if (IC_CONTA_IDHandle.Node() != NULL)		
		IC_CONTA_ID 		= IC_CONTA_IDHandle.Text()->Value();
	if (IC_ESEAL_IDHandle.Node() != NULL)			
		IC_ESEAL_ID 		= IC_ESEAL_IDHandle.Text()->Value();
	if (IC_BUSS_TYPEHandle.Node() != NULL)		
		IC_BUSS_TYPE 		= IC_BUSS_TYPEHandle.Text()->Value();
	if (IC_EX_DATAHandle.Node() != NULL)		
		IC_EX_DATA 	= IC_EX_DATAHandle.Text()->Value();
	if (GROSS_WTHandle.Node() != NULL)			
		GROSS_WT 		= GROSS_WTHandle.Text()->Value();
	if (VE_NAMEHandle.Node() != NULL)		
		VE_NAME 		= VE_NAMEHandle.Text()->Value();
	if (CAR_EC_NOHandle.Node() != NULL)		
		CAR_EC_NO 		= CAR_EC_NOHandle.Text()->Value();
	if (CAR_EC_NO2Handle.Node() != NULL)		
		CAR_EC_NO2 		= CAR_EC_NO2Handle.Text()->Value();
	if (VE_CUSTOMS_NOHandle.Node() != NULL)		
		VE_CUSTOMS_NO 		= VE_CUSTOMS_NOHandle.Text()->Value();
	if (VE_WTHandle.Node() != NULL)		
		VE_WT 		= VE_WTHandle.Text()->Value();
	if (CONTA_NUMHandle.Node() != NULL)		
		CONTA_NUM 	= CONTA_NUMHandle.Text()->Value();
	if (CONTA_RECOHandle.Node() != NULL)		
		CONTA_RECO 	= CONTA_RECOHandle.Text()->Value();
	if (CONTA_ID_FHandle.Node() != NULL)		
		CONTA_ID_F 		= CONTA_ID_FHandle.Text()->Value();
	if (CONTA_ID_BHandle.Node() != NULL)			
		CONTA_ID_B 		= CONTA_ID_BHandle.Text()->Value();	
	if (ESEAL_IDHandle.Node() != NULL)			
		ESEAL_ID 		= ESEAL_IDHandle.Text()->Value();	
	if (BAR_CODEHandle.Node() != NULL)			
		BAR_CODE 		= BAR_CODEHandle.Text()->Value();	

	memcpy(m_ChnlGatherData.szAREA_ID, (LPCTSTR)AREA_ID, AREA_ID.GetLength() < sizeof(m_ChnlGatherData.szAREA_ID) ? AREA_ID.GetLength() : sizeof(m_ChnlGatherData.szAREA_ID) - 1);
	memcpy(m_ChnlGatherData.szCHNL_NO, (LPCTSTR)CHNL_NO, CHNL_NO.GetLength() < sizeof(m_ChnlGatherData.szCHNL_NO) ? CHNL_NO.GetLength() : sizeof(m_ChnlGatherData.szCHNL_NO) - 1);

	m_ChnlGatherData.byI_E_TYPE = I_E_TYPE.GetLength() == 1 ? I_E_TYPE[0] : ' ';
	memcpy(m_ChnlGatherData.szSEQ_NO, (LPCTSTR)SEQ_NO, sizeof(m_ChnlGatherData.szSEQ_NO) - 1);
	memcpy(m_ChnlGatherData.szDR_IC_NO, (LPCTSTR)DR_IC_NO, DR_IC_NO.GetLength() < sizeof(m_ChnlGatherData.szDR_IC_NO) ? DR_IC_NO.GetLength() : sizeof(m_ChnlGatherData.szDR_IC_NO) - 1);//sizeof(m_ChnlGatherData.szDR_IC_NO) - 1);
	memcpy(m_ChnlGatherData.szIC_DR_CUSTOMS_NO, (LPCTSTR)IC_DR_CUSTOMS_NO, IC_DR_CUSTOMS_NO.GetLength() < sizeof(m_ChnlGatherData.szIC_DR_CUSTOMS_NO) ? IC_DR_CUSTOMS_NO.GetLength() : sizeof(m_ChnlGatherData.szIC_DR_CUSTOMS_NO) - 1);//sizeof(m_ChnlGatherData.szIC_DR_CUSTOMS_NO) - 1);
	memcpy(m_ChnlGatherData.szIC_CO_CUSTOMS_NO, (LPCTSTR)IC_CO_CUSTOMS_NO, IC_CO_CUSTOMS_NO.GetLength() < sizeof(m_ChnlGatherData.szIC_CO_CUSTOMS_NO) ? IC_CO_CUSTOMS_NO.GetLength() : sizeof(m_ChnlGatherData.szIC_CO_CUSTOMS_NO) - 1);//sizeof(m_ChnlGatherData.szIC_CO_CUSTOMS_NO) - 1);
	memcpy(m_ChnlGatherData.szIC_BILL_NO, (LPCTSTR)IC_BILL_NO, IC_BILL_NO.GetLength() < sizeof(m_ChnlGatherData.szIC_BILL_NO) ? IC_BILL_NO.GetLength() : sizeof(m_ChnlGatherData.szIC_BILL_NO) - 1);//sizeof(m_ChnlGatherData.szIC_BILL_NO) - 1);
	m_ChnlGatherData.IC_GROSS_WT = (float)atof((LPCTSTR)IC_GROSS_WT);
	memcpy(m_ChnlGatherData.szIC_VE_CUSTOMS_NO, (LPCTSTR)IC_VE_CUSTOMS_NO, IC_VE_CUSTOMS_NO.GetLength() < sizeof(m_ChnlGatherData.szIC_VE_CUSTOMS_NO) ? IC_VE_CUSTOMS_NO.GetLength() : sizeof(m_ChnlGatherData.szIC_VE_CUSTOMS_NO) - 1);//sizeof(m_ChnlGatherData.szIC_VE_CUSTOMS_NO) - 1);
	memcpy(m_ChnlGatherData.szIC_VE_NAME, (LPCTSTR)IC_VE_NAME, IC_VE_NAME.GetLength() < sizeof(m_ChnlGatherData.szIC_VE_NAME) ? IC_VE_NAME.GetLength() : sizeof(m_ChnlGatherData.szIC_VE_NAME) - 1);//sizeof(m_ChnlGatherData.szIC_VE_NAME) - 1);
	memcpy(m_ChnlGatherData.szIC_CONTA_ID, (LPCTSTR)IC_CONTA_ID, IC_CONTA_ID.GetLength() < sizeof(m_ChnlGatherData.szIC_CONTA_ID) ? IC_CONTA_ID.GetLength() : sizeof(m_ChnlGatherData.szIC_CONTA_ID) - 1);//sizeof(m_ChnlGatherData.szIC_CONTA_ID) - 1);
	memcpy(m_ChnlGatherData.szIC_ESEAL_ID, (LPCTSTR)IC_ESEAL_ID, IC_ESEAL_ID.GetLength() < sizeof(m_ChnlGatherData.szIC_ESEAL_ID) ? IC_ESEAL_ID.GetLength() : sizeof(m_ChnlGatherData.szIC_ESEAL_ID) - 1);//sizeof(m_ChnlGatherData.szIC_ESEAL_ID) - 1);
	memcpy(m_ChnlGatherData.szIC_BUSS_TYPE, (LPCTSTR)IC_BUSS_TYPE, IC_BUSS_TYPE.GetLength() < sizeof(m_ChnlGatherData.szIC_BUSS_TYPE) ? IC_BUSS_TYPE.GetLength() : sizeof(m_ChnlGatherData.szIC_BUSS_TYPE) - 1);//sizeof(m_ChnlGatherData.szIC_BUSS_TYPE) - 1);
	memcpy(m_ChnlGatherData.szIC_EX_DATA, (LPCTSTR)IC_EX_DATA, IC_EX_DATA.GetLength() < sizeof(m_ChnlGatherData.szIC_EX_DATA) ? IC_EX_DATA.GetLength() : sizeof(m_ChnlGatherData.szIC_EX_DATA) - 1);//sizeof(m_ChnlGatherData.szIC_EX_DATA) - 1);
	m_ChnlGatherData.GROSS_WT = (float)atof((LPCTSTR)GROSS_WT);
	memcpy(m_ChnlGatherData.szVE_NAME, (LPCTSTR)VE_NAME, VE_NAME.GetLength() < sizeof(m_ChnlGatherData.szVE_NAME) ? VE_NAME.GetLength() : sizeof(m_ChnlGatherData.szVE_NAME) - 1);//sizeof(m_ChnlGatherData.szVE_NAME) - 1);
	memcpy(m_ChnlGatherData.szCAR_EC_NO, (LPCTSTR)CAR_EC_NO, CAR_EC_NO.GetLength() < sizeof(m_ChnlGatherData.szCAR_EC_NO) ? CAR_EC_NO.GetLength() : sizeof(m_ChnlGatherData.szCAR_EC_NO) - 1);//sizeof(m_ChnlGatherData.szCAR_EC_NO) - 1);
	memcpy(m_ChnlGatherData.szCAR_EC_NO2, (LPCTSTR)CAR_EC_NO2, CAR_EC_NO2.GetLength() < sizeof(m_ChnlGatherData.szCAR_EC_NO2) ? CAR_EC_NO2.GetLength() : sizeof(m_ChnlGatherData.szCAR_EC_NO2) - 1);//sizeof(m_ChnlGatherData.szCAR_EC_NO2) - 1);
	memcpy(m_ChnlGatherData.szVE_CUSTOMS_NO, (LPCTSTR)VE_CUSTOMS_NO, VE_CUSTOMS_NO.GetLength() < sizeof(m_ChnlGatherData.szVE_CUSTOMS_NO) ? VE_CUSTOMS_NO.GetLength() : sizeof(m_ChnlGatherData.szVE_CUSTOMS_NO) - 1);//sizeof(m_ChnlGatherData.szVE_CUSTOMS_NO) - 1);
	m_ChnlGatherData.VE_WT = (float)atof((LPCTSTR)VE_WT);
	m_ChnlGatherData.nCONTA_NUM = atoi((LPCTSTR)CONTA_NUM);
	m_ChnlGatherData.byCONTA_RECO = CONTA_RECO.GetLength() == 1 ? CONTA_RECO[0] : ' ';
	memcpy(m_ChnlGatherData.szCONTA_ID_F, (LPCTSTR)CONTA_ID_F, CONTA_ID_F.GetLength() < sizeof(m_ChnlGatherData.szCONTA_ID_F) ? CONTA_ID_F.GetLength() : sizeof(m_ChnlGatherData.szCONTA_ID_F) - 1);//sizeof(m_ChnlGatherData.szCONTA_ID_F) - 1);
	memcpy(m_ChnlGatherData.szCONTA_ID_B, (LPCTSTR)CONTA_ID_B, CONTA_ID_B.GetLength() < sizeof(m_ChnlGatherData.szCONTA_ID_B) ? CONTA_ID_B.GetLength() : sizeof(m_ChnlGatherData.szCONTA_ID_B) - 1);//sizeof(m_ChnlGatherData.szCONTA_ID_B) - 1);
	memcpy(m_ChnlGatherData.szESEAL_ID, (LPCTSTR)ESEAL_ID, ESEAL_ID.GetLength() < sizeof(m_ChnlGatherData.szESEAL_ID) ? ESEAL_ID.GetLength() : sizeof(m_ChnlGatherData.szESEAL_ID) - 1);//sizeof(m_ChnlGatherData.szESEAL_ID) - 1);

	memcpy(m_ChnlGatherData.szBAR_CODE, (LPCTSTR)BAR_CODE, BAR_CODE.GetLength() < sizeof(m_ChnlGatherData.szBAR_CODE) ? BAR_CODE.GetLength() : sizeof(m_ChnlGatherData.szBAR_CODE) - 1);


	return TRUE;
}

BOOL CGatherInfoTransact::OnSaveInfo(CBasicConfigInfo &PackInfo)
{
	DataInterviewClass DataInterview;
	DataInterview.SetConnectParam(PackInfo.strDataUser, PackInfo.strDataPaw, PackInfo.strDataServer);

	if(!DataInterview.InsertGateGatherData(m_ChnlGatherData))
	{

	}

	return TRUE;
}

BOOL CGatherInfoTransact::OnGetRelInfo(CBasicConfigInfo &PackInfo, CStringArray &messageIDArray, CStringArray &contaArray)
{
	memcpy(m_CommandInfo.AREA_ID, m_ChnlGatherData.szAREA_ID, sizeof(m_CommandInfo.AREA_ID) - 1);
	memcpy(m_CommandInfo.CHNL_NO, m_ChnlGatherData.szCHNL_NO, sizeof(m_CommandInfo.CHNL_NO) - 1);
	m_CommandInfo.I_E_TYPE = m_ChnlGatherData.byI_E_TYPE;
	memcpy(m_CommandInfo.SEQ_NO, m_ChnlGatherData.szSEQ_NO, sizeof(m_CommandInfo.SEQ_NO) - 1);

	if (m_CommandInfo.I_E_TYPE == 'I')
	{
		CString  strCheckResult = "00000000000000000000";
		CString strOPHint = "直接放行";
		memcpy(m_CommandInfo.CHECK_RESULT, (LPCTSTR)strCheckResult, sizeof(m_CommandInfo.CHECK_RESULT) - 1);
		memcpy(m_CommandInfo.OP_HINT, (LPCTSTR)strOPHint, sizeof(m_CommandInfo.OP_HINT) - 1);
	}
	else
	{
		CString szChkResult = "11000000000000000000";
		CString strOPHint = "禁止通行";
		OnGetRelInfo(PackInfo, messageIDArray, contaArray, szChkResult, strOPHint);

		memcpy(m_CommandInfo.CHECK_RESULT, (LPCTSTR)szChkResult, sizeof(m_CommandInfo.CHECK_RESULT) - 1);
		memcpy(m_CommandInfo.OP_HINT, (LPCTSTR)strOPHint, sizeof(m_CommandInfo.OP_HINT) - 1);
	}



	return TRUE;
}

BOOL CGatherInfoTransact::OnSendRelToClient(CBasicConfigInfo &PackInfo, CStringArray &messageIDArray, CStringArray &contaArray)
{
	try
	{

		CString strESealID = "",strESealKey = "";
		//解包
		CString strCommandXml;
		CString strChkTmp;
		
		const char CIQPackageHeader[4]={(char)0xE2, (char)0x5C, (char)0x4B, (char)0x89}; //包头标记
		const char PackageTail[2]={(char)0xFF, (char)0xFF};

		CString strTmp = PackInfo.strLpPort + PackInfo.strChannel+PackInfo.strIOFlag;
		CString strShowMsg;

		//4+4+10+10+1+1+2   new 4+4+1+10+10+1+4+2
		strCommandXml.Format("<?xml version=\"1.0\" encoding=\"GB2312\"?>\r\n<COMMAND_INFO AREA_ID=\"%s\"  CHNL_NO=\"%s\"  I_E_TYPE=\"%s\" SEQ_NO=\"%s\">\
<CHECK_RESULT>%s</CHECK_RESULT>\
<GPS>\
<VE_NAME></VE_NAME>\
<GPS_ID></GPS_ID>\
<ORIGIN_CUSTOMS></ORIGIN_CUSTOMS>\
<DEST_CUSTOMS></DEST_CUSTOMS>\
</GPS>\
<SEAL>\
<ESEAL_ID>%s</ESEAL_ID>\
<SEAL_KEY>%s</SEAL_KEY>\
</SEAL>\
<FORM_ID></FORM_ID>\
<OP_HINT>%s</OP_HINT>\
</COMMAND_INFO>",
PackInfo.strLpPort,
PackInfo.strChannel,
PackInfo.strIOFlag,
m_CommandInfo.SEQ_NO,
m_CommandInfo.CHECK_RESULT,
strESealID,
strESealKey,
m_CommandInfo.OP_HINT);


		int xmllen=strCommandXml.GetLength();
		int PackDataLen = _PACKETHEAD_LEN+_PACKET_XMLLEN_SIZE+xmllen+_PACKETTAIL_LEN;

		boost::shared_array<BYTE> spPacket;
		spPacket.reset(new BYTE[PackDataLen+1]);
		ZeroMemory(spPacket.get(), PackDataLen+1);

		BYTE *p = spPacket.get();
		//包头标记
		memcpy(p, CIQPackageHeader ,4);
		p += 4;

		//总长	
		memcpy(p, (BYTE*)&PackDataLen, sizeof(int));
		p += 4;

		//消息类型
		PackInfo.szMsg = _WLJKChannlRelCmd;
		memcpy(p, (BYTE*)&PackInfo.szMsg, 1);
		p += 1;	

		//场站号
		memcpy(p, (LPCTSTR)PackInfo.strLpPort, PackInfo.strLpPort.GetLength());
		p += 10;

		//通道号
		memcpy(p, (LPCTSTR)PackInfo.strChannel, PackInfo.strChannel.GetLength());
		p += 10;

		char szIO[2] = {0};
		strcpy_s(szIO, sizeof(szIO), PackInfo.strIOFlag);
		//进出标志
		memcpy(p, szIO, 1);
		p += 1;							

		//标识	
		memset(p, 0xFF, 4);
		p += 4;	



		memcpy(p,&xmllen,4);
		p+=4;
		memcpy(p, (LPCTSTR)strCommandXml, xmllen);
		p+=xmllen;


		//包尾标记
		memcpy(p, PackageTail ,2);

		if(OnBasicDealInfo( (char*)spPacket.get(), PackInfo.strMonitorServerIp, PackInfo.nMonitorPort,PackDataLen,PackInfo))
		{			
			OnUpdateDB(PackInfo, messageIDArray, contaArray);
			TRACE("发送放行指令-->-->数据集中监控中心 "); 
		}
		else
		{
			LOG_ERR("发送放行指令-->-->数据集中监控中心失败!\n");
		}
	}
	catch(...)
	{
		LOG_ERR("异常\n");
		return FALSE;
	}
	return TRUE;
}

BOOL CGatherInfoTransact::OnUpdateDB(CBasicConfigInfo &PackInfo, CStringArray &messageIDArray, CStringArray &contaArray)
{
	if (messageIDArray.GetSize() <= 0)
		return FALSE;

	try
	{
		if (m_ChnlGatherData.nCONTA_NUM == 1)
		{
			char szSql[1000] = {0};
			DataInterviewClass DataInterview;
			DataInterview.SetConnectParam(PackInfo.strDataUser, PackInfo.strDataPaw, PackInfo.strDataServer);
			sprintf_s(szSql, sizeof(szSql), "update D_RELEASE_DICTATE_CONTA set REL_FLAG = '1' where MESSAGE_ID='%s' and CONTA_NO = '%s' and REL_FLAG = '0'", messageIDArray[0], m_ChnlGatherData.szCONTA_ID_F);
			DataInterview.ExecSQL_NoRecord(szSql);
			return TRUE;		
		}
		else if (m_ChnlGatherData.nCONTA_NUM == 2)
		{
			if (messageIDArray.GetSize() <= 1)
				return FALSE;

			char szSql[1000] = {0};
			DataInterviewClass DataInterview;
			DataInterview.SetConnectParam(PackInfo.strDataUser, PackInfo.strDataPaw, PackInfo.strDataServer);

			sprintf_s(szSql, sizeof(szSql), "update D_RELEASE_DICTATE_CONTA set REL_FLAG = '1' where CONTA_NO in ('%s','%s') and REL_FLAG = '0'", contaArray[0], contaArray[1]);
			DataInterview.ExecSQL_NoRecord(szSql);
		}
		else
		{
			;
		}
	}
	catch (...)
	{
		LOG_ERR("CGatherInfoTransact::OnGetRelInfo Exception5\n");
	}

	return TRUE;
}

BOOL CGatherInfoTransact::OnGetRelInfo(CBasicConfigInfo &PackInfo, CStringArray &messageIDArray, CStringArray &contaArray, CString &strCheckResult, CString &strOpHint)
{
	try
	{
		if (m_ChnlGatherData.nCONTA_NUM == 1)
		{
			char szSql[1000];
			memset(szSql,0,sizeof(szSql));

			_RecordsetPtr pRdset;
			DataInterviewClass DataInterview;
			DataInterview.SetConnectParam(PackInfo.strDataUser, PackInfo.strDataPaw, PackInfo.strDataServer);

			sprintf_s(szSql, sizeof(szSql), "select MESSAGE_ID, CONTA_NO from D_RELEASE_DICTATE_CONTA where  CONTA_NO='%s' and REL_FLAG = '0'", m_ChnlGatherData.szCONTA_ID_F);
			pRdset=DataInterview.exeSqlForAllData(szSql);
			if (pRdset == NULL)
			{ 
				strCheckResult = "11000000000000000000";
				strOpHint = "禁止通行";
				return FALSE;
			}
			if(!pRdset->GetadoEOF())
			{
				CString MESSAGE_ID =  (char *)GetRecordsetInfo("MESSAGE_ID",pRdset);
				messageIDArray.Add(MESSAGE_ID);

				CString CONTA_NO =  (char *)GetRecordsetInfo("CONTA_NO", pRdset);
				contaArray.Add(CONTA_NO);
				strCheckResult = "00000000000000000000";
				strOpHint = "直接放行";
			}
			DataInterview.CloseDB();
		}
		else if (m_ChnlGatherData.nCONTA_NUM == 2)
		{
			char szSql[1000];
			memset(szSql,0,sizeof(szSql));

			_RecordsetPtr pRdset;
			DataInterviewClass DataInterview;
			DataInterview.SetConnectParam(PackInfo.strDataUser, PackInfo.strDataPaw, PackInfo.strDataServer);

			sprintf_s(szSql, sizeof(szSql), "select MESSAGE_ID, CONTA_NO from D_RELEASE_DICTATE_CONTA where  CONTA_NO in ('%s','%s') and REL_FLAG = '0'", m_ChnlGatherData.szCONTA_ID_F, m_ChnlGatherData.szCONTA_ID_B);
			pRdset=DataInterview.exeSqlForAllData(szSql);
			if (pRdset == NULL)
			{ 
				strCheckResult = "11000000000000000000";
				strOpHint = "禁止通行";
				return FALSE;
			}

			int nContaNumTmp = 0;
			while (!pRdset->GetadoEOF())
			{
				++nContaNumTmp;
				CString MESSAGE_ID =  (char *)GetRecordsetInfo("MESSAGE_ID", pRdset);
				messageIDArray.Add(MESSAGE_ID);

				CString CONTA_NO =  (char *)GetRecordsetInfo("CONTA_NO", pRdset);
				contaArray.Add(CONTA_NO);
				pRdset->MoveNext();
			}
			DataInterview.CloseDB();

			if (nContaNumTmp == m_ChnlGatherData.nCONTA_NUM)
			{
				strCheckResult = "00000000000000000000";
				strOpHint = "直接放行";
			}
			else
			{
				strCheckResult = "11000000000000000000";
				strOpHint = "禁止通行";
			}
		}
		else
		{
			strCheckResult = "11000000000000000000";
			strOpHint = "禁止通行";
		}
	}
	catch (...)
	{
		LOG_ERR("CGatherInfoTransact::OnGetRelInfo Exception6\n");
	}

	return TRUE;
}

BOOL CGatherInfoTransact::OnUpdateRelInfo(CBasicConfigInfo &PackInfo)
{
	try
	{
		char szSql[1000] = {0};
		DataInterviewClass DataInterview;
		DataInterview.SetConnectParam(PackInfo.strDataUser, PackInfo.strDataPaw, PackInfo.strDataServer);
		sprintf_s(szSql, sizeof(szSql), "update R_GTW_GATHER_INFO set FREE_CMD = '%s' where SEQ_NO='%s'", m_CommandInfo.CHECK_RESULT, m_ChnlGatherData.szSEQ_NO);
		DataInterview.ExecSQL_NoRecord(szSql);
		return TRUE;		
	}
	catch (...)
	{
		LOG_ERR("CGatherInfoTransact::OnUpdateRelInfo Exception5\n");
	}
	return TRUE;
}