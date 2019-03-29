//USES
//    Windows


const 
//============================================================
//  错误码定义
//============================================================
MW_SUCCESS                              = $00000000;    // Success.
MW_ERROR_INVALID_HANDLE                 = $00000001;    // Handle may be NULL.
MW_ERROR_INVALID_PARAMETER              = $00000002;    // Invalid parameter.
MW_ERROR_NOT_ENOUGH_MEMORY              = $00000003;    // Not enough storage is available to process this command.
MW_ERROR_NO_DEVICE                      = $00000004;    // No device or no specific device on the pc.
MW_ERROR_TIMEOUT                        = $00000005;    // Time out.
MW_ERROR_UNSUPPORTED_FLAG               = $00000006;    // A unsupported flag was given to the APIs.
MW_ERROR_INSUFFICIENT_BUFFER            = $00000007;    // Buffer is too small to hold the data.
MW_ERROR_EXCHG_MEMORY_NOT_FOUND         = $00000008;    // Specific share memory is not found.
MW_ERROR_SYSTEM_FILE_NOT_FOUND          = $00000009;    // File in system not found.
MW_ERROR_SYSTEM_FILE_INVALID_ACCESS     = $0000000a;    // Can not access system file.
MW_ERROR_FILE_EXISTS                    = $0000000b;    // Specific file is exist.
MW_ERROR_FILE_NOT_FOUND                 = $0000000c;    // Specific file is not found.
MW_ERROR_NO_PRIVILEGE                   = $0000000d;    // The operation is required a high level privilege.
MW_ERROR_WRONG_PASSWORD                 = $0000000e;    // Password is incorrect.
MW_ERROR_PASSWORD_LOCKED                = $0000000f;    // Password is locked.
MW_ERROR_FUNCTION_NOT_SUPPORTED         = $00000011;    // the function is not supported
MW_ERROR_COMMUNICATION                  = $00000015;    // Communication error.
MW_ERROR_UNSUPPORTED_PASSWORD_TYPE      = $00000016;    // A unsupported password type was given to APIs.
MW_ERROR_WRONG_PASSWORD_LENGTH          = $00000017;    // Password length is not correct, developer password is 24 bytes, user password is 8 bytes.
MW_ERROR_ALREADY_EXCLUSIVELY_LOGIN      = $00000018;    // The device is already exclusively login, can not login with share mode.
MW_ERROR_ALREADY_SHARED_LOGIN           = $00000019;    // The device is already shared login, can not login with exclusively mode.                 
MW_ERROR_ALREADY_TEMP_EXCLUSIVELY_USING = $0000001a;    // The handle is already temporary using.
MW_ERROR_NOT_TEMP_EXCLUSIVELY_USING     = $0000001b;    // The handle is not temporary using.
MW_ERROR_TOO_MUCH_DATA                  = $0000001c;    // The massage reply function has a data length limit.                 
MW_ERROR_INSUFFICIENT_DEVICE_SPACE      = $0000001e;    // The device space is insufficient.
MW_ERROR_DEVICE_FILESYSTEM_ERROR        = $0000001f;    // Device file system error.
MW_ERROR_FILE_OUT_RANGE                 = $00000020;    // Device file is out range.
MW_ERROR_UNSUPPORTED_FILE_TYPE          = $00000021;    // an unsupported file type is given to EV APIs.
MW_ERROR_FILE_OFFSET_MUST_BE_ZERO       = $00000022;    // When read or write a key file, offset must be 0.  
MW_ERROR_BAD_EXECUTIVE_FILE_FORMAT      = $00000023;    // the executive file format is incorrect.
MW_ERROR_OPEN_TOO_MANY_DEVICE_FILES     = $00000024;    // Open too many device files.
MW_ERROR_INVALID_DEVICE_FILE_HANDLE     = $00000025;    // device file handle is incorrect.
MW_ERROR_FILE_NOT_FINISHED              = $00000026;    // The file is not finish.
MW_ERROR_BAD_FILENAME                   = $00000027;    // Filename is incorrect.
MW_ERROR_BAD_NAME                       = $00000028;    // The filename, directory name, or volume label syntax is incorrect.
MW_ERROR_DEVICE_TIMER                   = $00000029;    // Device timer error.
MW_ERROR_NO_EXECUTIVE_FILE_RUNNING      = $0000002a;    // No process is running in the device. 
MW_ERROR_DEVICE_USER_BUFFER_FULL        = $0000002b;    // Can not send data when device user buffer is full.
MW_ERROR_DEVICE_USER_BUFFER_EMPTY       = $0000002c;    // Can receive data when device user buffer is empty.
MW_ERROR_DEVICE_MSG_NOT_REPLIED         = $0000002d;    // Device need a message reply first.
MW_ERROR_DEVICE_DUMMY_MSG               = $0000002e;    // Device not need a message reply.
MW_ERROR_DEVICE_MEMORY_ADDR             = $0000002f;    // Device memory address error.
MW_ERROR_DEVICE_MEMORY_LENGTH           = $00000030;    // Device memory length error.
MW_ERROR_CONTROL_CODE                   = $00000031;    // Give an unsupported control code.
MW_ERROR_UNKNOW_COMMAND                 = $00000032;    // Give an unsupported command to device.
MW_ERROR_INVALID_COMMAND_PARAMETER      = $00000033;    // Command parameter error.
MW_ERROR_COMMAND_DATA_LENGTH            = $00000034;    // Command data length error.
MW_ERROR_DEVICE_BUFFER_FULL             = $00000035;    // Device buffer is full.
MW_ERROR_EXECUTIVE_FILE_RUNNING         = $00000036;    // When a process is running in the device, some operation unsupported.
MW_ERROR_NO_DEVICE_MESSAGE              = $00000037;    // No device message.
MW_ERROR_LOGIN_COUNT                    = $00000038;    // Device Login count error.
MW_ERROR_KEYEXCHANGE_FAILED             = $00000039;    // Communication key exchange error.
MW_ERROR_BAD_COMMUNICATION_KEY          = $0000003a;    // Communication key is incorrect.
MW_ERROR_BAD_DEVICE_TIME                = $0000003b;    // Device time error.
MW_ERROR_BAD_DEVICE_INFOMATION          = $0000003c;    // Device information error.
MW_ERROR_BAD_COMMAND_SEQUENCE           = $0000003d;    // Command sequence is not right.
MW_ERROR_COMMUNICATION_DATA_LENGTH      = $0000003e;    // Communication data length error.
MW_ERROR_ECC                            = $0000003f;    // Device ECC crypt error.
MW_ERROR_BAD_UPDATE_DATA_LENGTH         = $00000040;    // Update data length is incorrect.
MW_ERROR_UPDATE_STATE                   = $00000042;    // Update state is incorrect.
MW_ERROR_UPDATE_KEY_NOT_ENABLE          = $00000043;    // When use remote update, remote update key must set first.
MW_ERROR_LOCKED_FOREVER                 = $00000044;    // Device is locked forever.
MW_ERROR_LOCKED_TEMPORARY               = $00000045;    // Device is locked temporary.
MW_ERROR_DEVICE_UNLOCKED                = $00000046;    // Device is not locked.
MW_ERROR_DEVICE_FLASH                   = $00000047;    // Device flash error.
MW_ERROR_DEVICE_ERROR                   = $00000048;    // Device error.
MW_ERROR_TOO_MANY_DEVICE                = $00000049;    // Device numbers error, should not be larger than 128.
MW_ERROR_DEVICE_NOT_ENOUGH_USER_MEMORY  = $0000004a;    // There is no enough memory for user code.
MW_ERROR_FAKE_DEVICE                    = $0000004b;    // Device is fake.
MW_ERROR_DEVICE_BLK_OUT_RANGE           = $0000004c;    // Device bulk read or write out range.
MW_ERROR_DEVICE_FS_ERR_BAK_ERROR        = $0000004d;    // Device backup error.
MW_ERROR_DEVICE_TMR_ERR_ADJUST_TIME_TIMEOUT = $0000004e;    // Adjust time is time out.
MW_ERROR_DEVICE_EXCH_ERROR              = $0000004f;    // Exchange memory error.
MW_ERROR_DEVICE_AES_ERR                 = $00000050;    // Device AES error.
MW_ERROR_DEVICE_HASH_ERR_UNSUPPORTED_ALGO = $00000051;    // Unsupported hash algo.
MW_ERROR_DEVICE_BAD_PRIVATE_KEY         = $00000052;    // Bad private key.
MW_ERROR_DEVICE_BAD_PUBLIC_KEY          = $00000053;    // Bad public key.
MW_ERROR_DEVICE_BAD_SYMMETRIC_KEY       = $00000054;    // Bad symmetric key.
MW_ERROR_DEVICE_BAD_SIGNATURE           = $00000055;    // Bad signature.
MW_ERROR_DEVICE_KEY_ERR_BAD_ALGO        = $00000056;    // Bad algo.
MW_ERROR_DEVICE_LOGO_ERR_LOGO           = $00000057;    // Bad logo.
MW_ERROR_EXEC_PARAM_TOO_LONG            = $00000058;    // Execute parameter data is too long.
MW_ERROR_OPEN_ERROR                     = $00000059;    // Open device error.
MW_ERROR_LOAD_SYS_MODULE_ERROR          = $0000005A;    // Load system module error.
MW_ERROR_SYS_MODULE_FUNCTION_ERROR      = $0000005B;    // System module function address error.
MW_ERROR_RSA                            = $0000005C;    // Device RSA crypt error.
MW_ERROR_KEY                            = $0000005D;    // Crypt Key error.
MW_ERROR_DEVICE_EXEC_ERR_UNALIGNED_MEM_ADDR = $0000005E;    // Unaligned memory address.
MW_ERROR_DEVICE_EXEC_ERR_STACK_OVERFLOW = $0000005F;    // User stack overflow.
MW_ERROR_DEVELOPER_ID_MISMATCH          = $00000060;    // Developer ID not match.
MW_ERROR_LM_GENERAL_ERROR               = $00000061;    // LM return data format wrong
MW_ERROR_LM_UNSUPPORTED_CERT_TYPE       = $00000062;    // Unsupported certificate type
MW_ERROR_LM_UNSUPPORTED_UPDATE_OBJECT_TYPE = $00000063;    // Unsupported object type to update
MW_ERROR_LM_UPDATE_PKG_FORMAT_WRONG     = $00000064;    // Update Package format wrong
MW_ERROR_CERT                           = $00000065;    // Certificates wrong
MW_ERROR_DEX_NOT_LOADED                 = $00000066;    // Dynamic Executive not loaded to the lock
MW_ERROR_SYSAPP_NOT_FOUND               = $00000067;    // System Application Not Found
MW_ERROR_UNSUPPORTED_OBJECT_TYPE        = $00000068;    // Unsupported update object type
MW_ERROR_UPDATE_BLOCK_LENGTH            = $00000069;    // update block length wrong
MW_ERROR_UPDATE_SEED_NOT_FOUND          = $0000006A;    // update seed not found
MW_ERROR_HEAP                           = $0000006B;    // The system heap in the lock crashed
MW_ERROR_DEX_OUT_OF_RANGE               = $0000006C;    // Dynamic Executive out of range
MW_ERROR_DEX_TOO_LARGE                  = $0000006D;    // Dynamic Executive too large
MW_ERROR_UPDATE_INFO                    = $0000006E;    // Update information wrong
MW_ERROR_DEVICE_STACK_OVERFLOW          = $0000006F;    // The system stack in the lock crashed
MW_ERROR_COMMUNICATION_CRYPT            = $00000070;    // Communication Encrypt wrong
MW_ERROR_BAD_UPDATE_PKG                 = $00000071;    // Bad Update Package
MW_ERROR_UPDATE_PKG_VERSION_LOW         = $00000072;    // Update Package version low
MW_ERROR_UPDATE_OBJECT_TYPE             = $00000073;    // Update Object type wrong
MW_ERROR_UPDATE_DEVELOPER_ID            = $00000074;    // wrong developer id
MW_ERROR_UPDATE_FILE_TYPE               = $00000075;    // wrong file type
MW_ERROR_NO_ADJUST_TIME_REQUEST         = $00000076;    // no adjust time request have been get
MW_ERROR_CERT_REVOKED                   = $00000077;    // Certificates revoked
MW_ERROR_WRONG_CERT_SLOT                = $00000078;    // Certificates slot number wrong
MW_ERROR_DEVICE_HASH_ERR_NOT_MATCH      = $00000079;    // Hash value not match
MW_ERROR_BAD_RND                        = $0000007A;    // Bad Random
MW_ERROR_RTC                            = $0000007B;    // RTC error
MW_ERROR_RTC_POWER                      = $0000007C;    // RTC power drop

MW_ERROR_NO_PID_DEVICE                  = $00002001;    // 主机上找不到指定的PID魔锐锁
MW_ERROR_RET_DATA_FORMAT                = $00002002;    // 返加数据格式不正确
MW_ERROR_FILE_NAME_LEN                  = $00002003;    // 文件名长度不正确
MW_ERROR_BLOCK_DATA                     = $00002004;    // 数据块错误
MW_ERROR_FILE_PROPERTY_PARAM            = $00002005;    // 文件属性参数错误
MW_ERROR_FILE_COUNT_MAX                 = $00002006;    // 实际文件个数超过预分配的个数
MW_ERROR_MALLOC_MEMORY                  = $00002007;    // 分配内存失败
MW_ERROR_PIN_LENGTH                     = $00002008;    // PIN长度有错误
MW_ERROR_UPDATA_PKG_PID                 = $00002009;    // 升级包PID与锁内PID不一致

MW_ERROR_ENTRY_TYPE                     = $0000A001;    // entry 类型错误
MW_ERROR_ENTRY_FLAG                     = $0000A002;    // entry 标识错误
MW_ERROR_OPCODE                         = $0000A003;    // 错误命令
MW_ERROR_PARAM                          = $0000A004;    // 错误参数
MW_ERROR_DATA_LENGTH                    = $0000A005;    // 错误数据长度
MW_ERROR_PIN                            = $0000A006;    // 错误PIN 码
MW_ERROR_DEVICE_BLOCK                   = $0000A007;    // 设备PIN 码锁死
MW_ERROR_PRIVILEGE                      = $0000A008;    // 没有权限
MW_ERROR_DATA_INFO                      = $0000A009;    // 数据域数据错误
MW_ERROR_HASH_TYPE                      = $0000A00A;    // 错误HASH 类型
MW_ERROR_ALG_MODE                       = $0000A00B;    // 错误的模式
MW_ERROR_INIT_EVT                       = $0000A00C;    // 初始化过程问题错误
MW_ERROR_ALG_TYPE                       = $0000A00D;    // 错误算法类型
MW_ERROR_BIT_SIZE                       = $0000A00E;    // 错误的密钥位大小
MW_ERROR_FILE_TYPE                      = $0000A00F;    // 错误的文件类型
MW_ERROR_NO_INIT_PID                    = $0000A010;    // PID未初始化
MW_ERROR_NO_INIT_PIN                    = $0000A011;    // PIN码未修改
MW_ERROR_NO_VALIDATE_PKG                = $0000A012;    // 无效升级包
MW_ERROR_AUTH_STAGE                     = $0000A013;    // 权限价段错误
MW_ERROR_FILE_PRIVILEGE                 = $0000A014;    // 文件权限错误
MW_ERROR_FILE_RANGE                     = $0000A015;    // 文件范围错误
MW_ERROR_KEY_FILE                       = $0000A016;    // 文件密钥错误
MW_ERROR_REQ_IDENTIFY                   = $0000A017;    // 返回认证错误
MW_ERROR_INVALID_SEED                   = $0000A018;    // 无效种子错误
MW_ERROR_SEED_CHECK                     = $0000A019;    // 种子校验错误
MW_ERROR_UPDATE_HASH                    = $0000A01A;    // 升级哈希错误
MW_ERROR_UPDATE_PID                     = $0000A01B;    // 升级PID错误
MW_ERROR_UPDATE_INDEX                   = $0000A01C;    // 升级索引错误
MW_ERROR_UPDATE_TIME                    = $0000A01D;    // 升级时间错误
MW_ERROR_UPDATE_SERIAL_NUM              = $0000A01E;    // 升级序列号错误
MW_ERROR_UPDATE_SN                      = $0000A01F;    // 升级SN错误
MW_ERROR_UPDATE_ORDER                   = $0000A020;    // 升级顺序错误
MW_ERROR_UPDATE_OPERATE                 = $0000A021;    // 长级操作错误
MW_ERROR_KEY_TYPE                       = $0000A022;    // 关键字类型错误
MW_ERROR_OTHER                          = $0000AFFF;    // 其他错误

const
//打开锁的方式
////共享打开  */
MW_OPEN_SHARE_MODE                                  = 0;
////独占打开 */
MW_OPEN_EXCLUSIVE_MODE                              = 1;
//============================================================
//  算法类型表示
//============================================================
////算法类型AES */
MW_ALG_TYPE_AES                                     = $00;
////算法类型DES */
MW_ALG_TYPE_DES                                     = $01;
////算法类型TDES */
MW_ALG_TYPE_TDES                                    = $02;
////算法类型ECC */
MW_ALG_TYPE_ECC                                     = $10;
////算法类型RSA */
MW_ALG_TYPE_RSA                                     = $11;
//============================================================
//  对称算法模式
//============================================================
////对称算法模式 ECB mode */
MW_SYM_ALGO_MODE_ECB                                = $00;
////对称算法模式 CBC mode */
MW_SYM_ALGO_MODE_CBC                                = $01;
//============================================================
//  文件属性相关宏定义
//============================================================
////二进制文件 */
MW_FILE_TYPE_BINARY                                 = 0;
////密钥文件 */
MW_FILE_TYPE_KEY                                    = 2;
////密钥文件权限类型 */
MW_FILE_PRIV_TYPE_USE                               = $00;
////数据文件用户只读权限类型 */
MW_FILE_PRIV_TYPE_READ                              = $01;
////数据文件用户读写权限类型 */
MW_FILE_PRIV_TYPE_WRITE                             = $02;
//============================================================
//  获取设备信息宏定义 
//============================================================
////获取PID     返回4字节PID */
MW_GET_INFO_ITEM_PID  	                            = $00;
////获取序列号  返回16字节获取序列号 */
MW_GET_INFO_ITEM_SN                                 = $01;
////获取生产日期 返回4字节生产日期 */
MW_GET_INFO_ITEM_PRODUCE_DATE                       = $02;
////获取总容量   返回4字节总容量 */
MW_GET_INFO_ITEM_ALL_CAPACITY  	                    = $03;
////获取剩余容量 返回4字节剩余容量 */
MW_GET_INFO_ITEM_FREE_CAPACITY                      = $04;
////获取设备版本 返回4字节设备版本 */
MW_GET_INFO_ITEM_VERSION                            = $05;
////获取外壳号   返回第一字节为外壳号长度，后面是外壳号数据。注：最大256字节。 */
MW_GET_INFO_ITEM_SHELL_SN                           = $06;
////获取设置状态 返回1字节的标识，参见宏定义MW_STATUS_FLAG_PID、MW_STATUS_FLAG_PIN*/
MW_GET_INFO_ITEM_STATUS                             = $07;
////获取设备远程升级码      返回4 字节设备远程升级码 */
MW_GET_INFO_ITEM_UPDATE_CODE                        = $08;
////获取远程升级控制锁标识  返回1字节标识 */
MW_GET_INFO_ITEM_UPDATE_CTRL_CODE                   = $09;
////获取所有信息，返回MW_DEV_ALL_ITEMINFO结构体的大小 */
MW_GET_INFO_ITEM_ALL                                = $FF;
////默认状态 */
MW_STATUS_FLAG_DEFAULT                              = $00;
////已经设置PID状态 */
MW_STATUS_FLAG_PID                                  = $01;
////已经修改PIN状态 */
MW_STATUS_FLAG_PIN                                  = $02;
//============================================================
//  控制码
//============================================================
////复位设备，清除登陆状态。 */
MW_CTRL_CODE_ITEM_RESET                             = $00;
////控制LED。  注：数据第一字节非0标识LED亮，0表示灭 */
MW_CTRL_CODE_ITEM_LED                               = $01;
//============================================================
//  PIN类型定义
//============================================================
////默认PIN类型 */
MW_PIN_TYPE_NONE                                    = $00;
////用户PIN类型 */
MW_PIN_TYPE_USER                                    = $01;
////开发商PIN类型 */
MW_PIN_TYPE_DEVELOPER                               = $02;
////用户PIN长度 */
MW_PIN_LENGTH_USER                                  = 8;
////开发商PIN长度 */
MW_PIN_LENGTH_DEVELOPER                             = 24;

//============================================================
//  RSA加密模式定义
//============================================================
////RSA加密模式 */
MW_RSA_MODE_NORMAL                         = $00;
////RSA加密模式 */
MW_RSA_MODE_PKCS1_V1_5                     = $01;
//============================================================
//  RSA加密数据长度
//============================================================
////RSA-1024加密数据长度 */
MW_RSA_1024_BYTE_SIZE                      = 128;
////RSA-2048加密数据长度 */
MW_RSA_2048_BYTE_SIZE                      = 256;



//============================================================
//  签名hash算法类型
//============================================================
////签名MD5 hash算法 */
MW_HASH_ALGO_MD5                           = $00;
////签名SHA1 hash算法 */
MW_HASH_ALGO_SHA1                          = $01;
////签名SHA256 hash算法 */
MW_HASH_ALGO_SHA256                        = $02;
////签名默认 hash算法 */
MW_HASH_ALGO_NONE                          = $FF;



//============================================================
//  升级包相关宏定义
//============================================================
////一个升级包最大包含升级文件个数 */
MW_UPDATE_FILE_COUNT_MAX                   = 50;

////升级文件时的操作标志 删除文件 */
MW_UPDATE_MOD_FLAG_DELETE                 = $00000002;
////升级文件时的操作标志 写文件 */
MW_UPDATE_MOD_FLAG_WRITE                  = $00000010;
////升级文件时的操作标志 创建文件 */
MW_UPDATE_MOD_FLAG_CREATE                  = $00000020;
//============================================================
//  多语言宏定义
//============================================================
////默认语言 */
MW_LANGUAGE_ID_DEFAULT                     = 0;
////中文 */
MW_LANGUAGE_ID_CH                          = 1;
////英文 */
MW_LANGUAGE_ID_EN                          = 2;
//============================================================
//  魔锐相关大小宏定义
//============================================================
////最大路径 */
MW_PATH_LEN                                = 1024;
////对称加密算法向量长度 */
MW_SYM_ALG_IV_LENGTH                       = 16;

////设备唯一序列号长度 */
MW_SN_LENGTH                               = 16;

////文件名最大长度 */
MW_FILE_NAME_LENGTH_MAX                    = 16;

////HMAC MD5计算结果长度 */
MW_HMAC_MD5_DIGEST_LENGTH                  = 16;
////HMAC SHA1计算结果长度 */
MW_HMAC_SHA1_DIGEST_LENGTH                 = 20;
////HMAC SHA256计算结果长度 */
MW_HMAC_SHA256_DIGEST_LENGTH               = 32;

////一次最大转输数据长度 */
MW_INPUT_DATA_LENGTH_MAX                   = 1900;

//种子码最大长度宏定义
////最大种子码长度 */
MW_SEED_LENGTH_MAX                         = 32;
////最小种子码长度 */
MW_SEED_LENGTH_MIN                         = 4;

MW_ENUM_DEVICE_MAX_COUNT                   = 128;

type
    MWHANDLE = Pointer;

//============================================================
//  结构体定义
//============================================================

//设备信息结构体 */
PMW_DEVICE_INFO_CTX = ^MW_DEVICE_INFO_CTX;
MW_DEVICE_INFO_CTX = PACKED RECORD
	//产品ID */
    uiPID: LONGWORD;
	//唯一序列号 */
    ucSerialNum: array[0..MW_SN_LENGTH - 1] of BYTE;
	//设备的路径 */
    ucDevPath: array[0..MW_PATH_LEN - 1] of BYTE;
	//通讯协议 */
    uiProtocol: LONGWORD;  
	//Mac OS 系统 LocationID */
	uiLocationID: LONGWORD;
END;

//文件属性结构体 */
PMW_FILE_PROPERTY = ^MW_FILE_PROPERTY;
MW_FILE_PROPERTY = PACKED RECORD
    //该成员变量暂时不用 */
	ucValidate: BYTE; 
	//文件类型：二进制文件或者密钥文件。 */
	ucType: BYTE;   
	//文件权限 */
	usPrivilege: WORD;
	//文件大小 */
	uiSize: LONGWORD;    
	//创建时间 */
	uiTime: LONGWORD;   
	//文件名称 */
	acName: array[0..MW_FILE_NAME_LENGTH_MAX] of ANSICHAR; 
END;

//系统工作状态结构体 */
PMW_DEVICE_STATUS = ^MW_DEVICE_STATUS;
MW_DEVICE_STATUS = PACKED RECORD
	//系统时间 */
	uiSysTime: LONGWORD;
	//系统状态 */
	uiSysStatus: LONGWORD;
	//保留 */
	usReserved: WORD;
	//登录状态 */
	ucLoginStatus: BYTE;
	//灯的状态 */
	ucLedStatus: BYTE;
END;

//远程升级结构体 */
PMW_UPDADE_FILE_CTX = ^MW_UPDADE_FILE_CTX;
MW_UPDADE_FILE_CTX = PACKED RECORD
	//文件属性结构体 */
	stFileProperty: MW_FILE_PROPERTY;
	//升级文件的数据内容 */
	pData: PAnsiChar; 
	//升级文件的数据大小 */
	uiDateSize: LONGWORD;
END;

//设备信息子项结构体 */
PMW_DEVICE_ALL_ITEM_INFO = ^MW_DEVICE_ALL_ITEM_INFO;
MW_DEVICE_ALL_ITEM_INFO = PACKED RECORD
	//产品号 */
	uiPID: LONGWORD;
	//唯一序列号 */
	acSN: array[0..MW_SN_LENGTH - 1] of BYTE;
	//生产时间 */
	uiProduceDate: LONGWORD;
	//总容量 */
	uiAllCapacity: LONGWORD;
	//可用容量 */
	uiFreeCapacity: LONGWORD;
	//版本号 */
	uiVersion: LONGWORD;
	//升级码 */
	uiUpdateCode: LONGWORD;
	//状态 */
	ucStatus: BYTE;
	//升级控制码 */
	ucUpdateCtrlCode: BYTE;
	//保留 */
	reserved: WORD;
END;

