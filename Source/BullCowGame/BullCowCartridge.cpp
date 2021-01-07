#include "BullCowCartridge.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();
   
    const FString WordListPath = FPaths::ProjectContentDir() / TEXT("WordLists/HiddenWordList.txt");
    FFileHelper::LoadFileToStringArrayWithPredicate(ValidWords, *WordListPath, [](const FString& Word) { return IsValidWord(Word); });
    
    
    //PrintLine(TEXT("The number of valid words is %i"), ValidWords.Num());
    SetupGame();
     
}

void UBullCowCartridge::OnInput(const FString& Input) // When the player hits enter
{
    if (bGameOver) {
        ClearScreen();
        SetupGame();
    }
    else
    {
        ProcessGuess(Input);
    }
}

void UBullCowCartridge::SetupGame() 
{
    HiddenWord = ValidWords[FMath::RandRange(0,ValidWords.Num()-1)];
    Lives = HiddenWord.Len();
    bGameOver = false;
    PrintLine(TEXT("Welcome to Bull Cows! \nGuess the %i letter word..."),HiddenWord.Len());
   /* PrintLine(TEXT("The hidden word is %s"), *HiddenWord);*/
}

void UBullCowCartridge::EndGame() {
    bGameOver = true;
    PrintLine(TEXT("Press Enter to play again"));
}

void UBullCowCartridge::ProcessGuess(const FString& Guess) {
    
    
    if (Guess.Equals(HiddenWord)) {
        PrintLine(TEXT("Correct!"));
        EndGame();
        return;
    }
    else {
        PrintLine(TEXT("Incorrect"));

        if (Guess.Len() != HiddenWord.Len())
        {
            PrintLine(TEXT("The hidden word is %i letters long, try again"), HiddenWord.Len());
            return;
        }
  
        if (!IsIsogram(Guess))
        {
            PrintLine(TEXT("No repeating letters, guess again"));
        }

        --Lives;
        PrintLine(TEXT("You lost a life, you have %i lives left"), Lives);
        if (Lives <= 0) {
            PrintLine(TEXT("You ran out of lives!"));
            PrintLine(TEXT("The hidden word was %s"), *HiddenWord);
            EndGame();
            return;
        }

        int32 Bulls, Cows;
        GetBullCows(Guess, Bulls, Cows);
        PrintLine(TEXT("You have %i Bulls and %i Cows"), Bulls, Cows);
    }
}

bool UBullCowCartridge::IsIsogram(const FString& Word)
{
    for (int32 i = 0; i < Word.Len(); i++) 
    {
        for (int32 j = i+1; j < Word.Len(); j++) 
        {
            if (Word[i] == Word[j]) 
            {
                return false;
            }
        }
    }
    
    return true;
}

bool UBullCowCartridge::IsValidWord(const FString& Word)
{
    
    if (Word.Len() > 3 && Word.Len() < 8 && IsIsogram(Word))
    {

        return true;
    }
    else
    {
        return false;
    }

    
}

void UBullCowCartridge::GetBullCows(const FString& Guess, int32& Bulls, int32& Cows) const
{
    Bulls = 0;
    Cows = 0;

    for (int32 i = 0; i < Guess.Len(); i++)
    {
        if (Guess[i] == HiddenWord[i])
        {
            Bulls++;
            continue;
        }

        for (int32 HIndex = 0; HIndex < HiddenWord.Len(); HIndex++)
        {
            if (Guess[i]==HiddenWord[HIndex])
            {
                Cows++;
                break;
            }
        }
        
    }
}