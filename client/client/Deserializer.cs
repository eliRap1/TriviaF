using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;

using Newtonsoft.Json;


namespace client
{
    public class RoomInfo
    {
        public RoomInfo(int id, string name, int maxPlayers, int numOfQuestionsInGame, int timePerQuestion) { this.id = id; this.name = name; this.maxPlayers = maxPlayers; this.numOfQuestionsInGame = numOfQuestionsInGame; this.timePerQuestion = timePerQuestion; }
        public int id { get; set; }
        public string name { get; set; }
        public int maxPlayers { get; set; }
        public int numOfQuestionsInGame { get; set; }
        public int timePerQuestion { get; set; }
    }
    public class GetRoomStateResponse
    {
        public int status { get; set; }
        public bool hasGameBegun { get; set; }
        public List<string> players { get; set; }
        public int questionCount { get; set; }
        public int answerTimeout { get; set; }
    }

    public class GetRoomsResponse
    {
        public int status { get; set; }
        public List<RoomInfo> rooms { get; set; }
    }

    public class GetPlayersInRoomResponse
    {
        [JsonProperty("players")]
        public List<string> Players { get; set; }
    }
    public class GetGameRequest
    {
        public int status { get; set; }
        public List<PlayerRequest> players { get; set; }
    }
    public class PlayerRequest
    {
        string username { get; set; }
        int correctAnswerCount { get; set; }
        int wrongAnswerCount { get; set; }
        int averageAnswerTime { get; set; }
    }

    public class GetQuestionResponse
    {
        public int status { get; set; }
        public string question { get; set; }
        public Dictionary<int, string> answers { get; set; }
    }

    public class AnswerResponse
    {
        public int status { get; set; }
        public int correctAnswerId { get; set; }
    }


    public class Deserializer
    {
        public static int extractStatus(byte[] responseData)
        {
            string json = Encoding.UTF8.GetString(responseData);
            string key = "\"status\":";
            int index = json.IndexOf(key);
            if (index == -1)
            {
                key = "\"message\"";
                string message = json.Substring(json.IndexOf(key) + key.Length + 2);
                message = message.Substring(0, message.IndexOf("\""));
                throw new Exception(message);
            }
                

            index += key.Length;
            string number = "";
            while (index < json.Length && char.IsDigit(json[index]))
            {
                number += json[index];
                index++;
            }

            if (int.TryParse(number, out int status))
            {
                return status;
            }
            throw new Exception("Invalid status value in response");
        }
        public static List<RoomInfo> DeserializeGetRoomsResponse(byte[] responseData)
        {
            const int headerSize = 5; // 4 byte fot json
            if (responseData.Length <= headerSize)
                return new List<RoomInfo>(); // not right response
            
            byte[] jsonBytes = responseData.Skip(headerSize).ToArray(); // skip the header

            string json = Encoding.UTF8.GetString(jsonBytes);

            try
            {
                var response = JsonConvert.DeserializeObject<GetRoomsResponse>(json);
                return response?.rooms ?? new List<RoomInfo>();
            }
            catch (Exception e)
            {
                MessageBox.Show($"error:{e.Message}", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                return new List<RoomInfo>();
            }
        }
        public static List<string> DeserializeGetPlayersInRoomResponse(byte[] responseData)
        {
            const int headerSize = 5; // skip the header

            if (responseData.Length <= headerSize)
                return new List<string>(); // invalid response

            try
            {
                byte[] jsonBytes = responseData.Skip(headerSize).ToArray();
                string json = Encoding.UTF8.GetString(jsonBytes);

                var response = JsonConvert.DeserializeObject<GetPlayersInRoomResponse>(json);
                return response?.Players ?? new List<string>();
            }
            catch (Exception e)
            {
                MessageBox.Show($"Error while deserializing: {e.Message}", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                return new List<string>();
            }
        }
        public static GetRoomStateResponse DesirializeGetRoomStateResponse(byte[] responseData)
        {
            const int headerSize = 5; // skip the header

            if (responseData.Length <= headerSize)
                return new GetRoomStateResponse(); // invalid response

            try
            {
                byte[] jsonBytes = responseData.Skip(headerSize).ToArray();
                string json = Encoding.UTF8.GetString(jsonBytes);

                return JsonConvert.DeserializeObject<GetRoomStateResponse>(json);
            }
            catch (Exception e)
            {
                MessageBox.Show($"Error while deserializing: {e.Message}", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                return new GetRoomStateResponse();
            }
        }
        public static GetGameRequest DeserializeGetGameRequest(byte[] responseData)
        {
            const int headerSize = 5; // skip the header

            if (responseData.Length <= headerSize)
                return new GetGameRequest(); // invalid response

            try
            {
                byte[] jsonBytes = responseData.Skip(headerSize).ToArray();
                string json = Encoding.UTF8.GetString(jsonBytes);

                return JsonConvert.DeserializeObject<GetGameRequest>(json);
            }
            catch (Exception e)
            {
                MessageBox.Show($"Error while deserializing: {e.Message}", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                return new GetGameRequest();
            }
        }
        public static GetQuestionResponse DeserializeGetQuestionResponse(byte[] responseData)
        {
            const int headerSize = 5; // skip the header

            if (responseData.Length <= headerSize)
                return new GetQuestionResponse(); // invalid response

            try
            {
                byte[] jsonBytes = responseData.Skip(headerSize).ToArray();
                string json = Encoding.UTF8.GetString(jsonBytes);

                return JsonConvert.DeserializeObject<GetQuestionResponse>(json);
            }
            catch (Exception e)
            {
                MessageBox.Show($"Error while deserializing: {e.Message}", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                return new GetQuestionResponse();
            }
        }
        public static AnswerResponse DeserializeAnswerResponse(byte[] responseData)
        {
            const int headerSize = 5; // skip the header

            if (responseData.Length <= headerSize)
                return new AnswerResponse(); // invalid response

            try
            {
                byte[] jsonBytes = responseData.Skip(headerSize).ToArray();
                string json = Encoding.UTF8.GetString(jsonBytes);

                return JsonConvert.DeserializeObject<AnswerResponse>(json);
            }
            catch (Exception e)
            {
                MessageBox.Show($"Error while deserializing: {e.Message}", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                return new AnswerResponse();
            }
        }


    }
}
