// SuppressionRecursivePackage.Net.cpp 
// © Patrice Waechter-Ebling 2025-2026
// Outil complementaire pour git
#include <windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

std::vector<std::string> Dossiers = {"bin","obj","Debug","Release","x64","x86","ipch","packages","TestResults"};
bool SuppressionRecursive(const std::string& Dossier)
{
    std::string searchPath = Dossier + "\\*";
    WIN32_FIND_DATAA fd;
    HANDLE hFind = FindFirstFileA(searchPath.c_str(), &fd);
    if (hFind == INVALID_HANDLE_VALUE)return false;
    do
    {
        std::string name = fd.cFileName;
        if (name == "." || name == "..")continue;
        std::string fullPath = Dossier + "\\" + name;
        if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            SuppressionRecursive(fullPath);
        }
        else
        {
            SetFileAttributesA(fullPath.c_str(), FILE_ATTRIBUTE_NORMAL);
            DeleteFileA(fullPath.c_str());
        }
    } while (FindNextFileA(hFind, &fd));
    FindClose(hFind);
    SetFileAttributesA(Dossier.c_str(), FILE_ATTRIBUTE_NORMAL);
    return RemoveDirectoryA(Dossier.c_str());
}

bool EstCeUnDossier(const std::string& name)
{
    for (auto& f : Dossiers)
    {
        if (_stricmp(name.c_str(), f.c_str()) == 0)return true;
    }
    return false;
}

void RechercherCible(const std::string& root)
{
    std::string searchPath = root + "\\*";
    WIN32_FIND_DATAA fd;
    HANDLE hFind = FindFirstFileA(searchPath.c_str(), &fd);
    if (hFind == INVALID_HANDLE_VALUE)return;
    do
    {
        std::string name = fd.cFileName;
        if (name == "." || name == "..")continue;
        std::string fullPath = root + "\\" + name;
        if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            if (EstCeUnDossier(name))
            {
                std::cout << "Suppression : " << fullPath << std::endl;
                SuppressionRecursive(fullPath);
            }
            else
            {
                RechercherCible(fullPath);
            }
        }
    } while (FindNextFileA(hFind, &fd));
    FindClose(hFind);
}

int main(int argc, char* argv[])
{
    printf("SuppressionRecursive\tv:1.01\t%c Patrice Waechter-Ebling 2025-2026\nOutil complementaire pour git\n", 0xB8);
    if (argc < 2)
    {
        std::cout << "Usage : SuppressionRecursivePackage <dossier racine>\n";
        return 0;
    }
    std::string root = argv[1];
    RechercherCible(root);

    std::cout << "Terminé.\n";
    return 0;
}
