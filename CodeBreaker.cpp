// Instructions
//
// Implement a code breaking game as a console application. At the beginning of the game, it randomly generates a secret 4 digit code.
// Each digit should be unique. For example, 0123, 9548, and 3217 are valid codes, but 1102, 4445, and 7313 are not.
//
// Once the number has been generated, the player has 8 chances to guess the code. After each guess, the game should report how many digits
// they guessed correctly, and how many had the right number, but in the wrong place. For example, with a secret code of 0123, a guess of 0451
// has one digit correct (the 0), and one digit with the right number in the wrong place (the 1).
//
// After the player has either correctly broken the code or run out of guesses, ask if they would like to play again. If so, generate a new
// code and start again, otherwise exit the program.
//
// If the player enters a code that isn't valid, prompt them to try again but do NOT count the invalid guess as one of the 8 attempts. Also be sure that
// your game can handle any unexpected inputs without crashing.
//
// All your code should go in this file. Please do NOT include your name, as we anonymize tests during evaluation to reduce bias.
//
// Good luck!

#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <ctype.h>
using namespace std;

// Forward delarations
class GameSession;
string VectorToString(vector<int> input_vector);
string StringToLowercase(string user_input);
string NounFormOfAttempt(GameSession current_session);
vector<int> CreateRandomCode();
void GameStart();
void MainMenu(GameSession current_session);
void InputCheck(GameSession current_session, string user_input);
void NonDigitInputError(GameSession current_session);
void InputLengthError(GameSession current_session, string length_descriptor);
void DuplicateDigitError(GameSession current_session);
string GenerateHints(GameSession current_session, string user_input);
void IncorrectGuessError(GameSession current_session, string hint_string);
void EndGameFail(GameSession current_session);
void EndGameSuccess(GameSession current_session);

class GameSession
{
    // This class holds the data for any single game session, including the randomly generated code and number of remaining attempts.
public:
    vector<int> code = CreateRandomCode();
    int attempts = 8;
};

string VectorToString(vector<int> input_vector)
{
    // Declare a string variable.
    string output_string = "";

    // For each entry of the input vector, cast it to a string and concatenate it to the end of the string variable we just created.
    for (int i = 0; i < input_vector.size(); ++i)
    {
        output_string += to_string(input_vector[i]);
    }

    return output_string;
}

string StringToLowercase(string user_input)
{
    // Declare a string variable.
    string output_string = "";

    // For each index of the user's input, convert the currently iterated char to lowercase and add it to the created string variable.
    for (int i = 0; i < user_input.length(); ++i)
    {
        output_string += (unsigned char)tolower(user_input[i]);
    }

    return output_string;
}

string NounFormOfAttempt(GameSession current_session)
{
    // Declare a string variable to use for error messages.
    // We want to know whether to use the singular or plural form of the word "attempt" based on the user's progress, so the correct form will be stored here.
    string noun_form_of_attempt = "";

    // Create a switch case to determine the correct form of the word.
    switch (current_session.attempts)
    {
    case 1:
        noun_form_of_attempt = "attempt";
        break;
    default:
        noun_form_of_attempt = "attempts";
    }

    return noun_form_of_attempt;
}

vector<int> CreateRandomCode()
{
    // Create a unique seed of the random number algorithm.
    srand((unsigned int)time(0));

    // Declare an int vector.
    // This is the vector we will return at the end of the method that will contain our randomized code.
    vector<int> secret_code;

    // Create a loop that iterates for as long as the int vector size is under four.
    // We could be looping for an undetermined amount of time, but once the vector contains four digits, that's our stopping place.
    for (int i = 0; secret_code.size() < 4; ++i)
    {
        // Now, here come the conditions to fill the vector.
        // Create a randomized int between the values 0 and 9.
        int random_num = rand() % 10;

        // If the random number generated is not already in the vector, add it to the vector.
        // If not, skip it. We'll try again on the next loop.
        if (find(secret_code.begin(), secret_code.end(), random_num) == secret_code.end())
        {
            secret_code.push_back(random_num);
        }
    }

    return secret_code;
}

void GameStart()
{
    // Create a new GameSession object that holds this current game's randomized code and number of allowed attempts.
    GameSession new_session;

    // After the session data has been generated, go to the game's main menu.
    MainMenu(new_session);
    return;
}

void MainMenu(GameSession current_session)
{
    // Declare a string variable for the user's upcoming input.
    string user_input = "";

    // Send a message to the console.
    cout << "\n"
            "CODE BREAKER\n"
            "Crack the secret code to win...\n"
            "\n"
            "Rules:\n"
            "- The code is a 4-digit number.\n"
            "- Each digit in the code is unique.\n"
            "\n"
            "You have "
         << current_session.attempts << " " << NounFormOfAttempt(current_session) << " remaining.\n";

    // Take in the user's input.
    cin >> user_input;

    // Check the user's input for validity.
    InputCheck(current_session, user_input);
    return;
}

void InputCheck(GameSession current_session, string user_input)
{
    // Time to check the validity of the user's input! First, let's check its size.
    // If the number of characters in the user's input is less than 4, send an error message and return.
    if (user_input.length() < 4)
    {
        // We want to specify the user has "too few" characters in their input.
        InputLengthError(current_session, "few");
        return;
    }
    // If the number of characters in the user's input is greater than 4, send an error message and return.
    else if (user_input.length() > 4)
    {
        // We want to specify the user has "too many" characters in their input.
        InputLengthError(current_session, "many");
        return;
    }

    // Now that we've ensured we have the right input length, let's check to see if the input is actually a number.
    // For each index of the string, check if the contents of the currently iterated index is a digit.
    for (int i = 0; i < user_input.length(); ++i)
    {
        // If one of the indicies doesn't contain a digit, send an error message and return.
        if (isdigit(user_input.at(i)) == false)
        {
            NonDigitInputError(current_session);
            return;
        }
    }

    // Now, let's go through one more looping session.
    // This time, we want to check if the user entered any duplicate digits.
    // Create a for loop that iterates through every index of the user's input string.
    for (int i = 0; i < user_input.length(); ++i)
    {
        // We'll want to create a secondary nested loop that also iterates through every index of the user's input string.
        // We'll be comparing values in the string.
        for (int j = 0; j < user_input.length(); ++j)
        {
            // If the values of both loops are on the same index, do nothing.
            // This is a guaranteed match, but we aren't interested in it since we're looking for duplicate values.
            if (i == j)
            {
                // Do nothing
            }
            else
            {
                // Compare the contents of the first iterator's index with the contents of the second iterator's index.
                // If the two values match, we found a duplicate digit in the user's input.
                if (user_input[i] == user_input[j])
                {
                    // Send an error message and return.
                    DuplicateDigitError(current_session);
                    return;
                }
            }
        }
    }

    // If the code makes it this far, it's a perfectly valid syntax! Here comes the final check.
    // Compare the user's input with the randomly generated code for this game session.
    // If they match, the user won the game!
    if (user_input == VectorToString(current_session.code))
    {
        // Send the user to the win screen and return.
        EndGameSuccess(current_session);
        return;
    }
    // If they don't match, the user made an incorrect guess.
    else
    {
        // Decrement the number of attempts for this session by 1.
        current_session.attempts -= 1;

        // If the number of attempts for the session remaining is greater than 0, the game is still ongoing.
        if (current_session.attempts > 0)
        {
            // Send the user to the "Incorrect Guess" menu and create hints to help them on the next attempt.
            IncorrectGuessError(current_session, GenerateHints(current_session, user_input));
        }
        // Else, the user lost the game.
        else
        {
            // Send the user to the game over menu.
            EndGameFail(current_session);
        }

        return;
    }
}

void NonDigitInputError(GameSession current_session)
{
    // Declare a string variable for the user's upcoming input.
    string user_input = "";

    // Send a message to the console.
    cout << "\n"
            "--Invalid Input--\n"
            "A non-numerical character was entered in your code. Please try again.\n"
            "\n"
            "You still have "
         << current_session.attempts << " " << NounFormOfAttempt(current_session) << " remaining.\n";

    // Take in the user's input.
    cin >> user_input;

    // Check the user's input for validity.
    InputCheck(current_session, user_input);
}

void InputLengthError(GameSession current_session, string length_descriptor)
{
    // Declare a string variable for the user's upcoming input.
    string user_input = "";

    // Send a message to the console.
    cout << "\n"
            "--Invalid Input--\n"
            "There are too "
         << length_descriptor << " characters in your code. Please enter exactly 4 digits and try again.\n"
                                 "\n"
                                 "You still have "
         << current_session.attempts << " " << NounFormOfAttempt(current_session) << " remaining.\n";

    // Take in the user's input.
    cin >> user_input;

    // Check the user's input for validity.
    InputCheck(current_session, user_input);
}

void DuplicateDigitError(GameSession current_session)
{
    // Declare a string variable for the user's upcoming input.
    string user_input = "";

    // Send a message to the console.
    cout << "\n"
            "--Invalid Input--\n"
            "A duplicate digit was found in your code. Please make sure all 4 digits are unique and try again.\n"
            "\n"
            "You still have "
         << current_session.attempts << " " << NounFormOfAttempt(current_session) << " remaining.\n";

    // Take in the user's input.
    cin >> user_input;

    // Check the user's input for validity.
    InputCheck(current_session, user_input);
}

string GenerateHints(GameSession current_session, string user_input)
{
    // Declare an empty string variable. This is where our return value will be held.
    string hint_string = "";

    // Convert the current session's randomly generated code to a string.
    string current_session_code = VectorToString(current_session.code);

    // Declare three int variables. These will act as counters to keep track of needed statistics:
    // How many characters in the user's input match the same digit and placement in the session's code.
    // How many digits in the user's input are somewhere in the session's code.
    // How many digits the user has completely incorrect. This is initialized to 4 by default.
    int exact_match_count = 0;
    int misplaced_match_count = 0;
    int incorrect_digits_count = 4;

    // First, let's check how many digits in the user's input are a perfect match to this session's code.
    // For each index of the user's input, compare its contents to the same index of the session's code.
    for (int i = 0; i < user_input.length(); ++i)
    {
        // If the contents of both indicies match, increment the "exact match" counter by one.
        if (user_input[i] == current_session_code[i])
        {
            exact_match_count++;
        }
    }

    // Next, check if the user input digits that are in the session's code, but maybe in the wrong place.
    // Create a loop to iterate through each index of the user's input string.
    for (int i = 0; i < user_input.length(); ++i)
    {
        // Create a secondary loop to iterate through each index of the session's code.
        for (int j = 0; j < current_session_code.length(); ++j)
        {
            // If the contents of both indices for the user's input and the session's code match, they share the same digit somewhere!
            // This will also pick up any "exact match" digits we previously found.
            if (user_input[i] == current_session_code[j])
            {
                misplaced_match_count++;
            }
        }
    }

    // To get the proper number of misplaced digits, subtract the number of exact match digits from the current total.
    misplaced_match_count = misplaced_match_count - exact_match_count;

    // To get the number of incorrent digits remaining, subtract the number of misplaced digits and the number of exact match digits from the current total.
    incorrect_digits_count = incorrect_digits_count - misplaced_match_count - exact_match_count;

    // Now that we have our stats, time to construct the user message!
    // If the number of exact match digits is greater than 0, add a hint detailing how many are present.
    if (exact_match_count > 0)
    {
        // Create a switch case to account for singular and plural forms.
        switch (exact_match_count)
        {
        case 1:
            hint_string += to_string(exact_match_count) + " digit is correct.\n";
            break;
        default:
            hint_string += to_string(exact_match_count) + " digits are correct.\n";
        }
    }
    // If the number of misplaced digits is greater than 0, add a hint detailing how many are present.
    if (misplaced_match_count > 0)
    {
        switch (misplaced_match_count)
        {
        // Create a switch case to account for singular and plural forms.
        case 1:
            hint_string += to_string(misplaced_match_count) + " digit is in the code but in the wrong place.\n";
            break;
        default:
            hint_string += to_string(misplaced_match_count) + " digits are in the code but in the wrong place.\n";
        }
    }
    // If the number of incorrect digits is greater than 0, add a hint detailing how many are present.
    if (incorrect_digits_count > 0)
    {
        switch (incorrect_digits_count)
        {
        // Create a switch case to account for singular and plural forms.
        case 1:
            hint_string += to_string(incorrect_digits_count) + " digit is incorrect.\n";
            break;
        default:
            hint_string += to_string(incorrect_digits_count) + " digits are incorrect.\n";
            break;
        }
    }

    return hint_string;
}

void IncorrectGuessError(GameSession current_session, string hint_string)
{
    // Declare a string variable for the user's upcoming input.
    string user_input = "";

    // Send a message to the console.
    cout << "\n"
            "--Attempt Failed--\n"
            "\n"
            "Hints:\n" +
                hint_string +
                "\n"
                "You have "
         << current_session.attempts << " " << NounFormOfAttempt(current_session) << " remaining.\n";

    // Take in the user's input.
    cin >> user_input;

    // Check the user's input for validity.
    InputCheck(current_session, user_input);
}

void EndGameFail(GameSession current_session)
{
    // Declare a string variable for the user's upcoming input.
    string user_input = "";

    // Send a message to the console.
    cout << "\n"
            "All Attempts Failed...\n"
            "The correct code was "
         << VectorToString(current_session.code) << ".\n"
                                                    "\n"
                                                    "Would you like to play again? (Y/N)\n";

    // Take in the user's input.
    cin >> user_input;

    // Create an infinite loop.
    // We'll want to check for certain actions and keep looping if the conditions fail.
    while (1)
    {
        // Turn the user's input into a lowercase string.
        user_input = StringToLowercase(user_input);

        // If the user's input equates to 'y' or "yes", start up a new game session.
        if (user_input == "y" || user_input == "yes")
        {
            GameStart();
            return;
        }
        // If the user's input equates to 'n' or "no", close the program.
        else if (user_input == "n" || user_input == "no")
        {
            exit(0);
        }
        // Else, ask for the input again.
        else
        {
            cin >> user_input;
        }
    }
}

void EndGameSuccess(GameSession current_session)
{
    // Declare a string variable for the user's upcoming input.
    string user_input = "";

    // Send a message to the console.
    cout << "\n"
            "Code Break Successful!\n"
            "The secret code was "
         << VectorToString(current_session.code) << ".\n"
                                                    "\n"
                                                    "Would you like to play again? (Y/N)\n";

    // Take in the user's input.
    cin >> user_input;

    // Create an infinite loop.
    // We'll want to check for certain actions and keep looping if the conditions fail.
    while (1)
    {
        // Turn the user's input into a lowercase string.
        user_input = StringToLowercase(user_input);

        // If the user's input equates to 'y' or "yes", start up a new game session.
        if (user_input == "y" || user_input == "yes")
        {
            GameStart();
            return;
        }
        // If the user's input equates to 'n' or "no", close the program.
        else if (user_input == "n" || user_input == "no")
        {
            exit(0);
        }
        // Else, ask for the input again.
        else
        {
            cin >> user_input;
        }
    }
}

int main()
{
    GameStart();
    return 0;
}