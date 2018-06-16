#ifndef STRUCTDEF_H
#define STRUCTDEF_H

struct structShipInfo
{
	char szSHIP_IMO[10 + 1];
	char szSHIP_NAME_EN[130 + 1];
	char szSHIP_ID[10 + 1];
	char szSHIP_NATIONALITY[10 + 1];
	char szSHIP_TYPE[10 + 1];
	int nSHIP_LENGTH;
	int nSHIP_WIDTH;

	structShipInfo();
};

bool operator<(const structShipInfo &lhs, const structShipInfo &rhs);

struct structShipPosition
{
	char szSHIP_MMSI[10 + 1];
	char szSHIP_IMO[10 + 1];
	char szSHIP_DRAW[10 + 1];
	double dSHIP_LATITUDE;
	double dSHIP_LONGITUDE;
	char szSHIP_DIRECTION[10 + 1];
	char szSHIP_SIGN[10 + 1];
	int nSHIP_SPEED;
	char szSHIP_DESTINATION[30 + 1];
	char szEXPECTED_DATE_ARRIVAL[30 + 1];

	structShipPosition();
	bool operator == (const structShipPosition& rhs);
};

bool operator<(const structShipPosition &lhs, const structShipPosition &rhs);

struct structShipInfoTmp
{
	char szSHIP_MMSI[10 + 1];
	char szSHIP_IMO[10 + 1];
	char szSHIP_NAME_EN[130 + 1];
	char szSHIP_ID[10 + 1];
	char szSHIP_NATIONALITY[50 + 1];
	char szSHIP_TYPE[50 + 1];
	int nSHIP_LENGTH;
	int nSHIP_WIDTH;
	double szSHIP_DRAW;
	double dSHIP_LATITUDE;
	double dSHIP_LONGITUDE;
	char szSHIP_DIRECTION[10 + 1];
	char szSHIP_SIGN[10 + 1];
	double nSHIP_SPEED;
	char szSHIP_DESTINATION[30 + 1];
	char szTimeReserve[30 + 1];
	char szEXPECTED_DATE_ARRIVAL[30 + 1];
	structShipInfoTmp();
};

#endif