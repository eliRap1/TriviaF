using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
namespace client
{
    public class Serializer
    {
        private const int SIGNUP_CODE = 102;
        private const int LOGIN_CODE = 101;
        private const int LOGOUT_CODE = 4;
        private const int GET_ROOMS_CODE = 5;
        private const int GET_PLAYERS_IN_ROOM_CODE = 6;
        private const int JOIN_ROOM_CODE = 7;
        private const int CREATE_ROOM_CODE = 8;
        private const int HIGH_SCORE_CODE = 9;
        private const int PERSONAL_STATS_CODE = 10;
        public const int CLOSE_ROOM_CODE = 11;
        public const int START_GAME_CODE = 12;
        public const int GET_ROOM_STATE_CODE = 13;
        public const int LEAVE_ROOM_CODE = 14;
        public const int GET_GAME_RESULTS_CODE = 15;
        public const int GET_QUESTION_CODE = 17;
        public const int SUBMIT_ANSWER_CODE = 16;

        public static byte[] serializeSignupRequest(string username, string password, string email)
        {
            string json = "{"
                        + "\"username\":\"" + username + "\","
                        + "\"password\":\"" + password + "\","
                        + "\"email\":\"" + email + "\""
                        + "}";
            byte[] jsonBytes = Encoding.UTF8.GetBytes(json);
            int length = jsonBytes.Length;
            byte[] packet = new byte[1 + 4 + jsonBytes.Length];
            packet[0] = SIGNUP_CODE;
            packet[1] = (byte)((length >> 24) & 0xFF);
            packet[2] = (byte)((length >> 16) & 0xFF);
            packet[3] = (byte)((length >> 8) & 0xFF);
            packet[4] = (byte)(length & 0xFF);
            Buffer.BlockCopy(jsonBytes, 0, packet, 5, jsonBytes.Length);
            return packet;
        }
        public static byte[] serializeLoginRequest(string username, string password)
        {
            string json = "{"
                        + "\"username\":\"" + username + "\","
                        + "\"password\":\"" + password + "\""
                        + "}";

            byte[] jsonBytes = Encoding.UTF8.GetBytes(json);
            int length = jsonBytes.Length;
            byte[] packet = new byte[1 + 4 + jsonBytes.Length];
            packet[0] = LOGIN_CODE;
            packet[1] = (byte)((length >> 24) & 0xFF);
            packet[2] = (byte)((length >> 16) & 0xFF);
            packet[3] = (byte)((length >> 8) & 0xFF);
            packet[4] = (byte)(length & 0xFF);
            Buffer.BlockCopy(jsonBytes, 0, packet, 5, jsonBytes.Length);
            return packet;
        }
        public static byte[] serializeLogoutRequest(string username)
        {
            string json = "{"
                        + "\"username\":\"" + username + "\""
                        + "}";
            byte[] jsonBytes = Encoding.UTF8.GetBytes(json);
            int length = jsonBytes.Length;
            byte[] packet = new byte[1 + 4 + jsonBytes.Length];
            packet[0] = LOGOUT_CODE;
            packet[1] = (byte)((length >> 24) & 0xFF);
            packet[2] = (byte)((length >> 16) & 0xFF);
            packet[3] = (byte)((length >> 8) & 0xFF);
            packet[4] = (byte)(length & 0xFF);
            Buffer.BlockCopy(jsonBytes, 0, packet, 5, jsonBytes.Length);
            return packet;
        }
        public static byte[] serializeGetRoomsRequest()
        {
            string json = "{}";
            byte[] jsonBytes = Encoding.UTF8.GetBytes(json);
            int length = jsonBytes.Length;
            byte[] packet = new byte[1 + 4 + jsonBytes.Length];
            packet[0] = GET_ROOMS_CODE;
            packet[1] = (byte)((length >> 24) & 0xFF);
            packet[2] = (byte)((length >> 16) & 0xFF);
            packet[3] = (byte)((length >> 8) & 0xFF);
            packet[4] = (byte)(length & 0xFF);
            Buffer.BlockCopy(jsonBytes, 0, packet, 5, jsonBytes.Length);
            return packet;
        }
        public static byte[] SearalizeCreateRoomRequest(int maxUsers, int answerTimeout, string roomName, int QuestionsCount)
        {
            string json = "{"
                        + "\"maxUsers\":" + maxUsers + ","
                        + "\"answerTimeout\":" + answerTimeout + ","
                        + "\"roomName\":\"" + roomName + "\","
                        + "\"QuestionsCount\":" + QuestionsCount
                        + "}";
            byte[] jsonBytes = Encoding.UTF8.GetBytes(json);
            int length = jsonBytes.Length;
            byte[] packet = new byte[1 + 4 + jsonBytes.Length];
            packet[0] = CREATE_ROOM_CODE;
            packet[1] = (byte)((length >> 24) & 0xFF);
            packet[2] = (byte)((length >> 16) & 0xFF);
            packet[3] = (byte)((length >> 8) & 0xFF);
            packet[4] = (byte)(length & 0xFF);
            Buffer.BlockCopy(jsonBytes, 0, packet, 5, jsonBytes.Length);
            return packet;
        }
        public static byte[] SearalizeJoinRoomRequest(int id)
        {
            string json = "{"
                        + "\"roomId\":\"" + id + "\""
                        + "}";
            byte[] jsonBytes = Encoding.UTF8.GetBytes(json);
            int length = jsonBytes.Length;
            byte[] packet = new byte[1 + 4 + jsonBytes.Length];
            packet[0] = JOIN_ROOM_CODE;
            packet[1] = (byte)((length >> 24) & 0xFF);
            packet[2] = (byte)((length >> 16) & 0xFF);
            packet[3] = (byte)((length >> 8) & 0xFF);
            packet[4] = (byte)(length & 0xFF);
            Buffer.BlockCopy(jsonBytes, 0, packet, 5, jsonBytes.Length);
            return packet;
        }
        public static byte[] SearalizeGetPlayersInRoomRequest(int id)
        {
            string json = "{"
                        + "\"roomId\":\"" + id + "\""
                        + "}";
            byte[] jsonBytes = Encoding.UTF8.GetBytes(json);
            int length = jsonBytes.Length;
            byte[] packet = new byte[1 + 4 + jsonBytes.Length];
            packet[0] = GET_PLAYERS_IN_ROOM_CODE;
            packet[1] = (byte)((length >> 24) & 0xFF);
            packet[2] = (byte)((length >> 16) & 0xFF);
            packet[3] = (byte)((length >> 8) & 0xFF);
            packet[4] = (byte)(length & 0xFF);
            Buffer.BlockCopy(jsonBytes, 0, packet, 5, jsonBytes.Length);
            return packet;
        }
        /*
         this is for simple requests that are for just showing something simple like statistics or all time winners and it goes on the same build of all the others
        input: code
        output: packet
        */
        public static byte[] SerializeSimpleRequest(byte code)
        {
            string json = "{}";
            byte[] jsonBytes = Encoding.UTF8.GetBytes(json);
            int length = jsonBytes.Length;
            byte[] packet = new byte[1 + 4 + jsonBytes.Length];
            packet[0] = code;
            packet[1] = (byte)((length >> 24) & 0xFF);
            packet[2] = (byte)((length >> 16) & 0xFF);
            packet[3] = (byte)((length >> 8) & 0xFF);
            packet[4] = (byte)(length & 0xFF);
            Buffer.BlockCopy(jsonBytes, 0, packet, 5, jsonBytes.Length);
            return packet;
        }
        public static byte[] SerializeSumbitAnswerRequest(int Answerid)
        {
            string json = "{"
                        + "\"answerId\":\"" + Answerid + "\""
                        + "}";
            byte[] jsonBytes = Encoding.UTF8.GetBytes(json);
            int length = jsonBytes.Length;
            byte[] packet = new byte[1 + 4 + jsonBytes.Length];
            packet[0] = SUBMIT_ANSWER_CODE;
            packet[1] = (byte)((length >> 24) & 0xFF);
            packet[2] = (byte)((length >> 16) & 0xFF);
            packet[3] = (byte)((length >> 8) & 0xFF);
            packet[4] = (byte)(length & 0xFF);
            Buffer.BlockCopy(jsonBytes, 0, packet, 5, jsonBytes.Length);
            return packet;
        }

    }
}
