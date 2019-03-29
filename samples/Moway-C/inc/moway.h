/** 
*   @file      moway.h
*   @brief     ͷ�ļ��ӿ�����, ����һ��ֵ��������
*   @details   ħ��API�ӿ�ͷ�ļ�.
*   
*   
*   @version   1.0.0
*   @date      2019-04-01
*   @pre       
*
*/

#ifndef _MW_H_
#define _MW_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

#ifndef INOUT
#define INOUT
#endif

#ifdef _MSC_VER
#pragma pack(push)
#pragma pack(1)
#endif // _MSC_VER

#ifdef __GNUC__
#pragma pack(push, 1)
#endif // linux gcc

#ifndef MWAPI
#if defined WIN32 || defined _WIN32 || defined _WIN64
#define MWAPI __stdcall
#else
#define MWAPI
#endif
#endif


//============================================================
//  ��������ֵ����
//============================================================
#define MW_SUCCESS                                          0x00000000                  // Success.
#define MW_ERROR_INVALID_HANDLE                             0x00000001                  // Handle may be NULL.
#define MW_ERROR_INVALID_PARAMETER                          0x00000002                  // Invalid parameter.
#define MW_ERROR_NOT_ENOUGH_MEMORY                          0x00000003                  // Not enough storage is available to process this command.
#define MW_ERROR_NO_DEVICE                                  0x00000004                  // No device or no specific device on the pc.
#define MW_ERROR_TIMEOUT                                    0x00000005                  // Time out.
#define MW_ERROR_UNSUPPORTED_FLAG                           0x00000006                  // A unsupported flag was given to the APIs.
#define MW_ERROR_INSUFFICIENT_BUFFER                        0x00000007                  // Buffer is too small to hold the data.
#define MW_ERROR_EXCHG_MEMORY_NOT_FOUND                     0x00000008                  // Specific share memory is not found.
#define MW_ERROR_SYSTEM_FILE_NOT_FOUND                      0x00000009                  // File in system not found.
#define MW_ERROR_SYSTEM_FILE_INVALID_ACCESS                 0x0000000a                  // Can not access system file.
#define MW_ERROR_FILE_EXISTS                                0x0000000b                  // Specific file is exist.
#define MW_ERROR_FILE_NOT_FOUND                             0x0000000c                  // Specific file is not found.
#define MW_ERROR_NO_PRIVILEGE                               0x0000000d                  // The operation is required a high level privilege.
#define MW_ERROR_WRONG_PASSWORD                             0x0000000e                  // Password is incorrect.
#define MW_ERROR_PASSWORD_LOCKED                            0x0000000f                  // Password is locked.
#define MW_ERROR_FUNCTION_NOT_SUPPORTED                     0x00000011                  // the function is not supported
#define MW_ERROR_COMMUNICATION                              0x00000015                  // Communication error.
#define MW_ERROR_UNSUPPORTED_PASSWORD_TYPE                  0x00000016                  // A unsupported password type was given to APIs.
#define MW_ERROR_WRONG_PASSWORD_LENGTH                      0x00000017                  // Password length is not correct, developer password is 24 bytes, user password is 8 bytes.
#define MW_ERROR_ALREADY_EXCLUSIVELY_LOGIN                  0x00000018                  // The device is already exclusively login, can not login with share mode.
#define MW_ERROR_ALREADY_SHARED_LOGIN                       0x00000019                  // The device is already shared login, can not login with exclusively mode.                 
#define MW_ERROR_ALREADY_TEMP_EXCLUSIVELY_USING             0x0000001a                  // The handle is already temporary using.
#define MW_ERROR_NOT_TEMP_EXCLUSIVELY_USING                 0x0000001b                  // The handle is not temporary using.
#define MW_ERROR_TOO_MUCH_DATA                              0x0000001c                  // The massage reply function has a data length limit.                 
#define MW_ERROR_INSUFFICIENT_DEVICE_SPACE                  0x0000001e                  // The device space is insufficient.
#define MW_ERROR_DEVICE_FILESYSTEM_ERROR                    0x0000001f                  // Device file system error.
#define MW_ERROR_FILE_OUT_RANGE                             0x00000020                  // Device file is out range.
#define MW_ERROR_UNSUPPORTED_FILE_TYPE                      0x00000021                  // an unsupported file type is given to EV APIs.
#define MW_ERROR_FILE_OFFSET_MUST_BE_ZERO                   0x00000022                  // When read or write a key file, offset must be 0.  
#define MW_ERROR_BAD_EXECUTIVE_FILE_FORMAT                  0x00000023                  // the executive file format is incorrect.
#define MW_ERROR_OPEN_TOO_MANY_DEVICE_FILES                 0x00000024                  // Open too many device files.
#define MW_ERROR_INVALID_DEVICE_FILE_HANDLE                 0x00000025                  // device file handle is incorrect.
#define MW_ERROR_FILE_NOT_FINISHED                          0x00000026                  // The file is not finish.
#define MW_ERROR_BAD_FILENAME                               0x00000027                  // Filename is incorrect.
#define MW_ERROR_BAD_NAME                                   0x00000028                  // The filename, directory name, or volume label syntax is incorrect.
#define MW_ERROR_DEVICE_TIMER                               0x00000029                  // Device timer error.
#define MW_ERROR_NO_EXECUTIVE_FILE_RUNNING                  0x0000002a                  // No process is running in the device. 
#define MW_ERROR_DEVICE_USER_BUFFER_FULL                    0x0000002b                  // Can not send data when device user buffer is full.
#define MW_ERROR_DEVICE_USER_BUFFER_EMPTY                   0x0000002c                  // Can receive data when device user buffer is empty.
#define MW_ERROR_DEVICE_MSG_NOT_REPLIED                     0x0000002d                  // Device need a message reply first.
#define MW_ERROR_DEVICE_DUMMY_MSG                           0x0000002e                  // Device not need a message reply.
#define MW_ERROR_DEVICE_MEMORY_ADDR                         0x0000002f                  // Device memory address error.
#define MW_ERROR_DEVICE_MEMORY_LENGTH                       0x00000030                  // Device memory length error.
#define MW_ERROR_CONTROL_CODE                               0x00000031                  // Give an unsupported control code.
#define MW_ERROR_UNKNOW_COMMAND                             0x00000032                  // Give an unsupported command to device.
#define MW_ERROR_INVALID_COMMAND_PARAMETER                  0x00000033                  // Command parameter error.
#define MW_ERROR_COMMAND_DATA_LENGTH                        0x00000034                  // Command data length error.
#define MW_ERROR_DEVICE_BUFFER_FULL                         0x00000035                  // Device buffer is full.
#define MW_ERROR_EXECUTIVE_FILE_RUNNING                     0x00000036                  // When a process is running in the device, some operation unsupported.
#define MW_ERROR_NO_DEVICE_MESSAGE                          0x00000037                  // No device message.
#define MW_ERROR_LOGIN_COUNT                                0x00000038                  // Device Login count error.
#define MW_ERROR_KEYEXCHANGE_FAILED                         0x00000039                  // Communication key exchange error.
#define MW_ERROR_BAD_COMMUNICATION_KEY                      0x0000003a                  // Communication key is incorrect.
#define MW_ERROR_BAD_DEVICE_TIME                            0x0000003b                  // Device time error.
#define MW_ERROR_BAD_DEVICE_INFOMATION                      0x0000003c                  // Device information error.
#define MW_ERROR_BAD_COMMAND_SEQUENCE                       0x0000003d                  // Command sequence is not right.
#define MW_ERROR_COMMUNICATION_DATA_LENGTH                  0x0000003e                  // Communication data length error.
#define MW_ERROR_ECC                                        0x0000003f                  // Device ECC crypt error.
#define MW_ERROR_BAD_UPDATE_DATA_LENGTH                     0x00000040                  // Update data length is incorrect.
#define MW_ERROR_UPDATE_STATE                               0x00000042                  // Update state is incorrect.
#define MW_ERROR_UPDATE_KEY_NOT_ENABLE                      0x00000043                  // When use remote update, remote update key must set first.
#define MW_ERROR_LOCKED_FOREVER                             0x00000044                  // Device is locked forever.
#define MW_ERROR_LOCKED_TEMPORARY                           0x00000045                  // Device is locked temporary.
#define MW_ERROR_DEVICE_UNLOCKED                            0x00000046                  // Device is not locked.
#define MW_ERROR_DEVICE_FLASH                               0x00000047                  // Device flash error.
#define MW_ERROR_DEVICE_ERROR                               0x00000048                  // Device error.
#define MW_ERROR_TOO_MANY_DEVICE                            0x00000049                  // Device numbers error, should not be larger than 128.
#define MW_ERROR_DEVICE_NOT_ENOUGH_USER_MEMORY              0x0000004a                  // There is no enough memory for user code.
#define MW_ERROR_FAKE_DEVICE                                0x0000004b                  // Device is fake.
#define MW_ERROR_DEVICE_BLK_OUT_RANGE                       0x0000004c                  // Device bulk read or write out range.
#define MW_ERROR_DEVICE_FS_ERR_BAK_ERROR                    0x0000004d                  // Device backup error.
#define MW_ERROR_DEVICE_TMR_ERR_ADJUST_TIME_TIMEOUT         0x0000004e                  // Adjust time is time out.
#define MW_ERROR_DEVICE_EXCH_ERROR                          0x0000004f                  // Exchange memory error.
#define MW_ERROR_DEVICE_AES_ERR                             0x00000050                  // Device AES error.
#define MW_ERROR_DEVICE_HASH_ERR_UNSUPPORTED_ALGO           0x00000051                  // Unsupported hash algo.
#define MW_ERROR_DEVICE_BAD_PRIVATE_KEY                     0x00000052                  // Bad private key.
#define MW_ERROR_DEVICE_BAD_PUBLIC_KEY                      0x00000053                  // Bad public key.
#define MW_ERROR_DEVICE_BAD_SYMMETRIC_KEY                   0x00000054                  // Bad symmetric key.
#define MW_ERROR_DEVICE_BAD_SIGNATURE                       0x00000055                  // Bad signature.
#define MW_ERROR_DEVICE_KEY_ERR_BAD_ALGO                    0x00000056                  // Bad algo.
#define MW_ERROR_DEVICE_LOGO_ERR_LOGO                       0x00000057                  // Bad logo.
#define MW_ERROR_EXEC_PARAM_TOO_LONG                        0x00000058                  // Execute parameter data is too long.
#define MW_ERROR_OPEN_ERROR                                 0x00000059                  // Open device error.
#define MW_ERROR_LOAD_SYS_MODULE_ERROR                      0x0000005A                  // Load system module error.
#define MW_ERROR_SYS_MODULE_FUNCTION_ERROR                  0x0000005B                  // System module function address error.
#define MW_ERROR_RSA                                        0x0000005C                  // Device RSA crypt error.
#define MW_ERROR_KEY                                        0x0000005D                  // Crypt Key error.
#define MW_ERROR_DEVICE_EXEC_ERR_UNALIGNED_MEM_ADDR         0x0000005E                  // Unaligned memory address.
#define MW_ERROR_DEVICE_EXEC_ERR_STACK_OVERFLOW             0x0000005F                  // User stack overflow.
#define MW_ERROR_DEVELOPER_ID_MISMATCH                      0x00000060                  // Developer ID not match.
#define MW_ERROR_LM_GENERAL_ERROR                           0x00000061                  // LM return data format wrong
#define MW_ERROR_LM_UNSUPPORTED_CERT_TYPE                   0x00000062                  // Unsupported certificate type
#define MW_ERROR_LM_UNSUPPORTED_UPDATE_OBJECT_TYPE          0x00000063                  // Unsupported object type to update
#define MW_ERROR_LM_UPDATE_PKG_FORMAT_WRONG                 0x00000064                  // Update Package format wrong
#define MW_ERROR_CERT                                       0x00000065                  // Certificates wrong
#define MW_ERROR_DEX_NOT_LOADED                             0x00000066                  // Dynamic Executive not loaded to the lock
#define MW_ERROR_SYSAPP_NOT_FOUND                           0x00000067                  // System Application Not Found
#define MW_ERROR_UNSUPPORTED_OBJECT_TYPE                    0x00000068                  // Unsupported update object type
#define MW_ERROR_UPDATE_BLOCK_LENGTH                        0x00000069                  // update block length wrong
#define MW_ERROR_UPDATE_SEED_NOT_FOUND                      0x0000006A                  // update seed not found
#define MW_ERROR_HEAP                                       0x0000006B                  // The system heap in the lock crashed
#define MW_ERROR_DEX_OUT_OF_RANGE                           0x0000006C                  // Dynamic Executive out of range
#define MW_ERROR_DEX_TOO_LARGE                              0x0000006D                  // Dynamic Executive too large
#define MW_ERROR_UPDATE_INFO                                0x0000006E                  // Update information wrong
#define MW_ERROR_DEVICE_STACK_OVERFLOW                      0x0000006F                  // The system stack in the lock crashed
#define MW_ERROR_COMMUNICATION_CRYPT                        0x00000070                  // Communication Encrypt wrong
#define MW_ERROR_BAD_UPDATE_PKG                             0x00000071                  // Bad Update Package
#define MW_ERROR_UPDATE_PKG_VERSION_LOW                     0x00000072                  // Update Package version low
#define MW_ERROR_UPDATE_OBJECT_TYPE                         0x00000073                  // Update Object type wrong
#define MW_ERROR_UPDATE_DEVELOPER_ID                        0x00000074                  // wrong developer id
#define MW_ERROR_UPDATE_FILE_TYPE                           0x00000075                  // wrong file type
#define MW_ERROR_NO_ADJUST_TIME_REQUEST                     0x00000076                  // no adjust time request have been get
#define MW_ERROR_CERT_REVOKED                               0x00000077                  // Certificates revoked
#define MW_ERROR_WRONG_CERT_SLOT                            0x00000078                  // Certificates slot number wrong
#define MW_ERROR_DEVICE_HASH_ERR_NOT_MATCH                  0x00000079                  // Hash value not match
#define MW_ERROR_BAD_RND                                    0x0000007A                  // Bad Random
#define MW_ERROR_RTC                                        0x0000007B                  // RTC error
#define MW_ERROR_RTC_POWER                                  0x0000007C                  // RTC power drop

#define MW_ERROR_NO_PID_DEVICE                              0x00002001                  // �������Ҳ���ָ����PIDħ����
#define MW_ERROR_RET_DATA_FORMAT                            0x00002002                  // �������ݸ�ʽ����ȷ
#define MW_ERROR_FILE_NAME_LEN                              0x00002003                  // �ļ������Ȳ���ȷ
#define MW_ERROR_BLOCK_DATA                                 0x00002004                  // ���ݿ����
#define MW_ERROR_FILE_PROPERTY_PARAM                        0x00002005                  // �ļ����Բ�������
#define MW_ERROR_FILE_COUNT_MAX                             0x00002006                  // ʵ���ļ���������Ԥ����ĸ���
#define MW_ERROR_MALLOC_MEMORY                              0x00002007                  // �����ڴ�ʧ��
#define MW_ERROR_PIN_LENGTH                                 0x00002008                  // PIN�����д���
#define MW_ERROR_UPDATA_PKG_PID                             0x00002009                  // ������PID������PID��һ��

#define MW_ERROR_ENTRY_TYPE                                 0x0000A001                  // entry ���ʹ���
#define MW_ERROR_ENTRY_FLAG                                 0x0000A002                  // entry ��ʶ����
#define MW_ERROR_OPCODE                                     0x0000A003                  // ��������
#define MW_ERROR_PARAM                                      0x0000A004                  // �������
#define MW_ERROR_DATA_LENGTH                                0x0000A005                  // �������ݳ���
#define MW_ERROR_PIN                                        0x0000A006                  // ����PIN ��
#define MW_ERROR_DEVICE_BLOCK                               0x0000A007                  // �豸PIN ������
#define MW_ERROR_PRIVILEGE                                  0x0000A008                  // û��Ȩ��
#define MW_ERROR_DATA_INFO                                  0x0000A009                  // ���������ݴ���
#define MW_ERROR_HASH_TYPE                                  0x0000A00A                  // ����HASH ����
#define MW_ERROR_ALG_MODE                                   0x0000A00B                  // �����ģʽ
#define MW_ERROR_INIT_EVT                                   0x0000A00C                  // ��ʼ�������������
#define MW_ERROR_ALG_TYPE                                   0x0000A00D                  // �����㷨����
#define MW_ERROR_BIT_SIZE                                   0x0000A00E                  // �������Կλ��С
#define MW_ERROR_FILE_TYPE                                  0x0000A00F                  // ������ļ�����
#define MW_ERROR_NO_INIT_PID                                0x0000A010                  // PIDδ��ʼ��
#define MW_ERROR_NO_INIT_PIN                                0x0000A011                  // PIN��δ�޸�
#define MW_ERROR_NO_VALIDATE_PKG                            0x0000A012                  // ��Ч������
#define MW_ERROR_AUTH_STAGE                                 0x0000A013                  // Ȩ�޼۶δ���
#define MW_ERROR_FILE_PRIVILEGE                             0x0000A014                  // �ļ�Ȩ�޴���
#define MW_ERROR_FILE_RANGE                                 0x0000A015                  // �ļ���Χ����
#define MW_ERROR_KEY_FILE                                   0x0000A016                  // �ļ���Կ����
#define MW_ERROR_REQ_IDENTIFY                               0x0000A017                  // ������֤����
#define MW_ERROR_INVALID_SEED                               0x0000A018                  // ��Ч���Ӵ���
#define MW_ERROR_SEED_CHECK                                 0x0000A019                  // ����У�����
#define MW_ERROR_UPDATE_HASH                                0x0000A01A                  // ������ϣ����
#define MW_ERROR_UPDATE_PID                                 0x0000A01B                  // ����PID����
#define MW_ERROR_UPDATE_INDEX                               0x0000A01C                  // ������������
#define MW_ERROR_UPDATE_TIME                                0x0000A01D                  // ����ʱ�����
#define MW_ERROR_UPDATE_SERIAL_NUM                          0x0000A01E                  // �������кŴ���
#define MW_ERROR_UPDATE_SN                                  0x0000A01F                  // ����SN����
#define MW_ERROR_UPDATE_ORDER                               0x0000A020                  // ����˳�����
#define MW_ERROR_UPDATE_OPERATE                             0x0000A021                  // ������������
#define MW_ERROR_KEY_TYPE                                   0x0000A022                  // �ؼ������ʹ���
#define MW_ERROR_OTHER                                      0x0000AFFF                  // ��������

//�����ķ�ʽ
/** �����  */
#define MW_OPEN_SHARE_MODE                                  0    
/** ��ռ�� */
#define MW_OPEN_EXCLUSIVE_MODE                              1    
//============================================================
//  �㷨���ͱ�ʾ
//============================================================
/** �㷨����AES */
#define MW_ALG_TYPE_AES                                     0x00        
/** �㷨����DES */
#define MW_ALG_TYPE_DES                                     0x01        
/** �㷨����TDES */
#define MW_ALG_TYPE_TDES                                    0x02        
/** �㷨����ECC */
#define MW_ALG_TYPE_ECC                                     0x10        
/** �㷨����RSA */
#define MW_ALG_TYPE_RSA                                     0x11        

//============================================================
//  �Գ��㷨ģʽ
//============================================================
/** �Գ��㷨ģʽ ECB mode */
#define MW_SYM_ALGO_MODE_ECB                                0x00     
/** �Գ��㷨ģʽ CBC mode */
#define MW_SYM_ALGO_MODE_CBC                                0x01      

//============================================================
//  �ļ�������غ궨��
//============================================================
/** �������ļ� */
#define MW_FILE_TYPE_BINARY                                 0          
/** ��Կ�ļ� */
#define MW_FILE_TYPE_KEY                                    2           

/** ��Կ�ļ�Ȩ������ */
#define MW_FILE_PRIV_TYPE_USE                               0x00         
/** �����ļ��û�ֻ��Ȩ������ */
#define MW_FILE_PRIV_TYPE_READ                              0x01         
/** �����ļ��û���дȨ������ */
#define MW_FILE_PRIV_TYPE_READ_WRITE                        0x02         

//============================================================
//  ��ȡ�豸��Ϣ�궨�� 
//============================================================
/** ��ȡPID     ����4�ֽ�PID */
#define MW_GET_INFO_ITEM_PID  	                            0x00 
/** ��ȡ���к�  ����16�ֽڻ�ȡ���к� */
#define MW_GET_INFO_ITEM_SN                                 0x01 
/** ��ȡ�������� ����4�ֽ��������� */
#define MW_GET_INFO_ITEM_PRODUCE_DATE                       0x02 
/** ��ȡ������   ����4�ֽ������� */
#define MW_GET_INFO_ITEM_ALL_CAPACITY  	                    0x03 
/** ��ȡʣ������ ����4�ֽ�ʣ������ */
#define MW_GET_INFO_ITEM_FREE_CAPACITY                      0x04 
/** ��ȡ�豸�汾 ����4�ֽ��豸�汾 */
#define MW_GET_INFO_ITEM_VERSION                            0x05 
/** ��ȡ��Ǻ�   ���ص�һ�ֽ�Ϊ��Ǻų��ȣ���������Ǻ����ݡ�ע�����256�ֽڡ� */
#define MW_GET_INFO_ITEM_SHELL_SN                           0x06 
/** ��ȡ����״̬ ����1�ֽڵı�ʶ���μ��궨��MW_STATUS_FLAG_PID��MW_STATUS_FLAG_PIN */
#define MW_GET_INFO_ITEM_STATUS                             0x07 
/** ��ȡ�豸Զ��������      ����4 �ֽ��豸Զ�������� */
#define MW_GET_INFO_ITEM_UPDATE_CODE                        0x08 
/** ��ȡԶ��������������ʶ  ����1�ֽڱ�ʶ */
#define MW_GET_INFO_ITEM_UPDATE_CTRL_CODE                   0x09 
/** ��ȡ������Ϣ������MW_DEVICE_ALL_ITEM_INFO�ṹ��Ĵ�С */
#define MW_GET_INFO_ITEM_ALL                                0xFF 


/** Ĭ��״̬ */
#define MW_STATUS_FLAG_DEFAULT                              0x00 
/** �Ѿ�����PID״̬ */
#define MW_STATUS_FLAG_PID                                  0x01 
/** �Ѿ��޸�PIN״̬ */
#define MW_STATUS_FLAG_PIN                                  0x02 
//============================================================
//  ������
//============================================================
/** ��λ�豸�������½״̬�� */
#define MW_CTRL_CODE_ITEM_RESET                             0x00 
/** ����LED��  ע�����ݵ�һ�ֽڷ�0��ʶLED����0��ʾ�� */
#define MW_CTRL_CODE_ITEM_LED                               0x01 

//============================================================
//  PIN���Ͷ���
//============================================================
/** Ĭ��PIN���� */
#define MW_PIN_TYPE_NONE                                    0x00   
/** �û�PIN���� */
#define MW_PIN_TYPE_USER                                    0x01
/** ������PIN���� */
#define MW_PIN_TYPE_DEVELOPER                               0x02

/** �û�PIN���� */
#define MW_PIN_LENGTH_USER                                  8     
/** ������PIN���� */
#define MW_PIN_LENGTH_DEVELOPER                             24    

//============================================================
//  RSA����ģʽ����
//============================================================
/** RSA ��������ģʽ */
#define MW_RSA_MODE_NORMAL                         0x00
/** RSA PKCS1 V1_5 ������ģʽ */
#define MW_RSA_MODE_PKCS1_V1_5                     0x01 

//============================================================
//  RSA�������ݳ���
//============================================================
/** RSA-1024�������ݳ��� */
#define MW_RSA_1024_BYTE_SIZE                      128
/** RSA-2048�������ݳ��� */
#define MW_RSA_2048_BYTE_SIZE                      256



//============================================================
//  ǩ��hash�㷨����
//============================================================
/** ǩ��MD5 hash�㷨 */
#define MW_HASH_ALGO_MD5                           0x00
/** ǩ��SHA1 hash�㷨 */
#define MW_HASH_ALGO_SHA1                          0x01
/** ǩ��SHA256 hash�㷨 */
#define MW_HASH_ALGO_SHA256                        0x02
/** ǩ��ʱ����Hash���㣬�ɵ��ú������Լ���Hash���� */
#define MW_HASH_ALGO_NONE                          0xFF 



//============================================================
//  ��������غ궨��
//============================================================
/** һ�������������������ļ����� */
#define MW_UPDATE_FILE_COUNT_MAX                   50

/** �����ļ�ʱ�Ĳ�����־ ɾ���ļ� */
#define MW_UPDATE_MOD_FLAG_DELETE                  0x00000002
/** �����ļ�ʱ�Ĳ�����־ д�ļ� */
#define MW_UPDATE_MOD_FLAG_WRITE                   0x00000010
/** �����ļ�ʱ�Ĳ�����־ �����ļ� */
#define MW_UPDATE_MOD_FLAG_CREATE                  0x00000020

//============================================================
//  �����Ժ궨��
//============================================================
/** Ĭ������ */
#define MW_LANGUAGE_ID_DEFAULT                     0       
/** ���� */
#define MW_LANGUAGE_ID_CH                          1 
/** Ӣ�� */
#define MW_LANGUAGE_ID_EN                          2  

//============================================================
//  ħ����ش�С�궨��
//============================================================
/** ���·�� */
#define MW_PATH_LEN                                1024
/** �ԳƼ����㷨�������� */
#define MW_SYM_ALG_IV_LENGTH                       16

/** �豸Ψһ���кų��� */
#define MW_SN_LENGTH                               16

/** �ļ�����󳤶� */
#define MW_FILE_NAME_LENGTH_MAX                    16

/** HMAC MD5���������� */
#define MW_HMAC_MD5_DIGEST_LENGTH                  16
/** HMAC SHA1���������� */
#define MW_HMAC_SHA1_DIGEST_LENGTH                 20
/** HMAC SHA256���������� */
#define MW_HMAC_SHA256_DIGEST_LENGTH               32

/** һ�����ת�����ݳ��� */
#define MW_INPUT_DATA_LENGTH_MAX                   1900

//��������󳤶Ⱥ궨��
/** ��������볤�� */
#define MW_SEED_LENGTH_MAX                         32    
/** ��С�����볤�� */
#define MW_SEED_LENGTH_MIN                         4    

#define MW_ENUM_DEVICE_MAX_COUNT                   128  


//============================================================
//  ���Ͷ���
//============================================================
/** �豸�ľ�� */
typedef     void*               MWHANDLE;


//============================================================
//  �ṹ�嶨��
//============================================================

/** �豸��Ϣ�ṹ�� */
typedef struct _MW_DEVICE_INFO_CTX{
	/** ��ƷID */
    unsigned int    uiPID;
	/** Ψһ���к� */
    unsigned char   ucSerialNum[MW_SN_LENGTH];
	/** �豸��·�� */
    unsigned char   ucDevPath[MW_PATH_LEN];
	/** ͨѶЭ�� */
    unsigned int    uiProtocol;  
	/** Mac OS ϵͳ LocationID */
	unsigned int    uiLocationID;
}MW_DEVICE_INFO_CTX;

/** �ļ����Խṹ�� */
typedef struct _MW_FILE_PROPERTY{
	/** �ó�Ա������ʱ���� */
	unsigned char   ucValidate; 
	/** �ļ����ͣ��������ļ�������Կ�ļ��� */
	unsigned char   ucType;   
	/** �ļ�Ȩ�� */
	unsigned short  usPrivilege;
	/** �ļ���С */
	unsigned int    uiSize;    
	/** ����ʱ�� */
	unsigned int    uiTime;   
	/** �ļ����� */
	char acName[MW_FILE_NAME_LENGTH_MAX + 1]; 
}MW_FILE_PROPERTY;

/** ϵͳ����״̬�ṹ�� */
typedef struct _MW_DEVICE_STATUS
{
	/** ϵͳʱ�� */
	unsigned int    uiSysTime;
	/** ϵͳ״̬ */
	unsigned int    uiSysStatus;
	/** ���� */
	unsigned short  usReserved;
	/** ��¼״̬ */
	unsigned char   ucLoginStatus;
	/** �Ƶ�״̬ */
	unsigned char   ucLedStatus;
}MW_DEVICE_STATUS;

/** Զ�������ṹ�� */
typedef struct _MW_UPDADE_FILE_CTX
{
	/** �ļ����Խṹ�� */
	MW_FILE_PROPERTY stFileProperty;
	/** �����ļ����������� */
	unsigned char *pData; 
	/** �����ļ������ݴ�С */
	unsigned int uiDateSize;
}MW_UPDADE_FILE_CTX;

/** �豸��Ϣ����ṹ�� */
typedef struct _MW_DEVICE_ALL_ITEM_INFO
{
	/** ��Ʒ�� */
	unsigned int uiPID;
	/** Ψһ���к� */
	unsigned char acSN[MW_SN_LENGTH];
	/** ����ʱ�� */
	unsigned int uiProduceDate;
	/** ������ */
	unsigned int uiAllCapacity;
	/** �������� */
	unsigned int uiFreeCapacity;
	/** �汾�� */
	unsigned int uiVersion;
	/** ������ */
	unsigned int uiUpdateCode;
	/** ״̬ */
	unsigned char ucStatus;
	/** ���������� */
	unsigned char ucUpdateCtrlCode;
	/** ���� */
	unsigned short reserved;
}MW_DEVICE_ALL_ITEM_INFO;
//============================================================
//  ��������
//============================================================

/*!
*   @brief         ö��ħ���豸
*   @param[in out] pDevInfoList        ö�������ħ���豸�б�
*   @param[in]     uiDevListCount      ö���б�(pDevInfoList)�ܴ�ŵ��豸�ĸ�����ע���������μ��궨��MW_ENUM_DEVICE_MAX_COUNT
*   @param[out]    puiDevCount         ʵ��ö�ٳ���ħ���豸����
*   @return        ִ�гɹ����� MW_SUCCESS ��ִ��ʧ��ʱ���ش����롣
*   @remarks       ע�⣺�û����밴��uiDevListCount����sizeof(MW_DEVICE_INFO_CTX)�����ڴ��pDevInfoList��API�ڲ��������ڴ��pDevInfoList��
*/
unsigned  int MWAPI mw_enum(OUT MW_DEVICE_INFO_CTX     *pDevInfoList,
							IN  unsigned int       uiDevListCount,
                            OUT unsigned int       *puiDevCount);

/*!
*   @brief      ��ħ���豸
*   @param[in]  pDevInfo             ָ��ħ���豸��Ϣ�ṹ���ָ�롣
*   @param[in]  uiShareMode          ��ռ���߹���ģʽ�򿪡��μ��궨��MW_OPEN_SHARE_MODE  MW_OPEN_EXCLUSIVE_MODE
*   @param[out] phHandle             ��ħ���豸�����һ������Ա�������á�
*   @return     ִ�гɹ����� MW_SUCCESS ��ִ��ʧ��ʱ���ش����롣
*   @remarks    Open an moway device with DevInfo enumerated by mw_enum()
*/
unsigned  int MWAPI mw_open(IN   MW_DEVICE_INFO_CTX *pDevInfo,
                            IN   unsigned int   uiShareMode,
                            OUT  MWHANDLE       *phHandle);
                                                                           
/*!
*   @brief      �ر��豸
*   @param[in]  hHandle           ħ���豸���
*   @return     ִ�гɹ����� MW_SUCCESS ��ִ��ʧ��ʱ���ش����롣
*   @remarks    
*/
unsigned  int MWAPI mw_close(IN   MWHANDLE   hHandle);

/*!
*   @brief      У��PIN
*   @param[in]  hHandle             ħ���豸���
*   @param[in]  ucPinType           Pin������  �μ��궨�壺MW_PIN_TYPE_XXX
*   @param[in]  pucPin              Pin�����ݣ��û�8�ֽڣ�������24�ֽ�
*   @return     ִ�гɹ����� MW_SUCCESS ��ִ��ʧ��ʱ���ش����롣
*   @remarks    
*/
unsigned  int MWAPI mw_verify_pin(IN   MWHANDLE      hHandle,
                                  IN   unsigned char ucPinType,
                                  IN   char          *pucPin);

/*!
*   @brief      �޸�PIN
*   @param[in]  hHandle             ħ���豸���
*   @param[in]  ucPinType           Pin������  �μ��궨�壺MW_PIN_TYPE_XXX
*   @param[in]  usLimitCount        ����Դ�����ע�����޴�������Ϊ0�����ƴ�����Χ��1-15�� ��Ϊ�������򷵻ز�������
*   @param[in]  pucOldPin           ��Pin�����ݣ��û�8�ֽڣ�������24�ֽ�
*   @param[in]  pucNewPin           ��Pin�����ݣ��û�8�ֽڣ�������24�ֽ�
*   @return     ִ�гɹ����� MW_SUCCESS ��ִ��ʧ��ʱ���ش����롣
*   @remarks    
*/
unsigned  int MWAPI mw_change_pin(IN   MWHANDLE           hHandle,
                                  IN   unsigned char      ucPinType,
                                  IN   unsigned short     usLimitCount,
                                  IN   char               *pucOldPin,
                                  IN   char               *pucNewPin);
/*!
*   @brief      ����PID
*   @param[in]  hHandle             ħ���豸���
*   @param[in]  pucPIDSeed          ������
*   @param[in]  uiSeedLen           �����볤�ȣ����ȷ�Χ��4-32���μ��궨��MW_SEED_LENGTH_MIN��MW_SEED_LENGTH_MAX
*   @return     ִ�гɹ����� MW_SUCCESS ��ִ��ʧ��ʱ���ش����롣
*   @remarks    
*/
unsigned int MWAPI mw_set_pid(IN     MWHANDLE           hHandle,
                              IN     unsigned char      *pucPIDSeed,
                              IN     unsigned int       uiSeedLen);
                              
/*!
*   @brief      ��������
*   @param[in]  hHandle             ħ���豸���
*   @param[in]  uiCtrlCodeItem      �����룬�μ��궨�� MW_CTRL_CODE_ITEM_XXX
*   @param[in]  pvInBuffer          ��������
*   @param[in]  uiInBufferLen       �������ݵĳ���
*   @param[out] pvOutBuffer         �������
*   @param[in]  uiOutBufferLen      ������ݵĳ���
*   @param[out] puiReturnedLen      ʵ��������ݵĳ���   
*   @return     ִ�гɹ����� MW_SUCCESS ��ִ��ʧ��ʱ���ش����롣
*   @remarks    
*/
unsigned  int MWAPI mw_control(IN      MWHANDLE           hHandle,
                               IN      unsigned char      uiCtrlCodeItem,
                               IN      void               *pvInBuffer,
                               IN      unsigned int       uiInBufferLen,
                               OUT     void               *pvOutBuffer,
                               IN      unsigned int       uiOutBufferLen,
                               OUT     unsigned int       *puiReturnedLen);   
         
/*!
*   @brief      ��ȡ�豸��Ϣ
*   @param[in]  hHandle           ħ���豸���
*   @param[in]  ucInfoItem        ָ����ȡ���豸��Ϣ�ľ�����μ��궨�壺MW_GET_INFO_ITEM_XXX 
*   @param[out] pvBuffer          �����ȡ���豸��Ϣ��
*   @param[in out] puiBufferLength   ����豸��Ϣ������ݳ���
*   @return     ִ�гɹ����� MW_SUCCESS ��ִ��ʧ��ʱ���ش����롣
*   @remarks    
*/
unsigned  int MWAPI mw_get_device_info(IN  MWHANDLE        hHandle,
                                       IN  unsigned char   ucInfoItem,
                                       OUT void            *pvBuffer,
                                       INOUT unsigned int  *puiBufferLength);
         
/*!
*   @brief      ��ȡ�豸״̬
*   @param[in]  hHandle                   ħ���豸���
*   @param[out] pstDeviceStatusCtx        ��ǰ�豸״̬��
*   @return     ִ�гɹ����� MW_SUCCESS ��ִ��ʧ��ʱ���ش����롣
*   @remarks    
*/                                        
unsigned  int MWAPI mw_get_device_status(IN  MWHANDLE                   hHandle,
                                         OUT MW_DEVICE_STATUS       *pstDeviceStatusCtx);
                                        
/*!
*   @brief      �о��ļ�
*   @param[in]  hHandle              ħ���豸���
*   @param[out] pstFilePropertyList  �ṹ��MW_FILE_PROPERTY���飬���Ĵ�С��uiFileListCountָ��
*   @param[in]  uiFileListCount      ָ��pstFilePropertyList����Ĵ�С
*   @param[out] puiReturnedFileCount ʵ�ʷ����оٵ��ļ��ĸ���
*   @return     ִ�гɹ����� MW_SUCCESS ��ִ��ʧ��ʱ���ش����롣
*   @remarks    ע�⣺pstFilePropertyList�ɺ����ⲿ�����߷��䣬��СΪuiFileListCount * sizeof(MW_FILE_PROPERTY)
*/
unsigned  int MWAPI mw_enum_file(IN  MWHANDLE           hHandle,
								 OUT MW_FILE_PROPERTY   *pstFilePropertyList,
								 IN  unsigned int       uiFileListCount,
								 OUT unsigned int       *puiReturnedFileCount);

/*!
*   @brief      �����ļ�
*   @param[in]  hHandle           ħ���豸���
*   @param[in]  pstFileProperty   ָ����Ҫ�����ļ������ԣ������ļ����͡��ļ�Ȩ�ޡ��ļ���СΪ������
*   @return     ִ�гɹ����� MW_SUCCESS ��ִ��ʧ��ʱ���ش����롣
*   @remarks    
*/
unsigned  int MWAPI mw_create_file(IN      MWHANDLE           hHandle,
                                   IN      MW_FILE_PROPERTY   *pstFileProperty);

/*!
*   @brief      ���ļ�
*   @param[in]  hHandle           ħ���豸���
*   @param[in]  pcFileName        �ļ����ƣ���'\0'��β���ַ���      
*   @param[in]  uiReadOffset      ���ļ���ƫ����   
*   @param[in]  uiReadSize        Ԥ�ƶ�ȡ�ļ��Ĵ�С��
*   @param[out] pucReadBuffer     ��ȡ�ļ����ݵ���������ע����������㹻uiReadSize��С����������Խ�硣
*   @return     ִ�гɹ����� MW_SUCCESS ��ִ��ʧ��ʱ���ش����롣
*   @remarks    
*/
unsigned  int MWAPI mw_read_file(IN      MWHANDLE           hHandle,
                                 IN      char               *pcFileName,
                                 IN      unsigned int       uiReadOffset,
                                 IN      unsigned int       uiReadSize,
                                 OUT     unsigned char      *pucReadBuffer);
                                  
/*!
*   @brief      д�ļ�
*   @param[in]  hHandle           ħ���豸���
*   @param[in]  pcFileName        �ļ����ƣ���'\0'��β���ַ���      
*   @param[in]  uiWriteOffset     д�ļ���ƫ����   
*   @param[in]  uiWriteSize       д�ļ��Ĵ�С�� 
*   @param[in]  pucWriteBuffer    ��Ҫд���ļ��Ļ����� 
*   @return     ִ�гɹ����� MW_SUCCESS ��ִ��ʧ��ʱ���ش����롣
*   @remarks    
*/
unsigned  int MWAPI mw_write_file(IN      MWHANDLE           hHandle,
                                  IN      char               *pcFileName,
                                  IN      unsigned int       uiWriteOffset,
                                  IN      unsigned int       uiWriteSize,
                                  IN      unsigned char     *pucWriteBuffer);
                                   
/*!
*   @brief      ɾ���ļ�
*   @param[in]  hHandle           ħ���豸���
*   @param[in]  pcFileName        �ļ����ƣ���'\0'��β���ַ���      
*   @return     ִ�гɹ����� MW_SUCCESS ��ִ��ʧ��ʱ���ش����롣
*   @remarks    
*/
unsigned  int MWAPI mw_delete_file(IN      MWHANDLE      hHandle,
                                   IN      char          *pcFileName);
                                  
/*!
*   @brief      ��ȡ�ļ�����
*   @param[in]  hHandle           ħ���豸���
*   @param[in]  lpszFileName      �ļ����ƣ���'\0'��β���ַ��� 
*   @param[out] pstFileProperty   �ļ�������Ϣ    
*   @return     ִ�гɹ����� MW_SUCCESS ��ִ��ʧ��ʱ���ش����롣
*   @remarks    
*/
unsigned int MWAPI mw_get_file_property(IN      MWHANDLE           hHandle,
                                        IN      char               *pcFileName,
                                        OUT     MW_FILE_PROPERTY   *pstFileProperty);
                                        
/*!
*   @brief          �ԳƼ���
*   @param[in]      hHandle           ħ���豸���
*   @param[in]      pcKeyFileName     ��Կ�ļ����� 
*   @param[in]      ucAlgoMode        �ԳƼ��ܵ��㷨���μ��궨�壺MW_ALG_TYPE_XXX
*   @param[in]      pucIV             �ԳƼ����㷨����Ҫʹ�õ����������̶�16�ֽڣ�CBCģʽ��Ҫ���룬ECB�ɲ���
*   @param[in]      pucInData         ������������ģ�������16�ı�����
*   @param[in]      uiInDataLen       �������ĵ����ݳ���,
*   @param[out]     pucOutData        ���ܺ����������
*   @param[in out]  puiOutDataLen     ���ܺ����ĵĳ���
*   @return     ִ�гɹ����� MW_SUCCESS ��ִ��ʧ��ʱ���ش����롣
*   @remarks    
*/
unsigned int MWAPI mw_sym_encrypt(IN       MWHANDLE           hHandle,
                                  IN       char               *pcKeyFileName,
                                  IN       unsigned char      ucAlgoMode,
                                  IN       unsigned char      *pucIV,
                                  IN       unsigned char      *pucInData,
                                  IN       unsigned int       uiInDataLen,
                                  OUT      unsigned char      *pucOutData,
                                  INOUT    unsigned int       *puiOutDataLen);   
                        
/*!
*   @brief          �Գƽ���
*   @param[in]      hHandle           ħ���豸���
*   @param[in]      pcKeyFileName     ��Կ�ļ����� 
*   @param[in]      ucAlgoMode        �ԳƼ��ܵ��㷨���μ��궨�壺MW_ALG_TYPE_XXX
*   @param[in]      pucIV             �ԳƼ����㷨����Ҫʹ�õ����������̶�16�ֽڣ�CBCģʽ��Ҫ���룬ECB�ɲ���
*   @param[in]      pucInData         ��������ģ�������16�ı���,
*   @param[in]      uiInDataLen       �������ĳ���,
*   @param[out]     pucOutData        ���ܺ�������� 
*   @param[in out]  puiOutDataLen     ���ܺ�������ĳ���
*   @return     ִ�гɹ����� MW_SUCCESS ��ִ��ʧ��ʱ���ش����롣
*   @remarks    
*/
unsigned int MWAPI mw_sym_decrypt(IN       MWHANDLE           hHandle,
                                  IN       char               *pcKeyFileName,
                                  IN       unsigned char      ucAlgoMode,
                                  IN       unsigned char      *pucIV,
                                  IN       unsigned char      *pucInData,
                                  IN       unsigned int       uiInDataLen,
                                  OUT      unsigned char      *pucOutData,
                                  INOUT    unsigned int       *puiOutDataLen);            
                                    
/*!
*   @brief          rsa�ǶԳƼ���
*   @param[in]      hHandle           ħ���豸���
*   @param[in]      pcKeyFileName     ��Կ�ļ����� 
*   @param[in]      ucPadMode         ���ܵ��㷨���μ��궨�壺MW_RSA_MODE_XXX
*   @param[in]      pucInData         ������������
*   @param[in]      uiInDataLen       �����������ݵĳ��� 
*   @param[out]     pucOutData        �����������
*   @param[in out]  puiOutDataLen     ����������ݵĳ���
*   @return     ִ�гɹ����� MW_SUCCESS ��ִ��ʧ��ʱ���ش����롣
*   @remarks    ע����ucPadModeΪMW_RSA_MODE_NORMALʱ��RSA_1024λ�������������ĳ���Ϊ128,RSA_2048λ������󳤶�Ϊ256; 
*                   ��ucPadModeΪMW_RSA_MODE_PKCS1_V1_5ʱ��  RSA_1024λ�������������ĳ���Ϊ117,RSA_2048λ������󳤶�Ϊ245; 
*/
unsigned int MWAPI mw_rsa_encrypt(IN       MWHANDLE           hHandle,
                                  IN       const char         *pcKeyFileName,
                                  IN       unsigned char      ucPadMode,
                                  IN       unsigned char      *pucInData,
                                  IN       unsigned int       uiInDataLen,
                                  OUT      unsigned char      *pucOutData,
                                  INOUT    unsigned int       *puiOutDataLen);
                        
/*!
*   @brief          rsa�ǶԳƽ���
*   @param[in]      hHandle           ħ���豸���
*   @param[in]      pcKeyFileName     ��Կ�ļ����� 
*   @param[in]      ucPadMode         ���ܵ��㷨���μ��궨�壺MW_RSA_MODE_XXX
*   @param[in]      pucInData         ��������   
*   @param[in]      uiInDataLen       �������ĳ���  ע�����ĳ��ȱ���Ϊ128����256 
*   @param[out]     pucOutData        ������� 
*   @param[in out]  puiOutDataLen     ������ĳ���
*   @return     ִ�гɹ����� MW_SUCCESS ��ִ��ʧ��ʱ���ش����롣
*   @remarks    
*/
unsigned int MWAPI mw_rsa_decrypt(IN       MWHANDLE           hHandle,
                                  IN       char               *pcKeyFileName,
                                  IN       unsigned char      ucPadMode,
                                  IN       unsigned char      *pucInData,
                                  IN       unsigned int       uiInDataLen,
                                  OUT      unsigned char      *pucOutData,
                                  INOUT    unsigned int       *puiOutDataLen);
                                   
/*!
*   @brief          ǩ��
*   @param[in]      hHandle           ħ���豸���
*   @param[in]      pcKeyFileName     ��Կ�ļ����� 
*   @param[in]      ucHashAlg         ��ϣ�㷨���ͣ��μ��궨�壺MW_HASH_ALGO_SHA1, MW_HASH_ALGO_SHA256, MW_HASH_ALGO_NONE
*   @param[in]      pucMessageData    ������Ϣ����
*   @param[in]      uiMessageDataLen  ������Ϣ���ݳ��� ע��������ݳ��Ȳμ��궨�� MW_INPUT_DATA_LENGTH_MAX 
*   @param[out]     pucSignData       ���ǩ������ 
*   @param[in out]  puiSignDataLen    ���ǩ�����ݳ���
*   @return     ִ�гɹ����� MW_SUCCESS ��ִ��ʧ��ʱ���ش����롣
*   @remarks    
*/
unsigned int MWAPI mw_signature(IN       MWHANDLE           hHandle,
                                IN       char               *pcKeyFileName,
                                IN       unsigned char      ucHashAlg,
                                IN       unsigned char      *pucMessageData,
                                IN       unsigned int       uiMessageDataLen,
                                OUT      unsigned char      *pucSignData,
                                INOUT    unsigned int       *puiSignDataLen);

/*!
*   @brief      ��ǩ
*   @param[in]      hHandle           ħ���豸���
*   @param[in]      pcKeyFileName     ��Կ�ļ����� 
*   @param[in]      ucHashAlg         ��ϣ�㷨���ͣ��μ��궨�壺MW_HASH_ALGO_XXX
*   @param[in]      pucSignData       ����ǩ������
*   @param[in]      uiSignDataLen     ����ǩ�����ݳ���
*   @param[in]      pucMessageData    ������Ϣ����
*   @param[in]      uiMessageDataLen  ������Ϣ���ݳ���
*   @return     ִ�гɹ����� MW_SUCCESS ��ִ��ʧ��ʱ���ش����롣
*   @remarks    
*/
unsigned int MWAPI mw_verify_sign(IN       MWHANDLE           hHandle,
                                  IN       char               *pcKeyFileName,
                                  IN       unsigned char      ucHashAlg,
                                  IN       unsigned char      *pucSignData,
                                  IN       unsigned int       uiSignDataLen,
								  IN       unsigned char      *pucMessageData,
								  IN       unsigned int       uiMessageDataLen);

/*!
*   @brief          HMAC����
*   @param[in]      hHandle           ħ���豸���
*   @param[in]      pcKeyFileName     ��Կ�ļ����� 
*   @param[in]      pucInData         ��������
*   @param[in]      uiInDataLen       �������ݳ���
*   @param[out]     pucOutData        ������� 
*   @param[in out]  puiOutDataLen     ������ݳ��ȣ��μ��궨�壺MW_HMAC_MD5_DIGEST_LENGTH��MW_HMAC_SHA1_DIGEST_LENGTH��MW_HMAC_SHA256_DIGEST_LENGTH
*   @return     ִ�гɹ����� MW_SUCCESS ��ִ��ʧ��ʱ���ش����롣
*   @remarks    
*/
unsigned int MWAPI mw_hmac_calc(IN       MWHANDLE           hHandle,
                                IN       char               *pcKeyFileName,
                                IN       unsigned char      *pucInData,
                                IN       unsigned int       uiInDataLen,
                                OUT      unsigned char      *pucOutData,
                                INOUT    unsigned int       *puiOutDataLen);


                               

/*!
*   @brief          ����������
*   @param[in]      hHandle                ħ���豸���
*   @param[in]      uiDevPID               �豸��Ʒ��
*   @param[in]      pucSerialNum           Ӳ��Ψһ���к� �̶�����Ϊ��MW_SN_LENGTH����ָ��SN��ʱ������ΪNULL
*   @param[in]      pstUpdateFileList      �ļ�·���б�,����ļ������μ��궨��MW_UPDATE_FILE_COUNT_MAX,�����ļ��ĸ���ΪuiFileCount
*   @param[in]      uiFileCount            �����ļ��ĸ���,��ָ����pstUpdateFileList�еĴ�С 
*   @param[out]     pucOutPkg              ����������
*   @param[in out]  puiOutPkgLen           ���������ݳ���
*   @return     ִ�гɹ����� MW_SUCCESS ��ִ��ʧ��ʱ���ش����롣
*   @remarks    
*/
unsigned int MWAPI mw_make_update_pkg(IN       MWHANDLE           hHandle,
                                      IN       unsigned int       uiDevPID,
                                      IN       unsigned char      *pucSN,
									  IN       MW_UPDADE_FILE_CTX *pstUpdateFileList,
									  IN       unsigned int       uiFileCount,
                                      OUT      unsigned char      *pucOutPkg,
                                      INOUT    unsigned int       *puiOutPkgLen);
                        
/*!
*   @brief      Զ������
*   @param[in]  hHandle             ħ���豸���
*   @param[in]  pucInPkg            ����������
*   @param[in]  uiInPkgLen          ���������ݳ���
*   @return     ִ�гɹ����� MW_SUCCESS ��ִ��ʧ��ʱ���ش����롣
*   @remarks    
*/
unsigned int MWAPI mw_update(IN       MWHANDLE           hHandle,
                             IN       unsigned char      *pucInPkg,
                             IN       unsigned int       uiInPkgLen);    
                                     

/*!
*   @brief      �ָ���������
*   @param[in]  hHandle             ħ���豸���
*   @return     ִ�гɹ����� MW_SUCCESS ��ִ��ʧ��ʱ���ش����롣
*   @remarks    ���豸�ָ�����״̬����Ĭ��PID��Ĭ��PIN���ļ�ϵͳ��ա��迪����Ȩ��
*/
unsigned int MWAPI mw_restore_factory(IN       MWHANDLE           hHandle);   


/*!
*   @brief      ���������
*   @param[in]  hHandle             ħ���豸���
*   @param[in]  uiErrorCode         ������
*   @param[in]  uiLanguageID        ��������ID
*   @return     ִ�гɹ����� MW_SUCCESS ��ִ��ʧ��ʱ���ش����롣
*   @remarks    
*/
const char * mw_get_error_desc(IN  unsigned int uiErrorCode,
                           IN  unsigned int uiLanguageID);





#ifdef _MSC_VER
#pragma pack(pop)
#endif//_MSC_VER

#ifdef __GNUC__
#pragma pack(pop)
#endif

#ifdef __cplusplus
}
#endif

#ifdef _MSC_VER
#pragma comment(lib, "setupapi.lib")
#endif

#endif//_MW_H_
