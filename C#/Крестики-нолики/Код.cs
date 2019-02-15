using System;
using System.Collections.Generic;
using System.Data;
using System.Drawing;
using System.Drawing.Imaging;
using System.Linq;
using System.Windows.Forms;
using System.IO;
using System.Diagnostics;

namespace Obscurity
{
    public partial class Main_Form : Form
    {
        public Main_Form () // инициализация элементов управдения 
        {
            InitializeComponent ();
        }

        //----------------------------  переменные  -------------------------------------
        Random rand = new Random (); // рандом

        // игровые переменные
        int[] Game = new int[5]; // [0] - игрок, [1] - опонент, [2] - фаза игры, [3] - победитель, [4] - счётчик хода,
        int[] field = new int[10]; // используемые клетки
        int[] progress = new int[9];
        int[,] coordinates = new int[10,2]; // координаты клеток

        // игровое поле
        static Image map = new Bitmap (Properties.Resources.field);
        Graphics g = Graphics.FromImage (map);

        // дополнительная панель
        int extra_extent = 200;

        // линия победителя
        Pen pen = new Pen (Color.Red,5);
        int x1, x2, y1, y2; // координаты рисования
        
        // пути к файлам
        static string T_way = Application.StartupPath + "\\Temporary memory.txt"; // путь к временной памяти 
        static string M_way = Application.StartupPath + "\\Main memory.txt";      // путь к основной памяти
        static string asset_way = Application.StartupPath + "\\Asset.txt";  // путь к найстройкам
        StreamWriter T_txt, M_txt; // файл памяти для записи

        // память
        string estimation; // запись партии \ промежуточная запись
        string[] DATA; // промежуточная память
        List<string> reason = new List<string> (); // активная память опонента
        List<string> C_memory = new List<string> (), Z_memory = new List<string> (); // отдельная память для нолики-крестики
        List<string> M_memory = new List<string> (), T_memory = new List<string> (); // основная и временная память

        // размеры массивов с памятью
        // [0] - Обший размер вр. памяти
        // [1] - Размер осн. крестики, [2] - Размер осн. нолики
        int[] lor = new int[3]; // общий размер памяти

        // сепараторы
        string[] sep_1 = new string[] { "\r\n" }; // разбивание по линиям
        string[] sep_2 = new string[] { "-","."}; // разбивание по ходам

        //----------------------------  элементы управления  ----------------------------

        private void ResetBt_click (object sender,EventArgs e) // сброс игры 
        {
            map.Save ("estimation.jpg",ImageFormat.Jpeg); // сохранение картинки поля
            if (Game[2] == 2 & Game[3] != 0) Memory_record (); // для записи ничьей, убрать "& Game[3] != 0"

            Reset ();

            if (autoupd_check.Checked)  Memory_update (); // обновление
        }
        private void Update_click  (object sender,EventArgs e) // обновление памяти 
        {
            Memory_update (); // обновление
        }
        private void StandFd_click (object sender,EventArgs e) // игровое поле 
        {
            //1|2|3
            //4|5|6
            //7|8|9

            // координаты относительно формы
            int X = MousePosition.X - Location.X - 19;
            int Y = MousePosition.Y - Location.Y - 42;

            // 0 - выбор руки, 1 - ход игрока, 2 - конец игры
            // ---- --- --- --- --- ---

            if (Game[2] == 1) // ход игрока
            {
                if (X >= 10 & X <= 80 & Y >= 10 & Y <= 80 & field[1] == 0) Game_record (1,Game[0],1);
                if (X >= 90 & X <= 160 & Y >= 10 & Y <= 80 & field[2] == 0) Game_record (2,Game[0],1);
                if (X >= 170 & X <= 240 & Y >= 10 & Y <= 80 & field[3] == 0) Game_record (3,Game[0],1);

                if (X >= 10 & X <= 80 & Y >= 90 & Y <= 160 & field[4] == 0) Game_record (4,Game[0],1);
                if (X >= 90 & X <= 160 & Y >= 90 & Y <= 160 & field[5] == 0) Game_record (5,Game[0],1);
                if (X >= 170 & X <= 240 & Y >= 90 & Y <= 160 & field[6] == 0) Game_record (6,Game[0],1);

                if (X >= 10 & X <= 80 & Y >= 170 & Y <= 240 & field[7] == 0) Game_record (7,Game[0],1);
                if (X >= 90 & X <= 160 & Y >= 170 & Y <= 240 & field[8] == 0) Game_record (8,Game[0],1);
                if (X >= 170 & X <= 240 & Y >= 170 & Y <= 240 & field[9] == 0) Game_record (9,Game[0],1);

                if (Game[2] != 2) Сombination ();
            }

            // ---- --- --- --- --- ---

            if (Game[2] == 0) // выбор руки
            {
                // 1 - крестики, 2 - нолики
                if (X >= 40 & X <= 110 & Y >= 90 & Y <= 160)
                {
                    Game[0] = 1; Game[1] = 2;
                    reason.AddRange (Z_memory);
                    Game[2] = 1; Сombination ();
                }
                if (X >= 140 & X <= 210 & Y >= 90 & Y <= 160)
                {
                    Game[0] = 2; Game[1] = 1;
                    reason.AddRange (C_memory);
                    Oponent (); Сombination ();
                }
            }
        }
        private void T_Click       (object sender,EventArgs e) // открытие файла 
        {
            Process.Start (T_way);
        }
        private void M_Click       (object sender,EventArgs e) // открытие файла 
        {
            Process.Start (M_way);
        }
        private void extra_open    (object sender,EventArgs e) // панель интелекта 
        {
            this.Height += extra_extent;
            extra_1.Hide (); extra_2.Show ();
        }
        private void extra_close   (object sender,EventArgs e) // панель интелекта 
        {
            this.Height -= extra_extent;
            extra_2.Hide (); extra_1.Show ();
        }
        private void MainForm_load (object sender,EventArgs e) // запуск формы 
        {
            this.Height -= extra_extent; // скрытие панели интелекта

            Reset (); Set_load ();

            // координаты клеток
            for (int i = 1, j = 0, x = 10, y = -70; i<10; i++, j++, x += 80)
            {
                if (j % 3 == 0) { x = 10; y += 80; }
                coordinates[i,0] = x;
                coordinates[i,1] = y;
            }
        }

        //----------------------------  методы управления  ------------------------------

        public void Reset () // сброс 
        {
            // сброс массивов
            for (int i = 0; i<10; i++) field[i] = 0; // используемые клетки
            for (int i = 0; i<9; i++) progress[i] = 0; // прогресс игры

            // сброс параметров
            for (int i = 0; i<4; i++) Game[i] = 0; Game[4] = 1;
            estimation = "";
            label1.Text = "";
            reason.Clear ();

            // сброс картинок
            winner.Image = Properties.Resources.field;
            g.DrawImage (Properties.Resources.field,0,0,250,250);
            label1.Text = "";

            Memory_loadin (0,2);
            Selection_hand ();
        }
        public void Сombination () // рисование игрового поля 
        {
            g.DrawImage (Properties.Resources.field,0,0,250,250);

            for (int i = 1; i<10; i++)
            {
                if (field[i] == 0) g.DrawImage (Properties.Resources.free,
                    coordinates[i,0], coordinates[i,1], 70,70);
                if (field[i] == 1) g.DrawImage (Properties.Resources.cross,
                    coordinates[i,0], coordinates[i,1], 70,70);
                if (field[i] == 2) g.DrawImage (Properties.Resources.zero,
                    coordinates[i,0], coordinates[i,1], 70,70);
            }

            if (Game[3] == 1) pen.Color = Color.OrangeRed;  // крестики победили
            else pen.Color = Color.Aqua;                    // нолики победили
            if (Game[3] != 0) g.DrawLine (pen,x1,y1,x2,y2); // рисование линии         

            stand.Image = map; // вывод игрового поля на экран
        }
        public void Selection_hand () // выбор руки 
        {
            Game[2] = 0;

            g.DrawImage (Properties.Resources.cross, 40,90, 70,70);
            g.DrawImage (Properties.Resources.zero, 140,90, 70,70);

            stand.Image = map; // вывод игрового поля на экран
        }
        public void Verification () // проверка на конец игры 
        {
            // ничья
            if (field[1] != 0 & field[2] != 0 & field[3] != 0 &
                field[4] != 0 & field[5] != 0 & field[6] != 0 &
                field[7] != 0 & field[8] != 0 & field[9] != 0)
                {
                Game[2] = 2; Game[3] = 0; label1.Text = "Ничья!";
                Сombination (); winner.Image = Properties.Resources.free;
            }
            // победа крестика
            // ---
            if (field[1] == 1 & field[2] == 1 & field[3] == 1)
            { x1 = 10; y1 = 45; x2 = 240; y2 = 45; Game[3] = 1; }
            if  (field[4] == 1 & field[5] == 1 & field[6] == 1)
            { x1 = 10; y1 = 125; x2 = 240; y2 = 125; Game[3] = 1; }
            if (field[7] == 1 & field[8] == 1 & field[9] == 1)
            { x1 = 10; y1 = 205; x2 = 240; y2 = 205; Game[3] = 1; }
            // |
            if (field[1] == 1 & field[4] == 1 & field[7] == 1)
            { x1 = 45; y1 = 10; x2 = 45; y2 = 240; Game[3] = 1; }
            if (field[2] == 1 & field[5] == 1 & field[8] == 1)
            { x1 = 125; y1 = 10; x2 = 125; y2 = 240; Game[3] = 1; }
            if (field[3] == 1 & field[6] == 1 & field[9] == 1)
            { x1 = 205; y1 = 10; x2 = 205; y2 = 240; Game[3] = 1; }
            // \ /
            if (field[1] == 1 & field[5] == 1 & field[9] == 1)
            { x1 = 10; y1 = 10; x2 = 240; y2 = 240; Game[3] = 1; }
            if (field[3] == 1 & field[5] == 1 & field[7] == 1)
            { x1 = 10; y1 = 240; x2 = 240; y2 = 10; Game[3] = 1; }

            if (Game[3] == 1)
            {
                Game[2] = 2; label1.Text = "победили крестики!";
                Сombination (); winner.Image = Properties.Resources.cross;
            }
            // победа нолика
            // ---
            if (field[1] == 2 & field[2] == 2 & field[3] == 2)
            { x1 = 10; y1 = 40; x2 = 240; y2 = 40; Game[3] = 2; }
            if (field[4] == 2 & field[5] == 2 & field[6] == 2)
            { x1 = 10; y1 = 120; x2 = 240; y2 = 120; Game[3] = 2; }
            if (field[7] == 2 & field[8] == 2 & field[9] == 2)
            { x1 = 10; y1 = 200; x2 = 240; y2 = 200; Game[3] = 2; }
            // |
            if (field[1] == 2 & field[4] == 2 & field[7] == 2)
            { x1 = 40; y1 = 10; x2 = 40; y2 = 240; Game[3] = 2; }
            if (field[2] == 2 & field[5] == 2 & field[8] == 2)
            { x1 = 120; y1 = 10; x2 = 120; y2 = 240; Game[3] = 2; }
            if (field[3] == 2 & field[6] == 2 & field[9] == 2)
            { x1 = 200; y1 = 10; x2 = 200; y2 = 240; Game[3] = 2; }
            // \ /
            if (field[1] == 2 & field[5] == 2 & field[9] == 2)
            { x1 = 10; y1 = 10; x2 = 240; y2 = 240; Game[3] = 2; }
            if (field[3] == 2 & field[5] == 2 & field[7] == 2)
            { x1 = 10; y1 = 240; x2 = 240; y2 = 10; Game[3] = 2; }

            if (Game[3] == 2)
            {
                Game[2] = 2; label1.Text = "победили нолики!";
                Сombination (); winner.Image = Properties.Resources.zero;
            }
        }

        //----------------------------  работа с памятью  ------------------------------

        public void Game_record (int i, int hand, int player) // запись игры 
        {
            // i -- 1 - ход игрока, 2 - ход компьютера

            Verification (); // проверка на конец игры

            if (Game[2] != 2)
            {
                field[i] = hand;
                progress[Game[4]-1] = i;
                Game[4]++;
                Verification ();
            }
            if (player == 1) Oponent ();
            if (player == 2 & Game[2] != 2) Game[2] = 1;
        }
        public void Memory_record () // запись временной памяти 
        {
            Memory_loadin (1,0);

            // запись партий
            
            estimation += (Game[4]-1); for (int i = 0; i < 9;i++) estimation += "-" + progress[i]; estimation += "." + Game[3]; // текущая
            for (int i = 0; i<lor[0]; i++) estimation += "\r\n" + T_memory[i+1]; // предыдущие

            // обновление памяти
            T_txt.Write ((lor[0]+1) + "\r\n" + estimation); T_txt.Close (); // обновление вр. памяти
        }
        public void Memory_loadin (int a, int b) // загрузка памяти 
        {
            if (a == 1) // загрузка временной памяти
            {
                DATA = File.ReadAllText (T_way).Split (sep_1,StringSplitOptions.RemoveEmptyEntries);

                if (DATA.Length == 0) // проверка на отсуствие данных в файле
                {
                    DATA = new string[1];
                    DATA[0] = "0";
                }

                T_memory.Clear (); T_memory.AddRange (DATA); 
                lor[0] = Convert.ToInt32 (T_memory[0]);

                T_txt = new StreamWriter (T_way); // подготовка к записи
            }

            if (b != 0) // загрузка основной памяти
            {
                DATA = File.ReadAllText (M_way).Split (sep_1,StringSplitOptions.RemoveEmptyEntries);

                if (DATA.Length == 0) // проверка на отсуствие данных в файле
                {
                    DATA = new string [3]; 
                    DATA[0] = "0"; DATA[1] = "-0-cros-1-"; DATA[2] = "-0-zero-2-";
                }

                M_memory.Clear (); M_memory.AddRange (DATA);
                // оценка размера массивов
                lor[1] = Convert.ToInt32 (M_memory[1].Split ('-')[1]);
                lor[2] = Convert.ToInt32 (M_memory[2+lor[1]].Split ('-')[1]);

                C_memory.Clear (); Z_memory.Clear ();

                // распределение памяти по отдельным массивам
                for (int i = 0; i<lor[1]; i++) C_memory.Add (M_memory[2+i]); // крестики
                for (int i = 0; i<lor[2]; i++) Z_memory.Add (M_memory[3+i+lor[1]]); // нолики

                if (b == 1) M_txt = new StreamWriter (M_way); // подготовка к записи
            }
            
        }
        public void Memory_update () // обновление памяти 
        {
            // обновление
            Memory_loadin (1,1); // выгрузка всей памяти
            for (int i = 1; i<lor[0]+1; i++)
            { // определение победителя в партии и внесение изменений в основную память
                if (T_memory[i].Split ('.')[1] == "1") Memory_compare (i,C_memory,Z_memory,1,2); // Крестик
                if (T_memory[i].Split ('.')[1] == "2") Memory_compare (i,Z_memory,C_memory,2,1); // Нолик
            }

            Memory_sort (C_memory); // сортировка памяти крестика
            Memory_sort (Z_memory); // сортировка памяти нолика

            // запись обновленной памяти
            estimation += (lor[1]+lor[2]); // общий размер памяти
            // память крестика
            estimation += "\r\n-"+lor[1]+"-cros-1-";
            for (int i = 0; i<lor[1]; i++) estimation += "\r\n"+C_memory[i];
            // память нолика
            estimation += "\r\n-"+lor[2]+"-zero-2-";
            for (int i = 0; i<lor[2]; i++) estimation += "\r\n"+Z_memory[i];

            T_txt.Write ("0"); T_txt.Close ();        // обновление вр. памяти
            M_txt.Write (estimation); M_txt.Close (); // обновление ос. памяти

            estimation = "";
            reason.Clear ();

            Memory_loadin (0,2);
        }
        public void Memory_compare (int i,List<string> W,List<string> L, int W_S, int L_S) // сравнение участков памяти 
        { // i - переменная из внешнего цикла, W - лист победителя (W_S - его размер), L - лист проигравшего (L_S - его размер) 
            for (int j = 0; ; j++) // опрос победителя
            {
                if (j == lor[W_S]) // добавление, если не найдено
                {
                    W.Add (T_memory[i].Split ('.')[0] + ".10"); lor[W_S]++;
                    break;
                }
                if (W[j].Split ('.')[0] == T_memory[i].Split ('.')[0]) // увеличение рейтинга, если найдено (+)
                {
                    W[j] = Convert.ToString (W[j].Split ('.')[0] + '.' + (Convert.ToInt32 (W[j].Split ('.')[1])+1));
                    break;
                }
            }

            for (int j = 0; j < lor[L_S]; j++) // опрос проигравшего -- вероятнее всего это часть уже не актуальна
            {
                if (L[j].Split ('.')[0] == T_memory[i].Split ('.')[0]) // уменьшение рейтинга, если найдено (-)
                {
                    L[j] = Convert.ToString (Convert.ToInt32 ( L[j].Split ('.')[1] + '.' + L[j].Split ('.')[0])-1); // если будет ругаться, значит сработала
                    break;
                }
            }
        }
        public void Memory_sort (List<string> main_memory) // вылезала одна ошибка (выход за массив индексов) 
        {
            List<string> extra_memory = new List<string> (); // дополнительная промежуточная память
            int max, max_ind; // максимальное значение и его индекс

            extra_memory.AddRange (main_memory); main_memory.Clear ();

            for (;extra_memory.Count != 0;)
            {
                max = Convert.ToInt32 (extra_memory[0].Split ('.')[1]); // начальное значение для сортировки
                max_ind = 0;

                for (int i = 0; i < extra_memory.Count;i++) // Сортировка
                {
                    if (Convert.ToInt32 (extra_memory[i].Split ('.')[1]) > max)
                    {
                        max = Convert.ToInt32 (extra_memory[i].Split ('.')[1]);
                        max_ind = i;
                    }
                }

                main_memory.Add (extra_memory[max_ind]); // внесение максимального элемента
                extra_memory.RemoveAt (max_ind);         // удаление найденного максимального элемента
            }
        }

        //----------------------------  Интеллект компьютера  ---------------------------

        public void Set_load () // найстройка интелекта 
        {
            DATA = File.ReadAllText (asset_way).Split (sep_1,StringSplitOptions.RemoveEmptyEntries);
            first_move_check.Checked = Convert.ToBoolean (DATA[0].Split ('=')[1]); // первый ход у крестика
            randon_check.Checked = Convert.ToBoolean (DATA[1].Split ('=')[1]); // рандом
            autoupd_check.Checked = Convert.ToBoolean (DATA[2].Split ('=')[1]); // автообновление
        }
        public void Oponent () // выбор интелекта для хода 
        {
            if (randon_check.Checked) Random_strike ();
            else Memory_strike ();
        }
        public void Random_strike () // рандом 
        {
            if (Game[2] != 2)
            {
                for (int i; ;)
                {
                    i = rand.Next (1,10);

                    if (field[i] == 0)
                    {
                        Game_record (i,Game[1],2);
                        break;
                    }
                }
            }
        }
        public void Memory_strike () // память 
        {
            int R = 0, // прошлый ход
                D = 0; // текущий ход
            string S = ""; // записанная партия

            if (Game[2] != 2) // проверка на конец игры
            {
                for (int i=0; ; i++)
                {
                    if (reason.Count == 0) { Random_strike (); break; }

                    S = reason[i].Split ('.')[0];

                    if (Game[4] == 1 & Game[1] == 1) // первый ход для крестика
                    {
                        if (first_move_check.Checked) D = rand.Next (1,10); // рандом
                        else D = Convert.ToInt32 (S.Split ('-')[1]); // память

                        Game_record (D,Game[1],2);
                        int B; // первый ход из других партий
                        for (int j = reason.Count-1; 0 < j; j--) // предварительная фильтрация ходов
                        {
                            B = Convert.ToInt32 (reason[j].Split ('.')[0].Split ('-')[1]);
                            if (B != D) reason.RemoveAt (j); // удаление неподходящих партий
                        }
                        break;
                    }

                    D = Convert.ToInt32 (S.Split ('-')[Game[4]-1]);
                    R = Convert.ToInt32 (S.Split ('-')[Game[4]]);

                    if (field[D] == Game[0] && field[R] == 0 & R != 0)
                    {
                        Game_record (R,Game[1],2);
                        break;
                    }
                    else { if (reason.Count != 0) { reason.RemoveAt (i); i--; } }
                }
                label1.Text = "запас ходов у опонента - " + reason.Count;
            }
        }
    }
}


/* Аннатация
    
   ----- Что есть -----
   1. Элементы управления

        - Файлы - 4 -
        временная память
        основная память
        настройи интелекта
        изображение поле после сброса

        - Кнопки - 6 -
        сброса игры
        обновление памяти
        дополнительная панель интелекта (2 кнопки - открыть - закрыть)
        вызов файла временной памяти
        вызов файла постоянной памяти

        - Галочки - 3 - 
        первый ход для крестика
        автоматическое обновление памяти
        рандомные ходы

        - Изображения - 2 -
        игровое поле
        победитель

        - Внутренние ресурсы - 4 -
        картинки с клетками - пустая - с крестиком - с ноликом
        черный фон поля

   2. Интелект 
       
        - Рандомный ход -
        Обычный генератор случайных чисел. Создаёться число, если клетка свободна то, туда ставиться фигура

        - Ход по памяти -
        Расписать 

   3. 

   ----- Что нужно -----
   1. Автоматическое обучение и игра с самим собой (в будующем игра с другим вариантом).
   2. Отрегулировать взаимодействие с памятью (добавить убавление рейтинга при проигрыше, желательно больше чем прибавление при победе).

   ----- Что можно -----
   1. Эмоции для ИИ
   2. Панель с настройками эмоций
   3. Панель разговора с ИИ и панель с ответами (грубо говоря обратная связь с ИИ)
   4. Расширить до других игр и проектов
 */
