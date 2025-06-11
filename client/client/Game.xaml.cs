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
        public class PlayerResult
        {
            public string Name { get; set; }
            public int Correct { get; set; }
            public double AvgTime { get; set; }
        }
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

        }

        private void Answer_Click(object sender, RoutedEventArgs e)
        {
            QuestionTimerBar.Maximum = timePerQuestion;
            QuestionTimerBar.Value = timePerQuestion;
            timeLeft = timePerQuestion;
            timer.Start();
        }
        private void NextQuestion()
        {
            timer.Start();
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
                    ResultsList.ItemsSource = new List<PlayerResult>
                    {
                        new PlayerResult { Name="Eli", Correct=5, AvgTime=3.2 },
                        new PlayerResult { Name="idan",   Correct=4, AvgTime=4.1 }
                    };
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
