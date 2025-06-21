using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Remoting.Lifetime;
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

namespace client
{
    /// <summary>
    /// Interaction logic for Game.xaml
    /// </summary>
    public partial class Game : Page
    {
        private readonly DispatcherTimer timer;
        private readonly DispatcherTimer resultTimer;
        private double timeLeft;       // seconds remaining
        private readonly double timePerQuestion; // from room settings
        private int currentIndex;
        private int Count;
        private int correctCount;
        private double totalAnswerTime;
        //public class PlayerResult
        //{
        //    public string Name { get; set; }
        //    public int Correct { get; set; }
        //    public double AvgTime { get; set; }
        //}
        public Game(RoomInfo room)
        {
            InitializeComponent();
            timer = new DispatcherTimer();
            timer.Interval = TimeSpan.FromSeconds(0.1);
            timer.Tick += Timer_Tick;
            timer.Start();
            currentIndex = 0;
            Count = room.numOfQuestionsInGame;
            timePerQuestion = room.timePerQuestion;
            timeLeft = timePerQuestion;
            totalAnswerTime = timeLeft * room.numOfQuestionsInGame;
            QuestionsLeftText.Text = Count.ToString();
            QuestionTimerBar.Maximum = timePerQuestion;
            QuestionTimerBar.Value = timeLeft;
            correctCount = 0;
            resultTimer = new DispatcherTimer();
            NextQuestion();

        }
        private void Leave_Click(object sender, RoutedEventArgs e)
        {
            resultTimer.Stop();
            byte[] request = Serializer.SerializeSimpleRequest(Serializer.LEAVE_ROOM_CODE);
            byte[] response = MainWindow.communicator.sendAndReceive(request);
            page.Navigate(new Trivia());
        }
        private void Answer_Click(object sender, RoutedEventArgs e)
        {
            byte[] request = Serializer.SerializeSumbitAnswerRequest(Convert.ToInt32((sender as Button).Tag));
            byte[] response = MainWindow.communicator.sendAndReceive(request);
            AnswerResponse answer = Deserializer.DeserializeAnswerResponse(response);
            currentIndex++;
            if (answer.correctAnswerId == Convert.ToInt32((sender as Button).Tag))
            {
                correctCount++;
                CorrectSoFarText.Text = correctCount.ToString();
            }
            if (currentIndex == Count)
            {
                Results(null, null);
                timer.Stop();
                resultTimer.Interval = TimeSpan.FromSeconds(3);
                resultTimer.Tick += Results;
                resultTimer.Start();
            }
            else
            {
                NextQuestion();
                QuestionTimerBar.Maximum = timePerQuestion;
                QuestionTimerBar.Value = timePerQuestion;
                timeLeft = timePerQuestion;
                timer.Start();
            }
        }
            private void Results(object sender, EventArgs e)
        {
            leaveBtn.Visibility = Visibility.Visible;
            ResultsView.Visibility = Visibility.Visible;
            QuestionView.Visibility = Visibility.Collapsed;
            byte[] request = Serializer.SerializeSimpleRequest(Serializer.GET_GAME_RESULTS_CODE);
            byte[] response = MainWindow.communicator.sendAndReceive(request);
            GetGameResultsResponse game = Deserializer.DeserializeGetGameResultsResponse(response);
            ResultsList.ItemsSource = game.results;
            string winPlayer = "";
            int correctMax = -1;
            foreach (PlayerResults player in game.results)
            {
                if (player.correctAnswerCount > correctMax)
                {
                    winPlayer = player.username;
                    correctMax = player.correctAnswerCount;
                }
            }
            WinnerText.Text = "Winner: " + winPlayer;
        }
        private void NextQuestion()
        {
            QuestionTimerBar.Maximum = timePerQuestion;
            QuestionTimerBar.Value = timePerQuestion;
            timeLeft = timePerQuestion;
            timer.Start();
            byte[] request = Serializer.SerializeSimpleRequest(Serializer.GET_QUESTION_CODE);
            byte[] response = MainWindow.communicator.sendAndReceive(request);
            GetQuestionResponse qusestion = Deserializer.DeserializeGetQuestionResponse(response);
            QuestionText.Text = qusestion.question;
            List<int> indices = new List<int>(qusestion.answers.Keys);

            // Shuffle the list
            Random rng = new Random();
            indices = indices.OrderBy(_ => rng.Next()).ToList();

            AnswerButton1.Content = qusestion.answers[indices[0]];
            AnswerButton2.Content = qusestion.answers[indices[1]];
            AnswerButton3.Content = qusestion.answers[indices[2]];
            AnswerButton4.Content = qusestion.answers[indices[3]];

            AnswerButton1.Tag = indices[0];
            AnswerButton2.Tag = indices[1];
            AnswerButton3.Tag = indices[2];
            AnswerButton4.Tag = indices[3];

        }
        private void Timer_Tick(object sender, EventArgs e)
        {
            
            timeLeft -= timer.Interval.TotalSeconds;
            if (timeLeft <= 0)
            {
                timer.Stop();
                currentIndex++;
                if (currentIndex == Count)
                {
                    timer.Stop();
                    Results(null, null);
                    timer.Stop();
                    resultTimer.Interval = TimeSpan.FromSeconds(3);
                    resultTimer.Tick += Results;
                    resultTimer.Start();
                }
                else
                {
                   byte[] request = Serializer.SerializeSumbitAnswerRequest(-1);
                   byte[] response = MainWindow.communicator.sendAndReceive(request);
                   NextQuestion();
                }
                
            }
            else
            {
                QuestionTimerBar.Value = timeLeft;
            }
        }

    }
}
