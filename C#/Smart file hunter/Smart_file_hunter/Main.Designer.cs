namespace Smart_file_hunter
{
    partial class Form_Main
    {
        /// <summary>
        /// Обязательная переменная конструктора.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Освободить все используемые ресурсы.
        /// </summary>
        /// <param name="disposing">истинно, если управляемый ресурс должен быть удален; иначе ложно.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Код, автоматически созданный конструктором форм Windows

        /// <summary>
        /// Требуемый метод для поддержки конструктора — не изменяйте 
        /// содержимое этого метода с помощью редактора кода.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form_Main));
            this.b_startSearch = new System.Windows.Forms.Button();
            this.pattern_group = new System.Windows.Forms.GroupBox();
            this.b_abortSearch = new System.Windows.Forms.Button();
            this.b_browserDir = new System.Windows.Forms.Button();
            this.b_downTextPattern = new System.Windows.Forms.Button();
            this.b_upTextPattern = new System.Windows.Forms.Button();
            this.b_delTextPattern = new System.Windows.Forms.Button();
            this.textPattern_box = new System.Windows.Forms.ListBox();
            this.b_addTextPattern = new System.Windows.Forms.Button();
            this.b_downNamePattern = new System.Windows.Forms.Button();
            this.b_upNamePattern = new System.Windows.Forms.Button();
            this.b_delNamePattern = new System.Windows.Forms.Button();
            this.b_addNamePattern = new System.Windows.Forms.Button();
            this.namePattern_box = new System.Windows.Forms.ListBox();
            this.tb_textPattern = new System.Windows.Forms.TextBox();
            this.lb_03 = new System.Windows.Forms.Label();
            this.tb_namePattern = new System.Windows.Forms.TextBox();
            this.lb_02 = new System.Windows.Forms.Label();
            this.tb_startDir = new System.Windows.Forms.TextBox();
            this.lb_01 = new System.Windows.Forms.Label();
            this.find_group = new System.Windows.Forms.GroupBox();
            this.lb_filesFind = new System.Windows.Forms.Label();
            this.lb_07 = new System.Windows.Forms.Label();
            this.tv_findItem = new System.Windows.Forms.TreeView();
            this.icons = new System.Windows.Forms.ImageList(this.components);
            this.search_group = new System.Windows.Forms.GroupBox();
            this.tb_readir = new System.Windows.Forms.TextBox();
            this.lb_08 = new System.Windows.Forms.Label();
            this.lb_06 = new System.Windows.Forms.Label();
            this.lb_fileAll = new System.Windows.Forms.Label();
            this.lb_fileLast = new System.Windows.Forms.Label();
            this.lb_timePass = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.lb_05 = new System.Windows.Forms.Label();
            this.b_continueSearch = new System.Windows.Forms.Button();
            this.b_stopSearch = new System.Windows.Forms.Button();
            this.lb_04 = new System.Windows.Forms.Label();
            this.pb_progressSearch = new System.Windows.Forms.ProgressBar();
            this.helpTool = new System.Windows.Forms.ToolTip(this.components);
            this.dirSelecter = new System.Windows.Forms.FolderBrowserDialog();
            this.time = new System.Windows.Forms.Timer(this.components);
            this.pattern_group.SuspendLayout();
            this.find_group.SuspendLayout();
            this.search_group.SuspendLayout();
            this.SuspendLayout();
            // 
            // b_startSearch
            // 
            this.b_startSearch.Location = new System.Drawing.Point(120, 339);
            this.b_startSearch.Name = "b_startSearch";
            this.b_startSearch.Size = new System.Drawing.Size(75, 23);
            this.b_startSearch.TabIndex = 14;
            this.b_startSearch.Text = "Найти";
            this.b_startSearch.UseVisualStyleBackColor = true;
            this.b_startSearch.Click += new System.EventHandler(this.Search_Start);
            // 
            // pattern_group
            // 
            this.pattern_group.Controls.Add(this.b_abortSearch);
            this.pattern_group.Controls.Add(this.b_browserDir);
            this.pattern_group.Controls.Add(this.b_downTextPattern);
            this.pattern_group.Controls.Add(this.b_upTextPattern);
            this.pattern_group.Controls.Add(this.b_startSearch);
            this.pattern_group.Controls.Add(this.b_delTextPattern);
            this.pattern_group.Controls.Add(this.textPattern_box);
            this.pattern_group.Controls.Add(this.b_addTextPattern);
            this.pattern_group.Controls.Add(this.b_downNamePattern);
            this.pattern_group.Controls.Add(this.b_upNamePattern);
            this.pattern_group.Controls.Add(this.b_delNamePattern);
            this.pattern_group.Controls.Add(this.b_addNamePattern);
            this.pattern_group.Controls.Add(this.namePattern_box);
            this.pattern_group.Controls.Add(this.tb_textPattern);
            this.pattern_group.Controls.Add(this.lb_03);
            this.pattern_group.Controls.Add(this.tb_namePattern);
            this.pattern_group.Controls.Add(this.lb_02);
            this.pattern_group.Controls.Add(this.tb_startDir);
            this.pattern_group.Controls.Add(this.lb_01);
            this.pattern_group.Location = new System.Drawing.Point(12, 12);
            this.pattern_group.Name = "pattern_group";
            this.pattern_group.Size = new System.Drawing.Size(200, 371);
            this.pattern_group.TabIndex = 1;
            this.pattern_group.TabStop = false;
            this.pattern_group.Text = "Критерии поиска";
            // 
            // b_abortSearch
            // 
            this.b_abortSearch.Location = new System.Drawing.Point(120, 339);
            this.b_abortSearch.Name = "b_abortSearch";
            this.b_abortSearch.Size = new System.Drawing.Size(75, 23);
            this.b_abortSearch.TabIndex = 16;
            this.b_abortSearch.Text = "Стоп";
            this.b_abortSearch.UseVisualStyleBackColor = true;
            this.b_abortSearch.Click += new System.EventHandler(this.Search_Abort);
            // 
            // b_browserDir
            // 
            this.b_browserDir.Location = new System.Drawing.Point(172, 30);
            this.b_browserDir.Name = "b_browserDir";
            this.b_browserDir.Size = new System.Drawing.Size(23, 23);
            this.b_browserDir.TabIndex = 2;
            this.b_browserDir.Text = "..";
            this.b_browserDir.UseVisualStyleBackColor = true;
            this.b_browserDir.Click += new System.EventHandler(this.Open_FileDialog);
            // 
            // b_downTextPattern
            // 
            this.b_downTextPattern.BackColor = System.Drawing.SystemColors.Control;
            this.b_downTextPattern.BackgroundImage = global::Smart_Search.Properties.Resources.Вниз;
            this.b_downTextPattern.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.b_downTextPattern.Cursor = System.Windows.Forms.Cursors.PanSouth;
            this.b_downTextPattern.Location = new System.Drawing.Point(166, 304);
            this.b_downTextPattern.Name = "b_downTextPattern";
            this.b_downTextPattern.Size = new System.Drawing.Size(25, 28);
            this.b_downTextPattern.TabIndex = 13;
            this.b_downTextPattern.UseVisualStyleBackColor = false;
            this.b_downTextPattern.Click += new System.EventHandler(this.DownOrder_TextPattern);
            // 
            // b_upTextPattern
            // 
            this.b_upTextPattern.BackgroundImage = global::Smart_Search.Properties.Resources.Вверх;
            this.b_upTextPattern.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.b_upTextPattern.Cursor = System.Windows.Forms.Cursors.PanNorth;
            this.b_upTextPattern.Location = new System.Drawing.Point(166, 238);
            this.b_upTextPattern.Name = "b_upTextPattern";
            this.b_upTextPattern.Size = new System.Drawing.Size(25, 28);
            this.b_upTextPattern.TabIndex = 11;
            this.b_upTextPattern.TextImageRelation = System.Windows.Forms.TextImageRelation.TextBeforeImage;
            this.b_upTextPattern.UseVisualStyleBackColor = true;
            this.b_upTextPattern.Click += new System.EventHandler(this.UpOrder_TextPattern);
            // 
            // b_delTextPattern
            // 
            this.b_delTextPattern.BackgroundImage = global::Smart_Search.Properties.Resources.Удаление;
            this.b_delTextPattern.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.b_delTextPattern.Location = new System.Drawing.Point(166, 273);
            this.b_delTextPattern.Name = "b_delTextPattern";
            this.b_delTextPattern.Size = new System.Drawing.Size(25, 25);
            this.b_delTextPattern.TabIndex = 12;
            this.b_delTextPattern.UseVisualStyleBackColor = true;
            this.b_delTextPattern.Click += new System.EventHandler(this.DelOrder_TextPattern);
            // 
            // textPattern_box
            // 
            this.textPattern_box.FormattingEnabled = true;
            this.textPattern_box.HorizontalScrollbar = true;
            this.textPattern_box.Location = new System.Drawing.Point(9, 238);
            this.textPattern_box.Name = "textPattern_box";
            this.textPattern_box.Size = new System.Drawing.Size(151, 95);
            this.textPattern_box.TabIndex = 10;
            // 
            // b_addTextPattern
            // 
            this.b_addTextPattern.Location = new System.Drawing.Point(124, 209);
            this.b_addTextPattern.Name = "b_addTextPattern";
            this.b_addTextPattern.Size = new System.Drawing.Size(70, 23);
            this.b_addTextPattern.TabIndex = 10;
            this.b_addTextPattern.Text = "Добавить";
            this.b_addTextPattern.UseVisualStyleBackColor = true;
            this.b_addTextPattern.Click += new System.EventHandler(this.Add_TextPattern);
            // 
            // b_downNamePattern
            // 
            this.b_downNamePattern.BackColor = System.Drawing.SystemColors.Control;
            this.b_downNamePattern.BackgroundImage = global::Smart_Search.Properties.Resources.Вниз;
            this.b_downNamePattern.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.b_downNamePattern.Cursor = System.Windows.Forms.Cursors.PanSouth;
            this.b_downNamePattern.Location = new System.Drawing.Point(169, 163);
            this.b_downNamePattern.Name = "b_downNamePattern";
            this.b_downNamePattern.Size = new System.Drawing.Size(25, 28);
            this.b_downNamePattern.TabIndex = 8;
            this.b_downNamePattern.UseVisualStyleBackColor = false;
            this.b_downNamePattern.Click += new System.EventHandler(this.DownOrder_NamePattern);
            // 
            // b_upNamePattern
            // 
            this.b_upNamePattern.BackgroundImage = global::Smart_Search.Properties.Resources.Вверх;
            this.b_upNamePattern.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.b_upNamePattern.Cursor = System.Windows.Forms.Cursors.PanNorth;
            this.b_upNamePattern.Location = new System.Drawing.Point(169, 97);
            this.b_upNamePattern.Name = "b_upNamePattern";
            this.b_upNamePattern.Size = new System.Drawing.Size(25, 28);
            this.b_upNamePattern.TabIndex = 6;
            this.b_upNamePattern.TextImageRelation = System.Windows.Forms.TextImageRelation.TextBeforeImage;
            this.b_upNamePattern.UseVisualStyleBackColor = true;
            this.b_upNamePattern.Click += new System.EventHandler(this.UpOrder_NamePattern);
            // 
            // b_delNamePattern
            // 
            this.b_delNamePattern.BackgroundImage = global::Smart_Search.Properties.Resources.Удаление;
            this.b_delNamePattern.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.b_delNamePattern.Location = new System.Drawing.Point(169, 132);
            this.b_delNamePattern.Name = "b_delNamePattern";
            this.b_delNamePattern.Size = new System.Drawing.Size(25, 25);
            this.b_delNamePattern.TabIndex = 7;
            this.b_delNamePattern.UseVisualStyleBackColor = true;
            this.b_delNamePattern.Click += new System.EventHandler(this.DelOrder_NamePattern);
            // 
            // b_addNamePattern
            // 
            this.b_addNamePattern.Location = new System.Drawing.Point(125, 69);
            this.b_addNamePattern.Name = "b_addNamePattern";
            this.b_addNamePattern.Size = new System.Drawing.Size(70, 23);
            this.b_addNamePattern.TabIndex = 4;
            this.b_addNamePattern.Text = "Добавить";
            this.b_addNamePattern.UseVisualStyleBackColor = true;
            this.b_addNamePattern.Click += new System.EventHandler(this.Add_NamePattern);
            // 
            // namePattern_box
            // 
            this.namePattern_box.FormattingEnabled = true;
            this.namePattern_box.HorizontalScrollbar = true;
            this.namePattern_box.Location = new System.Drawing.Point(9, 97);
            this.namePattern_box.Name = "namePattern_box";
            this.namePattern_box.Size = new System.Drawing.Size(154, 95);
            this.namePattern_box.TabIndex = 2;
            // 
            // tb_textPattern
            // 
            this.tb_textPattern.Location = new System.Drawing.Point(9, 211);
            this.tb_textPattern.Name = "tb_textPattern";
            this.tb_textPattern.Size = new System.Drawing.Size(110, 20);
            this.tb_textPattern.TabIndex = 9;
            // 
            // lb_03
            // 
            this.lb_03.AutoSize = true;
            this.lb_03.Location = new System.Drawing.Point(6, 195);
            this.lb_03.Name = "lb_03";
            this.lb_03.Size = new System.Drawing.Size(81, 13);
            this.lb_03.TabIndex = 4;
            this.lb_03.Text = "Текст в файле";
            // 
            // tb_namePattern
            // 
            this.tb_namePattern.Location = new System.Drawing.Point(9, 71);
            this.tb_namePattern.Name = "tb_namePattern";
            this.tb_namePattern.Size = new System.Drawing.Size(110, 20);
            this.tb_namePattern.TabIndex = 3;
            // 
            // lb_02
            // 
            this.lb_02.AutoSize = true;
            this.lb_02.Location = new System.Drawing.Point(6, 55);
            this.lb_02.Name = "lb_02";
            this.lb_02.Size = new System.Drawing.Size(89, 13);
            this.lb_02.TabIndex = 2;
            this.lb_02.Text = "Шаблоны имени";
            // 
            // tb_startDir
            // 
            this.tb_startDir.Location = new System.Drawing.Point(9, 32);
            this.tb_startDir.Name = "tb_startDir";
            this.tb_startDir.Size = new System.Drawing.Size(157, 20);
            this.tb_startDir.TabIndex = 1;
            this.tb_startDir.TextChanged += new System.EventHandler(this.StartDir_TextChanged);
            // 
            // lb_01
            // 
            this.lb_01.AutoSize = true;
            this.lb_01.Location = new System.Drawing.Point(6, 16);
            this.lb_01.Name = "lb_01";
            this.lb_01.Size = new System.Drawing.Size(122, 13);
            this.lb_01.TabIndex = 0;
            this.lb_01.Text = "Стартовая директория";
            // 
            // find_group
            // 
            this.find_group.Controls.Add(this.lb_filesFind);
            this.find_group.Controls.Add(this.lb_07);
            this.find_group.Controls.Add(this.tv_findItem);
            this.find_group.Location = new System.Drawing.Point(219, 12);
            this.find_group.Name = "find_group";
            this.find_group.Size = new System.Drawing.Size(278, 371);
            this.find_group.TabIndex = 4;
            this.find_group.TabStop = false;
            this.find_group.Text = "Нашлось";
            // 
            // lb_filesFind
            // 
            this.lb_filesFind.AutoSize = true;
            this.lb_filesFind.Location = new System.Drawing.Point(108, 344);
            this.lb_filesFind.Name = "lb_filesFind";
            this.lb_filesFind.Size = new System.Drawing.Size(31, 13);
            this.lb_filesFind.TabIndex = 19;
            this.lb_filesFind.Text = "9999";
            // 
            // lb_07
            // 
            this.lb_07.AutoSize = true;
            this.lb_07.Location = new System.Drawing.Point(6, 344);
            this.lb_07.Name = "lb_07";
            this.lb_07.Size = new System.Drawing.Size(96, 13);
            this.lb_07.TabIndex = 18;
            this.lb_07.Text = "Файлов найдено:";
            // 
            // tv_findItem
            // 
            this.tv_findItem.ImageKey = "Файл.ico";
            this.tv_findItem.ImageList = this.icons;
            this.tv_findItem.Location = new System.Drawing.Point(6, 19);
            this.tv_findItem.Name = "tv_findItem";
            this.tv_findItem.SelectedImageKey = "Файл.ico";
            this.tv_findItem.Size = new System.Drawing.Size(266, 314);
            this.tv_findItem.TabIndex = 17;
            // 
            // icons
            // 
            this.icons.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("icons.ImageStream")));
            this.icons.TransparentColor = System.Drawing.Color.Transparent;
            this.icons.Images.SetKeyName(0, "Диск.ico");
            this.icons.Images.SetKeyName(1, "Папка.ico");
            this.icons.Images.SetKeyName(2, "Файл.ico");
            this.icons.Images.SetKeyName(3, "Папка (открытая).ico");
            // 
            // search_group
            // 
            this.search_group.Controls.Add(this.tb_readir);
            this.search_group.Controls.Add(this.lb_08);
            this.search_group.Controls.Add(this.lb_06);
            this.search_group.Controls.Add(this.lb_fileAll);
            this.search_group.Controls.Add(this.lb_fileLast);
            this.search_group.Controls.Add(this.lb_timePass);
            this.search_group.Controls.Add(this.label2);
            this.search_group.Controls.Add(this.lb_05);
            this.search_group.Controls.Add(this.b_continueSearch);
            this.search_group.Controls.Add(this.b_stopSearch);
            this.search_group.Controls.Add(this.lb_04);
            this.search_group.Controls.Add(this.pb_progressSearch);
            this.search_group.Location = new System.Drawing.Point(12, 391);
            this.search_group.Name = "search_group";
            this.search_group.Size = new System.Drawing.Size(485, 108);
            this.search_group.TabIndex = 5;
            this.search_group.TabStop = false;
            this.search_group.Text = "Поиск";
            // 
            // tb_readir
            // 
            this.tb_readir.Location = new System.Drawing.Point(209, 32);
            this.tb_readir.Multiline = true;
            this.tb_readir.Name = "tb_readir";
            this.tb_readir.Size = new System.Drawing.Size(270, 70);
            this.tb_readir.TabIndex = 29;
            // 
            // lb_08
            // 
            this.lb_08.AutoSize = true;
            this.lb_08.ForeColor = System.Drawing.SystemColors.ControlText;
            this.lb_08.Location = new System.Drawing.Point(280, 16);
            this.lb_08.Name = "lb_08";
            this.lb_08.Size = new System.Drawing.Size(114, 13);
            this.lb_08.TabIndex = 27;
            this.lb_08.Text = "Текущая директория";
            // 
            // lb_06
            // 
            this.lb_06.Location = new System.Drawing.Point(39, 58);
            this.lb_06.Name = "lb_06";
            this.lb_06.Size = new System.Drawing.Size(23, 16);
            this.lb_06.TabIndex = 26;
            this.lb_06.Text = "/";
            this.lb_06.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // lb_fileAll
            // 
            this.lb_fileAll.AutoSize = true;
            this.lb_fileAll.Font = new System.Drawing.Font("Microsoft Sans Serif", 6.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.lb_fileAll.Location = new System.Drawing.Point(60, 60);
            this.lb_fileAll.Name = "lb_fileAll";
            this.lb_fileAll.Size = new System.Drawing.Size(25, 12);
            this.lb_fileAll.TabIndex = 25;
            this.lb_fileAll.Text = "9000";
            this.lb_fileAll.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // lb_fileLast
            // 
            this.lb_fileLast.AutoSize = true;
            this.lb_fileLast.Font = new System.Drawing.Font("Microsoft Sans Serif", 6.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.lb_fileLast.Location = new System.Drawing.Point(5, 60);
            this.lb_fileLast.Name = "lb_fileLast";
            this.lb_fileLast.Size = new System.Drawing.Size(25, 12);
            this.lb_fileLast.TabIndex = 24;
            this.lb_fileLast.Text = "4500";
            this.lb_fileLast.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // lb_timePass
            // 
            this.lb_timePass.AutoEllipsis = true;
            this.lb_timePass.Location = new System.Drawing.Point(104, 58);
            this.lb_timePass.Name = "lb_timePass";
            this.lb_timePass.Size = new System.Drawing.Size(96, 16);
            this.lb_timePass.TabIndex = 23;
            this.lb_timePass.Text = "2 ч 20 м 10 с";
            this.lb_timePass.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(105, 58);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(0, 13);
            this.label2.TabIndex = 22;
            // 
            // lb_05
            // 
            this.lb_05.Location = new System.Drawing.Point(101, 45);
            this.lb_05.Name = "lb_05";
            this.lb_05.Size = new System.Drawing.Size(99, 13);
            this.lb_05.TabIndex = 21;
            this.lb_05.Text = "Времени прошло";
            this.lb_05.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // b_continueSearch
            // 
            this.b_continueSearch.Location = new System.Drawing.Point(115, 19);
            this.b_continueSearch.Name = "b_continueSearch";
            this.b_continueSearch.Size = new System.Drawing.Size(85, 23);
            this.b_continueSearch.TabIndex = 16;
            this.b_continueSearch.Text = "Продолжить";
            this.b_continueSearch.UseVisualStyleBackColor = true;
            this.b_continueSearch.Click += new System.EventHandler(this.Search_Continue);
            // 
            // b_stopSearch
            // 
            this.b_stopSearch.Location = new System.Drawing.Point(9, 19);
            this.b_stopSearch.Name = "b_stopSearch";
            this.b_stopSearch.Size = new System.Drawing.Size(85, 23);
            this.b_stopSearch.TabIndex = 15;
            this.b_stopSearch.Text = "Остановить";
            this.b_stopSearch.UseVisualStyleBackColor = true;
            this.b_stopSearch.Click += new System.EventHandler(this.Search_Stop);
            // 
            // lb_04
            // 
            this.lb_04.Location = new System.Drawing.Point(9, 45);
            this.lb_04.Name = "lb_04";
            this.lb_04.Size = new System.Drawing.Size(85, 13);
            this.lb_04.TabIndex = 2;
            this.lb_04.Text = "Прогресс";
            this.lb_04.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // pb_progressSearch
            // 
            this.pb_progressSearch.Location = new System.Drawing.Point(9, 79);
            this.pb_progressSearch.Name = "pb_progressSearch";
            this.pb_progressSearch.Size = new System.Drawing.Size(191, 23);
            this.pb_progressSearch.TabIndex = 0;
            this.pb_progressSearch.Value = 50;
            // 
            // dirSelecter
            // 
            this.dirSelecter.Description = "Выберете стартовую директорию поиска.";
            this.dirSelecter.RootFolder = System.Environment.SpecialFolder.MyComputer;
            // 
            // time
            // 
            this.time.Interval = 1000;
            this.time.Tick += new System.EventHandler(this.Time_Tick);
            // 
            // Form_Main
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(509, 511);
            this.Controls.Add(this.search_group);
            this.Controls.Add(this.find_group);
            this.Controls.Add(this.pattern_group);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.Fixed3D;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MaximizeBox = false;
            this.Name = "Form_Main";
            this.Text = "Smart file hunter";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Form_Closing);
            this.Load += new System.EventHandler(this.Form_Load);
            this.pattern_group.ResumeLayout(false);
            this.pattern_group.PerformLayout();
            this.find_group.ResumeLayout(false);
            this.find_group.PerformLayout();
            this.search_group.ResumeLayout(false);
            this.search_group.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button b_startSearch;
        private System.Windows.Forms.GroupBox pattern_group;
        private System.Windows.Forms.TextBox tb_textPattern;
        private System.Windows.Forms.Label lb_03;
        private System.Windows.Forms.TextBox tb_namePattern;
        private System.Windows.Forms.Label lb_02;
        private System.Windows.Forms.TextBox tb_startDir;
        private System.Windows.Forms.Label lb_01;
        private System.Windows.Forms.ListBox namePattern_box;
        private System.Windows.Forms.Button b_addNamePattern;
        private System.Windows.Forms.GroupBox find_group;
        private System.Windows.Forms.Button b_downNamePattern;
        private System.Windows.Forms.Button b_upNamePattern;
        private System.Windows.Forms.Button b_delNamePattern;
        private System.Windows.Forms.Button b_downTextPattern;
        private System.Windows.Forms.Button b_upTextPattern;
        private System.Windows.Forms.Button b_delTextPattern;
        private System.Windows.Forms.ListBox textPattern_box;
        private System.Windows.Forms.Button b_addTextPattern;
        private System.Windows.Forms.GroupBox search_group;
        private System.Windows.Forms.ToolTip helpTool;
        private System.Windows.Forms.TreeView tv_findItem;
        private System.Windows.Forms.ImageList icons;
        private System.Windows.Forms.Button b_browserDir;
        private System.Windows.Forms.FolderBrowserDialog dirSelecter;
        private System.Windows.Forms.ProgressBar pb_progressSearch;
        private System.Windows.Forms.Label lb_04;
        private System.Windows.Forms.Button b_continueSearch;
        private System.Windows.Forms.Button b_stopSearch;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label lb_05;
        private System.Windows.Forms.Label lb_timePass;
        private System.Windows.Forms.Label lb_06;
        private System.Windows.Forms.Label lb_fileAll;
        private System.Windows.Forms.Label lb_fileLast;
        private System.Windows.Forms.Timer time;
        private System.Windows.Forms.Button b_abortSearch;
        private System.Windows.Forms.Label lb_08;
        private System.Windows.Forms.TextBox tb_readir;
        private System.Windows.Forms.Label lb_filesFind;
        private System.Windows.Forms.Label lb_07;
    }
}

