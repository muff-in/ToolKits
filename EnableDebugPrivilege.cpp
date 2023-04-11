#include <iostream>
#include <Windows.h>

using namespace std;

int main() {

    HANDLE hToken{ NULL };

    BOOL bOpenProcessToken{

        OpenProcessToken(
            GetCurrentProcess(),
            TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY,
            &hToken
            )
    };

    if (bOpenProcessToken == 0) {

        cerr << "OpenProcessToken failed with error code: " << GetLastError() << endl;
    }


    LUID luid{0};

    BOOL bLookupPrivilegeValue{

            LookupPrivilegeValue(
                NULL,
                SE_DEBUG_NAME,
                &luid
            )
    };

    if (bLookupPrivilegeValue == 0) {

        cerr << "LookupPrivilegeValue failed with error code: " << GetLastError() << endl;
    }

    LUID_AND_ATTRIBUTES luidAndAttributes{
        luid,
        SE_PRIVILEGE_ENABLED
    };

    TOKEN_PRIVILEGES tokenPrivilege{
         1,
        luidAndAttributes
    };
    

    BOOL bAdjustTokenPrivilege{

              AdjustTokenPrivileges(
                  hToken,
                  FALSE,
                  &tokenPrivilege,
                  sizeof(TOKEN_PRIVILEGES),
                  NULL,
                  NULL
              )
    };

    if (bAdjustTokenPrivilege == 0) {

        cerr << "AdjustTokenPrivilege failed with error code: " << GetLastError() << endl;
    }


    cout << "SeDebugPrivilege enabled." << std::endl;


    CloseHandle(hToken);
    return 0;
}
