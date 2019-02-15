// stdafx.h: включаемый файл дл€ стандартных системных включаемых файлов
// или включаемых файлов дл€ конкретного проекта, которые часто используютс€, но
// не часто измен€ютс€
//

#pragma once

#include "targetver.h"
#include <iostream>
#include <stdio.h>
#include <algorithm>
#include <windows.h>

// запись в файл
#include <fstream>
#include <locale>
#include <codecvt>

// поключение типов std
#include <string>
#include <stack>
#include <list>
#include <vector>
#include <tchar.h>
#include <memory>
#include <map>
#include <set>
#include <queue>
#include <iterator>

// чтение узлов xml файла vcxproj
#include <ole2.h>
#include <xmllite.h>
#include <shlwapi.h>

// обработка аргументов из командной строки
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS 
#include <atlstr.h>
#include "CmdLineParser.h"

// дополнительные включени€
#include "SubInclus.h"
#include "StringRepository.h"
#include "File.h"
#include "Config.h"
#include "Project.h"
#include "Error.h"

// управл€ющие классы
#include "FileFinder.h"
#include "NetControl.h"
#include "XmlReader.h"
#include "SlnReader.h"
#include "SlnCreator.h"

// TODO: ”становите здесь ссылки на дополнительные заголовки, требующиес€ дл€ программы
