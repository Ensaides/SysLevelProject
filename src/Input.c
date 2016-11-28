#include "Library.h"
#include "GUI.h"
#include "Input.h"

void OpenBookMenu()
{
    CreateAddBookWindow();
    SetPromptText("Title:");

    bool EnteringAuthor = false;
    bool Finished = false;
    LibraryRecord NewRecord;
    memset(&NewRecord, 0, sizeof(NewRecord));

    while (true)
    {
        if (EnteringAuthor)
        {
            SetPromptText("Author:");
        }
        else
        {
            SetPromptText("Title:");
        }

        char* Input = GetUserInput();

        if (strlen(Input) == 1)
        {
            switch(Input[0])
            {
                case 'b':
                    // Return to menu
                    RemoveAddBookWindow();
                    SetPromptText("#");
                    return;
                    break;

                case 't':
                    // Retry key, reset all the data and menu
                    memset(&NewRecord.Title[0], 0, sizeof(NewRecord.Title));
                    memset(&NewRecord.Author[0], 0, sizeof(NewRecord.Author));
                    SetAddBookMenuRecord(NewRecord);
                    EnteringAuthor = false;
                    break;

                default:
                    PrintMessage("Unrecognized input!");
                    break;
            }
        }
        else if (strlen(Input) > 1)
        {
            if (!EnteringAuthor)
            {
                // Copy over the string
                memcpy(&NewRecord.Title[0], Input, strlen(Input));
            }
            else
            {
                // Copy over the string
                memcpy(&NewRecord.Author[0], Input, strlen(Input));
                Finished = true;
            }

            EnteringAuthor = !EnteringAuthor;

            SetAddBookMenuRecord(NewRecord);
        }

        if (Input)
        {
            free(Input);
            Input = NULL;
        }

        if (Finished)
        {
            AddBook(NewRecord);
            RemoveAddBookWindow();

            SetPromptText("#");

            // Print out the successful message
            char Buffer[256];
            snprintf(Buffer, 256, "Added book '%s' by %s.", NewRecord.Title, NewRecord.Author);
            PrintMessage(&Buffer[0]);
            return;
        }
    }
}

bool IsStringNumber(const char* Input)
{
    if (strlen(Input) == 0)
        return false;

    for (int i = 0; i < strlen(Input); i++)
    {
        if (Input[i] < '0' || Input[i] > '9')
            return false;
    }

    return true;
}

void OpenDeleteBookMenu()
{
    CreateDeleteBookWindow();
    SetPromptText("Book ID:");

    int BookID = -1;
    bool ConfirmDeleteBook = false;

    while (true)
    {
        char* Input = GetUserInput();

        if (strlen(Input) > 0)
        {
            if (IsStringNumber(Input))
            {
                BookID = atoi(Input);

                SetDeleteBookMenuBookID(BookID);

                if (BookExists(BookID))
                {
                    char Buffer[256];
                    snprintf(Buffer, 256, "Are you sure you want to delete book '%s'? (Y/N)", GetBookName(BookID));
                    PrintMessage(&Buffer[0]);
                    ConfirmDeleteBook = true;
                }
                else
                {
                    char Buffer[256];
                    snprintf(Buffer, 256, "Error: Book number %d doesn't exist!", BookID);
                    PrintMessage(&Buffer[0]);
                }
            }
            else if (strlen(Input) == 1)
            {
                switch(Input[0])
                {
                    case 'b':
                        // Return to menu
                        RemoveDeleteBookWindow();
                        SetPromptText("#");
                        return;
                        break;

                    case 't':
                        // Retry key, reset all the data and menu
                        BookID = -1;
                        SetDeleteBookMenuBookID(BookID);
                        ConfirmDeleteBook = false;
                        SetPromptText("Book ID:");
                        break;

                    case 'y':
                        if (ConfirmDeleteBook)
                        {
                            DeleteBook(BookID);
                            RemoveDeleteBookWindow();
                            SetPromptText("#");
                            return;
                            break;
                        }

                    case 'n':
                        if (ConfirmDeleteBook)
                        {
                            BookID = -1;
                            SetDeleteBookMenuBookID(BookID);
                            ConfirmDeleteBook = false;
                            SetPromptText("Book ID:");
                            break;
                        }

                    default:
                        PrintMessage("Unrecognized input!");
                        break;
                }
            }
            else
            {
                PrintMessage("Unrecognized input!");
            }
        }

        if (Input)
        {
            free(Input);
            Input = NULL;
        }
    }
}

void OpenCheckOutWindow()
{
    CreateCheckOutWindow();
    SetPromptText("Username:");

    int BookID = -1;
    char* Username = NULL;
    while (true)
    {
        char* Input = GetUserInput();
        if (strlen(Input) > 0)
        {
            // If we have a username then we're inputing the book id
            if (Username && IsStringNumber(Input))
            {
                BookID = atoi(Input);

                if (BookExists(BookID))
                {
                    SetCheckOutWindowBookID(BookID);

                    CheckOutBook(Username, BookID);

                    if (Username)
                    {
                        free(Username);
                        Username = NULL;
                    }

                    RemoveCheckOutWindow();
                    SetPromptText("#");

                    char* BookName = GetBookName(BookID);
                    char Buffer[256];
                    snprintf(Buffer, 256, "Book '%s' checked out successfully!", BookName);
                    PrintMessage(&Buffer[0]);
                    free(BookName);
                    return;
                }
                else
                {
                    char Buffer[256];
                    snprintf(Buffer, 256, "Error: Book number %d doesn't exist!", BookID);
                    PrintMessage(&Buffer[0]);
                }
            }
            else if (strlen(Input) == 1)
            {
                switch(Input[0])
                {
                    case 'b':
                        // Return to menu
                        if (Username)
                        {
                            free(Username);
                            Username = NULL;
                        }
                        BookID = -1;
                        RemoveCheckOutWindow();
                        SetPromptText("#");
                        return;
                        break;

                    case 't':
                        // Retry key, reset all the data and menu
                        if (Username)
                        {
                            free(Username);
                            Username = NULL;
                        }
                        BookID = -1;
                        SetCheckOutWindowUsername(Username);
                        SetPromptText("Username:");
                        break;

                    default:
                        PrintMessage("Unrecognized input!");
                        break;
                }
            }
            else if (Username == NULL)
            {
                Username = strdup(Input);

                if (UserExists(Username))
                {
                    SetCheckOutWindowUsername(Username);

                    if (GetNumBooksCheckedOut(Username) < 3)
                    {
                        SetPromptText("Book ID:");
                    }
                    else
                    {
                        SetCheckOutWindowUsername(NULL);
                        char Buffer[256];
                        snprintf(Buffer, 256, "Error: User %s has 3 or more books checked out!", Username);
                        PrintMessage(&Buffer[0]);

                        if (Username)
                        {
                            free(Username);
                            Username = NULL;
                        }
                    }
                }
                else
                {
                    SetCheckOutWindowUsername(NULL);
                    char Buffer[256];
                    snprintf(Buffer, 256, "Error: User %s doesn't exist!", Username);
                    PrintMessage(&Buffer[0]);

                    if (Username)
                    {
                        free(Username);
                        Username = NULL;
                    }
                }
            }
        }

        if (Input)
        {
            free(Input);
            Input = NULL;
        }
    }

    if (Username)
    {
        free(Username);
        Username = NULL;
    }
}