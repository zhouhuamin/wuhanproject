#pragma once
#include <string>

class CMessageType
{
public:
	CMessageType(void);
	~CMessageType(void);

public:
	static const std::string CLM10041;	// 集装箱卸船指令
	static const std::string CLM10042;	// 集装箱卸船指令回执
	static const std::string CLM10011;	// 集装箱提箱指令
	static const std::string CLM10012;	// 集装箱提箱指令回执

	static const std::string MT2101;	// 海运预配舱单
	static const std::string MT3101;	// 海运出口运抵
	static const std::string MT4101;	// 海运出口装载
	static const std::string MT5101;	// 海运进口理货
	static const std::string MT5102;	// 海运出口理货
	static const std::string MTXXXX;	// 

	static const std::string CSM001;	// 船舶备案申报报文
	static const std::string CSM002;	// 水运进境预报申报报文
	static const std::string CSM003;	// 水运进境确报申报报文
	static const std::string CSM004;	// 水运进境抵港申报报文
	static const std::string CSM005;	// 水运在港动态申报报文
	static const std::string CSM006;	// 水运出境预报申报报文
	static const std::string CSM007;	// 水运出境确报申报报文
	static const std::string CSM301;	// 水运回执报文

	static const std::string CLM10021;	
	static const std::string CLM10022;	
	static const std::string CLM10031;	
	static const std::string CLM10032;	
	static const std::string CLM10051;	
	static const std::string CLM10052;
	static const std::string CLM10061;	
	static const std::string CLM10062;

	static const std::string CLM20011;	
	static const std::string CLM20012;
	static const std::string CLM20013;	
	static const std::string CLM20014;
	static const std::string CLM20021;	
	static const std::string CLM20022;
	static const std::string CLM30011;	
	static const std::string CLM30012;

	static const std::string CLM40011;	
	static const std::string CLM40012;
	static const std::string CLM40021;	
	static const std::string CLM40022;
	static const std::string CLM40031;	
	static const std::string CLM40032;

	static const std::string CLM50011;	
	static const std::string CLM50012;
	static const std::string CLM50031;	
	static const std::string CLM50032;
	static const std::string CLM50033;	
	static const std::string CLM50034;
	static const std::string CLM50041;	
	static const std::string CLM50042;

	static const std::string CLM60011;	
	static const std::string CLM60012;
	static const std::string CLM60013;	
	static const std::string CLM60014;

	static const std::string CLM90011;
	static const std::string CLM90012;
	static const std::string CLM90021;
	static const std::string CLM90022;
	static const std::string CLM90031;
	static const std::string CLM90032;
	static const std::string CLM90041;
	static const std::string CLM90042;

	static const std::string CLM32101;
};
