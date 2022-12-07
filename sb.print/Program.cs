using System;
using System.Drawing;
using System.Drawing.Printing;
using System.IO;
using System.Media;
using System.Net;
using System.Reflection;
using System.Threading;
using System.Threading.Tasks;

namespace santabot
{
    class Program
    {
        private static string _dataDir;

        private static SoundPlayer _sp;

        private static Font computo15;
        private static Font computo20;
        private static Font computo30;
        private static Font courier;

        private static string[] adjectives =
        {
            "SORTA",
            "MOSTLY",
            "HELLA"
        };

        static void Main(string[] cmdLine)
        {
            computo15 = new Font("Computo Monospace", 15);
            computo20 = new Font("Computo Monospace", 25);
            computo30 = new Font("Computo Monospace", 30);
            courier = new Font("Courier New", 30);

#if (DEBUG)
            Console.WriteLine("DEBUG - NO PRINT OUTPUT!!!!");
            Console.WriteLine("DEBUG - NO PRINT OUTPUT!!!!");
            Console.WriteLine("DEBUG - NO PRINT OUTPUT!!!!");
            Console.WriteLine("DEBUG - NO PRINT OUTPUT!!!!");
            Console.WriteLine("DEBUG - NO PRINT OUTPUT!!!!");
            Console.WriteLine("DEBUG - NO PRINT OUTPUT!!!!");
            Console.WriteLine("DEBUG - NO PRINT OUTPUT!!!!");
#endif

            string hostName = Dns.GetHostName();
            IPAddress[] addresses = Dns.GetHostEntry(hostName).AddressList;
            string ip = addresses[1].ToString();

            _sp = new SoundPlayer();

            Console.WriteLine($"sb.print IP: http://{ip}:8000/");

            PrinterSettings printerSettings = new PrinterSettings();
            Console.WriteLine($"sb.print default printer: {printerSettings.PrinterName}");

            var home = Path.GetDirectoryName(new Uri(Assembly.GetExecutingAssembly().CodeBase).LocalPath);
            _dataDir = Path.Combine(home, "data");

            object strongly = new object();

            new Thread(new ThreadStart(() => HttpServer.Run($"http://{ip}:8000/",
                (url) => // GET handler
                {
                    lock (strongly)
                    {
                        Console.WriteLine($"GET {url}");
                        return "sb.print available";
                    }
                },
                (url, body) => // POST handler
                {
                    lock (strongly)
                    {
                        try
                        {
                            Console.WriteLine($"POST {url}, body: {body}\n");
#if (RELEASE)
                            if (url == "/print")
                            {
                                // POST  to /print with data:
                                // good <name>
                                // .. or
                                // naughty
                                if (body.StartsWith("nice"))
                                {
                                    var payload = body.Substring(body.IndexOf(' '));
                                    Task.Run(() => PrintCongrats(payload));
                                }
                                else if (body.StartsWith("naughty"))
                                {
                                    Task.Run(()=>PrintNaughty());
                                }
                            }
                            else 
#endif
                            if (url == "/loop")
                            {
                                _sp.Stop();
                                _sp.SoundLocation = Path.Combine(_dataDir, body);
                                _sp.PlayLooping();
                            }
                            else if (url == "/play")
                            {
                                _sp.Stop();
                                _sp.SoundLocation = Path.Combine(_dataDir, body);
                                _sp.Play();
                            }
                            else if (url == "/stop")
                            {
                                _sp.Stop();
                            }
                            else if (url == "/test")
                            {
                            }
                        }
                        catch { }
                    }
                }
                ))).Start();
        }

        static void PrintNaughty()
        {
            PrintDocument pd = new PrintDocument();
            PrintController printController = new StandardPrintController();
            pd.PrintController = printController;

            pd.DefaultPageSettings.Margins = new Margins(0, 0, 0, 0);
            pd.PrinterSettings.DefaultPageSettings.Margins = new Margins(0, 0, 0, 0);
            pd.DefaultPageSettings.Landscape = false;

            int page = 0;

            pd.PrintPage += (sndr, ev) =>
            {
                for (var Y = 0; Y < 600; Y+= 55)
                {
                    ev.Graphics.DrawString("*** VERY NAUGHTY ***", computo15, Brushes.Black, 0, Y, new StringFormat());
                    ev.Graphics.DrawString(" SANTABOT  OVERLOAD", computo15, Brushes.Black, 0, Y+27, new StringFormat());
                }
                ++page;
                ev.HasMorePages = page < 4;
            };
            pd.Print();
        }

        static void PrintCongrats(string name)
        {
            PrintDocument pd = new PrintDocument();
            PrintController printController = new StandardPrintController();
            pd.PrintController = printController;

            pd.DefaultPageSettings.Margins = new Margins(0, 0, 0, 0);
            pd.PrinterSettings.DefaultPageSettings.Margins = new Margins(0, 0, 0, 0);

            var page = 0;
            var rando = new Random();

            pd.PrintPage += (sndr, ev) =>
            {
                if (page == 0)
                {
                    var yPos = ev.MarginBounds.Top + 40;

                    ev.Graphics.DrawString("CONGRATULATIONS", computo30, Brushes.Black, 0, yPos, new StringFormat());
                    ev.Graphics.DrawString(name, courier, Brushes.Black, 0, yPos + 60, new StringFormat());
                    ev.Graphics.DrawString("SantaBot rated you:", computo20, Brushes.Black, 0, yPos + 150, new StringFormat());
                    ev.Graphics.DrawString($"*** {adjectives[rando.Next(adjectives.Length)]} NICE ***", computo20, Brushes.Black, 0, yPos + 230, new StringFormat());

                    pd.DefaultPageSettings.Landscape = true;
                }
                else
                {
                    var i = Image.FromFile(Path.Combine(_dataDir, "tu.png"));

                    Rectangle m = ev.MarginBounds;

                    if ((double)i.Width / (double)i.Height > (double)m.Width / (double)m.Height) // image is wider
                    {
                        m.Height = (int)((double)i.Height / (double)i.Width * (double)m.Width);
                    }
                    else
                    {
                        m.Width = (int)((double)i.Width / (double)i.Height * (double)m.Height);
                    }

                    m.X = 100;

                    ev.Graphics.DrawImage(i, m);
                }

                ++page;
                ev.HasMorePages = page < 2;  
            };
            pd.Print();
        }
    }
}
