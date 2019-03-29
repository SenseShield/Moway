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
    class MowayUpdatePkg
    {
        private static string updatePkgUpdateFilename = "userrd.evd";
        private static string updatePkgUpdateLocalFilename = "test_file/userrd.evd";

        private static string updatePkgCreateFilename = "userrdpkg.evd";
        private static string updatePkgCreateLocalFilename = "test_file/userrdpkg.evd";

        //private static string updatePkgFilename = "updatepkg.evd";
        //private static string updatePkgLocalFilename = "test_file/updatepkg.evd";

        private static byte[] pkgBuffer = new byte[4096];
        private static UInt32 pkgBufferLen = (UInt32)pkgBuffer.Length;

        /// <summary>
        /// A、开发商制作升级包
        /// </summary>
        /// <returns></returns>
        public static UInt32 DeveloperMakeUpdatePkg()
        {
            //1.枚举锁
            //2.打开锁
            //3.验证开发商PIN
            //4.制作升级包（升级二进制数据文件）
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

            //3.验证开发商PIN
            ret = MowayCS.mw_verify_pin(hHandle, Convert.ToByte(MowayCS.MW_PIN_TYPE_DEVELOPER), Encoding.ASCII.GetBytes(Program.developerPIN));
            if (ret != MowayCS.MW_SUCCESS)
            {
                Console.WriteLine("mw_verify_pin failed, {0:X8}", ret);
                MowayCS.mw_close(hHandle);
                return ret;
            }

            //4.制作升级包（升级二进制数据文件）
            // 获取设备PID，本例使用当前计算机插入加密锁PID，PID可以通过其他途径获取，例如开发者维护已发布产品与加密锁PID关系，远程升级时获取PID制作升级包。
            UInt32 devPID = GetDevicePIDFromPC(hHandle); // （必填）在未指定SN参数时，升级包可用于相同PID的所有加密锁。
            byte[] devSN = null;                         // （选填）指定SN后，升级包只能用于指定的SN才能使用。
            
            MW_UPDADE_FILE_CTX[] filePathCtx = new MW_UPDADE_FILE_CTX[2];
            UInt32 filePathCtxCount = (UInt32)filePathCtx.Length;

            // 4.2 更新文件内容
            filePathCtx[0].stFileProperty.acName = new byte[MowayCS.MW_FILE_NAME_LENGTH_MAX + 1];
            Array.Copy(Encoding.ASCII.GetBytes(updatePkgUpdateFilename), filePathCtx[0].stFileProperty.acName, updatePkgUpdateFilename.Length);

            filePathCtx[0].stFileProperty.uiSize = 128;
            filePathCtx[0].stFileProperty.ucType = Convert.ToByte(MowayCS.MW_FILE_TYPE_BINARY);
            filePathCtx[0].stFileProperty.usPrivilege = Convert.ToByte(MowayCS.MW_FILE_PRIV_TYPE_READ);

            FileStream fileStreamUpdate = new FileStream(updatePkgUpdateLocalFilename, FileMode.Open);
            BinaryReader bReaderUpdate = new BinaryReader(fileStreamUpdate);
            // 申请非托管内存，将需要升级的数据拷贝至非托管内存
            byte[] dataUpdate = bReaderUpdate.ReadBytes((int)fileStreamUpdate.Length);
            filePathCtx[0].uiDateSize = dataUpdate.Length;
            filePathCtx[0].pData = Marshal.AllocHGlobal(dataUpdate.Length);
            Marshal.Copy(dataUpdate, 0, filePathCtx[0].pData, dataUpdate.Length);

            bReaderUpdate.Close();
            fileStreamUpdate.Close();
            
            // 4.2 创建新文件
            filePathCtx[1].stFileProperty.acName = new byte[MowayCS.MW_FILE_NAME_LENGTH_MAX + 1];
            Array.Copy(Encoding.ASCII.GetBytes(updatePkgCreateFilename), filePathCtx[1].stFileProperty.acName, updatePkgCreateFilename.Length);

            filePathCtx[1].stFileProperty.uiSize = 128;
            filePathCtx[1].stFileProperty.ucType = Convert.ToByte(MowayCS.MW_FILE_TYPE_BINARY);
            filePathCtx[1].stFileProperty.usPrivilege = Convert.ToByte(MowayCS.MW_FILE_PRIV_TYPE_READ);

            FileStream fileStreamCreate = new FileStream(updatePkgCreateLocalFilename, FileMode.Open);
            BinaryReader bReaderCreate = new BinaryReader(fileStreamCreate);
            // 申请非托管内存，将需要升级的数据拷贝至非托管内存
            byte[] dataCreate = bReaderCreate.ReadBytes((int)fileStreamCreate.Length);
            filePathCtx[1].uiDateSize = dataCreate.Length;
            filePathCtx[1].pData = Marshal.AllocHGlobal(dataCreate.Length);
            Marshal.Copy(dataCreate, 0, filePathCtx[1].pData, dataCreate.Length);

            bReaderCreate.Close();
            fileStreamCreate.Close();

            ret = MowayCS.mw_make_update_pkg(hHandle, devPID, devSN, filePathCtx, filePathCtxCount, pkgBuffer, ref pkgBufferLen);
            if (ret == MowayCS.MW_SUCCESS)
            {
                Console.WriteLine("mw_make_update_pkg pid = {0}, pkg_len = {1}", devPID, pkgBufferLen);
            }
            else
            {
                Console.WriteLine("mw_make_update_pkg failed, {0:X8}", ret);
            }

            // 释放非托管内存
            Marshal.FreeHGlobal(filePathCtx[0].pData);
            Marshal.FreeHGlobal(filePathCtx[1].pData);

            //5.关闭锁
            MowayCS.mw_close(hHandle);

            return MowayCS.MW_SUCCESS;
        }

        /// <summary>
        /// B、用户导入升级包
        /// </summary>
        /// <returns></returns>
        public static UInt32 UserUpdatePkg()
        {
            //1.枚举锁
            //2.打开锁
            //3.验证用户PIN
            //4.导入升级包
            //5.验证升级包内容（读取验证内容与升级内容一致）
            //6.关闭锁

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
            if (ret != MowayCS.MW_SUCCESS)
            {
                Console.WriteLine("mw_verify_pin failed, {0:X8}", ret);
                MowayCS.mw_close(hHandle);
                return ret;
            }

            //4.导入升级包
            // 注意：导入前已经调用 DeveloperMakeUpdatePkg() 生成数据包内容，并且当前加密锁的PID与升级包PID相同。
            ret = MowayCS.mw_update(hHandle, pkgBuffer, pkgBufferLen);
            if (ret == MowayCS.MW_SUCCESS)
            {
                Console.WriteLine("mw_update success.");
            }
            else
            {
                Console.WriteLine("mw_update failed, {0:X8}", ret);
            }


            //5.验证升级包内容（读取验证内容与升级内容一致）
            // 5.1 读取升级后的锁内文件内容
            byte[] readBuffer = null;
            UInt32 readBufferLen = 0;
            UInt32 readOffset = 0;

            readBuffer = new byte[128];
            readBufferLen = (UInt32)readBuffer.Length;
            ret = MowayCS.mw_read_file(hHandle, updatePkgUpdateFilename, readOffset, readBufferLen, readBuffer);
            if (ret == MowayCS.MW_SUCCESS)
            {
                Console.WriteLine("mw_read_file {0} success, {1}", updatePkgUpdateFilename, Encoding.ASCII.GetString(readBuffer));
            }
            else
            {
                Console.WriteLine("mw_read_file {0} failed, {1:X8}", updatePkgUpdateFilename, ret);
            }

            // 5.2 对比文件内容与升级包写入内容相同
            FileStream fileStream = new FileStream(updatePkgUpdateLocalFilename, FileMode.Open);
            BinaryReader bReader = new BinaryReader(fileStream);
            byte[] updateDataBuffer = bReader.ReadBytes((int)fileStream.Length);
            int updateDataBufferLen = updateDataBuffer.Length;
            bReader.Close();
            fileStream.Close();

            if (Program.CompareBytes(readBuffer, updateDataBuffer, updateDataBufferLen))
            {
                Console.WriteLine("mw_update success, readBuffer = updateDataBuffer");
            }
            else
            {
                Console.WriteLine("mw_update failed, readBuffer != updateDataBuffer");
            }

            //6.关闭锁
            MowayCS.mw_close(hHandle);

            return MowayCS.MW_SUCCESS;
        }

        /// <summary>
        /// 从当前插入加密锁获取PID
        /// </summary>
        /// <param name="hHandle">当前插入设备句柄</param>
        /// <returns>加密锁PID</returns>
        private static UInt32 GetDevicePIDFromPC(IntPtr hHandle)
        {
            UInt32 ret = MowayCS.MW_SUCCESS;
            UInt32 devPID = 0;

            MW_DEVICE_ALL_ITEM_INFO devAllItemInfo = new MW_DEVICE_ALL_ITEM_INFO();
            UInt32 devAllItemInfoSize = (UInt32)Marshal.SizeOf(devAllItemInfo);
            ret = MowayCS.mw_get_device_all_info(hHandle, Convert.ToByte(MowayCS.MW_GET_INFO_ITEM_ALL), ref devAllItemInfo, ref devAllItemInfoSize);
            if (ret == MowayCS.MW_SUCCESS)
            {
                devPID = devAllItemInfo.PID;
            }
            else
            {
                // 示例演示代码，在插锁的情况下不会跑出异常。
                throw new Exception("Get device PID failed");
            }
               
            return devPID;
        }

    }
}
