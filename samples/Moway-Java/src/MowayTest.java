import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.Arrays;

public class MowayTest {
	private static String DEVELOPER_PIN = "000000000000000000000000";  // 加密锁出厂默认PIN 000000000000000000000000 ，此处本例设置为已设置后的PIN码，开发者初始化新锁前需要更改此值。
	private static String USER_PIN = "00000000";
	private static String USER_RW_FILENAME = "userwt.evd";  		// 读写文件
	private static String USER_RD_FILENAME = "userrd.evd";  		// 只读文件
	private static String USER_KEY_AES128_FILENAME = "userkeyaes.evd";     	// key文件
	private static String USER_KEY_HMAC_MD5_FILENAME = "userkeymd5.evd";    // key文件
	
	private static String UPDATE_PKG_UPDATE_FILENAME = "userrd.evd";
	private static String UPDATE_PKG_UPDATE_FILE_DATA = "update_pkg update file data";
	
	private static String UPDATE_PKG_CREATE_FILENAME = "userrdpkg.evd";
	private static String UPDATE_PKG_CREATE_FILE_DATA = "update_pkg create file data";
	
	private static String UPDATE_PKG_FILENAME = "update_pkg.evd";
	
	private static byte[] stringToCCharArray(String str){
		// 将 string 的内容转换为 byte[]，C部分接口需传递char数组，C接口内部定义char为1个字节，在Java接口中声明参数类型为byte[]，
		// 需要保证byte[]最后一个字符的内容为0，否则JNI封装动态库内部无法正确获取传参数据。
		byte[] byte_str = new byte[str.length() + 1];
		for (int i = 0; i < str.length(); i++) {
			byte_str[i] = str.getBytes()[i];
		}
		return byte_str;
	}

	public static void main(String[] args) {
		// TODO Auto-generated method stub
		// 1.演示基本操作（枚举锁、打开锁）
		helloMoway();
		
		// 2. 开发商设置锁
		developerInitDevice();
		// 获取加密锁信息（部分信息不需要验证PIN即可获取）
		getDeviceInfo();
		
		// 3. 用户使用
		UserUseDevice();
		
		// 4. 升级包
		// 4.1 开发商生成更新锁内文件的升级包
		developerMakeUpdatePkg();
		// 4.2 用户导入升级包更新锁内文件
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

		// 枚举当前计算机插入的魔锐加密锁
		int r = MowayJava.mw_enum(pDevInfoList, uiDevListCount, puiDevCount);
		if (r != MowayJava.MW_SUCCESS){
			System.out.println(r);
		}
		
		// 打开枚举到的第一把加密锁
		int uiShareMode = MowayJava.MW_OPEN_EXCLUSIVE_MODE;
		long[] phHandle = new long[1];
		r = MowayJava.mw_open(pDevInfoList[0], uiShareMode, phHandle);
		if (r != MowayJava.MW_SUCCESS){
			System.out.println(r);
		}
		
		// PIN验证（开发商权限）
		long hHandle = phHandle[0];
		byte ucPinType = MowayJava.MW_PIN_TYPE_DEVELOPER;
		byte[] devPIN = stringToCCharArray(DEVELOPER_PIN);
		r = MowayJava.mw_verify_pin(hHandle , ucPinType , devPIN);
		if (r != MowayJava.MW_SUCCESS){
			System.out.println(r);
		}
		
		// 关闭加密锁句柄
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

		// 枚举当前计算机插入的魔锐加密锁
		int r = MowayJava.mw_enum(pDevInfoList, uiDevListCount, puiDevCount);
		if (r != MowayJava.MW_SUCCESS){
			System.out.println(r);
		}
		
		// 打开枚举到的第一把加密锁
		long[] phHandle = new long[1];
		r = MowayJava.mw_open(pDevInfoList[0], MowayJava.MW_OPEN_EXCLUSIVE_MODE, phHandle);
		if (r != MowayJava.MW_SUCCESS){
			System.out.println(r);
		}
		long hHandle = phHandle[0];		
		
		// 获取加密锁状态
		MW_DEVICE_STATUS_CTX pstDeviceStatusCtx = new MW_DEVICE_STATUS_CTX();
		r = MowayJava.mw_get_device_status(hHandle, pstDeviceStatusCtx);
		if (r == MowayJava.MW_SUCCESS) {
			System.out.println("mw_get_device_status success, uiSysTime = " + pstDeviceStatusCtx.uiSysTime);
		} else {
			System.out.println("mw_get_device_status failed, r = " + r);
		}
		
		// 获取设备信息（所有）
		MW_DEVICE_ALL_ITEM_INFO pstDeviceAllInfo = new MW_DEVICE_ALL_ITEM_INFO();
		r = MowayJava.mw_get_device_all_info(hHandle, pstDeviceAllInfo);
		if (r == MowayJava.MW_SUCCESS) {
			System.out.println("mw_get_device_all_info success.");
		} else {
			System.out.println("mw_get_device_all_info failed, r = " + r);
		}
		
		byte[] buffer = new byte[128];
		int[] buffer_len_array = new int[1]; 
		// 获取单个信息
		buffer_len_array[0] = buffer.length;
		r = MowayJava.mw_get_device_info(hHandle, (byte)MowayJava.MW_GET_INFO_ITEM_STATUS, buffer, buffer_len_array);
		if (r == MowayJava.MW_SUCCESS) {
			byte status = buffer[0];
			System.out.println("MW_GET_INFO_ITEM_STATUS:" + status);
		}
		
		buffer_len_array[0] = buffer.length;
		r = MowayJava.mw_get_device_info(hHandle, (byte)MowayJava.MW_GET_INFO_ITEM_SN, buffer, buffer_len_array);
		if (r == MowayJava.MW_SUCCESS) {
			//System.out.println("MW_GET_INFO_ITEM_SN:" + sn);	// 16字节二进制数据，详情见 MW_DEVICE_ALL_ITEM_INFO 类描述
		}
		
		// 关闭加密锁句柄
		MowayJava.mw_close(hHandle);
	}
	
	private static void developerInitDevice() {
		// 开发商设置锁
		int uiDevListCount = 32;
		MW_DEVICE_INFO_CTX[] pDevInfoList = new MW_DEVICE_INFO_CTX[32];
		for(int i = 0; i < uiDevListCount; i++){
			pDevInfoList[i] = new MW_DEVICE_INFO_CTX();
		}
		int[] puiDevCount = new int[1]; 

		// 1.枚举锁
		int r = MowayJava.mw_enum(pDevInfoList, uiDevListCount, puiDevCount);
		if (r != MowayJava.MW_SUCCESS){
			System.out.println(r);
		}
		
		// 2.打开锁
		long[] phHandle = new long[1];
		r = MowayJava.mw_open(pDevInfoList[0], MowayJava.MW_OPEN_EXCLUSIVE_MODE, phHandle);
		if (r != MowayJava.MW_SUCCESS){
			System.out.println(r);
		}
		long hHandle = phHandle[0];
		
		// 3.验证开发商PIN
		byte[] devPIN = stringToCCharArray(DEVELOPER_PIN);
		r = MowayJava.mw_verify_pin(hHandle , (byte)MowayJava.MW_PIN_TYPE_DEVELOPER , devPIN);
		if (r != MowayJava.MW_SUCCESS){
			System.out.println(r);
		}
		
		// 4.设置PID
		int uiSeedLen = 16;
		byte[] pucPIDSeed = new byte[uiSeedLen];
		for(int i = 0; i < uiSeedLen; i++) {
			pucPIDSeed[i] = (byte) (i + 1);
		}
		r = MowayJava.mw_set_pid(hHandle, pucPIDSeed, uiSeedLen);
		if (r != MowayJava.MW_SUCCESS){
			System.out.println(r);
		}
		
		// 5.修改开发商PIN
		short limitCount = 15;	// 默认最大次数
		byte[] oldPIN = stringToCCharArray(DEVELOPER_PIN);
		byte[] newPIN = stringToCCharArray(DEVELOPER_PIN);	// 此处仅用于演示，旧PIN与新PIN相同。
		r = MowayJava.mw_change_pin(hHandle, (byte)MowayJava.MW_PIN_TYPE_DEVELOPER, limitCount, oldPIN, newPIN);
		if (r != MowayJava.MW_SUCCESS){
			System.out.println(r);
		}
		
		// 6.创建文件（二进制、读写、密钥文件-AES128、密钥文件-HMAC）
		MW_FILE_PROPERTY fileProperty = new MW_FILE_PROPERTY();
		// 6.1 创建二进制用户只读文件
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

		// 6.2 创建二进制用户可读写文件
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
		
		// 6.3 创建密钥文件-AES128
		fileProperty.ucType = (byte)MowayJava.MW_FILE_TYPE_KEY;
		fileProperty.uiSize = 256;
		fileProperty.usPrivilege = (short)MowayJava.MW_FILE_PRIV_TYPE_USE;
		fileProperty.acName = stringToCCharArray(USER_KEY_AES128_FILENAME);
		r = MowayJava.mw_create_file(hHandle, fileProperty);
		if (r == MowayJava.MW_SUCCESS){
			System.out.println("mw_create_file MW_FILE_PRIV_TYPE_USE success.");
			
			// 写入AES128秘钥数据
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
		
		// 6.4 创建密钥文件-HMAC
		fileProperty.ucType = (byte)MowayJava.MW_FILE_TYPE_KEY;
		fileProperty.uiSize = 256;
		fileProperty.usPrivilege = (short)MowayJava.MW_FILE_PRIV_TYPE_USE;
		fileProperty.acName = stringToCCharArray(USER_KEY_HMAC_MD5_FILENAME);
		r = MowayJava.mw_create_file(hHandle, fileProperty);
		if (r == MowayJava.MW_SUCCESS){
			System.out.println("mw_create_file MW_FILE_PRIV_TYPE_USE success.");
			
			// 写入AES128秘钥数据
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
		
		// 7.开发商权限写文件
		byte[] rwFilename;
		byte[] pucWriteBuffer;
		int uiWriteOffset = 0;
		int uiWriteSize = 0;
		
		// 7.1 修改用户只读文件
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
		
		// 7.2 修改用户可读写文件
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
		
		// 枚举锁内文件
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
		
		// 8.关闭锁
		MowayJava.mw_close(hHandle);
	}


	private static void UserUseDevice() {
		// 演示使用用户权限登录访问加密锁，完成以下功能：
		//* 修改用户PIN
		//* 读只读文件
		//* 修改可读写数据文件
		//* 使用密钥文件加解密（AES128）
		//* 使用密钥文件（HMAC）计算Hash值
		
		int uiDevListCount = 32;
		MW_DEVICE_INFO_CTX[] pDevInfoList = new MW_DEVICE_INFO_CTX[32];
		for(int i = 0; i < uiDevListCount; i++){
			pDevInfoList[i] = new MW_DEVICE_INFO_CTX();
		}
		int[] puiDevCount = new int[1]; 

		// 1.枚举锁
		int r = MowayJava.mw_enum(pDevInfoList, uiDevListCount, puiDevCount);
		if (r != MowayJava.MW_SUCCESS){
			System.out.println("mw_enum failed, r = " + r);
			return;
		}
		
		// 2.打开锁
		long[] phHandle = new long[1];
		r = MowayJava.mw_open(pDevInfoList[0], MowayJava.MW_OPEN_EXCLUSIVE_MODE, phHandle);
		if (r != MowayJava.MW_SUCCESS){
			System.out.println("mw_open failed, r = " + r);
			return;
		}
		long hHandle = phHandle[0];
		
		// 3.验证用户PIN
		byte[] userPIN = stringToCCharArray(USER_PIN);
		r = MowayJava.mw_verify_pin(hHandle, (byte)MowayJava.MW_PIN_TYPE_USER, userPIN);
		if (r != MowayJava.MW_SUCCESS){
			System.out.println("User mw_verify_pin failed, r = " + r);
			return;
		}
		
		// 4.修改用户PIN
		short limitCount = 15;	// 默认最大次数
		byte[] oldPIN = stringToCCharArray(USER_PIN);
		byte[] newPIN = stringToCCharArray(USER_PIN);	// 此处仅用于演示，旧PIN与新PIN相同。
		r = MowayJava.mw_change_pin(hHandle, (byte)MowayJava.MW_PIN_TYPE_USER, limitCount, oldPIN, newPIN);
		if (r != MowayJava.MW_SUCCESS){
			System.out.println("User mw_change_pin failed, r = " + r);
		}
		
		//5.读只读文件
		byte[] rFilename = stringToCCharArray(USER_RD_FILENAME);
		int readOffset = 0;
		int readSize = 16;	// 可通过查询文件属性接口，获得文件大小
		byte[] readBuffer = new byte[readSize];
		r = MowayJava.mw_read_file(hHandle, rFilename, readOffset, readSize, readBuffer);
		if (r == MowayJava.MW_SUCCESS){
			String readFileContent = new String(readBuffer); 
			System.out.println("User mw_read_file success, " + readFileContent);
		} else {
			System.out.println("User mw_read_file " + USER_RD_FILENAME + " failed, r = " + r);
		}
		
		//6.修改可读写数据文件
		byte[] rwFilename = stringToCCharArray(USER_RW_FILENAME);
		int writeOffset = 0;
		int writeSize = 16;	// 可通过查询文件属性接口，获得文件大小
		byte[] writeBuffer = "user write!".getBytes();
		r = MowayJava.mw_write_file(hHandle, rwFilename, writeOffset, writeSize, writeBuffer);
		if (r == MowayJava.MW_SUCCESS){
			System.out.println("User mw_write_file " + USER_RW_FILENAME + " success");
		} else {
			System.out.println("User mw_write_file " + USER_RW_FILENAME + " failed, r = " + r);
		}
		
		//7.使用密钥文件加解密（AES128）
		int aesDataLen = 128;
		// 7.1 加密
		byte[] aesKeyFilename = stringToCCharArray(USER_KEY_AES128_FILENAME);
		byte algoMode = (byte)MowayJava.MW_ALG_TYPE_AES;
		byte[] iv = null;	// 不设置IV向量
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
		
		// 7.2 解密
		byte[] decData = new byte[aesDataLen];
		int[] decDataLenArray = new int[1];
		decDataLenArray[0] = decData.length;
		r = MowayJava.mw_sym_decrypt(hHandle, aesKeyFilename, algoMode, iv, encData, encDataLenArray[0], decData, decDataLenArray);
		if (r == MowayJava.MW_SUCCESS){
			System.out.println("User mw_sym_decrypt " + USER_KEY_AES128_FILENAME + " success");
		} else {
			System.out.println("User mw_sym_decrypt " + USER_KEY_AES128_FILENAME + " failed, r = " + r);
		}
		
		// 7.3 加解密结果对比
		if (Arrays.equals(srcData, decData)) {
			System.out.println("User mw_sym_decrypt srcData = decData");
		} else {
			System.out.println("User mw_sym_decrypt srcData != decData");
		}
		
		//8.使用密钥文件（HMAC）计算Hash值
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
		
		// 9.关闭锁
		MowayJava.mw_close(hHandle);
	}
	
	private static void developerMakeUpdatePkg() {
		// 演示使用开发商权限创建加密锁升级包
		// 1.枚举锁
		// 2.打开锁
		// 3.验证开发商PIN
		// 4.制作升级包（升级二进制数据文件）
		// 5.关闭锁
		
		int uiDevListCount = 32;
		MW_DEVICE_INFO_CTX[] pDevInfoList = new MW_DEVICE_INFO_CTX[32];
		for(int i = 0; i < uiDevListCount; i++){
			pDevInfoList[i] = new MW_DEVICE_INFO_CTX();
		}
		int[] puiDevCount = new int[1]; 

		// 1.枚举锁
		int r = MowayJava.mw_enum(pDevInfoList, uiDevListCount, puiDevCount);
		if (r != MowayJava.MW_SUCCESS){
			System.out.println(r);
		}
		
		// 2.打开锁
		long[] phHandle = new long[1];
		r = MowayJava.mw_open(pDevInfoList[0], MowayJava.MW_OPEN_EXCLUSIVE_MODE, phHandle);
		if (r != MowayJava.MW_SUCCESS){
			System.out.println(r);
		}
		long hHandle = phHandle[0];
		
		// 3.验证开发商PIN
		byte[] devPIN = stringToCCharArray(DEVELOPER_PIN);
		r = MowayJava.mw_verify_pin(hHandle , (byte)MowayJava.MW_PIN_TYPE_DEVELOPER , devPIN);
		if (r != MowayJava.MW_SUCCESS){
			MowayJava.mw_close(hHandle);
			System.out.println("mw_verify_pin failed, r = " + r);
			return;
		}

		// 4.制作升级包（升级二进制数据文件）
		int devPID = 0;
		byte[] devSN = null;
		MW_UPDADE_FILE_CTX[] filePathArray = new MW_UPDADE_FILE_CTX[2];	// 允许升级多个文件
		int filePathArraySize = filePathArray.length; 
		
		byte[] pkgBuffer = new byte[4096];	// 请酌情根据升级的内容分配缓冲区大小
		int[] pkgBufferLenArray = new int[2];
		
		for (int i = 0; i < filePathArray.length; i++) {
			filePathArray[i] = new MW_UPDADE_FILE_CTX();
		}
		
		// 初始化制作升级包参数
		// 4.1 初始化升级文件内容
		MW_UPDADE_FILE_CTX updateFileCTXUpdate = filePathArray[0];
		updateFileCTXUpdate.ucType = (byte)MowayJava.MW_FILE_TYPE_BINARY;			// 类型（必填）
		updateFileCTXUpdate.usPrivilege = (short)MowayJava.MW_FILE_PRIV_TYPE_READ;	// 权限（必填）
		updateFileCTXUpdate.uiSize = 256;	// 修改文件大小（可选）
		updateFileCTXUpdate.acName = stringToCCharArray(UPDATE_PKG_UPDATE_FILENAME);
		updateFileCTXUpdate.pData = UPDATE_PKG_UPDATE_FILE_DATA.getBytes();
		updateFileCTXUpdate.uiDateSize = UPDATE_PKG_UPDATE_FILE_DATA.length();
		
		MW_UPDADE_FILE_CTX updateFileCTXCreate = filePathArray[1];
		updateFileCTXCreate.ucType = (byte)MowayJava.MW_FILE_TYPE_BINARY;			// 类型（必填）
		updateFileCTXCreate.usPrivilege = (short)MowayJava.MW_FILE_PRIV_TYPE_READ_WRITE;	// 权限（必填）
		updateFileCTXCreate.uiSize = 128;	// 修改文件大小（可选）
		updateFileCTXCreate.acName = stringToCCharArray(UPDATE_PKG_CREATE_FILENAME);
		updateFileCTXCreate.pData = UPDATE_PKG_CREATE_FILE_DATA.getBytes();
		updateFileCTXCreate.uiDateSize = UPDATE_PKG_CREATE_FILE_DATA.length();

		// 4.2 初始化PID和SN
		// 注意：1.指定PID时所有相同PID加密锁都可使用；2.当指定SN时，只有指定SN的加密锁能够使用升级包。

		// 本示例从当前加密锁获取PID，演示为同批次的加密锁制作升级包。注意：当为不同PID创建升级包时，PID参数可从其他途径获取。
		MW_DEVICE_ALL_ITEM_INFO pstDeviceAllInfo = new MW_DEVICE_ALL_ITEM_INFO();
		r = MowayJava.mw_get_device_all_info(hHandle, pstDeviceAllInfo);
		if (r == MowayJava.MW_SUCCESS){
			devPID = pstDeviceAllInfo.uiPID;
			System.out.println("mw_get_device_info MW_GET_INFO_ITEM_PID, " + devPID);
		} else {
			System.out.println("mw_get_device_info MW_GET_INFO_ITEM_PID failed, r = " + r);
		}
		
		// 4.2 初始化输出缓冲区长度
		pkgBufferLenArray[0] = pkgBuffer.length;
		r = MowayJava.mw_make_update_pkg(hHandle, devPID, devSN, filePathArray, filePathArraySize, pkgBuffer, pkgBufferLenArray);
		if (r == MowayJava.MW_SUCCESS){
			System.out.println("mw_make_update_pkg success");
			
			int pkgBufferLen = pkgBufferLenArray[0];
			if (pkgBufferLen > 0)
			{
				// 将升级包内容写入本地文件 
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
		
		// 5.关闭锁
		MowayJava.mw_close(hHandle);
	}

	private static void userUpdatePkg() {
		// 演示使用用户权限导入加密锁升级包
		//1.枚举锁
		//2.打开锁
		//3.验证用户PIN
		//4.导入升级包
		//5.验证升级包内容（读取验证内容与升级内容一致）
		//6.关闭锁
		
		int uiDevListCount = 32;
		MW_DEVICE_INFO_CTX[] pDevInfoList = new MW_DEVICE_INFO_CTX[32];
		for(int i = 0; i < uiDevListCount; i++){
			pDevInfoList[i] = new MW_DEVICE_INFO_CTX();
		}
		int[] puiDevCount = new int[1]; 

		// 1.枚举锁
		int r = MowayJava.mw_enum(pDevInfoList, uiDevListCount, puiDevCount);
		if (r != MowayJava.MW_SUCCESS){
			System.out.println("mw_enum failed, r = " + r);
			return;
		}
		
		// 2.打开锁
		long[] phHandle = new long[1];
		r = MowayJava.mw_open(pDevInfoList[0], MowayJava.MW_OPEN_EXCLUSIVE_MODE, phHandle);
		if (r != MowayJava.MW_SUCCESS){
			System.out.println("mw_open failed, r = " + r);
			return;
		}
		long hHandle = phHandle[0];
		
		// 3.验证用户PIN
		byte[] userPIN = stringToCCharArray(USER_PIN);
		r = MowayJava.mw_verify_pin(hHandle, (byte)MowayJava.MW_PIN_TYPE_USER, userPIN);
		if (r != MowayJava.MW_SUCCESS){
			System.out.println("User mw_verify_pin failed, r = " + r);
			MowayJava.mw_close(hHandle);
			return;
		}
		
		// 4.导入升级包
		byte[] pkgBuffer = null;
		int pkgBufferLen = 0;
		
		// 从本地文件加载升级包数据
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
		
		// 5.验证升级包内容（读取验证内容与升级内容一致）
		byte[] rFilename = null;
		int readOffset = 0;
		int readSize = 0;	// 可通过查询文件属性接口，获得文件大小
		byte[] readBuffer = new byte[readSize];
		
		// 5.1 读只读文件
		rFilename = stringToCCharArray(USER_RD_FILENAME);
		readOffset = 0;
		readSize = UPDATE_PKG_UPDATE_FILE_DATA.length();	// 可通过查询文件属性接口，获得文件大小
		readBuffer = new byte[readSize];
		r = MowayJava.mw_read_file(hHandle, rFilename, readOffset, readSize, readBuffer);
		if (r == MowayJava.MW_SUCCESS){
			String readFileContent = new String(readBuffer); 
			System.out.println("User update mw_read_file " + USER_RD_FILENAME + " success, " + readFileContent);
		} else {
			System.out.println("User update mw_read_file " + USER_RD_FILENAME + " failed, r = " + r);
		}
		
		// 5.2 读新建文件内容
		rFilename = stringToCCharArray(UPDATE_PKG_CREATE_FILENAME);
		readOffset = 0;
		readSize = UPDATE_PKG_CREATE_FILE_DATA.length();	// 可通过查询文件属性接口，获得文件大小
		readBuffer = new byte[readSize];
		r = MowayJava.mw_read_file(hHandle, rFilename, readOffset, readSize, readBuffer);
		if (r == MowayJava.MW_SUCCESS){
			String readFileContent = new String(readBuffer); 
			System.out.println("User update mw_read_file " + UPDATE_PKG_CREATE_FILENAME + " success, " + readFileContent);
		} else {
			System.out.println("User update mw_read_file " + UPDATE_PKG_CREATE_FILENAME + " failed, r = " + r);
		}
				
		// 9.关闭锁
		MowayJava.mw_close(hHandle);
	}
	

	private static int byteArrayToIntBE(byte[] b) {
		// 所有魔锐API接口返回的byte[]转int时，需要使用此函数，否则因C与Java大小端不一致造成int数据异常。
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
	 * @brief 枚举魔锐设备
	 * 
	 * @param[in out] pDevInfoList 枚举输出的魔锐设备列表
	 * 
	 * @param[in] uiDevListCount 枚举列表(pDevInfoList)能存放的设备的个数。注：最大个数参见宏定义MW_ENUM_DEVICE_MAX_COUNT
	 * 
	 * @param[out] puiDevCount 实际枚举出的魔锐设备数量
	 * 
	 * @return 执行成功返回 MW_SUCCESS ，执行失败时返回错误码。
	 * 
	 * @remarks 用户必须按照uiDevListCount乘以sizeof(MW_DEVICE_INFO_CTX)分配内存给pDevInfoList，API内部不分配内存给pDevInfoList。
	 */
	public final static native int mw_enum(MW_DEVICE_INFO_CTX[] pDevInfoList, int uiDevListCount, int[] puiDevCount);

	/*
	 * !
	 * 
	 * @brief 打开魔锐设备
	 * 
	 * @param[in] pDevInfo 指向魔锐设备信息结构体的指针。
	 * 
	 * @param[in] uiShareMode 独占或者共享模式打开。参见宏定义MW_OPEN_SHARE_MODE  MW_OPEN_EXCLUSIVE_MODE
	 * 
	 * @param[out] phHandle 打开魔锐设备后，输出一个句柄以便后续调用。
	 * 
	 * @return 执行成功返回 MW_SUCCESS ，执行失败时返回错误码。
	 * 
	 * @remarks Open an moway device with DevInfo enumerated by mw_enum()
	 */
	public final static native int mw_open(MW_DEVICE_INFO_CTX pDevInfo, int uiShareMode, long[] phHandle);

	/*
	 * !
	 * 
	 * @brief 关闭设备
	 * 
	 * @param[in] hHandle 魔锐设备句柄
	 * 
	 * @return 执行成功返回 MW_SUCCESS ，执行失败时返回错误码。
	 * 
	 * @remarks
	 */
	public final static native int mw_close(long hHandle);

	/*
	 * !
	 * 
	 * @brief 校验PIN
	 * 
	 * @param[in] hHandle 魔锐设备句柄
	 * 
	 * @param[in] ucPinType Pin码类型 参见宏定义：MW_PIN_TYPE_XXX
	 * 
	 * @param[in] pucPin码数据，用户8字节，开发商24字节
	 * 
	 * @return 执行成功返回 MW_SUCCESS ，执行失败时返回错误码。
	 * 
	 * @remarks
	 */
	public final static native int mw_verify_pin(long hHandle, byte ucPinType, byte[] pucPin);

	/*
	 * !
	 * 
	 * @brief 修改PIN
	 * 
	 * @param[in] hHandle 魔锐设备句柄
	 * 
	 * @param[in] ucPinType Pin码类型 参见宏定义：MW_PIN_TYPE_XXX
	 * 
	 * @param[in] usLimitCount 最大尝试次数，注：不限次数设置为0，限制次数范围是1-15， 如为其它数则返回参数错误
	 * 
	 * @param[in] pucOldPin           旧Pin码数据，用户8字节，开发商24字节
	 * 
	 * @param[in] pucNewPin           新Pin码数据，用户8字节，开发商24字节
	 * 
	 * @return 执行成功返回 MW_SUCCESS ，执行失败时返回错误码。
	 * 
	 * @remarks
	 */
	public final static native int mw_change_pin(long hHandle, byte ucPinType, short usLimitCount, byte[] pucOldPin,
			byte[] pucNewPin);

	/*
	 * !
	 * 
	 * @brief 设置PID
	 * 
	 * @param[in] hHandle 魔锐设备句柄
	 * 
	 * @param[in] pucPIDSeed 种子码
	 * 
	 * @param[in] uiSeedLen
	 * 种子码长度，长度范围是4-32，参见宏定义MW_SEED_LENGTH_MIN、MW_SEED_LENGTH_MAX
	 * 
	 * @return 执行成功返回 MW_SUCCESS ，执行失败时返回错误码。
	 * 
	 * @remarks
	 */
	public final static native int mw_set_pid(long hHandle, byte[] pucPIDSeed, int uiSeedLen);

	/*
	 * !
	 * 
	 * @brief 控制命令
	 * 
	 * @param[in] hHandle 魔锐设备句柄
	 * 
	 * @param[in] uiCtrlCodeItem 控制码，参见宏定义 MW_CTRL_CODE_ITEM_XXX
	 * 
	 * @param[in] pvInBuffer 输入数据
	 * 
	 * @param[in] uiInBufferLen 输入数据的长度
	 * 
	 * @param[out] pvOutBuffer 输出数据
	 * 
	 * @param[in] uiOutBufferLen 输出数据的长度
	 * 
	 * @param[out] puiReturnedLen 实际输出数据的长度
	 * 
	 * @return 执行成功返回 MW_SUCCESS ，执行失败时返回错误码。
	 * 
	 * @remarks
	 */
	public final static native int mw_control(long hHandle, byte uiCtrlCodeItem, byte[] pvInBuffer, int uiInBufferLen,
			byte[] pvOutBuffer, int uiOutBufferLen, int[] puiReturnedLen);

	/*
	 * !
	 * 
	 * @brief 获取设备信息
	 * 
	 * @param[in] hHandle 魔锐设备句柄
	 * 
	 * @param[in] ucInfoItem 指定获取的设备信息的具体项，参见宏定义：MW_GET_INFO_ITEM_XXX
	 * 
	 * @param[out] pvBuffer 输出获取的设备信息项
	 * 
	 * @param[in out] puiBufferLength 输出设备信息项的数据长度
	 * 
	 * @return 执行成功返回 MW_SUCCESS ，执行失败时返回错误码。
	 * 
	 * @remarks
	 */
	public final static native int mw_get_device_info(long hHandle, byte ucInfoItem, byte[] pvBuffer,
			int[] puiBufferLength);
	public final static native int mw_get_device_all_info(long hHandle, MW_DEVICE_ALL_ITEM_INFO pstDeviceAllInfo);

	/*
	 * !
	 * 
	 * @brief 获取设备状态
	 * 
	 * @param[in] hHandle 魔锐设备句柄
	 * 
	 * @param[out] pstDeviceStatusCtx 当前设备状态。
	 * 
	 * @return 执行成功返回 MW_SUCCESS ，执行失败时返回错误码。
	 * 
	 * @remarks
	 */
	public final static native int mw_get_device_status(long hHandle, MW_DEVICE_STATUS_CTX pstDeviceStatusCtx);

	/*
	 * !
	 * 
	 * @brief 列举文件
	 * 
	 * @param[in] hHandle 魔锐设备句柄
	 * 
	 * @param[out] pstFilePropertyList
	 * 结构体MW_FILE_PROPERTY数组，它的大小由uiFileListCount指定
	 * 
	 * @param[in] uiFileListCount 指定pstFilePropertyList数组的大小
	 * 
	 * @param[out] puiReturnedFileCount 实际返回列举到文件的个数
	 * 
	 * @return 执行成功返回 MW_SUCCESS ，执行失败时返回错误码。
	 * 
	 * @remarks 注意：pstFilePropertyList由函数外部调用者分配，大小为uiFileListCount * sizeof(MW_FILE_PROPERTY)
	 */
	public final static native int mw_enum_file(long hHandle, MW_FILE_PROPERTY[] pstFilePropertyList, int uiFileListCount,
			int[] puiReturnedFileCount);

	/*
	 * !
	 * 
	 * @brief 创建文件
	 * 
	 * @param[in] hHandle 魔锐设备句柄
	 * 
	 * @param[in] pstFileProperty 指定需要创建文件的属性，其中文件类型、文件权限、文件大小为必填项
	 * 
	 * @return 执行成功返回 MW_SUCCESS ，执行失败时返回错误码。
	 * 
	 * @remarks
	 */
	public final static native int mw_create_file(long hHandle, MW_FILE_PROPERTY pstFileProperty);

	/*
	 * !
	 * 
*   @brief      读文件
*   @param[in]  hHandle           魔锐设备句柄
*   @param[in]  pcFileName        文件名称，以'\0'结尾的字符串      
*   @param[in]  uiReadOffset      读文件的偏移量   
*   @param[in]  uiReadSize        预计读取文件的大小。
*   @param[out] pucReadBuffer     读取文件数据到缓冲区。注：必须分配足够uiReadSize大小，否则会访问越界。
	 * 
	 * @return 执行成功返回 MW_SUCCESS ，执行失败时返回错误码。
	 * 
	 * @remarks
	 */
	public final static native int mw_read_file(long hHandle, byte[] pcFileName, int uiReadOffset, int uiReadSize,
			byte[] pucReadBuffer);

	/*
	 * !
	 * 
	 * @brief 写文件
	 * 
	 * @param[in] hHandle 魔锐设备句柄
	 * 
	 * @param[in] pcFileName 文件名称，以'\0'结尾的字符串
	 * 
	 * @param[in] uiWriteOffset 写文件的偏移量
	 * 
	 * @param[in] uiWriteSize 写文件的大小。 
	 * 
	 * @param[in] pucWriteBuffer 需要写入文件的缓冲区
	 * 
	 * @return 执行成功返回 MW_SUCCESS ，执行失败时返回错误码。
	 * 
	 * @remarks
	 */
	public final static native int mw_write_file(long hHandle, byte[] pcFileName, int uiWriteOffset, int uiWriteSize,
			byte[] pucWriteBuffer);

	/*
	 * !
	 * 
	 * @brief 删除文件
	 * 
	 * @param[in] hHandle 魔锐设备句柄
	 * 
	 * @param[in] pcFileName 文件名称，以'\0'结尾的字符串
	 * 
	 * @return 执行成功返回 MW_SUCCESS ，执行失败时返回错误码。
	 * 
	 * @remarks
	 */
	public final static native int mw_delete_file(long hHandle, byte[] pcFileName);

	/*
	 * !
	 * 
	 * @brief 获取文件属性
	 * 
	 * @param[in] hHandle 魔锐设备句柄
	 * 
	 * @param[in] lpszFileName 文件名称，以'\0'结尾的字符串
	 * 
	 * @param[out] pstFileProperty 文件属性信息
	 * 
	 * @return 执行成功返回 MW_SUCCESS ，执行失败时返回错误码。
	 * 
	 * @remarks
	 */
	public final static native int mw_get_file_property(long hHandle, byte[] pcFileName,
			MW_FILE_PROPERTY pstFileProperty);

	/*
	 * !
	 * 
	 * @brief 对称加密
	 * 
	 * @param[in] hHandle 魔锐设备句柄
	 * 
	 * @param[in] pcKeyFileName 密钥文件名称
	 * 
	 * @param[in] ucAlgoMode 对称加密的算法，参见宏定义：MW_ALG_TYPE_XXX
	 * 
	 * @param[in] pucIV 对称加密算法中需要使用到的向量，固定16字节，CBC模式需要填入，ECB可不填
	 * 
	 * @param[in] pucInData 加密输入的明文，必须是16的倍数，
	 * 
	 * @param[in] uiInDataLen 加密明文的数据长度,
	 * 
	 * @param[out] pucOutData 加密后输出的密文
	 * 
	 * @param[in out] puiOutDataLen 加密后密文的长度
	 * 
	 * @return 执行成功返回 MW_SUCCESS ，执行失败时返回错误码。
	 * 
	 * @remarks
	 */
	public final static native int mw_sym_encrypt(long hHandle, byte[] pcKeyFileName, byte ucAlgoMode, byte[] pucIV,
			byte[] pucInData, int uiInDataLen, byte[] pucOutData, int[] puiOutDataLen);

	/*
	 * !
	 * 
	 * @brief 对称解密
	 * 
	 * @param[in] hHandle 魔锐设备句柄
	 * 
	 * @param[in] pcKeyFileName 密钥文件名称
	 * 
	 * @param[in] ucAlgoMode 对称加密的算法，参见宏定义：MW_ALG_TYPE_XXX
	 * 
	 * @param[in] pucIV 对称加密算法中需要使用到的向量，固定16字节，CBC模式需要填入，ECB可不填
	 * 
	 * @param[in] pucInData 输入的密文，必须是16的倍数,
	 * 
	 * @param[in] uiInDataLen 输入密文长度,
	 * 
	 * @param[out] pucOutData 解密后输出明文
	 * 
	 * @param[in out] puiOutDataLen 解密后输出明文长度
	 * 
	 * @return 执行成功返回 MW_SUCCESS ，执行失败时返回错误码。
	 * 
	 * @remarks
	 */
	public final static native int mw_sym_decrypt(long hHandle, byte[] pcKeyFileName, byte ucAlgoMode, byte[] pucIV,
			byte[] pucInData, int uiInDataLen, byte[] pucOutData, int[] puiOutDataLen);

	/*
	 * !
	 * 
	 * @brief rsa非对称加密
	 * 
	 * @param[in] hHandle 魔锐设备句柄
	 * 
	 * @param[in] pcKeyFileName 密钥文件名称
	 * 
	 * @param[in] ucPadMode 加密的算法，参见宏定义：MW_RSA_MODE_XXX
	 * 
	 * @param[in] pucInData 输入明文数据
	 * 
	 * @param[in] uiInDataLen 输入明文数据的长度
	 * 
	 * @param[out] pucOutData 输出密文数据
	 * 
	 * @param[in out] puiOutDataLen 输出密文数据的长度
	 * 
	 * @return 执行成功返回 MW_SUCCESS ，执行失败时返回错误码。
	 * 
*   @remarks    注：当ucPadMode为MW_RSA_MODE_NORMAL时，RSA_1024位加密最大加密明文长度为128,RSA_2048位加密最大长度为256; 
*                   当ucPadMode为MW_RSA_MODE_PKCS1_V1_5时，  RSA_1024位加密最大加密明文长度为117,RSA_2048位加密最大长度为245; 
*/

	public final static native int mw_rsa_encrypt(long hHandle, byte[] pcKeyFileName, byte ucPadMode, byte[] pucInData,
			int uiInDataLen, byte[] pucOutData, int[] puiOutDataLen);

	/*
	 * !
	 * 
	 * @brief rsa非对称解密
	 * 
	 * @param[in] hHandle 魔锐设备句柄
	 * 
	 * @param[in] pcKeyFileName 密钥文件名称
	 * 
	 * @param[in] ucPadMode 加密的算法，参见宏定义：MW_RSA_MODE_XXX
	 * 
	 * @param[in] pucInData 输入密文
	 * 
	 * @param[in] uiInDataLen 输入密文长度 注：它的长度必须为128或者256
	 * 
	 * @param[out] pucOutData 输出明文
	 * 
	 * @param[out] puiOutDataLen 输出明文长度
	 * 
	 * @return 执行成功返回 MW_SUCCESS ，执行失败时返回错误码。
	 * 
	 * @remarks
	 */
	public final static native int mw_rsa_decrypt(long hHandle, byte[] pcKeyFileName, byte ucPadMode, byte[] pucInData,
			int uiInDataLen, byte[] pucOutData, int[] puiOutDataLen);

	/*
	 * !
	 * 
	 * @brief 签名
	 * 
	 * @param[in] hHandle 魔锐设备句柄
	 * 
	 * @param[in] pcKeyFileName 密钥文件名称
	 * 
	 * @param[in] ucHashAlg 哈希算法类型，参见宏定义：MW_HASH_ALGO_SHA1, MW_HASH_ALGO_SHA256, MW_HASH_ALGO_NONE
	 * 
	 * @param[in] pucMessageData 输入消息数据
	 * 
	 * @param[in] uiMessageDataLen 输入消息数据长度 注：最大数据长度参见宏定义 MW_INPUT_DATA_LENGTH_MAX 
	 * 
	 * @param[out] pucSignData 输出签名数据
	 * 
	 * @param[out] puiSignDataLen 输出签名数据长度
	 * 
	 * @return 执行成功返回 MW_SUCCESS ，执行失败时返回错误码。
	 * 
	 * @remarks
	 */
	public final static native int mw_signature(long hHandle, byte[] pcKeyFileName, byte ucHashAlg,
			byte[] pucMessageData, int uiMessageDataLen, byte[] pucSignData, int[] puiSignDataLen);

	/*
	 * !
	 * 
	 * @brief 验签
	 * 
	 * @param[in] hHandle 魔锐设备句柄
	 * 
	 * @param[in] pcKeyFileName 密钥文件名称
	 * 
	 * @param[in] ucHashAlg 哈希算法类型，参见宏定义：MW_HASH_ALGO_XXX
	 * 
	 * @param[in] pucSignData 输入签名数据
	 * 
	 * @param[in] uiSignDataLen 输入签名数据长度
	 * 
	 * @param[in] pucMessageData 输入消息数据
	 * 
	 * @param[in] uiMessageDataLen 输入消息数据长度
	 * 
	 * @return 执行成功返回 MW_SUCCESS ，执行失败时返回错误码。
	 * 
	 * @remarks
	 */
	public final static native int mw_verify_sign(long hHandle, byte[] pcKeyFileName, byte ucHashAlg,
			byte[] pucSignData, int uiSignDataLen, byte[] pucMessageData, int uiMessageDataLen);

	/*
	 * !
	 * 
	 * @brief HMAC计算
	 * 
	 * @param[in] hHandle 魔锐设备句柄
	 * 
	 * @param[in] pcKeyFileName 密钥文件名称
	 * 
	 * @param[in] pucInData 输入数据
	 * 
	 * @param[in] uiInDataLen 输入数据长度
	 * 
	 * @param[out] pucOutData 输出数据
	 * 
	 * @param[out] puiOutDataLen
	 * 输出数据长度，参见宏定义：MW_HMAC_MD5_DIGEST_LENGTH、MW_HMAC_SHA1_DIGEST_LENGTH、MW_HMAC_SHA256_DIGEST_LENGTH
	 * 
	 * @return 执行成功返回 MW_SUCCESS ，执行失败时返回错误码。
	 * 
	 * @remarks
	 */
	public final static native int mw_hmac_calc(long hHandle, byte[] pcKeyFileName, byte[] pucInData, int uiInDataLen,
			byte[] pucOutData, int[] puiOutDataLen);

	/*
	 * !
	 * 
	 * @brief 制作升级包
	 * 
	 * @param[in] hHandle 魔锐设备句柄
	 * 
	 * @param[in] uiDevPID 设备产品号
	 * 
	 * @param[in] pucSN 硬件唯一序列号 固定长度为：MW_SN_LENGTH，不指定SN绑定时，可以为NULL
	 * 
	 * @param[in] pstUpdateFileList
	 * 文件路径列表,最大文件个数参见宏定义MW_UPDATE_FILE_COUNT_MAX,升级文件的个数为uiFileCount
	 * 
	 * @param[in] uiFileCount 升级文件的个数,它指定了pstUpdateFileList中的大小
	 * 
	 * @param[out] pucOutPkg 升级包数据
	 * 
	 * @param[in out] puiOutPkgLen 升级包数据长度
	 * 
	 * @return 执行成功返回 MW_SUCCESS ，执行失败时返回错误码。
	 * 
	 * @remarks
	 */
	public final static native int mw_make_update_pkg(long hHandle, int uiDevPID, byte[] pucSN,
			MW_UPDADE_FILE_CTX[] pstFilePathList, int uiFileCount, byte[] pucOutPkg, int[] puiOutPkgLen);

	/*
	 * !
	 * 
	 * @brief 远程升级
	 * 
	 * @param[in] hHandle 魔锐设备句柄
	 * 
	 * @param[in] pucInPkg 升级包数据
	 * 
	 * @param[in] uiInPkgLen 升级包数据长度
	 * 
	 * @return 执行成功返回 MW_SUCCESS ，执行失败时返回错误码。
	 * 
	 * @remarks
	 */
	public final static native int mw_update(long hHandle, byte[] pucInPkg, int uiInPkgLen);

	/*
	 * !
	 * 
	 * @brief 恢复出厂设置
	 * 
	 * @param[in] hHandle 魔锐设备句柄
	 * 
	 * @return 执行成功返回 MW_SUCCESS ，执行失败时返回错误码。
	 * 
	 * @remarks 将设备恢复出厂状态，即默认PID、默认PIN，文件系统清空。需开发商权限
	 */
	public final static native int mw_restore_factory(long hHandle);

	/*
	 * !
	 * 
	 * @brief 错误码帮助
	 * 
	 * @param[in] hHandle 魔锐设备句柄
	 * 
	 * @param[in] uiErrorCode 错误码
	 * 
	 * @param[in] uiLanguageID 语言种类ID
	 * 
	 * @return 执行成功返回 MW_SUCCESS ，执行失败时返回错误码。
	 * 
	 * @remarks
	 */
	public final static native byte[] mw_get_error_desc(int uiErrorCode, int uiLanguageID);
	
	//打开锁的方式
	/** 共享打开  */
	public final static int MW_OPEN_SHARE_MODE                                 = 0;
	/** 独占打开 */
	public final static int MW_OPEN_EXCLUSIVE_MODE                             = 1;
	//============================================================
	//  算法类型表示
	//============================================================
	/** 算法类型AES */
	public final static int MW_ALG_TYPE_AES                                    = 0x00;
	/** 算法类型DES */
	public final static int MW_ALG_TYPE_DES                                    = 0x01;
	/** 算法类型TDES */
	public final static int MW_ALG_TYPE_TDES                                   = 0x02;
	/** 算法类型ECC */
	public final static int MW_ALG_TYPE_ECC                                    = 0x10;
	/** 算法类型RSA */
	public final static int MW_ALG_TYPE_RSA                                    = 0x11;
	//============================================================
	//  对称算法模式
	//============================================================
	/** 对称算法模式 ECB mode */
	public final static int MW_SYM_ALGO_MODE_ECB                               = 0x00;
	/** 对称算法模式 CBC mode */
	public final static int MW_SYM_ALGO_MODE_CBC                               = 0x01;
	//============================================================
	//  文件属性相关宏定义
	//============================================================
	/** 二进制文件 */
	public final static int MW_FILE_TYPE_BINARY                                = 0;
	/** 密钥文件 */
	public final static int MW_FILE_TYPE_KEY                                   = 2;
	/** 密钥文件权限类型 */
	public final static int MW_FILE_PRIV_TYPE_USE                              = 0x00;
	/** 数据文件用户只读权限类型 */
	public final static int MW_FILE_PRIV_TYPE_READ                             = 0x01;
	/** 数据文件用户读写权限类型 */
	public final static int MW_FILE_PRIV_TYPE_READ_WRITE                       = 0x02;
	//============================================================
	//  获取设备信息宏定义 
	//============================================================
	/** 获取PID     返回4字节PID */
	public final static int MW_GET_INFO_ITEM_PID  	                           = 0x00;
	/** 获取序列号  返回16字节获取序列号 */
	public final static int MW_GET_INFO_ITEM_SN                                = 0x01;
	/** 获取生产日期 返回4字节生产日期 */
	public final static int MW_GET_INFO_ITEM_PRODUCE_DATE                      = 0x02;
	/** 获取总容量   返回4字节总容量 */
	public final static int MW_GET_INFO_ITEM_ALL_CAPACITY  	                   = 0x03;
	/** 获取剩余容量 返回4字节剩余容量 */
	public final static int MW_GET_INFO_ITEM_FREE_CAPACITY                     = 0x04;
	/** 获取设备版本 返回4字节设备版本 */
	public final static int MW_GET_INFO_ITEM_VERSION                           = 0x05;
	/** 获取外壳号   返回第一字节为外壳号长度，后面是外壳号数据。注：最大256字节。 */
	public final static int MW_GET_INFO_ITEM_SHELL_SN                          = 0x06;
	/** 获取设置状态 返回1字节的标识，参见宏定义MW_STATUS_FLAG_PID、MW_STATUS_FLAG_PIN */
	public final static int MW_GET_INFO_ITEM_STATUS                            = 0x07;
	/** 获取设备远程升级码      返回4 字节设备远程升级码 */
	public final static int MW_GET_INFO_ITEM_UPDATE_CODE                       = 0x08;
	/** 获取远程升级控制锁标识  返回1字节标识 */
	public final static int MW_GET_INFO_ITEM_UPDATE_CTRL_CODE                  = 0x09;
	/** 获取所有信息，返回MW_DEV_ALL_ITEMINFO结构体的大小 */
	public final static int MW_GET_INFO_ITEM_ALL                               = 0xFF;


	/** 默认状态 */
	public final static int MW_STATUS_FLAG_DEFAULT                             = 0x00;
	/** 已经设置PID状态 */
	public final static int MW_STATUS_FLAG_PID                                 = 0x01;
	/** 已经修改PIN状态 */
	public final static int MW_STATUS_FLAG_PIN                                 = 0x02;
	//============================================================
	//  控制码
	//============================================================
	/** 复位设备，清除登陆状态。 */
	public final static int MW_CTRL_CODE_ITEM_RESET                            = 0x00;
	/** 控制LED。  注：数据第一字节非0标识LED亮，0表示灭 */
	public final static int MW_CTRL_CODE_ITEM_LED                              = 0x01;
	//============================================================
	//  PIN类型定义
	//============================================================
	/** 默认PIN类型 */
	public final static int MW_PIN_TYPE_NONE                                   = 0x00;
	/** 用户PIN类型 */
	public final static int MW_PIN_TYPE_USER                                   = 0x01;
	/** 开发商PIN类型 */
	public final static int MW_PIN_TYPE_DEVELOPER                              = 0x02;
	/** 用户PIN长度 */
	public final static int MW_PIN_LENGTH_USER                                 = 8;
	/** 开发商PIN长度 */
	public final static int MW_PIN_LENGTH_DEVELOPER                            = 24;
	//============================================================
	//  RSA加密模式定义
	//============================================================
	/** RSA 不填充加密模式 */
	public final static int MW_RSA_MODE_NORMAL                        = 0x00;
	/** RSA PKCS1 V1_5 填充加密模式 */
	public final static int MW_RSA_MODE_PKCS1_V1_5                    = 0x01;
	//============================================================
	//  RSA加密数据长度
	//============================================================
	/** RSA-1024加密数据长度 */
	public final static int MW_RSA_1024_BYTE_SIZE                     = 128;
	/** RSA-2048加密数据长度 */
	public final static int MW_RSA_2048_BYTE_SIZE                     = 256;
	//============================================================
	//  签名hash算法类型
	//============================================================
	/** 签名MD5 hash算法 */
	public final static int MW_HASH_ALGO_MD5                          = 0x00;
	/** 签名SHA1 hash算法 */
	public final static int MW_HASH_ALGO_SHA1                         = 0x01;
	/** 签名SHA256 hash算法 */
	public final static int MW_HASH_ALGO_SHA256                       = 0x02;
	/** 签名时不做Hash运算，由调用函数者自己做Hash运算 */
	public final static int MW_HASH_ALGO_NONE                         = 0xFF;



	//============================================================
	//  升级包相关宏定义
	//============================================================
	/** 一个升级包最大包含升级文件个数 */
	public final static int MW_UPDATE_FILE_COUNT_MAX                  = 50;
	/** 升级文件时的操作标志 删除文件 */
	public final static int MW_UPDATE_MOD_FLAG_DELETE                 = 0x00000002;
	/** 升级文件时的操作标志 写文件 */
	public final static int MW_UPDATE_MOD_FLAG_WRITE                  = 0x00000010;
	/** 升级文件时的操作标志 创建文件 */
	public final static int MW_UPDATE_MOD_FLAG_CREATE                 = 0x00000020;
	//============================================================
	//  多语言宏定义
	//============================================================
    /** 默认语言 */
	public final static int MW_LANGUAGE_ID_DEFAULT                    = 0;

	/** 中文 */
	public final static int MW_LANGUAGE_ID_CH                         = 1;
	/** 英文 */
	public final static int MW_LANGUAGE_ID_EN                         = 2;
	
	//============================================================
	//  魔锐相关大小宏定义
	//============================================================
	/** 最大路径 */
	public final static int MW_PATH_LEN                               = 1024;
	/** 对称加密算法向量长度 */
	public final static int MW_SYM_ALG_IV_LENGTH                      = 16;
	/** 设备唯一序列号长度 */
	public final static int MW_SN_LENGTH                              = 16;
	/** 文件名最大长度 */
	public final static int MW_FILE_NAME_LENGTH_MAX                   = 16;
	/** HMAC MD5计算结果长度 */
	public final static int MW_HMAC_MD5_DIGEST_LENGTH                 = 16;
	/** HMAC SHA1计算结果长度 */
	public final static int MW_HMAC_SHA1_DIGEST_LENGTH                = 20;
	/** HMAC SHA256计算结果长度 */
	public final static int MW_HMAC_SHA256_DIGEST_LENGTH              = 32;
	/** 一次最大转输数据长度 */
	public final static int MW_INPUT_DATA_LENGTH_MAX                  = 1900;
	//种子码最大长度宏定义
	/** 最大种子码长度 */
	public final static int MW_SEED_LENGTH_MAX                        = 32;
	/** 最小种子码长度 */
	public final static int MW_SEED_LENGTH_MIN                        = 4;
	public final static int MW_ENUM_DEVICE_MAX_COUNT                  = 128;
	
	//============================================================
	//  函数返回值定义
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
	
	public final static int MW_ERROR_NO_PID_DEVICE                             = 0x00002001;                  // 主机上找不到指定的PID魔锐锁
	public final static int MW_ERROR_RET_DATA_FORMAT                           = 0x00002002;                  // 返加数据格式不正确
	public final static int MW_ERROR_FILE_NAME_LEN                             = 0x00002003;                  // 文件名长度不正确
	public final static int MW_ERROR_BLOCK_DATA                                = 0x00002004;                  // 数据块错误
	public final static int MW_ERROR_FILE_PROPERTY_PARAM                       = 0x00002005;                  // 文件属性参数错误
	public final static int MW_ERROR_FILE_COUNT_MAX                            = 0x00002006;                  // 实际文件个数超过预分配的个数
	public final static int MW_ERROR_MALLOC_MEMORY                             = 0x00002007;                  // 分配内存失败
    public final static int MW_ERROR_PIN_LENGTH                                = 0x00002008;                  // PIN长度有错误
    public final static int MW_ERROR_UPDATA_PKG_PID                            = 0x00002009;                  // 升级包PID与锁内PID不一致
	
	public final static int MW_ERROR_ENTRY_TYPE                                = 0x0000A001;                  // entry 类型错误
	public final static int MW_ERROR_ENTRY_FLAG                                = 0x0000A002;                  // entry 标识错误
	public final static int MW_ERROR_OPCODE                                    = 0x0000A003;                  // 错误命令
	public final static int MW_ERROR_PARAM                                     = 0x0000A004;                  // 错误参数
	public final static int MW_ERROR_DATA_LENGTH                               = 0x0000A005;                  // 错误数据长度
	public final static int MW_ERROR_PIN                                       = 0x0000A006;                  // 错误PIN 码
	public final static int MW_ERROR_DEVICE_BLOCK                              = 0x0000A007;                  // 设备PIN 码锁死
	public final static int MW_ERROR_PRIVILEGE                                 = 0x0000A008;                  // 没有权限
	public final static int MW_ERROR_DATA_INFO                                 = 0x0000A009;                  // 数据域数据错误
	public final static int MW_ERROR_HASH_TYPE                                 = 0x0000A00A;                  // 错误HASH 类型
	public final static int MW_ERROR_ALG_MODE                                  = 0x0000A00B;                  // 错误的模式
	public final static int MW_ERROR_INIT_EVT                                  = 0x0000A00C;                  // 初始化过程问题错误
	public final static int MW_ERROR_ALG_TYPE                                  = 0x0000A00D;                  // 错误算法类型
	public final static int MW_ERROR_BIT_SIZE                                  = 0x0000A00E;                  // 错误的密钥位大小
	public final static int MW_ERROR_FILE_TYPE                                 = 0x0000A00F;                  // 错误的文件类型
	public final static int MW_ERROR_NO_INIT_PID                               = 0x0000A010;                  // PID未初始化
	public final static int MW_ERROR_NO_INIT_PIN                               = 0x0000A011;                  // PIN码未修改
	public final static int MW_ERROR_NO_VALIDATE_PKG                           = 0x0000A012;                  // 无效升级包
	public final static int MW_ERROR_AUTH_STAGE                                = 0x0000A013;                  // 权限价段错误
	public final static int MW_ERROR_FILE_PRIVILEGE                            = 0x0000A014;                  // 文件权限错误
	public final static int MW_ERROR_FILE_RANGE                                = 0x0000A015;                  // 文件范围错误
	public final static int MW_ERROR_KEY_FILE                                  = 0x0000A016;                  // 文件密钥错误
	public final static int MW_ERROR_REQ_IDENTIFY                              = 0x0000A017;                  // 返回认证错误
	public final static int MW_ERROR_INVALID_SEED                              = 0x0000A018;                  // 无效种子错误
	public final static int MW_ERROR_SEED_CHECK                                = 0x0000A019;                  // 种子校验错误
	public final static int MW_ERROR_UPDATE_HASH                               = 0x0000A01A;                  // 升级哈希错误
	public final static int MW_ERROR_UPDATE_PID                                = 0x0000A01B;                  // 升级PID错误
	public final static int MW_ERROR_UPDATE_INDEX                              = 0x0000A01C;                  // 升级索引错误
	public final static int MW_ERROR_UPDATE_TIME                               = 0x0000A01D;                  // 升级时间错误
	public final static int MW_ERROR_UPDATE_SERIAL_NUM                         = 0x0000A01E;                  // 升级序列号错误
	public final static int MW_ERROR_UPDATE_SN                                 = 0x0000A01F;                  // 升级SN错误
	public final static int MW_ERROR_UPDATE_ORDER                              = 0x0000A020;                  // 升级顺序错误
	public final static int MW_ERROR_UPDATE_OPERATE                            = 0x0000A021;                  // 长级操作错误
	public final static int MW_ERROR_KEY_TYPE                                  = 0x0000A022;                  // 关键字类型错误
	public final static int MW_ERROR_OTHER                                     = 0x0000AFFF;                  // 其他错误
}


class MW_DEVICE_INFO_CTX {
	/** 产品ID */
    public int    uiPID;
	/** 唯一序列号 */
    public byte[]   ucSerialNum = new byte[MowayJava.MW_SN_LENGTH];
	/** 设备的路径 */
    public byte[]   ucDevPath = new byte[MowayJava.MW_PATH_LEN];
	/** 通讯协议 */
    public int    uiProtocol;  
	/** Mac OS 系统 LocationID */
    public int    uiLocationID;
}

class MW_FILE_PROPERTY {
	/** 该成员变量暂时不用 */
	public byte   ucValidate; 
	/** 文件类型：二进制文件或者密钥文件。 */
	public byte   ucType;   
	/** 文件权限 */
	public short  usPrivilege;
	/** 文件大小 */
	public int    uiSize;    
	/** 创建时间 */
	public int    uiTime;   
	/** 文件名称 */
	public byte[] acName = new byte[MowayJava.MW_FILE_NAME_LENGTH_MAX + 1]; 
}

class MW_DEVICE_STATUS_CTX {
	/** 系统时间 */
	public int    uiSysTime;
	/** 系统状态 */
	public int    uiSysStatus;
	/** 保留 */
	public short  usReserved;
	/** 登录状态 */
	public byte   ucLoginStatus;
	/** 灯的状态 */
	public byte   ucLedStatus;
}

class MW_UPDADE_FILE_CTX {
	///** 文件属性结构体 */ //MW_FILE_PROPERTY stFileProperty;
	//嵌套结构体无法使用JNI接口获取子类的内容并完成转换，临时在 MW_UPDADE_FILE_CTX 中使用平铺展开结构的方式处理。shijc 2019-3-15
	///** 文件属性结构体（开始） */
	/** 该成员变量暂时不用 */
	public byte   ucValidate; 
	/** 文件类型：二进制文件或者密钥文件。 */
	public byte   ucType;   
	/** 文件权限 */
	public short  usPrivilege;
	/** 文件大小 */
	public int    uiSize;    
	/** 创建时间 */
	public int    uiTime;   
	/** 文件名称 */
	public byte[] acName = new byte[MowayJava.MW_FILE_NAME_LENGTH_MAX + 1];
	///** 文件属性结构体（结束） */
	/** 文件路径 */
	//public byte[] filePath = new byte[MowayJava.MW_PATH_LEN]; 
	/** 升级文件的数据内容 */
	public byte[] pData; 
	/** 升级文件的数据大小 */
	public int uiDateSize;
}

class MW_DEVICE_ALL_ITEM_INFO {
	/** 产品号 */
	public int uiPID;
	/** 唯一序列号 */
	public byte[] acSN = new byte[MowayJava.MW_SN_LENGTH];
	/** 生产时间 */
	public int uiProduceDate;
	/** 总容量 */
	public int uiAllCapacity;
	/** 可用容量 */
	public int uiFreeCapacity;
	/** 版本号 */
	public int uiVersion;
	/** 升级码 */
	public int uiUpdateCode;
	/** 状态 */
	public byte ucStatus;
	/** 升级控制码 */
	public byte ucUpdateCtrlCode;
	/** 保留 */
	public short reserved;
}
