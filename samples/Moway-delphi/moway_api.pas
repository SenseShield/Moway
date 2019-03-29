unit moway_api;

{$IFDEF CompilerVersion > 14.5}
  // Delphi 6默认未处理FPC，故使用版本后区分，简化编译条件。 shijc 2016-8-8
  // 备注：原文无版本判断
  {$IFDEF FPC}
    {$WARNINGS OFF}
    {$HINTS OFF}
  {$ELSE FPC}
    {$WARN UNSAFE_CODE OFF}
    {$WARN UNSAFE_TYPE OFF}
    {$WARN UNSAFE_CAST OFF}
  {$ENDIF FPC}
{$ENDIF}


interface

uses
{$IF CompilerVersion >= 23}
  // XE2 及以上版本引用头文件
  Winapi.Windows, System.SysUtils, System.Classes,
  BTMemory_x86_64;
{$ELSE}
  // XE以下版本（包括Delphi7）
  windows, SysUtils, Classes,
  BTMemory_x86;
{$IFEND}

{$I moway_api.inc}

var
{/*!
*   @brief         枚举魔锐设备
*   @param[in out] pDevInfoList        枚举输出的魔锐设备列表
*   @param[in]     uiDevListCount      枚举列表(pDevInfoList)能存放的设备的个数。注：最大个数参见宏定义MW_ENUM_DEVICE_MAX_COUNT
*   @param[out]    puiDevCount         实际枚举出的魔锐设备数量
*   @return        执行成功返回 MW_SUCCESS ，执行失败时返回错误码。
*   @remarks       注意：用户必须按照uiDevListCount乘以sizeof(MW_DEVICE_INFO_CTX)分配内存给pDevInfoList，API内部不分配内存给pDevInfoList。
*/}
mw_enum: function (
{OUT} pDevInfoList: PMW_DEVICE_INFO_CTX;
{IN} uiDevListCount: LONGWORD;
{OUT} var puiDevCount: LONGWORD
): LONGWORD; stdcall;

{/*!
*   @brief      打开魔锐设备
*   @param[in]  pDevInfo             指向魔锐设备信息结构体的指针。
*   @param[in]  uiShareMode          独占或者共享模式打开。参见宏定义MW_OPEN_SHARE_MODE  MW_OPEN_EXCLUSIVE_MODE
*   @param[out] phHandle             打开魔锐设备后，输出一个句柄以便后续调用。
*   @return     执行成功返回 MW_SUCCESS ，执行失败时返回错误码。
*   @remarks    Open an moway device with DevInfo enumerated by mw_enum()
*/}
mw_open: function (
{IN} pDevInfo: PMW_DEVICE_INFO_CTX;
{IN} uiShareMode: LONGWORD;
{OUT} var phHandle: MWHANDLE
): LONGWORD; stdcall;
                                                                           
{/*!
*   @brief      关闭设备
*   @param[in]  hHandle           魔锐设备句柄
*   @return     执行成功返回 MW_SUCCESS ，执行失败时返回错误码。
*   @remarks    
*/}
mw_close: function (
{IN} hHandle: MWHANDLE
): LONGWORD; stdcall;

{/*!
*   @brief      校验PIN
*   @param[in]  hHandle             魔锐设备句柄
*   @param[in]  ucPinType           Pin码类型  参见宏定义：MW_PIN_TYPE_XXX
*   @param[in]  pucPin              Pin码数据，用户8字节，开发商24字节
*   @return     执行成功返回 MW_SUCCESS ，执行失败时返回错误码。
*   @remarks    
*/}
mw_verify_pin: function (
{IN} hHandle: MWHANDLE;
{IN} ucPinType: BYTE;
{IN} pucPin: PBYTE
): LONGWORD; stdcall;

{/*!
*   @brief      修改PIN
*   @param[in]  hHandle             魔锐设备句柄
*   @param[in]  ucPinType           Pin码类型  参见宏定义：MW_PIN_TYPE_XXX
*   @param[in]  usLimitCount        最大尝试次数，注：不限次数设置为0，限制次数范围是1-15， 如为其它数则返回参数错误
*   @param[in]  pucOldPin           旧Pin码数据，用户8字节，开发商24字节
*   @param[in]  pucNewPin           新Pin码数据，用户8字节，开发商24字节
*   @return     执行成功返回 MW_SUCCESS ，执行失败时返回错误码。
*   @remarks    
*/}
mw_change_Pin: function (
{IN} hHandle: MWHANDLE;
{IN} ucPinType: BYTE;
{IN} usLimitCount: WORD;
{IN} pucOldPin: PBYTE;
{IN} pucNewPin: PBYTE
): LONGWORD; stdcall;

{/*!
*   @brief      设置PID
*   @param[in]  hHandle             魔锐设备句柄
*   @param[in]  pucPIDSeed          种子码
*   @param[in]  uiSeedLen           种子码长度，长度范围是4-32，参见宏定义MW_SEED_LENGTH_MIN、MW_SEED_LENGTH_MAX
*   @return     执行成功返回 MW_SUCCESS ，执行失败时返回错误码。
*   @remarks    
*/}
mw_set_pid: function (
{IN} hHandle: MWHANDLE;
{IN} pucPIDSeed: PBYTE;
{IN} uiSeedLen: LONGWORD
): LONGWORD; stdcall;
                              
{/*!
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
*/}
mw_control: function (
{IN} hHandle: MWHANDLE;
{IN} uiCtrlCodeItem: BYTE;
{IN} pvInBuffer: Pointer;
{IN} uiInBufferLen: LONGWORD;
{OUT} pvOutBuffer: Pointer;
{IN} uiOutBufferLen: LONGWORD;
{OUT} var puiReturnedLen: LONGWORD
): LONGWORD; stdcall;

{/*!
*   @brief      获取设备信息
*   @param[in]  hHandle           魔锐设备句柄
*   @param[in]  ucInfoItem        指定获取的设备信息的具体项，参见宏定义：MW_GET_INFO_ITEM_XXX 
*   @param[out] pvBuffer          输出获取的设备信息项
*   @param[in out] puiBufferLength   输出设备信息项的数据长度
*   @return     执行成功返回 MW_SUCCESS ，执行失败时返回错误码。
*   @remarks    
*/}
mw_get_device_info: function (
{IN} hHandle: MWHANDLE;
{IN} ucInfoItem: BYTE;
{OUT} pvBuffer: Pointer;
{INOUT} var puiBufferLength: LONGWORD
): LONGWORD; stdcall;
         
{/*!
*   @brief      获取设备状态
*   @param[in]  hHandle                   魔锐设备句柄
*   @param[out] pstDeviceStatusCtx        当前设备状态。
*   @return     执行成功返回 MW_SUCCESS ，执行失败时返回错误码。
*   @remarks    
*/}                                        
mw_get_device_status: function (
{IN} hHandle: MWHANDLE;
{OUT} pstDeviceStatusCtx: PMW_DEVICE_STATUS
): LONGWORD; stdcall;
                                        
{/*!
*   @brief      列举文件
*   @param[in]  hHandle              魔锐设备句柄
*   @param[out] pstFilePropertyList  结构体MW_FILE_PROPERTY数组，它的大小由uiFileListCount指定
*   @param[in]  uiFileListCount      指定pstFilePropertyList数组的大小
*   @param[out] puiReturnedFileCount 实际返回列举到文件的个数
*   @return     执行成功返回 MW_SUCCESS ，执行失败时返回错误码。
*   @remarks    注意：pstFilePropertyList由函数外部调用者分配，大小为uiFileListCount * sizeof(MW_FILE_PROPERTY)
*/}
mw_enum_file: function (
{IN} hHandle: MWHANDLE;
{OUT} pstFilePropertyList: PMW_FILE_PROPERTY;
{IN} uiFileListCount: LONGWORD;
{OUT} var puiReturnedFileCount: LONGWORD
): LONGWORD; stdcall;

{/*!
*   @brief      创建文件
*   @param[in]  hHandle           魔锐设备句柄
*   @param[in]  pstFileProperty   指定需要创建文件的属性，其中文件类型、文件权限、文件大小为必填项
*   @return     执行成功返回 MW_SUCCESS ，执行失败时返回错误码。
*   @remarks    
*/}
mw_create_file: function (
{IN} hHandle: MWHANDLE;
{IN} pstFileProperty: PMW_FILE_PROPERTY
): LONGWORD; stdcall;

{/*!
*   @brief      读文件
*   @param[in]  hHandle           魔锐设备句柄
*   @param[in]  pcFileName        文件名称，以'\0'结尾的字符串      
*   @param[in]  uiReadOffset      读文件的偏移量   
*   @param[in]  uiReadSize        预计读取文件的大小。
*   @param[out] pucReadBuffer     读取文件数据到缓冲区。注：必须分配足够uiReadSize大小，否则会访问越界。
*   @return     执行成功返回 MW_SUCCESS ，执行失败时返回错误码。
*   @remarks    
*/}
mw_read_file: function (
{IN} hHandle: MWHANDLE;
{IN} pcFileName: PANSIChar;
{IN} uiReadOffset: LONGWORD;
{IN} uiReadSize: LONGWORD;
{OUT} pucReadBuffer: PBYTE
): LONGWORD; stdcall;
                                  
{/*!
*   @brief      写文件
*   @param[in]  hHandle           魔锐设备句柄
*   @param[in]  pcFileName        文件名称，以'\0'结尾的字符串      
*   @param[in]  uiWriteOffset     写文件的偏移量   
*   @param[in]  uiWriteSize       写文件的大小。 
*   @param[in]  pucWriteBuffer    需要写入文件的缓冲区 
*   @return     执行成功返回 MW_SUCCESS ，执行失败时返回错误码。
*   @remarks    
*/}
mw_write_file: function (
{IN} hHandle: MWHANDLE;
{IN} pcFileName: PANSIChar;
{IN} uiWriteOffset: LONGWORD;
{IN} uiWriteSize: LONGWORD;
{IN} pucWriteBuffer: PBYTE
): LONGWORD; stdcall;
                                   
{/*!
*   @brief      删除文件
*   @param[in]  hHandle           魔锐设备句柄
*   @param[in]  pcFileName        文件名称，以'\0'结尾的字符串      
*   @return     执行成功返回 MW_SUCCESS ，执行失败时返回错误码。
*   @remarks    
*/}
mw_delete_file: function (
{IN} hHandle: MWHANDLE;
{IN} pcFileName: PANSIChar
): LONGWORD; stdcall;
                                  
{/*!
*   @brief      获取文件属性
*   @param[in]  hHandle           魔锐设备句柄
*   @param[in]  lpszFileName      文件名称，以'\0'结尾的字符串 
*   @param[out] pstFileProperty   文件属性信息    
*   @return     执行成功返回 MW_SUCCESS ，执行失败时返回错误码。
*   @remarks    
*/}
mw_get_file_property: function (
{IN} hHandle: MWHANDLE;
{IN} pcFileName: PANSIChar;
{OUT} pstFileProperty: PMW_FILE_PROPERTY
): LONGWORD; stdcall;
                                        
{/*!
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
*/}
mw_sym_encrypt: function (
{IN} hHandle: MWHANDLE;
{IN} pcKeyFileName: PANSIChar;
{IN} ucAlgoMode: BYTE;
{IN} pucIV: PBYTE;
{IN} pucInData: PBYTE;
{IN} uiInDataLen: LONGWORD;
{OUT} pucOutData: PBYTE;
{INOUT} var puiOutDataLen: LONGWORD
): LONGWORD; stdcall;
{/*!
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
*/}
mw_sym_decrypt: function (
{IN} hHandle: MWHANDLE;
{IN} pcKeyFileName: PANSIChar;
{IN} ucAlgoMode: BYTE;
{IN} pucIV: PBYTE;
{IN} pucInData: PBYTE;
{IN} uiInDataLen: LONGWORD;
{OUT} pucOutData: PBYTE;
{INOUT} var puiOutDataLen: LONGWORD
): LONGWORD; stdcall;
{/*!
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
*/}
mw_rsa_encrypt: function (
{IN} hHandle: MWHANDLE;
{IN} pcKeyFileName: PANSIChar;
{IN} ucPadMode: BYTE;
{IN} pucInData: PBYTE;
{IN} uiInDataLen: LONGWORD;
{OUT} pucOutData: PBYTE;
{INOUT} var puiOutDataLen: LONGWORD
): LONGWORD; stdcall;
                        
{/*!
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
*/}
mw_rsa_decrypt: function (
{IN} hHandle: MWHANDLE;
{IN} pcKeyFileName: PANSIChar;
{IN} ucPadMode: BYTE;
{IN} pucInData: PBYTE;
{IN} uiInDataLen: LONGWORD;
{OUT} pucOutData: PBYTE;
{INOUT} var puiOutDataLen: LONGWORD
): LONGWORD; stdcall;
                                   
{/*!
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
*/}
mw_signature: function (
{IN} hHandle: MWHANDLE;
{IN} pcKeyFileName: PANSIChar;
{IN} ucHashAlg: BYTE;
{IN} pucMessageData: PBYTE;
{IN} uiMessageDataLen: LONGWORD;
{OUT} pucSignData: PBYTE;
{INOUT} var puiSignDataLen: LONGWORD
): LONGWORD; stdcall;

{/*!
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
*/}
mw_verify_sign: function (
{IN} hHandle: MWHANDLE;
{IN} pcKeyFileName: PANSIChar;
{IN} ucHashAlg: BYTE;
{IN} pucSignData: PBYTE;
{IN} uiSignDataLen: LONGWORD;
{IN} pucMessageData: PBYTE;
{IN} uiMessageDataLen: LONGWORD
): LONGWORD; stdcall;

{/*!
*   @brief          HMAC计算
*   @param[in]      hHandle           魔锐设备句柄
*   @param[in]      pcKeyFileName     密钥文件名称 
*   @param[in]      pucInData         输入数据
*   @param[in]      uiInDataLen       输入数据长度
*   @param[out]     pucOutData        输出数据 
*   @param[in out]  puiOutDataLen     输出数据长度，参见宏定义：MW_HMAC_MD5_DIGEST_LENGTH、MW_HMAC_SHA1_DIGEST_LENGTH、MW_HMAC_SHA256_DIGEST_LENGTH
*   @return     执行成功返回 MW_SUCCESS ，执行失败时返回错误码。
*   @remarks    
*/}
mw_hmac_calc: function (
{IN} hHandle: MWHANDLE;
{IN} pcKeyFileName: PANSIChar;
{IN} pucInData: PBYTE;
{IN} uiInDataLen: LONGWORD;
{OUT} pucOutData: PBYTE;
{INOUT} var puiOutDataLen: LONGWORD
): LONGWORD; stdcall;                               

{/*!
*   @brief          制作升级包
*   @param[in]      hHandle                魔锐设备句柄
*   @param[in]      uiDevPID               设备产品号
*   @param[in]      pucSerialNum           硬件唯一序列号 固定长度为：MW_SN_LENGTH，不指定SN绑定时，可以为NULL
*   @param[in]      pstUpdateFileList      文件路径列表,最大文件个数参见宏定义MW_UPDATE_FILE_COUNT_MAX,升级文件的个数为uiFileCount
*   @param[in]      uiFileCount            升级文件的个数,它指定了pstUpdateFileList中的大小 
*   @param[out]     pucOutPkg              升级包数据
*   @param[in out]  puiOutPkgLen           升级包数据长度
*   @return     执行成功返回 MW_SUCCESS ，执行失败时返回错误码。
*   @remarks    
*/}
mw_make_update_pkg: function (
{IN} hHandle: MWHANDLE;
{IN} uiDevPID: LONGWORD;
{IN} pucSN: PBYTE;
{IN} pstFilePathList: PMW_UPDADE_FILE_CTX;
{IN} uiFileCount: LONGWORD;
{OUT} pucOutPkg: PBYTE;
{INOUT} var puiOutPkgLen: LONGWORD
): LONGWORD; stdcall;
                        
{/*!
*   @brief      远程升级
*   @param[in]  hHandle             魔锐设备句柄
*   @param[in]  pucInPkg            升级包数据
*   @param[in]  uiInPkgLen          升级包数据长度
*   @return     执行成功返回 MW_SUCCESS ，执行失败时返回错误码。
*   @remarks    
*/}
mw_update: function (
{IN} hHandle: MWHANDLE;
{IN} pucInPkg: PBYTE;
{IN} uiInPkgLen: LONGWORD
): LONGWORD; stdcall;
{/*!
*   @brief      恢复出厂设置
*   @param[in]  hHandle             魔锐设备句柄
*   @return     执行成功返回 MW_SUCCESS ，执行失败时返回错误码。
*   @remarks    将设备恢复出厂状态，即默认PID、默认PIN，文件系统清空。需开发商权限
*/}
mw_restore_factory: function (
{IN} hHandle: MWHANDLE
): LONGWORD; stdcall;
{/*!
*   @brief      错误码帮助
*   @param[in]  hHandle             魔锐设备句柄
*   @param[in]  uiErrorCode         错误码
*   @param[in]  uiLanguageID        语言种类ID
*   @return     执行成功返回 MW_SUCCESS ，执行失败时返回错误码。
*   @remarks    
*/}
mw_get_error_desc: function(
{IN} uiErrorCode: LONGWORD;
{IN} uiLanguageID: LONGWORD
): PAnsiChar; stdcall;


implementation

resourcestring
  rsErrLoadMemLibary    = '加载库失败';

// 引用将 dll 转换为二进制数组的inc文件
// 安全升级方案：避免静态分析替换dll内容
//       可对 dll 转换后的二进制数据进行加密处理，然后在使用期间解密。
//       软件开发者可根据实际情况酌情考虑此方案。
const
{$I moway_api.dll.inc}

var
{$IF CompilerVersion >= 23}
  LibModule: TMemoryModule;
{$ELSE}
  LibModule: TBTMemoryModule;
{$IFEND}
  
  LibInited: Boolean;

procedure Init;
  function GetFuncAddress(const AProcName: PChar): Pointer;
  begin
  {$IF CompilerVersion >= 23}
    Result := MemoryGetProcAddress(LibModule, PAnsiChar(AProcName));
  {$ELSE}
    Result := BTMemoryGetProcAddress(LibModule, PChar(AProcName));
  {$IFEND}
  end;
begin
  if LibInited then
    Exit;

{$IF CompilerVersion >= 23}
  LibModule := MemoryLoadLibary(@data);
{$ELSE}
  LibModule := BTMemoryLoadLibary(@data, Length(data));
{$IFEND}
  if LibModule = nil then
    Exit;

  @mw_enum              := GetFuncAddress(PChar('mw_enum'));
  @mw_open              := GetFuncAddress(PChar('mw_open'));
  @mw_close             := GetFuncAddress(PChar('mw_close'));
  @mw_verify_pin        := GetFuncAddress(PChar('mw_verify_pin'));
  @mw_change_pin        := GetFuncAddress(PChar('mw_change_pin'));
  @mw_set_pid           := GetFuncAddress(PChar('mw_set_pid'));
  @mw_control           := GetFuncAddress(PChar('mw_control'));
  @mw_get_device_info   := GetFuncAddress(PChar('mw_get_device_info'));
  @mw_get_device_status := GetFuncAddress(PChar('mw_get_device_status'));
  @mw_enum_file         := GetFuncAddress(PChar('mw_enum_file'));
  @mw_create_file       := GetFuncAddress(PChar('mw_create_file'));
  @mw_read_file         := GetFuncAddress(PChar('mw_read_file'));
  @mw_write_file        := GetFuncAddress(PChar('mw_write_file'));
  @mw_delete_file       := GetFuncAddress(PChar('mw_delete_file'));
  @mw_get_file_property := GetFuncAddress(PChar('mw_get_file_property'));
  @mw_sym_encrypt       := GetFuncAddress(PChar('mw_sym_encrypt'));
  @mw_sym_decrypt       := GetFuncAddress(PChar('mw_sym_decrypt'));
  @mw_rsa_encrypt       := GetFuncAddress(PChar('mw_rsa_encrypt'));
  @mw_rsa_decrypt       := GetFuncAddress(PChar('mw_rsa_decrypt'));
  @mw_signature         := GetFuncAddress(PChar('mw_signature'));
  @mw_verify_sign       := GetFuncAddress(PChar('mw_verify_sign'));
  @mw_hmac_calc         := GetFuncAddress(PChar('mw_hmac_calc'));
  @mw_make_update_pkg   := GetFuncAddress(PChar('mw_make_update_pkg'));
  @mw_update            := GetFuncAddress(PChar('mw_update'));
  @mw_restore_factory   := GetFuncAddress(PChar('mw_restore_factory'));
  @mw_get_error_desc    := GetFuncAddress(PChar('mw_get_error_desc'));

  LibInited := True;
end;

procedure Done;
begin
  if not LibInited then
    Exit;
     //TODO Pointer for Function

  if LibModule <> nil then
  {$IF CompilerVersion >= 23}
    MemoryFreeLibrary(LibModule);
  {$ELSE}
    BTMemoryFreeLibrary(LibModule);
  {$IFEND}
  
  LibModule := nil;

  LibInited := False;
end;

////////////////////////////////////////////////////////////////////////////////
//功能：初始化、析构
//参数：
////////////////////////////////////////////////////////////////////////////////
initialization
  Init;

finalization
  Done;

end.
