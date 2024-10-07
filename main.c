#include <SDL.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <string.h>

#define MAX_LINES 100
#define MAX_LINE_LENGTH 256
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define FONT_SIZE 24
#define FONT_PATH "/Users/nikitavoitik/Documents/prog/cpp/TextEditor/Arial.ttf"

// Function Prototypes
int init(SDL_Window **window, SDL_Renderer **renderer, TTF_Font **font);

void cleanup(SDL_Window *window, SDL_Renderer *renderer, TTF_Font *font);

void renderText(SDL_Renderer *renderer, TTF_Font *font, char lines[MAX_LINES][MAX_LINE_LENGTH], int cursor_pos,
                int current_line, int x, int y);

void handleTextInput(char lines[MAX_LINES][MAX_LINE_LENGTH], const char *input, int *cursor_pos, int current_line);

void handleEnterKey(char lines[MAX_LINES][MAX_LINE_LENGTH], int *current_line, int *cursor_pos, int *line_count);

void handleBackspace(char lines[MAX_LINES][MAX_LINE_LENGTH], int *cursor_pos, int *current_line);

void moveCursorLeft(char lines[MAX_LINES][MAX_LINE_LENGTH], int *cursor_pos, int *current_line);

void moveCursorRight(char lines[MAX_LINES][MAX_LINE_LENGTH], int *cursor_pos, int *current_line);

void moveCursorUp(char lines[MAX_LINES][MAX_LINE_LENGTH], int *cursor_pos, int *current_line);

void moveCursorDown(char lines[MAX_LINES][MAX_LINE_LENGTH], int *cursor_pos, int *current_line, int *line_count);

void insertLine(char lines[MAX_LINES][MAX_LINE_LENGTH], int index, int *line_count);

void myprint(int c)
{
    if (isprint(c))
        printf("%c", c); // just print printable characters
    else if (c == '\n')
        printf("\\n"); // display newline as \n
    else
        printf("%02x", c); // print everything else as a number
}


int main() {
    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;
    TTF_Font *font = nullptr;

    // Initialize SDL and TTF
    if (init(&window, &renderer, &font) != 0) {
        return 1;
    }
    char ** lines = new char * [MAX_lINES]
    char lines[MAX_LINES][MAX_LINE_LENGTH] = {{0}};
    for (int i = 0; i < MAX_LINES; i++) {
        strncpy(lines[i], "            \0", 5);
    }
    int cursor_pos = 0;       // Cursor position within the current line
    int current_line = 0;     // The current line where the cursor is located
    int line_count = 1;       // Number of lines (start with 1)

    SDL_bool done = SDL_FALSE;
    SDL_StartTextInput();  // Start capturing text input

    while (!done) {
        SDL_Event event;
        if (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    done = SDL_TRUE;
                    break;

                case SDL_TEXTINPUT:
                    handleTextInput(lines, event.text.text, &cursor_pos, current_line);
                    break;

                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
                        case SDLK_LEFT:
                            moveCursorLeft(lines, &cursor_pos, &current_line);
                            break;

                        case SDLK_RIGHT:
                            moveCursorRight(lines, &cursor_pos, &current_line);
                            break;

                        case SDLK_BACKSPACE:
                            handleBackspace(lines, &cursor_pos, &current_line);
                            break;

                        case SDLK_RETURN:
                            handleEnterKey(lines, &current_line, &cursor_pos, &line_count);
                            break;

                        case SDLK_UP:
                            moveCursorUp(lines, &cursor_pos, &current_line);
                            break;

                        case SDLK_DOWN:
                            moveCursorDown(lines, &cursor_pos, &current_line, &line_count);
                            break;
                    }
                    break;
            }
        }

        // Clear the screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black background
        SDL_RenderClear(renderer);

        // Render final text along with the cursor
        renderText(renderer, font, lines, cursor_pos, current_line, 50, 50);

        // Present the rendered content to the screen
        SDL_RenderPresent(renderer);
    }

    // Cleanup resources
    cleanup(window, renderer, font);
    return 0;
}

/**
 * Initialize SDL, SDL_ttf, and create window, renderer, and font.
 */
int init(SDL_Window **window, SDL_Renderer **renderer, TTF_Font **font) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    if (TTF_Init() == -1) {
        printf("TTF_Init Error: %s\n", TTF_GetError());
        SDL_Quit();
        return 1;
    }

    *window = SDL_CreateWindow("SDL2 Text Input", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH,
                               WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (!*window) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
    if (!*renderer) {
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(*window);
        SDL_Quit();
        return 1;
    }

    *font = TTF_OpenFont(FONT_PATH, FONT_SIZE);
    if (!*font) {
        printf("TTF_OpenFont Error: %s\n", TTF_GetError());
        SDL_DestroyRenderer(*renderer);
        SDL_DestroyWindow(*window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    return 0;
}

/**
 * Cleanup SDL, SDL_ttf, window, and renderer.
 */
void cleanup(SDL_Window *window, SDL_Renderer *renderer, TTF_Font *font) {
    SDL_StopTextInput();
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
}

/**
 * Render the given text and the cursor at the specified position.
 */
void renderText(SDL_Renderer *renderer, TTF_Font *font, char lines[MAX_LINES][MAX_LINE_LENGTH], int cursor_pos,
                int current_line, int x, int y) {
    SDL_Color white = {255, 255, 255, 255};  // White color for the text
    int cursor_x = x;
    int cursor_y = y;
    int last_line = 0;
    for (int i = 0; i < MAX_LINES; i++) {
        if (lines[i][0] != '\0') {
            last_line = i;
        }
    }

    for (int i = 0; i <= last_line && i < MAX_LINES; i++) {

        char newString[MAX_LINE_LENGTH];
        strcpy(newString, lines[i]);
        //printf("123");
        // If the string is empty, add a space
        if (newString[0] == '\0') {
            strcpy(newString, " \0");
        }
        SDL_Surface *surfaceMessage = TTF_RenderText_Solid(font, newString, white);
        if (!surfaceMessage) {
            printf("Text render error: %s\n", TTF_GetError());
            return;
        }

        SDL_Texture *messageTexture = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
        if (!messageTexture) {
            printf("Texture creation error: %s\n", SDL_GetError());
            SDL_FreeSurface(surfaceMessage);
            return;
        }

        SDL_Rect messageRect = {x, y + i * surfaceMessage->h, surfaceMessage->w, surfaceMessage->h};
        SDL_RenderCopy(renderer, messageTexture, nullptr, &messageRect);

        // Render cursor (a simple vertical bar) at the right position
        if (i == current_line) {
            if (cursor_pos > 0) {
                char temp[MAX_LINE_LENGTH] = {0};
                //printf("%s\n", newString);
                strncpy(temp, &newString[i], cursor_pos);
                SDL_Surface *surfaceCursor = TTF_RenderText_Solid(font, temp, white);
                cursor_x = x + surfaceCursor->w;
                SDL_FreeSurface(surfaceCursor);
            }
            cursor_y = y + i * surfaceMessage->h;
        }

        SDL_FreeSurface(surfaceMessage);
        SDL_DestroyTexture(messageTexture);
    }

    // Draw the cursor
    SDL_Rect cursorRect = {cursor_x, cursor_y, 2, FONT_SIZE};
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White cursor
    SDL_RenderFillRect(renderer, &cursorRect);
}

/**
 * Handle text input events and insert the new input at the cursor position.
 */
void handleTextInput(char lines[MAX_LINES][MAX_LINE_LENGTH], const char *input, int *cursor_pos, int current_line) {
    int len = strlen(lines[current_line]);
    int input_len = strlen(input);

    if (len + input_len < MAX_LINE_LENGTH) {
        // Move text after cursor to the right to make space for new input
        memmove(lines[current_line] + *cursor_pos + input_len, lines[current_line] + *cursor_pos,
                len - *cursor_pos + 1);
        // Insert new input at cursor position
        memcpy(lines[current_line] + *cursor_pos, input, input_len);
        *cursor_pos += input_len;  // Move cursor after inserted text
    } else {
        printf("Line buffer is full!\n");
    }
}

/**
 * Handle Enter key event and create a new line.
 */
void handleEnterKey(char lines[MAX_LINES][MAX_LINE_LENGTH], int *current_line, int *cursor_pos, int *line_count) {
    // Ensure we don't exceed the maximum number of lines
    if (*line_count >= MAX_LINES - 1) {
        return;
    }
    if (*current_line >= MAX_LINES - 1) {
        return;  // Prevent accessing an out-of-bound line
    }

    // Insert a new blank line after the current line
    insertLine(lines, *current_line + 1, line_count);

    // Move the text after the cursor to the new line
    strncpy(lines[*current_line + 1], lines[*current_line] + *cursor_pos, MAX_LINE_LENGTH - 1);
    lines[*current_line + 1][MAX_LINE_LENGTH - 1] = '\0';  // Ensure null-termination for the new line
    lines[*current_line][*cursor_pos] = '\0';  // Terminate the current line at the cursor

    // Move the cursor to the new line
    *cursor_pos = 0;
    (*line_count)++;
    moveCursorDown(lines, cursor_pos, current_line, line_count);  // No need for &* here
}


void insertLine(char lines[MAX_LINES][MAX_LINE_LENGTH], int index, int *line_count) {
    // Shift lines down to make space for the new line
    for (int i = *line_count; i > index; i--) {
        strcpy(lines[i], lines[i - 1]);  // Safely copy existing lines down
    }

    // Clear the new line by setting it to an empty string
    memset(lines[index], 0, MAX_LINE_LENGTH);  // Zero out the new line
}

void removeLine(char lines[MAX_LINES][MAX_LINE_LENGTH], int index, int *line_count) {
    // Check if the index is valid
    if (index < 0 || index >= *line_count) {
        return;
    }

    // Move lines up to fill the gap
    for (int i = index; i < *line_count - 1; i++) {
        strcpy(lines[i], lines[i + 1]);
    }

    // Clear the last line
    lines[*line_count - 1][0] = '\0';

    // Decrement the line count
    (*line_count)--;
}


/**
 * Handle backspace key event, removing the character before the cursor.
 */
void handleBackspace(char lines[MAX_LINES][MAX_LINE_LENGTH], int *cursor_pos, int *current_line) {
    if (*cursor_pos > 0) {
        int len = strlen(lines[*current_line]);
        memmove(lines[*current_line] + *cursor_pos - 1, lines[*current_line] + *cursor_pos, len - *cursor_pos + 1);
        (*cursor_pos)--;
    } else if (*current_line > 0) {
        // Merge current line with the previous one
        int prev_len = strlen(lines[*current_line - 1]);
        int cur_len = strlen(lines[*current_line]);

        if (prev_len + cur_len < MAX_LINE_LENGTH) {
            strcat(lines[*current_line - 1], lines[*current_line]);
            (*current_line)--;
            *cursor_pos = prev_len;
        }
    }
}

/**
 * Move cursor to the left.
 */
void moveCursorLeft(char lines[MAX_LINES][MAX_LINE_LENGTH], int *cursor_pos, int *current_line) {
    if (*cursor_pos > 0) {
        (*cursor_pos)--;
    } else if (*current_line > 0) {
        (*current_line)--;
        *cursor_pos = strlen(lines[*current_line]);
    }
}

/**
 * Move cursor to the right.
 */
void moveCursorRight(char lines[MAX_LINES][MAX_LINE_LENGTH], int *cursor_pos, int *current_line) {
    int len = strlen(lines[*current_line]);

    printf("current line: %d, cursor_pos: %d, len: %d '%s'\n", *current_line, *cursor_pos, len, lines[*current_line]);
    for (int i = 0; i < strlen(lines[*current_line]); i++){
        myprint(lines[*current_line][i]);
    }
    if (*cursor_pos < len) {
        (*cursor_pos)++;
    } else if (*current_line < MAX_LINES - 1 && lines[*current_line + 1][0] != '\0') {
        (*current_line)++;
        (*cursor_pos) = 0;
    }
}


void moveCursorUp(char lines[MAX_LINES][MAX_LINE_LENGTH], int *cursor_pos, int *current_line) {
    if (*current_line <= 0) {
        *current_line = 0;
        return;
    }
    (*current_line)--;
    int len = strlen(lines[*current_line]);
    if (*cursor_pos >= len) {
        *cursor_pos = len;
    }
}

void moveCursorDown(char lines[MAX_LINES][MAX_LINE_LENGTH], int *cursor_pos, int *current_line, int *line_count) {
    printf("current line: %d, line_count: %d\n", *current_line, *line_count);
    if (*current_line >= *line_count - 1) {
        *current_line = *line_count - 1;
        return;
    }

    (*current_line)++;
    int len = strlen(lines[*current_line]);
    if (*cursor_pos >= len) {
        *cursor_pos = len;
    }
}
