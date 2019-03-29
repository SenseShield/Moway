using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System.Runtime.InteropServices;
using Moway;
using System.IO;

namespace MowayAPISample
{
    class MowayDevice
    {
        private static string userRDFilename = "userrd.evd";
        private static string userRWFilename = "userrw.evd";

        private static string userUseFileData = "Hello Moway!";

        private static string keyAES128Filename = "aes_128.sls";
        private static string keyHMACMD5Filename = "hmac_md5.sls";


        /// <summary>
        /// 设备信息获取（无权限要求）
        /// </summary>
        /// <returns></returns>
        public static UInt32 GetDeviceInfo()
        {
            //1.枚举锁
            //2.打开锁
            //3.获取所有信息（结构体）
            //4.获取指定信息（Item）
            //5.关闭锁

            UInt32 ret = MowayCS.MW_SUCCESS;
            UInt32 devInfoArraySize = 32;       // 获取设备的最大数量
            MW_DEVICE_INFO_CTX[] devInfoArray = new MW_DEVICE_INFO_CTX[devInfoArraySize];

            UInt32 devCount = 0;
            IntPtr hHandle = IntPtr.Zero;

            //1.枚举锁
            ret = MowayCS.mw_enum(devInfoArray, devInfoArraySize, ref devCount);
            if (ret != MowayCS.MW_SUCCESS)
            {
                Console.WriteLine("mw_enum failed, {0:X8}", ret);
                return ret;
            }

            if (devCount == 0)
            {
                Console.WriteLine("Not found device");
                return ret;
            }

            //2.打开锁
            ret = MowayCS.mw_open(ref devInfoArray[0], MowayCS.MW_OPEN_EXCLUSIVE_MODE, ref hHandle);
            if (ret != MowayCS.MW_SUCCESS)
            {
                Console.WriteLine("mw_open failed, {0:X8}", ret);
                return ret;
            }

            // 3.获取所有信息（结构体）
            MW_DEVICE_ALL_ITEM_INFO devAllItemInfo = new MW_DEVICE_ALL_ITEM_INFO();
            UInt32 devAllItemInfoSize = (UInt32)Marshal.SizeOf(devAllItemInfo);
            ret = MowayCS.mw_get_device_all_info(hHandle, Convert.ToByte(MowayCS.MW_GET_INFO_ITEM_ALL), ref devAllItemInfo, ref devAllItemInfoSize);
            if (ret == MowayCS.MW_SUCCESS)
            {
                Console.WriteLine("mw_get_device_all_info pid = {0}", devAllItemInfo.PID);
            }
            else
            {
                Console.WriteLine("mw_get_device_all_info failed, {0:X8}", ret);
            }

            //4.获取指定信息（Item）
            byte[] devItemInfoBuffer = null;
            UInt32 devItemInfoBufferLen = 0;
            // 4.1 获取PID
            devItemInfoBuffer = new byte[128];
            devItemInfoBufferLen = (UInt32)devItemInfoBuffer.Length;
            ret = MowayCS.mw_get_device_info(hHandle, Convert.ToByte(MowayCS.MW_GET_INFO_ITEM_PID), devItemInfoBuffer, ref devItemInfoBufferLen);
            if (ret == MowayCS.MW_SUCCESS)
            {
                Console.WriteLine("mw_get_device_info pid = {0}", devAllItemInfo.PID);
            }
            else
            {
                Console.WriteLine("mw_get_device_info failed, {0:X8}", ret);
            }

            // 4.2 获取SN
            devItemInfoBufferLen = (UInt32)devItemInfoBuffer.Length;    // 重置缓冲区大小
            ret = MowayCS.mw_get_device_info(hHandle, Convert.ToByte(MowayCS.MW_GET_INFO_ITEM_SN), devItemInfoBuffer, ref devItemInfoBufferLen);
            if (ret == MowayCS.MW_SUCCESS)
            {
                // 本例将SN转换字符串，实际内容为二进制数据。开发者需酌情存储相关数据，建议使用十六进制的字符串保存SN。
                Console.WriteLine("mw_get_device_info SN = {0}", BitConverter.ToString(devItemInfoBuffer, 0, 8));
            }
            else
            {
                Console.WriteLine("mw_get_device_info failed, {0:X8}", ret);
            }

            //5.关闭锁
            MowayCS.mw_close(hHandle);

            return MowayCS.MW_SUCCESS;
        }

        /// <summary>
        /// 开发者设置加密锁并初始化
        /// </summary>
        /// <returns></returns>
        /// <remarks>出厂加密锁必须进行初始化后才能使用文件操作、加解密等功能。</remarks>
        public static UInt32 DeveloperInitDevice()
        {
            //1.枚举锁
            //2.打开锁
            //3.验证开发商PIN
            //4.设置PID
            //5.修改开发商PIN
            //6.创建文件（二进制、读写、密钥文件-AES128、密钥文件-HMAC）
            //7.关闭锁

            UInt32 ret = MowayCS.MW_SUCCESS;
            UInt32 devInfoArraySize = 32;       // 获取设备的最大数量
            MW_DEVICE_INFO_CTX[] devInfoArray = new MW_DEVICE_INFO_CTX[devInfoArraySize];

            UInt32 devCount = 0;
            IntPtr hHandle = IntPtr.Zero;

            //1.枚举锁
            ret = MowayCS.mw_enum(devInfoArray, devInfoArraySize, ref devCount);
            if (ret != MowayCS.MW_SUCCESS)
            {
                Console.WriteLine("mw_enum failed, {0:X8}", ret);
                return ret;
            }

            if (devCount == 0)
            {
                Console.WriteLine("Not found device");
                return ret;
            }

            //2.打开锁
            ret = MowayCS.mw_open(ref devInfoArray[0], MowayCS.MW_OPEN_EXCLUSIVE_MODE, ref hHandle);
            if (ret != MowayCS.MW_SUCCESS)
            {
                Console.WriteLine("mw_open failed, {0:X8}", ret);
                return ret;
            }

            //3.验证开发商PIN
            ret = MowayCS.mw_verify_pin(hHandle, Convert.ToByte(MowayCS.MW_PIN_TYPE_DEVELOPER), Encoding.ASCII.GetBytes(Program.developerPIN));
            if (ret != MowayCS.MW_SUCCESS)
            {
                Console.WriteLine("mw_verify_pin failed, {0:X8}", ret);
                MowayCS.mw_close(hHandle);
                return ret;
            }

            //4.设置PID
            // 4.1 设置PID前获取加密锁信息
            MW_DEVICE_ALL_ITEM_INFO devAllItemInfo = new MW_DEVICE_ALL_ITEM_INFO();
            UInt32 devAllItemInfoSize = (UInt32)Marshal.SizeOf(devAllItemInfo);
            ret = MowayCS.mw_get_device_all_info(hHandle, Convert.ToByte(MowayCS.MW_GET_INFO_ITEM_ALL), ref devAllItemInfo, ref devAllItemInfoSize);
            if (ret == MowayCS.MW_SUCCESS)
            {
                Console.WriteLine("mw_get_device_all_info pid = {0}", devAllItemInfo.PID);
            }
            else
            {
                Console.WriteLine("mw_get_device_all_info failed, {0:X8}", ret);
            }

            // 4.2 设置加密锁PID
            byte[] PIDSeed = Encoding.ASCII.GetBytes("12345678");  // PID由加密锁根据PIDSeed，相同的PIDSeed 生成的PID相同。
            // 通常情况下，开发者通过PID来区分当前加密锁是否为本公司产品对应加密锁。
            // 开发者也可为每一款产品对应加密锁分配独立的PID。
            UInt32 PIDSeedLen = (UInt32)PIDSeed.Length;
            ret = MowayCS.mw_set_pid(hHandle, PIDSeed, PIDSeedLen);
            if (ret == MowayCS.MW_SUCCESS)
            {
                Console.WriteLine("mw_set_pid success.");

            }
            else
            {
                Console.WriteLine("mw_set_pid failed, {0:X8}", ret);
            }

            // 4.3 查看生成PID
            ret = MowayCS.mw_get_device_all_info(hHandle, Convert.ToByte(MowayCS.MW_GET_INFO_ITEM_ALL), ref devAllItemInfo, ref devAllItemInfoSize);
            if (ret == MowayCS.MW_SUCCESS)
            {
                Console.WriteLine("mw_get_device_all_info pid = {0}", devAllItemInfo.PID);
            }
            else
            {
                Console.WriteLine("mw_get_device_all_info failed, {0:X8}", ret);
            }

            //5.修改开发商PIN
            string currentDeveloperPIN = Program.developerPIN;
            byte[] oldPIN = Encoding.ASCII.GetBytes(currentDeveloperPIN);
            byte[] newPIN = Encoding.ASCII.GetBytes(Program.developerPIN);  // 本例仅用于演示接口使用方法，设置新PIN与旧PIN相同。
            UInt16 limitCount = 0;    // 使用默认值不限制重试次数
            ret = MowayCS.mw_change_pin(hHandle, Convert.ToByte(MowayCS.MW_PIN_TYPE_DEVELOPER), limitCount, oldPIN, newPIN);
            if (ret == MowayCS.MW_SUCCESS)
            {
                Console.WriteLine("mw_change_pin success oldPIN = {0}, newPIN = {1}.", currentDeveloperPIN, Program.developerPIN);
            }
            else
            {
                Console.WriteLine("mw_change_pin failed, {0:X8}", ret);
            }

            //6.创建文件（二进制、读写、密钥文件-AES128、密钥文件-HMAC）
            byte[] writeBuffer = null;
            UInt32 writeOffset = 0;
            UInt32 writeBufferLen = 0;

            // 6.1 创建二进制用户权限只读文件
            // 6.1.1 创建锁内文件
            MW_FILE_PROPERTY fileProperty = new MW_FILE_PROPERTY(userRDFilename);
            fileProperty.ucType = Convert.ToByte(MowayCS.MW_FILE_TYPE_BINARY);
            fileProperty.usPrivilege = Convert.ToByte(MowayCS.MW_FILE_PRIV_TYPE_READ);
            fileProperty.uiSize = 128;
            ret = MowayCS.mw_delete_file(hHandle, userRDFilename);
            ret = MowayCS.mw_create_file(hHandle, ref fileProperty);
            if (ret == MowayCS.MW_SUCCESS)
            {
                Console.WriteLine("mw_create_file(MW_FILE_PRIV_TYPE_READ) {0} success.", userRDFilename);
            }
            else if (ret == MowayCS.MW_ERROR_INVALID_DEVICE_FILE_HANDLE)
            {
                Console.WriteLine("mw_create_file(MW_FILE_PRIV_TYPE_READ) {0} existed.", userRDFilename);
            }
            else
            {
                Console.WriteLine("mw_create_file(MW_FILE_PRIV_TYPE_READ) {0} failed, {1:X8}", userRDFilename, ret);
            }

            // 6.1.2 向（用户权限）只读文件写入数据
            writeBuffer = Encoding.ASCII.GetBytes(userUseFileData);
            writeOffset = 0;
            writeBufferLen = (UInt32)writeBuffer.Length;
            ret = MowayCS.mw_write_file(hHandle, userRDFilename, writeOffset, writeBufferLen, writeBuffer);
            if (ret == MowayCS.MW_SUCCESS)
            {
                Console.WriteLine("mw_write_file(MW_FILE_PRIV_TYPE_READ) {0} success.", userRDFilename);
            }
            else
            {
                Console.WriteLine("mw_write_file(MW_FILE_PRIV_TYPE_READ) {0} failed, {1:X8}", userRDFilename, ret);
            }

            // 6.2 创建二进制用户权限可读写文件
            // 6.2.1 创建锁内文件
            fileProperty = new MW_FILE_PROPERTY(userRWFilename);            
            fileProperty.ucType = Convert.ToByte(MowayCS.MW_FILE_TYPE_BINARY);
            fileProperty.usPrivilege = Convert.ToByte(MowayCS.MW_FILE_PRIV_TYPE_READ_WRITE);
            fileProperty.uiSize = 128;
            ret = MowayCS.mw_delete_file(hHandle, userRWFilename);
            ret = MowayCS.mw_create_file(hHandle, ref fileProperty);
            if (ret == MowayCS.MW_SUCCESS)
            {
                Console.WriteLine("mw_create_file(MW_FILE_PRIV_TYPE_READ_WRITE) {0} success.", userRWFilename);
            }
            else if (ret == MowayCS.MW_ERROR_INVALID_DEVICE_FILE_HANDLE)
            {
                Console.WriteLine("mw_create_file(MW_FILE_PRIV_TYPE_READ_WRITE) {0} existed.", userRWFilename);
            }
            else
            {
                Console.WriteLine("mw_create_file(MW_FILE_PRIV_TYPE_READ_WRITE) {0} failed, {1:X8}", userRWFilename, ret);
            }

            // 6.2.2 向（用户权限）可读写文件写入数据
            writeBuffer = Encoding.ASCII.GetBytes(userUseFileData);
            writeOffset = 0;
            writeBufferLen = (UInt32)writeBuffer.Length;
            ret = MowayCS.mw_write_file(hHandle, userRWFilename, writeOffset, writeBufferLen, writeBuffer);
            if (ret == MowayCS.MW_SUCCESS)
            {
                Console.WriteLine("mw_write_file(MW_FILE_PRIV_TYPE_READ_WRITE) {0} success.", userRWFilename);
            }
            else
            {
                Console.WriteLine("mw_write_file(MW_FILE_PRIV_TYPE_READ_WRITE) {0} failed, {1:X8}", userRWFilename, ret);
            }

            // 6.3 创建密钥文件-AES128
            // 6.3.1 创建锁内文件
            fileProperty = new MW_FILE_PROPERTY(keyAES128Filename); 
            fileProperty.ucType = Convert.ToByte(MowayCS.MW_FILE_TYPE_KEY);
            fileProperty.usPrivilege = Convert.ToByte(MowayCS.MW_FILE_PRIV_TYPE_USE);
            fileProperty.uiSize = 128;
            ret = MowayCS.mw_delete_file(hHandle, keyAES128Filename);
            ret = MowayCS.mw_create_file(hHandle, ref fileProperty);
            if (ret == MowayCS.MW_SUCCESS)
            {
                Console.WriteLine("mw_create_file(MW_FILE_PRIV_TYPE_USE) {0} success.", keyAES128Filename);
            }
            else if (ret == MowayCS.MW_ERROR_INVALID_DEVICE_FILE_HANDLE)
            {
                Console.WriteLine("mw_create_file(MW_FILE_PRIV_TYPE_USE) {0} existed.", keyAES128Filename);
            }
            else
            {
                Console.WriteLine("mw_create_file(MW_FILE_PRIV_TYPE_USE) {0} failed, {1:X8}", keyAES128Filename, ret);
            }

            // 6.3.2 读取本地密钥文件数据并写入加密锁
            FileStream fileStreamAES128 = new FileStream(keyAES128Filename, FileMode.Open);
            BinaryReader bReaderAES128 = new BinaryReader(fileStreamAES128);
            writeBuffer = bReaderAES128.ReadBytes((int)fileStreamAES128.Length);
            writeOffset = 0;
            writeBufferLen = (UInt32)writeBuffer.Length;
            ret = MowayCS.mw_write_file(hHandle, keyAES128Filename, writeOffset, writeBufferLen, writeBuffer);
            if (ret == MowayCS.MW_SUCCESS)
            {
                Console.WriteLine("mw_write_file(MW_FILE_TYPE_KEY) {0} success.", keyAES128Filename);
            }
            else
            {
                Console.WriteLine("mw_write_file(MW_FILE_TYPE_KEY) {0} failed, {1:X8}", keyAES128Filename, ret);
            }

            // 6.4 创建密钥文件-HMAC
            // 6.3.1 创建锁内文件
            fileProperty = new MW_FILE_PROPERTY(keyHMACMD5Filename);
            fileProperty.ucType = Convert.ToByte(MowayCS.MW_FILE_TYPE_KEY);
            fileProperty.usPrivilege = Convert.ToByte(MowayCS.MW_FILE_PRIV_TYPE_USE);
            fileProperty.uiSize = 128;
            ret = MowayCS.mw_delete_file(hHandle, keyHMACMD5Filename);
            ret = MowayCS.mw_create_file(hHandle, ref fileProperty);
            if (ret == MowayCS.MW_SUCCESS)
            {
                Console.WriteLine("mw_create_file(MW_FILE_PRIV_TYPE_USE) {0} success.", keyHMACMD5Filename);
            }
            else if (ret == MowayCS.MW_ERROR_INVALID_DEVICE_FILE_HANDLE)
            {
                Console.WriteLine("mw_create_file(MW_FILE_PRIV_TYPE_USE) {0} existed.", keyHMACMD5Filename);
            }
            else
            {
                Console.WriteLine("mw_create_file(MW_FILE_PRIV_TYPE_USE) {0} failed, {1:X8}", keyHMACMD5Filename, ret);
            }

            // 6.3.2 读取本地密钥文件数据并写入加密锁
            FileStream fileStream = new FileStream(keyHMACMD5Filename, FileMode.Open);
            BinaryReader bReader = new BinaryReader(fileStream);
            writeBuffer = bReader.ReadBytes((int)fileStream.Length);
            writeOffset = 0;
            writeBufferLen = (UInt32)writeBuffer.Length;
            ret = MowayCS.mw_write_file(hHandle, keyHMACMD5Filename, writeOffset, writeBufferLen, writeBuffer);
            if (ret == MowayCS.MW_SUCCESS)
            {
                Console.WriteLine("mw_write_file(MW_FILE_TYPE_KEY) {0} success.", keyHMACMD5Filename);
            }
            else
            {
                Console.WriteLine("mw_write_file(MW_FILE_TYPE_KEY) {0} failed, {1:X8}", keyHMACMD5Filename, ret);
            }

            //7.关闭锁
            MowayCS.mw_close(hHandle);

            return MowayCS.MW_SUCCESS;
        }

        /// <summary>
        /// 演示用户权限完成文件读写和加解密操作
        /// </summary>
        /// <returns></returns>
        public static UInt32 UserUseDevice()
        {
            //1.枚举锁
            //2.打开锁
            //3.验证用户PIN
            //4.修改用户PIN
            //5.读只读文件
            //6.修改可读写数据文件
            //7.使用密钥文件加解密（AES128）
            //8.使用密钥文件（HMAC）计算Hash值
            //9.关闭锁

            UInt32 ret = MowayCS.MW_SUCCESS;
            UInt32 devInfoArraySize = 32;       // 获取设备的最大数量
            MW_DEVICE_INFO_CTX[] devInfoArray = new MW_DEVICE_INFO_CTX[devInfoArraySize];

            UInt32 devCount = 0;
            IntPtr hHandle = IntPtr.Zero;

            //1.枚举锁
            ret = MowayCS.mw_enum(devInfoArray, devInfoArraySize, ref devCount);
            if (ret != MowayCS.MW_SUCCESS)
            {
                Console.WriteLine("mw_enum failed, {0:X8}", ret);
                return ret;
            }

            if (devCount == 0)
            {
                Console.WriteLine("Not found device");
                return ret;
            }

            //2.打开锁
            ret = MowayCS.mw_open(ref devInfoArray[0], MowayCS.MW_OPEN_EXCLUSIVE_MODE, ref hHandle);
            if (ret != MowayCS.MW_SUCCESS)
            {
                Console.WriteLine("mw_open failed, {0:X8}", ret);
                return ret;
            }

            //3.验证用户PIN
            ret = MowayCS.mw_verify_pin(hHandle, Convert.ToByte(MowayCS.MW_PIN_TYPE_USER), Encoding.ASCII.GetBytes(Program.userPIN));
            if (ret == MowayCS.MW_SUCCESS)
            {
                Console.WriteLine("mw_verify_pin success PIN = {0}", Program.userPIN);
            }
            else
            {
                Console.WriteLine("mw_verify_pin failed, {0:X8}", ret);
                MowayCS.mw_close(hHandle);
                return ret;
            }

            //4.修改用户PIN
            byte[] oldPIN = Encoding.ASCII.GetBytes(Program.userPIN);
            byte[] newPIN = Encoding.ASCII.GetBytes(Program.userPIN);  // 本例仅用于演示接口使用方法，设置新PIN与旧PIN相同。
            UInt16 limitCount = 0;    // 使用默认值不限制重试次数
            ret = MowayCS.mw_change_pin(hHandle, Convert.ToByte(MowayCS.MW_PIN_TYPE_USER), limitCount, oldPIN, newPIN);
            if (ret == MowayCS.MW_SUCCESS)
            {
                Console.WriteLine("mw_change_pin success oldPIN = {0}, newPIN = {1}.", Program.userPIN, Program.userPIN);
            }
            else
            {
                Console.WriteLine("mw_change_pin failed, {0:X8}", ret);
            }

            //5.读只读文件
            byte[] readBuffer = null;
            UInt32 readBufferLen = 0;
            UInt32 readOffset = 0;

            readBuffer = new byte[128];
            readBufferLen = (UInt32)userUseFileData.Length;
            ret = MowayCS.mw_read_file(hHandle, userRDFilename, readOffset, readBufferLen, readBuffer);
            if (ret == MowayCS.MW_SUCCESS)
            {
                Console.WriteLine("mw_read_file {0} success, {1}", userRDFilename, Encoding.ASCII.GetString(readBuffer));
            }
            else
            {
                Console.WriteLine("mw_read_file {0} failed, {1:X8}", userRDFilename, ret);
            }

            //6.修改可读写数据文件
            // 6.1 向可读写文件写入数据
            byte[] writeBuffer = Encoding.ASCII.GetBytes("user privilege write!");
            UInt32 writeOffset = 0;
            UInt32 writeBufferLen = (UInt32)writeBuffer.Length;
            ret = MowayCS.mw_write_file(hHandle, userRWFilename, writeOffset, writeBufferLen, writeBuffer);
            if (ret == MowayCS.MW_SUCCESS)
            {
                Console.WriteLine("mw_write_file {0} success.", userRWFilename);

                // 6.2 读取可读写文件内容
                readBufferLen = (UInt32)readBuffer.Length;  // 重置缓冲区长度
                ret = MowayCS.mw_read_file(hHandle, userRWFilename, readOffset, readBufferLen, readBuffer);
                if (ret == MowayCS.MW_SUCCESS)
                {
                    Console.WriteLine("mw_read_file {0} success, {1}", userRWFilename, Encoding.ASCII.GetString(readBuffer));
                }
                else
                {
                    Console.WriteLine("mw_read_file {0} failed, {1:X8}", userRWFilename, ret);
                }
            }
            else
            {
                Console.WriteLine("mw_write_file {0} failed, {1:X8}", userRWFilename, ret);
            }

            //7.使用密钥文件加解密（AES128）
            string encrypTestData = "1234567890123456";
            byte[] data = Encoding.ASCII.GetBytes(encrypTestData);
            UInt32 dataLen = (UInt32)data.Length;
            byte[] iv = null;

            byte[] encryptData = new byte[128];
            UInt32 encryptDataLen = (UInt32)encryptData.Length;

            byte[] decryptData = new byte[128];
            UInt32 decryptDataLen = (UInt32)decryptData.Length;

            // 7.1 数据加密
            ret = MowayCS.mw_sym_encrypt(hHandle, keyAES128Filename, Convert.ToByte(MowayCS.MW_SYM_ALGO_MODE_ECB), null, data, dataLen, encryptData, ref encryptDataLen);
            if (ret == MowayCS.MW_SUCCESS)
            {
                Console.WriteLine("mw_sym_encrypt({0}) success, encryptDataLen = {1}", keyAES128Filename, encryptDataLen);

                // 7.2 数据解密
                ret = MowayCS.mw_sym_decrypt(hHandle, keyAES128Filename, Convert.ToByte(MowayCS.MW_SYM_ALGO_MODE_ECB), null, encryptData, encryptDataLen, decryptData, ref decryptDataLen);
                if (ret == MowayCS.MW_SUCCESS)
                {
                    Console.WriteLine("mw_sym_decrypt {0} success, {1}", keyAES128Filename, Encoding.ASCII.GetString(decryptData));

                    // 比较加解密数据
                    if (Program.CompareBytes(data, decryptData, (int)dataLen))
                    {
                        Console.WriteLine("data == decryptData");
                    }
                }
                else
                {
                    Console.WriteLine("mw_sym_decrypt({0}) failed, {1:X8}", keyAES128Filename, ret);
                }
            }
            else
            {
                Console.WriteLine("mw_sym_encrypt {0} failed, {1:X8}", keyAES128Filename, ret);
            }


            //8.使用密钥文件（HMAC）计算Hash值
            byte[] hmacData = new byte[128];            // 重置数据
            UInt32 hmacDataLen = (UInt32)hmacData.Length;
            ret = MowayCS.mw_hmac_calc(hHandle, keyHMACMD5Filename, data, dataLen, hmacData, ref hmacDataLen);
            if (ret == MowayCS.MW_SUCCESS)
            {
                Console.WriteLine("mw_hmac_calc({0}) success, md5 = {1}", keyHMACMD5Filename, BitConverter.ToString(hmacData));
            }
            else
            {
                Console.WriteLine("mw_hmac_calc {0} failed, {1:X8}", keyHMACMD5Filename, ret);
            }

            //9.关闭锁
            MowayCS.mw_close(hHandle);

            return MowayCS.MW_SUCCESS;
        }
    }
}
