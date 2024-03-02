#include <Windows.h>
#include <stdio.h>

int main(int argc, char* argv) {
    if (argc == 2 && argv[1] == "init") {
        printf("�����ʼ��\n");
    } else if (argc == 1) {
        HKEY hKey;
        LONG lResult = RegOpenKeyEx(HKEY_CURRENT_USER,
            "Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings",
            0, KEY_READ, &hKey);
        if (lResult != ERROR_SUCCESS) {
            printf("��ע����ȡ����ʱ����: %ld\n", lResult);
            return 1;
        }

        // ��ȡע����ֵ
        DWORD dwType, dwBufferSize;
        char szProxyServer[256];
        dwBufferSize = sizeof(szProxyServer);
        lResult = RegQueryValueEx(hKey, "ProxyServer", NULL, &dwType,
            (LPBYTE)szProxyServer, &dwBufferSize);
        if (lResult == ERROR_SUCCESS) {
            if (dwType == REG_SZ)
                printf("������Git����: %s\n", szProxyServer);
            else
                printf("ע�������ͷǷ�\n");
        } else
            printf("��ע����ȡ����ʱ����: %ld\n", lResult);

        RegCloseKey(hKey);

        char cmd[256] = {0};
        sprintf(
            cmd, "git config --global https.proxy http://%s", szProxyServer);
        system(cmd);
        sprintf(cmd, "git config --global http.proxy http://%s", szProxyServer);
        system(cmd);
    } else {
        printf("Usage: \"proxy [init]\"\n\n");
        return 1;
    }
    return 0;
}
