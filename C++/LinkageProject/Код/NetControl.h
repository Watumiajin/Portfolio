
#pragma once
#include "stdafx.h"

//-------------------------------------------------------------------
// C�������� ������� �����
//-------------------------------------------------------------------
struct Vertex
{
public:
	Vertex(SharPrj Prj, SharPrjConf Conf, wstring Name) :
		pass(false),
		incoming(0),
		outgoing(0),
		depth(-1),
		prj(Prj),
		conf(Conf),
		name(Name) {}
	
	// ���������� �������� ����� (�� ������� �������)
	bool AddDepend(SharVertex first, SharVertex second)
	{
		// ������ �� ����������� ������
		if (!second->outEdge_unsorted.count(first))
		{
			incoming++;
			second->outgoing++;
			second->outEdge_unsorted.insert(first);
			
			// ����� � ������ ��������?
			if (first->prj != second->prj)
			{
				prj->allDepends[second->prj->GUIDprj] = second->prj;
			}

			inEdge.insert(second); TO_CHK

			return true;
		}

		return false;
	}

public:
	// �������� ��������� �������
	wstring		name; // ��� �������
	SharPrj     prj;  // ������ �� ������
	SharPrjConf conf; // ������ �� ������

	// ��������� ��� ���������� ����
	bool pass;     // ������� �������� ��������
	int  incoming; // ����� ������ �� ������� �������
	int  outgoing; // ����� ��������� ������ �� ����
	int  depth;    // ������� ������������ �������

	//-------------------------------------------------------------------
	// ��������������� ��� �������� ��������� ����� ������������� �������
	//-------------------------------------------------------------------
	struct vert_less
	{
		bool operator() (const SharVertex& first, const SharVertex& second)
		{
			if (first->depth != second->depth) {
				return  first->depth > second->depth;
			}

			else if (first->outgoing != second->outgoing) {
				return  first->outgoing > second->outgoing;
			}

			else if (first->incoming != second->incoming) {
				return  first->incoming < second->incoming;
			}

			else if (first->name != second->name) {
				return  first->name > second->name;
			}

			else {
				return  first > second;
			}
		}
	};
	typedef std::set<SharVertex, vert_less>	SSharVertexSort;

	// ------------------------ ����� � ��������� -----------------------
	// ##################################################################

	SSharPrj addedStrongDepend; // ������� ����������� �� ������� ������

	// ��������� ����� (��������� �� ����)
	SSharVertex outEdge_unsorted; // ��������� ����� (�� ���������������)
	SSharVertexSort outEdge_sorted; // ��������� ����� (���������������)

	TO_CHK

	// �������� ����� (�� ������� �������)
	SSharVertex inEdge; // �������� �����

	// ��������� ����� (��������� �� ����)
	SSharVertex strongDepend; // ������� ����� (���������)
	SSharVertex weakDepend;   // ������ �����  (���������)
};


//-------------------------------------------------------------------
// C�������� ����
//-------------------------------------------------------------------
struct VertexNet
{
	VertexNet() : edge_count(0), �onf_count(0), steps(0) {};

	// ��������� ����
	int edge_count; // ����� ����
	int	�onf_count; // ����� ������ 
	int steps;      // ����� ������

	wstring cap;      // �����������
	wstring type;     // ��� ������������ ������
	size_t Release : 1, Win32 : 1; // ��������� � ����������� ����

	// �������
	SSharVertex vertexs_unsorted; // �� ���������������
	Vertex::SSharVertexSort vertexs_sorted; // ��������������� �������

	// �����
	map<wstring, SharVertex> strongDepends; // targetName <-> ������ �� �������
	map<wstring, SharVertex> weakDepends;   // prjName + L"." + confName <-> ������ �� �������
	map<wstring, SSharVertex> projects;     // �������� ������� � ��� �������� � ���� ���� �� �����
};


//-------------------------------------------------------------------
// C�������� ������
//-------------------------------------------------------------------
struct Report
{
public:
	Report(map<wstring, SSharVertex>& Projects) :
		projects(Projects),
		stepVertexs(0),
		stepEdges(0),
		stepPrjs(0),
		allVertexs(0),
		allEdges(0),
		allPrjs(0) {}

	// ����� ����
	void Reset_Step(bool verb)
	{
		if (verb)
		{
			DISPOUT << L"\n -- " 
					<< L"������: " << stepVertexs
					<< L"  ����: " << stepEdges
					<< L"  ��������: " << stepPrjs;
		}
		else
		{
			DISPOUT << L"\t������: " << stepVertexs
					<< L"\t����: " << stepEdges
					<< L"\t��������: " << stepPrjs;
		}



		allVertexs += stepVertexs;
		stepVertexs = 0;

		allEdges += stepEdges;
		stepEdges = 0;

		allPrjs += stepPrjs;
		stepPrjs = 0;
	}

	// �������� ������� � ������
	void Add_Vertex(SharPrj& prj)
	{
		stepVertexs++;

		for (auto vertex : projects[prj->prjName])
		{
			if (!vertex->pass) return;
		}

		stepPrjs++;
	}

public:

	map<wstring, SSharVertex> projects;

	// ������� �����
	int allVertexs; // �������
	int allEdges;   // ����
	int allPrjs;    // �������

	// ������� �� ������ ����
	int stepVertexs; // �������
	int stepEdges;   // ����
	int stepPrjs;    // �������
};


//-------------------------------------------------------------------
// ����� ���������� � ������ � ����� (����������)
//-------------------------------------------------------------------
class NetControl
{
public:
	NetControl(Config& Configurate);
	void Sorting();

public:
	int maxSteps;   // ������������ ����� �����
	Config *config; // ��������� �� ����� ������������

	// ������� � ����
	VsharPrj projects; // ��� ������� 
	VertexNet net[NETCOUNT];  // ��� ����

	// ����� ������������ �������� (�������� �� ����������)
	map<wstring, SharPrj>  prjPaths; // path <-> project
	map<wstring, SharPrj>  prjNames; // name <-> project
	map<wstring, SharPrj>  prjGUIDs; // GUID <-> project

private:
	// �������� � ���������� ������ � ����� ����
	void Create_Vertexs();

	// ������ ������������
	void Scan_Depends();

	// ���������� �� �������
	void Separation_Subnet();

	//-------------------------------------------------------------------

	// �������� �������� ������
	void Create_Dependes(int ind);

	// ���������� ������� ������
	void Filling_Strong_Depences(int ind);

	// ���������� ������ ������
	void Filling_Weak_Depences(int ind);

	// ���������� ����
	void Net_Sorting(int ind);

	// ����������� ����
	void Net_Optimization(int ind);

	// ����������� �������� ����
	void Net_Check(int ind);

	//-------------------------------------------------------------------

	// ����������� ����� ������������� ������
	void Release_vertex(SharVertex& vertex, int numSteps, Report& report);

	// ����������� ����� ���������� �� �������
	int Immersion(SharVertex& vertex);

	//-------------------------------------------------------------------

	// ���������� ����� ������ ��� ������������ C# ��������
	void CSproj_Add_Build();

	// ������ �� �������� (����� � ������)
	wstring Trim(const wstring& a_str);

	//-------------------------------------------------------------------

	// ����� ������
	void Net_Display(int ind); TO_CHK

private:
	// ���������� ����
	SharVertex selectVertex; // ��������� �������
	Swstring passStepPrj; // ���������� ������� �� ����

	// ����� �������
	SSharVertex vertexs; // �� ���������������
};

