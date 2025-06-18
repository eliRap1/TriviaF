#pragma once
#include "Question.h"

struct GameData {
    GameData();

    Question currentQuestion;
    unsigned int correctAnswerCount = 0;
    unsigned int wrongAnswerCount = 0;
    unsigned int averageAnswerTime = 0;
};