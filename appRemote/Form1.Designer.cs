namespace appRemote
{
    partial class Form1
    {
        /// <summary>
        ///  Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        ///  Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        ///  Required method for Designer support - do not modify
        ///  the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            textExtendedIFMode = new TextBox();
            buttonConnect = new Button();
            buttonEnable = new Button();
            textConnected = new TextBox();
            picPcRemote1 = new PictureBox();
            picPcRemote2 = new PictureBox();
            label1 = new Label();
            label2 = new Label();
            label3 = new Label();
            picCameras = new PictureBox();
            label4 = new Label();
            ((System.ComponentModel.ISupportInitialize)picPcRemote1).BeginInit();
            ((System.ComponentModel.ISupportInitialize)picPcRemote2).BeginInit();
            ((System.ComponentModel.ISupportInitialize)picCameras).BeginInit();
            SuspendLayout();
            // 
            // textExtendedIFMode
            // 
            textExtendedIFMode.Location = new Point(215, 404);
            textExtendedIFMode.Name = "textExtendedIFMode";
            textExtendedIFMode.ReadOnly = true;
            textExtendedIFMode.Size = new Size(131, 27);
            textExtendedIFMode.TabIndex = 0;
            textExtendedIFMode.TabStop = false;
            // 
            // buttonConnect
            // 
            buttonConnect.Location = new Point(12, 362);
            buttonConnect.Name = "buttonConnect";
            buttonConnect.Size = new Size(185, 33);
            buttonConnect.TabIndex = 0;
            buttonConnect.Text = "Connect";
            buttonConnect.TextAlign = ContentAlignment.MiddleLeft;
            buttonConnect.UseVisualStyleBackColor = true;
            buttonConnect.Click += buttonConnect_Click;
            // 
            // buttonEnable
            // 
            buttonEnable.Enabled = false;
            buttonEnable.Location = new Point(12, 401);
            buttonEnable.Name = "buttonEnable";
            buttonEnable.Size = new Size(185, 33);
            buttonEnable.TabIndex = 1;
            buttonEnable.Text = "Enable USB-C Remote";
            buttonEnable.TextAlign = ContentAlignment.MiddleLeft;
            buttonEnable.UseVisualStyleBackColor = true;
            buttonEnable.Click += buttonEnable_Click;
            // 
            // textConnected
            // 
            textConnected.Location = new Point(215, 365);
            textConnected.Name = "textConnected";
            textConnected.ReadOnly = true;
            textConnected.Size = new Size(131, 27);
            textConnected.TabIndex = 2;
            textConnected.TabStop = false;
            // 
            // picPcRemote1
            // 
            picPcRemote1.Image = EnableUSBC_Remote.Properties.Resources.pcremote_j1;
            picPcRemote1.Location = new Point(12, 76);
            picPcRemote1.Name = "picPcRemote1";
            picPcRemote1.Size = new Size(360, 240);
            picPcRemote1.SizeMode = PictureBoxSizeMode.Zoom;
            picPcRemote1.TabIndex = 3;
            picPcRemote1.TabStop = false;
            // 
            // picPcRemote2
            // 
            picPcRemote2.Image = EnableUSBC_Remote.Properties.Resources.pcremote_j2;
            picPcRemote2.Location = new Point(381, 76);
            picPcRemote2.Name = "picPcRemote2";
            picPcRemote2.Size = new Size(360, 240);
            picPcRemote2.SizeMode = PictureBoxSizeMode.StretchImage;
            picPcRemote2.TabIndex = 4;
            picPcRemote2.TabStop = false;
            // 
            // label1
            // 
            label1.AutoSize = true;
            label1.Location = new Point(12, 9);
            label1.Name = "label1";
            label1.Size = new Size(647, 60);
            label1.TabIndex = 5;
            label1.Text = "1. カメラ本体のMENUを操作して、[セットアップ] - [USB] - [USB接続モード] - [PCリモート] を選択して下さい。\r\n- USBCリモコンを使うときは [PCリモート] を設定\r\n- PCとカメラでファイル転送するときは [MSC] 又は [MTP] を設定";
            // 
            // label2
            // 
            label2.AutoSize = true;
            label2.Location = new Point(12, 330);
            label2.Name = "label2";
            label2.Size = new Size(570, 20);
            label2.TabIndex = 6;
            label2.Text = "2. カメラをパソコンにつないで、下記のボタンを順に押してください。OK! が表示されれば完了です。";
            // 
            // label3
            // 
            label3.AutoSize = true;
            label3.Location = new Point(12, 472);
            label3.Name = "label3";
            label3.Size = new Size(474, 60);
            label3.TabIndex = 7;
            label3.Text = "3. USB-Cリモコンをカメラに接続し、シャッター操作ができることを確認して下さい。\r\n※リモコンのUSB-Cコネクタには表(ポッチあり) と裏(ポッチなし) があります。\r\nα7CRとα7CM2 - ポッチが上、α6700 - ポッチが下、E10M2 - ポッチが右";
            // 
            // picCameras
            // 
            picCameras.Image = EnableUSBC_Remote.Properties.Resources.cameras;
            picCameras.Location = new Point(12, 546);
            picCameras.Name = "picCameras";
            picCameras.Size = new Size(800, 243);
            picCameras.SizeMode = PictureBoxSizeMode.StretchImage;
            picCameras.TabIndex = 8;
            picCameras.TabStop = false;
            // 
            // label4
            // 
            label4.AutoSize = true;
            label4.Location = new Point(12, 437);
            label4.Name = "label4";
            label4.Size = new Size(541, 20);
            label4.TabIndex = 6;
            label4.Text = "この操作でUSB-Cリモコンを有効にします。この設定は工場出荷設定するまで保存されます。";
            // 
            // Form1
            // 
            AutoScaleDimensions = new SizeF(8F, 20F);
            AutoScaleMode = AutoScaleMode.Font;
            ClientSize = new Size(841, 801);
            Controls.Add(picCameras);
            Controls.Add(label3);
            Controls.Add(label4);
            Controls.Add(label2);
            Controls.Add(label1);
            Controls.Add(picPcRemote2);
            Controls.Add(picPcRemote1);
            Controls.Add(textConnected);
            Controls.Add(buttonEnable);
            Controls.Add(buttonConnect);
            Controls.Add(textExtendedIFMode);
            Name = "Form1";
            Text = "Enable USB-C Remote 1.00";
            FormClosing += Form1_FormClosing;
            ((System.ComponentModel.ISupportInitialize)picPcRemote1).EndInit();
            ((System.ComponentModel.ISupportInitialize)picPcRemote2).EndInit();
            ((System.ComponentModel.ISupportInitialize)picCameras).EndInit();
            ResumeLayout(false);
            PerformLayout();
        }

        #endregion

        private TextBox textExtendedIFMode;
        private Button buttonConnect;
        private Button buttonEnable;
        private TextBox textConnected;
        private PictureBox picPcRemote1;
        private PictureBox picPcRemote2;
        private Label label1;
        private Label label2;
        private Label label3;
        private PictureBox picCameras;
        private Label label4;
    }
}
