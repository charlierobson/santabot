// Filename:  HttpServer.cs        
// Author:    Benjamin N. Summerton <define-private-public>        
// License:   Unlicense (http://unlicense.org/)

using System;
using System.Text;
using System.Net;
using System.Threading.Tasks;

namespace printx
{
    class HttpServer
    {
        public static HttpListener listener;

        public static async Task HandleIncomingConnections(Func<string, string> get, Action<string, string> post)
        {
            bool runServer = true;

            // While a user hasn't visited the `shutdown` url, keep on handling requests
            while (runServer)
            {
                // Will wait here until we hear from a connection
                HttpListenerContext ctx = await listener.GetContextAsync();

                // Peel out the requests and response objects
                HttpListenerRequest req = ctx.Request;
                HttpListenerResponse resp = ctx.Response;

                var body = "";
                if (req.HasEntityBody)
                {
                    using (var stream = req.InputStream) // here we have data
                    {
                        using (var reader = new System.IO.StreamReader(stream, req.ContentEncoding))
                        {
                            body = reader.ReadToEnd();
                        }
                    }
                }

                var content = "--";

                // If `shutdown` url requested w/ POST, then shutdown the server after serving the page
                if (req.HttpMethod == "POST")
                {
                    post(req.Url.LocalPath, body);
                }

                // If `shutdown` url requested w/ POST, then shutdown the server after serving the page
                if (req.HttpMethod == "GET")
                {
                    content = get(req.Url.LocalPath);
                }

                // Write the response info
                byte[] data = Encoding.UTF8.GetBytes(content);
                resp.ContentType = "text/plain";
                resp.ContentEncoding = Encoding.UTF8;
                resp.ContentLength64 = data.LongLength;

                // Write out to the response stream (asynchronously), then close it
                await resp.OutputStream.WriteAsync(data, 0, data.Length);
                resp.Close();
            }
        }

        public static void Run(string ip, Func<string, string> get, Action<string, string> post)
        {
            // Create a Http server and start listening for incoming connections
            listener = new HttpListener();
            listener.Prefixes.Add(ip);
            listener.Start();

            // Handle requests
            Task listenTask = HandleIncomingConnections(get, post);
            listenTask.GetAwaiter().GetResult();

            // Close the listener
            listener.Close();
        }
    }
}
