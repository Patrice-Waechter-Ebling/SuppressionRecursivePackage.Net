// SuppressionRecursivePackage.Net.cpp 
// © Patrice Waechter-Ebling 2025-2026
// Outil complementaire pour git

#include <windows.h>
#include <iostream>
#include <string>

int SupressionRecursive(const std::string& folder)
{
	std::string searchPath = folder + "\\*";
	WIN32_FIND_DATAA fd;
	HANDLE hFind = FindFirstFileA(searchPath.c_str(), &fd);
	if (hFind == INVALID_HANDLE_VALUE)return false;
	do
	{
		std::string name = fd.cFileName;
		if (name == "." || name == "..") continue;
		std::string fullPath = folder + "\\" + name;
		if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			SupressionRecursive(fullPath);
		}
		else
		{
			SetFileAttributesA(fullPath.c_str(), FILE_ATTRIBUTE_NORMAL);
			DeleteFileA(fullPath.c_str());
		}
	} while (FindNextFileA(hFind, &fd));
	FindClose(hFind);
	SetFileAttributesA(folder.c_str(), FILE_ATTRIBUTE_NORMAL);
	return RemoveDirectoryA(folder.c_str());
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
		if (name == "." || name == "..")    continue;
		std::string fullPath = root + "\\" + name;
		if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if (_stricmp(name.c_str(), "packages") == 0)
			{
				std::cout << "SuppressionRecursive:/Supp$ " << fullPath << std::endl;
				SupressionRecursive(fullPath);
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
	printf("SuppressionRecursive Package.Net\tv:1.00\t%c Patrice Waechter-Ebling 2025-2026\nOutil complementaire pour git\n", 0xB8);
	if (argc < 2)
	{
		std::cout << "SuppressionRecursive:/Err$ Dossier de travail non specifie.\n";
		return 0;
	}
	std::string root = argv[1];
	RechercherCible(root);
	std::cout << "Terminé.\n";
	return 0;
}
