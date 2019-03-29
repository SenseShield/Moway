program test_moway;

{$APPTYPE CONSOLE}

{$R *.res}


uses
  {$IF CompilerVersion >= 23}
    // XE2 及以上版本引用头文件
    Winapi.Windows, System.SysUtils, System.Classes,
  {$ELSE}
    // XE以下版本（包括Delphi7）等头文件不包括命名空间
    windows, SysUtils, Classes,
  {$IFEND}
  moway_api;


const
  uiMaxDevCount = 32;         // 枚举锁最大锁数量
  uiFileListMaxCount = 50;    // 锁内最大文件数
  PIN_CHANGE_DEFAULT_TRY_COUNT = 0; // PIN码修改默认修改次数，当设置为0时，使用默认值15。


const
  USER_RW_FILENAME = 'userwt.evd';  // 读写文件
  USER_RD_FILENAME = 'userrd.evd';  // 只读文件

  USER_KEY_AES128_FILENAME = 'aes_128.sls';     // key文件
  USER_KEY_HMAC_FILENAME = 'hmac_md5.sls';     // key文件

  UPDATE_PKG_UPDATE_FILENAME = 'userrd.evd';
  UPDATE_PKG_CREATE_FILENAME = 'userrwnew.evc';
  UPDATE_PKG_FILENAME = 'MowayUpdatePkg.dat';

  UPDATE_PKG_UPDATE_FILE_DATA = 'update pkg update rw file.';
  UPDATE_PKG_CREATE_FILE_DATA = 'update pkg create new rw file.';


var
  // 开发商PIN（已设置）
  // 加密锁未设置默认 PIN: 000000000000000000000000
  //     开发商使用前必须通过修改 PIN 接口修改PIN，否则无法执行文件操作等功能接口，详情见 DeveloperInitDevice() 示例
  // 注意：当前示例使用已设置后的PIN码，开发者初始化新锁时需要修改为默认PIN。
  PINDeveloper: AnsiString = '000000000000000000000000';
  // 用户PIN，出厂默认PIN码
  PINUser: AnsiString = '00000000';
  TestData: AnsiString = 'Hello Moway!';


// 枚举打开加密锁（示例）
function HelloMoway(): LongWord;
var
  puiDevCount: LongWord;
  pDevInfoList: PMW_DEVICE_INFO_CTX;
  hHandle: MWHANDLE;
  pErrMsg: PAnsiChar;
  testPIN: PByte;
label END_LAB;
begin
  pDevInfoList := GetMemory(uiMaxDevCount * SizeOf(MW_DEVICE_INFO_CTX));
  // 枚举锁
  Result := mw_enum(pDevInfoList, uiMaxDevCount, puiDevCount);
  if Result <> MW_SUCCESS then begin
    Writeln('mw_enum failed, Result = ', Result);
    GoTo END_LAB;
  end;
  if puiDevCount = 0 then begin
    Result := MW_ERROR_NO_DEVICE;
    Writeln('Not found moway lock.');
    GoTo END_LAB;
  end;

  // 打开锁
  Result := mw_open(pDevInfoList, MW_OPEN_EXCLUSIVE_MODE, hHandle);
	if Result <> MW_SUCCESS then begin
    Writeln('open moway device error, errorcode = ', Result);
    GoTo END_LAB;
  end
  else begin
    Writeln('open moway device success.');
  end;

  //认证开发商PIN
  Result := mw_verify_pin(hHandle, Byte(MW_PIN_TYPE_DEVELOPER), PByte(PINDeveloper));
  if Result <> MW_SUCCESS then begin
    pErrMsg := mw_get_error_desc(Result, MW_LANGUAGE_ID_EN);
    Writeln(Format('verify_pin developer error, errorcode = $%.8x, errormsg = %s', [Result, pErrMsg]));
    GoTo END_LAB;
  end
  else begin
    Writeln('verify_pin developer success.');
  end;        

END_LAB:
  if hHandle <> nil then begin
    //关闭锁
    Result := mw_close(hHandle);
    if Result <> MW_SUCCESS then begin
      Writeln('close moway device error, errorcode = ', Result);
    end
    else begin
      Writeln('close moway device success.');
    end;
  end;

  if pDevInfoList <> nil then begin
    FreeMemory(pDevInfoList);
  end;
  result := MW_SUCCESS;
end;

// 枚举打开加密锁
function OpenDevice(var hHandle: MWHANDLE): LongWord;
var
  puiDevCount: LongWord;
  pDevInfoList: PMW_DEVICE_INFO_CTX;
  pErrMsg: PAnsiChar;
label END_LAB;
begin
  Result := MW_SUCCESS;
  
  pDevInfoList := GetMemory(uiMaxDevCount * SizeOf(MW_DEVICE_INFO_CTX));
  // 枚举锁
  Result := mw_enum(pDevInfoList, uiMaxDevCount, puiDevCount);
  if Result <> MW_SUCCESS then begin
    Writeln('mw_enum failed, Result = ', Result);
    GoTo END_LAB;
  end;
  if puiDevCount = 0 then begin
    Writeln('Not found moway lock.');
    GoTo END_LAB;
  end;

  // 打开锁
  Result := mw_open(pDevInfoList, MW_OPEN_EXCLUSIVE_MODE, hHandle);
	if Result <> MW_SUCCESS then begin
    Writeln('open moway device error, errorcode = ', Result);
    GoTo END_LAB;
  end
  else begin
    Writeln('open moway device success.');
  end;

END_LAB:
  if pDevInfoList <> nil then begin
    FreeMemory(pDevInfoList);
  end;
end;

// 关闭加密锁
function CloseDevice(hHandle: MWHANDLE): LongWord;
begin
  if hHandle <> nil then begin
    Result := mw_close(hHandle);
    if Result <> MW_SUCCESS then begin
      Writeln('close moway device error, errorcode = ', Result);
    end
    else begin
      Writeln('close moway device success.');
    end;
  end;
end;

// 枚举当前锁内文件
function EnumFile(hHandle: MWHANDLE): LongWord;
var
  i: Integer;
  pErrMsg: PAnsiChar;

  pstFilePropertyList: PMW_FILE_PROPERTY;
  uiReturnedFileCount: LongWord;
  pFileProperty: PMW_FILE_PROPERTY;
label
  END_LAB;
begin
  //枚举锁内文件
  pstFilePropertyList := GetMemory(SizeOf(MW_FILE_PROPERTY) * uiFileListMaxCount);
  FillMemory(pstFilePropertyList, SizeOf(MW_FILE_PROPERTY) * uiFileListMaxCount, 0);  // 初始化内存缓冲区
  Result := mw_enum_file(hHandle, pstFilePropertyList, uiFileListMaxCount, uiReturnedFileCount);
  if Result <> MW_SUCCESS then begin
    pErrMsg := mw_get_error_desc(Result, MW_LANGUAGE_ID_EN);
    Writeln(Format('mw_enum_file error, errorcode = $%.8x, errormsg = %s', [Result, pErrMsg]));
    GoTo END_LAB;
  end
  else begin
    Writeln('mw_enum_file success, File count = ', uiReturnedFileCount);
  end;

  pFileProperty := pstFilePropertyList;
  if uiReturnedFileCount > 0 then begin
    for i := 0 to uiReturnedFileCount - 1 do
    begin
      Writeln(Format('filename:%s', [pFileProperty.acName]));
      Inc(pFileProperty);
    end;
  end;
  
END_LAB:
  FreeMem(pstFilePropertyList);
end;

// 清除权限验证状态
function DeviceResetPINVerifyStatus(hHandle: MWHANDLE): LongWord;
var
  uiReturnedLen: LongWord;
begin
  // 成功验证过一次后，在不拔出加密锁的情况下，调用接口 mw_close() 不会清除验证状态。
  // TDOO 此问题会在后续版本中修正。
  Result := mw_control(hHandle, MW_CTRL_CODE_ITEM_RESET, nil, 0, nil, 0, uiReturnedLen);
  Writeln(Format('mw_control(MW_CTRL_CODE_ITEM_RESET) errorcode = $%.8x', [Result]));
end;
                      
// 开发商权限操作文件（示例）
function DeviceDeveloperFileOpDemo(): LongWord;
var
  puiDevCount: LongWord;
  pDevInfoList: PMW_DEVICE_INFO_CTX;
  hHandle: MWHANDLE;
  pErrMsg: PAnsiChar;
  stFileProperty: MW_FILE_PROPERTY;
  szReadBuffer: PByte;
label END_LAB;
begin
  pDevInfoList := GetMemory(uiMaxDevCount * SizeOf(MW_DEVICE_INFO_CTX));
  // 枚举锁
  Result := mw_enum(pDevInfoList, uiMaxDevCount, puiDevCount);
  if Result <> MW_SUCCESS then begin
    Writeln('mw_enum failed, Result = ', Result);
    GoTo END_LAB;
  end;
  if puiDevCount = 0 then begin
    Writeln('Not found moway lock.');
    GoTo END_LAB;
  end;

  // 打开锁
  Result := mw_open(pDevInfoList, MW_OPEN_EXCLUSIVE_MODE, hHandle);
	if Result <> MW_SUCCESS then begin
    Writeln('open moway device error, errorcode = ', Result);
    GoTo END_LAB;
  end
  else begin
    Writeln('open moway device success.');
  end;

  //认证开发商PIN
  Result := mw_verify_pin(hHandle, Byte(MW_PIN_TYPE_DEVELOPER), PByte(PINDeveloper));
  if Result <> MW_SUCCESS then begin
    pErrMsg := mw_get_error_desc(Result, MW_LANGUAGE_ID_EN);
    Writeln(Format('verify_pin developer error, errorcode = $%.8x, errormsg = %s', [Result, pErrMsg]));
    GoTo END_LAB;
  end
  else begin
    Writeln('verify_pin developer success.');
  end;

  //枚举锁内文件
  EnumFile(hHandle);

  // 开发商权限，创建二进制用户可读写文件
  FillMemory(@stFileProperty, SizeOf(stFileProperty), 0);
	stFileProperty.ucType := MW_FILE_TYPE_BINARY;
	stFileProperty.usPrivilege := MW_FILE_PRIV_TYPE_WRITE;
	stFileProperty.uiSize := $123;
  StrCopy(stFileProperty.acName, USER_RW_FILENAME);
	Result := mw_create_file(hHandle, @stFileProperty);
  if Result <> MW_SUCCESS then begin
    pErrMsg := mw_get_error_desc(Result, MW_LANGUAGE_ID_EN);
    Writeln(Format('mw_create_file(MW_FILE_PRIV_TYPE_WRITE) error, errorcode = $%.8x, errormsg = %s', [Result, pErrMsg]));
  end
  else begin
    Writeln('mw_create_file(MW_FILE_PRIV_TYPE_WRITE) success');
  end;

  EnumFile(hHandle);

  {文件读写操作}
  // 文件读写
	Result := mw_write_file(hHandle, PAnsiChar(AnsiString(USER_RW_FILENAME)), 0, 3, PByte(PAnsiChar(AnsiString('abc'))));
  if Result <> MW_SUCCESS then begin
    pErrMsg := mw_get_error_desc(Result, MW_LANGUAGE_ID_EN);
    Writeln(Format('mw_write_file(%s) error, errorcode = $%.8x, errormsg = %s', [USER_RW_FILENAME, Result, pErrMsg]));
  end
  else begin
    Writeln(Format('mw_write_file(%s) success', [USER_RW_FILENAME]));
  end;

  // 读取文件内容
  szReadBuffer := GetMemory(10);
  Result := mw_read_file(hHandle, PAnsiChar(AnsiString(USER_RW_FILENAME)), 0, 3, szReadBuffer);
  if Result <> MW_SUCCESS then begin
    pErrMsg := mw_get_error_desc(Result, MW_LANGUAGE_ID_EN);
    Writeln(Format('mw_read_file(%s) error, errorcode = $%.8x, errormsg = %s', [USER_RW_FILENAME, Result, pErrMsg]));
  end
  else begin
    Writeln(Format('mw_read_file(%s) %s', [USER_RW_FILENAME, PAnsiChar(szReadBuffer)]));
  end;
  FreeMem(szReadBuffer);

  {文件删除操作}
  // 删除，二进制用户可读写文件
  Result := mw_delete_file(hHandle, PAnsiChar(AnsiString(USER_RW_FILENAME)));
  if Result <> MW_SUCCESS then begin
    pErrMsg := mw_get_error_desc(Result, MW_LANGUAGE_ID_EN);
    Writeln(Format('mw_delete_file(MW_FILE_PRIV_TYPE_WRITE) error, errorcode = $%.8x, errormsg = %s', [Result, pErrMsg]));
  end
  else begin
    Writeln('mw_delete_file(MW_FILE_PRIV_TYPE_WRITE) success');
  end;
  
  EnumFile(hHandle);

END_LAB:
  if hHandle <> nil then begin
    //关闭锁
    Result := mw_close(hHandle);
    if Result <> MW_SUCCESS then begin
      Writeln('close moway device error, errorcode = ', Result);
    end
    else begin
      Writeln('close moway device success.');
    end;
  end;

  if pDevInfoList <> nil then begin
    FreeMemory(pDevInfoList);
  end;
  result := MW_SUCCESS;
end;


// 获取设备信息（无权限要求）
function GetDeviceInfo(): LongWord;
const
  BufferLength = 257;
var
  hHandle: MWHANDLE;
  uiReturnedLen: LongWord;
  stDeviceStatusCtx: MW_DEVICE_STATUS;

  pvBuffer: array[0..BufferLength] of Byte;
  uiBufferLength: LongWord;
  
  bLEDOpen: Byte;
  bLEDClose: Byte;

  stDeviceAllitemInfo: MW_DEVICE_ALL_ITEM_INFO;
  stDeviceAllitemInfoSize: LongWord;
label
  END_LAB;
begin
  //1.枚举锁
  //2.打开锁
  //3.获取所有信息（结构体）
  //4.获取指定信息（Item）
  //5.关闭锁
  Result := OpenDevice(hHandle);
  if Result <> MW_SUCCESS then
  begin
    Goto END_LAB;
  end;

  bLEDOpen := 1;
  bLEDClose := 0;

  //关闭LED（插入后默认亮灯）
	Result := mw_control(hHandle,MW_CTRL_CODE_ITEM_LED,@bLEDOpen,1,nil,0,uiReturnedLen);
  Writeln(Format('mw_control(MW_CTRL_CODE_ITEM_LED) errorcode = $%.8x', [Result]));

  //打开LED
	Result := mw_control(hHandle,MW_CTRL_CODE_ITEM_LED,@bLEDClose,1,nil,0,uiReturnedLen);
  Writeln(Format('mw_control(MW_CTRL_CODE_ITEM_LED) errorcode = $%.8x', [Result]));

  // 获取设备状态
  FillMemory(@stDeviceStatusCtx, SizeOf(stDeviceStatusCtx), 0);
	Result := mw_get_device_status(hHandle, @stDeviceStatusCtx);
  Writeln(Format('mw_get_device_status errorcode = $%.8x', [Result]));

  // 3.获取所有设备信息
  stDeviceAllitemInfoSize := SizeOf(stDeviceAllitemInfo);
  Result := mw_get_device_info(hHandle,MW_GET_INFO_ITEM_ALL, Pointer(@stDeviceAllitemInfo), stDeviceAllitemInfoSize);
  if Result <> MW_SUCCESS then begin
    Writeln(Format('mw_delete_file(MW_FILE_PRIV_TYPE_WRITE) error, errorcode = $%.8x', [Result]));
  end
  else begin
    Writeln('mw_get_device_info(MW_GET_INFO_ITEM_ALL) success, PID=', stDeviceAllitemInfo.uiPID);
  end;

  uiBufferLength := Length(pvBuffer);

  //4.获取指定信息（Item）
  // 4.1 获取设备pid（需要使用管理员权限设置PID，否则无法使用文件、秘钥等功能）
	Result := mw_get_device_info(hHandle,MW_GET_INFO_ITEM_PID,@pvBuffer,uiBufferLength);
  Writeln(Format('mw_get_device_info(MW_GET_INFO_ITEM_PID) errorcode = $%.8x', [Result]));

  // 4.2 获取设备SN
	uiBufferLength := BufferLength;
  FillMemory(@pvBuffer, uiBufferLength, 0);
	Result := mw_get_device_info(hHandle,MW_GET_INFO_ITEM_SN,@pvBuffer,uiBufferLength);
  Writeln(Format('mw_get_device_info(MW_GET_INFO_ITEM_SN) errorcode = $%.8x', [Result]));

END_LAB:
  CloseDevice(hHandle);
end;

// 设备文件，开发商（权限）创建文件
function DeveloperCreateFiles(hHandle: MWHANDLE): LongWord;
var
  uiReturnedLen: LongWord;
  stDeviceStatusCtx: MW_DEVICE_STATUS;
  pErrMsg: PAnsiChar;

  pvBuffer: PByte;
  uiBufferLength: LongWord;

  stFileProperty: MW_FILE_PROPERTY;
  szReadBuffer: PByte;
  
  mStream: TMemoryStream;
begin
  // 1.开发商权限，创建（二进制）用户只读文件
  // 用户权限，只能读取文件；开发商权限可以进行读写操作。
  FillMemory(@stFileProperty, SizeOf(stFileProperty), 0);
	stFileProperty.ucType := MW_FILE_TYPE_BINARY;
	stFileProperty.usPrivilege := MW_FILE_PRIV_TYPE_READ;
	stFileProperty.uiSize := $345;
  StrCopy(stFileProperty.acName, USER_RD_FILENAME);
	Result := mw_create_file(hHandle, @stFileProperty);
  if Result <> MW_SUCCESS then begin
    pErrMsg := mw_get_error_desc(Result, MW_LANGUAGE_ID_EN);
    Writeln(Format('mw_create_file(MW_FILE_PRIV_TYPE_READ) error, errorcode = $%.8x, errormsg = %s', [Result, pErrMsg]));
  end
  else begin
    Writeln('mw_create_file(MW_FILE_PRIV_TYPE_WRITE) success');
  end;
  // 开发商权限向只读文件写入数据
	Result := mw_write_file(hHandle, PAnsiChar(AnsiString(USER_RD_FILENAME)), 0, Length(TestData), PByte(PAnsiChar(TestData)));
  if Result <> MW_SUCCESS then begin
    pErrMsg := mw_get_error_desc(Result, MW_LANGUAGE_ID_EN);
    Writeln(Format('mw_write_file(%s) error, errorcode = $%.8x, errormsg = %s', [USER_RD_FILENAME, Result, pErrMsg]));
  end
  else begin
    Writeln(Format('mw_write_file(%s) success', [USER_RD_FILENAME]));
  end;

  // 2.开发商权限，创建（二进制）用户可读写文件
  // 用户权限，可读写文件（但不能超过设置文件大小）
  // 2.1 创建锁内文件
  FillMemory(@stFileProperty, SizeOf(stFileProperty), 0);
	stFileProperty.ucType := MW_FILE_TYPE_BINARY;
	stFileProperty.usPrivilege := MW_FILE_PRIV_TYPE_WRITE;
	stFileProperty.uiSize := $123;
  StrCopy(stFileProperty.acName, USER_RW_FILENAME);
	Result := mw_create_file(hHandle, @stFileProperty);
  if Result <> MW_SUCCESS then begin
    pErrMsg := mw_get_error_desc(Result, MW_LANGUAGE_ID_EN);
    Writeln(Format('mw_create_file(MW_FILE_PRIV_TYPE_WRITE) error, errorcode = $%.8x, errormsg = %s', [Result, pErrMsg]));
  end
  else begin
    Writeln('mw_create_file(MW_FILE_PRIV_TYPE_WRITE) success');
  end;
  // 2.2 开发商权限向用户可读写文件写入数据
	Result := mw_write_file(hHandle, PAnsiChar(AnsiString(USER_RW_FILENAME)), 0, Length(TestData), PByte(PAnsiChar(TestData)));
  if Result <> MW_SUCCESS then begin
    pErrMsg := mw_get_error_desc(Result, MW_LANGUAGE_ID_EN);
    Writeln(Format('mw_write_file(%s) error, errorcode = $%.8x, errormsg = %s', [USER_RW_FILENAME, Result, pErrMsg]));
  end
  else begin
    Writeln(Format('mw_write_file(%s) success', [USER_RW_FILENAME]));
  end;

  // 3.开发商权限，创建AES128密钥文件
  // 用户权限，可以调用密码学算法使用密钥文件
  // 3.1 创建锁内文件
  FillMemory(@stFileProperty, SizeOf(stFileProperty), 0);
	stFileProperty.ucType := MW_FILE_TYPE_KEY;
	stFileProperty.usPrivilege := MW_FILE_PRIV_TYPE_USE;
	stFileProperty.uiSize := $FF;
  StrCopy(stFileProperty.acName, USER_KEY_AES128_FILENAME);
	Result := mw_create_file(hHandle, @stFileProperty);
  if Result <> MW_SUCCESS then begin
    pErrMsg := mw_get_error_desc(Result, MW_LANGUAGE_ID_EN);
    Writeln(Format('mw_create_file(MW_FILE_TYPE_KEY) error, errorcode = $%.8x, errormsg = %s', [Result, pErrMsg]));
  end
  else begin
    Writeln('mw_create_file(MW_FILE_TYPE_KEY) success');
  end;
  // 3.2 写入秘钥数据。密钥数据可通过密码学工具生成。
  mStream := TMemoryStream.Create;
  try
    mStream.LoadFromFile(USER_KEY_AES128_FILENAME);
    Result := mw_write_file(hHandle, PAnsiChar(AnsiString(USER_KEY_AES128_FILENAME)), 0, LongWord(mStream.Size), PByte(mStream.Memory));
    if Result <> MW_SUCCESS then begin
      pErrMsg := mw_get_error_desc(Result, MW_LANGUAGE_ID_EN);
      Writeln(Format('mw_write_file(%s) error, errorcode = $%.8x, errormsg = %s', [USER_KEY_AES128_FILENAME, Result, pErrMsg]));
    end
    else begin
      Writeln(Format('mw_write_file(%s) success', [USER_KEY_AES128_FILENAME]));
    end;    
  finally
    mStream.Free;
  end;

  // 4.开发商权限，创建HMAC密钥文件
  // 用户权限，可以调用密码学算法使用密钥文件
  // 4.1 创建锁内文件
  FillMemory(@stFileProperty, SizeOf(stFileProperty), 0);
	stFileProperty.ucType := MW_FILE_TYPE_KEY;
	stFileProperty.usPrivilege := MW_FILE_PRIV_TYPE_USE;
	stFileProperty.uiSize := $FF;
  StrCopy(stFileProperty.acName, USER_KEY_HMAC_FILENAME);
	Result := mw_create_file(hHandle, @stFileProperty);
  if Result <> MW_SUCCESS then begin
    pErrMsg := mw_get_error_desc(Result, MW_LANGUAGE_ID_EN);
    Writeln(Format('mw_create_file(MW_FILE_TYPE_KEY) %s error, errorcode = $%.8x, errormsg = %s', [USER_KEY_HMAC_FILENAME, Result, pErrMsg]));
  end
  else begin
    Writeln('mw_create_file(MW_FILE_TYPE_KEY)' + USER_KEY_HMAC_FILENAME + ' success');
  end;
  // 4.2 写入秘钥数据。
  mStream := TMemoryStream.Create;
  try
    mStream.LoadFromFile(USER_KEY_HMAC_FILENAME);
    Result := mw_write_file(hHandle, PAnsiChar(AnsiString(USER_KEY_HMAC_FILENAME)), 0, LongWord(mStream.Size), PByte(mStream.Memory));
    if Result <> MW_SUCCESS then begin
      pErrMsg := mw_get_error_desc(Result, MW_LANGUAGE_ID_EN);
      Writeln(Format('mw_write_file(%s) error, errorcode = $%.8x, errormsg = %s', [USER_KEY_HMAC_FILENAME, Result, pErrMsg]));
    end
    else begin
      Writeln(Format('mw_write_file(%s) success', [USER_KEY_HMAC_FILENAME]));
    end;    
  finally
    mStream.Free;
  end;
end;


// 初始化加密锁
function DeveloperInitDevice(): LongWord;
var
  hHandle: MWHANDLE;
  szPIDSeed: AnsiString;

  pErrMsg: PAnsiChar;
  
  buffer: array[0..256] of Byte;
  uiBufferLength: LongWord;
  szNewDeveloperPIN: AnsiString;
  uiPID: PInteger;
label
  END_LAB;
begin
  //1.枚举锁
  //2.打开锁
  //3.验证开发商PIN
  //4.设置PID
  //5.修改开发商PIN
  //6.创建文件（二进制、读写、密钥文件-AES128、密钥文件-HMAC）
  //7.开发商权限写文件
  //8.关闭锁
  Result := OpenDevice(hHandle);
  if Result <> MW_SUCCESS then
  begin
    Goto END_LAB;
  end;

  // 3.验证开发商PIN（获取开发商权限）
  Result := mw_verify_pin(hHandle, Byte(MW_PIN_TYPE_DEVELOPER), PByte(PINDeveloper));
  if Result <> MW_SUCCESS then begin
    pErrMsg := mw_get_error_desc(Result, MW_LANGUAGE_ID_EN);
    Writeln(Format('verify_pin developer error, errorcode = $%.8x, errormsg = %s', [Result, pErrMsg]));
    GoTo END_LAB;
  end
  else begin
    Writeln('verify_pin developer success.');
  end;

  // 4.设置PID
  // 设置PID种子，种子最好使用随机数。务必设置复杂且不易与其他人重复的数据，否则会造成严重的结果！！！
  // 如果不同开发商之间出现PID种子相同的情况，造成锁内PID相同，PID相同时加壳工具加密后的软件能够正常使用。
  szPIDSeed := '00000000000000000000000000000000';
  Writeln(Format('mw_set_pid PIDSeed(%d) = %s', [Length(szPIDSeed), szPIDSeed]));
  Result := mw_set_pid(hHandle, PByte(szPIDSeed), 32);
  if Result <> MW_SUCCESS then begin
    pErrMsg := mw_get_error_desc(Result, MW_LANGUAGE_ID_EN);
    Writeln(Format('mw_set_pid error, errorcode = $%.8x, errormsg = %s', [Result, pErrMsg]));
    GoTo END_LAB;
  end
  else begin
    Writeln('mw_set_pid success.');
  end;

  uiBufferLength := Length(buffer);
  FillMemory(@buffer, Length(buffer), 0);
  //获取设备pid（需要使用管理员权限设置PID，否则无法使用文件、密钥等功能）
	Result := mw_get_device_info(hHandle,MW_GET_INFO_ITEM_PID, @buffer, uiBufferLength);
  uiPID := @buffer; 
  Writeln(Format('mw_get_device_info(MW_GET_INFO_ITEM_PID) PID = %d errorcode = $%.8x', [uiPID^, Result]));

  // 5.修改开发商PIN
  // 使用默认PIN码无法使用其他应用功能
  szNewDeveloperPIN := PINDeveloper;    // 注意：本示例为了方便后续代码逻辑调用，此处将新旧PIN设置相同。开发者需修改为新PIN码
  Result := mw_change_pin(hHandle,MW_PIN_TYPE_DEVELOPER,
    WORD(PIN_CHANGE_DEFAULT_TRY_COUNT),
    PByte(PINDeveloper),
    PByte(szNewDeveloperPIN));
  Writeln(Format('mw_change_Pin new PIN = %s, errorcode = $%.8x', [szNewDeveloperPIN, Result]));

  //6.创建文件（二进制、读写、密钥文件-AES128、密钥文件-HMAC）
  //7.开发商权限写文件
  DeveloperCreateFiles(hHandle);

END_LAB:
  CloseDevice(hHandle);
end;


// 用户权限打开锁，读写文件
function UserReadWriteFiles(hHandle: MWHANDLE): LongWord;
var
  pErrMsg: PAnsiChar;
  szReadBuffer: PByte;
  uiReadBufferLen: LongWord;
begin
  uiReadBufferLen := 128;

  // 读取只读文件内容
  szReadBuffer := GetMemory(uiReadBufferLen);
  Result := mw_read_file(hHandle, PAnsiChar(AnsiString(USER_RD_FILENAME)), 0, Length(TestData), szReadBuffer);
  if Result <> MW_SUCCESS then begin
    pErrMsg := mw_get_error_desc(Result, MW_LANGUAGE_ID_EN);
    Writeln(Format('mw_read_file(%s) error, errorcode = $%.8x, errormsg = %s', [USER_RD_FILENAME, Result, pErrMsg]));
  end
  else begin
    Writeln(Format('mw_read_file(%s) %s', [USER_RD_FILENAME, PAnsiChar(szReadBuffer)]));
  end;

  // 写入可读写文件
	Result := mw_write_file(hHandle, PAnsiChar(AnsiString(USER_RW_FILENAME)), 0, 3, PByte(PAnsiChar(AnsiString('abc'))));
  if Result <> MW_SUCCESS then begin
    pErrMsg := mw_get_error_desc(Result, MW_LANGUAGE_ID_EN);
    Writeln(Format('mw_write_file(%s) error, errorcode = $%.8x, errormsg = %s', [USER_RW_FILENAME, Result, pErrMsg]));
  end
  else begin
    Writeln(Format('mw_write_file(%s) success', [USER_RW_FILENAME]));
  end;
  // 读取可读写文件内容
  FillMemory(szReadBuffer, uiReadBufferLen, 0);
  Result := mw_read_file(hHandle, PAnsiChar(AnsiString(USER_RW_FILENAME)), 0, Length(TestData), szReadBuffer);
  if Result <> MW_SUCCESS then begin
    pErrMsg := mw_get_error_desc(Result, MW_LANGUAGE_ID_EN);
    Writeln(Format('mw_read_file(%s) error, errorcode = $%.8x, errormsg = %s', [USER_RW_FILENAME, Result, pErrMsg]));
  end
  else begin
    Writeln(Format('mw_read_file(%s) %s', [USER_RW_FILENAME, PAnsiChar(szReadBuffer)]));
  end;

  FreeMem(szReadBuffer);
end;
           
// 用户权限打开锁，数据加解密
function UserEncryptDecrypt(hHandle: MWHANDLE): LongWord;
var
  i: Integer;
  pErrMsg: PAnsiChar;

  szTestData: array[0..255] of Byte;
  uiTestDataLength: LongWord;

  szEncryptData: array[0..255] of Byte;
  uiEncryptDataLength: LongWord;
  
  szDecryptData: array[0..255] of Byte;
  uiDecryptDataLength: LongWord;
begin
  uiTestDataLength := Length(szTestData);
  // 初始化数据
  for i := 0 to uiTestDataLength - 1 do
  begin
    szTestData[i] := i;
  end;

  uiEncryptDataLength := Length(szEncryptData);
  // 数据加密 AES128 
  Result := mw_sym_encrypt(hHandle, PAnsiChar(AnsiString(USER_KEY_AES128_FILENAME)),
    MW_SYM_ALGO_MODE_ECB, nil, @szTestData, Length(szTestData),
    @szEncryptData, uiEncryptDataLength);
  if Result <> MW_SUCCESS then begin
    pErrMsg := mw_get_error_desc(Result, MW_LANGUAGE_ID_EN);
    Writeln(Format('mw_sym_encrypt error, errorcode = $%.8x, errormsg = %s', [Result, pErrMsg]));
  end
  else begin
    Writeln('mw_sym_encrypt success');
  end;

  uiDecryptDataLength := Length(szDecryptData);
  // 数据解密 AES128
  Result := mw_sym_decrypt(hHandle, PAnsiChar(AnsiString(USER_KEY_AES128_FILENAME)),
    MW_SYM_ALGO_MODE_ECB, nil, @szEncryptData, uiEncryptDataLength,
    @szDecryptData, uiDecryptDataLength);
  if Result <> MW_SUCCESS then begin
    pErrMsg := mw_get_error_desc(Result, MW_LANGUAGE_ID_EN);
    Writeln(Format('mw_sym_decrypt error, errorcode = $%.8x, errormsg = %s', [Result, pErrMsg]));
  end
  else begin
    Writeln('mw_sym_decrypt success');
  end;

  // 加解密结果对比
  if CompareMem(@szTestData, @szDecryptData, uiTestDataLength) then begin
    Writeln('szTestData = szDecryptData');
  end
  else begin
    Writeln('szTestData <> szDecryptData');
  end; 
end;

// 用户权限打开锁，数据加解密
function UserHMAC(hHandle: MWHANDLE): LongWord;
var
  i: Integer;
  pErrMsg: PAnsiChar;

  szTestData: array[0..255] of Byte;
  uiTestDataLength: LongWord;

  szEncryptData: array[0..255] of Byte;
  uiEncryptDataLength: LongWord;
  
  szDecryptData: array[0..255] of Byte;
  uiDecryptDataLength: LongWord;
begin
  uiTestDataLength := Length(szTestData);
  // 初始化数据
  for i := 0 to uiTestDataLength - 1 do
  begin
    szTestData[i] := i;
  end;

  uiEncryptDataLength := Length(szEncryptData);
  // 数据加密 AES128 
  Result := mw_sym_encrypt(hHandle, PAnsiChar(AnsiString(USER_KEY_AES128_FILENAME)),
    MW_SYM_ALGO_MODE_ECB, nil, @szTestData, Length(szTestData),
    @szEncryptData, uiEncryptDataLength);
  if Result <> MW_SUCCESS then begin
    pErrMsg := mw_get_error_desc(Result, MW_LANGUAGE_ID_EN);
    Writeln(Format('mw_sym_encrypt error, errorcode = $%.8x, errormsg = %s', [Result, pErrMsg]));
  end
  else begin
    Writeln('mw_sym_encrypt success');
  end;

  uiDecryptDataLength := Length(szDecryptData);
  // 数据解密 AES128
  Result := mw_sym_decrypt(hHandle, PAnsiChar(AnsiString(USER_KEY_AES128_FILENAME)),
    MW_SYM_ALGO_MODE_ECB, nil, @szEncryptData, uiEncryptDataLength,
    @szDecryptData, uiDecryptDataLength);
  if Result <> MW_SUCCESS then begin
    pErrMsg := mw_get_error_desc(Result, MW_LANGUAGE_ID_EN);
    Writeln(Format('mw_sym_decrypt error, errorcode = $%.8x, errormsg = %s', [Result, pErrMsg]));
  end
  else begin
    Writeln('mw_sym_decrypt success');
  end;

  // 加解密结果对比
  if CompareMem(@szTestData, @szDecryptData, uiTestDataLength) then begin
    Writeln('szTestData = szDecryptData');
  end
  else begin
    Writeln('szTestData <> szDecryptData');
  end; 
end;
           
// 设备文件，用户（权限）使用文件
function UserUseDevice(): LongWord;
var
  hHandle: MWHANDLE;
  szNewUserPIN: AnsiString;
label
  END_LAB;
begin
  //1.枚举锁
  //2.打开锁
  //3.验证用户PIN
  //4.修改用户PIN
  //5.读只读文件
  //6.修改可读写数据文件
  //7.使用密钥文件加解密（AES128）
  //8.使用密钥文件（HMAC）计算Hash值
  //9.关闭锁
  Result := OpenDevice(hHandle);
  if Result <> MW_SUCCESS then
  begin
    Goto END_LAB;
  end;

  DeviceResetPINVerifyStatus(hHandle);

  //3.验证用户PIN
  Result := mw_verify_pin(hHandle, Byte(MW_PIN_TYPE_USER), PByte(PINUser));
  if Result <> MW_SUCCESS then begin
    Writeln(Format('verify_pin user error, errorcode = $%.8x', [Result]));
    GoTo END_LAB;
  end
  else begin
    Writeln('verify_pin user success.');
  end;

  // 4.修改用户PIN
  // 使用默认PIN码无法使用其他应用功能
  szNewUserPIN := PINUser;    // 注意：本示例为了方便后续代码逻辑调用，此处将新旧PIN设置相同。开发者需修改为新PIN码
  Result := mw_change_pin(hHandle,MW_PIN_TYPE_USER,
    WORD(PIN_CHANGE_DEFAULT_TRY_COUNT),
    PByte(PINUser),
    PByte(szNewUserPIN));
  Writeln(Format('mw_change_Pin(user) new PIN = %s, errorcode = $%.8x', [szNewUserPIN, Result]));

  // 用户权限枚举文件（所有文件）
  EnumFile(hHandle);

  //5.读只读文件
  //6.修改可读写数据文件
  UserReadWriteFiles(hHandle);
  
  //7.使用密钥文件加解密（AES128）
  UserEncryptDecrypt(hHandle);

  //8.使用密钥文件（HMAC）计算Hash值
  UserHMAC(hHandle);
  
END_LAB:
  CloseDevice(hHandle);
end;

function DeveloperMakeUpdatePkg(): LongWord;
var
  hHandle: MWHANDLE;
  szNewUserPIN: AnsiString;
  pErrMsg: PAnsiChar;

  stDeviceAllItemInfo: MW_DEVICE_ALL_ITEM_INFO;
  stDeviceAllItemInfoSize: LongWord;

  stUpdateFileCtxArray: array[0..1] of MW_UPDADE_FILE_CTX;
  stUpdateFileCtxArraySize: LongWord;

  devPID: LongWord;
  devSN: PByte;

  updatePkgBuffer: array[0..4096] of Byte;
  updatePkgBufferSize: LongWord;

  mem: TMemoryStream;
label
  END_LAB;
begin
  //A、开发商制作升级包
  //1.枚举锁
  //2.打开锁
  //3.验证开发商PIN
  //4.制作升级包（升级二进制数据文件）
  //5.关闭锁

  //1.枚举锁
  //2.打开锁
  Result := OpenDevice(hHandle);
  if Result <> MW_SUCCESS then
  begin
    Goto END_LAB;
  end;

  // 3.验证开发商PIN（获取开发商权限）
  Result := mw_verify_pin(hHandle, Byte(MW_PIN_TYPE_DEVELOPER), PByte(PINDeveloper));
  if Result <> MW_SUCCESS then begin
    pErrMsg := mw_get_error_desc(Result, MW_LANGUAGE_ID_EN);
    Writeln(Format('verify_pin developer error, errorcode = $%.8x, errormsg = %s', [Result, pErrMsg]));
    GoTo END_LAB;
  end
  else begin
    Writeln('verify_pin developer success.');
  end;

  // 4.制作升级包（升级二进制数据文件）
  // 4.1 获取当前设备PID，PID可以通过其他途径获取。
  stDeviceAllItemInfoSize := SizeOf(stDeviceAllItemInfo);
  Result := mw_get_device_info(hHandle, MW_GET_INFO_ITEM_ALL, @stDeviceAllItemInfo, stDeviceAllItemInfoSize);
  if Result <> MW_SUCCESS then begin
    pErrMsg := mw_get_error_desc(Result, MW_LANGUAGE_ID_EN);
    Writeln(Format('mw_get_device_info(PID) developer error, errorcode = $%.8x, errormsg = %s', [Result, pErrMsg]));
    GoTo END_LAB;
  end
  else begin
    Writeln('mw_get_device_info(PID) developer success, PID = ', stDeviceAllItemInfo.uiPID);
  end;

  // 4.2 设置升级包更新锁内文件（删除后重建）
  // 更新锁内已有文件
  stUpdateFileCtxArraySize := Length(stUpdateFileCtxArray);
  stUpdateFileCtxArray[0].stFileProperty.ucType := MW_FILE_TYPE_BINARY;
	stUpdateFileCtxArray[0].stFileProperty.usPrivilege := MW_FILE_PRIV_TYPE_WRITE;
	stUpdateFileCtxArray[0].stFileProperty.uiSize := 128;
  StrCopy(stUpdateFileCtxArray[0].stFileProperty.acName, UPDATE_PKG_UPDATE_FILENAME);
  stUpdateFileCtxArray[0].pData := UPDATE_PKG_UPDATE_FILE_DATA;
  stUpdateFileCtxArray[0].uiDateSize := Length(stUpdateFileCtxArray[0].pData);

  // 创建新的锁内文件
  stUpdateFileCtxArray[1].stFileProperty.ucType := MW_FILE_TYPE_BINARY;
	stUpdateFileCtxArray[1].stFileProperty.usPrivilege := MW_FILE_PRIV_TYPE_WRITE;
	stUpdateFileCtxArray[1].stFileProperty.uiSize := 128;
  StrCopy(stUpdateFileCtxArray[1].stFileProperty.acName, UPDATE_PKG_CREATE_FILENAME);
  stUpdateFileCtxArray[1].pData := UPDATE_PKG_CREATE_FILE_DATA;
  stUpdateFileCtxArray[1].uiDateSize := Length(stUpdateFileCtxArray[1].pData);

  devPID := stDeviceAllItemInfo.uiPID;
  devSN := nil;
  updatePkgBufferSize := Length(updatePkgBuffer);
  Result := mw_make_update_pkg(hHandle, devPID, devSN, @stUpdateFileCtxArray,
    stUpdateFileCtxArraySize, @updatePkgBuffer, updatePkgBufferSize);
  if Result <> MW_SUCCESS then begin
    pErrMsg := mw_get_error_desc(Result, MW_LANGUAGE_ID_EN);
    Writeln(Format('mw_make_update_pkg developer error, errorcode = $%.8x, errormsg = %s', [Result, pErrMsg]));
    GoTo END_LAB;
  end
  else begin
    Writeln('mw_make_update_pkg developer success, PID = ', stDeviceAllItemInfo.uiPID);
  end;

  // 写入本地文件
  mem := TMemoryStream.Create;
  mem.WriteBuffer(updatePkgBuffer, updatePkgBufferSize);
  mem.SaveToFile(UPDATE_PKG_FILENAME);
  mem.Clear;

END_LAB:
  //5.关闭锁
  CloseDevice(hHandle);
end;

function UserUpdatePkg(): LongWord;
var
  hHandle: MWHANDLE;
  szNewUserPIN: AnsiString;
  pErrMsg: PAnsiChar;

  mem: TMemoryStream;

  buffer: array[0..256] of Byte;
  bufferSize: LongWord; 
label
  END_LAB;
begin
  //B、用户导入升级包
  //1.枚举锁
  //2.打开锁
  //3.验证用户PIN
  //4.导入升级包
  //5.验证升级包内容（读取验证内容与升级内容一致）
  //6.关闭锁

  //1.枚举锁
  //2.打开锁
  Result := OpenDevice(hHandle);
  if Result <> MW_SUCCESS then
  begin
    Goto END_LAB;
  end;

  DeviceResetPINVerifyStatus(hHandle);

  //3.验证用户PIN
  Result := mw_verify_pin(hHandle, Byte(MW_PIN_TYPE_USER), PByte(PINUser));
  if Result <> MW_SUCCESS then begin
    Writeln(Format('verify_pin user error, errorcode = $%.8x', [Result]));
    GoTo END_LAB;
  end
  else begin
    Writeln('verify_pin user success.');
  end;

  //4.导入升级包
  // 4.1 读取升级包（本地文件内容）
  mem := TMemoryStream.Create;
  mem.LoadFromFile(UPDATE_PKG_FILENAME);
  Result := mw_update(hHandle, mem.Memory, mem.Size);
  mem.Clear;
  if Result <> MW_SUCCESS then begin
    Writeln(Format('mw_update user error, errorcode = $%.8x', [Result]));
    GoTo END_LAB;
  end
  else begin
    Writeln('mw_update user success.');
  end;
  
  //5.验证升级包内容（读取验证内容与升级内容一致）
  // 5.1 读取升级包更新文件内容
  bufferSize := Length(UPDATE_PKG_UPDATE_FILE_DATA);
  Result := mw_read_file(hHandle, PAnsiChar(AnsiString(UPDATE_PKG_UPDATE_FILENAME)), 0, bufferSize, @buffer);
  if Result <> MW_SUCCESS then begin
    pErrMsg := mw_get_error_desc(Result, MW_LANGUAGE_ID_EN);
    Writeln(Format('mw_read_file(%s) error, errorcode = $%.8x, errormsg = %s', [UPDATE_PKG_UPDATE_FILENAME, Result, pErrMsg]));
  end
  else begin
    Writeln(Format('mw_read_file(%s) %s', [UPDATE_PKG_UPDATE_FILENAME, PAnsiChar(@buffer)]));
  end;

  // 5.2 读取升级包新建文件内容
  bufferSize := Length(UPDATE_PKG_CREATE_FILE_DATA);
  Result := mw_read_file(hHandle, PAnsiChar(AnsiString(UPDATE_PKG_CREATE_FILENAME)), 0, bufferSize, @buffer);
  if Result <> MW_SUCCESS then begin
    pErrMsg := mw_get_error_desc(Result, MW_LANGUAGE_ID_EN);
    Writeln(Format('mw_read_file(%s) error, errorcode = $%.8x, errormsg = %s', [UPDATE_PKG_CREATE_FILENAME, Result, pErrMsg]));
  end
  else begin
    Writeln(Format('mw_read_file(%s) %s', [UPDATE_PKG_CREATE_FILENAME, PAnsiChar(@buffer)]));
  end;
END_LAB:
  //6.关闭锁
  CloseDevice(hHandle);
end;



begin
  try
    { TODO -oUser -cConsole Main : Insert code here }
    Writeln('------------ 魔锐 SDK 功能演示 ------------');

     //打开加密锁（基本）示例
    HelloMoway();

    // 1.开发商加密锁设置示例（设置PID，修改默认PIN）。
    // 注意：必须在使用前初始化设置。
    DeveloperInitDevice();

    // 2.设备信息获取-无权限要求
    GetDeviceInfo();

    // 3. 用户使用方案（读写、加解密）
    UserUseDevice();

    // 4.开发商（升级包）
    // A、开发商制作升级包
    DeveloperMakeUpdatePkg();
    // B、用户导入升级包
    UserUpdatePkg();

    Writeln('Press any key exit.');
    Readln;
  except
    on E: Exception do
      Writeln(E.ClassName, ': ', E.Message);
  end;
end.
