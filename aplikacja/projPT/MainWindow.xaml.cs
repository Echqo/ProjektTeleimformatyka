using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Windows.Threading;
using System.IO.Ports;

namespace projPT
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        DispatcherTimer timer = new DispatcherTimer();
        Ellipse E = new Ellipse();
        private double S1;
        private double S2;
        private double a;
        private double c;
        private double d;

        //
        public static SerialPort sp;
        public delegate void AddDelegate(String myString);
        public AddDelegate myDelegate;
        //
        public MainWindow()
        {
            InitializeComponent();
            E.Width = E.Height = 25;
            E.Fill = (SolidColorBrush)(new BrushConverter().ConvertFrom("#FF066A7A"));
            E.HorizontalAlignment = System.Windows.HorizontalAlignment.Left;
            E.VerticalAlignment = System.Windows.VerticalAlignment.Top;

            //przeliczanie rozmiaru Area na podstawie kalibracji!
            //kalibracja: S1 = ... S2 = ...
            S1 = 300; //OX
            S2 = 250; //OY
            grid_Area.Width = Convert.ToInt32(S1 * Scale(S1,S2));
            grid_Area.Height = Convert.ToInt32(S2 * Scale(S1,S2));
            border_Area.Width = grid_Area.Width;
            border_Area.Height = grid_Area.Height;

            grid_Area.Children.Add(E);

            this.myDelegate = new AddDelegate(ADM);//
            
            search_COMs();//

            timer.Interval = TimeSpan.FromMilliseconds(50);//50ms damy, to i tak za szybko odswiezanie
            timer.Tick += timer_Tick;
        }

        #region connection
        public void ADM(String myString)//
        {
            txtbox_Data.AppendText(myString);
        }
        public void Serial_DataReceived(object sender, SerialDataReceivedEventArgs e)//
        {
            SerialPort spp = (SerialPort)sender;
            string s = spp.ReadExisting();

            Application.Current.Dispatcher.Invoke((Action)(() =>
            {
                txtbox_Data.Text= s;
            }));
        }
        public void search_COMs()//
        {
            btnConnect.IsEnabled = false;
            var available_COMs = SerialPort.GetPortNames();
            int nOfCOMs = 0;
            listbox_available_COMs.Items.Clear();
            foreach(string com in available_COMs)
            {
                listbox_available_COMs.Items.Add(com);
                nOfCOMs++;
            }
            if(nOfCOMs > 0)
            {
                btnConnect.IsEnabled = true;
                btnDisconnect.IsEnabled = false;
              
            }
            else
            {
                MessageBox.Show("Podłącz urządzenie", Application.ResourceAssembly.GetName().Name);
                return;
            }
        }
        private void btnConnect_Click(object sender, EventArgs e)
        {
            try
            {
                sp = new SerialPort(listbox_available_COMs.SelectedItem.ToString(), 115200, Parity.None, 8, StopBits.One);
                sp.DataReceived += new SerialDataReceivedEventHandler(Serial_DataReceived);
                sp.Open();
                btnConnect.IsEnabled = false;
                btnDisconnect.IsEnabled = !btnDisconnect.IsEnabled;
            }
            catch(NullReferenceException)
            {

            }
            catch(Exception)
            {

            }
        }

        private void btnDisconnect_Click(object sender, RoutedEventArgs e)
        {
            btnDisconnect.IsEnabled = false;
            btnConnect.IsEnabled = !btnDisconnect.IsEnabled;
            try
            {
                sp.Close();
            }
            catch(Exception)
            {

            }
        }

        #endregion
        private void Calibration()
        {

        }

        private double Scale(double XReal, double YReal)
        {
            try
            {
                int XMax = 720;
                int YMax= 720;

                double XSym = XMax / XReal;
                double YSym = YMax / YReal;
                double Ypom = YReal * XSym;        
                double Xpom = XReal * YSym;
         
                if (Xpom > XMax)
                    return XSym;
                else if (Ypom > YMax)
                    return YSym;
                else
                    return 0;
                    
            }catch(DivideByZeroException ze)
            {
                MessageBox.Show(ze.Message);
                return 0;
            }catch(FormatException fe)
            {
                MessageBox.Show(fe.Message);
                return 0;
            }catch(Exception e)
            {
                MessageBox.Show("scale " + e.Message);
                return 0;
            }
        }

        void timer_Tick(object sender, EventArgs e)
        {
            double[] margin = new double[2];
            margin = XY(S1, S2, a, c, d);
            E.Margin = new Thickness(margin[0], margin[1], 0, 0);
            GC.Collect();// zobacz czy nie bedzie leak memory  bez tego, jak bedzie to zostaw ten kod, else zakomentuj
        }

        private void UpdateEllipsePosition()
        { 
          //  E.Margin = new Thickness(XY[0], XY[1], 0,0)
        }

        private double[] XY (double S1, double S2, double a, double c, double d)
        {
            double[] XY = new double[2];

            double X = 0;
            double omega1 = Omega(S1, a, c);
            if (omega1 != -1)
            {
                double h1 = Triangle_Height(omega1, a);
                double P1 = Field(c, h1);
                X = Coordinate(P1, S1);
            }
            else
                X = -50;

            double Y = 0;
            double omega2 = Omega(S2, c, d);
            if (omega2 != -1)
            {
                double h2 = Triangle_Height(omega2, d);
                double P2 = Field(c, h2);
                Y = Coordinate(P2, S2);
            }
            else
                Y = -50;


            XY[0] = X;
            XY[1] = Y;

            return XY;
        }

        private double Field(double a,double h)
        {
            return 0.5 * a * h;
        }

        private double Coordinate(double P, double a)
        {
            return P / (0.5 * a);
        }

        private double Omega(double s, double a, double b)
        {
            try
            {
                return 1 / Math.Cos(((Math.Pow(s, 2)) - (Math.Pow(a, 2)) - (Math.Pow(b, 2))) / ((-2) * a * b));
            }
            catch (ArithmeticException ae)
            {
                MessageBox.Show("Dzielenie przez 0 \n" + ae.Message);
                return -1;
            }
        }

        private double Triangle_Height(double alfa, double a)
        {
            if (alfa < 90)
                return Math.Sin(alfa) * a;
            else if (alfa == 90)
                return a;
            else
                return Math.Sin(alfa - 90) * a;
        }

        private void Window_Closing(object sender, System.ComponentModel.CancelEventArgs e)//
        {
            try
            {
                sp.Close();
            }
            catch(Exception)
            {

            }
            
        }

      
        

       
    }
}
