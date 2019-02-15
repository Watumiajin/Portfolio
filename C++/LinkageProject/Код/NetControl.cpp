
#include "stdafx.h"

//-------------------------------------------------------------------
// ����� ���������� � ������ � ����� (����������)
//-------------------------------------------------------------------

NetControl::NetControl(Config& Configs)
{
	net[0].cap = L"Win32";
	net[0].type = L"Debug";
	net[0].Release = 0;
	net[0].Win32 = 1;

	net[1].cap = L"x64";
	net[1].type = L"Debug";
	net[1].Release = 0;
	net[1].Win32 = 0;

	net[2].cap = L"Win32";
	net[2].type = L"Release";
	net[2].Release = 1;
	net[2].Win32 = 1;

	net[3].cap = L"x64";
	net[3].type = L"Release";
	net[3].Release = 1;
	net[3].Win32 = 0;

	maxSteps = 0;

	config = &Configs;
}

//-------------------------------------------------------------------

void NetControl::Sorting()
{
	Create_Vertexs();

	Scan_Depends();

	Separation_Subnet();

	for (int ind = 0; ind < NETCOUNT; ind++)
	{
		Create_Dependes(ind);

		Filling_Strong_Depences(ind);
		Filling_Weak_Depences(ind);
		
		Net_Sorting(ind);
		Net_Check(ind);
		
		TO_CHK
		// Net_Display(ind);

		if (net[ind].steps > maxSteps) maxSteps = net[ind].steps;
	}

	CSproj_Add_Build();

	DISPOUT << DISPEND;
}

//-------------------------------------------------------------------

// �������� � ���������� ������ � ����� ����
void NetControl::Create_Vertexs()
{
	for (auto& prj : projects)
	{
		for (auto& conf : prj->configs)
		{
			wstring name(prj->prjName + L"." + conf->confName);

			vertexs.insert(std::make_shared<Vertex>(prj, conf, name));
		}
	}
}

// ������ ������������
void NetControl::Scan_Depends()
{
	const wchar_t separator[] = SEPDEP;
	LPWCH bufValConf;
	
	// ���������� ��������� ������������� ����� ��������� ��� �������� ������� ���������� �� �� sln
	for (auto& prj : projects)
	{
		for (auto& depend : prj->prjReference)
		{
			if (prjGUIDs.count(depend))
			{
				prj->allDepends.insert(std::make_pair(depend, prjGUIDs[depend]));
			}
		}
	}

	// ���������� ��������� ������������� ����� ���������
	for (auto& prj : config->depends.localPrj)
	{
		SharPrj firstPrj = prjGUIDs[prj.first];

		for (auto& dep : prj.second)
		{
			if (prj.first != dep)
			{
				SharPrj findPrj = prjGUIDs[dep];

				firstPrj->allDepends[findPrj->GUIDprj] = findPrj;
			}
		}
	}

	// ������� ��������
	for (auto& prj : projects)
	{
		// ���������� ���������� ������������
		for (auto &dep : config->depends.global)
		{
			if (prj->GUIDprj != dep)
			{
				SharPrj findPrj = prjGUIDs[dep];

				prj->allDepends[findPrj->GUIDprj] = findPrj;
			}
		}

		// ������� ������
		for (auto& conf : prj->configs)
		{
			// ������ ������ ������������
			if (conf->additionalDependencies != L"")
			{
				LPWCH valConf = LPWCH(conf->additionalDependencies.c_str());
				LPWCH token = wcstok_s(valConf, separator, &bufValConf);

				while (token)
				{
					wstring buf = Trim(token);

					if (buf != L"%(additionaldependencies)")
					{
						conf->strongDepends.insert(buf);
					}

					token = wcstok_s(0, separator, &bufValConf);
				}
			}

			// ���������� ��������� ����������� ����� �������� ��������
			for (auto& depend : config->depends.localConf[prj->GUIDprj + L"." + conf->confName])
			{
				for (auto& findConf : prjGUIDs[depend.prj]->configs)
				{
					if (depend.conf == findConf->confName)
					{
						conf->weakDepends.insert(prjGUIDs[depend.prj]->prjName + L"." + findConf->confName);
					}
				}
			}
		}
	}
}

// ���������� �� �������
void NetControl::Separation_Subnet()
{
	for (auto &vertex : vertexs)
	{
		int ind = 0; // ������ ����

		// ����������� ����
		for (int i = 0; i < NETCOUNT; i++) 
		{
			if (vertex->conf->release == net[i].Release &&
				vertex->conf->win32   == net[i].Win32)
			{
				ind = i;
				net[i].�onf_count++;
			}
		}

		SharVertex newVertex = std::make_shared<Vertex>(vertex->prj, vertex->conf, vertex->name);

		net[ind].vertexs_unsorted.insert(newVertex);
		net[ind].projects[vertex->prj->prjName].insert (newVertex);
	}
}

//-------------------------------------------------------------------

// �������� �������� ������ � ����
void NetControl::Create_Dependes(int ind)
{
	for (auto& vertex : net[ind].vertexs_unsorted)
	{
		// ���������� ������� ����������
		// #############################

		// ����������� ����� ������� �������
		wstring name = vertex->conf->targetName == L"" ? vertex->prj->prjName : vertex->conf->targetName;
		std::transform(name.begin(), name.end(), name.begin(), [](wchar_t ch) { return wchar_t(tolower(ch)); });

		// ����� ������� ���� � ����?
		if (!net[ind].strongDepends.count(name + L".lib"))
		{
			net[ind].strongDepends.insert(std::make_pair(name + L".lib", vertex));
		}

		else
		{
			std::shared_ptr<Vertex> findVertex = net[ind].strongDepends[name + L".lib"];

			// ������ ��� ��?
			if (vertex->prj == findVertex->prj)
			{
				if (vertex->name > findVertex->name)
				{
					net[ind].strongDepends[name + L".lib"] = vertex;
				}
			}

			else
			{
				// ��� ������: 21
				Error(name + L" (" + vertex->conf->confName + L")",
					vertex->prj->prjPath,
					findVertex->prj->prjPath);
			}
		}

		// ���������� ������ ����������
		// #############################

		net[ind].weakDepends.insert(std::make_pair(vertex->prj->prjName + L"." + vertex->conf->confName, vertex));
	}

	//net[ind].Twin_Optimization();
}

// ���������� ������� ������
void NetControl::Filling_Strong_Depences(int ind)
{
	for (auto& vertex : net[ind].vertexs_unsorted)
	{
		// ���������� ������� ������ � ���� �������
		for (auto& depend : vertex->conf->strongDepends)
		{
			if (net[ind].strongDepends.count(depend))
			{
				bool addEdge = vertex->AddDepend(vertex, net[ind].strongDepends[depend]);
				if (addEdge)
				{
					vertex->addedStrongDepend.insert(net[ind].strongDepends[depend]->prj);
					net[ind].edge_count++;

					TO_CHK
					net[ind].strongDepends[depend]->strongDepend.insert(vertex);
				}
			}
		}
	}

	// depend == targetName + "lib" 
}

// ���������� ������ ������
void NetControl::Filling_Weak_Depences(int ind)
{
	for (auto& vertex : net[ind].vertexs_unsorted)
	{
		// ���������� ������ ������ � ���� �������
		for (auto& depend : vertex->conf->weakDepends)
		{
			if (net[ind].weakDepends.count(depend))
			{
				if (vertex->prj->GUIDtype == GUIDCSPROJ ||
					!vertex->addedStrongDepend.count(net[ind].weakDepends[depend]->prj))
				{
					bool addEdge = vertex->AddDepend(vertex, net[ind].weakDepends[depend]);
					if (addEdge)
					{
						net[ind].edge_count++;

						TO_CHK
						net[ind].weakDepends[depend]->weakDepend.insert(vertex);
					}
				}
			}
		}
	}

	// depend == prjName + L"." + confName
}

// ���������� ����
void NetControl::Net_Sorting(int ind)
{
	Report report(net[ind].projects); // �������� �� �������� ���������

	// ���������� � ������������
	DISPOUT << L"\n### ������������ - " << net[ind].type + L"|" + net[ind].cap;

	// �������������� ����������
	for (int numSteps = 1;; numSteps++)
	{
		DISPOUT << L"\n -- " << numSteps << L" ����:";
		
		Net_Optimization(ind);

		// ������� ������
		for (auto vertex : net[ind].vertexs_sorted)
		{	
			// ������� �� �������� � ��� ��������?
			if (!vertex->pass && vertex->incoming == 0)
			{
				Release_vertex(vertex, numSteps, report);
			}
		}

		if (report.stepVertexs == 0) break;

		// ������� �� ��������� ���
		net[ind].steps = numSteps;

		passStepPrj.clear();
		report.Reset_Step(config->verb);
	}

	// ����������� ����� �� ����������
	DISPOUT << L"\n  - ����� ������ ��������: " << report.allVertexs << L" �� " << net[ind].vertexs_unsorted.size()
			<< L"\n  - ����� ����  ��������: " << report.allEdges << L" �� " << net[ind].edge_count
			<< L"\n  - ����� �������� ��������: " << report.allPrjs << L" �� " << net[ind].projects.size()
			<< DISPEND;
}

// ����������� ����
void NetControl::Net_Optimization(int ind)
{
	net[ind].vertexs_sorted.clear();

	// ����� �������
	for (auto& vertex : net[ind].vertexs_unsorted)
	{
		vertex->depth = -1;
	}

	// ����������� �������
	for (auto vertex : net[ind].vertexs_unsorted)
	{
		if (!vertex->pass && vertex->depth == -1)
		{
			Immersion(vertex);
		}
	}

	/*
	// ��������� ������� ������ ��� ������� ������ (�� ������ � ������� ��������)
	for (auto &vertex : net[ind].strongDepends)
	{
		if (!vertex.second->pass)
		{
			for (auto &twinVertex : vertex.second->twinVertex)
			{
				if (vertex.second->prj->guidPrj == L"{426A12CD-1744-43C4-AA67-023442A0CEC6}")
				{
					int Test = 1;
				}

				if (vertex.second->depth != twinVertex->depth)
				{
					if (vertex.second->depth < twinVertex->depth)
					{
						vertex.second->conf = twinVertex->conf;
					}
				}

				else if (vertex.second->outgoing != twinVertex->outgoing)
				{
					if (vertex.second->outgoing < twinVertex->outgoing)
					{
						vertex.second->conf = twinVertex->conf;
					}
				}
			}
		}
	
		
	}
	*/

	// ���������� ������������ � ������ �� ������� � ����� ������������
	for (auto& vertex : net[ind].vertexs_unsorted)
	{
		if (!vertex->pass)
		{
			vertex->outEdge_sorted.clear();

			for (auto& edge : vertex->outEdge_unsorted)
			{
				if (!edge->pass)
				{
					vertex->outEdge_sorted.insert(edge);
				}
			}

			net[ind].vertexs_sorted.insert(vertex);
		}
	}
}

// ����������� �������� ����
void NetControl::Net_Check(int ind)
{
	map <wstring, std::shared_ptr<Vertex> > openVertex; // �� �������� �������

	// ���� ���������� ��������� �� ������ ��� �������, ������ ���-��, ��������� �� �������

	for (auto& vertex : net[ind].vertexs_unsorted)
	{
		if (!vertex->pass) openVertex[vertex->name] = vertex;
	}

	if (openVertex.empty()) return;

	DISPOUT << L"\n\n *** ��������� ������� �� �������:";

	for (auto& vertex : openVertex)
	{
		DISPOUT << L"\n   - " << vertex.second->name; // ������� �� ������� - 
	}
}

//-------------------------------------------------------------------

// ����������� ����� ������������� ������
void NetControl::Release_vertex(SharVertex& vertex, int numSteps, Report& report)
{
	// ������ �� ���� ���� ��������?
	if (!passStepPrj.count(vertex->prj->prjName))
	{
		vertex->pass = true;
		vertex->conf->stepsBuild.insert(numSteps);
		passStepPrj.insert(vertex->prj->prjName);
		report.Add_Vertex(vertex->prj);

		if (config->verb)
		{
			DISPOUT << L"\n    " << vertex->prj->prjName << L": " << vertex->conf->confName;
		}

		for (auto edge : vertex->outEdge_sorted)
		{
			edge->incoming--;
			edge->inEdge.erase(vertex); TO_CHK
			report.stepEdges++;

			if (!edge->pass && edge->incoming == 0)
			{
				// ���� � ��������
				Release_vertex(edge, numSteps, report);
			}
		}
	}
}

// ����������� ����� ���������� �� �������
int NetControl::Immersion(SharVertex& vertex)
{
	int maxDepth = 0;

	for (auto edge : vertex->outEdge_unsorted)
	{
		if (!edge->pass)
		{
			// ���� � ��������
			int depth = edge->depth == -1 ? Immersion(edge) : depth = edge->depth;

			if (maxDepth < depth) maxDepth = depth;
		}
	}

	vertex->depth = maxDepth + 1;
	return vertex->depth;
}

//-------------------------------------------------------------------

// ���������� ����� ������ ��� ������������ C# ��������
void NetControl::CSproj_Add_Build()
{
	for (auto& prj : projects)
	{
		if (prj->GUIDtype != GUIDCSPROJ) continue;
			
		for (auto& dep : prj->allDepends)
		{
			for (auto& conf : prj->configs)
			{
				bool exists = false;

				for (auto& depConf : dep.second->configs)
				{
					// �������� ������ �� ����������� ���������� �� ���� ����?
					if ((depConf->release == conf->release  &&
						 depConf->win32   == conf->win32)   &&
						 depConf->stepsBuild.count (*conf->stepsBuild.begin()))
					{
						exists = true;
					}
				}

				if (exists) continue;

				for (auto& depConf : dep.second->configs)
				{
					if (( depConf->release == conf->release &&
						  depConf->win32   == conf->win32))
					{
						// ��������� �������������� ��� ��� ������ ������ ���������� �������� ������
						depConf->stepsBuild.insert(*conf->stepsBuild.begin());
						break;
					}
				}
			}
		}
	}
}

// ������ �� �������� (����� � ������)
wstring NetControl::Trim(const wstring& a_str)
{
	wstring::size_type _from = a_str.find_first_not_of(L" \t"),
		from = (_from == wstring::npos ? 0 : _from);
	wstring::size_type to = a_str.find_last_not_of(L" \t");
	return a_str.substr(from, to == wstring::npos ? wstring::npos : to - from + 1);
}

//-------------------------------------------------------------------

// ����� ������
void NetControl::Net_Display(int ind)
{
	// ��������������� ������� ���������
	struct vert_less
	{
		bool operator() (const std::shared_ptr<Vertex>& first, const std::shared_ptr<Vertex>& second)
		{

			if (first->outgoing != second->outgoing)
			{
				return first->outgoing > second->outgoing; // first->name < second->name;
			}

			else
			{
				return first < second;
			}
		}
	};

	std::set<std::shared_ptr<Vertex>, vert_less> vertDisp;

	for (auto& vertex : net[ind].vertexs_unsorted)
	{
		vertDisp.insert(vertex);
	}

	for (auto& vertex : vertDisp)
	{
		DISPOUT << L"\n\n ### " << vertex->name;
		DISPOUT << L"\n  -- �������� - " << vertex->inEdge.size() << L":\n";

		for (auto& edge : vertex->inEdge)
		{
			DISPOUT << L"\n   - " << edge->name;
		}

		DISPOUT << L"\n\n  -- ��������� ����� - " << vertex->outEdge_sorted.size() << L":\n";
		DISPOUT << L"\n  -- ������� - " << vertex->strongDepend.size() << L":\n";

		for (auto& edge : vertex->strongDepend)
		{
			DISPOUT << L"\n   - " << edge->name;
		}

		DISPOUT << L"\n\n  -- ������ - " << vertex->weakDepend.size() << L":\n";

		for (auto& edge : vertex->weakDepend)
		{
			DISPOUT << L"\n   - " << edge->name;
		}
	}
}
