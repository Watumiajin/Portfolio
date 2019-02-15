
#pragma once
#include "stdafx.h"

//-------------------------------------------------------------------
// Cтруктура вершины графа
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
	
	// добавление входящей связи (от которой зависим)
	bool AddDepend(SharVertex first, SharVertex second)
	{
		// защита от дублирующих связей
		if (!second->outEdge_unsorted.count(first))
		{
			incoming++;
			second->outgoing++;
			second->outEdge_unsorted.insert(first);
			
			// связь с другим проектом?
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
	// основные параметры вершины
	wstring		name; // имя вершины
	SharPrj     prj;  // ссылка на проект
	SharPrjConf conf; // ссылка на сборку

	// параметры для сортировки сети
	bool pass;     // вершина полность пройдена
	int  incoming; // число вершин от которых зависим
	int  outgoing; // число зависимых вершин от этой
	int  depth;    // глубина зависимостей вершины

	//-------------------------------------------------------------------
	// Переопределение для операции сравнения между зависимостями вершины
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

	// ------------------------ связи с вершинами -----------------------
	// ##################################################################

	SSharPrj addedStrongDepend; // проекты добавленные от сильных связей

	// исходящие связи (зависимые от этой)
	SSharVertex outEdge_unsorted; // исходящие связи (не отсортированные)
	SSharVertexSort outEdge_sorted; // исходящие связи (отсортированные)

	TO_CHK

	// входящие связи (от которых зависим)
	SSharVertex inEdge; // входящие связи

	// исходящие связи (зависимые от этой)
	SSharVertex strongDepend; // сильные связи (исходящие)
	SSharVertex weakDepend;   // слабые связи  (исходящие)
};


//-------------------------------------------------------------------
// Cтруктура сети
//-------------------------------------------------------------------
struct VertexNet
{
	VertexNet() : edge_count(0), сonf_count(0), steps(0) {};

	// параметры сети
	int edge_count; // число ребёр
	int	сonf_count; // число сборок 
	int steps;      // шагов сборки

	wstring cap;      // разрядность
	wstring type;     // тип конфигурации сборки
	size_t Release : 1, Win32 : 1; // платформа и разрядность сети

	// вершины
	SSharVertex vertexs_unsorted; // не отсортированные
	Vertex::SSharVertexSort vertexs_sorted; // отсортированные вершины

	// связи
	map<wstring, SharVertex> strongDepends; // targetName <-> ссылка на вершину
	map<wstring, SharVertex> weakDepends;   // prjName + L"." + confName <-> ссылка на вершину
	map<wstring, SSharVertex> projects;     // привязка проекта к его вершинам в этой сети по имени
};


//-------------------------------------------------------------------
// Cтруктура отчёта
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

	// сброс шага
	void Reset_Step(bool verb)
	{
		if (verb)
		{
			DISPOUT << L"\n -- " 
					<< L"вершин: " << stepVertexs
					<< L"  рёбер: " << stepEdges
					<< L"  проектов: " << stepPrjs;
		}
		else
		{
			DISPOUT << L"\tвершин: " << stepVertexs
					<< L"\tрёбер: " << stepEdges
					<< L"\tпроектов: " << stepPrjs;
		}



		allVertexs += stepVertexs;
		stepVertexs = 0;

		allEdges += stepEdges;
		stepEdges = 0;

		allPrjs += stepPrjs;
		stepPrjs = 0;
	}

	// добавить вершину и проект
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

	// закрыто всего
	int allVertexs; // вершины
	int allEdges;   // рёбра
	int allPrjs;    // проекты

	// закрыто на данном шаге
	int stepVertexs; // вершины
	int stepEdges;   // рёбра
	int stepPrjs;    // проекты
};


//-------------------------------------------------------------------
// Класс управления и работы с сетью (топологией)
//-------------------------------------------------------------------
class NetControl
{
public:
	NetControl(Config& Configurate);
	void Sorting();

public:
	int maxSteps;   // максимальное число шагов
	Config *config; // параметры из файла конфигурации

	// проекты и сети
	VsharPrj projects; // все проекты 
	VertexNet net[NETCOUNT];  // все сети

	// ранее используемые значения (контроль от повторений)
	map<wstring, SharPrj>  prjPaths; // path <-> project
	map<wstring, SharPrj>  prjNames; // name <-> project
	map<wstring, SharPrj>  prjGUIDs; // GUID <-> project

private:
	// создание и добавление вершин в общую сеть
	void Create_Vertexs();

	// анализ зависимостей
	void Scan_Depends();

	// разделение на подсети
	void Separation_Subnet();

	//-------------------------------------------------------------------

	// создание основных связей
	void Create_Dependes(int ind);

	// добавление сильных связей
	void Filling_Strong_Depences(int ind);

	// добавление слабых связей
	void Filling_Weak_Depences(int ind);

	// сортировка сети
	void Net_Sorting(int ind);

	// оптимизация сети
	void Net_Optimization(int ind);

	// контрольная проверка сети
	void Net_Check(int ind);

	//-------------------------------------------------------------------

	// рекурсивный метод высвобождения вершин
	void Release_vertex(SharVertex& vertex, int numSteps, Report& report);

	// рекурсивный метод погружения на глубину
	int Immersion(SharVertex& vertex);

	//-------------------------------------------------------------------

	// добавление шагов сборки для зависимостей C# проектам
	void CSproj_Add_Build();

	// очиста от пробелов (слева и справо)
	wstring Trim(const wstring& a_str);

	//-------------------------------------------------------------------

	// вывод связей
	void Net_Display(int ind); TO_CHK

private:
	// сортировка сети
	SharVertex selectVertex; // выбранная вершина
	Swstring passStepPrj; // добавленые проекты на шаге

	// общие вершины
	SSharVertex vertexs; // не отсортированные
};

