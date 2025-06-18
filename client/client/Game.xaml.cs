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

namespace client
{
    /// <summary>
    /// Interaction logic for Game.xaml
    /// </summary>
    public partial class Game : Page
    {
        private readonly DispatcherTimer timer;
        private double timeLeft;       // seconds remaining
        private readonly double timePerQuestion; // from room settings
        private int currentIndex;
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
            correctCount = room.numOfQuestionsInGame;
            timePerQuestion = room.timePerQuestion;
            timeLeft = timePerQuestion;
            totalAnswerTime = timeLeft * room.numOfQuestionsInGame;
            QuestionsLeftText.Text = correctCount.ToString();
            QuestionTimerBar.Maximum = timePerQuestion;
            QuestionTimerBar.Value = timeLeft;
            NextQuestion();

        }

        private void Answer_Click(object sender, RoutedEventArgs e)
        {
            byte[] request = Serializer.SerializeSumbitAnswerRequest(Convert.ToInt32((sender as Button).Tag));
            byte[] response = MainWindow.communicator.sendAndReceive(request);
            AnswerResponse answer = Deserializer.DeserializeAnswerResponse(response);
            if (answer.correctAnswerId == Convert.ToInt32((sender as Button).Tag))
            {
                currentIndex++;
                if (currentIndex == correctCount)
                {
                    timer.Stop();
                    ResultsView.Visibility = Visibility.Visible;
                    QuestionView.Visibility = Visibility.Collapsed;
                    byte[] request1 = Serializer.SerializeSimpleRequest(Serializer.GET_GAME_RESULTS_CODE);
                    byte[] response1 = MainWindow.communicator.sendAndReceive(request1);
                    GetGameRequest game = Deserializer.DeserializeGetGameRequest(response1);
                    ResultsList.ItemsSource = game.players;
                }
                else
                {
                    NextQuestion();
                }
            }
               
            QuestionTimerBar.Maximum = timePerQuestion;
            QuestionTimerBar.Value = timePerQuestion;
            timeLeft = timePerQuestion;
            timer.Start();
        }
        private void NextQuestion()
        {
            timer.Start();
            byte[] request = Serializer.SerializeSimpleRequest(Serializer.GET_QUESTION_CODE);
            byte[] response = MainWindow.communicator.sendAndReceive(request);
            GetQuestionResponse qusestion = Deserializer.DeserializeGetQuestionResponse(response);
            QuestionText.Text = qusestion.question;
            AnswerButton1.Content = qusestion.answers.Values.ElementAt(0);
            AnswerButton2.Content = qusestion.answers.Values.ElementAt(1);
            AnswerButton3.Content = qusestion.answers.Values.ElementAt(2);
            AnswerButton4.Content = qusestion.answers.Values.ElementAt(3);
            AnswerButton1.Tag = qusestion.answers.Keys.ElementAt(0);
            AnswerButton2.Tag = qusestion.answers.Keys.ElementAt(1);
            AnswerButton3.Tag = qusestion.answers.Keys.ElementAt(2);
            AnswerButton4.Tag = qusestion.answers.Keys.ElementAt(3);
        }
        private void Timer_Tick(object sender, EventArgs e)
        {
            timeLeft -= timer.Interval.TotalSeconds;
            if (timeLeft <= 0)
            {
                timer.Stop();
                // treat as incorrect and move on
                currentIndex++;
                if (currentIndex == correctCount)
                {
                    timer.Stop();
                    ResultsView.Visibility = Visibility.Visible;
                    QuestionView.Visibility = Visibility.Collapsed;
                    byte[] request = Serializer.SerializeSimpleRequest(Serializer.GET_GAME_RESULTS_CODE);
                    byte[] response = MainWindow.communicator.sendAndReceive(request);
                    GetGameRequest game = Deserializer.DeserializeGetGameRequest(response);
                    ResultsList.ItemsSource = game.players;
                }
                NextQuestion();
            }
            else
            {
                QuestionTimerBar.Value = timeLeft;
            }
        }

    }
}
