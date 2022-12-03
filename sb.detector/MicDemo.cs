/*
    Copyright 2020-2021 Picovoice Inc.

    You may not use this file except in compliance with the license. A copy of the license is located in the "LICENSE"
    file accompanying this source.

    Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on
    an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the
    specific language governing permissions and limitations under the License.
*/

using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Media;
using System.Reflection;
using System.Threading;

using Pv;

namespace PorcupineDemo
{
    /// <summary>
    /// Microphone Demo for Porcupine wake word engine. It creates an input audio stream from a microphone, monitors it, and
    /// upon detecting the specified wake word(s) prints the detection time and wake word on console. It optionally saves
    /// the recorded audio into a file for further debugging.
    /// </summary>                
    public class MicDemo
    {
        public static void Main()
        {
            var homeDir = Path.GetDirectoryName(new Uri(Assembly.GetExecutingAssembly().CodeBase).LocalPath);

            AppDomain.CurrentDomain.UnhandledException += OnUnhandledException;

            string accessKey = "dRu0zJRd5PQ9HjWfsQlKaCaUvKyTLS/Vo3PJm2EEvcKl3VBnOoHABg==";

            var keywordPaths = new List<string>
            {
                Path.Combine(homeDir, @"data\santabot.ppn")
            };

            SoundPlayer sp = new SoundPlayer(Path.Combine(homeDir, @"data\santabot.wav"));

            Porcupine porcupine = Porcupine.FromKeywordPaths(accessKey, keywordPaths);

            // get keyword names for labeling detection results                
            List<string> keywordNames = keywordPaths.Select(k => Path.GetFileNameWithoutExtension(k).Split(' ')[0]).ToList();

            Console.CancelKeyPress += (s, o) =>
            {
                Console.WriteLine("Stopping...");
                porcupine?.Dispose();
            };

            // create and start recording
            using (PvRecorder recorder = PvRecorder.Create(deviceIndex: -1, frameLength: porcupine.FrameLength))
            {
                Console.WriteLine($"Using device: {recorder.SelectedDevice}");
                Console.Write($"Listening for [{string.Join(" ", keywordNames.Select(k => $"'{k}'"))}]...\n");
                recorder.Start();

                while (true)
                {
                    short[] pcm = recorder.Read();

                    int result = porcupine.Process(pcm);
                    if (result >= 0)
                    {
                        recorder.Stop();
                        Console.WriteLine($"[{DateTime.Now.ToLongTimeString()}] Detected '{keywordNames[result]}'");
                        sp.Play();

                        // pause recording so we don't hear ourselves santabotting
                        Thread.Sleep(2000);
                        recorder.Start();
                    }

                    Thread.Yield();
                }
            }
        }

        private static void OnUnhandledException(object sender, UnhandledExceptionEventArgs e)
        {
            Console.WriteLine(e.ExceptionObject.ToString());
            Environment.Exit(1);
        }
    }
}
