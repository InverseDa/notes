// See https://aka.ms/new-console-template for more information

using System.Net.Sockets;
using System.Text;

class Client { 
    public static void Main() {
        // 创建TcpClient对象
        try
        {
            TcpClient client = new TcpClient("127.0.0.1", 8888);
            // 获取网络流
            NetworkStream stream = client.GetStream();
            while (true) {
                // 读取命令
                Console.Write("请输入命令：");
                string command = Console.ReadLine();
                byte[] buffer = Encoding.ASCII.GetBytes(command);
                // 发送命令
                stream.Write(buffer, 0, buffer.Length);
                // 读取结果
                buffer = new byte[1024];
                int count = stream.Read(buffer, 0, buffer.Length);
                string result = Encoding.ASCII.GetString(buffer, 0, count).Trim();
                Console.WriteLine("接收到结果：" + result);
                // 判断是否退出
                if (result == "Bye") {
                    break;
                }
            }
            // 关闭连接
            stream.Close();
            client.Close();
            Console.WriteLine("客户端退出");
        }
        catch (Exception e)
        {
            Console.WriteLine(e);
        }
    }
}