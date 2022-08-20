#include "inject_playground.h"
#include <Windows.h>
#include <shobjidl.h>
#include "strconvert.h"

inject_playground::inject_playground() : renderable::renderable("inject_playground") {

};

char dll_path[MAX_PATH];
void inject_playground::on_render() {
	ImGui::Begin("Inject Playground");
    ImGui::InputText("DLL Path", dll_path, MAX_PATH);
    ImGui::SameLine();
    if (ImGui::Button("...")) {
        HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
        if (SUCCEEDED(hr))
        {
            IFileOpenDialog* pFileOpen;

            // Create the FileOpenDialog object.
            hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
                IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));

            if (SUCCEEDED(hr))
            {
                // Show the Open dialog box.
                hr = pFileOpen->Show(NULL);

                // Get the file name from the dialog box.
                if (SUCCEEDED(hr))
                {
                    IShellItem* pItem;
                    hr = pFileOpen->GetResult(&pItem);
                    if (SUCCEEDED(hr))
                    {
                        wchar_t* pszFilePath;
                        hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

                        // Display the file name to the user.
                        if (SUCCEEDED(hr))
                        {
                            std::wstring wide_path = std::wstring(pszFilePath);
                            std::string str_path = strconvert::ws_to_s(wide_path);
                            strcpy_s(dll_path, str_path.c_str());
                            CoTaskMemFree(pszFilePath);
                        }
                        pItem->Release();
                    }
                }
                pFileOpen->Release();
            }
            CoUninitialize();
        }
    }
    if (ImGui::Button("LoadLibraryA")) {
        HANDLE inject_thread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)LoadLibraryA, &dll_path, NULL, NULL);
        if(inject_thread == INVALID_HANDLE_VALUE) {
            MessageBoxA(nullptr, "Failed to load DLL at path", "Inject Playground", MB_OK);
        }
    }
	ImGui::End();
}