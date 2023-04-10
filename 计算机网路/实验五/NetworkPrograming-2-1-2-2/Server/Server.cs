
using System.Net;
using System.Net.Sockets;
using System.Text;

class Server { 
    public static void Main() {
        // 创建TcpListener对象
        TcpListener listener = new TcpListener(IPAddress.Any, 8888);
        Console.WriteLine("服务器启动完毕");
        // 开始监听
        listener.Start();
        while (true) {
            // 等待客户端连接
            TcpClient client = listener.AcceptTcpClient();
            Console.WriteLine("创建客户端连接：" + client.Client.RemoteEndPoint.ToString());
            // 获取网络流
            NetworkStream stream = client.GetStream();
            // 读取命令
            byte[] buffer = new byte[1024];
            int count = stream.Read(buffer, 0, buffer.Length);
            string command = Encoding.ASCII.GetString(buffer, 0, count).Trim();
            Console.WriteLine("接收到命令：" + command);
            // 处理命令
            string result = "";
            while (command != "Exit")
            {
                if (command == "Time") {
                    result = DateTime.Now.ToString();
                } 
                else {
                    result = "Unknown command";
                }
                // 发送结果
                buffer = Encoding.ASCII.GetBytes(result);
                stream.Write(buffer, 0, buffer.Length);
                Console.WriteLine("发送结果：" + result);
                // 读取下一个命令
                buffer = new byte[1024];
                count = stream.Read(buffer, 0, buffer.Length);
                command = Encoding.ASCII.GetString(buffer, 0, count).Trim();
                Console.WriteLine("接收到命令：" + command);
            }
            // 处理Exit命令
            result = "Bye";
            // 发送结果
            buffer = Encoding.ASCII.GetBytes(result);
            stream.Write(buffer, 0, buffer.Length);
            Console.WriteLine("发送结果：" + result);
            // 关闭连接
            stream.Close();
            client.Close();
            Console.WriteLine("客户端连接已关闭");
            break;
        }
        Console.WriteLine("Bye");
    }
}