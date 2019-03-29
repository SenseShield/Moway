unit moway_api;

{$IFDEF CompilerVersion > 14.5}
  // Delphi 6Ĭ��δ����FPC����ʹ�ð汾�����֣��򻯱��������� shijc 2016-8-8
  // ��ע��ԭ���ް汾�ж�
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
  // XE2 �����ϰ汾����ͷ�ļ�
  Winapi.Windows, System.SysUtils, System.Classes,
  BTMemory_x86_64;
{$ELSE}
  // XE���°汾������Delphi7��
  windows, SysUtils, Classes,
  BTMemory_x86;
{$IFEND}

{$I moway_api.inc}

var
{/*!
*   @brief         ö��ħ���豸
*   @param[in out] pDevInfoList        ö�������ħ���豸�б�
*   @param[in]     uiDevListCount      ö���б�(pDevInfoList)�ܴ�ŵ��豸�ĸ�����ע���������μ��궨��MW_ENUM_DEVICE_MAX_COUNT
*   @param[out]    puiDevCount         ʵ��ö�ٳ���ħ���豸����
*   @return        ִ�гɹ����� MW_SUCCESS ��ִ��ʧ��ʱ���ش����롣
*   @remarks       ע�⣺�û����밴��uiDevListCount����sizeof(MW_DEVICE_INFO_CTX)�����ڴ��pDevInfoList��API�ڲ��������ڴ��pDevInfoList��
*/}
mw_enum: function (
{OUT} pDevInfoList: PMW_DEVICE_INFO_CTX;
{IN} uiDevListCount: LONGWORD;
{OUT} var puiDevCount: LONGWORD
): LONGWORD; stdcall;

{/*!
*   @brief      ��ħ���豸
*   @param[in]  pDevInfo             ָ��ħ���豸��Ϣ�ṹ���ָ�롣
*   @param[in]  uiShareMode          ��ռ���߹���ģʽ�򿪡��μ��궨��MW_OPEN_SHARE_MODE  MW_OPEN_EXCLUSIVE_MODE
*   @param[out] phHandle             ��ħ���豸�����һ������Ա�������á�
*   @return     ִ�гɹ����� MW_SUCCESS ��ִ��ʧ��ʱ���ش����롣
*   @remarks    Open an moway device with DevInfo enumerated by mw_enum()
*/}
mw_open: function (
{IN} pDevInfo: PMW_DEVICE_INFO_CTX;
{IN} uiShareMode: LONGWORD;
{OUT} var phHandle: MWHANDLE
): LONGWORD; stdcall;
                                                                           
{/*!
*   @brief      �ر��豸
*   @param[in]  hHandle           ħ���豸���
*   @return     ִ�гɹ����� MW_SUCCESS ��ִ��ʧ��ʱ���ش����롣
*   @remarks    
*/}
mw_close: function (
{IN} hHandle: MWHANDLE
): LONGWORD; stdcall;

{/*!
*   @brief      У��PIN
*   @param[in]  hHandle             ħ���豸���
*   @param[in]  ucPinType           Pin������  �μ��궨�壺MW_PIN_TYPE_XXX
*   @param[in]  pucPin              Pin�����ݣ��û�8�ֽڣ�������24�ֽ�
*   @return     ִ�гɹ����� MW_SUCCESS ��ִ��ʧ��ʱ���ش����롣
*   @remarks    
*/}
mw_verify_pin: function (
{IN} hHandle: MWHANDLE;
{IN} ucPinType: BYTE;
{IN} pucPin: PBYTE
): LONGWORD; stdcall;

{/*!
*   @brief      �޸�PIN
*   @param[in]  hHandle             ħ���豸���
*   @param[in]  ucPinType           Pin������  �μ��궨�壺MW_PIN_TYPE_XXX
*   @param[in]  usLimitCount        ����Դ�����ע�����޴�������Ϊ0�����ƴ�����Χ��1-15�� ��Ϊ�������򷵻ز�������
*   @param[in]  pucOldPin           ��Pin�����ݣ��û�8�ֽڣ�������24�ֽ�
*   @param[in]  pucNewPin           ��Pin�����ݣ��û�8�ֽڣ�������24�ֽ�
*   @return     ִ�гɹ����� MW_SUCCESS ��ִ��ʧ��ʱ���ش����롣
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
*   @brief      ����PID
*   @param[in]  hHandle             ħ���豸���
*   @param[in]  pucPIDSeed          ������
*   @param[in]  uiSeedLen           �����볤�ȣ����ȷ�Χ��4-32���μ��궨��MW_SEED_LENGTH_MIN��MW_SEED_LENGTH_MAX
*   @return     ִ�гɹ����� MW_SUCCESS ��ִ��ʧ��ʱ���ش����롣
*   @remarks    
*/}
mw_set_pid: function (
{IN} hHandle: MWHANDLE;
{IN} pucPIDSeed: PBYTE;
{IN} uiSeedLen: LONGWORD
): LONGWORD; stdcall;
                              
{/*!
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
*   @brief      ��ȡ�豸��Ϣ
*   @param[in]  hHandle           ħ���豸���
*   @param[in]  ucInfoItem        ָ����ȡ���豸��Ϣ�ľ�����μ��궨�壺MW_GET_INFO_ITEM_XXX 
*   @param[out] pvBuffer          �����ȡ���豸��Ϣ��
*   @param[in out] puiBufferLength   ����豸��Ϣ������ݳ���
*   @return     ִ�гɹ����� MW_SUCCESS ��ִ��ʧ��ʱ���ش����롣
*   @remarks    
*/}
mw_get_device_info: function (
{IN} hHandle: MWHANDLE;
{IN} ucInfoItem: BYTE;
{OUT} pvBuffer: Pointer;
{INOUT} var puiBufferLength: LONGWORD
): LONGWORD; stdcall;
         
{/*!
*   @brief      ��ȡ�豸״̬
*   @param[in]  hHandle                   ħ���豸���
*   @param[out] pstDeviceStatusCtx        ��ǰ�豸״̬��
*   @return     ִ�гɹ����� MW_SUCCESS ��ִ��ʧ��ʱ���ش����롣
*   @remarks    
*/}                                        
mw_get_device_status: function (
{IN} hHandle: MWHANDLE;
{OUT} pstDeviceStatusCtx: PMW_DEVICE_STATUS
): LONGWORD; stdcall;
                                        
{/*!
*   @brief      �о��ļ�
*   @param[in]  hHandle              ħ���豸���
*   @param[out] pstFilePropertyList  �ṹ��MW_FILE_PROPERTY���飬���Ĵ�С��uiFileListCountָ��
*   @param[in]  uiFileListCount      ָ��pstFilePropertyList����Ĵ�С
*   @param[out] puiReturnedFileCount ʵ�ʷ����оٵ��ļ��ĸ���
*   @return     ִ�гɹ����� MW_SUCCESS ��ִ��ʧ��ʱ���ش����롣
*   @remarks    ע�⣺pstFilePropertyList�ɺ����ⲿ�����߷��䣬��СΪuiFileListCount * sizeof(MW_FILE_PROPERTY)
*/}
mw_enum_file: function (
{IN} hHandle: MWHANDLE;
{OUT} pstFilePropertyList: PMW_FILE_PROPERTY;
{IN} uiFileListCount: LONGWORD;
{OUT} var puiReturnedFileCount: LONGWORD
): LONGWORD; stdcall;

{/*!
*   @brief      �����ļ�
*   @param[in]  hHandle           ħ���豸���
*   @param[in]  pstFileProperty   ָ����Ҫ�����ļ������ԣ������ļ����͡��ļ�Ȩ�ޡ��ļ���СΪ������
*   @return     ִ�гɹ����� MW_SUCCESS ��ִ��ʧ��ʱ���ش����롣
*   @remarks    
*/}
mw_create_file: function (
{IN} hHandle: MWHANDLE;
{IN} pstFileProperty: PMW_FILE_PROPERTY
): LONGWORD; stdcall;

{/*!
*   @brief      ���ļ�
*   @param[in]  hHandle           ħ���豸���
*   @param[in]  pcFileName        �ļ����ƣ���'\0'��β���ַ���      
*   @param[in]  uiReadOffset      ���ļ���ƫ����   
*   @param[in]  uiReadSize        Ԥ�ƶ�ȡ�ļ��Ĵ�С��
*   @param[out] pucReadBuffer     ��ȡ�ļ����ݵ���������ע����������㹻uiReadSize��С����������Խ�硣
*   @return     ִ�гɹ����� MW_SUCCESS ��ִ��ʧ��ʱ���ش����롣
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
*   @brief      д�ļ�
*   @param[in]  hHandle           ħ���豸���
*   @param[in]  pcFileName        �ļ����ƣ���'\0'��β���ַ���      
*   @param[in]  uiWriteOffset     д�ļ���ƫ����   
*   @param[in]  uiWriteSize       д�ļ��Ĵ�С�� 
*   @param[in]  pucWriteBuffer    ��Ҫд���ļ��Ļ����� 
*   @return     ִ�гɹ����� MW_SUCCESS ��ִ��ʧ��ʱ���ش����롣
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
*   @brief      ɾ���ļ�
*   @param[in]  hHandle           ħ���豸���
*   @param[in]  pcFileName        �ļ����ƣ���'\0'��β���ַ���      
*   @return     ִ�гɹ����� MW_SUCCESS ��ִ��ʧ��ʱ���ش����롣
*   @remarks    
*/}
mw_delete_file: function (
{IN} hHandle: MWHANDLE;
{IN} pcFileName: PANSIChar
): LONGWORD; stdcall;
                                  
{/*!
*   @brief      ��ȡ�ļ�����
*   @param[in]  hHandle           ħ���豸���
*   @param[in]  lpszFileName      �ļ����ƣ���'\0'��β���ַ��� 
*   @param[out] pstFileProperty   �ļ�������Ϣ    
*   @return     ִ�гɹ����� MW_SUCCESS ��ִ��ʧ��ʱ���ش����롣
*   @remarks    
*/}
mw_get_file_property: function (
{IN} hHandle: MWHANDLE;
{IN} pcFileName: PANSIChar;
{OUT} pstFileProperty: PMW_FILE_PROPERTY
): LONGWORD; stdcall;
                                        
{/*!
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
*   @brief          HMAC����
*   @param[in]      hHandle           ħ���豸���
*   @param[in]      pcKeyFileName     ��Կ�ļ����� 
*   @param[in]      pucInData         ��������
*   @param[in]      uiInDataLen       �������ݳ���
*   @param[out]     pucOutData        ������� 
*   @param[in out]  puiOutDataLen     ������ݳ��ȣ��μ��궨�壺MW_HMAC_MD5_DIGEST_LENGTH��MW_HMAC_SHA1_DIGEST_LENGTH��MW_HMAC_SHA256_DIGEST_LENGTH
*   @return     ִ�гɹ����� MW_SUCCESS ��ִ��ʧ��ʱ���ش����롣
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
*   @brief      Զ������
*   @param[in]  hHandle             ħ���豸���
*   @param[in]  pucInPkg            ����������
*   @param[in]  uiInPkgLen          ���������ݳ���
*   @return     ִ�гɹ����� MW_SUCCESS ��ִ��ʧ��ʱ���ش����롣
*   @remarks    
*/}
mw_update: function (
{IN} hHandle: MWHANDLE;
{IN} pucInPkg: PBYTE;
{IN} uiInPkgLen: LONGWORD
): LONGWORD; stdcall;
{/*!
*   @brief      �ָ���������
*   @param[in]  hHandle             ħ���豸���
*   @return     ִ�гɹ����� MW_SUCCESS ��ִ��ʧ��ʱ���ش����롣
*   @remarks    ���豸�ָ�����״̬����Ĭ��PID��Ĭ��PIN���ļ�ϵͳ��ա��迪����Ȩ��
*/}
mw_restore_factory: function (
{IN} hHandle: MWHANDLE
): LONGWORD; stdcall;
{/*!
*   @brief      ���������
*   @param[in]  hHandle             ħ���豸���
*   @param[in]  uiErrorCode         ������
*   @param[in]  uiLanguageID        ��������ID
*   @return     ִ�гɹ����� MW_SUCCESS ��ִ��ʧ��ʱ���ش����롣
*   @remarks    
*/}
mw_get_error_desc: function(
{IN} uiErrorCode: LONGWORD;
{IN} uiLanguageID: LONGWORD
): PAnsiChar; stdcall;


implementation

resourcestring
  rsErrLoadMemLibary    = '���ؿ�ʧ��';

// ���ý� dll ת��Ϊ�����������inc�ļ�
// ��ȫ�������������⾲̬�����滻dll����
//       �ɶ� dll ת����Ķ��������ݽ��м��ܴ���Ȼ����ʹ���ڼ���ܡ�
//       ��������߿ɸ���ʵ��������鿼�Ǵ˷�����
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
//���ܣ���ʼ��������
//������
////////////////////////////////////////////////////////////////////////////////
initialization
  Init;

finalization
  Done;

end.
