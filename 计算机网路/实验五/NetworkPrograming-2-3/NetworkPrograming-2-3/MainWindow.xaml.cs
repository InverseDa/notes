using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.Sockets;
using System.Net;
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
using System.Windows.Markup;

namespace NetworkPrograming_2_3
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private const int PORT_SERVER = 12345, PORT_CLIENT_A = 12343, PORT_CLIENT_B = 12344; // UDP端口号
        private UdpClient server, clientA, clientB; // UDP服务端和客户端
        private IPEndPoint remoteEP_Server, remoteEP_Client_A, remoteEP_Client_B; // 服务端的远程结点

        public MainWindow()
        {
            InitializeComponent();

            // 启动UDP客户端
            clientA = new UdpClient(PORT_CLIENT_A);
            clientB = new UdpClient(PORT_CLIENT_B);

            // 启动UDP服务器
            server = new UdpClient(PORT_SERVER);

            IPAddress[] addresses = Dns.GetHostAddresses(Dns.GetHostName());
            IPAddress ipv4Address = Array.Find(addresses, addr => addr.AddressFamily == AddressFamily.InterNetwork);

            remoteEP_Server = new IPEndPoint(ipv4Address, PORT_SERVER);
            remoteEP_Client_A = new IPEndPoint(ipv4Address, PORT_CLIENT_A);
            remoteEP_Client_B = new IPEndPoint(ipv4Address, PORT_CLIENT_B);

            Task.Run(() => ServerReceiveMessages());
            Task.Run(() => ClientAReceiveMessages());
            Task.Run(() => ClientBReceiveMessages());

        }

        // 服务端接收消息并转发到另外一个客户端
        private async Task ServerReceiveMessages()
        {
            while (true)
            {
                try
                {
                    // 接收UDP数据包
                    // var result = await server.ReceiveAsync();
                    // var message = Encoding.UTF8.GetString(result.Buffer);
                    // var senderEndPoint = result.RemoteEndPoint;
                    IPEndPoint remoteEP = null;
                    var data = server.Receive(ref remoteEP);

                    if (remoteEP.Port == PORT_CLIENT_A)
                    {
                        server.Send(data, data.Length, remoteEP_Client_B);
                    }
                    if (remoteEP.Port == PORT_CLIENT_B)
                    {
                        server.Send(data, data.Length, remoteEP_Client_A);
                    }

                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.Message);
                }
            }
        }

        // 客户端A接收消息
        private async Task ClientAReceiveMessages()
        {
            while (true)
            {
                try
                {
                    // 接收UDP数据包
                    IPEndPoint remoteEP = null;
                    var data = clientA.Receive(ref remoteEP);
                    // 显示消息
                    Dispatcher.Invoke(() =>
                    {
                        var messageBlock = new TextBlock
                        {
                            Text = "Machine B: \n" + "                  " + Encoding.UTF8.GetString(data),
                            TextWrapping = TextWrapping.Wrap
                        };
                        chatLogStackPanelA.Children.Add(messageBlock);
                        chatLogScrollViewerA.ScrollToEnd();
                    });

                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.Message);
                }
            }
        }

        // 客户端B接收消息
        private async Task ClientBReceiveMessages()
        {
            while (true)
            {
                try
                {
                    // 接收UDP数据包
                    IPEndPoint remoteEP = null;
                    var data = clientB.Receive(ref remoteEP);
                    // 显示消息
                    Dispatcher.Invoke(() =>
                    {
                        var messageBlock = new TextBlock
                        {
                            Text = "Machine A: \n" + "                  " + Encoding.UTF8.GetString(data),
                            TextWrapping = TextWrapping.Wrap
                        };
                        chatLogStackPanelB.Children.Add(messageBlock);
                        chatLogScrollViewerB.ScrollToEnd();
                    });
                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.Message);
                }
            }
        }

        // A发送消息
        private void sendButton_ClickA(object sender, RoutedEventArgs e)
        {
            try
            {
                // 发送UDP数据包
                var message = messageTextBoxA.Text.Trim();
                if (!string.IsNullOrEmpty(message))
                {
                    var data = Encoding.UTF8.GetBytes(message);
                    clientA.Send(data, data.Length, remoteEP_Server);
                     
                    // 显示消息
                    var messageBlock = new TextBlock
                    {
                        Text = "Me:\n" + "                  " + message,
                        TextWrapping = TextWrapping.Wrap
                    };
                    chatLogStackPanelA.Children.Add(messageBlock);
                    chatLogScrollViewerA.ScrollToEnd();

                    messageTextBoxA.Text = ""; // 清空发送框
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }

        // A清空消息记录
        private void clearButton_ClickA(object sender, RoutedEventArgs e)
        {
            chatLogStackPanelA.Children.Clear();
        }

        // B发送消息
        private void sendButton_ClickB(object sender, RoutedEventArgs e)
        {
            try
            {
                // 发送UDP数据包
                var message = messageTextBoxB.Text.Trim();
                if (!string.IsNullOrEmpty(message))
                {
                    var data = Encoding.UTF8.GetBytes(message);
                    clientB.Send(data, data.Length, remoteEP_Server);

                    // 显示消息
                    var messageBlock = new TextBlock
                    {
                        Text = "Me:\n" + "                  " + message,
                        TextWrapping = TextWrapping.Wrap
                    };
                    chatLogStackPanelB.Children.Add(messageBlock);
                    chatLogScrollViewerB.ScrollToEnd();

                    messageTextBoxB.Text = ""; // 清空发送框
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }

        // B清空消息记录
        private void clearButton_ClickB(object sender, RoutedEventArgs e)
        {
            chatLogStackPanelB.Children.Clear();
        }
    }
}
