#include <SDL.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <string.h>
#include "tinyfiledialogs.h"

#define MAX_LINES 100
#define MAX_LINE_LENGTH 115
#define WINDOW_WIDTH 1710
#define WINDOW_HEIGHT 900
#define FONT_SIZE 24
#define FONT_PATH "../IBMPlexMono-Regular.ttf"
#define SCROLL_SPEED 20


int init(SDL_Window **window, SDL_Renderer **renderer, TTF_Font **font);

void cleanup(SDL_Window *window, SDL_Renderer *renderer, TTF_Font *font);

void renderText(SDL_Renderer *renderer, TTF_Font *font, char lines[MAX_LINES][MAX_LINE_LENGTH], int cursor_pos,
                int current_line, int x, int y, int line_count, int *scroll_offset, int window_height);

void handleTextInput(char lines[MAX_LINES][MAX_LINE_LENGTH], const char *input, int *cursor_pos, int current_line);

void handleEnterKey(char lines[MAX_LINES][MAX_LINE_LENGTH], int *current_line, int *cursor_pos, int *line_count);

void handleBackspace(char lines[MAX_LINES][MAX_LINE_LENGTH], int *cursor_pos, int *current_line, int *line_count);

void moveCursorLeft(char lines[MAX_LINES][MAX_LINE_LENGTH], int *cursor_pos, int *current_line);

void moveCursorRight(char lines[MAX_LINES][MAX_LINE_LENGTH], int *cursor_pos, int *current_line);

void moveCursorUp(char lines[MAX_LINES][MAX_LINE_LENGTH], int *cursor_pos, int *current_line);

void moveCursorDown(char lines[MAX_LINES][MAX_LINE_LENGTH], int *cursor_pos, int *current_line, int line_count);

void insertLine(char lines[MAX_LINES][MAX_LINE_LENGTH], int index, int line_count);

void removeLine(char lines[MAX_LINES][MAX_LINE_LENGTH], int index, int *line_count);

void optLeft(char lines[MAX_LINES][MAX_LINE_LENGTH], int *cursor_pos, int current_line);

void optRight(char lines[MAX_LINES][MAX_LINE_LENGTH], int *cursor_pos, int current_line);

void cmdRight(char lines[MAX_LINES][MAX_LINE_LENGTH], int *cursor_pos, int current_line);

void cmdLeft(int *cursor_pos);

void handleScroll(SDL_Event event, int *scroll_offset);

void SaveDialog(char lines[MAX_LINES][MAX_LINE_LENGTH]);

void OpenDialog(char lines[MAX_LINES][MAX_LINE_LENGTH], int *line_count, int *current_line, int *cursor_pos);


int main() {
    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;
    TTF_Font *font = nullptr;

    if (init(&window, &renderer, &font) != 0) {
        return 1;
    }

    char lines[MAX_LINES][MAX_LINE_LENGTH] = {{0}};
    int cursor_pos = 0;
    int current_line = 0;
    int line_count = 1;
    int scroll_offset = 0;
    SDL_SetWindowMinimumSize(window, WINDOW_WIDTH, WINDOW_HEIGHT);

    SDL_bool done = SDL_FALSE;
    SDL_StartTextInput();

    while (!done) {
        SDL_Event event;
        int window_width, window_height;
        SDL_GetWindowSize(window, &window_width, &window_height);
        if (SDL_PollEvent(&event)) {
            SDL_Keymod mod = SDL_GetModState();
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
                            if (mod & KMOD_ALT) {
                                optLeft(lines, &cursor_pos, current_line);
                            } else if (mod & KMOD_GUI) {
                                cmdLeft(&cursor_pos);
                            } else {
                                moveCursorLeft(lines, &cursor_pos, &current_line);
                            }
                            break;

                        case SDLK_RIGHT:
                            if (mod & KMOD_ALT) {
                                optRight(lines, &cursor_pos, current_line);
                            } else if (mod & KMOD_GUI) {
                                cmdRight(lines, &cursor_pos, current_line);
                            } else {
                                moveCursorRight(lines, &cursor_pos, &current_line);
                            }
                            break;

                        case SDLK_BACKSPACE:
                            handleBackspace(lines, &cursor_pos, &current_line, &line_count);
                            break;

                        case SDLK_RETURN:
                            handleEnterKey(lines, &current_line, &cursor_pos, &line_count);
                            break;

                        case SDLK_UP:
                            moveCursorUp(lines, &cursor_pos, &current_line);
                            break;
                        case SDLK_DOWN:
                            moveCursorDown(lines, &cursor_pos, &current_line, line_count);
                            break;

                        case SDLK_s:
                            if (mod & KMOD_CTRL) {
                                SaveDialog(lines);
                            }
                            break;

                        case SDLK_o:
                            if (mod & KMOD_CTRL) {
                                OpenDialog(lines, &line_count, &current_line, &cursor_pos);
                            }
                            break;
                    }
                    break;
                case SDL_MOUSEWHEEL:
                    handleScroll(event, &scroll_offset);
                    break;
            }
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);
            renderText(renderer, font, lines, cursor_pos, current_line, 50, 50, line_count, &scroll_offset, window_height);
            SDL_RenderPresent(renderer);
        }

    }
    cleanup(window, renderer, font);
    return 0;
}

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

    *window = SDL_CreateWindow("SDL2 Text Input", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                               WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
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

void cleanup(SDL_Window *window, SDL_Renderer *renderer, TTF_Font *font) {
    SDL_StopTextInput();
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
}

void renderText(SDL_Renderer *renderer, TTF_Font *font, char lines[MAX_LINES][MAX_LINE_LENGTH], int cursor_pos,
                int current_line, int x, int y, int line_count, int *scroll_offset, int window_height) {
    SDL_Color white = {255, 255, 255, 255};
    y = y - *scroll_offset;
    int cursor_x = x;
    int cursor_y = y;

    for (int i = 0; i < line_count && i < MAX_LINES; i++) {
        char line_number[10];
        snprintf(line_number, sizeof(line_number), "%d", i + 1);

        SDL_Surface *lineNumberSurface = TTF_RenderText_Solid(font, line_number, white);
        if (!lineNumberSurface) {
            printf("Line number render error: %s\n", TTF_GetError());
            return;
        }

        SDL_Texture *lineNumberTexture = SDL_CreateTextureFromSurface(renderer, lineNumberSurface);
        if (!lineNumberTexture) {
            printf("Line number texture creation error: %s\n", SDL_GetError());
            SDL_FreeSurface(lineNumberSurface);
            return;
        }

        SDL_Rect lineNumberRect = {5, y + i * lineNumberSurface->h, lineNumberSurface->w, lineNumberSurface->h};
        SDL_RenderCopy(renderer, lineNumberTexture, nullptr, &lineNumberRect);

        SDL_FreeSurface(lineNumberSurface);
        SDL_DestroyTexture(lineNumberTexture);

        char newString[MAX_LINE_LENGTH];
        strcpy(newString, lines[i]);
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
        if (messageRect.y + messageRect.h > 0 && messageRect.y < window_height) {
            SDL_RenderCopy(renderer, messageTexture, nullptr, &messageRect);
        }
        SDL_RenderCopy(renderer, messageTexture, nullptr, &messageRect);

        if (i == current_line) {
            if (cursor_pos > 0) {
                char temp[MAX_LINE_LENGTH] = {0};
                strncpy(temp, newString, cursor_pos);
                SDL_Surface *surfaceCursor = TTF_RenderText_Solid(font, temp, white);
                if (surfaceCursor == NULL) {
                    printf("SDL_Init Error: %s\n", SDL_GetError());
                }

                cursor_x = x + surfaceCursor->w;
                SDL_FreeSurface(surfaceCursor);
            }
            cursor_y = y + i * surfaceMessage->h + 4;
            
        }
        SDL_FreeSurface(surfaceMessage);
        SDL_DestroyTexture(messageTexture);
    }

    SDL_Rect cursorRect = {cursor_x, cursor_y, 2, FONT_SIZE};
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &cursorRect);
}

void handleScroll(SDL_Event event, int *scroll_offset) {
    if (event.type == SDL_MOUSEWHEEL) {
        *scroll_offset -= event.wheel.y * SCROLL_SPEED;
        if (*scroll_offset < 0)
            *scroll_offset = 0;
    }
}

void handleTextInput(char lines[MAX_LINES][MAX_LINE_LENGTH], const char *input, int *cursor_pos, int current_line) {
    int len = strlen(lines[current_line]);
    int input_len = strlen(input);

    if (len + input_len >= MAX_LINE_LENGTH) {
        printf("Line buffer is full!\n");
        return;
    }

    memmove(lines[current_line] + *cursor_pos + input_len, lines[current_line] + *cursor_pos, len - *cursor_pos + 1);
    memcpy(lines[current_line] + *cursor_pos, input, input_len);
    *cursor_pos += input_len;
}

void handleEnterKey(char lines[MAX_LINES][MAX_LINE_LENGTH], int *current_line, int *cursor_pos, int *line_count) {
    if (*line_count >= MAX_LINES - 1) {
        return;
    }

    insertLine(lines, *current_line + 1, *line_count);
    memcpy(lines[*current_line + 1], lines[*current_line] + *cursor_pos, (MAX_LINE_LENGTH - 1) * sizeof(char));
    lines[*current_line][*cursor_pos] = '\0';

    *cursor_pos = 0;
    (*line_count)++;

    moveCursorDown(lines, cursor_pos, current_line, *line_count);
}

void insertLine(char lines[MAX_LINES][MAX_LINE_LENGTH], int index, int line_count) {
    memmove(&lines[index + 1], &lines[index], (line_count - index) * sizeof(lines[0]));
    memset(lines[index], 0, sizeof(lines[0]));
}

void removeLine(char lines[MAX_LINES][MAX_LINE_LENGTH], int index, int *line_count) {
    if (index < 0 || index >= *line_count) {
        return;
    }

    if (index < *line_count - 1) {
        memmove(lines + index, lines + index + 1, (*line_count - index - 1) * sizeof(lines[0]));
    }

    memset(lines[*line_count - 1], 0, sizeof(lines[0]));
    (*line_count)--;
}

void handleBackspace(char lines[MAX_LINES][MAX_LINE_LENGTH], int *cursor_pos, int *current_line, int *line_count) {
    if (*cursor_pos > 0) {
        int len = strlen(lines[*current_line]);
        memmove(lines[*current_line] + *cursor_pos - 1, lines[*current_line] + *cursor_pos, len - *cursor_pos + 1);
        (*cursor_pos)--;
    } else if (*current_line > 0) {
        int prev_len = strlen(lines[*current_line - 1]);
        int cur_len = strlen(lines[*current_line]);

        if (prev_len + cur_len < MAX_LINE_LENGTH) {
            strcat(lines[*current_line - 1], lines[*current_line]);
            *cursor_pos = prev_len;
        }
        removeLine(lines, *current_line, line_count);
        (*current_line)--;
    }
}

void moveCursorLeft(char lines[MAX_LINES][MAX_LINE_LENGTH], int *cursor_pos, int *current_line) {
    if (*cursor_pos > 0) {
        (*cursor_pos)--;
    } else if (*current_line > 0) {
        (*current_line)--;
        *cursor_pos = strlen(lines[*current_line]);
    }
}

void moveCursorRight(char lines[MAX_LINES][MAX_LINE_LENGTH], int *cursor_pos, int *current_line) {
    int len = strlen(lines[*current_line]);
    if (*cursor_pos < len) {
        (*cursor_pos)++;
    } else if (*current_line < MAX_LINES - 1 && lines[*current_line + 1][0] != '\0') {
        (*current_line)++;
        *cursor_pos = 0;
    }
}


void moveCursorUp(char lines[MAX_LINES][MAX_LINE_LENGTH], int *cursor_pos, int *current_line) {
    if (*current_line <= 0) {
        *current_line = 0;
        return;
    }

    (*current_line)--;
    int len = strlen(lines[*current_line]);
    if (*cursor_pos > len) {
        *cursor_pos = len;
    }
}

void moveCursorDown(char lines[MAX_LINES][MAX_LINE_LENGTH], int *cursor_pos, int *current_line, int line_count) {
    if (*current_line >= line_count - 1) {
        *current_line = line_count - 1;
        return;
    }

    (*current_line)++;
    int len = strlen(lines[*current_line]);
    if (*cursor_pos > len) {
        *cursor_pos = len;
    }
}

void optLeft(char lines[MAX_LINES][MAX_LINE_LENGTH], int *cursor_pos, int current_line) {
    int i = *cursor_pos - 1;

    while (i >= 0 && lines[current_line][i] == ' ') {
        i--;
    }

    while (i >= 0 && lines[current_line][i] != ' ') {
        i--;
    }

    *cursor_pos = i + 1;
}

void optRight(char lines[MAX_LINES][MAX_LINE_LENGTH], int *cursor_pos, int current_line) {
    int len = strlen(lines[current_line]);
    int i = *cursor_pos;

    while (i < len && lines[current_line][i] == ' ') {
        i++;
    }

    while (i < len && lines[current_line][i] != ' ') {
        i++;
    }

    *cursor_pos = i;
}

void cmdRight(char lines[MAX_LINES][MAX_LINE_LENGTH], int *cursor_pos, int current_line) {
    int len = strlen(lines[current_line]);
    *cursor_pos = len;
}

void cmdLeft(int *cursor_pos) {
    *cursor_pos = 0;
}

void OpenDialog(char lines[MAX_LINES][MAX_LINE_LENGTH], int *line_count, int *current_line, int *cursor_pos) {
    
    for (int i = 0; i < MAX_LINES; i++) {
        lines[i][0] = '\0';
    }

    const char *openPath = tinyfd_openFileDialog(
            "Open Text File",
            "",
            1,
            (const char *[]) {"*.txt"},
            "Text files",
            0
    );

    if (openPath) {
        FILE *file = fopen(openPath, "r");
        if (file == NULL) {
            printf("Error: Could not open file for reading.\n");
            return;
        }
        *current_line = 0;
        *cursor_pos = 0;
        int i = 0;
        while (i < MAX_LINES && fgets(lines[i], MAX_LINE_LENGTH, file)) {
            lines[i][strcspn(lines[i], "\n")] = '\0';
            i++;
        }

        for (i = 0; i < MAX_LINES; i++) {
            if (lines[i][0] != '\0') {
                *line_count = i + 1;
            }
        }

        fclose(file);
    } else {
        printf("Open dialog was canceled.\n");
    }
}


void SaveDialog(char lines[MAX_LINES][MAX_LINE_LENGTH]) {
    const char *savePath = tinyfd_saveFileDialog(
            "Save Text File",
            "untitled.txt",
            1,
            (const char*[]){"*.txt"},
            "Text files");

    if (savePath) {
        FILE *file = fopen(savePath, "w");
        if (file == NULL) {
            printf("Error: Could not open file for writing.\n");
            return;
        }

        for (int i = 0; i < MAX_LINES; ++i) {
            fprintf(file, "%s\n", lines[i]);
        }

        fclose(file);
    } else {
        printf("Save dialog was canceled.\n");
    }
}