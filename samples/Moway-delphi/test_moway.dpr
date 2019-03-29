program test_moway;

{$APPTYPE CONSOLE}

{$R *.res}


uses
  {$IF CompilerVersion >= 23}
    // XE2 �����ϰ汾����ͷ�ļ�
    Winapi.Windows, System.SysUtils, System.Classes,
  {$ELSE}
    // XE���°汾������Delphi7����ͷ�ļ������������ռ�
    windows, SysUtils, Classes,
  {$IFEND}
  moway_api;


const
  uiMaxDevCount = 32;         // ö�������������
  uiFileListMaxCount = 50;    // ��������ļ���
  PIN_CHANGE_DEFAULT_TRY_COUNT = 0; // PIN���޸�Ĭ���޸Ĵ�����������Ϊ0ʱ��ʹ��Ĭ��ֵ15��


const
  USER_RW_FILENAME = 'userwt.evd';  // ��д�ļ�
  USER_RD_FILENAME = 'userrd.evd';  // ֻ���ļ�

  USER_KEY_AES128_FILENAME = 'aes_128.sls';     // key�ļ�
  USER_KEY_HMAC_FILENAME = 'hmac_md5.sls';     // key�ļ�

  UPDATE_PKG_UPDATE_FILENAME = 'userrd.evd';
  UPDATE_PKG_CREATE_FILENAME = 'userrwnew.evc';
  UPDATE_PKG_FILENAME = 'MowayUpdatePkg.dat';

  UPDATE_PKG_UPDATE_FILE_DATA = 'update pkg update rw file.';
  UPDATE_PKG_CREATE_FILE_DATA = 'update pkg create new rw file.';


var
  // ������PIN�������ã�
  // ������δ����Ĭ�� PIN: 000000000000000000000000
  //     ������ʹ��ǰ����ͨ���޸� PIN �ӿ��޸�PIN�������޷�ִ���ļ������ȹ��ܽӿڣ������ DeveloperInitDevice() ʾ��
  // ע�⣺��ǰʾ��ʹ�������ú��PIN�룬�����߳�ʼ������ʱ��Ҫ�޸�ΪĬ��PIN��
  PINDeveloper: AnsiString = '000000000000000000000000';
  // �û�PIN������Ĭ��PIN��
  PINUser: AnsiString = '00000000';
  TestData: AnsiString = 'Hello Moway!';


// ö�ٴ򿪼�������ʾ����
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
  // ö����
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

  // ����
  Result := mw_open(pDevInfoList, MW_OPEN_EXCLUSIVE_MODE, hHandle);
	if Result <> MW_SUCCESS then begin
    Writeln('open moway device error, errorcode = ', Result);
    GoTo END_LAB;
  end
  else begin
    Writeln('open moway device success.');
  end;

  //��֤������PIN
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
    //�ر���
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

// ö�ٴ򿪼�����
function OpenDevice(var hHandle: MWHANDLE): LongWord;
var
  puiDevCount: LongWord;
  pDevInfoList: PMW_DEVICE_INFO_CTX;
  pErrMsg: PAnsiChar;
label END_LAB;
begin
  Result := MW_SUCCESS;
  
  pDevInfoList := GetMemory(uiMaxDevCount * SizeOf(MW_DEVICE_INFO_CTX));
  // ö����
  Result := mw_enum(pDevInfoList, uiMaxDevCount, puiDevCount);
  if Result <> MW_SUCCESS then begin
    Writeln('mw_enum failed, Result = ', Result);
    GoTo END_LAB;
  end;
  if puiDevCount = 0 then begin
    Writeln('Not found moway lock.');
    GoTo END_LAB;
  end;

  // ����
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

// �رռ�����
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

// ö�ٵ�ǰ�����ļ�
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
  //ö�������ļ�
  pstFilePropertyList := GetMemory(SizeOf(MW_FILE_PROPERTY) * uiFileListMaxCount);
  FillMemory(pstFilePropertyList, SizeOf(MW_FILE_PROPERTY) * uiFileListMaxCount, 0);  // ��ʼ���ڴ滺����
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

// ���Ȩ����֤״̬
function DeviceResetPINVerifyStatus(hHandle: MWHANDLE): LongWord;
var
  uiReturnedLen: LongWord;
begin
  // �ɹ���֤��һ�κ��ڲ��γ�������������£����ýӿ� mw_close() ���������֤״̬��
  // TDOO ��������ں����汾��������
  Result := mw_control(hHandle, MW_CTRL_CODE_ITEM_RESET, nil, 0, nil, 0, uiReturnedLen);
  Writeln(Format('mw_control(MW_CTRL_CODE_ITEM_RESET) errorcode = $%.8x', [Result]));
end;
                      
// ������Ȩ�޲����ļ���ʾ����
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
  // ö����
  Result := mw_enum(pDevInfoList, uiMaxDevCount, puiDevCount);
  if Result <> MW_SUCCESS then begin
    Writeln('mw_enum failed, Result = ', Result);
    GoTo END_LAB;
  end;
  if puiDevCount = 0 then begin
    Writeln('Not found moway lock.');
    GoTo END_LAB;
  end;

  // ����
  Result := mw_open(pDevInfoList, MW_OPEN_EXCLUSIVE_MODE, hHandle);
	if Result <> MW_SUCCESS then begin
    Writeln('open moway device error, errorcode = ', Result);
    GoTo END_LAB;
  end
  else begin
    Writeln('open moway device success.');
  end;

  //��֤������PIN
  Result := mw_verify_pin(hHandle, Byte(MW_PIN_TYPE_DEVELOPER), PByte(PINDeveloper));
  if Result <> MW_SUCCESS then begin
    pErrMsg := mw_get_error_desc(Result, MW_LANGUAGE_ID_EN);
    Writeln(Format('verify_pin developer error, errorcode = $%.8x, errormsg = %s', [Result, pErrMsg]));
    GoTo END_LAB;
  end
  else begin
    Writeln('verify_pin developer success.');
  end;

  //ö�������ļ�
  EnumFile(hHandle);

  // ������Ȩ�ޣ������������û��ɶ�д�ļ�
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

  {�ļ���д����}
  // �ļ���д
	Result := mw_write_file(hHandle, PAnsiChar(AnsiString(USER_RW_FILENAME)), 0, 3, PByte(PAnsiChar(AnsiString('abc'))));
  if Result <> MW_SUCCESS then begin
    pErrMsg := mw_get_error_desc(Result, MW_LANGUAGE_ID_EN);
    Writeln(Format('mw_write_file(%s) error, errorcode = $%.8x, errormsg = %s', [USER_RW_FILENAME, Result, pErrMsg]));
  end
  else begin
    Writeln(Format('mw_write_file(%s) success', [USER_RW_FILENAME]));
  end;

  // ��ȡ�ļ�����
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

  {�ļ�ɾ������}
  // ɾ�����������û��ɶ�д�ļ�
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
    //�ر���
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


// ��ȡ�豸��Ϣ����Ȩ��Ҫ��
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
  //1.ö����
  //2.����
  //3.��ȡ������Ϣ���ṹ�壩
  //4.��ȡָ����Ϣ��Item��
  //5.�ر���
  Result := OpenDevice(hHandle);
  if Result <> MW_SUCCESS then
  begin
    Goto END_LAB;
  end;

  bLEDOpen := 1;
  bLEDClose := 0;

  //�ر�LED�������Ĭ�����ƣ�
	Result := mw_control(hHandle,MW_CTRL_CODE_ITEM_LED,@bLEDOpen,1,nil,0,uiReturnedLen);
  Writeln(Format('mw_control(MW_CTRL_CODE_ITEM_LED) errorcode = $%.8x', [Result]));

  //��LED
	Result := mw_control(hHandle,MW_CTRL_CODE_ITEM_LED,@bLEDClose,1,nil,0,uiReturnedLen);
  Writeln(Format('mw_control(MW_CTRL_CODE_ITEM_LED) errorcode = $%.8x', [Result]));

  // ��ȡ�豸״̬
  FillMemory(@stDeviceStatusCtx, SizeOf(stDeviceStatusCtx), 0);
	Result := mw_get_device_status(hHandle, @stDeviceStatusCtx);
  Writeln(Format('mw_get_device_status errorcode = $%.8x', [Result]));

  // 3.��ȡ�����豸��Ϣ
  stDeviceAllitemInfoSize := SizeOf(stDeviceAllitemInfo);
  Result := mw_get_device_info(hHandle,MW_GET_INFO_ITEM_ALL, Pointer(@stDeviceAllitemInfo), stDeviceAllitemInfoSize);
  if Result <> MW_SUCCESS then begin
    Writeln(Format('mw_delete_file(MW_FILE_PRIV_TYPE_WRITE) error, errorcode = $%.8x', [Result]));
  end
  else begin
    Writeln('mw_get_device_info(MW_GET_INFO_ITEM_ALL) success, PID=', stDeviceAllitemInfo.uiPID);
  end;

  uiBufferLength := Length(pvBuffer);

  //4.��ȡָ����Ϣ��Item��
  // 4.1 ��ȡ�豸pid����Ҫʹ�ù���ԱȨ������PID�������޷�ʹ���ļ�����Կ�ȹ��ܣ�
	Result := mw_get_device_info(hHandle,MW_GET_INFO_ITEM_PID,@pvBuffer,uiBufferLength);
  Writeln(Format('mw_get_device_info(MW_GET_INFO_ITEM_PID) errorcode = $%.8x', [Result]));

  // 4.2 ��ȡ�豸SN
	uiBufferLength := BufferLength;
  FillMemory(@pvBuffer, uiBufferLength, 0);
	Result := mw_get_device_info(hHandle,MW_GET_INFO_ITEM_SN,@pvBuffer,uiBufferLength);
  Writeln(Format('mw_get_device_info(MW_GET_INFO_ITEM_SN) errorcode = $%.8x', [Result]));

END_LAB:
  CloseDevice(hHandle);
end;

// �豸�ļ��������̣�Ȩ�ޣ������ļ�
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
  // 1.������Ȩ�ޣ������������ƣ��û�ֻ���ļ�
  // �û�Ȩ�ޣ�ֻ�ܶ�ȡ�ļ���������Ȩ�޿��Խ��ж�д������
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
  // ������Ȩ����ֻ���ļ�д������
	Result := mw_write_file(hHandle, PAnsiChar(AnsiString(USER_RD_FILENAME)), 0, Length(TestData), PByte(PAnsiChar(TestData)));
  if Result <> MW_SUCCESS then begin
    pErrMsg := mw_get_error_desc(Result, MW_LANGUAGE_ID_EN);
    Writeln(Format('mw_write_file(%s) error, errorcode = $%.8x, errormsg = %s', [USER_RD_FILENAME, Result, pErrMsg]));
  end
  else begin
    Writeln(Format('mw_write_file(%s) success', [USER_RD_FILENAME]));
  end;

  // 2.������Ȩ�ޣ������������ƣ��û��ɶ�д�ļ�
  // �û�Ȩ�ޣ��ɶ�д�ļ��������ܳ��������ļ���С��
  // 2.1 ���������ļ�
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
  // 2.2 ������Ȩ�����û��ɶ�д�ļ�д������
	Result := mw_write_file(hHandle, PAnsiChar(AnsiString(USER_RW_FILENAME)), 0, Length(TestData), PByte(PAnsiChar(TestData)));
  if Result <> MW_SUCCESS then begin
    pErrMsg := mw_get_error_desc(Result, MW_LANGUAGE_ID_EN);
    Writeln(Format('mw_write_file(%s) error, errorcode = $%.8x, errormsg = %s', [USER_RW_FILENAME, Result, pErrMsg]));
  end
  else begin
    Writeln(Format('mw_write_file(%s) success', [USER_RW_FILENAME]));
  end;

  // 3.������Ȩ�ޣ�����AES128��Կ�ļ�
  // �û�Ȩ�ޣ����Ե�������ѧ�㷨ʹ����Կ�ļ�
  // 3.1 ���������ļ�
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
  // 3.2 д����Կ���ݡ���Կ���ݿ�ͨ������ѧ�������ɡ�
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

  // 4.������Ȩ�ޣ�����HMAC��Կ�ļ�
  // �û�Ȩ�ޣ����Ե�������ѧ�㷨ʹ����Կ�ļ�
  // 4.1 ���������ļ�
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
  // 4.2 д����Կ���ݡ�
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


// ��ʼ��������
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
  //1.ö����
  //2.����
  //3.��֤������PIN
  //4.����PID
  //5.�޸Ŀ�����PIN
  //6.�����ļ��������ơ���д����Կ�ļ�-AES128����Կ�ļ�-HMAC��
  //7.������Ȩ��д�ļ�
  //8.�ر���
  Result := OpenDevice(hHandle);
  if Result <> MW_SUCCESS then
  begin
    Goto END_LAB;
  end;

  // 3.��֤������PIN����ȡ������Ȩ�ޣ�
  Result := mw_verify_pin(hHandle, Byte(MW_PIN_TYPE_DEVELOPER), PByte(PINDeveloper));
  if Result <> MW_SUCCESS then begin
    pErrMsg := mw_get_error_desc(Result, MW_LANGUAGE_ID_EN);
    Writeln(Format('verify_pin developer error, errorcode = $%.8x, errormsg = %s', [Result, pErrMsg]));
    GoTo END_LAB;
  end
  else begin
    Writeln('verify_pin developer success.');
  end;

  // 4.����PID
  // ����PID���ӣ��������ʹ���������������ø����Ҳ������������ظ������ݣ������������صĽ��������
  // �����ͬ������֮�����PID������ͬ��������������PID��ͬ��PID��ͬʱ�ӿǹ��߼��ܺ������ܹ�����ʹ�á�
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
  //��ȡ�豸pid����Ҫʹ�ù���ԱȨ������PID�������޷�ʹ���ļ�����Կ�ȹ��ܣ�
	Result := mw_get_device_info(hHandle,MW_GET_INFO_ITEM_PID, @buffer, uiBufferLength);
  uiPID := @buffer; 
  Writeln(Format('mw_get_device_info(MW_GET_INFO_ITEM_PID) PID = %d errorcode = $%.8x', [uiPID^, Result]));

  // 5.�޸Ŀ�����PIN
  // ʹ��Ĭ��PIN���޷�ʹ������Ӧ�ù���
  szNewDeveloperPIN := PINDeveloper;    // ע�⣺��ʾ��Ϊ�˷�����������߼����ã��˴����¾�PIN������ͬ�����������޸�Ϊ��PIN��
  Result := mw_change_pin(hHandle,MW_PIN_TYPE_DEVELOPER,
    WORD(PIN_CHANGE_DEFAULT_TRY_COUNT),
    PByte(PINDeveloper),
    PByte(szNewDeveloperPIN));
  Writeln(Format('mw_change_Pin new PIN = %s, errorcode = $%.8x', [szNewDeveloperPIN, Result]));

  //6.�����ļ��������ơ���д����Կ�ļ�-AES128����Կ�ļ�-HMAC��
  //7.������Ȩ��д�ļ�
  DeveloperCreateFiles(hHandle);

END_LAB:
  CloseDevice(hHandle);
end;


// �û�Ȩ�޴�������д�ļ�
function UserReadWriteFiles(hHandle: MWHANDLE): LongWord;
var
  pErrMsg: PAnsiChar;
  szReadBuffer: PByte;
  uiReadBufferLen: LongWord;
begin
  uiReadBufferLen := 128;

  // ��ȡֻ���ļ�����
  szReadBuffer := GetMemory(uiReadBufferLen);
  Result := mw_read_file(hHandle, PAnsiChar(AnsiString(USER_RD_FILENAME)), 0, Length(TestData), szReadBuffer);
  if Result <> MW_SUCCESS then begin
    pErrMsg := mw_get_error_desc(Result, MW_LANGUAGE_ID_EN);
    Writeln(Format('mw_read_file(%s) error, errorcode = $%.8x, errormsg = %s', [USER_RD_FILENAME, Result, pErrMsg]));
  end
  else begin
    Writeln(Format('mw_read_file(%s) %s', [USER_RD_FILENAME, PAnsiChar(szReadBuffer)]));
  end;

  // д��ɶ�д�ļ�
	Result := mw_write_file(hHandle, PAnsiChar(AnsiString(USER_RW_FILENAME)), 0, 3, PByte(PAnsiChar(AnsiString('abc'))));
  if Result <> MW_SUCCESS then begin
    pErrMsg := mw_get_error_desc(Result, MW_LANGUAGE_ID_EN);
    Writeln(Format('mw_write_file(%s) error, errorcode = $%.8x, errormsg = %s', [USER_RW_FILENAME, Result, pErrMsg]));
  end
  else begin
    Writeln(Format('mw_write_file(%s) success', [USER_RW_FILENAME]));
  end;
  // ��ȡ�ɶ�д�ļ�����
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
           
// �û�Ȩ�޴��������ݼӽ���
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
  // ��ʼ������
  for i := 0 to uiTestDataLength - 1 do
  begin
    szTestData[i] := i;
  end;

  uiEncryptDataLength := Length(szEncryptData);
  // ���ݼ��� AES128 
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
  // ���ݽ��� AES128
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

  // �ӽ��ܽ���Ա�
  if CompareMem(@szTestData, @szDecryptData, uiTestDataLength) then begin
    Writeln('szTestData = szDecryptData');
  end
  else begin
    Writeln('szTestData <> szDecryptData');
  end; 
end;

// �û�Ȩ�޴��������ݼӽ���
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
  // ��ʼ������
  for i := 0 to uiTestDataLength - 1 do
  begin
    szTestData[i] := i;
  end;

  uiEncryptDataLength := Length(szEncryptData);
  // ���ݼ��� AES128 
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
  // ���ݽ��� AES128
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

  // �ӽ��ܽ���Ա�
  if CompareMem(@szTestData, @szDecryptData, uiTestDataLength) then begin
    Writeln('szTestData = szDecryptData');
  end
  else begin
    Writeln('szTestData <> szDecryptData');
  end; 
end;
           
// �豸�ļ����û���Ȩ�ޣ�ʹ���ļ�
function UserUseDevice(): LongWord;
var
  hHandle: MWHANDLE;
  szNewUserPIN: AnsiString;
label
  END_LAB;
begin
  //1.ö����
  //2.����
  //3.��֤�û�PIN
  //4.�޸��û�PIN
  //5.��ֻ���ļ�
  //6.�޸Ŀɶ�д�����ļ�
  //7.ʹ����Կ�ļ��ӽ��ܣ�AES128��
  //8.ʹ����Կ�ļ���HMAC������Hashֵ
  //9.�ر���
  Result := OpenDevice(hHandle);
  if Result <> MW_SUCCESS then
  begin
    Goto END_LAB;
  end;

  DeviceResetPINVerifyStatus(hHandle);

  //3.��֤�û�PIN
  Result := mw_verify_pin(hHandle, Byte(MW_PIN_TYPE_USER), PByte(PINUser));
  if Result <> MW_SUCCESS then begin
    Writeln(Format('verify_pin user error, errorcode = $%.8x', [Result]));
    GoTo END_LAB;
  end
  else begin
    Writeln('verify_pin user success.');
  end;

  // 4.�޸��û�PIN
  // ʹ��Ĭ��PIN���޷�ʹ������Ӧ�ù���
  szNewUserPIN := PINUser;    // ע�⣺��ʾ��Ϊ�˷�����������߼����ã��˴����¾�PIN������ͬ�����������޸�Ϊ��PIN��
  Result := mw_change_pin(hHandle,MW_PIN_TYPE_USER,
    WORD(PIN_CHANGE_DEFAULT_TRY_COUNT),
    PByte(PINUser),
    PByte(szNewUserPIN));
  Writeln(Format('mw_change_Pin(user) new PIN = %s, errorcode = $%.8x', [szNewUserPIN, Result]));

  // �û�Ȩ��ö���ļ��������ļ���
  EnumFile(hHandle);

  //5.��ֻ���ļ�
  //6.�޸Ŀɶ�д�����ļ�
  UserReadWriteFiles(hHandle);
  
  //7.ʹ����Կ�ļ��ӽ��ܣ�AES128��
  UserEncryptDecrypt(hHandle);

  //8.ʹ����Կ�ļ���HMAC������Hashֵ
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
  //A������������������
  //1.ö����
  //2.����
  //3.��֤������PIN
  //4.���������������������������ļ���
  //5.�ر���

  //1.ö����
  //2.����
  Result := OpenDevice(hHandle);
  if Result <> MW_SUCCESS then
  begin
    Goto END_LAB;
  end;

  // 3.��֤������PIN����ȡ������Ȩ�ޣ�
  Result := mw_verify_pin(hHandle, Byte(MW_PIN_TYPE_DEVELOPER), PByte(PINDeveloper));
  if Result <> MW_SUCCESS then begin
    pErrMsg := mw_get_error_desc(Result, MW_LANGUAGE_ID_EN);
    Writeln(Format('verify_pin developer error, errorcode = $%.8x, errormsg = %s', [Result, pErrMsg]));
    GoTo END_LAB;
  end
  else begin
    Writeln('verify_pin developer success.');
  end;

  // 4.���������������������������ļ���
  // 4.1 ��ȡ��ǰ�豸PID��PID����ͨ������;����ȡ��
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

  // 4.2 �������������������ļ���ɾ�����ؽ���
  // �������������ļ�
  stUpdateFileCtxArraySize := Length(stUpdateFileCtxArray);
  stUpdateFileCtxArray[0].stFileProperty.ucType := MW_FILE_TYPE_BINARY;
	stUpdateFileCtxArray[0].stFileProperty.usPrivilege := MW_FILE_PRIV_TYPE_WRITE;
	stUpdateFileCtxArray[0].stFileProperty.uiSize := 128;
  StrCopy(stUpdateFileCtxArray[0].stFileProperty.acName, UPDATE_PKG_UPDATE_FILENAME);
  stUpdateFileCtxArray[0].pData := UPDATE_PKG_UPDATE_FILE_DATA;
  stUpdateFileCtxArray[0].uiDateSize := Length(stUpdateFileCtxArray[0].pData);

  // �����µ������ļ�
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

  // д�뱾���ļ�
  mem := TMemoryStream.Create;
  mem.WriteBuffer(updatePkgBuffer, updatePkgBufferSize);
  mem.SaveToFile(UPDATE_PKG_FILENAME);
  mem.Clear;

END_LAB:
  //5.�ر���
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
  //B���û�����������
  //1.ö����
  //2.����
  //3.��֤�û�PIN
  //4.����������
  //5.��֤���������ݣ���ȡ��֤��������������һ�£�
  //6.�ر���

  //1.ö����
  //2.����
  Result := OpenDevice(hHandle);
  if Result <> MW_SUCCESS then
  begin
    Goto END_LAB;
  end;

  DeviceResetPINVerifyStatus(hHandle);

  //3.��֤�û�PIN
  Result := mw_verify_pin(hHandle, Byte(MW_PIN_TYPE_USER), PByte(PINUser));
  if Result <> MW_SUCCESS then begin
    Writeln(Format('verify_pin user error, errorcode = $%.8x', [Result]));
    GoTo END_LAB;
  end
  else begin
    Writeln('verify_pin user success.');
  end;

  //4.����������
  // 4.1 ��ȡ�������������ļ����ݣ�
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
  
  //5.��֤���������ݣ���ȡ��֤��������������һ�£�
  // 5.1 ��ȡ�����������ļ�����
  bufferSize := Length(UPDATE_PKG_UPDATE_FILE_DATA);
  Result := mw_read_file(hHandle, PAnsiChar(AnsiString(UPDATE_PKG_UPDATE_FILENAME)), 0, bufferSize, @buffer);
  if Result <> MW_SUCCESS then begin
    pErrMsg := mw_get_error_desc(Result, MW_LANGUAGE_ID_EN);
    Writeln(Format('mw_read_file(%s) error, errorcode = $%.8x, errormsg = %s', [UPDATE_PKG_UPDATE_FILENAME, Result, pErrMsg]));
  end
  else begin
    Writeln(Format('mw_read_file(%s) %s', [UPDATE_PKG_UPDATE_FILENAME, PAnsiChar(@buffer)]));
  end;

  // 5.2 ��ȡ�������½��ļ�����
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
  //6.�ر���
  CloseDevice(hHandle);
end;



begin
  try
    { TODO -oUser -cConsole Main : Insert code here }
    Writeln('------------ ħ�� SDK ������ʾ ------------');

     //�򿪼�������������ʾ��
    HelloMoway();

    // 1.�����̼���������ʾ��������PID���޸�Ĭ��PIN����
    // ע�⣺������ʹ��ǰ��ʼ�����á�
    DeveloperInitDevice();

    // 2.�豸��Ϣ��ȡ-��Ȩ��Ҫ��
    GetDeviceInfo();

    // 3. �û�ʹ�÷�������д���ӽ��ܣ�
    UserUseDevice();

    // 4.�����̣���������
    // A������������������
    DeveloperMakeUpdatePkg();
    // B���û�����������
    UserUpdatePkg();

    Writeln('Press any key exit.');
    Readln;
  except
    on E: Exception do
      Writeln(E.ClassName, ': ', E.Message);
  end;
end.
