using System;
using System.Runtime.InteropServices;

using SLM_HANDLE_INDEX = System.UInt32;

namespace Moway
{
    using System.Text;
    //============================================================
    //  类型定义
    //============================================================
    using MWHANDLE = IntPtr;

    /// <summary>
    /// 
    /// </summary>
    internal class MowayCS
    {
        //============================================================
        //  函数返回值定义
        //============================================================
        public const UInt32 MW_SUCCESS = 0x00000000;                  // Success.
        public const UInt32 MW_ERROR_INVALID_HANDLE = 0x00000001;                  // Handle may be NULL.
        public const UInt32 MW_ERROR_INVALID_PARAMETER = 0x00000002;                  // Invalid parameter.
        public const UInt32 MW_ERROR_NOT_ENOUGH_MEMORY = 0x00000003;                  // Not enough storage is available to process this command.
        public const UInt32 MW_ERROR_NO_DEVICE = 0x00000004;                  // No device or no specific device on the pc.
        public const UInt32 MW_ERROR_TIMEOUT = 0x00000005;                  // Time out.
        public const UInt32 MW_ERROR_UNSUPPORTED_FLAG = 0x00000006;                  // A unsupported flag was given to the APIs.
        public const UInt32 MW_ERROR_INSUFFICIENT_BUFFER = 0x00000007;                  // Buffer is too small to hold the data.
        public const UInt32 MW_ERROR_EXCHG_MEMORY_NOT_FOUND = 0x00000008;                  // Specific share memory is not found.
        public const UInt32 MW_ERROR_SYSTEM_FILE_NOT_FOUND = 0x00000009;                  // File in system not found.
        public const UInt32 MW_ERROR_SYSTEM_FILE_INVALID_ACCESS = 0x0000000a;                  // Can not access system file.
        public const UInt32 MW_ERROR_FILE_EXISTS = 0x0000000b;                  // Specific file is exist.
        public const UInt32 MW_ERROR_FILE_NOT_FOUND = 0x0000000c;                  // Specific file is not found.
        public const UInt32 MW_ERROR_NO_PRIVILEGE = 0x0000000d;                  // The operation is required a high level privilege.
        public const UInt32 MW_ERROR_WRONG_PASSWORD = 0x0000000e;                  // Password is incorrect.
        public const UInt32 MW_ERROR_PASSWORD_LOCKED = 0x0000000f;                  // Password is locked.
        public const UInt32 MW_ERROR_FUNCTION_NOT_SUPPORTED = 0x00000011;                  // the function is not supported
        public const UInt32 MW_ERROR_COMMUNICATION = 0x00000015;                  // Communication error.
        public const UInt32 MW_ERROR_UNSUPPORTED_PASSWORD_TYPE = 0x00000016;                  // A unsupported password type was given to APIs.
        public const UInt32 MW_ERROR_WRONG_PASSWORD_LENGTH = 0x00000017;                  // Password length is not correct, developer password is 24 bytes, user password is 8 bytes.
        public const UInt32 MW_ERROR_ALREADY_EXCLUSIVELY_LOGIN = 0x00000018;                  // The device is already exclusively login, can not login with share mode.
        public const UInt32 MW_ERROR_ALREADY_SHARED_LOGIN = 0x00000019;                  // The device is already shared login, can not login with exclusively mode.                 
        public const UInt32 MW_ERROR_ALREADY_TEMP_EXCLUSIVELY_USING = 0x0000001a;                  // The handle is already temporary using.
        public const UInt32 MW_ERROR_NOT_TEMP_EXCLUSIVELY_USING = 0x0000001b;                  // The handle is not temporary using.
        public const UInt32 MW_ERROR_TOO_MUCH_DATA = 0x0000001c;                  // The massage reply function has a data length limit.                 
        public const UInt32 MW_ERROR_INSUFFICIENT_DEVICE_SPACE = 0x0000001e;                  // The device space is insufficient.
        public const UInt32 MW_ERROR_DEVICE_FILESYSTEM_ERROR = 0x0000001f;                  // Device file system error.
        public const UInt32 MW_ERROR_FILE_OUT_RANGE = 0x00000020;                  // Device file is out range.
        public const UInt32 MW_ERROR_UNSUPPORTED_FILE_TYPE = 0x00000021;                  // an unsupported file type is given to EV APIs.
        public const UInt32 MW_ERROR_FILE_OFFSET_MUST_BE_ZERO = 0x00000022;                  // When read or write a key file, offset must be 0.  
        public const UInt32 MW_ERROR_BAD_EXECUTIVE_FILE_FORMAT = 0x00000023;                  // the executive file format is incorrect.
        public const UInt32 MW_ERROR_OPEN_TOO_MANY_DEVICE_FILES = 0x00000024;                  // Open too many device files.
        public const UInt32 MW_ERROR_INVALID_DEVICE_FILE_HANDLE = 0x00000025;                  // device file handle is incorrect.
        public const UInt32 MW_ERROR_FILE_NOT_FINISHED = 0x00000026;                  // The file is not finish.
        public const UInt32 MW_ERROR_BAD_FILENAME = 0x00000027;                  // Filename is incorrect.
        public const UInt32 MW_ERROR_BAD_NAME = 0x00000028;                  // The filename, directory name, or volume label syntax is incorrect.
        public const UInt32 MW_ERROR_DEVICE_TIMER = 0x00000029;                  // Device timer error.
        public const UInt32 MW_ERROR_NO_EXECUTIVE_FILE_RUNNING = 0x0000002a;                  // No process is running in the device. 
        public const UInt32 MW_ERROR_DEVICE_USER_BUFFER_FULL = 0x0000002b;                  // Can not send data when device user buffer is full.
        public const UInt32 MW_ERROR_DEVICE_USER_BUFFER_EMPTY = 0x0000002c;                  // Can receive data when device user buffer is empty.
        public const UInt32 MW_ERROR_DEVICE_MSG_NOT_REPLIED = 0x0000002d;                  // Device need a message reply first.
        public const UInt32 MW_ERROR_DEVICE_DUMMY_MSG = 0x0000002e;                  // Device not need a message reply.
        public const UInt32 MW_ERROR_DEVICE_MEMORY_ADDR = 0x0000002f;                  // Device memory address error.
        public const UInt32 MW_ERROR_DEVICE_MEMORY_LENGTH = 0x00000030;                  // Device memory length error.
        public const UInt32 MW_ERROR_CONTROL_CODE = 0x00000031;                  // Give an unsupported control code.
        public const UInt32 MW_ERROR_UNKNOW_COMMAND = 0x00000032;                  // Give an unsupported command to device.
        public const UInt32 MW_ERROR_INVALID_COMMAND_PARAMETER = 0x00000033;                  // Command parameter error.
        public const UInt32 MW_ERROR_COMMAND_DATA_LENGTH = 0x00000034;                  // Command data length error.
        public const UInt32 MW_ERROR_DEVICE_BUFFER_FULL = 0x00000035;                  // Device buffer is full.
        public const UInt32 MW_ERROR_EXECUTIVE_FILE_RUNNING = 0x00000036;                  // When a process is running in the device, some operation unsupported.
        public const UInt32 MW_ERROR_NO_DEVICE_MESSAGE = 0x00000037;                  // No device message.
        public const UInt32 MW_ERROR_LOGIN_COUNT = 0x00000038;                  // Device Login count error.
        public const UInt32 MW_ERROR_KEYEXCHANGE_FAILED = 0x00000039;                  // Communication key exchange error.
        public const UInt32 MW_ERROR_BAD_COMMUNICATION_KEY = 0x0000003a;                  // Communication key is incorrect.
        public const UInt32 MW_ERROR_BAD_DEVICE_TIME = 0x0000003b;                  // Device time error.
        public const UInt32 MW_ERROR_BAD_DEVICE_INFOMATION = 0x0000003c;                  // Device information error.
        public const UInt32 MW_ERROR_BAD_COMMAND_SEQUENCE = 0x0000003d;                  // Command sequence is not right.
        public const UInt32 MW_ERROR_COMMUNICATION_DATA_LENGTH = 0x0000003e;                  // Communication data length error.
        public const UInt32 MW_ERROR_ECC = 0x0000003f;                  // Device ECC crypt error.
        public const UInt32 MW_ERROR_BAD_UPDATE_DATA_LENGTH = 0x00000040;                  // Update data length is incorrect.
        public const UInt32 MW_ERROR_UPDATE_STATE = 0x00000042;                  // Update state is incorrect.
        public const UInt32 MW_ERROR_UPDATE_KEY_NOT_ENABLE = 0x00000043;                  // When use remote update, remote update key must set first.
        public const UInt32 MW_ERROR_LOCKED_FOREVER = 0x00000044;                  // Device is locked forever.
        public const UInt32 MW_ERROR_LOCKED_TEMPORARY = 0x00000045;                  // Device is locked temporary.
        public const UInt32 MW_ERROR_DEVICE_UNLOCKED = 0x00000046;                  // Device is not locked.
        public const UInt32 MW_ERROR_DEVICE_FLASH = 0x00000047;                  // Device flash error.
        public const UInt32 MW_ERROR_DEVICE_ERROR = 0x00000048;                  // Device error.
        public const UInt32 MW_ERROR_TOO_MANY_DEVICE = 0x00000049;                  // Device numbers error, should not be larger than 128.
        public const UInt32 MW_ERROR_DEVICE_NOT_ENOUGH_USER_MEMORY = 0x0000004a;                  // There is no enough memory for user code.
        public const UInt32 MW_ERROR_FAKE_DEVICE = 0x0000004b;                  // Device is fake.
        public const UInt32 MW_ERROR_DEVICE_BLK_OUT_RANGE = 0x0000004c;                  // Device bulk read or write out range.
        public const UInt32 MW_ERROR_DEVICE_FS_ERR_BAK_ERROR = 0x0000004d;                  // Device backup error.
        public const UInt32 MW_ERROR_DEVICE_TMR_ERR_ADJUST_TIME_TIMEOUT = 0x0000004e;                  // Adjust time is time out.
        public const UInt32 MW_ERROR_DEVICE_EXCH_ERROR = 0x0000004f;                  // Exchange memory error.
        public const UInt32 MW_ERROR_DEVICE_AES_ERR = 0x00000050;                  // Device AES error.
        public const UInt32 MW_ERROR_DEVICE_HASH_ERR_UNSUPPORTED_ALGO = 0x00000051;                  // Unsupported hash algo.
        public const UInt32 MW_ERROR_DEVICE_BAD_PRIVATE_KEY = 0x00000052;                  // Bad private key.
        public const UInt32 MW_ERROR_DEVICE_BAD_PUBLIC_KEY = 0x00000053;                  // Bad public key.
        public const UInt32 MW_ERROR_DEVICE_BAD_SYMMETRIC_KEY = 0x00000054;                  // Bad symmetric key.
        public const UInt32 MW_ERROR_DEVICE_BAD_SIGNATURE = 0x00000055;                  // Bad signature.
        public const UInt32 MW_ERROR_DEVICE_KEY_ERR_BAD_ALGO = 0x00000056;                  // Bad algo.
        public const UInt32 MW_ERROR_DEVICE_LOGO_ERR_LOGO = 0x00000057;                  // Bad logo.
        public const UInt32 MW_ERROR_EXEC_PARAM_TOO_LONG = 0x00000058;                  // Execute parameter data is too long.
        public const UInt32 MW_ERROR_OPEN_ERROR = 0x00000059;                  // Open device error.
        public const UInt32 MW_ERROR_LOAD_SYS_MODULE_ERROR = 0x0000005A;                  // Load system module error.
        public const UInt32 MW_ERROR_SYS_MODULE_FUNCTION_ERROR = 0x0000005B;                  // System module function address error.
        public const UInt32 MW_ERROR_RSA = 0x0000005C;                  // Device RSA crypt error.
        public const UInt32 MW_ERROR_KEY = 0x0000005D;                  // Crypt Key error.
        public const UInt32 MW_ERROR_DEVICE_EXEC_ERR_UNALIGNED_MEM_ADDR = 0x0000005E;                  // Unaligned memory address.
        public const UInt32 MW_ERROR_DEVICE_EXEC_ERR_STACK_OVERFLOW = 0x0000005F;                  // User stack overflow.
        public const UInt32 MW_ERROR_DEVELOPER_ID_MISMATCH = 0x00000060;                  // Developer ID not match.
        public const UInt32 MW_ERROR_LM_GENERAL_ERROR = 0x00000061;                  // LM return data format wrong
        public const UInt32 MW_ERROR_LM_UNSUPPORTED_CERT_TYPE = 0x00000062;                  // Unsupported certificate type
        public const UInt32 MW_ERROR_LM_UNSUPPORTED_UPDATE_OBJECT_TYPE = 0x00000063;                  // Unsupported object type to update
        public const UInt32 MW_ERROR_LM_UPDATE_PKG_FORMAT_WRONG = 0x00000064;                  // Update Package format wrong
        public const UInt32 MW_ERROR_CERT = 0x00000065;                  // Certificates wrong
        public const UInt32 MW_ERROR_DEX_NOT_LOADED = 0x00000066;                  // Dynamic Executive not loaded to the lock
        public const UInt32 MW_ERROR_SYSAPP_NOT_FOUND = 0x00000067;                  // System Application Not Found
        public const UInt32 MW_ERROR_UNSUPPORTED_OBJECT_TYPE = 0x00000068;                  // Unsupported update object type
        public const UInt32 MW_ERROR_UPDATE_BLOCK_LENGTH = 0x00000069;                  // update block length wrong
        public const UInt32 MW_ERROR_UPDATE_SEED_NOT_FOUND = 0x0000006A;                  // update seed not found
        public const UInt32 MW_ERROR_HEAP = 0x0000006B;                  // The system heap in the lock crashed
        public const UInt32 MW_ERROR_DEX_OUT_OF_RANGE = 0x0000006C;                  // Dynamic Executive out of range
        public const UInt32 MW_ERROR_DEX_TOO_LARGE = 0x0000006D;                  // Dynamic Executive too large
        public const UInt32 MW_ERROR_UPDATE_INFO = 0x0000006E;                  // Update information wrong
        public const UInt32 MW_ERROR_DEVICE_STACK_OVERFLOW = 0x0000006F;                  // The system stack in the lock crashed
        public const UInt32 MW_ERROR_COMMUNICATION_CRYPT = 0x00000070;                  // Communication Encrypt wrong
        public const UInt32 MW_ERROR_BAD_UPDATE_PKG = 0x00000071;                  // Bad Update Package
        public const UInt32 MW_ERROR_UPDATE_PKG_VERSION_LOW = 0x00000072;                  // Update Package version low
        public const UInt32 MW_ERROR_UPDATE_OBJECT_TYPE = 0x00000073;                  // Update Object type wrong
        public const UInt32 MW_ERROR_UPDATE_DEVELOPER_ID = 0x00000074;                  // wrong developer id
        public const UInt32 MW_ERROR_UPDATE_FILE_TYPE = 0x00000075;                  // wrong file type
        public const UInt32 MW_ERROR_NO_ADJUST_TIME_REQUEST = 0x00000076;                  // no adjust time request have been get
        public const UInt32 MW_ERROR_CERT_REVOKED = 0x00000077;                  // Certificates revoked
        public const UInt32 MW_ERROR_WRONG_CERT_SLOT = 0x00000078;                  // Certificates slot number wrong
        public const UInt32 MW_ERROR_DEVICE_HASH_ERR_NOT_MATCH = 0x00000079;                  // Hash value not match
        public const UInt32 MW_ERROR_BAD_RND = 0x0000007A;                  // Bad Random
        public const UInt32 MW_ERROR_RTC = 0x0000007B;                  // RTC error
        public const UInt32 MW_ERROR_RTC_POWER = 0x0000007C;                  // RTC power drop

        public const UInt32 MW_ERROR_NO_PID_DEVICE = 0x00002001;                  // 主机上找不到指定的PID魔锐锁
        public const UInt32 MW_ERROR_RET_DATA_FORMAT = 0x00002002;                  // 返加数据格式不正确
        public const UInt32 MW_ERROR_FILE_NAME_LEN = 0x00002003;                  // 文件名长度不正确
        public const UInt32 MW_ERROR_BLOCK_DATA = 0x00002004;                  // 数据块错误
        public const UInt32 MW_ERROR_FILE_PROPERTY_PARAM = 0x00002005;                  // 文件属性参数错误
        public const UInt32 MW_ERROR_FILE_COUNT_MAX = 0x00002006;                  // 实际文件个数超过预分配的个数
        public const UInt32 MW_ERROR_MALLOC_MEMORY = 0x00002007;                  // 分配内存失败
        public const UInt32 MW_ERROR_PIN_LENGTH = 0x00002008;                  // PIN长度有错误
        public const UInt32 MW_ERROR_UPDATA_PKG_PID = 0x00002009;                  // 升级包PID与锁内PID不一致

        public const UInt32 MW_ERROR_ENTRY_TYPE = 0x0000A001;                  // entry 类型错误
        public const UInt32 MW_ERROR_ENTRY_FLAG = 0x0000A002;                  // entry 标识错误
        public const UInt32 MW_ERROR_OPCODE = 0x0000A003;                  // 错误命令
        public const UInt32 MW_ERROR_PARAM = 0x0000A004;                  // 错误参数
        public const UInt32 MW_ERROR_DATA_LENGTH = 0x0000A005;                  // 错误数据长度
        public const UInt32 MW_ERROR_PIN = 0x0000A006;                  // 错误PIN 码
        public const UInt32 MW_ERROR_DEVICE_BLOCK = 0x0000A007;                  // 设备PIN 码锁死
        public const UInt32 MW_ERROR_PRIVILEGE = 0x0000A008;                  // 没有权限
        public const UInt32 MW_ERROR_DATA_INFO = 0x0000A009;                  // 数据域数据错误
        public const UInt32 MW_ERROR_HASH_TYPE = 0x0000A00A;                  // 错误HASH 类型
        public const UInt32 MW_ERROR_ALG_MODE = 0x0000A00B;                  // 错误的模式
        public const UInt32 MW_ERROR_INIT_EVT = 0x0000A00C;                  // 初始化过程问题错误
        public const UInt32 MW_ERROR_ALG_TYPE = 0x0000A00D;                  // 错误算法类型
        public const UInt32 MW_ERROR_BIT_SIZE = 0x0000A00E;                  // 错误的密钥位大小
        public const UInt32 MW_ERROR_FILE_TYPE = 0x0000A00F;                  // 错误的文件类型
        public const UInt32 MW_ERROR_NO_INIT_PID = 0x0000A010;                  // PID未初始化
        public const UInt32 MW_ERROR_NO_INIT_PIN = 0x0000A011;                  // PIN码未修改
        public const UInt32 MW_ERROR_NO_VALIDATE_PKG = 0x0000A012;                  // 无效升级包
        public const UInt32 MW_ERROR_AUTH_STAGE = 0x0000A013;                  // 权限价段错误
        public const UInt32 MW_ERROR_FILE_PRIVILEGE = 0x0000A014;                  // 文件权限错误
        public const UInt32 MW_ERROR_FILE_RANGE = 0x0000A015;                  // 文件范围错误
        public const UInt32 MW_ERROR_KEY_FILE = 0x0000A016;                  // 文件密钥错误
        public const UInt32 MW_ERROR_REQ_IDENTIFY = 0x0000A017;                  // 返回认证错误
        public const UInt32 MW_ERROR_INVALID_SEED = 0x0000A018;                  // 无效种子错误
        public const UInt32 MW_ERROR_SEED_CHECK = 0x0000A019;                  // 种子校验错误
        public const UInt32 MW_ERROR_UPDATE_HASH = 0x0000A01A;                  // 升级哈希错误
        public const UInt32 MW_ERROR_UPDATE_PID = 0x0000A01B;                  // 升级PID错误
        public const UInt32 MW_ERROR_UPDATE_INDEX = 0x0000A01C;                  // 升级索引错误
        public const UInt32 MW_ERROR_UPDATE_TIME = 0x0000A01D;                  // 升级时间错误
        public const UInt32 MW_ERROR_UPDATE_SERIAL_NUM = 0x0000A01E;                  // 升级序列号错误
        public const UInt32 MW_ERROR_UPDATE_SN = 0x0000A01F;                  // 升级SN错误
        public const UInt32 MW_ERROR_UPDATE_ORDER = 0x0000A020;                  // 升级顺序错误
        public const UInt32 MW_ERROR_UPDATE_OPERATE = 0x0000A021;                  // 长级操作错误
        public const UInt32 MW_ERROR_KEY_TYPE = 0x0000A022;                  // 关键字类型错误
        public const UInt32 MW_ERROR_OTHER = 0x0000AFFF;                  // 其他错误

        //打开锁的方式
        /** 共享打开  */
        public const UInt32 MW_OPEN_SHARE_MODE = 0;
        /** 独占打开 */
        public const UInt32 MW_OPEN_EXCLUSIVE_MODE = 1;
        //============================================================
        //  算法类型表示
        //============================================================
        /** 算法类型AES */
        public const UInt32 MW_ALG_TYPE_AES = 0x00;
        /** 算法类型DES */
        public const UInt32 MW_ALG_TYPE_DES = 0x01;
        /** 算法类型TDES */
        public const UInt32 MW_ALG_TYPE_TDES = 0x02;
        /** 算法类型ECC */
        public const UInt32 MW_ALG_TYPE_ECC = 0x10;
        /** 算法类型RSA */
        public const UInt32 MW_ALG_TYPE_RSA = 0x11;
        //============================================================
        //  对称算法模式
        //============================================================
        /** 对称算法模式 ECB mode */
        public const UInt32 MW_SYM_ALGO_MODE_ECB = 0x00;
        /** 对称算法模式 CBC mode */
        public const UInt32 MW_SYM_ALGO_MODE_CBC = 0x01;
        //============================================================
        //  文件属性相关宏定义
        //============================================================
        /** 二进制文件 */
        public const UInt32 MW_FILE_TYPE_BINARY = 0;
        /** 密钥文件 */
        public const UInt32 MW_FILE_TYPE_KEY = 2;
        /** 密钥文件权限类型 */
        public const UInt32 MW_FILE_PRIV_TYPE_USE = 0x00;
        /** 数据文件用户只读权限类型 */
        public const UInt32 MW_FILE_PRIV_TYPE_READ = 0x01;
        /** 数据文件用户读写权限类型 */
        public const UInt32 MW_FILE_PRIV_TYPE_READ_WRITE = 0x02;
        //============================================================
        //  获取设备信息宏定义 
        //============================================================
        /** 获取PID     返回4字节PID */
        public const UInt32 MW_GET_INFO_ITEM_PID = 0x00;
        /** 获取序列号  返回16字节获取序列号 */
        public const UInt32 MW_GET_INFO_ITEM_SN = 0x01;
        /** 获取生产日期 返回4字节生产日期 */
        public const UInt32 MW_GET_INFO_ITEM_PRODUCE_DATE = 0x02;
        /** 获取总容量   返回4字节总容量 */
        public const UInt32 MW_GET_INFO_ITEM_ALL_CAPACITY = 0x03;
        /** 获取剩余容量 返回4字节剩余容量 */
        public const UInt32 MW_GET_INFO_ITEM_FREE_CAPACITY = 0x04;
        /** 获取设备版本 返回4字节设备版本 */
        public const UInt32 MW_GET_INFO_ITEM_VERSION = 0x05;
        /** 获取外壳号   返回第一字节为外壳号长度，后面是外壳号数据。注：最大256字节。 */
        public const UInt32 MW_GET_INFO_ITEM_SHELL_SN = 0x06;
        /** 获取设置状态 返回1字节的标识，参见宏定义MW_STATUS_FLAG_PID、MW_STATUS_FLAG_PIN */
        public const UInt32 MW_GET_INFO_ITEM_STATUS = 0x07;
        /** 获取设备远程升级码      返回4 字节设备远程升级码 */
        public const UInt32 MW_GET_INFO_ITEM_UPDATE_CODE = 0x08;
        /** 获取远程升级控制锁标识  返回1字节标识 */
        public const UInt32 MW_GET_INFO_ITEM_UPDATE_CTRL_CODE = 0x09;
        /** 获取所有信息，返回MW_DEVICE_ALL_ITEM_INFO结构体的大小 */
        public const UInt32 MW_GET_INFO_ITEM_ALL = 0xFF;
        /** 默认状态 */
        public const UInt32 MW_STATUS_FLAG_DEFAULT = 0x00;
        /** 已经设置PID状态 */
        public const UInt32 MW_STATUS_FLAG_PID = 0x01;
        /** 已经修改PIN状态 */
        public const UInt32 MW_STATUS_FLAG_PIN = 0x02;
        //============================================================
        //  控制码
        //============================================================
        /** 复位设备，清除登陆状态。 */
        public const UInt32 MW_CTRL_CODE_ITEM_RESET = 0x00;
        /** 控制LED。  注：数据第一字节非0标识LED亮，0表示灭 */
        public const UInt32 MW_CTRL_CODE_ITEM_LED = 0x01;
        //============================================================
        //  PIN类型定义
        //============================================================
        /** 默认PIN类型 */
        public const UInt32 MW_PIN_TYPE_NONE = 0x00;
        /** 用户PIN类型 */
        public const UInt32 MW_PIN_TYPE_USER = 0x01;
        /** 开发商PIN类型 */
        public const UInt32 MW_PIN_TYPE_DEVELOPER = 0x02;
        /** 用户PIN长度 */
        public const UInt32 MW_PIN_LENGTH_USER = 8;
        /** 开发商PIN长度 */
        public const UInt32 MW_PIN_LENGTH_DEVELOPER = 24;
        //============================================================
        //  RSA加密模式定义
        //============================================================
        /** RSA不填充加密模式 */
        public const UInt32 MW_RSA_MODE_NORMAL = 0x00;
        /** RSA PKCS1 V1_5 填充加密模式 */
        public const UInt32 MW_RSA_MODE_PKCS1_V1_5 = 0x01;
        //============================================================
        //  RSA加密数据长度
        //============================================================
        /** RSA-1024加密数据长度 */
        public const UInt32 MW_RSA_1024_BYTE_SIZE = 128;
        /** RSA-2048加密数据长度 */
        public const UInt32 MW_RSA_2048_BYTE_SIZE = 256;


        //============================================================
        //  签名hash算法类型
        //============================================================
        /** 签名MD5 hash算法 */
        public const UInt32 MW_HASH_ALGO_MD5 = 0x00;
        /** 签名SHA1 hash算法 */
        public const UInt32 MW_HASH_ALGO_SHA1 = 0x01;
        /** 签名SHA256 hash算法 */
        public const UInt32 MW_HASH_ALGO_SHA256 = 0x02;
        /** 签名时不做Hash运算，由调用函数者自己做Hash运算 */
        public const UInt32 MW_HASH_ALGO_NONE = 0xFF;
        //============================================================
        //  升级包相关宏定义
        //============================================================
        /** 一个升级包最大包含升级文件个数 */
        public const UInt32 MW_UPDATE_FILE_COUNT_MAX = 50;
        /** 升级文件时的操作标志 删除文件 */
        public const UInt32 MW_UPDATE_MOD_FLAG_DELETE = 0x00000002;
        /** 升级文件时的操作标志 写文件 */
        public const UInt32 MW_UPDATE_MOD_FLAG_WRITE = 0x00000010;
        /** 升级文件时的操作标志 创建文件 */
        public const UInt32 MW_UPDATE_MOD_FLAG_CREATE = 0x00000020;
        //============================================================
        //  多语言宏定义
        //============================================================
        /** 英文 */
        public const UInt32 MW_LANGUAGE_ID_EN = 0;
        /** 中文 */
        public const UInt32 MW_LANGUAGE_ID_CH = 1;
        //============================================================
        //  魔锐相关大小宏定义
        //============================================================
        /** 最大路径 */
        public const UInt32 MW_PATH_LEN = 1024;
        /** 对称加密算法向量长度 */
        public const UInt32 MW_SYM_ALG_IV_LENGTH = 16;
        /** 设备唯一序列号长度 */
        public const UInt32 MW_SN_LENGTH = 16;
        /** 文件名最大长度 */
        public const UInt32 MW_FILE_NAME_LENGTH_MAX = 16;
        /** HMAC MD5计算结果长度 */
        public const UInt32 MW_HMAC_MD5_DIGEST_LENGTH = 16;
        /** HMAC SHA1计算结果长度 */
        public const UInt32 MW_HMAC_SHA1_DIGEST_LENGTH = 20;
        /** HMAC SHA256计算结果长度 */
        public const UInt32 MW_HMAC_SHA256_DIGEST_LENGTH = 32;
        /** 一次最大转输数据长度 */
        public const UInt32 MW_INPUT_DATA_LENGTH_MAX = 1900;
        //种子码最大长度宏定义
        /** 最大种子码长度 */
        public const UInt32 MW_SEED_LENGTH_MAX = 32;
        /** 最小种子码长度 */
        public const UInt32 MW_SEED_LENGTH_MIN = 4;

        /*!
        *   @brief         枚举魔锐设备
        *   @param[in out] pDevInfoList        枚举输出的魔锐设备列表
        *   @param[in]     uiDevListCount      枚举列表(pDevInfoList)能存放的设备的个数
        *   @param[out]    puiDevCount         实际枚举出的魔锐设备数量
        *   @return        执行成功返回 MW_SUCCESS ，执行失败时返回错误码。
        *   @remarks       注意：用户必须按照uiDevListCount乘以sizeof(MW_DEVICE_INFO_CTX)分配内存给pDevInfoList，API内部不分配内存给pDevInfoList。
        */
        [DllImport("moway_api.dll", EntryPoint = "mw_enum", CallingConvention = CallingConvention.StdCall)]
        public static extern UInt32 mw_enum(
            [In, Out, MarshalAs(UnmanagedType.LPArray)]MW_DEVICE_INFO_CTX[] pDevInfoList,
            UInt32 uiDevListCount,
            ref UInt32 puiDevCount);

        /*!
        *   @brief      打开魔锐设备
        *   @param[in]  pDevInfo             指向魔锐设备信息结构体的指针。
        *   @param[in]  uiShareMode          独占或者共享模式打开。参见宏定义MW_OPEN_SHARE_MODE  MW_OPEN_EXCLUSIVE_MODE
        *   @param[out] phHandle             打开魔锐设备后，输出一个句柄以便后续调用。
        *   @return     执行成功返回 MW_SUCCESS ，执行失败时返回错误码。
        *   @remarks    Open an moway device with DevInfo enumerated by mw_enum()
        */
        [DllImport("moway_api.dll", EntryPoint = "mw_open", CallingConvention = CallingConvention.StdCall)]
        public static extern UInt32 mw_open(
            ref MW_DEVICE_INFO_CTX pDevInfo,
            UInt32 uiShareMode,
            ref MWHANDLE phHandle);
                                                                           
        /*!
        *   @brief      关闭设备
        *   @param[in]  hHandle           魔锐设备句柄
        *   @return     执行成功返回 MW_SUCCESS ，执行失败时返回错误码。
        *   @remarks    
        */
        [DllImport("moway_api.dll", EntryPoint = "mw_close", CallingConvention = CallingConvention.StdCall)]
        public static extern UInt32 mw_close(MWHANDLE hHandle);

        /*!
        *   @brief      校验PIN
        *   @param[in]  hHandle             魔锐设备句柄
        *   @param[in]  ucPinType           Pin码类型  参见宏定义
        *   @param[in]  pucPin              Pin码数据，用户8字节，开发商24字节
        *   @return     执行成功返回 MW_SUCCESS ，执行失败时返回错误码。
        *   @remarks    
        */
        [DllImport("moway_api.dll", EntryPoint = "mw_verify_pin", CallingConvention = CallingConvention.StdCall)]
        public static extern UInt32 mw_verify_pin(
            MWHANDLE hHandle,
            byte ucPinType,
            [In, MarshalAs(UnmanagedType.LPArray)] byte[] pucPin);
        /*!
        *   @brief      修改PIN
        *   @param[in]  hHandle             魔锐设备句柄
        *   @param[in]  ucPinType           Pin码类型  参见宏定义：MW_PIN_TYPE_XXX
        *   @param[in]  usLimitCount        最大尝试次数，注：不限次数设置为0，限制次数范围是1-15， 如为其它数则返回参数错误
        *   @param[in]  pucOldPin           旧Pin码数据，用户8字节，开发商24字节
        *   @param[in]  pucNewPin           新Pin码数据，用户8字节，开发商24字节
        *   @return     执行成功返回 MW_SUCCESS ，执行失败时返回错误码。
        *   @remarks    
        */
        [DllImport("moway_api.dll", EntryPoint = "mw_change_pin", CallingConvention = CallingConvention.StdCall)]
        public static extern UInt32 mw_change_pin(
            MWHANDLE hHandle,
            byte ucPinType,
            UInt16 usLimitCount,
            [In, MarshalAs(UnmanagedType.LPArray)] byte[] pucOldPin,
            [In, MarshalAs(UnmanagedType.LPArray)] byte[] pucNewPin);

        /*!
        *   @brief      设置PID
        *   @param[in]  hHandle             魔锐设备句柄
        *   @param[in]  pucPIDSeed          种子码
        *   @param[in]  uiSeedLen           种子码长度，长度范围是4-32，参见宏定义MW_SEED_LENGTH_MIN、MW_SEED_LENGTH_MAX
        *   @return     执行成功返回 MW_SUCCESS ，执行失败时返回错误码。
        *   @remarks    
        */
        [DllImport("moway_api.dll", EntryPoint = "mw_set_pid", CallingConvention = CallingConvention.StdCall)]
        public static extern UInt32 mw_set_pid(
            MWHANDLE hHandle,
            [In, MarshalAs(UnmanagedType.LPArray)] byte[] pucPIDSeed,
            UInt32 uiSeedLen);
                              
        /*!
        *   @brief      控制命令
        *   @param[in]  hHandle             魔锐设备句柄
        *   @param[in]  uiCtrlCodeItem      控制码，参见宏定义 MW_CTRL_CODE_ITEM_XXX
        *   @param[in]  pvInBuffer          输入数据
        *   @param[in]  uiInBufferLen       输入数据的长度
        *   @param[out] pvOutBuffer         输出数据
        *   @param[in]  uiOutBufferLen      输出数据的长度
        *   @param[out] puiReturnedLen      实际输出数据的长度   
        *   @return     执行成功返回 MW_SUCCESS ，执行失败时返回错误码。
        *   @remarks    
        */
        [DllImport("moway_api.dll", EntryPoint = "mw_control", CallingConvention = CallingConvention.StdCall)]
        public static extern UInt32 mw_control(
            MWHANDLE hHandle,
            byte uiCtrlCodeItem,
            [In, MarshalAs(UnmanagedType.LPArray)] byte[] pvInBuffer,
            UInt32 uiInBufferLen,
            [Out, MarshalAs(UnmanagedType.LPArray)] byte[] pvOutBuffer,
            UInt32 uiOutBufferLen,
            ref UInt32 puiReturnedLen);   
         
        /*!
        *   @brief      获取设备信息
        *   @param[in]  hHandle           魔锐设备句柄
        *   @param[in]  ucInfoItem        指定获取的设备信息的具体项，参见宏定义：MW_GET_INFO_ITEM_XXX 
        *   @param[out] pvBuffer          输出获取的设备信息项
        *   @param[in out] puiBufferLength   输出设备信息项的数据长度
        *   @return     执行成功返回 MW_SUCCESS ，执行失败时返回错误码。
        *   @remarks    
        */
        [DllImport("moway_api.dll", EntryPoint = "mw_get_device_info", CallingConvention = CallingConvention.StdCall)]
        public static extern UInt32 mw_get_device_info(
            MWHANDLE hHandle,
            byte ucInfoItem,
            [Out, MarshalAs(UnmanagedType.LPArray)] byte[] pvBuffer,
            ref UInt32 puiBufferLength);

        [DllImport("moway_api.dll", EntryPoint = "mw_get_device_info", CallingConvention = CallingConvention.StdCall)]
        public static extern UInt32 mw_get_device_all_info(
            MWHANDLE hHandle,
            byte ucInfoItem,
            ref MW_DEVICE_ALL_ITEM_INFO pstDeviceInfo,
            ref UInt32 puiBufferLength);

         
        /*!
        *   @brief      获取设备状态
        *   @param[in]  hHandle                   魔锐设备句柄
        *   @param[out] pstDeviceStatusCtx        当前设备状态。
        *   @return     执行成功返回 MW_SUCCESS ，执行失败时返回错误码。
        *   @remarks    
        */                                        
        [DllImport("moway_api.dll", EntryPoint = "mw_get_device_status", CallingConvention = CallingConvention.StdCall)]                     
        public static extern UInt32 mw_get_device_status(
            MWHANDLE hHandle,
            ref MW_DEVICE_STATUS pstDeviceStatus);
                                        
        /*!
        *   @brief      列举文件
        *   @param[in]  hHandle              魔锐设备句柄
        *   @param[out] pstFilePropertyList  结构体MW_FILE_PROPERTY数组，它的大小由uiFileListCount指定
        *   @param[in]  uiFileListCount      指定pstFilePropertyList数组的大小
        *   @param[out] puiReturnedFileCount 实际返回列举到文件的个数
        *   @return     执行成功返回 MW_SUCCESS ，执行失败时返回错误码。
        *   @remarks    注意：pstFilePropertyList由函数外部调用者分配，大小为uiFileListCount * sizeof(MW_FILE_PROPERTY)
        */
        [DllImport("moway_api.dll", EntryPoint = "mw_enum_file", CallingConvention = CallingConvention.StdCall)]
        public static extern UInt32 mw_enum_file(
            MWHANDLE hHandle,
            [Out, MarshalAs(UnmanagedType.LPStruct)] MW_FILE_PROPERTY[] pstFilePropertyList,
            UInt32 uiFileListCount,
            ref UInt32 puiReturnedFileCount);

        /*!
        *   @brief      创建文件
        *   @param[in]  hHandle           魔锐设备句柄
        *   @param[in]  pstFileProperty   指定需要创建文件的属性，其中文件类型、文件权限、文件大小为必填项
        *   @return     执行成功返回 MW_SUCCESS ，执行失败时返回错误码。
        *   @remarks    
        */
        [DllImport("moway_api.dll", EntryPoint = "mw_create_file", CallingConvention = CallingConvention.StdCall)]
        public static extern UInt32 mw_create_file(
            MWHANDLE hHandle,
            ref MW_FILE_PROPERTY pstFileProperty);

        /*!
        *   @brief      读文件
        *   @param[in]  hHandle           魔锐设备句柄
        *   @param[in]  pcFileName        文件名称，以'\0'结尾的字符串      
        *   @param[in]  uiReadOffset      读文件的偏移量   
        *   @param[in]  uiReadSize        预计读取文件的大小。
        *   @param[out] pucReadBuffer     读取文件数据到缓冲区。注：必须分配足够uiReadSize大小，否则会访问越界。
        *   @return     执行成功返回 MW_SUCCESS ，执行失败时返回错误码。
        *   @remarks    
        */
        [DllImport("moway_api.dll", EntryPoint = "mw_read_file", CallingConvention = CallingConvention.StdCall)]
        public static extern UInt32 mw_read_file(
            MWHANDLE hHandle,
            [In, MarshalAs(UnmanagedType.LPStr)] string pcFileName,
            UInt32 uiReadOffset,
            UInt32 uiReadSize,
            [Out, MarshalAs(UnmanagedType.LPArray)] byte[] pucReadBuffer);
                                  
        /*!
        *   @brief      写文件
        *   @param[in]  hHandle           魔锐设备句柄
        *   @param[in]  pcFileName        文件名称，以'\0'结尾的字符串      
        *   @param[in]  uiWriteOffset     写文件的偏移量   
        *   @param[in]  uiWriteSize       写文件的大小。 注：最大一次写入字节参见宏定义：MW_INPUT_DATA_LENGTH_MAX
        *   @param[in]  pucWriteBuffer    需要写入文件的缓冲区 
        *   @return     执行成功返回 MW_SUCCESS ，执行失败时返回错误码。
        *   @remarks    
        */
        [DllImport("moway_api.dll", EntryPoint = "mw_write_file", CallingConvention = CallingConvention.StdCall)]
        public static extern UInt32 mw_write_file(
            MWHANDLE hHandle,
            [In, MarshalAs(UnmanagedType.LPStr)] string pcFileName,
            UInt32 uiWriteOffset,
            UInt32 uiWriteSize,
            [In, MarshalAs(UnmanagedType.LPArray)] byte[] pucWriteBuffer);
                                   
        /*!
        *   @brief      删除文件
        *   @param[in]  hHandle           魔锐设备句柄
        *   @param[in]  pcFileName        文件名称，以'\0'结尾的字符串      
        *   @return     执行成功返回 MW_SUCCESS ，执行失败时返回错误码。
        *   @remarks    
        */
        [DllImport("moway_api.dll", EntryPoint = "mw_delete_file", CallingConvention = CallingConvention.StdCall)]
        public static extern UInt32 mw_delete_file(
            MWHANDLE hHandle,
            [In, MarshalAs(UnmanagedType.LPStr)] string pcFileName);
                                  
        /*!
        *   @brief      获取文件属性
        *   @param[in]  hHandle           魔锐设备句柄
        *   @param[in]  lpszFileName      文件名称，以'\0'结尾的字符串 
        *   @param[out] pstFileProperty   文件属性信息    
        *   @return     执行成功返回 MW_SUCCESS ，执行失败时返回错误码。
        *   @remarks    
        */
        [DllImport("moway_api.dll", EntryPoint = "mw_get_file_property", CallingConvention = CallingConvention.StdCall)]
        public static extern UInt32 mw_get_file_property(
            MWHANDLE hHandle,
            [In, MarshalAs(UnmanagedType.LPStr)] string pcFileName,
            ref MW_FILE_PROPERTY pstFileProperty);
                                        
        /*!
        *   @brief          对称加密
        *   @param[in]      hHandle           魔锐设备句柄
        *   @param[in]      pcKeyFileName     密钥文件名称 
        *   @param[in]      ucAlgoMode        对称加密的算法，参见宏定义：MW_ALG_TYPE_XXX
        *   @param[in]      pucIV             对称加密算法中需要使用到的向量，固定16字节，CBC模式需要填入，ECB可不填
        *   @param[in]      pucInData         加密输入的明文，必须是16的倍数，
        *   @param[in]      uiInDataLen       加密明文的数据长度,
        *   @param[out]     pucOutData        加密后输出的密文
        *   @param[in out]  puiOutDataLen     加密后密文的长度
        *   @return     执行成功返回 MW_SUCCESS ，执行失败时返回错误码。
        *   @remarks    
        */
        [DllImport("moway_api.dll", EntryPoint = "mw_sym_encrypt", CallingConvention = CallingConvention.StdCall)]
        public static extern UInt32 mw_sym_encrypt(
            MWHANDLE hHandle,
            [In, MarshalAs(UnmanagedType.LPStr)] string pcKeyFileName,
            byte ucAlgoMode,
            [In, MarshalAs(UnmanagedType.LPArray)] byte[] pucIV,
            [In, MarshalAs(UnmanagedType.LPArray)] byte[] pucInData,
            UInt32 uiInDataLen,
            [Out, MarshalAs(UnmanagedType.LPArray)] byte[] pucOutData,
            ref UInt32 puiOutDataLen);   
                        
        /*!
        *   @brief          对称解密
        *   @param[in]      hHandle           魔锐设备句柄
        *   @param[in]      pcKeyFileName     密钥文件名称 
        *   @param[in]      ucAlgoMode        对称加密的算法，参见宏定义：MW_ALG_TYPE_XXX
        *   @param[in]      pucIV             对称加密算法中需要使用到的向量，固定16字节，CBC模式需要填入，ECB可不填
        *   @param[in]      pucInData         输入的密文，必须是16的倍数,
        *   @param[in]      uiInDataLen       输入密文长度,
        *   @param[out]     pucOutData        解密后输出明文 
        *   @param[in out]  puiOutDataLen     解密后输出明文长度
        *   @return     执行成功返回 MW_SUCCESS ，执行失败时返回错误码。
        *   @remarks    
        */
        [DllImport("moway_api.dll", EntryPoint = "mw_sym_decrypt", CallingConvention = CallingConvention.StdCall)]
        public static extern UInt32 mw_sym_decrypt(
            MWHANDLE hHandle,
            [In, MarshalAs(UnmanagedType.LPStr)] string pcKeyFileName,
            byte ucAlgoMode,
            [In, MarshalAs(UnmanagedType.LPArray)] byte[] pucIV,
            [In, MarshalAs(UnmanagedType.LPArray)] byte[] pucInData,
            UInt32 uiInDataLen,
            [Out, MarshalAs(UnmanagedType.LPArray)] byte[] pucOutData,
            ref UInt32 puiOutDataLen);            
                                    
        /*!
        *   @brief          rsa非对称加密
        *   @param[in]      hHandle           魔锐设备句柄
        *   @param[in]      pcKeyFileName     密钥文件名称 
        *   @param[in]      ucPadMode         加密的算法，参见宏定义：MW_RSA_MODE_XXX
        *   @param[in]      pucInData         输入明文数据
        *   @param[in]      uiInDataLen       输入明文数据的长度 
        *   @param[out]     pucOutData        输出密文数据
        *   @param[in out]  puiOutDataLen     输出密文数据的长度
        *   @return     执行成功返回 MW_SUCCESS ，执行失败时返回错误码。
        *   @remarks    注：当ucPadMode为MW_RSA_MODE_NORMAL时，RSA_1024位加密最大加密明文长度为128,RSA_2048位加密最大长度为256; 
        *                   当ucPadMode为MW_RSA_MODE_PKCS1_V1_5时，  RSA_1024位加密最大加密明文长度为117,RSA_2048位加密最大长度为245; 
        */
        [DllImport("moway_api.dll", EntryPoint = "mw_rsa_encrypt", CallingConvention = CallingConvention.StdCall)]
        public static extern UInt32 mw_rsa_encrypt(
            MWHANDLE hHandle,
            [In, MarshalAs(UnmanagedType.LPStr)] string pcKeyFileName,
            byte ucPadMode,
            [In, MarshalAs(UnmanagedType.LPArray)] byte[] pucInData,
            UInt32 uiInDataLen,
            [Out, MarshalAs(UnmanagedType.LPArray)] byte[] pucOutData,
            ref UInt32 puiOutDataLen);
                        
        /*!
        *   @brief          rsa非对称解密
        *   @param[in]      hHandle           魔锐设备句柄
        *   @param[in]      pcKeyFileName     密钥文件名称 
        *   @param[in]      ucPadMode         加密的算法，参见宏定义：MW_RSA_MODE_XXX
        *   @param[in]      pucInData         输入密文   
        *   @param[in]      uiInDataLen       输入密文长度  注：它的长度必须为128或者256 
        *   @param[out]     pucOutData        输出明文 
        *   @param[in out]  puiOutDataLen     输出明文长度
        *   @return     执行成功返回 MW_SUCCESS ，执行失败时返回错误码。
        *   @remarks    
        */
        [DllImport("moway_api.dll", EntryPoint = "mw_rsa_decrypt", CallingConvention = CallingConvention.StdCall)]
        public static extern UInt32 mw_rsa_decrypt(
            MWHANDLE hHandle,
            [In, MarshalAs(UnmanagedType.LPStr)] string pcKeyFileName,
            byte ucPadMode,
            [In, MarshalAs(UnmanagedType.LPArray)] byte[] pucInData,
            UInt32 uiInDataLen,
            [Out, MarshalAs(UnmanagedType.LPArray)] byte[] pucOutData,
            ref UInt32 puiOutDataLen);
                                   
        /*!
        *   @brief          签名
        *   @param[in]      hHandle           魔锐设备句柄
        *   @param[in]      pcKeyFileName     密钥文件名称 
        *   @param[in]      ucHashAlg         哈希算法类型，参见宏定义：MW_HASH_ALGO_SHA1, MW_HASH_ALGO_SHA256, MW_HASH_ALGO_NONE
        *   @param[in]      pucMessageData    输入消息数据
        *   @param[in]      uiMessageDataLen  输入消息数据长度 注：最大数据长度参见宏定义 MW_INPUT_DATA_LENGTH_MAX 
        *   @param[out]     pucSignData       输出签名数据 
        *   @param[in out]  puiSignDataLen    输出签名数据长度
        *   @return     执行成功返回 MW_SUCCESS ，执行失败时返回错误码。
        *   @remarks    
        */
        [DllImport("moway_api.dll", EntryPoint = "mw_signature", CallingConvention = CallingConvention.StdCall)]
        public static extern UInt32 mw_signature(
            MWHANDLE hHandle,
            [In, MarshalAs(UnmanagedType.LPStr)] string pcKeyFileName,
            byte ucHashAlg,
            [In, MarshalAs(UnmanagedType.LPArray)] byte[] pucMessageData,
            UInt32 uiMessageDataLen,
            [Out, MarshalAs(UnmanagedType.LPArray)] byte[] pucSignData,
            ref UInt32 puiSignDataLen);

        /*!
        *   @brief      验签
        *   @param[in]      hHandle           魔锐设备句柄
        *   @param[in]      pcKeyFileName     密钥文件名称 
        *   @param[in]      ucHashAlg         哈希算法类型，参见宏定义：MW_HASH_ALGO_XXX
        *   @param[in]      pucSignData       输入签名数据
        *   @param[in]      uiSignDataLen     输入签名数据长度
        *   @param[in]      pucMessageData    输入消息数据
        *   @param[in]      uiMessageDataLen  输入消息数据长度
        *   @return     执行成功返回 MW_SUCCESS ，执行失败时返回错误码。
        *   @remarks    
        */
        [DllImport("moway_api.dll", EntryPoint = "mw_verify_sign", CallingConvention = CallingConvention.StdCall)]
        public static extern UInt32 mw_verify_sign(
            MWHANDLE hHandle,
            [In, MarshalAs(UnmanagedType.LPStr)] string pcKeyFileName,
            byte ucHashAlg,
            [In, MarshalAs(UnmanagedType.LPArray)] byte[] pucSignData,
            UInt32 uiSignDataLen,
            [In, MarshalAs(UnmanagedType.LPArray)] byte[] pucMessageData,
            UInt32 uiMessageDataLen);

        /*!
        *   @brief          HMAC计算
        *   @param[in]      hHandle           魔锐设备句柄
        *   @param[in]      pcKeyFileName     密钥文件名称 
        *   @param[in]      pucInData         输入数据
        *   @param[in]      uiInDataLen       输入数据长度
        *   @param[out]     pucOutData        输出数据 
        *   @param[in out]  puiOutDataLen     输出数据长度，参见宏定义：MW_HMAC_MD5_DIGEST_LENGTH、MW_HMAC_SHA1_DIGEST_LENGTH、MW_HMAC_SHA256_DIGEST_LENGTH
        *   @return     执行成功返回 MW_SUCCESS ，执行失败时返回错误码。
        *   @remarks    
        */
        [DllImport("moway_api.dll", EntryPoint = "mw_hmac_calc", CallingConvention = CallingConvention.StdCall)]
        public static extern UInt32 mw_hmac_calc(
            MWHANDLE hHandle,
            [In, MarshalAs(UnmanagedType.LPStr)] string pcKeyFileName,
            [In, MarshalAs(UnmanagedType.LPArray)] byte[] pucInData,
            UInt32 uiInDataLen,
            [Out, MarshalAs(UnmanagedType.LPArray)] byte[] pucOutData,
            ref UInt32 puiOutDataLen);                  


        /*!
        *   @brief          制作升级包
        *   @param[in]      hHandle                魔锐设备句柄
        *   @param[in]      uiDevPID               设备产品号
        *   @param[in]      pucSerialNum           硬件唯一序列号 固定长度为：MW_SN_LENGTH，不指定SN绑定时，可以为NULL
        *   @param[in]      pstUpdateFileList        文件路径列表,最大文件个数参见宏定义MW_UPDATE_FILE_COUNT_MAX,升级文件的个数为uiFileCount
        *   @param[in]      uiFileCount            升级文件的个数,它指定了pstUpdateFileList中的大小 
        *   @param[out]     pucOutPkg              升级包数据
        *   @param[in out]  puiOutPkgLen           升级包数据长度
        *   @return     执行成功返回 MW_SUCCESS ，执行失败时返回错误码。
        *   @remarks    
        */
        [DllImport("moway_api.dll", EntryPoint = "mw_make_update_pkg", CallingConvention = CallingConvention.StdCall)]
        public static extern UInt32 mw_make_update_pkg(
            MWHANDLE hHandle,
            UInt32 uiDevPID,
            [In, MarshalAs(UnmanagedType.LPArray)] byte[] pucSN,
            [In, Out, MarshalAs(UnmanagedType.LPArray)] MW_UPDADE_FILE_CTX[] pstUpdateFileCtx,
            UInt32 uiFileCount,
            [Out, MarshalAs(UnmanagedType.LPArray)] byte[] pucOutPkg,
            ref UInt32 puiOutPkgLen);
                        
        /*!
        *   @brief      远程升级
        *   @param[in]  hHandle             魔锐设备句柄
        *   @param[in]  pucInPkg            升级包数据
        *   @param[in]  uiInPkgLen          升级包数据长度
        *   @return     执行成功返回 MW_SUCCESS ，执行失败时返回错误码。
        *   @remarks    
        */
        [DllImport("moway_api.dll", EntryPoint = "mw_update", CallingConvention = CallingConvention.StdCall)]
        public static extern UInt32 mw_update(
            MWHANDLE hHandle,
            [In, MarshalAs(UnmanagedType.LPArray)] byte[] pucInPkg,
            UInt32 uiInPkgLen);    
                                     

        /*!
        *   @brief      恢复出厂设置
        *   @param[in]  hHandle             魔锐设备句柄
        *   @return     执行成功返回 MW_SUCCESS ，执行失败时返回错误码。
        *   @remarks    将设备恢复出厂状态，即默认PID、默认PIN，文件系统清空。需开发商权限
        */
        [DllImport("moway_api.dll", EntryPoint = "mw_restore_factory", CallingConvention = CallingConvention.StdCall)]
        public static extern UInt32 mw_restore_factory(
            MWHANDLE hHandle);   

        /*!
        *   @brief      错误码帮助
        *   @param[in]  hHandle             魔锐设备句柄
        *   @param[in]  uiErrorCode         错误码
        *   @param[in]  uiLanguageID        语言种类ID
        *   @return     执行成功返回 MW_SUCCESS ，执行失败时返回错误码。
        *   @remarks    
        */
        [DllImport("moway_api.dll", EntryPoint = "mw_get_error_desc", CallingConvention = CallingConvention.StdCall)]
        public static extern string mw_get_error_desc(
            UInt32 uiErrorCode, 
            UInt32 uiLanguageID);

    }

    //============================================================
    //  结构体定义
    //============================================================
    /** 设备信息结构体 */
    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct MW_DEVICE_INFO_CTX
    {
        /** 产品ID */
        public UInt32 uiPID;
        /** 唯一序列号 */
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = (int)MowayCS.MW_SN_LENGTH)]
        public byte[] ucSerialNum;
        /** 设备的路径 */
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = (int)MowayCS.MW_PATH_LEN)]
        public byte[] ucDevPath;
        /** 通讯协议 */
        public UInt32 uiProtocol;
        /** Mac OS 系统 LocationID */
        public UInt32 uiLocationID;
    }

    /** 文件属性结构体 */
    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct MW_FILE_PROPERTY
    {
        public byte ucValidate;
        /** 文件类型：二进制文件或者密钥文件。 */
        public byte ucType;
        /** 文件权限 */
        public short usPrivilege;
        /** 文件大小 */
        public UInt32 uiSize;
        /** 创建时间 */
        public UInt32 uiTime;
        /** 文件名称 */
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = (int)MowayCS.MW_FILE_NAME_LENGTH_MAX + 1)]
        public byte[] acName;

        public MW_FILE_PROPERTY(string filename)
        {
            ucValidate = 0;
            usPrivilege = 0;
            ucType = 0;
            uiSize = 0;
            uiTime = 0;

            acName = new byte[(int)MowayCS.MW_FILE_NAME_LENGTH_MAX + 1];
            byte[] bFilename = Encoding.ASCII.GetBytes(filename);
            Array.Copy(bFilename, acName, bFilename.Length);
        }
    }

    /** 系统工作状态结构体 */
    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct MW_DEVICE_STATUS
    {
        /** 系统时间 */
        public UInt32 uiSysTime;
        /** 系统状态 */
        public UInt32 uiSysStatus;
        /** 保留 */
        public short usReserved;
        /** 登录状态 */
        public byte ucLoginStatus;
        /** 灯的状态 */
        public byte ucLedStatus;
    }

    /** 远程升级结构体 */
    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct MW_UPDADE_FILE_CTX
    {
        /** 文件属性结构体 */
        public MW_FILE_PROPERTY stFileProperty;
        /** 升级文件的数据内容（拷贝数据时需要进行非托管内存拷贝处理） */
        public IntPtr pData; 
	    /** 升级文件的数据大小 */
        public int uiDateSize;
    }

    /** 设备信息子项结构体 */
    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct MW_DEVICE_ALL_ITEM_INFO
    {
        /** 产品号 */
        public UInt32 PID;
        /** 唯一序列号 */
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = (int)MowayCS.MW_SN_LENGTH)]
        public byte[] SN;
        /** 生产时间 */
        public UInt32 produce_date;
        /** 总容量 */
        public UInt32 all_capacity;
        /** 可用容量 */
        public UInt32 free_capacity;
        /** 版本号 */
        public UInt32 version;
        /** 升级码 */
        public UInt32 update_code;
        /** 状态 */
        public byte status;
        /** 升级控制码 */
        public byte update_ctrl_code;
        /** 保留 */
        public UInt16 reserved;
    };
}
