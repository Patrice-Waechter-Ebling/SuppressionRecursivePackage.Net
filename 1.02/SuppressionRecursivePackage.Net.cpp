// SuppressionRecursivePackage.Net.cpp 
// © Patrice Waechter-Ebling 2025-2026
// Outil complementaire pour git
#include <windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <shlobj.h> 
#include <Shlwapi.h> 
#pragma comment(lib, "Shlwapi.lib")
#pragma comment(lib, "shell32.lib")
int nbDossiersSupprimes = 0;
int nbFichiersSupprimes = 0;

std::vector<std::string> Cibles = {"bin","obj","Debug","Release","x64","x86","ipch","packages","TestResults",".vs",".git",".github"};
std::string ParcourirDossier(HWND hOwner, const char* title)
{
    BROWSEINFOA bi = { 0 };
    bi.hwndOwner = hOwner;
    bi.lpszTitle = title;
    bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE; 
    LPITEMIDLIST pidl = SHBrowseForFolderA(&bi);
    if (!pidl)return std::string();
    char path[MAX_PATH] = { 0 };
    if (!SHGetPathFromIDListA(pidl, path))
    {
        CoTaskMemFree(pidl);
        return std::string();
    }
    CoTaskMemFree(pidl);
    return std::string(path);
}
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
        std::string chemin = Dossier + "\\" + name;
        if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            SuppressionRecursive(chemin);
        }
        else
        {
            SetFileAttributesA(chemin.c_str(), FILE_ATTRIBUTE_NORMAL);
            DeleteFileA(chemin.c_str());
            nbFichiersSupprimes++;
        }
    } while (FindNextFileA(hFind, &fd));
    FindClose(hFind);
    SetFileAttributesA(Dossier.c_str(), FILE_ATTRIBUTE_NORMAL);
    nbDossiersSupprimes++;
    return RemoveDirectoryA(Dossier.c_str());
}

bool EstCeUnDossier(const std::string& name)
{
    for (auto& f : Cibles)
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
        std::string chemin = root + "\\" + name;
        if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            if (EstCeUnDossier(name))
            {
                std::cout << "Suppression : " << chemin << std::endl;
                SuppressionRecursive(chemin);
            }
            else
            {
                RechercherCible(chemin);
            }
        }
    } while (FindNextFileA(hFind, &fd));
    FindClose(hFind);
}

int main(int argc, char* argv[])
{
	SetConsoleTitleA("SuppressionRecursive v1.02 - Patrice Waechter-Ebling 2025-2026");
    printf("SuppressionRecursive\tv:1.02\t%c Patrice Waechter-Ebling 2025-2026\nOutil complementaire pour git\n", 0xB8);
    if (argc < 2)
    {
        CoInitialize(NULL);
        std::string folder = ParcourirDossier(NULL, "Choisissez un dossier :");
        if (!folder.empty()) {
            RechercherCible(folder);
            std::cout << "Dossier choisi : " << PathFindFileNameA(folder.c_str()) << std::endl;
            std::cout << "Termine.\n";
            std::cout << "Nombre de dossiers supprimes : " << nbDossiersSupprimes << std::endl;
            std::cout << "Nombre de fichiers supprimes : " << nbFichiersSupprimes << std::endl;

        }
        else {
            std::cout << "Aucun dossier selectionne." << std::endl;
            std::cout << "Usage : SuppressionRecursivePackage <dossier racine>\n";
        }
        std::string dummy = "SuppressionRecursive v1.02 - Patrice Waechter-Ebling 2025-2026 "+folder;
        SetConsoleTitleA(dummy.c_str());
        CoUninitialize();
        return 0;
    }
    else {
        std::string folder = argv[1];
        if (PathFileExistsA(folder.c_str()) && (GetFileAttributesA(folder.c_str()) & FILE_ATTRIBUTE_DIRECTORY))
        {
            RechercherCible(folder);
            std::cout << "Termine.\n";
            std::cout << "Nombre de dossiers supprimes : " << nbDossiersSupprimes << std::endl;
            std::cout << "Nombre de fichiers supprimes : " << nbFichiersSupprimes << std::endl;

        }
        else
        {
            std::cout << "Le chemin specifie n'est pas un dossier valide : " << folder << std::endl;
            std::cout << "Usage : SuppressionRecursivePackage <dossier racine>\n";
            return 1;
		}
    }
    return 0;
}
