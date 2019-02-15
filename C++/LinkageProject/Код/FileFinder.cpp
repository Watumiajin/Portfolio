
#include "stdafx.h"

//-------------------------------------------------------------------
// ����� ���������� ��������� ������ ������
//-------------------------------------------------------------------

void FileFinder::Find_Files(LPCWSTR root, Excluds& exc)
{
	wstring path = wstring(root) + L"\\*.*";

	WIN32_FIND_DATA fd; // ��������� ����

	HANDLE hFind = ::FindFirstFile(path.c_str(), &fd); // ����� �����
	if (hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (!_wcsicmp(fd.cFileName, L".") || !_wcsicmp(fd.cFileName, L".."))
			{
				continue;
			}

			if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) // ����������
			{
				// ���� � ��������
				path = wstring(root) + L"\\" + fd.cFileName;
				Find_Files(path.c_str(), exc);
			}

			else // ����
			{


				size_t pos = Cut_Ext(fd.cFileName); // �������

				if (pos)
				{
					wstring name = wstring(fd.cFileName).substr(0, pos);
					
					wstring find_path = wstring(root) + L"\\" + fd.cFileName; // ���� � �����
					
					// �������� �� ����������
					if (exc.prjPath.count(find_path)) continue;
					if (exc.prjName.count(name))      continue;
					
					prjFiles.push(PrjFile(find_path, name, GUIDVSXPROJ));
					exc.prjPath.insert(find_path); // ����� �� ������ ������ ���� ������
				}
			}

		} while (::FindNextFile(hFind, &fd)); // ����� ���������� �����/�����

		::FindClose(hFind);
	}
}

size_t FileFinder::Cut_Ext(const wstring st)
{
	size_t pos = st.rfind ('.');
	if (st.substr(pos + 1, wstring::npos) == L"vcxproj") return pos;
	return 0;
}
