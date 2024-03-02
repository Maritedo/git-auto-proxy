#include <Windows.h>
#include <stdio.h>

int main(int argc, char* argv) {
    if (argc == 2 && argv[1] == "init") {
        printf("无需初始化\n");
    } else if (argc == 1) {
        HKEY hKey;
        LONG lResult = RegOpenKeyEx(HKEY_CURRENT_USER,
            "Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings",
            0, KEY_READ, &hKey);
        if (lResult != ERROR_SUCCESS) {
            printf("从注册表读取代理时出错: %ld\n", lResult);
            return 1;
        }

        // 读取注册表键值
        DWORD dwType, dwBufferSize;
        char szProxyServer[256];
        dwBufferSize = sizeof(szProxyServer);
        lResult = RegQueryValueEx(hKey, "ProxyServer", NULL, &dwType,
            (LPBYTE)szProxyServer, &dwBufferSize);
        if (lResult == ERROR_SUCCESS) {
            if (dwType == REG_SZ)
                printf("已设置Git代理: %s\n", szProxyServer);
            else
                printf("注册表键类型非法\n");
        } else
            printf("从注册表读取代理时出错: %ld\n", lResult);

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
