#include "StructDef.h"
#include <string.h>

structShipInfo::structShipInfo()
{
	memset(szSHIP_IMO, 0x00, sizeof(szSHIP_IMO));
	memset(szSHIP_NAME_EN, 0x00, sizeof(szSHIP_NAME_EN));
	memset(szSHIP_ID, 0x00, sizeof(szSHIP_ID));
	memset(szSHIP_NATIONALITY, 0x00, sizeof(szSHIP_NATIONALITY));
	memset(szSHIP_TYPE, 0x00, sizeof(szSHIP_TYPE));
	nSHIP_LENGTH = 0;
	nSHIP_WIDTH = 0;
}


structShipPosition::structShipPosition()
{
	memset(szSHIP_IMO, 0x00, sizeof(szSHIP_IMO));
	memset(szSHIP_MMSI, 0x00, sizeof(szSHIP_MMSI));
	memset(szSHIP_DRAW, 0x00, sizeof(szSHIP_DRAW));
	dSHIP_LATITUDE = 0.0f;
	dSHIP_LONGITUDE = 0.0f;
	memset(szSHIP_DIRECTION, 0x00, sizeof(szSHIP_DIRECTION));
	memset(szSHIP_SIGN, 0x00, sizeof(szSHIP_SIGN));
	nSHIP_SPEED = 0;
	memset(szSHIP_DESTINATION, 0x00, sizeof(szSHIP_DESTINATION));
	memset(szEXPECTED_DATE_ARRIVAL, 0x00, sizeof(szEXPECTED_DATE_ARRIVAL));
}

structShipInfoTmp::structShipInfoTmp()
{
	memset(szSHIP_IMO, 0x00, sizeof(szSHIP_IMO));
	memset(szSHIP_NAME_EN, 0x00, sizeof(szSHIP_NAME_EN));
	memset(szSHIP_ID, 0x00, sizeof(szSHIP_ID));
	memset(szSHIP_NATIONALITY, 0x00, sizeof(szSHIP_NATIONALITY));
	memset(szSHIP_TYPE, 0x00, sizeof(szSHIP_TYPE));
	nSHIP_LENGTH = 0;
	nSHIP_WIDTH = 0;

	memset(szSHIP_MMSI, 0x00, sizeof(szSHIP_MMSI));
//	memset(szSHIP_DRAW, 0x00, sizeof(szSHIP_DRAW));
	szSHIP_DRAW = 0.0f;
	dSHIP_LATITUDE = 0.0f;
	dSHIP_LONGITUDE = 0.0f;
	memset(szSHIP_DIRECTION, 0x00, sizeof(szSHIP_DIRECTION));
	memset(szSHIP_SIGN, 0x00, sizeof(szSHIP_SIGN));
	nSHIP_SPEED = 0;
	memset(szSHIP_DESTINATION, 0x00, sizeof(szSHIP_DESTINATION));
	memset(szTimeReserve, 0x00, sizeof(szTimeReserve));
	memset(szEXPECTED_DATE_ARRIVAL, 0x00, sizeof(szEXPECTED_DATE_ARRIVAL));
}


bool structShipPosition::operator ==(const structShipPosition &rhs)
{
	int nSHIP_LATITUDEL = (int)(dSHIP_LATITUDE * 1000);
	int nSHIP_LATITUDER = (int)(rhs.dSHIP_LATITUDE * 1000);
	int ndSHIP_LONGITUDEL = (int)(dSHIP_LONGITUDE * 1000);
	int ndSHIP_LONGITUDER = (int)(rhs.dSHIP_LONGITUDE * 1000);

	if (strcmp(szSHIP_IMO, rhs.szSHIP_IMO) == 0 && nSHIP_LATITUDEL == nSHIP_LATITUDER && ndSHIP_LONGITUDEL == ndSHIP_LONGITUDER \
				&& strcmp(szSHIP_DIRECTION, rhs.szSHIP_DIRECTION) == 0 && strcmp(szSHIP_SIGN, rhs.szSHIP_SIGN) == 0 \
				&& nSHIP_SPEED == rhs.nSHIP_SPEED && strcmp(szSHIP_DESTINATION, rhs.szSHIP_DESTINATION) == 0 && strcmp(szEXPECTED_DATE_ARRIVAL, rhs.szEXPECTED_DATE_ARRIVAL) == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool operator<(const structShipInfo &lhs, const structShipInfo &rhs)
{
	if (strcmp(lhs.szSHIP_IMO, rhs.szSHIP_IMO) < 0)
	{
		return true;
	}

	return false;
}

bool operator<(const structShipPosition &lhs, const structShipPosition &rhs)
{
	if (strcmp(lhs.szSHIP_IMO, rhs.szSHIP_IMO) < 0)
	{
		return true;
	}

	return false;
}


































