using System;
using System.Runtime.InteropServices;
using System.Xml.Linq;
using static System.Runtime.CompilerServices.RuntimeHelpers;
using static System.Runtime.InteropServices.JavaScript.JSType;

namespace appRemote
{
    public partial class Form1 : Form
    {
        const string DLLPath = "RemoteCli.dll";

        [DllImport(DLLPath)]
        public extern static int RemoteCli_init();

        public Form1()
        {
            InitializeComponent();
            RemoteCli_init();
        }

        [DllImport(DLLPath)]
        public extern static int RemoteCli_connect();

        [DllImport(DLLPath)]
        public extern static int RemoteCli_disconnect();

        [DllImport(DLLPath, CharSet = CharSet.Ansi)]
        public extern static Int64 getDeviceProperty([MarshalAs(UnmanagedType.LPStr)] string code);

        private void updateDP()
        {
            string[] txtOnOff = new string[] { "", "Off", "On" };

            Int64 val;
            val = getDeviceProperty("ExtendedInterfaceMode");
            if (val < txtOnOff.Length)
            {
                textExtendedIFMode.Text = txtOnOff[val];
                textExtendedIFMode.Refresh();
            }
        }

        private void buttonConnect_Click(object sender, EventArgs e)
        {
            buttonConnect.Enabled = false;
            buttonEnable.Enabled = false;
            if (buttonConnect.Text == "Connect")
            {
                textConnected.Text = "Connecting..";
                textConnected.Update();
                int ret = RemoteCli_connect();
                if (ret == 0)
                {
                    Thread.Sleep(1500);
                    updateDP();
                    textConnected.Text = "Connected";
                    buttonConnect.Text = "Disconnect";
                    buttonEnable.Enabled = true;
                } else
                {
                    textConnected.Text = "Failed";
                    MessageBox.Show("No camera!");
                }
            }
            else
            {
                textConnected.Text = "";
                buttonConnect.Text = "Connect";
                RemoteCli_disconnect();
            }
            buttonConnect.Enabled = true;
        }

        [DllImport(DLLPath, CharSet = CharSet.Ansi)]
        public extern static int setDeviceProperty([MarshalAs(UnmanagedType.LPStr)] string code, Int64 data, bool blocking);

        private void buttonEnable_Click(object sender, EventArgs e)
        {
            buttonEnable.Enabled = false;
            int ret;
            ret = setDeviceProperty("ExtendedInterfaceMode", 2, true/*blocking*/);
            if (ret != 0) goto _Error;

            updateDP();
            MessageBox.Show("OK!");
            buttonEnable.Enabled = true;
            return;
        _Error:
            updateDP();
            MessageBox.Show("Error!");
            buttonEnable.Enabled = true;
            return;
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            RemoteCli_disconnect();
        }
    }
}
