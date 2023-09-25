// iscsi.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <windows.h>
#include<iscsierr.h>
#include <iscsidsc.h>
#include<iostream>
#include<string>
#pragma comment(lib,"iscsidsc.lib")
int main(int argc,char* argv[])
{
    bool isOk = false;
    if (argc > 3) {
        PSTR host = (PSTR)argv[1];
        int port = std::stoi(argv[2]);
        PSTR iqn = (PSTR)argv[3];

        ISCSI_LOGIN_OPTIONS ilo;
        memset(&ilo, 0, sizeof(ISCSI_LOGIN_OPTIONS));
        if (argc > 5) {
            ilo.InformationSpecified = ISCSI_LOGIN_OPTIONS_USERNAME | ISCSI_LOGIN_OPTIONS_PASSWORD | ISCSI_LOGIN_OPTIONS_AUTH_TYPE;
            ilo.AuthType = ISCSI_CHAP_AUTH_TYPE;
            ilo.Username = (UCHAR*)argv[4];
            ilo.UsernameLength = strlen(argv[4]);
            ilo.Password = (UCHAR*)argv[5];
            ilo.PasswordLength = strlen(argv[5]);
        }
        

        // 初始化 COM 库
        HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
        if (FAILED(hr)) {
            // 处理初始化错误
            return 1;
        }
        ISCSI_VERSION_INFO ivi;
        GetIScsiVersionInformation(&ivi);

        SetupPersistentIScsiDevices();

        ISDSC_STATUS is;
        is = SetIScsiInitiatorNodeName(iqn);

        ISCSI_TARGET_PORTAL itp;
        memset(&itp, 0, sizeof(ISCSI_TARGET_PORTAL));
        wsprintf(itp.Address, host);
        itp.Socket = port;

        ISCSI_TARGET_PORTAL_GROUP itpg;
        itpg.Count = 1;
        itpg.Portals[0] = itp;

        
        //ilo.LoginFlags = ISCSI_LOGIN_FLAG_ALLOW_PORTAL_HOPPING;
        ISCSI_UNIQUE_SESSION_ID iusi;
        ISCSI_UNIQUE_CONNECTION_ID iuci;
        memset(&iusi, 0, sizeof(ISCSI_UNIQUE_SESSION_ID));
        memset(&iuci, 0, sizeof(ISCSI_UNIQUE_CONNECTION_ID));

        is = AddIScsiSendTargetPortal(NULL, ISCSI_ANY_INITIATOR_PORT, &ilo, 0, &itp);
        is = AddIScsiStaticTarget(iqn, NULL, 0, true, NULL, &ilo, &itpg);
        ULONG ret = LoginIScsiTarget(iqn, false, NULL, ISCSI_ANY_INITIATOR_PORT, NULL, 0, NULL, &ilo, 0, NULL, false, &iusi, &iuci);
        if (ret == 0) {
            isOk = true;
        }

        // 清理资源
        CoUninitialize();
    }

    if (isOk) {
        std::cout << "success" << std::endl;
    }
    else {
        std::cout << "failed" << std::endl;
    }
    
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
