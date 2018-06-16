#ifndef SP_STRUCT_H_HH
#define SP_STRUCT_H_HH

//������Э�����ݲɼ���
struct _CHNL_GATHER_DATA
{
	char		szAREA_ID[11];			//��վ����
	char		szCHNL_NO[11];			//ͨ����
	BYTE		byI_E_TYPE;				//��������־
	char		szSEQ_NO[21];			//���к�
	BYTE        byDataReco;
	char        szDR_IC_NO[11];         //��ʻԱIC����
	char		szIC_DR_CUSTOMS_NO[11];	//˾����Ϣ������Ϊ�գ�
	char		szIC_CO_CUSTOMS_NO[11];	//�������ر��
	char        szIC_BILL_NO[200];       //IC�����ص�֤��
	BYTE		byIC_FORM_TYPE;			 //��֤���� 
	char		szIC_REG_DATETIME[20];	//IC������ʱ��
	int			nIC_PER_DAY_DUE;		//��Чʱ�䣨������
	float       IC_GROSS_WT ;           //������������ǩ�����У�����ֵ����Ϊ�գ�
	char        szIC_VE_CUSTOMS_NO[11]; //�������ر��
	char        szIC_VE_NAME[33];       //���ڳ��ƺţ���ǩ�����У�����ֵ����Ϊ�գ�

	char        szIC_CONTA_ID[50];		//���ڼ�װ���
	char        szIC_CONTA_ID_F[17];      //���ڼ�װ���
	char        szIC_CONTA_ID_B[17]; 
	char        szIC_ESEAL_ID[121];     //���ڹ����ţ���ǩ�����У�����ֵ����Ϊ�գ�
	char		szIC_BUSS_TYPE[10 + 1]; // ҵ������
	char        szIC_EX_DATA[200 + 1];  // IC����չ����
	char        szBAR_CODE[50 + 1];

	float		GROSS_WT;				//����
	//   �ѳ�����33-70������ID��17-70��
	char		szVE_NAME[70];			// ���乤�����ƣ����ƺţ�
	char 		szCAR_EC_NO[70];		// ���ӳ��ƺ�
	char		szCAR_EC_NO2[17];		//���ӳ��ƺ�

	char        szVE_CUSTOMS_NO[11];        //���س�����ţ�ֵ����Ϊ�գ�
	float		VE_WT;					//������

	char        szTR_EC_NO[50 + 1];		// �ϳ����ӳ��ƺ�
	char		szTR_NAME[50 + 1];			// �ϳ���
	float       fTR_WT;					// ����

	char        szVE_TYPE[10];			// ���ӳ��Ƴ�������
	//char       szIC_WARE_TYPE[10];		// IC����������
	char		szIDENTIFY_VE_NAME[20 + 1]; // ��ѧͼ��ʶ����ĳ��ƺ�


	int			nCONTA_NUM;				//����
	BYTE		byCONTA_RECO;			//���ʶ����ȷ��ʾ
	char		szCONTA_ID_F[17];		//ǰ���
	char		szCONTA_ID_B[17];		//�����
	char		szCONTA_MODEL_F[10];	//ǰ������
	char		szCONTA_MODEL_B[10];	//��������
	char		szOrder_Num[50];		//ԤԼ��
	BYTE		byDamageLeave_F;			//����̶�
	BYTE		byDamageLeave_B;			//����̶�
	char		szESEAL_ID[121];		//������
	char        szESEAL_IC_NO[10 + 1];  // �����Ŷ�ӦIC����

	char		szESEAL_KEY[21];		//����������Կ
	char		szGPS_ID[21];			//GPS ID��
	BYTE		byMODI_FLAG;			//�˹��޸ı�־
	char		szI_E_TIME[21];			//������ʱ��
	char		byIC_RW_FLAG;			//�Ƿ�дIC����־
	char		szEXC_CMD[50];			//ִ��ָ��
	char		szEXC_RESULT[30];		//���豸�ķ���ָ��ִ�н��
	char		szCHK_RESULT[50];		//�ɼ����ݱȶԷ������
	char		byREL_TYPE;				//���з�ʽ

	char        szUSER_ID[21];          //����Ա����

	char 			pPicB[100];   //��-ץ��ͼ��
	char 			pPicF[100];   //ǰ-ץ��ͼ��
	char 			pPicRB[100];  //�Һ�-ץ��ͼ��
	char 			pPicRF[100];  //��ǰ-ץ��ͼ��
	char 			pPicLB[100];  //���-ץ��ͼ��
	char 			pPicLF[100];  //��ǰ-ץ��ͼ��
	char			pVehiPicF[100]; //ǰ����ͼ��
	char			pVehiPicB[100]; //����ͼ��

};

struct _structCommandInfo
{
	char		AREA_ID[10 + 1];			// ��վ����
	char		CHNL_NO[10 + 1];			// ͨ����
	BYTE		I_E_TYPE;					// ��������־
	char		SEQ_NO[20 + 1];				// ���к�
	char		CHECK_RESULT[20 + 1];		// ����ָ��
	char		VE_NAME[70 + 1];			// ���س������
	char		GPS_ID[20 + 1];				// GPS ID��
	char		ORIGIN_CUSTOMS[20 + 1];		// 
	char		DEST_CUSTOMS[20 + 1];		// 
	char		ESEAL_ID[50 + 1];			// 
	char		SEAL_KEY[100 + 1];			// 
	char		FORM_ID[50 + 1];			// 
	char		OP_HINT[100 + 1];			// 
};


#endif