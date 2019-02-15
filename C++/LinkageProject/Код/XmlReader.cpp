
#include "stdafx.h"

//-------------------------------------------------------------------
// ����� ��� ������ ������ xml
//-------------------------------------------------------------------

XmlReader::~XmlReader()
{
	if (pReader)
	{
		pReader->Release();
	}
	if (pFileStream)
	{
		pFileStream->Release();
	}

	pReader = NULL;
	pFileStream = NULL;
}

//-------------------------------------------------------------------

void XmlReader::Read_PrjFiles(StkPrjFile& prjFiles)
{
	LPCWSTR localName;     // ��� ����
	XmlNodeType nodeType;  // ��� ��������� ����
	LPCWSTR nodeValue;     // �������� ����

	// ������ ������ ��������
	while (!prjFiles.empty())
	{
		bool ok = true;
		path = prjFiles.top().path;

		selectProject = std::make_shared<Project>
			(prjFiles.top().name, prjFiles.top().path, prjFiles.top().typeGUID);

		prjFiles.pop();


		SHCreateStreamOnFile(path.c_str(), STGM_READ, &pFileStream);
		CreateXmlReader(__uuidof(IXmlReader), (void**) &pReader, NULL);
		pReader->SetProperty(XmlReaderProperty_DtdProcessing, DtdProcessing_Prohibit);
		pReader->SetInput(pFileStream);

		// ����� ������?
		if (!pFileStream)
		{
			Error(path, 3);
			continue;
		}

		// ������ �����
		while (!pReader->IsEOF())
		{
			Get_Next(localName, nodeType, nodeValue);

			// ������ ���� ��������
			if (nodeType == XmlNodeType_Element)
			{
				// ������ ������ ����������� (� ���� ��������)
				if (!_wcsicmp(localName, NOD_PRJREF))
				{
					do
					{
						Get_Next(localName, nodeType, nodeValue);

						if (!_wcsicmp(localName, NOD_PRJ) && nodeType == XmlNodeType_Element)
						{
							Get_Next(localName, nodeType, nodeValue);

							wstring bufval = nodeValue;

							std::transform(bufval.begin(), bufval.end(), bufval.begin(),
								[](wchar_t ch) { return wchar_t(toupper(ch)); });

							selectProject->prjReference.insert(bufval);
						}
					}
					while (!((!_wcsicmp(localName, NOD_PRJREF) && nodeType == XmlNodeType_EndElement)));
				}

				// ������ ������ c++ �������
				if (selectProject->GUIDtype == GUIDVSXPROJ)
				{
					// ����� � ���������� ������
					if (!_wcsicmp(localName, NOD_PRJCONF))
					{
						wstring bufval;
						Read_Attributes(ATT_INGL, bufval);
						selectProject->Add_Conf(bufval, L"", 0, netCont->config->excluds.confName);
					}

					// ����������� ���. ����� ��� ������
					if (!_wcsicmp(localName, NOD_PRPGROUP))
					{
						wstring bufval;

						if (Read_Values(NOD_CONFTYPE, bufval))
						{
							if (bufconf != NULL)
							{
								bufconf->�onfigurationType = bufval;
							}
						}
					}

					// ����������� ������������
					if (!_wcsicmp(localName, NOD_ITDEFGROUP))
					{
						wstring bufval;

						if (Read_Values(NOD_ADDITDEP, bufval))
						{
							if (bufconf != NULL)
							{
								std::transform(bufval.begin(), bufval.end(), bufval.begin(),
									[](wchar_t ch) { return wchar_t(tolower(ch)); });

								bufconf->additionalDependencies = bufval;
							}
						}

						else continue;
					}
				}

				else
				{
					// ������ GUID ������� � C# ��������
					if (!_wcsicmp(localName, NOD_GUID))
					{
						Get_Next(localName, nodeType, nodeValue);
						selectProject->GUIDprj = nodeValue;
					}
				}
			}
		}

		// �������� �� ������� ������
		if (selectProject->configs.size() < 1 && selectProject->GUIDtype == GUIDVSXPROJ)
		{
			ok = false;
			Error(selectProject->prjPath, 15);
		}

		// �������� �� ������� guid �������
		if (selectProject->GUIDprj == L"")
		{
			ok = false;
			Error(selectProject->prjPath, 16);
		}

		// ���������� ������� � ����
		if (ok)
		{
			Filling_Project();
		}

		//completePath.insert(path);
	}
}

bool XmlReader::Read_Config()
{
	LPCWSTR     localName; // ��� ����
	XmlNodeType nodeType;  // ��� ��������� ����
	LPCWSTR     nodeValue; // �������� ����

	path = netCont->config->confPath;

	SHCreateStreamOnFile(path.c_str(), STGM_READ, &pFileStream);
	CreateXmlReader(__uuidof(IXmlReader), (void**)&pReader, NULL);
	pReader->SetProperty(XmlReaderProperty_DtdProcessing, DtdProcessing_Prohibit);
	pReader->SetInput(pFileStream);

	if (!pFileStream)
	{
		Error(path,3);
		return false;
	}

	// ������ �����
	while (!pReader->IsEOF())
	{
		Get_Next(localName, nodeType, nodeValue);

		if (nodeType == XmlNodeType_Element)
		{
			// ���� Verbosity
			if (!_wcsicmp(localName, NOD_VB))
			{
				Get_Next(localName, nodeType, nodeValue);
				netCont->config->verb = nodeValue[0] == '1' ? true : false;
			}

			// ���� Output
			if (!_wcsicmp(localName, NOD_OUP))
			{
				Get_Next(localName, nodeType, nodeValue);
				netCont->config->outPath = nodeValue;
			}

			try
			{
				// ���� Dependencies
				if (!_wcsicmp(localName, NOD_DEP))
				{
					wstring bufval;
					Read_Attributes(ATT_TP, bufval);
					Get_Next(localName, nodeType, nodeValue);

					// ���������� �����������
					if (bufval == DEP_ATT_GL)  netCont->config->depends.global.insert(nodeValue);
					else if (bufval == DEP_ATT_LP)
					{
						wstring dep = nodeValue;

						size_t poz = dep.find(L'>', 0);
						if (poz == wstring::npos) throw (22);

						wstring firstPrj = dep.substr(0, poz - 1);
						wstring secondPrj = dep.substr(poz + 2);

						netCont->config->depends.localPrj[firstPrj].insert(secondPrj);
					}
					else if (bufval == DEP_ATT_LC)
					{
						wstring dep = nodeValue;

						size_t poz = dep.find(L'>', 0);
						if (poz == wstring::npos) throw (22);

						wstring firstPrj = dep.substr(0, poz - 1);
						wstring secondPrj = dep.substr(poz + 2);

						poz = secondPrj.find(L'.', 0);
						if (poz == wstring::npos) throw (22);

						wstring secondConf = secondPrj.substr(poz + 1);
						secondPrj = secondPrj.substr(0, poz);

						Depend secondDepend;
						secondDepend.prj = secondPrj;
						secondDepend.conf = secondConf;

						netCont->config->depends.localConf[firstPrj].push_back(secondDepend);
					}
				}
			}

			catch (int error)
			{
				Error(nodeValue, error);
				continue;
			}

			// ���� Exclusions->Exclusion
			if (!_wcsicmp(localName, NOD_EXC))
			{
				wstring bufval;
				Read_Attributes(ATT_TP, bufval);
				Get_Next(localName, nodeType, nodeValue);
				
				// ���������� ����������
					 if (bufval == EXC_TPGD)  netCont->config->excluds.typeGUID.insert(nodeValue);
				else if (bufval == EXC_PRJGD) netCont->config->excluds.prjGUID.insert(nodeValue);
				else if (bufval == EXC_PRJPH) netCont->config->excluds.prjPath.insert(nodeValue);
				else if (bufval == EXC_PRJNM) netCont->config->excluds.prjName.insert(nodeValue);
				else if (bufval == EXC_�FNM)
				{
					wstring buf(nodeValue);

					size_t poz = buf.find(L'*');
					if (poz != wstring::npos)
					{
						size_t sig = buf.find(L'|');
						if (poz != wstring::npos)
						{
							if (poz < sig) netCont->config->excluds.confName.cap.insert(buf.substr(sig+1));
							else  netCont->config->excluds.confName.type.insert(buf.substr(0,sig));
						}

						// ����� ������������� ������ ��� �� ��������� ������ ������ (��� '|')
					}

					else netCont->config->excluds.confName.typeCap.insert(nodeValue);
				}
			}

			// ���� Search->Path
			if (!_wcsicmp(localName, NOD_PH))
			{
				Get_Next(localName, nodeType, nodeValue);
				netCont->config->searchPaths.insert(nodeValue);
			}

			// ���� Inputs->sln
			if (!_wcsicmp(localName, L"Sln"))
			{
				wstring bufval;
				Read_Attributes(ATT_PH, bufval);

				SharSln selectFile = std::make_shared<SlnFile>();
				selectFile->path = bufval;

				map<wstring, int> minConf; // ����������� ����� ������ (��� �������) � �� �����

				// ���������� ������ � �� ����������
				while (!pReader->IsEOF())
				{
					Get_Next(localName, nodeType, nodeValue);

					if (!_wcsicmp(localName, NOD_CF))
					{
						Get_Next(localName, nodeType, nodeValue);

						wstring minName = Minimaz_ConfName(nodeValue);

						if (minName == L"") continue;

						minConf[minName] = minConf[minName]++;
						selectFile->targetConf[nodeValue] = minConf[minName];
						selectFile->allConf.insert(nodeValue);
					}

					if (!_wcsicmp(localName, L"Sln")) break;
				}

				netCont->config->slnFiles.push_back(selectFile);
			}
		}
	}

	return Test_Arg();
}

//-------------------------------------------------------------------

void XmlReader::Get_Next(LPCWSTR& localName, XmlNodeType& nodeType, LPCWSTR& nodeValue)
{
	HRESULT hr = pReader->Read(&nodeType);

	// �������� �� ����� ��������
	if (hr == E_PENDING)
	{
		Error(path, 13);

		// ������� �������� ��������� ���� 
		while (hr == E_PENDING)
		{
			::Sleep(1000);
			hr = pReader->Read(&nodeType);
		}

		if (hr != S_OK) throw (25);
	}

	// �������� �� ����� �����
	if (hr != S_OK && !pReader->IsEOF())
	{
		Error(path, 14);
		throw (-1);
	}

	hr = pReader->GetLocalName(&localName, NULL);
	hr = pReader->GetValue(&nodeValue, NULL);

	if (hr != S_OK)
	{
		Error(path, 26);
		throw (-1);
	}
}

void XmlReader::Read_Attributes(LPCWSTR wanted, wstring& bufval)
{
	LPCWSTR localName; // ��� ����
	LPCWSTR nodeValue;     // �������� ����
	HRESULT hr = pReader->MoveToFirstAttribute();
	
	if (S_FALSE == hr) return;

	try
	{
		if (S_OK != hr) throw (17);

		else
		{
			while (TRUE)
			{
				if (!pReader->IsDefault())
				{
					if (FAILED(hr = pReader->GetLocalName(&localName, NULL))) throw (18);

					if (FAILED(hr = pReader->GetValue(&nodeValue, NULL))) throw (19);

					if (!_wcsicmp(localName, wanted))
					{
						// ����������� ����� ������, ���� ������� 'Condition'
						if (!_wcsicmp(wanted, ATT_COND))
						{
							wchar_t separator[] = SEPCONF;
							LPWCH bufValConf = NULL;
							LPWCH valConf = (LPWCH)nodeValue;
							LPWCH token = wcstok_s(valConf, separator, &bufValConf); // ���������� ������ �� ������

							token = wcstok_s(0, separator, &bufValConf); // ������� �� ��������� �����
							bufval = token;

							break;
						}

						// ������� ��������� ��������
						bufval = nodeValue;
					}
				}

				if (S_OK != pReader->MoveToNextAttribute()) break;
			}
		}
	}

	catch (int error)
	{
		Error(path, error);
	}
}

bool XmlReader::Read_Values(LPCWSTR node, wstring& bufval)
{
	LPCWSTR localName;    // ��� ����
	XmlNodeType nodeType; // ��� ��������� ����
	LPCWSTR nodeValue;    // �������� ����

	Read_Attributes(ATT_COND, bufval); // ����������� ����� ������
	bufconf = NULL;

	// ���� � bufval �� �����, ������ ����� ��� ������, ����� ������ �� label
	if (bufval != L"")
	{
		for (auto &conf : selectProject->configs)
		{
			if (conf->confName == bufval)
			{
				bufconf = conf;
				break;
			}
		}
	}
	
	// ��� �������� 'Condition', ������� �� �������� Label
	else 
	{
		Read_Attributes(ATT_LAB, bufval);

		// ��������������� �������� ����, ���� Label = Globals
		if (!_wcsicmp(bufval.c_str(), NOD_ATT_GLOB))
		{
			node = NOD_GUID;
		}

		else
		{
			return false;
		}
	}

	// ������ ������
	do
	{
		Get_Next(localName, nodeType, nodeValue);

		// ����������� ��������� ����� ��� GUID �������
		if (!_wcsicmp(localName, node))
		{
			Get_Next(localName, nodeType, nodeValue);

			// GUID �������
			if (!_wcsicmp(node, NOD_GUID))
			{
				selectProject->GUIDprj = nodeValue;
				return false;
			}

			// �������� ����
			bufval = nodeValue;
			return true;
		}

		// �������� ��� ������
		if (!_wcsicmp(localName, NOD_TARGNAME))
		{
			Get_Next(localName, nodeType, nodeValue);

			if (bufconf == NULL)
			{
				return false;
			}

			wchar_t separator[] = SEPTOKEN;
			LPWCH bufValConf;
			LPWCH valConf = (LPWCH) nodeValue;
			LPWCH token = wcstok_s(valConf, separator, &bufValConf);

			// ������ TargetName
			while (token)
			{
				if (!_wcsicmp(token, L"ProjectName"))
				{
					bufconf->targetName += selectProject->prjName;
				}

				else if (!_wcsicmp(token, L"TargetName.Replace"))
				{					
					token = wcstok_s (0, separator, &bufValConf);
					wstring arg1(token);
					token = wcstok_s (0, separator, &bufValConf);
					wstring arg2 (token);

					wstring replaceName = selectProject->prjName;

					for (auto wc = replaceName.begin(); wc != replaceName.end(); wc++)
					{
						if (*wc == *arg1.begin())
						{
							replaceName.replace(wc, wc + 1,arg2);
						}
					}

					bufconf->targetName += replaceName;
				}

				else
				{
					bufconf->targetName += token;
				}

				token = wcstok_s(0, separator, &bufValConf);
			}

			return false;
		}

	}
	while (!((!_wcsicmp(localName, NOD_PRPGROUP) && nodeType == XmlNodeType_EndElement)
		  || (!_wcsicmp(localName, NOD_ITDEFGROUP) && nodeType == XmlNodeType_EndElement)));

	return false;
}

//-------------------------------------------------------------------

wstring XmlReader::Minimaz_ConfName(wstring name)
{
	wstring type(L"");

	size_t poz;
	poz = name.find(L"Debug");
	if (poz != std::string::npos)
	{
		type += L"Debug|";
	}

	poz = name.find(L"Release");
	if (poz != std::string::npos)
	{
		type += L"Release|";
	}

	if (type == L"") return L"";

	wstring cap;
	poz = name.find(L"|") + 1;
	if (poz != std::string::npos)
	{
		if (name.substr(poz) != L"x64") cap = L"Win32";
		else cap = L"x64";
	}

	return type + cap;
}

bool XmlReader::Test_Arg()
{
	if (netCont->config->searchPaths.empty() && netCont->config->slnFiles.empty())
	{
		Error(L"", 4);
		return false;
	}

	// �������� ���� ������
	if (netCont->config->outPath == L"")
	{
		Error(L"", 5);
		return false;
	}

	// �������������� �����
	if (netCont->config->verb)
	{
		DISPOUT << L"### ���� ������������ - " << path << DISPEND;
		DISPOUT << L"### ���� ������ - " << netCont->config->outPath << DISPEND;
		
		if (!netCont->config->slnFiles.empty())
		{
			DISPOUT << L"\n### ����� sln:" << DISPEND;

			for (auto &File : netCont->config->slnFiles)
			{
				DISPOUT << L" -- " << File->path << L"\n";

				if (File->targetConf.empty())
				{
					DISPOUT << L"  - ��� ������" << L"\n";
				}

				else
				{
					for (auto &conf : File->targetConf)
					{
						DISPOUT << L"  - "<< L"["<< conf.second << L"] " << conf.first  << L"\n" ;
					}
				}

				DISPOUT << DISPEND;
			}
		}

		if (!netCont->config->searchPaths.empty())
		{
			DISPOUT << L"### ���������� ��� ������ ��������:" << DISPEND;
			
			for (auto &sPath : netCont->config->searchPaths)
			{
				if (sPath == L"")
				{
					DISPOUT << L" -- �� ������" << sPath << DISPEND;
				}
				
				else
				{
					DISPOUT << L" -- " << sPath << DISPEND;
				}
			}
		}
	}

	return true;
}

//-------------------------------------------------------------------

void XmlReader::Filling_Project()
{
	// �������� ������������ ����, ����� � guid
	int p = netCont->prjPaths.count(selectProject->prjPath),
		n = netCont->prjNames.count(selectProject->prjName),
		g = netCont->prjGUIDs.count(selectProject->GUIDprj);

	if (p || n || g)
	{
		if (netCont->prjPaths[selectProject->prjPath] == netCont->prjNames[selectProject->prjName] &&
			netCont->prjGUIDs[selectProject->GUIDprj] == netCont->prjNames[selectProject->prjName])
		{
			//
			// ������ ��� �������� ����� sln
			// ���������� ��� ��������� ������� �� vcxprj
			//

			SharPrj findProject = netCont->prjPaths[selectProject->prjPath];

			// ���������� ������������ �� ������ ProjectReference
			for (auto& posDep : selectProject->prjReference)
			{
				findProject->prjReference.insert(posDep);
			}

			// ���������� ������
			for (auto& confFindPrj : findProject->configs)
			{
				for (auto& confSelPrj : selectProject->configs)
				{
					if (confFindPrj->confName == confSelPrj->confName)
					{
						// ������� ������������ ����������
						confFindPrj->�onfigurationType = confSelPrj->�onfigurationType;
						confFindPrj->additionalDependencies = confSelPrj->additionalDependencies;
						confFindPrj->targetName = confSelPrj->targetName;
					}
				}
			}

			return;
		}

		Error(path, selectProject, p, n, g);

		return;
	}

	// ����� ������� ������ � ��� ���������� �� �����, ���� � guid
	netCont->projects.insert(netCont->projects.end(), selectProject);
	netCont->prjGUIDs[selectProject->GUIDprj] = selectProject;
	netCont->prjPaths[selectProject->prjPath] = selectProject;
	netCont->prjNames[selectProject->prjName] = selectProject;
}
