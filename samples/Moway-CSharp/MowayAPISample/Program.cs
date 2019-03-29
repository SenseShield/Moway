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
    class Program
    {
        public static string developerPIN = "000000000000000000000000";  // 出厂默认开发商PIN
        public static string userPIN = "00000000";                 // 出厂默认用户PIN
        
        static void Main(string[] args)
        {
            HelloMoway();

            // 1.开发商设置锁
            MowayDevice.DeveloperInitDevice();

            // 2.获取设备信息（无权限要求）
            MowayDevice.GetDeviceInfo();

            // 3.用户使用加密锁
            MowayDevice.UserUseDevice();

            // 4.升级包
            // A. 开发商制作升级包
            MowayUpdatePkg.DeveloperMakeUpdatePkg();
            // B. 用户导入升级包
            MowayUpdatePkg.UserUpdatePkg();
        }

        /// <summary>
        /// 演示枚举锁、打开锁、验证（开发商）PIN、关闭锁等基本操作
        /// </summary>
        /// <returns></returns>
        private static UInt32 HelloMoway()
        {
            UInt32 ret = MowayCS.MW_SUCCESS;
            UInt32 devInfoArraySize = 32;       // 获取设备的最大数量
            MW_DEVICE_INFO_CTX[] devInfoArray = new MW_DEVICE_INFO_CTX[devInfoArraySize];

            UInt32 devCount = 0;
            IntPtr hHandle = IntPtr.Zero;

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

            ret = MowayCS.mw_open(ref devInfoArray[0], MowayCS.MW_OPEN_EXCLUSIVE_MODE, ref hHandle);
            if (ret != MowayCS.MW_SUCCESS)
            {
                Console.WriteLine("mw_open failed, {0:X8}", ret);
                return ret;
            }

            ret = MowayCS.mw_verify_pin(hHandle, Convert.ToByte(MowayCS.MW_PIN_TYPE_DEVELOPER), Encoding.ASCII.GetBytes(developerPIN));
            if (ret != MowayCS.MW_SUCCESS)
            {
                Console.WriteLine("mw_verify_pin(MW_PIN_TYPE_DEVELOPER) failed, {0:X8}", ret);
                MowayCS.mw_close(hHandle);
                return ret;
            }

            MowayCS.mw_close(hHandle);

            return MowayCS.MW_SUCCESS;
        }


        /// <summary>
        /// 比较两个byte[]数组内容是否相等
        /// </summary>
        /// <param name="b1">数组1</param>
        /// <param name="b2">数组2</param>
        /// <param name="length">比较长度</param>
        /// <returns>是否相等</returns>
        public static bool CompareBytes(byte[] b1, byte[] b2, int length)
        {
            if (b1.Length < length || b2.Length < length)
                return false;

            for (int i = 0; i < length; i++)
            {
                if (b1[i] != b2[i])
                    return false;
            }
            return true;
        }
    }
}
