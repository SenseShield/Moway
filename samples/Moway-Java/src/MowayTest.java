import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.Arrays;

public class MowayTest {
	private static String DEVELOPER_PIN = "000000000000000000000000";  // ����������Ĭ��PIN 000000000000000000000000 ���˴���������Ϊ�����ú��PIN�룬�����߳�ʼ������ǰ��Ҫ���Ĵ�ֵ��
	private static String USER_PIN = "00000000";
	private static String USER_RW_FILENAME = "userwt.evd";  		// ��д�ļ�
	private static String USER_RD_FILENAME = "userrd.evd";  		// ֻ���ļ�
	private static String USER_KEY_AES128_FILENAME = "userkeyaes.evd";     	// key�ļ�
	private static String USER_KEY_HMAC_MD5_FILENAME = "userkeymd5.evd";    // key�ļ�
	
	private static String UPDATE_PKG_UPDATE_FILENAME = "userrd.evd";
	private static String UPDATE_PKG_UPDATE_FILE_DATA = "update_pkg update file data";
	
	private static String UPDATE_PKG_CREATE_FILENAME = "userrdpkg.evd";
	private static String UPDATE_PKG_CREATE_FILE_DATA = "update_pkg create file data";
	
	private static String UPDATE_PKG_FILENAME = "update_pkg.evd";
	
	private static byte[] stringToCCharArray(String str){
		// �� string ������ת��Ϊ byte[]��C���ֽӿ��贫��char���飬C�ӿ��ڲ�����charΪ1���ֽڣ���Java�ӿ���������������Ϊbyte[]��
		// ��Ҫ��֤byte[]���һ���ַ�������Ϊ0������JNI��װ��̬���ڲ��޷���ȷ��ȡ�������ݡ�
		byte[] byte_str = new byte[str.length() + 1];
		for (int i = 0; i < str.length(); i++) {
			byte_str[i] = str.getBytes()[i];
		}
		return byte_str;
	}

	public static void main(String[] args) {
		// TODO Auto-generated method stub
		// 1.��ʾ����������ö������������
		helloMoway();
		
		// 2. ������������
		developerInitDevice();
		// ��ȡ��������Ϣ��������Ϣ����Ҫ��֤PIN���ɻ�ȡ��
		getDeviceInfo();
		
		// 3. �û�ʹ��
		UserUseDevice();
		
		// 4. ������
		// 4.1 ���������ɸ��������ļ���������
		developerMakeUpdatePkg();
		// 4.2 �û��������������������ļ�
		userUpdatePkg();
		
		System.exit(0);
	}
	
	private static void helloMoway() {
		System.out.println("Hello Moway!");
				
		int uiDevListCount = 32;
		MW_DEVICE_INFO_CTX[] pDevInfoList = new MW_DEVICE_INFO_CTX[32];
		for(int i = 0; i < uiDevListCount; i++){
			pDevInfoList[i] = new MW_DEVICE_INFO_CTX();
		}
		int[] puiDevCount = new int[1]; 

		// ö�ٵ�ǰ����������ħ�������
		int r = MowayJava.mw_enum(pDevInfoList, uiDevListCount, puiDevCount);
		if (r != MowayJava.MW_SUCCESS){
			System.out.println(r);
		}
		
		// ��ö�ٵ��ĵ�һ�Ѽ�����
		int uiShareMode = MowayJava.MW_OPEN_EXCLUSIVE_MODE;
		long[] phHandle = new long[1];
		r = MowayJava.mw_open(pDevInfoList[0], uiShareMode, phHandle);
		if (r != MowayJava.MW_SUCCESS){
			System.out.println(r);
		}
		
		// PIN��֤��������Ȩ�ޣ�
		long hHandle = phHandle[0];
		byte ucPinType = MowayJava.MW_PIN_TYPE_DEVELOPER;
		byte[] devPIN = stringToCCharArray(DEVELOPER_PIN);
		r = MowayJava.mw_verify_pin(hHandle , ucPinType , devPIN);
		if (r != MowayJava.MW_SUCCESS){
			System.out.println(r);
		}
		
		// �رռ��������
		MowayJava.mw_close(hHandle);
		
		System.out.println("Moway test end!");
	}
		
	private static void getDeviceInfo(){
		int uiDevListCount = 32;
		MW_DEVICE_INFO_CTX[] pDevInfoList = new MW_DEVICE_INFO_CTX[32];
		for(int i = 0; i < uiDevListCount; i++){
			pDevInfoList[i] = new MW_DEVICE_INFO_CTX();
		}
		int[] puiDevCount = new int[1]; 

		// ö�ٵ�ǰ����������ħ�������
		int r = MowayJava.mw_enum(pDevInfoList, uiDevListCount, puiDevCount);
		if (r != MowayJava.MW_SUCCESS){
			System.out.println(r);
		}
		
		// ��ö�ٵ��ĵ�һ�Ѽ�����
		long[] phHandle = new long[1];
		r = MowayJava.mw_open(pDevInfoList[0], MowayJava.MW_OPEN_EXCLUSIVE_MODE, phHandle);
		if (r != MowayJava.MW_SUCCESS){
			System.out.println(r);
		}
		long hHandle = phHandle[0];		
		
		// ��ȡ������״̬
		MW_DEVICE_STATUS_CTX pstDeviceStatusCtx = new MW_DEVICE_STATUS_CTX();
		r = MowayJava.mw_get_device_status(hHandle, pstDeviceStatusCtx);
		if (r == MowayJava.MW_SUCCESS) {
			System.out.println("mw_get_device_status success, uiSysTime = " + pstDeviceStatusCtx.uiSysTime);
		} else {
			System.out.println("mw_get_device_status failed, r = " + r);
		}
		
		// ��ȡ�豸��Ϣ�����У�
		MW_DEVICE_ALL_ITEM_INFO pstDeviceAllInfo = new MW_DEVICE_ALL_ITEM_INFO();
		r = MowayJava.mw_get_device_all_info(hHandle, pstDeviceAllInfo);
		if (r == MowayJava.MW_SUCCESS) {
			System.out.println("mw_get_device_all_info success.");
		} else {
			System.out.println("mw_get_device_all_info failed, r = " + r);
		}
		
		byte[] buffer = new byte[128];
		int[] buffer_len_array = new int[1]; 
		// ��ȡ������Ϣ
		buffer_len_array[0] = buffer.length;
		r = MowayJava.mw_get_device_info(hHandle, (byte)MowayJava.MW_GET_INFO_ITEM_STATUS, buffer, buffer_len_array);
		if (r == MowayJava.MW_SUCCESS) {
			byte status = buffer[0];
			System.out.println("MW_GET_INFO_ITEM_STATUS:" + status);
		}
		
		buffer_len_array[0] = buffer.length;
		r = MowayJava.mw_get_device_info(hHandle, (byte)MowayJava.MW_GET_INFO_ITEM_SN, buffer, buffer_len_array);
		if (r == MowayJava.MW_SUCCESS) {
			//System.out.println("MW_GET_INFO_ITEM_SN:" + sn);	// 16�ֽڶ��������ݣ������ MW_DEVICE_ALL_ITEM_INFO ������
		}
		
		// �رռ��������
		MowayJava.mw_close(hHandle);
	}
	
	private static void developerInitDevice() {
		// ������������
		int uiDevListCount = 32;
		MW_DEVICE_INFO_CTX[] pDevInfoList = new MW_DEVICE_INFO_CTX[32];
		for(int i = 0; i < uiDevListCount; i++){
			pDevInfoList[i] = new MW_DEVICE_INFO_CTX();
		}
		int[] puiDevCount = new int[1]; 

		// 1.ö����
		int r = MowayJava.mw_enum(pDevInfoList, uiDevListCount, puiDevCount);
		if (r != MowayJava.MW_SUCCESS){
			System.out.println(r);
		}
		
		// 2.����
		long[] phHandle = new long[1];
		r = MowayJava.mw_open(pDevInfoList[0], MowayJava.MW_OPEN_EXCLUSIVE_MODE, phHandle);
		if (r != MowayJava.MW_SUCCESS){
			System.out.println(r);
		}
		long hHandle = phHandle[0];
		
		// 3.��֤������PIN
		byte[] devPIN = stringToCCharArray(DEVELOPER_PIN);
		r = MowayJava.mw_verify_pin(hHandle , (byte)MowayJava.MW_PIN_TYPE_DEVELOPER , devPIN);
		if (r != MowayJava.MW_SUCCESS){
			System.out.println(r);
		}
		
		// 4.����PID
		int uiSeedLen = 16;
		byte[] pucPIDSeed = new byte[uiSeedLen];
		for(int i = 0; i < uiSeedLen; i++) {
			pucPIDSeed[i] = (byte) (i + 1);
		}
		r = MowayJava.mw_set_pid(hHandle, pucPIDSeed, uiSeedLen);
		if (r != MowayJava.MW_SUCCESS){
			System.out.println(r);
		}
		
		// 5.�޸Ŀ�����PIN
		short limitCount = 15;	// Ĭ��������
		byte[] oldPIN = stringToCCharArray(DEVELOPER_PIN);
		byte[] newPIN = stringToCCharArray(DEVELOPER_PIN);	// �˴���������ʾ����PIN����PIN��ͬ��
		r = MowayJava.mw_change_pin(hHandle, (byte)MowayJava.MW_PIN_TYPE_DEVELOPER, limitCount, oldPIN, newPIN);
		if (r != MowayJava.MW_SUCCESS){
			System.out.println(r);
		}
		
		// 6.�����ļ��������ơ���д����Կ�ļ�-AES128����Կ�ļ�-HMAC��
		MW_FILE_PROPERTY fileProperty = new MW_FILE_PROPERTY();
		// 6.1 �����������û�ֻ���ļ�
		fileProperty.ucType = (byte)MowayJava.MW_FILE_TYPE_BINARY;
		fileProperty.uiSize = 128;
		fileProperty.usPrivilege = (short)MowayJava.MW_FILE_PRIV_TYPE_READ;
		fileProperty.acName = stringToCCharArray(USER_RD_FILENAME);
		r = MowayJava.mw_create_file(hHandle, fileProperty);
		if (r == MowayJava.MW_SUCCESS){
			System.out.println("mw_create_file MW_FILE_PRIV_TYPE_READ sucess.");
		} else if (r == MowayJava.MW_ERROR_INVALID_DEVICE_FILE_HANDLE) {
			System.out.println("mw_create_file MW_FILE_PRIV_TYPE_READ, " + USER_RD_FILENAME + " file existed.");
		} else {
			System.out.println("mw_create_file MW_FILE_PRIV_TYPE_READ failed, r = " + r);
		}

		// 6.2 �����������û��ɶ�д�ļ�
		fileProperty.ucType = (byte)MowayJava.MW_FILE_TYPE_BINARY;
		fileProperty.uiSize = 128;
		fileProperty.usPrivilege = (short)MowayJava.MW_FILE_PRIV_TYPE_READ_WRITE;
		fileProperty.acName = stringToCCharArray(USER_RW_FILENAME);
		r = MowayJava.mw_create_file(hHandle, fileProperty);
		if (r == MowayJava.MW_SUCCESS){
			System.out.println("mw_create_file MW_FILE_PRIV_TYPE_READ_WRITE sucess.");
		} else if (r == MowayJava.MW_ERROR_INVALID_DEVICE_FILE_HANDLE) {
			System.out.println("mw_create_file MW_FILE_PRIV_TYPE_READ_WRITE, " + USER_RW_FILENAME + " file existed.");
		} else {
			System.out.println("mw_create_file MW_FILE_PRIV_TYPE_READ_WRITE failed, r = " + r);
		}
		
		// 6.3 ������Կ�ļ�-AES128
		fileProperty.ucType = (byte)MowayJava.MW_FILE_TYPE_KEY;
		fileProperty.uiSize = 256;
		fileProperty.usPrivilege = (short)MowayJava.MW_FILE_PRIV_TYPE_USE;
		fileProperty.acName = stringToCCharArray(USER_KEY_AES128_FILENAME);
		r = MowayJava.mw_create_file(hHandle, fileProperty);
		if (r == MowayJava.MW_SUCCESS){
			System.out.println("mw_create_file MW_FILE_PRIV_TYPE_USE success.");
			
			// д��AES128��Կ����
			File file = new File("aes_128.sls");
			try {
				byte[] keyFilename = stringToCCharArray(USER_KEY_AES128_FILENAME);
				
				FileInputStream fileInputStream = new FileInputStream(file);
				
				int uiWriteOffset = 0;
				int uiWriteSize;
				try {
					uiWriteSize = fileInputStream.available();
					byte[] pucWriteBuffer = new byte[uiWriteSize];
					fileInputStream.read(pucWriteBuffer);
					fileInputStream.close();
					
					r = MowayJava.mw_write_file(hHandle, keyFilename, uiWriteOffset, uiWriteSize, pucWriteBuffer);
					if (r == MowayJava.MW_SUCCESS) {
						System.out.println("mw_write_file MW_FILE_PRIV_TYPE_USE success.");
					} else {
						System.out.println("mw_write_file MW_FILE_PRIV_TYPE_USE failed, r = " + r);
					}
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}			
			} catch (FileNotFoundException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}else if(r == MowayJava.MW_ERROR_INVALID_DEVICE_FILE_HANDLE) {
			System.out.println("mw_create_file MW_FILE_PRIV_TYPE_USE, " + USER_KEY_AES128_FILENAME + " file existed.");
		}
		else {
			System.out.println("mw_create_file MW_FILE_PRIV_TYPE_USE failed, r = " + r);
		}
		
		// 6.4 ������Կ�ļ�-HMAC
		fileProperty.ucType = (byte)MowayJava.MW_FILE_TYPE_KEY;
		fileProperty.uiSize = 256;
		fileProperty.usPrivilege = (short)MowayJava.MW_FILE_PRIV_TYPE_USE;
		fileProperty.acName = stringToCCharArray(USER_KEY_HMAC_MD5_FILENAME);
		r = MowayJava.mw_create_file(hHandle, fileProperty);
		if (r == MowayJava.MW_SUCCESS){
			System.out.println("mw_create_file MW_FILE_PRIV_TYPE_USE success.");
			
			// д��AES128��Կ����
			File file = new File("hmac_md5.sls");
			try {
				byte[] keyFilename = stringToCCharArray(USER_KEY_HMAC_MD5_FILENAME);
				
				FileInputStream fileInputStream = new FileInputStream(file);
				
				int uiWriteOffset = 0;
				int uiWriteSize;
				try {
					uiWriteSize = fileInputStream.available();
					byte[] pucWriteBuffer = new byte[uiWriteSize];
					fileInputStream.read(pucWriteBuffer);
					fileInputStream.close();
					
					r = MowayJava.mw_write_file(hHandle, keyFilename, uiWriteOffset, uiWriteSize, pucWriteBuffer);
					if (r == MowayJava.MW_SUCCESS) {
						System.out.println("mw_write_file MW_FILE_PRIV_TYPE_USE success.");
					} else {
						System.out.println("mw_write_file MW_FILE_PRIV_TYPE_USE failed, r = " + r);
					}
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}			
			} catch (FileNotFoundException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}else if(r == MowayJava.MW_ERROR_INVALID_DEVICE_FILE_HANDLE) {
			System.out.println("mw_create_file MW_FILE_PRIV_TYPE_USE, " + USER_KEY_HMAC_MD5_FILENAME + " file existed.");
		}
		else {
			System.out.println("mw_create_file MW_FILE_PRIV_TYPE_USE failed, r = " + r);
		}
		
		// 7.������Ȩ��д�ļ�
		byte[] rwFilename;
		byte[] pucWriteBuffer;
		int uiWriteOffset = 0;
		int uiWriteSize = 0;
		
		// 7.1 �޸��û�ֻ���ļ�
		rwFilename = stringToCCharArray(USER_RD_FILENAME);
		pucWriteBuffer = "Hello Moway(RD)!".getBytes();
		uiWriteOffset = 0;
		uiWriteSize = pucWriteBuffer.length;
		r = MowayJava.mw_write_file(hHandle, rwFilename, uiWriteOffset, uiWriteSize, pucWriteBuffer);
		if (r == MowayJava.MW_SUCCESS) {
			System.out.printf("mw_write_file %s success.\n", USER_RD_FILENAME);
		} else {
			System.out.printf("mw_write_file %s failed, r = \n", USER_RD_FILENAME, r);
		}
		
		// 7.2 �޸��û��ɶ�д�ļ�
		rwFilename = stringToCCharArray(USER_RW_FILENAME);
		pucWriteBuffer = "Hello Moway(RW)!".getBytes();
		uiWriteOffset = 0;
		uiWriteSize = pucWriteBuffer.length;
		r = MowayJava.mw_write_file(hHandle, rwFilename, uiWriteOffset, uiWriteSize, pucWriteBuffer);
		if (r == MowayJava.MW_SUCCESS) {
			System.out.printf("mw_write_file %s success.\n", USER_RW_FILENAME);
		} else {
			System.out.printf("mw_write_file %s failed, r = \n", USER_RW_FILENAME, r);
		}
		
		// ö�������ļ�
		MW_FILE_PROPERTY[] filePropertyArray = new MW_FILE_PROPERTY[50];
		int filePropertyArraySize = filePropertyArray.length;
		for (int i = 0; i < filePropertyArraySize; i++) {
			filePropertyArray[i] = new MW_FILE_PROPERTY();
		}
		int[] filePropertyCountArray = new int[1];
		r = MowayJava.mw_enum_file(hHandle, filePropertyArray, filePropertyArraySize, filePropertyCountArray);
		if (r == MowayJava.MW_SUCCESS) {
			System.out.println("mw_enum_file success, file count = " + filePropertyCountArray[0]);
			for (int i = 0; i < filePropertyCountArray[0]; i++) {
				String filename = new String(filePropertyArray[i].acName);
				System.out.println(filename);
			}
		} else {
			System.out.printf("mw_write_file %s failed, r = %d\n", USER_RW_FILENAME, r);
		}
		
		// 8.�ر���
		MowayJava.mw_close(hHandle);
	}


	private static void UserUseDevice() {
		// ��ʾʹ���û�Ȩ�޵�¼���ʼ�������������¹��ܣ�
		//* �޸��û�PIN
		//* ��ֻ���ļ�
		//* �޸Ŀɶ�д�����ļ�
		//* ʹ����Կ�ļ��ӽ��ܣ�AES128��
		//* ʹ����Կ�ļ���HMAC������Hashֵ
		
		int uiDevListCount = 32;
		MW_DEVICE_INFO_CTX[] pDevInfoList = new MW_DEVICE_INFO_CTX[32];
		for(int i = 0; i < uiDevListCount; i++){
			pDevInfoList[i] = new MW_DEVICE_INFO_CTX();
		}
		int[] puiDevCount = new int[1]; 

		// 1.ö����
		int r = MowayJava.mw_enum(pDevInfoList, uiDevListCount, puiDevCount);
		if (r != MowayJava.MW_SUCCESS){
			System.out.println("mw_enum failed, r = " + r);
			return;
		}
		
		// 2.����
		long[] phHandle = new long[1];
		r = MowayJava.mw_open(pDevInfoList[0], MowayJava.MW_OPEN_EXCLUSIVE_MODE, phHandle);
		if (r != MowayJava.MW_SUCCESS){
			System.out.println("mw_open failed, r = " + r);
			return;
		}
		long hHandle = phHandle[0];
		
		// 3.��֤�û�PIN
		byte[] userPIN = stringToCCharArray(USER_PIN);
		r = MowayJava.mw_verify_pin(hHandle, (byte)MowayJava.MW_PIN_TYPE_USER, userPIN);
		if (r != MowayJava.MW_SUCCESS){
			System.out.println("User mw_verify_pin failed, r = " + r);
			return;
		}
		
		// 4.�޸��û�PIN
		short limitCount = 15;	// Ĭ��������
		byte[] oldPIN = stringToCCharArray(USER_PIN);
		byte[] newPIN = stringToCCharArray(USER_PIN);	// �˴���������ʾ����PIN����PIN��ͬ��
		r = MowayJava.mw_change_pin(hHandle, (byte)MowayJava.MW_PIN_TYPE_USER, limitCount, oldPIN, newPIN);
		if (r != MowayJava.MW_SUCCESS){
			System.out.println("User mw_change_pin failed, r = " + r);
		}
		
		//5.��ֻ���ļ�
		byte[] rFilename = stringToCCharArray(USER_RD_FILENAME);
		int readOffset = 0;
		int readSize = 16;	// ��ͨ����ѯ�ļ����Խӿڣ�����ļ���С
		byte[] readBuffer = new byte[readSize];
		r = MowayJava.mw_read_file(hHandle, rFilename, readOffset, readSize, readBuffer);
		if (r == MowayJava.MW_SUCCESS){
			String readFileContent = new String(readBuffer); 
			System.out.println("User mw_read_file success, " + readFileContent);
		} else {
			System.out.println("User mw_read_file " + USER_RD_FILENAME + " failed, r = " + r);
		}
		
		//6.�޸Ŀɶ�д�����ļ�
		byte[] rwFilename = stringToCCharArray(USER_RW_FILENAME);
		int writeOffset = 0;
		int writeSize = 16;	// ��ͨ����ѯ�ļ����Խӿڣ�����ļ���С
		byte[] writeBuffer = "user write!".getBytes();
		r = MowayJava.mw_write_file(hHandle, rwFilename, writeOffset, writeSize, writeBuffer);
		if (r == MowayJava.MW_SUCCESS){
			System.out.println("User mw_write_file " + USER_RW_FILENAME + " success");
		} else {
			System.out.println("User mw_write_file " + USER_RW_FILENAME + " failed, r = " + r);
		}
		
		//7.ʹ����Կ�ļ��ӽ��ܣ�AES128��
		int aesDataLen = 128;
		// 7.1 ����
		byte[] aesKeyFilename = stringToCCharArray(USER_KEY_AES128_FILENAME);
		byte algoMode = (byte)MowayJava.MW_ALG_TYPE_AES;
		byte[] iv = null;	// ������IV����
		byte[] srcData = new byte[aesDataLen];
		int srcDataLen = srcData.length;
		byte[] encData = new byte[aesDataLen];
		int[] encDataLenArray = new int[1];
		
		for (int i = 0; i < srcDataLen; i++) {
			srcData[i] = (byte)(i);
		}
		encDataLenArray[0] = encData.length;
		
		r = MowayJava.mw_sym_encrypt(hHandle, aesKeyFilename, algoMode, iv, srcData, srcDataLen, encData, encDataLenArray);
		if (r == MowayJava.MW_SUCCESS){
			System.out.println("User mw_sym_encrypt " + USER_KEY_AES128_FILENAME + " success");
		} else {
			System.out.println("User mw_sym_encrypt " + USER_KEY_AES128_FILENAME + " failed, r = " + r);
		}
		
		// 7.2 ����
		byte[] decData = new byte[aesDataLen];
		int[] decDataLenArray = new int[1];
		decDataLenArray[0] = decData.length;
		r = MowayJava.mw_sym_decrypt(hHandle, aesKeyFilename, algoMode, iv, encData, encDataLenArray[0], decData, decDataLenArray);
		if (r == MowayJava.MW_SUCCESS){
			System.out.println("User mw_sym_decrypt " + USER_KEY_AES128_FILENAME + " success");
		} else {
			System.out.println("User mw_sym_decrypt " + USER_KEY_AES128_FILENAME + " failed, r = " + r);
		}
		
		// 7.3 �ӽ��ܽ���Ա�
		if (Arrays.equals(srcData, decData)) {
			System.out.println("User mw_sym_decrypt srcData = decData");
		} else {
			System.out.println("User mw_sym_decrypt srcData != decData");
		}
		
		//8.ʹ����Կ�ļ���HMAC������Hashֵ
		byte[] hmacMD5KeyFilename = stringToCCharArray(USER_KEY_HMAC_MD5_FILENAME);
		byte[] md5Data = new byte[128];
		int[] md5DataLenArray = new int[1];
		md5DataLenArray[0] = md5Data.length;
		r = MowayJava.mw_hmac_calc(hHandle, hmacMD5KeyFilename, srcData, srcDataLen, md5Data, md5DataLenArray);
		if (r == MowayJava.MW_SUCCESS){
			System.out.println("User mw_hmac_calc " + USER_KEY_HMAC_MD5_FILENAME + " success");
		} else {
			System.out.println("User mw_hmac_calc " + USER_KEY_HMAC_MD5_FILENAME + " failed, r = " + r);
		}
		
		// 9.�ر���
		MowayJava.mw_close(hHandle);
	}
	
	private static void developerMakeUpdatePkg() {
		// ��ʾʹ�ÿ�����Ȩ�޴���������������
		// 1.ö����
		// 2.����
		// 3.��֤������PIN
		// 4.���������������������������ļ���
		// 5.�ر���
		
		int uiDevListCount = 32;
		MW_DEVICE_INFO_CTX[] pDevInfoList = new MW_DEVICE_INFO_CTX[32];
		for(int i = 0; i < uiDevListCount; i++){
			pDevInfoList[i] = new MW_DEVICE_INFO_CTX();
		}
		int[] puiDevCount = new int[1]; 

		// 1.ö����
		int r = MowayJava.mw_enum(pDevInfoList, uiDevListCount, puiDevCount);
		if (r != MowayJava.MW_SUCCESS){
			System.out.println(r);
		}
		
		// 2.����
		long[] phHandle = new long[1];
		r = MowayJava.mw_open(pDevInfoList[0], MowayJava.MW_OPEN_EXCLUSIVE_MODE, phHandle);
		if (r != MowayJava.MW_SUCCESS){
			System.out.println(r);
		}
		long hHandle = phHandle[0];
		
		// 3.��֤������PIN
		byte[] devPIN = stringToCCharArray(DEVELOPER_PIN);
		r = MowayJava.mw_verify_pin(hHandle , (byte)MowayJava.MW_PIN_TYPE_DEVELOPER , devPIN);
		if (r != MowayJava.MW_SUCCESS){
			MowayJava.mw_close(hHandle);
			System.out.println("mw_verify_pin failed, r = " + r);
			return;
		}

		// 4.���������������������������ļ���
		int devPID = 0;
		byte[] devSN = null;
		MW_UPDADE_FILE_CTX[] filePathArray = new MW_UPDADE_FILE_CTX[2];	// ������������ļ�
		int filePathArraySize = filePathArray.length; 
		
		byte[] pkgBuffer = new byte[4096];	// ������������������ݷ��仺������С
		int[] pkgBufferLenArray = new int[2];
		
		for (int i = 0; i < filePathArray.length; i++) {
			filePathArray[i] = new MW_UPDADE_FILE_CTX();
		}
		
		// ��ʼ����������������
		// 4.1 ��ʼ�������ļ�����
		MW_UPDADE_FILE_CTX updateFileCTXUpdate = filePathArray[0];
		updateFileCTXUpdate.ucType = (byte)MowayJava.MW_FILE_TYPE_BINARY;			// ���ͣ����
		updateFileCTXUpdate.usPrivilege = (short)MowayJava.MW_FILE_PRIV_TYPE_READ;	// Ȩ�ޣ����
		updateFileCTXUpdate.uiSize = 256;	// �޸��ļ���С����ѡ��
		updateFileCTXUpdate.acName = stringToCCharArray(UPDATE_PKG_UPDATE_FILENAME);
		updateFileCTXUpdate.pData = UPDATE_PKG_UPDATE_FILE_DATA.getBytes();
		updateFileCTXUpdate.uiDateSize = UPDATE_PKG_UPDATE_FILE_DATA.length();
		
		MW_UPDADE_FILE_CTX updateFileCTXCreate = filePathArray[1];
		updateFileCTXCreate.ucType = (byte)MowayJava.MW_FILE_TYPE_BINARY;			// ���ͣ����
		updateFileCTXCreate.usPrivilege = (short)MowayJava.MW_FILE_PRIV_TYPE_READ_WRITE;	// Ȩ�ޣ����
		updateFileCTXCreate.uiSize = 128;	// �޸��ļ���С����ѡ��
		updateFileCTXCreate.acName = stringToCCharArray(UPDATE_PKG_CREATE_FILENAME);
		updateFileCTXCreate.pData = UPDATE_PKG_CREATE_FILE_DATA.getBytes();
		updateFileCTXCreate.uiDateSize = UPDATE_PKG_CREATE_FILE_DATA.length();

		// 4.2 ��ʼ��PID��SN
		// ע�⣺1.ָ��PIDʱ������ͬPID����������ʹ�ã�2.��ָ��SNʱ��ֻ��ָ��SN�ļ������ܹ�ʹ����������

		// ��ʾ���ӵ�ǰ��������ȡPID����ʾΪͬ���εļ�����������������ע�⣺��Ϊ��ͬPID����������ʱ��PID�����ɴ�����;����ȡ��
		MW_DEVICE_ALL_ITEM_INFO pstDeviceAllInfo = new MW_DEVICE_ALL_ITEM_INFO();
		r = MowayJava.mw_get_device_all_info(hHandle, pstDeviceAllInfo);
		if (r == MowayJava.MW_SUCCESS){
			devPID = pstDeviceAllInfo.uiPID;
			System.out.println("mw_get_device_info MW_GET_INFO_ITEM_PID, " + devPID);
		} else {
			System.out.println("mw_get_device_info MW_GET_INFO_ITEM_PID failed, r = " + r);
		}
		
		// 4.2 ��ʼ���������������
		pkgBufferLenArray[0] = pkgBuffer.length;
		r = MowayJava.mw_make_update_pkg(hHandle, devPID, devSN, filePathArray, filePathArraySize, pkgBuffer, pkgBufferLenArray);
		if (r == MowayJava.MW_SUCCESS){
			System.out.println("mw_make_update_pkg success");
			
			int pkgBufferLen = pkgBufferLenArray[0];
			if (pkgBufferLen > 0)
			{
				// ������������д�뱾���ļ� 
				File file = new File(UPDATE_PKG_FILENAME);
				if (file.exists() && file.isFile()){
					file.delete();
				}
				
				try {
					if (file.createNewFile()){
						FileOutputStream outputStream = new	FileOutputStream(file);
						outputStream.write(pkgBuffer, 0, pkgBufferLen);
						outputStream.close();	
					}
				} catch (FileNotFoundException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}
		} else {
			System.out.println("User mw_hmac_calc " + USER_KEY_HMAC_MD5_FILENAME + " failed, r = " + r);
		}
		
		// 5.�ر���
		MowayJava.mw_close(hHandle);
	}

	private static void userUpdatePkg() {
		// ��ʾʹ���û�Ȩ�޵��������������
		//1.ö����
		//2.����
		//3.��֤�û�PIN
		//4.����������
		//5.��֤���������ݣ���ȡ��֤��������������һ�£�
		//6.�ر���
		
		int uiDevListCount = 32;
		MW_DEVICE_INFO_CTX[] pDevInfoList = new MW_DEVICE_INFO_CTX[32];
		for(int i = 0; i < uiDevListCount; i++){
			pDevInfoList[i] = new MW_DEVICE_INFO_CTX();
		}
		int[] puiDevCount = new int[1]; 

		// 1.ö����
		int r = MowayJava.mw_enum(pDevInfoList, uiDevListCount, puiDevCount);
		if (r != MowayJava.MW_SUCCESS){
			System.out.println("mw_enum failed, r = " + r);
			return;
		}
		
		// 2.����
		long[] phHandle = new long[1];
		r = MowayJava.mw_open(pDevInfoList[0], MowayJava.MW_OPEN_EXCLUSIVE_MODE, phHandle);
		if (r != MowayJava.MW_SUCCESS){
			System.out.println("mw_open failed, r = " + r);
			return;
		}
		long hHandle = phHandle[0];
		
		// 3.��֤�û�PIN
		byte[] userPIN = stringToCCharArray(USER_PIN);
		r = MowayJava.mw_verify_pin(hHandle, (byte)MowayJava.MW_PIN_TYPE_USER, userPIN);
		if (r != MowayJava.MW_SUCCESS){
			System.out.println("User mw_verify_pin failed, r = " + r);
			MowayJava.mw_close(hHandle);
			return;
		}
		
		// 4.����������
		byte[] pkgBuffer = null;
		int pkgBufferLen = 0;
		
		// �ӱ����ļ���������������
		File file = new File(UPDATE_PKG_FILENAME);
		try {
			FileInputStream fileInputStream = new FileInputStream(file);
			try {
				pkgBufferLen = fileInputStream.available();
				pkgBuffer = new byte[pkgBufferLen];
				fileInputStream.read(pkgBuffer);
				fileInputStream.close();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
				return;
			}
		} catch (FileNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
			return;
		}
		
		r = MowayJava.mw_update(hHandle, pkgBuffer, pkgBufferLen);
		if (r == MowayJava.MW_SUCCESS){
			System.out.println("User mw_update " + UPDATE_PKG_FILENAME + " success");
		} else {
			System.out.println("User mw_update " + UPDATE_PKG_FILENAME + " failed, r = " + r);
		}
		
		// 5.��֤���������ݣ���ȡ��֤��������������һ�£�
		byte[] rFilename = null;
		int readOffset = 0;
		int readSize = 0;	// ��ͨ����ѯ�ļ����Խӿڣ�����ļ���С
		byte[] readBuffer = new byte[readSize];
		
		// 5.1 ��ֻ���ļ�
		rFilename = stringToCCharArray(USER_RD_FILENAME);
		readOffset = 0;
		readSize = UPDATE_PKG_UPDATE_FILE_DATA.length();	// ��ͨ����ѯ�ļ����Խӿڣ�����ļ���С
		readBuffer = new byte[readSize];
		r = MowayJava.mw_read_file(hHandle, rFilename, readOffset, readSize, readBuffer);
		if (r == MowayJava.MW_SUCCESS){
			String readFileContent = new String(readBuffer); 
			System.out.println("User update mw_read_file " + USER_RD_FILENAME + " success, " + readFileContent);
		} else {
			System.out.println("User update mw_read_file " + USER_RD_FILENAME + " failed, r = " + r);
		}
		
		// 5.2 ���½��ļ�����
		rFilename = stringToCCharArray(UPDATE_PKG_CREATE_FILENAME);
		readOffset = 0;
		readSize = UPDATE_PKG_CREATE_FILE_DATA.length();	// ��ͨ����ѯ�ļ����Խӿڣ�����ļ���С
		readBuffer = new byte[readSize];
		r = MowayJava.mw_read_file(hHandle, rFilename, readOffset, readSize, readBuffer);
		if (r == MowayJava.MW_SUCCESS){
			String readFileContent = new String(readBuffer); 
			System.out.println("User update mw_read_file " + UPDATE_PKG_CREATE_FILENAME + " success, " + readFileContent);
		} else {
			System.out.println("User update mw_read_file " + UPDATE_PKG_CREATE_FILENAME + " failed, r = " + r);
		}
				
		// 9.�ر���
		MowayJava.mw_close(hHandle);
	}
	

	private static int byteArrayToIntBE(byte[] b) {
		// ����ħ��API�ӿڷ��ص�byte[]תintʱ����Ҫʹ�ô˺�����������C��Java��С�˲�һ�����int�����쳣��
		int i = (b[3] & 0xFF) << 24;
		i |= (b[2] & 0xFF) << 16;
		i |= (b[1] & 0xFF) << 8;
		i |= b[0] & 0xFF;
		return i;
	}
}

class MowayJava {
	static {
		try {
			System.loadLibrary("moway_api_java");
		} catch (UnsatisfiedLinkError e) {
			System.err.println("Native API library load failed.\n" + e);
			System.exit(1);
		}
	} 

	/*
	 * !
	 * 
	 * @brief ö��ħ���豸
	 * 
	 * @param[in out] pDevInfoList ö�������ħ���豸�б�
	 * 
	 * @param[in] uiDevListCount ö���б�(pDevInfoList)�ܴ�ŵ��豸�ĸ�����ע���������μ��궨��MW_ENUM_DEVICE_MAX_COUNT
	 * 
	 * @param[out] puiDevCount ʵ��ö�ٳ���ħ���豸����
	 * 
	 * @return ִ�гɹ����� MW_SUCCESS ��ִ��ʧ��ʱ���ش����롣
	 * 
	 * @remarks �û����밴��uiDevListCount����sizeof(MW_DEVICE_INFO_CTX)�����ڴ��pDevInfoList��API�ڲ��������ڴ��pDevInfoList��
	 */
	public final static native int mw_enum(MW_DEVICE_INFO_CTX[] pDevInfoList, int uiDevListCount, int[] puiDevCount);

	/*
	 * !
	 * 
	 * @brief ��ħ���豸
	 * 
	 * @param[in] pDevInfo ָ��ħ���豸��Ϣ�ṹ���ָ�롣
	 * 
	 * @param[in] uiShareMode ��ռ���߹���ģʽ�򿪡��μ��궨��MW_OPEN_SHARE_MODE  MW_OPEN_EXCLUSIVE_MODE
	 * 
	 * @param[out] phHandle ��ħ���豸�����һ������Ա�������á�
	 * 
	 * @return ִ�гɹ����� MW_SUCCESS ��ִ��ʧ��ʱ���ش����롣
	 * 
	 * @remarks Open an moway device with DevInfo enumerated by mw_enum()
	 */
	public final static native int mw_open(MW_DEVICE_INFO_CTX pDevInfo, int uiShareMode, long[] phHandle);

	/*
	 * !
	 * 
	 * @brief �ر��豸
	 * 
	 * @param[in] hHandle ħ���豸���
	 * 
	 * @return ִ�гɹ����� MW_SUCCESS ��ִ��ʧ��ʱ���ش����롣
	 * 
	 * @remarks
	 */
	public final static native int mw_close(long hHandle);

	/*
	 * !
	 * 
	 * @brief У��PIN
	 * 
	 * @param[in] hHandle ħ���豸���
	 * 
	 * @param[in] ucPinType Pin������ �μ��궨�壺MW_PIN_TYPE_XXX
	 * 
	 * @param[in] pucPin�����ݣ��û�8�ֽڣ�������24�ֽ�
	 * 
	 * @return ִ�гɹ����� MW_SUCCESS ��ִ��ʧ��ʱ���ش����롣
	 * 
	 * @remarks
	 */
	public final static native int mw_verify_pin(long hHandle, byte ucPinType, byte[] pucPin);

	/*
	 * !
	 * 
	 * @brief �޸�PIN
	 * 
	 * @param[in] hHandle ħ���豸���
	 * 
	 * @param[in] ucPinType Pin������ �μ��궨�壺MW_PIN_TYPE_XXX
	 * 
	 * @param[in] usLimitCount ����Դ�����ע�����޴�������Ϊ0�����ƴ�����Χ��1-15�� ��Ϊ�������򷵻ز�������
	 * 
	 * @param[in] pucOldPin           ��Pin�����ݣ��û�8�ֽڣ�������24�ֽ�
	 * 
	 * @param[in] pucNewPin           ��Pin�����ݣ��û�8�ֽڣ�������24�ֽ�
	 * 
	 * @return ִ�гɹ����� MW_SUCCESS ��ִ��ʧ��ʱ���ش����롣
	 * 
	 * @remarks
	 */
	public final static native int mw_change_pin(long hHandle, byte ucPinType, short usLimitCount, byte[] pucOldPin,
			byte[] pucNewPin);

	/*
	 * !
	 * 
	 * @brief ����PID
	 * 
	 * @param[in] hHandle ħ���豸���
	 * 
	 * @param[in] pucPIDSeed ������
	 * 
	 * @param[in] uiSeedLen
	 * �����볤�ȣ����ȷ�Χ��4-32���μ��궨��MW_SEED_LENGTH_MIN��MW_SEED_LENGTH_MAX
	 * 
	 * @return ִ�гɹ����� MW_SUCCESS ��ִ��ʧ��ʱ���ش����롣
	 * 
	 * @remarks
	 */
	public final static native int mw_set_pid(long hHandle, byte[] pucPIDSeed, int uiSeedLen);

	/*
	 * !
	 * 
	 * @brief ��������
	 * 
	 * @param[in] hHandle ħ���豸���
	 * 
	 * @param[in] uiCtrlCodeItem �����룬�μ��궨�� MW_CTRL_CODE_ITEM_XXX
	 * 
	 * @param[in] pvInBuffer ��������
	 * 
	 * @param[in] uiInBufferLen �������ݵĳ���
	 * 
	 * @param[out] pvOutBuffer �������
	 * 
	 * @param[in] uiOutBufferLen ������ݵĳ���
	 * 
	 * @param[out] puiReturnedLen ʵ��������ݵĳ���
	 * 
	 * @return ִ�гɹ����� MW_SUCCESS ��ִ��ʧ��ʱ���ش����롣
	 * 
	 * @remarks
	 */
	public final static native int mw_control(long hHandle, byte uiCtrlCodeItem, byte[] pvInBuffer, int uiInBufferLen,
			byte[] pvOutBuffer, int uiOutBufferLen, int[] puiReturnedLen);

	/*
	 * !
	 * 
	 * @brief ��ȡ�豸��Ϣ
	 * 
	 * @param[in] hHandle ħ���豸���
	 * 
	 * @param[in] ucInfoItem ָ����ȡ���豸��Ϣ�ľ�����μ��궨�壺MW_GET_INFO_ITEM_XXX
	 * 
	 * @param[out] pvBuffer �����ȡ���豸��Ϣ��
	 * 
	 * @param[in out] puiBufferLength ����豸��Ϣ������ݳ���
	 * 
	 * @return ִ�гɹ����� MW_SUCCESS ��ִ��ʧ��ʱ���ش����롣
	 * 
	 * @remarks
	 */
	public final static native int mw_get_device_info(long hHandle, byte ucInfoItem, byte[] pvBuffer,
			int[] puiBufferLength);
	public final static native int mw_get_device_all_info(long hHandle, MW_DEVICE_ALL_ITEM_INFO pstDeviceAllInfo);

	/*
	 * !
	 * 
	 * @brief ��ȡ�豸״̬
	 * 
	 * @param[in] hHandle ħ���豸���
	 * 
	 * @param[out] pstDeviceStatusCtx ��ǰ�豸״̬��
	 * 
	 * @return ִ�гɹ����� MW_SUCCESS ��ִ��ʧ��ʱ���ش����롣
	 * 
	 * @remarks
	 */
	public final static native int mw_get_device_status(long hHandle, MW_DEVICE_STATUS_CTX pstDeviceStatusCtx);

	/*
	 * !
	 * 
	 * @brief �о��ļ�
	 * 
	 * @param[in] hHandle ħ���豸���
	 * 
	 * @param[out] pstFilePropertyList
	 * �ṹ��MW_FILE_PROPERTY���飬���Ĵ�С��uiFileListCountָ��
	 * 
	 * @param[in] uiFileListCount ָ��pstFilePropertyList����Ĵ�С
	 * 
	 * @param[out] puiReturnedFileCount ʵ�ʷ����оٵ��ļ��ĸ���
	 * 
	 * @return ִ�гɹ����� MW_SUCCESS ��ִ��ʧ��ʱ���ش����롣
	 * 
	 * @remarks ע�⣺pstFilePropertyList�ɺ����ⲿ�����߷��䣬��СΪuiFileListCount * sizeof(MW_FILE_PROPERTY)
	 */
	public final static native int mw_enum_file(long hHandle, MW_FILE_PROPERTY[] pstFilePropertyList, int uiFileListCount,
			int[] puiReturnedFileCount);

	/*
	 * !
	 * 
	 * @brief �����ļ�
	 * 
	 * @param[in] hHandle ħ���豸���
	 * 
	 * @param[in] pstFileProperty ָ����Ҫ�����ļ������ԣ������ļ����͡��ļ�Ȩ�ޡ��ļ���СΪ������
	 * 
	 * @return ִ�гɹ����� MW_SUCCESS ��ִ��ʧ��ʱ���ش����롣
	 * 
	 * @remarks
	 */
	public final static native int mw_create_file(long hHandle, MW_FILE_PROPERTY pstFileProperty);

	/*
	 * !
	 * 
*   @brief      ���ļ�
*   @param[in]  hHandle           ħ���豸���
*   @param[in]  pcFileName        �ļ����ƣ���'\0'��β���ַ���      
*   @param[in]  uiReadOffset      ���ļ���ƫ����   
*   @param[in]  uiReadSize        Ԥ�ƶ�ȡ�ļ��Ĵ�С��
*   @param[out] pucReadBuffer     ��ȡ�ļ����ݵ���������ע����������㹻uiReadSize��С����������Խ�硣
	 * 
	 * @return ִ�гɹ����� MW_SUCCESS ��ִ��ʧ��ʱ���ش����롣
	 * 
	 * @remarks
	 */
	public final static native int mw_read_file(long hHandle, byte[] pcFileName, int uiReadOffset, int uiReadSize,
			byte[] pucReadBuffer);

	/*
	 * !
	 * 
	 * @brief д�ļ�
	 * 
	 * @param[in] hHandle ħ���豸���
	 * 
	 * @param[in] pcFileName �ļ����ƣ���'\0'��β���ַ���
	 * 
	 * @param[in] uiWriteOffset д�ļ���ƫ����
	 * 
	 * @param[in] uiWriteSize д�ļ��Ĵ�С�� 
	 * 
	 * @param[in] pucWriteBuffer ��Ҫд���ļ��Ļ�����
	 * 
	 * @return ִ�гɹ����� MW_SUCCESS ��ִ��ʧ��ʱ���ش����롣
	 * 
	 * @remarks
	 */
	public final static native int mw_write_file(long hHandle, byte[] pcFileName, int uiWriteOffset, int uiWriteSize,
			byte[] pucWriteBuffer);

	/*
	 * !
	 * 
	 * @brief ɾ���ļ�
	 * 
	 * @param[in] hHandle ħ���豸���
	 * 
	 * @param[in] pcFileName �ļ����ƣ���'\0'��β���ַ���
	 * 
	 * @return ִ�гɹ����� MW_SUCCESS ��ִ��ʧ��ʱ���ش����롣
	 * 
	 * @remarks
	 */
	public final static native int mw_delete_file(long hHandle, byte[] pcFileName);

	/*
	 * !
	 * 
	 * @brief ��ȡ�ļ�����
	 * 
	 * @param[in] hHandle ħ���豸���
	 * 
	 * @param[in] lpszFileName �ļ����ƣ���'\0'��β���ַ���
	 * 
	 * @param[out] pstFileProperty �ļ�������Ϣ
	 * 
	 * @return ִ�гɹ����� MW_SUCCESS ��ִ��ʧ��ʱ���ش����롣
	 * 
	 * @remarks
	 */
	public final static native int mw_get_file_property(long hHandle, byte[] pcFileName,
			MW_FILE_PROPERTY pstFileProperty);

	/*
	 * !
	 * 
	 * @brief �ԳƼ���
	 * 
	 * @param[in] hHandle ħ���豸���
	 * 
	 * @param[in] pcKeyFileName ��Կ�ļ�����
	 * 
	 * @param[in] ucAlgoMode �ԳƼ��ܵ��㷨���μ��궨�壺MW_ALG_TYPE_XXX
	 * 
	 * @param[in] pucIV �ԳƼ����㷨����Ҫʹ�õ����������̶�16�ֽڣ�CBCģʽ��Ҫ���룬ECB�ɲ���
	 * 
	 * @param[in] pucInData ������������ģ�������16�ı�����
	 * 
	 * @param[in] uiInDataLen �������ĵ����ݳ���,
	 * 
	 * @param[out] pucOutData ���ܺ����������
	 * 
	 * @param[in out] puiOutDataLen ���ܺ����ĵĳ���
	 * 
	 * @return ִ�гɹ����� MW_SUCCESS ��ִ��ʧ��ʱ���ش����롣
	 * 
	 * @remarks
	 */
	public final static native int mw_sym_encrypt(long hHandle, byte[] pcKeyFileName, byte ucAlgoMode, byte[] pucIV,
			byte[] pucInData, int uiInDataLen, byte[] pucOutData, int[] puiOutDataLen);

	/*
	 * !
	 * 
	 * @brief �Գƽ���
	 * 
	 * @param[in] hHandle ħ���豸���
	 * 
	 * @param[in] pcKeyFileName ��Կ�ļ�����
	 * 
	 * @param[in] ucAlgoMode �ԳƼ��ܵ��㷨���μ��궨�壺MW_ALG_TYPE_XXX
	 * 
	 * @param[in] pucIV �ԳƼ����㷨����Ҫʹ�õ����������̶�16�ֽڣ�CBCģʽ��Ҫ���룬ECB�ɲ���
	 * 
	 * @param[in] pucInData ��������ģ�������16�ı���,
	 * 
	 * @param[in] uiInDataLen �������ĳ���,
	 * 
	 * @param[out] pucOutData ���ܺ��������
	 * 
	 * @param[in out] puiOutDataLen ���ܺ�������ĳ���
	 * 
	 * @return ִ�гɹ����� MW_SUCCESS ��ִ��ʧ��ʱ���ش����롣
	 * 
	 * @remarks
	 */
	public final static native int mw_sym_decrypt(long hHandle, byte[] pcKeyFileName, byte ucAlgoMode, byte[] pucIV,
			byte[] pucInData, int uiInDataLen, byte[] pucOutData, int[] puiOutDataLen);

	/*
	 * !
	 * 
	 * @brief rsa�ǶԳƼ���
	 * 
	 * @param[in] hHandle ħ���豸���
	 * 
	 * @param[in] pcKeyFileName ��Կ�ļ�����
	 * 
	 * @param[in] ucPadMode ���ܵ��㷨���μ��궨�壺MW_RSA_MODE_XXX
	 * 
	 * @param[in] pucInData ������������
	 * 
	 * @param[in] uiInDataLen �����������ݵĳ���
	 * 
	 * @param[out] pucOutData �����������
	 * 
	 * @param[in out] puiOutDataLen ����������ݵĳ���
	 * 
	 * @return ִ�гɹ����� MW_SUCCESS ��ִ��ʧ��ʱ���ش����롣
	 * 
*   @remarks    ע����ucPadModeΪMW_RSA_MODE_NORMALʱ��RSA_1024λ�������������ĳ���Ϊ128,RSA_2048λ������󳤶�Ϊ256; 
*                   ��ucPadModeΪMW_RSA_MODE_PKCS1_V1_5ʱ��  RSA_1024λ�������������ĳ���Ϊ117,RSA_2048λ������󳤶�Ϊ245; 
*/

	public final static native int mw_rsa_encrypt(long hHandle, byte[] pcKeyFileName, byte ucPadMode, byte[] pucInData,
			int uiInDataLen, byte[] pucOutData, int[] puiOutDataLen);

	/*
	 * !
	 * 
	 * @brief rsa�ǶԳƽ���
	 * 
	 * @param[in] hHandle ħ���豸���
	 * 
	 * @param[in] pcKeyFileName ��Կ�ļ�����
	 * 
	 * @param[in] ucPadMode ���ܵ��㷨���μ��궨�壺MW_RSA_MODE_XXX
	 * 
	 * @param[in] pucInData ��������
	 * 
	 * @param[in] uiInDataLen �������ĳ��� ע�����ĳ��ȱ���Ϊ128����256
	 * 
	 * @param[out] pucOutData �������
	 * 
	 * @param[out] puiOutDataLen ������ĳ���
	 * 
	 * @return ִ�гɹ����� MW_SUCCESS ��ִ��ʧ��ʱ���ش����롣
	 * 
	 * @remarks
	 */
	public final static native int mw_rsa_decrypt(long hHandle, byte[] pcKeyFileName, byte ucPadMode, byte[] pucInData,
			int uiInDataLen, byte[] pucOutData, int[] puiOutDataLen);

	/*
	 * !
	 * 
	 * @brief ǩ��
	 * 
	 * @param[in] hHandle ħ���豸���
	 * 
	 * @param[in] pcKeyFileName ��Կ�ļ�����
	 * 
	 * @param[in] ucHashAlg ��ϣ�㷨���ͣ��μ��궨�壺MW_HASH_ALGO_SHA1, MW_HASH_ALGO_SHA256, MW_HASH_ALGO_NONE
	 * 
	 * @param[in] pucMessageData ������Ϣ����
	 * 
	 * @param[in] uiMessageDataLen ������Ϣ���ݳ��� ע��������ݳ��Ȳμ��궨�� MW_INPUT_DATA_LENGTH_MAX 
	 * 
	 * @param[out] pucSignData ���ǩ������
	 * 
	 * @param[out] puiSignDataLen ���ǩ�����ݳ���
	 * 
	 * @return ִ�гɹ����� MW_SUCCESS ��ִ��ʧ��ʱ���ش����롣
	 * 
	 * @remarks
	 */
	public final static native int mw_signature(long hHandle, byte[] pcKeyFileName, byte ucHashAlg,
			byte[] pucMessageData, int uiMessageDataLen, byte[] pucSignData, int[] puiSignDataLen);

	/*
	 * !
	 * 
	 * @brief ��ǩ
	 * 
	 * @param[in] hHandle ħ���豸���
	 * 
	 * @param[in] pcKeyFileName ��Կ�ļ�����
	 * 
	 * @param[in] ucHashAlg ��ϣ�㷨���ͣ��μ��궨�壺MW_HASH_ALGO_XXX
	 * 
	 * @param[in] pucSignData ����ǩ������
	 * 
	 * @param[in] uiSignDataLen ����ǩ�����ݳ���
	 * 
	 * @param[in] pucMessageData ������Ϣ����
	 * 
	 * @param[in] uiMessageDataLen ������Ϣ���ݳ���
	 * 
	 * @return ִ�гɹ����� MW_SUCCESS ��ִ��ʧ��ʱ���ش����롣
	 * 
	 * @remarks
	 */
	public final static native int mw_verify_sign(long hHandle, byte[] pcKeyFileName, byte ucHashAlg,
			byte[] pucSignData, int uiSignDataLen, byte[] pucMessageData, int uiMessageDataLen);

	/*
	 * !
	 * 
	 * @brief HMAC����
	 * 
	 * @param[in] hHandle ħ���豸���
	 * 
	 * @param[in] pcKeyFileName ��Կ�ļ�����
	 * 
	 * @param[in] pucInData ��������
	 * 
	 * @param[in] uiInDataLen �������ݳ���
	 * 
	 * @param[out] pucOutData �������
	 * 
	 * @param[out] puiOutDataLen
	 * ������ݳ��ȣ��μ��궨�壺MW_HMAC_MD5_DIGEST_LENGTH��MW_HMAC_SHA1_DIGEST_LENGTH��MW_HMAC_SHA256_DIGEST_LENGTH
	 * 
	 * @return ִ�гɹ����� MW_SUCCESS ��ִ��ʧ��ʱ���ش����롣
	 * 
	 * @remarks
	 */
	public final static native int mw_hmac_calc(long hHandle, byte[] pcKeyFileName, byte[] pucInData, int uiInDataLen,
			byte[] pucOutData, int[] puiOutDataLen);

	/*
	 * !
	 * 
	 * @brief ����������
	 * 
	 * @param[in] hHandle ħ���豸���
	 * 
	 * @param[in] uiDevPID �豸��Ʒ��
	 * 
	 * @param[in] pucSN Ӳ��Ψһ���к� �̶�����Ϊ��MW_SN_LENGTH����ָ��SN��ʱ������ΪNULL
	 * 
	 * @param[in] pstUpdateFileList
	 * �ļ�·���б�,����ļ������μ��궨��MW_UPDATE_FILE_COUNT_MAX,�����ļ��ĸ���ΪuiFileCount
	 * 
	 * @param[in] uiFileCount �����ļ��ĸ���,��ָ����pstUpdateFileList�еĴ�С
	 * 
	 * @param[out] pucOutPkg ����������
	 * 
	 * @param[in out] puiOutPkgLen ���������ݳ���
	 * 
	 * @return ִ�гɹ����� MW_SUCCESS ��ִ��ʧ��ʱ���ش����롣
	 * 
	 * @remarks
	 */
	public final static native int mw_make_update_pkg(long hHandle, int uiDevPID, byte[] pucSN,
			MW_UPDADE_FILE_CTX[] pstFilePathList, int uiFileCount, byte[] pucOutPkg, int[] puiOutPkgLen);

	/*
	 * !
	 * 
	 * @brief Զ������
	 * 
	 * @param[in] hHandle ħ���豸���
	 * 
	 * @param[in] pucInPkg ����������
	 * 
	 * @param[in] uiInPkgLen ���������ݳ���
	 * 
	 * @return ִ�гɹ����� MW_SUCCESS ��ִ��ʧ��ʱ���ش����롣
	 * 
	 * @remarks
	 */
	public final static native int mw_update(long hHandle, byte[] pucInPkg, int uiInPkgLen);

	/*
	 * !
	 * 
	 * @brief �ָ���������
	 * 
	 * @param[in] hHandle ħ���豸���
	 * 
	 * @return ִ�гɹ����� MW_SUCCESS ��ִ��ʧ��ʱ���ش����롣
	 * 
	 * @remarks ���豸�ָ�����״̬����Ĭ��PID��Ĭ��PIN���ļ�ϵͳ��ա��迪����Ȩ��
	 */
	public final static native int mw_restore_factory(long hHandle);

	/*
	 * !
	 * 
	 * @brief ���������
	 * 
	 * @param[in] hHandle ħ���豸���
	 * 
	 * @param[in] uiErrorCode ������
	 * 
	 * @param[in] uiLanguageID ��������ID
	 * 
	 * @return ִ�гɹ����� MW_SUCCESS ��ִ��ʧ��ʱ���ش����롣
	 * 
	 * @remarks
	 */
	public final static native byte[] mw_get_error_desc(int uiErrorCode, int uiLanguageID);
	
	//�����ķ�ʽ
	/** �����  */
	public final static int MW_OPEN_SHARE_MODE                                 = 0;
	/** ��ռ�� */
	public final static int MW_OPEN_EXCLUSIVE_MODE                             = 1;
	//============================================================
	//  �㷨���ͱ�ʾ
	//============================================================
	/** �㷨����AES */
	public final static int MW_ALG_TYPE_AES                                    = 0x00;
	/** �㷨����DES */
	public final static int MW_ALG_TYPE_DES                                    = 0x01;
	/** �㷨����TDES */
	public final static int MW_ALG_TYPE_TDES                                   = 0x02;
	/** �㷨����ECC */
	public final static int MW_ALG_TYPE_ECC                                    = 0x10;
	/** �㷨����RSA */
	public final static int MW_ALG_TYPE_RSA                                    = 0x11;
	//============================================================
	//  �Գ��㷨ģʽ
	//============================================================
	/** �Գ��㷨ģʽ ECB mode */
	public final static int MW_SYM_ALGO_MODE_ECB                               = 0x00;
	/** �Գ��㷨ģʽ CBC mode */
	public final static int MW_SYM_ALGO_MODE_CBC                               = 0x01;
	//============================================================
	//  �ļ�������غ궨��
	//============================================================
	/** �������ļ� */
	public final static int MW_FILE_TYPE_BINARY                                = 0;
	/** ��Կ�ļ� */
	public final static int MW_FILE_TYPE_KEY                                   = 2;
	/** ��Կ�ļ�Ȩ������ */
	public final static int MW_FILE_PRIV_TYPE_USE                              = 0x00;
	/** �����ļ��û�ֻ��Ȩ������ */
	public final static int MW_FILE_PRIV_TYPE_READ                             = 0x01;
	/** �����ļ��û���дȨ������ */
	public final static int MW_FILE_PRIV_TYPE_READ_WRITE                       = 0x02;
	//============================================================
	//  ��ȡ�豸��Ϣ�궨�� 
	//============================================================
	/** ��ȡPID     ����4�ֽ�PID */
	public final static int MW_GET_INFO_ITEM_PID  	                           = 0x00;
	/** ��ȡ���к�  ����16�ֽڻ�ȡ���к� */
	public final static int MW_GET_INFO_ITEM_SN                                = 0x01;
	/** ��ȡ�������� ����4�ֽ��������� */
	public final static int MW_GET_INFO_ITEM_PRODUCE_DATE                      = 0x02;
	/** ��ȡ������   ����4�ֽ������� */
	public final static int MW_GET_INFO_ITEM_ALL_CAPACITY  	                   = 0x03;
	/** ��ȡʣ������ ����4�ֽ�ʣ������ */
	public final static int MW_GET_INFO_ITEM_FREE_CAPACITY                     = 0x04;
	/** ��ȡ�豸�汾 ����4�ֽ��豸�汾 */
	public final static int MW_GET_INFO_ITEM_VERSION                           = 0x05;
	/** ��ȡ��Ǻ�   ���ص�һ�ֽ�Ϊ��Ǻų��ȣ���������Ǻ����ݡ�ע�����256�ֽڡ� */
	public final static int MW_GET_INFO_ITEM_SHELL_SN                          = 0x06;
	/** ��ȡ����״̬ ����1�ֽڵı�ʶ���μ��궨��MW_STATUS_FLAG_PID��MW_STATUS_FLAG_PIN */
	public final static int MW_GET_INFO_ITEM_STATUS                            = 0x07;
	/** ��ȡ�豸Զ��������      ����4 �ֽ��豸Զ�������� */
	public final static int MW_GET_INFO_ITEM_UPDATE_CODE                       = 0x08;
	/** ��ȡԶ��������������ʶ  ����1�ֽڱ�ʶ */
	public final static int MW_GET_INFO_ITEM_UPDATE_CTRL_CODE                  = 0x09;
	/** ��ȡ������Ϣ������MW_DEV_ALL_ITEMINFO�ṹ��Ĵ�С */
	public final static int MW_GET_INFO_ITEM_ALL                               = 0xFF;


	/** Ĭ��״̬ */
	public final static int MW_STATUS_FLAG_DEFAULT                             = 0x00;
	/** �Ѿ�����PID״̬ */
	public final static int MW_STATUS_FLAG_PID                                 = 0x01;
	/** �Ѿ��޸�PIN״̬ */
	public final static int MW_STATUS_FLAG_PIN                                 = 0x02;
	//============================================================
	//  ������
	//============================================================
	/** ��λ�豸�������½״̬�� */
	public final static int MW_CTRL_CODE_ITEM_RESET                            = 0x00;
	/** ����LED��  ע�����ݵ�һ�ֽڷ�0��ʶLED����0��ʾ�� */
	public final static int MW_CTRL_CODE_ITEM_LED                              = 0x01;
	//============================================================
	//  PIN���Ͷ���
	//============================================================
	/** Ĭ��PIN���� */
	public final static int MW_PIN_TYPE_NONE                                   = 0x00;
	/** �û�PIN���� */
	public final static int MW_PIN_TYPE_USER                                   = 0x01;
	/** ������PIN���� */
	public final static int MW_PIN_TYPE_DEVELOPER                              = 0x02;
	/** �û�PIN���� */
	public final static int MW_PIN_LENGTH_USER                                 = 8;
	/** ������PIN���� */
	public final static int MW_PIN_LENGTH_DEVELOPER                            = 24;
	//============================================================
	//  RSA����ģʽ����
	//============================================================
	/** RSA ��������ģʽ */
	public final static int MW_RSA_MODE_NORMAL                        = 0x00;
	/** RSA PKCS1 V1_5 ������ģʽ */
	public final static int MW_RSA_MODE_PKCS1_V1_5                    = 0x01;
	//============================================================
	//  RSA�������ݳ���
	//============================================================
	/** RSA-1024�������ݳ��� */
	public final static int MW_RSA_1024_BYTE_SIZE                     = 128;
	/** RSA-2048�������ݳ��� */
	public final static int MW_RSA_2048_BYTE_SIZE                     = 256;
	//============================================================
	//  ǩ��hash�㷨����
	//============================================================
	/** ǩ��MD5 hash�㷨 */
	public final static int MW_HASH_ALGO_MD5                          = 0x00;
	/** ǩ��SHA1 hash�㷨 */
	public final static int MW_HASH_ALGO_SHA1                         = 0x01;
	/** ǩ��SHA256 hash�㷨 */
	public final static int MW_HASH_ALGO_SHA256                       = 0x02;
	/** ǩ��ʱ����Hash���㣬�ɵ��ú������Լ���Hash���� */
	public final static int MW_HASH_ALGO_NONE                         = 0xFF;



	//============================================================
	//  ��������غ궨��
	//============================================================
	/** һ�������������������ļ����� */
	public final static int MW_UPDATE_FILE_COUNT_MAX                  = 50;
	/** �����ļ�ʱ�Ĳ�����־ ɾ���ļ� */
	public final static int MW_UPDATE_MOD_FLAG_DELETE                 = 0x00000002;
	/** �����ļ�ʱ�Ĳ�����־ д�ļ� */
	public final static int MW_UPDATE_MOD_FLAG_WRITE                  = 0x00000010;
	/** �����ļ�ʱ�Ĳ�����־ �����ļ� */
	public final static int MW_UPDATE_MOD_FLAG_CREATE                 = 0x00000020;
	//============================================================
	//  �����Ժ궨��
	//============================================================
    /** Ĭ������ */
	public final static int MW_LANGUAGE_ID_DEFAULT                    = 0;

	/** ���� */
	public final static int MW_LANGUAGE_ID_CH                         = 1;
	/** Ӣ�� */
	public final static int MW_LANGUAGE_ID_EN                         = 2;
	
	//============================================================
	//  ħ����ش�С�궨��
	//============================================================
	/** ���·�� */
	public final static int MW_PATH_LEN                               = 1024;
	/** �ԳƼ����㷨�������� */
	public final static int MW_SYM_ALG_IV_LENGTH                      = 16;
	/** �豸Ψһ���кų��� */
	public final static int MW_SN_LENGTH                              = 16;
	/** �ļ�����󳤶� */
	public final static int MW_FILE_NAME_LENGTH_MAX                   = 16;
	/** HMAC MD5���������� */
	public final static int MW_HMAC_MD5_DIGEST_LENGTH                 = 16;
	/** HMAC SHA1���������� */
	public final static int MW_HMAC_SHA1_DIGEST_LENGTH                = 20;
	/** HMAC SHA256���������� */
	public final static int MW_HMAC_SHA256_DIGEST_LENGTH              = 32;
	/** һ�����ת�����ݳ��� */
	public final static int MW_INPUT_DATA_LENGTH_MAX                  = 1900;
	//��������󳤶Ⱥ궨��
	/** ��������볤�� */
	public final static int MW_SEED_LENGTH_MAX                        = 32;
	/** ��С�����볤�� */
	public final static int MW_SEED_LENGTH_MIN                        = 4;
	public final static int MW_ENUM_DEVICE_MAX_COUNT                  = 128;
	
	//============================================================
	//  ��������ֵ����
	//============================================================
	public final static int MW_SUCCESS                                         = 0x00000000;                  // Success.
	public final static int MW_ERROR_INVALID_HANDLE                            = 0x00000001;                  // Handle may be NULL.
	public final static int MW_ERROR_INVALID_PARAMETER                         = 0x00000002;                  // Invalid parameter.
	public final static int MW_ERROR_NOT_ENOUGH_MEMORY                         = 0x00000003;                  // Not enough storage is available to process this command.
	public final static int MW_ERROR_NO_DEVICE                                 = 0x00000004;                  // No device or no specific device on the pc.
	public final static int MW_ERROR_TIMEOUT                                   = 0x00000005;                  // Time out.
	public final static int MW_ERROR_UNSUPPORTED_FLAG                          = 0x00000006;                  // A unsupported flag was given to the APIs.
	public final static int MW_ERROR_INSUFFICIENT_BUFFER                       = 0x00000007;                  // Buffer is too small to hold the data.
	public final static int MW_ERROR_EXCHG_MEMORY_NOT_FOUND                    = 0x00000008;                  // Specific share memory is not found.
	public final static int MW_ERROR_SYSTEM_FILE_NOT_FOUND                     = 0x00000009;                  // File in system not found.
	public final static int MW_ERROR_SYSTEM_FILE_INVALID_ACCESS                = 0x0000000a;                  // Can not access system file.
	public final static int MW_ERROR_FILE_EXISTS                               = 0x0000000b;                  // Specific file is exist.
	public final static int MW_ERROR_FILE_NOT_FOUND                            = 0x0000000c;                  // Specific file is not found.
	public final static int MW_ERROR_NO_PRIVILEGE                              = 0x0000000d;                  // The operation is required a high level privilege.
	public final static int MW_ERROR_WRONG_PASSWORD                            = 0x0000000e;                  // Password is incorrect.
	public final static int MW_ERROR_PASSWORD_LOCKED                           = 0x0000000f;                  // Password is locked.
	public final static int MW_ERROR_FUNCTION_NOT_SUPPORTED                    = 0x00000011;                  // the function is not supported
	public final static int MW_ERROR_COMMUNICATION                             = 0x00000015;                  // Communication error.
	public final static int MW_ERROR_UNSUPPORTED_PASSWORD_TYPE                 = 0x00000016;                  // A unsupported password type was given to APIs.
	public final static int MW_ERROR_WRONG_PASSWORD_LENGTH                     = 0x00000017;                  // Password length is not correct, developer password is 24 bytes, user password is 8 bytes.
	public final static int MW_ERROR_ALREADY_EXCLUSIVELY_LOGIN                 = 0x00000018;                  // The device is already exclusively login, can not login with share mode.
	public final static int MW_ERROR_ALREADY_SHARED_LOGIN                      = 0x00000019;                  // The device is already shared login, can not login with exclusively mode.                 
	public final static int MW_ERROR_ALREADY_TEMP_EXCLUSIVELY_USING            = 0x0000001a;                  // The handle is already temporary using.
	public final static int MW_ERROR_NOT_TEMP_EXCLUSIVELY_USING                = 0x0000001b;                  // The handle is not temporary using.
	public final static int MW_ERROR_TOO_MUCH_DATA                             = 0x0000001c;                  // The massage reply function has a data length limit.                 
	public final static int MW_ERROR_INSUFFICIENT_DEVICE_SPACE                 = 0x0000001e;                  // The device space is insufficient.
	public final static int MW_ERROR_DEVICE_FILESYSTEM_ERROR                   = 0x0000001f;                  // Device file system error.
	public final static int MW_ERROR_FILE_OUT_RANGE                            = 0x00000020;                  // Device file is out range.
	public final static int MW_ERROR_UNSUPPORTED_FILE_TYPE                     = 0x00000021;                  // an unsupported file type is given to EV APIs.
	public final static int MW_ERROR_FILE_OFFSET_MUST_BE_ZERO                  = 0x00000022;                  // When read or write a key file, offset must be 0.  
	public final static int MW_ERROR_BAD_EXECUTIVE_FILE_FORMAT                 = 0x00000023;                  // the executive file format is incorrect.
	public final static int MW_ERROR_OPEN_TOO_MANY_DEVICE_FILES                = 0x00000024;                  // Open too many device files.
	public final static int MW_ERROR_INVALID_DEVICE_FILE_HANDLE                = 0x00000025;                  // device file handle is incorrect.
	public final static int MW_ERROR_FILE_NOT_FINISHED                         = 0x00000026;                  // The file is not finish.
	public final static int MW_ERROR_BAD_FILENAME                              = 0x00000027;                  // Filename is incorrect.
	public final static int MW_ERROR_BAD_NAME                                  = 0x00000028;                  // The filename, directory name, or volume label syntax is incorrect.
	public final static int MW_ERROR_DEVICE_TIMER                              = 0x00000029;                  // Device timer error.
	public final static int MW_ERROR_NO_EXECUTIVE_FILE_RUNNING                 = 0x0000002a;                  // No process is running in the device. 
	public final static int MW_ERROR_DEVICE_USER_BUFFER_FULL                   = 0x0000002b;                  // Can not send data when device user buffer is full.
	public final static int MW_ERROR_DEVICE_USER_BUFFER_EMPTY                  = 0x0000002c;                  // Can receive data when device user buffer is empty.
	public final static int MW_ERROR_DEVICE_MSG_NOT_REPLIED                    = 0x0000002d;                  // Device need a message reply first.
	public final static int MW_ERROR_DEVICE_DUMMY_MSG                          = 0x0000002e;                  // Device not need a message reply.
	public final static int MW_ERROR_DEVICE_MEMORY_ADDR                        = 0x0000002f;                  // Device memory address error.
	public final static int MW_ERROR_DEVICE_MEMORY_LENGTH                      = 0x00000030;                  // Device memory length error.
	public final static int MW_ERROR_CONTROL_CODE                              = 0x00000031;                  // Give an unsupported control code.
	public final static int MW_ERROR_UNKNOW_COMMAND                            = 0x00000032;                  // Give an unsupported command to device.
	public final static int MW_ERROR_INVALID_COMMAND_PARAMETER                 = 0x00000033;                  // Command parameter error.
	public final static int MW_ERROR_COMMAND_DATA_LENGTH                       = 0x00000034;                  // Command data length error.
	public final static int MW_ERROR_DEVICE_BUFFER_FULL                        = 0x00000035;                  // Device buffer is full.
	public final static int MW_ERROR_EXECUTIVE_FILE_RUNNING                    = 0x00000036;                  // When a process is running in the device, some operation unsupported.
	public final static int MW_ERROR_NO_DEVICE_MESSAGE                         = 0x00000037;                  // No device message.
	public final static int MW_ERROR_LOGIN_COUNT                               = 0x00000038;                  // Device Login count error.
	public final static int MW_ERROR_KEYEXCHANGE_FAILED                        = 0x00000039;                  // Communication key exchange error.
	public final static int MW_ERROR_BAD_COMMUNICATION_KEY                     = 0x0000003a;                  // Communication key is incorrect.
	public final static int MW_ERROR_BAD_DEVICE_TIME                           = 0x0000003b;                  // Device time error.
	public final static int MW_ERROR_BAD_DEVICE_INFOMATION                     = 0x0000003c;                  // Device information error.
	public final static int MW_ERROR_BAD_COMMAND_SEQUENCE                      = 0x0000003d;                  // Command sequence is not right.
	public final static int MW_ERROR_COMMUNICATION_DATA_LENGTH                 = 0x0000003e;                  // Communication data length error.
	public final static int MW_ERROR_ECC                                       = 0x0000003f;                  // Device ECC crypt error.
	public final static int MW_ERROR_BAD_UPDATE_DATA_LENGTH                    = 0x00000040;                  // Update data length is incorrect.
	public final static int MW_ERROR_UPDATE_STATE                              = 0x00000042;                  // Update state is incorrect.
	public final static int MW_ERROR_UPDATE_KEY_NOT_ENABLE                     = 0x00000043;                  // When use remote update, remote update key must set first.
	public final static int MW_ERROR_LOCKED_FOREVER                            = 0x00000044;                  // Device is locked forever.
	public final static int MW_ERROR_LOCKED_TEMPORARY                          = 0x00000045;                  // Device is locked temporary.
	public final static int MW_ERROR_DEVICE_UNLOCKED                           = 0x00000046;                  // Device is not locked.
	public final static int MW_ERROR_DEVICE_FLASH                              = 0x00000047;                  // Device flash error.
	public final static int MW_ERROR_DEVICE_ERROR                              = 0x00000048;                  // Device error.
	public final static int MW_ERROR_TOO_MANY_DEVICE                           = 0x00000049;                  // Device numbers error, should not be larger than 128.
	public final static int MW_ERROR_DEVICE_NOT_ENOUGH_USER_MEMORY             = 0x0000004a;                  // There is no enough memory for user code.
	public final static int MW_ERROR_FAKE_DEVICE                               = 0x0000004b;                  // Device is fake.
	public final static int MW_ERROR_DEVICE_BLK_OUT_RANGE                      = 0x0000004c;                  // Device bulk read or write out range.
	public final static int MW_ERROR_DEVICE_FS_ERR_BAK_ERROR                   = 0x0000004d;                  // Device backup error.
	public final static int MW_ERROR_DEVICE_TMR_ERR_ADJUST_TIME_TIMEOUT        = 0x0000004e;                  // Adjust time is time out.
	public final static int MW_ERROR_DEVICE_EXCH_ERROR                         = 0x0000004f;                  // Exchange memory error.
	public final static int MW_ERROR_DEVICE_AES_ERR                            = 0x00000050;                  // Device AES error.
	public final static int MW_ERROR_DEVICE_HASH_ERR_UNSUPPORTED_ALGO          = 0x00000051;                  // Unsupported hash algo.
	public final static int MW_ERROR_DEVICE_BAD_PRIVATE_KEY                    = 0x00000052;                  // Bad private key.
	public final static int MW_ERROR_DEVICE_BAD_PUBLIC_KEY                     = 0x00000053;                  // Bad public key.
	public final static int MW_ERROR_DEVICE_BAD_SYMMETRIC_KEY                  = 0x00000054;                  // Bad symmetric key.
	public final static int MW_ERROR_DEVICE_BAD_SIGNATURE                      = 0x00000055;                  // Bad signature.
	public final static int MW_ERROR_DEVICE_KEY_ERR_BAD_ALGO                   = 0x00000056;                  // Bad algo.
	public final static int MW_ERROR_DEVICE_LOGO_ERR_LOGO                      = 0x00000057;                  // Bad logo.
	public final static int MW_ERROR_EXEC_PARAM_TOO_LONG                       = 0x00000058;                  // Execute parameter data is too long.
	public final static int MW_ERROR_OPEN_ERROR                                = 0x00000059;                  // Open device error.
	public final static int MW_ERROR_LOAD_SYS_MODULE_ERROR                     = 0x0000005A;                  // Load system module error.
	public final static int MW_ERROR_SYS_MODULE_FUNCTION_ERROR                 = 0x0000005B;                  // System module function address error.
	public final static int MW_ERROR_RSA                                       = 0x0000005C;                  // Device RSA crypt error.
	public final static int MW_ERROR_KEY                                       = 0x0000005D;                  // Crypt Key error.
	public final static int MW_ERROR_DEVICE_EXEC_ERR_UNALIGNED_MEM_ADDR        = 0x0000005E;                  // Unaligned memory address.
	public final static int MW_ERROR_DEVICE_EXEC_ERR_STACK_OVERFLOW            = 0x0000005F;                  // User stack overflow.
	public final static int MW_ERROR_DEVELOPER_ID_MISMATCH                     = 0x00000060;                  // Developer ID not match.
	public final static int MW_ERROR_LM_GENERAL_ERROR                          = 0x00000061;                  // LM return data format wrong
	public final static int MW_ERROR_LM_UNSUPPORTED_CERT_TYPE                  = 0x00000062;                  // Unsupported certificate type
	public final static int MW_ERROR_LM_UNSUPPORTED_UPDATE_OBJECT_TYPE         = 0x00000063;                  // Unsupported object type to update
	public final static int MW_ERROR_LM_UPDATE_PKG_FORMAT_WRONG                = 0x00000064;                  // Update Package format wrong
	public final static int MW_ERROR_CERT                                      = 0x00000065;                  // Certificates wrong
	public final static int MW_ERROR_DEX_NOT_LOADED                            = 0x00000066;                  // Dynamic Executive not loaded to the lock
	public final static int MW_ERROR_SYSAPP_NOT_FOUND                          = 0x00000067;                  // System Application Not Found
	public final static int MW_ERROR_UNSUPPORTED_OBJECT_TYPE                   = 0x00000068;                  // Unsupported update object type
	public final static int MW_ERROR_UPDATE_BLOCK_LENGTH                       = 0x00000069;                  // update block length wrong
	public final static int MW_ERROR_UPDATE_SEED_NOT_FOUND                     = 0x0000006A;                  // update seed not found
	public final static int MW_ERROR_HEAP                                      = 0x0000006B;                  // The system heap in the lock crashed
	public final static int MW_ERROR_DEX_OUT_OF_RANGE                          = 0x0000006C;                  // Dynamic Executive out of range
	public final static int MW_ERROR_DEX_TOO_LARGE                             = 0x0000006D;                  // Dynamic Executive too large
	public final static int MW_ERROR_UPDATE_INFO                               = 0x0000006E;                  // Update information wrong
	public final static int MW_ERROR_DEVICE_STACK_OVERFLOW                     = 0x0000006F;                  // The system stack in the lock crashed
	public final static int MW_ERROR_COMMUNICATION_CRYPT                       = 0x00000070;                  // Communication Encrypt wrong
	public final static int MW_ERROR_BAD_UPDATE_PKG                            = 0x00000071;                  // Bad Update Package
	public final static int MW_ERROR_UPDATE_PKG_VERSION_LOW                    = 0x00000072;                  // Update Package version low
	public final static int MW_ERROR_UPDATE_OBJECT_TYPE                        = 0x00000073;                  // Update Object type wrong
	public final static int MW_ERROR_UPDATE_DEVELOPER_ID                       = 0x00000074;                  // wrong developer id
	public final static int MW_ERROR_UPDATE_FILE_TYPE                          = 0x00000075;                  // wrong file type
	public final static int MW_ERROR_NO_ADJUST_TIME_REQUEST                    = 0x00000076;                  // no adjust time request have been get
	public final static int MW_ERROR_CERT_REVOKED                              = 0x00000077;                  // Certificates revoked
	public final static int MW_ERROR_WRONG_CERT_SLOT                           = 0x00000078;                  // Certificates slot number wrong
	public final static int MW_ERROR_DEVICE_HASH_ERR_NOT_MATCH                 = 0x00000079;                  // Hash value not match
	public final static int MW_ERROR_BAD_RND                                   = 0x0000007A;                  // Bad Random
	public final static int MW_ERROR_RTC                                       = 0x0000007B;                  // RTC error
	public final static int MW_ERROR_RTC_POWER                                 = 0x0000007C;                  // RTC power drop
	
	public final static int MW_ERROR_NO_PID_DEVICE                             = 0x00002001;                  // �������Ҳ���ָ����PIDħ����
	public final static int MW_ERROR_RET_DATA_FORMAT                           = 0x00002002;                  // �������ݸ�ʽ����ȷ
	public final static int MW_ERROR_FILE_NAME_LEN                             = 0x00002003;                  // �ļ������Ȳ���ȷ
	public final static int MW_ERROR_BLOCK_DATA                                = 0x00002004;                  // ���ݿ����
	public final static int MW_ERROR_FILE_PROPERTY_PARAM                       = 0x00002005;                  // �ļ����Բ�������
	public final static int MW_ERROR_FILE_COUNT_MAX                            = 0x00002006;                  // ʵ���ļ���������Ԥ����ĸ���
	public final static int MW_ERROR_MALLOC_MEMORY                             = 0x00002007;                  // �����ڴ�ʧ��
    public final static int MW_ERROR_PIN_LENGTH                                = 0x00002008;                  // PIN�����д���
    public final static int MW_ERROR_UPDATA_PKG_PID                            = 0x00002009;                  // ������PID������PID��һ��
	
	public final static int MW_ERROR_ENTRY_TYPE                                = 0x0000A001;                  // entry ���ʹ���
	public final static int MW_ERROR_ENTRY_FLAG                                = 0x0000A002;                  // entry ��ʶ����
	public final static int MW_ERROR_OPCODE                                    = 0x0000A003;                  // ��������
	public final static int MW_ERROR_PARAM                                     = 0x0000A004;                  // �������
	public final static int MW_ERROR_DATA_LENGTH                               = 0x0000A005;                  // �������ݳ���
	public final static int MW_ERROR_PIN                                       = 0x0000A006;                  // ����PIN ��
	public final static int MW_ERROR_DEVICE_BLOCK                              = 0x0000A007;                  // �豸PIN ������
	public final static int MW_ERROR_PRIVILEGE                                 = 0x0000A008;                  // û��Ȩ��
	public final static int MW_ERROR_DATA_INFO                                 = 0x0000A009;                  // ���������ݴ���
	public final static int MW_ERROR_HASH_TYPE                                 = 0x0000A00A;                  // ����HASH ����
	public final static int MW_ERROR_ALG_MODE                                  = 0x0000A00B;                  // �����ģʽ
	public final static int MW_ERROR_INIT_EVT                                  = 0x0000A00C;                  // ��ʼ�������������
	public final static int MW_ERROR_ALG_TYPE                                  = 0x0000A00D;                  // �����㷨����
	public final static int MW_ERROR_BIT_SIZE                                  = 0x0000A00E;                  // �������Կλ��С
	public final static int MW_ERROR_FILE_TYPE                                 = 0x0000A00F;                  // ������ļ�����
	public final static int MW_ERROR_NO_INIT_PID                               = 0x0000A010;                  // PIDδ��ʼ��
	public final static int MW_ERROR_NO_INIT_PIN                               = 0x0000A011;                  // PIN��δ�޸�
	public final static int MW_ERROR_NO_VALIDATE_PKG                           = 0x0000A012;                  // ��Ч������
	public final static int MW_ERROR_AUTH_STAGE                                = 0x0000A013;                  // Ȩ�޼۶δ���
	public final static int MW_ERROR_FILE_PRIVILEGE                            = 0x0000A014;                  // �ļ�Ȩ�޴���
	public final static int MW_ERROR_FILE_RANGE                                = 0x0000A015;                  // �ļ���Χ����
	public final static int MW_ERROR_KEY_FILE                                  = 0x0000A016;                  // �ļ���Կ����
	public final static int MW_ERROR_REQ_IDENTIFY                              = 0x0000A017;                  // ������֤����
	public final static int MW_ERROR_INVALID_SEED                              = 0x0000A018;                  // ��Ч���Ӵ���
	public final static int MW_ERROR_SEED_CHECK                                = 0x0000A019;                  // ����У�����
	public final static int MW_ERROR_UPDATE_HASH                               = 0x0000A01A;                  // ������ϣ����
	public final static int MW_ERROR_UPDATE_PID                                = 0x0000A01B;                  // ����PID����
	public final static int MW_ERROR_UPDATE_INDEX                              = 0x0000A01C;                  // ������������
	public final static int MW_ERROR_UPDATE_TIME                               = 0x0000A01D;                  // ����ʱ�����
	public final static int MW_ERROR_UPDATE_SERIAL_NUM                         = 0x0000A01E;                  // �������кŴ���
	public final static int MW_ERROR_UPDATE_SN                                 = 0x0000A01F;                  // ����SN����
	public final static int MW_ERROR_UPDATE_ORDER                              = 0x0000A020;                  // ����˳�����
	public final static int MW_ERROR_UPDATE_OPERATE                            = 0x0000A021;                  // ������������
	public final static int MW_ERROR_KEY_TYPE                                  = 0x0000A022;                  // �ؼ������ʹ���
	public final static int MW_ERROR_OTHER                                     = 0x0000AFFF;                  // ��������
}


class MW_DEVICE_INFO_CTX {
	/** ��ƷID */
    public int    uiPID;
	/** Ψһ���к� */
    public byte[]   ucSerialNum = new byte[MowayJava.MW_SN_LENGTH];
	/** �豸��·�� */
    public byte[]   ucDevPath = new byte[MowayJava.MW_PATH_LEN];
	/** ͨѶЭ�� */
    public int    uiProtocol;  
	/** Mac OS ϵͳ LocationID */
    public int    uiLocationID;
}

class MW_FILE_PROPERTY {
	/** �ó�Ա������ʱ���� */
	public byte   ucValidate; 
	/** �ļ����ͣ��������ļ�������Կ�ļ��� */
	public byte   ucType;   
	/** �ļ�Ȩ�� */
	public short  usPrivilege;
	/** �ļ���С */
	public int    uiSize;    
	/** ����ʱ�� */
	public int    uiTime;   
	/** �ļ����� */
	public byte[] acName = new byte[MowayJava.MW_FILE_NAME_LENGTH_MAX + 1]; 
}

class MW_DEVICE_STATUS_CTX {
	/** ϵͳʱ�� */
	public int    uiSysTime;
	/** ϵͳ״̬ */
	public int    uiSysStatus;
	/** ���� */
	public short  usReserved;
	/** ��¼״̬ */
	public byte   ucLoginStatus;
	/** �Ƶ�״̬ */
	public byte   ucLedStatus;
}

class MW_UPDADE_FILE_CTX {
	///** �ļ����Խṹ�� */ //MW_FILE_PROPERTY stFileProperty;
	//Ƕ�׽ṹ���޷�ʹ��JNI�ӿڻ�ȡ��������ݲ����ת������ʱ�� MW_UPDADE_FILE_CTX ��ʹ��ƽ��չ���ṹ�ķ�ʽ����shijc 2019-3-15
	///** �ļ����Խṹ�壨��ʼ�� */
	/** �ó�Ա������ʱ���� */
	public byte   ucValidate; 
	/** �ļ����ͣ��������ļ�������Կ�ļ��� */
	public byte   ucType;   
	/** �ļ�Ȩ�� */
	public short  usPrivilege;
	/** �ļ���С */
	public int    uiSize;    
	/** ����ʱ�� */
	public int    uiTime;   
	/** �ļ����� */
	public byte[] acName = new byte[MowayJava.MW_FILE_NAME_LENGTH_MAX + 1];
	///** �ļ����Խṹ�壨������ */
	/** �ļ�·�� */
	//public byte[] filePath = new byte[MowayJava.MW_PATH_LEN]; 
	/** �����ļ����������� */
	public byte[] pData; 
	/** �����ļ������ݴ�С */
	public int uiDateSize;
}

class MW_DEVICE_ALL_ITEM_INFO {
	/** ��Ʒ�� */
	public int uiPID;
	/** Ψһ���к� */
	public byte[] acSN = new byte[MowayJava.MW_SN_LENGTH];
	/** ����ʱ�� */
	public int uiProduceDate;
	/** ������ */
	public int uiAllCapacity;
	/** �������� */
	public int uiFreeCapacity;
	/** �汾�� */
	public int uiVersion;
	/** ������ */
	public int uiUpdateCode;
	/** ״̬ */
	public byte ucStatus;
	/** ���������� */
	public byte ucUpdateCtrlCode;
	/** ���� */
	public short reserved;
}
