
������ʾʹ���ڴ���صķ�ʽ�� Delphi ��ʹ�� moway �ӿڣ�ʵ�������ع��ܽӿڼ�Ӳ�������ƿ��Ƶȹ��ܡ�

�ļ�˵����
BTMemory_x86.pas BTMemory��֧��Delphi XE��֮ǰ�汾����Delphi7���ã���
BTMemory_x86_64.pas BTMemory��֧��Delphi XE2��֮��汾��
moway.pas ħ��SDK API�ӿڶ���ͷ�ļ����ļ������� moway_api.dll.inc �ļ���
moway_api.dll.inc ��¼ ħ��̬���ļ���moway_api.dll��ת���ɶ����Ƶ��������ݣ����� moway.pas ʱ�ر��������ļ���
moway_dll2inc.exe ħ��̬���ļ�ת���� inc �����ļ����ߣ�����ʹ�÷������������"DLLתINC����ʹ��˵��"��

ʹ��˵����
1.�½����̣����� moway.pas BTMemory_x86.pas BTMemory_x86_64.pas ����ĿĿ¼������ moway��
2.���ݹ�������ƽ̨��32-bit �� 64-bit���ֱ�� {SDK��װĿ¼} Ŀ¼���� moway_api.dll ����ǰ��ĿĿ¼��ʹ�� moway_dll2inc.exe ���߽� DLL ת��Ϊ inc �ļ���

ע�����
1.�л�����ѡ��Debug��Release��32-bit��64-bitʱ����Ҫ�ֶ�ɾ����Ŀ����Ŀ¼�µ�*.dcu�ļ����������Ϊ����Ĭ�����������ɵ�dcu����ִ�н����������һ�µ������

�������⣺
1.�л���������ƽ̨��32-bit -> 64-bit��ʱ��ʾ���´���
[DCC Fatal Error] F2048 Bad unit format: 'moway.dcu' - Expected version: 23.0, Windows Unicode(x64) Found version: 23.0, Windows Unicode(x86)
���������ɾ������Ŀ¼���Ѿ����ɵ�.dcu�ļ������±��뼴�ɡ�
2.���г���ʱ���������򵯴���ʾ�����쳣��ַ�������� $C0000005��������̬���뵱ǰ��������ƽ̨��һ�£�32-bit��64-bit��������"ʹ��˵��"-"����2"��"ע������"���в��������¹������򼴿ɡ�

DLLתINC����ʹ��˵����
moway_dll2inc.exe ����˵����
  ����1���������Ҫת����DLL�ļ�·��
  ����2����ѡ����ָ������INC�ļ�·��
  ʾ��1��ʹ��Ĭ������ļ�·��(${filename}.inc) moway_dll2inc.exe moway_api.dll������ ${filename} ��ʾ�����ļ�·����
  ʾ��2��ָ������ļ�·�� moway_dll2inc.exe moway_api.dll moway_api.dll.inc
  ע�⣺���� moway.pas ����Ҫ�� inc �ļ��������� moway_api.dll.inc����������ʧ��
ʹ�ù��ߵ����ַ�ʽ��
1.��ק�ļ��� moway_dll2inc.exe ����ͼ����ִ�У���ʹ��Ĭ������ļ�·��ת�������ļ���
2.ʹ��������ʾ�����ߣ�ִ��CMD���