// See https://aka.ms/new-console-template for more information
using System.Net;
using System.Net.Sockets;
public class Server
{
    public static void Main(string[] args)
    {
        GetHostNameAndIp();
        GetCsdnIpByDns();
        GetPagesSize();
    }

    private static void GetHostNameAndIp()
    {
        string localIP, localMachine;
        using (var socket = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, 0))
        {
            socket.Connect("8.8.8.8", 65530);
            var endPoint = socket.LocalEndPoint as IPEndPoint;
            localIP = endPoint.Address.ToString();
            localMachine = Dns.GetHostName();
        }
        Console.WriteLine("本机名称：{1}, 本机IP为：{0}",localIP, localMachine);
    }

    private static void GetCsdnIpByDns()
    {
        try
        {
            IPAddress[] addresses = Dns.GetHostAddresses("www.csdn.net");
            Console.WriteLine("CSDN被解析后的IP：");
            foreach (IPAddress address in addresses)
            {
                Console.WriteLine(address.ToString());
            }
        }catch (Exception e)
        {
            Console.WriteLine(e.ToString());
        }
    }

    private static void GetPagesSize()
    {
        try
        {
            WebClient client = new WebClient();
            byte[] data = client.DownloadData("https://www.szu.edu.cn");
            int fileSize = data.Length;
            Console.WriteLine("文件大小：" + fileSize + "字节");
            Console.WriteLine("下载完成！");
        }catch (Exception e)
        {
            Console.WriteLine(e.ToString());
        }
    }
}