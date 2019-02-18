using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Smart_file_hunter
{
    // основной класс
    public partial class Form_Main : Form
    {
        string startDir;  // стартовая директория поиска

        // потоки
        SynchronizationContext uiContext; // синхронизация
        Thread find_thread;     // поток поиска файлов
        Thread count_thread;    // поток подсчёта файлов

        // счетчики
        int second; // секунды
        int minute; // минуты
        int hour;   // часы
        int countFiles; // всего файлов
        int leftFiles;  // пройдено файлов
        int filesFind;  // найденно файлов
        Regex[] regexs; // шаблоны имени


        // #########################    Методы    #####################################
        // ############################################################################

        
        public Form_Main()  
        {
            InitializeComponent();

            string help = "используются регулярные выражения:\n";
            help += @"^: соответствие должно начинаться в начале строки (например, выражение  ^пр\w*  соответствует слову 'привет' в строке 'привет мир')" + "\n";
            help += @"$: конец строки (например, выражение \w*ир$ соответствует слову 'мир' в строке 'привет мир', так как часть 'ир' находится в самом конце)" + "\n";
            help += @".: знак точки определяет любой одиночный символ (например, выражение 'м.р' соответствует слову 'ми' или 'мор')" + "\n";
            help += @"*: предыдущий символ повторяется 0 и более раз" + "\n";
            help += @"+: предыдущий символ повторяется 1 и более раз" + "\n";
            help += @"?: предыдущий символ повторяется 0 или 1 раз" + "\n";
            help += @"\s: соответствует любому пробельному символу" + "\n";
            help += @"\S: соответствует любому символу, не являющемуся пробелом" + "\n";
            help += @"\w: соответствует любому алфавитно-цифровому символу" + "\n";
            help += @"\W: соответствует любому не алфавитно-цифровому символу" + "\n";
            help += @"\d: соответствует любой десятичной цифре" + "\n";
            help += @"\D : соответствует любому символу, не являющемуся десятичной цифрой" + "\n";

            helpTool.SetToolTip(namePattern_box, help);

        }

        // -------------------------    События формы    ------------------------------------

        private void Form_Load(object sender, EventArgs e)              
        { // событие: загрузка формы

            Read_Config();

            // начальное положение счетчиков
            lb_timePass.Text = "-- -- -- --";
            lb_fileAll.Text = "-- -- --";
            lb_fileLast.Text = "-- -- --";
            lb_filesFind.Text = "-- -- --";
            filesFind = 0;

            b_abortSearch.Visible = false;
            b_abortSearch.Enabled = false;

            pb_progressSearch.Value = 0;
            search_group.Enabled = false;
            b_continueSearch.Enabled = false;

            uiContext = SynchronizationContext.Current;

        } // событие: открытие формы
        private void Form_Closing(object sender, FormClosingEventArgs e)
        { // событие: закрытие формы

            Save_Config();

        } // событие: закрытие формы

        // -------------------------    Панель стартовой директории    ------------------------

        private void Open_FileDialog(object sender, EventArgs e)        
        { // кнопка: выбор стартовой директории

            dirSelecter.ShowDialog();

            string line = dirSelecter.SelectedPath;

            if (line != "")
            {
                tb_startDir.Text = line;
                dirSelecter.SelectedPath = "";
            }

        } // кнопка: выбор стартовой директории
        private void StartDir_TextChanged(object sender, EventArgs e)   
        { // Событие: изменение стартовой директории

            startDir = tb_startDir.Text;

        } // Событие: изменение стартовой директории

        // -------------------------    Панель шаблонов    ------------------------------------

        private void Add_NamePattern(object sender, EventArgs e)        
        { // кнопка: добавить шаблон имени

            namePattern_box.Items.Add(tb_namePattern.Text);

        } // кнопка: добавить шаблон имени
        private void UpOrder_NamePattern(object sender, EventArgs e)    
        { // кнопка: поднять приоритет шаблона имени

            int ind = namePattern_box.SelectedIndex;

            if (ind > 0)
            {
                string line = (string) namePattern_box.Items[ind];
                namePattern_box.Items.RemoveAt(ind);
                namePattern_box.Items.Insert(ind - 1, line);
                namePattern_box.SelectedIndex = ind - 1;
            }

        } // кнопка: поднять приоритет шаблона имени
        private void DelOrder_NamePattern(object sender, EventArgs e)   
        { // кнопка: удалить шаблон имени

            if (namePattern_box.SelectedIndex != -1)
            {
                namePattern_box.Items.RemoveAt(namePattern_box.SelectedIndex);
            }

        } // кнопка: удалить шаблон имени
        private void DownOrder_NamePattern(object sender, EventArgs e)  
        { // кнопка: опустить приоритет шаблона имени

            int ind = namePattern_box.SelectedIndex;

            if (ind != -1 && ind < namePattern_box.Items.Count - 1)
            {
                string line = (string)namePattern_box.Items[ind];
                namePattern_box.Items.RemoveAt(ind);
                namePattern_box.Items.Insert(ind + 1, line);
                namePattern_box.SelectedIndex = ind + 1;
            }

        } // кнопка: опустить приоритет шаблона имени

        // -------------------------    Панель текста    ------------------------------------

        private void Add_TextPattern(object sender, EventArgs e)        
        { // кнопка: добавить шаблон текста

            textPattern_box.Items.Add(tb_textPattern.Text);

        } // кнопка: добавить шаблон текста
        private void UpOrder_TextPattern(object sender, EventArgs e)    
        { // кнопка: поднять приоритет шаблона текста

            int ind = textPattern_box.SelectedIndex;

            if (ind > 0)
            {
                string line = (string) textPattern_box.Items[ind];
                textPattern_box.Items.RemoveAt(ind);
                textPattern_box.Items.Insert(ind - 1, line);
                textPattern_box.SelectedIndex = ind - 1;
            }

        } // кнопка: поднять приоритет шаблона текста
        private void DelOrder_TextPattern(object sender, EventArgs e)   
        { // кнопка: удалить шаблон текста

            if (textPattern_box.SelectedIndex != -1)
            {
                textPattern_box.Items.RemoveAt(textPattern_box.SelectedIndex);
            }

        } // кнопка: удалить шаблон текста
        private void DownOrder_TextPattern(object sender, EventArgs e)  
        { // кнопка: поднять приоритет шаблона текста

            int ind = textPattern_box.SelectedIndex;

            if (ind != -1 && ind < textPattern_box.Items.Count-1)
            {
                string line = (string)textPattern_box.Items[ind];
                textPattern_box.Items.RemoveAt(ind);
                textPattern_box.Items.Insert(ind + 1, line);
                textPattern_box.SelectedIndex = ind + 1;
            }

        } // кнопка: опустить приоритет шаблона текста

        // -------------------------    Панель поиска    ------------------------------------

        private void Search_Start(object sender, EventArgs e)           
        { // кнопка: начать поиск

            // регулярные выражения
            regexs = new Regex[namePattern_box.Items.Count];
            for (int i = 0; i<regexs.Length; i++) regexs[i] = new Regex((string)namePattern_box.Items[i]);

            // создание потоков
            find_thread = new Thread(Run_Search); // основной поток поиска
            count_thread = new Thread(Run_Counter); // поток подсчёта

            // активация панели поиска
            search_group.Enabled = true;
            b_stopSearch.Enabled = true;
            b_continueSearch.Enabled = false;

            // смена кнопок
            b_startSearch.Visible = false;
            b_startSearch.Enabled = false;
            b_abortSearch.Visible = true;
            b_abortSearch.Enabled = true;

            // обнуление счетчиков
            lb_fileAll.Text = "считаю";
            lb_fileLast.Text = "считаю";
            countFiles = 0;
            leftFiles = 0;
            filesFind = 0;

            // создание корня дерева
            FileSystemInfo diRoot = new DirectoryInfo(startDir);
            TreeNode root = new TreeNode(diRoot.Name, 1, 1)
            {
                Name = startDir
            };
            tv_findItem.Nodes.Clear();
            tv_findItem.Nodes.Add(root);

            // активация времени
            second = 0;
            minute = 0;
            hour = 0;
            time.Start();

            // активация поиска
            find_thread.Start(root); //uiContext
            count_thread.Start(null); //uiContext

        } // кнопка: начать поиск
        private void Search_Abort(object sender, EventArgs e)           
        { // кнопка: прекратить поиск

            search_group.Enabled = false;

            // смена кнопок
            b_startSearch.Visible = true;
            b_startSearch.Enabled = true;
            b_abortSearch.Visible = false;
            b_abortSearch.Enabled = false;

            // закрытие активных потоков
            time.Stop();
            if (find_thread.ThreadState == ThreadState.Suspended) find_thread.Resume();
            find_thread.Abort();

            if (count_thread.IsAlive)
            {
                lb_fileAll.Text = "остановлен";
                count_thread.Abort();
            }

        } // кнопка: прекратить поиск
        private void Search_Stop(object sender, EventArgs e)            
        { // кнопка: остановить поиск

            b_stopSearch.Enabled = false;
            b_continueSearch.Enabled = true;

            find_thread.Suspend();
            time.Stop();

        } // кнопка: остановить поиск
        private void Search_Continue(object sender, EventArgs e)        
        { // кнопка: продолжить поиск

            b_stopSearch.Enabled = true;
            b_continueSearch.Enabled = false;

            find_thread.Resume();
            time.Start();

        } // кнопка: продолжить поиск

        // -------------------------    Управление потоками    ------------------------------

        private void Run_Counter(object root)                           
        { // поток: подсчёт файлов

            countFiles = Count_AllFiles(startDir);
            uiContext.Post(UI_UpdCount, null);
            count_thread.Abort();

        } // поток: подсчёт файлов
        private void Run_Search(object root)                            
        { // поток: поиск файлов

            Search_Files(startDir,(TreeNode) root);

            uiContext.Post(UI_StopSearch, null);
            find_thread.Abort();

        } // поток: поиск файлов
        private void Time_Tick(object sender, EventArgs e)              
        { // Поток: отсчёт времени

            second++;
            if (second > 59)
            {
                second = 0;
                minute++;

                if (minute > 59)
                {
                    minute = 0;
                    hour++;
                }
            }

            string time = "";

            if (hour > 0) time += hour + " ч  ";
            if (minute > 0) time += minute + " м  ";
            if (second > 0) time += second + " с  ";

            lb_timePass.Text = time;

        } // Поток: отсчёт времени

        private void UI_UpdTree(object state)                           
        { // обновление интерфейса: добавление узла в дерево

            FindItem findI = state as FindItem;
            filesFind += findI.add;
            lb_filesFind.Text = filesFind + "";

            // поиск корня в дерево по имени
            TreeNode[] root = tv_findItem.Nodes.Find(findI.root.Name, true);

            if (root.Length > 0)
            {
                // этот корень встроен в дерево
                // можно подсоединять найденный файл
                findI.root.Nodes.Add(findI.node);
            }
            else
            {
                // этого корня в дереве нет, его нужно создать
                // а точнее, нужно восстановить полное дерево
                // от найденного листа (файла) до папки, которая встроена в дерево
                Tree_Building(findI.root.Name, findI.node);
            }

        } // обновление интерфейса: добавление узла в дерево
        private void UI_StopSearch(object state)                        
        { // обновление интерфейса: конец поиска

            Search_Abort(null, null);

        } // обновление интерфейса: конец поиска
        private void UI_UpdFileLeft(object state)                       
        { // обновление интерфейса: пройдено файлов

            ReportItem report = state as ReportItem;

            leftFiles += report.filesCount;
            lb_fileLast.Text = leftFiles + "";

            if (countFiles > 0)
            {
                double result = (double) leftFiles / countFiles * 100;
                pb_progressSearch.Value = (int) result;
            }

            tb_readir.Text = report.diread;

        } // обновление интерфейса: пройдено файлов
        private void UI_UpdCount(object state)                          
        { // обновление интерфейса: добавление числа файлов

            lb_fileAll.Text = countFiles + "";

        } // обновление интерфейса: добавление числа файлов

        // -------------------------    Рекурсии    -----------------------------------------

        private void Search_Files(string findPath, TreeNode root)       
        { // поиск файлов по критериям

            // получить все элементы по пути
            string[] dirs = Directory.GetFileSystemEntries(findPath);
            uiContext.Post(UI_UpdFileLeft, new ReportItem(dirs.Length, findPath));

            // обход элементов корневого узла
            foreach (string dir in dirs)
            {
                FileSystemInfo diNode = new DirectoryInfo(dir);
                TreeNode node = new TreeNode(diNode.Name)
                {
                    ImageIndex = diNode.Attributes == FileAttributes.Directory ? 1 : 2,
                    SelectedImageIndex = diNode.Attributes == FileAttributes.Directory ? 1 : 2,
                    Name = dir
                };

                if (diNode.Attributes == FileAttributes.Directory)
                {
                    // уход в рекурсию, если папка
                    Search_Files(diNode.FullName, node);
                }
                else
                {
                    // если не папка, то анализируем по критериям
                    bool flag = true;

                    // анализ имени
                    foreach (Regex r in regexs)
                    {
                        if (r.Match(diNode.Name).Success)
                        {
                            uiContext.Post(UI_UpdTree, new FindItem(root, node, 1));
                            flag = false;
                            break;
                        }
                    }

                    // если имя не найдено, анализируем текст
                    if (flag)
                    {
                        if (Read_Files(dir))
                            uiContext.Post(UI_UpdTree, new FindItem(root, node, 1));
                    }
                }
            }
        } // поиск файлов по критериям
        private int Count_AllFiles(string findPath)                     
        { // подсчёт файлов

            // нужно для определения количества файлов, которое надо проанализировать
            // работает в отдельном потоке и не останавливается при приостановке потока поиска
            // останавливается либо при завершении подсчёта или при жестком завершении

            int filesFind = 0;

            // получить все элементы по пути
            try
            {
                string[] dirs = Directory.GetFileSystemEntries(findPath);
                filesFind += dirs.Length;

                // обход элементов корневого узла
                foreach (string dir in dirs)
                {
                    FileSystemInfo diNode = new DirectoryInfo(dir);

                    if (diNode.Attributes == FileAttributes.Directory)
                    {
                        // уход в рекурсию, если папка
                        filesFind += Count_AllFiles(diNode.FullName);
                    }
                }
            }
            catch
            {
                return filesFind;
            }
            

            return filesFind;

        } // подсчёт файлов
        private void Tree_Building(string rootPath, TreeNode node)      
        { // сборка дерева

            // дерево собирается от дочерних (найденных файлов) к основному дереву
            // каждая следующая итерация, приближает ветку к основному дереву
            // и в итоге прицепляется к нему

            // небольшая заплатка, чтобы можно было искать от начала диска
            // без этого, возникает проблема несуществующего адреса (например с: )
            if (rootPath.Length < 3) rootPath += "\\";

            TreeNode[] nodes = tv_findItem.Nodes.Find(rootPath, true);

            if (nodes.Length > 0)
            {
                // найдено дерево, можно подсоединятся

                nodes[0].Nodes.Add(node);
                return;
            }

            FileSystemInfo diNode = new DirectoryInfo(rootPath);
            TreeNode root = new TreeNode(diNode.Name, 1, 1)
            {
                Name = rootPath
            };

            root.Nodes.Add(node);

            string newPath = rootPath.Remove(rootPath.LastIndexOf('\\'));

            // уход в рекурсию
            // создание родительского узла
            Tree_Building(newPath, root);

        } // сборка дерева

        // -------------------------    Чтение/запись файлов    -----------------------------

        private bool Read_Files(string path)                            
        { // чтение файла

            try
            {
                using (StreamReader fs = new StreamReader(path))
                {
                    while (true)
                    {
                        string temp = fs.ReadLine();

                        if (temp == null) break;

                        // обход по строчкам поиска
                        foreach (var str in textPattern_box.Items)
                        {
                            if (temp.IndexOf((string)str) > -1) return true;
                        }
                    }
                }
            }
            catch
            {
                // сюда может попасть, если нет доступа на чтение
                return false;
            }

            return false;

        } // чтение файла
        private void Read_Config()                                      
        { // чтение конфига

            string path = "Config.txt";

            if (!File.Exists(path)) return;

            string[] data = File.ReadAllLines(path, Encoding.GetEncoding(1251));

            tb_startDir.Text = data[0].Split('-')[1];

            int countN = int.Parse(data[1].Split('-')[1]);
            for (int i = 0; i <= countN - 1; i++) namePattern_box.Items.Add(data[i + 2]);

            int countT = int.Parse(data[2+countN].Split('-')[1]);
            for (int i = 0; i <= countT - 1; i++) textPattern_box.Items.Add(data[i + 3 + countN]);

        } // чтение конфига
        private void Save_Config()                                      
        { // сохранение конфига

            string path = "Config.txt";
            int count = 3 + namePattern_box.Items.Count + textPattern_box.Items.Count;
            string[] data = new string[count];

            data[0] = "стартовая директория-" + startDir;
            data[1] = "шаблоны имен-" + namePattern_box.Items.Count;
            for (int i = 0; i < namePattern_box.Items.Count; i++)
                data[i + 2] = (string) namePattern_box.Items[i];

            data[2 + namePattern_box.Items.Count] = "шаблоны текста-" + textPattern_box.Items.Count;
            for (int i = 0; i < textPattern_box.Items.Count; i++)
                data[i + 3 + namePattern_box.Items.Count] = (string) textPattern_box.Items[i];

            File.WriteAllLines(path, data, Encoding.GetEncoding(1251));

        } // сохранение конфига

    }

    // побочный класс: хранит корень узла и узел
    class FindItem      
    {
        public int add; // увеличитель счетчика найденных файлов
        public TreeNode root; // корень узла
        public TreeNode node; // узел

        public FindItem(TreeNode Root, TreeNode Node, int Add)
        {
            root = Root;
            node = Node;
            add = Add;
        }
    }

    // побочный класс: хранит число прочитанных файлов и имена
    class ReportItem    
    {
        public int filesCount;
        public string diread;

        public ReportItem(int FilesC, string Diread)
        {
            filesCount = FilesC;
            diread = Diread;
        }
    }

}